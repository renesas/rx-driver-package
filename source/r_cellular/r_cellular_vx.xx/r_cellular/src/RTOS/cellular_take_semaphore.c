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
 * File Name    : cellular_take_semaphore.c
 * Description  : Function to get semaphore.
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
 * Function Name  @fn            cellular_take_semaphore
 ****************************************************************************************/
e_cellular_err_semaphore_t cellular_take_semaphore(void * const xSemaphore)
{
    e_cellular_err_semaphore_t ret = CELLULAR_SEMAPHORE_SUCCESS;
#if BSP_CFG_RTOS_USED == (1)
    if (pdTRUE != xSemaphoreTake((SemaphoreHandle_t)xSemaphore, pdMS_TO_TICKS(CELLULAR_CFG_SEMAPHORE_BLOCK_TIME)))
    {
        ret = CELLULAR_SEMAPHORE_ERR_TAKE;
    }
#elif BSP_CFG_RTOS_USED == (5)
    if (TX_SUCCESS != tx_semaphore_get((TX_SEMAPHORE *)xSemaphore, MS_TO_TICKS(CELLULAR_CFG_SEMAPHORE_BLOCK_TIME)))
    {
        ret = CELLULAR_SEMAPHORE_ERR_TAKE;
    }
#endif

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_take_semaphore
 *********************************************************************************************************************/
