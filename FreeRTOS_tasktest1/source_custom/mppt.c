

#include "mppt.h"
#include "ina226.h"
#include "stdio.h"


/***************************************************************************
 * @brief
 *   Hunt's algorithm to decide the scale of step size
 *
 * @param[in] data
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
void mppt_hunts(mppt_data *data)
{
    if(data->dir == data->predir)                       //when the direction does not change
    {
        if(data->counter < MAX_DIRCOUNTER)               //when change with same direction for 4 times
         {
            if(data->stepsize < MAX_STEPSIZE)         //maximum step size is 16 times of minimum
            {
                data->stepsize = data->stepsize << 1;       //step size is doubled
            }

            data->counter = 0;
         }
     }
    else
    {
        if(data->stepsize > MIN_STEPSIZE)                                  //minimum step size
        {
            data->stepsize = data->stepsize >> 1;                //step size is halved
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

    data->predacOUT = data->dacOUT;

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
        if(data->dacOUT+data->stepsize < 4095)
        {
            data->dacOUT = data->dacOUT + data->stepsize;
        }
        else
        {
            data->dacOUT = 4095;
        }
    }
    else
    {
        if(data->dacOUT>data->stepsize)
        {
            data->dacOUT = data->dacOUT - data->stepsize;
        }
        else
        {
            data->dacOUT = 0;
        }

    }

    data->presumP = data->sumP;

//    printf("Average Voltage:%d\n",(int)avgV);
//    printf("Average Power:%d\n",(int)avgP);
}


