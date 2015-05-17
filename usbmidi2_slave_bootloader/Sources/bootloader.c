#include <string.h>

#include "PTA.h"
#include "COREUART.h"
#include "GPIO_PDD.h"
#include "IFsh1.h"

#define APP_FLASH_VECTOR_START 0x4000

byte flash_block[IFsh1_AREA_SECTOR_SIZE];

void WarmReset(unsigned long userSP, unsigned long userStartup)
{
	// set up stack pointer
	__asm("msr msp, r0");
	__asm("msr psp, r0");

	// Jump to PC (r1)
	__asm("mov pc, r1");
}

void BootToApplication()
{
	/* Read application initial stack pointer */
	uint32_t app_stackptr = ((uint32_t*)APP_FLASH_VECTOR_START)[0];

	/* Read application reset vector */
	uint32_t app_reset = ((uint32_t*)APP_FLASH_VECTOR_START)[1];

	/* If reset vector is valid, jump to it */
	if (app_reset != 0xFFFFFFFF) {

		/* Set interrupt vector table to application */
		SCB_VTOR = APP_FLASH_VECTOR_START;

		WarmReset(app_stackptr, app_reset);
	}
}

byte ReadCoreUartByte()
{
	byte databyte = 0;
	while (COREUART_RecvChar(&databyte) == ERR_RXEMPTY);
	return databyte;
}

int ReadCoreUart(void* req, uint32_t reqsize)
{
	word xferred = 0;
	byte r = 0;
	while ((r = COREUART_RecvBlock(req, reqsize, &xferred)) == ERR_RXEMPTY) {
		req += xferred;
		reqsize -= xferred;
	}
	if (r != ERR_OK) {
		return 1;
	}

	return 0;
}

void SlaveReply(void* res, uint32_t ressize)
{
	word xferred = 0;
	while (COREUART_SendBlock(res, ressize, &xferred) == ERR_TXFULL) {
		res += xferred;
		ressize -= xferred;
	}
}

struct buildid_t {
	uint32_t id;
	uint32_t payload_hash;
	void* payload_start;
	uint32_t payload_size;
};

// Application info is stored after interrupt table
struct buildid_t* slave_buildid = (struct buildid_t*)(APP_FLASH_VECTOR_START + 0x400);

void HandleStateRead()
{
	byte substate = ReadCoreUartByte();

	if (substate == 0x1) {
		uint32_t blocksize = IFsh1_AREA_SECTOR_SIZE;
		SlaveReply(&blocksize, sizeof(blocksize));
	}
	else if (substate = 0x2) {
		SlaveReply(slave_buildid, sizeof(*slave_buildid));
	}
}

void HandleFlashRead()
{
	byte readsize = ReadCoreUartByte();

	/* two padding bytes */
	(void) ReadCoreUartByte();
	(void) ReadCoreUartByte();

	uint32_t address;
	ReadCoreUart(&address, sizeof(address));

	SlaveReply((void*)address, readsize);
}

__attribute__((__aligned__(1024))) byte flashblock[IFsh1_AREA_SECTOR_SIZE];

void HandleFlashWrite()
{
	/* padding byte */
	(void) ReadCoreUartByte();

	uint16_t writesize;
	ReadCoreUart(&writesize, sizeof(writesize));

	uint32_t address;
	ReadCoreUart(&address, sizeof(address));

	byte ack = 1; // starting to flash

	if (writesize > sizeof(flashblock)) {
		ack = 3; // size error
		SlaveReply(&ack, sizeof(ack));
		// throw away input
		for (int i = 0; i < writesize; i++) {
			(void) ReadCoreUartByte();
		}
	}
	else {
		memset(flashblock, 0xFF, sizeof(flashblock));

		for (int index = 0; index < writesize; index += 16) {
			int singlesize = writesize - index;
			if (singlesize > 16) singlesize = 16;
			SlaveReply(&ack, sizeof(ack));
			if (ReadCoreUart(&flashblock[index], singlesize)) {
				ack = 4; // data error
				break;
			}
		}
	}

	if (address < APP_FLASH_VECTOR_START || address + writesize < APP_FLASH_VECTOR_START) {
		ack = 2; // address error
	}

	SlaveReply(&ack, sizeof(ack));

	if (ack != 1) {
		goto flashend;
	}

	ack = 1; // ok

	if (IFsh1_SetBlockFlash(flashblock, address, IFsh1_AREA_SECTOR_SIZE) != ERR_OK) {
		ack = 3; // write failed
		goto flashend;
	}
	while (IFsh1_Busy(0));
flashend:
	SlaveReply(&ack, sizeof(ack));
}

void bootloader_main()
{
	int bootmode = (GPIO_PDD_GetPortDataInput(PTA_BASE_PTR) & (1 << 1)) != 0;

	if (bootmode == 1) {
		BootToApplication();

		// Fall through...
	}

	// Boot to bootloader

	COREUART_Enable();

	while (1) {
		byte databyte;
		byte result = COREUART_RecvChar(&databyte);
		if (result == ERR_OK) {
			if (databyte == 0x5A) {
				COREUART_SendChar(databyte);
			}

			if (databyte == 0x1) {
				HandleStateRead();
			}

			if (databyte == 0x2) {
				HandleFlashRead();
			}

			if (databyte == 0x3) {
				HandleFlashWrite();
			}

			//WAIT1_Waitms(1);
			//BootToApplication();
		}
	}
}

