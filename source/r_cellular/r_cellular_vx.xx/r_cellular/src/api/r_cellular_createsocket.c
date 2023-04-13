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
static int32_t cellular_socket_cfg (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_num,
                                    const uint8_t protocol_type, const uint8_t ip_version);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_CreateSocket
 ***********************************************************************/
int32_t R_CELLULAR_CreateSocket(st_cellular_ctrl_t * const p_ctrl, const uint8_t protocol_type,
                                    const uint8_t ip_version)
{
    uint32_t                   preemption    = 0;
    uint8_t                    socket_num    = 0;
    int32_t                    api_ret       = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) || ((CELLULAR_PROTOCOL_TCP  != protocol_type) &&
                            (CELLULAR_PROTOCOL_UDP   != protocol_type)) ||
                            ((CELLULAR_PROTOCOL_IPV4 != ip_version) &&
                            (CELLULAR_PROTOCOL_IPV6  != ip_version)))
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
        else if (CELLULAR_SYSTEM_OPEN == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_CONNECT;
        }
        else
        {
            R_BSP_NOP();
        }

        if (CELLULAR_SUCCESS == ret)
        {
            if ((CELLULAR_PROTOCOL_IPV6 == ip_version) &&
                    (CELLULAR_IPV6_ADDR_LENGTH > strlen((char *)p_ctrl->pdp_addr.ipv6))) //(uint8_t *)->(char *)
            {
                ret = CELLULAR_ERR_SIM_NOT_SUPPORT_IPV6;
            }
            else
            {
                p_ctrl->running_api_count += 2;
            }
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
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
                api_ret = cellular_socket_cfg(p_ctrl, socket_num, protocol_type, ip_version);
            }
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
        p_ctrl->running_api_count -= 2;
    }

    if (CELLULAR_SUCCESS != ret)
    {
        api_ret = (int32_t)ret; //cast
    }

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_CreateSocket
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_socket_cfg
 ***********************************************************************/
static int32_t cellular_socket_cfg(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_num,
                                    const uint8_t protocol_type, const uint8_t ip_version)
{
    uint8_t          start_num = CELLULAR_START_SOCKET_NUMBER;
    int32_t          ret;
    e_cellular_err_t atc_ret;

    atc_ret = atc_sqnscfg(p_ctrl, (uint8_t)(socket_num + start_num));           //cast
    if (CELLULAR_SUCCESS == atc_ret)
    {
        atc_ret = atc_sqnscfgext(p_ctrl, (uint8_t)(socket_num + start_num));    //cast
    }

#if (CELLULAR_IMPLEMENT_TYPE == 'B')
    if (CELLULAR_SUCCESS == atc_ret)
    {
        atc_ret = atc_sqnsscfg(p_ctrl, (uint8_t)(socket_num + start_num),       //cast
                    CELLULAR_SSL_DEACTIVE, (uint8_t)(socket_num + start_num));  //cast
    }
#endif
    if (CELLULAR_SUCCESS == atc_ret)
    {
        p_ctrl->p_socket_ctrl[socket_num].ipversion                = ip_version;
        p_ctrl->p_socket_ctrl[socket_num].protocol                 = protocol_type;
        p_ctrl->p_socket_ctrl[socket_num].socket_status            = CELLULAR_SOCKET_STATUS_SOCKET;
        p_ctrl->p_socket_ctrl[socket_num].receive_count            = 0;
        p_ctrl->p_socket_ctrl[socket_num].receive_num              = 0;
        p_ctrl->p_socket_ctrl[socket_num].port                     = 0;
        p_ctrl->p_socket_ctrl[socket_num].receive_unprocessed_size = 0;
        p_ctrl->p_socket_ctrl[socket_num].receive_flg              = CELLULAR_RECEIVE_FLAG_OFF;
        memset(&p_ctrl->p_socket_ctrl[socket_num].ip_addr, 0x00, sizeof(st_cellular_ipaddr_t));

        ret = socket_num + CELLULAR_START_SOCKET_NUMBER;
    }
    else
    {
        ret = (int32_t)atc_ret; //cast
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_socket_cfg
 *********************************************************************************************************************/
