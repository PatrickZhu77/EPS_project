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
#include "ina3221.h"
#include "ina226.h"

#define     NUM_OF_CHANNELS  18

#define     LOWEST_CH   0
#define     LOW_CH      1
#define     MID_CH      2
#define     HIGH_CH     3
#define     HIGHEST_CH  4

typedef struct
{
    uint8_t num;                   //# of channel
    uint8_t priority;              //priority of channel
    uint8_t sw;                    //switch of channel. 0:off, 1:on
    uint16_t group;                //group channels. The 16 bits represent channels.If grouped with a channel, that bit is set to 1
    uint32_t onlevel;              //mV   battery voltage level to turn the channel on
    uint32_t offlevel;             //mV   battery voltage level to turn the channel off
    uint32_t maxV;                 //mV   overvoltage limit
    uint32_t minV;                 //mV   undervoltage limit

}channel_data;


/******************List of EN pins for channels*********************/
static gioPORT_t * CH[18] = {
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
static uint8_t CH_num[18] = {
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

void channel_on(channel_data *Fchannel, channel_data *channel);
void channel_off(channel_data *Fchannel, channel_data *channel);
void channel_set_group(channel_data *Fchannel, channel_data *channel1, channel_data *channel2);
void channel_check_charge_level(ina3221_data *data, channel_data *Fchannel);
void channel_check_trip(ina226_data *data, channel_data *Fchannel);

#endif /* INCLUDE_CUSTOM_CHANNEL_H_ */
