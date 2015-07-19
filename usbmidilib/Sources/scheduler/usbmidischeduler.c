#include "PE_Error.h"
#include "usbmidischeduler.h"

void usbmidi_scheduler_reset(usbmidi_scheduler_state_t* state)
{
	state->msgbytes = 0;
}

int usbmidi_msglen(const midicmd_t cmd);

void usbmidi_scheduler(usbmidi_scheduler_state_t* state)
{
	int can_transmit = 1;
	if (state->msgbytes >= 1) {
		int cableid = (state->msg[0] >> 4);
		if (cableid < state->bus_count && state->CanTransmit[cableid] != NULL) {
			can_transmit = state->CanTransmit[cableid](4, state->busstate[cableid]);
		}
	}

	byte b;
	byte status = ERR_OK;
	while (can_transmit && (status = state->RecvByte(&b)) == ERR_OK) {
		state->msg[state->msgbytes++] = b;

		if (state->msgbytes == 1) {
			int cableid = (state->msg[0] >> 4);
			if (cableid < state->bus_count && state->CanTransmit[cableid] != NULL) {
				can_transmit = state->CanTransmit[cableid](4, state->busstate[cableid]);
			}
			else {
				// avoid hanging
				can_transmit = 1;
			}
		}

		if (state->msgbytes == 4) {
			midicmd_t cmd;
			cmd.header = state->msg[0];
			cmd.b1 = state->msg[1];
			cmd.b2 = state->msg[2];
			cmd.b3 = state->msg[3];
			state->msgbytes = 0;

			int cableid = (cmd.header >> 4);
			if (cableid < state->bus_count
				&& (state->TransmitHook[cableid] == NULL
					|| !state->TransmitHook[cableid](cmd)))
			{
				state->Transmit[cableid](cmd, state->busstate[cableid]);
			}
		}
	}

	if (status == ERR_FRAMING) {
		state->msgbytes = 0;
	}
}
