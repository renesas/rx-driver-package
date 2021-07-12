/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_ble_rtos.c
 * Description  : BLE Task functions for FreeRTOS. 
 **********************************************************************************************************************/

/***********************************************************************************************************************
  Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "r_ble_rtos.h"

#if (BSP_CFG_RTOS_USED == 1)

#include "FreeRTOS.h"
#include "task.h"

/***********************************************************************************************************************
Export global variables
***********************************************************************************************************************/
TaskHandle_t g_ble_task_hdl;

/***********************************************************************************************************************
* Function Name: R_BLE_RTOS_WakeTask
* Description  : Wake up the ble task from other task.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_BLE_RTOS_WakeTask(void)
{
    xTaskNotifyGive(g_ble_task_hdl);
    taskYIELD();
}

/***********************************************************************************************************************
* Function Name: R_BLE_RTOS_WakeTaskFromIsr
* Description  : Wake up the ble task from ISR.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void R_BLE_RTOS_WakeTaskFromIsr(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;
    if (g_ble_task_hdl != NULL)
    {
        vTaskNotifyGiveFromISR(g_ble_task_hdl, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

#endif /* (BSP_CFG_RTOS_USED == 1) */
