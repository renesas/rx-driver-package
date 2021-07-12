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
 * File Name    : r_emwin_rx_if.h
 * Version      : 1.30
 * Description  : This module enables to use emWin, Graphic Library with Graphical User Interface.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.07.2020 1.00     First Release
 *         : 04.09.2020 1.10     Update to adjust r_emwin_rx_config.h file.
 *         : 11.12.2020 1.20     Update to adjust emWin v6.14g. Modify multi-touch and timer function.
 *                               Adjust GCC and IAR compilers.
 *         : 31.03.2021 1.30     Update to adjust the spec of Smart Configurator and QE for Display.
 *********************************************************************************************************************/
#ifndef EMWIN_RX_IF_H
#define EMWIN_RX_IF_H

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include "r_emwin_rx_config.h"
#if (EMWIN_USE_DRW2D == 1)
#include "dave_driver.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Version number of emWin FIT module. */
#define EMWIN_VERSION_EMWIN_MAJOR   (6)
#define EMWIN_VERSION_EMWIN_MINOR   (14)
#define EMWIN_VERSION_EMWIN_CHAR    ('g')
#define EMWIN_VERSION_FIT_MAJOR     (1)
#define EMWIN_VERSION_FIT_MINOR     (30)

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
void * R_EMWIN_GetBufferAddr (void);
#if (EMWIN_USE_DRW2D == 1)
d2_device * R_EMWIN_GetD2 (void);
void R_EMWIN_EnableDave2D (void);
void R_EMWIN_DisableDave2D (void);
uint32_t R_EMWIN_GetDaveActive (void);
#endif
void _VSYNC_ISR (void * p);
void R_EMWIN_GetVersion (st_emwin_version_t * version);

#endif /* EMWIN_RX_IF_H */
