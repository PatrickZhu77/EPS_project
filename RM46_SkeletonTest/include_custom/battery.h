/*
 * battery.h
 *
 *  Created on: Mar 7, 2022
 *      Author: sdamkjar
 */

#ifndef INCLUDE_CUSTOM_BATTERY_H_
#define INCLUDE_CUSTOM_BATTERY_H_

#include "stdint.h"
#include "ina226.h"
#include "gio.h"
#include "het.h"
#include "mppt.h"

#define     NUM_OF_BATTERY   2          // 4 is default value according to schematic of battery board, 1 and 3 are grouped, 2 and 4 are grouped.


typedef struct
{
    uint8_t num;                   //# of battery
    uint8_t sw;
    uint8_t temp_v;                //Voltage of thermistor. -20C:10110111; 0C:10011110; 100C:00001111
    uint32_t maxV;                //mV   battery fully charged voltage
    uint32_t maxI;                //mA   overcurrent limit
                                   //10K Thermistor and 8.66K extended register
}battery_data;

static gioPORT_t * BSW[4] = {
                                 gioPORTB,
                                 hetPORT2,
                                 gioPORTB,
                                 hetPORT2
                            };
static uint32_t BSW_num[4] = {
                                 1,
                                 12,
                                 0,
                                 29
                            };

void battery_on(battery_data *battery);
void battery_off(battery_data *battery);
void battery_compareVI(ina226_data *data1, mppt_data *data2, battery_data *battery);
void battery_switch(battery_data *battery);


#endif /* INCLUDE_CUSTOM_BATTERY_H_ */
