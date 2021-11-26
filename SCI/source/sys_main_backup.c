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
    uint16_t data= 0;
    uint16_t dataS2 = 0x4126;
    uint8_t dataS[2]={0x41,0x27};
    uint8_t dataR[2]={0};

    _enable_IRQ();
    sciInit();
    sciSend(scilinREG,14,(unsigned char *)"Hello World!\r\n");
    printf("hi!\n");

    int repeat = 0; int delay =0;

        /* I2C Init as per GUI
         *  Mode = Master - Transmitter
         *  baud rate = 100KHz
         *  Count = 10
         *  Bit Count = 8bit
         */
        i2cInit();
//        INA226_Init(i2cREG1, 0x44);
//        INA226_SendData(i2cREG1, 0x44, 0x00, dataS2);
//        INA226_SendData2(i2cREG1, 0x44, 0x00, dataS2);
//        INA226_ReadData(i2cREG1, 0x44, 0x00, &data);
//*****************************************************************
        INA226_ReceiveData2(i2cREG1, 0x44, 0xFE, dataR);
//        i2cSetSlaveAdd(i2cREG1, 0x44);
//
//        /* Set direction to Transmitter */
//        /* Note: Optional - It is done in Init */
//        i2cSetDirection(i2cREG1, I2C_TRANSMITTER);
//
//        /* Configure Data count */
//        /* Slave address + Word address write operation before reading */
//        i2cSetCount(i2cREG1, 0x1);
//
//        /* Set mode as Master */
//        i2cSetMode(i2cREG1, I2C_MASTER);
//
//        /* Set Stop after programmed Count */
//        i2cSetStop(i2cREG1);
//
//        /* Transmit Start Condition */
//        i2cSetStart(i2cREG1);
//
//        /* Send the Word Address */
//        i2cSendByte(i2cREG1, 0xFE);
//
//        /* Wait until Bus Busy is cleared */
//        while(i2cIsBusBusy(i2cREG1) == true);
//
//        /* Wait until Stop is detected */
//        while(i2cIsStopDetected(i2cREG1) == 0);
//
//        /* Clear the Stop condition */
//        i2cClearSCD(i2cREG1);
//
//
//        while(i2cIsMasterReady(i2cREG1) != true);
//        /* Configure address of Slave to talk to */
//        i2cSetSlaveAdd(i2cREG1, 0x44);
//
//        /* Set direction to receiver */
//        i2cSetDirection(i2cREG1, I2C_RECEIVER);
//
//        /* Configure Data count */
//        /* Note: Optional - It is done in Init, unless user want to change */
//        i2cSetCount(i2cREG1, 2);
//
//        /* Set mode as Master */
//        i2cSetMode(i2cREG1, I2C_MASTER);
//
//        /* Set Stop after programmed Count */
//        i2cSetStop(i2cREG1);
//
//        /* Transmit Start Condition */
//        i2cSetStart(i2cREG1);
//
//        /* Tranmit DATA_COUNT number of data in Polling mode */
//        i2cReceive(i2cREG1, 2, dataR);
//
//        /* Wait until Bus Busy is cleared */
//        while(i2cIsBusBusy(i2cREG1) == true);
//
//        /* Wait until Stop is detected */
//        while(i2cIsStopDetected(i2cREG1) == 0);
//
//        /* Clear the Stop condition */
//        i2cClearSCD(i2cREG1);
//        for(delay=0;delay<100000;delay++);
//*****************************************************************

    /////////////////////////////////////////////////////////////
    //       Master Transfer Functionality                      //
    /////////////////////////////////////////////////////////////
        INA226_SendData2(i2cREG1,0x44,0x00,dataS);
//        while(i2cIsMasterReady(i2cREG1) != true);
//        /* Configure address of Slave to talk to */
//        i2cSetSlaveAdd(i2cREG1, 0x44);
//
//        /* Set direction to Transmitter */
//        /* Note: Optional - It is done in Init */
//        i2cSetDirection(i2cREG1, I2C_TRANSMITTER);
//
//        /* Configure Data count */
//        /* Data Count + 1 ( Word Address) */
//        i2cSetCount(i2cREG1, 2 + 1);
//
//        /* Set mode as Master */
//        i2cSetMode(i2cREG1, I2C_MASTER);
//
//        /* Set Stop after programmed Count */
//        i2cSetStop(i2cREG1);
//
//        /* Transmit Start Condition */
//        i2cSetStart(i2cREG1);
//
//        /* Send the Word Address */
//        i2cSendByte(i2cREG1, 0x00);
//
//        /* Tranmit DATA_COUNT number of data in Polling mode */
//        i2cSend(i2cREG1, 2, dataS);
//
//        /* Wait until Bus Busy is cleared */
//        while(i2cIsBusBusy(i2cREG1) == true);
//
//        /* Wait until Stop is detected */
//        while(i2cIsStopDetected(i2cREG1) == 0);
//
//        /* Clear the Stop condition */
//        i2cClearSCD(i2cREG1);
//
//        /* Simple Dealya before starting Next Block */
//        /* Depends on how quick the Slave gets ready */
//        for(delay=0;delay<100000;delay++);

        //*****************************************************************
        ///////////////////////////////////////////////////////////////
        //        Master Receive Functionality                      //
        ///////////////////////////////////////////////////////////////

        /*****************************************/
        //// Setup Slave to receving the data
        /*****************************************/

        /* wait until MST bit gets cleared, this takes
         * few cycles after Bus Busy is cleared */
        INA226_ReceiveData2(i2cREG1, 0x44, 0x00, dataR);
//        while(i2cIsMasterReady(i2cREG1) != true);
//
//        /* Configure address of Slave to talk to */
//        i2cSetSlaveAdd(i2cREG1, 0x44);
//
//        /* Set direction to Transmitter */
//        /* Note: Optional - It is done in Init */
//        i2cSetDirection(i2cREG1, I2C_TRANSMITTER);
//
//        /* Configure Data count */
//        /* Slave address + Word address write operation before reading */
//        i2cSetCount(i2cREG1, 0x1);
//
//        /* Set mode as Master */
//        i2cSetMode(i2cREG1, I2C_MASTER);
//
//        /* Set Stop after programmed Count */
//        i2cSetStop(i2cREG1);
//
//        /* Transmit Start Condition */
//        i2cSetStart(i2cREG1);
//
//        /* Send the Word Address */
//        i2cSendByte(i2cREG1, 0x00);
//
//        /* Wait until Bus Busy is cleared */
//        while(i2cIsBusBusy(i2cREG1) == true);
//
//        /* Wait until Stop is detected */
//        while(i2cIsStopDetected(i2cREG1) == 0);
//
//        /* Clear the Stop condition */
//        i2cClearSCD(i2cREG1);
//
//        /*****************************************/
//        //// Start receving the data From Slave
//        /*****************************************/
//
//        /* wait until MST bit gets cleared, this takes
//         * few cycles after Bus Busy is cleared */
//        while(i2cIsMasterReady(i2cREG1) != true);
//
//        /* Configure address of Slave to talk to */
//        i2cSetSlaveAdd(i2cREG1, 0x44);
//
//        /* Set direction to receiver */
//        i2cSetDirection(i2cREG1, I2C_RECEIVER);
//
//        /* Configure Data count */
//        /* Note: Optional - It is done in Init, unless user want to change */
//        i2cSetCount(i2cREG1, 2);
//
//        /* Set mode as Master */
//        i2cSetMode(i2cREG1, I2C_MASTER);
//
//        /* Set Stop after programmed Count */
//        i2cSetStop(i2cREG1);
//
//        /* Transmit Start Condition */
//        i2cSetStart(i2cREG1);
//
//        /* Tranmit DATA_COUNT number of data in Polling mode */
//        i2cReceive(i2cREG1, 2, dataR);
//
//        /* Wait until Bus Busy is cleared */
//        while(i2cIsBusBusy(i2cREG1) == true);
//
//        /* Wait until Stop is detected */
//        while(i2cIsStopDetected(i2cREG1) == 0);
//
//        /* Clear the Stop condition */
//        i2cClearSCD(i2cREG1);
//        for(delay=0;delay<100000;delay++);



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

void INA226_SendData2(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
    int delay=0;

    while(i2cIsMasterReady(i2c) != true);
    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Data Count + 1 ( Word Address) */
    i2cSetCount(i2c, 2 + 1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Send the Word Address */
    i2cSendByte(i2c, reg);

    /* Transmit DATA_COUNT number of data in Polling mode */
    i2cSend(i2c, 2, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);

    /* Simple Delay before starting Next Block */
    /* Depends on how quick the Slave gets ready */
    for(delay=0;delay<100000;delay++);

}

void INA226_ReceiveData2(i2cBASE_t *i2c, uint8_t addr, uint8_t reg, uint8_t *data)
{
    int delay=0;

    //while(i2cIsMasterReady(i2c) != true);

    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to Transmitter */
    /* Note: Optional - It is done in Init */
    i2cSetDirection(i2c, I2C_TRANSMITTER);

    /* Configure Data count */
    /* Slave address + Word address write operation before reading */
    i2cSetCount(i2c, 0x1);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Send the Word Address */
    i2cSendByte(i2c, reg);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);

    /*****************************************/
    //// Start receving the data From Slave
    /*****************************************/

    /* wait until MST bit gets cleared, this takes
     * few cycles after Bus Busy is cleared */
    while(i2cIsMasterReady(i2c) != true);

    /* Configure address of Slave to talk to */
    i2cSetSlaveAdd(i2c, addr);

    /* Set direction to receiver */
    i2cSetDirection(i2c, I2C_RECEIVER);

    /* Configure Data count */
    /* Note: Optional - It is done in Init, unless user want to change */
    i2cSetCount(i2c, 2);

    /* Set mode as Master */
    i2cSetMode(i2c, I2C_MASTER);

    /* Set Stop after programmed Count */
    i2cSetStop(i2c);

    /* Transmit Start Condition */
    i2cSetStart(i2c);

    /* Tranmit DATA_COUNT number of data in Polling mode */
    i2cReceive(i2c, 2, data);

    /* Wait until Bus Busy is cleared */
    while(i2cIsBusBusy(i2c) == true);

    /* Wait until Stop is detected */
    while(i2cIsStopDetected(i2c) == 0);

    /* Clear the Stop condition */
    i2cClearSCD(i2c);
    for(delay=0;delay<100000;delay++);

}

/* USER CODE END */
