#include "USB1.h"
#include "WAIT1.h"
#include "USBMIDI1.h"
#include "COREUART.h"

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];
//static uint8_t cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t in_buffer[USB1_DATA_BUFF_SIZE];

int usbmidi_msglen(const midicmd_t cmd)
{
	switch (cmd.header & 0x0F) {
	case 0x2: // 2-byte system common
	case 0x6: // 2-byte sysex end
	case 0xC: // program change
	case 0xD: // channel pressure
		return 2; // 2 bytes
	case 0x5:
	case 0xF: // single byte
		return 1; // 1 byte
	case 0x3: // 3-byte system common
	case 0x4: // 3-byte sysex start/continue
	case 0x7: // 3-byte sysex end sysex end
	case 0x8: // note off
	case 0x9: // note on
	case 0xA: // poly-keypress
	case 0xB: // control change
	case 0xE: // pitchbend
		return 3; // 3 bytes
		break;
	default:
		// unimplemented
		break;
	}

	return 0;
}

void intercore_transmit(const midicmd_t cmd)
{
	switch (usbmidi_msglen(cmd)) {
	case 1:
		// 1 byte
		COREUART_SendChar(cmd.b1);
		break;
	case 2:
		// 2 bytes
		COREUART_SendChar(cmd.b1);
		COREUART_SendChar(cmd.b2);
		break;
	case 3:
		// 3 bytes
		COREUART_SendChar(cmd.b1);
		COREUART_SendChar(cmd.b2);
		COREUART_SendChar(cmd.b3);
		break;
	default:
		// unimplemented
		break;
	}
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

void usb_run(void) {
    int i;

    for(;;) {
    	while (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		WAIT1_Waitms(100);
    	}

    	if (UsbMidiRx_NofElements() >= 4) {
    		midicmd_t cmd;
    		UsbMidiRx_Get(&cmd.header);
    		UsbMidiRx_Get(&cmd.b1);
    		UsbMidiRx_Get(&cmd.b2);
    		UsbMidiRx_Get(&cmd.b3);

    		usbmidi_transmit(cmd);
    		intercore_transmit(cmd);

    	}
    }
}
