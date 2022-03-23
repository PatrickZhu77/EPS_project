

#include "mppt.h"
#include "ina226.h"
#include "stdio.h"

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
        if(data->counter >= 4)               //when change with same direction for 4 or less times
         {
            if(data->increment < 8)         //maximum increment is 8 times of standard (INCOMPLETED!!!!!!!!!!!!!!)
            {
                data->increment = data->increment << 1;       //increment is doubled
                data->counter = 0;
            }

         }
     }
    else
    {
        if(data->increment >= 0x2)
        {
            data->increment = data->increment >> 1;                //increment is halved
        }
        data->predir = data->dir;
    }
 }

/***************************************************************************
 * @brief
 *   Perturb and observe algorithm to decide the change of boost converter
 *
 * @param[in] data1
 *   Pointer to public data structure of Housekeeping data.
 *
 * @param[in] data2
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/

void mppt_pno(ina226_data *data1, mppt_data *data2)
{

    if(data1->power > data2->preP)         // P(k) > P(k-1), current power greater than previous power
    {
        if(data1->bus_voltage > data2->preV)        // V(k) > V(k-1), current voltage greater than previous voltage
        {
            if(data2->dir == 1)            // If the direction does not change, counter plus one.
            {
                data2->counter++;
            }
            else                           // Otherwise clear the counter.
            {
                data2->counter = 0;
            }

            data2->dir = 1;
            mppt_hunts(data2);
//            printf("Voltage increased by %d V.\n",(int)data2->increment);

        }
        else                                        // V(k) < V(k-1), current voltage smaller than previous voltage
        {
            if(data2->dir == 0)
            {
                data2->counter++;
            }
            else
            {
                data2->counter = 0;
            }

            data2->dir = 0;
            mppt_hunts(data2);
//            printf("Voltage decreased by %d V.\n",(int)data2->increment);

        }

    }
    else                                    // P(k) > P(k-1), current power smaller than previous power
    {
        if(data1->bus_voltage > data2->preV)        // V(k) > V(k-1), current voltage greater than previous voltage
        {
            if(data2->dir == 0)
            {
                data2->counter++;
            }
            else
            {
                data2->counter = 0;
            }

            data2->dir = 0;
            mppt_hunts(data2);
//            printf("Voltage decreased by %d V.\n",(int)data2->increment);

        }
        else                                        // V(k) < V(k-1), current voltage smaller than previous voltage
        {
            if(data2->dir == 1)
            {
                data2->counter++;
            }
            else
            {
                data2->counter = 0;
            }

            data2->dir = 1;
            mppt_hunts(data2);
//            printf("Voltage increased by %d V.\n",(int)data2->increment);

        }

    }

    data2->preP = data1->power;
    data2->preV = data1->bus_voltage;

}


