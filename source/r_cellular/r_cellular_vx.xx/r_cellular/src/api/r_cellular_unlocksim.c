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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_cellular_unlocksim.c
 * Description  : Function to enter PIN code.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "at_command.h"

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
static e_cellular_err_t cellular_unlocksim (st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_pass);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_UnlockSIM
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_UnlockSIM(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_pass)
{
    uint32_t                   preemption    = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) || (NULL == p_pass))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (0 != (p_ctrl->running_api_count % 2))
        {
            ret = CELLULAR_ERR_OTHER_API_RUNNING;
        }
        else if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else
        {
            p_ctrl->running_api_count += 2;
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            ret = atc_cfun_check(p_ctrl);
            if (CELLULAR_SUCCESS == ret)
            {
                if ((CELLULAR_MODULE_OPERATING_LEVEL4 != p_ctrl->module_status) &&
                    (CELLULAR_MODULE_OPERATING_LEVEL1 != p_ctrl->module_status))
                {
                    ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
                }
            }

            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cpin_check(p_ctrl);
                if (CELLULAR_SUCCESS != ret)
                {
                    ret = cellular_unlocksim(p_ctrl, p_pass);
                }
            }
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
        p_ctrl->running_api_count -= 2;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_UnlockSIM
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_unlocksim
 ************************************************************************************************/
static e_cellular_err_t cellular_unlocksim(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_pass)
{
    uint8_t          str[CELLULAR_MAX_SIM_PASS_LENGTH+1]   = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    strncpy((char *)str, (char *)p_pass, sizeof(str));  // (uint8_t *)->(char *)

    p_command_arg[0] = str;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_PIN_LOCK_RELEASE], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_PIN_LOCK_RELEASE);

    memset(p_ctrl->sci_ctrl.atc_buff, 0x00, CELLULAR_ATC_BUFF_SIZE);
    memset(str, 0x00, sizeof(str));

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_unlocksim
 *********************************************************************************************************************/
