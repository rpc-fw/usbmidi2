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

void writeid(FILE* f, const char* varname, const char* dataname, uint8_t* buf, uint32_t size, const char* idstring)
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
	if (idstring != NULL) {
		fprintf(f, "/* .id = */\t\t%s,\n", idstring);
	}
	else {
		fprintf(f, "/* .id = */\t\t%u,\n", hash);
	}
	fprintf(f, "/* .payload_hash = */\t%u,\n", hash);
	fprintf(f, "/* .payload_start = */\t&%s[0],\n", dataname);
	fprintf(f, "/* .payload_size = */\tsizeof(%s)\n", dataname);
	fprintf(f, "};\n");
}

void usage(const char* argv0)
{
	fprintf(stderr, "Usage: %s <.hex file> [-i id]\n", argv0);
	exit(1);
}

const char* filename;
const char* id;
void default_args()
{
	filename = NULL;
	id = NULL;
}

void parse_args(int argc, char** argv)
{
	int i;
	int ignoreflags = 0;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '\0') {
			continue;
		}

		int isflag = ((argv[i])[0] == '-');
		char flag = (argv[i])[1];

		if (!ignoreflags && isflag) {
			if (flag == '-') {
				ignoreflags = 1;
			}
			else if (flag == 'i') {
				if (id != NULL) {
					usage(argv[0]);
				}
				id = argv[i+1];
				i++;
			}
			else {
				usage(argv[0]);
			}
		}
		else {
			if (filename != NULL) {
				usage(argv[0]);
			}
			filename = argv[i];
		}
	}
}

void validate_args(int argc, char** argv)
{
	if (filename == NULL) {
		usage(argv[0]);
	}
}

int main(int argc, char** argv)
{
	uint32_t size = 0;
	uint32_t baseaddr = 0x4000;

	default_args();
	parse_args(argc, argv);
	validate_args(argc, argv);

	filename = argv[1];

	uint8_t* buf = readhex(&size, baseaddr, filename);

	writec(stdout, "hex_data", buf, size, baseaddr);

	writeid(stdout, "__buildid", "hex_data", buf, size, id);

	return 0;
}

