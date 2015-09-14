#include "usb/USB1.h"
#include "WAIT1.h"
#include "usb/USBMIDI1.h"
#include "COREUART.h"
#include "MIDIUART3.h"
#include "MIDIUART4.h"
#include "LED1.h"
#include "TMOUT1.h"
#include "DMAT1.h"
#include "uart/intercore.h"
#include "uart/midiparser.h"
#include "scheduler/usbmidischeduler.h"

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];

int usbmidi_receive(byte* b)
{
	return UsbMidiRx_Get(b);
}

byte usb_timeout_counter_handle;

int usbmidi_can_transmit(int numbytes, void* dummy)
{
	if (TMOUT1_CounterExpired(usb_timeout_counter_handle) && UsbMidiTx_NofElements() > 0) {
		return 0;
	}

	return UsbMidiTx_NofFreeElements() >= numbytes;
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

static midiin_state_t midiin3_state;
static midiin_state_t midiin4_state;
static midiout_state_t midiout3_state;
static midiout_state_t midiout4_state;

void usb_run(void)
{
	usbmidi_scheduler_state_t schedstate;
	usbmidi_scheduler_state_t intercoreschedstate;

	midicmd_t cmd;

	usb_timeout_counter_handle = TMOUT1_GetCounter(2);

	midiin_reset(&midiin3_state);
	midiin3_state.GetCharsInRxBuf = MIDIUART3_GetCharsInRxBuf;
	midiin3_state.RecvChar = MIDIUART3_RecvChar;

	midiin_reset(&midiin4_state);
	midiin4_state.GetCharsInRxBuf = MIDIUART4_GetCharsInRxBuf;
	midiin4_state.RecvChar = MIDIUART4_RecvChar;

	midiout_reset(&midiout3_state);
	midiout3_state.OutBufSize = MIDIUART3_OUT_BUF_SIZE;
	midiout3_state.GetCharsInTxBuf = MIDIUART3_GetCharsInTxBuf;
	midiout3_state.SendChar = MIDIUART3_SendChar;
	midiout3_state.CounterExpired = TMOUT1_CounterExpired;
	midiout3_state.SetCounter = TMOUT1_SetCounter;
	midiout3_state.midiout_counterhandle = TMOUT1_GetCounter(10);

	midiout_reset(&midiout4_state);
	midiout4_state.OutBufSize = MIDIUART4_OUT_BUF_SIZE;
	midiout4_state.GetCharsInTxBuf = MIDIUART4_GetCharsInTxBuf;
	midiout4_state.SendChar = MIDIUART4_SendChar;
	midiout4_state.CounterExpired = TMOUT1_CounterExpired;
	midiout4_state.SetCounter = TMOUT1_SetCounter;
	midiout4_state.midiout_counterhandle = TMOUT1_GetCounter(10);

	usbmidi_scheduler_reset(&schedstate);
	schedstate.RecvByte = usbmidi_receive;
	schedstate.bus_count = 5;
	schedstate.CanTransmit[0] = intercore_can_transmit;
	schedstate.Transmit[0] = intercore_transmit;
	schedstate.TransmitHook[0] = NULL;
	schedstate.busstate[0] = NULL;
	schedstate.CanTransmit[1] = intercore_can_transmit;
	schedstate.Transmit[1] = intercore_transmit;
	schedstate.TransmitHook[1] = NULL;
	schedstate.busstate[1] = NULL;
	schedstate.CanTransmit[2] = intercore_can_transmit;
	schedstate.Transmit[2] = intercore_transmit;
	schedstate.TransmitHook[2] = NULL;
	schedstate.busstate[2] = NULL;
	schedstate.CanTransmit[3] = midiout_can_transmit;
	schedstate.Transmit[3] = midiout_transmit;
	schedstate.TransmitHook[3] = NULL;
	schedstate.busstate[3] = &midiout3_state;
	schedstate.CanTransmit[4] = midiout_can_transmit;
	schedstate.Transmit[4] = midiout_transmit;
	schedstate.TransmitHook[4] = NULL;
	schedstate.busstate[4] = &midiout4_state;

	usbmidi_scheduler_reset(&intercoreschedstate);
	intercoreschedstate.RecvByte = intercore_receive;
	intercoreschedstate.bus_count = 5;
	intercoreschedstate.CanTransmit[0] = usbmidi_can_transmit;
	intercoreschedstate.CanTransmit[1] = usbmidi_can_transmit;
	intercoreschedstate.CanTransmit[2] = usbmidi_can_transmit;
	intercoreschedstate.CanTransmit[3] = midiout_can_transmit;
	intercoreschedstate.CanTransmit[4] = midiout_can_transmit;
	intercoreschedstate.Transmit[0] = usbmidi_transmit;
	intercoreschedstate.Transmit[1] = usbmidi_transmit;
	intercoreschedstate.Transmit[2] = usbmidi_transmit;
	intercoreschedstate.Transmit[3] = midiout_transmit;
	intercoreschedstate.Transmit[4] = midiout_transmit;
	intercoreschedstate.TransmitHook[0] = NULL;
	intercoreschedstate.TransmitHook[1] = NULL;
	intercoreschedstate.TransmitHook[2] = NULL;
	intercoreschedstate.TransmitHook[3] = NULL;
	intercoreschedstate.TransmitHook[4] = NULL;
	intercoreschedstate.busstate[0] = NULL;
	intercoreschedstate.busstate[1] = NULL;
	intercoreschedstate.busstate[2] = NULL;
	intercoreschedstate.busstate[3] = &midiout3_state;
	intercoreschedstate.busstate[4] = &midiout4_state;

	COREUART_Init();
	DMAT1_AllocateChannel(DMAT1_DeviceData);
	DMAT1_EnableChannel(DMAT1_DeviceData);
	UART0_C5 |= (1 << 5); /* RDMAE=1 Enable DMA */
	COREUART_Enable();

	intercore_sync_slave();

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

    	if (TMOUT1_CounterExpired(usb_timeout_counter_handle)) {
    		// No data going out on USB, reset queue!
    		EnterCritical();
    		UsbMidiTx_Init();
    		ExitCritical();
    	}

		usbmidi_scheduler(&intercoreschedstate);

#if 0
    	int repeat = 1;
    	while (repeat) {
    		repeat = 0;
			if (usbmidi_can_transmit(4, NULL)) {
				TMOUT1_SetCounter(usb_timeout_counter_handle, 2);
				int ready = intercore_receive(&cmd);
				if (ready) {
					// cable is already defined in cmd.header
					usbmidi_transmit(cmd);
					repeat = 1;
				}
			}
    	}
#endif

    	int repeat = 1;
    	while (repeat) {
    		repeat = 0;
			int ready = midiin_receive(&cmd, &midiin3_state);
			if (ready) {
				repeat = 1;
				cmd.header &= 0x0F;
				cmd.header |= (0x3 << 4);
				if (usbmidi_can_transmit(4, NULL)) {
					usbmidi_transmit(cmd, NULL);
				}
				//if (intercore_can_transmit(4, NULL)) {
					intercore_transmit(cmd, NULL);
				//}
			}
			ready = midiin_receive(&cmd, &midiin4_state);
			if (ready) {
				repeat = 1;
				cmd.header &= 0x0F;
				cmd.header |= (0x4 << 4);
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
