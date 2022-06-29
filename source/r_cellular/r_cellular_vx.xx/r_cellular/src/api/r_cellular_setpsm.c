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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_cellular_setpsm.c
 * Description  : Function to set the PSM of the module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : xx.xx.xxxx 1.00     First Release
 *         : 02.09.2021 1.01     Fixed reset timing
 *         : 21.10.2021 1.02     Support for Azure RTOS
 *                               Support for GCC for Renesas GNURX Toolchain
 *         : 15.11.2021 1.03     Improved receiving behavior, removed socket buffers
 *         : 24.01.2022 1.04     R_CELLULAR_SetPSM and R_CELLULAR_SetEDRX have been added as new APIs
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
static e_cellular_err_t cellular_psm_config (st_cellular_ctrl_t * const p_ctrl, const e_cellular_psm_mode_t mode);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetPSM
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetPSM(st_cellular_ctrl_t * const p_ctrl, const e_cellular_psm_mode_t mode,
                            const e_cellular_tau_cycle_t tau, const e_cellular_cycle_multiplier_t tau_multiplier,
                            const e_cellular_active_cycle_t active, const e_cellular_cycle_multiplier_t active_multiplier)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if ((NULL == p_ctrl) || ((CELLULAR_PSM_MODE_INVALID > mode) || (CELLULAR_PSM_MODE_INIT < mode)) ||
            ((CELLULAR_TAU_CYCLE_10_MIN > tau) || (CELLULAR_TAU_CYCLE_NONE < tau)) ||
            ((CELLULAR_ACTIVE_CYCLE_2_SEC > active) || (CELLULAR_ACTIVE_CYCLE_NONE < active)) ||
            ((CELLULAR_CYCLE_MULTIPLIER_0 > tau_multiplier) || (CELLULAR_CYCLE_MULTIPLIER_31 < tau_multiplier)) ||
            ((CELLULAR_CYCLE_MULTIPLIER_0 > active_multiplier) || (CELLULAR_CYCLE_MULTIPLIER_31 < active_multiplier)))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            ret = cellular_psm_config(p_ctrl, mode);
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cpsms(p_ctrl, mode, tau, tau_multiplier, active, active_multiplier);
            }
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_SetPSM
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_psm_config
 ***********************************************************************/
static e_cellular_err_t cellular_psm_config(st_cellular_ctrl_t * const p_ctrl, const e_cellular_psm_mode_t mode)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    if (CELLULAR_PSM_MODE_ACTIVE == mode)
    {
        ret = atc_sqnricfg(p_ctrl, CELLULAR_PSM_MODE_ACTIVE);

        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_cmer(p_ctrl, CELLULAR_PSM_MODE_ACTIVE);
        }
        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_sqnipscfg(p_ctrl, CELLULAR_PSM_MODE_ACTIVE);
        }
    }
    else
    {
        ret = atc_sqnricfg(p_ctrl, CELLULAR_PSM_MODE_INVALID);

        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_cmer(p_ctrl, CELLULAR_PSM_MODE_INVALID);
        }
        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_sqnipscfg(p_ctrl, CELLULAR_PSM_MODE_INVALID);
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_psm_config
 *********************************************************************************************************************/
