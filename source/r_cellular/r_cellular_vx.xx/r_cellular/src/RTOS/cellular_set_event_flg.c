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
 * File Name    : cellular_set_event_flg.c
 * Description  : Function to set event flags using RTOS features.
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

/****************************************************************************
 * Function Name  @fn            cellular_set_event_flg
 ***************************************************************************/
e_cellular_err_t cellular_set_event_flg(void * const xEventGroup, const uint32_t uxBitsToSet,
                                        int32_t * pxHigherPriorityTaskWoken)
{
    e_cellular_err_t ret = CELLULAR_ERR_EVENT_GROUP_INIT;
#if BSP_CFG_RTOS_USED == (1)
    BaseType_t rtos_ret;
    rtos_ret = xEventGroupSetBitsFromISR((EventGroupHandle_t)xEventGroup,
                                            (EventBits_t)uxBitsToSet,
                                            (BaseType_t *)pxHigherPriorityTaskWoken);
    if (pdFAIL != rtos_ret)
    {
        ret = CELLULAR_SUCCESS;
    }
#elif BSP_CFG_RTOS_USED == (5)
    (void *)pxHigherPriorityTaskWoken;
    UINT rtos_ret;

    rtos_ret = tx_event_flags_set((TX_EVENT_FLAGS_GROUP *)xEventGroup,
                                    (ULONG)uxBitsToSet,
                                    (UINT)TX_OR);

    if (TX_SUCCESS == rtos_ret)
    {
        ret = CELLULAR_SUCCESS;
    }
#endif
    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_set_event_flg
 *********************************************************************************************************************/
