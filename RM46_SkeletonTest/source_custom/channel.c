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
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] channel
 *   Pointer to channel.
 *
 ******************************************************************************/
void channel_on(channel_data *Fchannel, channel_data *channel)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->group & (0x1 << (channel->num)))       // if grouped with the channel
        {
            gioSetBit(CH[i],CH_num[i],1);
            (Fchannel+i)->sw = 1;
        }
    }
}

/***************************************************************************
 * @brief
 *   Switch the channel off
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] channel
 *   Pointer to channel.
 *
 ******************************************************************************/
void channel_off(channel_data *Fchannel, channel_data *channel)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->group & (0x1 << (channel->num)))       // if grouped with the channel
        {
            gioSetBit(CH[i],CH_num[i],0);
            (Fchannel+i)->sw = 0;
        }
    }
}


/***************************************************************************
 * @brief
 *   Decide when to turn channel off
 *
 *  @param[in] data
 *   Pointer to current sensor.
 *
 * @param[in] channel
 *   Pointer to channel.
 *
 ******************************************************************************/
//void channel_compare(ina226_data *data, channel_data *channel)
//{
//    if(data->bus_voltage[0] > channel->maxV)               //when channel overvoltage
//    {
//        channel_off(channel);
//    }
//    else if((data->shunt_voltage *2500 /1000 /data->shunt_resistance) > channel->maxI)
//    {
//        channel_off(channel);
//    }
//}

/***************************************************************************
 * @brief
 *   Group the channels
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] channel1
 *   Pointer to one channel to be grouped.
 *
 * @param[in] channel2
 *   Pointer to another channel to be grouped.
 *
 ******************************************************************************/
void channel_set_group(channel_data *Fchannel, channel_data *channel1, channel_data *channel2)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->group & (0x1 << (channel1->num)))       // if grouped with channel1
        {
            (Fchannel+i)->group |= (0x1 << (channel2->num));   // group it with channel2
        }

        if((Fchannel+i)->group & (0x1 << (channel2->num)))       // if grouped with channel2
        {
            (Fchannel+i)->group |= (0x1 << (channel1->num));   // group it with channel1
        }

    }
}

/***************************************************************************
 * @brief
 *   Check the input voltage level
 *   Control switch of All THE CHANNELS according to priority
 *
 * @param[in] data
 *   Pointer to current sensor(of solar panel).
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 ******************************************************************************/
void channel_check_lowVoltage(ina3221_data *data, channel_data *Fchannel)
{
    uint8_t i;

    /*  Vin >= 4.4V    */
    if(data->bus_voltage[0] >= 3250)
    {
        for(i=0;i<NUM_OF_CHANNELS;i++)
        {
            channel_on(Fchannel, Fchannel+i);
        }
    }
    /*  Vin < 4.4V and >= 3.3V  */
    else if(data->bus_voltage[0] < 3250 && data->bus_voltage[0] >= 2640)
    {
        for(i=0;i<NUM_OF_CHANNELS;i++)
        {
            if((Fchannel+i)->priority >= LOW_CH)
            {
                channel_on(Fchannel, Fchannel+i);
            }
            else
            {
                channel_off(Fchannel, Fchannel+i);
            }
        }
    }
    /*  Vin < 3.3V and >= 2.7V  */
    else if(data->bus_voltage[0] < 2640 && data->bus_voltage[0] >= 2160)
    {
        for(i=0;i<NUM_OF_CHANNELS;i++)
        {
            if((Fchannel+i)->priority >= MID_CH)
            {
                channel_on(Fchannel, Fchannel+i);
            }
            else
            {
                channel_off(Fchannel, Fchannel+i);
            }
        }
    }
    /*  Vin < 2.7V  */
    else if(data->bus_voltage[0] < 2160)
    {
        for(i=0;i<NUM_OF_CHANNELS;i++)
        {
            if((Fchannel+i)->priority == HIGHEST_CH)
            {
                channel_on(Fchannel, Fchannel+i);
            }
            else
            {
                channel_off(Fchannel, Fchannel+i);
            }
        }
    }

}

/***************************************************************************
 * @brief
 *   Check if channels are tripped
 *
 * @param[in] data
 *   Pointer to data structure of current sensor(of channel).
 *   !!! Pointer must be the first
 *
 * @param[in] Fchannel
 *   Pointer to data structure of channel.
 *
 ******************************************************************************/
void channel_check_trip(ina226_data *data, channel_data *Fchannel)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if(((Fchannel+i)->sw == 1) && ((data+i)->shunt_voltage <= 400))   // 400: about 1mV for Vshunt
        {
            channel_off(Fchannel, Fchannel+i);
        }
    }
}
