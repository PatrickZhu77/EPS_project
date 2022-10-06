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
 *   Perturb and observe algorithm to decide the change of boost converter (SS pin controlling)
 *
 * @param[in] data1
 *   Pointer to public data structure of Housekeeping data.
 *
 * @param[in] data2
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
void mppt_pno_ss(mppt_data_t *data)
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
        if(data->dacOUT+data->stepsize < DAC_MAX)
        {
            data->dacOUT = data->dacOUT + data->stepsize;
        }
        else
        {
            data->dacOUT = DAC_MAX;
        }
    }
    else
    {
        if(data->dacOUT>=(data->stepsize+DAC_MIN))
        {
            data->dacOUT = data->dacOUT - data->stepsize;
        }
        else
        {
            data->dacOUT = DAC_MIN;
        }

    }

    data->prePower = data->power;

//    printf("Average Voltage:%d\n",(int)avgV);
//    printf("Average Power:%d\n",(int)avgP);
}

/***************************************************************************
 * @brief
 *   Perturb and observe algorithm to decide the change of boost converter (EN pin controlling)
 *
 * @param[in] data1
 *   Pointer to public data structure of Housekeeping data.
 *
 * @param[in] data2
 *   Pointer to public data structure of MPPT task.
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
        if(data->dacOUT+data->stepsize < DAC_MAX)
        {
            data->dacOUT = data->dacOUT + data->stepsize;
        }
        else
        {
            data->dacOUT = DAC_MAX;
        }
    }
    else
    {
        if(data->dacOUT>(data->stepsize+DAC_MIN))
        {
            data->dacOUT = data->dacOUT - data->stepsize;
        }
        else
        {
            data->dacOUT = DAC_MIN;
        }

    }

    data->prePower = data->power;

//    printf("Average Voltage:%d\n",(int)avgV);
//    printf("Average Power:%d\n",(int)avgP);
}

