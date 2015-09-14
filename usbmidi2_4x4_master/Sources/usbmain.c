#include <string.h>

#include "usb/USB1.h"
#include "WAIT1.h"
#include "usb/USBMIDI1.h"
#include "COREUART.h"
#include "MIDIUART1.h"
#include "MIDIUART2.h"
#include "LED1.h"
#include "PTD.h"
#include "SlaveReset.h"
#include "KIN1.h"
#include "TMOUT1.h"
#include "DMAT1.h"

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

	return sysex_flashing | slave_initialized;
}

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];

static midiin_state_t midiin1_state;
static midiin_state_t midiin2_state;
static midiout_state_t midiout1_state;
static midiout_state_t midiout2_state;

byte usb_timeout_counter_handle;

int usbmidi_can_transmit(int numbytes, void* dummy)
{
	if (TMOUT1_CounterExpired(usb_timeout_counter_handle) && UsbMidiTx_NofElements() > 0) {
		return 0;
	}

	return UsbMidiTx_NofFreeElements() > numbytes;
}

void usbmidi_transmit(const midicmd_t cmd, void* dummy)
{
	if (UsbMidiTx_NofFreeElements() >= 4) {
		UsbMidiTx_Put(cmd.header);
		UsbMidiTx_Put(cmd.b1);
		UsbMidiTx_Put(cmd.b2);
		UsbMidiTx_Put(cmd.b3);
	}
}

int usbmidi_receive(byte* b)
{
	return UsbMidiRx_Get(b);
}

void usb_run(void)
{
	usbmidi_scheduler_state_t usbschedstate;
	usbmidi_scheduler_state_t intercoreschedstate;
	midicmd_t cmd;

	usb_timeout_counter_handle = TMOUT1_GetCounter(2);

	midiin_reset(&midiin1_state);
	midiin1_state.GetCharsInRxBuf = MIDIUART1_GetCharsInRxBuf;
	midiin1_state.RecvChar = MIDIUART1_RecvChar;

	midiin_reset(&midiin2_state);
	midiin2_state.GetCharsInRxBuf = MIDIUART2_GetCharsInRxBuf;
	midiin2_state.RecvChar = MIDIUART2_RecvChar;

	midiout_reset(&midiout1_state);
	midiout1_state.OutBufSize = MIDIUART1_OUT_BUF_SIZE;
	midiout1_state.GetCharsInTxBuf = MIDIUART1_GetCharsInTxBuf;
	midiout1_state.SendChar = MIDIUART1_SendChar;
	midiout1_state.CounterExpired = TMOUT1_CounterExpired;
	midiout1_state.SetCounter = TMOUT1_SetCounter;
	midiout1_state.midiout_counterhandle = TMOUT1_GetCounter(10);

	midiout_reset(&midiout2_state);
	midiout2_state.OutBufSize = MIDIUART2_OUT_BUF_SIZE;
	midiout2_state.GetCharsInTxBuf = MIDIUART2_GetCharsInTxBuf;
	midiout2_state.SendChar = MIDIUART2_SendChar;
	midiout2_state.CounterExpired = TMOUT1_CounterExpired;
	midiout2_state.SetCounter = TMOUT1_SetCounter;
	midiout2_state.midiout_counterhandle = TMOUT1_GetCounter(10);

	usbmidi_scheduler_reset(&usbschedstate);
	usbschedstate.RecvByte = usbmidi_receive;
	usbschedstate.bus_count = 5;
	usbschedstate.CanTransmit[0] = intercore_can_transmit;
	usbschedstate.CanTransmit[1] = midiout_can_transmit;
	usbschedstate.CanTransmit[2] = midiout_can_transmit;
	usbschedstate.CanTransmit[3] = intercore_can_transmit;
	usbschedstate.CanTransmit[4] = intercore_can_transmit;
	usbschedstate.Transmit[0] = intercore_transmit;
	usbschedstate.Transmit[1] = midiout_transmit;
	usbschedstate.Transmit[2] = midiout_transmit;
	usbschedstate.Transmit[3] = intercore_transmit;
	usbschedstate.Transmit[4] = intercore_transmit;
	usbschedstate.TransmitHook[0] = sysex_process;
	usbschedstate.TransmitHook[1] = NULL;
	usbschedstate.TransmitHook[2] = NULL;
	usbschedstate.TransmitHook[3] = NULL;
	usbschedstate.TransmitHook[4] = NULL;
	usbschedstate.busstate[0] = NULL;
	usbschedstate.busstate[1] = &midiout1_state;
	usbschedstate.busstate[2] = &midiout2_state;
	usbschedstate.busstate[3] = NULL;
	usbschedstate.busstate[4] = NULL;

	usbmidi_scheduler_reset(&intercoreschedstate);
	intercoreschedstate.RecvByte = intercore_receive;
	intercoreschedstate.bus_count = 5;
	intercoreschedstate.CanTransmit[0] = usbmidi_can_transmit;
	intercoreschedstate.CanTransmit[1] = midiout_can_transmit;
	intercoreschedstate.CanTransmit[2] = midiout_can_transmit;
	intercoreschedstate.CanTransmit[3] = usbmidi_can_transmit;
	intercoreschedstate.CanTransmit[4] = usbmidi_can_transmit;
	intercoreschedstate.Transmit[0] = usbmidi_transmit;
	intercoreschedstate.Transmit[1] = midiout_transmit;
	intercoreschedstate.Transmit[2] = midiout_transmit;
	intercoreschedstate.Transmit[3] = usbmidi_transmit;
	intercoreschedstate.Transmit[4] = usbmidi_transmit;
	intercoreschedstate.TransmitHook[0] = NULL;
	intercoreschedstate.TransmitHook[1] = NULL;
	intercoreschedstate.TransmitHook[2] = NULL;
	intercoreschedstate.TransmitHook[3] = NULL;
	intercoreschedstate.TransmitHook[4] = NULL;
	intercoreschedstate.busstate[0] = NULL;
	intercoreschedstate.busstate[1] = &midiout1_state;
	intercoreschedstate.busstate[2] = &midiout2_state;
	intercoreschedstate.busstate[3] = NULL;
	intercoreschedstate.busstate[4] = NULL;

	COREUART_Init();
	DMAT1_AllocateChannel(DMAT1_DeviceData);
	DMAT1_EnableChannel(DMAT1_DeviceData);
	//UART0_C2 &= ~(1 << 5); /* Clear RIE */
	UART0_C5 |= (1 << 5); /* RDMAE=1 Enable DMA */
	COREUART_Enable();

	intercore_sync_master();

    for(;;) {
    	if (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		LED1_Off();
    		usbmidi_scheduler_reset(&usbschedstate);
    		UsbMidiRx_Init();
    		UsbMidiTx_Init();
    	}
    	else {
    		LED1_On();
    		if (sysex_flashing | slave_initialized) {
    			// Ignore intercore bus during flashing
    			usbschedstate.CanTransmit[0] = NULL;
    		}
    		usbmidi_scheduler(&usbschedstate);
    	}

    	if (TMOUT1_CounterExpired(usb_timeout_counter_handle)) {
    		// No data going out on USB, reset queue!
    		EnterCritical();
    		UsbMidiTx_Init();
    		ExitCritical();
    	}

    	if (!(sysex_flashing || slave_initialized)) {
    		usbmidi_scheduler(&intercoreschedstate);
    	}

    	int repeat = 1;
    	while (repeat) {
    		repeat = 0;
			int ready = midiin_receive(&cmd, &midiin1_state);
			if (ready) {
				repeat = 1;
				cmd.header &= 0x0F;
				cmd.header |= (0x1 << 4);
				if (usbmidi_can_transmit(4, NULL)) {
					usbmidi_transmit(cmd, NULL);
				}
				//if (intercore_can_transmit(4, NULL)) {
					intercore_transmit(cmd, NULL);
				//}
			}
			ready = midiin_receive(&cmd, &midiin2_state);
			if (ready) {
				repeat = 1;
				cmd.header &= 0x0F;
				cmd.header |= (0x2 << 4);
				if (usbmidi_can_transmit(4, NULL)) {
					usbmidi_transmit(cmd, NULL);
				}
				//if (intercore_can_transmit(4, NULL)) {
					intercore_transmit(cmd, NULL);
				//}
			}
    	}
    }
}
