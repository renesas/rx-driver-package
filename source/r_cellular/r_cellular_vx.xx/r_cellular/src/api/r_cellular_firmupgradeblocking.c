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
 * File Name    : r_cellular_firmupgradeblocking.c
 * Description  : Function to perform firmware upgrade.
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
#define CELLULAR_RESTART_LIMIT      (100)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static e_cellular_err_t cellular_firmupgradeblocking (st_cellular_ctrl_t * const p_ctrl,
                                                        const uint8_t * const p_url,
                                                        const uint8_t spid);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_FirmUpgradeBlocking
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_FirmUpgradeBlocking(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_url,
                                                    const uint8_t spid)
{
    uint8_t                    cnt           = 0;
    uint32_t                   preemption    = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) || (NULL == p_url) || (CELLULAR_SECURITY_PROFILE_ID_H < spid))
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
        else if (CELLULAR_SYSTEM_OPEN == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_CONNECT;
        }
        else
        {
            p_ctrl->running_api_count += 1;
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        for (cnt = CELLULAR_START_SOCKET_NUMBER; cnt <= p_ctrl->creatable_socket; cnt++)
        {
            ret = cellular_shutdownsocket(p_ctrl, cnt);
            if (CELLULAR_SUCCESS != ret)
            {
                cnt = p_ctrl->creatable_socket;
            }
        }

        if (CELLULAR_SUCCESS == ret)
        {
            for (cnt = CELLULAR_START_SOCKET_NUMBER; cnt <= p_ctrl->creatable_socket; cnt++)
            {
                ret = cellular_closesocket(p_ctrl, cnt);
            }
        }

        if (CELLULAR_SUCCESS == ret)
        {
            semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
            if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
            {
                ret = cellular_firmupgradeblocking(p_ctrl, p_url, spid);
                cellular_give_semaphore(p_ctrl->at_semaphore);
            }
            else
            {
                ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
            }
        }
        p_ctrl->running_api_count -= 1;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_FirmUpgradeBlocking
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_firmupgradeblocking
 ***********************************************************************/
static e_cellular_err_t cellular_firmupgradeblocking(st_cellular_ctrl_t * const p_ctrl,
                                                        const uint8_t * const p_url,
                                                        const uint8_t spid)
{
    volatile uint8_t flg = CELLULAR_FLG_OFF;
    uint8_t          cnt = 0;
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    p_ctrl->recv_data = (void *) &flg; //(&uint8_t)->(void *)
    ret               = atc_sqnsupgrade(p_ctrl, p_url, 1, CELLULAR_FIRM_UPGRADE_BLOCKING, spid);
    if (CELLULAR_SUCCESS == ret)
    {
        p_ctrl->system_state = CELLULAR_SYSTEM_OPEN;
        ret                  = CELLULAR_ERR_RECV_TASK;

        do
        {
            if (CELLULAR_FLG_START == flg)
            {
                ret = CELLULAR_SUCCESS;
                cnt = CELLULAR_RESTART_LIMIT;
            }
            else
            {
                cellular_delay_task(1000);
            }
            cnt++;
        } while (cnt < CELLULAR_RESTART_LIMIT);

        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_cfun_check(p_ctrl);
        }

        if ((CELLULAR_SUCCESS                 == ret) &&
            (CELLULAR_MODULE_OPERATING_LEVEL4 != p_ctrl->module_status) &&
            (CELLULAR_MODULE_OPERATING_LEVEL1 != p_ctrl->module_status))
        {
            ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
        }
    }

    p_ctrl->recv_data = NULL;

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_firmupgradeblocking
 *********************************************************************************************************************/
