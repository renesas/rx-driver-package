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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_wifi_da16xxx_os_wrap.h
 * Description  : RTOS wrap functions definition.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "platform.h"

#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

#if WIFI_CFG_LOGGING_OPTION == 1
#include "logging_levels.h"

/* Logging configuration */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "DA16XXX Wi-Fi driver"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL   (WIFI_CFG_DEBUG_LOG)
#endif

#include "logging_stack.h"
#endif

#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
#include "tx_api.h"
#include "tx_mutex.h"
#include "tx_thread.h"
#include "tx_semaphore.h"

#endif /* BSP_CFG_RTOS_USED */

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_WIFI_DA16XXX_OS_WRAP_H
#define R_WIFI_DA16XXX_OS_WRAP_H

#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
#define OS_WRAP_MS_TO_TICKS(ms)    pdMS_TO_TICKS(ms)       /* ms -> tick count            */
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
#define TICK_VALUE                 (1000 / TX_TIMER_TICKS_PER_SECOND)
#define OS_WRAP_MS_TO_TICKS(ms)    (ms < TICK_VALUE ? 1 : ms / TICK_VALUE) /* ms -> tick count */
#else                             /* Bare metal is used. */
#define OS_WRAP_MS_TO_TICKS(ms)    (ms*1000) /* ms -> tick count */
#endif /* BSP_CFG_RTOS_USED */

#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
typedef SemaphoreHandle_t  OS_MUTEX;            /* mutex              */
typedef TimerHandle_t      OS_TIMER_HANDLE;     /* handle for timer   */
typedef TickType_t         OS_TICK;             /* ticks */
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
typedef TX_MUTEX           OS_MUTEX;            /* mutex              */
typedef TX_TIMER           OS_TIMER_HANDLE;     /* handle for timer   */
typedef ULONG              OS_TICK;             /* ticks */
#else                             /* Bare metal is used. */
typedef uint32_t*        OS_MUTEX;            /* mutex              */
typedef uint32_t         OS_TIMER_HANDLE;     /* handle for timer   */
typedef uint32_t         OS_TICK;             /* ticks */
#endif /* BSP_CFG_RTOS_USED */

#define MUTEX_TX             (0x01)     // WIFI API(TX)
#define MUTEX_RX             (0x02)     // WIFI API(RX)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
typedef enum {
    OS_WRAP_SUCCESS = 0,
    OS_WRAP_OTHER_ERR
} e_os_return_code_t;

typedef enum
{
    UNIT_TICK = 0,
    UNIT_MSEC
} e_timer_unit_t;

typedef enum
{
    CALL_ISR = 0,
    CALL_NOT_ISR
} e_call_from_isr_t;

typedef enum
{
    TICK_NOT_EXPIRED = 0,
    TICK_EXPIRED
} e_tick_expired_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == 0        /* Bare metal is used.   */
extern OS_TICK g_tick_count;
#endif /* BSP_CFG_RTOS_USED */

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/*
 * Mutex functions
 */
/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_create
 * Description  : This function creates the specified mutex.
 * Arguments    : None
 * Return Value : OS_WRAP_SUCCESS
 *                OS_WRAP_OTHER_ERR
 *********************************************************************************************************************/
e_os_return_code_t os_wrap_mutex_create(void);

/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_delete
 * Description  : This function deletes the specified mutex.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void os_wrap_mutex_delete (void);

/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_take
 * Description  : This function gets the specified mutex.
 * Arguments    : mutex_flag - Bitmask representing which mutex(es) to take.
 * Return Value : OS_WRAP_SUCCESS
 *                OS_WRAP_OTHER_ERR
 *********************************************************************************************************************/
e_os_return_code_t os_wrap_mutex_take (uint8_t mutex_flag);

/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_give
 * Description  : This function releases the specified mutex.
 * Arguments    : mutex_flag - Bitmask representing which mutex(es) to give.
 * Return Value : OS_WRAP_SUCCESS
 *                OS_WRAP_OTHER_ERR
 *********************************************************************************************************************/
e_os_return_code_t os_wrap_mutex_give (uint8_t mutex_flag);

/*
 * Common function
 */
/**********************************************************************************************************************
 * Function Name: os_wrap_sleep
 * Description  : This function handles application thread sleep requests.
 * Arguments    : timer_tick - timer value
 *                unit       - tick or milliseconds
 * Return Value : None
 *********************************************************************************************************************/
void os_wrap_sleep (uint32_t val, e_timer_unit_t unit);

/*
 * Tick count
 */
/**********************************************************************************************************************
 * Function Name: os_wrap_tickcount_get
 * Description  : This function returns system clock value.
 * Arguments    : None
 * Return Value : Returns the system clock value
 *********************************************************************************************************************/
OS_TICK os_wrap_tickcount_get (void);

/**********************************************************************************************************************
 * Function Name: tick_count_start
 * Description  : Start tick counting.
 * Arguments    : timeout_ms  - expired threshold
                  p_func      - callback function
 * Return Value : None
 *********************************************************************************************************************/
void tick_count_start (uint32_t timeout_ms, void(*p_func)(void));

/**********************************************************************************************************************
 * Function Name: tick_count_stop
 * Description  : Stop tick counting.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void tick_count_stop (void);

/**********************************************************************************************************************
 * Function Name: tick_count_check
 * Description  : check a expired tick count.
 * Arguments    : None
 * Return Value : TICK_NOT_EXPIERD
 *                TICK_EXPIERD
 *********************************************************************************************************************/
uint32_t tick_count_check (void);

/**********************************************************************************************************************
 * Function Name: os_wrap_swdelay
 * Description  : This function handles R_BSP_SoftwareDelay.
 * Arguments    : delay  - timer value
 *                unit   - tick or milliseconds
 * Return Value : None
 *********************************************************************************************************************/
void os_wrap_swdelay (uint32_t delay, e_timer_unit_t unit);

#endif /* R_WIFI_DA16XXX_OS_WRAP_H */
