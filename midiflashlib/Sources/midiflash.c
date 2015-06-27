#include <string.h>

#include "slavectrl.h"
#include "checksum.h"

void SoftwareReset(void);

void FlashFailed(int warning);

static void flash_write_page(uint8_t* buffer, uint32_t address, uint32_t blocksize)
{
	if (WriteSlaveBlock(address, buffer, blocksize)) {
		FlashFailed(0);
	}
}

static uint32_t flash_slave_blocksize()
{
	return GetSlaveBlockSize();
}

static void flash_slave_reset_bootloader()
{
	SlaveToBootloader();
}

static void flash_slave_reset_application()
{
	SlaveToProgram();
}

int slave_initialized = 0;
static uint32_t slave_blocksize = 1024;
static uint32_t curblock_base = 0xFFFFFFFF;
__attribute__((__aligned__(512))) static uint8_t blockbuf[1024];

static void flash_finish_page()
{
	if (curblock_base != 0xFFFFFFFF) {
		flash_write_page(blockbuf, curblock_base, slave_blocksize);
		curblock_base = 0xFFFFFFFF;
	}
}

static void flash_64_bytes(uint8_t* block, uint32_t address)
{
	if (!slave_initialized) {
		flash_slave_reset_bootloader();
		slave_initialized = 1;
		slave_blocksize = flash_slave_blocksize();
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

static int flash_expect_blockid = 0;

static int block_index = 0;
static uint8_t block[70];

static int flash_7_bytes(uint8_t* buffer)
{
	int i;
	memcpy(&block[block_index + i], buffer, 7);

	block_index += 7;

	if (block_index == fullblocksize) {
		block_index = 0;

		// verify checksum
		uint16_t checksum = fletcher_checksum(block, fullblocksize);
		if (checksum != 0) {
			return 1;
		}

		// retrieve block id
		uint32_t blockid = block[blockidstart] + (block[blockidstart + 1] << 8);
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

static int buf_7bit_index = 0;
static uint8_t buf_7bit[7];

static int flash_data_7bit(uint8_t b)
{
	if (buf_7bit_index < 7) {
		buf_7bit[buf_7bit_index] = b;
		buf_7bit_index++;
		return 0;
	}

	// else (buf_7bit_index == 7)

	for (int i = 0; i < 7; i++) {
		buf_7bit[i] |= ((b >> i) & 0x01) << 7;
	}
	buf_7bit_index = 0;

	return flash_7_bytes(buf_7bit);
}

static void flash_reset()
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

int sysex_started = 0; // 2 if ignore
int sysex_flashing = 0;
static int sysex_queue_used = 0;
static uint8_t sysex_queue[4];

static void sysex_stop_impl()
{
	sysex_started = 0;
	sysex_queue_used = 0;
	sysex_flashing = 0;
}

static void sysex_start_impl()
{
	//if (sysex_started) {
	//	sysex_stop();
	//}

	sysex_started = 1;
	sysex_queue_used = 0;
	sysex_flashing = 0;
}

void sysex_stop()
{
	sysex_stop_impl();
}

void sysex_start()
{
	sysex_start_impl();
}

static void sysex_start_request(uint8_t* requestheader)
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

	sysex_started = 2;
}

int sysex_queue_push(uint8_t b)
{
	if (sysex_started != 1) {
		return 0;
	}

	if (sysex_flashing) {
		if (b == 0xF7) {
			// end
			return 1;
		}

		if (b < 0x80) {
			if (flash_data_7bit(b)) {
				// fail!
				flash_reset();
				return 1;
			}
		}
		return 0;
	}

	if (sysex_queue_used < 4) {
		sysex_queue[sysex_queue_used++] = b;

		if (sysex_queue_used == 4) {
			sysex_start_request(sysex_queue);
		}
	}

	return 0;
}

int sysex_process_byte(uint8_t byte)
{
	if (byte == 0xF0) {
		sysex_start_impl();
	}

	int need_stop = sysex_queue_push(byte);

	if (byte == 0xF7 || need_stop) {
		sysex_stop_impl();
	}

	return sysex_flashing | slave_initialized;
}

