
#ifndef INCLUDE_AD5324_H_
#define INCLUDE_AD5324_H_


#include "mppt.h"


#define AD5324_Mask     0x0FFF

#define dac_OUT_A   0
#define dac_OUT_B   1
#define dac_OUT_C   2
#define dac_OUT_D   3

#define LDAC_low        2       //All 4 outputs update simultaneously
#define LDAC_high       3       //Only addressed input register is updated

#define V1_2   1494         // D = (Vout * (2^N))/Vref = (1.204V * (2^12))/3.3V = 1494.42 => 1495
#define V0_4    496         // D = (Vout * (2^N))/Vref = (0.4V * (2^12))/3.3V = 496.48 => 496

//void dac_write_en(spiBASE_t *spi, mppt_data *data);
void dac_write_ss(gioPORT_t *spi, mppt_data *data);
void dac_write_en(gioPORT_t *spi, mppt_data *data);
void dac_write_en_ss(gioPORT_t *spi,uint16_t value);
void get_binary(uint16_t data, uint8_t out_channel);

#endif /* INCLUDE_AD5324_H_ */
