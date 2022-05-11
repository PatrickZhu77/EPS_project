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
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
static unsigned char command;
static unsigned char cmd1[];
static uint8_t len, counter;
static int flag1;

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
    _enable_IRQ();

    sciInit();
    flag1 = 0;
    counter = 0;
    //int i;

    while(1)
    {

        //i=0;
        //sciSend(scilinREG,2,(unsigned char *)"\r\n");
        sciSend(scilinREG,21,(unsigned char *)"Please press a key!\r\n");

        len = 5;
        sciReceive(scilinREG,1,(unsigned char *)&command);

//      sciReceive(scilinREG,1,(unsigned char *)&cmd1);

        while(flag1 == 0){}
        flag1 = 0;
        len = 1;
        counter = 0;


        sciSend(scilinREG,17,(unsigned char *)"Command in cmd1: ");
        sciSend(scilinREG,strlen((const char *)cmd1),(unsigned char *)&cmd1);
        sciSend(scilinREG,2,(unsigned char *)"\r\n");

        sciSend(scilinREG,7,(unsigned char *)"Done!\r\n");

        //printf("Command in cmd1: %s\n", cmd1);
        cmd1[0]='\0';
        //while(i<10000){i++;}
/******************************************************************************/
        sciSend(scilinREG,25,(unsigned char *)"Please press a new key!\r\n");

        len = 3;
        sciReceive(scilinREG,1,(unsigned char *)&command);

//      sciReceive(scilinREG,1,(unsigned char *)&cmd1);

        while(flag1 == 0){}
        flag1 = 0;
        len = 1;
        counter = 0;


        sciSend(scilinREG,17,(unsigned char *)"Command in cmd1: ");
        sciSend(scilinREG,strlen((const char *)cmd1),(unsigned char *)&cmd1);
        sciSend(scilinREG,2,(unsigned char *)"\r\n");

        sciSend(scilinREG,7,(unsigned char *)"Done!\r\n");

        //printf("Command in cmd1: %s\n", cmd1);
        cmd1[0]='\0';
        //while(i<10000){i++;}


    }
//    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void sciNotification(sciBASE_t *sci,unsigned flags)
{
    cmd1[counter]=command;
    sciSend(sci,1,(unsigned char *)&command);
    sciReceive(sci,1,(unsigned char *)&command);
    counter++;

    if(counter >= len)
    {
        cmd1[counter] = '\0';
        sciSend(sci,2,(unsigned char *)"\r\n");
        flag1 = 1;
        return;
    }
    //sciReceive(sci,1,(unsigned char *)&command);
    //sciSend(scilinREG,2,(unsigned char *)" ");

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
