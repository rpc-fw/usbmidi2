/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : SlaveReset.h
**     Project     : usbmidi2_4x4_master_bootloader
**     Processor   : MKL26Z64VFM4
**     Component   : BitIO_LDD
**     Version     : Component 01.033, Driver 01.03, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-07, 20:11, # CodeGen: 0
**     Abstract    :
**         The HAL BitIO component provides a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL BitIO API are simpler and more
**         portable to various microprocessors.
**     Settings    :
**          Component name                                 : SlaveReset
**          Pin for I/O                                    : ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/TPM1_CH1
**          Pin signal                                     : 
**          Direction                                      : Input/Output
**          Initialization                                 : 
**            Init. direction                              : Output
**            Init. value                                  : 0
**            Auto initialization                          : yes
**          Safe mode                                      : no
**     Contents    :
**         Init     - LDD_TDeviceData* SlaveReset_Init(LDD_TUserData *UserDataPtr);
**         SetDir   - void SlaveReset_SetDir(LDD_TDeviceData *DeviceDataPtr, bool Dir);
**         SetInput - void SlaveReset_SetInput(LDD_TDeviceData *DeviceDataPtr);
**         ClrVal   - void SlaveReset_ClrVal(LDD_TDeviceData *DeviceDataPtr);
**         SetVal   - void SlaveReset_SetVal(LDD_TDeviceData *DeviceDataPtr);
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
** @file SlaveReset.h
** @version 01.03
** @brief
**         The HAL BitIO component provides a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL BitIO API are simpler and more
**         portable to various microprocessors.
*/         
/*!
**  @addtogroup SlaveReset_module SlaveReset module documentation
**  @{
*/         

#ifndef __SlaveReset_H
#define __SlaveReset_H

/* MODULE SlaveReset. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "IO_Map.h"
#include "GPIO_PDD.h"

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif 



/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define SlaveReset_PRPH_BASE_ADDRESS  0x400FF040U
  
/*! Device data structure pointer used when auto initialization property is enabled. This constant can be passed as a first parameter to all component's methods. */
#define SlaveReset_DeviceData  ((LDD_TDeviceData *)PE_LDD_GetDeviceStructure(PE_LDD_COMPONENT_SlaveReset_ID))

/* Methods configuration constants - generated for all enabled component's methods */
#define SlaveReset_Init_METHOD_ENABLED /*!< Init method of the component SlaveReset is enabled (generated) */
#define SlaveReset_SetDir_METHOD_ENABLED /*!< SetDir method of the component SlaveReset is enabled (generated) */
#define SlaveReset_SetInput_METHOD_ENABLED /*!< SetInput method of the component SlaveReset is enabled (generated) */
#define SlaveReset_ClrVal_METHOD_ENABLED /*!< ClrVal method of the component SlaveReset is enabled (generated) */
#define SlaveReset_SetVal_METHOD_ENABLED /*!< SetVal method of the component SlaveReset is enabled (generated) */

/* Definition of implementation constants */
#define SlaveReset_MODULE_BASE_ADDRESS FPTB_BASE_PTR /*!< Name of macro used as the base address */
#define SlaveReset_PORTCONTROL_BASE_ADDRESS PORTB_BASE_PTR /*!< Name of macro used as the base address */
#define SlaveReset_PORT_MASK 0x02U     /*!< Mask of the allocated pin from the port */



/*
** ===================================================================
**     Method      :  SlaveReset_Init (component BitIO_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the "Enable
**         in init. code" is set to "yes" value then the device is also
**         enabled(see the description of the Enable() method). In this
**         case the Enable() method is not necessary and needn't to be
**         generated. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* SlaveReset_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  SlaveReset_SetDir (component BitIO_LDD)
*/
/*!
**     @brief
**         Sets a pin direction (available only if the direction =
**         _[input/output]_).
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         Dir             - Direction to set. Possible values:
**                           <false> - Input
**                           <true> - Output
*/
/* ===================================================================*/
void SlaveReset_SetDir(LDD_TDeviceData *DeviceDataPtr, bool Dir);

/*
** ===================================================================
**     Method      :  SlaveReset_SetInput (component BitIO_LDD)
*/
/*!
**     @brief
**         Sets a pin direction to input (available only if the
**         direction = _[input/output]_).
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
*/
/* ===================================================================*/
void SlaveReset_SetInput(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SlaveReset_ClrVal (component BitIO_LDD)
*/
/*!
**     @brief
**         Clears (set to zero) the output value. It is equivalent to
**         the [PutVal(FALSE)]. This method is available only if the
**         direction = _[output]_ or _[input/output]_.
**     @param
**         DeviceDataPtr   - Pointer to device data
**                           structure returned by <Init> method.
*/
/* ===================================================================*/
void SlaveReset_ClrVal(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  SlaveReset_SetVal (component BitIO_LDD)
*/
/*!
**     @brief
**         Sets (to one) the output value. It is equivalent to the
**         [PutVal(TRUE)]. This method is available only if the
**         direction = _[output]_ or _[input/output]_.
**     @param
**         DeviceDataPtr   - Pointer to device data
**                           structure returned by <Init> method.
*/
/* ===================================================================*/
void SlaveReset_SetVal(LDD_TDeviceData *DeviceDataPtr);

/* END SlaveReset. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif
/* ifndef __SlaveReset_H */
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