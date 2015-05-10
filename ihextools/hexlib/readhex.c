#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <errno.h>

#include "readhex.h"

static int parsebyte(uint8_t* result, const char* text)
{
	char tmp[3];
	char* endptr;

	tmp[0] = text[0];
	if (tmp[0] == '\0') {
		return 1;
	}
	tmp[1] = text[1];
	if (tmp[1] == '\0') {
		return 1;
	}
	tmp[2] = '\0';

	*result = strtol(tmp, &endptr, 16);
	if (endptr != &tmp[2]) {
		return 1;
	}

	return 0;
}

static int parseword(uint16_t* result, const char* text)
{
	char tmp[5];
	char* endptr;
	int i;

	for (i = 0; i < 4; i++) {
		tmp[i] = text[i];
		if (tmp[i] == '\0') {
			return 1;
		}
	}
	tmp[4] = '\0';

	*result = strtol(tmp, &endptr, 16);
	if (endptr != &tmp[4]) {
		return 1;
	}

	return 0;
}

static int parsehex(int* size, uint32_t* address, int* cmd, uint8_t* data, const char* line)
{
	uint8_t value8;
	uint16_t value16;
	int i;
	const char* p = &line[9];
	uint8_t checksum = 0;

	if (line[0] != ':') {
		return 1;
	}

	if (parsebyte(&value8, &line[1])) {
		return 1;
	}
	*size = value8;
	checksum += value8;
	
	if (parseword(&value16, &line[3])) {
		return 1;
	}
	*address = value16;
	checksum += value16 & 0xFF;
	checksum += value16 >> 8;
	
	if (parsebyte(&value8, &line[7])) {
		return 1;
	}
	*cmd = value8;
	checksum += value8;

	for (i = 0; i < *size; i++) {
		if (parsebyte(&data[i], p)) {
			return 1;
		}
		checksum += data[i];
		p += 2;
	}

	/* two's complement for checksum calculation */
	checksum = (~checksum) + 1;
	
	/* checksum */
	if (parsebyte(&value8, p)) {
		return 1;
	}
	p += 2;

	if (checksum != value8) {
		return 1;
	}

	if ((*p != '\0') && (*p != '\r') && (*p != '\n')) {
		return 1;
	}

	return 0;
}

uint8_t* readhex(uint32_t* size, uint32_t baseaddr, const char* filename)
{
	uint8_t* buf = NULL;
	uint32_t datasize = 0;
	FILE* f = fopen(filename, "rb");
	uint32_t hexbase = 0;

	if (f == NULL) {
		fprintf(stderr, "%s: %s\n", filename, strerror(errno));
		exit(1);
	}

	while(1) {
		char line[256];

		char* p = fgets(line, sizeof(line), f);
		if (feof(f) || p == NULL) {
			break;
		}
		line[255] = '\0';

		int size;
		uint32_t address;
		int cmd;
		uint8_t data[16];
		if (parsehex(&size, &address, &cmd, data, line)) {
			fprintf(stderr, "%s: parse error\n", filename);
			fputs(line, stderr);
			exit(1);
		}

		if (cmd == 0x00) {
			address = address + hexbase;
			if (address < baseaddr) {
				fprintf(stderr, "Ignoring data at address 0x%x\n", address);
				continue;
			}

			uint32_t bufaddr = address - baseaddr;
			if (bufaddr + size > datasize) {
				uint32_t newsize = bufaddr + size;
				buf = realloc(buf, newsize);
				memset(&buf[datasize], 0xFF, newsize - datasize);
				datasize = bufaddr + size;
			}

			memcpy(&buf[bufaddr], data, size);
		}
		
		else if (cmd == 0x01) {
			// quietly consume command 0x01
		}
		else if (cmd == 0x02) {
			if (size != 2) {
				fprintf(stderr, "Ignoring command 02 with unsupported size %d\n", datasize);
			}
			hexbase = ((data[0] << 8) + data[1]) * 16;
		}
		
		else if (cmd == 0x03) {
			// quietly consume command 0x03
		}
		else {
			fprintf(stderr, "Ignoring unsupported command %02x\n", cmd);
			continue;
		}
	}

	fclose(f);

	*size = datasize;
	return buf;
}

