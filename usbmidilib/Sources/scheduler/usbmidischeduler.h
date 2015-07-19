#ifndef USBMIDISCHEDULER_H_
#define USBMIDISCHEDULER_H_

#include "../midicmd.h"

typedef int (*CanTransmit_func)(int numbytes, void* state);
typedef void (*Transmit_func)(midicmd_t cmd, void* state);
typedef int (*TransmitHook_func)(midicmd_t cmd);

#define USBMIDI_SCHEDULER_MAX_BUSES		8
typedef struct _usbmidi_scheduler_state_t
{
	int bus_count;

	byte (*UsbMidiRx_NofElements) (void);
	byte (*UsbMidiRx_Get) (byte*);
	int (*RecvByte) (byte* b);

	CanTransmit_func CanTransmit[USBMIDI_SCHEDULER_MAX_BUSES];
	Transmit_func Transmit[USBMIDI_SCHEDULER_MAX_BUSES];
	TransmitHook_func TransmitHook[USBMIDI_SCHEDULER_MAX_BUSES];

	void* busstate[USBMIDI_SCHEDULER_MAX_BUSES];

	byte msg[4];
	int msgbytes;

} usbmidi_scheduler_state_t;

void usbmidi_scheduler_reset(usbmidi_scheduler_state_t* state);
void usbmidi_scheduler(usbmidi_scheduler_state_t* state);

#endif /* USBMIDISCHEDULER_H_ */
