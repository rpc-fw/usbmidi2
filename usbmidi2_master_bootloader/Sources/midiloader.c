#include "MIDIUART1.h"

#include "midiflash.h"

void midiin_receive()
{
	while (MIDIUART1_GetCharsInRxBuf() > 0)
	{
		byte b;
		if (MIDIUART1_RecvChar(&b) == ERR_OK) {

			sysex_process_byte(b);
		}
	}

}

void MidiLoader()
{
	while(1) {
		midiin_receive();
	}
}
