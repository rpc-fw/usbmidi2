/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PTE.c
**     Project     : usbmidi2_master_bootloader
**     Processor   : MKL26Z64VFM4
**     Component   : Init_GPIO
**     Version     : Component 01.006, Driver 01.06, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-05-17, 00:02, # CodeGen: 60
**     Abstract    :
**          This file implements the GPIO (PTE) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : PTE
**          Device                                         : PTE
**          Settings                                       : 
**            Pin 0                                        : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : Enabled
**              Pull selection                             : Pull Up
**              Slew rate                                  : No initialization
**            Pin 30                                       : Do not initialize
**          Pin selection/routing                          : 
**            Pin 0                                        : Enabled
**              Pin                                        : PTE0/SPI1_MISO/UART1_TX/RTC_CLKOUT/CMP0_OUT/I2C1_SDA
**              Pin signal                                 : 
**            Pin 30                                       : Disabled
**          Initialization                                 : 
**            Call Init method                             : yes
**     Contents    :
**         Init - void PTE_Init(void);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PTE.c
** @version 01.06
** @brief
**          This file implements the GPIO (PTE) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup PTE_module PTE module documentation
**  @{
*/         

/* MODULE PTE. */

#include "PTE.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  PTE_Init (component Init_GPIO)
**     Description :
**         This method initializes registers of the GPIO module
**         according to the Peripheral Initialization settings.
**         Call this method in user code to initialize the module. By
**         default, the method is called by PE automatically; see "Call
**         Init method" property of the component for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PTE_Init(void)
{
  /* GPIOE_PDDR: PDD&=~1 */
  GPIOE_PDDR &= (uint32_t)~(uint32_t)(GPIO_PDDR_PDD(0x01));
}


/* END PTE. */
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
