#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "readhex.h"
#include "checksum.h"

void write7bit(const uint8_t* block, uint32_t size, FILE* f)
{
	uint8_t bytes[8];
	int i, k;

	for (i = 0; i < size; i += 7) {
		for (k = 0; k < 7; k++) {
			bytes[k] = block[i+k] & 0x7f;
		}
		bytes[7] = 0;
		for (k = 0; k < 7; k++) {
			bytes[7] |= ((block[i+k] & 0x80) >> 7) << k;
		}

		fwrite(bytes, 8, 1, f);
	}
}

const static int syxblocksize = 64;

void writeblockheader(FILE* f)
{
	fputc(0x6f, f);
	fputc(0x6b, f);
	fputc(0x5f, f);
}

void writefinalheader(FILE* f)
{
	fputc(0x6f, f);
	fputc(0x6b, f);
	fputc(0x21, f);
}

void writesyx(FILE* f, const uint8_t* buf, uint32_t size, uint32_t baseaddr)
{
	int i;

	const uint32_t blockidstart = syxblocksize;
	const uint32_t addressstart = blockidstart + 2;
	const uint32_t checksumstart = addressstart + 2;
	const uint32_t fullblocksize = checksumstart + 2;

	uint8_t block[fullblocksize];
	uint16_t blockid = 0;

	for (i = 0; i < size; i += syxblocksize) {
		fputc(0xf0, f);

		writeblockheader(f);

		int datasize = size - i;
		if (datasize > syxblocksize) {
			memcpy(block, &buf[i], syxblocksize);
		}
		else {
			memcpy(block, &buf[i], datasize);
			memset(&block[datasize], 0xFF, syxblocksize-datasize);
		}

		// index
		block[blockidstart] = blockid & 0xFF;
		block[blockidstart + 1] = (blockid >> 8) & 0xFF;

		// size
		uint32_t address = (baseaddr + i) >> 6;
		block[addressstart] = address & 0xFF;
		block[addressstart + 1] = (address >> 8) & 0xFF;

		uint16_t checksum = fletcher_checksum(block, checksumstart);
		memcpy(&block[checksumstart], fletcher_checksum_checkbytes(checksum), 2);

		checksum = fletcher_checksum(block, fullblocksize);
		if (checksum != 0) {
			printf("Checksum error\n");
			exit(1);
		}

		write7bit(block, fullblocksize, f);
		
		fputc(0xf7, f);

		blockid++;
	}

	fputc(0xf0, f);
	writefinalheader(f);
	fputc(0xf7, f);
}

int main(int argc, char** argv)
{
	uint32_t size = 0;
	uint32_t baseaddr = 0x4000;
	const char* filename;
	char outfilename[PATH_MAX];

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <.hex file>\n", argv[0]);
		exit(1);
	}

	filename = argv[1];

	strncpy(outfilename, filename, sizeof(outfilename));
	outfilename[sizeof(outfilename)-1] = '\0';
	strncat(outfilename, ".syx", sizeof(outfilename));
	outfilename[sizeof(outfilename)-1] = '\0';

	uint8_t* buf = readhex(&size, baseaddr, filename);
	FILE* fout = fopen(outfilename, "wb");
	if (fout == NULL) {
		fprintf(stderr, "%s: %s\n", outfilename, strerror(errno));
		exit(1);
	}
	writesyx(fout, buf, size, baseaddr);
	fclose(fout);

	return 0;
}

