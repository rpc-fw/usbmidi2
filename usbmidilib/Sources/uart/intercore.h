#ifndef INTERCORE_H_
#define INTERCORE_H_

#include "../midicmd.h"

extern byte intercore_dma_data[];

void intercore_sync_master();
void intercore_sync_slave();

int intercore_receive(byte* b);

int intercore_can_transmit(int numbytes, void* state);
void intercore_transmit(const midicmd_t cmd, void* state);

#endif /* INTERCORE_H_ */
