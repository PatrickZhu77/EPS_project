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
#include "spi.h"
#include "stdio.h"
#include "gio.h"
#include "het.h"
#include "i2c.h"
#include "ina226.h"
#include "ad5324.h"
#include "mppt.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

//    uint32_t shunt_v[1]={0},bus_v[1]={0},current[1]={0},power[1]={0};

    spiDAT1_t dataconfig1_t;

    dataconfig1_t.CS_HOLD = FALSE;
    dataconfig1_t.WDEL    = TRUE;
    dataconfig1_t.DFSEL   = SPI_FMT_0;
    dataconfig1_t.CSNR    = 0xFE;


    /* Enable CPU Interrupt through CPSR */
    _enable_IRQ();

    gioInit();
//    hetInit();
    spiInit();
    i2cInit();

//    uint16_t cal_temp[1] = {0x400};


    /*ina226*/
//    INA226_SetCalReg(i2cREG1,0x41,cal_temp);
//    INA226_GetShuntVoltage(i2cREG1,0x41,shunt_v);
//    INA226_GetVoltage(i2cREG1,0x41,bus_v);
//    INA226_GetCurrent(i2cREG1,0x41,current);
//    INA226_GetPower(i2cREG1,0x41,power);

    /*ad5324*/
    gioSetDirection(hetPORT2, 0x40); //0x40 -> 100_0000, set N2HET2[6] as output, others for input
    gioToggleBit(hetPORT2, 0x40);

    uint16_t p[1]={0x6FFF};
    gioSetBit(hetPORT2,6,0);
//    gioSetBit(gioPORTA,7,0);

    spiTransmitData(spiREG3, &dataconfig1_t, 1, p);

    gioSetBit(hetPORT2,6,1);
//    gioSetBit(gioPORTA,7,1);


    while(1);

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
