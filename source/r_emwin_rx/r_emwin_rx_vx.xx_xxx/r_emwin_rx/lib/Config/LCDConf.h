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
 * File Name    : LCDConf.h
 * Version      : 1.00
 * Description  : SEGGER internal standard configuration file.
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
 *                                     Added LCD_MIRROR definition. (Required for CLUT1)
 *********************************************************************************************************************/
#ifndef LCDCONF_H
#define LCDCONF_H

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_emwin_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Check if the Dave2D engine can be used */
#if (EMWIN_BITS_PER_PIXEL > 8)
#define USE_DAVE2D  (EMWIN_USE_DRW2D)
#else
#define USE_DAVE2D  (0)
#endif

#define LCD_MIRROR  (1) /* Required for CLUT1 */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#if (USE_DAVE2D == 1)
/**********************************************************************************************************************
 * Function Name: drw_int_isr
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
extern void drw_int_isr (void);
#endif

#endif /* LCDCONF_H */
