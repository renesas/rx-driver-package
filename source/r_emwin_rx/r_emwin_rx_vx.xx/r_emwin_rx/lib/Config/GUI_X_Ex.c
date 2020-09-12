/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.

----------------------------------------------------------------------
File        : GUI_X_Ex.c
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

#include "platform.h"
#include "r_cmt_rx_if.h"
#include "r_emwin_rx_config.h"

/*********************************************************************
*
*       Global data
*/
volatile GUI_TIMER_TIME OS_TimeMS;
#if (BSP_CFG_RTOS_USED) == 1
static xSemaphoreHandle xQueueMutex;
static xSemaphoreHandle xSemaTxDone;
#endif

/*********************************************************************
*
*      Timing:
*                 GUI_X_GetTime()
*                 GUI_X_Delay(int)

  Some timing dependent routines require a GetTime
  and delay function. Default time unit (tick), normally is
  1 ms.
*/

GUI_TIMER_TIME GUI_X_GetTime(void) { 
  return OS_TimeMS; 
}

void GUI_X_Delay(int ms) { 
  int tEnd = OS_TimeMS + ms;
  while ((tEnd - OS_TimeMS) > 0);
}


/*********************************************************************
*
*       _cbTimer
*/
static void _cbTimer(void * pData) {
  OS_TimeMS++;
}

/*********************************************************************
*
*       GUI_X_Init()
*
* Note:
*     GUI_X_Init() is called from GUI_Init is a possibility to init
*     some hardware which needs to be up and running before the GUI.
*     If not required, leave this routine blank.
*/

void GUI_X_Init(void) {
  U32 Channel;

  R_CMT_CreatePeriodic(1000, _cbTimer, &Channel);
}


/*********************************************************************
*
*       GUI_X_ExecIdle
*
* Note:
*  Called if WM is in idle state
*/

void GUI_X_ExecIdle(void) {
  volatile int temp;
  temp=0;
}

/*********************************************************************
*
*      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

*/

void GUI_X_Log     (const char *s) { GUI_USE_PARA(s); }
void GUI_X_Warn    (const char *s) { GUI_USE_PARA(s); }
void GUI_X_ErrorOut(const char *s) { GUI_USE_PARA(s); }

/*********************************************************************
*
*      Multitasking:
*
*                 GUI_X_InitOS()
*                 GUI_X_GetTaskId()
*                 GUI_X_Lock()
*                 GUI_X_Unlock()
*
* Note:
*   The following routines are required only if emWin is used in a
*   true multi task environment, which means you have more than one
*   thread using the emWin API.
*   In this case the
*                       #define GUI_OS 1
*  needs to be in GUIConf.h
*/

void GUI_X_InitOS(void)
{
#if (BSP_CFG_RTOS_USED) == 1
	  /* Create Mutex lock */
	  xQueueMutex = xSemaphoreCreateMutex();
	  configASSERT (xQueueMutex != NULL);

	  /* Queue Semaphore */
	  vSemaphoreCreateBinary( xSemaTxDone );
	  configASSERT ( xSemaTxDone != NULL );
#endif
}
void GUI_X_Unlock(void)
{
#if (BSP_CFG_RTOS_USED) == 1
	xSemaphoreGive( xQueueMutex );
#endif
}

void GUI_X_Lock(void)     
{
#if (BSP_CFG_RTOS_USED) == 1
	if(xQueueMutex == NULL)
	{
	  GUI_X_InitOS();
	}
	xSemaphoreTake( xQueueMutex, portMAX_DELAY );
#endif
}
U32  GUI_X_GetTaskId(void)
{
#if (BSP_CFG_RTOS_USED) == 1
	return ((U32) xTaskGetCurrentTaskHandle());
#endif
}


/*********************************************************************
*
*      Event driving (optional with multitasking)
*
*                 GUI_X_WaitEvent()
*                 GUI_X_WaitEventTimed()
*                 GUI_X_SignalEvent()
*/

void GUI_X_WaitEvent(void)
{
#if (BSP_CFG_RTOS_USED) == 1
    while( xSemaphoreTake(xSemaTxDone, portMAX_DELAY ) != pdTRUE );
#endif
}
void GUI_X_SignalEvent(void)
{
#if (BSP_CFG_RTOS_USED) == 1
	xSemaphoreGive( xSemaTxDone );
#endif
}
void GUI_X_WaitEventTimed(int Period) {  }

/*************************** End of file ****************************/
