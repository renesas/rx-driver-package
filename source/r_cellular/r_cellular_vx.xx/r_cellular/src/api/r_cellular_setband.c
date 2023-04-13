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
 * File Name    : r_cellular_setband.c
 * Description  : Function to configure band settings.
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
#define CELLULAR_FLG_CHECK_LIMIT    (60)

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetBand
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetBand(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_band)
{
    uint8_t                    ctm_name[CELLULAR_MAX_CTM_LENGTH+1] = {0};

    volatile uint8_t           flg           = CELLULAR_FLG_OFF;
    uint8_t                    cnt           = 0;
    uint32_t                   preemption    = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) || (NULL == p_band))
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
        else if (CELLULAR_SYSTEM_CONNECT == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_ALREADY_CONNECT;
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
            p_ctrl->recv_data = (void *)ctm_name;   //(uint8_t[])->(void *)
            ret               = atc_sqnctm_check(p_ctrl);
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_sqnbandsel(p_ctrl, ctm_name, p_band);
            }

            if (CELLULAR_SUCCESS == ret)
            {
                p_ctrl->recv_data = (void *) &flg;  //(&uint8_t)->(void *)
                ret               = atc_reset(p_ctrl);
            }

            if (CELLULAR_SUCCESS == ret)
            {
                for (cnt = CELLULAR_START_SOCKET_NUMBER; cnt <= p_ctrl->creatable_socket; cnt++)
                {
                    ret = cellular_closesocket(p_ctrl, cnt);
                }

                cnt = 0;
                ret = CELLULAR_ERR_MODULE_COM;
                do
                {
                    if (CELLULAR_FLG_START == flg)
                    {
                        cnt = CELLULAR_FLG_CHECK_LIMIT;
                        ret = CELLULAR_SUCCESS;
                    }
                    else
                    {
                        cellular_delay_task(1000);
                    }
                    cnt++;
                } while (cnt < CELLULAR_FLG_CHECK_LIMIT);
            }

            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cfun_check(p_ctrl);
                if ((CELLULAR_SUCCESS                 == ret) &&
                    (CELLULAR_MODULE_OPERATING_LEVEL4 != p_ctrl->module_status) &&
                    (CELLULAR_MODULE_OPERATING_LEVEL1 != p_ctrl->module_status))
                {
                    ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
                }
            }
            p_ctrl->recv_data = NULL;
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
 * End of function R_CELLULAR_SetBand
 *********************************************************************************************************************/
