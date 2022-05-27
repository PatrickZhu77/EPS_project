
#include "battery.h"
#include "ina226.h"
#include "stdio.h"
#include "mppt.h"

/***************************************************************************
 * @brief
 *   Switch the battery on (Start to discharge)
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_on(battery_data *battery)
{
    gioSetBit(BSW[battery->num],BSW_num[battery->num],1);
    battery->sw = 1;
}

/***************************************************************************
 * @brief
 *   Switch the battery off (Stop discharging)
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_off(battery_data *battery)
{
    gioSetBit(BSW[battery->num],BSW_num[battery->num],0);
    battery->sw = 0;
}


/***************************************************************************
 * @brief
 *   Decide how to control charging according to voltage and current
 *
 *  @param[in] data
 *   Pointer to public data structure of current sensor.
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_compareVI(ina226_data *data1, battery_data *data2)
{

    if((data1->bus_voltage *1250 /1000) > data2->maxV)               //when battery is full. (sumV LSB=1.25mV)
    {
        battery_off(data2);
    }
}

/***************************************************************************
 * @brief
 *   Decide when to turn battery off according to temperature
 *
 *  @param[in] data
 *   Pointer to public data structure of current sensor.
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_compareT(max6698_data *data1, battery_data *data2)
{
    if(data1->temp[0] < data2->temp_charge)               //when battery temp. < 10C
    {
        battery_off(data2);
    }

    if(data1->temp[0] < data2->temp_discharge)               //when battery temp. < -20C
    {
        battery_off(data2);
    }

}
