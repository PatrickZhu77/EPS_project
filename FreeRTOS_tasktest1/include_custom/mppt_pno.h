#ifndef INCLUDE_CUSTOM_MPPT_PNO_H_
#define INCLUDE_CUSTOM_MPPT_PNO_H_


/*Data structure for MPPT task*/
typedef struct
{
    uint8_t channel;            //channel # of boost converter (1 - 4)
    double preP;                //W
    double preV;                //V
    double preI;                //A
    uint16_t increment;           //V
    uint8_t dir;                //direction of change
    uint16_t counter;            //counter for same direction (if direction doesn't change for this turn, counter++)
    uint16_t counter2;   // store the value when counter is doubled
    uint8_t n;           // store the time that counter has been doubled

}mppt_data;


const double Isc = 440e-3;
const double Voc = 20.1;

const double Imp = 400e-3;
const double Vmp = 17.8;


#endif /* INCLUDE_CUSTOM_MPPT_PNO_H_ */
