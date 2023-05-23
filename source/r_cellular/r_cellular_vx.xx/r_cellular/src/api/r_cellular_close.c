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
 * File Name    : r_cellular_close.c
 * Description  : Function to terminate communication with the module.
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
    uint8_t          cnt        = 0;
    uint32_t         preemption = 0;
    e_cellular_err_t ret        = CELLULAR_SUCCESS;

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
        p_ctrl->ring_ctrl.psm = CELLULAR_PSM_DEACTIVE;

        cellular_irq_close(p_ctrl);

        if (NULL != p_ctrl->ring_ctrl.ring_event)
        {
            cellular_delete_event_group(p_ctrl->ring_ctrl.ring_event);
            p_ctrl->ring_ctrl.ring_event = NULL;
        }
        if (NULL != p_ctrl->ring_ctrl.rts_semaphore)
        {
            cellular_delete_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
            p_ctrl->ring_ctrl.rts_semaphore = NULL;
        }
        if (NULL != p_ctrl->ring_ctrl.ring_taskhandle)
        {
            cellular_delete_task(p_ctrl->ring_ctrl.ring_taskhandle);
            p_ctrl->ring_ctrl.ring_taskhandle = NULL;
        }

#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_enable();
#else
        cellular_rts_ctrl(0);
#endif
#ifdef CELLULAR_RTS_DELAY
        cellular_delay_task(CELLULAR_RTS_DELAYTIME);
#endif

        ret = cellular_module_reset(p_ctrl);

        if (CELLULAR_SUCCESS == ret)
        {
            ret = cellular_power_down(p_ctrl);
        }

        cellular_delete_event_group(p_ctrl->eventgroup);
        p_ctrl->eventgroup = NULL;

        cellular_delete_task(p_ctrl->recv_taskhandle);
        p_ctrl->recv_taskhandle = NULL;

        cellular_delete_semaphore(p_ctrl->at_semaphore);
        p_ctrl->at_semaphore = NULL;

        for (cnt = 0; cnt < p_ctrl->creatable_socket; cnt++)
        {
            cellular_delete_semaphore(p_ctrl->p_socket_ctrl[cnt].rx_semaphore);
            p_ctrl->p_socket_ctrl[cnt].rx_semaphore = NULL;
        }

        cellular_free(p_ctrl->p_socket_ctrl);
        p_ctrl->p_socket_ctrl = NULL;

        cellular_serial_close(p_ctrl);

#if BSP_CFG_RTOS_USED == (5)
        cellular_block_pool_delete();
#endif

        preemption = cellular_interrupt_disable();

        p_ctrl->running_api_count -= 1;
        p_ctrl->system_state       = CELLULAR_SYSTEM_CLOSE;
        memset(p_ctrl, 0, sizeof(st_cellular_ctrl_t));

        cellular_interrupt_enable(preemption);
    }
    else
    {
        cellular_delay_task(1);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_Close
 *********************************************************************************************************************/
