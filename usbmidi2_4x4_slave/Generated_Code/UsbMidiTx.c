/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : UsbMidiTx.c
**     Project     : usbmidi2_4x4_slave
**     Processor   : MKL26Z64VFM4
**     Component   : RingBufferUInt8
**     Version     : Component 01.003, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-07-19, 21:15, # CodeGen: 0
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
** @file UsbMidiTx.c
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup UsbMidiTx_module UsbMidiTx module documentation
**  @{
*/         

/* MODULE UsbMidiTx. */

#include "UsbMidiTx.h"

static uint8_t UsbMidiTx_buffer[UsbMidiTx_BUF_SIZE]; /* ring buffer */
static uint8_t UsbMidiTx_inIdx;  /* input index */
static uint8_t UsbMidiTx_outIdx; /* output index */
static uint8_t UsbMidiTx_inSize; /* size of input data */
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
byte UsbMidiTx_Put(byte elem)
{
  byte res = ERR_OK;

  EnterCritical();
  if (UsbMidiTx_inSize==UsbMidiTx_BUF_SIZE) {
    res = ERR_TXFULL;
  } else {
    UsbMidiTx_buffer[UsbMidiTx_inIdx] = elem;
    UsbMidiTx_inSize++;
    UsbMidiTx_inIdx++;
    if (UsbMidiTx_inIdx==UsbMidiTx_BUF_SIZE) {
      UsbMidiTx_inIdx = 0;
    }
  }
  ExitCritical();
  return res;
}

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
byte UsbMidiTx_Get(byte *elemP)
{
  byte res = ERR_OK;

  EnterCritical();
  if (UsbMidiTx_inSize==0) {
    res = ERR_RXEMPTY;
  } else {
    *elemP = UsbMidiTx_buffer[UsbMidiTx_outIdx];
    UsbMidiTx_inSize--;
    UsbMidiTx_outIdx++;
    if (UsbMidiTx_outIdx==UsbMidiTx_BUF_SIZE) {
      UsbMidiTx_outIdx = 0;
    }
  }
  ExitCritical();
  return res;
}

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
byte UsbMidiTx_NofElements(void)
{
  return UsbMidiTx_inSize;
}

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
byte UsbMidiTx_NofFreeElements(void)
{
  return (byte)(UsbMidiTx_BUF_SIZE-UsbMidiTx_inSize);
}

/*
** ===================================================================
**     Method      :  UsbMidiTx_Init (component RingBufferUInt8)
**     Description :
**         Initializes the data structure
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void UsbMidiTx_Init(void)
{
  UsbMidiTx_inIdx = 0;
  UsbMidiTx_outIdx = 0;
  UsbMidiTx_inSize = 0;
}

/* END UsbMidiTx. */

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
