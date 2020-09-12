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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/*********************************************************************************************************************
 * File Name    : r_lpc_rx71m.c
 * Description  : Implementation of the LPC module for the RX71M
 **********************************************************************************************************************/
/*********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.10.2016 1.00    First Release
 *         : 01.04.2019 1.41    Added "WAIT_LOOP" keyword.
 *         : 14.11.2019 2.00    Added support for GNUC and ICCRX.
 **********************************************************************************************************************/

/*********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX71M)

/* Public interface header file for this package. */
#include "r_lpc_rx_if.h"
#include "r_lpc_rx_private.h"
#include "r_lpc_rx71m_private.h"

/********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
/* See cgc_init_local_data() for the initialization of these arrays */

#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
static uint32_t clock_frequency[6];    // Clock source frequencies used by lpc_clockhzget()
static uint32_t hoco_frequency[4];     // Possible HOCO frequencies; Indices 3 is not used for the RX71M.

static uint32_t lpc_clockhzget (lpc_system_clocks_t clock);
static lpc_err_t lpc_freq_range_check (lpc_operating_mode_t e_mode);
static lpc_err_t lpc_lowpower_activate_check (void);

#endif /* LPC_CFG_PARAM_CHECKING_ENABLE */

/********************************************************************************************************************
 * Function Name: lpc_operating_mode_set
 * Description  : This function will configure the MCU for the specified operating mode. In any of the operating
 *                modes, the CPU is running, but the maximum clock frequencies are limited based on the mode.
 *                The supported modes are listed in the lpc_operating_mode_t enum. The modes with the
 *                lower operating frequencies use regulator switches to reduce current draw. It is imperative
 *                that when switching to a higher clock frequency, the mode is changed first to support that
 *                frequency.
 * Arguments    : e_mode - Operating Mode
 *                    LPC_OP_HIGH_SPEED  
 *                    LPC_OP_LOW_SPEED_1 
 *                    LPC_OP_LOW_SPEED_2 
 * Return Value : LPC_SUCCESS -
 *                LPC_ERR_CLOCK_EXCEEDED -
 *                    Clock exceeds the limit of current operating mode
 *                LPC_ERR_P_E_MODE -
 *                    The operation mode can't be changed in P/E mode.
 ********************************************************************************************************************/
lpc_err_t lpc_operating_mode_set (lpc_operating_mode_t e_mode)
{
#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
    lpc_err_t err;

    /* Check FENTRYC(b0) and FENTRYD(b7) */
    if (0x0000 != (FLASH.FENTRYR.WORD & 0x0081))
    {
        return LPC_ERR_P_E_MODE;
    }
    
    err = lpc_freq_range_check(e_mode);
    if (LPC_SUCCESS != err)
    {
        return err;
    }
#endif    

    /* WAIT_LOOP */
    while (LPC_TRANSITION_ONGOING == SYSTEM.OPCCR.BIT.OPCMTSF)
    {
        /* do not modify OPCCR if a transition is ongoing */
    }
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    SYSTEM.OPCCR.BIT.OPCM = e_mode;     // switch Mode
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
    /* WAIT_LOOP */
    while (LPC_TRANSITION_ONGOING == SYSTEM.OPCCR.BIT.OPCMTSF)
    {
        /* wait till transition is complete */
    }
    return LPC_SUCCESS;

}

/*********************************************************************************************************************
 End of function lpc_operating_mode_set
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: lpc_low_power_mode_configure
 * Description  : This function will configure the MCU for the specified Low Power Mode. In the Low Power Mode,
 *                the CPU is turned OFF and peripherals may be on or off depending on the mode. The supported
 *                low power modes are listed in the enum lpc_low_power_mode_t
 * Arguments    : e_mode - Low Power Mode
 *                    LPC_LP_SLEEP
 *                    LPC_LP_ALL_MODULE_STOP
 *                    LPC_LP_SW_STANDBY
 *                    LPC_LP_DEEP_SW_STANDBY
 *
 * Return Value : LPC_SUCCESS -
 ********************************************************************************************************************/
lpc_err_t lpc_low_power_mode_configure (lpc_low_power_mode_t e_mode)
{
    switch (e_mode)
    {
        case LPC_LP_SLEEP:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);// unlock clock control registers
            SYSTEM.SBYCR.BIT.SSBY   = 0;        // Move to Sleep on R_BSP_WAIT()
            SYSTEM.MSTPCRA.BIT.ACSE = 0;
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); // lock clock control registers
        break;
        
        case LPC_LP_ALL_MODULE_STOP:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);// unlock clock control registers
            SYSTEM.SBYCR.BIT.SSBY      = 0;     // Move to All Module Stop on R_BSP_WAIT(). ACSE also needs to be set
            SYSTEM.MSTPCRA.BIT.ACSE    = 1;
            SYSTEM.MSTPCRA.BIT.MSTPA24 = 1;
            SYSTEM.MSTPCRA.BIT.MSTPA27 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD0 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD1 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD2 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD3 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD4 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD5 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD6 = 1;
            SYSTEM.MSTPCRD.BIT.MSTPD7 = 1;
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); // lock clock control registers
    
       break;

        case LPC_LP_SW_STANDBY:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
            SYSTEM.SBYCR.BIT.SSBY =  0x1;       // Move to Sw Stby on R_BSP_WAIT(); DPSBY also has to be modified
            SYSTEM.DPSBYCR.BIT.DPSBY=0;
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
        break;
        
        case LPC_LP_DEEP_SW_STANDBY:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);// unlock clock control registers
            SYSTEM.SBYCR.BIT.SSBY    = 0x1;       // Move to Sw Stby on R_BSP_WAIT();DPSBY also has to be modified
            SYSTEM.DPSBYCR.BIT.DPSBY = 1;
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); // lock clock control registers
        break;
    
        default:

            /* nothing here */
        break;
    }

    return LPC_SUCCESS;
}
/*********************************************************************************************************************
 End of function lpc_low_power_mode_configure
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: lpc_lowpower_activate
 * Description  : This function will activate the pre-configured low power mode. The R_BSP_WAIT() function re-enables
 *                interrupts.
 * Arguments    : void (*pcallback)(void* pdata)-
 *                Call back function to configure any un-configured interrupt that can be used to wake up from the low
 *                power mode. This can be a FIT_NO_FUNC if the interrupt(s) are already configured.
 *
 * Return Value : LPC_SUCCESS - Function will return only on waking up from low power mode.
 *                LPC_ERR_OSC_STOP_ENABLED -
 *                    The Software Standby Mode cannot be activated if oscillation stop detection is enabled.
 *                LPC_ERR_ILL_CLOCK_SOURCE -
 *                    The Sleep Mode Return Clock Switch is only supported if the clock source at the time of
 *                    entry to sleep mode is allowed clock.
 *                LPC_ERR_ILLEGAL
 *                    Other than those above.
 ********************************************************************************************************************/
lpc_err_t lpc_lowpower_activate (lpc_callback_set_t pcallback)
{
#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
    lpc_err_t err;

    err = lpc_lowpower_activate_check();
    
    if (LPC_SUCCESS != err)
    {
        return err;
    }
#endif

    R_BSP_InterruptsDisable(); /* Clear the I bit to disable interrupts */
    if ((NULL != pcallback) && (FIT_NO_FUNC != pcallback)) /* If call-back is valid */
    {
        pcallback(FIT_NO_PTR); /* Call call-back to configure wake-up interrupt source */
    }
    else
    {
        R_BSP_NOP();
        R_BSP_NOP();
        R_BSP_NOP();
        R_BSP_NOP();
        R_BSP_NOP(); /* Read last written IO register to make sure it is complete */
        R_BSP_NOP(); /* or wait for 4-5 cycles till any prior write is complete */
    }

    R_BSP_WAIT();                         /* Enter Low Power mode */
    return LPC_SUCCESS;
}
/*********************************************************************************************************************
 End of function lpc_lowpower_activate
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: lpc_return_clock_switch
 * Description  : This function will configure the return clock switching parameter that allows the clock source to
 *                be switched on returning from Sleep Mode to either the HOCO or Main Clock. 
 * Arguments    : lpc_clock_switch_t e_clock_source -
 *                  Specify the clock source to switch to.
 *                bool enable
 *                  Enable/disable the Return clock Source switching function.
 * Return Value : LPC_SUCCESS
 *********************************************************************************************************************/
lpc_err_t lpc_return_clock_switch (lpc_clock_switch_t e_clock_source, bool enable)
{
    if (true == enable)
    {
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
        SYSTEM.RSTCKCR.BYTE = (uint8_t) ((LPC_CLOCK_SWITCH_ENABLE << 7) | e_clock_source);
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
    }
    else
    {
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
        SYSTEM.RSTCKCR.BYTE = LPC_CLOCK_SWITCH_DISABLE;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
    }
    
    return LPC_SUCCESS;
}
/*********************************************************************************************************************
 End of function lpc_return_clock_switch
 ********************************************************************************************************************/

#if (LPC_CFG_PARAM_CHECKING_ENABLE == 1)
/*********************************************************************************************************************
 * Function Name: lpc_freq_range_check
 * Description  : This function will check clock frequency.
 * Arguments    : e_mode - Operating Mode
 *                    LPC_OP_HIGH_SPEED  
 *                    LPC_OP_LOW_SPEED_1 
 *                    LPC_OP_LOW_SPEED_2 
 * Return Value : LPC_SUCCESS -
 *                LPC_ERR_CLOCK_EXCEEDED -
 *                    If the current system clocks dividers are higher than those supported by the e_mode operating mode.
 ********************************************************************************************************************/
static lpc_err_t lpc_freq_range_check (lpc_operating_mode_t e_mode)
{
    uint32_t iclk_freq;
    uint32_t bclk_freq;
    uint32_t fclk_freq;
    uint32_t pclka_freq;
    uint32_t pclkb_freq;
    uint32_t pclkc_freq;
    uint32_t pclkd_freq;

    /* Clock source frequencies. The HOCO and PLL values need to be calculated by
     * R_CGC_ClockHzGet() before they are valid. These map to enum cgc_clocks_t */
    clock_frequency[0] = BSP_LOCO_HZ;      // LOCO
    clock_frequency[1] = BSP_HOCO_HZ;      // HOCO
    clock_frequency[2] = BSP_CFG_XTAL_HZ;  // MAIN
    clock_frequency[3] = BSP_SUB_CLOCK_HZ; // SUBCLK
    clock_frequency[4] = 0;                // PLL
    clock_frequency[5] = 120000;           // IWDT - 120 kHz
    hoco_frequency[0] = 16000000; // 16 MHz
    hoco_frequency[1] = 18000000; // 18 MHz
    hoco_frequency[2] = 20000000; // 20 MHz
    hoco_frequency[3] = 0;        // not use

    iclk_freq = lpc_clockhzget(LPC_ICLK);
    bclk_freq = lpc_clockhzget(LPC_BCLK);
    fclk_freq = lpc_clockhzget(LPC_FCLK);
    pclka_freq = lpc_clockhzget(LPC_PCLKA);
    pclkb_freq = lpc_clockhzget(LPC_PCLKB);
    pclkc_freq = lpc_clockhzget(LPC_PCLKC);
    pclkd_freq = lpc_clockhzget(LPC_PCLKD);

    /* Check if clock speeds are within limits */
    switch (e_mode)
    {
        case LPC_OP_HIGH_SPEED:
            if (((((((iclk_freq > LPC_MAX_ICK_HIGH_SPEED_FREQ_HZ)
                  || (bclk_freq > LPC_MAX_BCK_HIGH_SPEED_FREQ_HZ))
                  || (fclk_freq > LPC_MAX_FCK_HIGH_SPEED_FREQ_HZ))
                  || (pclka_freq > LPC_MAX_PCKA_HIGH_SPEED_FREQ_HZ))
                  || (pclkb_freq > LPC_MAX_PCKB_HIGH_SPEED_FREQ_HZ))
                  || (pclkc_freq > LPC_MAX_PCKC_HIGH_SPEED_FREQ_HZ))
                  || (pclkd_freq > LPC_MAX_PCKD_HIGH_SPEED_FREQ_HZ))
            {
                return LPC_ERR_CLOCK_EXCEEDED;
            }
        break;
        case LPC_OP_LOW_SPEED_1:
            if (((((((iclk_freq > LPC_MAX_ICK_LOW1_SPEED_FREQ_HZ)
                  || (bclk_freq > LPC_MAX_BCK_LOW1_SPEED_FREQ_HZ))
                  || (fclk_freq > LPC_MAX_FCK_LOW1_SPEED_FREQ_HZ))
                  || (pclka_freq > LPC_MAX_PCKA_LOW1_SPEED_FREQ_HZ))
                  || (pclkb_freq > LPC_MAX_PCKB_LOW1_SPEED_FREQ_HZ))
                  || (pclkc_freq > LPC_MAX_PCKC_LOW1_SPEED_FREQ_HZ))
                  || (pclkd_freq > LPC_MAX_PCKD_LOW1_SPEED_FREQ_HZ))
            {
                return LPC_ERR_CLOCK_EXCEEDED;
            }
            
            if (LPC_CLOCK_ACTIVE == SYSTEM.PLLCR2.BYTE) /* Check if PLL is running */
            {
                return LPC_ERR_CLOCK_EXCEEDED;
            }
        break;

        case LPC_OP_LOW_SPEED_2:
            if ((((((((iclk_freq > LPC_MAX_ICK_LOW2_SPEED_FREQ_HZ) || (iclk_freq < LPC_MIN_ICK_LOW2_SPEED_FREQ_HZ))
                  || ((bclk_freq > LPC_MAX_BCK_LOW2_SPEED_FREQ_HZ) || (bclk_freq < LPC_MIN_BCK_LOW2_SPEED_FREQ_HZ)))
                  || ((fclk_freq > LPC_MAX_FCK_LOW2_SPEED_FREQ_HZ) || (fclk_freq < LPC_MIN_FCK_LOW2_SPEED_FREQ_HZ)))
                  || ((pclka_freq > LPC_MAX_PCKA_LOW2_SPEED_FREQ_HZ) || (pclka_freq < LPC_MIN_PCKA_LOW2_SPEED_FREQ_HZ)))
                  || ((pclkb_freq > LPC_MAX_PCKB_LOW2_SPEED_FREQ_HZ) || (pclkb_freq < LPC_MIN_PCKB_LOW2_SPEED_FREQ_HZ)))
                  || ((pclkc_freq > LPC_MAX_PCKC_LOW2_SPEED_FREQ_HZ) || (pclkc_freq < LPC_MIN_PCKC_LOW2_SPEED_FREQ_HZ)))
                  || ((pclkd_freq > LPC_MAX_PCKD_LOW2_SPEED_FREQ_HZ) || (pclkd_freq < LPC_MIN_PCKD_LOW2_SPEED_FREQ_HZ)))
            {
                return LPC_ERR_CLOCK_EXCEEDED;
            }

            if ((((LPC_CLOCK_ACTIVE == SYSTEM.HOCOCR.BYTE)      /* Check if HOCO is running */
               || (1 == SYSTEM.OSTDCR.BIT.OSTDE))               /* Check if Oscillation Main Stop Detection Function is enabled */
               || (LPC_CLOCK_ACTIVE == SYSTEM.PLLCR2.BYTE))     /* Check if PLL is running */
               || ((LPC_SUB_CLOCK == SYSTEM.SCKCR3.WORD)        /* Check if ICLK and FCLK are no divided when System Clock is selected Sub clock  */
                   && ((0x00 != SYSTEM.SCKCR.BIT.ICK) || (0x00 != SYSTEM.SCKCR.BIT.FCK)))) 
            {
                return LPC_ERR_CLOCK_EXCEEDED;
            }
        break;
        
        default:

            /* nothing here */
        break;
    }

    return LPC_SUCCESS;
}
/*********************************************************************************************************************
 End of function lpc_freq_range_check
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: lpc_clockhzget
 * Description  : This function will return the requested internal clock frequency in Hz
 * Arguments    : lpc_system_clocks_t clock -
 *                    Specifies the internal clock whose frequency will be returned in Hz .
 *
 * Return Value : uint32_t frequency - clock  frequency in hz
 ********************************************************************************************************************/
static uint32_t lpc_clockhzget (lpc_system_clocks_t clock)
{
    uint32_t divisor;
    uint32_t pll_clk_src_freq;
    float pll_multiplier;

    /* Determine the current HOCO frequency */
    clock_frequency[LPC_CKSEL_HOCO] = 0;
    if (LPC_CLOCK_ACTIVE == SYSTEM.HOCOCR.BIT.HCSTP)
    {
        /* Get the currently programmed HOCO frequency and update the clock_frequency[]
         * array in case it has changed. */
        clock_frequency[LPC_CKSEL_HOCO] = hoco_frequency[SYSTEM.HOCOCR2.BIT.HCFRQ];
    }

    /* Determine the current PLL frequency */
    clock_frequency[LPC_CKSEL_PLL] = 0;
    if (LPC_CLOCK_ACTIVE == SYSTEM.PLLCR2.BIT.PLLEN)
    {
        /*
         * Calculate the PLL clock frequency as follows:
         * - Determine the main oscillator's frequency
         * - Determine the programmed PLL input divider value from PLLCR.PLIDIV
         * - Determine the programmed PLL output multiplier from PLLCR.STC
         * - Calculate the PLL clock frequency as ((SRC_CLK_FREQ / PLL_DIV) * PLL_MUL)
         */
        if (0 == SYSTEM.PLLCR.BIT.PLLSRCSEL)        /* Check PLL Closk source */
        {
            pll_clk_src_freq = clock_frequency[LPC_CKSEL_MAIN_OSC];
        }
        else
        {
            pll_clk_src_freq = clock_frequency[LPC_CKSEL_HOCO];
        }
        pll_multiplier = (((float)SYSTEM.PLLCR.BIT.STC + 1.0f) / 2.0f);

        /* Finally, calculate the PLL clock frequency */
        clock_frequency[LPC_CKSEL_PLL] = (uint32_t) (((float) pll_clk_src_freq / (float)(SYSTEM.PLLCR.BIT.PLIDIV + 1)) 
                * pll_multiplier);
    }

    /* Retrieve the programmed divisor for the clock being queried */
    switch (clock)
    {
        case LPC_ICLK :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.ICK);
        break;
        case LPC_BCLK :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.BCK);
        break;
        case LPC_FCLK :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.FCK);
        break;
        case LPC_PCLKA :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.PCKA);
        break;
        case LPC_PCLKB :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.PCKB);
        break;
        case LPC_PCLKC :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.PCKC);
        break;
        case LPC_PCLKD :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.PCKD);
        break;
        default :
            divisor = (uint32_t) (1 << SYSTEM.SCKCR.BIT.ICK);    /* Forced select ICLK when invalid arg */
        break;
    }

    return (uint32_t) ((float) clock_frequency[SYSTEM.SCKCR3.BIT.CKSEL] / (float) divisor);
}
/*********************************************************************************************************************
 End of function lpc_clockhzget
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: lpc_lowpower_activate_check
 * Description  : This function will check if low power activate can be executed.
 * Arguments    : void 
 *
 * Return Value : LPC_SUCCESS - 
 *                LPC_ERR_OSC_STOP_ENABLED -
 *                    The Software Standby Mode cannot be activated if oscillation stop detection is enabled.
 *                LPC_ERR_ILL_CLOCK_SOURCE -
 *                    The Sleep Mode Return Clock Switch is only supported if the clock source at the time of
 *                    entry to sleep mode is the SubClock or LOCO.
 *                    If the return mode is Medium Speed Mode, then HOCO cannot be used as the return clock.
 *                LPC_ERR_ILLEGAL
 *                    Other than those above.
 ********************************************************************************************************************/
static lpc_err_t lpc_lowpower_activate_check (void)
{
    uint8_t rstckcr_rstcksel;

    if (0x1 == SYSTEM.SBYCR.BIT.SSBY) /* check if entering software standby or deep software standby */
    {
        if (LPC_OSC_STOP_ENABLED == SYSTEM.OSTDCR.BIT.OSTDE)
        {
            return LPC_ERR_OSC_STOP_ENABLED;    /* Cannot enter SW_Standby if Oscillation Stop is enabled */
        }
        
        if ((0 == SYSTEM.MSTPCRA.BIT.MSTPA28) 
            && ((1 == DTC.DTCST.BIT.DTCST) || (1 == DMAC.DMAST.BIT.DMST)))
        {
            return LPC_ERR_ILLEGAL;         /* Cannot enter SW_Stanbay if DTC or DMAC is enabled */
        }

        /* Check FENTRYC(b0) and FENTRYD(b7) */
        if (0x0000 != (FLASH.FENTRYR.WORD & 0x0081))
        {
            return LPC_ERR_ILLEGAL;         /* Cannot enter SW_Stanbay if P/E mode entry */
        }

        if (0x01 == SYSTEM.DPSBYCR.BIT.DPSBY)   /* check if entering deep software standby */
        {
            if (0x00000000 == (LPC_OFS0_REG_VALUE & 0x00000002))    /* IWDT auto start check */
            {
                if (0x00000000 == (LPC_OFS0_REG_VALUE & 0x00004000))
                {
                    return LPC_ERR_ILLEGAL;     /* Cannot enter DSW_Standby if IWDT auto start and IWDT count stop disable */
                }
            }
            else
            {
                if (0 == IWDT.IWDTCSTPR.BIT.SLCSTP)
                {
                    return LPC_ERR_ILLEGAL;     /* Cannot enter DSW_Standby if IWDT count stop disable */
                }
            }
            
            if ((1 == SYSTEM.LVD1CR0.BIT.LVD1RI) || (1 == SYSTEM.LVD2CR0.BIT.LVD2RI))
            {
                return LPC_ERR_ILLEGAL;         /* Cannot enter DSW_Standby if LVD reset enable */
            }
        }
        
    }

    else if ((0 == SYSTEM.MSTPCRA.BIT.ACSE) && /* Check if entering Sleep Mode */
            (LPC_CLOCK_SWITCH_ENABLE == SYSTEM.RSTCKCR.BIT.RSTCKEN)) /* If Return Clock Switching is enabled */
    {
        /* When entering sleep mode with return clock switching enabled, the system clock must be the sub-clock */
        if ((LPC_SUB_CLOCK != SYSTEM.SCKCR3.WORD) && (LPC_LOCO != SYSTEM.SCKCR3.WORD))
        {
            return LPC_ERR_ILL_CLOCK_SOURCE; /* Cannot enter Sleep if System clock is not Subclock or LOCO */
        }
        
        rstckcr_rstcksel = (SYSTEM.RSTCKCR.BYTE & 0x07);

        /* Exiting from sleep-mode to medium-speed mode with the HOCO selected is not permitted */
        if (LPC_HOCO == rstckcr_rstcksel) /* If HOCO is chosen as the return clock */
        {
            if (1 == SYSTEM.HOCOPCR.BIT.HOCOPCNT)
            {
                return LPC_ERR_ILL_CLOCK_SOURCE;    /* Cannot enter Sleep if HOCO power off */
            }
        }
        

    }
    else if(1 == SYSTEM.MSTPCRA.BIT.ACSE) /* Check if entering All Clock Stop Mode */
    {
        if ((((LPC_ACS_CHK_MSTPCRA != (SYSTEM.MSTPCRA.LONG & LPC_ACS_CHK_MSTPCRA))
            ||(LPC_ACS_CHK_MSTPCRB != (SYSTEM.MSTPCRB.LONG & LPC_ACS_CHK_MSTPCRB)))
            ||(LPC_ACS_CHK_MSTPCRC != (SYSTEM.MSTPCRC.LONG & LPC_ACS_CHK_MSTPCRC)))
            ||(LPC_ACS_CHK_MSTPCRD != (SYSTEM.MSTPCRD.LONG & LPC_ACS_CHK_MSTPCRD)))
        {
            return LPC_ERR_ILLEGAL;             /* Cannot enter All Clock stop if Module stop release */
        }

        /* Check FENTRYC(b0) and FENTRYD(b7) */
        if (0x0000 != (FLASH.FENTRYR.WORD & 0x0081))
        {
            return LPC_ERR_ILLEGAL;         /* Cannot enter SW_Stanbay if P/E mode entry */
        }
    }
    else
    {
        /* nothing here */
    }

    return LPC_SUCCESS;
}
/*********************************************************************************************************************
 End of function lpc_lowpower_activate_check
 ********************************************************************************************************************/

#endif /* LPC_CFG_PARAM_CHECKING_ENABLE */


#endif /* BSP_MCU_RX71M */


/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
