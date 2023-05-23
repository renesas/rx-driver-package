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
 * File Name    : cellular_interrupt_ctrl.c
 * Description  : Functions to enter/exit critical sections.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
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

/*****************************************************************************************
 * Function Name  @fn            cellular_interrupt_disable
 ****************************************************************************************/
uint32_t cellular_interrupt_disable(void)
{
#if BSP_CFG_RTOS_USED == (1)
    uint32_t ret = 0;

    taskENTER_CRITICAL();
#elif BSP_CFG_RTOS_USED == (5)
    UINT        ret = 0;
    UINT        rtos_ret = 0;
    UINT        old_threshold = 0;
    TX_THREAD * p_thread = NULL;

    p_thread = tx_thread_identify();
    ret = p_thread->tx_thread_preempt_threshold;

    rtos_ret = tx_thread_preemption_change(p_thread, 0, &old_threshold);
#endif
    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_interrupt_disable
 *********************************************************************************************************************/

/*****************************************************************************************
 * Function Name  @fn            cellular_interrupt_enable
 ****************************************************************************************/
void cellular_interrupt_enable(uint32_t preemption)
{
#if BSP_CFG_RTOS_USED == (1)
    taskEXIT_CRITICAL();
#elif BSP_CFG_RTOS_USED == (5)
    UINT        old_threshold = 0;
    TX_THREAD * p_thread = NULL;

    p_thread = tx_thread_identify();
    tx_thread_preemption_change(p_thread, (UINT)preemption, &old_threshold);
#endif
    return;
}
/**********************************************************************************************************************
 * End of function cellular_interrupt_enable
 *********************************************************************************************************************/
