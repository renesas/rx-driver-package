/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * Copyright (C) 2014(2018) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/******************************************************************************
 * File Name    : r_usb_pmsc_mini_if.h
 * Description  : Interface file for USB vendor class API for RX
 ******************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2014 1.00     First Release
 *         : 01.06.2015 1.01     Added RX231.
 *         : 30.11.2018 1.10     Supporting Smart Configurator
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* Used to get which MCU is currently being used. */
/* User specific options for Flash API */
#include "r_usb_basic_mini_config.h"
#include "r_usb_pmsc_mini_config.h"

#ifndef USB_PMSC_MINI_IF_H
#define USB_PMSC_MINI_IF_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* MSC Class Request code define. */
#define USB_MASS_STORAGE_RESET          (0xFF00)    /* Mass storage reset request */
#define USB_GET_MAX_LUN                 (0xFE00)    /* Get max logical unit number */

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

#endif  /* USB_PMSC_MINI_IF_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/

