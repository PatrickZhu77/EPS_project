#include "ti_fee.h"
#include "sci.h"
#include "sys_common.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "uart_cmd.h"
#include "ina226.h"
#include "ina3221.h"
#include "battery.h"
#include "channel.h"

#ifndef INCLUDE_CUSTOM_CMD_INTERFACE_H_
#define INCLUDE_CUSTOM_CMD_INTERFACE_H_

void get_hk_bc(ina3221_data *data);
void get_hk_batt(ina226_data *data, battery_data *battery);
void get_hk_channel(ina226_data *data,channel_data *channel);

#endif /* INCLUDE_CUSTOM_CMD_INTERFACE_H_ */
