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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : GUI_X_Ex.c
 * Version      : 1.30
 * Description  : Config / System dependent externals for GUI.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.07.2020 1.00     First Release
 *         : 04.09.2020 1.10     Update to adjust r_emwin_rx_config.h file.
 *         : 11.12.2020 1.20     Update to adjust emWin v6.14g. Modify multi-touch and timer function.
 *                               Adjust GCC and IAR compilers.
 *         : 31.03.2021 1.30     Update to adjust the spec of Smart Configurator and QE for Display.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "GUI.h"
#if (BSP_CFG_RTOS_USED != 1) /* FreeRTOS */
#include "r_cmt_rx_if.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
#define GUI_MAXIMUM_WAIT        (60000)
#define GUI_WAIT_EVENT_VALUE    (0x00000001)
#endif

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
volatile GUI_TIMER_TIME g_time_ms;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
static TimerHandle_t        s_timer_hdl = NULL;
static SemaphoreHandle_t    s_sem_hdl   = NULL;
static EventGroupHandle_t   s_event_hdl = NULL;
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
    int32_t t_end = g_time_ms + ms;

    /* WAIT_LOOP */
    while ((t_end - g_time_ms) > 0)
    {
        R_BSP_NOP();
    }
}
/**********************************************************************************************************************
 End of function GUI_X_Delay
 *********************************************************************************************************************/

#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
/**********************************************************************************************************************
 * Function Name: cb_os_timer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_os_timer(TimerHandle_t x_timer)
{
    g_time_ms++;
}
/**********************************************************************************************************************
 End of function cb_os_timer
 *********************************************************************************************************************/
#else
/**********************************************************************************************************************
 * Function Name: cb_timer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_timer(void * p_data)
{
    g_time_ms++;
}
/**********************************************************************************************************************
 End of function cb_timer
 *********************************************************************************************************************/
#endif

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
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    const char timer_name = 'a';
    BaseType_t ret;

    s_timer_hdl = xTimerCreate(&timer_name, pdMS_TO_TICKS(1), pdTRUE, 0, cb_os_timer);
    if (NULL == s_timer_hdl)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }

    ret = xTimerStart(s_timer_hdl, 0);
    if (pdPASS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
#else
    uint32_t channel;

    R_CMT_CreatePeriodic(1000, cb_timer, &channel);
#endif
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
 * Description  : This routine creates the resource semaphore or mutex typically used by GUI_X_Lock() and GUI_X_Unlock().
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_InitOS(void)
{
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
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
    R_BSP_NOP();
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
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    BaseType_t ret;

    ret = xSemaphoreGive(s_sem_hdl);
    if (pdPASS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
#else
    R_BSP_NOP();
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
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    BaseType_t ret;

    ret = xSemaphoreTake(s_sem_hdl, 10000);
    if (pdPASS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();
        }
    }
#else
    R_BSP_NOP();
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
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    TaskHandle_t handle;
    TaskStatus_t status;

    handle = xTaskGetCurrentTaskHandle();
    vTaskGetTaskInfo(handle, &status, pdTRUE, eInvalid);

    return status.xTaskNumber;
#else
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
 * Description  : This routine waits an event.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_X_WaitEvent(void)
{
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    xEventGroupWaitBits(s_event_hdl, GUI_WAIT_EVENT_VALUE, pdTRUE, pdFALSE, GUI_MAXIMUM_WAIT);
#else
    R_BSP_NOP();
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
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    xEventGroupSetBits(s_event_hdl, GUI_WAIT_EVENT_VALUE);
#else
    R_BSP_NOP();
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
#if (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
    xEventGroupWaitBits(s_event_hdl, GUI_WAIT_EVENT_VALUE, pdTRUE, pdFALSE, period);
#else
    R_BSP_NOP();
#endif
}
/**********************************************************************************************************************
 End of function GUI_X_WaitEventTimed
 *********************************************************************************************************************/
