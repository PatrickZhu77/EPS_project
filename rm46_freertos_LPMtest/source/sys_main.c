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
/* Include FreeRTOS scheduler files */
#include "system.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "sys_core.h"
#include "sys_vim.h"
#include "stdio.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "het.h"
#include "gio.h"
#include "rti.h"
#include "reg_rti.h"
#include "reg_system.h"
#include "sci.h"
//#include "spi.h"

/* Define Task Handles */
xTaskHandle xTask1Handle;

uint8_t dir = 0;

void post_wakeup(void);
void enter_doze(void);

void post_wakeup(void);
void enter_snooze(void);

#define DOZE    1
#define SNOOZE  2
#define SLEEP   3


/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        /* Taggle HET[1] with timer tick */
        gioSetBit(gioPORTB, 1, dir);
        dir = ~dir;

        sciSend(scilinREG,3,(unsigned char *)"0\r\n");

        if ((gioGetPort(gioPORTA) & 0x3) == SNOOZE)
        {
//            flashWREG->FPAC2 = 0x7;
//
//            /* Bank 7 */
//            flashWREG->FMAC = 0x6;
//            flashWREG->FBAC = 0x70F;
//
//            /* Bank 0 */
//            flashWREG->FMAC = 0x0;
//            flashWREG->FBAC = 0x70F;
//
//            /* Setup flash module to change fallback modes for banks/pump to be "sleep" */
//            flashWREG->FBFALLBACK = 0x00000000U
//                                  | (uint32)((uint32)SYS_SLEEP << 14U) /* BANK 7 */
//                                  | (uint32)((uint32)SYS_SLEEP << 2U)  /* BANK 1 */
//                                  | (uint32)((uint32)SYS_SLEEP << 0U); /* BANK 0 */
//
//            /* Disable oscillator monitoring to prevent detection of osc fail */
//            systemREG1->CLKTEST = 0x010A0000;


            enter_snooze();                           // wake up via RTI clocked by OSCIN

            post_wakeup();
        }


//        if(gioGetBit(spiPORT1, 4)==0)
//        {
//            printf("button1 = 1\r\n");
//        }
//        else
//        {
//            printf("button1 = 0\r\n");
//        }
//
//        if(gioGetBit(spiPORT3, 0)==0)
//        {
//            printf("button2 = 1\r\n");
//        }
//        else
//        {
//            printf("button2 = 0\r\n");
//        }



//        printf("task1\r\n");
        vTaskDelay(1000);
    }
}
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

    /* Set high end timer GIO port hetPort pin direction to all output */
//    gioSetDirection(hetPORT1, 0xFFFFFFFF);

    gioInit();
    sciInit();
//    spiInit();

    vimREG->WAKEMASKSET0 = 1<<2 | 1<<3;

    systemREG1->RCLKSRC = 0x4;

    /* Enable IRQ and FIQ inside CPU */
    _enable_interrupt_();

    gioPORTA->DIR &= 0xFC; //GIOA[1:0] used as input

    /* Create Task 1 */
    xTaskCreate(vTask1,"Task1", 128, NULL, (1|portPRIVILEGE_BIT), &xTask1Handle);
//    {
//        /* Task could not be created */
//        while(1);
//    }

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void enter_snooze(void)
{

    sciSend(scilinREG,3,(unsigned char *)"1\r\n");

    /* RTI is configured to generate compare 0 interrupt every 1 second using 16MHz OSCIN as source */
    /* Change this to use the LF LPO, which is typically 80KHz */
//    rtiStopCounter(rtiCOUNTER_BLOCK1);
//    rtiResetCounter(rtiCOUNTER_BLOCK1);
//
//    sciSend(scilinREG,3,(unsigned char *)"2\r\n");
//
    /* Clock RTI using LF LPO, the 80KHz clock source */
    systemREG1->RCLKSRC = 0x4;
//
//    /** - Setup compare 0 value. This value is compared with selected free running counter. */
//    rtiREG1->CMP[1U].COMPx = 400000U;
//
//    /** - Setup update compare 0 value. This value is added to the compare 0 value on each compare match. */
//    rtiREG1->CMP[1U].UDCPx = 400000U;
//
//    /** - Clear all pending interrupts */
//    rtiREG1->INTFLAG = 0x0007000FU;
//
//    /** - Disable all interrupts */
//    rtiREG1->CLEARINTENA = 0x00070F0FU;
//
//    /** - Enable RTI Compare 0 Interrupt **/
//    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE1;
//
//    sciSend(scilinREG,3,(unsigned char *)"3\r\n");

/*******************************************/
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

    sciSend(scilinREG,5,(unsigned char *)"3.4\r\n");

    /* Disable oscillator monitoring to prevent detection of osc fail */
    systemREG1->CLKTEST = 0x010A0000;

    sciSend(scilinREG,3,(unsigned char *)"4\r\n");

/*******************************************/

    /* Start counter 0 */
//    rtiStopCounter(rtiCOUNTER_BLOCK0);
//    rtiStartCounter(rtiCOUNTER_BLOCK1);

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
    /* toggle GIOA[7] for signaling wake-up */
    //gioSetPort(gioPORTB, gioGetPort(gioPORTB)^0x02);    // toggle GIOB[1]
//    gioSetPort(gioPORTB, (0x1 << 1));    // set GIOB[1]

    gioSetPort(gioPORTB, (0x1 << 2));

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

//    gioSetPort(gioPORTB, (0x1 << 2));

    sciSend(scilinREG,3,(unsigned char *)"5\r\n");

    vTaskStepTick(400);

//    rtiStopCounter(rtiCOUNTER_BLOCK0);
//    rtiResetCounter(rtiCOUNTER_BLOCK0);
//
//    rtiStartCounter(rtiCOUNTER_BLOCK0);

    /* Restore original clock source/domain bindings */
    mapClocks();

    /* Resume oscillator monitoring */
    systemREG1->CLKTEST = 0x000A0000;

    sciSend(scilinREG,3,(unsigned char *)"6\r\n");

}


/* USER CODE BEGIN (4) */
void rtiNotification(uint32 notification)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
    //gioToggleBit(gioPORTB, 0x2);  //GIOB[2]
//    gioSetPort(gioPORTB, (0x1 << 2));    // set GIOB[2]

}


/* USER CODE END */
