#include <stdint.h>

uint16_t fletcher_checksum_running(uint16_t checksum, const uint8_t* block, int count)
{
	uint32_t checksum1 = checksum & 0xFF;
	uint32_t checksum2 = (checksum >> 8) & 0xFF;
	int i;

	for (i = 0; i < count; i++) {
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

uint16_t fletcher_checksum_uint32(uint16_t checksum, uint32_t value)
{
	uint8_t byte;

	byte = value & 0xFF;
	checksum = fletcher_checksum_running(checksum, &byte, 1);
	byte = (value >> 8) & 0xFF;
	checksum = fletcher_checksum_running(checksum, &byte, 1);
	byte = (value >> 16) & 0xFF;
	checksum = fletcher_checksum_running(checksum, &byte, 1);
	byte = (value >> 24) & 0xFF;
	checksum = fletcher_checksum_running(checksum, &byte, 1);

	return checksum;
}

uint16_t fletcher_checksum(const uint8_t* block, int count)
{
	return fletcher_checksum_running(0, block, count);
}

const uint8_t* fletcher_checksum_checkbytes(uint16_t checksum)
{
	static uint8_t checkbytes[2];
	
	int f0 = checksum & 0xff;
	int f1 = (checksum >> 8) & 0xff;
	int c0 = 0xff - ((f0 + f1) % 0xff);
	int c1 = 0xff - ((f0 + c0) % 0xff);

	checkbytes[0] = c0;
	checkbytes[1] = c1;

	return checkbytes;
}

