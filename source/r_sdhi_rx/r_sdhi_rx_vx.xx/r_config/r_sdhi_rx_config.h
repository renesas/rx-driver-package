/**********************************************************************************************************************
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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdhi_rx_config.h
* Device       : RX Family MCU
* Tool-Chain   : RX Family C Compiler
* H/W Platform : 
* Description  : Interface file for SDHI API for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 31.07.2017 2.00    First Release
**********************************************************************************************************************/

#ifndef SDHI_CONFIG_H
#define SDHI_CONFIG_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/* #define for SDHI channel to be valid */
#define SDHI_CFG_CH0_INCLUDED           (1)         /* SDHI Channel 0 Active */
#define SDHI_CFG_CH1_INCLUDED           (0)         /* SDHI Channel 1 Active */

#if (SDHI_CFG_CH0_INCLUDED == 1)
#define SDHI_CFG_CH0_CD_ACTIVE          (1)         /* CD PORT ACTIVE */
#define SDHI_CFG_CH0_WP_ACTIVE          (1)         /* WP PORT ACTIVE */
#define SDHI_CFG_CH0_INT_LEVEL          (10)        /* SDHI channel 0 interrupt level */
#define SDHI_CFG_CH0_INT_LEVEL_DMADTC   (10)        /* SDHI channel 0 DMA/DTC interrupt level */
#endif  /* #if (SDHI_CFG_CH0_INCLUDED == 1) */

#if (SDHI_CFG_CH1_INCLUDED == 1)
#define SDHI_CFG_CH1_CD_ACTIVE          (1)         /* CD PORT ACTIVE */
#define SDHI_CFG_CH1_WP_ACTIVE          (1)         /* WP PORT ACTIVE */
#define SDHI_CFG_CH1_INT_LEVEL          (10)        /* SDHI channel 1 interrupt level */
#define SDHI_CFG_CH1_INT_LEVEL_DMADTC   (10)        /* SDHI channel 1 DMA/DTC interrupt level */
#endif  /* #if (SDHI_CFG_CH1_INCLUDED == 1) */

/* #define for SDHI clock DIV */
#define SDHI_CFG_DIV_HIGH_SPEED         (SDHI_DIV_2)
#define SDHI_CFG_DIV_DEFAULT_SPEED      (SDHI_DIV_4)
#define SDHI_CFG_DIV_INIT_SPEED         (SDHI_DIV_256)

/* #define for SDHI time out count */
#define SDHI_CFG_SDOPT_CTOP             (0x000eu)   /* CD time out count       */
#define SDHI_CFG_SDOPT_TOP              (0x00e0u)   /* response time out count */

/* #define for parameter checking */
#define SDHI_CFG_PARAM_CHECKING_ENABLE  (1)         /* Parameter Checking Active */

/* #define SDHI_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* SDHI_CONFIG_H */

