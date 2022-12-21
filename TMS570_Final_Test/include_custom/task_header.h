

#include "FreeRTOS.h"


#ifndef SOURCE_TASK_HEADER_H_
#define SOURCE_TASK_HEADER_H_

#define init_TASK_PRIO         14       //The init_task should be lower than getHK task. It is in critical section besides creating getHK task.
#define init_STK_SIZE         128
TaskHandle_t initTask_Handle;
void init_task(void *pvParameters);


#define receiveCMD_TASK_PRIO         18
#define receiveCMD_STK_SIZE         128
TaskHandle_t receiveCMDTask_Handle;
void receiveCMD_task(void *pvParameters);


#define executeCMD_TASK_PRIO         18
#define executeCMD_STK_SIZE         128
TaskHandle_t executeCMDTask_Handle;
void executeCMD_task(void *pvParameters);

#define getHK_TASK_PRIO         16          //Must be higher than all the other controlling tasks
#define getHK_STK_SIZE         128
TaskHandle_t getHKTask_Handle;
void getHK_task(void *pvParameters);

#define checkActive_TASK_PRIO         15
#define checkActive_STK_SIZE         128
TaskHandle_t checkActiveTask_Handle;
void check_other_tasks_activity_task(void *pvParameters);


#define heaterCtrl_TASK_PRIO         12
#define heaterCtrl_STK_SIZE         128
TaskHandle_t heaterCtrlTask_Handle;
void heaterCtrl_task(void *pvParameters);


#define powerConversion_and_battCtrl_TASK_PRIO         10
#define powerConversion_and_battCtrl_STK_SIZE         256
TaskHandle_t powerConversion_and_battCtrlTask_Handle;
void powerConversion_and_battCtrl_task(void *pvParameters);


#define outputchanCtrl_TASK_PRIO         6
#define outputchanCtrl_STK_SIZE         128
TaskHandle_t outputchanCtrlTask_Handle;
void outputchanCtrl_task(void *pvParameters);





/*Repetition period of each task*/
#define GET_HK_TASK_PERIOD          100    //ms. Must be longer than sensors' conversion time: ina226/ina3221-16ms, max6698-31ms
#define CHECK_ACTIVE_TASK_PERIOD    10      //ms.

#define CHAN_CTRL_TASK_PERIOD       100     //ms. Determined by the recovery delay of task
#define HEATER_CTRL_TASK_PERIOD     500     //ms.
#define POWER_CONV_TASK_PERIOD      200     //ms.
#define RECEIVE_CMD_TASK_PERIOD     100     //ms.


#define NUM_OF_WDT_TIMEOUT     6


#define TASK_INIT           1
#define TASK_RECEIVE_CMD    2
#define TASK_CHECK_ACTIVE   3
#define TASK_GET_HK         4
#define TASK_HEATER_CTRL    5
#define TASK_POWER_CONV     6
#define TASK_CHAN_CTRL      7


#define TASK_INIT_WDT           1
#define TASK_GET_HK_WDT         2
#define TASK_RECEIVE_CMD_WDT    3
#define TASK_CHECK_ACTIVE_WDT   4
#define TASK_CHAN_CTRL_WDT      6
#define TASK_HEATER_CTRL_WDT    7
#define TASK_POWER_CONV_WDT     8




//...
#endif /* SOURCE_TASK_HEADER_H_ */
