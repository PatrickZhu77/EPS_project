
#ifndef INCLUDE_AD5324_H_
#define INCLUDE_AD5324_H_


#include "spi.h"
#include "mppt.h"

#define AD5324_Mask     0x0FFF

#define dac_OUT_A   0
#define dac_OUT_B   1
#define dac_OUT_C   2
#define dac_OUT_D   3

#define LDAC_low        2       //All 4 outputs update simultaneously
#define LDAC_high       3       //Only addressed input register is updated

void dac_write(spiBASE_t *spi, uint16 * srcbuff, mppt_data *data);


#endif /* INCLUDE_AD5324_H_ */
