

#include "mppt.h"
#include "ina226.h"
#include "stdio.h"

/***************************************************************************
 * @brief
 *   Hunt's algorithm to decide the proper value of increment/decrement
 *
 * @param[in] data
 *   Pointer to public data structure of MPPT task.
 *
 ******************************************************************************/
void mppt_hunts(mppt_data *data)
{
    double temp_incr = 1;           //basic increment

    if(data->counter < 2)               //when change with same direction for 2  or less times
    {
        data->increment = temp_incr;
    }
    else if(data->counter >= 2 && data->counter < 4)        //when change with same direction for 2 to 4 times
    {
        data->increment = temp_incr*2;
    }
    else if(data->counter >= 4)         //when change with same direction for more than 4 times
    {
        data->increment = temp_incr*3;
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
            printf("Voltage increased by %d V.\n",(int)data2->increment);

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
            printf("Voltage decreased by %d V.\n",(int)data2->increment);

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
            printf("Voltage decreased by %d V.\n",(int)data2->increment);

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
            printf("Voltage increased by %d V.\n",(int)data2->increment);

        }

    }

    data2->preP = data1->bus_voltage;
    data2->preV = data1->power;

}


