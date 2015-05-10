#ifndef SLAVECTRL_H
#define SLAVECTRL_H

#include "PE_Types.h"
#include "stddef.h"

void SlaveResetMode(int mode);

int IsSlaveReady();

byte ReadCoreUartByte();

void SlaveHello();

void SlaveToBootloader();

void SlaveToProgram();

void SlaveRequest(void* req, size_t reqsize, void* res, size_t ressize);

uint32_t GetSlaveBlockSize();

int WriteSlaveBlock(uint32_t address, void* data, int datasize);

#endif /* SLAVECTRL_H */
