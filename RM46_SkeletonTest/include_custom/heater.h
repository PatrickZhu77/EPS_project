
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
#define HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW       0x3E8   //Solar panel minimum power threshold. 0x3E8: 1000 mW
#define HEATER_ORBIT_PERIOD_S                       0x1518  //Orbit period of the satellite. 0x1518: 5400s = 90min
#define HEATER_HEAT_UP_TIME_S                       0x78    //Time to heat up the battery from minimum discharging temp. to minimum charging temp. 0x78: 120s


#define HEATER_PROFILE_SWITCH_COUNTING      5       //A constant used to check if heater profile should switch

#define HEATER_ECLIPSE_TEMP_ON_C     -18    //Temp. threshold to turn on heater when in eclipse according to Panasonic NCR18650B.
#define HEATER_ECLIPSE_TEMP_OFF_C    -15    //Temp. threshold to turn off heater when in eclipse according to Panasonic NCR18650B.
#define HEATER_SUNSHINE_TEMP_ON_C    12     //Temp. threshold to turn on heater when in sunshine according to Panasonic NCR18650B.
#define HEATER_SUNSHINE_TEMP_OFF_C   15     //Temp. threshold to turn off heater when in sunshine according to Panasonic NCR18650B.

#define HEATER_SOLAR_PANEL_MINIMUM_POWER    0x3E8 //mW. Solar panel minimum power threshold. Satellite is in eclipse if power is smaller than this value. 0x3E8: 1000

#define NUM_OF_HEATER   2        //There are 2 heaters according to schematic of battery board

typedef struct
{
    uint8_t num;                   //# of heater. Starting from 1.
    uint8_t sw;                    //switch of heater. 0:off, 1:on
    uint8_t profile;               //The profile of the heater. 0: in eclipse, 1: in sunshine
    uint32_t time_of_first_light_per_orbit;       //sec. Time of fisrt light per orbit
    uint32_t time_light_last_seen;                //sec. Time that the light is seen for last time
    int32_t temp;                      //Voltage of thermistor
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
