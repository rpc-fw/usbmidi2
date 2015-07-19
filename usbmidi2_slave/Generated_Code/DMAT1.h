/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : DMAT1.h
**     Project     : usbmidi2_slave
**     Processor   : MKL26Z64VFM4
**     Component   : DMATransfer_LDD
**     Version     : Component 01.100, Driver 01.08, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-19, 19:05, # CodeGen: 52
**     Abstract    :
**          This embedded component implements
**          a DMA transfer channel descriptor definition.
**     Settings    :
**          Component name                                 : DMAT1
**          DMA controller device                          : DMA1
**          Channel                                        : 
**            Channel select                               : Fixed
**              Channel                                    : DMA_Channel2
**              Interrupts                                 : Disabled
**              Allocate channel                           : yes
**          Trigger                                        : 
**            Trigger source type                          : Peripheral device
**              Trigger source                             : UART0_Receive_DMA_Request
**              Periodic trigger                           : Disabled
**          Data source                                    : 
**            External object declaration                  : 
**            Address                                      : (uint32_t)&UART0_D
**            Transfer size                                : 8-bit
**            Address offset                               : 0
**            Circular buffer                              : Buffer disabled
**          Data destination                               : 
**            External object declaration                  : #include "uart/intercore.h"
**            Address                                      : (uint32_t)&intercore_dma_data
**            Transfer size                                : 8-bit
**            Address offset                               : 1
**            Circular buffer                              : 128 Bytes
**          Data size                                      : 
**            External object declaration                  : 
**            Value                                        : 0x7FFFF
**          Transfer control                               : Cycle-steal
**            Disable after transfer                       : no
**            Asynchronous requests                        : Disabled
**            Channel linking                              : Disabled
**            After request complete                       : No action
**            After transfer complete                      : No action
**          Initialization                                 : 
**            Auto initialization                          : yes
**            Half complete                                : Disabled
**            Event mask                                   : 
**              OnComplete                                 : Disabled
**              OnError                                    : Disabled
**     Contents    :
**         Init            - LDD_TDeviceData* DMAT1_Init(LDD_TUserData *UserDataPtr);
**         Deinit          - void DMAT1_Deinit(LDD_TDeviceData *DeviceDataPtr);
**         AllocateChannel - LDD_TError DMAT1_AllocateChannel(LDD_TDeviceData *DeviceDataPtr);
**         EnableChannel   - LDD_TError DMAT1_EnableChannel(LDD_TDeviceData *DeviceDataPtr);
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
** @file DMAT1.h
** @version 01.08
** @brief
**          This embedded component implements
**          a DMA transfer channel descriptor definition.
*/         
/*!
**  @addtogroup DMAT1_module DMAT1 module documentation
**  @{
*/         

#ifndef __DMAT1_H
#define __DMAT1_H

/* MODULE DMAT1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "DMA1.h"
#include "DMA_PDD.h"
#include "DMAMUX_PDD.h"

#include "Cpu.h"

/*! Peripheral base address of a device allocated by the component. This constant can be used directly in PDD macros. */
#define DMAT1_PRPH_BASE_ADDRESS  0x40008000U
  
/*! Device data structure pointer used when auto initialization property is enabled. This constant can be passed as a first parameter to all component's methods. */
#define DMAT1_DeviceData  ((LDD_TDeviceData *)PE_LDD_GetDeviceStructure(PE_LDD_COMPONENT_DMAT1_ID))

/* Methods configuration constants - generated for all enabled component's methods */
#define DMAT1_Init_METHOD_ENABLED      /*!< Init method of the component DMAT1 is enabled (generated) */
#define DMAT1_Deinit_METHOD_ENABLED    /*!< Deinit method of the component DMAT1 is enabled (generated) */
#define DMAT1_AllocateChannel_METHOD_ENABLED /*!< AllocateChannel method of the component DMAT1 is enabled (generated) */
#define DMAT1_EnableChannel_METHOD_ENABLED /*!< EnableChannel method of the component DMAT1 is enabled (generated) */

/* Events configuration constants - generated for all enabled component's events */

/* Source circular buffer base address mask. Represents required 0-modulo-size mask of base source address when using circular buffer. */
#define DMAT1_SOURCE_CIRCULAR_BUFFER_ADDRESS_MASK 0U
/* Destination circular buffer base address mask. Represents required 0-modulo-size mask of base destination address when using circular buffer. */
#define DMAT1_DESTINATION_CIRCULAR_BUFFER_ADDRESS_MASK 127U

/*
** ===================================================================
**     Method      :  DMAT1_Init (component DMATransfer_LDD)
*/
/*!
**     @brief
**         Initializes the DMATransfer component. Allocates memory for
**         the device data structure.
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* DMAT1_Init(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Method      :  DMAT1_Deinit (component DMATransfer_LDD)
*/
/*!
**     @brief
**         Deinitializes the DMATransfer component.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
*/
/* ===================================================================*/
void DMAT1_Deinit(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  DMAT1_AllocateChannel (component DMATransfer_LDD)
*/
/*!
**     @brief
**         The method allocates DMA channel for the DMA transfer. This
**         method must be called before DMA channel is initialized.
**         Otherwise ERR_NOTAVAIL error code is returned after call of
**         DMA_LDD channel methods.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - Error code, possible codes:
**                           - ERR_OK - OK.
**                           - ERR_SPEED - This device does not work in
**                           the active clock configuration.
**                           - ERR_DISABLED - Component or device is
**                           disabled.
**                           - ERR_NOTAVAIL - There is not any possible
**                           channel with desired features to be
**                           allocated.
*/
/* ===================================================================*/
LDD_TError DMAT1_AllocateChannel(LDD_TDeviceData *DeviceDataPtr);

/*
** ===================================================================
**     Method      :  DMAT1_EnableChannel (component DMATransfer_LDD)
*/
/*!
**     @brief
**         The method initializes and/or enables the DMA channel
**         allocated for the DMA transfer defined by the transfer
**         descriptor input parameter DescriptorPtr. First call of this
**         method on given DMA channel initializes it and enables HW
**         requests if Peripheral device or Always enabled trigger
**         source is selected in transfer descriptor Trigger source
**         type item. This method can be also used together with
**         Disable method to temporary switch On/Off the DMA channel HW
**         requests. In such case DMA channel registers are not
**         reinitialized again. New initialization of DMA channel can
**         be done via SetChannelDescriptorUpdate method which
**         reinitializes whole channel with new transfer descriptor.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - Error code, possible codes:
**                           - ERR_OK - OK.
**                           - ERR_SPEED - This device does not work in
**                           the active clock configuration.
**                           - ERR_DISABLED - Component or device is
**                           disabled.
**                           - ERR_NOTAVAIL - There is not any DMA
**                           channel allocated for the selected transfer
**                           descriptor.
**                           - ERR_PARAM_DATA - Transfer descriptor data
**                           are inconsistent.
*/
/* ===================================================================*/
LDD_TError DMAT1_EnableChannel(LDD_TDeviceData *DeviceDataPtr);

/* END DMAT1. */

#endif
/* ifndef __DMAT1_H */
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
