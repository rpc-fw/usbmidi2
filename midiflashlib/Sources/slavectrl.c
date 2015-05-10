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
	byte databyte = 0;
	while (COREUART_RecvChar(&databyte) == ERR_RXEMPTY);
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

void SlaveToBootloader()
{
	LED1_Off();
	COREUART_Disable();
	COREUART_ClearTxBuf();
	UART0_C2_REG((volatile UART0_MemMapPtr)UART0_BASE_PTR) = 0;
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
	SlaveReset_ClrVal(SlaveReset_DeviceData);
	WAIT1_Waitms(1);
	SlaveReset_SetVal(SlaveReset_DeviceData);
	WAIT1_Waitms(1);
	slaveresetticks = 0;
	while (!IsSlaveReady()) {
		slaveresetticks++;
	}

	COREUART_Enable();
	SlaveHello();
	LED1_On();
}

void SlaveToProgram()
{
	LED1_On();
	COREUART_Init();
	COREUART_Enable();

	SlaveReset_ClrVal(SlaveReset_DeviceData);
	WAIT1_Waitms(1);
	SlaveReset_SetVal(SlaveReset_DeviceData);
	LED1_Off();

	slaveresetticks = 0;
	while (!IsSlaveReady()) {
		slaveresetticks++;
	}
}

void SlaveRequest(void* req, size_t reqsize, void* res, size_t ressize)
{
	COREUART_ClearRxBuf();

	word xferred = 0;
	while (COREUART_SendBlock(req, reqsize, &xferred) == ERR_TXFULL) {
		req += xferred;
		reqsize -= xferred;
	}

	if (ressize > 0) {
		xferred = 0;
		while (COREUART_RecvBlock(res, ressize, &xferred) == ERR_RXEMPTY) {
			res += xferred;
			ressize -= xferred;
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

	LED1_Off();
	byte ack = ReadCoreUartByte();
	LED1_On();
	if (ack != 1) {
		// parameter error
		return ack;
	}

	for (int dataindex = 0; dataindex < datasize; dataindex += 16) {
		int singlesize = datasize - dataindex;
		if (singlesize > 16) singlesize = 16;
		SlaveRequest(&data[dataindex], singlesize, NULL, 0);

		LED1_Off();
		ack = ReadCoreUartByte();
		LED1_On();
		if (ack != 1) {
			// transfer failed or parameter error
			return ack;
		}
	}

	/* write started, wait for completion */

	LED1_Off();
	ack = ReadCoreUartByte();
	LED1_On();
	if (ack != 1) {
		// write failed
		return ack;
	}

	return 0;
}
