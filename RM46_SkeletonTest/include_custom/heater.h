
#ifndef INCLUDE_CUSTOM_HEATER_H_
#define INCLUDE_CUSTOM_HEATER_H_

#include "stdint.h"
#include "gio.h"
#include "het.h"

#include "max6698.h"


typedef struct
{
    uint8_t num;                   //# of heater
    uint8_t sw;                    //switch of heater. 0:off, 1:on
    uint8_t maxT;                  //Minimum temperature(Voltage of thermistor) -20C:10110111; 0C:10011110
    uint8_t minT;                  //Minimum temperature(Voltage of thermistor) -20C:10110111; 0C:10011110
}heater_data;

static gioPORT_t * HEAT[2] = {
                                 hetPORT2,
                                 hetPORT2
                            };
static uint8_t HEAT_num[2] = {
                                 10,
                                 17
                            };


void heater_on(heater_data *heater);
void heater_off(heater_data *heater);

#endif /* INCLUDE_CUSTOM_HEATER_H_ */
