#include "low_power_mode.h"

/***********************************************
 * Enter the doze mode:
 * - All the interrupts, besides main OSC, are disabled
 * - All the clock domains, besides RTICLK, are turned off
 * - Flash module are set to sleep mode
 *
 **********************************************/
void enter_doze(void)
{
    /* set up flash pump active grace period as 7 HCLK/16 cycles */
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


    /* Clock RTI using LF LPO, the 80KHz clock source */
    systemREG1->RCLKSRC |= 0x4;

    /** - Setup compare 0 value. This value is compared with selected free running counter. */
    rtiREG1->CMP[1U].COMPx = 400000U;//(uint32_t)xTaskGetExpectedIdleTime();//400000U;

    /** - Setup update compare 0 value. This value is added to the compare 0 value on each compare match. */
    rtiREG1->CMP[1U].UDCPx = 400000U;//(uint32_t)xTaskGetExpectedIdleTime();//400000U;

    /** - Clear all pending interrupts */
    rtiREG1->INTFLAG = 0x0007000FU;

    /** - Disable all interrupts */
    rtiREG1->CLEARINTENA = 0x00070F0FU;

    /** - Enable RTI Compare 0 Interrupt **/
    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE1;


    /* Start counter 0 */
//    rtiStopCounter(rtiCOUNTER_BLOCK0);
//    rtiStartCounter(rtiCOUNTER_BLOCK1);

    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG1->GHVSRC = (uint32)((uint32)SYS_OSC << 24U)
                       | (uint32)((uint32)SYS_OSC << 16U)
                       | (uint32)((uint32)SYS_PLL1 << 0U);

    /* Enable low-power modes */
    systemREG1->VRCTL = 0x0F;

    /* turn off all clock sources except LF LPO */
    systemREG1->CSDISSET = 0xFE;

    /* turn off all clock domains except RTICLK */
    systemREG1->CDDISSET = 0xFFBF;


    asm(" WFI");
    asm(" nop");
    asm(" nop");
    asm(" nop");
    asm(" nop");

}



/***********************************************
 * Enter the snooze mode:
 * - All the interrupts, besides RTI, are disabled
 * - All the clock domains, besides RTICLK, are turned off
 * - Flash module are set to sleep mode
 *
 **********************************************/
void enter_snooze(void)
{
    /* set up flash pump active grace period as 7 HCLK/16 cycles */
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


    /* Clock RTI using LF LPO, the 80KHz clock source */
    systemREG1->RCLKSRC |= 0x4;

    /** - Setup compare 0 value. This value is compared with selected free running counter. */
    rtiREG1->CMP[1U].COMPx = 400000U;//(uint32_t)xTaskGetExpectedIdleTime();//400000U;

    /** - Setup update compare 0 value. This value is added to the compare 0 value on each compare match. */
    rtiREG1->CMP[1U].UDCPx = 400000U;//(uint32_t)xTaskGetExpectedIdleTime();//400000U;

    /** - Clear all pending interrupts */
    rtiREG1->INTFLAG = 0x0007000FU;

    /** - Disable all interrupts */
    rtiREG1->CLEARINTENA = 0x00070F0FU;

    /** - Enable RTI Compare 0 Interrupt **/
    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE1;


    /* Start counter 0 */
//    rtiStopCounter(rtiCOUNTER_BLOCK0);
//    rtiStartCounter(rtiCOUNTER_BLOCK1);

    /** - Setup GCLK, HCLK and VCLK clock source for normal operation, power down mode and after wakeup */
    systemREG1->GHVSRC = (uint32)((uint32)SYS_OSC << 24U)
                       | (uint32)((uint32)SYS_OSC << 16U)
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


/***********************************************
 * Wake the system up from the snooze mode:
 * - Resume clock sources and domains
 * - Resume Flash module
 * - Resume interrupts
 *
 **********************************************/
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

//    gioSetPort(gioPORTB, (0x1 << 2));


    vTaskStepTick((uint32_t)xTaskGetExpectedIdleTime());


    /* Restore original clock source/domain bindings */
    mapClocks();

    /* Resume oscillator monitoring */
    systemREG1->CLKTEST = 0x000A0000;

    rtiREG1->SETINTENA = rtiNOTIFICATION_COMPARE0;

}
