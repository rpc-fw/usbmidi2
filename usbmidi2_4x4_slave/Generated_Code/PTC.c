/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PTC.c
**     Project     : usbmidi2_4x4_slave
**     Processor   : MKL26Z64VFM4
**     Component   : Init_GPIO
**     Version     : Component 01.006, Driver 01.06, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-10, 17:29, # CodeGen: 2
**     Abstract    :
**          This file implements the GPIO (PTC) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : PTC
**          Device                                         : PTC
**          Settings                                       : 
**            Pin 1                                        : Do not initialize
**            Pin 2                                        : Do not initialize
**            Pin 3                                        : Do not initialize
**            Pin 4                                        : Do not initialize
**            Pin 5                                        : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : Enabled
**              Pull selection                             : Pull Down
**              Slew rate                                  : No initialization
**              Interrupt/DMA request                      : No initialization
**            Pin 6                                        : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : Enabled
**              Pull selection                             : Pull Down
**              Slew rate                                  : No initialization
**              Interrupt/DMA request                      : No initialization
**            Pin 7                                        : Initialize
**              Pin direction                              : Input
**              Output value                               : No initialization
**              Pull resistor                              : Enabled
**              Pull selection                             : Pull Down
**              Slew rate                                  : No initialization
**              Interrupt/DMA request                      : No initialization
**          Pin selection/routing                          : 
**            Pin 1                                        : Disabled
**            Pin 2                                        : Disabled
**            Pin 3                                        : Disabled
**            Pin 4                                        : Disabled
**            Pin 5                                        : Enabled
**              Pin                                        : PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT
**              Pin signal                                 : 
**            Pin 6                                        : Enabled
**              Pin                                        : CMP0_IN0/PTC6/LLWU_P10/SPI0_MOSI/EXTRG_IN/I2S0_RX_BCLK/SPI0_MISO/I2S0_MCLK
**              Pin signal                                 : 
**            Pin 7                                        : Enabled
**              Pin                                        : CMP0_IN1/PTC7/SPI0_MISO/audioUSB_SOF_OUT/I2S0_RX_FS/SPI0_MOSI
**              Pin signal                                 : 
**          Interrupts                                     : 
**            Port interrupt                               : Enabled
**              Interrupt                                  : INT_PORTC_PORTD
**              Interrupt request                          : Disabled
**              Interrupt priority                         : 0 (Highest)
**              ISR Name                                   : 
**          Initialization                                 : 
**            Call Init method                             : yes
**     Contents    :
**         Init - void PTC_Init(void);
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
** @file PTC.c
** @version 01.06
** @brief
**          This file implements the GPIO (PTC) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup PTC_module PTC module documentation
**  @{
*/         

/* MODULE PTC. */

#include "PTC.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  PTC_Init (component Init_GPIO)
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
void PTC_Init(void)
{
  /* GPIOC_PDDR: PDD&=~0xE0 */
  GPIOC_PDDR &= (uint32_t)~(uint32_t)(GPIO_PDDR_PDD(0xE0));
}


/* END PTC. */
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
