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
 * File Name    : r_wifi_da16xxx_os_wrap.c
 * Description  : RTOS wrap functions definition.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_os_wrap.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == 0        /* Bare metal is used.   */
OS_TICK g_tick_count;
#endif /* BSP_CFG_RTOS_USED */

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* for Tick count functions */
static OS_TICK s_exec_tick_count_flg = 0;
static OS_TICK s_tick_begin = 0;
static OS_TICK s_tick_timeout = 0;
static void(*sp_callback)(void);

#if BSP_CFG_RTOS_USED != 0        /* Bare metal is not used.   */
/* OS parameters */
static OS_MUTEX s_binary_sem_tx;
static OS_MUTEX s_binary_sem_rx;
static const OS_TICK s_sem_block_time = OS_WRAP_MS_TO_TICKS(10000UL);
#endif /* BSP_CFG_RTOS_USED */

/*
 * Mutex
 */
/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_create
 * Description  : This function creates the specified mutex.
 * Arguments    : None
 * Return Value : OS_WRAP_SUCCESS
 *                OS_WRAP_OTHER_ERR
 *********************************************************************************************************************/
e_os_return_code_t os_wrap_mutex_create(void)
{
#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
    s_binary_sem_tx = xSemaphoreCreateMutex();
    if (NULL == s_binary_sem_tx)
    {
        return OS_WRAP_OTHER_ERR;
    }

    s_binary_sem_rx = xSemaphoreCreateMutex();
    if (NULL == s_binary_sem_rx)
    {
        return OS_WRAP_OTHER_ERR;
    }
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
    if (TX_SUCCESS != tx_mutex_create(s_binary_sem_tx, "DA16XXX tx mutex", TX_INHERIT))
    {
        return OS_WRAP_OTHER_ERR;
    }

    if (TX_SUCCESS != tx_mutex_create(s_binary_sem_rx, "DA16XXX rx mutex", TX_INHERIT))
    {
        return OS_WRAP_OTHER_ERR;
    }
#else                             /* Bare metal is used. */
    /* Do nothing */
#endif
    return OS_WRAP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function os_wrap_mutex_create
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_delete
 * Description  : This function deletes the specified mutex.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void os_wrap_mutex_delete(void)
{
#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
    if (NULL != s_binary_sem_tx)
    {
        vSemaphoreDelete(s_binary_sem_tx);
        s_binary_sem_tx = NULL;
    }

    if (NULL != s_binary_sem_rx)
    {
        vSemaphoreDelete(s_binary_sem_rx);
        s_binary_sem_rx = NULL;
    }
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
    if (TX_MUTEX_ID == s_binary_sem_tx.tx_mutex_id)
    {
        tx_mutex_delete(s_binary_sem_tx);
    }

    if (TX_MUTEX_ID == s_binary_sem_rx.tx_mutex_id)
    {
        tx_mutex_delete(s_binary_sem_rx);
    }
#else                             /* Bare metal is used. */
    /* Do nothing */
#endif /* BSP_CFG_RTOS_USED */
}
/**********************************************************************************************************************
 * End of function os_wrap_mutex_delete
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_take
 * Description  : This function gets the specified mutex.
 * Arguments    : mutex_flag - Bitmask representing which mutex(es) to take.
 * Return Value : OS_WRAP_SUCCESS
 *                OS_WRAP_OTHER_ERR
 *********************************************************************************************************************/
e_os_return_code_t os_wrap_mutex_take(uint8_t mutex_flag)
{
#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
    if (0 != (mutex_flag & MUTEX_TX))
    {
        if (pdTRUE != xSemaphoreTake(s_binary_sem_tx, s_sem_block_time))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }

    if (0 != (mutex_flag & MUTEX_RX))
    {
        if (pdTRUE != xSemaphoreTake(s_binary_sem_rx, s_sem_block_time))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
    if (0 != (mutex_flag & MUTEX_TX))
    {
        if (TX_SUCCESS != tx_mutex_get(&s_binary_sem_tx, s_sem_block_time))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }

    if (0 != (mutex_flag & MUTEX_RX))
    {
        if (TX_SUCCESS != tx_mutex_get(&s_binary_sem_rx, s_sem_block_time))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }
#else                             /* Bare metal is used. */
    /* Do nothing */
#endif /* BSP_CFG_RTOS_USED */
    return OS_WRAP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function os_wrap_mutex_take
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: os_wrap_mutex_give
 * Description  : This function releases the specified mutex.
 * Arguments    : mutex_flag - Bitmask representing which mutex(es) to give.
 * Return Value : OS_WRAP_SUCCESS
 *                OS_WRAP_OTHER_ERR
 *********************************************************************************************************************/
e_os_return_code_t os_wrap_mutex_give(uint8_t mutex_flag)
{
#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
    if (0 != (mutex_flag & MUTEX_TX))
    {
        if (pdTRUE != xSemaphoreGive(s_binary_sem_tx))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }

    if (0 != (mutex_flag & MUTEX_RX))
    {
        if (pdTRUE != xSemaphoreGive(s_binary_sem_rx))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }
    os_wrap_sleep(10, UNIT_TICK);
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
    if (0 != (mutex_flag & MUTEX_TX))
    {
        if (TX_SUCCESS != tx_mutex_put(&s_binary_sem_tx))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }

    if (0 != (mutex_flag & MUTEX_RX))
    {
        if (TX_SUCCESS != tx_mutex_put(&s_binary_sem_rx))
        {
            return OS_WRAP_OTHER_ERR;
        }
    }
    os_wrap_sleep(10, UNIT_TICK);
#else                             /* Bare metal is used. */
    /* Do nothing */
#endif /* BSP_CFG_RTOS_USED */
    return OS_WRAP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function os_wrap_mutex_give
 *********************************************************************************************************************/

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
void os_wrap_sleep(uint32_t val, e_timer_unit_t unit)
{
    uint32_t tick;

    if (UNIT_MSEC == unit)
    {
        tick =  OS_WRAP_MS_TO_TICKS(val);
    }
    else
    {
        tick =  val;
    }
#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
    vTaskDelay(tick);
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
    tx_thread_sleep(tick);
#else                             /* Bare metal is used. */
    g_tick_count += tick;
    os_wrap_swdelay(val, unit);
#endif /* BSP_CFG_RTOS_USED */
}
/**********************************************************************************************************************
 * End of function os_wrap_sleep
 *********************************************************************************************************************/

/*
 * Tick count
 */
/**********************************************************************************************************************
 * Function Name: os_wrap_tickcount_get
 * Description  : This function returns system clock value.
 * Arguments    : None
 * Return Value : Returns the system clock value
 *********************************************************************************************************************/
OS_TICK os_wrap_tickcount_get(void)
{
#if BSP_CFG_RTOS_USED == 1        /* FreeRTOS is used.   */
    return xTaskGetTickCount();
#elif BSP_CFG_RTOS_USED == 5      /* Azure RTOS is used. */
    return tx_time_get();
#else                             /* Bare metal is used. */
    return g_tick_count;
#endif /* BSP_CFG_RTOS_USED */
}
/**********************************************************************************************************************
 * End of function os_wrap_tickcount_get
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: tick_count_start
 * Description  : Start tick counting.
 * Arguments    : timeout_ms  - expired threshold
                  p_func      - callback function
 * Return Value : None
 *********************************************************************************************************************/
void tick_count_start(uint32_t timeout_ms, void(*p_func)(void))
{
    if (0 == timeout_ms)
    {
        return;
    }

    s_exec_tick_count_flg = 1;
    sp_callback = p_func;
    s_tick_timeout = OS_WRAP_MS_TO_TICKS(timeout_ms);
    s_tick_begin = os_wrap_tickcount_get();
}
/**********************************************************************************************************************
 * End of function tick_count_start
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: tick_count_stop
 * Description  : Stop tick counting.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void tick_count_stop(void)
{
    s_exec_tick_count_flg = 0;
    sp_callback = NULL;
}
/**********************************************************************************************************************
 * End of function tick_count_stop
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: tick_count_check
 * Description  : check a expired tick count.
 * Arguments    : None
 * Return Value : TICK_NOT_EXPIERD
 *                TICK_EXPIERD
 *********************************************************************************************************************/
uint32_t tick_count_check(void)
{
    uint32_t ret = TICK_NOT_EXPIRED;
    OS_TICK tick_tmp;

    if (1 != s_exec_tick_count_flg)
    {
        return ret;
    }

    tick_tmp = os_wrap_tickcount_get() - s_tick_begin;
    if (tick_tmp >= s_tick_timeout)
    {
        if (NULL != sp_callback)
        {
            sp_callback();
        }
        s_exec_tick_count_flg = 0;
        ret = TICK_EXPIRED;
    }
    return ret;
}
/**********************************************************************************************************************
 * End of function tick_count_check
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name: os_wrap_swdelay
 * Description  : software delay
 * Arguments    : delay - delay value
 *                unit  - delay unit
 * Return Value : None
 *********************************************************************************************************************/
void os_wrap_swdelay (uint32_t delay, e_timer_unit_t unit)
{
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
    bsp_delay_units_t time_unit;

    if (UNIT_TICK == unit)
    {
        time_unit = BSP_DELAY_MICROSECS;
    }
    else if (UNIT_MSEC == unit)
    {
        time_unit = BSP_DELAY_MILLISECS;
    }

    R_BSP_SoftwareDelay(delay, time_unit);

#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
    uint32_t time;
    uint32_t fclk;

    /* FIX ME: add defined clock from setting*/
    fclk = 32;

    switch (unit)
    {
        case UNIT_TICK:
            time = delay;
            fclk = fclk / 20;
            break;
        case UNIT_MSEC:
            time = delay * 100;
            break;
        default:
            break;
    }

    while(0 < time)
    {
        /* Delay 1us */
        for(uint16_t cnt = fclk; 0 < cnt; cnt--)
        {
            BSP_NOP();
            BSP_NOP();
        }
        time--;
    }
#endif
}
/**********************************************************************************************************************
 * End of function os_wrap_swdelay
 *********************************************************************************************************************/
