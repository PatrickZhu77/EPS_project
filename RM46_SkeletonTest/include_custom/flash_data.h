

#ifndef INCLUDE_CUSTOM_FLASH_DATA_H_
#define INCLUDE_CUSTOM_FLASH_DATA_H_

#include "data_structure_const.h"

typedef struct
{
    uint8_t priority;              //priority of channel
    uint16_t onlevel_mV;              //mV. battery voltage level to turn the channel on
    uint16_t offlevel_mV;             //mV. battery voltage level to turn the channel off
    uint16_t maxI_mA;                 //mA. overcurrent alert threshold
    uint16_t maxV_mV;                 //mV. overvoltage limit
    uint16_t minV_mV;                 //mV. undervoltage limit
    uint32_t group_mask;           //group mask channels. 1 bit for each channel. If grouped with a channel, that bit is set to 1
}channel_config_t;


typedef struct
{
    uint8_t batt_charging_temp_min_c;               //C. battery minimum charging temperature
    uint8_t batt_charging_temp_max_c;               //C. battery maximum charging temperature
    uint8_t batt_discharging_temp_min_c;            //C. battery minimum discharging temperature
    uint8_t batt_discharging_temp_max_c;            //C. battery maximum discharging temperature
    uint8_t heater_sunshine_temp_on_c;              //C. heater switch on temperature threshold in sunshine profile
    uint8_t heater_sunshine_temp_off_c;             //C. heater switch off temperature threshold in sunshine profile
    uint8_t heater_eclipse_temp_on_c;               //C. heater switch on temperature threshold in eclipse profile
    uint8_t heater_eclipse_temp_off_c;              //C. heater switch off temperature threshold in eclipse profile
    uint16_t overcurrent_protection_alert[NUM_OF_INA226_OVERCURRENT_PROTECTION];    //overcurrent alert for ina226 of overcurrent protection module
    uint16_t current_monitor_alert[NUM_OF_INA226_MONITOR];                          //overcurrent alert for ina226 of current monitor module
//    uint16_t current_sensor_shunt_resistance;       //mOhm. Shunt resistance for both ina226 and ina3221
    uint16_t overcurrent_protection_Rshunt[NUM_OF_INA226_OVERCURRENT_PROTECTION];         //mOhm. Shunt resistance for overcurrent module
    uint16_t current_monitor_Rshunt[NUM_OF_INA226_MONITOR];                //mOhm. Shunt resistance for current monitor module
    uint16_t battery_protection_Rshunt[NUM_OF_INA226_BATTERY];             //mOhm. Shunt resistance for battery protection module
    uint16_t channel_monitor_Rshunt[NUM_OF_INA226_CHANNEL];                //mOhm. Shunt resistance for channel monitor module
    uint16_t power_conversion_Rshunt[NUM_OF_INA3221];               //mOhm. Shunt resistance for boost converter
    uint16_t batt_charging_current_limit_mA;        //mA. battery charging current
    uint16_t batt_discharging_current_limit_mA;     //mA. battery discharging current
    uint16_t heater_tumble_threshold_time_s;        //sec. cubesate tumble threshold time
    uint16_t heater_solar_panel_threshold_power_mW;  //watt. solar panel minimum power threshold
    uint16_t heater_delay_time_s;                   //sec. the delay time since last time exiting eclipse to switch battery heater profile to the charge profile
    uint16_t dac_init;                              //initial output value of DACs
    uint16_t dac_stepsize_init;                     //initial stepsize of DACs
    channel_config_t chan_config_data[NUM_OF_CHANNELS];
}system_config_t;


typedef struct
{
    uint8_t software_version;           //# of software version (used to verify the version of eps board)
    uint8_t max6698_cfg1_setting;       //max6698 configuration register 1 init value
    uint8_t max6698_cfg2_setting;       //max6698 configuration register 2 init value
    uint8_t max6698_cfg3_setting;       //max6698 configuration register 3 init value
    uint16_t ina226_cfg_setting;        //ina226 configuration register init value
    uint16_t ina226_overcurrent_mask;   //ina226 of overcurrent protection module mask register init value
    uint16_t ina226_monitor_mask;       //ina226 of current monitor mask register init value
    uint16_t ina226_battery_mask;       //ina226 of battery protection module mask register init value
    uint16_t ina226_channel_mask;       //ina226 of channel protection module mask register init value
    uint16_t ina3221_cfg_setting;       //ina3221 configuration register init value
    uint16_t ina3221_mask_setting;      //ina3221 mask register init value
}sensor_config_t;


#endif /* INCLUDE_CUSTOM_FLASH_DATA_H_ */
