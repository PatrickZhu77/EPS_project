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
#include "sci.h"
#include "i2c.h"
#include "ina226.h"
#include "ina3221.h"
#include "stdio.h"

/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
static unsigned char command;
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void INA226_SendData2(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);
void INA226_ReceiveData2(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    uint8_t dataS[2]={0x41,0x26};
    uint8_t dataR[2]={0};
    double dataSV,dataBV;

    _enable_IRQ();
    sciInit();
    sciSend(scilinREG,14,(unsigned char *)"Hello World!\r\n");
    printf("hi!\n");


        /* I2C Init as per GUI
         *  Mode = Master - Transmitter
         *  Bit Count = 8bit
         */
        i2cInit();
//        INA226_Init(i2cREG1, 0x44);
/*
        INA226_ReceiveData(i2cREG1, 0x44, 0xFE, dataR);
        printf("Read1\n");

        INA226_SendData(i2cREG1,0x44,0x00,dataS);
        printf("Write\n");

        INA226_ReceiveData(i2cREG1, 0x44, 0x00, dataR);
        printf("Read2\n");
*/
        INA226_GetShuntVoltage(i2cREG1,0x44,&dataSV);
        printf("Shunt Voltage: %f mV.\n",dataSV);

        INA226_GetVoltage(i2cREG1,0x44,&dataBV);
        printf("Shunt Voltage: %f V.\n",dataBV);

    while(1);



/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void sciNotification(sciBASE_t *sci,unsigned flags)
{
    sciSend(sci,1,(unsigned char *)&command);
    sciReceive(sci,1,(unsigned char *)&command);
}

void i2cNotification(i2cBASE_t *i2c,unsigned flags)
{
    return;
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
