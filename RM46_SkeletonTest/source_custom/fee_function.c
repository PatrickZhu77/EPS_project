

#include "ti_fee.h"
#include "sys_common.h"

#include "fee_function.h"

static Std_ReturnType oResult=E_OK;
static uint16 u16JobResult,Status;

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


void fee_write(uint16_t BlockNumber, uint8_t *pData)
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


void fee_read(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData, uint16_t Length)
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
