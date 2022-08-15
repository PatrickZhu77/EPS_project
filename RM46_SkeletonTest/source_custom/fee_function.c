

#include "fee_function.h"
#include "ti_fee.h"
#include "sys_common.h"


static Std_ReturnType oResult=E_OK;
static uint16 Status;

void delay(void)
{
    unsigned int dummycnt=0x0000FFU;
    do
    {
        dummycnt--;
    }
    while(dummycnt>0);
}


void fee_initial(void)
{
    TI_Fee_Init();
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0 );
    }
    while(Status!= IDLE);

}


void fee_write_8bit(uint16_t BlockNumber, uint8_t *pData)
{
    TI_Fee_WriteAsync(BlockNumber, pData);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

}

void fee_write_16bit(uint16_t BlockNumber, uint16_t *pData, uint8_t Length)
{
    uint8_t converted_data[Length*2];
    uint8_t *pCdata = converted_data;

    uint8_t i = 0;
    for(i=0;i<Length;i++)
    {
        *(pCdata+i*2) = (*(pData + i)) << 8;
        *(pCdata+(i*2+1)) = *(pData + i);
    }

    TI_Fee_WriteAsync(BlockNumber, pCdata);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

}


void fee_read_8bit(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData, uint16_t Length)
{
    oResult=TI_Fee_Read(BlockNumber,BlockOffset,pData,Length);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

}

void fee_read_16bit(uint16_t BlockNumber, uint16_t BlockOffset, uint16_t *pData, uint16_t Length)
{
    uint8_t converted_data[(uint8_t)Length*2];
    uint8_t *pCdata = converted_data;

    oResult=TI_Fee_Read(BlockNumber,BlockOffset,pCdata,Length);
    do
    {
        TI_Fee_MainFunction();
        delay();
        Status=TI_Fee_GetStatus(0);
    }
    while(Status!=IDLE);

    uint8_t i = 0;
    for(i=0;i<Length;i++)
    {

        *(pData + i) = (*(pCdata+i*2) << 8) | (*(pCdata+(i*2+1)));
    }

}

