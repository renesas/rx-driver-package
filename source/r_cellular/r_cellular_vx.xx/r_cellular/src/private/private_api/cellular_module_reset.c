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
 * File Name    : cellular_module_reset.c
 * Description  : Function to reset the module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "at_command.h"
#include "cellular_freertos.h"

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

/************************************************************************
 * Function Name  @fn            cellular_module_reset
 ***********************************************************************/
e_cellular_err_t cellular_module_reset(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_ERR_MODULE_TIMEOUT;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;
    e_cellular_auto_connect_t type = CELLULAR_DISABLE_AUTO_CONNECT;

    uint8_t flg = CELLULAR_START_FLG_OFF;
    uint8_t count = 0;

    p_ctrl->recv_data = (void *) &flg; //(&uint8_t)->(void *)

    for (count = CELLULAR_START_SOCKET_NUMBER; count <= p_ctrl->creatable_socket; count++ )
    {
        cellular_closesocket(p_ctrl, count);
    }

    if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
    {
        while (1)
        {
            semaphore_ret = cellular_take_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
            if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
            {
                break;
            }
            else
            {
                cellular_delay_task(1);
            }
        }
        cellular_rts_ctrl(0);
    }

    CELLULAR_SET_PODR(CELLULAR_CFG_RESET_PORT, CELLULAR_CFG_RESET_PIN) = CELLULAR_CFG_RESET_SIGNAL_ON;
    CELLULAR_SET_PDR(CELLULAR_CFG_RESET_PORT, CELLULAR_CFG_RESET_PIN) = CELLULAR_PIN_DIRECTION_MODE_OUTPUT;

    p_ctrl->system_state = CELLULAR_SYSTEM_OPEN;
    p_ctrl->module_status = CELLULAR_MODULE_OPERATING_RESET;
    p_ctrl->sci_ctrl.atc_flg = CELLULAR_ATC_RESPONSE_CONFIRMED;

    do
    {
        cellular_delay_task(1); /* hold reset signal time for cellular module */
    } while ((count < CELLULAR_RESTART_LIMIT) && (CELLULAR_MODULE_OPERATING_LEVEL0 != p_ctrl->module_status));

    CELLULAR_SET_PODR(CELLULAR_CFG_RESET_PORT, CELLULAR_CFG_RESET_PIN) = CELLULAR_CFG_RESET_SIGNAL_OFF;
    cellular_delay_task(2000); /* wait wake up time for cellular module initialization */

    do
    {
        if (CELLULAR_START_FLG_ON == flg)
        {
            count = CELLULAR_RESTART_LIMIT;
            ret = CELLULAR_SUCCESS;
        }
        else
        {
            cellular_delay_task(1000);
        }
        count++;
    } while (count < CELLULAR_RESTART_LIMIT);

    p_ctrl->recv_data = NULL;

    if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
    {
        cellular_give_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
        cellular_rts_ctrl(1);
    }

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            p_ctrl->recv_data = (void *) &type; //cast
            ret = atc_sqnautoconnect_check(p_ctrl);
            if ((CELLULAR_SUCCESS == ret) && (CELLULAR_DISABLE_AUTO_CONNECT == type))
            {
                ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
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
 * End of function cellular_module_reset
 *********************************************************************************************************************/
