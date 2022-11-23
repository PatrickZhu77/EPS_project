

#include "channel.h"
#include "stdio.h"

/***************************************************************************
 * @brief
 *   Switch the channel and grouped ones on
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] channel
 *   Pointer to target channel.
 *
 ******************************************************************************/
void channel_on(channel_data_t *Fchannel, channel_data_t *channel)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if(((Fchannel+i)->group_mask & (0x1 << (channel->num-1))) == (0x1 << (channel->num-1)))       // if grouped with the channel
        {
            gioSetBit(CH[i],CH_num[i],1);
            (Fchannel+i)->sw = 1;
        }
    }
}

/***************************************************************************
 * @brief
 *   Switch the channel and grouped ones off
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] channel
 *   Pointer to target channel.
 *
 ******************************************************************************/
void channel_off(channel_data_t *Fchannel, channel_data_t *channel)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if(((Fchannel+i)->group_mask & (0x1 << (channel->num-1))) == (0x1 << (channel->num-1)))       // if grouped with the channel
        {
            gioSetBit(CH[i],CH_num[i],0);
            (Fchannel+i)->sw = 0;
        }
    }
}



/***************************************************************************
 * @brief
 *   group the channels
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
void channel_set_group_mask(channel_data_t *Fchannel, channel_data_t *channel1, channel_data_t *channel2)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if(((Fchannel+i)->group_mask & (0x1 << (channel1->num-1))) == (0x1 << (channel1->num-1)))       // if grouped with channel1
        {
            (Fchannel+i)->group_mask |= (0x1 << (channel2->num - 1));   // group it with channel2
        }

        if(((Fchannel+i)->group_mask & (0x1 << (channel2->num-1))) == (0x1 << (channel2->num-1)))       // if grouped with channel2
        {
            (Fchannel+i)->group_mask |= (0x1 << (channel1->num - 1));   // group it with channel1
        }

    }
}

/***************************************************************************
 * @brief
 *   group the channels
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] sys_mode
 *   Global variable that represents system mode
 *
 ******************************************************************************/
void channel_check_mode(channel_data_t *Fchannel, uint8_t sys_mode)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        switch (sys_mode)
        {
            case 0:                                          //critical mode
                (Fchannel+i)->mode = 0;                      //all channels disabled
                break;
            case 1:                                          //safe mode
                if((Fchannel+i)->priority == HIGHEST_CH)     //Highest priority channels are enabled (Normally channels for OBC and UHF)
                {
                    (Fchannel+i)->mode = 1;
                }
                else
                {
                    (Fchannel+i)->mode = 0;
                }
                break;
            case 2:                                         //full mode
                if((Fchannel+i)->priority >= LOW_CH)        //Priority higher than low-ch channels are enabled (Normally all of the channels)
                {
                    (Fchannel+i)->mode = 1;
                }
                else
                {
                    (Fchannel+i)->mode = 0;
                }
                break;
            default:
                break;
        }
    }

}

/***************************************************************************
 * @brief
 *   Update channel measurement values
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] Fdata
 *   Pointer to the first current sensor for channels.
 *
 ******************************************************************************/
void channel_read_rawdata_and_convert(channel_data_t *Fchannel, ina226_housekeeping_t *Fdata)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->sw == 1)
        {
            (Fchannel+i)->current = INA226_ConvToCurrent_mA(Fdata+i);
            (Fchannel+i)->voltage = INA226_ConvToVoltage_mV(Fdata+i);
        }
    }
}

/***************************************************************************
 * @brief
 *   Check the battery voltage level
 *   Control switch of All THE CHANNELS according to battery voltage
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] Fdata1
 *   Pointer to the first battery data structure.
 *
 * @param[in] Fdata2
 *   Pointer to the first non-volatile data structure.
 *
 ******************************************************************************/
void channel_check_batteryV_then_update_switch(channel_data_t *Fchannel, battery_data_t *Fdata1, system_config_t *Fdata2)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->mode == 1)             //if channel should be enabled in this mode
        {
            if((Fdata1+i)->voltage >= Fdata2->chan_config_data[i].onlevel_mV)          //switch channel on when battery voltage is higher than onlevel threshold
            {
                channel_on(Fchannel, Fchannel+i);
            }
            else if((Fdata1+i)->voltage < Fdata2->chan_config_data[i].offlevel_mV)     //switch channel off when battery voltage is lower than offlevel threshold
            {
                channel_off(Fchannel, Fchannel+i);
            }
        }
    }

}

/***************************************************************************
 * @brief
 *   Check the battery discharging current.
 *   If overcurrent condition happens, switch off the channel with lowest priority
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] Fdata1
 *   Pointer to the first battery data structure.
 *
 * @param[in] Fdata2
 *   Pointer to the first non-volatile data structure.
 *
 ******************************************************************************/
void channel_check_batteryI_then_update_switch(channel_data_t *Fchannel, battery_data_t *Fdata1, system_config_t *Fdata2)
{
    uint8_t i;
    uint8_t low_pri = 0;

    if(Fdata1->status == 0)             //if battery is discharging
    {
        if(Fdata1->current > Fdata2->batt_discharging_current_limit_mA)         //if overcurrent happens
        {
            for(i=0;i<NUM_OF_CHANNELS;i++)
            {
                if((Fchannel+i)->mode == 1)
                {
                    if((Fchannel+i)->priority <= (Fchannel+low_pri)->priority)
                    {
                        low_pri = i;            //find the channel with lowest priority
                    }
                }
            }
            channel_off(Fchannel, Fchannel+low_pri);
            (Fchannel+low_pri)->resume = 1;
        }
    }

}

/***************************************************************************
 * @brief
 *   Check the voltage of every channel
 *   Control switch of All THE CHANNELS according to channel voltage
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] Fdata
 *   Pointer to the first non-volatile data structure.
 *
 ******************************************************************************/
void channel_check_chanV_then_update_switch(channel_data_t *Fchannel, system_config_t *Fdata)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->mode == 1)
        {
            if((Fchannel+i)->voltage > Fdata->chan_config_data[i].maxV_mV)         //if channel voltage is higher than maximum limit
            {
                channel_off(Fchannel, Fchannel+i);
                (Fchannel+i)->resume = 1;
            }
            else if((Fchannel+i)->voltage < Fdata->chan_config_data[i].minV_mV)    //if channel voltage is lower than minimum limit
            {
                channel_off(Fchannel, Fchannel+i);
                (Fchannel+i)->resume = 1;
            }
        }
    }

}

/***************************************************************************
 * @brief
 *   Recover channel that is closed due to temporary error
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 ******************************************************************************/
void channel_resume(channel_data_t *Fchannel)
{
    uint8_t i;

    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        if((Fchannel+i)->mode == 1)
        {
            if((Fchannel+i)->resume == 1)
            {
                channel_on(Fchannel, Fchannel+i);
                (Fchannel+i)->resume = 0;           //clear resume to 0 after resuming
            }
        }
    }

}

/***************************************************************************
 * @brief
 *   Check the overcurrent condition of the channels. If overcurrent happens very often in a short time, then increase the threshold value
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] Fdata
 *   Pointer to the current sensor of the first channel.
 *
 * @param[in] data
 *   Pointer to the config data structure.
 *
 * @param[in] data2
 *   Pointer to the sensor config data structure.
 *
 * @param[in] i2c
 *   Pointer to I2C peripheral register block.
 *
 ******************************************************************************/
void channel_check_overcurrent_then_config_and_resume(channel_data_t *Fchannel, ina226_housekeeping_t *Fdata, system_config_t *data, sensor_config_t data2, i2cBASE_t *i2c)
{
    uint8_t i, j;

    for(i=0; i<NUM_OF_INA226_CHANNEL_TEST; i++)//for(i=0; i<NUM_OF_CHANNELS; i++)
    {
        INA226_ReadMaskReg(i2c, Fdata+i);           //should be deleted after test

//        if((INA226_ReadMaskReg(i2c, Fdata+i) & MASK_REG_ALERT_FLAG_MASK))           //mask register alert flag is 1
//        {
//            /*Record the latest three trip time intervals*/
//            (Fchannel+i)->t_interval[2] = (Fchannel+i)->t_interval[1];
//            (Fchannel+i)->t_interval[1] = (Fchannel+i)->t_interval[0];
//            (Fchannel+i)->t_interval[0] = (uint32_t)xTaskGetTickCount() - (Fchannel+i)->t_lastTrip;
//
//            /*Record the trip time in tick*/
//            (Fchannel+i)->t_lastTrip = (uint32_t)xTaskGetTickCount();
//
//            uint64_t t_interval_avg = 0;
//            for(j=0; j<NUM_OF_T_INTERVAL; j++)
//            {
//                t_interval_avg += (Fchannel+i)->t_interval[j];
//            }
//            t_interval_avg /= NUM_OF_T_INTERVAL;
//
//            if(t_interval_avg < MINIMUM_TIME_INTERVAL_AVERAGE)                      //average time interval smaller than minimum threshold
//            {
//                /*Update the global data structure of RAM copy and proper sensor*/
//                data->chan_config_data[i].maxI_mA += data->chan_config_data[i].maxI_increment_mA;
//                (Fdata+i)->alert_reg = data->chan_config_data[i].maxI_mA;
//
//                /*Update the sensor register*/
//                INA226_Init(i2c, data->channel_monitor_Rshunt[i], data2, data2.ina226_channel_mask, (Fdata+i));
//
//            }
//        }
    }
}

