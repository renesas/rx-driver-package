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
 * File Name    : cellular_create_task.c
 * Description  : Function to create a task using RTOS features.
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
#if BSP_CFG_RTOS_USED == (5)
st_cellular_ctrl_t * gp_cellular_ctrl;
#endif

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/****************************************************************************
 * Function Name  @fn            cellular_create_task
 ***************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
e_cellular_err_t
#elif BSP_CFG_RTOS_USED == (5)
void *
#endif
cellular_create_task(void *(pxTaskCode)(void *),
                        const char * const pcName,
                        const uint16_t usStackDepth,
                        void * const pvParameters,
                        const uint32_t uxPriority,
                        void * const pxCreatedTask)
{
#if BSP_CFG_RTOS_USED == (1)
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    int32_t rtos_ret;

    rtos_ret = xTaskCreate((TaskFunction_t)pxTaskCode,
                            pcName,
                            (configSTACK_DEPTH_TYPE)usStackDepth,
                            pvParameters,
                            tskIDLE_PRIORITY + (UBaseType_t)uxPriority,
                            (TaskHandle_t *)pxCreatedTask);

    if (pdPASS != rtos_ret)
    {
        ret = CELLULAR_ERR_CREATE_TASK;
    }

    return ret;
#elif BSP_CFG_RTOS_USED == (5)
    UINT tx_ret = 0;
    void * p_ret = NULL;

    p_ret = cellular_malloc(sizeof(TX_THREAD));
    if (NULL != p_ret)
    {
        tx_ret = tx_thread_create((TX_THREAD *)p_ret, (CHAR *)pcName,
                                    pxTaskCode, 0u,
                                    (usStackDepth  / sizeof(ULONG)), usStackDepth,
                                    uxPriority, uxPriority,
                                    TX_NO_TIME_SLICE, TX_AUTO_START);
        if(tx_ret != TX_SUCCESS)
        {
            cellular_free(p_ret);
        }
        else
        {
            gp_cellular_ctrl = (st_cellular_ctrl_t *)pvParameters;
        }
    }

    return p_ret;
#endif
}
/**********************************************************************************************************************
 End of function cellular_create_task
 *********************************************************************************************************************/
