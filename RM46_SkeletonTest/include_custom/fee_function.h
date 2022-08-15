
#ifndef INCLUDE_CUSTOM_FEE_FUNCTION_H_
#define INCLUDE_CUSTOM_FEE_FUNCTION_H_

#include "ti_fee.h"
#include "stdint.h"
#include "flash_data.h"
#include "custom_crc.h"


void fee_initial(void);
void fee_write_8bit(uint16_t BlockNumber, uint8_t *pData);
void fee_write_16bit(uint16_t BlockNumber, uint16_t *pData, uint8_t Length);
void fee_read_8bit(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData, uint16_t Length);
void fee_read_16bit(uint16_t BlockNumber, uint16_t BlockOffset, uint16_t *pData, uint16_t Length);


#endif /* INCLUDE_CUSTOM_FEE_FUNCTION_H_ */
