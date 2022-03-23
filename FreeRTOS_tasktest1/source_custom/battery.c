
#include "battery.h"
#include "ina226.h"
#include "stdio.h"
#include "mppt.h"

/***************************************************************************
 * @brief
 *   SwitBSW the battery on (Start to disBSWarge)
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_on(battery_data *battery)
{
    battery->sw = 1;
}

/***************************************************************************
 * @brief
 *   SwitBSW the battery off (Stop disBSWarging)
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_off(battery_data *battery)
{
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
void battery_compareVI(mppt_data *data, battery_data *battery)
{

    if(data->preV >= battery->maxV)               //when battery is full
    {
        battery_off(battery);                      //should turn off the converter but not battery!!!!!
    }

    if(data->preI >= battery->maxI)
    {
        data->preV = battery->preV;                //retrieve to previous voltage if overcurrent happens
        if(data->increment >= 0x2)
        {
            data->increment = data->increment >> 1;
            data->counter = 0;
        }
    }

    battery->preV = data->preV;                    //store the previous voltage value for retrieving
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
//void battery_compareTemp(mppt_data *data, battery_data *battery)
//{
//    if(data->preV >= battery->maxV)               //when battery is full
//    {
//        battery_off(battery);
//    }
//
//    if(data->preI > battery->maxI)
//    {
//        battery->overFlag = 1;
//    }
//    else
//    {
//        battery->overFlag = 0;
//    }
//}


/***************************************************************************
 * @brief
 *   Execute the switBSW
 *
 * @param[in] battery
 *   Pointer to battery data.
 *
 ******************************************************************************/
void battery_switch(battery_data *battery)
{
    if(battery->sw == 1)
    {
        gioSetBit(BSW[battery->num],BSW_num[battery->num],1);
    }
    else if(battery->sw == 0)
    {
        gioSetBit(BSW[battery->num],BSW_num[battery->num],0);
    }
}
