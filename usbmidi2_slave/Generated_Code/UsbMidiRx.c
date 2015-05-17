/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : UsbMidiRx.c
**     Project     : usbmidi2_slave
**     Processor   : MKL26Z64VFM4
**     Component   : RingBufferUInt8
**     Version     : Component 01.003, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-05-17, 13:50, # CodeGen: 26
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
** @file UsbMidiRx.c
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup UsbMidiRx_module UsbMidiRx module documentation
**  @{
*/         

/* MODULE UsbMidiRx. */

#include "UsbMidiRx.h"

static uint8_t UsbMidiRx_buffer[UsbMidiRx_BUF_SIZE]; /* ring buffer */
static uint8_t UsbMidiRx_inIdx;  /* input index */
static uint8_t UsbMidiRx_outIdx; /* output index */
static uint8_t UsbMidiRx_inSize; /* size of input data */
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
byte UsbMidiRx_Put(byte elem)
{
  byte res = ERR_OK;

  EnterCritical();
  if (UsbMidiRx_inSize==UsbMidiRx_BUF_SIZE) {
    res = ERR_TXFULL;
  } else {
    UsbMidiRx_buffer[UsbMidiRx_inIdx] = elem;
    UsbMidiRx_inSize++;
    UsbMidiRx_inIdx++;
    if (UsbMidiRx_inIdx==UsbMidiRx_BUF_SIZE) {
      UsbMidiRx_inIdx = 0;
    }
  }
  ExitCritical();
  return res;
}

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
byte UsbMidiRx_Get(byte *elemP)
{
  byte res = ERR_OK;

  EnterCritical();
  if (UsbMidiRx_inSize==0) {
    res = ERR_RXEMPTY;
  } else {
    *elemP = UsbMidiRx_buffer[UsbMidiRx_outIdx];
    UsbMidiRx_inSize--;
    UsbMidiRx_outIdx++;
    if (UsbMidiRx_outIdx==UsbMidiRx_BUF_SIZE) {
      UsbMidiRx_outIdx = 0;
    }
  }
  ExitCritical();
  return res;
}

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
byte UsbMidiRx_NofElements(void)
{
  return UsbMidiRx_inSize;
}

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
byte UsbMidiRx_NofFreeElements(void)
{
  return (byte)(UsbMidiRx_BUF_SIZE-UsbMidiRx_inSize);
}

/*
** ===================================================================
**     Method      :  UsbMidiRx_Init (component RingBufferUInt8)
**     Description :
**         Initializes the data structure
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void UsbMidiRx_Init(void)
{
  UsbMidiRx_inIdx = 0;
  UsbMidiRx_outIdx = 0;
  UsbMidiRx_inSize = 0;
}

/* END UsbMidiRx. */

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
