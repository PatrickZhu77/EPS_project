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
/*All the custom user codes must be inserted in between "USER CODE BEGIN" and correspond "USER CODE END" statement*/
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
//The include statements go here
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
#include "low_power_mode.h"
#include "error_message.h"
#include "status.h"

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
//Global symbol definitions and data structures

#define DEBUGGING_MODE 1
/*******************************************************/

#define TIMER_ID                1
#define DELAY_10_SECONDS        10000UL /* 1000 ticks per sec on this H/W */
#define DELAY_1_SECOND          1000UL
#define DELAY_100_MS_SECONDS    100UL
#define TIMER_CHECK_THRESHOLD   9

/*****************RTC Variables************************/
static RTC_t global_realtimeClock;
static RTC_t *global_prealtimeClock = &global_realtimeClock;
//static char temp1[10] = {0};


/***************FLASH Data Array***************/
/*Writing Access: init_task, receiveCMD_task*/
/*Reading Access: init_task, outputchanCtrl_task, powerConversion_and_battCtrl_task, heaterCtrl_task, receiveCMD_task*/
static uint8_t global_RAM_copy_array[FLASHED_DATA_LENGTH] = {0};   //Converted configuration data (in uint8_t array format). Used for writing to/reading from FLASH and for transmission by commands
static uint8_t *pglobal_RAM_copy_array = &global_RAM_copy_array[0];

static uint8_t global_err_msg_timestamp_s_array[4*ERROR_BUFFER_SIZE];      //Converted error message timestamp in sec (in uint8_t array format)
static uint8_t *pglobal_err_msg_timestamp_s_array = &global_err_msg_timestamp_s_array[0];
static uint8_t global_err_msg_timestamp_ms_array[2*ERROR_BUFFER_SIZE];      //Converted error message timestamp in ms (in uint8_t array format)
static uint8_t *pglobal_err_msg_timestamp_ms_array = &global_err_msg_timestamp_ms_array[0];

/***************Configuration Data***************/
static system_config_t global_RAM_copyD = {NUM_OF_CONFIG_VERSION,
                            {INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT, INA226_OVERCURRENT_ALERT},{INA226_MONITOR_ALERT},
                            {INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE}, {INA226_SHUNT_RESISTANCE},
                            {INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE},{0},{INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE,INA226_SHUNT_RESISTANCE},
                            BATT_CHARGING_CURRENT_LIMIT_mA, BATT_DISCHARGING_CURRENT_LIMIT_mA,
                            HEATER_TUMBLE_THRESHOLD_TIME_S, HEATER_SOLAR_PANEL_THRESHOLD_POWER_mW, HEATER_ORBIT_PERIOD_S, HEATER_HEAT_UP_TIME_S,
                            DAC_INIT, EN_STEPSIZE_INIT,
                            BATT_CHARGING_TEMP_MIN_C, BATT_CHARGING_TEMP_MAX_C, BATT_DISCHARGING_TEMP_MIN_C, BATT_DISCHARGING_TEMP_MAX_C,
                            HEATER_SUNSHINE_TEMP_ON_C, HEATER_SUNSHINE_TEMP_OFF_C, HEATER_ECLIPSE_TEMP_ON_C, HEATER_ECLIPSE_TEMP_OFF_C,
                            {0},{0}};
static system_config_t *pglobal_RAM_copyD = &global_RAM_copyD;


/*******************Sensor Data**********************/
/*Writing Access: init_task, receiveCMD_task, getHK_task*/
/*Reading Access: outputchanCtrl_task, powerConversion_and_battCtrl_task, heaterCtrl_task, receiveCMD_task*/

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
/*Writing Access: powerConversion_and_battCtrl_task*/
/*Reading Access: powerConversion_and_battCtrl_task, receiveCMD_task*/

static mppt_data_t global_mpptD[NUM_OF_MPPTS];
static mppt_data_t *pglobal_mpptD = &global_mpptD[0];
static uint8_t global_mppt_counter = 0;

/*****************Battery Data*************************/
/*Writing Access: powerConversion_and_battCtrl_task*/
/*Reading Access: powerConversion_and_battCtrl_task, receiveCMD_task*/

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

/*Writing Access: powerConversion_and_battCtrl_task*/
static uint32_t global_batt_last_ticktime = 0;

/*Writing Access: heaterCtrl_task*/
static uint32_t global_heater_last_ticktime = 0;

/*Writing Access: wdt_task*/
static uint32_t global_wdt_last_ticktime = 0;

/***************Software Watchdog Timer Data**********************/
/*Writing Access: wdt_task, receiveCMD_task*/
/*Reading Access: wdt_task, receiveCMD_task*/

static uint32_t global_wdt_counter_obc = WDT_COUNTER_OBC;            //counter used by software watchdog timer task
static uint32_t global_wdt_counter_ground = WDT_COUNTER_GROUND;      //counter used by software watchdog timer task

static uint8_t  global_wdt_reset_obc = 0;
static uint8_t  global_wdt_reset_ground = 0;

/***************Error message buffer**********************/
/*Writing Access: wdt_task, getHK_task, receiveCMD_task*/
/*Reading Access: init_task, wdt_task, getHK_task, receiveCMD_task*/
static uint8_t global_err_msg_buffer[2+2*ERROR_BUFFER_SIZE] = {0};      //bit 0 is the write pointer of the circular buffer, bit 1 is the number of error message logged
                                                                        //write pointer indicates the next bit to write
                                                                        //starting from bit 2, even bit stores the type of the error and odd bit stores data of the error
                                                                        //for example: bit 2 stores a 1 (ERROR_BAD_CRC), bit 3 stores 3 (REBOOT_COPY has this error)
static uint8_t *pglobal_err_msg_buffer = &global_err_msg_buffer[0];

static uint32_t global_err_msg_timestamp_s_buffer[ERROR_BUFFER_SIZE] = {0};
static uint32_t *pglobal_err_msg_timestamp_s_buffer = &global_err_msg_timestamp_s_buffer[0];

static uint16_t global_err_msg_timestamp_ms_buffer[ERROR_BUFFER_SIZE] = {0};
static uint16_t *pglobal_err_msg_timestamp_ms_buffer = &global_err_msg_timestamp_ms_buffer[0];


/***************System Status Data**********************/
/*Writing Access: init_task, getHK_task, receiveCMD_task*/
/*Reading Access: receiveCMD_task*/

/***************Others**********************/
static uint8_t global_sys_mode = 1;        //variable that stores system mode. 0:critical mode, 1:safe mode, 2:full mode.
static uint8_t global_loop_counter = 0;    //global variable used for internal counter of loops



/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
//


    /*Part of main function standard template*/
    _enable_IRQ();

    /*TI built-in code will do some initialization but the following initialization functions still need to be called*/
    /*Initialize peripherals that will be used. The details of initial configuration is done using TI tool HAL Code Generator*/
    sciInit();      //serial port for debugging
    gioInit();      //general-purpose I/O (The initialization parameters are defined in other TI tool called HalCoGen)
    i2cInit();      //i2c interface for current sensors and temp. sensors (The initialization parameters are defined in other TI tool called HalCoGen)
    canInit();      //interface to Cubesat Space Protocal (CSP) (The initialization parameters are defined in other TI tool called HalCoGen)
    mibspiInit();   //SPI interface to digital to analog converter (The initialization parameters are defined in other TI tool called HalCoGen)

    fee_initial();      //Initialize the Flash EEPROM module

    /*The following functions are included for clarity. The same effect can be done using HALCoGen*/
    /* Wakeup mask setting to enable rti counter1 and counter2 to wake the system from snooze (low-power) mode*/
    vimREG->WAKEMASKSET0 = 1<<2 | 1<<3;

    /* Enable Real-time Interrupts that were disabled when snooze mode is entered */
    systemREG1->RCLKSRC = 0x4;


    /* Read error message and corresponding timestamp from copies in FLASH*/
    fee_read_flashed_data(ERR_MSG_COPY, pglobal_err_msg_buffer, 2+2*ERROR_BUFFER_SIZE);
    fee_read_flashed_data(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array, 4*ERROR_BUFFER_SIZE);
    fee_read_flashed_data(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array, 2*ERROR_BUFFER_SIZE);
    fee_unpackage_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_array, pglobal_err_msg_timestamp_s_buffer);
    fee_unpackage_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_array, pglobal_err_msg_timestamp_ms_buffer);

    int32_t check_task_creation = 0;

    /*Create initialization task*/
    check_task_creation = xTaskCreate((TaskFunction_t )init_task,
                                      (const char*    )"init_task",
                                      (uint16_t       )init_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(init_TASK_PRIO|portPRIVILEGE_BIT),
                                      (TaskHandle_t*  )&initTask_Handle);

    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_INIT);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }

    /* Reset real-time clock in debugging mode */
    resetRTC_debug(global_prealtimeClock);

#ifdef DEBUGGING_MODE
    sciSend(scilinREG,19,(unsigned char *)"init_task created\r\n");
#endif

    vTaskStartScheduler();


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

/***********************************************
 * Initialization task:
 * - run in critical section
 * - initialize data structures
 * - create other tasks
 * - delete itself in the end
 *
 ***********************************************/
void init_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    /* Read packaged data from reboot copy in FLASH */
    fee_read_flashed_data(REBOOT_COPY, pglobal_RAM_copy_array, FLASHED_DATA_LENGTH);

    /* If CRC is good, unpackage it and store it in global data structure of configurations */
    if(fee_check_crc_then_unpackage_data(pglobal_RAM_copy_array, pglobal_RAM_copyD))
    {
        /* If CRC is bad, log the error and read packaged data from factory copy 1 in FLASH*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_BAD_CRC, REBOOT_COPY);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));

        fee_read_flashed_data(FACTORY_COPY_1, pglobal_RAM_copy_array, FLASHED_DATA_LENGTH);

        /* If CRC is good, unpackage it and store it in global data structure of configurations */
        if(fee_check_crc_then_unpackage_data(pglobal_RAM_copy_array, pglobal_RAM_copyD))
        {
            /* If CRC is bad, log the error and read packaged data from factory copy 2 in FLASH*/
            error_log_the_data(pglobal_err_msg_buffer, ERROR_BAD_CRC, FACTORY_COPY_1);
            error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));

            fee_read_flashed_data(FACTORY_COPY_2, pglobal_RAM_copy_array, FLASHED_DATA_LENGTH);

            /* If CRC is good, unpackage it and store it in global data structure of configurations */
            if(fee_check_crc_then_unpackage_data(pglobal_RAM_copy_array, pglobal_RAM_copyD))
            {
                /* If CRC is still bad, log the error and ignore the crc then unpackage the data and store it in global data structure of configurations*/
                error_log_the_data(pglobal_err_msg_buffer, ERROR_BAD_CRC, FACTORY_COPY_2);
                error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));

                fee_unpackage_data_ignore_crc(pglobal_RAM_copy_array, pglobal_RAM_copyD);
            }
        }
    }



    /*In the case of sensors, both data structures and the hardware are initialized in this task. For other modules, only data structures are initialized in this task.
     * The hardware components will be initialized in controlling tasks using data in data structures*/

    /*Initialize MPPT data structure*/
    for(global_mppt_counter=0; global_mppt_counter<NUM_OF_MPPTS; global_mppt_counter++)
    {
        global_mpptD[global_mppt_counter].channel = global_mppt_counter;
        global_mpptD[global_mppt_counter].counter = 0;
        global_mpptD[global_mppt_counter].dir = 0xFF;
        global_mpptD[global_mppt_counter].predir = 0xFF;
        global_mpptD[global_mppt_counter].stepsize = global_RAM_copyD.dac_stepsize_init;
        global_mpptD[global_mppt_counter].dacOUT = global_RAM_copyD.dac_init;
        global_mpptD[global_mppt_counter].prePower = 0;
        global_mpptD[global_mppt_counter].power = 0;
    }

    /*Initialize INA226 single-channel current sensor data structure and the hardware*/

    /*sensors of overcurrent protection module*/
    global_loop_counter = 0;
    for(global_ina226_counter=0;global_ina226_counter<NUM_OF_INA226_OVERCURRENT_PROTECTION;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
        global_ina226D[global_ina226_counter].alert_reg = global_RAM_copyD.overcurrent_protection_alert_mA[global_ina226_counter];

        INA226_Init(i2cREG1, pglobal_RAM_copyD->overcurrent_protection_Rshunt[global_loop_counter], pglobal_RAM_copyD->sensor_config_data, pglobal_RAM_copyD->sensor_config_data.ina226_overcurrent_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }

    /*sensors of current monitor*/
    global_loop_counter = 0;
    for(global_ina226_counter=NUM_OF_INA226_OVERCURRENT_PROTECTION;global_ina226_counter<NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
        global_ina226D[global_ina226_counter].alert_reg = global_RAM_copyD.current_monitor_alert_mA[global_ina226_counter-NUM_OF_INA226_OVERCURRENT_PROTECTION];

        INA226_Init(i2cREG1, pglobal_RAM_copyD->current_monitor_Rshunt[global_loop_counter], pglobal_RAM_copyD->sensor_config_data, pglobal_RAM_copyD->sensor_config_data.ina226_monitor_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }

    /*sensors of battery protection*/
    global_loop_counter = 0;
    for(global_ina226_counter=NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR;global_ina226_counter<NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR+NUM_OF_INA226_BATTERY;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
        global_ina226D[global_ina226_counter].alert_reg = 0x0;

        INA226_Init(i2cREG1, pglobal_RAM_copyD->battery_protection_Rshunt[global_loop_counter], pglobal_RAM_copyD->sensor_config_data, pglobal_RAM_copyD->sensor_config_data.ina226_battery_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }

    /*sensors of channel monitor*/
    global_loop_counter = 0;
    for(global_ina226_counter=NUM_OF_INA226-NUM_OF_INA226_CHANNEL;global_ina226_counter<NUM_OF_INA226;global_ina226_counter++)
    {
        global_ina226D[global_ina226_counter].address = INA226_ADDR1;
        global_ina226D[global_ina226_counter].alert_reg = global_RAM_copyD.chan_config_data[global_ina226_counter-(NUM_OF_INA226-NUM_OF_INA226_CHANNEL)].maxI_mA;

        INA226_Init(i2cREG1, pglobal_RAM_copyD->channel_monitor_Rshunt[global_loop_counter], pglobal_RAM_copyD->sensor_config_data, pglobal_RAM_copyD->sensor_config_data.ina226_channel_mask, pglobal_ina226D+global_ina226_counter);
        global_loop_counter++;
    }


    /*Initialize INA3221 triple-channel current sensor data structure and the hardware*/
    for(global_ina3221_counter=0;global_ina3221_counter<NUM_OF_INA3221;global_ina3221_counter++)
    {
        global_ina3221D[global_ina3221_counter].address = INA3221_ADDR1;

        INA3221_Init(i2cREG1, pglobal_RAM_copyD->sensor_config_data, pglobal_ina3221D+global_ina3221_counter);
    }

    /*Initialize MAX6698 temperature sensor data structure  and the hardware*/
    for(global_max6698_counter=0;global_max6698_counter<NUM_OF_MAX6698;global_max6698_counter++)
    {
        global_max6698D[global_max6698_counter].address = MAX6698_ADDR1;

        MAX6698_Init(i2cREG1, pglobal_RAM_copyD->sensor_config_data, pglobal_max6698D+global_max6698_counter);
    }

    /*Initialize battery heater data structure, and the hardware is updated in power conversion and battery controlling task*/
    for(global_heater_counter=0;global_heater_counter<NUM_OF_HEATER;global_heater_counter++)
    {
        global_heaterD[global_heater_counter].num = global_heater_counter+1;
        global_heaterD[global_heater_counter].sw = 0;
        global_heaterD[global_heater_counter].profile = 1;
    }

    /*Initialize battery data structure, and the hardware is updated in battery controlling task*/
    for(global_battery_counter=0;global_battery_counter<NUM_OF_BATTERY_PAIR;global_battery_counter++)
    {
        global_battD[global_battery_counter].num = global_battery_counter+1;
        global_battD[global_battery_counter].sw[0] = 1;
        global_battD[global_battery_counter].sw[1] = 1;
    }


    /*Initialize channel data structure,  and the hardware is updated in channel controlling task*/
    for(global_channel_counter=0;global_channel_counter<NUM_OF_CHANNELS;global_channel_counter++)
    {
        global_channelD[global_channel_counter].num = global_channel_counter+1;
        global_channelD[global_channel_counter].priority = global_RAM_copyD.chan_config_data[global_channel_counter].priority;
        global_channelD[global_channel_counter].sw = 0;
        global_channelD[global_channel_counter].group_mask = global_RAM_copyD.chan_config_data[global_channel_counter].group_mask;
        global_channelD[global_channel_counter].resume = 0;
    }


    /* Create all the other tasks */
    int32_t check_task_creation = 0;


    /*Create get housekeeping task*/
    check_task_creation = xTaskCreate((TaskFunction_t )getHK_task,
                                      (const char*    )"getHK_task",
                                      (uint16_t       )getHK_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(getHK_TASK_PRIO|portPRIVILEGE_BIT),
                                      (TaskHandle_t*  )&getHKTask_Handle);
//    sciSend(scilinREG,20,(unsigned char *)"getHK task created\r\n");
    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_GET_HK);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }

    taskEXIT_CRITICAL();

    taskENTER_CRITICAL();

    /*Create output channel controlling task*/
    check_task_creation = xTaskCreate((TaskFunction_t )outputchanCtrl_task,
                                      (const char*    )"outputchanCtrl_task",
                                      (uint16_t       )outputchanCtrl_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(outputchanCtrl_TASK_PRIO|portPRIVILEGE_BIT),      //could be run in user mode
                                      (TaskHandle_t*  )&outputchanCtrlTask_Handle);
//    sciSend(scilinREG,22,(unsigned char *)"Output channel task created\r\n");

    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_CHAN_CTRL);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }


#ifdef DEBUGGING_MODE
    /*Create command receive task*/
    check_task_creation = xTaskCreate((TaskFunction_t )receiveCMD_task,
                                      (const char*    )"receiveCMD_task",
                                      (uint16_t       )receiveCMD_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(receiveCMD_TASK_PRIO|portPRIVILEGE_BIT),      //could be run in user mode
                                      (TaskHandle_t*  )&receiveCMDTask_Handle);
//    sciSend(scilinREG,25,(unsigned char *)"receiveCMD task created\r\n");
#else

    check_task_creation = xTaskCreate((TaskFunction_t )executeCMD_task,
                                      (const char*    )"executeCMD_task",
                                      (uint16_t       )executeCMD_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )executeCMD_TASK_PRIO,
                                      (TaskHandle_t*  )&executeCMDTask_Handler);
//    sciSend(scilinREG,25,(unsigned char *)"executeCMD task created\r\n");
#endif
    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_RECEIVE_CMD);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }


    /*Create check task activity task*/
    check_task_creation = xTaskCreate((TaskFunction_t )check_other_tasks_activity_task,
                                      (const char*    )"check_other_tasks_activity_task",
                                      (uint16_t       )checkActive_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(checkActive_TASK_PRIO|portPRIVILEGE_BIT),
                                      (TaskHandle_t*  )&checkActiveTask_Handle);
//    sciSend(scilinREG,24,(unsigned char *)"check_other_tasks_activity task created\r\n");
    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_CHECK_ACTIVE);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }


    /*Create heater control task*/
    check_task_creation = xTaskCreate((TaskFunction_t )heaterCtrl_task,
                                      (const char*    )"heaterCtrl_task",
                                      (uint16_t       )heaterCtrl_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(heaterCtrl_TASK_PRIO|portPRIVILEGE_BIT),
                                      (TaskHandle_t*  )&heaterCtrlTask_Handle);
//    sciSend(scilinREG,34,(unsigned char *)"heater controlling task created\r\n");
    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_HEATER_CTRL);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }


    /*Create power conversion and battery control task*/
    check_task_creation = xTaskCreate((TaskFunction_t )powerConversion_and_battCtrl_task,
                                      (const char*    )"powerConversion_and_battCtrl_task",
                                      (uint16_t       )powerConversion_and_battCtrl_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(powerConversion_and_battCtrl_TASK_PRIO|portPRIVILEGE_BIT),
                                      (TaskHandle_t*  )&powerConversion_and_battCtrlTask_Handle);
//    sciSend(scilinREG,34,(unsigned char *)"power convertion and battery controlling task created\r\n");
    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_POWER_CONV);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }


    /*Create software watchdog timer task*/
    check_task_creation = xTaskCreate((TaskFunction_t )sw_wdt_task,
                                      (const char*    )"powerConversion_and_battCtrl_task",
                                      (uint16_t       )sw_wdt_STK_SIZE,
                                      (void*          )NULL,
                                      (UBaseType_t    )(sw_wdt_TASK_PRIO|portPRIVILEGE_BIT),
                                      (TaskHandle_t*  )&sw_wdtTask_Handle);
//    sciSend(scilinREG,34,(unsigned char *)"software watchdog timer task created\r\n");
    if(check_task_creation == -1)      //if the task is not created successfully
    {
        /*log the error*/
        error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_CREATED, TASK_WDT);
        error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, 0, 0);

        /*Update the error log to FLASH*/
        fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
        fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
        TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
        TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

        /*Software reset the system*/
        systemREG1->SYSECR = 0x8000;
    }



    taskEXIT_CRITICAL();

    vTaskDelete(initTask_Handle);

}


/***********************************************
 * Get housekeeping data task:
 * - Read raw data from sensors and update all the global data structure of sensors
 *
 **********************************************/
void getHK_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(GET_HK_TASK_PERIOD);

    while(1)
    {
        /* Read raw data from ina226 single-channel current sensor registers and update the data structure */
        for(global_ina226_counter=0;global_ina226_counter<NUM_OF_INA226;global_ina226_counter++)
        {
            INA226_ReadShuntVoltage_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);
            INA226_ReadBusVoltage_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);
            INA226_ReadCurrent_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);
            INA226_ReadPower_Raw(i2cREG1, pglobal_ina226D+global_ina226_counter);

            /* Save time stamp */
            global_ina226D[global_ina226_counter].timestamp_sec = getcurrTime_sec(global_prealtimeClock);
        }

        /* Read raw data from ina3221 multi-channel current sensor registers and update the data structure */
        for(global_ina3221_counter=0;global_ina3221_counter<NUM_OF_INA3221;global_ina3221_counter++)
        {

            INA3221_ReadShuntVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 1);
            INA3221_ReadBusVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 1);

            INA3221_ReadShuntVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 2);
            INA3221_ReadBusVoltage_Raw(i2cREG1, pglobal_ina3221D+global_ina3221_counter, 2);

            /* Save time stamp */
            global_ina3221D[global_ina3221_counter].timestamp_sec = getcurrTime_sec(global_prealtimeClock);
        }

        /* Read raw data from max6698 multi-channel temperature sensor registers and update the data structure */
        for(global_max6698_counter=0;global_max6698_counter<NUM_OF_MAX6698;global_max6698_counter++)
        {
            MAX6698_ReadTemp_Raw(i2cREG1, pglobal_max6698D+global_max6698_counter, 1);
            MAX6698_ReadTemp_Raw(i2cREG1, pglobal_max6698D+global_max6698_counter, 2);

            /* Save time stamp */
            global_max6698D[global_max6698_counter].timestamp_sec = getcurrTime_sec(global_prealtimeClock);
        }


        /* copy current tick count from tick counter */
        global_hk_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}


/***********************************************
 * Check activity task:
 * - Check the activity of all the other tasks
 * - Pet the watchdog timer if all the tasks is active
 * - Store (print) the error message if some tasks are inactive
 *
 **********************************************/
void check_other_tasks_activity_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(CHECK_ACTIVE_TASK_PERIOD);
    static uint32_t preTick[5] = {0};
    char str_temp[1] = {0};

    gioSetBit(hetPORT2,11,1);
    for(global_delay_counter=0;global_delay_counter<100;global_delay_counter++);
    gioSetBit(hetPORT2,11,0);


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
        else
        {
            /*If this task is not active, log the error to buffer*/
            error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_ACTIVE, TASK_GET_HK);
            error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));
        }

        if(global_wdt_last_ticktime != preTick[1])
        {
            preTick[1] = global_wdt_last_ticktime;
            global_selfCheck_counter++;
        }
        else
        {
            /*If this task is not active, log the error to buffer*/
            error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_ACTIVE, TASK_WDT);
            error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));
        }

        if(global_chanctrl_last_ticktime != preTick[2])
        {
            preTick[2] = global_chanctrl_last_ticktime;
            global_selfCheck_counter++;
        }
        else
        {
            /*If this task is not active, log the error to buffer*/
            error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_ACTIVE, TASK_CHAN_CTRL);
            error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));
        }

        if(global_heater_last_ticktime != preTick[3])
        {
            preTick[3] = global_heater_last_ticktime;
            global_selfCheck_counter++;
        }
        else
        {
            /*If this task is not active, log the error to buffer*/
            error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_ACTIVE, TASK_HEATER_CTRL);
            error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));
        }

        if(global_batt_last_ticktime != preTick[4])
        {
            preTick[4] = global_batt_last_ticktime;
            global_selfCheck_counter++;
        }
        else
        {
            /*If this task is not active, log the error to buffer*/
            error_log_the_data(pglobal_err_msg_buffer, ERROR_TASK_NOT_ACTIVE, TASK_POWER_CONV);
            error_log_the_timestamp(pglobal_err_msg_buffer, pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_ms_buffer, getcurrTime_sec(global_prealtimeClock), getcurrTime_ms(global_prealtimeClock));
        }


        if(global_selfCheck_counter == 5) // all other tasks are verified to be running
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

            /*Update the error log to FLASH*/
            fee_package_err_msg_timestamp_s(pglobal_err_msg_timestamp_s_buffer, pglobal_err_msg_timestamp_s_array);
            fee_package_err_msg_timestamp_ms(pglobal_err_msg_timestamp_ms_buffer, pglobal_err_msg_timestamp_ms_array);
            TI_Fee_WriteSync(ERR_MSG_COPY, pglobal_err_msg_buffer);
            TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_S_COPY, pglobal_err_msg_timestamp_s_array);
            TI_Fee_WriteSync(ERR_MSG_TIMESTAMP_MS_COPY, pglobal_err_msg_timestamp_ms_array);

#ifdef DEBUGGING_MODE
            sprintf(str_temp,"%d",(int)global_selfCheck_counter);
            while (sciIsTxReady == 0);
            sciSend(scilinREG,15,(unsigned char *)"Working tasks: ");
            sciSend(scilinREG,strlen((const char *)str_temp),(unsigned char *)str_temp);
            sciSend(scilinREG,4,(unsigned char *)"\r\n\r\n");
#endif

        }

        vTaskDelay(xDelay);

    }
}


/***********************************************
 * Software watchdog timer task:
 * - If there is no command from OBC for 1 min or no command from ground station for 1 week, the system will be reset
 *
 **********************************************/
void sw_wdt_task(void *pvParameters)
{
    const portTickType xDelay = pdMS_TO_TICKS(WDT_TASK_PERIOD);
    vTaskDelay(xDelay);

    while(1)
    {
        if(global_wdt_reset_obc == 0)       //if global reset variable is not set to 1, which means there is no command
        {
            global_wdt_counter_obc--;
        }
        else                                //reset the counter
        {
            global_wdt_counter_obc = WDT_COUNTER_OBC;
            global_wdt_reset_obc = 0;
        }

        if(global_wdt_reset_ground == 0)    //if global reset variable is not set to 1, which means there is no command
        {
            global_wdt_counter_ground--;
        }
        else                                //reset the counter
        {
            global_wdt_counter_ground = WDT_COUNTER_GROUND;
            global_wdt_reset_ground = 0;
        }


        if(global_wdt_counter_obc == 0 || global_wdt_counter_ground == 0)
        {
            systemREG1->SYSECR = 0x8000;        //soft reset the system
        }

        global_wdt_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }

}

/***********************************************
 * Output channel control task:
 * - Read data from sensor data structure (updated by getHK_task).
 * - Update channel data structure.
 * - Detect errors according to the channel data structure and configuration data structure.
 * - Update the hardware according to the channel data structure.
 *
 **********************************************/
void outputchanCtrl_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(CHAN_CTRL_TASK_PERIOD);
    vTaskDelay(xDelay);

    while(1)
    {

        channel_check_mode(pglobal_channelD, global_sys_mode);      //check if the channel is allowed to be on in the current mode
        channel_resume(pglobal_channelD);                           //re-enable task that was disabled for one period
        channel_check_overcurrent_then_config_and_resume(pglobal_channelD, pglobal_ina226D+(NUM_OF_INA226-NUM_OF_INA226_CHANNEL),
                                                         pglobal_RAM_copyD, pglobal_RAM_copyD->sensor_config_data, i2cREG1);

        channel_check_batteryV_then_update_switch(pglobal_channelD, pglobal_battD, pglobal_RAM_copyD);
        channel_check_batteryI_then_update_switch(pglobal_channelD, pglobal_battD, pglobal_RAM_copyD);
        channel_check_chanV_then_update_switch(pglobal_channelD, pglobal_RAM_copyD);

        channel_read_rawdata_and_convert(pglobal_channelD, pglobal_ina226D+(NUM_OF_INA226-NUM_OF_INA226_CHANNEL));


        global_chanctrl_last_ticktime = (uint32_t)xTaskGetTickCount();      //update last ticktime to indicate task activity
        vTaskDelay(xDelay);
    }
}


/***********************************************
 * Heater control task:
 * - Read data from sensor data structure (updated by getHK_task).
 * - Update heater data structure.
 * - Update the hardware according to the heater data structure and configuration data structure.
 *
 **********************************************/
void heaterCtrl_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(HEATER_CTRL_TASK_PERIOD);
    vTaskDelay(xDelay);

    while(1)
    {
        for(global_heater_counter=0; global_heater_counter<NUM_OF_HEATER; global_heater_counter++)
        {
            heater_update_profile(pglobal_heaterD+global_heater_counter, pglobal_RAM_copyD, pglobal_ina3221D, getcurrTime_sec(global_prealtimeClock));
            heater_read_rawdata_and_convert(pglobal_heaterD+global_heater_counter, pglobal_max6698D);
            heater_temp_SW(pglobal_heaterD+global_heater_counter, pglobal_RAM_copyD);
        }

        global_heater_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);
    }
}

/***********************************************
 * Power conversion and battery control task:
 * - Read data from sensor data structure (updated by getHK_task).
 * - Update mppt data structure and battery data structure.
 * - Detect errors according to the battery data structure and configuration data structure.
 * - Update the hardware according to the battery data structure.
 *
 **********************************************/
void powerConversion_and_battCtrl_task(void *pvParameters)
{

    const portTickType xDelay = pdMS_TO_TICKS(POWER_CONV_TASK_PERIOD);
    vTaskDelay(xDelay);


    while(1)
    {

        /*FB_pin Control MPPT Algorithm*/
        for(global_mppt_counter=0;global_mppt_counter<NUM_OF_INA3221;global_mppt_counter++)
        {

            (pglobal_mpptD+global_mppt_counter)->power = 0;

            mppt_getPower_ina226(pglobal_ina226D+global_mppt_counter, pglobal_mpptD+global_mppt_counter);

            mppt_pno_fb(pglobal_mpptD+global_mppt_counter);

            dac_write_fb(mibspiPORT3,pglobal_mpptD+global_mppt_counter, global_mppt_counter);
         }

        /*Control battery switches according to the thresholds*/
        for(global_battery_counter=0;global_battery_counter<NUM_OF_BATTERY_PAIR;global_battery_counter++)
        {
            battery_check_charging_status(pglobal_battD+global_battery_counter, pglobal_ina226D+NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR+global_battery_counter);
            battery_read_rawdata_and_convert(pglobal_battD+global_battery_counter, pglobal_ina226D+NUM_OF_INA226_OVERCURRENT_PROTECTION+NUM_OF_INA226_MONITOR+global_battery_counter, pglobal_max6698D);
            battery_check_temp_then_SW(pglobal_battD+global_battery_counter, pglobal_RAM_copyD);
        }


        global_batt_last_ticktime = (uint32_t)xTaskGetTickCount();
        vTaskDelay(xDelay);

    }
}

/***********************************************
 * Receive command task:
 * - Receive command through serial port.
 * - Execute the command.
 *
 **********************************************/
void receiveCMD_task(void *pvParameters)
{
    unsigned char *cmd1;
//    uint32_t current_sec = 0;

    const portTickType xDelay = pdMS_TO_TICKS(RECEIVE_CMD_TASK_PERIOD);
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

        }
        else
        {
            sciSend(scilinREG,15,(unsigned char *)"Wrong command\r\n");
        }

        vTaskDelay(xDelay);
    }

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

