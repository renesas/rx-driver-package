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
 * File Name    : r_emwin_rx_if.h
 * Version      : 1.00
 * Description  : This module enables to use emWin, Graphic Library with Graphical User Interface.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.07.2020 6.14.a.1.00    First Release
 *         : 04.09.2020 6.14.a.1.10    Update to adjust r_emwin_rx_config.h file.
 *         : 11.12.2020 6.14.g.1.20    Update to adjust emWin v6.14g. Modify multi-touch and timer function.
 *                                     Adjust GCC and IAR compilers.
 *         : 31.03.2021 6.14.g.1.30    Update to adjust the spec of Smart Configurator and QE for Display.
 *         : 29.12.2021 6.20.  1.00    Update emWin library to v6.22.
 *                                     Adjust configuration option with Smart Configurator.
 *         : 31.08.2022 6.26.c.1.00    Update emWin library to v6.26c.
 *                                     Update FIT module version.
 *                                     Added preprocessor condition by LCD interface.
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *         : 31.01.2024 6.34.g.1.00    Update emWin library to v6.34g.
 *         : 12.04.2024 6.34.g.1.10    Update FIT module version.
 *         : 30.08.2024 6.34.g.1.20    Update FIT module version.
 *********************************************************************************************************************/
#ifndef EMWIN_RX_IF_H
#define EMWIN_RX_IF_H

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "r_emwin_rx_config.h"
#if ((EMWIN_LCD_IF == LCD_IF_GLCDC) && (EMWIN_USE_DRW2D == 1))
#include "dave_driver.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Version number of emWin FIT module. */
#define EMWIN_VERSION_EMWIN_MAJOR   (6)
#define EMWIN_VERSION_EMWIN_MINOR   (34)
#define EMWIN_VERSION_EMWIN_CHAR    ('g')
#define EMWIN_VERSION_FIT_MAJOR     (1)
#define EMWIN_VERSION_FIT_MINOR     (20)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* Version type of emWin FIT module. */
typedef struct
{
    uint8_t emwin_major;
    uint8_t emwin_minor;
    uint8_t emwin_char;
    uint8_t fit_major;
    uint8_t fit_minor;
} st_emwin_version_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetBufferAddr
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void * R_EMWIN_GetBufferAddr (void);

#if (EMWIN_USE_DRW2D == 1)
/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetD2
 * Description  : Get Dave2D handle.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
d2_device * R_EMWIN_GetD2 (void);

/**********************************************************************************************************************
 * Function Name: R_EMWIN_EnableDave2D
 * Description  : Enable Dave2D functions.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void R_EMWIN_EnableDave2D (void);

/**********************************************************************************************************************
 * Function Name: R_EMWIN_DisableDave2D
 * Description  : Disable Dave2D functions.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void R_EMWIN_DisableDave2D (void);

/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetDaveActive
 * Description  : Get status of Dave2D functions.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
uint32_t R_EMWIN_GetDaveActive (void);
#endif /* EMWIN_USE_DRW2D == 1 */

/**********************************************************************************************************************
 * Function Name: _VSYNC_ISR
 * Description  : Callback function of LCD interface.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void _VSYNC_ISR (void * p);

#endif /* EMWIN_LCD_IF */

/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetVersion
 * Description  : This function will return the version of the currently installed emWin FIT module.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void R_EMWIN_GetVersion (st_emwin_version_t * version);

#endif /* EMWIN_RX_IF_H */
