/*
 * cmdList.c
 *
 *  Created on: Feb 16, 2022
 *      Author: sdamkjar
 */

#include "ti_fee.h"
#include "sci.h"
#include "sys_common.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "cmdList.h"
#include "uart_cmd.h"

static uint16 oResult, Status;
static unsigned int BlockOffset, Length;
static uint8 read_data[100]={0};

void delay(void)
{
    unsigned int dummycnt=0x0000FFU;
    do
    {
        dummycnt--;
    }
    while(dummycnt>0);
}


void set_default()
{
    unsigned int bnum;
    unsigned int *data = 0;

    unsigned char *blocknum = uart_tx(20,(unsigned char*)"Please input the blocknumber:\r\n");

    bnum = atoi((const char *)blocknum);
    //printf("bnum is %d\n",bnum);

    unsigned char *blockdata = uart_tx(20,(unsigned char*)"Please input the data:\r\n");

    //data[0] = atoi((const char *)blockdata);

//    TI_Fee_Init();
//    do
//    {
//        TI_Fee_MainFunction();
//        delay();
//        Status=TI_Fee_GetStatus(0);
//    }
//    while(Status!= IDLE);
//
//
//    TI_Fee_WriteAsync(bnum, (uint8*)data[0]);
//    do
//    {
//        TI_Fee_MainFunction();
//        delay();
//        Status=TI_Fee_GetStatus(0);
//    }
//    while(Status!=IDLE);

    sciSend(scilinREG,28,(unsigned char *)"Data is saved in the block\r\n");
}

void get_default()
{
    unsigned int bnum;
    unsigned int *data = 0;

    BlockOffset = 0;
    Length = 0xFFFF;

    unsigned char *blocknum = uart_tx(20,(unsigned char*)"Please input the blocknumber:\r\n");

    bnum = atoi((const char *)blocknum);
    //printf("bnum is %d\n",bnum);

//   oResult=TI_Fee_Read(bnum,BlockOffset,&read_data[0],Length);
//
//    do
//    {
//        TI_Fee_MainFunction();
//        delay();
//        Status=TI_Fee_GetStatus(0);
//    }
//   while(Status!=IDLE);

    char *Result= "123";


    sciSend(scilinREG,30,(unsigned char *)"Data is read from the block:\r\n");
    sciSend(scilinREG,strlen((const char*)Result),(unsigned char *)Result);
    sciSend(scilinREG,2,(unsigned char *)"\r\n");

}
