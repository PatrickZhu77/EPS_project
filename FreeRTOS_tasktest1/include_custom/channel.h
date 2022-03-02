/*
 * channel.h
 *
 *  Created on: Mar 2, 2022
 *      Author: sdamkjar
 */

#ifndef INCLUDE_CUSTOM_CHANNEL_H_
#define INCLUDE_CUSTOM_CHANNEL_H_

#include "stdint.h"

typedef struct
{
    uint32_t overV;                //mV   overvoltage limit
    uint32_t overI;                //mA   overcurrent limit
    uint8_t num;                   //# of channel
    uint8_t address;               //i2c address
    uint8_t en;                    //switch of the channel. 1:on, 0:off;

}channel_data;

const uint8_t channel_en[17];

#endif /* INCLUDE_CUSTOM_CHANNEL_H_ */
