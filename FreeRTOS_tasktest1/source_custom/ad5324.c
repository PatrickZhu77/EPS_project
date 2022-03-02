
#include "sys_common.h"
#include "system.h"
#include "spi.h"

#include "ad5324.h"
#include "gio.h"
#include "reg_het.h"
#include "mppt.h"

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
    uint16 * srcbuff = 0;

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

    dc2output_en(srcbuff, dutycycle);

    *srcbuff = (*srcbuff) & AD5324_Mask;
    *srcbuff = (*srcbuff) | (dac_OUT_A << 14) | (LDAC_high << 12);

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = TRUE;
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0xFE;

    gioSetBit(hetPORT2,6,0);

    spiSendData(spiREG2, &dataconfig1_t, 1, srcbuff);

    //maybe need a wait function here to confirm spi_tx success?

    gioSetBit(hetPORT2,6,1);
}

