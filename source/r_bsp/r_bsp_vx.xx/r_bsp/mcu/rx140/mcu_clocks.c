/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : mcu_clocks.c
* Description  : Contains clock specific routines
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2021 1.00     First Release
*         : 20.08.2021 1.01     Fixed compile switch of PLL clock settings.
*                               Added the Waiting for the IWDT clock oscillation stabilization in 
*                               operating_frequency_set function.
*         : 30.11.2021 1.02     Added comments for when use simulator.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define BSP_PRV_CKSEL_LOCO            (0x0)
#define BSP_PRV_CKSEL_HOCO            (0x1)
#define BSP_PRV_CKSEL_MAIN_OSC        (0x2)
#define BSP_PRV_CKSEL_SUBCLOCK        (0x3)
#define BSP_PRV_CKSEL_PLL             (0x4)

#define BSP_PRV_NORMALIZE_X10  (10)   /* used to avoid floating point arithmetic */

/* This macro runs or stops the PLL circuit.
   If the following conditions are satisfied, PLL circuit will operate.
   1. System clock source is PLL circuit.
   2. Clock output enable and clock output source is PLL circuit.
 */
#if (BSP_CFG_CLOCK_SOURCE == 4) \
     || ((BSP_CFG_CLKOUT_OUTPUT == 1) && (BSP_CFG_CLKOUT_SOURCE == 4))
    #define BSP_PRV_PLL_CLK_OPERATING    (1)    /* PLL circuit is operating. */
#else /* PLL is not used as clock source. */
    #define BSP_PRV_PLL_CLK_OPERATING    (0)    /* PLL circuit is stopped. */
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0
static void operating_frequency_set(void);
static void clock_source_select(void);
static void lpt_clock_source_select(void);
#if BSP_CFG_CLKOUT_OUTPUT != 0
/* CLKOUT initial configuration function declaration */
static void bsp_clkout_initial_configure(void);
#endif /* BSP_CFG_CLKOUT_OUTPUT != 0 */
#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

/***********************************************************************************************************************
* Function Name: get_iclk_freq_hz
* Description  : Return the current ICLK frequency in Hz. Called by R_BSP_GetIClkFreqHz().
*                The system clock source can be changed at any time via SYSTEM.SCKCR3.BIT.CKSEL, so in order to
*                determine the ICLK frequency we need to first find the current system clock source and then,
*                in some cases where the clock source can be configured for multiple frequencies, calculate the
*                frequency at which it is currently running.
* Arguments    : None
* Return Value : uint32_t - the iclk frequency in Hz
***********************************************************************************************************************/
uint32_t get_iclk_freq_hz(void)
{
    uint32_t sys_clock_src_freq;
    uint32_t pll_multiplier;

    /* Casting is valid because it matches the type to the retern value. */
    uint8_t  cksel = (uint8_t)SYSTEM.SCKCR3.BIT.CKSEL;

    switch (cksel)
    {
        case BSP_PRV_CKSEL_LOCO:
            sys_clock_src_freq = BSP_LOCO_HZ;
            break;

        case BSP_PRV_CKSEL_HOCO:
            sys_clock_src_freq = BSP_HOCO_HZ;
            break;

        case BSP_PRV_CKSEL_MAIN_OSC:
            sys_clock_src_freq = BSP_CFG_XTAL_HZ;
            break;

        case BSP_PRV_CKSEL_SUBCLOCK:
            sys_clock_src_freq = BSP_SUB_CLOCK_HZ;
            break;

        case BSP_PRV_CKSEL_PLL:

            /* Casting is valid because it matches the type to the retern value. */
            pll_multiplier = ((((uint32_t)(SYSTEM.PLLCR.BIT.STC + 1)) * BSP_PRV_NORMALIZE_X10) / 2);

            /* Casting is valid because it matches the type to the retern value. */
            sys_clock_src_freq = ((BSP_CFG_XTAL_HZ / (((uint32_t)(1 << SYSTEM.PLLCR.BIT.PLIDIV)) * BSP_PRV_NORMALIZE_X10)) * pll_multiplier);
            break;

        default:

            /* Should never arrive here. Use the Main OSC freq as a default... */
            sys_clock_src_freq = BSP_CFG_XTAL_HZ;
            break;
    }

    /* Finally, divide the system clock source frequency by the currently set ICLK divider to get the ICLK frequency */
    return (sys_clock_src_freq / (uint32_t)(1 << SYSTEM.SCKCR.BIT.ICK));
} /* End of function get_iclk_freq_hz() */

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0

/***********************************************************************************************************************
* Function name: mcu_clock_setup
* Description  : Contains clock functions called at device restart.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void mcu_clock_setup(void)
{
    /* Switch to high-speed operation */
    operating_frequency_set();
    lpt_clock_source_select();

#if BSP_CFG_CLKOUT_OUTPUT != 0
    bsp_clkout_initial_configure();
#endif /* BSP_CFG_CLKOUT_OUTPUT != 0 */
} /* End of function mcu_clock_setup() */

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void operating_frequency_set (void)
{
    /* Used for constructing value to write to SCKCR, and SCKCR3 registers. */
    uint32_t tmp_clock = 0;

    /* Protect off. */
    SYSTEM.PRCR.WORD = 0xA50F;

    /* Select the clock based upon user's choice. */
    clock_source_select();

    /* Figure out setting for FCK bits. */
#if   BSP_CFG_FCK_DIV == 1
    /* Do nothing since FCK bits should be 0. */
#elif BSP_CFG_FCK_DIV == 2
    tmp_clock |= 0x10000000;
#elif BSP_CFG_FCK_DIV == 4
    tmp_clock |= 0x20000000;
#elif BSP_CFG_FCK_DIV == 8
    tmp_clock |= 0x30000000;
#elif BSP_CFG_FCK_DIV == 16
    tmp_clock |= 0x40000000;
#elif BSP_CFG_FCK_DIV == 32
    tmp_clock |= 0x50000000;
#elif BSP_CFG_FCK_DIV == 64
    tmp_clock |= 0x60000000;
#else
    #error "Error! Invalid setting for BSP_CFG_FCK_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for ICK bits. */
#if   BSP_CFG_ICK_DIV == 1
    /* Do nothing since ICK bits should be 0. */
#elif BSP_CFG_ICK_DIV == 2
    tmp_clock |= 0x01000000;
#elif BSP_CFG_ICK_DIV == 4
    tmp_clock |= 0x02000000;
#elif BSP_CFG_ICK_DIV == 8
    tmp_clock |= 0x03000000;
#elif BSP_CFG_ICK_DIV == 16
    tmp_clock |= 0x04000000;
#elif BSP_CFG_ICK_DIV == 32
    tmp_clock |= 0x05000000;
#elif BSP_CFG_ICK_DIV == 64
    tmp_clock |= 0x06000000;
#else
    #error "Error! Invalid setting for BSP_CFG_ICK_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for PCKB bits. */
#if   BSP_CFG_PCKB_DIV == 1
    /* Do nothing since PCKB bits should be 0. */
#elif BSP_CFG_PCKB_DIV == 2
    tmp_clock |= 0x00000100;
#elif BSP_CFG_PCKB_DIV == 4
    tmp_clock |= 0x00000200;
#elif BSP_CFG_PCKB_DIV == 8
    tmp_clock |= 0x00000300;
#elif BSP_CFG_PCKB_DIV == 16
    tmp_clock |= 0x00000400;
#elif BSP_CFG_PCKB_DIV == 32
    tmp_clock |= 0x00000500;
#elif BSP_CFG_PCKB_DIV == 64
    tmp_clock |= 0x00000600;
#else
    #error "Error! Invalid setting for BSP_CFG_PCKB_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for PCKD bits. */
#if   BSP_CFG_PCKD_DIV == 1
    /* Do nothing since PCKD bits should be 0. */
#elif BSP_CFG_PCKD_DIV == 2
    tmp_clock |= 0x00000001;
#elif BSP_CFG_PCKD_DIV == 4
    tmp_clock |= 0x00000002;
#elif BSP_CFG_PCKD_DIV == 8
    tmp_clock |= 0x00000003;
#elif BSP_CFG_PCKD_DIV == 16
    tmp_clock |= 0x00000004;
#elif BSP_CFG_PCKD_DIV == 32
    tmp_clock |= 0x00000005;
#elif BSP_CFG_PCKD_DIV == 64
    tmp_clock |= 0x00000006;
#else
    #error "Error! Invalid setting for BSP_CFG_PCKD_DIV in r_bsp_config.h"
#endif

    /* Set SCKCR register. */
    SYSTEM.SCKCR.LONG = tmp_clock;

    /* Dummy read and compare. cf."5. I/O Registers", "(2) Notes on writing to I/O registers" in User's manual.
       This is done to ensure that the register has been written before the next register access. The RX has a 
       pipeline architecture so the next instruction could be executed before the previous write had finished.
    */
    if(tmp_clock == SYSTEM.SCKCR.LONG)
    {
        R_BSP_NOP();
    }

    /* Choose clock source. Default for r_bsp_config.h is PLL. */
    tmp_clock = ((uint16_t)BSP_CFG_CLOCK_SOURCE) << 8;

    /* Casting is valid because it matches the type to the retern value. */
    SYSTEM.SCKCR3.WORD = (uint16_t)tmp_clock;

    /* Dummy read and compare. cf."5. I/O Registers", "(2) Notes on writing to I/O registers" in User's manual.
       This is done to ensure that the register has been written before the next register access. The RX has a 
       pipeline architecture so the next instruction could be executed before the previous write had finished.
    */
    if((uint16_t)tmp_clock == SYSTEM.SCKCR3.WORD)
    {
        R_BSP_NOP();
    }

#if BSP_CFG_IWDT_CLOCK_OSCILLATE_ENABLE == 1
    /* IWDT clock is stopped after reset. Oscillate the IWDT. */
    SYSTEM.ILOCOCR.BIT.ILCSTP = 0;

    /* Wait processing for the IWDT clock oscillation stabilization (80us) */
    R_BSP_SoftwareDelay((uint32_t)80, BSP_DELAY_MICROSECS);
#endif

#if BSP_CFG_LOCO_OSCILLATE_ENABLE == 0
    /* We can now turn LOCO off since it is not going to be used. */
    SYSTEM.LOCOCR.BYTE = 0x01;

    /* Wait for five the LOCO cycles */
    /* 5 count of LOCO : (1000000/3440000)*5 = 1.453488us
       1.45 + 2 = 3.45us ("+2" is overhead cycle) */
    R_BSP_SoftwareDelay((uint32_t)4, BSP_DELAY_MICROSECS);
#endif

    /* Protect on. */
    SYSTEM.PRCR.WORD = 0xA500;
} /* End of function operating_frequency_set() */

/***********************************************************************************************************************
* Function name: clock_source_select
* Description  : Enables and disables clocks as chosen by the user. If a clock other than LOCO or HOCO is already
*                running when this function is called then that usually means a bootloader was run beforehand and set
*                up the clocks already.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void clock_source_select (void)
{
#if (BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 1) || (BSP_CFG_RTC_ENABLE == 1)
    uint8_t tmp;
#endif

    /* Set to High-speed operating mode if ICLK is > 24MHz. */
    if (BSP_ICLK_HZ > BSP_MIDDLE_SPEED_MAX_FREQUENCY)
    {
        /* WAIT_LOOP */
        while(1 == SYSTEM.OPCCR.BIT.OPCMTSF)
        {
            /* Wait for transition to finish. */
            R_BSP_NOP();
        }

        /* set to high-speed mode */
        SYSTEM.OPCCR.BYTE = 0x00;

        /* WAIT_LOOP */
        while(1 == SYSTEM.OPCCR.BIT.OPCMTSF)
        {
            /* Wait for transition to finish. */
            R_BSP_NOP();
        }
    }

    /* At this time the MCU is still running on the 4MHz LOCO. */

#if BSP_CFG_HOCO_OSCILLATE_ENABLE == 1
    /* HOCO is chosen. Start it operating if it is not already operating. */
    if (1 == SYSTEM.HOCOCR.BIT.HCSTP)
    {
        #if BSP_CFG_HOCO_TRIMMING_ENABLE == 1
        /* Set the frequency trimming value for the HOCO. */
        SYSTEM.HOCOTRR0.BYTE = (0x3F & BSP_CFG_HOCO_TRIMMING_REG_VALUE);
        #endif

        /* HOCO is chosen. Start it operating. */
        SYSTEM.HOCOCR.BYTE = 0x00;

        /* Dummy read and compare. cf."5. I/O Registers", "(2) Notes on writing to I/O registers" in User's manual.
           This is done to ensure that the register has been written before the next register access. The RX has a 
           pipeline architecture so the next instruction could be executed before the previous write had finished.
         */
        if(0x00 == SYSTEM.HOCOCR.BYTE)
        {
            R_BSP_NOP();
        }

        /* WAIT_LOOP */
        while(0 == SYSTEM.OSCOVFSR.BIT.HCOVF)
        {
            /* The delay period needed is to make sure that the HOCO has stabilized.
               If you use simulator, the flag is not set to 1, resulting in an infinite loop. */
            R_BSP_NOP();
        }
    }
#endif /* BSP_CFG_HOCO_OSCILLATE_ENABLE == 1 */

#if BSP_CFG_MAIN_CLOCK_OSCILLATE_ENABLE == 1
    /* Set the oscillation source of the main clock oscillator. */
    SYSTEM.MOFCR.BIT.MOSEL = BSP_CFG_MAIN_CLOCK_SOURCE;

    /* If the main oscillator is > 10MHz then the main clock oscillator
       forced oscillation control register (MOFCR) must be changed. */
    if (BSP_CFG_XTAL_HZ >= 10000000)
    {
        /* 10 - 20MHz. */
        SYSTEM.MOFCR.BIT.MODRV21 = 1;
    }
    else
    {
        /* 1 - 10MHz. */
        SYSTEM.MOFCR.BIT.MODRV21 = 0;
    }

    /* Set the oscillation stabilization wait time of the main clock oscillator. */
#if BSP_CFG_MAIN_CLOCK_SOURCE == 0 /* Resonator */
    SYSTEM.MOSCWTCR.BYTE = BSP_CFG_MOSC_WAIT_TIME;
#elif BSP_CFG_MAIN_CLOCK_SOURCE == 1 /* External oscillator input */
    SYSTEM.MOSCWTCR.BYTE = 0x00;
#else
    #error "Error! Invalid setting for BSP_CFG_MAIN_CLOCK_SOURCE in r_bsp_config.h"
#endif

    /* Set the main clock to operating. */
    SYSTEM.MOSCCR.BYTE = 0x00;

    /* WAIT_LOOP */
    while (0 == SYSTEM.OSCOVFSR.BIT.MOOVF)
    {
        /* Make sure clock has stabilized.
           If you use simulator, the flag is not set to 1, resulting in an infinite loop. */
        R_BSP_NOP();
    }
#else /* (BSP_CFG_MAIN_CLOCK_OSCILLATE_ENABLE == 0) */
    /* Main clock is stopped after reset. */
#endif /* BSP_CFG_MAIN_CLOCK_OSCILLATE_ENABLE == 1 */

    /* Sub-clock setting. */

    /* Cold start setting */
    if (0 == SYSTEM.RSTSR1.BIT.CWSF)
    {

        /* SOSCCR - Sub-Clock Oscillator Control Register
        b7:b1    Reserved - The write value should be 0.
        b0       SOSTP    - Sub-clock oscillator Stop - Sub-clock oscillator is stopped. */
        SYSTEM.SOSCCR.BYTE = 0x01;

        /* WAIT_LOOP */
        while (0x01 != SYSTEM.SOSCCR.BYTE)
        {
            /* wait for bit to change */
            R_BSP_NOP();
        }

#if (BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 1) || (BSP_CFG_RTC_ENABLE == 1)
        /* Wait for 5 sub-clock cycles (153us): measurement result is approx. 176us */
        R_BSP_SoftwareDelay(176, BSP_DELAY_MICROSECS);    /* 153us * 4.56 / 4.00 (LOCO max) */

        /* Set the drive capacity of the sub-clock oscillator */
        #if   (BSP_CFG_SOSC_DRV_CAP == 0) /* Standard CL */
            tmp = 0x00;
        #elif (BSP_CFG_SOSC_DRV_CAP == 2) /* High-drive output for the low CL */
            tmp = 0x01;
        #elif (BSP_CFG_SOSC_DRV_CAP == 3) /* Middle-drive output for the low CL */
            tmp = 0x02;
        #elif (BSP_CFG_SOSC_DRV_CAP == 4) /* Low-drive output for the low CL */
            tmp = 0x03;
        #else
            #error "Error! Invalid setting for BSP_CFG_SOSC_DRV_CAP in r_bsp_config.h"
        #endif

        /* Set the Sub-Clock Oscillator Drive Capacity Control. */
        SYSTEM.SOMCR.BIT.SODRV = tmp;

        /* WAIT_LOOP */
        while (tmp != SYSTEM.SOMCR.BIT.SODRV)
        {
            /* wait for bits to change */
            R_BSP_NOP();
        }
#endif

#if BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 1
        /* Operate the Sub-clock oscillator */
        SYSTEM.SOSCCR.BYTE = 0x00;

        /* WAIT_LOOP */
        while (0x00 != SYSTEM.SOSCCR.BYTE)
        {
            /* wait for bit to change */
            R_BSP_NOP();
        }

        /* Wait for the oscillation stabilization time of the sub-clock. */
        R_BSP_SoftwareDelay(BSP_CFG_SOSC_WAIT_TIME, BSP_DELAY_MILLISECS);
#endif

#if (BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 0) && (BSP_CFG_RTC_ENABLE == 1)
        /* Wait for the oscillation stabilization time of the sub-clock. */
        R_BSP_SoftwareDelay(BSP_CFG_SOSC_WAIT_TIME, BSP_DELAY_MILLISECS);
#endif

#if (BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 1) || (BSP_CFG_RTC_ENABLE == 1)
        /* Wait for six the sub-clock cycles */
        /* 6 count of sub-clock : (1000000/32768)*6=183.10546875us
           In the case of LOCO frequency is 264kHz : 183.10546875/(1000000/264000)=48.33984375cycle
           (48.33984375+2)*(1000000/240000)=209.7493489583333us ("+2" is overhead cycle) */
        R_BSP_SoftwareDelay((uint32_t)210, BSP_DELAY_MICROSECS);
#endif

#if (BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 1) && (BSP_CFG_RTC_ENABLE == 0)
        /* Stop prescaler and counter */
        /* RCR2 - RTC Control Register 2
        b7  CNTMD - Count Mode Select - The calendar count mode.
        b6  HR24  - Hours Mode - The RTC operates in 24-hour mode.
        b5  AADJP - Automatic Adjustment Period Select - The RADJ.ADJ[5:0] setting value is adjusted from 
                                                           the count value of the prescaler every 10 seconds.
        b4  AADJE - Automatic Adjustment Enable - Automatic adjustment is enabled.
        b3  RTCOE - RTCOUT Output Enable - RTCOUT output enabled.
        b2  ADJ30 - 30-Second Adjustment - 30-second adjustment is executed.
        b1  RESET - RTC Software Reset - The prescaler and the target registers for RTC software reset are initialized.
        b0  START - start - Prescaler is stopped. */
        RTC.RCR2.BYTE &= 0x7E;

        /* WAIT_LOOP */
        while (0 != RTC.RCR2.BIT.START)
        {
            /* Confirm that the written value can be read correctly. */
            R_BSP_NOP();
        }

        /* WAIT_LOOP */
        while (0 != RTC.RCR2.BIT.CNTMD)
        {
            /* Confirm that the written value can be read correctly. */
            R_BSP_NOP();
        }

        /* RTC Software Reset */
        RTC.RCR2.BIT.RESET = 1;

        /* WAIT_LOOP */
        while (0 != RTC.RCR2.BIT.RESET)
        {
            /* Confirm that the written value can be read correctly.
               If you use simulator, the flag is not set to 0, resulting in an infinite loop. */
            R_BSP_NOP();
        }

        /* An alarm interrupt request is disabled */
        /* RCR1 - RTC Control Register 1
        b7:b4  PES   - Periodic Interrupt Select - These bits specify the period for the periodic interrupt.
        b3     RTCOS - RTCOUT Output Select - RTCOUT outputs 1 Hz.
        b2     PIE   - Periodic Interrupt Enable - A periodic interrupt request is disabled.
        b1     CIE   - Carry Interrupt Enable - A carry interrupt request is disabled.
        b0     AIE   - Alarm Interrupt Enable - An alarm interrupt request is disabled. */
        RTC.RCR1.BYTE &= 0xF8;

        /* WAIT_LOOP */
        while (0 != (0x07 & RTC.RCR1.BYTE))
        {
            /* Confirm that the written value can be read correctly. */
            R_BSP_NOP();
        }
#endif /* (BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 1) && (BSP_CFG_RTC_ENABLE == 0) */
    }
    /* Warm start setting */
    else
    {
#if BSP_CFG_SUB_CLOCK_OSCILLATE_ENABLE == 0
        /* SOSCCR - Sub-Clock Oscillator Control Register
        b7:b1    Reserved - The write value should be 0.
        b0       SOSTP    - Sub-clock oscillator Stop - Sub-clock oscillator is stopped. */
        SYSTEM.SOSCCR.BYTE = 0x01;

        /* WAIT_LOOP */
        while (0x01 != SYSTEM.SOSCCR.BYTE)
        {
            /* wait for bit to change */
            R_BSP_NOP();
        }
#endif
    }

#if BSP_PRV_PLL_CLK_OPERATING == 1
    /* PLL is chosen. Start it operating if it is not already. Must start main clock as well since PLL uses it. */

    /* Set PLL Input Divisor. */
    SYSTEM.PLLCR.BIT.PLIDIV = BSP_CFG_PLL_DIV >> 1;

    /* Set PLL Multiplier. */
    SYSTEM.PLLCR.BIT.STC = (BSP_CFG_PLL_MUL * 2) - 1;

    /* Set the PLL to operating. */
    SYSTEM.PLLCR2.BYTE = 0x00;

    /* WAIT_LOOP */
    while (0 == SYSTEM.OSCOVFSR.BIT.PLOVF)
    {
        /* Make sure clock has stabilized.
           If you use simulator, the flag is not set to 1, resulting in an infinite loop. */
        R_BSP_NOP();
    }
#endif

    /* LOCO is saved for last since it is what is running by default out of reset. This means you do not want to turn
       it off until another clock has been enabled and is ready to use. */
#if BSP_CFG_LOCO_OSCILLATE_ENABLE == 1
    /* LOCO is chosen. This is the default out of reset. */
#else
    /* LOCO is not chosen but it cannot be turned off yet since it is still being used. */
#endif

    /* Set to memory wait if ICLK is > 32MHz. */
    if( BSP_ICLK_HZ > BSP_MEMORY_NO_WAIT_MAX_FREQUENCY )
    {
        /* Use wait states */
        FLASH.MEMWAITR.WORD = 0xAA01;

        /* WAIT_LOOP */
        while (0x0001 != FLASH.MEMWAITR.WORD)
        {
            /* wait for bit to set */
            R_BSP_NOP();
        }
    }
} /* End of function clock_source_select() */

/***********************************************************************************************************************
* Function name: lpt_clock_source_select
* Description  : Enables clock sources for the lpt (if not already done) as chosen by the user.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void lpt_clock_source_select (void)
{
    /* Protect off. */
    SYSTEM.PRCR.WORD = 0xA50F;

    /* INITIALIZE AND SELECT LPT CLOCK SOURCE */

#if (BSP_CFG_LPT_CLOCK_SOURCE == 0) || (BSP_CFG_LPT_CLOCK_SOURCE == 2)
    /* Sub-clock or None is chosen. */
    /* sub-clock oscillator already initialized in clock_source_select() */

#elif (BSP_CFG_LPT_CLOCK_SOURCE == 1)
    /* IWDTCLK is chosen. Start it operating. */
    SYSTEM.ILOCOCR.BYTE = 0x00;

    /* Wait processing for the IWDT clock oscillation stabilization (50us) */
    R_BSP_SoftwareDelay((uint32_t)50, BSP_DELAY_MICROSECS);

    /* Controls whether to stop the IWDT counter in a low power consumption state.
    IWDTCSTPR - IWDT Count Stop Control Register
    b7     SLCSTP   - Sleep Mode Count Stop Control - Count stop is disabled.
    b6:b1  Reserved - These bits are read as 0. Writing to these bits has no effect. */
    IWDT.IWDTCSTPR.BIT.SLCSTP = 0;
#elif (BSP_CFG_LPT_CLOCK_SOURCE == 3)
    /* LOCO is chosen. */
    /* LOCO already initialized in clock_source_select() */
#endif

    /* Enable protect bit */
    SYSTEM.PRCR.WORD = 0xA500;
} /* End of function lpt_clock_source_select() */

#if BSP_CFG_CLKOUT_OUTPUT != 0
/***********************************************************************************************************************
* Function name: bsp_clkout_initial_configure
* Description  : Configures the CLKOUT initial settings
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void bsp_clkout_initial_configure(void)
{
    /* Protect off. */
    SYSTEM.PRCR.WORD = 0xA50B;

    /* Set the CLKOUT Output Divisor Select. */
    SYSTEM.CKOCR.BIT.CKODIV = BSP_CFG_CLKOUT_DIV;

    /* Set the CLKOUT Output Source Select. */
    SYSTEM.CKOCR.BIT.CKOSEL = BSP_CFG_CLKOUT_SOURCE;
#if BSP_CFG_CLKOUT_OUTPUT == 1

    /* Set the CLKOUT Output Stop Control. */
    SYSTEM.CKOCR.BIT.CKOSTP = 0;
#elif BSP_CFG_CLKOUT_OUTPUT == 0
    /* do nothing */
#else
    #error "Error! Invalid setting for BSP_CFG_CLKOUT_OUTPUT in r_bsp_config.h"
#endif
    /* Protect on. */
    SYSTEM.PRCR.WORD = 0xA500;
} /* End of function bsp_clkout_initial_configure() */
#endif /* BSP_CFG_CLKOUT_OUTPUT != 0 */

#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

