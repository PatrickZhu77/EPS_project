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
//#include "os_projdefs.h"

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
#define START_STK_SIZE         512

TaskHandle_t StartTask_Handler;

void start_task(void *pvParameters);

#define tx1_TASK_PRIO         3

#define tx1_STK_SIZE         128

TaskHandle_t tx1Task_Handler;

void tx1_task(void *pvParameters);

#define rx_TASK_PRIO         4

#define rx_STK_SIZE         128

TaskHandle_t rxTask_Handler;

void rx_task(void *pvParameters);

#define tx2_TASK_PRIO         2

#define tx2_STK_SIZE         128

TaskHandle_t tx2Task_Handler;

void init_task(void *pvParameters);

#define init_TASK_PRIO         2

#define init_STK_SIZE         128

TaskHandle_t initTask_Handler;

void init_task(void *pvParameters);

void getHK_task(void *pvParameters);

#define getHK_TASK_PRIO         2

#define getHK_STK_SIZE         128

TaskHandle_t getHKTask_Handler;

void selfCheck_task(void *pvParameters);

#define selfCheck_TASK_PRIO         2

#define selfCheck_STK_SIZE         128

TaskHandle_t selfCheckTask_Handler;

void mppt_task(void *pvParameters);

#define mppt_TASK_PRIO         2

#define mppt_STK_SIZE         128

TaskHandle_t mpptTask_Handler;


void selfCheck_task(void *pvParameters);



#define TIMER_ID                1
#define DELAY_10_SECONDS        10000UL /* 1000 ticks per sec on this H/W */
#define DELAY_1_SECOND          1000UL
#define TIMER_CHECK_THRESHOLD   9

static QueueHandle_t xQueue = NULL;
static TimerHandle_t xTimer = NULL;
char HWstring[15] = "Hello World";
char HWstring2[15] = "Hello World2";
long TxTaskCntr, RxTaskCntr = 0;

static unsigned char command;

static void prvTimerCallback( TimerHandle_t pxTimer );


/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    _enable_IRQ();
    sciInit();
    sciSend(scilinREG,14,(unsigned char *)"Hello World!\r\n");
    printf("Hi!\n");
    //vPortYeildWithinAPI();

    gioInit();

#if 1
    xTaskCreate((TaskFunction_t )start_task,
                (const char*    )"start_task",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*  )&StartTask_Handler);
    printf("Task created\n");

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

    printf("Timer created\n");

    configASSERT( xTimer );


#endif
    xTimerStart( xTimer, 0 );
    printf("Timer started\n");

    vTaskStartScheduler();

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();
#if 1
    xTaskCreate((TaskFunction_t )tx1_task,
                (const char*    )"tx1_task",
                (uint16_t       )tx1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )tx1_TASK_PRIO,
                (TaskHandle_t*  )&tx1Task_Handler);

    xTaskCreate((TaskFunction_t )rx_task,
                (const char*    )"rx_task",
                (uint16_t       )rx_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )rx_TASK_PRIO,
                (TaskHandle_t*  )&rxTask_Handler);

    xTaskCreate((TaskFunction_t )tx2_task,
                (const char*    )"tx2_task",
                (uint16_t       )tx2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )tx2_TASK_PRIO,
                (TaskHandle_t*  )&tx2Task_Handler);

    vTaskSuspend(tx2Task_Handler);

    xQueue = xQueueCreate(1,sizeof(HWstring));
    configASSERT(xQueue);

#endif
    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();

}

void tx1_task(void *pvParameters)
{
    const portTickType xDelay = pdMS_TO_TICKS(1000);
    while(1)
    {
        //sciSend(scilinREG,3,(unsigned char *)"0\r\n");
        /*gioSetBit(gioPORTB,1,1);
        vTaskDelay(xDelay);
        gioSetBit(gioPORTB,1,0);*/
        //vTaskDelay(xDelay);
        printf( "Tx1 task sent string\n");
        xQueueSend(xQueue,HWstring,0);
        TxTaskCntr++;

//        if (TxTaskCntr >= 4) {
//            printf("Tx1_Task interrupted\n");
//            portYIELD_WITHIN_API();
//        }

    }
}

void tx2_task(void *pvParameters)
{
    const portTickType xDelay = pdMS_TO_TICKS(1000);
    while(1)
    {
        //vTaskDelay(xDelay);
        printf( "Tx2 task sent string\n");
        xQueueSend(xQueue,HWstring2,0);
        TxTaskCntr++;

        /*if (TxTaskCntr >= 4) {
            printf("Tx1_Task interrupted");
            portYIELD();
        }*/

    }
}


void rx_task(void *pvParameters)
{
    unsigned int task1_num = 0;
    char receivString[15]="";

    //const portTickType xDelay = pdMS_TO_TICKS(500);
    while(1)
    {
        //sciSend(scilinREG,3,(unsigned char *)"1\r\n");
        /*task1_num++;
        if(task1_num==10)
        {
            gioSetBit(gioPORTB,2,0);
            vTaskDelete(rxTask_Handler);
        }
        gioSetBit(gioPORTB,2,1);
        vTaskDelay(xDelay);*/
        xQueueReceive(xQueue,receivString,portMAX_DELAY);
        RxTaskCntr++;

        printf( "Rx task received string from Tx task: %s\n",receivString );
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

