
#ifndef INCLUDE_CUSTOM_FEE_FUNCTION_H_
#define INCLUDE_CUSTOM_FEE_FUNCTION_H_

#include "ti_fee.h"
#include "stdint.h"
#include "flash_data.h"
#include "custom_crc.h"
#include "data_structure_const.h"


void fee_initial(void);
void fee_write_8bit(uint16_t BlockNumber, uint8_t *pData);
void fee_write_16bit(uint16_t BlockNumber, uint16_t *pData, uint8_t Length);
void fee_read_8bit(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData, uint16_t numOfParameter);
void fee_read_16bit(uint16_t BlockNumber, uint16_t BlockOffset, uint16_t *pData, uint16_t numOfParameter);
void fee_read_32bit(uint16_t BlockNumber, uint16_t BlockOffset, uint32_t *pData, uint16_t numOfParameter);

uint8_t fee_read_8bit_and_check_crc(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData);
uint8_t fee_read_16bit_and_check_crc(uint16_t BlockNumber, uint16_t BlockOffset, uint16_t *pData);
uint8_t fee_read_32bit_and_check_crc(uint16_t BlockNumber, uint16_t BlockOffset, uint32_t *pData);

void fee_read_sensor_config(sensor_config_t *data);
uint8_t fee_read_flash_copy(uint8_t copy_version, system_config_t *data);

#endif /* INCLUDE_CUSTOM_FEE_FUNCTION_H_ */
