
#ifndef INCLUDE_MPPT_H_
#define INCLUDE_MPPT_H_

#include "ina226.h"

/*Data structure for MPPT task    (need initial value!)*/
typedef struct
{
    uint32_t preP;                //uW  previous power
    uint32_t preV;                //mV  previous voltage
    uint32_t preI;                //mA  previous current
    uint16_t increment;           //standard step size (?)
    uint16_t counter;             //counter for same direction (if direction doesn't change for this turn, counter++)
    uint8_t dir;                  //direction of change
    uint8_t predir;               //direction of previous change
    uint8_t channel;              //channel # of boost converter (1 - 4)

}mppt_data;

void mppt_hunts(mppt_data *data);
void mppt_pno(ina226_data *data1, mppt_data *data2);


#endif /* INCLUDE_MPPT_H_ */
