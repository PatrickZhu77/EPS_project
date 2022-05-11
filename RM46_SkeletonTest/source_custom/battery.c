
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
    gioSetBit(BSW[battery->num],BSW_num[battery->num],1);
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
void battery_compareVI(ina226_data *data1, mppt_data *data2, battery_data *data3)
{

    if((data1->bus_voltage *1250 /1000) > data3->maxV)               //when battery is full. (sumV LSB=1.25mV)
    {
        battery_off(data3);                      //should turn off the converter but not battery!!!!!
    }



//    if((data1->shunt_voltage *2500 /1000 /data1->shunt_resistance) >= data3->maxI)    //when Vout of boost converter is overcurrent
//    {
//        data2->dacOUT = data2->predacOUT;            //retrieve to previous voltage if overcurrent happens
//        data2->counter = 0;
//
//        if(data2->stepsize > EN_STEPSIZE_MIN)                                  //minimum step size
//        {
//            data2->stepsize = data2->stepsize >> 1;                //step size is halved
//        }
//
//    }

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
