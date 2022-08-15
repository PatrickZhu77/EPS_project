
#include "heater.h"

/***************************************************************************
 * @brief
 *   Switch the heater on
 *
 * @param[in] heater
 *   Pointer to heater data structure.
 *
 ******************************************************************************/
void heater_on(heater_data_t *heater)
{
    gioSetBit(HEAT[heater->num-1],HEAT_num[heater->num-1],1);
    heater->sw = 1;
}

/***************************************************************************
 * @brief
 *   Switch the heater off
 *
 * @param[in] heater
 *   Pointer to heater data structure.
 *
 ******************************************************************************/
void heater_off(heater_data_t *heater)
{
    gioSetBit(HEAT[heater->num-1],HEAT_num[heater->num-1],0);
    heater->sw = 0;
}

void heater_read_rawdata_and_convert(heater_data_t *heater, max6698_housekeeping_t *data)
{
    heater->temp = data->temp[heater->num-1];
}

/***************************************************************************
 * @brief
 *   Control the switch depending on temperature
 *
 * @param[in] heater
 *   Pointer to heater data structure.
 *
 * @param[in] data
 *   Pointer to data structure of non-volatile data.
 *
 ******************************************************************************/
void heater_temp_SW(heater_data_t *heater, system_config_t *data)
{
    if(heater->profile == 1)
    {
        if(heater->temp < data->heater_sunshine_temp_on_c)    //if temperature is lower than Ton
        {
            heater_on(heater);
        }
        else if(heater->temp > data->heater_sunshine_temp_off_c)    //if temperature is higher than Toff
        {
            heater_off(heater);
        }
    }
    else
    {
        if(heater->temp < data->heater_eclipse_temp_on_c)    //if temperature is lower than Ton
        {
            heater_on(heater);
        }
        else if(heater->temp > data->heater_eclipse_temp_off_c)    //if temperature is higher than Toff
        {
            heater_off(heater);
        }
    }

}

void heater_check_profile(heater_data_t *heater, system_config_t *data)
{

}
