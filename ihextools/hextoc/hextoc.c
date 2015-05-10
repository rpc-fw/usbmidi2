#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "readhex.h"
#include "checksum.h"

void writec(FILE* f, const char* varname, uint8_t* buf, uint32_t size, uint32_t baseaddr)
{
	int i, k;

	fprintf(f, "const uint8_t %s[%d] = {\n", varname, size);

	for (i = 0; i < size; i += 16) {
		if (baseaddr + size < 0x10000) {
			fprintf(f, "/* 0x%04x */\t", baseaddr + i);
		}
		else {
			fprintf(f, "/* 0x%08x */\t", baseaddr + i);
		}
		for (k = 0; k < 16 && k+i < size; k++) {
			fprintf(f, "%s0x%02x", k > 0 ? ", " : "", buf[i+k]);
		}
		fprintf(f, "%s\n", k+i < size ? "," : "");
	}
	fprintf(f, "};\n");
}

void writeid(FILE* f, const char* varname, const char* dataname, uint8_t* buf, uint32_t size)
{
	uint16_t checksum = 0;

	int c;
	for (c = 0; c < size; c += 16) {
		int s = size - c;
		if (s > 16) s = 16;
		checksum = fletcher_checksum_running(checksum, &buf[c], s);
	}
	checksum = fletcher_checksum_uint32(checksum, size);
	const uint8_t* checkbytes = fletcher_checksum_checkbytes(checksum);
	uint16_t hash = checkbytes[0] + (checkbytes[1] << 8);
	
	if (fletcher_checksum_running(checksum, checkbytes, 2) != 0) {
		fprintf(stderr, "Checksum error\n");
		exit(1);
	}

	fprintf(f, "\n");
	fprintf(f, "\n");
	fprintf(f, "const struct buildid_t %s = {\n", varname);
	fprintf(f, "/* .id = */\t\t%u,\n", /*hash*/ 0x55aa5a5f);
	fprintf(f, "/* .payload_hash = */\t%u,\n", hash);
	fprintf(f, "/* .payload_start = */\t&%s[0],\n", dataname);
	fprintf(f, "/* .payload_size = */\tsizeof(%s)\n", dataname);
	fprintf(f, "};\n");
}

int main(int argc, char** argv)
{
	uint32_t size = 0;
	uint32_t baseaddr = 0x4000;
	const char* filename;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <.hex file>\n", argv[0]);
		exit(1);
	}

	filename = argv[1];

	uint8_t* buf = readhex(&size, baseaddr, filename);

	writec(stdout, "hex_data", buf, size, baseaddr);

	writeid(stdout, "__buildid", "hex_data", buf, size);

	return 0;
}

