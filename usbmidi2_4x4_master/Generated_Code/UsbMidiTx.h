/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : UsbMidiTx.h
**     Project     : usbmidi2_4x4_master
**     Processor   : MKL26Z64VFM4
**     Component   : RingBufferUInt8
**     Version     : Component 01.003, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-10, 17:25, # CodeGen: 0
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         Put             - byte UsbMidiTx_Put(byte elem);
**         Get             - byte UsbMidiTx_Get(byte *elemP);
**         NofElements     - byte UsbMidiTx_NofElements(void);
**         NofFreeElements - byte UsbMidiTx_NofFreeElements(void);
**         Init            - void UsbMidiTx_Init(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2012, all rights reserved.
**     This an open source software of an embedded component for Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file UsbMidiTx.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup UsbMidiTx_module UsbMidiTx module documentation
**  @{
*/         

#ifndef __UsbMidiTx_H
#define __UsbMidiTx_H

/* MODULE UsbMidiTx. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */

#include "Cpu.h"


#define UsbMidiTx_BUF_SIZE   128  /* number of elements in the buffer */


byte UsbMidiTx_Put(byte elem);
/*
** ===================================================================
**     Method      :  UsbMidiTx_Put (component RingBufferUInt8)
**     Description :
**         Puts a new element into the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**         elem            - New element to be put into the buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

byte UsbMidiTx_Get(byte *elemP);
/*
** ===================================================================
**     Method      :  UsbMidiTx_Get (component RingBufferUInt8)
**     Description :
**         Removes an element from the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**       * elemP           - Pointer to where to store the received
**                           element
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

byte UsbMidiTx_NofElements(void);
/*
** ===================================================================
**     Method      :  UsbMidiTx_NofElements (component RingBufferUInt8)
**     Description :
**         Returns the actual number of elements in the buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/

void UsbMidiTx_Init(void);
/*
** ===================================================================
**     Method      :  UsbMidiTx_Init (component RingBufferUInt8)
**     Description :
**         Initializes the data structure
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

byte UsbMidiTx_NofFreeElements(void);
/*
** ===================================================================
**     Method      :  UsbMidiTx_NofFreeElements (component RingBufferUInt8)
**     Description :
**         Returns the actual number of free elements/space in the
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/

/* END UsbMidiTx. */

#endif
/* ifndef __UsbMidiTx_H */
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
