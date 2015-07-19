#include "midiparser.h"

static int usbmidi_msglen(const midicmd_t cmd)
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
	case 0x7: // 3-byte sysex end
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

void midiin_reset(midiin_state_t* state)
{
	state->midiin_status = 0;
	state->midiin_header = 0;
	state->midiin_count = 0;
	state->midiin_length = 0;
}

static void midiin_sysex(midicmd_t* cmd, midiin_state_t* state)
{
	cmd->header = 0x4;
	cmd->b1 = state->midiin_bytes[0];
	cmd->b2 = state->midiin_bytes[1];
	cmd->b3 = state->midiin_bytes[2];
}

static void midiin_sysex_end(midicmd_t* cmd, midiin_state_t* state)
{
	switch (state->midiin_count) {
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

	cmd->b1 = state->midiin_bytes[0];
	cmd->b2 = state->midiin_bytes[1];
	cmd->b3 = state->midiin_bytes[2];
}

static void midiin_pack_cmd(midicmd_t* cmd, midiin_state_t* state)
{
	if (state->midiin_status < 0xF0) {
		cmd->header = state->midiin_status >> 4;
	}
	else if (state->midiin_status == 0xF0) {
		if (state->midiin_bytes[0] == 0xF0) {
			cmd->header = 0x4;
		}
		else {
			switch (state->midiin_count) {
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
		if (state->midiin_length == 1) {
			cmd->header = 0x2;
		}
		else if (state->midiin_length == 2) {
			cmd->header = 0x3;
		}
		else {
			// uh-oh
			cmd->header = 0xF;
		}
	}
	cmd->b1 = state->midiin_status;
	cmd->b2 = state->midiin_bytes[0];
	cmd->b3 = state->midiin_bytes[1];
}

int midiin_process(midicmd_t* cmd, byte b, midiin_state_t* state)
{
	if (b == 0xf4 || b == 0xf5) {
		// ignore internal sync bytes
		return 0;
	}

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
		state->midiin_bytes[state->midiin_count] = b;
		state->midiin_count++;
		midiin_sysex_end(cmd, state);
		state->midiin_status = 0;
		state->midiin_count = 0;
		state->midiin_length = 0;
		return 1;
	}

	if ((b & 0x80) == 0) {
		if (state->midiin_status != 0) {
			state->midiin_bytes[state->midiin_count] = b;
			state->midiin_count++;

			if (state->midiin_count == state->midiin_length) {
				if (state->midiin_status == 0xF0) {
					midiin_sysex(cmd, state);
				}
				else {
					midiin_pack_cmd(cmd, state);
				}
				state->midiin_count = 0;

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
			state->midiin_status = b;
			state->midiin_count = 0;
			state->midiin_length = 2;
			return 0;
		case 0xC0: // program change
		case 0xD0: // channel pressure
			state->midiin_status = b;
			state->midiin_count = 0;
			state->midiin_length = 1;
			return 0;
		default:
			switch (b) {
			case 0xF0: // SysEx begin
				state->midiin_status = b;
				state->midiin_length = 3;
				state->midiin_bytes[0] = b;
				state->midiin_count = 1;
				return 0;
			case 0xF1: // MTC quarter frame
				state->midiin_status = b;
				state->midiin_count = 0;
				state->midiin_length = 1;
				return 0;
			case 0xF2: // Song position
				state->midiin_status = b;
				state->midiin_count = 0;
				state->midiin_length = 2;
				return 0;
			case 0xF3: // Song select
				state->midiin_status = b;
				state->midiin_count = 0;
				state->midiin_length = 1;
				return 0;
			case 0xF6: // Tune request
				state->midiin_status = 0;

				cmd->header = 0x5;
				cmd->b1 = b;
				cmd->b2 = 0;
				cmd->b3 = 0;
				return 1;
			}
		}
	}

	// ignored
	state->midiin_status = 0;
	return 0;
}

#ifndef ERR_OK
#define ERR_OK                          0x00U /*!< OK */
#endif

int midiin_receive(midicmd_t* cmd, midiin_state_t* state)
{
	while (state->GetCharsInRxBuf() > 0)
	{
		byte b;
		if (state->RecvChar(&b) == ERR_OK) {
			if (midiin_process(cmd, b, state)) {
				return 1;
			}
		}
	}

	return 0;
}


void midiout_reset(midiout_state_t* state)
{
	state->midiout_laststatus = 0;
}

int midiout_can_transmit(int numbytes, midiout_state_t* state)
{
	return (state->OutBufSize - state->GetCharsInTxBuf()) > numbytes;
}

void midiout_resetstatus(midiout_state_t* state)
{
	state->midiout_laststatus = 0;
}

int midiout_checkstatus(byte status, midiout_state_t* state)
{
	// allow non-status bytes
	if ((status & 0x80) == 0) {
		return 1;
	}

	// allow all real-time status bytes
	if ((status & 0xF0) == 0xF0) {
		if (status <= 0xF7) {
			midiout_resetstatus(state);
		}
		return 1;
	}

	// filter rest of status bytes if status did not change
	if (status == state->midiout_laststatus && !state->CounterExpired(state->midiout_counterhandle)) {
		return 0;
	}

	state->midiout_laststatus = status;
	state->SetCounter(state->midiout_counterhandle, 10); // 10 ticks, 10ms each = 100ms

	return 1;
}

void midiout_transmit(const midicmd_t cmd, midiout_state_t* state)
{
	switch (usbmidi_msglen(cmd)) {
	case 1:
		// 1 byte
		state->SendChar(cmd.b1);
		break;
	case 2:
		// 2 bytes
		if (midiout_checkstatus(cmd.b1, state)) {
			state->SendChar(cmd.b1);
		}
		state->SendChar(cmd.b2);
		break;
	case 3:
		// 3 bytes
		if (midiout_checkstatus(cmd.b1, state)) {
			state->SendChar(cmd.b1);
		}
		state->SendChar(cmd.b2);
		state->SendChar(cmd.b3);
		break;
	default:
		// unimplemented
		break;
	}
}
