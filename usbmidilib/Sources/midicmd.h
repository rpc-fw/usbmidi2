#ifndef MIDICMD_H_
#define MIDICMD_H_

#include "PE_Types.h"

typedef union _midicmd_t {
	struct {
		byte header;
		byte b1;
		byte b2;
		byte b3;
	};

	uint32_t word;
} midicmd_t;

#endif /* MIDICMD_H_ */
