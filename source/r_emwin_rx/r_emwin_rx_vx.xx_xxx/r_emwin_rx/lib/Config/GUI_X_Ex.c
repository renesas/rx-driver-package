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
 * File Name    : GUI_X_Ex.c
 * Version      : 1.00
 * Description  : Config / System dependent externals for GUI.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.07.2020 6.14.a.1.00    First Release
 *         : 04.09.2020 6.14.a.1.10    Update to adjust r_emwin_rx_config.h file.
 *         : 11.12.2020 6.14.g.1.20    Update to adjust emWin v6.14g. Modify multi-touch and timer function.
 *                                     Adjust GCC and IAR compilers.
 *         : 31.03.2021 6.14.g.1.30    Update to adjust the spec of Smart Configurator and QE for Display.
 *         : 29.12.2021 6.22.  1.00    Update emWin library to v6.22.
 *                                     Adjust configuration option with Smart Configurator.
 *         : 31.08.2022 6.26.c.1.00    Update emWin library to v6.26c.
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *                                     Fixed GUI_X_GetTaskId function processing.
 *                                     Fixed preprocessing with value of BSP_CFG_RTOS_USED.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "GUI.h"
#include "GUIConf.h"
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
#include "r_cmt_rx_if.h"
#endif
#include "../../src/r_emwin_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define EMWIN_RX_TIMER_NUM      (2)

#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
#define GUI_MAXIMUM_WAIT        (60000)
#define GUI_WAIT_EVENT_VALUE    (0x00000001)
#endif

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
typedef void TIMER_CALLBACK (void * p_data);
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
typedef void TIMER_CALLBACK (TimerHandle_t x_timer);
#else
#warning "Warning!! It is necessary to implement callback functuon for timer."
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
volatile GUI_TIMER_TIME g_time_ms;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static uint8_t s_timer_num = 0;

#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
static uint32_t             timer_channel[EMWIN_RX_TIMER_NUM];
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
static char                 timer_name                      = '0';
static TimerHandle_t        s_timer_hdl[EMWIN_RX_TIMER_NUM] = { 0 };
static SemaphoreHandle_t    s_sem_hdl                       = NULL;
static EventGroupHandle_t   s_event_hdl                     = NULL;
#else
#warning "Warning!! It is necessary to implement variables and functuons for the OS."
#endif

/*********************************************************************
 *
 *      Timing:
 *                 GUI_X_GetTime()
 *                 GUI_X_Delay(int)
 *
 *  Some timing dependent routines require a GetTime
 *  and delay function. Default time unit (tick), normally is
 *  1 ms. */

/**********************************************************************************************************************
 * Function Name: GUI_X_GetTime
 * Description  : Used by GUI_GetTime() to return the current system time in miliseconds.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
GUI_TIMER_TIME GUI_X_GetTime(void)
{
    return g_time_ms;
}
/**********************************************************************************************************************
 End of function GUI_X_GetTime
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_Delay
 * Description  : Return after a specified period in miliseconds.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_Delay(int ms)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    int32_t t_end = g_time_ms + ms;

    /* WAIT_LOOP */
    while ((t_end - g_time_ms) > 0)
    {
        R_BSP_NOP();
    }
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    #warning "Warning!! It is necessary to implement to delay process."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_Delay
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: cb_timer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_timer(cb_timer_arg_t arg)
{
    g_time_ms++;
}
/**********************************************************************************************************************
 End of function cb_timer
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_guix_timer_create
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_guix_timer_create(uint32_t period_ms, emwin_rx_cb_timer * pf_callback_timer)
{
    e_emwin_rx_err_t ret = EMWIN_RX_SUCCESS;
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    bool             cmt_ret;
    uint32_t         frequency_hz;
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    BaseType_t       os_ret;
#else
    #warning "Warning!! It is necessary to implement to create timer."
#endif

    if (s_timer_num >= EMWIN_RX_TIMER_NUM)
    {
        ret = EMWIN_RX_FAIL;
    }

    if (EMWIN_RX_SUCCESS == ret)
    {
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
        frequency_hz = 1000 / period_ms;

        /* Casting TIMER_CALLBACK pointer is used for callback function. */
        cmt_ret = R_CMT_CreatePeriodic(frequency_hz, (TIMER_CALLBACK *)pf_callback_timer, &timer_channel[s_timer_num]);
        if (true != cmt_ret)
        {
            ret = EMWIN_RX_FAIL;
        }
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
        ret = EMWIN_RX_FAIL;
        timer_name += 1;

        /* Casting TIMER_CALLBACK pointer is used for callback function. */
        s_timer_hdl[s_timer_num] = xTimerCreate(&timer_name, pdMS_TO_TICKS(period_ms), pdTRUE, 0,
                                                (TIMER_CALLBACK *)pf_callback_timer);
        if (NULL != s_timer_hdl[s_timer_num])
        {
            os_ret = xTimerStart(s_timer_hdl[s_timer_num], 0);
            if (pdPASS == os_ret)
            {
                ret = EMWIN_RX_SUCCESS;
            }
        }
#else
        #warning "Warning!! It is necessary to implement to create timer."
#endif

        s_timer_num++;
    }

    return ret;
}
/**********************************************************************************************************************
 End of function r_emwin_rx_guix_timer_create
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_Init
 * Description  : GUI_X_Init() is called from GUI_Init is a possibility to init
 *                some hardware which needs to be up and running before the GUI.
 *                If not required, leave this routine blank.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_Init(void)
{
    e_emwin_rx_err_t ret;

    /* Casting emwin_rx_cb_timer pointer is used for callback function. */
    ret = r_emwin_rx_guix_timer_create(1, (emwin_rx_cb_timer *)cb_timer);
    if (EMWIN_RX_SUCCESS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
}
/**********************************************************************************************************************
 End of function GUI_X_Init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_ExecIdle
 * Description  : Called only from non-blocking functions of the WIndow Manager.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_ExecIdle(void)
{
    R_BSP_NOP();
}
/**********************************************************************************************************************
 End of function GUI_X_ExecIdle
 *********************************************************************************************************************/

/*********************************************************************
 *
 *      Logging: OS dependent
 *
 *  Note:
 *    Logging is used in higher debug levels only. The typical target
 *    build does not use logging and does therefor not require any of
 *    the logging routines below. For a release build without logging
 *    the routines below may be eliminated to save some space.
 *    (If the linker is not function aware and eliminates unreferenced
 *    functions automatically) */

/**********************************************************************************************************************
 * Function Name: GUI_X_Log
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_Log(const char *s)
{
    /* This routine is called by emWin with debug information in higher debug levels in case a problem (error) or
     * potential problem is discovered. This routine is used to output messages. */
    GUI_USE_PARA(s);
}
/**********************************************************************************************************************
 End of function GUI_X_Log
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_Warn
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_Warn(const char *s)
{
    /* This routine is called by emWin with debug information in higher debug levels in case a problem (error) or
     * potential problem is discovered. This routine is used to output warnings. */
    GUI_USE_PARA(s);
}
/**********************************************************************************************************************
 End of function GUI_X_Warn
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_ErrorOut
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_ErrorOut(const char *s)
{
    /* This routine is called by emWin with debug information in higher debug levels in case a problem (error) or
     * potential problem is discovered. This routine is used to output fatal errors. */
    GUI_USE_PARA(s);
}
/**********************************************************************************************************************
 End of function GUI_X_ErrorOut
 *********************************************************************************************************************/

/*********************************************************************
 *
 *      Multitasking:
 *
 *                 GUI_X_InitOS()
 *                 GUI_X_GetTaskId()
 *                 GUI_X_Lock()
 *                 GUI_X_Unlock()
 *
 *  Note:
 *    The following routines are required only if emWin is used in a
 *    true multi task environment, which means you have more than one
 *    thread using the emWin API.
 *    In this case the
 *                         #define GUI_OS 1
 *    needs to be in GUIConf.h */

/**********************************************************************************************************************
 * Function Name: GUI_X_InitOS
 * Description  : This routine creates the resource semaphore or mutex typically used by GUI_X_Lock() and
 *                GUI_X_Unlock().
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_InitOS(void)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    R_BSP_NOP();
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    s_sem_hdl = xSemaphoreCreateMutex();
    if (NULL == s_sem_hdl)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }

    s_event_hdl = xEventGroupCreate();
    if (NULL == s_event_hdl)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
#else
    #warning "Warning!! It is necessary to implement to create semaphore or mutex."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_InitOS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_Unlock
 * Description  : This routine unlocks the GUI.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_Unlock(void)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    R_BSP_NOP();
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    BaseType_t os_ret;

    os_ret = xSemaphoreGive(s_sem_hdl);
    if (pdPASS != os_ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
#else
    #warning "Warning!! It is necessary to implement to unlock."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_Unlock
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_Lock
 * Description  : This routine locks the GUI.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_Lock(void)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    R_BSP_NOP();
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    BaseType_t os_ret;

    os_ret = xSemaphoreTake(s_sem_hdl, 10000);
    if (pdPASS != os_ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
#else
    #warning "Warning!! It is necessary to implement to lock."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_Lock
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_GetTaskId
 * Description  : This routine returns a unique ID for the current task.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
U32 GUI_X_GetTaskId(void)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    return 1;
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    TaskHandle_t handle;

    handle = xTaskGetCurrentTaskHandle();

    return (U32)handle;
#else
    #warning "Warning!! It is necessary to implement to get task ID."
    return 1;
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_GetTaskId
 *********************************************************************************************************************/

/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/

/**********************************************************************************************************************
 * Function Name: GUI_X_WaitEvent
 * Description  : This routine waits for an event.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_WaitEvent(void)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    R_BSP_NOP();
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    xEventGroupWaitBits(s_event_hdl, GUI_WAIT_EVENT_VALUE, pdTRUE, pdFALSE, GUI_MAXIMUM_WAIT);
#else
    #warning "Warning!! It is necessary to implement to wait for an event."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_WaitEvent
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_SignalEvent
 * Description  : This routine signals an event.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_SignalEvent(void)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    R_BSP_NOP();
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    xEventGroupSetBits(s_event_hdl, GUI_WAIT_EVENT_VALUE);
#else
    #warning "Warning!! It is necessary to implement to signal an event."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_SignalEvent
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_X_WaitEventTimed
 * Description  : This routine waits for an event for the given period.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_WaitEventTimed(int period)
{
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
    R_BSP_NOP();
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    xEventGroupWaitBits(s_event_hdl, GUI_WAIT_EVENT_VALUE, pdTRUE, pdFALSE, period);
#else
    #warning "Warning!! It is necessary to implement to wait for an event for the given period."
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_WaitEventTimed
 *********************************************************************************************************************/
