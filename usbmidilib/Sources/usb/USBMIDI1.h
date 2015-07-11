#ifndef USBMIDI1_H
#define USBMIDI1_H

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "UsbMidiRx.h"
#include "UsbMidiTx.h"
#include <stddef.h> /* for size_t */

#include "Cpu.h"
#include "../midicmd.h"

#define USBMIDI1_DATA_BUFF_SIZE    64

void USBMIDI1_Init(byte regen);
byte USBMIDI1_App_Task(byte *txBuf, size_t txBufSize);

void USBMIDI1_Send(midicmd_t cmd);
midicmd_t USBMIDI1_Recv();

#endif

#if 0
/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : CDC1.h
**     Project     : usbmidi2_master
**     Processor   : MKL26Z128VFM4
**     Component   : FSL_USB_CDC_Device
**     Version     : Component 01.001, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-02-08, 21:05, # CodeGen: 3
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         GetFreeInTxBuf   - word CDC1_GetFreeInTxBuf(void);
**         GetCharsInTxBuf  - word CDC1_GetCharsInTxBuf(void);
**         GetCharsInRxBuf  - word CDC1_GetCharsInRxBuf(void);
**         GetChar          - byte CDC1_GetChar(CDC1_TComData *Chr);
**         RecvChar         - byte CDC1_RecvChar(CDC1_TComData *Chr);
**         SendChar         - byte CDC1_SendChar(CDC1_TComData Chr);
**         SendString       - byte CDC1_SendString(CDC1_TComData *Chr);
**         PutBufferChecked - byte CDC1_PutBufferChecked(byte *buf, size_t bufSize);
**         App_Callback     - void CDC1_App_Callback(byte controller_ID, byte event_type, void *val);
**         Notify_Callback  - void CDC1_Notify_Callback(byte controller_ID, byte event_type, void *val);
**         App_Task         - byte CDC1_App_Task(byte *txBuf, size_t txBufSize);
**         Init             - byte CDC1_Init(void);
**
**     (c) Copyright Freescale, all rights reserved, 2012
**     Ported as Processor Expert component: Erich Styger
**     http: www.freescale.com
** ###################################################################*/
/*!
** @file CDC1.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup CDC1_module CDC1 module documentation
**  @{
*/         

#ifndef __CDC1_H
#define __CDC1_H

/* MODULE CDC1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "Tx1.h"
#include "Rx1.h"
#include <stddef.h> /* for size_t */

#include "Cpu.h"


#ifndef __BWUserType_CDC1_TComData
#define __BWUserType_CDC1_TComData
  typedef byte CDC1_TComData ;         /* User type for communication data type. */
#endif

/*
   DATA_BUFF_SIZE should be greater than or equal to the endpoint buffer size,
   otherwise there will be data loss. For MC9S08JS16, maximum DATA_BUFF_SIZE
   supported is 16 Bytes
*/
#define CDC1_DATA_BUFF_SIZE    64

#define CDC1_USB_ERR_SEND            1  /* Error while sending */
#define CDC1_USB_ERR_BUSOFF          2  /* Bus not ready */
#define CDC1_USB_ERR_INIT            3  /* USB initialization error */
#define CDC1_USB_ERR_TX_CHAR         4  /* Error sending character */
#define CDC1_USB_ERR_TX_STRING       5  /* Error sending string */
#define CDC1_USB_ERR_CHECKED_TXFULL  6  /* Error during sending a checked block */
#define CDC1_USB_ERR_RECEIVE         7  /* Error while starting an receive transaction */


#define CDC1_GetFreeInTxBuf() \
  Tx1_NofFreeElements()
/*
** ===================================================================
**     Method      :  CDC1_GetFreeInTxBuf (component FSL_USB_CDC_Device)
**     Description :
**         Returns the number of free character in the send buffer
**     Parameters  : None
**     Returns     :
**         ---             - Number of free character in the receive
**                           buffer.
** ===================================================================
*/

byte CDC1_RecvChar(CDC1_TComData *Chr);
/*
** ===================================================================
**     Method      :  CDC1_RecvChar (component FSL_USB_CDC_Device)
**     Description :
**         Receives a character from the USB interface. Function is
**         blocking if there is no character in the input buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to where to store the character
**                           received
**     Returns     :
**         ---             - Error code, ERR_OK for success,
**                           ERR_RXEMPTY if nothing is in RX buffer.
** ===================================================================
*/

byte CDC1_SendChar(CDC1_TComData Chr);
/*
** ===================================================================
**     Method      :  CDC1_SendChar (component FSL_USB_CDC_Device)
**     Description :
**         Method to send a character to the USB interface. Method is
**         non-blocking: If the output buffer is full, it tries to send
**         it over USB. If this fails or buffer is still full, the
**         character will be lost. If OnError() event is enabled, the
**         error event will be called in case of error.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send.
**     Returns     :
**         ---             - Error code. ERR_OK for success and
**                           ERR_FAILED otherwise.
** ===================================================================
*/

#define CDC1_GetCharsInTxBuf() \
  Tx1_NofElements()
/*
** ===================================================================
**     Method      :  CDC1_GetCharsInTxBuf (component FSL_USB_CDC_Device)
**     Description :
**         Returns the number of character in the send buffer
**     Parameters  : None
**     Returns     :
**         ---             - Number of character in the receive buffer.
** ===================================================================
*/

#define CDC1_GetCharsInRxBuf() \
  Rx1_NofElements()
/*
** ===================================================================
**     Method      :  CDC1_GetCharsInRxBuf (component FSL_USB_CDC_Device)
**     Description :
**         Returns the number of character in the receive buffer
**     Parameters  : None
**     Returns     :
**         ---             - Number of character in the receive buffer.
** ===================================================================
*/

byte CDC1_Init(void);
/*
** ===================================================================
**     Method      :  CDC1_Init (component FSL_USB_CDC_Device)
**     Description :
**         Initializes the driver
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

byte CDC1_App_Task(byte *txBuf, size_t txBufSize);
/*
** ===================================================================
**     Method      :  CDC1_App_Task (component FSL_USB_CDC_Device)
**     Description :
**         Application task to be called periodically from the main
**         task.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * txBuf           - Pointer to temporary buffer used to
**                           transmit data over USB. Should be equal or
**                           greater than the endpoint buffer size. Data
**                           will be sent in an asynchronous way, so
**                           make sure the buffer is *not* on the stack.
**                           This buffer must be available until the
**                           next transmission.
**         txBufSize       - Size of the buffer in bytes
**     Returns     :
**         ---             - Error code, returns ERR_OK if USB
**                           enumeration has been finished, error code
**                           otherwise.
** ===================================================================
*/

byte CDC1_SendString(CDC1_TComData *Chr);
/*
** ===================================================================
**     Method      :  CDC1_SendString (component FSL_USB_CDC_Device)
**     Description :
**         Method to send a string to the USB interface. Method is
**         non-blocking: if string cannot be sent, it will be lost. If
**         OnError() event is enabled, the error event will be called
**         in case of error.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to string to send.
**     Returns     :
**         ---             - Error code. ERR_OK for success and
**                           ERR_FAILED otherwise.
** ===================================================================
*/

#define CDC1_GetChar(Chr) \
  Rx1_Get(Chr)

/*
** ===================================================================
**     Method      :  CDC1_GetChar (component FSL_USB_CDC_Device)
**     Description :
**         Receives a character from the USB interface. Function is not
**         blocking if there is no character in the input buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to where to store the character
**                           received
**     Returns     :
**         ---             - Error code, ERR_OK for success,
**                           ERR_RXEMPTY if nothing is in RX buffer.
** ===================================================================
*/

byte CDC1_PutBufferChecked(byte *buf, size_t bufSize);
/*
** ===================================================================
**     Method      :  CDC1_PutBufferChecked (component FSL_USB_CDC_Device)
**     Description :
**         Puts a data block into the output buffer, but does not send
**         it. If there is not enough size available, then ERR_TXFULL
**         is returned, otherwise ERR_OK. The application then needs to
**         call USB_App_Callback() to actually send the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buf             - Pointer to buffer to be sent
**         bufsize         - Buffer size in bytes
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void CDC1_App_Callback(byte controller_ID, byte event_type, void *val);
/*
** ===================================================================
**     Method      :  CDC1_App_Callback (component FSL_USB_CDC_Device)
**     Description :
**         Method to handle class callbacks from USB
**     Parameters  :
**         NAME            - DESCRIPTION
**         controller_ID   - controller ID
**         event_type      - value of the event
**         val             - gives the configuration value
**     Returns     : Nothing
** ===================================================================
*/

void CDC1_Notify_Callback(byte controller_ID, byte event_type, void *val);
/*
** ===================================================================
**     Method      :  CDC1_Notify_Callback (component FSL_USB_CDC_Device)
**     Description :
**         Method to handle PSTN Sub Class callbacks
**     Parameters  :
**         NAME            - DESCRIPTION
**         controller_ID   - controller ID
**         event_type      - value of the event
**         val             - 
**     Returns     : Nothing
** ===================================================================
*/

/* END CDC1. */

#endif
/* ifndef __CDC1_H */
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
#endif
