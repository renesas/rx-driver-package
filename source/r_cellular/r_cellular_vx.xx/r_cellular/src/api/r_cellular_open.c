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
 * File Name    : r_cellular_open.c
 * Description  : Function to establish the communication state with the module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>

#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "at_command.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define PHASE_1     (0x01u << 0)
#define PHASE_2     (0x01u << 1)
#define PHASE_3     (0x01u << 2)
#define PHASE_4     (0x01u << 3)
#define PHASE_5     (0x01u << 4)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
st_cellular_ctrl_t * gp_cellular_ctrl = NULL;

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static e_cellular_err_t cellular_init (st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg);
static e_cellular_err_t cellular_config_init (st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg);
static void             cellular_open_fail (st_cellular_ctrl_t * const p_ctrl, const uint8_t open_phase);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_Open
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_Open(st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg)
{
    uint8_t          open_phase = 0;
    uint32_t         preemption = 0;
    e_cellular_err_t ret        = CELLULAR_SUCCESS;

    CELLULAR_LOG_INFO(("Called: R_CELLULAR_Open()"));

    preemption = cellular_interrupt_disable();
    if (NULL == p_ctrl)
    {
        ret = CELLULAR_ERR_PARAMETER;
        cellular_interrupt_enable(preemption);
        goto R_CELLULAR_Open_error;
    }
    else
    {
        if (0 < (p_ctrl->running_api_count))
        {
            ret = CELLULAR_ERR_OTHER_API_RUNNING;
            cellular_interrupt_enable(preemption);
            goto R_CELLULAR_Open_error;
        }
        else if (CELLULAR_SYSTEM_CLOSE != p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_ALREADY_OPEN;
            cellular_interrupt_enable(preemption);
            goto R_CELLULAR_Open_error;
        }
        else
        {
            memset(p_ctrl, 0, sizeof(st_cellular_ctrl_t));
            p_ctrl->running_api_count += 1;
        }
    }
    cellular_interrupt_enable(preemption);

    gp_cellular_ctrl = p_ctrl;

#if BSP_CFG_RTOS_USED == (5)
    ret = cellular_block_pool_create();
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }
    open_phase |= PHASE_1;
#endif

    ret = cellular_config_init(p_ctrl, p_cfg);
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }
    open_phase |= PHASE_2;

    ret = cellular_serial_open(p_ctrl);
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }
    open_phase |= PHASE_3;

    ret = cellular_semaphore_init(p_ctrl);
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }
    open_phase |= PHASE_4;

    ret = cellular_start_recv_task(p_ctrl);
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }
    open_phase |= PHASE_5;

    if ((CELLULAR_MAIN_TASK_BIT | CELLULAR_RECV_TASK_BIT) !=
            cellular_synchro_event_group(p_ctrl->eventgroup, CELLULAR_MAIN_TASK_BIT,
                (CELLULAR_MAIN_TASK_BIT | CELLULAR_RECV_TASK_BIT), CELLULAR_TIME_WAIT_TASK_START))
    {
        ret = CELLULAR_ERR_CREATE_TASK;
        goto R_CELLULAR_Open_fail;
    }

    ret = cellular_module_reset(p_ctrl);
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }

    ret = cellular_init(p_ctrl, p_cfg);
    if (CELLULAR_SUCCESS != ret)
    {
        goto R_CELLULAR_Open_fail;
    }

    CELLULAR_LOG_INFO(("Return from: R_CELLULAR_Open() with return code = %d.", ret));

    p_ctrl->running_api_count -= 1;

    return ret;

R_CELLULAR_Open_fail:
    {
        cellular_open_fail(p_ctrl, open_phase);
    }

R_CELLULAR_Open_error:
    {
        CELLULAR_LOG_ERROR(("Error: R_CELLULAR_Open()."));
        CELLULAR_LOG_INFO(("Return from: R_CELLULAR_Open() with return code = %d.", ret));
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_Open
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_config_init
 ***********************************************************************/
static e_cellular_err_t cellular_config_init(st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg)
{
    uint8_t          cnt;
    void *           malloc_ret = NULL;
    e_cellular_err_t ret        = CELLULAR_SUCCESS;

    if (NULL == p_cfg)
    {
        p_ctrl->creatable_socket = CELLULAR_CREATABLE_SOCKETS;

        malloc_ret = cellular_malloc((size_t)p_ctrl->creatable_socket * sizeof(st_cellular_socket_ctrl_t)); // -> long
        if (NULL != malloc_ret)
        {
            memset(malloc_ret, 0x00, (size_t)p_ctrl->creatable_socket * sizeof(st_cellular_socket_ctrl_t)); // -> long
            p_ctrl->p_socket_ctrl        = malloc_ret;
            p_ctrl->ap_connect_retry     = CELLULAR_CFG_ATC_RETRY_CGATT;
            p_ctrl->sci_ctrl.baud_rate   = CELLULAR_BAUDRATE;
            p_ctrl->sci_ctrl.atc_timeout = CELLULAR_COMMAND_TIMEOUT;

            for (cnt = 0; cnt < p_ctrl->creatable_socket; cnt++)
            {
                p_ctrl->p_socket_ctrl[cnt].send_timeout     = CELLULAR_TX_TIMEOUT;
                p_ctrl->p_socket_ctrl[cnt].exchange_timeout = CELLULAR_CFG_EX_TIMEOUT;
                p_ctrl->p_socket_ctrl[cnt].connect_timeout  = CELLULAR_CONNECT_TIMEOUT;
                p_ctrl->p_socket_ctrl[cnt].packet_data_size = CELLULAR_PACKET_DATA_SIZE;
            }
            p_ctrl->sci_ctrl.rx_process_size = CELLULAR_MAX_RX_SIZE;
            p_ctrl->sci_ctrl.tx_process_size = CELLULAR_MAX_TX_SIZE;
        }
        else
        {
            ret = CELLULAR_ERR_MEMORY_ALLOCATION;
        }
    }
    else
    {
        if ((CELLULAR_MAX_SIM_PASS_LENGTH < strlen((const char *)p_cfg->sim_pin_code)) || // (&uint8_t[])->(char *)
            (CELLULAR_START_SOCKET_NUMBER > p_cfg->creatable_socket) ||
            (CELLULAR_CREATABLE_SOCKETS < p_cfg->creatable_socket) ||
            (CELLULAR_BAUDRATE != p_cfg->baud_rate) ||
            (CELLULAR_CONNECT_TIMEOUT_LIMIT_L > p_cfg->connect_timeout) ||
            (CELLULAR_CONNECT_TIMEOUT_LIMIT_H < p_cfg->connect_timeout) ||
            (CELLULAR_PACKET_DATA_SIZE_LIMIT < p_cfg->packet_data_size) ||
            (CELLULAR_TX_SIZE_LIMIT_L > p_cfg->tx_process_size) ||
            (CELLULAR_TX_SIZE_LIMIT_H < p_cfg->tx_process_size) ||
            (CELLULAR_RX_SIZE_LIMIT_L > p_cfg->rx_process_size) ||
            (CELLULAR_RX_SIZE_LIMIT_H < p_cfg->rx_process_size))
        {
            ret = CELLULAR_ERR_PARAMETER;
        }
        else
        {
            p_ctrl->creatable_socket = p_cfg->creatable_socket;

            malloc_ret = cellular_malloc((size_t)p_ctrl->creatable_socket * sizeof(st_cellular_socket_ctrl_t)); // long
            if (NULL != malloc_ret)
            {
                memset(malloc_ret, 0x00, (size_t)p_ctrl->creatable_socket * sizeof(st_cellular_socket_ctrl_t)); // long
                p_ctrl->p_socket_ctrl        = malloc_ret;
                p_ctrl->ap_connect_retry     = p_cfg->ap_cgatt_retry_count;
                p_ctrl->sci_ctrl.baud_rate   = p_cfg->baud_rate;
                p_ctrl->sci_ctrl.atc_timeout = p_cfg->sci_timeout;

                for (cnt = 0; cnt < p_ctrl->creatable_socket; cnt++)
                {
                    p_ctrl->p_socket_ctrl[cnt].send_timeout     = p_cfg->send_timeout;
                    p_ctrl->p_socket_ctrl[cnt].exchange_timeout = p_cfg->exchange_timeout;
                    p_ctrl->p_socket_ctrl[cnt].connect_timeout  = p_cfg->connect_timeout;
                    p_ctrl->p_socket_ctrl[cnt].packet_data_size = p_cfg->packet_data_size;
                }
                p_ctrl->sci_ctrl.rx_process_size = p_cfg->rx_process_size;
                p_ctrl->sci_ctrl.tx_process_size = p_cfg->tx_process_size;
            }
            else
            {
                ret = CELLULAR_ERR_MEMORY_ALLOCATION;
            }
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_config_init
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_init
 ***********************************************************************/
static e_cellular_err_t cellular_init(st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg)
{
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;

    semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
    {
        ret = atc_ate0(p_ctrl);
        if (CELLULAR_SUCCESS != ret)
        {
            CELLULAR_LOG_INFO(("First sending AT command to RYZ014A -> Failed."));
            goto R_CELLULAR_Open_fail;
        }

        CELLULAR_LOG_INFO(("First sending AT command to RYZ014A -> Succeeded."));

        ret = atc_sqnsimst(p_ctrl);
        if (CELLULAR_SUCCESS != ret)
        {
            goto R_CELLULAR_Open_fail;
        }

        ret = atc_cereg(p_ctrl, (e_cellular_network_result_t)CELLULAR_CFG_NETWORK_NOTIFY_LEVEL); //cast
        if (CELLULAR_SUCCESS != ret)
        {
            goto R_CELLULAR_Open_fail;
        }

        ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
        if (CELLULAR_SUCCESS != ret)
        {
            goto R_CELLULAR_Open_fail;
        }

        ret = atc_cpin_check(p_ctrl);
        if (CELLULAR_SUCCESS != ret)
        {
            ret = atc_cpin(p_ctrl, p_cfg);
            if (CELLULAR_SUCCESS != ret)
            {
                CELLULAR_LOG_ERROR(("PIN CODE ERROR."));
                goto R_CELLULAR_Open_fail;
            }
        }

        p_ctrl->system_state = CELLULAR_SYSTEM_OPEN;
    }
    else
    {
        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
    }

R_CELLULAR_Open_fail:
    {
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }

        return ret;
    }
}
/**********************************************************************************************************************
 * End of function cellular_init
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_open_fail
 ***********************************************************************/
static void cellular_open_fail(st_cellular_ctrl_t * const p_ctrl, const uint8_t open_phase)
{
    uint8_t cnt = 0;

    if ((open_phase & PHASE_5) == PHASE_5)
    {
        cellular_delete_event_group(p_ctrl->eventgroup);
        p_ctrl->eventgroup = NULL;

        cellular_delete_task(p_ctrl->recv_taskhandle);
        p_ctrl->recv_taskhandle = NULL;
    }

    if ((open_phase & PHASE_4) == PHASE_4)
    {
        cellular_delete_semaphore(p_ctrl->at_semaphore);
        p_ctrl->at_semaphore = NULL;

        for (cnt = 0; cnt < p_ctrl->creatable_socket; cnt++)
        {
            cellular_delete_semaphore(p_ctrl->p_socket_ctrl[cnt].rx_semaphore);
            p_ctrl->p_socket_ctrl[cnt].rx_semaphore = NULL;
        }
    }

    if ((open_phase & PHASE_3) == PHASE_3)
    {
        cellular_serial_close(p_ctrl);
    }

    if ((open_phase & PHASE_2) == PHASE_2)
    {
        cellular_free(p_ctrl->p_socket_ctrl);
        p_ctrl->p_socket_ctrl = NULL;
    }

    if (NULL != p_ctrl)
    {
        /* p_ctrl->running_api_count is set "0" and
         * p_ctrl->system_state is "0"(CELLULAR_SYSTEM_CLOSE)
         * by the below memset 0 operation. */
        memset(p_ctrl, 0, sizeof(st_cellular_ctrl_t));
    }

#if BSP_CFG_RTOS_USED == (5)
    if ((open_phase & PHASE_1) == PHASE_1)
    {
        cellular_block_pool_delete();
    }
#endif
}
/**********************************************************************************************************************
 * End of function cellular_open_fail
 *********************************************************************************************************************/
