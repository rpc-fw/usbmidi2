/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : I2S0.h
**     Project     : usbmidi2_4x4_master
**     Processor   : MKL26Z64VFM4
**     Component   : Init_I2S
**     Version     : Component 01.002, Driver 01.02, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-10, 17:25, # CodeGen: 0
**     Abstract    :
**          This file implements the I2S (I2S0) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : I2S0
**          Device                                         : I2S0
**          Settings                                       : 
**            Clock gate                                   : Enabled
**            Clock setting                                : 
**              Clock source                               : OSC0 clock
**              Clock ratio                                : 
**              MCLK clock source/direction                : Internal
**              MCLK out frequency                         : 12 MHz
**              Transmit clock setting                     : 
**                Clock source                             : Bus clock
**                Bit clock divider                        : divide by 4
**                Bit clock source/direction               : Internal
**                Bit clock frequency                      : 6 MHz
**                Synchronous mode                         : Asynchronous
**                Bit clock swap                           : no
**                Bit clock input                          : internal
**              Receive clock setting                      : 
**                Clock source                             : Bus clock
**                Bit clock divider                        : divide by 4
**                Bit clock source/direction               : Internal
**                Bit clock frequency                      : 6 MHz
**                Synchronous mode                         : Asynchronous
**                Bit clock swap                           : no
**                Bit clock input                          : internal
**            Transmit configuration                       : 
**              Frame sync source/direction                : Internal
**              Frame sync active state                    : High
**              Frame sync initiated                       : first bit of data
**              Frame sync width [bits]                    : 1
**              Frame size [words]                         : 1
**              Shift direction                            : LSB first
**              Data clocked out on                        : rising edge
**              First bit shifted index                    : 0
**              Word 0 width [bits]                        : 8
**              Word N width [bits]                        : 8
**              Word flag configuration [words]            : 1
**              Data channel                               : Disabled
**              Time slot mask                             : 0
**              Stop in wait mode                          : yes
**              Stop in debug mode                         : yes
**            Receive configuration                        : 
**              Frame sync source/direction                : Internal
**              Frame sync active state                    : High
**              Frame sync initiated                       : first_bit_of_data
**              Frame sync width [bits]                    : 1
**              Frame size [words]                         : 1
**              Shift direction                            : LSB first
**              Data clocked in on                         : rising edge
**              First bit shifted index                    : 0
**              Word 0 width [bits]                        : 8
**              Word N width [bits]                        : 8
**              Word flag configuration [words]            : 1
**              Data channel                               : Disabled
**              Time slot mask                             : 0
**              Stop in wait mode                          : yes
**              Stop in debug mode                         : yes
**          Pins                                           : 
**            Transmit pins                                : 
**              Serial transmit data pin                   : Disabled
**              Serial bit clock pin                       : Disabled
**              Serial frame sync pin                      : Disabled
**            Receive pins                                 : 
**              Serial receive data pin                    : Disabled
**              Serial bit clock pin                       : Disabled
**              Serial frame sync pin                      : Disabled
**            Serial master clock pin                      : Enabled
**              Pin                                        : PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/TPM0_CH3/I2S0_MCLK
**              Pin signal                                 : 
**          Interrupts/DMA                                 : 
**            Transmit                                     : 
**              Tx interrupt                               : INT_I2S0
**              Interrupt request                          : Disabled
**              Interrupt priority                         : 0 (Highest)
**              Tx ISR Name                                : 
**              Tx Word start interrupt                    : Disabled
**              Tx sync error interrupt                    : Disabled
**              Tx FIFO error interrupt                    : Disabled
**              Tx FIFO warning interrupt                  : Disabled
**              Tx FIFO warning DMA                        : Disabled
**            Receive                                      : 
**              Rx interrupt                               : INT_I2S0
**              Interrupt request                          : Disabled
**              Interrupt priority                         : 0 (Highest)
**              Rx ISR Name                                : 
**              Rx Word start interrupt                    : Disabled
**              Rx sync error interrupt                    : Disabled
**              Rx FIFO error interrupt                    : Disabled
**              Rx FIFO warning interrupt                  : Disabled
**              Rx FIFO warning DMA                        : Disabled
**          Initialization                                 : 
**            Transmitter                                  : Disabled
**            Transmitter bit clock                        : Disabled
**            Receiver                                     : Disabled
**            Receiver bit clock                           : Disabled
**            Call Init method                             : yes
**     Contents    :
**         Init - void I2S0_Init(void);
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
** @file I2S0.h
** @version 01.02
** @brief
**          This file implements the I2S (I2S0) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup I2S0_module I2S0 module documentation
**  @{
*/         

#ifndef I2S0_H_
#define I2S0_H_

/* MODULE I2S0. */

/* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

/* Peripheral base address parameter */
#define I2S0_DEVICE I2S0_BASE_PTR


/*
** ===================================================================
**     Method      :  I2S0_Init (component Init_I2S)
**     Description :
**         This method initializes registers of the I2S module
**         according to the Peripheral Initialization settings.
**         Call this method in user code to initialize the module. By
**         default, the method is called by PE automatically; see "Call
**         Init method" property of the component for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2S0_Init(void);


/* END I2S0. */
#endif /* #ifndef __I2S0_H_ */
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
