

#include "FreeRTOS.h"


#ifndef SOURCE_TASK_HEADER_H_
#define SOURCE_TASK_HEADER_H_

#define init_TASK_PRIO         18
#define init_STK_SIZE         128
TaskHandle_t initTask_Handle;
void init_task(void *pvParameters);


#define getHK_TASK_PRIO         14
#define getHK_STK_SIZE         128
TaskHandle_t getHKTask_Handle;
void getHK_task(void *pvParameters);


#define checkActive_TASK_PRIO         4
#define checkActive_STK_SIZE         128
TaskHandle_t checkActiveTask_Handle;
void check_other_tasks_activity_task(void *pvParameters);

//#define watchdog_TASK_PRIO         2
//#define watchdog_STK_SIZE         128
//TaskHandle_t watchdogTask_Handle;
//void watchdog_task(void *pvParameters);


#define outputchanCtrl_TASK_PRIO         6
#define outputchanCtrl_STK_SIZE         128
TaskHandle_t outputchanCtrlTask_Handle;
void outputchanCtrl_task(void *pvParameters);


#define powerConvertion_and_battCtrl_TASK_PRIO         10
#define powerConvertion_and_battCtrl_STK_SIZE         128
TaskHandle_t powerConvertion_and_battCtrlTask_Handle;
void powerConvertion_and_battCtrl_task(void *pvParameters);


#define heaterCtrl_TASK_PRIO         12
#define heaterCtrl_STK_SIZE         128
TaskHandle_t heaterCtrlTask_Handle;
void heaterCtrl_task(void *pvParameters);


#define receiveCMD_TASK_PRIO         19
#define receiveCMD_STK_SIZE         128
TaskHandle_t receiveCMDTask_Handle;
void receiveCMD_task(void *pvParameters);


#define executeCMD_TASK_PRIO         15
#define executeCMD_STK_SIZE         128
TaskHandle_t executeCMDTask_Handle;
void executeCMD_task(void *pvParameters);



#endif /* SOURCE_TASK_HEADER_H_ */
