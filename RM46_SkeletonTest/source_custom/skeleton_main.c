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
#include "stdio.h"
#include "string.h"
#include "system.h"
#include "time.h"

#include "FreeRTOS.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_queue.h"

#include "ti_fee.h"
#include "reg_het.h"
#include "gio.h"
#include "sci.h"
#include "can.h"
#include "i2c.h"
#include "mibspi.h"
#include "sys_mpu.h"
#include "sys_selftest.h"

#include "task_header.h"
#include "ina226.h"
#include "ina3221.h"
#include "max6698.h"
#include "mppt.h"
#include "channel.h"
#include "battery.h"
#include "ad5324.h"
#include "fee_function.h"
#include "uart_cmd.h"
#include "cmd_interface.h"
#include "realtimeClock.h"

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

static uint64_t RTOS_RunTimeCounter; /* runtime counter, used for configGENERATE_RUNTIME_STATS */

static uint64_t ulTotalTime;

void vPortRTOSRunTimeISR(void);

/*******************************************************/

#define TIMER_ID                1
#define DELAY_10_SECONDS        10000UL /* 1000 ticks per sec on this H/W */
#define DELAY_1_SECOND          1000UL
#define TIMER_CHECK_THRESHOLD   9

/*****************RTC Variables************************/
static RTC realtimeClock;
static RTC *prealtimeClock = &realtimeClock;
static char temp1[10] = {0};

/*****************FEE Variables************************/
uint8 SpecialRamBlock[128];
uint8 read_data[128]={0};

/*****************Housekeeping Data********************/
static unsigned char command;
static ina226_data ina226D[NUM_OF_INA226+1] = {0};
static ina226_data *pina226D = &ina226D[0];
static uint8_t ina226_counter = 0;

static ina3221_data ina3221D[NUM_OF_INA3221+1] = {0};
static ina3221_data *pina3221D = &ina3221D[0];
static uint8_t ina3221_counter = 0;

static max6698_data max6698D[NUM_OF_MAX6698+1] = {0};
static max6698_data *pmax6698D = &max6698D[0];

/*****************SelfCheck Variables********************/
//static boolean checkFlag[30][2]={0};         // checkFlag[][0]:current flag; checkFlag[][1]:previous flag;
static uint8_t selfCheck_counter = 0;
static uint8_t watchdog_counter = 0;

/*****************Battery Data*************************/
static mppt_data mpptD[4];
static mppt_data *pmpptD = &mpptD[0];
static uint8_t mppt_counter = 0;

static battery_data battD[NUM_OF_BATTERY];
static battery_data *pbattD = &battD[0];
static uint8_t battery_counter = 0;

/*****************Channel Data*************************/
static channel_data channelD[NUM_OF_CHANNELS];
static channel_data *pchannelD = &channelD[0];
static uint8_t channel_counter = 0;

/***************Task Tick Counter**********************/
static uint32_t ch_t = 0;
static uint32_t rxCMD_t = 0;
static uint32_t hk_t = 0;
static uint32_t wdt_t = 0;
static uint32_t batt_t = 0;
static uint32_t heat_t = 0;


static uint8_t delay=0;
//static time_t seconds,seconds2;
//static char temp1[40] = {0}, temp2[40] = {0};


/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    _enable_IRQ();

    checkFlashECC();
    checkFlashEEPROMECC();

    sciInit();
    gioInit();
    i2cInit();
//    canInit();
    mibspiInit();

    xTaskCreate((TaskFunction_t )init_task,
                (const char*    )"init_task",
                (uint16_t       )init_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )init_TASK_PRIO,
                (TaskHandle_t*  )&initTask_Handler);
//    printf("init_task created\r\n");


    resetRTC_debug(prealtimeClock);
    sciSend(scilinREG,19,(unsigned char *)"init_task created\r\n");

//    while((sciIsRxReady(scilinREG) == 0));
//    while(sciIsIdleDetected(scilinREG) != 0);

//    seconds = time(NULL);
//    while((sciIsRxReady(scilinREG) == 0));
//    while(sciIsIdleDetected(scilinREG) != 0);

//    xQueue = xQueueCreate(1, sizeof(unsigned char*));

//    configASSERT( xQueue );

    /*Run time stats ISR*/


    vTaskStartScheduler();


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void init_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    uint32_t BlockNumber;
    uint32_t BlockOffset, Length;
    uint8_t *Read_Ptr=read_data;

//    seconds = time(NULL);

    for(delay=0;delay<100;delay++);
    uint32_t loop;



    /* Initialize RAM array.*/
    for(loop=0;loop<128;loop++)SpecialRamBlock[loop] = loop;

//    fee_initial();
//
//    /*read and write to flash */
//    BlockNumber = 1;
//    fee_write(BlockNumber, SpecialRamBlock);
//
//    /* Read the block with unknown length */
//    BlockNumber = 1;
//    BlockOffset = 0;
//    Length = 0xFFFF;
//    fee_read(BlockNumber, BlockOffset, Read_Ptr, Length);


//    printf("Fee read\n");
    sciSend(scilinREG,10,(unsigned char *)"Fee read\r\n");
    for(delay=0;delay<100;delay++);


    /* Temp. default data (should be removed when fee works)*/
//    uint16_t ina226_config_data=0x4127;
//    uint16_t ina226_cal_data=0x0A00;
//    uint16_t ina3221_config_data=0x7127;

    /*Initialize DAC*/
    for(mppt_counter=0;mppt_counter<1;mppt_counter++)
    {
        mpptD[mppt_counter].channel = mppt_counter;
        mpptD[mppt_counter].counter = 0;
        mpptD[mppt_counter].dir = 0xFF;
        mpptD[mppt_counter].predir = 0xFF;
        mpptD[mppt_counter].stepsize = 64;
        mpptD[mppt_counter].dacOUT = 500;
        mpptD[mppt_counter].prePower = 0;
        mpptD[mppt_counter].power = 0;
    }

    /*Initialize INA226*/
    for(ina226_counter=0;ina226_counter<NUM_OF_INA226;ina226_counter++)
    {
        ina226D[ina226_counter].address = INA226_ADDR1;
        ina226D[ina226_counter].config_reg = INA226_CFG_SETTING;
//        INA226_Init(i2cREG1, ina226D[ina3221_counter].address, pina226D+ina226_counter);
    }

    /*Initialize INA3221*/
    for(ina3221_counter=0;ina3221_counter<NUM_OF_INA3221;ina3221_counter++)
    {
        ina3221D[ina3221_counter].address = INA3221_ADDR1;
        ina3221D[ina3221_counter].config_reg = INA3221_CFG_SETTING;
//        INA3221_Init(i2cREG1, ina3221D[ina3221_counter].address, pina3221D+ina3221_counter);
    }

    /*Initialize MAX6698*/
    max6698D[0].address = MAX6698_ADDR1;
    max6698D[0].config_reg[0] = MAX6698_CFG1_SETTING;
    max6698D[0].config_reg[1] = MAX6698_CFG2_SETTING;
    max6698D[0].config_reg[2] = MAX6698_CFG3_SETTING;
//    MAX6698_Init(i2cREG1, max6698D[0].address, pmax6698D);


    /*Initialize battery*/
    for(battery_counter=0;battery_counter<NUM_OF_BATTERY;battery_counter++)
    {
        battD[battery_counter].num = battery_counter+1;
        battD[battery_counter].sw = battery_counter;
        battD[battery_counter].temp_charge = battery_counter;
        battD[battery_counter].temp_discharge = battery_counter;
        battD[battery_counter].maxV = 8400;    //mV
   }


    /*Initialize channel*/
    for(channel_counter=0;channel_counter<NUM_OF_CHANNELS;channel_counter++)
    {
        channelD[channel_counter].num = channel_counter+1;
        channelD[channel_counter].sw = 0;
        channelD[channel_counter].group = (0x1<<channel_counter);
//        channelD[channel_counter].maxV = 5*1e3;    //mV
//        channelD[channel_counter].maxI = 1*1e3;    //mA
   }


    xTaskCreate((TaskFunction_t )channelCtrl_task,
                (const char*    )"channelCtrl_task",
                (uint16_t       )channelCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )channelCtrl_TASK_PRIO,
                (TaskHandle_t*  )&channelCtrlTask_Handler);
    sciSend(scilinREG,22,(unsigned char *)"Channel task created\r\n");


    xTaskCreate((TaskFunction_t )receiveCMD_task,
                (const char*    )"receiveCMD_task",
                (uint16_t       )receiveCMD_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )receiveCMD_TASK_PRIO,
                (TaskHandle_t*  )&receiveCMDTask_Handler);
    sciSend(scilinREG,25,(unsigned char *)"receiveCMD task created\r\n");


//    xTaskCreate((TaskFunction_t )executeCMD_task,
//                (const char*    )"executeCMD_task",
//                (uint16_t       )executeCMD_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )executeCMD_TASK_PRIO,
//                (TaskHandle_t*  )&executeCMDTask_Handler);
//    sciSend(scilinREG,25,(unsigned char *)"executeCMD task created\r\n");


    xTaskCreate((TaskFunction_t )getHK_task,
                (const char*    )"getHK_task",
                (uint16_t       )getHK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )getHK_TASK_PRIO,
                (TaskHandle_t*  )&getHKTask_Handler);
    sciSend(scilinREG,20,(unsigned char *)"getHK task created\r\n");


    xTaskCreate((TaskFunction_t )selfCheck_task,
                (const char*    )"selfCheck_task",
                (uint16_t       )selfCheck_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )selfCheck_TASK_PRIO,
                (TaskHandle_t*  )&selfCheckTask_Handler);
    sciSend(scilinREG,24,(unsigned char *)"selfCheck task created\r\n");


    xTaskCreate((TaskFunction_t )watchdog_task,
                (const char*    )"watchdog_task",
                (uint16_t       )watchdog_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )watchdog_TASK_PRIO,
                (TaskHandle_t*  )&watchdogTask_Handler);
    sciSend(scilinREG,23,(unsigned char *)"watchdog task created\r\n");


    xTaskCreate((TaskFunction_t )heatCtrl_task,
                (const char*    )"heatCtrl_task",
                (uint16_t       )heatCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )heatCtrl_TASK_PRIO,
                (TaskHandle_t*  )&heatCtrlTask_Handler);
    sciSend(scilinREG,34,(unsigned char *)"heater controlling task created\r\n");


    xTaskCreate((TaskFunction_t )battCtrl_task,
                (const char*    )"battCtrl_task",
                (uint16_t       )battCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )battCtrl_TASK_PRIO,
                (TaskHandle_t*  )&battCtrlTask_Handler);
    sciSend(scilinREG,34,(unsigned char *)"battery controlling task created\r\n");


//    seconds = time(NULL);
//    sprintf(temp1,"%d",(uint32_t)seconds/60);


    vTaskDelete(initTask_Handler);

    taskEXIT_CRITICAL();

}



void getHK_task(void *pvParameters)
{
//    printf( "getHK task running\n");

    const portTickType xDelay = pdMS_TO_TICKS(1000);

    while(1)
    {
        /* call ina226 functions */
        for(ina226_counter=0;ina226_counter<NUM_OF_INA226;ina226_counter++)
        {
            ina226D[ina226_counter].shunt_voltage = ina226_counter;
            ina226D[ina226_counter].bus_voltage = ina226_counter;
//            INA226_GetShuntVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].shunt_voltage);
//            INA226_GetVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].bus_voltage);

//            ina226D[ina226_counter].timestamp_sec = getcurrTime(prealtimeClock);
        }

        /* call ina3221 functions */
        for(ina3221_counter=0;ina3221_counter<NUM_OF_INA3221;ina3221_counter++)
        {

            ina3221D[ina3221_counter].shunt_voltage[0] = ina3221_counter;
            ina3221D[ina3221_counter].bus_voltage[0] = ina3221_counter;

            ina3221D[ina3221_counter].shunt_voltage[2] = ina3221_counter*3;
            ina3221D[ina3221_counter].bus_voltage[2] = ina3221_counter*3;

//            INA3221_GetShuntVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].shunt_voltage[0], 1);
//            INA3221_GetVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].bus_voltage[0], 1);
//
//            INA3221_GetShuntVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].shunt_voltage[2], 3);
//            INA3221_GetVoltage(i2cREG1, ina3221D[ina3221_counter].address, &ina3221D[ina3221_counter].bus_voltage[2], 3);

//            ina3221D[ina3221_counter].timestamp_sec = getcurrTime(prealtimeClock);
        }



//        printf("Number %d sensor updated. Power: %d uW.\n",ina226_counter,(int)ina226D[ina226_counter].power);
        hk_t = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}

void selfCheck_task(void *pvParameters)
{
//    printf( "selfCheck task running\n");

    const portTickType xDelay = pdMS_TO_TICKS(4000);
    static uint32_t preTick[5] = {0};
    char temp[1] = {0};
    char temp1[40] = {0};

    while(1)
    {
        /* check if the flag has been updated */

        /* clear the counter */
        selfCheck_counter = 0;

        if(hk_t != preTick[0])
        {
            preTick[0] = hk_t;
            selfCheck_counter++;
        }
        if(ch_t != preTick[1])
        {
            preTick[1] = ch_t;
            selfCheck_counter++;
        }
        if(wdt_t != preTick[2])
        {
            preTick[2] = wdt_t;
            selfCheck_counter++;
        }
        if(heat_t != preTick[3])
        {
            preTick[3] = heat_t;
            selfCheck_counter++;
        }
        if(batt_t != preTick[4])
        {
            preTick[4] = batt_t;
            selfCheck_counter++;
        }


        if(selfCheck_counter == 5) // all tasks are checked
        {
            /* pet the watchdog timer */
            gioSetBit(hetPORT2,11,1);
            for(delay=0;delay<100;delay++);
            gioSetBit(hetPORT2,11,0);

//            printf( "Pet the watchdog\n");
            while (sciIsTxReady == 0);
            sciSend(scilinREG,18,(unsigned char *)"Pet the watchdog\r\n");

        }
        else
        {
            sprintf(temp,"%d",(int)selfCheck_counter);
            while (sciIsTxReady == 0);
            sciSend(scilinREG,15,(unsigned char *)"Working tasks: ");
            sciSend(scilinREG,strlen((const char *)temp),(unsigned char *)temp);
            sciSend(scilinREG,4,(unsigned char *)"\r\n\r\n");
        }


//        sciSend(scilinREG,18,(unsigned char *)"Pet the watchdog\r\n");
//        for(delay=0;delay<100;delay++);

        #if configGENERATE_RUN_TIME_STATS
            ulTotalTime = portGET_RUN_TIME_COUNTER_VALUE(); /* get total time passed in system */
        #endif

//        sprintf(temp1,"%d",(int)ulTotalTime);
//        sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
//        sciSend(scilinREG,2,(unsigned char *)"\r\n");


        vTaskDelay(xDelay);

    }
}

void watchdog_task(void *pvParameters)
{
//    printf( "selfCheck task running\n");

    const portTickType xDelay = pdMS_TO_TICKS(1000);

    uint16_t rst = 0;
    char temp1[20] = {0};
    char pcWriteBuffer[100] = {0};

//    char temp1[40] = {0};


    while(1)
    {
        if(watchdog_counter>4)
        {
//            seconds2 = time(NULL);
            while (sciIsTxReady == 0);
            sciSend(scilinREG,23,(unsigned char *)"Failed to contact OBC\r\n");
            watchdog_counter=0;

            rst = (uint16_t)systemREG1->SYSESR;
            sprintf(temp1,"%d",(int)rst);
            sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
            sciSend(scilinREG,2,(unsigned char *)"\r\n");

//            printf("seconds since January 1, 1970 = %d\n", seconds);

        }
        watchdog_counter++;

        wdt_t = (uint32_t)xTaskGetTickCount();

//        vTaskGetRunTimeStats(pcWriteBuffer);
//        sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
//        sciSend(scilinREG,2,(unsigned char *)"\r\n");


        vTaskDelay(xDelay);

    }
}


void channelCtrl_task(void *pvParameters)
{
//    printf( "Channel task running\n");

    const portTickType xDelay = pdMS_TO_TICKS(100);
    vTaskDelay(xDelay);

    while(1)
    {
        channel_check_lowVoltage(pina3221D, pchannelD);
        channel_check_trip(pina226D+9, pchannelD);

        ch_t = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}

void heatCtrl_task(void *pvParameters)
{
//    printf( "Heater task running\n");

    const portTickType xDelay = pdMS_TO_TICKS(2000);
    vTaskDelay(xDelay);

    while(1)
    {

        heat_t = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}


void battCtrl_task(void *pvParameters)
{
//    printf( "battery controlling task running\n");

    const portTickType xDelay = pdMS_TO_TICKS(MPPT_TASK_DELAY);
    vTaskDelay(xDelay);


    while(1)
    {

        /*EN_pin Controlling*/
        for(mppt_counter=0;mppt_counter<NUM_OF_INA3221;mppt_counter++)
        {

            (pmpptD+mppt_counter)->power = 0;

            mppt_getPower_ina226(pina226D+mppt_counter, pmpptD+mppt_counter);

            mppt_pno_en(pmpptD+mppt_counter);

            dac_write_en(mibspiPORT3,pmpptD+mppt_counter);
         }


        for(battery_counter=0;battery_counter<NUM_OF_BATTERY;battery_counter++)
        {
            battery_compareVI(pina226D+7, pbattD+battery_counter);
            battery_compareT(pmax6698D, pbattD+battery_counter);
        }


        batt_t = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);

    }
}

void receiveCMD_task(void *pvParameters)
{
    unsigned char *cmd1;
    uint32_t current_sec = 0;

    const portTickType xDelay = pdMS_TO_TICKS(100);
    while(1)
    {

        cmd1 = NULL;
        while (sciIsTxReady == 0);
        cmd1 = uart_tx(20,(unsigned char*)"\r\nWaiting for Command:\r\n");

//        xQueueSendToBack(xQueue, cmd1, 0UL);

        if(strcmp((const char *)cmd1, (const char *)"get_hk_bc")==0)
        {
            get_hk_bc(pina3221D);
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_hk_batt")==0)
        {
            get_hk_batt(pina226D+7, pbattD);
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_hk_ch")==0)
        {
            get_hk_channel(pina226D+9, pchannelD);
        }
        else if(strcmp((const char *)cmd1, (const char *)"reset")==0)
        {
            systemREG1->SYSECR = 0x8000;
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_time")==0)
        {
            current_sec = getcurrTime(prealtimeClock);
            temp1[0] = '0'+ current_sec/1000000000;
            temp1[1] = '0'+ current_sec%1000000000/100000000;
            temp1[2] = '0'+ current_sec%1000000000%100000000/10000000;
            temp1[3] = '0'+ current_sec%1000000000%100000000%10000000/1000000;
            temp1[4] = '0'+ current_sec%1000000000%100000000%10000000%1000000/100000;
            temp1[5] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000/10000;
            temp1[6] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000/1000;
            temp1[7] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000%1000/100;
            temp1[8] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000%1000%100/10;
            temp1[9] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000%1000%100%10;

            sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);


            sciSend(scilinREG,4,(unsigned char *)"\r\n\r\n");

        }
        else
        {
            sciSend(scilinREG,15,(unsigned char *)"Wrong command\r\n");
        }

        vTaskDelay(xDelay);
    }

}

void RTOS_AppConfigureTimerForRuntimeStats(void)
{
        RTOS_RunTimeCounter = 0;
}

uint32_t RTOS_AppGetRuntimeCounterValueFromISR(void)
{
       return RTOS_RunTimeCounter;
}


void vPortRTOSRunTimeISR(void)
{
       /* Clear interrupt flag.*/
//       rtiREG1->INTFLAG = 2U;
      *((volatile uint32_t *) 0xFFFFFC88) = 2U;
      RTOS_RunTimeCounter++;    /* increment runtime counter */
}

//void executeCMD_task(void *pvParameters)
//{
//    unsigned char *cmd1;
//
//    xQueueReceive(xQueue, cmd1, portMAX_DELAY);
//
//    while(1)
//    {
//
//        vTaskDelay(xDelay);
//    }
//
//}


//void esmGroup1Notification(int bit)
//{
//    return;
//}
//
//void esmGroup2Notification(int bit)
//{
//    return;
//}

/* USER CODE END */

