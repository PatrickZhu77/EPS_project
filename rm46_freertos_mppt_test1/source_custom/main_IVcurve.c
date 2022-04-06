
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
#include "reg_het.h"
#include "i2c.h"
#include "mibspi.h"
#include "het.h"

#include "task_header.h"
#include "ina226.h"
#include "mppt.h"
#include "ad5324.h"


/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

#define DAC_MIN  4090
#define DAC_MAX  4095
#define DAC_INIT 4090
#define STEP_SIZE 1


/*****************Housekeeping Data********************/
static unsigned char command;
static ina226_data ina226D[26] = {0};
static ina226_data *pina226D = &ina226D[0];
static uint8_t ina226_counter = 0;

/*****************Battery Data*************************/
static mppt_data mpptD[4];
static mppt_data *pmpptD = &mpptD[0];
static uint8_t mppt_counter = 0;
static uint16_t counter = 0;
static uint32_t avgP = 0;
static uint32_t last_avgP = 0;


//static uint16_t binary[16] = {0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1};
//static uint16_t binary[16] = {0};

//void get_binary(uint16_t data);

void delay(void)
{
    unsigned int dummycnt=0x0000FFU;
    do
    {
        dummycnt--;
    }
    while(dummycnt>0);
}

void mppt_hunts2(mppt_data *data)
{
    if(data->dir == data->predir)                       //when the direction does not change
    {
        if(data->counter >= 3)               //when change with same direction for 4 times
         {
            if(data->increment < 128)         //maximum increment is 8 times of standard (INCOMPLETED!!!!!!!!!!!!!!)
            {
                data->increment = data->increment << 1;       //increment is doubled
            }

            data->counter = 0;
         }
     }
    else
    {

        if(data->increment > 32)
        {
            data->increment = data->increment >> 1;                //increment is halved
        }
        data->predir = data->dir;
    }
 }



/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    _enable_IRQ();
    sciInit();
    gioInit();
    i2cInit();
    hetInit();
    mibspiInit();

    xTaskCreate((TaskFunction_t )init_task,
                (const char*    )"init_task",
                (uint16_t       )init_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )init_TASK_PRIO,
                (TaskHandle_t*  )&initTask_Handler);
    printf("init_task created\n");


    vTaskStartScheduler();


/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void init_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    int i = 0;

//    uint16_t ina226_config_data=0x4127;
//    uint16_t ina226_cal_data=0x0400;
//    uint16_t cal_temp[1] = {0x800}; //2048


    /*Initialize sensors*/
    for(ina226_counter=0;ina226_counter<1;ina226_counter++)
    {
        ina226D[ina226_counter].address = 0x41;
//        ina226D[ina226_counter].config_reg = ina226_config_data;
//        ina226D[ina226_counter].cal_reg = ina226_cal_data;
//        INA226_Init(i2cREG1, ina226D[ina226_counter].address, pina226D+ina226_counter);
//        INA226_SetCalReg(i2cREG1,0x41,cal_temp);
    }

    /*Initialize created data structures*/
    for(mppt_counter=0;mppt_counter<1;mppt_counter++)
    {
        mpptD[mppt_counter].channel = mppt_counter;
        mpptD[mppt_counter].counter = 0;
        mpptD[mppt_counter].dir = 0xff;
        mpptD[mppt_counter].predir = 0xff;
        mpptD[mppt_counter].increment = 32;
        for(i=0;i<NUM_AVERAGE;i++)
        {
            mpptD[mppt_counter].preP[i] = 3000000;  //Vshunt*Vbus
            mpptD[mppt_counter].preV[i] = 3000;   //LSB 1.25mV (~5V)
        }
        mpptD[mppt_counter].dacOUT = DAC_INIT;
    }


    xTaskCreate((TaskFunction_t )getHK_task,
                (const char*    )"getHK_task",
                (uint16_t       )getHK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )getHK_TASK_PRIO,
                (TaskHandle_t*  )&getHKTask_Handler);

    xTaskCreate((TaskFunction_t )battCtrl_task,
                (const char*    )"battCtrl_task",
                (uint16_t       )battCtrl_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )battCtrl_TASK_PRIO,
                (TaskHandle_t*  )&battCtrlTask_Handler);

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
        for(ina226_counter=0;ina226_counter<1;ina226_counter++)
        {
            INA226_GetShuntVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].shunt_voltage);
            INA226_GetVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].bus_voltage);
//            INA226_GetCurrent(i2cREG1, ina226D[ina226_counter].address, &ina226D[ina226_counter].current);
//            INA226_GetPower(i2cREG1, ina226D[ina226_counter].address, &ina226D[ina226_counter].power);

        }


        vTaskDelay(xDelay);

    }

}


void battCtrl_task(void *pvParameters)
{
    printf( "battery controlling task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(100);

    static uint8_t  dac_direction = 1;
    static uint8_t i = 0;
    uint32_t p = 0;
    counter = 0;

    while(1)
    {
        if(pmpptD->dacOUT >= DAC_MAX-STEP_SIZE)
        {
            counter = 1;
        }
        else if(pmpptD->dacOUT < DAC_MIN+STEP_SIZE)
        {
            counter = 0;
        }

        if(counter == 0)
        {

            if (pmpptD->dacOUT > 3850 && pmpptD->dacOUT < 3950)
                pmpptD->dacOUT = pmpptD->dacOUT+32;
            else
                pmpptD->dacOUT = pmpptD->dacOUT+STEP_SIZE;
        }
        else
        {
            pmpptD->dacOUT = pmpptD->dacOUT-STEP_SIZE;;
        }
        printf("%d\t%d\t%d",(int)pmpptD->dacOUT,(int)ina226D[0].shunt_voltage,(int)ina226D[0].bus_voltage);
        dac_write_ss(mibspiPORT3,pmpptD);
        vTaskDelay(pdMS_TO_TICKS(2000));
//        mppt_hunts2(pmpptD);
//        dac_write_ss(mibspiPORT3,pmpptD);
//        vTaskDelay(xDelay);
//



        last_avgP = avgP;



        printf("\n");



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

