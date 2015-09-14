#ifndef MIDIPARSER_H_
#define MIDIPARSER_H_

#include "PE_Types.h"
#include "../midicmd.h"

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

typedef struct _midiin_state_t
{
	byte midiin_status;
	byte midiin_header;
	byte midiin_bytes[3];
	int midiin_count;
	int midiin_length;

	word (*GetCharsInRxBuf) (void);
	byte (*RecvChar) (byte *Chr);
} midiin_state_t;

void midiin_reset(midiin_state_t* state);
int midiin_process(midicmd_t* cmd, byte b, midiin_state_t* state);

int midiin_receive(midicmd_t* cmd, midiin_state_t* state);

typedef struct _midiout_state_t
{
	int midiout_laststatus;

	int OutBufSize;
	word (*GetCharsInTxBuf) (void);
	byte (*SendChar) (byte);

	byte midiout_counterhandle;
	bool (*CounterExpired) (byte);
	int16_t (*SetCounter)(byte, int16_t);

} midiout_state_t;

void midiout_reset(midiout_state_t *state);
int midiout_can_transmit(int numbytes, midiout_state_t* state);
void midiout_transmit(const midicmd_t cmd, void* _state);

#endif /* MIDIPARSER_H_ */
