#include <string.h>

#include "usb/USB1.h"
#include "WAIT1.h"
#include "usb/USBMIDI1.h"
#include "COREUART.h"
#include "MIDIUART1.h"
#include "LED1.h"
#include "PTD.h"
#include "SlaveReset.h"
#include "KIN1.h"
#include "TMOUT1.h"

#include "midiflash.h"
#include "uart/intercore.h"
#include "uart/midiparser.h"
#include "scheduler/usbmidischeduler.h"

void SoftwareReset(void)
{
	KIN1_SoftwareReset();
}

extern int slave_initialized;
extern int sysex_flashing;
extern int sysex_started;

void FlashFailed(int warning)
{
	int infinite = 1;
	int repeats = 0;
	if (warning) {
		infinite = 0;
		repeats = 3;
	}

	while(infinite || repeats > 0) {
		LED1_On();
		WAIT1_Waitms(200);
		LED1_Off();
		WAIT1_Waitms(200);

		if (!infinite) {
			repeats--;
		}
	}
}

int sysex_process(const midicmd_t cmd)
{
	int need_stop = 0;
	switch (cmd.header & 0x0F)
	{
	case 0x5:
		sysex_queue_push(cmd.b1);
		need_stop = 1;
		break;
	case 0xF:
		if (cmd.b1 & 0x80) {
			switch (cmd.b1) {
			case 0xF0:
				sysex_start();
				sysex_queue_push(cmd.b1);
				break;
			case 0xF7:
				sysex_queue_push(cmd.b1);
				need_stop = 1;
				break;
			default:
				need_stop = 1;
				break;
			}
		}
		else if (sysex_started) {
			need_stop = sysex_queue_push(cmd.b1);
		}
		break;
	case 0x6:
		sysex_queue_push(cmd.b1);
		sysex_queue_push(cmd.b2);
		need_stop = 1;
		break;
	case 0x4:
		if (cmd.b1 == 0xF0) {
			sysex_start();
		}
		need_stop |= sysex_queue_push(cmd.b1);
		need_stop |= sysex_queue_push(cmd.b2);
		need_stop |= sysex_queue_push(cmd.b3);
		break;
	case 0x7:
		sysex_queue_push(cmd.b1);
		sysex_queue_push(cmd.b2);
		sysex_queue_push(cmd.b3);
		need_stop = 1;
		break;
	}

	if (need_stop) sysex_stop();

	return sysex_flashing || slave_initialized;
}

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];

static midiin_state_t midiin_state;
static midiout_state_t midiout_state;

void usbmidi_transmit(const midicmd_t cmd)
{
	if (UsbMidiTx_NofFreeElements() >= 4) {
		UsbMidiTx_Put(cmd.header);
		UsbMidiTx_Put(cmd.b1);
		UsbMidiTx_Put(cmd.b2);
		UsbMidiTx_Put(cmd.b3);
	}
}

void usb_run(void)
{
	usbmidi_scheduler_state_t schedstate;
	midicmd_t cmd;

	midiin_reset(&midiin_state);
	midiin_state.GetCharsInRxBuf = MIDIUART1_GetCharsInRxBuf;
	midiin_state.RecvChar = MIDIUART1_RecvChar;

	midiout_reset(&midiout_state);
	midiout_state.OutBufSize = MIDIUART1_OUT_BUF_SIZE;
	midiout_state.GetCharsInTxBuf = MIDIUART1_GetCharsInTxBuf;
	midiout_state.SendChar = MIDIUART1_SendChar;
	midiout_state.CounterExpired = TMOUT1_CounterExpired;
	midiout_state.SetCounter = TMOUT1_SetCounter;
	midiout_state.midiout_counterhandle = TMOUT1_GetCounter(1);

	usbmidi_scheduler_reset(&schedstate);
	schedstate.UsbMidiRx_NofElements = UsbMidiRx_NofElements;
	schedstate.UsbMidiRx_Get = UsbMidiRx_Get;
	schedstate.bus_count = 2;
	schedstate.CanTransmit[0] = intercore_can_transmit;
	schedstate.CanTransmit[1] = midiout_can_transmit;
	schedstate.Transmit[0] = intercore_transmit;
	schedstate.Transmit[1] = midiout_transmit;
	schedstate.TransmitHook[0] = sysex_process;
	schedstate.TransmitHook[1] = NULL;
	schedstate.busstate[0] = NULL;
	schedstate.busstate[1] = &midiout_state;

	COREUART_Init();
	COREUART_Enable();

	intercore_sync_master();

    for(;;) {
    	if (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		LED1_Off();
    		usbmidi_scheduler_reset(&schedstate);
    		UsbMidiRx_Init();
    		UsbMidiTx_Init();
    	}
    	else {
    		LED1_On();
    		usbmidi_scheduler(&schedstate);
    	}

    	if (!(sysex_flashing || slave_initialized)) {
			int ready = intercore_receive(&cmd);
			if (ready) {
				switch (cmd.header & 0xf0) {
				case 0x00:
					usbmidi_transmit(cmd);
					break;
				case 0x10:
					if (midiout_can_transmit(4, &midiout_state)) {
						midiout_transmit(cmd, &midiout_state);
					}
					break;
				}
			}
    	}

		int ready = midiin_receive(&cmd, &midiin_state);
		if (ready) {
			cmd.header &= 0x0F;
			cmd.header |= (0x1 << 4);
			usbmidi_transmit(cmd);
			if (intercore_can_transmit(4, NULL)) {
				intercore_transmit(cmd, NULL);
			}
		}
    }
}
