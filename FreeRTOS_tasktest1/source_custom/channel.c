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
    else if((data->shunt_voltage *2500 /1000 /data->shunt_resistance) > channel->maxI)
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

void channel_setPri(channel_data *channel, uint8_t pri)
{
    channel->priority = pri;
}

/***************************************************************************
 * @brief
 *   Check the input voltage level
 *   Control switch of channels according to priority
 *
 * @param[in] data
 *   Pointer to data structure of current sensor.
 *
 ******************************************************************************/
void channel_check(ina226_data *data, channel_data *pchannelD)
{
    int i;

    /*  Vin >= 4.4V    */
    if(data->bus_voltage >= 3250)
    {
        for(i=0;i<16;i++)
        {
            channel_on(pchannelD+i);
        }
    }
    /*  Vin < 4.4V and >= 3.3V  */
    else if(data->bus_voltage < 3250 && data->bus_voltage >= 2640)
    {
        for(i=0;i<16;i++)
        {
            if((pchannelD+i)->priority >= LOW_CH)
            {
                channel_on(pchannelD+i);
            }
            else
            {
                channel_off(pchannelD+i);
            }
        }
    }
    /*  Vin < 3.3V and >= 2.7V  */
    else if(data->bus_voltage < 2640 && data->bus_voltage >= 2160)
    {
        for(i=0;i<16;i++)
        {
            if((pchannelD+i)->priority >= MID_CH)
            {
                channel_on(pchannelD+i);
            }
            else
            {
                channel_off(pchannelD+i);
            }
        }
    }
    /*  Vin < 2.7V  */
    else if(data->bus_voltage < 2160)
    {
        for(i=0;i<16;i++)
        {
            if((pchannelD+i)->priority = HIGHEST_CH)
            {
                channel_on(pchannelD+i);
            }
            else
            {
                channel_off(pchannelD+i);
            }
        }
    }

}
