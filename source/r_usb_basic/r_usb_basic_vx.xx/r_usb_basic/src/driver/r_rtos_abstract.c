/**********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.    
 *********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_rtos_abstract.c
* Version      : 1.0
* Description  : RTOS related processing
 *********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.03.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_rtos_abstract.h"

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private global variables and functions
 *********************************************************************************************************************/


#if (BSP_CFG_RTOS_USED != 4)         /* Excluding Renesas RI600V4 & RI600PX */
/**********************************************************************************************************************
 * Function Name: rtos_create_fixed_memory
 * Description  : Create fixed memory block
 * Arguments    : p_id      : Pointer to the area to store the created fixed-sized memory pool ID
 *              : p_info    : Pointer to rtos_mpf_info_t structure area
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t rtos_create_fixed_memory(rtos_mem_id_t *p_id, rtos_mpf_info_t *p_info)
{
    void   *p_memblk;
    uint16_t    block_number;
    uint16_t    block_size;

#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t  err;
    uint8_t     *p_start;
    uint16_t    i;

    p_start             = (uint8_t *)p_info->p_mempool;
    block_size          = p_info->block_size;
    block_number        = p_info->block_number;

    *p_id   = xQueueCreate((UBaseType_t)block_number, sizeof(void *));
    for (i = 0; i < block_number; i++)
    {
        p_memblk = (void *)(p_start + (i * block_size));
        err = xQueueSend(*p_id, (const void *)&p_memblk, RTOS_ZERO);
        if (pdPASS != err)
        {
            return RTOS_ERROR;
        }
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#else
#endif
    return RTOS_SUCCESS;
} /* End of function rtos_create_fixed_memory() */

/**********************************************************************************************************************
 * Function Name: rtos_delete_fixed_memory
 * Description  : Delete memory block.
 * Arguments    : p_id      : Pointer to the area to store the deleted fixed-sized memory pool ID
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t rtos_delete_fixed_memory(rtos_mem_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_delete_fixed_memory() */
#endif /* (BSP_CFG_RTOS_USED != 4) */

/**********************************************************************************************************************
 * Function Name: rtos_get_fixed_memory
 * Description  : Get the the fixed-memory block
 * Arguments    : p_id          : Pointer to the area to store the fixed-sized memory pool ID.
 *              : pp_memblock   : Pointer to the area to store the start address of the allocated memory block
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t rtos_get_fixed_memory(rtos_mem_id_t *p_id, void **pp_memblk, rtos_time_t tmo_val)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t  err;
    BaseType_t  xHigherPriorityTaskWoken = pdFALSE;
    err = xQueueReceive(*p_id, (void *)pp_memblk, tmo_val);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
    portYIELD_FROM_ISR (xHigherPriorityTaskWoken);

#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER      err;
    err = tget_mpf(*p_id, (VP *)pp_memblk, tmo_val);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
} /* End of function rtos_get_fixed_memory() */

/**********************************************************************************************************************
 * Function Name: rtos_get_fixed_memory_isr
 * Description  : Get the start address of the fixed-memory block for ISR
 * Arguments    : p_id          : Pointer to the area to store the fixed-sized memory pool ID.
 *              : pp_memblock   : Pointer to the area to store the start address of the allocated memory block
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t rtos_get_fixed_memory_isr(rtos_mem_id_t *p_id, void **pp_memblk)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t  err;
    BaseType_t  xHigherPriorityTaskWoken = pdFALSE;

    err = xQueueReceiveFromISR (*p_id, (void *)pp_memblk, &xHigherPriorityTaskWoken);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
    portYIELD_FROM_ISR ( xHigherPriorityTaskWoken );
    return RTOS_SUCCESS;
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER          err;
    err = pget_mpf (*p_id, (VP *)pp_memblk);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
} /* End of function rtos_get_fixed_memory_isr() */

/**********************************************************************************************************************
 * Function Name: rtos_fixed_release_memory
 * Description  : Release the fixed-memory block
 * Arguments    : p_id          : Pointer to the area to store the fixed-sized memory pool ID.
 *              : p_memblock    : Pointer to the area to store the start address of the released memory block
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t rtos_release_fixed_memory(rtos_mem_id_t *p_id, void *p_memblk)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t      err;
    err = xQueueSend(*p_id, (const void *)&p_memblk, (TickType_t)RTOS_ZERO);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER  err;
    err = rel_mpf(*p_id, (VP)p_memblk);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
} /* End of function rtos_release_fixed_memory() */

#if (BSP_CFG_RTOS_USED != 4)         /* Excluding Renesas RI600V4 & RI600PX */
/**********************************************************************************************************************
 * Function Name: rtos_create_task
 * Description  : Create the task
 * Arguments    : p_id          : Pointer to the area to store the created task ID.
 *              : p_info        : Pointer to rtos_task_info_t structure area
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_create_task (rtos_task_id_t *p_id, rtos_task_info_t *p_info)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t  err;
    err = xTaskCreate(p_info->task_code,    /** Entry function of HCD task  **/
                    (const char *)p_info->p_name,
                    p_info->stack_depth,
                    p_info->p_parameter,
                    p_info->priority,
                    p_id);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
    return RTOS_SUCCESS;
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_create_task() */

/**********************************************************************************************************************
 * Function Name: rtos_delete_task
 * Description  : Delete the task
 * Arguments    : p_id          : Pointer to the area to store the deleted task ID.
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_delete_task (rtos_task_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    vTaskDelete(*p_id);
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_delete_task() */
#endif /* (BSP_CFG_RTOS_USED != 4) */

/**********************************************************************************************************************
 * Function Name: rtos_start_task
 * Description  : Start the task
 * Arguments    : p_id          : Pointer to the area to store the starat task ID.
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_start_task (rtos_task_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER  err;
    err = act_tsk(*p_id);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_start_task() */

/**********************************************************************************************************************
 * Function Name: rtos_get_task_id
 * Description  : Get the current task id
 * Arguments    : p_id          : Pointer to the area to store the current task ID.
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_get_task_id (rtos_task_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    *p_id = xTaskGetCurrentTaskHandle();
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER  err;
    err = get_tid (p_id);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_get_task_id() */

#if (BSP_CFG_RTOS_USED != 4)         /* Excluding Renesas RI600V4 & RI600PX */
/**********************************************************************************************************************
 * Function Name: rtos_create_mailbox
 * Description  : Create the mailbox
 * Arguments    : p_id      : Pointer to the area to store the created mailbox ID.
 *              : p_info    : Pointer to rtos_mbx_info_t structure area
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_create_mailbox (rtos_mbx_id_t *p_id, rtos_mbx_info_t *p_info)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    *p_id = xQueueCreate(p_info->length, sizeof(void *));
    if (NULL == *p_id)
    {
        return RTOS_ERROR;
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_create_mailbox() */

/**********************************************************************************************************************
 * Function Name: rtos_delete_mailbox
 * Description  : Delete the mailbox
 * Arguments    : p_id      : Pointer to the area to store the deleted mailbox ID.
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_delete_mailbox (rtos_mbx_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    vQueueDelete(*p_id);
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_delete_mailbox() */
#endif /* (BSP_CFG_RTOS_USED != 4) */

/**********************************************************************************************************************
 * Function Name: rtos_send_mailbox
 * Description  : Send the message to the mailbox
 * Arguments    : p_id          : Pointer to the area to store the mailbox ID.
 *              : p_message     : Sent message
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_send_mailbox (rtos_mbx_id_t *p_id, void *p_message)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t  err;
    err = xQueueSend(*p_id, (const void *)&p_message, RTOS_ZERO);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER      err;
    err = snd_mbx(*p_id, (T_MSG *)p_message);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_send_mailbox() */

/**********************************************************************************************************************
 * Function Name: rtos_send_mailbox_isr
 * Description  : Send the message to the mailbox for ISR
 * Arguments    : p_id          : Pointer to the area to store the mailbox ID.
 *              : p_message     : Sent message
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_send_mailbox_isr (rtos_mbx_id_t *p_id, void *p_message)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t      err;
    BaseType_t      xHigherPriorityTaskWoken = pdFALSE;
    err = xQueueSendFromISR(*p_id, (const void *)&p_message, &xHigherPriorityTaskWoken);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
    portYIELD_FROM_ISR (xHigherPriorityTaskWoken);
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER      err;
    err = isnd_mbx(*p_id, (T_MSG *)p_message);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_send_mailbox_isr() */

/**********************************************************************************************************************
 * Function Name: rtos_receive_mailbox
 * Description  : Receive the message from the mailbox
 * Arguments    : p_id          : Pointer to the area to store the mailbox ID
 *              : pp_message    : Pointer to the area to store the received message
 *              : tmo_val       : Timeout value
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_receive_mailbox (rtos_mbx_id_t *p_id, void **pp_message, rtos_time_t tmo_val)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t  err;
    err = xQueueReceive(*p_id, (void *)pp_message, tmo_val);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER      err;
    err = trcv_mbx(*p_id, (T_MSG **)pp_message, tmo_val);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_receive_mailbox() */

/**********************************************************************************************************************
 * Function Name: rtos_create_semaphore
 * Description  : Create the semaphore
 * Arguments    : p_id      : Pointer to the area to store the created semaphore ID.
 *              : p_info    : Pointer to rtos_sem_info_t structure area
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_create_semaphore (rtos_sem_id_t *p_id, rtos_sem_info_t *p_info)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    vSemaphoreCreateBinary(*p_id);
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_create_semaphore() */

/**********************************************************************************************************************
 * Function Name: rtos_delete_semaphore
 * Description  : Delete the semaphore
 * Arguments    : p_id      : Pointer to the area to store the deleted semaphore ID.
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_delete_semaphore (rtos_sem_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    vSemaphoreDelete(*p_id);
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_delete_semaphore() */

/**********************************************************************************************************************
 * Function Name: rtos_get_semaphore
 * Description  : Get the semaphore
 * Arguments    : p_id          : Pointer to the area to store the semaphore ID
 *              : tmo_val       : Timeout value
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_get_semaphore (rtos_sem_id_t *p_id, rtos_time_t tmo_val)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t     err;
    err = xSemaphoreTake(*p_id, tmo_val);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER      err;
    err = twai_sem (*p_id, tmo_val);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_get_semaphore() */

/**********************************************************************************************************************
 * Function Name: rtos_release_semaphore
 * Description  : Release the semaphore
 * Arguments    : p_id          : Pointer to the area to store the semaphore ID
 * Return Value : RTOS_SUCCESS / RTOS_ERROR
 *********************************************************************************************************************/
rtos_err_t  rtos_release_semaphore (rtos_sem_id_t *p_id)
{
#if     BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
    BaseType_t     err;
    err = xSemaphoreGive (*p_id);
    if (pdPASS != err)
    {
        return RTOS_ERROR;
    }
#elif   BSP_CFG_RTOS_USED == 2      /* SEGGER embOS */
#elif   BSP_CFG_RTOS_USED == 3      /* Micrium MicroC/OS */
#elif   BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
    ER      err;
    err = sig_sem (*p_id);
    if (E_OK != err)
    {
        return RTOS_ERROR;
    }
#else
#endif
    return RTOS_SUCCESS;
}/* End of function rtos_release_semaphore() */

#endif /* BSP_CFG_RTOS_USED != 0 */
/******************************************************************************
 End  Of File
 ******************************************************************************/
