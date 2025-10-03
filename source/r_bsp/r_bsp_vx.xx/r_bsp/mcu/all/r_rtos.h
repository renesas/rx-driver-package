/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_rtos.h
* Description  : This module implements functions of rtos.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2019 1.00     First Release
*         : 08.10.2019 1.10     Added include file and macro definitions for Renesas RTOS (RI600V4 or RI600PX).
*         : 26.02.2021 1.11     Changed BSP_CFG_RTOS_USED for Azure RTOS.
*         : 26.02.2025 1.12     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_bsp_config.h"

#if BSP_CFG_RTOS_USED == 0      /* Non-OS */
#elif BSP_CFG_RTOS_USED == 1    /* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "croutine.h"
#include "timers.h"
#include "event_groups.h"
#include "freertos_start.h"
#elif BSP_CFG_RTOS_USED == 2    /* SEGGER embOS */
#elif BSP_CFG_RTOS_USED == 3    /* Micrium MicroC/OS */
#elif BSP_CFG_RTOS_USED == 4    /* Renesas RI600V4 & RI600PX */
#include "kernel.h"
#include "kernel_id.h"

#define RENESAS_RI600V4  0
#define RENESAS_RI600PX  1

#undef  BSP_CFG_RTOS_SYSTEM_TIMER
#define BSP_CFG_RTOS_SYSTEM_TIMER      _RI_CLOCK_TIMER
#elif BSP_CFG_RTOS_USED == 5    /* Azure RTOS */
#else
#endif

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_RTOS_H
#define R_RTOS_H

#endif  /* R_RTOS_H */

