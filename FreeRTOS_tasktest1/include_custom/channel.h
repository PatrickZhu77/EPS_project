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
    uint32_t maxV;                //mV   overvoltage limit
    uint32_t maxI;                //mA   overcurrent limit
    uint8_t num;                   //# of channel
    uint8_t address;               //i2c address
    uint8_t sw;                    //switch of channel. 0:off, 1:on

}channel_data;


/******************List of EN ports for channels*********************/
static gioPORT_t * CH[16] = {
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
                                 hetPORT2
                            };
static uint32_t CH_num[16] = {
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
                                 11
                            };

void channel_on(channel_data *channel);
void channel_off(channel_data *channel);
void channel_compare(ina226_data *data, channel_data *channel);
void channel_switch(channel_data *channel);

#endif /* INCLUDE_CUSTOM_CHANNEL_H_ */
