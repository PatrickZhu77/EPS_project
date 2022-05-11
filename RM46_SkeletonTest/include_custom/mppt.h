
#ifndef INCLUDE_MPPT_H_
#define INCLUDE_MPPT_H_

#include "ina3221.h"
#include "ina226.h"

#define NUM_AVERAGE 10

#define DAC_MIN  500
#define DAC_MAX  3000
#define DAC_INIT 3000
#define EN_STEPSIZE_MIN     64
#define EN_STEPSIZE_MAX     128
#define EN_STEPSIZE_INIT    64

#define MPPT_TASK_DELAY     2000 //ms


/*Data structure for MPPT task    (need initial value!)*/
typedef struct
{
    uint8_t dir;                  //direction of change
    uint8_t predir;               //direction of previous change
    uint8_t channel;              //channel # of boost converter (1 - 4)
    uint16_t stepsize;           //standard step size (?)
    uint16_t counter;             //counter for same direction (if direction doesn't change for this turn, counter++)
    uint16_t dacOUT;               //output set of dac (0-4095)
    uint16_t predacOUT;             //store the previous value for retrieving
    uint32_t power;                  //power of solar panel
    uint32_t prePower;               //previous power

}mppt_data;

void mppt_hunts(mppt_data *data);
void mppt_getPower(ina3221_data *data1,mppt_data *data2);
void mppt_getPower_ina226(ina226_data *data1,mppt_data *data2);
void mppt_pno_ss(mppt_data *data);
void mppt_pno_en(mppt_data *data);


#endif /* INCLUDE_MPPT_H_ */
