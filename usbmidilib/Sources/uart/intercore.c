#include "../usb/USBMIDI1.h"
#include "intercore.h"

byte COREUART_RecvChar(byte *Chr);
byte COREUART_SendChar(byte Chr);
word COREUART_GetCharsInRxBuf(void);
word COREUART_GetCharsInTxBuf(void);
#define COREUART_OUT_BUF_SIZE  0x0100U

static int intercore_waiting_ack = 0;
static int intercore_sent_counter = 0;
static int intercore_recv_counter = 0;

void intercore_sync_master()
{
	byte b;

	// wait for slave to send sync
	COREUART_ClearRxBuf();
	COREUART_ClearTxBuf();

	while(1) {
		if (COREUART_GetCharsInRxBuf() > 0) {
			byte b;
			if (COREUART_RecvChar(&b) == ERR_OK) {
				if (b == 0xf5) {
					// got sync, done
					break;
				}
			}
		}
	}

	// send sync
	COREUART_SendChar(0xf5);

	while(1) {
		if (COREUART_GetCharsInRxBuf() == 0 && COREUART_GetCharsInTxBuf() == 0) {
			// queues are empty, send a new sync
			COREUART_SendChar(0xf5);
		}
		if (COREUART_GetCharsInRxBuf() > 0) {
			byte b;
			if (COREUART_RecvChar(&b) == ERR_OK) {
				if (b == 0xf4) {
					// ready, done
					return;
				}
			}
		}
	}
}

void intercore_sync_slave()
{
	byte b;

	// send sync
	COREUART_SendChar(0xf5);

	while(1) {
		if (COREUART_GetCharsInRxBuf() == 0 && COREUART_GetCharsInTxBuf() == 0) {
			// queues are empty, send a new sync
			COREUART_SendChar(0xf5);
		}
		if (COREUART_GetCharsInRxBuf() > 0) {
			byte b;
			if (COREUART_RecvChar(&b) == ERR_OK) {
				if (b == 0xf5) {
					// got sync, done
					COREUART_SendChar(0xf4);
					return;
				}
			}
		}
	}
}

void intercore_received_4()
{
	intercore_recv_counter += 4;
	if (intercore_recv_counter >= 16) {
		COREUART_SendChar(0xf4);
	}
}

void intercore_sent_4()
{
	intercore_sent_counter += 4;
	if (intercore_sent_counter >= 16) {
		intercore_waiting_ack = 1;
	}
}

static byte intercore_msgbuf[4];
static int intercore_msgbuf_counter = 0;

int intercore_receive(midicmd_t* cmd)
{
	while (COREUART_GetCharsInRxBuf() >= 1)
	{
		byte b;
		if (COREUART_RecvChar(&b) != ERR_OK) {
			return 0;
		}

		if (b == 0xf5) {
			// sync
			intercore_msgbuf_counter = 0;
			intercore_recv_counter = 0;
			return 0;
		}
		else if (b == 0xf4) {
			// ack
			intercore_sent_counter = 0;
			intercore_waiting_ack = 0;
			return 0;
		}
		else {
			intercore_msgbuf[intercore_msgbuf_counter++] = b;
		}

		if (intercore_msgbuf_counter >= 4) {
			cmd->header = intercore_msgbuf[0];
			cmd->b1 = intercore_msgbuf[1];
			cmd->b2 = intercore_msgbuf[2];
			cmd->b3 = intercore_msgbuf[3];

			intercore_msgbuf_counter = 0;
			intercore_received_4();
			return 1;
		}
	}

	return 0;
}

int intercore_can_transmit(int numbytes, void* state)
{
	if (intercore_waiting_ack) {
		return 0;
	}

	return (COREUART_OUT_BUF_SIZE - COREUART_GetCharsInTxBuf()) >= numbytes;
}

void intercore_transmit(const midicmd_t cmd, void* state)
{
	COREUART_SendChar(cmd.header);
	COREUART_SendChar(cmd.b1);
	COREUART_SendChar(cmd.b2);
	COREUART_SendChar(cmd.b3);

	intercore_sent_4();
}
