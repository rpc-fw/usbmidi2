/* ###################################################################
**     Filename    : Events.h
**     Project     : kl26test
**     Processor   : MKL26Z64VFM4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-01-31, 10:37, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PTC.h"
#include "MIDIUART1.h"
#include "ASerialLdd1.h"
#include "COREUART.h"
#include "ASerialLdd2.h"
#include "PTE.h"
#include "BBA1.h"
#include "USB0.h"
#include "LED1.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "WAIT1.h"
#include "PTD.h"
#include "SlaveReset.h"
#include "KIN1.h"
#include "UTIL1.h"
#include "TMOUT1.h"
#include "CS1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "PTA.h"
#include "UsbMidiTx.h"
#include "UsbMidiRx.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL26Z256MC4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


/*
** ===================================================================
**     Event       :  MIDIUART1_OnError (module Events)
**
**     Component   :  MIDIUART1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MIDIUART1_OnError(void);

/*
** ===================================================================
**     Event       :  MIDIUART1_OnRxChar (module Events)
**
**     Component   :  MIDIUART1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MIDIUART1_OnRxChar(void);

/*
** ===================================================================
**     Event       :  MIDIUART1_OnTxChar (module Events)
**
**     Component   :  MIDIUART1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MIDIUART1_OnTxChar(void);

/*
** ===================================================================
**     Event       :  MIDIUART1_OnFullRxBuf (module Events)
**
**     Component   :  MIDIUART1 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MIDIUART1_OnFullRxBuf(void);

/*
** ===================================================================
**     Event       :  MIDIUART1_OnFreeTxBuf (module Events)
**
**     Component   :  MIDIUART1 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void MIDIUART1_OnFreeTxBuf(void);

/*
** ===================================================================
**     Event       :  COREUART_OnError (module Events)
**
**     Component   :  COREUART [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void COREUART_OnError(void);

/*
** ===================================================================
**     Event       :  COREUART_OnRxChar (module Events)
**
**     Component   :  COREUART [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void COREUART_OnRxChar(void);

/*
** ===================================================================
**     Event       :  COREUART_OnTxChar (module Events)
**
**     Component   :  COREUART [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void COREUART_OnTxChar(void);

/*
** ===================================================================
**     Event       :  COREUART_OnFullRxBuf (module Events)
**
**     Component   :  COREUART [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void COREUART_OnFullRxBuf(void);

/*
** ===================================================================
**     Event       :  COREUART_OnFreeTxBuf (module Events)
**
**     Component   :  COREUART [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void COREUART_OnFreeTxBuf(void);

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
