
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
    heater->temp = MAX6698_ConvertTemp_C(data, heater->num);
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
    if(heater->profile == 1)                        //in sunshine
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
void heater_update_profile(heater_data_t *heater, system_config_t *data, ina226_housekeeping_t *data2, uint32_t curr_time)
{
    if(INA226_ConvToPower_mW(data2) > data->heater_solar_panel_threshold_power_mW)      //If satellite is in sunshine
    {
        if((curr_time - heater->time_light_last_seen) > data->heater_tumble_threshold_time_s)
        {
            heater->time_of_first_light_per_orbit = curr_time;

            heater->profile = 1;        //set heater profile to sunshine
        }

        if(heater->profile == 1)
        {
            heater->time_light_last_seen = curr_time;
        }

    }

    if(curr_time - heater->time_light_last_seen > data->heater_tumble_threshold_time_s)         //If satellite is in eclipse
    {
        if((curr_time - heater->time_of_first_light_per_orbit) > (data->heater_orbit_period_s - data->heater_battery_heat_up_time_s))       //orbit period - heat up time = delay time
        {
            heater->profile = 1;        //set heater profile to sunshine
        }
        else
        {
            heater->profile = 0;        //set heater profile to eclipse
        }
    }

}
