#include "USB1.h"
#include "WAIT1.h"
#include "USBMIDI1.h"
#include "COREUART.h"
#include "LED1.h"

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];
//static uint8_t cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t in_buffer[USB1_DATA_BUFF_SIZE];

int usbmidi_msglen(const midicmd_t cmd)
{
	switch (cmd.header & 0x0F) {
	case 0x5: // 1-byte sysex
	case 0xF: // single byte
		return 1; // 1 byte
	case 0x2: // 2-byte system common
	case 0x6: // 2-byte sysex end
	case 0xC: // program change
	case 0xD: // channel pressure
		return 2; // 2 bytes
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

int intercore_receive(midicmd_t* cmd)
{
	while (COREUART_GetCharsInRxBuf() >= 4)
	{
		if (COREUART_RecvChar(&cmd->header) != ERR_OK) {
			return 0;
		}
		if (COREUART_RecvChar(&cmd->b1) != ERR_OK) {
			return 0;
		}
		if (COREUART_RecvChar(&cmd->b2) != ERR_OK) {
			return 0;
		}
		if (COREUART_RecvChar(&cmd->b3) != ERR_OK) {
			return 0;
		}
		return 1;
	}

	return 0;
}

void intercore_transmit(const midicmd_t cmd)
{
	COREUART_SendChar(cmd.header);
	COREUART_SendChar(cmd.b1);
	COREUART_SendChar(cmd.b2);
	COREUART_SendChar(cmd.b3);
#if 0
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
#endif
}

enum {
	NoteOff,
	NoteOn,
	Aftertouch,
	ControlChange,
	PatchChange,
	ChannelPressure,
	PitchWheel,
	SysexBegin,
	MTCQuarterFrame,
	SongPosition,
	SongSelect,
	TuneRequest,
	SysexEnd
} modes;

byte midiin_status;
byte midiin_header;
byte midiin_bytes[3];
int midiin_count = 0;
int midiin_length = 0;

void midiin_sysex(midicmd_t* cmd)
{
	cmd->header = 0x4;
	cmd->b1 = midiin_bytes[0];
	cmd->b2 = midiin_bytes[1];
	cmd->b3 = midiin_bytes[2];
}

void midiin_sysex_end(midicmd_t* cmd)
{
	switch (midiin_count) {
	case 1:
		cmd->header = 0x5;
		break;
	case 2:
		cmd->header = 0x6;
		break;
	case 3:
		cmd->header = 0x7;
		break;
	default:
		return;
	}

	cmd->b1 = midiin_bytes[0];
	cmd->b2 = midiin_bytes[1];
	cmd->b3 = midiin_bytes[2];
}

void midiin_pack_cmd(midicmd_t* cmd)
{
	if (midiin_status < 0xF0) {
		cmd->header = midiin_status >> 4;
	}
	else if (midiin_status == 0xF0) {
		if (midiin_bytes[0] == 0xF0) {
			cmd->header = 0x4;
		}
		else {
			switch (midiin_count) {
			case 1:
				cmd->header = 0x5;
				break;
			case 2:
				cmd->header = 0x6;
				break;
			case 3:
				cmd->header = 0x7;
				break;
			default:
				return;
			}
		}
	}
	else {
		if (midiin_length == 1) {
			cmd->header = 0x2;
		}
		else if (midiin_length == 2) {
			cmd->header = 0x3;
		}
		else {
			// uh-oh
			cmd->header = 0xF;
		}
	}
	cmd->b1 = midiin_status;
	cmd->b2 = midiin_bytes[0];
	cmd->b3 = midiin_bytes[1];
}

int midiin_process(midicmd_t* cmd, byte b)
{
	if (b >= 0xF8) {
		// single byte system realtime messages
		cmd->header = 0xF;
		cmd->b1 = b;
		cmd->b2 = 0;
		cmd->b3 = 0;
		return 1;
	}

	if (b == 0xF7) {
		// SysEx End
		midiin_bytes[midiin_count] = b;
		midiin_count++;
		midiin_sysex_end(cmd);
		midiin_status = 0;
		midiin_count = 0;
		midiin_length = 0;
		return 1;
	}

	if ((b & 0x80) == 0) {
		if (midiin_status != 0) {
			midiin_bytes[midiin_count] = b;
			midiin_count++;

			if (midiin_count == midiin_length) {
				if (midiin_status == 0xF0) {
					midiin_sysex(cmd);
				}
				else {
					midiin_pack_cmd(cmd);
				}
				midiin_count = 0;

				return 1;
			}
		}

		return 0;
	}
	else {
		// (b & 0x80)

		switch (b & 0xF0) {
		case 0x80: // note off
		case 0x90: // note on
		case 0xA0: // aftertouch
		case 0xB0: // control change
		case 0xE0: // pitch bend
			midiin_status = b;
			midiin_count = 0;
			midiin_length = 2;
			return 0;
		case 0xC0: // program change
		case 0xD0: // channel pressure
			midiin_status = b;
			midiin_count = 0;
			midiin_length = 1;
			return 0;
		default:
			switch (b) {
			case 0xF0: // SysEx begin
				midiin_status = b;
				midiin_length = 3;
				midiin_bytes[0] = b;
				midiin_count = 1;
				return 0;
			case 0xF1: // MTC quarter frame
				midiin_status = b;
				midiin_count = 0;
				midiin_length = 1;
				return 0;
			case 0xF2: // Song position
				midiin_status = b;
				midiin_count = 0;
				midiin_length = 2;
				return 0;
			case 0xF3: // Song select
				midiin_status = b;
				midiin_count = 0;
				midiin_length = 1;
				return 0;
			case 0xF6: // Tune request
				midiin_status = 0;

				cmd->header = 0x5;
				cmd->b1 = b;
				cmd->b2 = 0;
				cmd->b3 = 0;
				return 1;
			}
		}
	}

	// ignored
	midiin_status = 0;
	return 0;
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
	midicmd_t cmd;

    for(;;) {
    	if (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		LED1_Off();
    		//WAIT1_Waitms(100);
    	}
    	else {
    		LED1_On();
    		if (UsbMidiRx_NofElements() >= 4) {

				while (UsbMidiRx_NofElements() >= 4) {
					UsbMidiRx_Get(&cmd.header);
					UsbMidiRx_Get(&cmd.b1);
					UsbMidiRx_Get(&cmd.b2);
					UsbMidiRx_Get(&cmd.b3);

					// transmit all cables
					intercore_transmit(cmd);
				}
    		}
    	}

		int ready = intercore_receive(&cmd);
		if (ready) {
			// cable is already defined in cmd.header
			usbmidi_transmit(cmd);
		}
    }
}
