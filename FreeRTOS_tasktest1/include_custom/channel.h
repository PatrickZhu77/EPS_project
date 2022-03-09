/*
 * channel.h
 *
 *  Created on: Mar 2, 2022
 *      Author: sdamkjar
 */

#ifndef INCLUDE_CUSTOM_CHANNEL_H_
#define INCLUDE_CUSTOM_CHANNEL_H_

#include "stdint.h"
#include "gio.h"
#include "het.h"
#include "ina226.h"

typedef struct
{
    uint32_t overV;                //mV   overvoltage limit
    uint32_t overI;                //mA   overcurrent limit
    uint8_t num;                   //# of channel
    uint8_t address;               //i2c address

}channel_data;


/******************List of EN ports for channels*********************/
const gioPORT_t * CH[18] = {
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 hetPORT2,
                                 gioPORTA,
                                 gioPORTA,
                                 gioPORTA,
                                 gioPORTA,
                                 gioPORTA,
                                 hetPORT2,
                                 gioPORTB,
                                 gioPORTB
                            };
const uint32_t CH_num[18] = {
                                 30,
                                 0,
                                 7,
                                 13,
                                 15,
                                 24,
                                 3,
                                 2,
                                 1,
                                 5,
                                 2,
                                 7,
                                 1,
                                 5,
                                 0,
                                 11,
                                 2,
                                 3
                            };

const uint8_t channel_en[18];

void channel_switch(ina226_data *data, channel_data *channel);
void channel_control(channel_data *channel);

#endif /* INCLUDE_CUSTOM_CHANNEL_H_ */
