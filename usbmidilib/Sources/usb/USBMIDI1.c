#include "USB1.h"
#include "USBMIDI1.h"
#include "TMOUT1.h"
#include "hidef.h"          /* for EnableInterrupts macro */
#include "derivative.h"     /* include peripheral declarations */
#include "types.h"          /* Contains User Defined Data Types */
#include "usb_midi.h"        /* USB CDC Class Header File */

#define  CONTROLLER_ID      (0)   /* ID to identify USB CONTROLLER */

/* Virtual COM Application start Init Flag */
static volatile boolean start_app = FALSE;

/* Virtual COM Application Carrier Activate Flag */
static volatile boolean start_transactions = FALSE;

static volatile boolean transactionOngoing = FALSE;

static volatile int recvactive = 0;

extern byte usb_timeout_counter_handle;

__attribute__((__aligned__(USB1_DATA_BUFF_SIZE))) static uint8_t usbmidi_rx_buffer[USB1_DATA_BUFF_SIZE];

byte USBMIDI1_App_Task(byte *txBuf, size_t txBufSize)
{
  uint8_t i;

  USB_Class_Periodic_Task();

  /* check whether enumeration is complete or not */
  if (start_app == TRUE) {
	  	EnterCritical();
		if (!recvactive && UsbMidiRx_NofFreeElements() >= 64) {
			recvactive = 1;
			ExitCritical();
			_usb_device_recv_data(CONTROLLER_ID, 1/*endpoint*/, usbmidi_rx_buffer, USB1_DATA_BUFF_SIZE);
		}
		else {
			ExitCritical();
		}

    if (UsbMidiTx_NofElements() >= 4 && !transactionOngoing) {

      i = 0;
      while (i < 64 && UsbMidiTx_NofElements() >= 4) {
		  for (int k = 0;
			   k < 4 && UsbMidiTx_Get(&txBuf[i]) == ERR_OK;
			   k++) {
			i++;
		  }
      }

      transactionOngoing = TRUE;
      TMOUT1_SetCounter(usb_timeout_counter_handle, 2);

      if (USB_Class_MIDI_Send_Data(CONTROLLER_ID, 2, txBuf, i) != USB_OK) {
        transactionOngoing = FALSE;
        return ERR_FAULT;
      }


#if 0 /* workaround for problem in USB stack v3.1.1: if last block is 8, 16, 32, 40, 48, ... bytes, it does not get out until the next transfer? */
      if ((i%8)==0) {
        /* workaround: sending a dummy block of zero bytes */
        transactionOngoing = TRUE;
        if (USB_Class_MIDI_Send_Data(CONTROLLER_ID, 2, txBuf, 0) != USB_OK) {
          transactionOngoing = FALSE;
          return ERR_FAULT;
        }
        while(transactionOngoing){} /* wait until transaction is finished */
      }
#endif
    }
    else if (!transactionOngoing) {
        TMOUT1_SetCounter(usb_timeout_counter_handle, 5);
    }
    return ERR_OK;
  } else {
    return ERR_BUSOFF; /* USB bus not available yet */
  }
}

void USBMIDI1_App_Callback(byte controller_ID, byte event_type, void *val)
{
	UNUSED (controller_ID);
	UNUSED (val);

	if (event_type == USB_APP_BUS_RESET) {
		start_app = FALSE;
	}
	else if (event_type == USB_APP_ENUM_COMPLETE) {
#if HIGH_SPEED_DEVICE
		/* prepare for the next receive event */
		USB_Class_CDC_Interface_DIC_Recv_Data(&controller_ID,
				(uint_8_ptr)g_midiBuffer,
				DIC_BULK_OUT_ENDP_PACKET_SIZE);
#else
		EnterCritical();
		if (!recvactive) {
			recvactive = 1;
			ExitCritical();
			_usb_device_recv_data(CONTROLLER_ID, 1, usbmidi_rx_buffer, USB1_DATA_BUFF_SIZE);
		}
		else {
			ExitCritical();
		}
#endif
		start_app = TRUE;
	}
	else if ((event_type == USB_APP_DATA_RECEIVED) && (start_app == TRUE)) {

		/* Copy Received Data buffer to Application Buffer */
		USB_PACKET_SIZE BytesToBeCopied;
		APP_DATA_STRUCT *dp_rcv = (APP_DATA_STRUCT*)val;
		uint_8 index;

		BytesToBeCopied = (USB_PACKET_SIZE)((dp_rcv->data_size > USBMIDI1_DATA_BUFF_SIZE) ? USBMIDI1_DATA_BUFF_SIZE : dp_rcv->data_size);
		for(index = 0; index < BytesToBeCopied; index++) {
			UsbMidiRx_Put(dp_rcv->data_ptr[index]);
#if 0
			while(UsbMidiRx_Put(dp_rcv->data_ptr[index]) != ERR_OK) {
				/* retry until we can place things in buffer */
			}
#endif
		}

		EnterCritical();
		if (UsbMidiRx_NofFreeElements() >= 64) {
			recvactive = 1;
			ExitCritical();
			_usb_device_recv_data(CONTROLLER_ID, 1/*endpoint*/, usbmidi_rx_buffer, USB1_DATA_BUFF_SIZE);
		}
		else {
			recvactive = 0;
			ExitCritical();
		}

	} else if ((event_type == USB_APP_SEND_COMPLETE)) {
		TMOUT1_SetCounter(usb_timeout_counter_handle, 5);
		transactionOngoing = FALSE;
		/* Previous Send is complete. Queue next receive */
#if HIGH_SPEED_DEVICE
		//(void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, g_cdcBuffer, 0);
#else
		//_usb_device_recv_data(CONTROLLER_ID, 1/*endpoint*/, NULL, 0);
#endif
	} else if (event_type == USB_APP_ERROR) { /* detach? */
		start_app = FALSE;
		start_transactions = FALSE;
		TMOUT1_SetCounter(usb_timeout_counter_handle, 1);
	}
}


void USBMIDI1_Init(byte regen)
{
	uint_8 err;

	err = USB_Class_MIDI_Init(CONTROLLER_ID, USBMIDI1_App_Callback, NULL, regen);
	while (err != ERR_OK)
	{
		// init failed
	};
}
