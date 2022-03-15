
#include "sys_common.h"
#include "system.h"
#include "spi.h"

#include "ad5324.h"
#include "gio.h"
#include "reg_het.h"
#include "mppt.h"
#include "het.h"

static uint16 counter = 0;
static uint16 dutycycle, outputV;

void dc2output_en(uint16 * srcbuff, uint16 dc)
{
    if(counter < 100)
    {
        if(counter < dc)
        {
            *srcbuff = Vout_high;
            counter++;
        }
        else
        {
            *srcbuff = Vout_low;
            counter++;
        }
    }
    else
    {
        *srcbuff = Vout_low;
        counter = 0;
    }


}


void dac_write_en(spiBASE_t *spi, mppt_data *data)
{
    uint16 srcbuff[1];

    gioSetBit(hetPORT2,6,1);

/*****************Determine the output**********************/
    if(data->dir == 1)
    {
       dutycycle = dutycycle + data->increment;

    }
    else
    {
       dutycycle = dutycycle - data->increment;

    }
/******************************************************/

    dc2output_en(srcbuff[0], dutycycle);

    srcbuff[0] = srcbuff[0] & AD5324_Mask;
    srcbuff[0] = srcbuff[0] | (dac_OUT_A << 14) | (LDAC_high << 12);

    gioSetDirection(hetPORT2, 0x40); //0x40 -> 100_0000, set N2HET2[6] as output, others for input
    gioToggleBit(hetPORT2, 0x40);

    uint16_t p[1]={0xABC};
    gioSetBit(hetPORT2,6,0);

    spiTransmitData(spiREG3, &dataconfig1_t, 1, srcbuff);

    gioSetBit(hetPORT2,6,1);

}

