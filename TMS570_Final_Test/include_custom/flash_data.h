

#ifndef INCLUDE_CUSTOM_FLASH_DATA_H_
#define INCLUDE_CUSTOM_FLASH_DATA_H_

#include "data_structure_const.h"
#include "flash_data.h"

#define     FACTORY_COPY    1
#define     CONFIG_COPY     2

/*******************************************************
 * List of blocks in EEPROM
 *
 * Block 1 - Sensor configuration 8 bits data
 * Block 2 - Sensor configuration 16 bits data
 * Block 3 - Factory copy 8 bits data
 * Block 4 - Factory copy 16 bits data
 * Block 5 - Factory copy of channel configuration 8 bits data
 * Block 6 - Factory copy of channel configuration 16 bits data
 * Block 7 - Factory copy of channel configuration 32 bits data
 * Block 8 - Flashed copy 8 bits data and its crc code (including channel configuration)
 * Block 9 - Flashed copy 16 bits data and its crc code (including channel configuration)
 * Block 10 - Flashed copy of channel configuration 8 bits data and its crc code (including channel configuration)
 * Block 11 - Flashed copy of channel configuration 16 bits data and its crc code (including channel configuration)
 * Block 12 - Flashed copy of channel configuration 32 bits data and its crc code (including channel configuration)
 *
 * *****************************************************/
#define     SENSOR_CONFIG_8BIT_BLOCK_NUM            1
#define     SENSOR_CONFIG_16BIT_BLOCK_NUM           2
#define     FACTORY_COPY_8BIT_BLOCK_NUM             3
#define     FACTORY_COPY_16BIT_BLOCK_NUM            4
#define     CHANNEL_FACTORY_COPY_8BIT_BLOCK_NUM     5
#define     CHANNEL_FACTORY_COPY_16BIT_BLOCK_NUM    6
#define     CHANNEL_FACTORY_COPY_32BIT_BLOCK_NUM    7
#define     FLASHED_COPY_8BIT_BLOCK_NUM             8
#define     FLASHED_COPY_16BIT_BLOCK_NUM            9
#define     CHANNEL_FLASHED_COPY_8BIT_BLOCK_NUM     10
#define     CHANNEL_FLASHED_COPY_16BIT_BLOCK_NUM    11
#define     CHANNEL_FLASHED_COPY_32BIT_BLOCK_NUM    12

#define     FLASHED_DATA_LENGTH     454     //in bytes. 450 bytes of data + 4 bytes of CRC checksum


typedef struct
{
    uint8_t priority;              //priority of channel
    uint16_t onlevel_mV;              //mV. battery voltage level to turn the channel on
    uint16_t offlevel_mV;             //mV. battery voltage level to turn the channel off
    uint16_t maxI_mA;                 //mA. overcurrent alert threshold
    uint16_t maxV_mV;                 //mV. overvoltage limit
    uint16_t minV_mV;                 //mV. undervoltage limit
    uint16_t maxI_increment_mA;       //mA. increment of overcurrent threshold
    uint32_t group_mask;           //group mask channels. 1 bit for each channel. If grouped with a channel, that bit is set to 1
}channel_config_t;



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
//    uint16_t ina226_bc_mask;            //ina226 of boost converter monitor module mask register init value
    uint16_t ina3221_cfg_setting;       //ina3221 configuration register init value
    uint16_t ina3221_mask_setting;      //ina3221 mask register init value
}sensor_config_t;


/*This data structure must be 32 bit aligned in order to fit the 32 bit crc*/
typedef struct
{
    uint16_t configuration_version;            //The version number of the configuration
    uint16_t overcurrent_protection_alert_mA[NUM_OF_INA226_OVERCURRENT_PROTECTION];    //mA. overcurrent alert for ina226 of overcurrent protection module
    uint16_t current_monitor_alert_mA[NUM_OF_INA226_MONITOR];                          //mA. overcurrent alert for ina226 of current monitor module
    uint16_t overcurrent_protection_Rshunt[NUM_OF_INA226_OVERCURRENT_PROTECTION];      //mOhm. Shunt resistance for overcurrent module
    uint16_t current_monitor_Rshunt[NUM_OF_INA226_MONITOR];                //mOhm. Shunt resistance for current monitor module
    uint16_t battery_protection_Rshunt[NUM_OF_INA226_BATTERY];             //mOhm. Shunt resistance for battery protection module
    uint16_t channel_monitor_Rshunt[NUM_OF_INA226_CHANNEL];                //mOhm. Shunt resistance for channel monitor module
    uint16_t power_conversion_Rshunt[NUM_OF_INA226_BOOSTCONVERTER];        //mOhm. Shunt resistance for boost converter
    uint16_t batt_charging_current_limit_mA;        //mA. battery charging current
    uint16_t batt_discharging_current_limit_mA;     //mA. battery discharging current
    uint16_t heater_tumble_threshold_time_s;        //sec. cubesate tumble threshold time
    uint16_t heater_solar_panel_threshold_power_mW;  //watt. solar panel minimum power threshold
    uint16_t heater_orbit_period_s;                 //sec. time that the satellite runs for one period//sec. the delay time since last time exiting eclipse to switch battery heater profile to the charge profile
    uint16_t heater_battery_heat_up_time_s;         //sec. the time to heat up battery to proper temperature for charging
    uint16_t dac_init;                              //initial output value of DACs
    uint16_t dac_stepsize_init;                     //initial stepsize of DACs
    uint16_t obc_timeout_s;                         //sec. timeout period of obc command petting the eps
    uint32_t gs_timeout_s;                          //sec. timeout period of ground station command petting the eps
    int32_t batt_charging_temp_min_c;               //C. battery minimum charging temperature (2's complement format)
    int32_t batt_charging_temp_max_c;               //C. battery maximum charging temperature (2's complement format)
    int32_t batt_discharging_temp_min_c;            //C. battery minimum discharging temperature (2's complement format)
    int32_t batt_discharging_temp_max_c;            //C. battery maximum discharging temperature (2's complement format)
    int32_t heater_sunshine_temp_on_c;              //C. heater switch on temperature threshold in sunshine profile (2's complement format)
    int32_t heater_sunshine_temp_off_c;             //C. heater switch off temperature threshold in sunshine profile (2's complement format)
    int32_t heater_eclipse_temp_on_c;               //C. heater switch on temperature threshold in eclipse profile (2's complement format)
    int32_t heater_eclipse_temp_off_c;              //C. heater switch off temperature threshold in eclipse profile (2's complement format)
    sensor_config_t sensor_config_data;
    channel_config_t chan_config_data[NUM_OF_CHANNELS];
}system_config_t;



#endif /* INCLUDE_CUSTOM_FLASH_DATA_H_ */
