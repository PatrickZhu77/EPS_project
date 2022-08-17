

#include "fee_function.h"
#include "ti_fee.h"
#include "sys_common.h"


static Std_ReturnType oResult=E_OK;
static uint16 Status;

void delay(void)
{
    unsigned int dummycnt=0x0000FFU;
    do
    {
        dummycnt--;
    }
    while(dummycnt>0);
}


void fee_initial(void)
{
    TI_Fee_Init();
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0 );
    }
    while(Status!= IDLE);

}


void fee_write_8bit(uint16_t BlockNumber, uint8_t *pData)
{

    TI_Fee_WriteAsync(BlockNumber, pData);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

}

void fee_write_16bit(uint16_t BlockNumber, uint16_t *pData, uint8_t Length)
{
    uint8_t converted_data[Length*2];
    uint8_t *pCdata = converted_data;

    uint8_t i = 0;
    for(i=0;i<Length;i++)
    {
        *(pCdata+i*2) = (*(pData + i)) << 8;
        *(pCdata+(i*2+1)) = *(pData + i);
    }

    TI_Fee_WriteAsync(BlockNumber, pCdata);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

}


void fee_read_8bit(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData, uint16_t numOfParameter)
{
    oResult=TI_Fee_Read(BlockNumber,BlockOffset,pData,numOfParameter);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

}

void fee_read_16bit(uint16_t BlockNumber, uint16_t BlockOffset, uint16_t *pData, uint16_t numOfParameter)
{
    uint8_t converted_data[(uint8_t)numOfParameter*2];
    uint8_t *pCdata = converted_data;

    oResult=TI_Fee_Read(BlockNumber,BlockOffset,pCdata,numOfParameter*2);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

    uint8_t i = 0;
    for(i=0;i<numOfParameter;i++)
    {

        *(pData + i) = (converted_data[i*2] << 8) | converted_data[i*2+1];
    }

}

void fee_read_32bit(uint16_t BlockNumber, uint16_t BlockOffset, uint32_t *pData, uint16_t numOfParameter)
{
    uint8_t converted_data[(uint8_t)numOfParameter*4];
    uint8_t *pCdata = converted_data;

    oResult=TI_Fee_Read(BlockNumber,BlockOffset,pCdata,numOfParameter);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

    uint8_t i = 0;
    for(i=0;i<numOfParameter;i++)
    {

        *(pData + i) = (converted_data[i*4] << 24) | (converted_data[i*4+1] << 16) | (converted_data[i*4+2] << 8) | converted_data[i*4+3];
    }

}


uint8_t fee_read_8bit_and_check_crc(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData)
{
    uint8_t converted_data[2];
    uint8_t *pCdata = converted_data;

    /* Read 8 bit data and its 8 bit crc*/
    fee_read_8bit(BlockNumber, BlockOffset, pCdata, 2);

    if(crc8_check(converted_data[0], converted_data[1]))
    {
        *pData = converted_data[0];
        return 1;
    }
    else
    {
        return 0;
    }

}

uint8_t fee_read_16bit_and_check_crc(uint16_t BlockNumber, uint16_t BlockOffset, uint16_t *pData)
{
    uint8_t converted_data[4];
    uint8_t *pCdata = converted_data;

    /* Read 8 bit data and its 8 bit crc*/
    fee_read_8bit(BlockNumber, BlockOffset, pCdata, 4);

    uint8_t i = 0;
    for(i=0; i<2; i++)
    {
        if(crc8_check(converted_data[i*2], converted_data[i*2+1]) == 0);
        {
            return 0;
        }
    }

    *pData = (converted_data[0] << 8) | converted_data[2];

    return 1;

}

uint8_t fee_read_32bit_and_check_crc(uint16_t BlockNumber, uint16_t BlockOffset, uint32_t *pData)
{
    uint8_t converted_data[8];
    uint8_t *pCdata = converted_data;

    /* Read 8 bit data and its 8 bit crc*/
    fee_read_8bit(BlockNumber, BlockOffset, pCdata, 8);

    uint8_t i = 0;
    for(i=0; i<4; i++)
    {
        if(crc8_check(converted_data[i*2], converted_data[i*2+1]) == 0);
        {
            return 0;
        }
    }

    *pData = (converted_data[0] << 24) | (converted_data[2] << 16) | (converted_data[4] << 8) | converted_data[6];

    return 1;

}


void fee_read_sensor_config(sensor_config_t *data)
{
    uint8_t temp_counter = 0;

    fee_read_8bit(SENSOR_CONFIG_8BIT_BLOCK_NUM, temp_counter, &data->software_version, 1);
    temp_counter++;

    fee_read_8bit(SENSOR_CONFIG_8BIT_BLOCK_NUM, temp_counter, &data->max6698_cfg1_setting, 1);
    temp_counter++;

    fee_read_8bit(SENSOR_CONFIG_8BIT_BLOCK_NUM, temp_counter, &data->max6698_cfg2_setting, 1);
    temp_counter++;

    fee_read_8bit(SENSOR_CONFIG_8BIT_BLOCK_NUM, temp_counter, &data->max6698_cfg3_setting, 1);
    temp_counter = 0;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina226_cfg_setting, 1);
    temp_counter++;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina226_overcurrent_mask, 1);
    temp_counter++;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina226_monitor_mask, 1);
    temp_counter++;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina226_battery_mask, 1);
    temp_counter++;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina226_channel_mask, 1);
    temp_counter++;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina3221_cfg_setting, 1);
    temp_counter++;

    fee_read_16bit(SENSOR_CONFIG_16BIT_BLOCK_NUM, temp_counter*2, &data->ina3221_mask_setting, 1);
    temp_counter = 0;

}

uint8_t fee_read_flash_copy(uint8_t copy_version, system_config_t *data)
{
    uint8_t temp_counter = 0;
    uint8_t temp_copy_version = 0;
    uint8_t i = 0;

    switch(copy_version)
    {
        case 1:
        {
            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->batt_charging_temp_min_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->batt_charging_temp_max_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->batt_discharging_temp_min_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->batt_discharging_temp_max_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->heater_sunshine_temp_on_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->heater_sunshine_temp_off_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->heater_eclipse_temp_on_c, 1);
            temp_counter++;

            fee_read_8bit(FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter,  &data->heater_eclipse_temp_off_c, 1);
            temp_counter = 0;

            for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->overcurrent_protection_alert_mA[i], 1);
                temp_counter++;
            }

            for(i=0; i<NUM_OF_INA226_MONITOR; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->current_monitor_alert_mA[i], 1);
                temp_counter++;
            }

            for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->overcurrent_protection_Rshunt[i], 1);
                temp_counter++;
            }

            for(i=0; i<NUM_OF_INA226_MONITOR; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->current_monitor_Rshunt[i], 1);
                temp_counter++;
            }

            for(i=0; i<NUM_OF_INA226_BATTERY; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->battery_protection_Rshunt[i], 1);
                temp_counter++;
            }

            for(i=0; i<NUM_OF_INA226_CHANNEL; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->channel_monitor_Rshunt[i], 1);
                temp_counter++;
            }

            for(i=0; i<NUM_OF_INA3221; i++)
            {
                fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->power_conversion_Rshunt[i], 1);
                temp_counter++;
            }

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->batt_charging_current_limit_mA, 1);
            temp_counter++;

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->batt_discharging_current_limit_mA, 1);
            temp_counter++;

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->heater_tumble_threshold_time_s, 1);
            temp_counter++;

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->heater_solar_panel_threshold_power_mW, 1);
            temp_counter++;

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->heater_delay_time_s, 1);
            temp_counter++;

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->dac_init, 1);
            temp_counter++;

            fee_read_16bit(FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter,  &data->dac_stepsize_init, 1);
            temp_counter = 0;

            for(i=0; i<NUM_OF_CHANNELS; i++)
            {
                fee_read_8bit(CHANNEL_FACTORY_COPY_8BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].priority, 1);
                temp_counter = i * NUM_OF_16BIT_IN_CHANNEL_CONFIG_T;


                fee_read_16bit(CHANNEL_FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].onlevel_mV, 1);
                temp_counter++;
                fee_read_16bit(CHANNEL_FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].offlevel_mV, 1);
                temp_counter++;
                fee_read_16bit(CHANNEL_FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].maxI_mA, 1);
                temp_counter++;
                fee_read_16bit(CHANNEL_FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].maxV_mV, 1);
                temp_counter++;
                fee_read_16bit(CHANNEL_FACTORY_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].minV_mV, 1);
                temp_counter = i * NUM_OF_32BIT_IN_CHANNEL_CONFIG_T;


                fee_read_32bit(CHANNEL_FACTORY_COPY_32BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].group_mask, 1);
                temp_counter = i * NUM_OF_8BIT_IN_CHANNEL_CONFIG_T;

            }

        }

        case 2:
        {

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_charging_temp_min_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_charging_temp_max_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_discharging_temp_min_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_discharging_temp_max_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_sunshine_temp_on_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_sunshine_temp_off_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_eclipse_temp_on_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_eclipse_temp_off_c) == 0)
            {
                temp_copy_version = 1;
                break;
            }
            else
            {
                temp_counter = 0;
            }

            for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->overcurrent_protection_alert_mA[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            for(i=0; i<NUM_OF_INA226_MONITOR; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->current_monitor_alert_mA[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            for(i=0; i<NUM_OF_INA226_OVERCURRENT_PROTECTION; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->overcurrent_protection_Rshunt[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            for(i=0; i<NUM_OF_INA226_MONITOR; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->current_monitor_Rshunt[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            for(i=0; i<NUM_OF_INA226_BATTERY; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->battery_protection_Rshunt[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            for(i=0; i<NUM_OF_INA226_CHANNEL; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->channel_monitor_Rshunt[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            for(i=0; i<NUM_OF_INA3221; i++)
            {
                if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->power_conversion_Rshunt[i]) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter++;
                }

            }
            if(temp_copy_version == 1)
            {
                break;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->batt_charging_current_limit_mA) == 0)
            {
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->batt_discharging_current_limit_mA) == 0)
            {
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->heater_tumble_threshold_time_s) == 0)
            {
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->heater_solar_panel_threshold_power_mW) == 0)
            {
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->heater_delay_time_s) == 0)
            {
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->dac_init) == 0)
            {
                break;
            }
            else
            {
                temp_counter++;
            }

            if(fee_read_16bit_and_check_crc(FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->dac_stepsize_init) == 0)
            {
                break;
            }
            else
            {
                temp_counter = 0;
            }


            for(i=0; i<NUM_OF_CHANNELS; i++)
            {
                if(fee_read_8bit_and_check_crc(CHANNEL_FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].priority) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter = i * NUM_OF_16BIT_IN_CHANNEL_CONFIG_T;
                }

                if(fee_read_16bit_and_check_crc(CHANNEL_FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].onlevel_mV) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter ++;
                }

                if(fee_read_16bit_and_check_crc(CHANNEL_FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].offlevel_mV) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter ++;
                }

                if(fee_read_16bit_and_check_crc(CHANNEL_FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].maxI_mA) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter ++;
                }

                if(fee_read_16bit_and_check_crc(CHANNEL_FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].maxV_mV) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter ++;
                }

                if(fee_read_16bit_and_check_crc(CHANNEL_FLASHED_COPY_16BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].minV_mV) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter = i * NUM_OF_32BIT_IN_CHANNEL_CONFIG_T;
                }

                if(fee_read_32bit_and_check_crc(CHANNEL_FLASHED_COPY_32BIT_BLOCK_NUM, temp_counter, &data->chan_config_data[i].group_mask) == 0)
                {
                    temp_copy_version = 1;
                    break;
                }
                else
                {
                    temp_counter = i * NUM_OF_8BIT_IN_CHANNEL_CONFIG_T;
                }


            }
            if(temp_copy_version == 1)
            {
                break;
            }

        }

        default:
            break;

    }

    if(temp_copy_version == 1)
    {
//        fee_read_flash_copy(temp_copy_version, data);
        return 0;
    }

    return 1;
}

//void fee_write_flash_copy()
//{
//    switch(copy_version)
//    {
//        case 1:
//
//
//        case 2:
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_charging_temp_min_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_charging_temp_max_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_discharging_temp_min_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->batt_discharging_temp_max_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_sunshine_temp_on_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_sunshine_temp_off_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//            if(fee_read_8bit_and_check_crc(FLASHED_COPY_8BIT_BLOCK_NUM, temp_counter, &data->heater_eclipse_temp_on_c) == 0)
//            {
//                temp_copy_version = 1;
//                break;
//            }
//            else
//            {
//                temp_counter++;
//            }
//
//}
