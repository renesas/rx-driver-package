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
 * File Name    : r_cellular_setpsm.c
 * Description  : Function to set the PSM of the module.
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

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetPSM
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetPSM(st_cellular_ctrl_t * const p_ctrl, const st_cellular_psm_config_t * const p_config,
                                    st_cellular_psm_config_t * const p_result)
{
    uint32_t                   preemption    = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) || (NULL == p_config))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (((CELLULAR_PSM_MODE_INVALID > p_config->psm_mode) ||
            (CELLULAR_PSM_MODE_INIT < p_config->psm_mode)) ||
                ((CELLULAR_TAU_CYCLE_10_MIN > p_config->tau_cycle) ||
                (CELLULAR_TAU_CYCLE_NONE < p_config->tau_cycle)) ||
                    ((CELLULAR_ACTIVE_CYCLE_2_SEC > p_config->active_cycle) ||
                    (CELLULAR_ACTIVE_CYCLE_NONE < p_config->active_cycle)) ||
                        ((CELLULAR_CYCLE_MULTIPLIER_0 > p_config->tau_multiplier) ||
                        (CELLULAR_CYCLE_MULTIPLIER_31 < p_config->tau_multiplier)) ||
                            ((CELLULAR_CYCLE_MULTIPLIER_0 > p_config->active_multiplier) ||
                            (CELLULAR_CYCLE_MULTIPLIER_31 < p_config->active_multiplier)))
        {
            ret = CELLULAR_ERR_PARAMETER;
        }
        else if (0 != (p_ctrl->running_api_count % 2))
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
            ret = cellular_psm_config(p_ctrl, p_config->psm_mode);
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cpsms(p_ctrl, p_config);
            }

            if ((CELLULAR_SUCCESS == ret) && (NULL != p_result))
            {
                p_ctrl->recv_data = p_result;
                ret               = atc_cpsms_check(p_ctrl);
                p_ctrl->recv_data = NULL;
                cellular_delay_task(1000);
            }
            cellular_give_semaphore(p_ctrl->at_semaphore);

            if (CELLULAR_SUCCESS == ret)
            {
                ret = cellular_module_reset(p_ctrl);
            }
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
 * End of function R_CELLULAR_SetPSM
 *********************************************************************************************************************/
