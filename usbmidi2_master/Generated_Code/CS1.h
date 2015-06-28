/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CS1.h
**     Project     : usbmidi2_master
**     Processor   : MKL26Z64VFM4
**     Component   : CriticalSection
**     Version     : Component 01.006, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-06-28, 14:10, # CodeGen: 52
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         CriticalVariable - void CS1_CriticalVariable(void);
**         EnterCritical    - void CS1_EnterCritical(void);
**         ExitCritical     - void CS1_ExitCritical(void);
**
**     License   : Open Source (LGPL)
**     Copyright : Erich Styger, 2014, all rights reserved.
**     Web       : www.mcuoneclipse.com
**     This an open source software implementing a driver using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file CS1.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup CS1_module CS1 module documentation
**  @{
*/         

#ifndef __CS1_H
#define __CS1_H

/* MODULE CS1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */

#include "Cpu.h"


/* workaround macros for wrong EnterCritical()/ExitCritical() in the low level drivers. Will be removed once PEx is fixed */
#define CS1_CriticalVariableDrv() \
  CS1_CriticalVariable()
#define CS1_EnterCriticalDrv() \
  CS1_EnterCritical()
#define CS1_ExitCriticalDrv() \
  CS1_ExitCritical()

#define CS1_CriticalVariable() \
  uint8_t cpuSR; /* variable to store current status */

/*
** ===================================================================
**     Method      :  CS1_CriticalVariable (component CriticalSection)
**     Description :
**         Defines a variable if necessary. This is a macro.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define CS1_EnterCritical() \
  do {                                  \
    __asm (                             \
    "mrs   r0, PRIMASK     \n\t"        \
    "cpsid i               \n\t"        \
    "strb r0, %[output]   \n\t"         \
    : [output] "=m" (cpuSR) :: "r0");   \
    __asm ("" ::: "memory");            \
  } while(0)

/*
** ===================================================================
**     Method      :  CS1_EnterCritical (component CriticalSection)
**     Description :
**         Enters a critical section
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define CS1_ExitCritical() \
  do{                                  \
    __asm (                            \
    "ldrb r0, %[input]    \n\t"        \
    "msr PRIMASK,r0        \n\t"       \
    ::[input] "m" (cpuSR) : "r0");     \
  } while(0)

/*
** ===================================================================
**     Method      :  CS1_ExitCritical (component CriticalSection)
**     Description :
**         Exits a critical section
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END CS1. */

#endif
/* ifndef __CS1_H */
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
