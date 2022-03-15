/*
 * battery.h
 *
 *  Created on: Mar 7, 2022
 *      Author: sdamkjar
 */

#ifndef INCLUDE_CUSTOM_BATTERY_H_
#define INCLUDE_CUSTOM_BATTERY_H_

#include "stdint.h"

typedef struct
{
    uint32_t maxV;                //mV   battery fully charged
    uint32_t maxI;                //mA   overcurrent limit
    uint8_t num;                   //# of battery
    uint8_t address;               //i2c address
    uint8_t temp_v;                //Voltage of thermistor. -20°C:10110111; 0°C:10011110; 100°C:1111
                                   //Thermistor output table used: https://www.arroyoinstruments.com/thermistors/

}battery_data;


#endif /* INCLUDE_CUSTOM_BATTERY_H_ */
