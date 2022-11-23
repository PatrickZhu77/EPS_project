#include "battery.h"
#include "channel.h"
#include "fee_function.h"
#include "ti_fee.h"
#include "sci.h"
#include "sys_common.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "uart_cmd.h"
#include "ina226.h"
#include "ina3221.h"

#ifndef INCLUDE_CUSTOM_CMD_INTERFACE_H_
#define INCLUDE_CUSTOM_CMD_INTERFACE_H_

void get_hk_bc(ina3221_housekeeping_t *data);
void get_hk_batt(ina226_housekeeping_t *data, battery_data_t *battery);
void get_hk_channel(ina226_housekeeping_t *data,channel_data_t *channel);
void set_default_max6698_init(uint8_t *ptr, uint8_t num, uint8_t value);

#endif /* INCLUDE_CUSTOM_CMD_INTERFACE_H_ */
