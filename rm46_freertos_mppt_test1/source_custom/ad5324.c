
#include "sys_common.h"
#include "system.h"

#include "ad5324.h"
#include "gio.h"
#include "mibspi.h"
#include "het.h"
#include "mppt.h"

static uint16_t binary[16] = {0};

void get_binary(uint16_t data)
{

    uint16_t i;
    for(i=0; i<12; i++)
    {
        uint16_t mask =  1 << i;
        uint16_t masked = data & mask;
        uint16_t bit = masked >> i;
        binary[15-i] = bit;
    }

    binary[0] = 0;
    binary[1] = 1;
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
//void dac_write_en(spiBASE_t *spi, mppt_data *data)
//{
//    uint16_t Vsrcbuff[2] = {0};
//    uint32_t Vout = 0;
//
//    spiDAT1_t dataconfig1_t;
//
//    dataconfig1_t.CS_HOLD = FALSE;
//    dataconfig1_t.WDEL    = TRUE;
//    dataconfig1_t.DFSEL   = SPI_FMT_0;
//    dataconfig1_t.CSNR    = 0xFE;
//
//    /*Determine the EN pin output*/
//    Vout = (6150-data->preV)*1000-data->increment*806;      //uV     step size of Vdac=0.806mV
//    Vsrcbuff[0] = Vout/806;
//
//    Vsrcbuff[0] = Vsrcbuff[0] & AD5324_Mask;
//    Vsrcbuff[0] = Vsrcbuff[0] | (dac_OUT_B << 14) | (LDAC_low << 12);
//
//
//    gioSetDirection(hetPORT2, 0x10040);            //0x40 -> 100_0000, set N2HET2[6] and N2HET2[16] as output, others for input
//    gioToggleBit(hetPORT2, 0x10040);
//
//    gioSetBit(hetPORT2,6,0);
//
//    spiTransmitData(spi, &dataconfig1_t, 1, Vsrcbuff);
//
//    gioSetBit(hetPORT2,6,1);
//
//    /*Determine the SS pin output*/
//    Vsrcbuff[1] = V1_2;
//
//    Vsrcbuff[1] = Vsrcbuff[1] & AD5324_Mask;
//    Vsrcbuff[1] = Vsrcbuff[1] | (dac_OUT_A << 14) | (LDAC_high << 12);
//
//
//    gioSetBit(hetPORT2,6,0);
//
//    spiTransmitData(spi, &dataconfig1_t, 1, &Vsrcbuff[1]);
//
//    gioSetBit(hetPORT2,6,1);
//
//}

void dac_write_ss(gioPORT_t *spi, mppt_data *data)
{
    uint16_t t;

//    gioSetDirection(hetPORT2, 0x40); //0x40 -> 100_0000, set N2HET2[6] as output, others for input
//    gioToggleBit(hetPORT2, 0x40);

    gioToggleBit(spi, 0x600);
    gioSetDirection(spi,0x600); //0x600 -> 110_0000_0000, set port 9(CLK) and port 10(MOSI) as output, others for input


//    uint16_t sample = 0x6FFF;
    get_binary(data->dacOUT);

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
