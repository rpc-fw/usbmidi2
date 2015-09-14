#include "KIN1.h"

void SoftwareReset()
{
	KIN1_SoftwareReset();
}

void WarmReset(unsigned long userSP, unsigned long userStartup)
{
	// set up stack pointer
	__asm("msr msp, r0");
	__asm("msr psp, r0");

	// Jump to PC (r1)
	__asm("mov pc, r1");
}
