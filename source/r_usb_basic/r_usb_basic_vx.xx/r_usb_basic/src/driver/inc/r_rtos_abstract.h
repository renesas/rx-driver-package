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
* Copyright (C) 2016(2020) Renesas Electronics Corporation. All rights reserved.    
 *********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_rtos_abstract.h
* Version      : 1.0
* Description  : Create tasks, mailboxes, memory pool for USB in freeRTOS.
 *****************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.06.2016 1.00     First Release
*         : 16.11.2018 1.24     Supporting RTOS Thread safe
*         : 01.03.2020 1.30 RX72N/RX66N is added and uITRON is supported.
 *********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#ifndef R_RTOS_ABSTRACT_H
#define R_RTOS_ABSTRACT_H

#include "platform.h"

#if   (BSP_CFG_RTOS_USED == 1)      /* FreeRTOS */
#include    "FreeRTOS.h"
#include    "task.h"
#include    "queue.h"
#include    "semphr.h"
#elif (BSP_CFG_RTOS_USED == 2)      /* SEGGER embOS */
#elif (BSP_CFG_RTOS_USED == 3)      /* Micrium MicroC/OS */
#elif (BSP_CFG_RTOS_USED == 4)      /* Renesas RI600V4 & RI600PX */
#include    "kernel.h"
#include    "kernel_id.h"
#else
#endif  /* BSP_CFG_RTOS_USED == 4 */

#if   (BSP_CFG_RTOS_USED != 0)      /* Use RTOS */
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 1)        /* FreeRTOS */
#define RTOS_FOREVER                (portMAX_DELAY)
#else  /* (BSP_CFG_RTOS_USED != 1) */
#define RTOS_FOREVER                (-1)
#endif /* (BSP_CFG_RTOS_USED != 1) */

#define RTOS_ZERO                   (0u)
#define RTOS_TMO                    (3000u)


/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/
#if   (BSP_CFG_RTOS_USED == 1)      /* FreeRTOS */
typedef TaskHandle_t        rtos_task_id_t;
typedef QueueHandle_t       rtos_mbx_id_t;
typedef QueueHandle_t       rtos_mem_id_t;
typedef SemaphoreHandle_t   rtos_sem_id_t;
typedef TickType_t          rtos_time_t;
#elif (BSP_CFG_RTOS_USED == 2)      /* SEGGER embOS */
#elif (BSP_CFG_RTOS_USED == 3)      /* Micrium MicroC/OS */
#elif (BSP_CFG_RTOS_USED == 4)      /* Renesas RI600V4 & RI600PX */
typedef ID                  rtos_task_id_t;
typedef ID                  rtos_mbx_id_t;
typedef ID                  rtos_mem_id_t;
typedef ID                  rtos_sem_id_t;
typedef TMO                 rtos_time_t;
#else
#endif
typedef uint8_t             rtos_semaphore_count_t;

typedef struct rtos_task_info 
{
#if   (BSP_CFG_RTOS_USED == 1)      /* FreeRTOS */
    TaskFunction_t  task_code;
    char            *p_name;
    uint16_t        stack_depth;
    void            *p_parameter;
    UBaseType_t     priority;
#elif (BSP_CFG_RTOS_USED == 2)      /* SEGGER embOS */
#elif (BSP_CFG_RTOS_USED == 3)      /* Micrium MicroC/OS */
#elif (BSP_CFG_RTOS_USED == 4)      /* Renesas RI600V4 & RI600PX */
    ATR             tskatr;
    VP_INT          exinf;
    FP              task;
    PRI             itskpri;
    SIZE            stksz;
    VP              stk;
#else
#endif
} rtos_task_info_t;

typedef struct rtos_mbx_info
{
#if (BSP_CFG_RTOS_USED == 1)        /* FreeRTOS */
    UBaseType_t length;
#elif (BSP_CFG_RTOS_USED == 2)      /* SEGGER embOS */
#elif (BSP_CFG_RTOS_USED == 3)      /* Micrium MicroC/OS */
#elif (BSP_CFG_RTOS_USED == 4)      /* Renesas RI600V4 & RI600PX */
    ATR         mbxatr;
    PRI         maxpri;
    VP          mprihd;
#else
#endif
} rtos_mbx_info_t;

/* For Fixed Type Memory block */
typedef struct rtos_mpf_info 
{
#if (BSP_CFG_RTOS_USED == 1)        /* FreeRTOS */
    void            *p_mempool;
    uint16_t        block_number;
    uint16_t        block_size;
#elif (BSP_CFG_RTOS_USED == 2)      /* SEGGER embOS */
#elif (BSP_CFG_RTOS_USED == 3)      /* Micrium MicroC/OS */
#elif (BSP_CFG_RTOS_USED == 4)      /* Renesas RI600V4 & RI600PX */
    ATR             mpfatr;
    UINT            blkcnt;
    UINT            blksz;
    VP              mpf;
#else
#endif
} rtos_mpf_info_t;

/* For Semaphore */
typedef struct rtos_sem_info 
{
#if (BSP_CFG_RTOS_USED == 1)        /* FreeRTOS */
    UBaseType_t     initial_count;
    UBaseType_t     max_count;
#elif (BSP_CFG_RTOS_USED == 2)      /* SEGGER embOS */
#elif (BSP_CFG_RTOS_USED == 3)      /* Micrium MicroC/OS */
#elif (BSP_CFG_RTOS_USED == 4)      /* Renesas RI600V4 & RI600PX */
    ATR             sematr;
    UINT            initial_count;
    UINT            max_count;
#else
#endif
} rtos_sem_info_t;

typedef enum e_usb_rtos_err
{
    RTOS_SUCCESS,                   /* Success */
    RTOS_ERROR,                     /* Error */
} rtos_err_t;

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/* For Fixed type memory pool */
rtos_err_t      rtos_create_fixed_memory(rtos_mem_id_t *p_id, rtos_mpf_info_t *p_info);
rtos_err_t      rtos_delete_fixed_memory(rtos_mem_id_t *p_id);
rtos_err_t      rtos_get_fixed_memory(rtos_mem_id_t *p_id, void **pp_memblk, rtos_time_t tmo_val);
rtos_err_t      rtos_get_fixed_memory_isr(rtos_mem_id_t *p_id, void **pp_memblk);
rtos_err_t      rtos_release_fixed_memory(rtos_mem_id_t *p_id, void *p_memblk);

/* For task */
rtos_err_t      rtos_create_task (rtos_task_id_t *p_id, rtos_task_info_t *p_info);
rtos_err_t      rtos_delete_task (rtos_task_id_t *p_id);
rtos_err_t      rtos_get_task_id (rtos_task_id_t *p_id);
rtos_err_t      rtos_start_task  (rtos_task_id_t *p_id);

/* For mailbox */
rtos_err_t      rtos_create_mailbox (rtos_mbx_id_t *p_id, rtos_mbx_info_t *p_info);
rtos_err_t      rtos_delete_mailbox (rtos_mbx_id_t *p_id);
rtos_err_t      rtos_send_mailbox (rtos_mbx_id_t *p_id, void *p_message);
rtos_err_t      rtos_send_mailbox_isr (rtos_mbx_id_t *p_id, void *p_message);
rtos_err_t      rtos_receive_mailbox (rtos_mbx_id_t *p_id, void **pp_message, rtos_time_t tmo_val);

/* For semaphore */
rtos_err_t      rtos_create_semaphore (rtos_sem_id_t *p_id, rtos_sem_info_t *p_info);
rtos_err_t      rtos_delete_semaphore (rtos_sem_id_t *p_id);
rtos_err_t      rtos_get_semaphore (rtos_sem_id_t *p_id, rtos_time_t tmo_val);
rtos_err_t      rtos_release_semaphore (rtos_sem_id_t *p_id);

#endif /* BSP_CFG_RTOS_USED != 0 */
#endif /* R_RTOS_ABSTRACT_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/
