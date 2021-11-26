#include "ad5324.h"
#include "spi.h"



static uint16_t AD5324_RESOLUTION = 0x0FFF;     // Bit mask for the chip's resolution
static spiDAT1_t *config = 0;


/***************************************************************************//**
 * @brief
 *   Configure spi DAT1 register.
 *
 * @param[in] CS_HOLD
 *   Chip select hold mode.
 *
 * @param[in] WDEL
 *   Enable the delay counter at the end of the current transaction.
 *
 * @param[in] DFSEL
 *   Data word format select.
 *
 * @param[in] CSNR
 *   Chip select (CS) number.
 ******************************************************************************/
void AD5324_SpiRegConfig(boolean CS_HOLD, boolean WDEL, SPIDATAFMT_t DFSEL, uint8 CSNR)
{
    config -> CS_HOLD = CS_HOLD;
    config -> WDEL = WDEL;
    config -> DFSEL = DFSEL;
    config -> CSNR = CSNR;
}


/***************************************************************************//**
 * @brief
 *   Write data to dac.
 *
 * @param[in] spi
 *   Spi module base address.
 *
 * @param[in] channel
 *   Channel to output.
 *
 * @param[in] value
 *   Value to be sent.
 *
 ******************************************************************************/
void AD5324_SendData(spiBASE_t *spi, adc_channel_t channel, uint16_t value)
{
    // trim value according to chip resolution
    value  &= AD5324_RESOLUTION;

    value  |= (channel << 14);      // select channel
    value |= (1 << 12);             // set nLDAC: only update addressed register
    value |= (1 << 13);             // no power-down (nPD)

    spiSendData(spi, config, 16, &value);

    // for 1 MHz: 14us
    // for 2 MHz: 5us
    // for 4 MHz: 1us
    // 8 MHz: too fast for the DAC (or DAC test setup)
    delay_us(1);

 }
