
#ifndef INCLUDE_CUSTOM_ERROR_MESSAGE_H_
#define INCLUDE_CUSTOM_ERROR_MESSAGE_H_


#include "stdint.h"

#define ERROR_LENGTH_OF_BUFFER    100


#define ERROR_BAD_CRC                 1
#define ERROR_TASK_NOT_CREATED        2
#define ERROR_TASK_NOT_ACTIVE         3
#define ERROR_NO_CMD_PETTING          4
#define ERROR_SENSOR_OVERCURRENT      5

#define OBC_TIMEOUT         1
#define GS_TIMEOUT          2



#define ERROR_BUFFER_SIZE       100


void error_buffer_init(uint8_t *err_buffer);
void error_log_the_data(uint8_t *err_buffer, uint8_t err_type, uint8_t err_data);
void error_log_the_timestamp(uint8_t *err_buffer, uint32_t *timestamp_s_buffer, uint16_t *timestamp_ms_buffer, uint32_t timestamp_s, uint16_t timestamp_ms);


#endif /* INCLUDE_CUSTOM_ERROR_MESSAGE_H_ */
