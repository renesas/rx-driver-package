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
 * File Name    : cellular_irq_ctrl.c
 * Description  : Function for managing the external terminal interrupt function.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "r_irq_rx_pinset.h"
#ifndef R_IRQ_RX_H
#error "Please add IRQ Pin setting in Smart Configurator."
#endif

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CELLULAR_RING_EVENT (0x01)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static void cellular_ring_callback (void * const p_Args);

/**********************************************************************************************
 * Function Name  @fn            cellular_irq_open
 *********************************************************************************************/
e_cellular_err_t cellular_irq_open(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t    ret = CELLULAR_SUCCESS;
    irq_err_t           irq_ret = IRQ_SUCCESS;

    irq_ret = R_IRQ_Open((irq_number_t)CELLULAR_CFG_IRQ_NUM,    //cast
                            IRQ_TRIG_FALLING,
                            IRQ_PRI_1,
                            &p_ctrl->ring_ctrl.ring_irqhandle,
                            cellular_ring_callback);

    if (IRQ_SUCCESS != irq_ret)
    {
        ret = CELLULAR_ERR_IRQ_OPEN;
    }
    else
    {
        R_ICU_PinSet();
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_irq_open
 *********************************************************************************************************************/

/**********************************************************************************************
 * Function Name  @fn            cellular_irq_close
 *********************************************************************************************/
void cellular_irq_close(st_cellular_ctrl_t * const p_ctrl)
{
    R_IRQ_Close(p_ctrl->ring_ctrl.ring_irqhandle);
    return;
}
/**********************************************************************************************************************
 * End of function cellular_irq_close
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_ring_callback
 * Description    @details       Callback function for RING line.
 **********************************************************************************************/
static void cellular_ring_callback(void * const p_Args)
{
#if BSP_CFG_RTOS_USED == (1)
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    int32_t xHigherPriorityTaskWoken = 0;

    ret = cellular_set_event_flg(gp_cellular_ctrl->ring_ctrl.ring_event,
                                    CELLULAR_RING_EVENT,
                                    &xHigherPriorityTaskWoken);

    if (CELLULAR_SUCCESS == ret)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
#elif BSP_CFG_RTOS_USED == (5)
    cellular_set_event_flg(gp_cellular_ctrl->ring_ctrl.ring_event,
                            CELLULAR_RING_EVENT,
                            NULL);
#endif
}
/**********************************************************************************************************************
 * End of function cellular_ring_callback
 *********************************************************************************************************************/

/**************************************************************************************************************
 * Function Name  @fn            cellular_ring_task
 * Description    @details       Control of the RTS terminal in accordance with the operation of the RING line.
 * Arguments      @param[in/out] p_pvParameters -
 *                                  Pointer to the parameter given at the time of task creation.
 *************************************************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
void cellular_ring_task(void * const p_pvParameters)
#elif BSP_CFG_RTOS_USED == (5)
void cellular_ring_task(ULONG p_pvParameters)
#endif
{
#if BSP_CFG_RTOS_USED == (1)
    st_cellular_ctrl_t * const p_ctrl = p_pvParameters;
#elif BSP_CFG_RTOS_USED == (5)
    st_cellular_ctrl_t * const p_ctrl = gp_cellular_ctrl;
#endif
    st_cellular_time_ctrl_t * const p_timeout_ctrl = &p_ctrl->ring_ctrl.timeout;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;
    e_cellular_timeout_check_t timeout = CELLULAR_NOT_TIMEOUT;

    while (1)
    {
        cellular_get_event_flg(p_ctrl->ring_ctrl.ring_event,
                                CELLULAR_RING_EVENT,
                                CELLULAR_TIME_OUT_MAX_DELAY);

        cellular_timeout_init(p_timeout_ctrl, CELLULAR_CFG_RING_LINE_ACTIVE_TIME);
#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_enable();
#else
        cellular_rts_ctrl(0);
#endif

        while (1)
        {
            timeout = cellular_check_timeout(p_timeout_ctrl);
            if (CELLULAR_TIMEOUT == timeout)
            {
                break;
            }
        }

        while (1)
        {
            semaphore_ret = cellular_take_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
            if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
            {
#if CELLULAR_CFG_CTS_SW_CTRL == 1
                cellular_rts_hw_flow_disable();
#endif
                cellular_rts_ctrl(1);
                cellular_give_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
                break;
            }
            else
            {
                cellular_delay_task(1);
            }
        }
    }
}
/**********************************************************************************************************************
 * End of function cellular_ring_task
 *********************************************************************************************************************/
