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
 * File Name    : r_cellular_softwarereset.c
 * Description  : Reset function by AT command.
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
#define CELLULAR_FLG_CHECK_LIMIT    (60)

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
 * Function Name  @fn            R_CELLULAR_SoftwareReset
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SoftwareReset(st_cellular_ctrl_t * const p_ctrl)
{
    uint32_t preemption = 0;
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;
    e_cellular_auto_connect_t type = CELLULAR_DISABLE_AUTO_CONNECT;
    uint8_t i = 0;
    uint8_t flg = CELLULAR_START_FLG_OFF;
    uint8_t count = 0;

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
        for (i = CELLULAR_START_SOCKET_NUMBER; i <= p_ctrl->creatable_socket; i++ )
        {
            cellular_closesocket(p_ctrl, i);
        }

        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            p_ctrl->recv_data = (void *) &type; //(&e_cellular_auto_connect_t)->(void *)
            ret = atc_sqnautoconnect_check(p_ctrl);
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cereg(p_ctrl, CELLULAR_ENABLE_NETWORK_RESULT_CODE_LEVEL2);
            }
            if (CELLULAR_SUCCESS == ret)
            {
                p_ctrl->recv_data = (void *) &flg; //(&uint8_t)->(void *)
                ret = atc_reset(p_ctrl);
            }
            if (CELLULAR_SUCCESS == ret)
            {
                ret = CELLULAR_ERR_MODULE_COM;
                do
                {
                    if (CELLULAR_START_FLG_ON == flg)
                    {
                        count = CELLULAR_FLG_CHECK_LIMIT;
                        ret = CELLULAR_SUCCESS;
                    }
                    else
                    {
                        cellular_delay_task(1000);
                    }
                    count++;
                } while (count < CELLULAR_FLG_CHECK_LIMIT);
            }
            if ((CELLULAR_SUCCESS == ret) && (CELLULAR_DISABLE_AUTO_CONNECT == type))
            {
                ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
            }
            p_ctrl->recv_data = NULL;
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }

        p_ctrl->running_api_count -= 1;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_SoftwareReset
 *********************************************************************************************************************/
