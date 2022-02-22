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

static QueueHandle_t xQueue_channel = NULL;
static TimerHandle_t xTimer = NULL;
char HWstring[5] = "test1";
char HWstring2[5] = "test2";
unsigned int channelSW[2]= {4,1};
long TxTaskCntr, RxTaskCntr = 0;

/*****************Default Data************************/
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
ina226_data ina226D[3];
ina226_data *pina226D = &ina226D[0];
static boolean checkFlag[3][2]={false}; // checkFlag[][0]:current ina226 flag; checkFlag[][1]:previous ina226 flag;
static uint8_t getHK_counter = 0,selfCheck_counter = 0,mppt_counter = 0;

/*****************MPPT Data*************************/
mppt_data mpptD[4];
mppt_data *pmpptD = &mpptD[0];


static void prvTimerCallback( TimerHandle_t pxTimer );


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
    canInit();
    spiInit();

    //sciSend(scilinREG,14,(unsigned char *)"Hello World!\r\n");

    xTaskCreate((TaskFunction_t )init_task,
                (const char*    )"init_task",
                (uint16_t       )init_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )init_TASK_PRIO,
                (TaskHandle_t*  )&initTask_Handler);
    printf("init_task created\n");

    xTimer = xTimerCreate((const char *)"Timer",
                          pdMS_TO_TICKS(10000),
                          pdFALSE,
                          (void *) TIMER_ID,
                          prvTimerCallback
                          );
    if (xTimer==NULL) {
        printf("Timer creation failed\n");
       for(;;); /* failure! */
    }

    if (xTimerStart(xTimer, 0)!=pdPASS) {

      for(;;); /* failure! */

    }

    configASSERT( xTimer );
    printf("Timer created\n");




    xTimerStart( xTimer, 0 );
    printf("Timer started\n");

    vTaskStartScheduler();


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void init_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    uint8_t config_data[2]={0x41,0x27};
    uint8_t cal_data[2]={0x0A,0x00};
    /*read and write to flash */
//    unsigned int BlockNumber;
//    unsigned int BlockOffset, Length;
//    unsigned char *Read_Ptr=read_data;
//
//    unsigned int loop;
//
//    /* Initialize RAM array.*/
//    for(loop=0;loop<100;loop++)SpecialRamBlock[loop] = loop;
//    printf("readdata= %d\n", (unsigned int)read_data[7]);
//
//    /* Format bank 7 */
//    TI_Fee_Format(0xA5A5A5A5U);
//
//    /* Initialize FEE. This will create Virtual sectors, initialize global variables etc.*/
//    TI_Fee_Init();
//    do
//    {
//        TI_Fee_MainFunction();
//        delay();
//        Status=TI_Fee_GetStatus(0 );
//    }
//    while(Status!= IDLE);
//    printf("Fee initialized\n");
//
//    /* Write the block into EEP Asynchronously. Block size is configured in ti_fee_cfg.c file. Default Block size is
//       8 bytes */
//    BlockNumber=0x1;
//    TI_Fee_WriteAsync(BlockNumber, &SpecialRamBlock[0]);
//    do
//    {
//        TI_Fee_MainFunction();
//        delay();
//        Status=TI_Fee_GetStatus(0);
//    }
//    while(Status!=IDLE);
//
//    /* Write the block into EEP Synchronously. Write will not happen since data is same. */
//    TI_Fee_WriteSync(BlockNumber, &SpecialRamBlock[0]);
//    printf("Fee write\n");
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



    xTaskCreate((TaskFunction_t )channelCtrl_task,
                (const char*    )"channelCtrl_task",
                (uint16_t       )channelCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )channelCtrl_TASK_PRIO,
                (TaskHandle_t*  )&channelCtrlTask_Handler);

    xTaskCreate((TaskFunction_t )receiveCMD_task,
                (const char*    )"receiveCMD_task",
                (uint16_t       )receiveCMD_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )receiveCMD_TASK_PRIO,
                (TaskHandle_t*  )&receiveCMDTask_Handler);

    xTaskCreate((TaskFunction_t )mppt_task,
                (const char*    )"mppt_task",
                (uint16_t       )mppt_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )mppt_TASK_PRIO,
                (TaskHandle_t*  )&mpptTask_Handler);

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

    xTaskCreate((TaskFunction_t )bcCtrl_task,
                (const char*    )"bcCtrl_task",
                (uint16_t       )bcCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )bcCtrl_TASK_PRIO,
                (TaskHandle_t*  )&bcCtrlTask_Handler);


    xQueue_channel = xQueueCreate(1,sizeof(channelSW));
    configASSERT(xQueue_channel);


    for(getHK_counter=0;getHK_counter<3;getHK_counter++)
    {
        INA226_SendData(i2cREG1,INA226_ADDR1, CFG_REG,config_data);
        INA226_SendData(i2cREG1,INA226_ADDR1, CAL_REG,cal_data);
    }
    getHK_counter = 0;

    for(mppt_counter=0;mppt_counter<2;mppt_counter++)
    {
        mpptD[mppt_counter].channel = mppt_counter;
        mpptD[mppt_counter].counter = 0;
        mpptD[mppt_counter].dir = 1;
        mpptD[mppt_counter].preP = 70;  //W Expected power value from the sensor. (~85W)
        mpptD[mppt_counter].preV = 7;   //V Expected voltage value from the sensor. (~8.5V)
    }
    mppt_counter = 0;


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
        INA226_GetShuntVoltage(i2cREG1,INA226_ADDR1,&ina226D[getHK_counter].shunt_voltage);
        INA226_GetVoltage(i2cREG1,INA226_ADDR1,&ina226D[getHK_counter].bus_voltage);
        INA226_GetCalReg(i2cREG1, INA226_ADDR1, &ina226D[getHK_counter].calibration);
        INA226_GetCurrent(i2cREG1, INA226_ADDR1, &ina226D[getHK_counter].current);
        INA226_GetPower(i2cREG1, INA226_ADDR1, &ina226D[getHK_counter].power);

        /* inverse flag */
        ina226D[getHK_counter].flag = !ina226D[getHK_counter].flag;

        printf("Number %d sensor updated. Power: %d uW.\n",getHK_counter,(int)ina226D[getHK_counter].power);

        /* point to next sensor */
        if (getHK_counter < 2)
            getHK_counter++;
        else
            getHK_counter = 0;


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

void receiveCMD_task(void *pvParameters)
{
    printf( "receiveCMD task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(1000);
    while(1) //while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX1))
    {
        uint8  rx_data = 1;
        //canGetData(canREG2, canMESSAGE_BOX1, rx_data);  /* receive on can1  */
        switch(rx_data)
        {
            case 1:
                xQueueSend(xQueue_channel,channelSW,0);
                printf( "cmd sent to channelCtrl task\n");
                break;
            case 2:
                //xQueueSend(xQueue_battery,channelSW,0);
                printf( "cmd sent to channelCtrl task\n");
                break;
            case 3:
                //xQueueSend(xQueue_heater,channelSW,0);
                printf( "cmd sent to channelCtrl task\n");
                break;
            default:
                break;
        }

        vTaskDelay(xDelay);
    }


}

void channelCtrl_task(void *pvParameters)
{
    unsigned int channelSwitch[2] = {0};

    while(1)
    {

        xQueueReceive(xQueue_channel,channelSwitch,portMAX_DELAY);
        gioSetBit(gioPORTA,channelSwitch[0],channelSwitch[1]);

        printf( "channelCtrl task received string from Tx task: channel %d, switch: %d\n",channelSwitch[0], channelSwitch[1]);
    }
}

void mppt_task(void *pvParameters)
{
    printf( "mppt task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(100);
    while(1)
    {
        mppt_pno(pina226D+mppt_counter,pmpptD+mppt_counter);

        printf( "Mppt result for channel %d: direction:%d, increment:%d.\n",mppt_counter,mpptD[mppt_counter].dir,(int)mpptD[mppt_counter].increment);

        mppt_counter++;

        if(mppt_counter == 4)
        {
            mppt_counter = 0;
        }


        vTaskDelay(xDelay);

    }

}

void bcCtrl_task(void *pvParameters)
{
    unsigned int channelSwitch[2] = {0};

    while(1)
    {

        xQueueReceive(xQueue_channel,channelSwitch,portMAX_DELAY);
        gioSetBit(gioPORTA,channelSwitch[0],channelSwitch[1]);

        printf( "channelCtrl task received string from Tx task: channel %d, switch: %d\n",channelSwitch[0], channelSwitch[1]);
    }
}



static void prvTimerCallback( TimerHandle_t pxTimer )
{
    long lTimerId;

    configASSERT( pxTimer );

    lTimerId = ( long ) pvTimerGetTimerID( pxTimer );
    if (lTimerId != TIMER_ID) {
        printf("FreeRTOS Hello World Example FAILED\n");
    }

    /* If the RxtaskCntr is updated every time the Rx task is called. The
     Rx task is called every time the Tx task sends a message. The Tx task
     sends a message every 1 second.

     The timer expires after 10 seconds. We expect the RxtaskCntr to at least
     have a value of 9 (TIMER_CHECK_THRESHOLD) when the timer expires. */
    if (RxTaskCntr >= TIMER_CHECK_THRESHOLD) {
        printf("Successfully ran FreeRTOS Hello World Example\n");
    }
    else {
        printf("FreeRTOS Hello World Example FAILED\n");
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

