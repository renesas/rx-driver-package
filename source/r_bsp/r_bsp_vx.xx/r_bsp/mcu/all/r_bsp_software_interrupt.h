/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp_software_interrupt.h
* Description  : This module implements software interrupt specific functions.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 08.10.2019 1.00     First Release
*         : 26.02.2025 1.01     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/* Multiple inclusion prevention macro */
#ifndef SOFTWARE_INTERRUPT_H
#define SOFTWARE_INTERRUPT_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#if (defined(BSP_CFG_SWINT_UNIT1_ENABLE) && (BSP_CFG_SWINT_UNIT1_ENABLE == 1)) || \
    (defined(BSP_CFG_SWINT_UNIT2_ENABLE) && (BSP_CFG_SWINT_UNIT2_ENABLE == 1))

typedef enum e_bsp_swint_unit
{
#if (defined(BSP_CFG_SWINT_UNIT1_ENABLE) && (BSP_CFG_SWINT_UNIT1_ENABLE == 1))
    BSP_SWINT_UNIT1,
#endif /* (defined(BSP_CFG_SWINT_UNIT1_ENABLE) && (BSP_CFG_SWINT_UNIT1_ENABLE == 1)) */
#if (defined(BSP_CFG_SWINT_UNIT2_ENABLE) && (BSP_CFG_SWINT_UNIT2_ENABLE == 1))
    BSP_SWINT_UNIT2,
#endif /* (defined(BSP_CFG_SWINT_UNIT2_ENABLE) && (BSP_CFG_SWINT_UNIT2_ENABLE == 1)) */
    BSP_SWINT_UNIT_MAX
} e_bsp_swint_unit_t;

typedef enum e_bsp_swint_err
{
    BSP_SWINT_SUCCESS = 0,
    BSP_SWINT_ERR_ALREADY_OPEN,
    BSP_SWINT_ERR_NOT_CLOSED,
    BSP_SWINT_ERR_INVALID_UNIT,
    BSP_SWINT_ERR_INVALID_IPR,
    BSP_SWINT_ERR_INVALID_CMD,
    BSP_SWINT_ERR_INVALID_TASK,
    BSP_SWINT_ERR_INVALID_BUFFER_NUMBER,
    BSP_SWINT_ERR_TASK_EXECUTING,
    BSP_SWINT_ERR_FULL_BUFFER,
    BSP_SWINT_ERR_ACCESS_REJECTION
} e_bsp_swint_err_t;

typedef enum e_bsp_swint_cmd
{
    BSP_SWINT_CMD_GET_INTERRUPT_INFORMATION = 0,
    BSP_SWINT_CMD_ENABLE_INTERRUPT,
    BSP_SWINT_CMD_DISABLE_INTERRUPT,
    BSP_SWINT_CMD_SET_INTERRUPT_PRIORITY,
    BSP_SWINT_CMD_SET_INTERRUPT_REQUEST,
    BSP_SWINT_CMD_CLEAR_INTERRUPT_REQUEST,
    BSP_SWINT_CMD_ENABLE_NESTED_INTERRUPT,
    BSP_SWINT_CMD_DISABLE_NESTED_INTERRUPT,
    BSP_SWINT_CMD_CLEAR_TASK,
    BSP_SWINT_CMD_CLEAR_ALL_TASK,
    BSP_SWINT_CMD_GET_ALL_TASK_STATUS,
    BSP_SWINT_CMD_GET_USED_BUFFER,
    BSP_SWINT_CMD_GET_UNUSED_BUFFER
} e_bsp_swint_cmd_t;

typedef struct st_bsp_swint_int_info
{
    uint8_t ipr;
    uint8_t ien;
    uint8_t ir;
    uint8_t nested_int;
} st_bsp_swint_int_info_t;

typedef enum e_bsp_swint_task_status
{
    BSP_SWINT_TASK_STATUS_NO_REQUEST = 0,
    BSP_SWINT_TASK_STATUS_REQUESTED,
    BSP_SWINT_TASK_STATUS_EXECUTING,
    BSP_SWINT_TASK_STATUS_COMPLETED
} e_bsp_swint_task_status_t;

typedef struct st_bsp_swint_task
{
    e_bsp_swint_task_status_t status;
    void (*p_taskAddr)(void *p_task_args);
    void *p_context;
} st_bsp_swint_task_t;

typedef struct st_bsp_swint_task_buffer
{
    uint8_t number;
} st_bsp_swint_task_buffer_t;

typedef struct st_bsp_swint_access_control
{
    int32_t status;
} st_bsp_swint_access_control_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
e_bsp_swint_err_t R_BSP_SoftwareInterruptOpen(e_bsp_swint_unit_t unit);
e_bsp_swint_err_t R_BSP_SoftwareInterruptClose(e_bsp_swint_unit_t unit);
e_bsp_swint_err_t R_BSP_SoftwareInterruptSetTask(e_bsp_swint_unit_t unit, st_bsp_swint_task_t set_task);
e_bsp_swint_err_t R_BSP_SoftwareInterruptControl(e_bsp_swint_unit_t unit, e_bsp_swint_cmd_t const cmd, void * const p_args);

#endif /* (BSP_CFG_SWINT_UNIT1_ENABLE == 1) || (BSP_CFG_SWINT_UNIT2_ENABLE == 1) */

#endif  /* End of multiple inclusion prevention macro */

