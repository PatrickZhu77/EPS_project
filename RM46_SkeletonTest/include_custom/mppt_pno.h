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

}mppt_data;

const double Isc_15 = 1406e-3;   //A
const double Voc_15 = 5.42;      //V

const double Imp_15 = 1352e-3;   //A
const double Vmp_15 = 4.87;       //V



const double Isc_28 = 1418e-3;   //A
const double Voc_28 = 5.27;      //V

const double Imp_28 = 1359e-3;   //A
const double Vmp_28 = 4.7;       //V


const double Isc_75 = 1462e-3;   //A
const double Voc_75 = 4.72;      //V

const double Imp_75 = 1384e-3;   //A
const double Vmp_75 = 4.09;       //V




#endif /* INCLUDE_CUSTOM_MPPT_PNO_H_ */
