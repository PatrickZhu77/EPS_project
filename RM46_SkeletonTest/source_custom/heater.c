
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

/***************************************************************************
 * @brief
 *   Update the profile of heaters
 *
 * @param[in] heater
 *   Pointer to heater data structure.
 *
 * @param[in] data
 *   Pointer to data structure of non-volatile data.
 *
 * @param[in] data2
 *   Pointer to the current sensor of boost converter.
 *
 * @param[in] curr_time
 *   Current time in sec
 *
 ******************************************************************************/
void heater_update_profile(heater_data_t *heater, system_config_t *data, ina3221_housekeeping_t *data2, uint32_t curr_time)
{

    if(heater->profile == 0)        //satellite is in eclipse
    {

        if((heater->init_time == 0) && (INA3221_ConvToPower_mW(data2, 1, data->power_conversion_Rshunt[1]) >= HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW))     //boost converter input power larger than solar panel threshold power and init_time is zero
        {
            heater->init_time = curr_time;        //set the init_time to current time
        }
        else if((heater->init_time) && (INA3221_ConvToPower_mW(data2, 1, data->power_conversion_Rshunt[1]) >= HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW))
        {
            heater->profile_counter++;

            if((heater->profile_counter == HEATER_PROFILE_SWITCH_COUNTING) && (curr_time - heater->init_time < HEATER_TUMBLE_THRESHOLD_TIME_S))     //The power threshold has been reached for proper times and the passed time is smaller than threshold tumble time
            {
                heater->profile = 1;        //change profile to sunshine
                heater->init_time = 0;
                heater->profile_counter = 0;
            }
        }
    }
    else            //satellite is in sunshine
    {
        if((heater->init_time == 0) && (INA3221_ConvToPower_mW(data2, 1, data->power_conversion_Rshunt[1]) < HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW))     //boost converter input power smaller than solar panel threshold power and init_time is zero
        {
            heater->init_time = curr_time;        //set the init_time to current time
        }
        else if((heater->init_time) && (INA3221_ConvToPower_mW(data2, 1, data->power_conversion_Rshunt[1]) < HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW))
        {
            heater->profile_counter++;

            if((heater->profile_counter == HEATER_PROFILE_SWITCH_COUNTING) && (curr_time - heater->init_time < HEATER_TUMBLE_THRESHOLD_TIME_S))     //The power threshold has been reached for proper times and the passed time is smaller than threshold tumble time
            {
                heater->profile = 0;        //change profile to eclipse
                heater->init_time = 0;
                heater->profile_counter = 0;
            }
        }

    }
}
