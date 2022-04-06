

#include "mppt.h"
#include "ina226.h"
#include "stdio.h"

//static uint8_t counter = 0;

/***************************************************************************
 * @brief
 *   Hunt's algorithm to decide the scale of increment/decrement
 *
 * @param[in] data
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
void mppt_hunts(mppt_data *data)
{
    if(data->dir == data->predir)                       //when the direction does not change
    {
        if(data->counter >= 3)               //when change with same direction for 4 times
         {
            if(data->increment < EN_STEPSIZE_MAX)         //maximum increment is 8 times of standard (INCOMPLETED!!!!!!!!!!!!!!)
            {
                data->increment = data->increment << 1;       //increment is doubled
            }

            data->counter = 0;
         }
     }
    else
    {
        if(data->increment > EN_STEPSIZE_MIN)
        {
            data->increment = data->increment >> 1;                //increment is halved
        }
        data->predir = data->dir;
    }
}

void mppt_getSumV(ina226_data *data1,mppt_data *data2)
{
    data2->sumV = data2->sumV+ data1->bus_voltage;
}


void mppt_getSumP(ina226_data *data1,mppt_data *data2)
{
    data2->sumP = data2->sumP+ data1->shunt_voltage * data1->bus_voltage;
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
void mppt_pno_ss(mppt_data *data)
{

    if (data->sumP < data->presumP)
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
        if(data->dacOUT+data->increment < DAC_MAX)
        {
            data->dacOUT = data->dacOUT + data->increment;
        }
        else
        {
            data->dacOUT = DAC_MAX;
        }
    }
    else
    {
        if(data->dacOUT>=(data->increment+DAC_MIN))
        {
            data->dacOUT = data->dacOUT - data->increment;
        }
        else
        {
            data->dacOUT = DAC_MIN;
        }

    }

    data->presumP = data->sumP;

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
void mppt_pno_en(mppt_data *data)
{

    if (data->sumP < data->presumP)
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
        if(data->dacOUT+data->increment < DAC_MAX)
        {
            data->dacOUT = data->dacOUT + data->increment;
        }
        else
        {
            data->dacOUT = DAC_MAX;
        }
    }
    else
    {
        if(data->dacOUT>(data->increment+DAC_MIN))
        {
            data->dacOUT = data->dacOUT - data->increment;
        }
        else
        {
            data->dacOUT = DAC_MIN;
        }

    }

    data->presumP = data->sumP;

//    printf("Average Voltage:%d\n",(int)avgV);
//    printf("Average Power:%d\n",(int)avgP);
}

