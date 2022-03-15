/*
 * channel.c
 *
 *  Created on: Mar 2, 2022
 *      Author: sdamkjar
 */

#include "channel.h"
#include "ina226.h"
#include "stdio.h"

/***************************************************************************
 * @brief
 *   Switch the channel on
 *
 * @param[in] channel
 *   Pointer to channel data.
 *
 ******************************************************************************/
void channel_on(channel_data *channel)
{
    channel->sw = 1;
}

/***************************************************************************
 * @brief
 *   Switch the channel off
 *
 * @param[in] channel
 *   Pointer to channel data.
 *
 ******************************************************************************/
void channel_off(channel_data *channel)
{
    channel->sw = 0;
}


/***************************************************************************
 * @brief
 *   Decide when to turn channel off
 *
 *  @param[in] data
 *   Pointer to public data structure of current sensor.
 *
 * @param[in] channel
 *   Pointer to channel data.
 *
 ******************************************************************************/
void channel_compare(ina226_data *data, channel_data *channel)
{
    if(data->bus_voltage > channel->maxV)               //when channel overvoltage
    {
        channel_off(channel);
    }
    else if(data->current > channel->maxI)
    {
        channel_off(channel);
    }
}

/***************************************************************************
 * @brief
 *   Execute the switch
 *
 * @param[in] channel
 *   Pointer to channel data.
 *
 ******************************************************************************/
void channel_switch(channel_data *channel)
{
    if(channel->sw == 1)
    {
        gioSetBit(CH[channel->num],CH_num[channel->num],1);
    }
    else if(channel->sw == 0)
    {
        gioSetBit(CH[channel->num],CH_num[channel->num],0);
    }
}
