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
#include "sci.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_queue.h"

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


#define TIMER_ID                1
#define DELAY_10_SECONDS        10000UL /* 1000 ticks per sec on this H/W */
#define DELAY_1_SECOND          1000UL
#define TIMER_CHECK_THRESHOLD   9

static unsigned char command;

/* The Tx and Rx tasks as described at the top of this file. */
/* These function prototypes must be defined here for type checking to pass
 later when the main() function refers to them. */

static void prvTxTask( void *pvParameters );
static void prvRxTask( void *pvParameters );
static void prvTimerCallback( TimerHandle_t pxTimer );

/* Declare the global variables and constants that will be shared by the tasks.
*/
static TaskHandle_t xTxTask;
static TaskHandle_t xRxTask;
static QueueHandle_t xQueue = NULL;
static TimerHandle_t xTimer = NULL;
char HWstring[15] = "Hello World";      /* Test string to be displayed. */
long RxTaskCntr = 0;                    /* Count of the number of strings received from queue. */

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    _enable_IRQ();
    sciInit();
    sciSend(scilinREG,14,(unsigned char *)"Hello World!\r\n");

    const TickType_t x10seconds = pdMS_TO_TICKS( DELAY_10_SECONDS );
    printf( "Hello from FreeRTOS example main\r\n" );

    /* Create the two tasks. The Tx task is given a lower priority than the
     Rx task, so the Rx task will leave the Blocked state and pre-empt the Tx
     task as soon as the Tx task places an item in the queue. */

    xTaskCreate( prvTxTask,                 /* The function that implements the task. */
                 ( const char * ) "Tx",     /* Text name for the task, for debugging. */
                 configMINIMAL_STACK_SIZE,  /* The stack allocated to the task. */
                 NULL,                      /* The task parameter is not used, so set to NULL. */
                 tskIDLE_PRIORITY,          /* The task runs at the idle priority. */
                 &xTxTask );

    xTaskCreate( prvRxTask,
                 ( const char * ) "Rx",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 tskIDLE_PRIORITY + 1,      /* Ensures that prvRxTask preempts prvTxTask */
                 &xRxTask );
    printf( "Task Created\r\n" );

    /* Create the queue used by the tasks. The Rx task has a higher priority
    than the Tx task, so will preempt the Tx task and remove values from the
    queue as soon as the Tx task writes to the queue - therefore the queue can
    never have more than one item in it. */

    xQueue = xQueueCreate( 1,               /* There is only one space in the queue. */
                           sizeof( HWstring ) );
    printf( "Queue Created\r\n" );

    /* Each space in the queue is large enough to hold a uint32_t. */
    /* Check the queue was created. */

    configASSERT( xQueue );

    /* Create a timer with a timer expiry of 10 seconds. The timer would expire
    after 10 seconds and the timer call back would get called. In the timer call back
    checks are done to ensure that the tasks have been running properly till then.
    The tasks are deleted in the timer call back and a message is printed to convey
     that the example has run successfully.
    The timer expiry is set to 10 seconds and the timer set to not auto reload. */

    xTimer = xTimerCreate( (const char *) "Timer",
                           x10seconds,
                           pdFALSE,
                           (void *) TIMER_ID,
                           prvTimerCallback);
    printf( "Timer Created\r\n" );

    /* Check the timer was created. */

    configASSERT( xTimer );

    /* Start the timer with a block time of 0 ticks. This means as soon
     as the schedule starts the timer will start running and will expire after
     10 seconds. */

    xTimerStart( xTimer, 0 );
    printf( "Timer started\r\n" );
    /* Start the tasks and timer running. */

    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following line
     will never be reached. If the following line does execute, then there was
     insufficient FreeRTOS heap memory available for the idle and/or timer tasks
     to be created. See the memory management section on the FreeRTOS web site
     for more details. */

    for ( ;; );

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

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

static void prvTxTask( void *pvParameters )
{

    const TickType_t x1second = pdMS_TO_TICKS( DELAY_1_SECOND );

    for ( ;; ) {
        printf( "In transmitting task\r\n" );

        /* Delay for 1 second. */
        vTaskDelay( x1second );

        /* Send the next value on the queue. The queue should always be
         empty at this point so a block time of 0 is used. */
        xQueueSendToBack(
                xQueue,                         /* The queue being written to. */
                HWstring,                       /* The address of the data being sent. */
                0UL );                          /* The block time. */
    } /* end for */
}

static void prvRxTask( void *pvParameters )
{
    char Recdstring[15] = ""; /* Instantiate a buffer for the received string. */
    for ( ;; ) {
        printf( "In receiving task\r\n" );

        /* Block to wait for data arriving on the queue. */
        xQueueReceive( xQueue,                  /* The queue being read. */
                       Recdstring,              /* Data is read into this address. */
                       portMAX_DELAY );         /* Wait without a timeout for data. */

        /* Print the received data. */
        printf( "Rx task received string from Tx task: %s\r\n",
                    Recdstring );
        RxTaskCntr++;
    } /* end for */
}

static void prvTimerCallback( TimerHandle_t pxTimer )
{
    long lTimerId;
    configASSERT( pxTimer );

    lTimerId = ( long ) pvTimerGetTimerID( pxTimer );

    if (lTimerId != TIMER_ID) {
        printf("FreeRTOS Hello World Example FAILED");
    }

    /* If the RxtaskCntr is updated every time the Rx task is called. The
     Rx task is called every time the Tx task sends a message. The Tx task
     sends a message every 1 second.
     The timer expires after 10 seconds. We expect the RxtaskCntr to at least
     have a value of 9 (TIMER_CHECK_THRESHOLD) when the timer expires. */

    if (RxTaskCntr >= TIMER_CHECK_THRESHOLD) {
        printf("Successfully ran FreeRTOS Hello World Example");
    } else {
        printf("FreeRTOS Hello World Example FAILED");
    }

    vTaskDelete( xRxTask );
    vTaskDelete( xTxTask );
}

/* USER CODE END */
