
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

//#define EN_CTRL 1             // en pin controlling
//#define SS_CTRL 1             // ss pin controlling
#define TRAVERSAL 1             // traversal en
//#define TRAVERSAL 2           // traversal ss
//#define TRAVERSAL2 1             // traversal en 4 times with ss changes

#define NUM_OF_SAMPLING     1001

/*****************Housekeeping Data********************/
static unsigned char command;
static ina226_data ina226D[26] = {0};
static ina226_data *pina226D = &ina226D[0];
static uint8_t ina226_counter = 0;
static uint8_t temp[2] = {0};

/*****************Battery Data*************************/
static mppt_data mpptD[4];
static mppt_data *pmpptD = &mpptD[0];
static uint8_t mppt_counter = 0;
static uint16_t sample_counter = 0;
static int Vshunt_buffer[NUM_OF_SAMPLING] = {0};
static int Vbus_buffer[NUM_OF_SAMPLING] = {0};

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

    /*Change the baud rate of I2C interface*/
//    i2cREG1->MDR = i2cREG1->MDR & 0xFFDF;  //set nIRS (bit 5) to 0
//    i2cSetBaudrate(i2cREG1, 100);
//    i2cREG1->MDR = i2cREG1->MDR | 0x20;    //set nIRS (bit 5) to 1

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

//        INA226_SetCfgReg(i2cREG1,ina226D[ina226_counter].address,CFG_4MS_512);
    }

    /*Initialize created data structures*/
    for(mppt_counter=0;mppt_counter<1;mppt_counter++)
    {
        mpptD[mppt_counter].channel = mppt_counter;
        mpptD[mppt_counter].counter = 0;
        mpptD[mppt_counter].dir = 0x00;
        mpptD[mppt_counter].predir = 0x00;
        mpptD[mppt_counter].increment = EN_STEPSIZE_INIT;
        mpptD[mppt_counter].dacOUT = DAC_INIT;
        mpptD[mppt_counter].presumP = 0;
        mpptD[mppt_counter].presumV = 0;
        mpptD[mppt_counter].sumP = 0;
        mpptD[mppt_counter].sumV = 0;
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
    const portTickType xDelay = pdMS_TO_TICKS(100);
    INA226_SetCfgReg(i2cREG1,ina226D[0].address,CFG_REG_SETTING);
//    INA226_SetRegPointer(i2cREG1,ina226D[0].address,CAL_REG);
    vTaskDelay(pdMS_TO_TICKS(100));
    while(1)
    {
//        /* call ina226 functions */
        for(ina226_counter=0;ina226_counter<1;ina226_counter++)
        {
            INA226_GetShuntVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].shunt_voltage);
            INA226_GetVoltage(i2cREG1,ina226D[ina226_counter].address,&ina226D[ina226_counter].bus_voltage);
        }

        vTaskDelay(xDelay);

    }

}


void battCtrl_task(void *pvParameters)
{
    printf( "battery controlling task running\n");
    const portTickType xDelay = pdMS_TO_TICKS(200);

    static int i = 0;
    static int counter = 1;
    static int sweep_counter = 1;

#ifdef EN_CTRL
    dac_write_en_ss(mibspiPORT3,2730);
#endif

#ifdef TRAVERSAL
    if (TRAVERSAL == 1)
        dac_write_en_ss(mibspiPORT3,4095);
#endif


    for(sample_counter=0;sample_counter<NUM_OF_SAMPLING;sample_counter++)
    {
        /*EN_pin Controlling*/
#ifdef EN_CTRL
        for(mppt_counter=0;mppt_counter<1;mppt_counter++)
        {
            dac_write_en(mibspiPORT3,pmpptD+mppt_counter);
            printf("%d\t%d\t%d\n",(int)pmpptD->dacOUT,(int)ina226D[0].shunt_voltage,(int)(mpptD[0].sumP/NUM_AVERAGE));

            (pmpptD+mppt_counter)->sumP = 0;
            vTaskDelay(pdMS_TO_TICKS(10));

            for(i = 0;i<NUM_AVERAGE;i++)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
                mppt_getSumP(pina226D+mppt_counter, pmpptD+mppt_counter);
            }

            mppt_pno_en(pmpptD+mppt_counter);
        }

        vTaskDelay(xDelay);
#endif

        /*SS_pin Controlling*/
#ifdef SS_CTRL
        for(mppt_counter=0;mppt_counter<1;mppt_counter++)
        {
            dac_write_ss(mibspiPORT3,pmpptD+mppt_counter);
            printf("%d\t%d\t%d\n",(int)pmpptD->dacOUT,(int)ina226D[0].shunt_voltage,(int)ina226D[0].bus_voltage);

            (pmpptD+mppt_counter)->sumP = 0;
            vTaskDelay(pdMS_TO_TICKS(10));

            for(i = 0;i<NUM_AVERAGE;i++)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
                mppt_getSumP(pina226D+mppt_counter, pmpptD+mppt_counter);
            }

            mppt_pno_ss(pmpptD+mppt_counter);
        }


        vTaskDelay(xDelay);
#endif

        /*Traversal*/
#ifdef TRAVERSAL

        if (TRAVERSAL == 1)
            dac_write_en(mibspiPORT3,pmpptD);
        else
            dac_write_ss(mibspiPORT3,pmpptD);

//        printf("%d\t%d\t%d\n",(int)pmpptD->dacOUT,(int)ina226D[0].shunt_voltage,(int)ina226D[0].bus_voltage);
//        sprintf(Vshunt_buffer,"%d",(int)ina226D[0].shunt_voltage);
//        sprintf(Vbus_buffer,"%d",(int)ina226D[0].bus_voltage);
        Vshunt_buffer[sample_counter] = (int)ina226D[0].shunt_voltage;
        Vbus_buffer[sample_counter] = (int)ina226D[0].bus_voltage;

        if(pmpptD->dacOUT >= DAC_MAX)
        {
            counter = 1;
        }
        else if(pmpptD->dacOUT < EN_STEPSIZE_INIT)
        {
            counter = 0;
        }

        if(counter == 0)
        {

            if (pmpptD->dacOUT > 3850 && pmpptD->dacOUT < 3950)
                pmpptD->dacOUT = pmpptD->dacOUT+EN_STEPSIZE_INIT;
            else if ((pmpptD->dacOUT+EN_STEPSIZE_INIT) >= DAC_MAX)
                pmpptD->dacOUT = DAC_MAX;
            else
                pmpptD->dacOUT = pmpptD->dacOUT+EN_STEPSIZE_INIT;
        }
        else
        {
            if (pmpptD->dacOUT < EN_STEPSIZE_INIT)
                pmpptD->dacOUT = 0;
            else
                pmpptD->dacOUT = pmpptD->dacOUT-EN_STEPSIZE_INIT;;
        }


        vTaskDelay(xDelay);

        //printf("DAC out:%d\n",(int)mpptD[0].dacOUT);
#endif

#ifdef TRAVERSAL2

        if(pmpptD->dacOUT == DAC_MAX)
        {
            dac_write_en_ss(mibspiPORT3, (uint16_t)(2730));
        }

        dac_write_en(mibspiPORT3,pmpptD);

        printf("%d\t%d\t%d\n",(int)pmpptD->dacOUT,(int)ina226D[0].shunt_voltage,(int)ina226D[0].bus_voltage);

        if(pmpptD->dacOUT == DAC_MAX)
        {
            pmpptD->dacOUT = 0;
            sweep_counter++;
        }
        else
        {
            if (pmpptD->dacOUT < EN_STEPSIZE_INIT)
                pmpptD->dacOUT = 0;
            else if(pmpptD->dacOUT + EN_STEPSIZE_INIT > DAC_MAX)
                pmpptD->dacOUT = DAC_INIT;
            else
                pmpptD->dacOUT = pmpptD->dacOUT+EN_STEPSIZE_INIT;;
        }


        vTaskDelay(xDelay);
        //printf("DAC out:%d\n",(int)mpptD[0].dacOUT);
#endif


//        vTaskDelay(xDelay);

    }

    for(sample_counter=0;sample_counter<NUM_OF_SAMPLING;sample_counter++)
    {
        printf("%d\t%d\n",Vshunt_buffer[sample_counter],Vbus_buffer[sample_counter]);
    }

    printf("Sampling completed.\n");
    vTaskSuspend(battCtrl_task);
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

