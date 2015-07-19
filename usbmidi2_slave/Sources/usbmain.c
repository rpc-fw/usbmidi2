#include "usb/USB1.h"
#include "WAIT1.h"
#include "usb/USBMIDI1.h"
#include "COREUART.h"
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
	usbmidi_scheduler_state_t intercoreschedstate;

	midicmd_t cmd;

	usb_timeout_counter_handle = TMOUT1_GetCounter(2);

	usbmidi_scheduler_reset(&schedstate);
	schedstate.RecvByte = usbmidi_receive;
	schedstate.bus_count = 2;
	schedstate.CanTransmit[0] = intercore_can_transmit;
	schedstate.Transmit[0] = intercore_transmit;
	schedstate.TransmitHook[0] = NULL;
	schedstate.busstate[0] = NULL;
	schedstate.CanTransmit[1] = intercore_can_transmit;
	schedstate.Transmit[1] = intercore_transmit;
	schedstate.TransmitHook[1] = NULL;
	schedstate.busstate[1] = NULL;

	usbmidi_scheduler_reset(&intercoreschedstate);
	intercoreschedstate.RecvByte = intercore_receive;
	intercoreschedstate.bus_count = 2;
	intercoreschedstate.CanTransmit[0] = usbmidi_can_transmit;
	intercoreschedstate.CanTransmit[1] = usbmidi_can_transmit;
	intercoreschedstate.Transmit[0] = usbmidi_transmit;
	intercoreschedstate.Transmit[1] = usbmidi_transmit;
	intercoreschedstate.TransmitHook[0] = NULL;
	intercoreschedstate.TransmitHook[1] = NULL;
	intercoreschedstate.busstate[0] = NULL;
	intercoreschedstate.busstate[1] = NULL;

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
    }
}
