

#ifndef INCLUDE_CUSTOM_CUSTOM_CRC_H_
#define INCLUDE_CUSTOM_CUSTOM_CRC_H_

#include "stdint.h"

/******************************************************************************
 * CRC-8               x8+x2+x+1
 * Poly:    0x07 = 0111b (ignore the first 1 of 1_0000_0111b)
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 *****************************************************************************/

uint8_t crc8_calculate(uint8_t data);
uint8_t crc8_check(uint8_t data, uint8_t crc);

#endif /* INCLUDE_CUSTOM_CUSTOM_CRC_H_ */
