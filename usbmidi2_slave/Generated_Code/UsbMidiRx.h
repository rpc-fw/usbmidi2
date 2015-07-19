/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : UsbMidiRx.h
**     Project     : usbmidi2_slave
**     Processor   : MKL26Z64VFM4
**     Component   : RingBufferUInt8
**     Version     : Component 01.003, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-11, 16:31, # CodeGen: 37
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         Put             - byte UsbMidiRx_Put(byte elem);
**         Get             - byte UsbMidiRx_Get(byte *elemP);
**         NofElements     - byte UsbMidiRx_NofElements(void);
**         NofFreeElements - byte UsbMidiRx_NofFreeElements(void);
**         Init            - void UsbMidiRx_Init(void);
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
** @file UsbMidiRx.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup UsbMidiRx_module UsbMidiRx module documentation
**  @{
*/         

#ifndef __UsbMidiRx_H
#define __UsbMidiRx_H

/* MODULE UsbMidiRx. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */

#include "Cpu.h"


#define UsbMidiRx_BUF_SIZE   128  /* number of elements in the buffer */


byte UsbMidiRx_Put(byte elem);
/*
** ===================================================================
**     Method      :  UsbMidiRx_Put (component RingBufferUInt8)
**     Description :
**         Puts a new element into the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**         elem            - New element to be put into the buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

byte UsbMidiRx_Get(byte *elemP);
/*
** ===================================================================
**     Method      :  UsbMidiRx_Get (component RingBufferUInt8)
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

byte UsbMidiRx_NofElements(void);
/*
** ===================================================================
**     Method      :  UsbMidiRx_NofElements (component RingBufferUInt8)
**     Description :
**         Returns the actual number of elements in the buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/

void UsbMidiRx_Init(void);
/*
** ===================================================================
**     Method      :  UsbMidiRx_Init (component RingBufferUInt8)
**     Description :
**         Initializes the data structure
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

byte UsbMidiRx_NofFreeElements(void);
/*
** ===================================================================
**     Method      :  UsbMidiRx_NofFreeElements (component RingBufferUInt8)
**     Description :
**         Returns the actual number of free elements/space in the
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/

/* END UsbMidiRx. */

#endif
/* ifndef __UsbMidiRx_H */
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
