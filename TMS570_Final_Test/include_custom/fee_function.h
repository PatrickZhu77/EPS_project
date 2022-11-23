
#ifndef INCLUDE_CUSTOM_FEE_FUNCTION_H_
#define INCLUDE_CUSTOM_FEE_FUNCTION_H_

#include "ti_fee.h"
#include "stdint.h"
#include "flash_data.h"
#include "custom_crc.h"
#include "data_structure_const.h"
#include "custom_crc.h"
#include "error_message.h"


#define     FACTORY_COPY_1              1
#define     FACTORY_COPY_2              2
#define     REBOOT_COPY                 3
#define     ERR_MSG_COPY                4
#define     ERR_MSG_TIMESTAMP_S_COPY    5
#define     ERR_MSG_TIMESTAMP_MS_COPY   6


void fee_initial(void);
void fee_read_flashed_data(uint16_t BlockNumber, uint8_t *pData, uint16_t length);

void fee_package_data_with_crc(system_config_t *source_data, uint8_t *packed_data);
uint8_t fee_check_crc_then_unpackage_data(uint8_t *packed_data, system_config_t *dest_data);
void fee_unpackage_data_ignore_crc(uint8_t *packed_data, system_config_t *dest_data);

void fee_package_err_msg_timestamp_s(uint32_t *source_data, uint8_t *packed_data);
void fee_package_err_msg_timestamp_ms(uint16_t *source_data, uint8_t *packed_data);
void fee_unpackage_err_msg_timestamp_s(uint8_t *packed_data, uint32_t *dest_data);
void fee_unpackage_err_msg_timestamp_ms(uint8_t *packed_data, uint16_t *dest_data);

#endif /* INCLUDE_CUSTOM_FEE_FUNCTION_H_ */
