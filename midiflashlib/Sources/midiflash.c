#include <string.h>

#include "slavectrl.h"

void SoftwareReset(void);

void FlashFailed(int warning);

void flash_write_page(uint8_t* buffer, uint32_t address, uint32_t blocksize)
{
	volatile int res =  0;
	if (res = WriteSlaveBlock(address, buffer, blocksize)) {
		FlashFailed(0);
	}
}

uint32_t flash_slave_blocksize()
{
	return GetSlaveBlockSize();
}

void flash_slave_reset_bootloader()
{
	SlaveToBootloader();
}

void flash_slave_reset_application()
{
	SlaveToProgram();
}

int slave_initialized = 0;
uint32_t slave_blocksize = 4096;
uint32_t curblock_base = 0xFFFFFFFF;
__attribute__((__aligned__(512))) uint8_t blockbuf[4096];

void flash_finish_page()
{
	if (curblock_base != 0xFFFFFFFF) {
		flash_write_page(blockbuf, curblock_base, slave_blocksize);
		curblock_base = 0xFFFFFFFF;
	}
}

void flash_64_bytes(uint8_t* block, uint32_t address)
{
	if (!slave_initialized) {
		flash_slave_reset_bootloader();
		slave_initialized = 1;
		slave_blocksize = flash_slave_blocksize();
	}

	if (address == 0x4ac0) {
		static volatile int x = 0;
		x++;
	}

	const uint32_t slave_blockmask = ~(slave_blocksize-1);

	if ((address & slave_blockmask) != curblock_base) {
		flash_finish_page();

		curblock_base = address & slave_blockmask;
		memset(blockbuf, 0xFF, slave_blocksize);
	}

	memcpy(&blockbuf[address & (~slave_blockmask)], block, 64);
}


const static uint32_t syxblocksize = 64;
const static uint32_t blockidstart = 64;
const static uint32_t addressstart = 66;
const static uint32_t checksumstart = 68;
const static uint32_t fullblocksize = 70;

int flash_expect_blockid = 0;

int block_index = 0;
uint8_t block[70];

uint16_t flash_fletcher_checksum(uint8_t* block, int count)
{
	uint32_t checksum1 = 0;
	uint32_t checksum2 = 0;

	for (int i = 0; i < count; i++) {
		checksum1 += block[i];
		if (checksum1 >= 255) {
			checksum1 -= 255;
		}
		checksum2 += checksum1;
		if (checksum2 >= 255) {
			checksum2 -= 255;
		}
	}

	return ((uint8_t)checksum2 << 8) | (uint8_t)checksum1;
}

int flash_7_bytes(uint8_t* buffer)
{
	for (int i = 0; i < 7; i++) {
		block[block_index + i] = buffer[i];
	}

	block_index += 7;

	if (block_index == fullblocksize) {
		block_index = 0;

		// verify checksum
		uint16_t checksum = flash_fletcher_checksum(block, fullblocksize);
		if (checksum != 0) {
			return 1;
		}

		// retrieve block id
		uint32_t blockid = (block[blockidstart] + (block[blockidstart + 1] << 8));
		if (blockid != flash_expect_blockid) {
			// didn't expect this block, fail
			return 1;
		}

		// retrieve write address
		uint32_t address = (block[addressstart] + (block[addressstart + 1] << 8)) << 6;

		// ready to flash
		flash_64_bytes(block, address);

		flash_expect_blockid++;
	}

	return 0;
}

int buf_7bit_index = 0;
uint8_t buf_7bit[7];

int flash_data_7bit(uint8_t b)
{
	if (buf_7bit_index < 7) {
		buf_7bit[buf_7bit_index] = b;
		buf_7bit_index++;
		return 0;
	}

	if (buf_7bit_index == 7) {
		for (int i = 0; i < 7; i++) {
			buf_7bit[i] |= ((b >> i) & 0x01) << 7;
		}
		buf_7bit_index = 0;
		return flash_7_bytes(buf_7bit);
	}

	// reached here -> fail
	return 1;
}

void flash_reset()
{
	if (slave_initialized) {
		flash_finish_page();
		flash_slave_reset_application();
		slave_initialized = 0;
		flash_expect_blockid = 0;
	}

	buf_7bit_index = 0;
	block_index = 0;
}

int sysex_ignore = 0;
int sysex_started = 0;
int sysex_flashing = 0;
int sysex_queue_used = 0;
uint8_t sysex_queue[4];

void sysex_stop()
{
	sysex_started = 0;
	sysex_queue_used = 0;
	sysex_flashing = 0;
	sysex_ignore = 0;
}

void sysex_start()
{
	if (sysex_started) {
		sysex_stop();
	}

	sysex_started = 1;
	sysex_queue_used = 0;
	sysex_flashing = 0;
	sysex_ignore = 0;
}

void sysex_start_request(uint8_t* requestheader)
{
	if (requestheader[0] == 0xF0
		&& requestheader[1] == 0x6F
		&& requestheader[2] == 0x6B
		&& requestheader[3] == 0x5F)
	{
		sysex_flashing = 1;
		return;
	}

	if (requestheader[0] == 0xF0
		&& requestheader[1] == 0x6F
		&& requestheader[2] == 0x6B
		&& requestheader[3] == 0x21)
	{
		flash_reset();
		SoftwareReset();
		return;
	}

	sysex_ignore = 1;
}

void sysex_queue_push(uint8_t b)
{
	if (!sysex_started || sysex_ignore) {
		return;
	}

	if (sysex_flashing) {
		if (b == 0xF7) {
			// end
			sysex_stop();
			return;
		}

		if (b < 0x80) {
			if (flash_data_7bit(b)) {
				// fail!
				flash_reset();
				sysex_stop();
			}
		}
		return;
	}

	if (sysex_queue_used < 4) {
		sysex_queue[sysex_queue_used++] = b;

		if (sysex_queue_used == 4) {
			sysex_start_request(sysex_queue);
		}
	}
}

int sysex_process_byte(uint8_t byte)
{
	if (byte == 0xF0) {
		sysex_start();
	}

	sysex_queue_push(byte);

	if (byte == 0xF7) {
		sysex_stop();
	}

	return sysex_flashing | slave_initialized;
}

