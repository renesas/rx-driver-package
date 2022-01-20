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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : QSPI single master driver
* File Name    : r_qspi_smstr_rx_config.h
* Version      : 1.08
* Device       : RX
* Abstract     : Configuration file for QSPI single master driver
* Tool-Chain   : Renesas RXC Toolchain v2.07.00
* OS           : not use
* H/W Platform : not use
* Description  : Configures the QSPI single master driver.
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 24.06.2014 1.05     First Release
*              : 29.08.2014 1.06     Not change program due to changing r_qspi_smstr_private.h.
*              :                     Added demo source for DTC in this driver.
*              : 28.11.2014 1.07     Changed the waiting for transfer completion of DMAC or DTC
*              :                     from the software wait to the timer wait.
*              : 30.01.2015 1.08     Added RX71M.
*              : 31.07.2017 2.08     Supported RX65N-2MB.
*              :                     Fixed to correspond to Renesas coding rule.
*******************************************************************************/
#ifndef QSPI_SMSTR_CONFIG_H
#define QSPI_SMSTR_CONFIG_H

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_qspi_smstr_rx_pin_config.h"


/******************************************************************************
USE FIRMWARE INTEGRATION TECHNOLOGY
******************************************************************************/
#define QSPI_SMSTR_CFG_USE_FIT


/******************************************************************************
SPECIFY CHANNELS TO INCLUDE SOFTWARE SUPPORT
******************************************************************************/
/* If these are defined, then the code for the specified channel is valid.
   If the #define which channel is not supported on the MCU is uncommented,
   then the compile error occurs. */

/* #define for QSPI channel 0 to be valid. */
#define QSPI_SMSTR_CFG_CH0_INCLUDED

/* #define for QSPI channel 1 to be valid. */
/* #define QSPI_SMSTR_CFG_CH1_INCLUDED */


/******************************************************************************
ENABLE DEBUGGING INFORMATION
******************************************************************************/
/* Store error log information with LONGQ driver.*/
/* #define QSPI_SMSTR_CFG_LONGQ_ENABLE */


/******************************************************************************
SET INTERRUPT SOURCE PRIORITY LEVEL OF SPTI AND SPRI USING DMAC/DTC
******************************************************************************/
/* In the case of DMAC/DTC, set the interrupt source priority level of spti and spri. */
#define QSPI_SMSTR_CFG_CH0_INT_SPTI_LEVEL     (10)
#define QSPI_SMSTR_CFG_CH0_INT_SPRI_LEVEL     (10)
#define QSPI_SMSTR_CFG_CH1_INT_SPTI_LEVEL     
#define QSPI_SMSTR_CFG_CH1_INT_SPRI_LEVEL     


#endif /* _QSPI_API_CONFIG_H */

/* End of File */

