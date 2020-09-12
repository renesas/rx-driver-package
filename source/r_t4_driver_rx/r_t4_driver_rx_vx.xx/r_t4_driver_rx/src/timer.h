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

/*  Copyright (C) 2010-2016 Renesas Electronics Corporation All rights reserved.  */

/*""FILE COMMENT""******************************************************************************
 * System Name : ether driver
 * File Name   : timer.h
 * Version     : ----
 * Contents    : timer driver
 * Customer    : Standard
 * Model       : None
 * CPU         : RX610
 * Compiler    :
 * OS          : ---
 * Programmer  :
 * Note        :
 ***********************************************************************************************/
/* ***********************************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.06.2010 ---- First Release
 *""FILE COMMENT END""**************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void OpenTimer(void);
extern void CloseTimer(void);

#endif //_TIMER_H_
/* FILE END */
