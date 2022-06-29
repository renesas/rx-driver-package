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
 * File Name    : r_cellular_close.c
 * Description  : Function to terminate communication with the module.
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

/***************************************************************************************
 * Function Name  @fn            R_CELLULAR_Close
 **************************************************************************************/
e_cellular_err_t R_CELLULAR_Close(st_cellular_ctrl_t * const p_ctrl)
{
    uint8_t i;
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    if (NULL == p_ctrl)
    {
        ret = CELLULAR_ERR_PARAMETER;
    }

    if (CELLULAR_SUCCESS == ret)
    {
        for (i = CELLULAR_START_SOCKET_NUMBER; i <= p_ctrl->creatable_socket; i++ )
        {
            cellular_closesocket(p_ctrl, i);
        }

        if (CELLULAR_SYSTEM_CONNECT == p_ctrl->system_state)
        {
            cellular_disconnect(p_ctrl);
        }

        cellular_delete_event_group(p_ctrl->eventgroup);
        p_ctrl->eventgroup = NULL;

        cellular_delete_task(p_ctrl->recv_taskhandle);
        p_ctrl->recv_taskhandle = NULL;

        cellular_delete_semaphore(p_ctrl->at_semaphore);
        p_ctrl->at_semaphore = NULL;

        for (i = 0; i < p_ctrl->creatable_socket; i++ )
        {
            cellular_delete_semaphore(p_ctrl->p_socket_ctrl[i].rx_semaphore);
            p_ctrl->p_socket_ctrl[i].rx_semaphore = NULL;
        }

        cellular_free(p_ctrl->p_socket_ctrl);
        p_ctrl->p_socket_ctrl = NULL;

        cellular_serial_close(p_ctrl);

#if BSP_CFG_RTOS_USED == (5)
        cellular_block_pool_delete();
#endif

        memset(p_ctrl, 0, sizeof(st_cellular_ctrl_t));

        ret = CELLULAR_SUCCESS;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_Close
 *********************************************************************************************************************/
