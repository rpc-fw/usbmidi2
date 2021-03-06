/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : DMAT1.c
**     Project     : usbmidi2_4x4_master
**     Processor   : MKL26Z64VFM4
**     Component   : DMATransfer_LDD
**     Version     : Component 01.100, Driver 01.08, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-10, 17:25, # CodeGen: 0
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
** @file DMAT1.c
** @version 01.08
** @brief
**          This embedded component implements
**          a DMA transfer channel descriptor definition.
*/         
/*!
**  @addtogroup DMAT1_module DMAT1 module documentation
**  @{
*/         

/* MODULE DMAT1. */

#include "DMAT1.h"
/* User external destination object declaration */
#include "uart/intercore.h"

typedef struct {
  DMA1_TDeviceData *DMA_LDD_DeviceDataPtr; /* Pointer to the DMA_LDD component data structure */
  LDD_DMA_TTransferDescriptor *DescriptorPtr; /* Pointer to the component's transfer descriptor */
} DMAT1_TDeviceData;

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static DMAT1_TDeviceData DeviceDataPtr__DEFAULT_RTOS_ALLOC;
/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static LDD_DMA_TTransferDescriptor DescriptorPtr__DEFAULT_RTOS_ALLOC;
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
LDD_TDeviceData* DMAT1_Init(LDD_TUserData *UserDataPtr)
{
  DMAT1_TDeviceData *DeviceDataPtr;    /* LDD device structure */
  LDD_DMA_TTransferDescriptor *DescriptorPtr; /* Transfer descriptor structure */

  /* Allocate LDD device structure */
  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPtr = &DeviceDataPtr__DEFAULT_RTOS_ALLOC;
  /* Allocate Transfer descriptor structure */
  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DescriptorPtr = &DescriptorPtr__DEFAULT_RTOS_ALLOC;
  DeviceDataPtr->DescriptorPtr = DescriptorPtr; /* Store pointer to the descriptor structure */
  /* Transfer descriptor initialization */
  DeviceDataPtr->DescriptorPtr->UserDataPtr = UserDataPtr; /* User device data structure pointer to be returned by the DMA_LDD component's ISR to the dynamic callback of this Descriptor */
  /* Source settings */
  DeviceDataPtr->DescriptorPtr->SourceAddress = (LDD_DMA_TAddress)(uint32_t)&UART0_D; /* Address of a DMA transfer source data */
  DeviceDataPtr->DescriptorPtr->SourceTransferSize = (LDD_DMA_TTransferSize)DMA_PDD_8_BIT; /* 8-bit source data transfer size. */
  DeviceDataPtr->DescriptorPtr->SourceModuloSize = (LDD_DMA_TModuloSize)DMA_PDD_CIRCULAR_BUFFER_DISABLED; /* Circular buffer size. */
  DeviceDataPtr->DescriptorPtr->SourceAddressIncrement = FALSE; /* Address not incremented. */
  /* Destination settings */
  DeviceDataPtr->DescriptorPtr->DestinationAddress = (LDD_DMA_TAddress)(uint32_t)&intercore_dma_data; /* Address of a DMA transfer destination data */
  DeviceDataPtr->DescriptorPtr->DestinationTransferSize = (LDD_DMA_TTransferSize)DMA_PDD_8_BIT; /* 8-bit destination data transfer size. */
  DeviceDataPtr->DescriptorPtr->DestinationModuloSize = (LDD_DMA_TModuloSize)DMA_PDD_CIRCULAR_BUFFER_128_BYTES; /* Circular buffer size. */
  DeviceDataPtr->DescriptorPtr->DestinationAddressIncrement = TRUE; /* Address incremented after each elemental read operation. */
  /* Byte count value */
  DeviceDataPtr->DescriptorPtr->ByteCount = (LDD_DMA_TByteCount)0x7FFFF; /* Size of data to be transferred. */
  /* Trigger settings */
  DeviceDataPtr->DescriptorPtr->TriggerType = LDD_DMA_HW_TRIGGER; /* External peripheral trigger is used */
  DeviceDataPtr->DescriptorPtr->TriggerSource = (LDD_DMA_TTriggerSource)0x02U; /* External peripheral trigger source number */
  DeviceDataPtr->DescriptorPtr->PeriodicTrigger = FALSE; /* Periodic trigger mode is not used */
  DeviceDataPtr->DescriptorPtr->ChannelAutoSelection = FALSE; /* DMA channel fixed value */
  DeviceDataPtr->DescriptorPtr->ChannelNumber = (LDD_DMA_TChannelNumber)0x02U; /* DMA channel number */
  DeviceDataPtr->DescriptorPtr->AsynchronousRequests = FALSE; /* DMA channel asynchronous requests disabled. */
  /* Inner and outer loop linking settings */
  DeviceDataPtr->DescriptorPtr->ChannelLinkingMode = LDD_DMA_LINKING_DISABLED; /* Channel linking disabled. */
  DeviceDataPtr->DescriptorPtr->InnerLoopLinkedChannel = 0x00U; /* Linked DMA channel number not used. */
  DeviceDataPtr->DescriptorPtr->OuterLoopLinkedChannel = 0x00U; /* Linked DMA channel number not used. */
  /* Transfer control settings */
  DeviceDataPtr->DescriptorPtr->TransferMode = LDD_DMA_CYCLE_STEAL_TRANSFERS; /* Cycle-steal transfers - one elementary read-write transfer per request. */
  DeviceDataPtr->DescriptorPtr->DisableAfterRequest = FALSE; /* Leave enabled after request. */
  /* Interrupts and events settings */
  DeviceDataPtr->DescriptorPtr->Interrupts = FALSE; /* Interrupts are not requested. */
  DeviceDataPtr->DescriptorPtr->OnComplete = FALSE; /* Event disabled in initialization code. */
  DeviceDataPtr->DescriptorPtr->OnError = FALSE; /* Event disabled in initialization code. */
  DeviceDataPtr->DescriptorPtr->OnCompleteEventPtr = NULL; /* OnComplete event not enabled */
  DeviceDataPtr->DescriptorPtr->OnErrorEventPtr = NULL; /* OnError event not enabled */
  DeviceDataPtr->DescriptorPtr->ChannelEnabled = FALSE; /* Descriptor is not allocating nor using any channel. */
  if (PE_LDD_DeviceDataList[PE_LDD_COMPONENT_DMA1_ID] == NULL) {
    DeviceDataPtr->DMA_LDD_DeviceDataPtr = DMA1_Init(NULL);
  }
  else {
    DeviceDataPtr->DMA_LDD_DeviceDataPtr = PE_LDD_DeviceDataList[PE_LDD_COMPONENT_DMA1_ID];
  }
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_DMAT1_ID,DeviceDataPtr);
  return ((LDD_TDeviceData *)DeviceDataPtr); /* Return pointer to the data data structure. */
}

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
void DMAT1_Deinit(LDD_TDeviceData *DeviceDataPtr)
{
  ((DMAT1_TDeviceData *)DeviceDataPtr)->DescriptorPtr = NULL;
  /* Unregistration of the device structure */
  PE_LDD_UnregisterDeviceStructure(PE_LDD_COMPONENT_DMAT1_ID);
  /* Deallocation of the Transfer descriptor structure */
  /* {Bareboard RTOS Adapter} Driver memory deallocation: Dynamic allocation is simulated, no deallocation code is generated */
  /* Deallocation of the device structure */
  /* {Bareboard RTOS Adapter} Driver memory deallocation: Dynamic allocation is simulated, no deallocation code is generated */
}

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
LDD_TError DMAT1_AllocateChannel(LDD_TDeviceData *DeviceDataPtr)
{
  return DMA1_AllocateChannel((LDD_TDeviceData *)(((DMAT1_TDeviceData *)DeviceDataPtr)->DMA_LDD_DeviceDataPtr), ((DMAT1_TDeviceData *)DeviceDataPtr)->DescriptorPtr);
}

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
LDD_TError DMAT1_EnableChannel(LDD_TDeviceData *DeviceDataPtr)
{
  return DMA1_EnableChannel((LDD_TDeviceData *)(((DMAT1_TDeviceData *)DeviceDataPtr)->DMA_LDD_DeviceDataPtr), ((DMAT1_TDeviceData *)DeviceDataPtr)->DescriptorPtr);
}

/* END DMAT1. */

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
