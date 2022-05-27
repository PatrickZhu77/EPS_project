
#include "heater.h"

/***************************************************************************
 * @brief
 *   Switch the heater on
 *
 * @param[in] Fheater
 *   Pointer to FIRST heater. (Must be the pointer to first heater!!!)
 *
 * @param[in] heater
 *   Pointer to heater.
 *
 ******************************************************************************/
void heater_on(heater_data *heater)
{
    gioSetBit(HEAT[heater->num],HEAT_num[heater->num],1);
    heater->sw = 1;
}

/***************************************************************************
 * @brief
 *   Switch the heater off
 *
 * @param[in] Fheater
 *   Pointer to FIRST heater. (Must be the pointer to first heater!!!)
 *
 * @param[in] heater
 *   Pointer to heater.
 *
 ******************************************************************************/
void heater_off(heater_data *heater)
{
    gioSetBit(HEAT[heater->num],HEAT_num[heater->num],0);
    heater->sw = 0;
}


void heater_compareT(max6698_data *data1, heater_data *data2)
{
    if(data1->temp[0] <= data2->minT)               //when heater temp. < 0C
    {
        heater_on(data2);
    }

    if(data1->temp[0] >= data2->maxT)               //when heater temp. > 25C
    {
        heater_off(data2);
    }

}
