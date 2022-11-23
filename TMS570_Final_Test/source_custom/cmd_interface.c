
#include "cmd_interface.h"

//static uint16 oResult, Status;
//static uint16_t BlockOffset, Length;
//static uint8 read_data[100]={0};


//void get_default()
//{
//    uint16_t bnum;
//    uint16_t *data = 0;
//
//    BlockOffset = 0;
//    Length = 0xFFFF;
//
//    unsigned char *blocknum = uart_tx(20,(unsigned char*)"Please input the blocknumber:\r\n");
//
//    bnum = atoi((const char *)blocknum);
//    //printf("bnum is %d\n",bnum);
//
////   oResult=TI_Fee_Read(bnum,BlockOffset,&read_data[0],Length);
////
////    do
////    {
////        TI_Fee_MainFunction();
////        delay();
////        Status=TI_Fee_GetStatus(0);
////    }
////   while(Status!=IDLE);
//
//    char *Result= "123";
//
//
//    sciSend(scilinREG,30,(unsigned char *)"Data is read from the block:\r\n");
//    sciSend(scilinREG,strlen((const char*)Result),(unsigned char *)Result);
//    sciSend(scilinREG,2,(unsigned char *)"\r\n");
//
//}

void get_hk_bc(ina3221_housekeeping_t *data)
{
    char temp1[10] = {0}, temp2[10] = {0};
    char temp3[5] = {0};

    uint8_t i;

    for(i=0;i<NUM_OF_INA3221;i++)
    {
        sprintf(temp1,"%d",(int)(data+i)->bus_voltage[0]);
        sprintf(temp3,"%d",i);

        sciSend(scilinREG,9,(unsigned char *)"Vin of PV");
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        sciSend(scilinREG,2,(unsigned char *)": ");
        sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
        sciSend(scilinREG,1,(unsigned char *)"\t");

        sprintf(temp2,"%d",(int)(data+i)->bus_voltage[1]);

        sciSend(scilinREG,10,(unsigned char *)"Vout of PV");
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        sciSend(scilinREG,2,(unsigned char *)": ");
        sciSend(scilinREG,strlen((const char *)temp2),(unsigned char *)temp2);

        sciSend(scilinREG,2,(unsigned char *)"\r\n");
    }
}

void get_hk_batt(ina226_housekeeping_t *data, battery_data_t *battery)
{
    char temp1[10] = {0}, temp2[10] = {0}, temp4[5] = {0};
    char temp3[5] = {0};

    uint8_t i;


    for(i=0;i<NUM_OF_BATTERY_PAIR;i++)
    {
        sprintf(temp1,"%d",(int)(data+i)->bus_voltage);
        sprintf(temp3,"%d",i);

        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,18,(unsigned char *)"Voltage of Battery");
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,2,(unsigned char *)": ");
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,1,(unsigned char *)"\t");

        sprintf(temp4,"%d",(int)(battery+i)->sw);

        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,17,(unsigned char *)"Switch of Battery");
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,2,(unsigned char *)": ");
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,strlen((const char *)temp4),(unsigned char *)temp4);
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,1,(unsigned char *)"\t");

//        sprintf(temp2,"%d",(int)(battery+i)->temp_charge);

        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,16,(unsigned char *)"Temp. of Battery");
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,2,(unsigned char *)": ");
        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,strlen((const char *)temp2),(unsigned char *)temp2);

        while ((scilinREG->FLR & 0x4) == 4);
        sciSend(scilinREG,2,(unsigned char *)"\r\n");
    }

}

void get_hk_channel(ina226_housekeeping_t *data,channel_data_t *channel)
{
    char temp1[20] = {0}, temp2[20] = {0};
    char temp3[5] = {0};

    uint8_t i;


    for(i=0;i<NUM_OF_CHANNELS;i++)
    {
        sprintf(temp1,"%d",(int)(data+i)->bus_voltage);
        sprintf(temp2,"%d",(int)(channel+i)->sw);
        sprintf(temp3,"%d",i+1);

        sciSend(scilinREG,17,(unsigned char *)"Switch of Channel");
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        sciSend(scilinREG,2,(unsigned char *)": ");
        sciSend(scilinREG,strlen((const char *)temp2),(unsigned char *)temp2);
        sciSend(scilinREG,1,(unsigned char *)"\t");


        sciSend(scilinREG,18,(unsigned char *)"Voltage of Channel");
        sciSend(scilinREG,strlen((const char *)temp3),(unsigned char *)temp3);
        sciSend(scilinREG,2,(unsigned char *)": ");
        sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
        sciSend(scilinREG,1,(unsigned char *)"\t");


        sciSend(scilinREG,2,(unsigned char *)"\r\n");
    }

}
