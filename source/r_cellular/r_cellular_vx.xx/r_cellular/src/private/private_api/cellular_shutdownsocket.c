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
 * File Name    : cellular_shutdownsocket.c
 * Description  : Function to disconnect a socket connection.
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

/************************************************************************
 * Function Name  @fn            cellular_shutdownsocket
 * Description    @details       Disconnect the socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to close socket.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected the socket connection.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t cellular_shutdownsocket(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no)
{
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if (CELLULAR_SOCKET_STATUS_CONNECTED ==
            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].socket_status)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            ret = atc_sqnsh(p_ctrl, socket_no);
            if (CELLULAR_SUCCESS == ret)
            {
                p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].socket_status
                    = CELLULAR_SOCKET_STATUS_SOCKET;
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
 End of function cellular_shutdownsocket
 *********************************************************************************************************************/
