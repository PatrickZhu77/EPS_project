/*
 * status.h
 *
 *  Created on: Sep 08, 2022
 *      Author: zqqqq
 */

#ifndef INCLUDE_CUSTOM_STATUS_H_
#define INCLUDE_CUSTOM_STATUS_H_

#include "data_structure_const.h"

typedef struct
{
    uint8_t config_ver;                                     //version of the current configuration RAM copy
    uint16_t solar_panel_input_power_mW[NUM_OF_INA3221];    //mW. input power of solar panels
    uint16_t battery_voltage_mV[NUM_OF_INA226_BATTERY];     //mW. voltage of battery pairs
    uint16_t num_of_ON_channels;                            //number of the output channels that is ON
    uint32_t runtime_s;                                     //sec. system runtime
    uint32_t last_time_update_config_to_flash_s;            //sec. last time user update the configuration RAM copy to flash (either to reboot copy or to factory copy)
    uint32_t last_time_user_request_HK_s;                   //sec. last time user request housekeeping data
}system_status_t;


#endif /* INCLUDE_CUSTOM_STATUS_H_ */
