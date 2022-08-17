
#ifndef INCLUDE_CUSTOM_HEATER_H_
#define INCLUDE_CUSTOM_HEATER_H_

#include "fee_function.h"
#include "stdint.h"
#include "gio.h"
#include "het.h"

#include "max6698.h"
#include "ina3221.h"

#define HEATER_TUMBLE_THRESHOLD_TIME_S              0x12C   //Cubesate tumble threshold time. 0x12C: 300 sec
                                                            //If solar cell get power for this period of time, we can regard it in sunshine.
#define HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW        0x3E8   //Solar panel minimum power threshold. 0x3E8: 1000 mW
#define HEATER_DELAY_TIME_S                         0x12C   //the delay time since last time exiting eclipse to switch battery heater
                                                            //profile to the charge profile.  0x12C: 300 sec

#define HEATER_PROFILE_SWITCH_COUNTING      5       //A constant used to check if heater profile should switch

#define HEATER_ECLIPSE_TEMP_ON_C     0xB7   //Temp. threshold to turn on heater when in eclipse.
                                            //0xB7: -20C according to MAX6698.
#define HEATER_ECLIPSE_TEMP_OFF_C    0xB7   //Temp. threshold to turn off heater when in eclipse.
                                            //0xB7: -20C according to MAX6698.
#define HEATER_SUNSHINE_TEMP_ON_C    0x9E   //Temp. threshold to turn on heater when in sunshine.
                                            //0x9E: 0C according to MAX6698.
#define HEATER_SUNSHINE_TEMP_OFF_C   0x9E   //Temp. threshold to turn off heater when in sunshine.
                                            //0x9E: 0C according to MAX6698.

#define NUM_OF_HEATER   2        //There are 2 heaters according to schematic of battery board

typedef struct
{
    uint8_t num;                   //# of heater. Starting from 1.
    uint8_t sw;                    //switch of heater. 0:off, 1:on
    uint8_t temp;                  //Voltage of thermistor
    uint8_t profile;               //The profile of the heater. 0: in eclipse, 1: in sunshine
    uint8_t profile_counter;          //A counter used by profile updating function
    uint32_t init_time;            //sec. Initial time of profile judgment. Used by profile updating function
}heater_data_t;

static gioPORT_t * HEAT[2] = {
                                 hetPORT2,
                                 hetPORT2
                             };

static uint8_t HEAT_num[2] = {
                                 10,
                                 17
                             };


void heater_on(heater_data_t *heater);
void heater_off(heater_data_t *heater);

void heater_read_rawdata_and_convert(heater_data_t *heater, max6698_housekeeping_t *data);
void heater_temp_SW(heater_data_t *heater, system_config_t *data);

void heater_update_profile(heater_data_t *heater, system_config_t *data, ina3221_housekeeping_t *data2, uint32_t curr_time);


#endif /* INCLUDE_CUSTOM_HEATER_H_ */
