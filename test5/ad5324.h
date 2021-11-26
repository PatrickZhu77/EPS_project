#ifndef AD5324_H_
#define AD5324_H_

#include <stdint.h>

// The ADC chip has four output channels
typedef enum
{
    ADC_OUT_A = 0,
    ADC_OUT_B = 1,
    ADC_OUT_C = 2,
    ADC_OUT_D = 3
} adc_channel_t;

// remember the state of the selected SPI device
typedef enum
{
    IDLE,
    TRANSMITTING
} spi_activity_t;


void AD5324_SpiRegConfig(boolean CS_HOLD, boolean WDEL, SPIDATAFMT_t DFSEL, uint8 CSNR);
void AD5324_SendData(spiBASE_t *spi, adc_channel_t channel, uint16_t value);


#endif /* AD5324_H_ */
