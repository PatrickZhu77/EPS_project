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
#include "uart_cmd.h"
#include "string.h"
#include "cmdList.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
//task priority
#define START_TASK_PRIO         1
//stack size
#define START_STK_SIZE         128

TaskHandle_t StartTask_Handler;

void start_task(void *pvParameters);

#define LED0_TASK_PRIO         2

#define LED0_STK_SIZE         50

TaskHandle_t LED0Task_Handler;

void led0_task(void *pvParameters);

#define LED1_TASK_PRIO         3

#define LED1_STK_SIZE         50

TaskHandle_t LED1Task_Handler;

void led1_task(void *pvParameters);

#define CMD_TASK_PRIO         2

#define CMD_STK_SIZE         128

TaskHandle_t CMDTask_Handler;

void cmd_task(void *pvParameters);


//static unsigned char command;
unsigned char *cmd1;
unsigned char *cmd2 = "123456";
//static uint8_t len, counter;
//static int flag1;


/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */


    _enable_IRQ();
    sciInit();
    gioInit();

    sciSend(scilinREG,36,(unsigned char *)"Console Command Interface Started!\r\n");


#if 1
    xTaskCreate((TaskFunction_t )start_task,
                (const char*    )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*  )&StartTask_Handler);

#endif

    vTaskStartScheduler();

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
#if 1
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_TASK_PRIO,
                (TaskHandle_t*  )&LED0Task_Handler);

    xTaskCreate((TaskFunction_t )cmd_task,
                (const char*    )"cmd_task",
                (uint16_t       )CMD_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CMD_TASK_PRIO,
                (TaskHandle_t*  )&CMDTask_Handler);


#endif

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();

}

void led0_task(void *pvParameters)
{
    const portTickType xDelay = pdMS_TO_TICKS(500);
    while(1)
    {
        //sciSend(scilinREG,3,(unsigned char *)"0\r\n");
        gioSetBit(gioPORTB,1,1);
        vTaskDelay(xDelay);
        gioSetBit(gioPORTB,1,0);
        vTaskDelay(xDelay);
    }
}

void led1_task(void *pvParameters)
{
    unsigned int task1_num = 0;

    const portTickType xDelay = pdMS_TO_TICKS(1000);
    while(1)
    {
        sciSend(scilinREG,3,(unsigned char *)"1\r\n");
        task1_num++;
        if(task1_num==10)
        {
            gioSetBit(gioPORTB,2,0);
            vTaskDelete(LED1Task_Handler);
        }
        gioSetBit(gioPORTB,2,1);
        vTaskDelay(xDelay);
    }
}

void cmd_task(void *pvParameters)
{
    const portTickType xDelay = pdMS_TO_TICKS(1000);
    while(1)
    {
        cmd1 = NULL;
        cmd1 = uart_tx(20,(unsigned char*)"Please input your command:\r\n");

        sciSend(scilinREG,17,(unsigned char *)"Command Received: ");
        sciSend(scilinREG,strlen((const char*)cmd1),(unsigned char *)cmd1);
        sciSend(scilinREG,2,(unsigned char *)"\r\n");


        if(strcmp((const char *)cmd1, (const char *)"set_default")==0)
        {
            //sciSend(scilinREG,17,(unsigned char *)"cmd is the same\r\n");
            set_default();
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_default")==0)
        {
            get_default();
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_hk")==0)
        {

        }
        else if(strcmp((const char *)cmd1, (const char *)"set_battery")==0)
        {

        }
        else
        {
            sciSend(scilinREG,15,(unsigned char *)"Wrong command\r\n");
        }

        vTaskDelay(xDelay);
    }

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
