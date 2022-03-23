
#ifndef INCLUDE_MPPT_H_
#define INCLUDE_MPPT_H_

#include "ina226.h"

#define NUM_AVERAGE 10

/*Data structure for MPPT task    (need initial value!)*/
typedef struct
{
    uint8_t dir;                  //direction of change
    uint8_t predir;               //direction of previous change
    uint8_t channel;              //channel # of boost converter (1 - 4)
//    uint16_t preV[NUM_AVERAGE];   //mV  previous voltage
    uint16_t increment;           //standard step size (?)
    uint16_t counter;             //counter for same direction (if direction doesn't change for this turn, counter++)
    uint16_t dacOUT;               //output set of dac (0-4095)
    uint16_t predacOUT;
//    uint32_t preP[NUM_AVERAGE];    //uW  previous power
    uint32_t sumV;                  //sum of voltage for NUM_AVERAGE times
    uint32_t presumV;               //previous sum of voltage
    uint32_t sumP;                  //sum of power for NUM_AVERAGE times
    uint32_t presumP;               //previous sum of power

}mppt_data;

void mppt_hunts(mppt_data *data);
void mppt_getSumV(ina226_data *data1, mppt_data *data2);
void mppt_getSumP(ina226_data *data1, mppt_data *data2);
void mppt_pno_ss(mppt_data *data);


#endif /* INCLUDE_MPPT_H_ */
