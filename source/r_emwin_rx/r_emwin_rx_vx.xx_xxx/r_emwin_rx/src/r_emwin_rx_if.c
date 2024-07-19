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
 * File Name    : r_emwin_rx_if.c
 * Version      : 1.00
 * Description  : Functions to use emWin on RX devices.
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
 *         : 31.01.2024 6.34.g.1.00    Update emWin library to v6.34g.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_emwin_rx_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetVersion
 * Description  : This function will return the version of the currently installed emWin FIT module.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void R_EMWIN_GetVersion(st_emwin_version_t * version)
{
    version->emwin_major = EMWIN_VERSION_EMWIN_MAJOR;
    version->emwin_minor = EMWIN_VERSION_EMWIN_MINOR;
    version->emwin_char  = EMWIN_VERSION_EMWIN_CHAR;
    version->fit_major   = EMWIN_VERSION_FIT_MAJOR;
    version->fit_minor   = EMWIN_VERSION_FIT_MINOR;
}
/**********************************************************************************************************************
 End of function R_EMWIN_GetVersion
 *********************************************************************************************************************/
