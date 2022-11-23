
#ifndef INCLUDE_CUSTOM_BATTERY_H_
#define INCLUDE_CUSTOM_BATTERY_H_

#include "stdio.h"
#include "gio.h"
#include "het.h"

#include "ina226.h"
#include "mppt.h"
#include "max6698.h"
#include "flash_data.h"

#define     NUM_OF_BATTERY_PAIR   2          //There are 4 cells according to schematic of battery board, 1 and 3 are grouped, 2 and 4 are grouped.

#define     BATT_CHARGING_TEMP_MIN_C         10    //C. Minimum charging temp. according to Panasonic NCR18650B.
#define     BATT_CHARGING_TEMP_MAX_C         45    //C. Maximum charging temp. according to Panasonic NCR18650B.
#define     BATT_DISCHARGING_TEMP_MIN_C      -20   //C. Minimum discharging temp. according to Panasonic NCR18650B.
#define     BATT_DISCHARGING_TEMP_MAX_C      60    //C. Maximum discharging temp. according to Panasonic NCR18650B.

#define     BATT_CHARGING_CURRENT_LIMIT_mA       0x659  //Battery charging current limit. 0x659: 1625mA according to Panasonic NCR18650B.
#define     BATT_DISCHARGING_CURRENT_LIMIT_mA    0x659  //Battery discharging current limit. 0x659: 1625mA according to Panasonic NCR18650B.

#define     BATT_STATUS_CHARGING        1
#define     BATT_STATUS_DISCHARGING     0

typedef struct
{
    uint8_t num;                  //# of battery pair. Starting from 1.
    uint8_t sw[2];                //Switches. SW1 is charging switch and SW2 is discharging switch. 1:ON, 0:OFF.
    uint8_t status;               //Status of battery (depending on the direction of current). 1:charging, 0:discharging.
    uint16_t current;             //mA. Battery current.
    uint16_t voltage;             //mV. Battery voltage.
    int32_t temp;                     //Voltage of thermistor.
}battery_data_t;

/******************List of SW pins for battery*********************/
static gioPORT_t * BSW[4] = {                       //SW1 and SW3 are charging switches. SW2 and SW4 are discharging switches.
                                 gioPORTB,
                                 hetPORT1,
                                 gioPORTB,
                                 hetPORT1
                            };
static uint32_t BSW_num[4] = {
                                 1,
                                 12,
                                 0,
                                 30
                            };


void battery_chargingSW_on(battery_data_t *battery);
void battery_chargingSW_off(battery_data_t *battery);
void battery_dischargingSW_on(battery_data_t *battery);
void battery_dischargingSW_off(battery_data_t *battery);

void battery_check_charging_status(battery_data_t *battery, ina226_housekeeping_t *data);
void battery_read_rawdata_and_convert(battery_data_t *battery, ina226_housekeeping_t *data1, max6698_housekeeping_t *data2);

void battery_check_temp_then_SW(battery_data_t *battery, system_config_t *data);

void battery_check_overcurrent_then_change_MPP(battery_data_t *battery, system_config_t *data, mppt_data_t *data2);


#endif /* INCLUDE_CUSTOM_BATTERY_H_ */
