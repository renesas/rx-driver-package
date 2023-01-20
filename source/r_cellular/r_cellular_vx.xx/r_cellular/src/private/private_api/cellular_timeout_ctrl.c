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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_timeout_ctrl.c
 * Description  : Functions to manage timeouts.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*******************************************************************************
 * Function Name  @fn            cellular_timeout_init
 ******************************************************************************/
void cellular_timeout_init(st_cellular_time_ctrl_t * const p_timeout_ctrl, const uint32_t timeout_ms)
{
    uint32_t timeout = timeout_ms;

    if (0 == timeout_ms)
    {
        timeout = CELLULAR_TIME_OUT_MAX_DELAY;
    }

    p_timeout_ctrl->start_time = cellular_get_tickcount();
    p_timeout_ctrl->end_time = p_timeout_ctrl->start_time + timeout;

    if (p_timeout_ctrl->end_time < p_timeout_ctrl->start_time)
    {
        p_timeout_ctrl->timeout_overflow_flag = CELLULAR_TIMEOUT_OVERFLOW;
    }
    else
    {
        p_timeout_ctrl->timeout_overflow_flag = CELLULAR_TIMEOUT_NOT_OVERFLOW;
    }
}
/**********************************************************************************************************************
 * End of function cellular_timeout_init
 *********************************************************************************************************************/

/*******************************************************************************
 * Function Name  @fn            cellular_check_timeout
 ******************************************************************************/
e_cellular_timeout_check_t cellular_check_timeout(st_cellular_time_ctrl_t * const p_timeout_ctrl)
{
    p_timeout_ctrl->this_time = cellular_get_tickcount();

    if (CELLULAR_TIMEOUT_NOT_OVERFLOW == p_timeout_ctrl->timeout_overflow_flag)
    {
        if ((p_timeout_ctrl->this_time >= p_timeout_ctrl->end_time) ||
                (p_timeout_ctrl->this_time < p_timeout_ctrl->start_time))
        {
            return CELLULAR_TIMEOUT;
        }
    }
    else
    {
        if ((p_timeout_ctrl->this_time < p_timeout_ctrl->start_time) &&
                (p_timeout_ctrl->this_time >= p_timeout_ctrl->end_time))
        {
            return CELLULAR_TIMEOUT;
        }
    }

    cellular_delay_task(1);

    return CELLULAR_NOT_TIMEOUT;
}
/**********************************************************************************************************************
 * End of function cellular_check_timeout
 *********************************************************************************************************************/
