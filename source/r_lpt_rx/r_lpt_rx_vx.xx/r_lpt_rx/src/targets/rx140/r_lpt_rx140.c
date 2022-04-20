/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lpt_rx140.c
 * Version      : 3.01
 * Description  : Functions for using even link controller
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 25.09.2020 1.00    First Release.
 *         : 31.03.2021 3.00    Added function R_LPT_InitChan, R_LPT_SetCMT, R_LPT_FinalChan, R_LPT_InitPWM
 *                              Added command LPT_CMD_PWM_START and LPT_CMD_PWM_STOP to R_LPT_Control()
 *         : 31.01.2022 3.01    Fixed function lpt_open
 *                              Fixed warning
 *********************************************************************************************************************/
/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

#if defined (BSP_MCU_RX140) /* This pre-processer need for compile when module added by FIT-Plugin */

/* Include specifics for chosen MCU.  */
#include "r_lpt_rx_private.h"

/*****************************************************************************
  Private global variables and functions
******************************************************************************/
/*------- global variables -------*/
const uint8_t g_lpt_division_ratio[] = {  /* for register set */
    LPT_DIVISION_RATIO_1,
    LPT_DIVISION_RATIO_2,
    LPT_DIVISION_RATIO_4,
    LPT_DIVISION_RATIO_8,
    LPT_DIVISION_RATIO_16,
    LPT_DIVISION_RATIO_32
};

const uint8_t g_lpt_divratio_value[] = {  /* for calculation */
    1,  /* LPT_DIVISION_RATIO_1  */
    2,  /* LPT_DIVISION_RATIO_2  */
    4,  /* LPT_DIVISION_RATIO_4  */
    8,  /* LPT_DIVISION_RATIO_8  */
    16, /* LPT_DIVISION_RATIO_16 */
    32  /* LPT_DIVISION_RATIO_32 */
};

static uint32_t lpt_regval_calc (uint32_t const period, uint32_t const srcclk, uint8_t const divratio);

/*****************************************************************************
* Function Name: r_lpt_mcu_hardware_lock
* Description  : Attempt to acquire the lock that has been sent in.
* Arguments    : none
* Return Value : true                   ; Successful operation
*              : false                  ; error operation
******************************************************************************/
bool r_lpt_mcu_hardware_lock(void)
{
    bool        chk = false;

    chk = R_BSP_HardwareLock(BSP_LOCK_LPT);
    return chk;
}
/*****************************************************************************
 * End of function r_lpt_mcu_hardware_lock()
 *****************************************************************************/

/*****************************************************************************
* Function Name: r_lpt_mcu_hardware_unlock
* Description  : Release hold on lock.
* Arguments    : none
* Return Value : none
******************************************************************************/
void r_lpt_mcu_hardware_unlock(void)
{
    R_BSP_HardwareUnlock(BSP_LOCK_LPT);
}
/*****************************************************************************
 * End of function r_lpt_mcu_hardware_unlock()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_regval_calc
* Description  : calculate LPTPRD register value from period(us)
* Arguments    : period -
*                    LPT period(unit:us)
*                srcclk -
*                    count clock(Hz) (IWDT or Sub-Clock)
*                divratio -
*                    division ratio
* Return Value : calculation result
******************************************************************************/
/* #pragma inline(lpt_regval_calc) */
static uint32_t lpt_regval_calc(uint32_t const period, uint32_t const srcclk, uint8_t const divratio)
{
    uint64_t calc_work;
    uint32_t retval;

    /* It's multiplication of 32 bits and 32 bits, so 64 bits aren't exceeded. */
    calc_work = (uint64_t)srcclk * (uint64_t)period;
    /* It'll be an appropriate area because 32 bits are being expanded in 64 bits. */
    calc_work *= (uint64_t)LPT_CALCSCALE_OFFSET;
    /* It'll be an appropriate area because 8 bits are being expanded in 64 bits. */
    calc_work /= (uint64_t)divratio;
    /* add denominator/2 for rounding */
    calc_work += (uint64_t)((LPT_CONV_US * LPT_CALCSCALE_OFFSET) / 2);

    /* It'll be an appropriate area because 32 bits are being expanded in 64 bits. */
    retval = (uint32_t)(calc_work / (uint64_t)(LPT_CONV_US * LPT_CALCSCALE_OFFSET)) - 1;

    return retval;
}
/*****************************************************************************
 * End of function lpt_regval_calc()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_open
* Description  : Initialization for using the LPT.
* Arguments    : period -
*                    LPT period (microsecond)
* Return Value : LPT_SUCCESS -
*                    channel opened successfully
*                LPT_ERR_INVALID_ARG -
*                    The argument has an invalid value.
*
* NOTE: The associated clock must be configured/initialized prior to
*       calling this function.
*
******************************************************************************/
lpt_err_t lpt_open(uint32_t const lpt_period)
{
    lpt_err_t   err = LPT_SUCCESS;
    uint32_t    calc_period;
    uint16_t    period_value = LPT_MAX_PERIOD;
    uint8_t     ratio_value = LPT_DIVISION_RATIO_1;
    uint8_t     ratio_select;
#if (LPT_CFG_PARAM_CHECKING_ENABLE == 1)
    bool        prdset_flag = false;
#endif  /* LPT_CFG_PARAM_CHECKING_ENABLE */
    size_t      array_num = (sizeof (g_lpt_division_ratio)) / (sizeof (g_lpt_division_ratio[0]));

    /* WAIT_LOOP */
    for (ratio_select = 0; array_num > ratio_select; ratio_select++)
    {
        /* The if statement just before this processing confirms that the calculation result is 0xFFFF or less.
          Thus the value is typecasted to uint16_t correctly and stored within the proper range. */
        calc_period = lpt_regval_calc(lpt_period, LPT_LPTSRCCLK_HZ, g_lpt_divratio_value[ratio_select]);
        if ((LPT_MIN_PERIOD <= calc_period)
        && (LPT_MAX_PERIOD >= calc_period))
        {
            ratio_value  = g_lpt_division_ratio[ratio_select];  /* Source clock divider */
            period_value = (uint16_t)calc_period;               /* low-power timer cycle. */
#if (LPT_CFG_PARAM_CHECKING_ENABLE == 1)
            prdset_flag  = true;                                /* LPTPRD value decision */
#endif  /* LPT_CFG_PARAM_CHECKING_ENABLE */
            ratio_select = array_num;              /* LOOP stop */
        }
    }

#if (LPT_CFG_PARAM_CHECKING_ENABLE == 1)
    if (false == prdset_flag)                                        /* Error check */
    {
        /* When the value is out of range, the error is treated as an argument error. */
        return LPT_ERR_INVALID_ARG;
    }
#endif  /* LPT_CFG_PARAM_CHECKING_ENABLE */

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    if(0 == SYSTEM.SNZCR.WORD)
    {
        /* Low-power timer: enable to wakeup from standby mode. */
        LPT.LPWUCR.BIT.LPWKUPEN = 1;
    }
    else
    {
        LPT.LPWUCR.BIT.LPWKUPEN = 0;
    }

    /* Set the low-power timer clock source and its clock division ratio. */
    LPT.LPTCR1.BIT.LPCNTPSSEL = (uint8_t)(ratio_value & 0x07);

    /* Cast type of "LPT_CLOCK_SOURCE" to match type of LPCNTCKSEL register. */
    LPT.LPTCR1.BIT.LPCNTCKSEL = (uint8_t)(LPT_CLOCK_SOURCE &0x01);

    /* Cast type of "LPT_CLOCK_SOURCE" to match type of LPCNTCKSEL2 register. */
    LPT.LPTCR1.BIT.LPCNTCKSEL2 = (uint8_t)((LPT_CLOCK_SOURCE >> 1) &0x01);

    /* Set the low-power timer cycle. */
    LPT.LPTPRD.WORD         = period_value;

    /* Set the compare match 1 value for when PWME bit is enabled. */
    LPT.LPCMR1.WORD         = period_value;

    /* Supply the low-power timer clock. */
    LPT.LPTCR2.BIT.LPCNTSTP = 0;

    /* Clear the low-power timer. */
    LPT.LPTCR3.BIT.LPCNTRST = 1;

    /* WAIT_LOOP */
    while (0 != LPT.LPTCR3.BIT.LPCNTRST)
    {
        /* Wait for completion of the low-power timer reset. */
    }

    /* Enable Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);
    return (err);
}
/*****************************************************************************
 * End of function lpt_open()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_close
* Description  : Releases the LPT function.
* Arguments    : None
* Return Value : None
******************************************************************************/
void lpt_close(void)
{
    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Stop the low-power timer. */
    LPT.LPTCR3.BIT.LPCNTEN  = 0;

    if (0 == LPT.LPTCR2.BIT.LPCNTSTP)
    {
        /* Reset the low-power timer. */
        LPT.LPTCR3.BIT.LPCNTRST = 1;

        /* WAIT_LOOP */
        while (0 != LPT.LPTCR3.BIT.LPCNTRST)
        {
            /* Wait for completion of the low-power timer reset. */
        }

        /* Wait for divider circuit and counter reset. */
        R_BSP_SoftwareDelay(LPT_LPTSRCCLK_CYC, BSP_DELAY_MICROSECS);
    }

    /* Stop the low-power timer clock. */
    LPT.LPTCR2.BYTE         = LPT_LPTCR2_INIT;

    /* Initialize low-power timer compare match 0. */
    LPT.LPCMR0.WORD         = LPT_LPCMR0_INIT;
    
    /* Initialize low-power timer compare match 1. */
    LPT.LPCMR1.WORD         = LPT_LPCMR1_INIT;

    /* Reset the low-power timer cycle. */
    LPT.LPTPRD.WORD         = LPT_LPTPRD_INIT;

    /* Initialize low-power timer control register 1. */
    LPT.LPTCR1.BYTE         = LPT_LPTCR1_INIT;

    /* Low-power timer: disable to wakeup from standby mode. */
    LPT.LPWUCR.WORD         = LPT_LPWUCR_INIT;

    /* Enable Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);
}
/*****************************************************************************
 * End of function lpt_close()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_start
* Description  : Starts the LPT.
* Arguments    : None
* Return Value : None
******************************************************************************/
void lpt_start(void)
{
    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Operate the low-power timer. */
    LPT.LPTCR3.BIT.LPCNTEN = 1;

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);
}
/*****************************************************************************
 * End of function lpt_start()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_stop
* Description  : Stops the LPT.
* Arguments    : None
* Return Value : None
******************************************************************************/
void lpt_stop(void)
{
    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Stop the low-power timer. */
    LPT.LPTCR3.BIT.LPCNTEN = 0;

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);
}
/*****************************************************************************
 * End of function lpt_stop()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_count_reset
* Description  : Reset the LPT.
* Arguments    : None
* Return Value : LPT_SUCCESS -
*                    LPT count reset successfully.
*                LPT_ERR_CONDITION_NOT_ERR -
*                    LPT count not stop.
******************************************************************************/
lpt_err_t lpt_count_reset(void)
{
    lpt_err_t   err = LPT_SUCCESS;

    if (1 == LPT.LPTCR2.BIT.LPCNTSTP)
    {
        /* When CLOCK is not supplied to the LPT , the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    if (0 != LPT.LPTCR3.BIT.LPCNTEN)
    {
        /* When LPT start, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Reset the low-power timer. */
    LPT.LPTCR3.BIT.LPCNTRST = 1;

    /* WAIT_LOOP */
    while (0 != LPT.LPTCR3.BIT.LPCNTRST)
    {
        /* Wait for completion of the low-power timer reset. */
    }

    /* Wait for divider circuit and counter reset. */
    R_BSP_SoftwareDelay(LPT_LPTSRCCLK_CYC, BSP_DELAY_MICROSECS);

    /* Enable Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);
    return (err);
}
/*****************************************************************************
 * End of function lpt_count_reset()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_pwm_start
* Description  : Starts the PWM output.
* Arguments    : None
* Return Value : LPT_SUCCESS -
*                    LPT PWM start successfully.
*                LPT_ERR_CONDITION_NOT_MET -
*                    LPT count not stop or the values of LPTPRD and LPCMR1
*                    are not equal.
******************************************************************************/
lpt_err_t lpt_pwm_start(void)
{
    lpt_err_t   err = LPT_SUCCESS;

    if (LPT.LPTPRD.WORD != LPT.LPCMR1.WORD)
    {
        /* When the values of LPTPRD and LPCMR1 are not equal, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    if (0 != LPT.LPTCR3.BIT.LPCNTEN)
    {
        /* When LPT start, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Starts the PWM output. */
    LPT.LPTCR2.BIT.PWME = 1;

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);

    return (err);
}
/*****************************************************************************
 * End of function lpt_pwm_start()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_pwm_stop
* Description  : Stops the PWM output.
* Arguments    : None
* Return Value : LPT_SUCCESS -
*                    LPT PWM stop successfully.
*                LPT_ERR_CONDITION_NOT_MET -
*                    LPT count not stop.
******************************************************************************/
lpt_err_t lpt_pwm_stop(void)
{
    lpt_err_t   err = LPT_SUCCESS;

    if (0 != LPT.LPTCR3.BIT.LPCNTEN)
    {
        /* When LPT start, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Stops the PWM output. */
    LPT.LPTCR2.BIT.PWME = 0;

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);

    return (err);
}
/*****************************************************************************
 * End of function lpt_pwm_stop()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_initchan
* Description  : Initialize the LPT configuration.
* Arguments    : chan -
*                    LPT channel
*                cmt_period -
*                    CMT period(unit:us)
* Return Value : LPT_SUCCESS -
*                    LPT Initialize successfully
*                LPT_ERR_INVALID_ARG -
*                    cmt_period is invalid.
*                LPT_ERR_CONDITION_NOT_MET -
*                    LPT count not stop or LPCMR1 value not equal to LPTPRD
*                LPT_ERR_INVALID_CH -
*                    Selected channel is invalid.
******************************************************************************/
lpt_err_t lpt_initchan(lpt_ch_t chan, uint32_t const cmt_period)
{
    lpt_err_t   err = LPT_SUCCESS;
    uint32_t    calc_period;
    uint8_t     ratio_value;

    if ((LPT_CH0 != chan) && (LPT_CH1 != chan))
    {
        /* When selected invalid channnel, the error is treated as a invalid channel error. */
        return LPT_ERR_INVALID_CH;
    }

    if (0 != LPT.LPTCR3.BIT.LPCNTEN)
    {
        /* When LPT start, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    /* Calculate the value from the argument cmt_period */
    ratio_value = g_lpt_divratio_value[LPT.LPTCR1.BIT.LPCNTPSSEL];
    calc_period = lpt_regval_calc(cmt_period, LPT_LPTSRCCLK_HZ, ratio_value);

    if ((LPT_CH1 == chan) && (1 == LPT.LPTCR2.BIT.PWME))
    {
        if (LPT.LPTPRD.WORD != calc_period)
        {
            /* If the setting value not equal to LPTPRD when selected channel 1 and PWM output enable,
             * the error is treated as a condition error. */
            return LPT_ERR_CONDITION_NOT_MET;
        }
    }
    
    if (!((LPT_MIN_PERIOD <= calc_period) && (LPT_MAX_PERIOD >= calc_period)))
    {
        /* If cmt_period is not a computable value, the error is treated as a invalid error. */
        return LPT_ERR_INVALID_ARG;
    }

    if (LPT.LPTPRD.WORD < calc_period)
    {
        /* If cmt_period is greater than LPTPRD, the error is treated as a invalid error. */
        return LPT_ERR_INVALID_ARG;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Enable low-power compare match and set compare match value. */
    if (LPT_CH0 == chan)
    {
        LPT.LPTCR1.BIT.LPCMRE0 = 1;

        /* Cast type of "calc_period" to match type of LPCMR0 register. */
        LPT.LPCMR0.WORD = (uint16_t)calc_period;
    }
    else if (LPT_CH1 == chan)
    {
        LPT.LPTCR1.BIT.LPCMRE1 = 1;

        /* Cast type of "calc_period" to match type of LPCMR1 register. */
        LPT.LPCMR1.WORD = (uint16_t)calc_period;
    }
    else
    {
        /* RX140 has only channel 0 and 1 */
        /* Do nothing */
    }

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);

    return (err);
}
/*****************************************************************************
 * End of function lpt_initchan()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_setcmt
* Description  : Set the value of compare match 0 or 1.
* Arguments    : chan -
*                    LPT channel
*                cmt_period -
*                    CMT period(unit:us)
* Return Value : LPT_SUCCESS -
*                    Compare match value set successfully.
*                LPT_ERR_INVALID_ARG -
*                    cmt_period is invalid.
*                LPT_ERR_CONDITION_NOT_MET -
*                    LPT count not stop or LPCMR1 value not equal to LPTPRD
*                LPT_ERR_INVALID_CH -
*                    Selected channel is invalid.
******************************************************************************/
lpt_err_t lpt_setcmt(lpt_ch_t chan, uint32_t const cmt_period)
{
    lpt_err_t   err = LPT_SUCCESS;
    uint32_t    calc_period;
    uint8_t     ratio_value;

    if ((LPT_CH0 != chan) && (LPT_CH1 != chan))
    {
        /* When selected invalid channnel, the error is treated as a invalid channel error. */
        return LPT_ERR_INVALID_CH;
    }

    if ((LPT_CH1 == chan) || (0 == LPT.LPTCR2.BIT.PWME))
    {
        if (0 != LPT.LPTCR3.BIT.LPCNTEN)
        {
            /* When LPT start, the error is treated as a condition error. */
            return LPT_ERR_CONDITION_NOT_MET;
        }
    }

    /* Calculate the value from the argument cmt_period */
    ratio_value = g_lpt_divratio_value[LPT.LPTCR1.BIT.LPCNTPSSEL];
    calc_period = lpt_regval_calc(cmt_period, LPT_LPTSRCCLK_HZ, ratio_value);
    
    if ((LPT_CH1 == chan) && (1 == LPT.LPTCR2.BIT.PWME))
    {
        if (LPT.LPTPRD.WORD != calc_period)
        {
            /* If the setting value not equal to LPTPRD when selected channel 1 and PWM output enable,
             * the error is treated as a condition error. */
            return LPT_ERR_CONDITION_NOT_MET;
        }
    }

    if (!((LPT_MIN_PERIOD <= calc_period) && (LPT_MAX_PERIOD >= calc_period)))
    {
        /* If cmt_period is not a computable value, the error is treated as a invalid error. */
        return LPT_ERR_INVALID_ARG;
    }

    if (LPT.LPTPRD.WORD < calc_period)
    {
        /* If cmt_period is greater than LPTPRD, the error is treated as a invalid error. */
        return LPT_ERR_INVALID_ARG;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Set compare match for comparison with the LPT. */
    if (LPT_CH0 == chan)
    {
        /* Cast type of "calc_period" to match type of LPCMR0 register. */
        LPT.LPCMR0.WORD = (uint16_t)calc_period;
    }
    else if (LPT_CH1 == chan)
    {
        /* Cast type of "calc_period" to match type of LPCMR1 register. */
        LPT.LPCMR1.WORD = (uint16_t)calc_period;
    }
    else
    {
        /* RX140 has only channel 0 and 1 */
        /* Do nothing */
    }

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);

    return (err);
}
/*****************************************************************************
 * End of function lpt_setcmt()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_finalchan
* Description  : Finalize the LPT configuration.
* Arguments    : chan -
*                    LPT channel
* Return Value : LPT_SUCCESS -
*                    LPT Initialize successfully
*                LPT_ERR_CONDITION_NOT_MET -
*                    LPT count not stop.
*                LPT_ERR_INVALID_CH -
*                    Selected channel is invalid.
******************************************************************************/
lpt_err_t lpt_finalchan(lpt_ch_t chan)
{
    lpt_err_t   err = LPT_SUCCESS;

    if ((LPT_CH0 != chan) && (LPT_CH1 != chan))
    {
        /* When selected invalid channnel, the error is treated as a invalid channel error. */
        return LPT_ERR_INVALID_CH;
    }

    if (0 != LPT.LPTCR3.BIT.LPCNTEN)
    {
        /* When LPT start, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Disable low-power compare match. */
    if (LPT_CH0 == chan)
    {
        /* Disable low-power compare match 0. */
        LPT.LPTCR1.BIT.LPCMRE0 = 0;

        /* Finalize low-power timer compare match 0. */
        LPT.LPCMR0.WORD         = LPT_LPCMR0_INIT;
    }
    else if (LPT_CH1 == chan)
    {
        /* Disable low-power compare match 1*/
        LPT.LPTCR1.BIT.LPCMRE1 = 0;

        /* Finalize low-power timer compare match 1. */
        LPT.LPCMR1.WORD         = LPT_LPCMR1_INIT;
    }
    else
    {
        /* RX140 has only channel 0 and 1 */
        /* Do nothing */
    }

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);

    return (err);
}
/*****************************************************************************
 * End of function lpt_finalchan()
 *****************************************************************************/

/*****************************************************************************
* Function Name: lpt_initpwm
* Description  : Sets the PWM configuration.
* Arguments    : chan -
*                    LPT channel (Only 0ch can be used now)
*                p_config -
*                    PWM output configuration.
* Return Value : LPT_SUCCESS -
*                    LPT PWM configuration sets successfully
*                LPT_ERR_INVALID_ARG -
*                    output_polarity or output_level is invalid.
*                LPT_ERR_INVALID_CH -
*                    Selected invalid channel.
*                LPT_ERR_CONDITION_NOT_MET -
*                    LPT count not stop.
******************************************************************************/
lpt_err_t lpt_initpwm(lpt_ch_t chan, lpt_pwm_cfg_t * const p_config)
{
    lpt_err_t   err = LPT_SUCCESS;

    if (LPT_CH0 != chan)
    {
        /* Success is returned only when 0ch is selected. */
        return LPT_ERR_INVALID_CH;
    }

    if (0 != LPT.LPTCR3.BIT.LPCNTEN)
    {
        /* When LPT start, the error is treated as a condition error. */
        return LPT_ERR_CONDITION_NOT_MET;
    }

    if ((output_polarity_low != p_config->output_polarity) && (output_polarity_high != p_config->output_polarity))
    {
        /* When selected invalid output polarity, the error is treated as a invalid error. */
        return LPT_ERR_INVALID_ARG;
    }

    if ((output_level_low != p_config->output_level) && (output_level_high != p_config->output_level))
    {
        /* When selected invalid output level, the error is treated as a invalid error. */
        return LPT_ERR_INVALID_ARG;
    }

    /* Disable protection for registers related to the LPT. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPT);

    /* Sets the PWM output polarity and output level. */
    LPT.LPTCR2.BIT.OPOL = p_config->output_polarity;
    LPT.LPTCR2.BIT.OLVL = p_config->output_level;

    /* Enable protection for registers related to the LPT. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPT);

    return (err);
}
/*****************************************************************************
 * End of function lpt_initpwm()
 *****************************************************************************/
#endif /* defined BSP_MCU_RX140 */

/**********************************************************************************************************************
 * End of File
 *********************************************************************************************************************/
