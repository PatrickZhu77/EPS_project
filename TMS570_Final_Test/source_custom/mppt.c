#include "mppt.h"
#include "ina226.h"
#include "stdio.h"

//static uint8_t counter = 0;

/***************************************************************************
 * @brief
 *   Hunt's algorithm to decide the scale of stepsize/decrement
 *
 * @param[in] data
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
void mppt_hunts(mppt_data_t *data)
{
    if(data->dir == data->predir)                       //when the direction does not change
    {
        if(data->counter >= 7)               //when change with same direction for 8 times
         {
            if(data->stepsize < EN_STEPSIZE_MAX)         //maximum stepsize is 8 times of standard (INCOMPLETED!!!!!!!!!!!!!!)
            {
                data->stepsize = data->stepsize << 1;       //stepsize is doubled
            }

            data->counter = 0;
         }
     }
    else
    {
        if(data->stepsize > EN_STEPSIZE_MIN)
        {
            data->stepsize = data->stepsize >> 1;                //stepsize is halved
        }
        data->predir = data->dir;
    }
}


void mppt_getPower(ina3221_housekeeping_t *data1,mppt_data_t *data2)
{
    data2->power = data1->shunt_voltage[0] * data1->bus_voltage[0];
}


void mppt_getPower_ina226(ina226_housekeeping_t *data1,mppt_data_t *data2)
{
    data2->power = data1->shunt_voltage * data1->bus_voltage;
}



/***************************************************************************
 * @brief
 *   Perturb and observe algorithm to decide the change of boost converter (FB pin controlling)
 *
 * @param[in] data
 *   Pointer to data structure of mppt data.
 *
 *
 ******************************************************************************/
void mppt_pno_fb(mppt_data_t *data)
{

    if (data->power < data->prePower)
    {
        data->dir = ~data->dir;
        data->counter=0;
    }
    else
    {
        data->counter++;
    }

    mppt_hunts(data);

    if(data->dir == 0xff)
    {
        if(data->dacOUT+data->stepsize < data->dacOUT_max)
        {
            data->dacOUT = data->dacOUT + data->stepsize;
        }
        else
        {
            data->dacOUT = data->dacOUT_max;
        }
    }
    else
    {
        if(data->dacOUT > (data->stepsize+data->dacOUT_min))
        {
            data->dacOUT = data->dacOUT - data->stepsize;
        }
        else
        {
            data->dacOUT = data->dacOUT_min;
        }

    }

    if(data->dacOUT > data->dacOUT_max)
    {
        data->dacOUT = data->dacOUT_max;
    }
    if(data->dacOUT < data->dacOUT_min)
    {
        data->dacOUT = data->dacOUT_min;
    }


    data->prePower = data->power;

//    printf("Average Voltage:%d\n",(int)avgV);
//    printf("Average Power:%d\n",(int)avgP);
}

/***************************************************************************
 * @brief
 *   Reset the DAC output values if the power is abnormal
 *
 * @param[in] data1
 *   Pointer to data structure of mppt data.
 *
 * @param[in] data2
 *   Pointer to data structure of current sensor at the boost converter.
 *
 ******************************************************************************/
void mppt_reset_dac_if_no_power(mppt_data_t *data, ina226_housekeeping_t *data2)
{
    if((abs(data->dacOUT-data->dacOUT_min) <= EN_STEPSIZE_MAX) && (INA226_ConvToPower_mW(data2) < 10))      //If the set point of the boost converter is high but the power is almost zero
    {
        data->dacOUT_min = DAC_MIN;                         //Reset the DAC limit and initial restart it from initial value
        data->dacOUT = DAC_INIT;
    }

}


/***************************************************************************
 * @brief
 *   Perturb and observe algorithm to decide the change of boost converter (SS pin controlling)
 *
 * @param[in] data1
 *   Pointer to public data structure of Housekeeping data.
 *
 * @param[in] data2
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
//void mppt_pno_ss(mppt_data_t *data)
//{
//
//    if (data->power < data->prePower)
//    {
//        data->dir = ~data->dir;
//        data->counter=0;
//    }
//    else
//    {
//        data->counter++;
//    }
//
//    mppt_hunts(data);
//
//    if(data->dir == 0xff)
//    {
//        if(data->dacOUT+data->stepsize < DAC_MAX)
//        {
//            data->dacOUT = data->dacOUT + data->stepsize;
//        }
//        else
//        {
//            data->dacOUT = DAC_MAX;
//        }
//    }
//    else
//    {
//        if(data->dacOUT>=(data->stepsize+DAC_MIN))
//        {
//            data->dacOUT = data->dacOUT - data->stepsize;
//        }
//        else
//        {
//            data->dacOUT = DAC_MIN;
//        }
//
//    }
//
//    data->prePower = data->power;
//
////    printf("Average Voltage:%d\n",(int)avgV);
////    printf("Average Power:%d\n",(int)avgP);
//}
