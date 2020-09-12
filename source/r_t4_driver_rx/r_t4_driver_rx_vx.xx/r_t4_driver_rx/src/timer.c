/************************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.


* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
************************************************************************************/

/*  Copyright (C) 2011-2019 Renesas Electronics Corporation All rights reserved.  */

/*""FILE COMMENT""******************************************************************************
 * System Name  : ether driver
 * File Name    : timer.c
 * Version      : ----
 * Contents     : timer driver
 * Customer     : Standard
 * Model        : None
 * CPU          : RX
 * Compiler     :
 * OS           : ---
 * Programmer   :
 * Note         :
 ***********************************************************************************************
 ***********************************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.06.2010 ---- First Release
*               : 25.01.2011 ---- clean up source code.
*               : 05.01.2015 ---- Fixed include file.
*               : 20.06.2019 ---- Source code maintenance.
*""FILE COMMENT END""**************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdbool.h>
#include "r_t4_driver_rx_config.h"
#include "timer.h"
#include "r_sys_time_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void OpenTimer(void);
void CloseTimer(void);

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void timer_interrupt(void *pdata);

/******************************************************************************
* Function Name : OpenTimer
* Declaration   : void OpenTimer(void)
* Description   : This function starts with the setting of the Timer interrupt
* Arguments     : none
* Return value  : none
******************************************************************************/
void OpenTimer(void)
{
    if(SYS_TIME_SUCCESS != R_SYS_TIME_RegisterPeriodicCallback((callback_from_sys_time_t)&timer_interrupt, 1))
    {
        /* WAIT_LOOP */
        while (1)
        {
            /* infinite loop(setting error) */
        }
    }
}

/******************************************************************************
* Function Name : CloseTimer
* Declaration   : void CloseTimer(void)
* Description   : This function finishes a Timer interrupt
* Arguments     : none
* Return value  : none
******************************************************************************/
void CloseTimer(void)
{
    if(SYS_TIME_SUCCESS != R_SYS_TIME_UnregisterPeriodicCallback((callback_from_sys_time_t)&timer_interrupt))
    {
        return;
    }
}

/* FILE END*/
