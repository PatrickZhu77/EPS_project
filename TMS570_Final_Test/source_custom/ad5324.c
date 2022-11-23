
#include "sys_common.h"
#include "system.h"

#include "ad5324.h"

static uint16_t binary[16] = {0};

/***************************************************************************
 * @brief
 *   Internal function used during data conversion process
 *
 * @param[in] data
 *   Data to be sent
 *
 * @param[in] out_channel
 *   Output channel number of the DAC
 *
 ******************************************************************************/
void get_binary(uint16_t data, uint8_t out_channel)
{

    uint16_t i;
    for(i=0; i<12; i++)
    {
        uint16_t mask =  1 << i;
        uint16_t masked = data & mask;
        uint16_t bit = masked >> i;
        binary[15-i] = bit;
    }

    binary[0] = out_channel >> 1;
    binary[1] = out_channel & 0x1;
    binary[2] = 1;
    binary[3] = 0;

}


/***************************************************************************
 * @brief
 *   Generate real output for DAC5324 in FB controlling method
 *
 *  @param[in] spi
 *   Pointer to SPI peripheral register block.
 *
 * @param[in] data
 *   Pointer to mppt data.
 *
 ******************************************************************************/
void dac_write_fb(gioPORT_t *spi, mppt_data_t *data, uint8_t out_channel)
{
    uint16_t t;


    gioToggleBit(spi, 0x600);
    gioSetDirection(spi,0x600); //0x600 -> 110_0000_0000, set port 9(CLK) and port 10(MOSI) as output, others for input  //according to "mibspi.h"


    get_binary(data->dacOUT, out_channel);

    uint16_t i;

    gioSetBit(spi,9,0);
    gioSetBit(spi,9,1);


    gioSetBit(hetPORT1,16,0);           //N2HET2_16 is the enable pin (sync) of the DAC. It is default to be one and active low.
    for(t=0;t<0x80;t++);

    for(i=0;i<16;i++)
    {
        gioSetBit(spi,10,binary[i]);
        for(t=0;t<0x80;t++);
        gioSetBit(spi,9,0);
        for(t=0;t<0x80;t++);

        gioSetBit(spi,9,1);
        for(t=0;t<0x80;t++);

    }

    gioSetBit(hetPORT1,16,1);

}

