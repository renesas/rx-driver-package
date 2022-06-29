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
 * File Name    : r_cellular_createsocket.c
 * Description  : Function to create a socket.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
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
static e_cellular_err_t cellular_socket_cfg (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_num);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_CreateSocket
 ***********************************************************************/
int32_t R_CELLULAR_CreateSocket(st_cellular_ctrl_t * const p_ctrl, const uint8_t protocol_type,
                                    const uint8_t ip_version)
{
    uint8_t socket_num = 0;
    int32_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if ((NULL == p_ctrl) || ((CELLULAR_SOCKET_IP_PROTOCOL_TCP != protocol_type) &&
            (CELLULAR_SOCKET_IP_PROTOCOL_UDP != protocol_type)) ||
                    (CELLULAR_SOCKET_IP_VERSION_4 != ip_version))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else if (CELLULAR_SYSTEM_OPEN == p_ctrl->system_state)
        {
            ret =  CELLULAR_ERR_NOT_CONNECT;
        }
        else
        {
            R_BSP_NOP();
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret =  cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            while (socket_num < p_ctrl->creatable_socket)
            {
                if (CELLULAR_SOCKET_STATUS_CLOSED == p_ctrl->p_socket_ctrl[socket_num].socket_status)
                {
                    break;
                }
                else
                {
                    socket_num++;
                }
            }

            if (socket_num >= p_ctrl->creatable_socket)
            {
                ret = CELLULAR_ERR_SOCKET_CREATE_LIMIT;
            }
            else
            {
                ret = cellular_socket_cfg(p_ctrl, socket_num);
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
                if (CELLULAR_SUCCESS == ret)
                {
                    ret = atc_sqnsscfg(p_ctrl, socket_num + CELLULAR_START_SOCKET_NUMBER,
                            CELLULAR_SSL_DEACTIVE, socket_num + CELLULAR_START_SOCKET_NUMBER);
                }
#endif
                if (CELLULAR_SUCCESS == ret)
                {
                    p_ctrl->p_socket_ctrl[socket_num].ipversion = ip_version;
                    p_ctrl->p_socket_ctrl[socket_num].protocol = protocol_type;
                    p_ctrl->p_socket_ctrl[socket_num].socket_status = CELLULAR_SOCKET_STATUS_SOCKET;
                    p_ctrl->p_socket_ctrl[socket_num].receive_count = 0;
                    p_ctrl->p_socket_ctrl[socket_num].receive_num = 0;
                    p_ctrl->p_socket_ctrl[socket_num].port = 0;
                    p_ctrl->p_socket_ctrl[socket_num].ipaddr = 0;
                    p_ctrl->p_socket_ctrl[socket_num].receive_unprocessed_size = 0;
                    p_ctrl->p_socket_ctrl[socket_num].receive_flg = CELLULAR_RECEIVE_FLAG_OFF;
                    ret = socket_num + CELLULAR_START_SOCKET_NUMBER;
                }
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
 * End of function R_CELLULAR_CreateSocket
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_socket_cfg
 ***********************************************************************/
static e_cellular_err_t cellular_socket_cfg(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_num)
{
    e_cellular_err_t ret;

    ret = atc_sqnscfg(p_ctrl, socket_num + CELLULAR_START_SOCKET_NUMBER);
    if (CELLULAR_SUCCESS == ret)
    {
        ret = atc_sqnscfgext(p_ctrl, socket_num + CELLULAR_START_SOCKET_NUMBER);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_socket_cfg
 *********************************************************************************************************************/
