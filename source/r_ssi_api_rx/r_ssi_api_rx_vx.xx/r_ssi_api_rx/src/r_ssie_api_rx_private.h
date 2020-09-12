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
* File Name    : r_ssie_api_rx_private.h
* Version      : 2.00
* Tool-Chain   : RX Family C Compiler
*                GCC for Renesas RX
*                IAR C/C++ Compiler for Renesas RX
* Description  : SSIE driver module file for RX MCUs.
*******************************************************************************
* History : DD.MM.YYYY  Version   Description
*         : 01.08.2014   1.00      First release.
*         : 11.12.2014   1.11      RX71M support added.
*         : 28.04.2015   1.20      RX113, RX231, RX230 support added.
*         : 07.04.2017   1.21      Version number updated.
*         : 01.02.2019   1.22      Changed Minor version to 1.22.
*         : 20.05.2019   1.23      Added support for the GCC and IAR compiler.
*         : 20.06.2019   1.24      RX23W support added.
*         : 25.06.2019   2.00      SSIE support added.
*
******************************************************************************/

#ifndef SSIE_API_RX_PRIVATE_H_
#define SSIE_API_RX_PRIVATE_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Get r_bsp info. */
#include "platform.h"

#if (defined(SSIE0) || defined(SSIE1))
#include "r_ssi_api_rx_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
  #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

#if defined(__CCRX__)
  #define R_SSIE_EVENACCESS    __evenaccess
#elif defined(__GNUC__)
  #define R_SSIE_EVENACCESS    /* none */
#elif defined(__ICCRX__)
  #define R_SSIE_EVENACCESS    __sfr
#endif

/******************************************************************************
  Operands and values just used for functions in r_ssi_api_rx.c.
******************************************************************************/
/* Values to be set to SSIE_IO_MODE. */
#define SSIE_IO_MODE_NONE            (0u)    /* SSIE not to use            */
#define SSIE_IO_MODE_RX              (1u)    /* SSIE to Receive data       */
#define SSIE_IO_MODE_TX              (2u)    /* SSIE to Transmit data      */
#define SSIE_IO_MODE_TR              (3u)    /* SSIE to Receive & Transmit */
        
/* Values to be set to SSICR. */
#define SSIE_SSICR_REN_DISABLE       (0u)
#define SSIE_SSICR_REN_ENABLE        (1u)
#define SSIE_SSICR_TEN_DISABLE       (0u)
#define SSIE_SSICR_TEN_ENABLE        (1u)
#define SSIE_SSICR_DWL08             (0u)
#define SSIE_SSICR_DWL16             (1u)
#define SSIE_SSICR_DWL18             (2u)
#define SSIE_SSICR_DWL20             (3u)
#define SSIE_SSICR_DWL22             (4u)
#define SSIE_SSICR_DWL24             (5u)
#define SSIE_SSICR_DWL32             (6u)
#define SSIE_SSICR_ROIEN_DISABLE     (0u)
#define SSIE_SSICR_ROIEN_ENABLE      (1u)
#define SSIE_SSICR_RUIEN_DISABLE     (0u)
#define SSIE_SSICR_RUIEN_ENABLE      (1u)
#define SSIE_SSICR_TOIEN_DISABLE     (0u)
#define SSIE_SSICR_TOIEN_ENABLE      (1u)
#define SSIE_SSICR_TUIEN_DISABLE     (0u)
#define SSIE_SSICR_TUIEN_ENABLE      (1u)
#define SSIE_SSICR_MST_ON            (1u)

/* Shift operands to make up SSICR value. */
#define SSIE_REN_SHIFT               (0u)
#define SSIE_TEN_SHIFT               (1u)
#define SSIE_CKDV_SHIFT              (4u)
#define SSIE_DEL_SHIFT               (8u)
#define SSIE_PDTA_SHIFT              (9u)
#define SSIE_SDTA_SHIFT              (10u)
#define SSIE_SPDP_SHIFT              (11u)
#define SSIE_LRCKP_SHIFT             (12u)
#define SSIE_BCKP_SHIFT              (13u)
#define SSIE_MST_SHIFT               (14u)
#define SSIE_SWL_SHIFT               (16u)
#define SSIE_DWL_SHIFT               (19u)
#define SSIE_FRM_SHIFT               (22u)
#define SSIE_ROIEN_SHIFT             (26u)
#define SSIE_RUIEN_SHIFT             (27u)
#define SSIE_TOIEN_SHIFT             (28u)
#define SSIE_TUIEN_SHIFT             (29u)

/* Shift operands to make up SSIOFR value. */
#define SSIE_OMOD_SHIFT              (0u)
#define SSIE_BCKASTP_SHIFT           (9u)

/* Values to be shown on SSIOFR. */
#define SSIE_BCKASTP_ON              (0u)
#define SSIE_BCKASTP_OFF             (1u)

/* Values to be shown on SSISR. */
#define SSIE_SSISR_IIRQ_OFF          (0u)
#define SSIE_SSISR_IIRQ_ON           (1u)
#define SSIE_SSISR_ROIRQ_OFF         (0u)
#define SSIE_SSISR_ROIRQ_ON          (1u)
#define SSIE_SSISR_RUIRQ_OFF         (0u)
#define SSIE_SSISR_RUIRQ_ON          (1u)
#define SSIE_SSISR_TOIRQ_OFF         (0u)
#define SSIE_SSISR_TOIRQ_ON          (1u)
#define SSIE_SSISR_TUIRQ_OFF         (0u)
#define SSIE_SSISR_TUIRQ_ON          (1u)

/* Values to be set to SSIFCR. */
#define SSIE_SSIFCR_SSIRST_DISABLE   (0u)
#define SSIE_SSIFCR_SSIRST_ENABLE    (1u)
#define SSIE_SSIFCR_RFRST_DISABLE    (0u)
#define SSIE_SSIFCR_RFRST_ENABLE     (1u)
#define SSIE_SSIFCR_TFRST_DISABLE    (0u)
#define SSIE_SSIFCR_TFRST_ENABLE     (1u)
#define SSIE_SSIFCR_RIE_DISABLE      (0u)
#define SSIE_SSIFCR_RIE_ENABLE       (1u)
#define SSIE_SSIFCR_TIE_DISABLE      (0u)
#define SSIE_SSIFCR_TIE_ENABLE       (1u)

/* Shift operands to make up SSIFCR value. */
#define SSIE_RFRST_SHIFT             (0u)
#define SSIE_TFRST_SHIFT             (1u)
#define SSIE_RIE_SHIFT               (2u)
#define SSIE_TIE_SHIFT               (3u)
#define SSIE_BSW_SHIFT               (11u)
#define SSIE_AUCKE_SHIFT             (31u)

/* Shift operands to make up SSISCR value. */
#define SSIE_RDFS_SHIFT              (0u)
#define SSIE_TDES_SHIFT              (8u)

/* Values to be shown on SSIFSR. */
#define SSIE_SSIFSR_TDCMAX           (32u)


#ifdef SSI_STANDARD_CONFIG
/******************************************************************************

    "Standarad configuration" 
       SSIE register value calculations using macro definition of 
       "Standard configuration" described in "r_ssi_api_rx_config.h".

******************************************************************************/
/******************************************************************************
    Calculations to get values to set SSIE peripheral registers.
******************************************************************************/
/*********************************************************
    Elements to calclate SSIE register value calculation.
*********************************************************/
/* The "SSIEn_DWL_VALUE" is just used in this file locally. */
#define SSIE_CH0_DWL_VALUE           (SSIE_CH0_DATA_WIDTH)    /* Ch0 DataWord length */
#define SSIE_CH1_DWL_VALUE           (SSIE_CH1_DATA_WIDTH)    /* Ch1 DataWord length */

/* The "SSIEn_CKDV_VALUE" is just used in this file locally. */
#define SSIE_CH0_CKDV_VALUE          (SSIE_MCLK / SSIE_CH0_BCLK)   /* Ch0 Division Ratio */
#define SSIE_CH1_CKDV_VALUE          (SSIE_MCLK / SSIE_CH1_BCLK)   /* Ch1 Division Ratio */

/*********************************************************
    SSIEn.SSICR.DEL, PDTA, SDTA, SPDP, LRCKP, BCKP calculation
*********************************************************/
/*  SSIEn_DEL,PDTA,SDTA,SPDP,LRCKP,BCKP are chosen
      correspnding to SSIEn_DWL_VALUE value.
*/
/************************
*      Channel 0        *
************************/
#if ((SSIE_CH0_SERIAL_IF_FMT >= 0u) && (SSIE_CH0_SERIAL_IF_FMT <= 2u))
    #if ((SSIE_CH0_DWL_VALUE == 8u) || (SSIE_CH0_DWL_VALUE == 16u) || (SSIE_CH0_DWL_VALUE == 32u))
        #define SSIE_CH0_PDTA        (0u)
    #elif ((SSIE_CH0_DWL_VALUE == 18u) || (SSIE_CH0_DWL_VALUE == 20u) || (SSIE_CH0_DWL_VALUE == 22u) || (SSIE_CH0_DWL_VALUE == 24u))
        #define SSIE_CH0_PDTA        (1u)
    #else
        #define SSIE_CH0_PDTA        (0xFFFFFFFFu)
    #endif
    #define SSIE_CH0_SPDP        (0u)
#else
    #define SSIE_CH0_PDTA        (0xFFFFFFFFu)
    #define SSIE_CH0_SPDP        (0xFFFFFFFFu)
#endif

#if (SSIE_CH0_SERIAL_IF_FMT == 0u)
    /* I2S */
    #define SSIE_CH0_DEL         (0u)
    #define SSIE_CH0_SDTA        (0u)
    #define SSIE_CH0_LRCKP       (0u)
    #define SSIE_CH0_BCKP        (0u)
    #define SSIE_CH0_FRM         (0u)
    #define SSIE_CH0_OMOD        (0u)
#elif (SSIE_CH0_SERIAL_IF_FMT == 1u)
    /* Left-justified */
    #define SSIE_CH0_DEL         (1u)
    #define SSIE_CH0_SDTA        (0u)
    #define SSIE_CH0_LRCKP       (1u)
    #define SSIE_CH0_BCKP        (0u)
    #define SSIE_CH0_FRM         (0u)
    #define SSIE_CH0_OMOD        (0u)
#elif (SSIE_CH0_SERIAL_IF_FMT == 2u)
    /* Right-justified */
    #define SSIE_CH0_DEL         (1u)
    #define SSIE_CH0_SDTA        (1u)
    #define SSIE_CH0_LRCKP       (1u)
    #define SSIE_CH0_BCKP        (0u)
    #define SSIE_CH0_FRM         (0u)
    #define SSIE_CH0_OMOD        (0u)
#else
    #define SSIE_CH0_DEL         (0xFFFFFFFFu)
    #define SSIE_CH0_SDTA        (0xFFFFFFFFu)
    #define SSIE_CH0_LRCKP       (0xFFFFFFFFu)
    #define SSIE_CH0_BCKP        (0xFFFFFFFFu)
    #define SSIE_CH0_FRM         (0xFFFFFFFFu)
    #define SSIE_CH0_OMOD        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if ((SSIE_CH1_SERIAL_IF_FMT >= 0u) && (SSIE_CH1_SERIAL_IF_FMT <= 2u))
    #if ((SSIE_CH1_DWL_VALUE == 8u) || (SSIE_CH1_DWL_VALUE == 16u) || (SSIE_CH1_DWL_VALUE == 32u))
        #define SSIE_CH1_PDTA        (0u)
    #elif ((SSIE_CH1_DWL_VALUE == 18u) || (SSIE_CH1_DWL_VALUE == 20u) || (SSIE_CH1_DWL_VALUE == 22u) || (SSIE_CH1_DWL_VALUE == 24u))
        #define SSIE_CH1_PDTA        (1u)
    #else
        #define SSIE_CH1_PDTA        (0xFFFFFFFFu)
    #endif
    #define SSIE_CH1_SPDP        (0u)
#else
    #define SSIE_CH1_PDTA        (0xFFFFFFFFu)
    #define SSIE_CH1_SPDP        (0xFFFFFFFFu)
#endif

#if (SSIE_CH1_SERIAL_IF_FMT == 0u)
    /* I2S */
    #define SSIE_CH1_DEL         (0u)
    #define SSIE_CH1_SDTA        (0u)
    #define SSIE_CH1_LRCKP       (0u)
    #define SSIE_CH1_BCKP        (0u)
    #define SSIE_CH1_FRM         (0u)
    #define SSIE_CH1_OMOD        (0u)
#elif (SSIE_CH1_SERIAL_IF_FMT == 1u)
    /* Left-justified */
    #define SSIE_CH1_DEL         (1u)
    #define SSIE_CH1_SDTA        (0u)
    #define SSIE_CH1_LRCKP       (1u)
    #define SSIE_CH1_BCKP        (0u)
    #define SSIE_CH1_FRM         (0u)
    #define SSIE_CH1_OMOD        (0u)
#elif (SSIE_CH1_SERIAL_IF_FMT == 2u)
    /* Right-justified */
    #define SSIE_CH1_DEL         (1u)
    #define SSIE_CH1_SDTA        (1u)
    #define SSIE_CH1_LRCKP       (1u)
    #define SSIE_CH1_BCKP        (0u)
    #define SSIE_CH1_FRM         (0u)
    #define SSIE_CH1_OMOD        (0u)
#else
    #define SSIE_CH1_DEL         (0xFFFFFFFFu)
    #define SSIE_CH1_SDTA        (0xFFFFFFFFu)
    #define SSIE_CH1_LRCKP       (0xFFFFFFFFu)
    #define SSIE_CH1_BCKP        (0xFFFFFFFFu)
    #define SSIE_CH1_FRM         (0xFFFFFFFFu)
    #define SSIE_CH1_OMOD        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIEn.SSICR.SWL calculation
*********************************************************/
/*  SSIEn_SWL is chosen correspnding to SSIEn_BCLK value. */
/************************
*      Channel 0        *
************************/
#if ((SSIE_CH0_SERIAL_IF_FMT == 0u) || (SSIE_CH0_SERIAL_IF_FMT == 1u) || (SSIE_CH0_SERIAL_IF_FMT == 2u))
    #define SSIE_CH0_SWL_LEN        (SSIE_CH0_BCLK / 2u)
#else
    #define SSIE_CH0_SWL_LEN        (0xFFFFFFFFu)
#endif

#if (SSIE_CH0_SWL_LEN == 8u)
    #define SSIE_CH0_SWL         (0x0u)
#elif (SSIE_CH0_SWL_LEN == 16u)
    #define SSIE_CH0_SWL         (0x1u)
#elif (SSIE_CH0_SWL_LEN == 24u)
    #define SSIE_CH0_SWL         (0x2u)
#elif (SSIE_CH0_SWL_LEN == 32u)
    #define SSIE_CH0_SWL         (0x3u)
#elif (SSIE_CH0_SWL_LEN == 48u)
    #define SSIE_CH0_SWL         (0x4u)
#elif (SSIE_CH0_SWL_LEN == 64u)
    #define SSIE_CH0_SWL         (0x5u)
#elif (SSIE_CH0_SWL_LEN == 128u)
    #define SSIE_CH0_SWL         (0x6u)
#elif (SSIE_CH0_SWL_LEN == 256u)
    #define SSIE_CH0_SWL         (0x7u)
#else
    #define SSIE_CH0_SWL         (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if ((SSIE_CH1_SERIAL_IF_FMT == 0u) || (SSIE_CH1_SERIAL_IF_FMT == 1u) || (SSIE_CH1_SERIAL_IF_FMT == 2u))
    #define SSIE_CH1_SWL_LEN        (SSIE_CH1_BCLK / 2u)
#else
    #define SSIE_CH1_SWL_LEN        (0xFFFFFFFFu)
#endif

#if (SSIE_CH1_SWL_LEN == 8u)
    #define SSIE_CH1_SWL         (0x0u)
#elif (SSIE_CH1_SWL_LEN == 16u)
    #define SSIE_CH1_SWL         (0x1u)
#elif (SSIE_CH1_SWL_LEN == 24u)
    #define SSIE_CH1_SWL         (0x2u)
#elif (SSIE_CH1_SWL_LEN == 32u)
    #define SSIE_CH1_SWL         (0x3u)
#elif (SSIE_CH1_SWL_LEN == 48u)
    #define SSIE_CH1_SWL         (0x4u)
#elif (SSIE_CH1_SWL_LEN == 64u)
    #define SSIE_CH1_SWL         (0x5u)
#elif (SSIE_CH1_SWL_LEN == 128u)
    #define SSIE_CH1_SWL         (0x6u)
#elif (SSIE_CH1_SWL_LEN == 256u)
    #define SSIE_CH1_SWL         (0x7u)
#else
    #define SSIE_CH1_SWL         (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIEn.SSICR.DWL calculation
*********************************************************/
/*  SSIEn_DWL is chosen correspnding to SSIEn_DWL_VALUE value. */
/************************
*      Channel 0        *
************************/
#if (SSIE_CH0_DWL_VALUE == 8u)
    #define SSIE_CH0_DWL         (0x0u)
#elif (SSIE_CH0_DWL_VALUE == 16u)
    #define SSIE_CH0_DWL         (0x1u)
#elif (SSIE_CH0_DWL_VALUE == 18u)
    #define SSIE_CH0_DWL         (0x2u)
#elif (SSIE_CH0_DWL_VALUE == 20u)
    #define SSIE_CH0_DWL         (0x3u)
#elif (SSIE_CH0_DWL_VALUE == 22u)
    #define SSIE_CH0_DWL         (0x4u)
#elif (SSIE_CH0_DWL_VALUE == 24u)
    #define SSIE_CH0_DWL         (0x5u)
#elif (SSIE_CH0_DWL_VALUE == 32u)
    #define SSIE_CH0_DWL         (0x6u)
#else
    /* case error */
    #define SSIE_CH0_DWL         (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if (SSIE_CH1_DWL_VALUE == 8u)
    #define SSIE_CH1_DWL         (0x0u)
#elif (SSIE_CH1_DWL_VALUE == 16u)
    #define SSIE_CH1_DWL         (0x1u)
#elif (SSIE_CH1_DWL_VALUE == 18u)
    #define SSIE_CH1_DWL         (0x2u)
#elif (SSIE_CH1_DWL_VALUE == 20u)
    #define SSIE_CH1_DWL         (0x3u)
#elif (SSIE_CH1_DWL_VALUE == 22u)
    #define SSIE_CH1_DWL         (0x4u)
#elif (SSIE_CH1_DWL_VALUE == 24u)
    #define SSIE_CH1_DWL         (0x5u)
#elif (SSIE_CH1_DWL_VALUE == 32u)
    #define SSIE_CH1_DWL         (0x6u)
#else
    /* case error */
    #define SSIE_CH1_DWL         (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIEn.SSICR.MST,AUCKE calculation
*********************************************************/
/*  SSIEn_MST, AUCKE are chosen correspnding to
      SSIEn_CLK_MODE value.
*/
/************************
*      Channel 0        *
************************/
#if (SSIE_CH0_CLK_MODE == 0u)
    /* Master mode */
    #define SSIE_CH0_MST         (1u)
    #define SSIE_CH0_AUCKE       (1u)
#elif (SSIE_CH0_CLK_MODE == 1u)
    /* Slave mode */
    #define SSIE_CH0_MST         (0u)
    #define SSIE_CH0_AUCKE       (0u)
#else 
    /* case error */
    #define SSIE_CH0_MST         (0xFFFFFFFFu)
    #define SSIE_CH0_AUCKE       (0xFFFFFFFFu)
#endif
                            
/************************
*      Channel 1        *
************************/
#if (SSIE_CH1_CLK_MODE == 0u)
    /* Master mode */
    #define SSIE_CH1_MST         (1u)
    #define SSIE_CH1_AUCKE       (1u)
#elif (SSIE_CH1_CLK_MODE == 1u)
    /* Slave mode */
    #define SSIE_CH1_MST         (0u)
    #define SSIE_CH1_AUCKE       (0u)
#else 
    /* case error */
    #define SSIE_CH1_MST         (0xFFFFFFFFu)
    #define SSIE_CH1_AUCKE       (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIE0.SSICR.CKDV calculation
*********************************************************/
/*  SSIEn_CKDV is chosen correspnding to SSIEn_CKDV_VALUE value. */
/************************
*      Channel 0        *
************************/
#if (SSIE_CH0_CKDV_VALUE == 1u)
    #define SSIE_CH0_CKDV        (0x0u)
#elif (SSIE_CH0_CKDV_VALUE == 2u)
    #define SSIE_CH0_CKDV        (0x1u)
#elif (SSIE_CH0_CKDV_VALUE == 4u)
    #define SSIE_CH0_CKDV        (0x2u)
#elif (SSIE_CH0_CKDV_VALUE == 8u)
    #define SSIE_CH0_CKDV        (0x3u)
#elif (SSIE_CH0_CKDV_VALUE == 16u)
    #define SSIE_CH0_CKDV        (0x4u)
#elif (SSIE_CH0_CKDV_VALUE == 32u)
    #define SSIE_CH0_CKDV        (0x5u)
#elif (SSIE_CH0_CKDV_VALUE == 64u)
    #define SSIE_CH0_CKDV        (0x6u)
#elif (SSIE_CH0_CKDV_VALUE == 128u)
    #define SSIE_CH0_CKDV        (0x7u)
#elif (SSIE_CH0_CKDV_VALUE == 6u)
    #define SSIE_CH0_CKDV        (0x8u)
#elif (SSIE_CH0_CKDV_VALUE == 12u)
    #define SSIE_CH0_CKDV        (0x9u)
#elif (SSIE_CH0_CKDV_VALUE == 24u)
    #define SSIE_CH0_CKDV        (0xAu)
#elif (SSIE_CH0_CKDV_VALUE == 48u)
    #define SSIE_CH0_CKDV        (0xBu)
#elif (SSIE_CH0_CKDV_VALUE == 96u)
    #define SSIE_CH0_CKDV        (0xCu)
#else
    /* case error */
    #define SSIE_CH0_CKDV        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if (SSIE_CH1_CKDV_VALUE == 1u)
    #define SSIE_CH1_CKDV        (0x0u)
#elif (SSIE_CH1_CKDV_VALUE == 2u)
    #define SSIE_CH1_CKDV        (0x1u)
#elif (SSIE_CH1_CKDV_VALUE == 4u)
    #define SSIE_CH1_CKDV        (0x2u)
#elif (SSIE_CH1_CKDV_VALUE == 8u)
    #define SSIE_CH1_CKDV        (0x3u)
#elif (SSIE_CH1_CKDV_VALUE == 16u)
    #define SSIE_CH1_CKDV        (0x4u)
#elif (SSIE_CH1_CKDV_VALUE == 32u)
    #define SSIE_CH1_CKDV        (0x5u)
#elif (SSIE_CH1_CKDV_VALUE == 64u)
    #define SSIE_CH1_CKDV        (0x6u)
#elif (SSIE_CH1_CKDV_VALUE == 128u)
    #define SSIE_CH1_CKDV        (0x7u)
#elif (SSIE_CH1_CKDV_VALUE == 6u)
    #define SSIE_CH1_CKDV        (0x8u)
#elif (SSIE_CH1_CKDV_VALUE == 12u)
    #define SSIE_CH1_CKDV        (0x9u)
#elif (SSIE_CH1_CKDV_VALUE == 24u)
    #define SSIE_CH1_CKDV        (0xAu)
#elif (SSIE_CH1_CKDV_VALUE == 48u)
    #define SSIE_CH1_CKDV        (0xBu)
#elif (SSIE_CH1_CKDV_VALUE == 96u)
    #define SSIE_CH1_CKDV        (0xCu)
#else
    /* case error */
    #define SSIE_CH1_CKDV        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIEn.SSISCR.TDES value calculation.
*********************************************************/
/************************
*      Channel 0        *
************************/
#if ((SSIE_CH0_SERIAL_IF_FMT == 0u) || (SSIE_CH0_SERIAL_IF_FMT == 1u) || (SSIE_CH0_SERIAL_IF_FMT == 2u))
    #define SSIE_CH0_TDES        ((((SSIE_SSIFSR_TDCMAX / 2u) - SSIE_CH0_TDES_NUMBER) * 2u) - 1u)
#else
    #define SSIE_CH0_TDES        (0xFFFFFFFFu)
#endif

#if ((SSIE_CH0_TDES < 0u) || (SSIE_CH0_TDES >= 32u))
    #define SSIE_CH0_TDES        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if ((SSIE_CH1_SERIAL_IF_FMT == 0u) || (SSIE_CH1_SERIAL_IF_FMT == 1u) || (SSIE_CH1_SERIAL_IF_FMT == 2u))
    #define SSIE_CH1_TDES        ((((SSIE_SSIFSR_TDCMAX / 2u) - SSIE_CH1_TDES_NUMBER) * 2u) - 1u)
#else
    #define SSIE_CH1_TDES        (0xFFFFFFFFu)
#endif

#if ((SSIE_CH1_TDES < 0u) || (SSIE_CH1_TDES >= 32u))
    #define SSIE_CH1_TDES        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIEn.SSISCR.RDFS value calculation.
*********************************************************/
/************************
*      Channel 0        *
************************/
#if ((SSIE_CH0_SERIAL_IF_FMT == 0u) || (SSIE_CH0_SERIAL_IF_FMT == 1u) || (SSIE_CH0_SERIAL_IF_FMT == 2u))
    #define SSIE_CH0_RDFS        ((SSIE_CH0_RDFS_NUMBER * 2u) - 1u)
#else
    #define SSIE_CH0_RDFS        (0xFFFFFFFFu)
#endif

#if ((SSIE_CH0_RDFS < 0u) || (SSIE_CH0_RDFS >= 32u))
    #define SSIE_CH0_RDFS        (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if ((SSIE_CH1_SERIAL_IF_FMT == 0u) || (SSIE_CH1_SERIAL_IF_FMT == 1u) || (SSIE_CH1_SERIAL_IF_FMT == 2u))
    #define SSIE_CH1_RDFS        ((SSIE_CH1_RDFS_NUMBER * 2u) - 1u)
#else
    #define SSIE_CH1_RDFS        (0xFFFFFFFFu)
#endif

#if ((SSIE_CH1_RDFS < 0u) || (SSIE_CH1_RDFS >= 32u))
    #define SSIE_CH1_RDFS        (0xFFFFFFFFu)
#endif

/*********************************************************
    SSIEn.SSIFCR.BSW value calculation.
*********************************************************/
/************************
*      Channel 0        *
************************/
#if ((SSIE_CH0_BYTE_SWAP == 0u) || (SSIE_CH0_BYTE_SWAP == 1u))
    #define SSIE_CH0_BSW         (SSIE_CH0_BYTE_SWAP)
#else
    #define SSIE_CH0_BSW         (0xFFFFFFFFu)
#endif

/************************
*      Channel 1        *
************************/
#if ((SSIE_CH1_BYTE_SWAP == 0u) || (SSIE_CH1_BYTE_SWAP == 1u))
    #define SSIE_CH1_BSW         (SSIE_CH1_BYTE_SWAP)
#else
    #define SSIE_CH1_BSW         (0xFFFFFFFFu)
#endif


/******************************************************************************
    End of SSIE register value calculation for "Standarad configuration".
******************************************************************************/

/* SSI_STANDARD_CONFIG */
#else
/* SSI_USER_UNIQUE_CONFIG */

/******************************************************************************

    "User unique configuration"
       Elements calculation for SSI configuration error checking.

******************************************************************************/
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
#define SSIE_CONFIG_ERR_VAL      (0xFFFFFFFFu)

/* "Transmit / Receive mode selection (I/O mode)" checking */
#if (SSIE_CH0_IO_MODE > SSIE_IO_MODE_TR)
    #error "ERROR !!! The value set to SSIE_CH0_IO_MODE is invalid."
#endif

#if (SSIE_CH1_IO_MODE > SSIE_IO_MODE_TX)
    #error "ERROR !!! The value set to SSIE_CH1_IO_MODE is invalid."
#endif

/* Transmit Data Trigger Number (Tx FIFO empty) configuration checking */
#if (SSIE_CH0_TDES == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH0_TDES is invalid."
#endif

#if (SSIE_CH1_TDES == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH1_TDES is invalid."
#endif

/* Receive Data Trigger Number configuration checking */
#if (SSIE_CH0_RDFS == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH0_RDFS_FIFO is invalid."
#endif

#if (SSIE_CH1_RDFS == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH1_RDFS_FIFO is invalid."
#endif

/* Audio serial interface format configuration checking */
#if ((SSIE_CH0_DEL == SSIE_CONFIG_ERR_VAL) || (SSIE_CH0_PDTA == SSIE_CONFIG_ERR_VAL)        \
    || (SSIE_CH0_SDTA == SSIE_CONFIG_ERR_VAL) || (SSIE_CH0_SPDP == SSIE_CONFIG_ERR_VAL)     \
    || (SSIE_CH0_LRCKP == SSIE_CONFIG_ERR_VAL) || (SSIE_CH0_BCKP == SSIE_CONFIG_ERR_VAL))
    #error "ERROR !!! SSIE0 serial interface format configuration is invalid."
#endif

#if ((SSIE_CH1_DEL == SSIE_CONFIG_ERR_VAL) || (SSIE_CH1_PDTA == SSIE_CONFIG_ERR_VAL)        \
    || (SSIE_CH1_SDTA == SSIE_CONFIG_ERR_VAL) || (SSIE_CH1_SPDP == SSIE_CONFIG_ERR_VAL)     \
    || (SSIE_CH1_LRCKP == SSIE_CONFIG_ERR_VAL) || (SSIE_CH1_BCKP == SSIE_CONFIG_ERR_VAL))
    #error "ERROR !!! SSIE1 serial interface format configuration is invalid."
#endif

/* Bit Clock configuration checking */
#if (SSIE_CH0_SWL == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH0_SWL is invalid."
#endif
#if (SSIE_CH1_SWL == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH1_SWL is invalid."
#endif

/* PCM data width configuration checking */
#if (SSIE_CH0_DWL == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH0_DWL is invalid."
#endif
#if (SSIE_CH1_DWL == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH1_DWL is invalid."
#endif
#if (SSIE_CH0_SWL < 2)
    #if (SSIE_CH0_DWL > SSIE_CH0_SWL)
        #error "ERROR !!! The value of SSIE_CH0_DWL must be smaller than SSIE_CH0_SWL."
    #endif
#endif
#if (SSIE_CH1_SWL < 2)
    #if (SSIE_CH1_DWL > SSIE_CH1_SWL)
        #error "ERROR !!! The value of SSIE_CH1_DWL must be smaller than SSIE_CH1_SWL."
    #endif
#endif

/* Clock supply direction (Master or Slave) configuration checking */
#if ((SSIE_CH0_MST == SSIE_CONFIG_ERR_VAL) || (SSIE_CH0_AUCKE == SSIE_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSIE_CH0_MST & SSIE_CH0_AUCKE are invalid."
#endif
#if ((SSIE_CH1_MST == SSIE_CONFIG_ERR_VAL) || (SSIE_CH1_AUCKE == SSIE_CONFIG_ERR_VAL))
    #error "ERROR !!! The value set to SSIE_CH1_MST & SSIE_CH1_AUCKE are invalid."
#endif

/* AUDIO_CLK Division Ratio (to get bit clock) configuration check */
#if (SSIE_CH0_CKDV == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH0_CKDV is invalid."
#endif
#if (SSIE_CH1_CKDV == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH1_CKDV is invalid."
#endif

#if (SSIE_CH0_BSW == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH0_BSW is invalid."
#endif
#if (SSIE_CH1_BSW == SSIE_CONFIG_ERR_VAL)
    #error "ERROR !!! The value set to SSIE_CH1_BSW is invalid."
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/
/* no typedef defined */

#endif /* SSIE_API_RX_PRIVATE_H_ */

#endif
/* End of file */
