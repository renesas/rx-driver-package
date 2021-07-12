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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#ifndef _MESH_SYSTEMTIME_
#define _MESH_SYSTEMTIME_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"

/*******************************************************************************
* Compilation Switches
*******************************************************************************/
#define SYSTEMTIME_EN                   (1)
#define SYSTEMTIME_STRING_EN            (1)

#if SYSTEMTIME_EN

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/**
 *  \brief API to initialize System Time driver.
 *
 *  \par Description
 *  System Timer Driver uses two 8bit-Timers (TMR2 and TMR3) in 16bit Count Mode.
 *  Two timers clear the 16bit counter and interrupt occurs every 16,768[ms].
 *
 *  \return 0, if both TMR2 and TMR3 are available.
 *  1 or 2, if TMR2 or/and TMR3 is not available.
 *
 * \attention System Time driver assumes that PCLKB is 32MHz clock.
 */
uint8_t mesh_systemtime_init(void);

/**
 *  \brief API to check 8bit-Timers for System Time are active.
 *
 *  \return 1, if 8bit-Timers are counting.
 *  0, if 8bit-Timers stop.
 */
uint8_t mesh_systemtime_is_active(void);

/**
 *  \brief API to read Sytem Time.
 *
 *  \par Description
 *  System Time is 32bit-length and counted by millisecond step.
 *  That is why the sytem time wrap-around every 1193 hours.
 *
 * \return 32bit-length millisecond time
 */
uint32_t mesh_systemtime_read(void);

#if SYSTEMTIME_STRING_EN
/**
 *  \brief API to read Sytem Time String.
 *
 *  \par Description
 *  System Time String is represented in the following format:
 *   H:MM:SS.mmm, max 15characters followed by null character
 *     H: 0 to 1193 hours
 *     M: 0 to 59 minutes
 *     S: 0 to 50 seconds
 *     m: 0 to 999 milliseconds
 *
 * \return system time string
 */
char * mesh_systemtime_read_string(void);
#endif /* SYSTEMTIME_STRING_EN */

#endif /* SYSTEMTIME_EN */

#endif /* _MESH_SYSTEMTIME_ */
