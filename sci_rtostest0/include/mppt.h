
#ifndef INCLUDE_MPPT_H_
#define INCLUDE_MPPT_H_

#include "ina226.h"

/*Data structure for MPPT task*/
typedef struct
{
    uint8_t channel;            //channel # of boost converter (1 - 4)
    double preP;                //W
    double preV;                //V
    double increment;           //V
    uint8_t dir;                //direction of change
    uint16_t counter;            //counter for same direction (if direction doesn't change for this turn, counter++)
}mppt_data;

void mppt_hunts(mppt_data *data);
void mppt_pno(ina226_data *data1, mppt_data *data2);


#endif /* INCLUDE_MPPT_H_ */
