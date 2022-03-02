
#ifndef INCLUDE_MPPT_H_
#define INCLUDE_MPPT_H_

#include "ina226.h"

/*Data structure for MPPT task*/
typedef struct
{
    uint32_t preP;                //uW
    uint32_t preV;                //mV
    uint32_t preI;                //mA
    uint16_t increment;         // step size (?)
    uint16_t counter;          //counter for same direction (if direction doesn't change for this turn, counter++)
    uint8_t dir;                //direction of change
    uint8_t channel;            //channel # of boost converter (1 - 4)


}mppt_data;

void mppt_hunts(mppt_data *data);
void mppt_pno(ina226_data *data1, mppt_data *data2);


#endif /* INCLUDE_MPPT_H_ */
