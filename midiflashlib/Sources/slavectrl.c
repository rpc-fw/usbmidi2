#include "WAIT1.h"
#include "COREUART.h"
#include "UART0_PDD.h"
#include "MIDIUART1.h"
#include "LED1.h"
#include "PTD.h"
#include "SlaveReset.h"
#include "KIN1.h"

int slaveresetticks = 0;

void SlaveResetMode(int mode)
{
	// PTD7: UART0_TX
	// Slave expects boot mode selection
	if (mode) {
		// normal boot, set pin
		GPIO_PDD_SetPortDataOutputMask(PTD_BASE_PTR, (1 << 7));
	}
	else {
		// stop in boot loader, clear pin
		GPIO_PDD_ClearPortDataOutputMask(PTD_BASE_PTR, (1 << 7));
	}
}

int IsSlaveReady()
{
	// PTD7: UART0_RX
	// Expect slave to pull pin high when ready.
	uint32_t result = GPIO_PDD_GetPortDataInput(PTD_BASE_PTR);
	return (result & (1 << 6)) != 0;
}

byte ReadCoreUartByte()
{
	LED1_Off();
	byte databyte = 0;
	while (COREUART_RecvChar(&databyte) == ERR_RXEMPTY);
	LED1_On();
	return databyte;
}

void SlaveHello()
{
	byte b = 0;
	do {
		COREUART_ClearRxBuf();

		COREUART_SendChar(0x5A);

		b = ReadCoreUartByte();
	} while (b != 0x5A);
}

void SlaveReset()
{
	SlaveReset_ClrVal(SlaveReset_DeviceData);
	WAIT1_Waitms(1);
	SlaveReset_SetVal(SlaveReset_DeviceData);
	LED1_Off();

	slaveresetticks = 0;
	while (!IsSlaveReady()) {
		slaveresetticks++;
	}
}

void SlaveToBootloader()
{
	LED1_On();
	COREUART_ClearTxBuf();
	COREUART_Disable();
	UART0_C2 = 0;
	UART0_C5 &= ~(1 << 5); /* RDMAE=0 Disable DMA */
	//	  UART0_PDD_EnableTransmitter(UART0_BASE_PTR, PDD_DISABLE); /* Disable transmitter. */
	//UART0_PDD_EnableReceiver(UART0_BASE_PTR, PDD_DISABLE); /* Disable receiver. */
	COREUART_Init();
	/* PORTD_PCR6: ISF=0,PE=1,PS=0 */
	PORTD_PCR6 = (uint32_t)((PORTD_PCR6 & (uint32_t)~(uint32_t)(
			PORT_PCR_ISF_MASK |
			PORT_PCR_PS_MASK
	)) | (uint32_t)(
			PORT_PCR_PE_MASK
	));
	/* PORTD_PCR7: ISF=0,PE=1,PS=0 */
	PORTD_PCR7 = (uint32_t)((PORTD_PCR7 & (uint32_t)~(uint32_t)(
			PORT_PCR_ISF_MASK |
			PORT_PCR_PS_MASK
	)) | (uint32_t)(
			PORT_PCR_PE_MASK
	));
	PTD_Init();
	SlaveResetMode(0);
	SlaveReset();
	COREUART_Enable();
	SlaveHello();
	LED1_On();
}

void SlaveToProgram()
{
	LED1_On();
	COREUART_Init();
	COREUART_Enable();

	SlaveReset();
}

void SlaveRequest(void* req, size_t reqsize, void* res, size_t ressize)
{
	int i;
	COREUART_ClearRxBuf();

	for (i = 0; i < reqsize; i++) {
		while (COREUART_SendChar(((uint8_t*)req)[i]) != ERR_OK) {}
	}

	if (ressize > 0) {
		for (i = 0; i < ressize; i++) {
			while (COREUART_RecvChar(&((uint8_t*)res)[i]) == ERR_RXEMPTY) {}
		}
	}
}

uint32_t GetSlaveBlockSize()
{
	uint32_t blocksize = 0;

	byte req[] = { 0x1, 0x1 };
	SlaveRequest(req, sizeof(req), &blocksize, sizeof(blocksize));

	return blocksize;
}

int WriteSlaveBlock(uint32_t address, void* data, int datasize)
{
	byte req[] = { 0x3, 0, 0, 0, 0, 0, 0, 0 };
	*(uint16_t*)&req[2] = datasize;
	*(uint32_t*)&req[4] = address;
	SlaveRequest(&req, sizeof(req), NULL, 0);

	byte ack = ReadCoreUartByte();
	if (ack != 1) {
		// parameter error
		return ack;
	}

	for (int dataindex = 0; dataindex < datasize; dataindex += 16) {
		int singlesize = datasize - dataindex;
		if (singlesize > 16) singlesize = 16;
		SlaveRequest(&((uint8_t*)data)[dataindex], singlesize, NULL, 0);

		ack = ReadCoreUartByte();
		if (ack != 1) {
			// transfer failed or parameter error
			return ack;
		}
	}

	/* write started, wait for completion */

	ack = ReadCoreUartByte();
	if (ack != 1) {
		// write failed
		return ack;
	}

	return 0;
}
