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
 * File Name    : r_cellular_disconnect.c
 * Description  : Function to disconnect from the access point.
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

/*******************************************************************************
 * Function Name  @fn            R_CELLULAR_Disconnect
 ******************************************************************************/
e_cellular_err_t R_CELLULAR_Disconnect(st_cellular_ctrl_t * const p_ctrl)
{
    uint8_t          cnt;
    uint32_t         preemption = 0;
    e_cellular_err_t ret        = CELLULAR_SUCCESS;

    preemption = cellular_interrupt_disable();
    if (NULL == p_ctrl)
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
            p_ctrl->running_api_count += 2;
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
            ret = cellular_disconnect(p_ctrl);
        }
        p_ctrl->running_api_count -= 2;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_Disconnect
 *********************************************************************************************************************/
