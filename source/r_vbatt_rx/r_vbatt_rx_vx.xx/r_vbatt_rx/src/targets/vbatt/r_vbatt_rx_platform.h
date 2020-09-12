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
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.06.2015 1.00     First Release
*              : 30.06.2019 1.04     Added support for RX23W
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx_platform.h
* Description  : Functions for using Battery Backup Function on RX devices. 
*******************************************************************************/
#ifndef R_VBATT_RX_PLATFORM_H
#define R_VBATT_RX_PLATFORM_H

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Include specifics for chosen MCU.  */
#if defined(BSP_MCU_RX231)
    #include "./rx231/r_vbatt_rx231_private.h"
#elif defined(BSP_MCU_RX230)
    #include "./rx230/r_vbatt_rx230_private.h"
#elif defined(BSP_MCU_RX23W)
    #include "./rx23w/r_vbatt_rx23w_private.h"
#else
    #error "This MCU is not supported by the current r_vbatt_rx module."
#endif


#endif /* R_VBATT_RX_PLATFORM_H */

/* End of File */

