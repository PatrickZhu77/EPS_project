

#ifndef INCLUDE_CUSTOM_CUSTOM_CRC_H_
#define INCLUDE_CUSTOM_CUSTOM_CRC_H_

#include "stdint.h"

/******************************************************************************
 * CRC-32   x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 *
 * Poly:    0x4C11DB7 = 100110000010001110110110111b (ignore the first 1 of 1_0100_1100_0001_0001_1101_1011_0111b)
 * Init:    0xFFFFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFFFFF
 *****************************************************************************/


uint32_t crc32_calculate(uint8_t *data, uint16_t length);

#endif /* INCLUDE_CUSTOM_CUSTOM_CRC_H_ */
