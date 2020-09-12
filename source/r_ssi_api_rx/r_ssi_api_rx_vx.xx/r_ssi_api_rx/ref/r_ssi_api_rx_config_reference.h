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
* File Name    : r_ssi_api_rx_config_reference.h
* Version      : 2.00
* Tool-Chain   : RX Family C Compiler
*                GCC for Renesas RX
*                IAR C/C++ Compiler for Renesas RX
* Description  : SSI driver module file for RX MCUs.
*
*                This is a configuration file for SSI module.
*
*                Users can configure SSI peripheral by the one of following
*                  two ways.
*
*                   a) Standard configuration
*                   b) User unique configuration
*
*                The default is "a) Standard configuration", in case of
*                  "b) User unique configuration" to use, change macro 
*                   definition "SSI_STANDARD_CONFIG" to "SSI_USER_UNIQUE_CONFIG".
*
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

#ifndef SSI_API_CONFIG_REFERENCE_H_
#define SSI_API_CONFIG_REFERENCE_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
    This SSI module support Standatrd configuration to be easy for users to 
      configure SSI peripheral. However some users woulkd like to configure on
      their own way. Set SSI_STANDARD_CONFIG or SSI_USER_UNIQUE_CONFIG to
      definition below accordind to the requirement.
    ----------------------------------
    'Standard      SSI_STANDARD_CONFIG
    'User unique   SSI_USER_UNIQUE_CONFIG
*******************************************************************************/
#define SSI_STANDARD_CONFIG

#if (defined(SSI0) || defined(SSI1))
/******************************************************************************
    Transmit / Receive mode selection (I/O mode)
      Set a value from following alternatives to SSIn_IO_MODE.
    ----------------------------------
     SSI usage                  value
    ----------------------------------
    'SSI not to use             (0u)
    'SSI to Receive data        (1u)
    'SSI to Transmit data       (2u)
    'SSI to Receive & Transmit  (3u)

    NOTE: This is a configuration which must be set wether
            in case of "SSI_STANDARD_CONFIG" or "SSI_USER_UNIQUE_CONFIG". 
*******************************************************************************/
#define SSI_CH0_IO_MODE    (2u)    /* Ch0 Transmit / Receive mode selection */
#define SSI_CH1_IO_MODE    (0u)    /* Ch1 Transmit / Receive mode selection */



#ifdef SSI_STANDARD_CONFIG
/******************************************************************************

    Standard configuration

******************************************************************************/
/******************************************************************************
    Audio serial interface format selection
      Set a value from following alternatives to SSIn_SERIAL_IF_FMT.
    ----------------------------------
     IF format             value
    ----------------------------------
      I2S                   (0u)
      Left-justified        (1u)
      Right-justified       (2u)
*******************************************************************************/
#define SSI_CH0_SERIAL_IF_FMT   (0u)    /* Ch0 Audio serial interface format */
#define SSI_CH1_SERIAL_IF_FMT   (0u)    /* Ch1 Audio serial interface format */

/******************************************************************************
    PCM data width selection
      Set a value from following alternatives to SSIn_DATA_WIDTH.
    ----------------------------------
     PCM Data widdth       value
    ----------------------------------
       8bit                 (8u)
      16bit                 (16u)
      18bit                 (18u)
      20bit                 (20u)
      22bit                 (22u)
      24bit                 (24u)
*******************************************************************************/
#define SSI_CH0_DATA_WIDTH   (16u)          /* Ch0 PCM data width */
#define SSI_CH1_DATA_WIDTH   (16u)          /* Ch1 PCM data width */

/******************************************************************************
    Bit clock selection
      Set a value from following alternatives to SSIn_BCLK.
    ----------------------------------
     bit clock             value
    ----------------------------------
      16Fs                  (16u)
      32Fs                  (32u)
      48Fs                  (48u)
      64Fs                  (64u)
*******************************************************************************/
#define SSI_CH0_BCLK         (64u)          /* Ch0 Bit Clock */
#define SSI_CH1_BCLK         (64u)          /* Ch1 Bit Clock */

/******************************************************************************
    Master clock frequency 
      Set a value from following alternatives to SSIn_MCLK.
    ----------------------------------
     Clock               value
    ----------------------------------
      16Fs                (16u)
      32Fs                (32u)
       :                    :
      8192Fs              (8192u)

    NOTE: Set the Master Clock to the integral multiple of the Bit Clock.
*******************************************************************************/
#define SSI_MCLK         (256u)         /* Master Clock */

/******************************************************************************
    Clock supply mode selection
      Set a value from following alternatives to SSIn_CLK_MODE.
    ----------------------------------
     Data length           value
    ----------------------------------
      Master mode           (0u)
      Slave mode            (1u)
*******************************************************************************/
#define SSI_CH0_CLK_MODE     (0u)           /* Ch0 Clock Supply Mode */
#define SSI_CH1_CLK_MODE     (0u)           /* Ch1 Clock Supply Mode */

/******************************************************************************
    Transmit Data Trigger number selection
      Set a value from following alternatives to SSIn_TTRG_NUMBER.
    ----------------------------------
     Data length           value
    ----------------------------------
       8bit                 (12u) or (8u) or (4u)
      16bit                 (6u)  or (4u) or (2u)
      18,20,22,24bit        (3u)  or (2u) or (1u)
*******************************************************************************/
#define SSI_CH0_TTRG_NUMBER    (4u)    /* Ch0 Tx Empty trigger configuration */
#define SSI_CH1_TTRG_NUMBER    (4u)    /* Ch1 Tx Empty trigger configuration */

/******************************************************************************
    Receive Data Trigger number selection
      Set a value from following alternatives to SSIn_RTRG_NUMBER.
    ----------------------------------
     Data length           value
    ----------------------------------
       8bit                 (4u)  or (8u) or (12u)
      16bit                 (2u)  or (4u) or (6u)
      18,20,22,24bit        (1u)  or (2u) or (3u)
*******************************************************************************/
#define SSI_CH0_RTRG_NUMBER    (4u)    /* Ch0 Rx Full trigger configuration */
#define SSI_CH1_RTRG_NUMBER    (4u)    /* Ch1 Rx Full trigger configuration */

/******************************************************************************
    end of Standard configuration:
******************************************************************************/

#else /* SSI_USER_UNIQUE_CONFIG */

/******************************************************************************

    User unique configuration

******************************************************************************/
/*
    In case "User unique configuration", change following macro definitions
      to fit for user unique requirement. Refer to Hardware user's manual.
*/
/* Channel 0 */
#define SSI_CH0_TTRG   (3u)    /* value for "SSI0.SSIFCR.TTRG" */
#define SSI_CH0_RTRG   (3u)    /* value for "SSI0.SSIFCR.RTRG" */
#define SSI_CH0_DEL    (0u)    /* value for "SSI0.SSICR.DEL" */
#define SSI_CH0_PDTA   (0u)    /* value for "SSI0.SSICR.PDATA" */
#define SSI_CH0_SDTA   (0u)    /* value for "SSI0.SSICR.SDATA" */
#define SSI_CH0_SPDP   (0u)    /* value for "SSI0.SSICR.SPDP" */
#define SSI_CH0_SWSP   (0u)    /* value for "SSI0.SSICR.SWSP" */
#define SSI_CH0_SCKP   (0u)    /* value for "SSI0.SSICR.SCKP" */
#define SSI_CH0_SWL    (0u)    /* value for "SSI0.SSICR.SWL" */
#define SSI_CH0_DWL    (0u)    /* value for "SSI0.SSICR.DWL" */
#define SSI_CH0_SWSD   (0u)    /* value for "SSI0.SSICR.SWSD" */
#define SSI_CH0_SCKD   (0u)    /* value for "SSI0.SSICR.SCKD" */
#define SSI_CH0_AUCKE  (0u)    /* value for "SSI0.SSIFCR.AUCKE" */
#define SSI_CH0_CKDV   (0u)    /* value for "SSI0.SSICR.SCDV" */

/* Channel 1 */
#define SSI_CH1_TTRG   (3u)    /* value for "SSI1.SSIFCR.TTRG" */
#define SSI_CH1_RTRG   (3u)    /* value for "SSI1.SSIFCR.RTRG" */
#define SSI_CH1_DEL    (0u)    /* value for "SSI1.SSICR.DEL" */
#define SSI_CH1_PDTA   (0u)    /* value for "SSI1.SSICR.PDATA" */
#define SSI_CH1_SDTA   (0u)    /* value for "SSI1.SSICR.SDATA" */
#define SSI_CH1_SPDP   (0u)    /* value for "SSI1.SSICR.SPDP" */
#define SSI_CH1_SWSP   (0u)    /* value for "SSI1.SSICR.SWSP" */
#define SSI_CH1_SCKP   (0u)    /* value for "SSI1.SSICR.SCKP" */
#define SSI_CH1_SWL    (0u)    /* value for "SSI1.SSICR.SWL" */
#define SSI_CH1_DWL    (0u)    /* value for "SSI1.SSICR.DWL" */
#define SSI_CH1_SWSD   (0u)    /* value for "SSI1.SSICR.SWSD" */
#define SSI_CH1_SCKD   (0u)    /* value for "SSI1.SSICR.SCKD" */
#define SSI_CH1_AUCKE  (0u)    /* value for "SSI1.SSIFCR.AUCKE" */
#define SSI_CH1_CKDV   (0u)    /* value for "SSI1.SSICR.SCDV" */

/******************************************************************************
    End of User unique configuration area
******************************************************************************/
/* USER_UNIQUE_CONFIG */
#endif

#else /* (defined(SSI0) || defined(SSI1)) */
/******************************************************************************
    Transmit / Receive mode selection (I/O mode)
      Set a value from following alternatives to SSIEn_IO_MODE.
    ----------------------------------
     SSIE usage                  value
    ----------------------------------
    'SSIE not to use             (0u)
    'SSIE to Receive data        (1u)
    'SSIE to Transmit data       (2u)
    'SSIE to Receive & Transmit  (3u)

    NOTE: This is a configuration which must be set wether
            in case of "SSI_STANDARD_CONFIG" or "SSI_USER_UNIQUE_CONFIG". 
*******************************************************************************/
#define SSIE_CH0_IO_MODE    (2u)    /* Ch0 Transmit / Receive mode selection */
#define SSIE_CH1_IO_MODE    (0u)    /* Ch1 Transmit / Receive mode selection */

#ifdef SSI_STANDARD_CONFIG
/******************************************************************************

    Standard configuration

******************************************************************************/
/******************************************************************************
    Audio serial interface format selection
      Set a value from following alternatives to SSIEn_SERIAL_IF_FMT.
    ----------------------------------
     IF format             value
    ----------------------------------
      I2S                   (0u)
      Left-justified        (1u)
      Right-justified       (2u)
*******************************************************************************/
#define SSIE_CH0_SERIAL_IF_FMT   (0u)    /* Ch0 Audio serial interface format */
#define SSIE_CH1_SERIAL_IF_FMT   (0u)    /* Ch1 Audio serial interface format */

/******************************************************************************
    PCM data width selection
      Set a value from following alternatives to SSIEn_DATA_WIDTH.
    ----------------------------------
     PCM Data widdth       value
    ----------------------------------
       8bit                 (8u)
      16bit                 (16u)
      18bit                 (18u)
      20bit                 (20u)
      22bit                 (22u)
      24bit                 (24u)
      32bit                 (32u)
*******************************************************************************/
#define SSIE_CH0_DATA_WIDTH   (16u)          /* Ch0 PCM data width */
#define SSIE_CH1_DATA_WIDTH   (16u)          /* Ch1 PCM data width */

/******************************************************************************
    Bit clock selection
      Set a value from following alternatives to SSIEn_BCLK.
    -------------------------------------------------------------------------
    System word size     bit clock             value
    -------------------------------------------------------------------------
      8bit                 (2 *   8)fs           (16u)
      16bit                (2 *  16)fs           (32u)
      24bit                (2 *  24)fs           (48u)
      32bit                (2 *  32)fs           (64u)
      48bit                (2 *  48)fs           (96u)
      64bit                (2 *  64)fs           (128u)
      128bit               (2 * 128)fs           (256u)
      256bit               (2 * 256)fs           (512u)
*******************************************************************************/
#define SSIE_CH0_BCLK         (64u)          /* Ch0 Bit Clock */
#define SSIE_CH1_BCLK         (64u)          /* Ch1 Bit Clock */

/******************************************************************************
    Master clock frequency 
      Set a value from following alternatives to SSIEn_MCLK.
    ----------------------------------
     Clock               value
    ----------------------------------
      16Fs                (16u)
      32Fs                (32u)
       :                    :
      8192Fs              (8192u)

    NOTE: Set the Master Clock to the integral multiple of the Bit Clock.
*******************************************************************************/
#define SSIE_MCLK         (256u)         /* Master Clock */

/******************************************************************************
    Clock supply mode selection
      Set a value from following alternatives to SSIEn_CLK_MODE.
    ----------------------------------
     Data length           value
    ----------------------------------
      Master mode           (0u)
      Slave mode            (1u)
*******************************************************************************/
#define SSIE_CH0_CLK_MODE     (0u)           /* Ch0 Clock Supply Mode */
#define SSIE_CH1_CLK_MODE     (0u)           /* Ch1 Clock Supply Mode */

/******************************************************************************
    Transmit Data Trigger number selection
      Set a value from following alternatives to SSIEn_TDES_NUMBER.
    ---------------------------------------------------------------------------
     IF format             value
    ---------------------------------------------------------------------------
     I2S                   0 to 15
     Left-justified        0 to 15
     Right-justified       0 to 15
*******************************************************************************/
#define SSIE_CH0_TDES_NUMBER    (8u)    /* Ch0 Tx Empty trigger configuration */
#define SSIE_CH1_TDES_NUMBER    (8u)    /* Ch1 Tx Empty trigger configuration */

/******************************************************************************
    Receive Data Trigger number selection
      Set a value from following alternatives to SSIEn_RDFS_NUMBER.
    ----------------------------------------------------------------------------
     IF format             value
    ---------------------------------------------------------------------------
     I2S                   1 to 16
     Left-justified        1 to 16
     Right-justified       1 to 16
*******************************************************************************/
#define SSIE_CH0_RDFS_NUMBER    (8u)    /* Ch0 Rx Full trigger configuration */
#define SSIE_CH1_RDFS_NUMBER    (8u)    /* Ch1 Rx Full trigger configuration */

/******************************************************************************
    Byte swapping
*******************************************************************************/
#define SSIE_CH0_BYTE_SWAP    (0u)
#define SSIE_CH1_BYTE_SWAP    (0u)

/******************************************************************************
    end of Standard configuration:
******************************************************************************/

#else /* SSI_USER_UNIQUE_CONFIG */

/******************************************************************************

    User unique configuration

******************************************************************************/
/*
    In case "User unique configuration", change following macro definitions
      to fit for user unique requirement. Refer to Hardware user's manual.
*/
/* Channel 0 */
#define SSIE_CH0_TDES   (15u)   /* value for "SSIE0.SSISCR.TDES" */
#define SSIE_CH0_RDFS   (15u)   /* value for "SSIE0.SSISCR.RDFS" */
#define SSIE_CH0_DEL    (0u)    /* value for "SSIE0.SSICR.DEL" */
#define SSIE_CH0_PDTA   (0u)    /* value for "SSIE0.SSICR.PDATA" */
#define SSIE_CH0_SDTA   (0u)    /* value for "SSIE0.SSICR.SDATA" */
#define SSIE_CH0_SPDP   (0u)    /* value for "SSIE0.SSICR.SPDP" */
#define SSIE_CH0_LRCKP  (0u)    /* value for "SSIE0.SSICR.LRCKP" */
#define SSIE_CH0_BCKP   (0u)    /* value for "SSIE0.SSICR.BCKP" */
#define SSIE_CH0_SWL    (3u)    /* value for "SSIE0.SSICR.SWL" */
#define SSIE_CH0_DWL    (1u)    /* value for "SSIE0.SSICR.DWL" */
#define SSIE_CH0_CKDV   (2u)    /* value for "SSIE0.SSICR.SCDV" */
#define SSIE_CH0_MST    (1u)    /* value for "SSIE0.SSICR.MST" */
#define SSIE_CH0_FRM    (0u)    /* value for "SSIE0.SSICR.FRM" */
#define SSIE_CH0_BSW    (0u)    /* value for "SSIE0.SSIFCR.BSW" */
#define SSIE_CH0_AUCKE  (1u)    /* value for "SSIE0.SSIFCR.AUCKE" */
#define SSIE_CH0_OMOD   (0u)    /* value for "SSIE0.SSIOFR.OMOD" */

/* Channel 1 */
#define SSIE_CH1_TDES   (15u)   /* value for "SSIE1.SSISCR.TDES" */
#define SSIE_CH1_RDFS   (15u)   /* value for "SSIE1.SSISCR.RDFS" */
#define SSIE_CH1_DEL    (0u)    /* value for "SSIE1.SSICR.DEL" */
#define SSIE_CH1_PDTA   (0u)    /* value for "SSIE1.SSICR.PDATA" */
#define SSIE_CH1_SDTA   (0u)    /* value for "SSIE1.SSICR.SDATA" */
#define SSIE_CH1_SPDP   (0u)    /* value for "SSIE1.SSICR.SPDP" */
#define SSIE_CH1_LRCKP  (0u)    /* value for "SSIE1.SSICR.LRCKP" */
#define SSIE_CH1_BCKP   (0u)    /* value for "SSIE1.SSICR.BCKP" */
#define SSIE_CH1_SWL    (3u)    /* value for "SSIE1.SSICR.SWL" */
#define SSIE_CH1_DWL    (1u)    /* value for "SSIE1.SSICR.DWL" */
#define SSIE_CH1_CKDV   (2u)    /* value for "SSIE1.SSICR.SCDV" */
#define SSIE_CH1_MST    (1u)    /* value for "SSIE1.SSICR.MST" */
#define SSIE_CH1_FRM    (0u)    /* value for "SSIE1.SSICR.FRM" */
#define SSIE_CH1_BSW    (0u)    /* value for "SSIE1.SSIFCR.BSW" */
#define SSIE_CH1_AUCKE  (1u)    /* value for "SSIE1.SSIFCR.AUCKE" */
#define SSIE_CH1_OMOD   (0u)    /* value for "SSIE1.SSIOFR.OMOD" */

/******************************************************************************
    End of User unique configuration area
******************************************************************************/
/* USER_UNIQUE_CONFIG */
#endif

#endif /* (defined(SSI0) || defined(SSI1)) */

#endif /* SSI_API_CONFIG_REFERENCE_H_ */
