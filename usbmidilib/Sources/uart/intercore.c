#include "../usb/USBMIDI1.h"
#include "intercore.h"

#define INTERCORE_ACK_BYTE 0xF4
#define INTERCORE_SYNC_BYTE 0xF5

__attribute__((__aligned__(128))) byte intercore_dma_data[128];

uint32_t intercore_dma_readptr = (uint32_t)&intercore_dma_data[0];
byte intercore_dma_recv(byte *Chr)
{
	if (DMA_DAR(2) != intercore_dma_readptr) {
		*Chr = *(byte*)intercore_dma_readptr;
		intercore_dma_readptr = (uint32_t)&intercore_dma_data[0] | ((intercore_dma_readptr + 1) & 127);

		DMA_DSR_BCR(2) = 0xFFFF;
		return ERR_OK;
	}

	return ERR_RXEMPTY;
}

int intercore_dma_charsinrxbuf()
{
	return (DMA_DAR(2) - intercore_dma_readptr) & 127;
}

//byte COREUART_RecvChar(byte *Chr);
byte COREUART_SendChar(byte Chr);
//word COREUART_GetCharsInRxBuf(void);
word COREUART_GetCharsInTxBuf(void);
//byte COREUART_ClearRxBuf(void);
byte COREUART_ClearTxBuf(void);
#define COREUART_OUT_BUF_SIZE  0x0100U

static int intercore_sent_counter = 0;
static int intercore_recv_counter = 0;

void intercore_sync_master()
{
	byte b;

	// wait for slave to send sync
	COREUART_ClearRxBuf();
	COREUART_ClearTxBuf();

	while(1) {
		if (intercore_dma_charsinrxbuf() > 0) {
			byte b;
			if (intercore_dma_recv(&b) == ERR_OK) {
				if (b == INTERCORE_SYNC_BYTE) {
					// got sync, done
					break;
				}
			}
		}
	}

	// send sync
	COREUART_SendChar(INTERCORE_SYNC_BYTE);

	while(1) {
		if (intercore_dma_charsinrxbuf() == 0 && COREUART_GetCharsInTxBuf() == 0) {
			// queues are empty, send a new sync
			COREUART_SendChar(INTERCORE_SYNC_BYTE);
		}

		byte b;
		if (intercore_dma_recv(&b) == ERR_OK) {
			if (b == INTERCORE_ACK_BYTE) {
				// ready, done
				return;
			}
		}
	}
}

void intercore_sync_slave()
{
	byte b;

	// send sync
	COREUART_SendChar(INTERCORE_SYNC_BYTE);

	while(1) {
		if (intercore_dma_charsinrxbuf() == 0 && COREUART_GetCharsInTxBuf() == 0) {
			// queues are empty, send a new sync
			COREUART_SendChar(INTERCORE_SYNC_BYTE);
		}

		byte b;
		if (intercore_dma_recv(&b) == ERR_OK) {
			if (b == INTERCORE_SYNC_BYTE) {
				// got sync, done
				COREUART_SendChar(INTERCORE_ACK_BYTE);
				return;
			}
		}
	}
}

int intercore_receive(byte* bret)
{
	byte b;
	byte status;

	while ((status = intercore_dma_recv(&b)) == ERR_OK) {
		if (b == INTERCORE_SYNC_BYTE) {
			// sync
			intercore_recv_counter = 0;
			return ERR_FRAMING;
		}
		else if (b == INTERCORE_ACK_BYTE) {
			// ack
			intercore_sent_counter -= 16;
			if (intercore_sent_counter < 0) {
				intercore_sent_counter = 0;
			}
			continue;
		}
		else {
			*bret = b;

			intercore_recv_counter++;
			if (intercore_recv_counter >= 16) {
				COREUART_SendChar(INTERCORE_ACK_BYTE);
				intercore_recv_counter = 0;
			}

			return ERR_OK;
		}
	}

	return status;
}

int intercore_can_transmit(int numbytes, void* state)
{
	if (intercore_sent_counter >= 16) {
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

	intercore_sent_counter += 4;
}
