#include "usbmidischeduler.h"

void usbmidi_scheduler_reset(usbmidi_scheduler_state_t* state)
{
	state->have_peek_byte = 0;
	state->peek_byte = 0;
}

void usbmidi_scheduler(usbmidi_scheduler_state_t* state)
{
	midicmd_t cmd;

	int cannot_transmit = 0;
	if (state->have_peek_byte) {
		int cableid = (state->have_peek_byte >> 4);
		if (cableid < state->bus_count) {
			cannot_transmit = !state->CanTransmit[cableid](4, state->busstate[cableid]);
		}
	}

	while (!cannot_transmit && state->UsbMidiRx_NofElements()+state->have_peek_byte >= 4) {
		if (!state->have_peek_byte) {
			state->UsbMidiRx_Get(&state->peek_byte);
			state->have_peek_byte = 1;

			int cableid = (state->peek_byte >> 4);
			if (cableid < 2) {
				cannot_transmit = !state->CanTransmit[cableid](4, state->busstate[cableid]);
			}
		}

		if (!cannot_transmit) {
			if (!state->have_peek_byte) {
				UsbMidiRx_Get(&cmd.header);
			}
			else {
				cmd.header = state->peek_byte;
				state->have_peek_byte = 0;
			}
			UsbMidiRx_Get(&cmd.b1);
			UsbMidiRx_Get(&cmd.b2);
			UsbMidiRx_Get(&cmd.b3);

			int cableid = (cmd.header >> 4);
			if (cableid < state->bus_count
				&& (state->TransmitHook[cableid] == NULL
					|| !state->TransmitHook[cableid](cmd)))
			{
				state->Transmit[cableid](cmd, state->busstate[cableid]);
			}
		}
	}
}
