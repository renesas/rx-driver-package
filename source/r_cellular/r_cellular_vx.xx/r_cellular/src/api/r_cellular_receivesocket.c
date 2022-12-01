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
 * File Name    : r_cellular_receivesocket.c
 * Description  : Functions to read data from a socket.
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
static int32_t cellular_receive_data (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                        uint8_t * const p_data, const int32_t length, const uint32_t timeout_ms);
static e_cellular_timeout_check_t cellular_receive_flag_check (st_cellular_ctrl_t * const p_ctrl,
                                        st_cellular_time_ctrl_t * const p_cellular_timeout_ctrl,
                                        const uint8_t socket_no);
static uint32_t cellular_recv_size_check (st_cellular_ctrl_t * const p_ctrl,
                                            const uint8_t socket_no, const int32_t length);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_ReceiveSocket
 * Description    @details       Reading data from a socket.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for reading data.
 *                @param[in]     p_data -
 *                                  Pointer to store the read data.
 *                @param[in]     length -
 *                                  Read data size.
 *                @param[in]     timeout_ms -
 *                                  timeout millisecond.
 * Return Value   @retval        More than 1 -
 *                                  Successfully loaded data.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
int32_t R_CELLULAR_ReceiveSocket(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                        uint8_t * const p_data, const int32_t length, const uint32_t timeout_ms)
{
    uint32_t preemption = 0;
    int32_t total_receive_length = 0;
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_data) || (0 >= length) || (NULL == p_ctrl))
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
            if ((CELLULAR_START_SOCKET_NUMBER > socket_no) || (p_ctrl->creatable_socket < socket_no))
            {
                ret = CELLULAR_ERR_PARAMETER;
            }
            else if (CELLULAR_SOCKET_STATUS_CONNECTED !=
                        p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].socket_status)
            {
                ret = CELLULAR_ERR_SOCKET_NOT_READY;
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
        semaphore_ret = cellular_take_semaphore(p_ctrl->p_socket_ctrl[socket_no
                                                    - CELLULAR_START_SOCKET_NUMBER].rx_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS != semaphore_ret)
        {
            total_receive_length = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
        else
        {
            total_receive_length = cellular_receive_data(p_ctrl, socket_no, p_data, length, timeout_ms);
            cellular_give_semaphore(p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].rx_semaphore);
        }

        p_ctrl->running_api_count -= 2;
    }
    else
    {
        total_receive_length = ret;
    }

    return total_receive_length;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_ReceiveSocket
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_receive_data
 * Description    @details       Reading data from a socket.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for reading data.
 *                @param[in]     p_data -
 *                                  Pointer to store the read data.
 *                @param[in]     length -
 *                                  Read data size.
 *                @param[in]     timeout_ms -
 *                                  timeout millisecond.
 * Return Value   @retval        More than 1 -
 *                                  Successfully loaded data.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
static int32_t cellular_receive_data(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                        uint8_t * const p_data, const int32_t length, const uint32_t timeout_ms)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;
    e_cellular_timeout_check_t timeout = CELLULAR_NOT_TIMEOUT;
    int32_t total_receive_length = 0;
    uint32_t receive_size;

    st_cellular_time_ctrl_t * const p_cellular_timeout_ctrl
        = &p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].cellular_rx_timeout_ctrl;

    cellular_timeout_init(p_cellular_timeout_ctrl, timeout_ms);

    if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
    {
        while (1)
        {
            semaphore_ret = cellular_take_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
            if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
            {
                break;
            }
            cellular_delay_task(1);
        }
#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_enable();
#else
        cellular_rts_ctrl(0);
#endif
    }

    /* Data receive loop */
    while (length > total_receive_length)
    {
        timeout = cellular_receive_flag_check(p_ctrl, p_cellular_timeout_ctrl, socket_no);
        if (CELLULAR_TIMEOUT == timeout)
        {
            break; /* Break of the data receive loop */
        }

        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS != semaphore_ret)
        {
            break; /* Break of the data receive loop */
        }
        else
        {
            receive_size = cellular_recv_size_check(p_ctrl, socket_no,
                        length - p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].total_recv_count);

            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].p_recv = p_data;

            ret = atc_sqnsrecv(p_ctrl, socket_no, receive_size);
            if (CELLULAR_SUCCESS == ret)
            {
                total_receive_length = p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].total_recv_count;

                timeout = cellular_check_timeout(p_cellular_timeout_ctrl);
                if (CELLULAR_TIMEOUT == timeout)
                {
                    goto cellular_receive_data_fail;
                }
            }
            else
            {
                goto cellular_receive_data_fail;
            }
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
    } /* End of data receive loop */

    if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
    {
        cellular_give_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_disable();
#endif
        cellular_rts_ctrl(1);
    }

    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].total_recv_count = 0;
    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].p_recv = NULL;

    return total_receive_length;

cellular_receive_data_fail:
    {
        if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
        {
            cellular_give_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
#if CELLULAR_CFG_CTS_SW_CTRL == 1
            cellular_rts_hw_flow_disable();
#endif
            cellular_rts_ctrl(1);
        }

        cellular_give_semaphore(p_ctrl->at_semaphore);
        p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].total_recv_count = 0;
        p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].p_recv = NULL;
        return total_receive_length;
    }
}
/**********************************************************************************************************************
 * End of function  cellular_receive_data
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_receive_flag_check
 * Description    @details       Wait for the AT command response.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for sending data.
 * Return Value   @retval        CELLULAR_NOT_TIMEOUT -
 *                                  Successfully flag check.
 *                @retval        CELLULAR_TIMEOUT -
 *                                  Time out.
 ************************************************************************************************/
static e_cellular_timeout_check_t cellular_receive_flag_check(st_cellular_ctrl_t * const p_ctrl,
                                        st_cellular_time_ctrl_t * const p_cellular_timeout_ctrl,
                                        const uint8_t socket_no)
{
    e_cellular_timeout_check_t  timeout = CELLULAR_NOT_TIMEOUT;

    while (1)
    {
        if (CELLULAR_RECEIVE_FLAG_ON == p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_flg)
        {
            break;
        }

        timeout = cellular_check_timeout(p_cellular_timeout_ctrl);
        if (CELLULAR_TIMEOUT == timeout)
        {
            break;
        }
    }

    return timeout;
}
/**********************************************************************************************************************
 * End of function cellular_receive_flag_check
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_recv_size_check
 * Description    @details       Check the size of the data to be receive.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for sending data.
 * Return Value   @retval        Number of bytes received from the module.
 ************************************************************************************************/
static uint32_t cellular_recv_size_check(st_cellular_ctrl_t * const p_ctrl,
                                            const uint8_t socket_no, const int32_t length)
{
    uint32_t receive_size = 0;

    if (p_ctrl->sci_ctrl.rx_buff_size > p_ctrl->sci_ctrl.rx_process_size)
    {
        receive_size = p_ctrl->sci_ctrl.rx_process_size;
    }
    else
    {
        receive_size = p_ctrl->sci_ctrl.rx_buff_size;
    }

    if (receive_size > p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_unprocessed_size)
    {
        receive_size = p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].receive_unprocessed_size;
    }

    if (receive_size > length)
    {
        receive_size = length;
    }

    return receive_size;
}
/**********************************************************************************************************************
 * End of function cellular_recv_size_check
 *********************************************************************************************************************/
