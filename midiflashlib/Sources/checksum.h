#pragma once

#include <stdint.h>

uint16_t fletcher_checksum_running(uint16_t checksum, const uint8_t* block, int count);

uint16_t fletcher_checksum_uint32(uint16_t checksum, uint32_t value);

uint16_t fletcher_checksum(const uint8_t* block, int count);

const uint8_t* fletcher_checksum_checkbytes(uint16_t checksum);

