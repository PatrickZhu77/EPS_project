
#include <battery.h>

/***************************************************************************
 * @brief
 *   Turn on charging SW
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ****************************************************************************/
void battery_chargingSW_on(battery_data_t *battery)
{
    gioSetBit(BSW[((battery->num)-1)],BSW_num[((battery->num)-1)],0);
    battery->sw[0] = 0;
}

/***************************************************************************
 * @brief
 *   Turn off charging SW
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ****************************************************************************/
void battery_chargingSW_off(battery_data_t *battery)
{
    gioSetBit(BSW[((battery->num)-1)],BSW_num[((battery->num)-1)],1);
    battery->sw[0] = 1;
}


/***************************************************************************
 * @brief
 *   Turn on discharging SW
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ****************************************************************************/
void battery_dischargingSW_on(battery_data_t *battery)
{
    gioSetBit(BSW[((battery->num)-1)+2],BSW_num[((battery->num)-1)+2],0);
    battery->sw[1] = 0;
}

/***************************************************************************
 * @brief
 *   Turn off discharging SW
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ****************************************************************************/
void battery_dischargingSW_off(battery_data_t *battery)
{
    gioSetBit(BSW[((battery->num)-1)+2],BSW_num[((battery->num)-1)+2],1);
    battery->sw[1] = 1;
}

/***************************************************************************
 * @brief
 *   Update battery status
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 * @param[in] data
 *   Pointer to data structure of current sensor data structure.
 *
 ****************************************************************************/
void battery_check_charging_status(battery_data_t *battery, ina226_housekeeping_t *data)
{
    if(data->current & 0x8000)        //if current is negative
    {
        battery->status = 0;                //battery is discharging only
    }
    else
    {
        battery->status = 1;                //battery is charging
    }

}

/***************************************************************************
 * @brief
 *   Update battery measurement values
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 * @param[in] data1
 *   Pointer to data structure of current sensor data structure.
 *
 * @param[in] data2
 *   Pointer to data structure of temperature sensor data structure.
 *
 ****************************************************************************/
void battery_read_rawdata_and_convert(battery_data_t *battery, ina226_housekeeping_t *data1, max6698_housekeeping_t *data2)
{
    battery->current = INA226_ConvToCurrent_mA(data1);
    battery->voltage = INA226_ConvToVoltage_mV(data1);
    battery->temp = MAX6698_ConvertTemp_C(data2,battery->num);
}


/***************************************************************************
 * @brief
 *   Control switches depending on temperature
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 * @param[in] data
 *   Pointer to data structure of non-volatile data.
 *
 ****************************************************************************/
void battery_check_temp_then_SW(battery_data_t *battery, system_config_t *data)
{
    if(battery->status == BATT_STATUS_CHARGING)
    {
        if((battery->temp < data->batt_charging_temp_min_c) || (battery->temp > data->batt_charging_temp_max_c))
        {
            battery_chargingSW_off(battery);            //temperature is out of range, stop charging
        }
        else
        {
            battery_chargingSW_on(battery);             //temperature is in range, continue charging
        }
    }
    else
    {
        if((battery->temp < data->batt_discharging_temp_min_c) || (battery->temp > data->batt_discharging_temp_max_c))
        {
            battery_dischargingSW_off(battery);         //temperature is out of range, stop discharging
        }
        else
        {
            battery_dischargingSW_on(battery);          //temperature is in range, continue discharging
        }
    }
}

/***************************************************************************
 * @brief
 *   Check the overcurrent condition of the battery. If overcurrent happens, change the dac limit to reach new MPP
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 * @param[in] data
 *   Pointer to data structure of non-volatile data.
 *
 * @param[in] data
 *   Pointer to data structure of the FIRST mppt data
 *
 ****************************************************************************/
void battery_check_overcurrent_then_change_MPP(battery_data_t *battery, system_config_t *data, mppt_data_t *data2)
{
    uint8_t i = 0;

    if((battery->status == 0) && (battery->current > data->batt_discharging_current_limit_mA))      //if battery discharging overcurrent
    {
        for(i=0;i<NUM_OF_MPPTS;i++)                                                                 //Increase the minimum value of DACs output to decrease the MPP
        {
            (data2+i)->dacOUT_min = (data2+i)->dacOUT + EN_STEPSIZE_MIN;
        }
    }

    if((battery->status == 1) && (battery->current > data->batt_charging_current_limit_mA))         //if battery charging overcurrent
    {
        for(i=0;i<NUM_OF_MPPTS;i++)                                                                 //Increase the minimum value of DACs output to decrease the MPP
        {
            (data2+i)->dacOUT_min = (data2+i)->dacOUT + EN_STEPSIZE_MIN;
        }
    }

}
