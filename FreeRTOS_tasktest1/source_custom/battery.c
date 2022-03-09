/*
 * battery.c
 *
 *  Created on: Mar 7, 2022
 *      Author: sdamkjar
 */


#include "battery.h"
#include "ina226.h"
#include "stdio.h"

/***************************************************************************
 * @brief
 *   Decide when to turn channel off
 *
 * @param[in] data
 *   Pointer to public data structure of current sensor.
 *
 ******************************************************************************/
void channel_switch(ina226_data *data, channel_data *channel)
{
    if(data->bus_voltage > channel->overV)               //when channel overvoltage
    {
        channel->sw = 0;
    }
    else if(data->current > channel->overI)
    {
        channel->sw = 0;
    }
}

