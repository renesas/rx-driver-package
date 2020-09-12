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
/***********************************************************************************************************************
 * File Name    : r_lpc_rx.c
 * Description  : The LPC module configures the MCU for the different operating and low power modes.
 *                This file is the LPC module's interface header file and should be included by the application that
 *                intends to use the API.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.10.2016 1.00    First Release
 *         : 04.22.2019 1.41    Delete inline definition of GetVersion function
 *         : 14.11.2019 2.00    Added support for GNUC and ICCRX.
 ***********************************************************************************************************************/
/*******************************************************************************
  Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Defines for LPC support */
#include "r_lpc_rx_if.h"

/* Include specifics for chosen MCU.  */
#include "r_lpc_rx_private.h"

/********************************************************************************************************************
 * Function Name: R_LPC_OperatingModeSet
 * Description  : This function will configure the MCU for the specified operating mode. In any of the operating
 *                modes, the CPU is running, but the maximum clock frequencies are limited based on the mode.
 * Arguments    : e_mode - Operating Mode
 * Return Value : LPC_SUCCESS - SUCCESS
 *                LPC_ERR_CLOCK_EXCEEDED -
 *                    Clock exceeds the limit of current operating mode
 *                LPC_ERR_P_E_MODE -
 *                    The operation mode can't be changed in P/E mode.
 ********************************************************************************************************************/
lpc_err_t R_LPC_OperatingModeSet (lpc_operating_mode_t e_mode)
{
    return lpc_operating_mode_set(e_mode);

}

/*********************************************************************************************************************
 End of function R_LPC_OperatingModeSet
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: R_LPC_LowPowerModeConfigure
 * Description  : This function will configure the MCU for the specified Low Power Mode. In the Low Power Mode,
 *                the CPU is turned OFF and peripherals may be on or off depending on the mode. The supported
 *                low power modes are listed in the enum lpc_low_power_mode_t
 * Arguments    : e_mode - Low Power Mode
 *
 * Return Value : LPC_SUCCESS -
 ********************************************************************************************************************/
lpc_err_t R_LPC_LowPowerModeConfigure (lpc_low_power_mode_t e_mode)
{
    return lpc_low_power_mode_configure(e_mode);
}
/*********************************************************************************************************************
 End of function R_LPC_LowPowerModeConfigure
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * Function Name: R_LPC_LowPowerModeActivate
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
 *                LPC_ERR_ILL_MAIN_CLK_FREQ -
 *                    If Main Oscillator is more than limit and is chosen as the Sleep return clock
 *                LPC_ERR_DEEP_SLEEP_STATUS -
 *                    The condition to go to a deep sleep mode isn't met.
 *                LPC_ERR_ILLEGAL -
 *                    Other than those above.
 ********************************************************************************************************************/
lpc_err_t R_LPC_LowPowerModeActivate (lpc_callback_set_t pcallback)
{
    return lpc_lowpower_activate(pcallback);

}
/*********************************************************************************************************************
 End of function R_LPC_LowPowerModeActivate
 ********************************************************************************************************************/


/*********************************************************************************************************************
 * Function Name: R_LPC_ReturnClockSwitch
 * Description  : This function will configure the return clock switching parameter that allows the clock source to
 *                be switched on returning from Sleep Mode to either the HOCO, LOCO or Main Clock. 
 * Arguments    : lpc_clock_switch_t e_clock_source -
 *                  Specify the clock source to switch to.
 *                bool enable
 *                  Enable/disable the Return clock Source switching function.
 * Return Value : LPC_SUCCESS
 *********************************************************************************************************************/
lpc_err_t R_LPC_ReturnClockSwitch (lpc_clock_switch_t e_clock_source, bool enable)
{
    return lpc_return_clock_switch(e_clock_source, enable);

}
/*********************************************************************************************************************
 End of function R_LPC_ReturnClockSwitch
 ********************************************************************************************************************/

/*****************************************************************************
 * Function Name: R_LPC_GetVersion
 * Description  : Returns the version of this module. The version number is
 *                encoded such that the top two bytes are the major version
 *                number and the bottom two bytes are the minor version number.
 * Arguments    : none
 * Return Value : version number
 ******************************************************************************/
uint32_t R_LPC_GetVersion (void)
{
    uint32_t version;

    version = (LPC_RX_VERSION_MAJOR << 16) | LPC_RX_VERSION_MINOR;

    return version;
}
/*********************************************************************************************************************
 End of function R_LPC_GetVersion
 ********************************************************************************************************************/

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
