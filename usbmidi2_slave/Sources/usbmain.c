#include "usb/USB1.h"
#include "WAIT1.h"
#include "usb/USBMIDI1.h"
#include "COREUART.h"
#include "LED1.h"
#include "uart/intercore.h"
#include "uart/midiparser.h"
#include "scheduler/usbmidischeduler.h"

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];

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

	usbmidi_scheduler_reset(&schedstate);
	schedstate.UsbMidiRx_NofElements = UsbMidiRx_NofElements;
	schedstate.UsbMidiRx_Get = UsbMidiRx_Get;
	schedstate.bus_count = 2;
	schedstate.CanTransmit[0] = intercore_can_transmit;
	schedstate.Transmit[0] = intercore_transmit;
	schedstate.TransmitHook[0] = NULL;
	schedstate.busstate[0] = NULL;
	schedstate.CanTransmit[1] = intercore_can_transmit;
	schedstate.Transmit[1] = intercore_transmit;
	schedstate.TransmitHook[1] = NULL;
	schedstate.busstate[1] = NULL;

	intercore_sync_slave();

    for(;;) {
    	if (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		LED1_Off();
    		usbmidi_scheduler_reset(&schedstate);
    		UsbMidiRx_Init();
    		UsbMidiTx_Init();
    		//WAIT1_Waitms(100);
    	}
    	else {
    		LED1_On();
    		usbmidi_scheduler(&schedstate);
    	}

		int ready = intercore_receive(&cmd);
		if (ready) {
			// cable is already defined in cmd.header
			usbmidi_transmit(cmd);
		}
    }
}
