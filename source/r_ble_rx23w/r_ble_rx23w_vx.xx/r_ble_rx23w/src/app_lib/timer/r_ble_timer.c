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
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#include "r_ble_timer.h"

#if (BLE_CFG_SOFT_TIMER_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) && ((BSP_CFG_RTOS_USED == 0) || (BSP_CFG_RTOS_USED ==1))

typedef enum {
    BLE_TIMER_STATUS_FREE,
    BLE_TIMER_STATUS_IDLE,
    BLE_TIMER_STATUS_STARTED,
    BLE_TIMER_STATUS_EXPIRED,
} e_ble_timer_status_t;

typedef struct {
    uint8_t        status;
    uint32_t       timer_hdl;
    uint32_t       timeout_ms;
    uint32_t       remaining_time_ms;
    uint8_t        type;
    ble_timer_cb_t cb;
} st_ble_timer_t;

#if (BSP_CFG_RTOS_USED == 0)
static st_ble_timer_t gs_timer[BLE_TIMER_NUM_OF_SLOT];

/***********************************************************************************************************************
* app_lib software timer Functions for bare metal.
***********************************************************************************************************************/
extern void pl_init_timer(void);
extern void pl_terminate_timer(void);
extern void pl_start_timer(uint32_t timeout_ms);
extern void pl_stop_timer(void);
extern uint16_t pl_get_elapsed_time_ms(bool expired);
static void event_cb(void);

static void update_remaining_time_ms(bool expired)
{
    R_BSP_InterruptsDisable();

    uint8_t set_event = false;
    uint32_t elapsed_time_ms = pl_get_elapsed_time_ms(expired);

    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (BLE_TIMER_STATUS_STARTED == gs_timer[i].status)
        {
            if (gs_timer[i].remaining_time_ms > elapsed_time_ms)
            {
                gs_timer[i].remaining_time_ms -= elapsed_time_ms;
            }
            else
            {
                gs_timer[i].remaining_time_ms = 0;
                gs_timer[i].status = BLE_TIMER_STATUS_EXPIRED;
                set_event = true;
            }
        }
    }

    if (false != set_event)
    {
        R_BLE_SetEvent(event_cb);
    }

    R_BSP_InterruptsEnable();
}

static uint32_t alloc_timer(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (gs_timer[i].status == BLE_TIMER_STATUS_FREE)
        {
            gs_timer[i].status = BLE_TIMER_STATUS_IDLE;
            return i;
        }
    }

    return BLE_TIMER_INVALID_HDL;
}

static void free_timer(uint32_t timer_hdl)
{
    gs_timer[timer_hdl].status     = BLE_TIMER_STATUS_FREE;
    gs_timer[timer_hdl].timer_hdl  = BLE_TIMER_INVALID_HDL;
    gs_timer[timer_hdl].timeout_ms = 0;
    gs_timer[timer_hdl].type       = BLE_TIMER_ONE_SHOT;
    gs_timer[timer_hdl].cb         = NULL;
}

static void start_timer(void)
{
    uint32_t next_hdl = BLE_TIMER_INVALID_HDL;
    uint32_t shortest = 0xFFFFFFFF;

    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if ((BLE_TIMER_STATUS_STARTED == gs_timer[i].status) &&
            (shortest > gs_timer[i].remaining_time_ms))
        {
            shortest = gs_timer[i].remaining_time_ms;
            next_hdl = i;
        }
    }

    if (BLE_TIMER_INVALID_HDL != next_hdl)
    {
        pl_start_timer(gs_timer[next_hdl].remaining_time_ms);
    }
}

static void stop_timer(void)
{
    pl_stop_timer();
}

static void add_timer(uint32_t timer_hdl)
{
    gs_timer[timer_hdl].status            = BLE_TIMER_STATUS_STARTED;
    gs_timer[timer_hdl].remaining_time_ms = gs_timer[timer_hdl].timeout_ms;
}

static void remove_timer(uint32_t timer_hdl)
{
    gs_timer[timer_hdl].status = BLE_TIMER_STATUS_IDLE;
}

static void event_cb(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (BLE_TIMER_STATUS_EXPIRED == gs_timer[i].status)
        {
            if (BLE_TIMER_PERIODIC == gs_timer[i].type)
            {
                add_timer(i);
            }
            else
            {
                remove_timer(i);
            }
            gs_timer[i].cb(i);
        }
    }

    stop_timer();
    start_timer();
}

void process_timer_expire(void)
{
    update_remaining_time_ms(true);

    start_timer();
}

void R_BLE_TIMER_Init(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        free_timer(i);
    }

    pl_init_timer();
}

void R_BLE_TIMER_Terminate(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        free_timer(i);
    }

    pl_terminate_timer();
}

ble_status_t R_BLE_TIMER_Create(uint32_t *p_timer_hdl,
                                uint32_t timeout_ms,
                                uint8_t type,
                                ble_timer_cb_t cb)
{
    uint32_t timer_hdl;

    if ((NULL == p_timer_hdl) || (timeout_ms == 0) || (NULL == cb) ||
        ((type != BLE_TIMER_ONE_SHOT) && (type != BLE_TIMER_PERIODIC)))
    {
        return BLE_ERR_INVALID_ARG;
    }

    timer_hdl = alloc_timer();

    if (timer_hdl == BLE_TIMER_INVALID_HDL)
    {
        return BLE_ERR_LIMIT_EXCEEDED;
    }

    *p_timer_hdl = timer_hdl;

    gs_timer[timer_hdl].timeout_ms = timeout_ms;
    gs_timer[timer_hdl].type       = type;
    gs_timer[timer_hdl].cb         = cb;

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_Delete(uint32_t *p_timer_hdl)
{
    if (NULL == p_timer_hdl)
    {
        return BLE_ERR_INVALID_ARG;
    }

    uint32_t timer_hdl = *p_timer_hdl;
    if (BLE_TIMER_NUM_OF_SLOT <= timer_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    *p_timer_hdl = BLE_TIMER_INVALID_HDL;

    if (BLE_TIMER_STATUS_STARTED == gs_timer[timer_hdl].status)
    {
        R_BLE_TIMER_Stop(timer_hdl);
    }

    update_remaining_time_ms(false);
    stop_timer();
    free_timer(timer_hdl);
    start_timer();

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_Start(uint32_t timer_hdl)
{
    if (timer_hdl >= BLE_TIMER_NUM_OF_SLOT)
    {
        return BLE_ERR_INVALID_HDL;
    }

    if ((BLE_TIMER_STATUS_FREE == gs_timer[timer_hdl].status) ||
        (BLE_TIMER_STATUS_EXPIRED == gs_timer[timer_hdl].status))
    {
        return BLE_ERR_INVALID_HDL;
    }

    update_remaining_time_ms(false);
    stop_timer();
    add_timer(timer_hdl);
    start_timer();

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_Stop(uint32_t timer_hdl)
{
    if (timer_hdl >= BLE_TIMER_NUM_OF_SLOT)
    {
        return BLE_ERR_INVALID_HDL;
    }

    update_remaining_time_ms(false);
    stop_timer();
    remove_timer(timer_hdl);
    start_timer();

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_UpdateTimeout(uint32_t timer_hdl, uint32_t timeout_ms)
{
    if (timer_hdl >= BLE_TIMER_NUM_OF_SLOT)
    {
        return BLE_ERR_INVALID_HDL;
    }

    if ((BLE_TIMER_STATUS_FREE == gs_timer[timer_hdl].status) ||
        (BLE_TIMER_STATUS_EXPIRED == gs_timer[timer_hdl].status))
    {
        return BLE_ERR_INVALID_HDL;
    }

    if (timeout_ms == 0)
    {
        return BLE_ERR_INVALID_ARG;
    }

    update_remaining_time_ms(false);
    stop_timer();
    gs_timer[timer_hdl].timeout_ms = timeout_ms;
    add_timer(timer_hdl);
    start_timer();

    return BLE_SUCCESS;
}

#else /* (BSP_CFG_RTOS_USED == 0) */
/***********************************************************************************************************************
* app_lib software timer Functions for FreeRTOS.
***********************************************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "rtos/r_ble_rtos.h"

static st_ble_timer_t gs_timer[BLE_TIMER_NUM_OF_SLOT];

static uint32_t get_timer_index(TimerHandle_t xTimer)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (gs_timer[i].timer_hdl == (uint32_t)xTimer)
        {
            return i;
        }
    }

    return BLE_TIMER_INVALID_INDEX;
}

static void sf_timer_cb(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (BLE_TIMER_STATUS_EXPIRED == gs_timer[i].status)
        {
            gs_timer[i].status = BLE_TIMER_STATUS_IDLE;
            gs_timer[i].cb(gs_timer[i].timer_hdl);
        }
    }
}

static void event_cb(TimerHandle_t xTimer)
{
    uint32_t timer_idx;

    timer_idx = get_timer_index(xTimer);

    if(BLE_TIMER_INVALID_INDEX != timer_idx)
    {
        /* set event for software timer callback */
        gs_timer[timer_idx].status = BLE_TIMER_STATUS_EXPIRED;
        R_BLE_SetEvent(sf_timer_cb);

        /* wake BLE task */
        R_BLE_RTOS_WakeTaskFromIsr();
    }
}

static uint32_t alloc_timer(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (gs_timer[i].status == BLE_TIMER_STATUS_FREE)
        {
            gs_timer[i].status = BLE_TIMER_STATUS_IDLE;
            return i;
        }
    }

    return BLE_TIMER_INVALID_INDEX;
}

static void free_timer(uint32_t timer_idx)
{
    gs_timer[timer_idx].status     = BLE_TIMER_STATUS_FREE;
    gs_timer[timer_idx].timer_hdl  = BLE_TIMER_INVALID_HDL;
    gs_timer[timer_idx].timeout_ms = 0;
    gs_timer[timer_idx].type       = BLE_TIMER_ONE_SHOT;
    gs_timer[timer_idx].cb         = NULL;
}


void R_BLE_TIMER_Init(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        free_timer(i);
    }
}

void R_BLE_TIMER_Terminate(void)
{
    for (uint32_t i = 0; i < BLE_TIMER_NUM_OF_SLOT; i++)
    {
        if (gs_timer[i].status == BLE_TIMER_STATUS_IDLE)
        {
            R_BLE_TIMER_Delete(&gs_timer[i].timer_hdl);
        }
    }
}

ble_status_t R_BLE_TIMER_Create(uint32_t *p_timer_hdl,
                                uint32_t timeout_ms,
                                uint8_t type,
                                ble_timer_cb_t cb)
{
    TimerHandle_t xRtosTimerHdl;
    uint32_t timer_period;
    uint32_t timer_idx;

    if ((NULL == p_timer_hdl) || (timeout_ms == 0) || (NULL == cb) ||
        ((type != BLE_TIMER_ONE_SHOT) && (type != BLE_TIMER_PERIODIC)))
    {
        return BLE_ERR_INVALID_ARG;
    }

    timer_period = timeout_ms / portTICK_PERIOD_MS;
    if(0 == timer_period)
    {
        timer_period = 1;
    }

    timer_idx = alloc_timer();
    if(BLE_TIMER_INVALID_INDEX == timer_idx)
    {
        return BLE_ERR_LIMIT_EXCEEDED;
    }

    xRtosTimerHdl = xTimerCreate("app_lib",
                             timer_period,
                             ((BLE_TIMER_PERIODIC == type) ? pdTRUE : pdFALSE),
                             ( void * ) 0,
                             (TimerCallbackFunction_t)event_cb);

    if (NULL == xRtosTimerHdl)
    {
        free_timer(timer_idx);
        return BLE_ERR_LIMIT_EXCEEDED;
    }

    *p_timer_hdl = (uint32_t)xRtosTimerHdl;
    gs_timer[timer_idx].timer_hdl = (uint32_t)xRtosTimerHdl;
    gs_timer[timer_idx].cb = cb;

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_Delete(uint32_t *p_timer_hdl)
{
    BaseType_t ret;

    if (NULL == p_timer_hdl)
    {
        return BLE_ERR_INVALID_ARG;
    }

    if (BLE_TIMER_INVALID_HDL == *p_timer_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    TimerHandle_t timer_hdl = (TimerHandle_t)*p_timer_hdl;

    *p_timer_hdl = BLE_TIMER_INVALID_HDL;

    if (pdFALSE != xTimerIsTimerActive(timer_hdl))
    {
        xTimerStop((TimerHandle_t)timer_hdl, 0);
    }

    ret = xTimerDelete(timer_hdl, 0);

    uint32_t timer_idx;
    timer_idx = get_timer_index(timer_hdl);
    if(BLE_TIMER_INVALID_INDEX != timer_idx)
    {
        free_timer(timer_idx);
    }

    if(pdFAIL == ret)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_Start(uint32_t timer_hdl)
{
    BaseType_t ret;

    if (BLE_TIMER_INVALID_HDL == timer_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    ret = xTimerStart((TimerHandle_t)timer_hdl, 0);
    if(pdFAIL == ret)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_Stop(uint32_t timer_hdl)
{
    BaseType_t ret;

    if (BLE_TIMER_INVALID_HDL == timer_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    ret = xTimerStop((TimerHandle_t)timer_hdl, 0);
    if(pdFAIL == ret)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    return BLE_SUCCESS;
}

ble_status_t R_BLE_TIMER_UpdateTimeout(uint32_t timer_hdl, uint32_t timeout_ms)
{
    BaseType_t ret;
    uint32_t timer_period;

    if (timeout_ms == 0)
    {
        return BLE_ERR_INVALID_ARG;
    }

    if (BLE_TIMER_INVALID_HDL == timer_hdl)
    {
        return BLE_ERR_INVALID_HDL;
    }

    timer_period = timeout_ms / portTICK_PERIOD_MS;
    if(0 == timer_period)
    {
        timer_period = 1;
    }

    ret = xTimerChangePeriod((TimerHandle_t)timer_hdl, timer_period, 0);
    if(pdFAIL == ret)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    return BLE_SUCCESS;
}

#endif /* (BSP_CFG_RTOS_USED == 0) */

#else /* (BLE_CFG_SOFT_TIMER_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) && ((BSP_CFG_RTOS_USED == 0) || (BSP_CFG_RTOS_USED ==1)) */
void R_BLE_TIMER_Init(void)
{
}

void R_BLE_TIMER_Terminate(void)
{
}

ble_status_t R_BLE_TIMER_Create(uint32_t *p_timer_hdl, uint32_t timeout_ms, uint8_t type, ble_timer_cb_t cb)
{
    (void)p_timer_hdl;
    (void)timeout_ms;
    (void)type;
    (void)cb;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_TIMER_Delete(uint32_t *p_timer_hdl)
{
    (void)p_timer_hdl;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_TIMER_Start(uint32_t timer_hdl)
{
    (void)timer_hdl;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_TIMER_Stop(uint32_t timer_hdl)
{
    (void)timer_hdl;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_TIMER_UpdateTimeout(uint32_t timer_hdl, uint32_t timeout_ms)
{
    (void)timer_hdl;
    (void)timeout_ms;
    return BLE_ERR_UNSUPPORTED;
}

#endif /* (BLE_CFG_SOFT_TIMER_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) && ((BSP_CFG_RTOS_USED == 0) || (BSP_CFG_RTOS_USED ==1)) */
