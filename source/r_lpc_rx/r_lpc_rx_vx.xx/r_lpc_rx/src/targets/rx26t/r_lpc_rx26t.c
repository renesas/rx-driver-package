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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/*********************************************************************************************************************
 * File Name    : r_lpc_rx26t.c
 * Description  : Implementation of the LPC module for the RX26T
 **********************************************************************************************************************/
/*********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 15.08.2022 1.00    First Release
 **********************************************************************************************************************/

/*********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX26T)

/* Public interface header file for this package. */
#include "r_lpc_rx_if.h"
#include "r_lpc_rx_private.h"
#include "r_lpc_rx26t_private.h"

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

static lpc_err_t lpc_lowpower_activate_check (void);

#endif /* LPC_CFG_PARAM_CHECKING_ENABLE */

/*********************************************************************************************************************
 * Function Name: lpc_low_power_mode_configure
 * Description  : This function will configure the MCU for the specified Low Power Mode. In the Low Power Mode,
 *                the CPU is turned OFF and peripherals may be on or off depending on the mode. The supported
 *                low power modes are listed in the enum lpc_low_power_mode_t
 * Arguments    : e_mode - Low Power Mode
 *                    LPC_LP_SLEEP
 *                    LPC_LP_ALL_MODULE_STOP
 *                    LPC_LP_SW_STANDBY
 *
 * Return Value : LPC_SUCCESS -
 ********************************************************************************************************************/
lpc_err_t lpc_low_power_mode_configure(lpc_low_power_mode_t e_mode)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
    bsp_int_ctrl_t int_ctrl;
#endif

    switch (e_mode)
    {
        case LPC_LP_SLEEP:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);// unlock clock control registers
            SYSTEM.SBYCR.BIT.SSBY   = 0;        // Move to Sleep on R_BSP_WAIT()
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
            SYSTEM.MSTPCRA.BIT.ACSE = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); // lock clock control registers
            break;
        
        case LPC_LP_ALL_MODULE_STOP:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);// unlock clock control registers
            SYSTEM.SBYCR.BIT.SSBY      = 0;     // Move to All Module Stop on R_BSP_WAIT(). ACSE also needs to be set
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
            SYSTEM.MSTPCRA.BIT.ACSE    = 1;
            SYSTEM.MSTPCRA.BIT.MSTPA24 = 1;
            SYSTEM.MSTPCRA.BIT.MSTPA27 = 1;
            SYSTEM.MSTPCRA.BIT.MSTPA29 = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) 
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); // lock clock control registers
    
            break;

        case LPC_LP_SW_STANDBY:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
            SYSTEM.SBYCR.BIT.SSBY    = 1;       // Move to Sw Stby on R_BSP_WAIT(); DPSBY also has to be modified
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
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
lpc_err_t lpc_lowpower_activate(lpc_callback_set_t pcallback)
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
lpc_err_t lpc_return_clock_switch(lpc_clock_switch_t e_clock_source, bool enable)
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
static lpc_err_t lpc_lowpower_activate_check(void)
{
    uint8_t rstckcr_rstcksel;

    if (0x1 == SYSTEM.SBYCR.BIT.SSBY) /* check if entering software standby */
    {
        if (LPC_OSC_STOP_ENABLED == SYSTEM.OSTDCR.BIT.OSTDE)
        {
            return LPC_ERR_OSC_STOP_ENABLED;    /* Cannot enter SW_Standby if Oscillation Stop is enabled */
        }
        
        if ((0 == SYSTEM.MSTPCRA.BIT.MSTPA28) 
            && ((1 == DTC.DTCST.BIT.DTCST) || (1 == DMAC.DMAST.BIT.DMST)))
        {
            return LPC_ERR_ILLEGAL;         /* Cannot enter SW_Stanby if DTC or DMAC is enabled */
        }

        /* Check FENTRYC(b0) and FENTRYD(b7) */
        if (0x0000 != (FLASH.FENTRYR.WORD & 0x0081))
        {
            return LPC_ERR_ILLEGAL;         /* Cannot enter SW_Stanby if P/E mode entry */
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

        /* Exiting from sleep-mode with the HOCO selected is not permitted */
        if (LPC_HOCO == rstckcr_rstcksel) /* If HOCO is chosen as the return clock */
        {
            if (1 == SYSTEM.HOCOPCR.BIT.HOCOPCNT)
            {
                return LPC_ERR_ILL_CLOCK_SOURCE;    /* Cannot enter Sleep if HOCO power off */
            }
        }

    }
    else if (1 == SYSTEM.MSTPCRA.BIT.ACSE) /* Check if entering All Clock Stop Mode */
    {
        if ((((LPC_ACS_CHK_MSTPCRA != (SYSTEM.MSTPCRA.LONG &LPC_ACS_CHK_MSTPCRA))
            ||(LPC_ACS_CHK_MSTPCRB != (SYSTEM.MSTPCRB.LONG &LPC_ACS_CHK_MSTPCRB)))
            ||(LPC_ACS_CHK_MSTPCRC != (SYSTEM.MSTPCRC.LONG &LPC_ACS_CHK_MSTPCRC)))
            ||(LPC_ACS_CHK_MSTPCRD != (SYSTEM.MSTPCRD.LONG &LPC_ACS_CHK_MSTPCRD)))
        {
            return LPC_ERR_ILLEGAL;             /* Cannot enter All Clock stop if Module stop release */
        }
        
        /* Check FENTRYC(b0) and FENTRYD(b7) */
        if (0x0000 != (FLASH.FENTRYR.WORD & 0x0081))
        {
            return LPC_ERR_ILLEGAL;         /* Cannot enter SW_Stanby if P/E mode entry */
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


#endif /* BSP_MCU_RX26T */


/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
