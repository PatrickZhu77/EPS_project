
#include "sci.h"
#include "sys_common.h"
#include "string.h"

#include "uart_cmd.h"



static unsigned char command;
static unsigned char cmd1[20];
static uint8_t len, counter;
static int flag1;

unsigned char * uart_tx(uint8_t length,uint8 * cmd2)
{
    int i;
    flag1 = 0;
    counter = 0;
    //cmd1[0]='\0';
    for(i=0;i<strlen((const char *)cmd1);i++)
    {
        cmd1[i]='\0';
    }

    sciSend(scilinREG,strlen((const char*)cmd2),cmd2);

    len = length;
    sciReceive(scilinREG,1,(unsigned char *)&command);


//      sciReceive(scilinREG,1,(unsigned char *)&cmd1);

    while(flag1 == 0){vTaskDelay(100);}
    flag1 = 0;
    len = 1;
    counter = 0;

//    sciSend(scilinREG,17,(unsigned char *)"Command in cmd1: ");
//    sciSend(scilinREG,strlen((const char *)cmd1),(unsigned char *)&cmd1);
//    sciSend(scilinREG,2,(unsigned char *)"\r\n");

    sciSend(scilinREG,2,(unsigned char *)"\r\n");

    //printf("Command in cmd1: %s\n", cmd1);


    return cmd1;

}

//unsigned char * tx_cmd_nonlen()
//{
//    flag1 = 0;
//    counter = 0;
//    cmd1[0]='\0';
//
//    sciSend(scilinREG,21,(unsigned char *)"Please press a key!\r\n");
//
//    len = length;
//    sciReceive(scilinREG,1,(unsigned char *)&command);
//
////      sciReceive(scilinREG,1,(unsigned char *)&cmd1);
//
//    while(flag1 == 0){}
//    flag1 = 0;
//    len = 1;
//    counter = 0;
//
////    sciSend(scilinREG,17,(unsigned char *)"Command in cmd1: ");
////    sciSend(scilinREG,strlen((const char *)cmd1),(unsigned char *)&cmd1);
////    sciSend(scilinREG,2,(unsigned char *)"\r\n");
//
//    sciSend(scilinREG,7,(unsigned char *)"Done!\r\n");
//
//    //printf("Command in cmd1: %s\n", cmd1);
//
//
//    return cmd1;
//
//}


void sciNotification(sciBASE_t *sci,unsigned flags)
{
    cmd1[counter]=command;
    sciSend(sci,1,(unsigned char *)&command);
    sciReceive(sci,1,(unsigned char *)&command);
    counter++;

    if(counter >= len||command=='\r')
    {
        cmd1[counter-1] = '\0';
        sciSend(sci,2,(unsigned char *)"\r\n");
        flag1 = 1;
        return;
    }
    //sciReceive(sci,1,(unsigned char *)&command);
    //sciSend(scilinREG,2,(unsigned char *)" ");

}
