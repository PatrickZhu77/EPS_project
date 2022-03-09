/*
 * task_header.h
 *
 *  Created on: Dec 2, 2021
 *      Author: sdamkjar
 */

#include "FreeRTOS.h"


#ifndef SOURCE_TASK_HEADER_H_
#define SOURCE_TASK_HEADER_H_

#define init_TASK_PRIO         2
#define init_STK_SIZE         128
TaskHandle_t initTask_Handler;
void init_task(void *pvParameters);


#define getHK_TASK_PRIO         2
#define getHK_STK_SIZE         128
TaskHandle_t getHKTask_Handler;
void getHK_task(void *pvParameters);


#define selfCheck_TASK_PRIO         2
#define selfCheck_STK_SIZE         128
TaskHandle_t selfCheckTask_Handler;
void selfCheck_task(void *pvParameters);


#define mppt_TASK_PRIO         2
#define mppt_STK_SIZE         128
TaskHandle_t mpptTask_Handler;
void mppt_task(void *pvParameters);


#define channelCtrl_TASK_PRIO         2
#define channelCtrl_STK_SIZE         128
TaskHandle_t channelCtrlTask_Handler;
void channelCtrl_task(void *pvParameters);


#define battCtrl_TASK_PRIO         2
#define battCtrl_STK_SIZE         128
TaskHandle_t battCtrlTask_Handler;
void battCtrl_task(void *pvParameters);


#define heatCtrl_TASK_PRIO         2
#define heatCtrl_STK_SIZE         128
TaskHandle_t heatCtrlTask_Handler;
void heatCtrl_task(void *pvParameters);


#define bcCtrl_TASK_PRIO         2
#define bcCtrl_STK_SIZE         128
TaskHandle_t bcCtrlTask_Handler;
void bcCtrl_task(void *pvParameters);

#define receiveCMD_TASK_PRIO         2
#define receiveCMD_STK_SIZE         128
TaskHandle_t receiveCMDTask_Handler;
void receiveCMD_task(void *pvParameters);



#endif /* SOURCE_TASK_HEADER_H_ */
