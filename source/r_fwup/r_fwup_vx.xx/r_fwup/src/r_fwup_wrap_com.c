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
 * Copyright (C) 2023-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_com.c
 * Description  : Functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 20.07.2023 2.00    First Release
 *         : 29.09.2023 2.01    Fixed log messages.
 *                              Add parameter checking.
 *                              Added arguments to R_FWUP_WriteImageProgram API.
 *         : 28.03.2024 2.02    Update wrapper functions.
 *         : 09.04.2024 2.03    Fixed wrapper function.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_wrap_com.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#if (FWUP_CFG_USER_DISABLE_INTERRUPT_ENABLED != 0)
void FWUP_CFG_USER_DISABLE_INTERRUPT_FUNCTION(void);
#endif /* (FWUP_CFG_USER_DISABLE_INTERRUPT_ENABLED != 0) */

#if (FWUP_CFG_USER_ENABLE_INTERRUPT_ENABLED != 0)
void FWUP_CFG_USER_ENABLE_INTERRUPT_FUNCTION(void);
#endif /* (FWUP_CFG_USER_ENABLE_INTERRUPT_ENABLED != 0) */

#if (FWUP_CFG_USER_SOFTWARE_RESET_ENABLED != 0)
void FWUP_CFG_USER_SOFTWARE_RESET_FUNCTION(void);
#endif /* (FWUP_CFG_USER_SOFTWARE_RESET_ENABLED != 0) */

#if (FWUP_CFG_USER_SOFTWARE_DELAY_ENABLED != 0)
uint32_t FWUP_CFG_USER_SOFTWARE_DELAY_FUNCTION(uint32_t delay, e_fwup_delay_units_t units);
#endif /* (FWUP_CFG_USER_SOFTWARE_DELAY_ENABLED != 0) */

/*
 * Common functions
 */
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_disable_interrupt
 * Description  : wrapper function for disable interrupts
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void r_fwup_wrap_disable_interrupt(void)
{
#if (FWUP_CFG_USER_DISABLE_INTERRUPT_ENABLED == 1)
    FWUP_CFG_USER_DISABLE_INTERRUPT_FUNCTION();
#else
    R_BSP_InterruptsDisable();
#endif /* (FWUP_CFG_USER_DISABLE_INTERRUPT_ENABLED == 1) */
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_disable_interrupt
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_enable_interrupt
 * Description  : wrapper function for enable interrupts
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void r_fwup_wrap_enable_interrupt(void)
{
#if (FWUP_CFG_USER_ENABLE_INTERRUPT_ENABLED == 1)
    FWUP_CFG_USER_ENABLE_INTERRUPT_FUNCTION();
#else /* (FWUP_CFG_USER_ENABLE_INTERRUPT_ENABLED == 1) */
    R_BSP_InterruptsEnable();
#endif /* (FWUP_CFG_USER_ENABLE_INTERRUPT_ENABLED == 1) */
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_enable_interrupt
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_software_reset
 * Description  : wrapper function for execute software reset.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void r_fwup_wrap_software_reset(void)
{
#if (FWUP_CFG_USER_SOFTWARE_RESET_ENABLED == 1)
    FWUP_CFG_USER_SOFTWARE_RESET_FUNCTION();
#else
    r_fwup_wrap_disable_interrupt();
    r_fwup_wrap_software_delay(1000, FWUP_DELAY_MILLISECS);

    /* SW Reset */
    R_BSP_SoftwareReset();
#endif /* (FWUP_CFG_USER_SOFTWARE_RESET_ENABLED == 1) */
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_software_reset
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_software_delay
 * Description  : wrapper function for execute software delay.
 * Arguments    : delay : delay count
 *                units : FWUP_DELAY_MICROSECS/FWUP_DELAY_MILLISECS/FWUP_DELAY_SECS
 * Return Value : 1     : success
 *                other : error
 *********************************************************************************************************************/
uint32_t r_fwup_wrap_software_delay(uint32_t delay, e_fwup_delay_units_t units)
{
#if (FWUP_CFG_USER_SOFTWARE_DELAY_ENABLED == 1)
    return FWUP_CFG_USER_SOFTWARE_DELAY_FUNCTION(delay, units);
#else
    uint32_t  time_units;

    if (FWUP_DELAY_MICROSECS == units)
    {
        time_units = BSP_DELAY_MICROSECS;
    }
    else if (FWUP_DELAY_MILLISECS == units)
    {
        time_units = BSP_DELAY_MILLISECS;
    }
    else
    {
        time_units = BSP_DELAY_SECS;
    }
    return ((uint32_t)R_BSP_SoftwareDelay(delay, (bsp_delay_units_t)time_units));
#endif /* (FWUP_CFG_USER_SOFTWARE_DELAY_ENABLED == 1) */
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_software_delay
 *********************************************************************************************************************/
