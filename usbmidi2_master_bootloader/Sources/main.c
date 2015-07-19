/* ###################################################################
**     Filename    : main.c
**     Project     : kl26test
**     Processor   : MKL26Z64VFM4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-01-31, 10:37, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "PTC.h"
#include "MIDIUART1.h"
#include "ASerialLdd1.h"
#include "COREUART.h"
#include "ASerialLdd2.h"
#include "PTE.h"
#include "BBA1.h"
#include "LED1.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "WAIT1.h"
#include "PTD.h"
#include "SlaveReset.h"
#include "IFsh1.h"
#include "IntFlashLdd1.h"
#include "KIN1.h"
#include "UTIL1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "UART0_PDD.h"
#include "checksum.h"

#include "midiflash.h"
#include "slavectrl.h"

#define APP_FLASH_VECTOR_START 0x2000

void MidiLoader();

struct buildid_t {
	uint32_t id;
	uint32_t payload_hash;
	void* payload_start;
	uint32_t payload_size;
};

uint32_t slaveblocksize = 0;
struct buildid_t slaveid;

byte slaveidreq[] = { 0x1, 0x2 };
void IdentifySlave()
{
	slaveblocksize = GetSlaveBlockSize();
	SlaveRequest(slaveidreq, sizeof(slaveidreq), &slaveid, sizeof(slaveid));
}

// Application info is stored after the interrupt vectors
volatile struct buildid_t* masterid = (struct buildid_t*)(APP_FLASH_VECTOR_START + 0x400);

__attribute__((__aligned__(IFsh1_AREA_SECTOR_SIZE)))
byte flashblock[IFsh1_AREA_SECTOR_SIZE];

const int FAIL_HANG = 0;
const int FAIL_WARN = 1;

void FlashFailed(int warning)
{
	int infinite = 1;
	int repeats = 0;
	if (warning) {
		infinite = 0;
		repeats = 3;
	}

	while(infinite || repeats > 0) {
		LED1_On();
		WAIT1_Waitms(200);
		LED1_Off();
		WAIT1_Waitms(200);

		if (!infinite) {
			repeats--;
		}
	}
}

typedef void (*slave_16_func) (uint32_t index, uint32_t size);
typedef void (*slave_block_func) (uint32_t address, uint32_t size);

void ReadSlaveProgram(struct buildid_t slavebuild, slave_16_func slave_16, slave_block_func slave_block)
{
	uint16_t checksum = 0;

	for (uint32_t blockindex = 0; blockindex < slavebuild.payload_size; blockindex += sizeof(flashblock))
	{
		//memset(flashblock, 0xFF, sizeof(flashblock));

		int readsize = slavebuild.payload_size - blockindex;
		if (readsize > sizeof(flashblock)) {
			readsize = sizeof(flashblock);
		}

		for (uint32_t k = 0; k < readsize; k += 16) {
			int singlesize = readsize - k;
			if (singlesize > 16) {
				singlesize = 16;
			}

			byte req[] = { 0x2, 0, 0, 0, 0, 0, 0, 0 };
			req[1] = singlesize;
			*(uint32_t*)&req[4] = (uint32_t)slavebuild.payload_start + blockindex + k;
			SlaveRequest(&req, sizeof(req), &flashblock[k], singlesize);

			if (slave_16) {
				slave_16(k, singlesize);
			}
		}

		if (slave_block) {
			slave_block(APP_FLASH_VECTOR_START + blockindex, IFsh1_AREA_SECTOR_SIZE);
		}
	}
}

uint16_t verify_checksum;

void verify_slave_16(uint32_t index, uint32_t size)
{
	verify_checksum = fletcher_checksum_running(verify_checksum, &flashblock[index], size);
}

int VerifySlaveProgram(struct buildid_t slavebuild)
{
	verify_checksum = 0;

	ReadSlaveProgram(slavebuild, verify_slave_16, NULL);

	verify_checksum = fletcher_checksum_uint32(verify_checksum, slavebuild.payload_size);

	uint8_t checkbytes[2];
	checkbytes[0] = slavebuild.payload_hash & 0xFF;
	checkbytes[1] = (slavebuild.payload_hash >> 8) & 0xFF;
	verify_checksum = fletcher_checksum_running(verify_checksum, checkbytes, 2);

	return (verify_checksum == 0);
}

void load_slave_block(uint32_t address, uint32_t size)
{
	if (IFsh1_SetBlockFlash(flashblock, address, size) != ERR_OK) {
		FlashFailed(FAIL_HANG);
	}
}

void LoadSlaveProgram(struct buildid_t slavebuild)
{
	ReadSlaveProgram(slavebuild, NULL, load_slave_block);
}

#define APP_FLASH_VECTOR_START 0x2000

void WarmReset(unsigned long userSP, unsigned long userStartup);

void BootToApplication()
{
	/* Read application initial stack pointer */
	uint32_t app_stackptr = ((uint32_t*)APP_FLASH_VECTOR_START)[0];

	/* Read application reset vector */
	uint32_t app_reset = ((uint32_t*)APP_FLASH_VECTOR_START)[1];

	/* If reset vector is valid, jump to it */
	if (app_reset != 0xFFFFFFFF) {

		// Avoid crashing in interrupts after program start
		EnterCritical();

		/* Set interrupt vector table to application */
		SCB_VTOR = APP_FLASH_VECTOR_START;

		WarmReset(app_stackptr, app_reset);
	}
	else {
		MidiLoader();
	}
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */

  SlaveToBootloader();
  IdentifySlave();

  // determine whether to load firmware from slave or not
  if (slaveid.id != masterid->id || masterid->id == 0xFFFFFFFF) {
	  if (slaveid.id == 0xFFFFFFFF || !VerifySlaveProgram(slaveid)) {
		  // Slave program is invalid, don't reprogram master
		  FlashFailed(FAIL_WARN);
		  MidiLoader();
		  for(;;) {} // just in case
	  }

	  LoadSlaveProgram(slaveid);

	  if (slaveid.id != masterid->id) {
		  // Lock up, don't keep repeating flashing!
		  FlashFailed(FAIL_WARN);
		  MidiLoader();
		  for(;;) {} // just in case
	  }

	  KIN1_SoftwareReset();
  }

  SlaveToProgram();
  BootToApplication();

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
