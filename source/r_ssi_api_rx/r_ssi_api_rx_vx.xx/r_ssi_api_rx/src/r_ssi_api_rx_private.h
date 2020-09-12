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
* Copyright (C) 2014-2019 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name    : r_ssi_api_rx_private.h
* Version      : 1.24
* Device       : RX64M, RX71M, RX113, RX231, RX230, RX23W
* Tool-Chain   : RX Family C Compiler
*                GCC for Renesas RX
*                IAR C/C++ Compiler for Renesas RX
* Description  : SSI driver module file for RX MCUs.
*******************************************************************************
* History : DD.MM.YYYY  Version   Description
*         : 01.08.2014   1.00      First release.
*         : 11.12.2014   1.11      RX71M support added.
*         : 28.04.2015   1.20      RX113, RX231, RX230 support added.
*         : 07.04.2017   1.21      Version number updated.
*         : 01.02.2019   1.22      Changed Minor version to 1.22.
*         : 20.05.2019   1.23      Added support for the GCC and IAR compiler.
*         : 20.06.2019   1.24      RX23W support added.
*
******************************************************************************/

#ifndef SSI_API_RX_PRIVATE_H_
#define SSI_API_RX_PRIVATE_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Get r_bsp info. */
#include "platform.h"

#if (defined(SSI0) || defined(SSI1))
#include "r_ssi_api_rx_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
  #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

#if defined(__CCRX__)
  #define R_SSI_EVENACCESS    __evenaccess
#elif defined(__GNUC__)
  #define R_SSI_EVENACCESS    /* none */
#elif defined(__ICCRX__)
  #define R_SSI_EVENACCESS    __sfr
#endif

/******************************************************************************
  Operands and values just used for functions in r_ssi_api_rx.c.
******************************************************************************/
/* Values to be set to SSI_IO_MODE. */
#define SSI_IO_MODE_NONE            (0u)    /* SSI not to use            */
#define SSI_IO_MODE_RX              (1u)    /* SSI to Receive data       */
#define SSI_IO_MODE_TX              (2u)    /* SSI to Transmit data      */
#define SSI_IO_MODE_TR              (3u)    /* SSI to Receive & Transmit */
        
/* Values to be set to SSICR. */
#define SSI_SSICR_REN_DISABLE       (0u)
#define SSI_SSICR_REN_ENABLE        (1u)
#define SSI_SSICR_TEN_DISABLE       (0u)
#define SSI_SSICR_TEN_ENABLE        (1u)
#define SSI_SSICR_DWL08             (0u)
#define SSI_SSICR_DWL16             (1u)
#define SSI_SSICR_DWL18             (2u)
#define SSI_SSICR_DWL20             (3u)
#define SSI_SSICR_DWL22             (4u)
#define SSI_SSICR_DWL24             (5u)
#define SSI_SSICR_IIEN_DISABLE      (0u)
#define SSI_SSICR_IIEN_ENABLE       (1u)
#define SSI_SSICR_ROIEN_DISABLE     (0u)
#define SSI_SSICR_ROIEN_ENABLE      (1u)
#define SSI_SSICR_RUIEN_DISABLE     (0u)
#define SSI_SSICR_RUIEN_ENABLE      (1u)
#define SSI_SSICR_TOIEN_DISABLE     (0u)
#define SSI_SSICR_TOIEN_ENABLE      (1u)
#define SSI_SSICR_TUIEN_DISABLE     (0u)
#define SSI_SSICR_TUIEN_ENABLE      (1u)

/* Shift operands to make up SSICR value. */
#define SSI_REN_SHIFT               (0u)
#define SSI_TEN_SHIFT               (1u)
#define SSI_IIEN_SHIFT              (25u)
#define SSI_ROIEN_SHIFT             (26u)
#define SSI_RUIEN_SHIFT             (27u)
#define SSI_TOIEN_SHIFT             (28u)
#define SSI_TUIEN_SHIFT             (29u)
#define SSI_CKDV_SHIFT              (4u)
#define SSI_DEL_SHIFT               (8u)
#define SSI_PDTA_SHIFT              (9u)
#define SSI_SDTA_SHIFT              (10u)
#define SSI_SPDP_SHIFT              (11u)
#define SSI_SWSP_SHIFT              (12u)
#define SSI_SCKP_SHIFT              (13u)
#define SSI_SWSD_SHIFT              (14u)
#define SSI_SCKD_SHIFT              (15u)
#define SSI_SWL_SHIFT               (16u)
#define SSI_DWL_SHIFT               (19u)


/* Values shown on SSICR after reset. */
#define SSI_SSISR_RESET_VALUE       (0x02000013u)

/* Values to be shown on SSISR. */
#define SSI_SSISR_IDST_BUSY         (0u)
#define SSI_SSISR_IDST_STOP         (1u)
#define SSI_SSISR_IIRQ_OFF          (0u)
#define SSI_SSISR_IIRQ_ON           (1u)
#define SSI_SSISR_ROIRQ_OFF         (0u)
#define SSI_SSISR_ROIRQ_ON          (1u)
#define SSI_SSISR_RUIRQ_OFF         (0u)
#define SSI_SSISR_RUIRQ_ON          (1u)
#define SSI_SSISR_TOIRQ_OFF         (0u)
#define SSI_SSISR_TOIRQ_ON          (1u)
#define SSI_SSISR_TUIRQ_OFF         (0u)
#define SSI_SSISR_TUIRQ_ON          (1u)
#define SSI_SSISR_TSWNO_OFF         (0u)
#define SSI_SSISR_TSWNO_ON          (1u)

/* Values to be set to SSIFCR. */
#define SSI_SSIFCR_RFRST_DISABLE    (0u)
#define SSI_SSIFCR_RFRST_ENABLE     (1u)
#define SSI_SSIFCR_TFRST_DISABLE    (0u)
#define SSI_SSIFCR_TFRST_ENABLE     (1u)
#define SSI_SSIFCR_RIE_DISABLE      (0u)
#define SSI_SSIFCR_RIE_ENABLE       (1u)
#define SSI_SSIFCR_TIE_DISABLE      (0u)
#define SSI_SSIFCR_TIE_ENABLE       (1u)

/* Shift operands to make up SSIFCR value. */
#define SSI_RFRST_SHIFT             (0u)
#define SSI_TFRST_SHIFT             (1u)
#define SSI_RIE_SHIFT               (2u)
#define SSI_TIE_SHIFT               (3u)
#define SSI_RTRG_SHIFT              (4u)
#define SSI_TTRG_SHIFT              (6u)
#define SSI_AUCKE_SHIFT             (31u)

/* Values to be shown on SSIFSR. */
#define SSI_SSIFSR_RDF_OFF          (0u)
#define SSI_SSIFSR_RDF_ON           (1u)
#define SSI_SSIFSR_RDC0             (0u)
#define SSI_SSIFSR_RDC1             (1u)
#define SSI_SSIFSR_RDC2             (2u)
#define SSI_SSIFSR_RDC3             (3u)
#define SSI_SSIFSR_RDC4             (4u)
#define SSI_SSIFSR_RDC5             (5u)
#define SSI_SSIFSR_RDC6             (6u)
#define SSI_SSIFSR_RDC7             (7u)
#define SSI_SSIFSR_RDC8             (8u)
#define SSI_SSIFSR_RDCMIN           SSI_SSIFSR_RDC0
#define SSI_SSIFSR_RDCMAX           SSI_SSIFSR_RDC8
#define SSI_SSIFSR_TDE_OFF          (0u)
#define SSI_SSIFSR_TDE_ON           (1u)
#define SSI_SSIFSR_TDC0             (0u)
#define SSI_SSIFSR_TDC1             (1u)
#define SSI_SSIFSR_TDC2             (2u)
#define SSI_SSIFSR_TDC3             (3u)
#define SSI_SSIFSR_TDC4             (4u)
#define SSI_SSIFSR_TDC5             (5u)
#define SSI_SSIFSR_TDC6             (6u)
#define SSI_SSIFSR_TDC7             (7u)
#define SSI_SSIFSR_TDC8             (8u)
#define SSI_SSIFSR_TDCMIN           SSI_SSIFSR_TDC0
#define SSI_SSIFSR_TDCMAX           SSI_SSIFSR_TDC8


#ifdef SSI_STANDARD_CONFIG
/******************************************************************************

    "Standarad configuration" 
       SSI register value calculations using macro definition of 
       "Standard configuration" described in "r_ssi_api_rx_config.h".

******************************************************************************/
/******************************************************************************
    Calculations to get values to set SSI peripheral registers.
******************************************************************************/
/*********************************************************
    Elements to calclate SSI register value calculation.
*********************************************************/
/* The "SSIn_DWL_VALUE" is just used in this file locally. */
#define SSI_CH0_DWL_VALUE           (SSI_CH0_DATA_WIDTH)    /* Ch0 DataWord length */
#define SSI_CH1_DWL_VALUE           (SSI_CH1_DATA_WIDTH)    /* Ch1 DataWord length */

/* The "SSIn_CKDV_VALUE" is just used in this file locally. */
#define SSI_CH0_CKDV_VALUE          (SSI_MCLK / SSI_CH0_BCLK)   /* Ch0 Division Ratio */
#define SSI_CH1_CKDV_VALUE          (SSI_MCLK / SSI_CH1_BCLK)   /* Ch1 Division Ratio */

/*********************************************************
    SSIn.SSIFCR.TTRG value calculation.
*********************************************************/
/************************
*      Channel 0        *
************************/
/* The "SSIn_TTRG_FIFO" is just used in this file locally. */
/* case PCM data word length : 8bit */
#if ((SSI_CH0_DWL_VALUE == 8u) && ((SSI_CH0_TTRG_NUMBER == 12u)     \
                                || (SSI_CH0_TTRG_NUMBER == 8u)      \
                                || (SSI_CH0_TTRG_NUMBER == 4u))) 
    #define SSI_CH0_TTRG_FIFO       (SSI_CH0_TTRG_NUMBER / 2u)

/* case PCM data word length : 16bit */
#elif ((SSI_CH0_DWL_VALUE == 16u) && ((SSI_CH0_TTRG_NUMBER == 6u)   \
                                || (SSI_CH0_TTRG_NUMBER == 4u)      \
                                || (SSI_CH0_TTRG_NUMBER == 2u)))
    #define SSI_CH0_TTRG_FIFO       (SSI_CH0_TTRG_NUMBER)

/* case PCM data word length : 18, 20, 22, 24bit */
#elif (((SSI_CH0_DWL_VALUE == 18u) || (SSI_CH0_DWL_VALUE == 20u)    \
                                || (SSI_CH0_DWL_VALUE == 22u)       \
                                || (SSI_CH0_DWL_VALUE == 24u))      \
    && ((SSI_CH0_TTRG_NUMBER == 3u) || (SSI_CH0_TTRG_NUMBER == 2u)  \
                                || (SSI_CH0_TTRG_NUMBER == 1u)))
    #define SSI_CH0_TTRG_FIFO       (SSI_CH0_TTRG_NUMBER * 2u)

/* case illegal value was set */
#else
    #define SSI_CH0_TTRG_FIFO       (0xFFFFFFFFu)

#endif

/* The "SSI_CH0_TTRG" is plactically used in SSI functions. */
/* case trigger level : 6 */
#if (SSI_CH0_TTRG_FIFO == 6u)
    /* The "SSI_CH0_TTRG" is just used in this file locally. */
    #define SSI_CH0_TTRG            (0x1u)
/* case trigger level 4 */
#elif (SSI_CH0_TTRG_FIFO == 4u)
    #define SSI_CH0_TTRG            (0x2u)
/* case trigger level : 2 */
#elif (SSI_CH0_TTRG_FIFO == 2u)
    #define SSI_CH0_TTRG            (0x3u)
/* case illegal value */
#else
    #define SSI_CH0_TTRG            (0xFFFFFFFFu)
#endif
                            
/************************
*      Channel 1        *
************************/
/* The "SSI_CH1_TTRG_FIFO" is just used in this file locally. */
/* case PCM data word length : 8bit */
#if ((SSI_CH1_DWL_VALUE == 8u) && ((SSI_CH1_TTRG_NUMBER == 12u)     \
                                || (SSI_CH1_TTRG_NUMBER == 8u)      \
                                || (SSI_CH1_TTRG_NUMBER == 4u)))
                                
    /* following definition is just used in this file to get SSI_CH1_TTRG. */
    #define SSI_CH1_TTRG_FIFO      (SSI_CH1_TTRG_NUMBER / 2u)

/* case PCM data word length : 16bit */
#elif ((SSI_CH1_DWL_VALUE == 16u) && ((SSI_CH1_TTRG_NUMBER == 6u)   \
                                || (SSI_CH1_TTRG_NUMBER == 4u)      \
                                || (SSI_CH1_TTRG_NUMBER == 2u)))
    /* following definition is just used in this file to get SSI_CH1_TTRG. */
    #define SSI_CH1_TTRG_FIFO      (SSI_CH1_TTRG_NUMBER)

/* case PCM data word length : 18, 20, 22, 24bit */
#elif (((SSI_CH1_DWL_VALUE == 18u) || (SSI_CH1_DWL_VALUE == 20u)    \
                                || (SSI_CH1_DWL_VALUE == 22u)       \
                                || (SSI_CH1_DWL_VALUE == 24u))      \
    && ((SSI_CH1_TTRG_NUMBER == 3) || (SSI_CH1_TTRG_NUMBER == 2u)   \
                                || (SSI_CH1_TTRG_NUMBER == 1u)))
    /* following definition is just used in this file to get SSI_CH1_TTRG. */
    #define SSI_CH1_TTRG_FIFO      (SSI_CH1_TTRG_NUMBER * 2u)

/* case illegal value was set */
#else
    /* following definition is just used in this file. */
    #define SSI_CH1_TTRG_FIFO       (0xFFFFFFFFu)

#endif

/* The "SSI_CH1_TTRG" is plactically used in SSI functions. */
/* case trigger level : 6 */
#if (SSI_CH1_TTRG_FIFO == 6u)
    #define SSI_CH1_TTRG        (0x1u)
/* case trigger level : 4 */
#elif (SSI_CH1_TTRG_FIFO == 4u)
    #define SSI_CH1_TTRG        (0x2u)
/* case trigger level : 2 */
#elif (SSI_CH1_TTRG_FIFO == 2u)
    #define SSI_CH1_TTRG        (0x3u)
/* case illegal value */
#else
    #define SSI_CH1_TTRG        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIn.SSIFCR.RTRG value calculation.
*********************************************************/
/************************
*      Channel 0        *
************************/
/* The "SSI_CH0_RTRG_FIFO" is just used in this file locally. */
/* case PCM data word length : 8bit */
#if ((SSI_CH0_DWL_VALUE == 8u) && ((SSI_CH0_RTRG_NUMBER == 4u)      \
                                || (SSI_CH0_RTRG_NUMBER == 8u)      \
                                || (SSI_CH0_RTRG_NUMBER == 12u)))
    #define SSI_CH0_RTRG_FIFO      (SSI_CH0_RTRG_NUMBER / 2u)

/* case PCM data word length : 16bit */
#elif ((SSI_CH0_DWL_VALUE == 16u) && ((SSI_CH0_RTRG_NUMBER == 2u)   \
                                || (SSI_CH0_RTRG_NUMBER == 4u)      \
                                || (SSI_CH0_RTRG_NUMBER == 6u)))
    #define SSI_CH0_RTRG_FIFO      (SSI_CH0_RTRG_NUMBER)

/* case PCM data word length : 18, 20, 22, 24bit */
#elif (((SSI_CH0_DWL_VALUE == 18u) || ((SSI_CH0_DWL_VALUE == 20u)    \
                                || (SSI_CH0_DWL_VALUE == 22u)       \
                                || (SSI_CH0_DWL_VALUE == 24u)))      \
    && ((SSI_CH0_RTRG_NUMBER == 1u) || (SSI_CH0_RTRG_NUMBER == 2u)  \
                                || (SSI_CH0_RTRG_NUMBER == 3u)))  
    #define SSI_CH0_RTRG_FIFO      (SSI_CH0_RTRG_NUMBER * 2u)

/* case illegal value was set */
#else
    #define SSI_CH0_RTRG_FIFO      (0xFFFFFFFFu)
#endif

/* case trigger level : 2 */
#if (SSI_CH0_RTRG_FIFO == 2u)
    #define SSI_CH0_RTRG        (0x1u)
/* case trigger level : 4 */
#elif (SSI_CH0_RTRG_FIFO == 4u)
    #define SSI_CH0_RTRG        (0x2u)
/* case trigger level : 6 */
#elif (SSI_CH0_RTRG_FIFO == 6u)
    #define SSI_CH0_RTRG        (0x3u)
/* case illegal value */
#else
    #define SSI_CH0_RTRG            (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
/* The "SSI_CH1_RTRG_FIFO" is just used in this file locally. */
/* case PCM data word length : 8bit */
#if ((SSI_CH1_DWL_VALUE == 8u) && ((SSI_CH1_RTRG_NUMBER == 4u)      \
                                || (SSI_CH1_RTRG_NUMBER == 8u)      \
                                || (SSI_CH1_RTRG_NUMBER == 12u)))
    #define SSI_CH1_RTRG_FIFO      (SSI_CH1_RTRG_NUMBER / 2u)

/* case PCM data word length : 16bit */
#elif ((SSI_CH1_DWL_VALUE == 16u) && ((SSI_CH1_RTRG_NUMBER == 2u)   \
                                || (SSI_CH1_RTRG_NUMBER == 4u)      \
                                || (SSI_CH1_RTRG_NUMBER == 6u)))
    #define SSI_CH1_RTRG_FIFO      (SSI_CH1_RTRG_NUMBER)

/* case PCM data word length : 18, 20, 22, 24bit */
#elif (((SSI_CH1_DWL_VALUE == 18u) || (SSI_CH1_DWL_VALUE == 20u)    \
                                || (SSI_CH1_DWL_VALUE == 22u)       \
                                || (SSI_CH1_DWL_VALUE == 24u))      \
    && ((SSI_CH1_RTRG_NUMBER == 1u) || (SSI_CH1_RTRG_NUMBER == 2u)  \
                                || (SSI_CH1_RTRG_NUMBER == 3u)))
    #define SSI_CH1_RTRG_FIFO      (SSI_CH1_RTRG_NUMBER * 2u)

/* case illegal value was set */
#else
    #define SSI_CH1_RTRG_FIFO      (0xFFFFFFFFu)

#endif

/* The "SSI_CH1_RTRG" is plactically used in SSI functions. */
/* case trigger level : 2 */
#if (SSI_CH1_RTRG_FIFO == 2u)
    #define SSI_CH1_RTRG        (0x1u)
/* case trigger level : 2 */
#elif (SSI_CH1_RTRG_FIFO == 4u)
    #define SSI_CH1_RTRG        (0x2u)
/* case trigger level : 6 */
#elif (SSI_CH1_RTRG_FIFO == 6u)
    #define SSI_CH1_RTRG        (0x3u)
/* case illegal value */
#else
    #define SSI_CH1_RTRG        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIn.SSICR.DEL, PDTA, SDTA, SPDP, SWSP, SCKP calculation
*********************************************************/
/*  SSIn_DEL,PDTA,SDTA,SPDP,SWSP,SCKP are chosen
      correspnding to SSIn_DWL_VALUE value.
*/
/************************
*      Channel 0        *
************************/
#if ((SSI_CH0_DWL_VALUE <= 16u) && (SSI_CH0_SERIAL_IF_FMT == 0u))
    #define SSI_CH0_DEL         (0u)
    #define SSI_CH0_PDTA        (0u)
    #define SSI_CH0_SDTA        (0u)
    #define SSI_CH0_SPDP        (0u)
    #define SSI_CH0_SWSP        (0u)
    #define SSI_CH0_SCKP        (0u)
#elif ((SSI_CH0_DWL_VALUE <= 16u) && (SSI_CH0_SERIAL_IF_FMT == 1u))
    #define SSI_CH0_DEL         (1u)
    #define SSI_CH0_PDTA        (0u)
    #define SSI_CH0_SDTA        (0u)
    #define SSI_CH0_SPDP        (0u)
    #define SSI_CH0_SWSP        (1u)
    #define SSI_CH0_SCKP        (0u)
#elif ((SSI_CH0_DWL_VALUE <= 16u) && (SSI_CH0_SERIAL_IF_FMT == 2u))
    #define SSI_CH0_DEL         (1u)
    #define SSI_CH0_PDTA        (0u)
    #define SSI_CH0_SDTA        (1u)
    #define SSI_CH0_SPDP        (0u)
    #define SSI_CH0_SWSP        (1u)
    #define SSI_CH0_SCKP        (0u)
#elif ((SSI_CH0_DWL_VALUE >= 18u) && (SSI_CH0_SERIAL_IF_FMT == 0u))
    #define SSI_CH0_DEL         (0u)
    #define SSI_CH0_PDTA        (1u)
    #define SSI_CH0_SDTA        (0u)
    #define SSI_CH0_SPDP        (0u)
    #define SSI_CH0_SWSP        (0u)
    #define SSI_CH0_SCKP        (0u)
#elif ((SSI_CH0_DWL_VALUE >= 18u) && (SSI_CH0_SERIAL_IF_FMT == 1u))
    #define SSI_CH0_DEL         (1u)
    #define SSI_CH0_PDTA        (1u)
    #define SSI_CH0_SDTA        (0u)
    #define SSI_CH0_SPDP        (0u)
    #define SSI_CH0_SWSP        (1u)
    #define SSI_CH0_SCKP        (0u)
#elif ((SSI_CH0_DWL_VALUE >= 18u) && (SSI_CH0_SERIAL_IF_FMT == 2u))
    #define SSI_CH0_DEL         (1u)
    #define SSI_CH0_PDTA        (1u)
    #define SSI_CH0_SDTA        (1u)
    #define SSI_CH0_SPDP        (0u)
    #define SSI_CH0_SWSP        (1u)
    #define SSI_CH0_SCKP        (0u)
#else 
    /* case error */
    #define SSI_CH0_DEL         (0xFFFFFFFFu)
    #define SSI_CH0_PDTA        (0xFFFFFFFFu)
    #define SSI_CH0_SDTA        (0xFFFFFFFFu)
    #define SSI_CH0_SPDP        (0xFFFFFFFFu)
    #define SSI_CH0_SWSP        (0xFFFFFFFFu)
    #define SSI_CH0_SCKP        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if ((SSI_CH1_DWL_VALUE <= 16u) && (SSI_CH1_SERIAL_IF_FMT == 0u))
    #define SSI_CH1_DEL         (0u)
    #define SSI_CH1_PDTA        (0u)
    #define SSI_CH1_SDTA        (0u)
    #define SSI_CH1_SPDP        (0u)
    #define SSI_CH1_SWSP        (0u)
    #define SSI_CH1_SCKP        (0u)
#elif ((SSI_CH1_DWL_VALUE <= 16u) && (SSI_CH1_SERIAL_IF_FMT == 1u))
    #define SSI_CH1_DEL         (1u)
    #define SSI_CH1_PDTA        (0u)
    #define SSI_CH1_SDTA        (0u)
    #define SSI_CH1_SPDP        (0u)
    #define SSI_CH1_SWSP        (1u)
    #define SSI_CH1_SCKP        (0u)
#elif ((SSI_CH1_DWL_VALUE <= 16u) && (SSI_CH1_SERIAL_IF_FMT == 2u))
    #define SSI_CH1_DEL         (1u)
    #define SSI_CH1_PDTA        (0u)
    #define SSI_CH1_SDTA        (1u)
    #define SSI_CH1_SPDP        (0u)
    #define SSI_CH1_SWSP        (1u)
    #define SSI_CH1_SCKP        (0u)
#elif ((SSI_CH1_DWL_VALUE >= 18u) && (SSI_CH1_SERIAL_IF_FMT == 0u))
    #define SSI_CH1_DEL         (0u)
    #define SSI_CH1_PDTA        (1u)
    #define SSI_CH1_SDTA        (0u)
    #define SSI_CH1_SPDP        (0u)
    #define SSI_CH1_SWSP        (0u)
    #define SSI_CH1_SCKP        (0u)
#elif ((SSI_CH1_DWL_VALUE >= 18u) && (SSI_CH1_SERIAL_IF_FMT == 1u))
    #define SSI_CH1_DEL         (1u)
    #define SSI_CH1_PDTA        (1u)
    #define SSI_CH1_SDTA        (0u)
    #define SSI_CH1_SPDP        (0u)
    #define SSI_CH1_SWSP        (1u)
    #define SSI_CH1_SCKP        (0u)
#elif ((SSI_CH1_DWL_VALUE >= 18u) && (SSI_CH1_SERIAL_IF_FMT == 2u))
    #define SSI_CH1_DEL         (1u)
    #define SSI_CH1_PDTA        (1u)
    #define SSI_CH1_SDTA        (1u)
    #define SSI_CH1_SPDP        (0u)
    #define SSI_CH1_SWSP        (1u)
    #define SSI_CH1_SCKP        (0u)
#else
    /* case error */
    #define SSI_CH1_DEL         (0xFFFFFFFFu)
    #define SSI_CH1_PDTA        (0xFFFFFFFFu)
    #define SSI_CH1_SDTA        (0xFFFFFFFFu)
    #define SSI_CH1_SPDP        (0xFFFFFFFFu)
    #define SSI_CH1_SWSP        (0xFFFFFFFFu)
    #define SSI_CH1_SCKP        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIn.SSICR.SWL calculation
*********************************************************/
/*  SSIn_SWL is chosen correspnding to SSIn_BCLK value. */
/************************
*      Channel 0        *
************************/
#if (SSI_CH0_BCLK == 16u)
    #define SSI_CH0_SWL         (0x0u)
#elif (SSI_CH0_BCLK == 32u)
    #define SSI_CH0_SWL         (0x1u)
#elif (SSI_CH0_BCLK == 48u)
    #define SSI_CH0_SWL         (0x2u)
#elif (SSI_CH0_BCLK == 64u)
    #define SSI_CH0_SWL         (0x3u)
#else
    #define SSI_CH0_SWL         (0xFFFFFFFFu)
#endif

/************************
*      Channel 0        *
************************/
#if (SSI_CH1_BCLK == 16u)
    #define SSI_CH1_SWL         (0x0u)
#elif (SSI_CH1_BCLK == 32u)
    #define SSI_CH1_SWL         (0x1u)
#elif (SSI_CH1_BCLK == 48u)
    #define SSI_CH1_SWL         (0x2u)
#elif (SSI_CH1_BCLK == 64u)
    #define SSI_CH1_SWL         (0x3u)
#else
    #define SSI_CH1_SWL         (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIn.SSICR.DWL calculation
*********************************************************/
/*  SSIn_DWL is chosen correspnding to SSIn_DWL_VALUE value. */
/************************
*      Channel 0        *
************************/
#if (SSI_CH0_DWL_VALUE == 8u)
    #define SSI_CH0_DWL         (0x0u)
#elif (SSI_CH0_DWL_VALUE == 16u)
    #define SSI_CH0_DWL         (0x1u)
#elif (SSI_CH0_DWL_VALUE == 18u)
    #define SSI_CH0_DWL         (0x2u)
#elif (SSI_CH0_DWL_VALUE == 20u)
    #define SSI_CH0_DWL         (0x3u)
#elif (SSI_CH0_DWL_VALUE == 22u)
    #define SSI_CH0_DWL         (0x4u)
#elif (SSI_CH0_DWL_VALUE == 24u)
    #define SSI_CH0_DWL         (0x5u)
#else
    /* case error */
    #define SSI_CH0_DWL         (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if (SSI_CH1_DWL_VALUE == 8u)
    #define SSI_CH1_DWL         (0x0u)
#elif (SSI_CH1_DWL_VALUE == 16u)
    #define SSI_CH1_DWL         (0x1u)
#elif (SSI_CH1_DWL_VALUE == 18u)
    #define SSI_CH1_DWL         (0x2u)
#elif (SSI_CH1_DWL_VALUE == 20u)
    #define SSI_CH1_DWL         (0x3u)
#elif (SSI_CH1_DWL_VALUE == 22u)
    #define SSI_CH1_DWL         (0x4u)
#elif (SSI_CH1_DWL_VALUE == 24u)
    #define SSI_CH1_DWL         (0x5u)
#else
    /* case error */
    #define SSI_CH1_DWL         (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIn.SSICR.SWSD,SCKD,AUCKE calculation
*********************************************************/
/*  SSIn_SWSD, SCKD, AUCKE are chosen correspnding to 
      SSIn_CLK_MODE value.
*/
/************************
*      Channel 0        *
************************/
#if (SSI_CH0_CLK_MODE == 0u)
    /* Master mode */
    #define SSI_CH0_SWSD        (1u)
    #define SSI_CH0_SCKD        (1u)
    #define SSI_CH0_AUCKE       (1u)
#elif (SSI_CH0_CLK_MODE == 1u)
    /* Slave mode */
    #define SSI_CH0_SWSD        (0u)
    #define SSI_CH0_SCKD        (0u)
    #define SSI_CH0_AUCKE       (0u)
#else 
    /* case error */
    #define SSI_CH0_SWSD        (0xFFFFFFFFu)
    #define SSI_CH0_SCKD        (0xFFFFFFFFu)
    #define SSI_CH0_AUCKE       (0xFFFFFFFFu)
#endif
                            
/************************
*      Channel 1        *
************************/
#if (SSI_CH1_CLK_MODE == 0u)
    /* Master mode */
    #define SSI_CH1_SWSD        (1u)
    #define SSI_CH1_SCKD        (1u)
    #define SSI_CH1_AUCKE       (1u)
#elif (SSI_CH1_CLK_MODE == 1u)
    /* Slave mode */
    #define SSI_CH1_SWSD        (0u)
    #define SSI_CH1_SCKD        (0u)
    #define SSI_CH1_AUCKE       (0u)
#else 
    /* case error */
    #define SSI_CH1_SWSD        (0xFFFFFFFFu)
    #define SSI_CH1_SCKD        (0xFFFFFFFFu)
    #define SSI_CH1_AUCKE       (0xFFFFFFFFu)
#endif

/*********************************************************
    SSI0.SSICR.CKDV calculation
*********************************************************/
/*  SSIn_CKDV is chosen correspnding to SSIn_CKDV_VALUE value. */
/************************
*      Channel 0        *
************************/
#if (SSI_CH0_CKDV_VALUE == 1u)
    #define SSI_CH0_CKDV        (0x0u)
#elif (SSI_CH0_CKDV_VALUE == 2u)
    #define SSI_CH0_CKDV        (0x1u)
#elif (SSI_CH0_CKDV_VALUE == 4u)
    #define SSI_CH0_CKDV        (0x2u)
#elif (SSI_CH0_CKDV_VALUE == 8u)
    #define SSI_CH0_CKDV        (0x3u)
#elif (SSI_CH0_CKDV_VALUE == 16u)
    #define SSI_CH0_CKDV        (0x4u)
#elif (SSI_CH0_CKDV_VALUE == 32u)
    #define SSI_CH0_CKDV        (0x5u)
#elif (SSI_CH0_CKDV_VALUE == 64u)
    #define SSI_CH0_CKDV        (0x6u)
#elif (SSI_CH0_CKDV_VALUE == 128u)
    #define SSI_CH0_CKDV        (0x7u)
#elif (SSI_CH0_CKDV_VALUE == 6u)
    #define SSI_CH0_CKDV        (0x8u)
#elif (SSI_CH0_CKDV_VALUE == 12u)
    #define SSI_CH0_CKDV        (0x9u)
#elif (SSI_CH0_CKDV_VALUE == 24u)
    #define SSI_CH0_CKDV        (0xAu)
#elif (SSI_CH0_CKDV_VALUE == 48u)
    #define SSI_CH0_CKDV        (0xBu)
#elif (SSI_CH0_CKDV_VALUE == 96u)
    #define SSI_CH0_CKDV        (0xCu)
#else
    /* case error */
    #define SSI_CH0_CKDV        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if (SSI_CH1_CKDV_VALUE == 1u)
    #define SSI_CH1_CKDV        (0x0u)
#elif (SSI_CH1_CKDV_VALUE == 2u)
    #define SSI_CH1_CKDV        (0x1u)
#elif (SSI_CH1_CKDV_VALUE == 4u)
    #define SSI_CH1_CKDV        (0x2u)
#elif (SSI_CH1_CKDV_VALUE == 8u)
    #define SSI_CH1_CKDV        (0x3u)
#elif (SSI_CH1_CKDV_VALUE == 16u)
    #define SSI_CH1_CKDV        (0x4u)
#elif (SSI_CH1_CKDV_VALUE == 32u)
    #define SSI_CH1_CKDV        (0x5u)
#elif (SSI_CH1_CKDV_VALUE == 64u)
    #define SSI_CH1_CKDV        (0x6u)
#elif (SSI_CH1_CKDV_VALUE == 128u)
    #define SSI_CH1_CKDV        (0x7u)
#elif (SSI_CH1_CKDV_VALUE == 6u)
    #define SSI_CH1_CKDV        (0x8u)
#elif (SSI_CH1_CKDV_VALUE == 24u)
    #define SSI_CH1_CKDV        (0xAu)
#else
    /* case error */
    #define SSI_CH1_CKDV        (0xFFFFFFFFu)
#endif
                                    
/******************************************************************************
    End of SSI register value calculation for "Standarad configuration".
******************************************************************************/

/* SSI_STANDARD_CONFIG */
#else
/* SSI_USER_UNIQUE_CONFIG */

/******************************************************************************

    "User unique configuration"
       Elements calculation for SSI configuration error checking.

******************************************************************************/
/* SSIn_TTRG_FIFO & SSIn_RTRG_FIFO are just used for configuration check.
   SSIn_TTRG_FIFO value is chosen correspnding to SSIn_TTRG value.
     SSIn_RTRG_FIFO value is chosen correspnding to SSIn_RTRG value.
*/
/************************
*      Channel 0        *
************************/
#if (SSI_CH0_TTRG == 0x1u)
    #define SSI_CH0_TTRG_FIFO       (6u)
#elif (SSI_CH0_TTRG == 0x2u)
    #define SSI_CH0_TTRG_FIFO       (4u)
#elif (SSI_CH0_TTRG == 0x3u)
    #define SSI_CH0_TTRG_FIFO       (2u)
#else
    /* case error */
    #define SSI_CH0_TTRG_FIFO       (0xFFFFFFFFu)
#endif

/* SSI_CH0_RTRG_FIFO calculation */
#if (SSI_CH0_RTRG == 0x0u)
    /* case error */
    #define SSI_CH0_RTRGFIFO        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if (SSI_CH1_TTRG == 0x1u)
    #define SSI_CH1_TTRG_FIFO      (6u)
#elif (SSI_CH1_TTRG == 0x2u)
    #define SSI_CH1_TTRG_FIFO       (4u)
#elif (SSI_CH1_TTRG == 0x3u)
    #define SSI_CH1_TTRG_FIFO       (2u)
#else
    /* case error */
    #define SSI_CH1_TTRG_FIFO       (0xFFFFFFFFu)
#endif

/* SSI_CH1_RTRG_FIFO calculation */
#if (SSI_CH1_RTRG == 0x0u)
    /* case error */
    #define SSI_CH1_RTRGFIFO        (0xFFFFFFFFu)
#endif

/******************************************************************************
    End of SSI element calculation for "Use unique configuration".
******************************************************************************/
#endif


/******************************************************************************
    Error checking
******************************************************************************/
/***************************************
    Checking configured values
***************************************/
/* number to detect error */
#define SSI_CONFIG_ERR_VAL      (0xFFFFFFFFu)

/* "Transmit / Receive mode selection (I/O mode)" checking */
#if (SSI_CH0_IO_MODE > SSI_IO_MODE_TR)
    #error "ERROR !!! The value set to SSI_CH0_IO_MODE is invalid."
#endif

#if (SSI_CH1_IO_MODE > SSI_IO_MODE_TX)
    #error "ERROR !!! The value set to SSI_CH1_IO_MODE is invalid."
#endif

/* Transmit Data Trigger Number (Tx FIFO empty) configuration checking */
#if ((SSI_CH0_TTRG_FIFO == SSI_CONFIG_ERR_VAL)                  \
            || (SSI_CH0_TTRG == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSI_CH0_TTRG is invalid."
#endif

#if ((SSI_CH1_TTRG_FIFO == SSI_CONFIG_ERR_VAL)                  \
            || (SSI_CH1_TTRG == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSI_CH1_TTRG is invalid."
#endif

/* Receive Data Trigger Number configuration checking */
#if ((SSI_CH0_RTRG_FIFO == SSI_CONFIG_ERR_VAL)                  \
            || (SSI_CH0_RTRG == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSI_CH0_RTRG_FIFO is invalid."
#endif

#if ((SSI_CH1_RTRG_FIFO == SSI_CONFIG_ERR_VAL)                  \
            || (SSI_CH1_RTRG == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSI_CH1_RTRG_FIFO is invalid."
#endif

/* Audio serial interface format configuration checking */
#if ((SSI_CH0_DEL == SSI_CONFIG_ERR_VAL) || (SSI_CH0_PDTA == SSI_CONFIG_ERR_VAL)        \
    || (SSI_CH0_SDTA == SSI_CONFIG_ERR_VAL) || (SSI_CH0_SPDP == SSI_CONFIG_ERR_VAL)     \
    || (SSI_CH0_SWSP == SSI_CONFIG_ERR_VAL) || (SSI_CH0_SCKP == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! SSI0 serial interface format configuration is invalid."
#endif

#if ((SSI_CH1_DEL == SSI_CONFIG_ERR_VAL) || (SSI_CH1_PDTA == SSI_CONFIG_ERR_VAL)        \
    || (SSI_CH1_SDTA == SSI_CONFIG_ERR_VAL) || (SSI_CH1_SPDP == SSI_CONFIG_ERR_VAL)     \
    || (SSI_CH1_SWSP == SSI_CONFIG_ERR_VAL) || (SSI_CH1_SCKP == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! SSI1 serial interface format configuration is invalid."
#endif

/* Bit Clock configuration checking */
#if (SSI_CH0_SWL == SSI_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSI_CH0_SWL is invalid."
#endif
#if (SSI_CH1_SWL == SSI_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSI_CH1_SWL is invalid."
#endif

/* PCM data width configuration checking */
#if (SSI_CH0_DWL == SSI_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSI_CH0_DWL is invalid."
#endif
#if (SSI_CH1_DWL == SSI_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSI_CH1_DWL is invalid."
#endif
#if (SSI_CH0_SWL < 2)
    #if (SSI_CH0_DWL > SSI_CH0_SWL)
        #error "ERROR !!! The value of SSI_CH0_DWL must be smaller than SSI_CH0_SWL."
    #endif
#endif
#if (SSI_CH1_SWL < 2)
    #if (SSI_CH1_DWL > SSI_CH1_SWL)
        #error "ERROR !!! The value of SSI_CH1_DWL must be smaller than SSI_CH1_SWL."
    #endif
#endif

/* Clock supply direction (Master or Slave) configuration checking */
#if ((SSI_CH0_SWSD == SSI_CONFIG_ERR_VAL) || (SSI_CH0_SCKD == SSI_CONFIG_ERR_VAL)       \
                                        || (SSI_CH0_AUCKE == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSI_CH0_SWSD, SSI_CH0_SCKD & SSI_CH0_AUCKE are invalid."
#endif
#if ((SSI_CH1_SWSD == SSI_CONFIG_ERR_VAL) || (SSI_CH1_SCKD == SSI_CONFIG_ERR_VAL)       \
                                        || (SSI_CH1_AUCKE == SSI_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSI_CH1_SWSD, SSI_CH1_SCKD & SSI_CH1_AUCKE are invalid."
#endif

/* AUDIO_CLK Division Ratio (to get bit clock) configuration check */
#if (SSI_CH0_CKDV == SSI_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSI_CH0_CKDV is invalid."
#endif
#if (SSI_CH1_CKDV == SSI_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSI_CH1_CKDV is invalid."
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/
/* no typedef defined */

#endif /* SSI_API_RX_PRIVATE_H_ */

#endif
/* End of file */
