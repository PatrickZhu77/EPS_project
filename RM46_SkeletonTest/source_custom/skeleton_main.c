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
#include "rti.h"
#include "sys_mpu.h"
#include "sys_selftest.h"

#include "task_header.h"
#include "ina226.h"
#include "ina3221.h"
#include "max6698.h"
#include "mppt.h"
#include "heater.h"
#include "ad5324.h"
#include "uart_cmd.h"
#include "cmd_interface.h"
#include "realtimeClock.h"
#include "fee_function.h"
#include "channel.h"
#include "battery.h"
#include "flash_data.h"
#include "data_structure_const.h"

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

/*******************************************************/

#define TIMER_ID                1
#define DELAY_10_SECONDS        10000UL /* 1000 ticks per sec on this H/W */
#define DELAY_1_SECOND          1000UL
#define TIMER_CHECK_THRESHOLD   9

/*****************RTC Variables************************/
static RTC_t global_realtimeClock;
static RTC_t *global_prealtimeClock = &global_realtimeClock;
//static char temp1[10] = {0};


/***************FLASH Data***************/
/*Writing Access: init_task, receiveCMD_task*/
/*Reading Access: init_task, outputchanCtrl_task, powerConvertion_and_battCtrl_task, heaterCtrl_task, receiveCMD_task*/

static system_config_t global_flashD = {BATT_CHARGING_TEMP_MIN_C, BATT_CHARGING_TEMP_MAX_C, BATT_DISCHARGING_TEMP_MIN_C, BATT_DISCHARGING_TEMP_MAX_C,
                            HEATER_SUNSHINE_TEMP_ON_C, HEATER_SUNSHINE_TEMP_OFF_C, HEATER_ECLIPSE_TEMP_ON_C, HEATER_ECLIPSE_TEMP_OFF_C,
                            {INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT},
                            {INA226_MONITOR_ALERT}, INA226_SHUNT_RESISTANCE, BATT_CHARGING_CURRENT_LIMIT_mA, BATT_DISCHARGING_CURRENT_LIMIT_mA,
                            HEATER_TUMBLE_THRESHOLD_TIME_S, HEATER_SOLAR_PANEL_THRESHOLD_POWER_S, HEATER_DELAY_TIME_S,
                            DAC_INIT, EN_STEPSIZE_INIT, {0}};
static system_config_t *pglobal_flashD = &global_flashD;

static sensor_config_t global_flash_senser_config = {0, MAX6698_CFG1_SETTING, MAX6698_CFG2_SETTING, MAX6698_CFG3_SETTING,
                                                     INA226_CFG_SETTING, INA226_OVERCURRENT_MASK, INA226_MONITOR_MASK, INA226_BATTERY_MASK, INA226_CHANNEL_MASK,
                                                     INA3221_CFG_SETTING, INA3221_MASK_SETTING};
static sensor_config_t *pglobal_flash_sensor_config = &global_flash_senser_config;

/*******************Sensor Data**********************/
/*Writing Access: init_task, receiveCMD_task, getHK_task*/
/*Reading Access: outputchanCtrl_task, powerConvertion_and_battCtrl_task, heaterCtrl_task, receiveCMD_task*/

static ina226_housekeeping_t global_ina226D[NUM_OF_INA226];
static ina226_housekeeping_t *pglobal_ina226D = &global_ina226D[0];
static uint8_t global_ina226_counter = 0;

static ina3221_housekeeping_t global_ina3221D[NUM_OF_INA3221];
static ina3221_housekeeping_t *pglobal_ina3221D = &global_ina3221D[0];
static uint8_t global_ina3221_counter = 0;

static max6698_housekeeping_t global_max6698D[NUM_OF_MAX6698];
static max6698_housekeeping_t *pglobal_max6698D = &global_max6698D[0];
static uint8_t global_max6698_counter = 0;

/*****************SelfCheck Variables********************/
/*Writing Access: check_other_tasks_activity_task*/
/*Reading Access: check_other_tasks_activity_task*/

static uint8_t global_selfCheck_counter = 0;
static uint8_t global_delay_counter=0;
//static uint8_t watchdog_counter = 0;

/*****************MPPT Data*************************/
/*Writing Access: powerConvertion_and_battCtrl_task*/
/*Reading Access: powerConvertion_and_battCtrl_task, receiveCMD_task*/

static mppt_data_t global_mpptD[NUM_OF_MPPTS];
static mppt_data_t *pglobal_mpptD = &global_mpptD[0];
static uint8_t global_mppt_counter = 0;

/*****************Battery Data*************************/
/*Writing Access: powerConvertion_and_battCtrl_task*/
/*Reading Access: powerConvertion_and_battCtrl_task, receiveCMD_task*/

static battery_data_t global_battD[NUM_OF_BATTERY_PAIR];
static battery_data_t *pglobal_battD = &global_battD[0];
static uint8_t global_battery_counter = 0;

/*****************Heater Data*************************/
/*Writing Access: heaterCtrl_task*/
/*Reading Access: heaterCtrl_task, receiveCMD_task*/

static heater_data_t global_heaterD[NUM_OF_HEATER];
static heater_data_t *pglobal_heaterD = &global_heaterD[0];
static uint8_t global_heater_counter = 0;

/*****************Channel Data*************************/
/*Writing Access: outputchanCtrl_task*/
/*Reading Access: outputchanCtrl_task, receiveCMD_task*/

static channel_data_t global_channelD[NUM_OF_CHANNELS];
static channel_data_t *pglobal_channelD = &global_channelD[0];
static uint8_t global_channel_counter = 0;

/***************Task Tick Counter**********************/
/*Writing Access: check_other_tasks_activity_task*/
/*Reading Access: check_other_tasks_activity_task*/

/*Writing Access: outputchanCtrl_task*/
static uint32_t global_chanctrl_last_ticktime = 0;

/*Writing Access: getHK_task*/
static uint32_t global_hk_last_ticktime = 0;

/*Writing Access: powerConvertion_and_battCtrl_task*/
static uint32_t global_batt_last_ticktime = 0;

/*Writing Access: heaterCtrl_task*/
static uint32_t global_heater_last_ticktime = 0;

//static uint32_t rxCMD_t = 0;
//static uint32_t wdt_t = 0;
/***************Others**********************/
static uint8_t global_sys_mode = 1;        //variable that stores system mode. 0:critical mode, 1:safe mode, 2:full mode.
static uint8_t global_loop_counter = 0;    //global variable used for internal counter of loops

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    /*Check FLASH memory*/
    checkFlashECC();
    checkFlashEEPROMECC();

    /*Part of main function standard template*/
    _enable_IRQ();

    /*Initialize peripherals that will be used*/
    sciInit();      //serial port for debugging
    gioInit();      //general-purpose I/O
    i2cInit();      //i2c interface for current sensors and temp. sensors)
    canInit();      //interface to cubesat space protocal (CSP)
    mibspiInit();   //SPI interface to digital to analog converter

    /* Wakeup mask setting to enable rti counter1 and counter2 to wake the system from snooze (low-power) mode*/
    vimREG->WAKEMASKSET0 = 1<<2 | 1<<3;

    /* Enable Real-time Interrupts that were disabled when snooze mode is entered */
    systemREG1->RCLKSRC = 0x4;


    /*Create initialization task*/
    xTaskCreate((TaskFunction_t )init_task,
                (const char*    )"init_task",
                (uint16_t       )init_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(init_TASK_PRIO|portPRIVILEGE_BIT),
                (TaskHandle_t*  )&initTask_Handle);


    /* Reset real-time clock in debugging mode */
    resetRTC_debug(global_prealtimeClock);

    sciSend(scilinREG,19,(unsigned char *)"init_task created\r\n");


//    xQueue = xQueueCreate(1, sizeof(unsigned char*));
//    configASSERT( xQueue );

    vTaskStartScheduler();


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

/*
 * Initialization task:
 * - initialize data structures
 * - create other tasks
 * - delete itself in the end
 *
 */
void init_task(void *pvParameters)
{
    taskENTER_CRITICAL();


    /* Read data from FLASH and save to flash_data_t data strcuture*/
//
//
//    sciSend(scilinREG,10,(unsigned char *)"Fee read\r\n");
//    for(delay=0;delay<100;delay++);

    /*Initialize tick_counter for tasks*/
    /**************************/

    /*Initialize MPPT data structure*/
    for(global_mppt_counter=0; global_mppt_counter<NUM_OF_MPPTS; global_mppt_counter++)
    {
        global_mpptD[global_mppt_counter].channel = global_mppt_counter;
        global_mpptD[global_mppt_counter].counter = 0;
        global_mpptD[global_mppt_counter].dir = 0xFF;
        global_mpptD[global_mppt_counter].predir = 0xFF;
        global_mpptD[global_mppt_counter].stepsize = global_flashD.dac_stepsize_init;
        global_mpptD[global_mppt_counter].dacOUT = global_flashD.dac_init;
        global_mpptD[global_mppt_counter].prePower = 0;
        global_mpptD[global_mppt_counter].power = 0;
    }

    /*Initialize INA226 single-channel current sensor data structure*/

    /*sensors of overcurrent protection module*/
    global_loop_counter = 0;
    for(global_ina226_counter=0;global_ina226_counter<NUM_OF_INA226_OVERCURRENT_PROTECTION;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
//        global_ina226D[global_ina226_counter].mask_reg = pglobal_flash_sensor_config->ina226_overcurrent_mask;
        global_ina226D[global_ina226_counter].alert_reg = global_flashD.overcurrent_protection_alert[global_ina226_counter];

        INA226_Init(i2cREG1, pglobal_flashD->overcurrent_protection_Rshunt[global_loop_counter], pglobal_flash_sensor_config, pglobal_flash_sensor_config->ina226_overcurrent_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }

    /*sensors of current monitor*/
    global_loop_counter = 0;
    for(global_ina226_counter=NUM_OF_INA226_OVERCURRENT_PROTECTION;global_ina226_counter<NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
//        global_ina226D[global_ina226_counter].mask_reg = pglobal_flash_sensor_config->ina226_monitor_mask;
        global_ina226D[global_ina226_counter].alert_reg = global_flashD.current_monitor_alert[global_ina226_counter-NUM_OF_INA226_OVERCURRENT_PROTECTION];

        INA226_Init(i2cREG1, pglobal_flashD->current_monitor_Rshunt[global_loop_counter], pglobal_flash_sensor_config, pglobal_flash_sensor_config->ina226_monitor_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }

    /*sensors of battery protection*/
    global_loop_counter = 0;
    for(global_ina226_counter=NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR;global_ina226_counter<NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR+NUM_OF_INA226_BATTERY;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
//        global_ina226D[global_ina226_counter].mask_reg = pglobal_flash_sensor_config->ina226_battery_mask;
        global_ina226D[global_ina226_counter].alert_reg = 0x0;

        INA226_Init(i2cREG1, pglobal_flashD->battery_protection_Rshunt[global_loop_counter], pglobal_flash_sensor_config, pglobal_flash_sensor_config->ina226_battery_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }

    /*sensors of channel monitor*/
    global_loop_counter = 0;
    for(global_ina226_counter=NUM_OF_INA226-NUM_OF_INA226_CHANNEL;global_ina226_counter<NUM_OF_INA226;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
//        global_ina226D[global_ina226_counter].mask_reg = pglobal_flash_sensor_config->ina226_channel_mask;
        global_ina226D[global_ina226_counter].alert_reg = global_flashD.chan_config_data[global_ina226_counter-(NUM_OF_INA226-NUM_OF_INA226_CHANNEL)].maxI_mA;

        INA226_Init(i2cREG1, pglobal_flashD->channel_monitor_Rshunt[global_loop_counter], pglobal_flash_sensor_config, pglobal_flash_sensor_config->ina226_channel_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }


    /*Initialize INA3221 triple-channel current sensor data structure*/
    for(global_ina3221_counter=0;global_ina3221_counter<NUM_OF_INA3221;global_ina3221_counter++)
    {
        global_ina3221D[global_ina3221_counter].address = INA3221_ADDR1;

        INA3221_Init(i2cREG1, pglobal_flash_sensor_config, pglobal_ina3221D+global_ina3221_counter);
    }

    /*Initialize MAX6698 temperature sensor data structure*/
    for(global_max6698_counter=0;global_max6698_counter<NUM_OF_MAX6698;global_max6698_counter++)
    {
        global_max6698D[global_max6698_counter].address = MAX6698_ADDR1;

        MAX6698_Init(i2cREG1, pglobal_flash_sensor_config, pglobal_max6698D+global_max6698_counter);
    }

    /*Initialize battery heater data structure, updated in power conversion and battery controlling task*/
    for(global_heater_counter=0;global_heater_counter<NUM_OF_HEATER;global_heater_counter++)
    {
        global_heaterD[global_heater_counter].num = global_heater_counter+1;
        global_heaterD[global_heater_counter].sw = 0;
        global_heaterD[global_heater_counter].profile = 1;
    }

    /*Initialize battery data structure, updated in battery controlling task*/
    for(global_battery_counter=0;global_battery_counter<NUM_OF_BATTERY_PAIR;global_battery_counter++)
    {
        global_battD[global_battery_counter].num = global_battery_counter+1;
        global_battD[global_battery_counter].sw[0] = 1;
        global_battD[global_battery_counter].sw[1] = 1;
    }


    /*Initialize channel data structure, update*/
    for(global_channel_counter=0;global_channel_counter<NUM_OF_CHANNELS;global_channel_counter++)
    {
        global_channelD[global_channel_counter].num = global_channel_counter+1;
        global_channelD[global_channel_counter].priority = global_flashD.chan_config_data[global_channel_counter].priority;
        global_channelD[global_channel_counter].sw = 0;
        global_channelD[global_channel_counter].group_mask = global_flashD.chan_config_data[global_channel_counter].group_mask;
        global_channelD[global_channel_counter].resume = 0;
    }


    /* Create other tasks */

    xTaskCreate((TaskFunction_t )outputchanCtrl_task,
                (const char*    )"outputchanCtrl_task",
                (uint16_t       )outputchanCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(outputchanCtrl_TASK_PRIO|portPRIVILEGE_BIT),      //could be run in user mode
                (TaskHandle_t*  )&outputchanCtrlTask_Handle);
    sciSend(scilinREG,22,(unsigned char *)"Output channel task created\r\n");


    xTaskCreate((TaskFunction_t )receiveCMD_task,
                (const char*    )"receiveCMD_task",
                (uint16_t       )receiveCMD_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(receiveCMD_TASK_PRIO|portPRIVILEGE_BIT),
                (TaskHandle_t*  )&receiveCMDTask_Handle);
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
                (UBaseType_t    )(getHK_TASK_PRIO|portPRIVILEGE_BIT),
                (TaskHandle_t*  )&getHKTask_Handle);
    sciSend(scilinREG,20,(unsigned char *)"getHK task created\r\n");


    xTaskCreate((TaskFunction_t )check_other_tasks_activity_task,
                (const char*    )"check_other_tasks_activity_task",
                (uint16_t       )checkActive_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(checkActive_TASK_PRIO|portPRIVILEGE_BIT),         //could be run in user mode
                (TaskHandle_t*  )&checkActiveTask_Handle);
    sciSend(scilinREG,24,(unsigned char *)"check_other_tasks_activity task created\r\n");


//    xTaskCreate((TaskFunction_t )watchdog_task,
//                (const char*    )"watchdog_task",
//                (uint16_t       )watchdog_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )(watchdog_TASK_PRIO|portPRIVILEGE_BIT),
//                (TaskHandle_t*  )&watchdogTask_Handle);
//    sciSend(scilinREG,23,(unsigned char *)"watchdog task created\r\n");


    xTaskCreate((TaskFunction_t )heaterCtrl_task,
                (const char*    )"heaterCtrl_task",
                (uint16_t       )heaterCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(heaterCtrl_TASK_PRIO|portPRIVILEGE_BIT),          //could be run in user mode
                (TaskHandle_t*  )&heaterCtrlTask_Handle);
    sciSend(scilinREG,34,(unsigned char *)"heater controlling task created\r\n");


    xTaskCreate((TaskFunction_t )powerConvertion_and_battCtrl_task,
                (const char*    )"powerConvertion_and_battCtrl_task",
                (uint16_t       )powerConvertion_and_battCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )(powerConvertion_and_battCtrl_TASK_PRIO|portPRIVILEGE_BIT),            //could be run in user mode
                (TaskHandle_t*  )&powerConvertion_and_battCtrlTask_Handle);
    sciSend(scilinREG,34,(unsigned char *)"power convertion and battery controlling task created\r\n");


    taskEXIT_CRITICAL();

    vTaskDelete(initTask_Handle);

}



void getHK_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(1000);

    while(1)
    {
        /* Read raw data from ina226 single-channel current sensor registers and save them to data structure */
        for(global_ina226_counter=0;global_ina226_counter<NUM_OF_INA226;global_ina226_counter++)
        {
            INA226_ReadShuntVoltage_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);
            INA226_ReadBusVoltage_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);
            INA226_ReadCurrent_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);
            INA226_ReadPower_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);

            /* Save time stamp */
            global_ina226D[global_ina226_counter].timestamp_sec = getcurrTime(global_prealtimeClock);
        }

        /* Read raw data from ina3221 multi-channel current sensor registers and save them to data structure */
        for(global_ina3221_counter=0;global_ina3221_counter<NUM_OF_INA3221;global_ina3221_counter++)
        {

            INA3221_ReadShuntVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 1);
            INA3221_ReadBusVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 1);

            INA3221_ReadShuntVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 2);
            INA3221_ReadBusVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 2);

            /* Save time stamp */
            global_ina3221D[global_ina3221_counter].timestamp_sec = getcurrTime(global_prealtimeClock);
        }

        /* Read raw data from max6698 multi-channel temperature sensor registers and save them to data structure */
        for(global_max6698_counter=0;global_max6698_counter<NUM_OF_MAX6698;global_max6698_counter++)
        {
            MAX6698_ReadTemp_Raw(i2cREG1, pglobal_max6698D+global_max6698_counter, 1);
            MAX6698_ReadTemp_Raw(i2cREG1, pglobal_max6698D+global_max6698_counter, 2);

            /* Save time stamp */
            global_ina3221D[global_max6698_counter].timestamp_sec = getcurrTime(global_prealtimeClock);
        }


        /* copy current tick count from tick counter */
        global_hk_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}


void check_other_tasks_activity_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(4000);
    static uint32_t preTick[5] = {0};
    char str_temp[1] = {0};

    while(1)
    {

        /* clear the counter */
        global_selfCheck_counter = 0;

        /* check if every task has executed at least once */
        if(global_hk_last_ticktime != preTick[0])
        {
            preTick[0] = global_hk_last_ticktime;
            global_selfCheck_counter++;
        }
        if(global_chanctrl_last_ticktime != preTick[1])
        {
            preTick[1] = global_chanctrl_last_ticktime;
            global_selfCheck_counter++;
        }
//        if(wdt_t != preTick[2])
//        {
//            preTick[2] = wdt_t;
//            global_selfCheck_counter++;
//        }
        if(global_heater_last_ticktime != preTick[3])
        {
            preTick[3] = global_heater_last_ticktime;
            global_selfCheck_counter++;
        }
        if(global_batt_last_ticktime != preTick[4])
        {
            preTick[4] = global_batt_last_ticktime;
            global_selfCheck_counter++;
        }


        if(global_selfCheck_counter == 5) // all tasks are checked
        {
            /* pet the watchdog timer */
            gioSetBit(hetPORT2,11,1);
            for(global_delay_counter=0;global_delay_counter<100;global_delay_counter++);
            gioSetBit(hetPORT2,11,0);

            while (sciIsTxReady == 0);
            sciSend(scilinREG,18,(unsigned char *)"Pet the watchdog\r\n");

        }
        else
        {
            sprintf(str_temp,"%d",(int)global_selfCheck_counter);
            while (sciIsTxReady == 0);
            sciSend(scilinREG,15,(unsigned char *)"Working tasks: ");
            sciSend(scilinREG,strlen((const char *)str_temp),(unsigned char *)str_temp);
            sciSend(scilinREG,4,(unsigned char *)"\r\n\r\n");
        }


        vTaskDelay(xDelay);

    }
}

//void watchdog_task(void *pvParameters)
//{
////    printf( "selfCheck task running\n");
//
//    const portTickType xDelay = pdMS_TO_TICKS(1000);
//
//    uint16_t rst = 0;
//    char temp1[20] = {0};
//    char pcWriteBuffer[100] = {0};
//
////    char temp1[40] = {0};
//
//
//    while(1)
//    {
//        if(watchdog_counter>4)
//        {
////            seconds2 = time(NULL);
//            while (sciIsTxReady == 0);
//            sciSend(scilinREG,23,(unsigned char *)"Failed to contact OBC\r\n");
//            watchdog_counter=0;
//
//            rst = (uint16_t)systemREG1->SYSESR;
//            sprintf(temp1,"%d",(int)rst);
//            sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
//            sciSend(scilinREG,2,(unsigned char *)"\r\n");
//
////            printf("seconds since January 1, 1970 = %d\n", seconds);
//
//        }
//        watchdog_counter++;
//
//        wdt_t = (uint32_t)xTaskGetTickCount();
//
////        vTaskGetRunTimeStats(pcWriteBuffer);
////        sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
////        sciSend(scilinREG,2,(unsigned char *)"\r\n");
//
//
//        vTaskDelay(xDelay);
//
//    }
//}


void outputchanCtrl_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(100);
    vTaskDelay(xDelay);

    while(1)
    {

        channel_check_mode(pglobal_channelD, global_sys_mode);
        channel_resume(pglobal_channelD);

        channel_check_batteryV_then_SW(pglobal_channelD, pglobal_battD, pglobal_flashD);
        channel_check_batteryI_then_SW(pglobal_channelD, pglobal_battD, pglobal_flashD);
        channel_check_chanV_then_SW(pglobal_channelD, pglobal_flashD);

        channel_read_rawdata_and_convert(pglobal_channelD, pglobal_ina226D+(NUM_OF_INA226-NUM_OF_INA226_CHANNEL));


        global_chanctrl_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}


void heaterCtrl_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(2000);
    vTaskDelay(xDelay);

    while(1)
    {
        for(global_heater_counter=0; global_heater_counter<NUM_OF_HEATER; global_heater_counter++)
        {
            heater_read_rawdata_and_convert(pglobal_heaterD+global_heater_counter, pglobal_max6698D);
            heater_temp_SW(pglobal_heaterD+global_heater_counter, pglobal_flashD);
        }

        global_heater_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}


void powerConvertion_and_battCtrl_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(MPPT_TASK_DELAY);
    vTaskDelay(xDelay);


    while(1)
    {

        /*EN_pin Controlling*/
        for(global_mppt_counter=0;global_mppt_counter<NUM_OF_INA3221;global_mppt_counter++)
        {

            (pglobal_mpptD+global_mppt_counter)->power = 0;

            mppt_getPower_ina226(pglobal_ina226D+global_mppt_counter, pglobal_mpptD+global_mppt_counter);

            mppt_pno_en(pglobal_mpptD+global_mppt_counter);

            dac_write_en(mibspiPORT3,pglobal_mpptD+global_mppt_counter);
         }


        for(global_battery_counter=0;global_battery_counter<NUM_OF_BATTERY_PAIR;global_battery_counter++)
        {
            battery_check_charging_status(pglobal_battD+global_battery_counter, pglobal_ina226D+NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR+global_battery_counter);
            battery_read_rawdata_and_convert(pglobal_battD+global_battery_counter, pglobal_ina226D+NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR+global_battery_counter, pglobal_max6698D);
            battery_check_temp_then_SW(pglobal_battD+global_battery_counter, pglobal_flashD);
        }


        global_batt_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);

    }
}

void receiveCMD_task(void *pvParameters)
{
    unsigned char *cmd1;
//    uint32_t current_sec = 0;

    const portTickType xDelay = pdMS_TO_TICKS(100);
    while(1)
    {

        cmd1 = NULL;
        while (sciIsTxReady == 0);
        cmd1 = uart_tx(30,(unsigned char*)"\r\nWaiting for Command:\r\n");

//        xQueueSendToBack(xQueue, cmd1, 0UL);

        if(strcmp((const char *)cmd1, (const char *)"get_hk_bc")==0)
        {
            get_hk_bc(pglobal_ina3221D);
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_hk_batt")==0)
        {
            get_hk_batt(pglobal_ina226D+7, pglobal_battD);
        }
        else if(strcmp((const char *)cmd1, (const char *)"get_hk_ch")==0)
        {
            get_hk_channel(pglobal_ina226D+9, pglobal_channelD);
        }
        else if(strcmp((const char *)cmd1, (const char *)"reset")==0)
        {
            systemREG1->SYSECR = 0x8000;
        }
        else if(strcmp((const char *)cmd1, (const char *)"set_default max6698_init")==0)
        {
            cmd1 = NULL;
            cmd1 = uart_tx(8,(unsigned char*)"\r\nWhich one?\r\n");
            uint8_t num = atoi((const char*)cmd1);

            cmd1 = NULL;
            cmd1 = uart_tx(16,(unsigned char*)"\r\nWhat value?\r\n");
            uint8_t value = atoi((const char*)cmd1);

//            set_default_max6698_init(pmax6698_init, num, value);
        }
//        else if(strcmp((const char *)cmd1, (const char *)"get_time")==0)
//        {
//            current_sec = getcurrTime(global_prealtimeClock);
//            temp1[0] = '0'+ current_sec/1000000000;
//            temp1[1] = '0'+ current_sec%1000000000/100000000;
//            temp1[2] = '0'+ current_sec%1000000000%100000000/10000000;
//            temp1[3] = '0'+ current_sec%1000000000%100000000%10000000/1000000;
//            temp1[4] = '0'+ current_sec%1000000000%100000000%10000000%1000000/100000;
//            temp1[5] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000/10000;
//            temp1[6] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000/1000;
//            temp1[7] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000%1000/100;
//            temp1[8] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000%1000%100/10;
//            temp1[9] = '0'+ current_sec%1000000000%100000000%10000000%1000000%100000%10000%1000%100%10;
//
//            sciSend(scilinREG,strlen((const char *)temp1),(unsigned char *)temp1);
//
//
//            sciSend(scilinREG,4,(unsigned char *)"\r\n\r\n");
//
//        }
        else
        {
            sciSend(scilinREG,15,(unsigned char *)"Wrong command\r\n");
        }

        vTaskDelay(xDelay);
    }

}


void enter_snooze(void)
{
    /* RTI is configured to generate compare 1 interrupt every 1 second using 16MHz OSCIN as source */
    /* Change this to use the LF LPO, which is typically 80KHz */
    rtiStopCounter(rtiCOUNTER_BLOCK1);
    rtiResetCounter(rtiCOUNTER_BLOCK1);

    /* Clock RTI using LF LPO, the 80KHz clock source */
    systemREG1->RCLKSRC = 0x4;

    /** - Setup compare 1 value. This value is compared with selected free running counter. */
    rtiREG1->CMP[1U].COMPx = (uint32_t)xTaskGetExpectedIdleTime();

    /** - Setup update compare 1 value. This value is added to the compare 1 value on each compare match. */
    rtiREG1->CMP[1U].UDCPx = (uint32_t)xTaskGetExpectedIdleTime();

    /** - Clear all pending interrupts */
    rtiREG1->INTFLAG = 0x0007000FU;

    /** - Disable all interrupts */
    rtiREG1->CLEARINTENA = 0x00070F0FU;

    /** - Enable RTI Compare 1 Interrupt **/
    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE1;

    flashWREG->FPAC2 = 0x7;

    /* Bank 7 */
    flashWREG->FMAC = 0x6;
    flashWREG->FBAC = 0x70F;

    /* Bank 0 */
    flashWREG->FMAC = 0x0;
    flashWREG->FBAC = 0x70F;

    /* Setup flash module to change fallback modes for banks/pump to be "sleep" */
    flashWREG->FBFALLBACK = 0x00000000U
                          | (uint32)((uint32)SYS_SLEEP << 14U) /* BANK 7 */
                          | (uint32)((uint32)SYS_SLEEP << 2U)  /* BANK 1 */
                          | (uint32)((uint32)SYS_SLEEP << 0U); /* BANK 0 */

    /* Disable oscillator monitoring to prevent detection of osc fail */
    systemREG1->CLKTEST = 0x010A0000;

    /* Start counter 1 */
    rtiStartCounter(rtiCOUNTER_BLOCK1);

    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG1->GHVSRC = (uint32)((uint32)SYS_LPO_HIGH << 24U)
                       | (uint32)((uint32)SYS_LPO_HIGH << 16U)
                       | (uint32)((uint32)SYS_PLL1 << 0U);

    /* Enable low-power modes */
    systemREG1->VRCTL = 0x0F;

    /* turn off all clock sources except LF LPO */
    systemREG1->CSDISSET = 0xEF;

    /* turn off all clock domains except RTICLK */
    systemREG1->CDDISSET = 0xFFBF;


    asm(" WFI");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");

}

void post_wakeup(void)
{

    /* Restart main oscillator, LF LPO and HF LPO first */
    systemREG1->CSDISCLR = 0x31;

    /* Configure PLLs based on original configuration */
    /**   - Setup pll control register 1:
    *     - Setup reset on oscillator slip
    *     - Setup bypass on pll slip
    *     - setup Pll output clock divider to max before Lock
    *     - Setup reset on oscillator fail
    *     - Setup reference clock divider
    *     - Setup Pll multiplier
    */
    systemREG1->PLLCTL1 =  (uint32)0x00000000U
                        |  (uint32)0x20000000U
                        |  (uint32)((uint32)0x1FU << 24U)
                        |  (uint32)0x00000000U
                        |  (uint32)((uint32)(6U - 1U)<< 16U)
                        |  (uint32)((uint32)(165U - 1U)<< 8U);

    /**   - Setup pll control register 2
    *     - Setup spreading rate
    *     - Setup bandwidth adjustment
    *     - Setup internal Pll output divider
    *     - Setup spreading amount
    */
    systemREG1->PLLCTL2 =  (uint32)((uint32)255U << 22U)
                        |  (uint32)((uint32)7U << 12U)
                        |  (uint32)((uint32)(2U - 1U) << 9U)
                        |  (uint32)61U;

    /** @b Initialize @b Pll2: */

    /**   - Setup pll2 control register :
    *     - setup Pll output clock divider to max before Lock
    *     - Setup reference clock divider
    *     - Setup internal Pll output divider
    *     - Setup Pll multiplier
    */
    systemREG2->PLLCTL3 = (uint32)((uint32)(2U - 1U) << 29U)
                        | (uint32)((uint32)0x1FU << 24U)
                        | (uint32)((uint32)(6U - 1U)<< 16U)
                        | (uint32)((uint32)(165U - 1U) << 8U);

    /* Restart all other clock sources. All clock domains are enabled automatically. */
    systemREG1->CSDISCLR = 0xFF;

    /* Reconfigure flash bank/pump fallback modes to be "active" */
    /** - Setup flash bank power modes */
    flashWREG->FBFALLBACK = 0x00000000U
                          | (uint32)((uint32)SYS_ACTIVE << 14U) /* BANK 7 */
                          | (uint32)((uint32)SYS_ACTIVE << 2U)  /* BANK 1 */
                          | (uint32)((uint32)SYS_ACTIVE << 0U); /* BANK 0 */

    vTaskStepTick(400);

    /* Restore original clock source/domain bindings */
    mapClocks();

    /* Resume oscillator monitoring */
    systemREG1->CLKTEST = 0x000A0000;

}


void vApplicationIdleHook(void)
{
    enter_snooze();
    post_wakeup();
}

void rtiNotification(uint32 notification)
{

}


/* USER CODE END */

