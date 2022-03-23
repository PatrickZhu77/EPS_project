/** @file sys_main.c
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/*
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "system.h"
#include "gio.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "sci.h"
#include "stdio.h"
#include "os_timer.h"
#include "os_queue.h"
#include "ti_fee.h"
#include "reg_het.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"

#include "task_header.h"
#include "ina226.h"
#include "ina3221.h"
#include "mppt.h"
#include "channel.h"
#include "battery.h"
#include "ad5324.h"


/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

#define TIMER_ID                1
#define DELAY_10_SECONDS        10000UL /* 1000 ticks per sec on this H/W */
#define DELAY_1_SECOND          1000UL
#define TIMER_CHECK_THRESHOLD   9


/*****************FEE Variables************************/
uint16 u16JobResult,Status;
Std_ReturnType oResult=E_OK;
unsigned char read_data[100]={0};

uint8 SpecialRamBlock[100];

//unsigned char pattern;
//uint16 u16writecounter;
//
//unsigned int  FeeVirtualSectorNumber;
//unsigned char VsState, u8EEPIndex;
//unsigned char u8VirtualSector;

/*****************Housekeeping Data********************/
static unsigned char command;
static ina226_data ina226D[26] = {0};
static ina226_data *pina226D = &ina226D[0];
static uint8_t ina226_counter = 0;

static ina3221_data ina3221D[4] = {0};
static ina3221_data *pina3221D = &ina3221D[0];
static uint8_t ina3221_counter = 0;

/*****************SelfCheck Variables********************/
static boolean checkFlag[30][2]={0};         // checkFlag[][0]:current flag; checkFlag[][1]:previous flag;
static uint8_t selfCheck_counter = 0;

/*****************Battery Data*************************/
static mppt_data mpptD[4];
static mppt_data *pmpptD = &mpptD[0];
static uint8_t mppt_counter = 0;

static battery_data battD[4];
static battery_data *pbattD = &battD[0];
static uint8_t battery_counter = 0;

/*****************Channel Data*************************/
static channel_data channelD[16];
static channel_data *pchannelD = &channelD[0];
static uint8_t channel_counter = 0;



void delay(void)
{
    unsigned int dummycnt=0x0000FFU;
    do
    {
        dummycnt--;
    }
    while(dummycnt>0);
}


/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    _enable_IRQ();
    sciInit();
    gioInit();
    i2cInit();
//    canInit();
    spiInit();

    xTaskCreate((TaskFunction_t )init_task,
                (const char*    )"init_task",
                (uint16_t       )init_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )init_TASK_PRIO,
                (TaskHandle_t*  )&initTask_Handler);
    printf("init_task created\n");


    vTaskStartScheduler();


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void init_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    /*read and write to flash */
//    unsigned int BlockNumber;
//    unsigned int BlockOffset, Length;
//    unsigned char *Read_Ptr=read_data;
//
//    unsigned int loop;
//
//    /* Read the block with unknown length */
//     BlockOffset = 0;
//     Length = 0xFFFF;
//     oResult=TI_Fee_Read(BlockNumber,BlockOffset,Read_Ptr,Length);
//     do
//     {
//         TI_Fee_MainFunction();
//         delay();
//         Status=TI_Fee_GetStatus(0);
//     }
//    while(Status!=IDLE);
//     printf("Fee read\n");
//
//     printf("readdata= %d\n", (unsigned int)read_data[7]);
//
//
//    /* Invalidate a written block  */
////    TI_Fee_InvalidateBlock(BlockNumber);
////    do
////    {
////        TI_Fee_MainFunction();
////        delay();
////        Status=TI_Fee_GetStatus(0);
////    }
////    while(Status!=IDLE);
//
//    /* Format bank 7 */
//    //TI_Fee_Format(0xA5A5A5A5U);


    /* Temp. default data (should be removed when fee works)*/
    uint16_t ina226_config_data=0x4127;
    uint16_t ina226_cal_data=0x0A00;
    uint16_t ina3221_config_data=0x7127;


    /*Initialize sensors*/
    for(ina226_counter=0;ina226_counter<26;ina226_counter++)
    {
        ina226D[ina226_counter].address = INA226_ADDR1;
        ina226D[ina226_counter].config_reg = ina226_config_data;
        ina226D[ina226_counter].cal_reg = ina226_cal_data;
        INA226_Init(i2cREG1, ina226D[ina226_counter].address, pina226D+ina226_counter);
    }

    for(ina3221_counter=0;ina3221_counter<4;ina3221_counter++)
    {
        ina3221D[ina3221_counter].address = INA3221_ADDR1;
        ina3221D[ina3221_counter].config_reg = ina3221_config_data;
        INA3221_Init(i2cREG1, ina3221D[ina3221_counter].address, pina3221D+ina3221_counter);
    }

    /*Initialize created data structures*/
    for(mppt_counter=0;mppt_counter<4;mppt_counter++)
    {
        mpptD[mppt_counter].channel = mppt_counter;
        mpptD[mppt_counter].counter = 0;
        mpptD[mppt_counter].dir = 1;
        mpptD[mppt_counter].increment = 1;
        mpptD[mppt_counter].preP = 6*1e6;  //uW (~6W)
        mpptD[mppt_counter].preV = 5*1e3;   //mV (~5V)
    }

    for(channel_counter=0;channel_counter<16;channel_counter++)
    {
        channelD[channel_counter].address = INA226_ADDR1;
        channelD[channel_counter].num = channel_counter+1;
        channelD[channel_counter].sw = 0;
        channelD[channel_counter].maxV = 5*1e3;    //mV
        channelD[channel_counter].maxI = 1*1e3;    //mA
   }

    for(battery_counter=0;battery_counter<16;battery_counter++)
    {
        battD[battery_counter].address = INA226_ADDR1;
        battD[battery_counter].num = battery_counter+1;
        battD[battery_counter].sw = 0;
        battD[battery_counter].maxV = 8.4*1e3;    //mV
        battD[battery_counter].maxI = 1.5*1e3;    //mA
   }


    xTaskCreate((TaskFunction_t )channelCtrl_task,
                (const char*    )"channelCtrl_task",
                (uint16_t       )channelCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )channelCtrl_TASK_PRIO,
                (TaskHandle_t*  )&channelCtrlTask_Handler);

//    xTaskCreate((TaskFunction_t )receiveCMD_task,
//                (const char*    )"receiveCMD_task",
//                (uint16_t       )receiveCMD_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )receiveCMD_TASK_PRIO,
//                (TaskHandle_t*  )&receiveCMDTask_Handler);

    xTaskCreate((TaskFunction_t )getHK_task,
                (const char*    )"getHK_task",
                (uint16_t       )getHK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )getHK_TASK_PRIO,
                (TaskHandle_t*  )&getHKTask_Handler);

    xTaskCreate((TaskFunction_t )selfCheck_task,
                (const char*    )"selfCheck_task",
                (uint16_t       )selfCheck_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )selfCheck_TASK_PRIO,
                (TaskHandle_t*  )&selfCheckTask_Handler);

    xTaskCreate((TaskFunction_t )battCtrl_task,
                (const char*    )"battCtrl_task",
                (uint16_t       )battCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )battCtrl_TASK_PRIO,
                (TaskHandle_t*  )&battCtrlTask_Handler);

    vTaskDelete(initTask_Handler);

    taskEXIT_CRITICAL();

}



void getHK_task(void *pvParameters)
{
    printf( "getHK task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(10);
    while(1)
    {
        /* call ina226 functions */
        for(ina226_counter=0;ina226_counter<26;ina226_counter++)
        {
            INA226_GetShuntVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].shunt_voltage);
            INA226_GetVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].bus_voltage);
            INA226_GetCalReg(i2cREG1, ina226D[ina226_counter].address, &ina226D[ina226_counter].calibration);
            INA226_GetCurrent(i2cREG1, ina226D[ina226_counter].address, &ina226D[ina226_counter].current);
            INA226_GetPower(i2cREG1, ina226D[ina226_counter].address, &ina226D[ina226_counter].power);

            /* inverse flag */
            ina226D[ina226_counter].flag = !ina226D[ina226_counter].flag;
        }

        /* call ina3221 functions */
        for(ina3221_counter=0;ina3221_counter<4;ina3221_counter++)
        {
            INA3221_GetShuntVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].shunt_voltage[0], 1);
            INA3221_GetBusVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].bus_voltage[0], 1);
            INA3221_DoCalculation(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter], 1);

            INA3221_GetShuntVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].shunt_voltage[2], 3);
            INA3221_GetBusVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].bus_voltage[2], 3);
            INA3221_DoCalculation(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter], 3);

            /* inverse flag */
            ina3221D[ina3221_counter].flag = !ina3221D[ina3221_counter].flag;
        }


//        printf("Number %d sensor updated. Power: %d uW.\n",ina226_counter,(int)ina226D[ina226_counter].power);

        vTaskDelay(xDelay);

    }

}

void selfCheck_task(void *pvParameters)
{
    printf( "selfCheck task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(10);
    while(1)
    {
        /* check if the flag has been updated */
        checkFlag[selfCheck_counter][0] = ina226D[selfCheck_counter].flag;
        if(checkFlag[selfCheck_counter][0] != checkFlag[selfCheck_counter][1])
        {
            checkFlag[selfCheck_counter][1] = checkFlag[selfCheck_counter][0];
            selfCheck_counter++;
        }

        if(selfCheck_counter == 3) // all the data are checked
        {
            /* pet the watchdog timer */
            gioSetBit(hetPORT2,11,1);
            delay();
            gioSetBit(hetPORT2,11,0);

            printf( "Pet the watchdog\n");

            /* clear the counter */
            selfCheck_counter = 0;
        }


        vTaskDelay(xDelay);

    }
}

//void receiveCMD_task(void *pvParameters)
//{
//    printf( "receiveCMD task running\n");
//    const portTickType xDelay = pdMS_TO_TICKS(1000);
//    while(1) //while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX1))
//    {
//        uint8  rx_data = 1;
//        //canGetData(canREG2, canMESSAGE_BOX1, rx_data);  /* receive on can1  */
//        switch(rx_data)
//        {
//            case 1:
//                xQueueSend(xQueue_channel,channelSW,0);
//                printf( "cmd sent to channelCtrl task\n");
//                break;
//            case 2:
//                //xQueueSend(xQueue_battery,channelSW,0);
//                printf( "cmd sent to channelCtrl task\n");
//                break;
//            case 3:
//                //xQueueSend(xQueue_heater,channelSW,0);
//                printf( "cmd sent to channelCtrl task\n");
//                break;
//            default:
//                break;
//        }
//
//        vTaskDelay(xDelay);
//    }
//}

void channelCtrl_task(void *pvParameters)
{
    printf( "Channel task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(100);

    while(1)
    {
        for(channel_counter=0;channel_counter<16;channel_counter++)
        {
            channel_compare((pina226D+9)+channel_counter,pchannelD+channel_counter);
            channel_switch(pchannelD+channel_counter);
        }

        vTaskDelay(xDelay);
    }
}

void battCtrl_task(void *pvParameters)
{
    printf( "battery controlling task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(100);

    while(1)
    {
        for(mppt_counter=0;mppt_counter<4;mppt_counter++)
        {
            mppt_pno(pina226D+mppt_counter,pmpptD+mppt_counter);
        }

//        printf( "Mppt result for channel %d: direction:%d, increment:%d.\n",mppt_counter,mpptD[mppt_counter].dir,(int)mpptD[mppt_counter].increment);

        for(battery_counter=0;battery_counter<4;battery_counter++)
        {
            battery_compareVI(pmpptD+battery_counter,pbattD+battery_counter);
            battery_switch(pbattD+battery_counter);
            dac_write_en(spiREG3,pmpptD+battery_counter);
        }

        vTaskDelay(xDelay);

    }

}


void sciNotification(sciBASE_t *sci,unsigned flags)
{
    sciSend(sci,1,(unsigned char *)&command);
    sciReceive(sci,1,(unsigned char *)&command);
}

void esmGroup1Notification(int bit)
{
    return;
}

void esmGroup2Notification(int bit)
{
    return;
}

/* USER CODE END */

