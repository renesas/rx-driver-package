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
 * File Name    : r_cellular_factoryreset.c
 * Description  : Function to restore factory conditions.
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
#define RESET_LIMIT     (10)
#define MAX_CID_NUM     (8u)
#define DUMMY_DATA      "RESET"

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static e_cellular_err_t cellular_factoryreset (st_cellular_ctrl_t * const p_ctrl);
static e_cellular_err_t cellular_psm_check (st_cellular_ctrl_t * const p_ctrl);
static e_cellular_err_t private_cgdcont (st_cellular_ctrl_t * const p_ctrl, const uint8_t cid);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_FactoryReset
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_FactoryReset(st_cellular_ctrl_t * const p_ctrl)
{
    uint32_t                   preemption    = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;

    preemption = cellular_interrupt_disable();
    if (NULL == p_ctrl)
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (0 < (p_ctrl->running_api_count))
        {
            ret = CELLULAR_ERR_OTHER_API_RUNNING;
        }
        else if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else
        {
            p_ctrl->running_api_count += 1;
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        ret = cellular_factoryreset(p_ctrl);

        p_ctrl->running_api_count -= 1;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_FactoryReset
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_factoryreset
 ************************************************************************************************/
static e_cellular_err_t cellular_factoryreset(st_cellular_ctrl_t * const p_ctrl)
{
    uint8_t                    cnt                 = 0;
    uint8_t                    cnt_res             = MAX_CID_NUM;
    uint8_t                    reset_cnt           = 0;
    volatile uint8_t           flg[MAX_CID_NUM][7] = {0};
    e_cellular_err_t           ret                 = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret       = CELLULAR_SEMAPHORE_SUCCESS;

    p_ctrl->recv_data = (void *) flg;   //cast
    semaphore_ret     = cellular_take_semaphore(p_ctrl->at_semaphore);
    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
    {
        ret = atc_cgdcont_check(p_ctrl);
        if (CELLULAR_SUCCESS == ret)
        {
            for (cnt = MAX_CID_NUM; cnt > 0; cnt--)
            {
                if (0 == flg[cnt - 1][0])
                {
                    cnt_res = cnt;
                    cnt     = 1;    /* Exit this For Loop to set "1" */
                }
            }
            ret = private_cgdcont(p_ctrl, cnt_res);
        }
        cellular_give_semaphore(p_ctrl->at_semaphore);
    }
    else
    {
        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
    }

    if (CELLULAR_SUCCESS == ret)
    {
        while (1)
        {
            semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
            if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
            {
                atc_sqnsfactoryreset(p_ctrl);
                cellular_give_semaphore(p_ctrl->at_semaphore);
                ret = cellular_module_reset(p_ctrl);
                if (CELLULAR_SUCCESS == ret)
                {
                    memset((uint8_t *)flg, 0x00, sizeof(flg));  //cast
                    p_ctrl->recv_data = (void *) flg;           //cast
                    semaphore_ret     = cellular_take_semaphore(p_ctrl->at_semaphore);
                    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
                    {
                        ret = atc_cgdcont_check(p_ctrl);
                        cellular_give_semaphore(p_ctrl->at_semaphore);
                    }
                    else
                    {
                        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
                    }
                }
                else
                {
                    break;
                }

                if (CELLULAR_SUCCESS == ret)
                {
                    if ((0 == flg[cnt_res - 1][0]) ||
                        (0 != strncmp((char *)&flg[cnt_res - 1][1], DUMMY_DATA, 5))) //(uint8_t *)->(char *)
                    {
                        break;
                    }
                    else
                    {
                        reset_cnt++;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                reset_cnt++;
                ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
            }
            if (RESET_LIMIT <= reset_cnt)
            {
                break;
            }
        }

        if ((CELLULAR_SUCCESS == ret) && (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm))
        {
            ret = cellular_psm_check(p_ctrl);
        }
    }
    p_ctrl->recv_data = NULL;

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_factoryreset
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_psm_check
 ************************************************************************************************/
static e_cellular_err_t cellular_psm_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t                  ret        = CELLULAR_SUCCESS;
    volatile st_cellular_psm_config_t psm_result = {CELLULAR_PSM_MODE_INVALID,
                                                    CELLULAR_TAU_CYCLE_10_MIN,
                                                    CELLULAR_CYCLE_MULTIPLIER_0,
                                                    CELLULAR_ACTIVE_CYCLE_2_SEC,
                                                    CELLULAR_CYCLE_MULTIPLIER_0};
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
    {
        p_ctrl->recv_data = (void *) &psm_result;   //cast
        ret               = atc_cpsms_check(p_ctrl);
        if (CELLULAR_PSM_MODE_ACTIVE != psm_result.psm_mode)
        {
            ret = cellular_psm_config(p_ctrl, CELLULAR_PSM_MODE_INVALID);
        }
        p_ctrl->recv_data = NULL;
        cellular_give_semaphore(p_ctrl->at_semaphore);
    }
    else
    {
        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_psm_check
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            private_cgdcont
 ************************************************************************************************/
static e_cellular_err_t private_cgdcont(st_cellular_ctrl_t * const p_ctrl, const uint8_t cid)
{
    uint8_t          str_1[2]                              = {0};
    uint8_t          str_2[6]                              = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str_1, "%d", cid);          // (uint8_t *)->(char *)
    sprintf((char *)str_2, "%s", DUMMY_DATA);   // (uint8_t *)->(char *)

    p_command_arg[0] = str_1;
    p_command_arg[1] = str_2;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_PRIVATE_AP_CONFIG], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_PRIVATE_AP_CONFIG);

    return ret;
}
/**********************************************************************************************************************
 * End of function private_cgdcont
 *********************************************************************************************************************/
