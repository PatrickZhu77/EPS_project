

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
void channel_check_batteryV_then_SW(channel_data_t *Fchannel, battery_data_t *Fdata1, system_config_t *Fdata2)
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
void channel_check_batteryI_then_SW(channel_data_t *Fchannel, battery_data_t *Fdata1, system_config_t *Fdata2)
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
 *   Check the battery voltage level
 *   Control switch of All THE CHANNELS according to battery voltage
 *
 * @param[in] Fchannel
 *   Pointer to FIRST channel. (Must be the pointer to first channel!!!)
 *
 * @param[in] Fdata
 *   Pointer to the first non-volatile data structure.
 *
 ******************************************************************************/
void channel_check_chanV_then_SW(channel_data_t *Fchannel, system_config_t *Fdata)
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
