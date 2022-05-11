
#include "sys_common.h"
#include "system.h"

#include "ad5324.h"
#include "gio.h"
#include "mppt.h"

static uint16_t binary[16] = {0};

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
 *   Generate real output for DAC5324 in EN controlling method
 *
 *  @param[in] spi
 *   Pointer to SPI peripheral register block.
 *
 * @param[in] data
 *   Pointer to mppt data.
 *
 ******************************************************************************/
void dac_write_en(gioPORT_t *spi, mppt_data *data)
{
    uint16_t t;

//    gioSetDirection(hetPORT2, 0x40); //0x40 -> 100_0000, set N2HET2[6] as output, others for input
//    gioToggleBit(hetPORT2, 0x40);

    gioToggleBit(spi, 0x600);
    gioSetDirection(spi,0x600); //0x600 -> 110_0000_0000, set port 9(CLK) and port 10(MOSI) as output, others for input


//    uint16_t sample = 0x6FFF;
    get_binary(data->dacOUT,dac_OUT_A);

    uint16_t i;

    gioSetBit(spi,9,0);
    gioSetBit(spi,9,1);
    gioSetBit(gioPORTA,7,0);
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

    gioSetBit(gioPORTA,7,1);

}

void dac_write_en_ss(gioPORT_t *spi,uint16_t value)
{
    uint16_t t;

//    gioSetDirection(hetPORT2, 0x40); //0x40 -> 100_0000, set N2HET2[6] as output, others for input
//    gioToggleBit(hetPORT2, 0x40);

    gioToggleBit(spi, 0x600);
    gioSetDirection(spi,0x600); //0x600 -> 110_0000_0000, set port 9(CLK) and port 10(MOSI) as output, others for input


//    uint16_t sample = 0x6FFF;
    get_binary(value,dac_OUT_B);

    uint16_t i;

    gioSetBit(spi,9,0);
    gioSetBit(spi,9,1);
    gioSetBit(gioPORTA,7,0);
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

    gioSetBit(gioPORTA,7,1);

}


void dac_write_ss(gioPORT_t *spi, mppt_data *data)
{
    uint16_t t;

//    gioSetDirection(hetPORT2, 0x40); //0x40 -> 100_0000, set N2HET2[6] as output, others for input
//    gioToggleBit(hetPORT2, 0x40);

    gioToggleBit(spi, 0x600);
    gioSetDirection(spi,0x600); //0x600 -> 110_0000_0000, set port 9(CLK) and port 10(MOSI) as output, others for input


//    uint16_t sample = 0x6FFF;
    get_binary(data->dacOUT,dac_OUT_B);

    uint16_t i;

    gioSetBit(spi,9,0);
    gioSetBit(spi,9,1);
    gioSetBit(gioPORTA,7,0);
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

    gioSetBit(gioPORTA,7,1);

}

