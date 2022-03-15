
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

#define Vout_high   1495         // D = (Vout * (2^N))/Vref = (1.204V * (2^12))/3.3V = 1494.42 => 1495
#define Vout_low    496         // D = (Vout * (2^N))/Vref = (0.4V * (2^12))/3.3V = 496.48 => 496

static spiDAT1_t dataconfig1_t = {FALSE,TRUE,SPI_FMT_0,0xFE};


void dac_write_en(spiBASE_t *spi, mppt_data *data);
void dc2output_en(uint16 * srcbuff, uint16 dc);


#endif /* INCLUDE_AD5324_H_ */
