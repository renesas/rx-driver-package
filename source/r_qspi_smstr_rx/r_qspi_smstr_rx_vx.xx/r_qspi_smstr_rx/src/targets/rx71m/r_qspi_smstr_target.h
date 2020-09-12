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
* Copyright (C) 2014(2015-2019) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : QSPI single master driver
* File Name    : r_qspi_smstr_target.h
* Version      : 1.12
* Device       : RX
* Abstract     : Specifics file dedicated to RX71M for QSPI single master drive
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions for QSPI single master driver
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 24.06.2014 1.05     First Release
*              : 29.08.2014 1.06     Not change program due to changing r_qspi_smstr_private.h.
*              :                     Added demo source for DTC in this driver.
*              : 28.11.2014 1.07     Changed r_qspi_smstr_tx_dmacdtc_wait() and r_qspi_smstr_rx_dmacdtc_wait().
*              : 30.01.2015 1.08     Added RX71M.
*              : 30.09.2016 1.09     Changed Tool-Chain version.
*              :                     Changed pin name.
*              : 20.05.2019 1.12     Added support for GNUC and ICCRX.
*              :                     Fixed coding style. 
*******************************************************************************/
#ifndef _QSPI_SMSTR_TARGET_H_
#define _QSPI_SMSTR_TARGET_H_

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#ifdef QSPI_SMSTR_CFG_USE_FIT
/* **** Test to check specific settings. **** */
/* This checks to make sure a valid version of the r_bsp package is installed. */
#if (R_BSP_VERSION_MAJOR < 0)
    #error "ERROR !!! QSPI API requires r_bsp >= v0.01"
#endif  /* (R_BSP_VERSION_MAJOR < 0) */

/* This checks to make sure the system clock frequency is specified in r_bsp package. */
#if !defined(BSP_ICLK_HZ)
    #error "ERROR !!! You must specify the System Clock Frequency (BSP_ICLK_HZ)!"
#endif  /* !defined(BSP_ICLK_HZ) */
#endif  /* QSPI_SMSTR_CFG_USE_FIT */

/* This checks to make sure the system clock frequency is specified in r_bsp package. */
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
    #error "ERROR !!! This MCU does not support QSPI channel 1."
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
#if defined(QSPI_SMSTR_CFG_CH2_INCLUDED)
    #error "ERROR !!! This MCU does not support QSPI channel 2."
#endif  /* defined(QSPI_SMSTR_CFG_CH2_INCLUDED) */


/*******************************************************************************
Macro definitions
*******************************************************************************/
/* ---- QSPI input size ---- */
#define QSPI_FIFO_HALF_SIZE     (uint32_t)(16)          /* Half size of QSPI FIFO buffer */

/* Definition of QSPI channel number */
#if   defined(QSPI_SMSTR_CFG_CH2_INCLUDED)
    #define QSPI_NUM_CHANNELS   (3)
#elif defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
    #define QSPI_NUM_CHANNELS   (2)
#elif defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
    #define QSPI_NUM_CHANNELS   (1)
#else
    #define QSPI_NUM_CHANNELS   (0)
#endif

#if (QSPI_NUM_CHANNELS == 0)
    #error "ERROR !!! The number of QSPI channels cannot be 0."
#endif

/* **** Definition of bit of Open-Drain Control Register **** */
/* ==== QSPI channel 0. ==== */
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
    /* ---- QSPCLK ---- */
    #if (('E' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) && \
         ('e' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) && \
         ('1' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT))
        #define QSPI_CH0_CLK_ODRREGNO           0
        #define QSPI_CH0_CLK_ODRBITNO           3
    #else
        #if ('4' > R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
            #define QSPI_CH0_CLK_ODRREGNO       0
        #else
            #define QSPI_CH0_CLK_ODRREGNO       1
        #endif
        
        #if   (('0' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT) || ('4' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT))
            #define QSPI_CH0_CLK_ODRBITNO       0
        #elif (('1' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT) || ('5' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT))
            #define QSPI_CH0_CLK_ODRBITNO       2
        #elif (('2' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT) || ('6' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT))
            #define QSPI_CH0_CLK_ODRBITNO       4
        #elif (('3' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT) || ('7' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT))
            #define QSPI_CH0_CLK_ODRBITNO       6
        #endif
    #endif
    
    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     7
    #elif ('8' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     8
    #elif ('9' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     9
    #elif (('A' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('a' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     A
    #elif (('B' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('b' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     B
    #elif (('C' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('c' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     C
    #elif (('D' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('d' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     D
    #elif (('E' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('e' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     E
    #elif (('F' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('f' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     F
    #elif (('G' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('g' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     G
    #elif (('J' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT) || ('j' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR     J
    #endif
    
    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR   7
    #endif

    /* ---- QMO/QIO0 ---- */
    #if (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) && \
         ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) && \
         ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
        #define QSPI_CH0_DATAIO0_ODRREGNO       0
        #define QSPI_CH0_DATAIO0_ODRBITNO       3
    #else
        #if ('4' > R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
            #define QSPI_CH0_DATAIO0_ODRREGNO   0
        #else
            #define QSPI_CH0_DATAIO0_ODRREGNO   1
        #endif
        
        #if   (('0' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT) || ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
            #define QSPI_CH0_DATAIO0_ODRBITNO   0
        #elif (('1' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT) || ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
            #define QSPI_CH0_DATAIO0_ODRBITNO   2
        #elif (('2' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT) || ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
            #define QSPI_CH0_DATAIO0_ODRBITNO   4
        #elif (('3' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT) || ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
            #define QSPI_CH0_DATAIO0_ODRBITNO   6
        #endif
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     7
    #elif ('8' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     8
    #elif ('9' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     9
    #elif (('A' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('a' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     A
    #elif (('B' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('b' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     B
    #elif (('C' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('c' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     C
    #elif (('D' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('d' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     D
    #elif (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     E
    #elif (('F' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('f' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     F
    #elif (('G' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('g' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     G
    #elif (('J' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) || ('j' == R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR     J
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR   7
    #endif
    
    /* ---- QMI/QIO1 ---- */
    #if (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) && \
         ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) && \
         ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
        #define QSPI_CH0_DATAIO1_ODRREGNO       0
        #define QSPI_CH0_DATAIO1_ODRBITNO       3
    #else
        #if ('4' > R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
            #define QSPI_CH0_DATAIO1_ODRREGNO   0
        #else
            #define QSPI_CH0_DATAIO1_ODRREGNO   1
        #endif
        
        #if   (('0' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT) || ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
            #define QSPI_CH0_DATAIO1_ODRBITNO   0
        #elif (('1' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT) || ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
            #define QSPI_CH0_DATAIO1_ODRBITNO   2
        #elif (('2' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT) || ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
            #define QSPI_CH0_DATAIO1_ODRBITNO   4
        #elif (('3' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT) || ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
            #define QSPI_CH0_DATAIO1_ODRBITNO   6
        #endif
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     7
    #elif ('8' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     8
    #elif ('9' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     9
    #elif (('A' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('a' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     A
    #elif (('B' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('b' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     B
    #elif (('C' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('c' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     C
    #elif (('D' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('d' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     D
    #elif (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     E
    #elif (('F' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('f' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     F
    #elif (('G' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('g' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     G
    #elif (('J' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) || ('j' == R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR     J
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR   7
    #endif
    
    /* ---- QIO2 ---- */
    #if (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) && \
         ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) && \
         ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT))
        #define QSPI_CH0_DATAIO2_ODRREGNO       0
        #define QSPI_CH0_DATAIO2_ODRBITNO       3
    #else
        #if ('4' > R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
            #define QSPI_CH0_DATAIO2_ODRREGNO   0
        #else
            #define QSPI_CH0_DATAIO2_ODRREGNO   1
        #endif
        
        #if   (('0' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT) || ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT))
            #define QSPI_CH0_DATAIO2_ODRBITNO   0
        #elif (('1' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT) || ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT))
            #define QSPI_CH0_DATAIO2_ODRBITNO   2
        #elif (('2' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT) || ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT))
            #define QSPI_CH0_DATAIO2_ODRBITNO   4
        #elif (('3' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT) || ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT))
            #define QSPI_CH0_DATAIO2_ODRBITNO   6
        #endif
    #endif
    
    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     7
    #elif ('8' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     8
    #elif ('9' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     9
    #elif (('A' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('a' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     A
    #elif (('B' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('b' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     B
    #elif (('C' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('c' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     C
    #elif (('D' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('d' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     D
    #elif (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     E
    #elif (('F' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('f' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     F
    #elif (('G' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('g' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     G
    #elif (('J' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT) || ('j' == R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR     J
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR   7
    #endif

    /* ---- QIO3 ---- */
    #if (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) && \
         ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) && \
         ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT))
        #define QSPI_CH0_DATAIO3_ODRREGNO       0
        #define QSPI_CH0_DATAIO3_ODRBITNO       3
    #else
        #if ('4' > R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
            #define QSPI_CH0_DATAIO3_ODRREGNO   0
        #else
            #define QSPI_CH0_DATAIO3_ODRREGNO   1
        #endif
        
        #if   (('0' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT) || ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT))
            #define QSPI_CH0_DATAIO3_ODRBITNO   0
        #elif (('1' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT) || ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT))
            #define QSPI_CH0_DATAIO3_ODRBITNO   2
        #elif (('2' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT) || ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT))
            #define QSPI_CH0_DATAIO3_ODRBITNO   4
        #elif (('3' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT) || ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT))
            #define QSPI_CH0_DATAIO3_ODRBITNO   6
        #endif
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     7
    #elif ('8' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     8
    #elif ('9' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     9
    #elif (('A' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('a' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     A
    #elif (('B' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('b' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     B
    #elif (('C' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('c' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     C
    #elif (('D' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('d' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     D
    #elif (('E' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('e' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     E
    #elif (('F' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('f' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     F
    #elif (('G' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('g' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     G
    #elif (('J' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT) || ('j' == R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT))
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR     J
    #endif

    #if   ('0' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   0
    #elif ('1' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   1
    #elif ('2' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   2
    #elif ('3' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   3
    #elif ('4' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   4
    #elif ('5' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   5
    #elif ('6' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   6
    #elif ('7' == R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT)
        #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR   7
    #endif

#endif


/* **** Definitions of string conversion to access I/O registers **** */
#define CAT_2(a,b) a##b                                     /* String "ab"                  */
#define CAT(a,b) CAT_2(a,b)                                 /* Refer to CAT_2(a,b)          */

#define SFR1_PDR( x,y )        PORT ## x.PDR.BIT.B ## y        /* String "PORTx.PDR.BIT.By"    */
#define SFR2_PDR( x,y )        SFR1_PDR( x , y )               /* Refer to __PDR( x )          */
#define SFR1_PIDR( x,y )       PORT ## x.PIDR.BIT.B ## y       /* String "PORTx.PIDR.BIT.By"   */
#define SFR2_PIDR( x,y )       SFR1_PIDR( x , y )              /* Refer to __PIDR( x )         */
#define SFR1_PODR( x,y )       PORT ## x.PODR.BIT.B ## y       /* String "PORTx.PODR.BIT.By"   */
#define SFR2_PODR( x,y )       SFR1_PODR( x , y )              /* Refer to __PODR( x )         */
#define SFR1_PMR( x,y )        PORT ## x.PMR.BIT.B ## y        /* String "PORTx.PMR.BIT.By"    */
#define SFR2_PMR( x,y )        SFR1_PMR( x , y )               /* Refer to __PMR( x )          */
#define SFR1_DSCR( x,y )       PORT ## x.DSCR.BIT.B ## y       /* String "PORTx.DCSR.BIT.By"   */
#define SFR2_DSCR( x,y )       SFR1_DSCR( x , y )              /* Refer to __DSCR( x )         */
#define SFR1_MPC( x,y )        MPC.P ## x ## y ## PFS.BYTE     /* String "MPC.PxyPFS.BYTE"     */
#define SFR2_MPC( x,y )        SFR1_MPC( x , y )               /* Refer to __MPC( x )          */
#define SFR1_ODR( x,y,z )      PORT ## x.ODR ## y.BIT.B ## z   /* String "PORTx.ODRy.BIT.Bz"   */
#define SFR2_ODR( x,y,z )      SFR1_ODR( x , y , z)            /* Refer to __ODR( x )          */
#define SFR1_PCR( x,y )        PORT ## x.PCR.BIT.B ## y        /* String "PORTx.PCR.BIT.By"    */
#define SFR2_PCR( x,y )        SFR1_PCR( x , y )               /* Refer to __PCR( x )          */


/* **** Definitions of I/O registers of ports used for QSPI **** */
/* ==== QSPI channel 0 ==== */
/* ---- Registers of the port used as QSPCLK ---- */
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
#define QSPI_CH0_PDR_CLK            SFR2_PDR(  R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR )
#define QSPI_CH0_PODR_CLK           SFR2_PODR( R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR )
#define QSPI_CH0_PMR_CLK            SFR2_PMR(  R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR )
#define QSPI_CH0_ODR_CLK            SFR2_ODR(  R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , QSPI_CH0_CLK_ODRREGNO , \
                                               QSPI_CH0_CLK_ODRBITNO )
#define QSPI_CH0_PCR_CLK            SFR2_PCR(  R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR )
#define QSPI_CH0_DSCR_CLK           SFR2_DSCR( R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR )
#define QSPI_CH0_MPC_CLK            SFR2_MPC(  R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT_SFR )

/* ---- Registers of the port used as QMO/QIO0 ---- */
#define QSPI_CH0_PDR_DATAIO0        SFR2_PDR(  R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR )
#define QSPI_CH0_PODR_DATAIO0       SFR2_PODR( R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR )
#define QSPI_CH0_PMR_DATAIO0        SFR2_PMR(  R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR )
#define QSPI_CH0_ODR_DATAIO0        SFR2_ODR(  R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , QSPI_CH0_DATAIO0_ODRREGNO , \
                                               QSPI_CH0_DATAIO0_ODRBITNO )
#define QSPI_CH0_PCR_DATAIO0        SFR2_PCR(  R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR )
#define QSPI_CH0_DSCR_DATAIO0       SFR2_DSCR( R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR )
#define QSPI_CH0_MPC_DATAIO0        SFR2_MPC(  R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT_SFR )

/* ---- Registers of the port used as QMI/QIO1 ---- */
#define QSPI_CH0_PDR_DATAIO1        SFR2_PDR(  R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR )
#define QSPI_CH0_PODR_DATAIO1       SFR2_PODR( R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR )
#define QSPI_CH0_PMR_DATAIO1        SFR2_PMR(  R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR )
#define QSPI_CH0_ODR_DATAIO1        SFR2_ODR(  R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , QSPI_CH0_DATAIO1_ODRREGNO , \
                                               QSPI_CH0_DATAIO1_ODRBITNO )
#define QSPI_CH0_PCR_DATAIO1        SFR2_PCR(  R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR )
#define QSPI_CH0_DSCR_DATAIO1       SFR2_DSCR( R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR )
#define QSPI_CH0_MPC_DATAIO1        SFR2_MPC(  R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT_SFR )

/* ---- Registers of the port used as QIO2 ---- */
#define QSPI_CH0_PDR_DATAIO2        SFR2_PDR(  R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR )
#define QSPI_CH0_PODR_DATAIO2       SFR2_PODR( R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR )
#define QSPI_CH0_PMR_DATAIO2        SFR2_PMR(  R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR )
#define QSPI_CH0_ODR_DATAIO2        SFR2_ODR(  R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , QSPI_CH0_DATAIO2_ODRREGNO , \
                                               QSPI_CH0_DATAIO2_ODRBITNO )
#define QSPI_CH0_PCR_DATAIO2        SFR2_PCR(  R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR )
#define QSPI_CH0_DSCR_DATAIO2       SFR2_DSCR( R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR )
#define QSPI_CH0_MPC_DATAIO2        SFR2_MPC(  R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT_SFR )

/* ---- Registers of the port used as QIO3 ---- */
#define QSPI_CH0_PDR_DATAIO3        SFR2_PDR(  R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR )
#define QSPI_CH0_PODR_DATAIO3       SFR2_PODR( R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR )
#define QSPI_CH0_PMR_DATAIO3        SFR2_PMR(  R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR )
#define QSPI_CH0_ODR_DATAIO3        SFR2_ODR(  R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , QSPI_CH0_DATAIO3_ODRREGNO , \
                                               QSPI_CH0_DATAIO3_ODRBITNO )
#define QSPI_CH0_PCR_DATAIO3        SFR2_PCR(  R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR )
#define QSPI_CH0_DSCR_DATAIO3       SFR2_DSCR( R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR ) 
#define QSPI_CH0_MPC_DATAIO3        SFR2_MPC(  R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT_SFR , R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT_SFR )
#endif  /* if defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */


/* **** Definitions of I/O registers of QSPI **** */
/* ---- Registers and bits of write protection ---- */
#define QSPI_PRCR               SYSTEM.PRCR.WORD                                /* Protect Register                 */
#define QSPI_PWPR_BOWI          MPC.PWPR.BIT.B0WI                               /* PFSWE Bit Write Disable (PWPR)   */
#define QSPI_PWPR_PFSWE         MPC.PWPR.BIT.PFSWE                              /* PFS Register Write Enable (PWPR) */

/* ---- Registers and bits of QSPI ---- */
#define QSPI_SPCR(channel)      (*g_qspi_base_adr[channel]).SPCR.BYTE           /* QSPI Control Register            */
#define QSPI_SSLP(channel)      (*g_qspi_base_adr[channel]).SSLP.BYTE           /* QSPI Slave Select Polarity
                                                                                    Register                        */
#define QSPI_SPPCR(channel)     (*g_qspi_base_adr[channel]).SPPCR.BYTE          /* QSPI Pin Control Register        */
#define QSPI_SPSR(channel)      (*g_qspi_base_adr[channel]).SPSR.BYTE           /* QSPI Status Register             */
#define QSPI_SPDR_LONG(channel) (*g_qspi_base_adr[channel]).SPDR.LONG           /* QSPI Data Register
                                                                                                (Longword access)   */
#define QSPI_SPDR_BYTE(channel) (*(volatile R_BSP_EVENACCESS_SFR uint8_t *)(&(*g_qspi_base_adr[channel]).SPDR.LONG))
                                                                                /* QSPI Data Register (Byte access) */
#define QSPI_SPSCR(channel)     (*g_qspi_base_adr[channel]).SPSCR.BYTE          /* QSPI Sequence Control Register   */
#define QSPI_SPBR(channel)      (*g_qspi_base_adr[channel]).SPBR.BYTE           /* QSPI Bit Rate Register           */
#define QSPI_SPDCR(channel)     (*g_qspi_base_adr[channel]).SPDCR.BYTE          /* QSPI Data Control Register       */
#define QSPI_SPCKD(channel)     (*g_qspi_base_adr[channel]).SPCKD.BYTE          /* QSPI Clock Delay Register        */
#define QSPI_SSLND(channel)     (*g_qspi_base_adr[channel]).SSLND.BYTE          /* QSPI Slave Select Negation Delay
                                                                                                        Register    */
#define QSPI_SPND(channel)      (*g_qspi_base_adr[channel]).SPND.BYTE           /* QSPI Next-Access Delay Register  */
#define QSPI_SPCMD0(channel)    (*g_qspi_base_adr[channel]).SPCMD0.WORD         /* QSPI Command Register 0          */
#define QSPI_SPCMD1(channel)    (*g_qspi_base_adr[channel]).SPCMD1.WORD         /* QSPI Command Register 1          */
#define QSPI_SPCMD2(channel)    (*g_qspi_base_adr[channel]).SPCMD2.WORD         /* QSPI Command Register 2          */
#define QSPI_SPBFCR(channel)    (*g_qspi_base_adr[channel]).SPBFCR.BYTE         /* QSPI Buffer Control register     */
#define QSPI_SPBMUL0(channel)   (*g_qspi_base_adr[channel]).SPBMUL0             /* QSPI Transfer Data Length
                                                                                    Multiplier Setting Register 0   */
#define QSPI_SPBMUL1(channel)   (*g_qspi_base_adr[channel]).SPBMUL1             /* QSPI Transfer Data Length
                                                                                    Multiplier Setting Register 1   */
#define QSPI_SPBMUL2(channel)   (*g_qspi_base_adr[channel]).SPBMUL2             /* QSPI Transfer Data Length
                                                                                    Multiplier Setting Register 2   */
#define QSPI_SPB0(channel)      (*g_qspi_base_adr[channel]).SPCMD0.BIT.SPB      /* Transfer Data Length Setting
                                                                                                        (SPCMD0)    */
#define QSPI_SPB1(channel)      (*g_qspi_base_adr[channel]).SPCMD1.BIT.SPB      /* Transfer Data Length Setting
                                                                                                        (SPCMD1)    */
#define QSPI_SPB2(channel)      (*g_qspi_base_adr[channel]).SPCMD2.BIT.SPB      /* Transfer Data Length Setting
                                                                                                        (SPCMD2)    */
#define QSPI_SPRW0(channel)     (*g_qspi_base_adr[channel]).SPCMD0.BIT.SPRW     /* SPI Read/Write Access (SPCMD0)   */
#define QSPI_SPRW1(channel)     (*g_qspi_base_adr[channel]).SPCMD1.BIT.SPRW     /* SPI Read/Write Access (SPCMD1)   */
#define QSPI_SPRW2(channel)     (*g_qspi_base_adr[channel]).SPCMD2.BIT.SPRW     /* SPI Read/Write Access (SPCMD2)   */
#define QSPI_SPIMOD0(channel)   (*g_qspi_base_adr[channel]).SPCMD0.BIT.SPIMOD   /* SPI Operating Mode (SPCMD0)      */
#define QSPI_SPIMOD1(channel)   (*g_qspi_base_adr[channel]).SPCMD1.BIT.SPIMOD   /* SPI Operating Mode (SPCMD1)      */
#define QSPI_SPIMOD2(channel)   (*g_qspi_base_adr[channel]).SPCMD2.BIT.SPIMOD   /* SPI Operating Mode (SPCMD2)      */
#define QSPI_SSLKP0(channel)    (*g_qspi_base_adr[channel]).SPCMD0.BIT.SSLKP    /* QSSL Signal Level Keeping
                                                                                                        (SPCMD0)    */
#define QSPI_SSLKP1(channel)    (*g_qspi_base_adr[channel]).SPCMD1.BIT.SSLKP    /* QSSL Signal Level Keeping
                                                                                                        (SPCMD1)    */
#define QSPI_SSLKP2(channel)    (*g_qspi_base_adr[channel]).SPCMD2.BIT.SSLKP    /* QSSL Signal Level Keeping
                                                                                                        (SPCMD2)    */
#define QSPI_SPCR_SPE(channel)  (*g_qspi_base_adr[channel]).SPCR.BIT.SPE        /* QSPI Function Enable (SPCR)      */
#define QSPI_SPSR_SPTEF(channel)    (*g_qspi_base_adr[channel]).SPSR.BIT.SPTEF  /* QSPI Transmit Buffer Empty Flag  */
#define QSPI_SPSR_SPRFF(channel)    (*g_qspi_base_adr[channel]).SPSR.BIT.SPRFF  /* QSPI Receive Buffer Full Flag    */


/* **** Definitions of data to be set to I/O registers **** */
/* ==== Data for QSPI ==== */
#define QSPI_SPCR_INIT          (uint8_t)(0x08) /* QSPI Control Register initial setting                */
                            /*  00001000B                                                               */
                            /*  |||||||+---------- Reserved                     : Write 0.              */
                            /*  ||||||+----------- QSSL Negation Interrupt      : Disable               */
                            /*  |||||+------------ Reserved                     : Write 0.              */
                            /*  ||||+------------- Master Slave Select          : Master mode (Write 1.)*/
                            /*  |||+-------------- Reserved                     : Write 0.              */
                            /*  ||+--------------- Transmit Interrupt           : Disable               */
                            /*  |+---------------- QSPI Function Enable         : Disable (SPE = 0)     */
                            /*  +----------------- Receive Interrupt            : Disable               */

#define QSPI_SPCR_TRX_SINGLE    (uint8_t)(0xE8) /* QSPI Control Register setting                        */
                            /*  11101000B          Set to enable QSPI transmission.                     */
                            /*  |||||||+---------- Reserved                     : Write 0.              */
                            /*  ||||||+----------- QSSL Negation Interrupt      : Disable               */
                            /*  |||||+------------ Reserved                     : Write 0.              */
                            /*  ||||+------------- Master Slave Select          : Master mode (Write 1.)*/
                            /*  |||+-------------- Reserved                     : Write 0.              */
                            /*  ||+--------------- Transmit Interrupt           : Disable               */
                            /*  |+---------------- QSPI Function Enable         : Enable (SPE = 1)      */
                            /*  +----------------- Receive Interrupt            : Disable               */

#define QSPI_SPCR_TX_DUALQUAD   (uint8_t)(0x68) /* QSPI Control Register setting                        */
                            /*  01101000B          Set to enable QSPI transmission.                     */
                            /*  |||||||+---------- Reserved                     : Write 0.              */
                            /*  ||||||+----------- QSSL Negation Interrupt      : Disable               */
                            /*  |||||+------------ Reserved                     : Write 0.              */
                            /*  ||||+------------- Master Slave Select          : Master mode (Write 1.)*/
                            /*  |||+-------------- Reserved                     : Write 0.              */
                            /*  ||+--------------- Transmit Interrupt           : Disable               */
                            /*  |+---------------- QSPI Function Enable         : Enable (SPE = 1)      */
                            /*  +----------------- Receive Interrupt            : Disable               */

#define QSPI_SPCR_RX_DUALQUAD   (uint8_t)(0xC8) /* QSPI Control Register setting                        */
                            /*  11001000B          Set to enable QSPI reception.                        */
                            /*  |||||||+---------- Reserved                     : Write 0.              */
                            /*  ||||||+----------- QSSL Negation Interrupt      : Disable               */
                            /*  |||||+------------ Reserved                     : Write 0.              */
                            /*  ||||+------------- Master Slave Select          : Master mode (Write 1.)*/
                            /*  |||+-------------- Reserved                     : Write 0.              */
                            /*  ||+--------------- Transmit Interrupt           : Disable               */
                            /*  |+---------------- QSPI Function Enable         : Enable (SPE = 1)      */
                            /*  +----------------- Receive Interrupt            : Disable               */

#define QSPI_SSLP_INIT          (uint8_t)(0x00) /* QSPI Slave Select Polarity Register initial setting  */
                            /*  00000000B                                                               */
                            /*  |||||||+---------- QSSL Signal Polarity Setting : Active low            */
                            /*  +++++++----------- Reserved                     : Write 0.              */

#define QSPI_SPSR_INIT          (uint8_t)(0x60) /* QSPI Status Register initial setting                 */
                            /*  01100000B                                                               */
                            /*  |||||||+---------- Reserved                     : Write 0.              */
                            /*  ||||||+----------- Reserved                     : Write 0.              */
                            /*  |||||+------------ Reserved                     : Write 0.              */
                            /*  ||||+------------- Reserved                     : Write 0.              */
                            /*  |||+-------------- QSSL Negation Flag           : Write 0.              */
                            /*  ||+--------------- Transmit Buffer Enpty Flag   : Write 1.              */
                            /*  |+---------------- Transmit End Flag            : Read Only             */
                            /*  +----------------- Receive Buffer Full Flag     : Write 0.              */

#define QSPI_SPPCR_INIT         (uint8_t)(0x36) /* QSPI Slave Select Polarity Register initial setting  */
                            /*  00110110B                                                               */
                            /*  |||||||+---------- Loopback mode                            : Normal    */
                            /*  ||||||+----------- Single/Dual-SPI QIO2 Output Fixed Value  : Output=1  */
                            /*  |||||+------------ Single/Dual-SPI QIO3 Output Fixed Value  : Output=1  */
                            /*  ||||+------------- Reserved                                 : Write 0.  */
                            /*  |||+-------------- Data Output Idle Fixed Value             : Output=1  */
                            /*  ||+--------------- Data Output Idle Value Fixing Enable : Set in MOIFV. */
                            /*  ++---------------- Reserved                                 : Write 0.  */

#define QSPI_SPSCR_INIT         (uint8_t)(0x00) /* QSPI Sequence Control Register initial setting       */
                            /*  00000000B                                                               */
                            /*  ||||||++---------- Sequence Control : Use SPCMD0 only.                  */
                            /*  ++++++------------ Reserved         : Write 0.                          */

#define QSPI_SPSCR_SEQ0         (uint8_t)(0x00) /* QSPI Sequence Control Register setting               */
                            /*  00000000B          Set sequence length to 1.                            */
                            /*  ||||||++---------- Sequence Control : Use SPCMD0 only.                  */
                            /*  ++++++------------ Reserved         : Write 0.                          */

#define QSPI_SPSCR_SEQ1         (uint8_t)(0x01) /* QSPI Sequence Control Register setting               */
                            /*  00000001B          Set sequence length to 2.                            */
                            /*  ||||||++---------- Sequence Control : Use SPCMD0 and SPCMD1.            */
                            /*  ++++++------------ Reserved         : Write 0.                          */

#define QSPI_SPSCR_SEQ2         (uint8_t)(0x02) /* QSPI Sequence Control Register setting               */
                            /*  00000010B          Set sequence length to 3.                            */
                            /*  ||||||++---------- Sequence Control : Use SPCMD0, SPCMD1 and SPCMD2.    */
                            /*  ++++++------------ Reserved         : Write 0.                          */

#define QSPI_SPDCR_INIT         (uint8_t)(0x00) /* QSPI Data Control Register initial setting           */
                            /*  00000000B                                                               */
                            /*  |+++++++---------- Reserved                         : Write 0.          */
                            /*  +----------------- Dummy Data Transmission Enable   : Disable           */

#define QSPI_SPCKD_INIT         (uint8_t)(0x00) /* QSPI Clock Delay Register initial setting            */
                            /*  00000000B                                                               */
                            /*  |||||+++---------- Clock Delay Setting              : 1.5 QSPCLK        */
                            /*  +++++------------- Reserved                         : Write 0.          */

#define QSPI_SSLND_INIT         (uint8_t)(0x00) /* QSPI Slave Select Negation Delay Register initial setting    */
                            /*  00000000B                                                               */
                            /*  |||||+++---------- QSSL Negation Delay Setting      : 1 QSPCLK          */
                            /*  +++++------------- Reserved                         : Write 0.          */

#define QSPI_SPND_INIT          (uint8_t)(0x00) /* QSPI Next-Access Delay Register initial setting      */
                            /*  00000000B                                                               */
                            /*  |||||+++---------- Next-Access Delay Setting        : 1 QSPCLK          */
                            /*  +++++------------- Reserved                         : Write 0.          */

#define QSPI_SPCMD_INIT         (uint16_t)(0xE203)  /* QSPI Command Register initial setting            */
                            /*  1110001000000011B                                                       */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : CPHA=1 (SPI clock mode 3) */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : CPOL=1 (SPI clock mode 3) */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Invalid bit in Single-SPI */
                            /*  |||||||||++------- SPI Operating Mode       : Single-SPI                */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : 32 bits                   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPCMD_TX_SINGLE    (uint16_t)(0xE003)  /* QSPI Command Register setting                    */
                            /*  1110000000000011B  Set to transmit data in Single-SPI mode.             */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : (Use value set before.)   */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : (Use value set before.)   */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Invalid bit in Single-SPI */
                            /*  |||||||||++------- SPI Operating Mode       : Single-SPI                */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : (Use value set before.)   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPCMD_TX_DUAL      (uint16_t)(0xE023)  /* QSPI Command Register setting                    */
                            /*  1110000000100011B  Set to transmit data in Dual-SPI mode.               */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : (Use value set before.)   */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : (Use value set before.)   */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Write operation           */
                            /*  |||||||||++------- SPI Operating Mode       : Dual-SPI                  */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : (Use value set before.)   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPCMD_TX_QUAD      (uint16_t)(0xE043)  /* QSPI Command Register setting                    */
                            /*  1110000001000011B  Set to transmit data in Quad-SPI mode.               */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : (Use value set before.)   */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : (Use value set before.)   */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Write operation           */
                            /*  |||||||||++------- SPI Operating Mode       : Quad-SPI                  */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : (Use value set before.)   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPCMD_RX_SINGLE    (uint16_t)(0xE003)  /* QSPI Command Register setting                    */
                            /*  1110000000000011B  Set to receive data in Single-SPI mode.              */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : (Use value set before.)   */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : (Use value set before.)   */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Invalid bit in Single-SPI */
                            /*  |||||||||++------- SPI Operating Mode       : Single-SPI                */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : (Use value set before.)   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPCMD_RX_DUAL      (uint16_t)(0xE033)  /* QSPI Command Register setting                    */
                            /*  1110000000110011B  Set to receive data in Dual-SPI mode.                */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : (Use value set before.)   */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : (Use value set before.)   */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Read operation            */
                            /*  |||||||||++------- SPI Operating Mode       : Dual-SPI                  */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : (Use value set before.)   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPCMD_RX_QUAD      (uint16_t)(0xE053)  /* QSPI Command Register setting                    */
                            /*  1110000001010011B  Set to receive data in Quad-SPI mode.                */
                            /*  |||||||||||||||+-- QSPCLK Phase Setting     : (Use value set before.)   */
                            /*  ||||||||||||||+--- QSPCLK Polarity Setting  : (Use value set before.)   */
                            /*  ||||||||||||++---- Bit Rate Division        : Base bit rate             */
                            /*  |||||||||||+------ SPI Read/Write Access    : Read operation            */
                            /*  |||||||||++------- SPI Operating Mode       : Quad-SPI                  */
                            /*  ||||||||+--------- QSSL Signal Level        : Negate QSSL.              */
                            /*  ||||++++---------- Transfer Data Length     : (Use value set before.)   */
                            /*  |||+-------------- LSB First                : MSB first                 */
                            /*  ||+--------------- Next-Access Delay        : Delay = SPND setting      */
                            /*  |+---------------- QSSL Negation Delay      : Delay = SSLND setting     */
                            /*  +----------------- Clock Delay              : Delay = SPCKD setting     */

#define QSPI_SPBFCR_INIT        (uint8_t)(0x30) /* QSPI Buffer Control Register initial setting         */
                            /*  00110000B                                                               */
                            /*  |||||+++---------- Receive Buffer Data Triggering Number    : 1 byte    */
                            /*  ||+++------------- Transmit Buffer Data Triggering Number   : 0 byte    */
                            /*  |+---------------- Receive Buffer Data Reset    : Normal operation      */
                            /*  +----------------- Transmit Buffer Data Reset   : Normal operation      */

#define QSPI_SPBFCR_BYTE        (uint8_t)(0x30) /* QSPI Buffer Control Register setting                 */
                            /*  00110000B          Set to transmit or receive byte data.                */
                            /*  |||||+++---------- Receive Buffer Data Triggering Number    : 1 byte    */
                            /*  ||+++------------- Transmit Buffer Data Triggering Number   : 0 byte    */
                            /*  |+---------------- Receive Buffer Data Reset    : Normal operation      */
                            /*  +----------------- Transmit Buffer Data Reset   : Normal operation      */

#define QSPI_SPBFCR_FIFO_HALF_SIZE  (uint8_t)(0x1d) /* QSPI Buffer Control Register setting             */
                            /*  00011101B          Set to transmit or receive longword data.            */
                            /*  |||||+++---------- Receive Buffer Data Triggering Number    : 16 bytes  */
                            /*  ||+++------------- Transmit Buffer Data Triggering Number   : 16 bytes  */
                            /*  |+---------------- Receive Buffer Data Reset    : Normal operation      */
                            /*  +----------------- Transmit Buffer Data Reset   : Normal operation      */

#define QSPI_SPBFCR_RST         (uint8_t)(0xC0) /* QSPI Buffer Control Register setting                 */
                            /*  11000000B          Set to reset transmit and receive buffer.            */
                            /*  |||||+++---------- Receive Buffer Data Triggering Number    : 1 byte    */
                            /*  ||+++------------- Transmit Buffer Data Triggering Number   : 31 bytes  */
                            /*  |+---------------- Receive Buffer Data Reset    : Reset buffer          */
                            /*  +----------------- Transmit Buffer Data Reset   : Reset buffer          */

/* ==== Data for Register Write Protection Function ==== */
#define QSPI_PRCR_ENABLE        (uint16_t)(0xA502)  /* Protect Register setting                         */
                            /*  1010010100000010B  Set to enable writing to MSTPCR.                     */
                            /*  |||||||||||||||+-- Protect bit0         : Write disabled                */
                            /*  ||||||||||||||+--- Protect bit1         : Write enabled                 */
                            /*  |||||||||||||+---- Reserved             : Write 0.                      */
                            /*  ||||||||||||+----- Protect bit3         : Write disabled                */
                            /*  ||||||||++++------ Reserved             : Write 0.                      */
                            /*  ++++++++---------- PRC Key Code         : A5h (Write enabled)           */

#define QSPI_PRCR_DISABLE       (uint16_t)(0xA500)  /* PRCR Protect Register setting  */
                            /*  1010010100000000B  Set to disable writing to MSTPCR.                    */
                            /*  |||||||||||||||+-- Protect bit0         : Write disabled                */
                            /*  ||||||||||||||+--- Protect bit1         : Write disabled                */
                            /*  |||||||||||||+---- Reserved             : Write 0.                      */
                            /*  ||||||||||||+----- Protect bit3         : Write disabled                */
                            /*  ||||||||++++------ Reserved             : Write 0.                      */
                            /*  ++++++++---------- PRC Key Code         : A5h (Write enabled)           */


/* ==== Data for Multi-Function Pin Controller ==== */
/* ---- QSPI channel 0 ---- */
#define QSPI_MPCCLK_INIT        (uint8_t)(0x00) /* Port Pin Function Control Register initial setting   */
                            /*  00000000B          Set port to Hi-Z.                                    */
                            /*  |||+++++---------- Pin Function Select              : Hi-Z              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO0_INIT    (uint8_t)(0x00) /* Port Pin Function Control Register initial setting   */
                            /*  00000000B          Set port to Hi-Z.                                    */
                            /*  |||+++++---------- Pin Function Select              : Hi-Z              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO1_INIT    (uint8_t)(0x00) /* Port Pin Function Control Register initial setting   */
                            /*  00000000B          Set port to Hi-Z.                                    */
                            /*  |||+++++---------- Pin Function Select              : Hi-Z              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO2_INIT    (uint8_t)(0x00) /* Port Pin Function Control Register initial setting   */
                            /*  00000000B          Set port to Hi-Z.                                    */
                            /*  |||+++++---------- Pin Function Select              : Hi-Z              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO3_INIT    (uint8_t)(0x00) /* Port Pin Function Control Register initial setting   */
                            /*  00000000B          Set port to Hi-Z.                                    */
                            /*  |||+++++---------- Pin Function Select              : Hi-Z              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCCLK_ENABLE      (uint8_t)(0x1B) /* Port Pin Function Control Register setting           */
                            /*  00011011B          Set port used as QSPCLK of QSPI.                     */
                            /*  |||+++++---------- Pin Function Select              : QSPCLK            */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO0_ENABLE  (uint8_t)(0x1B) /* Port Pin Function Control Register setting           */
                            /*  00011011B          Set port used as QMO/QIO0 of QSPI.                   */
                            /*  |||+++++---------- Pin Function Select              : QMO/QIO0          */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO1_ENABLE  (uint8_t)(0x1B) /* Port Pin Function Control Register setting           */
                            /*  00011011B          Set port used as QMI/QIO1 of QSPI.                   */
                            /*  |||+++++---------- Pin Function Select              : QMI/QIO1          */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO2_ENABLE  (uint8_t)(0x1B) /* Port Pin Function Control Register setting           */
                            /*  00011011B          Set port used as QIO2 of QSPI.                       */
                            /*  |||+++++---------- Pin Function Select              : QIO2              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */

#define QSPI_MPCDATAIO3_ENABLE  (uint8_t)(0x1B) /* Port Pin Function Control Register setting           */
                            /*  00011011B          Set port used as QIO3 of QSPI.                       */
                            /*  |||+++++---------- Pin Function Select              : QIO3              */
                            /*  ||+--------------- Reserved                         : Write 0.          */
                            /*  |+---------------- Interrupt Input Function Select  : Not use.          */
                            /*  +----------------- Analog Input Function Select     : Not use.          */


/*******************************************************************************
Private variables and functions
*******************************************************************************/


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
/* r_qspi_smstr_target.c */
qspi_smstr_status_t r_qspi_smstr_ch_check(uint8_t channel);
void     r_qspi_smstr_enable(uint8_t channel, uint8_t br_data);
void     r_qspi_smstr_disable(uint8_t channel);
void     r_qspi_smstr_change(uint8_t channel, uint8_t num_mode, uint8_t br_data);
void     r_qspi_smstr_data_set_long(uint8_t channel, uint32_t set_data);
void     r_qspi_smstr_data_set_byte(uint8_t channel, uint8_t set_data);
uint32_t r_qspi_smstr_data_get_long(uint8_t channel);
uint8_t  r_qspi_smstr_data_get_byte(uint8_t channel);
void     r_qspi_smstr_sptef_clear(uint8_t channel);
void     r_qspi_smstr_sprff_clear(uint8_t channel);
void     r_qspi_smstr_spsslf_clear(uint8_t channel);
volatile uint8_t R_BSP_EVENACCESS_SFR * r_qspi_smstr_spsr_addr(uint8_t channel);
void     r_qspi_smstr_trx_enable_single(uint8_t channel, uint32_t size);
void r_qspi_smstr_trx_enable_single_dmacdtc(uint8_t channel, uint32_t size);
void     r_qspi_smstr_trx_disable(uint8_t channel);
void     r_qspi_smstr_tx_enable_dual(uint8_t channel, uint32_t size);
void     r_qspi_smstr_tx_enable_quad(uint8_t channel, uint32_t size);
void     r_qspi_smstr_tx_disable(uint8_t channel);
void     r_qspi_smstr_rx_enable_dual(uint8_t channel, uint32_t size);
void     r_qspi_smstr_rx_enable_quad(uint8_t channel, uint32_t size);
void     r_qspi_smstr_rx_disable(uint8_t channel);
void     r_qspi_smstr_get_buffregaddress(uint8_t channel, uint32_t *p_spdr_adr);

/* r_qspi_smstr_target_dev_port.c */
void     r_qspi_smstr_io_init(uint8_t channel);
void     r_qspi_smstr_io_reset(uint8_t channel);
void     r_qspi_smstr_mpc_enable(uint8_t channel);
void     r_qspi_smstr_mpc_disable(uint8_t channel);
void     r_qspi_smstr_dataio0_init(uint8_t channel);
void     r_qspi_smstr_dataio0_reset(uint8_t channel);
void     r_qspi_smstr_dataio1_init(uint8_t channel);
void     r_qspi_smstr_dataio1_reset(uint8_t channel);
void     r_qspi_smstr_dataio2_init(uint8_t channel);
void     r_qspi_smstr_dataio2_reset(uint8_t channel);
void     r_qspi_smstr_dataio3_init(uint8_t channel);
void     r_qspi_smstr_dataio3_reset(uint8_t channel);
void     r_qspi_smstr_clk_init(uint8_t channel);
void     r_qspi_smstr_clk_reset(uint8_t channel);
void     r_qspi_smstr_module_enable(uint8_t channel);
void     r_qspi_smstr_module_disable(uint8_t channel);
qspi_smstr_status_t r_qspi_smstr_tx_dmacdtc_wait(uint8_t channel, uint32_t size);
qspi_smstr_status_t r_qspi_smstr_rx_dmacdtc_wait(uint8_t channel, uint32_t size);
void     r_qspi_smstr_int_spti_init(uint8_t channel);
void     r_qspi_smstr_int_spri_init(uint8_t channel);
void     r_qspi_smstr_int_spti_ier_set(uint8_t channel);
void     r_qspi_smstr_int_spri_ier_set(uint8_t channel);


#endif /* _QSPI_SMSTR_TARGET_H_ */

/* End of File */
