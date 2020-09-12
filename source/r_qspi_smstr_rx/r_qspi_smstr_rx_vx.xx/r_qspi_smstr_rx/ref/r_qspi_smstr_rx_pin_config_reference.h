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
* File Name    : r_qspi_smstr_rx_pin_config.h
* Version      : 1.14
* Device       : RX
* Abstract     : Configuration file for QSPI single master driver
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
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
*              : 30.09.2016 1.09     Changed Tool-Chain version.
*              :                     Changed pin name.
*              :                     Added RX65N.
*              : 31.07.2017 2.08     Supported RX65N-2MB.
*              :                     Fixed to correspond to Renesas coding rule.
*              : 30.07.2019 1.13     Added RX72M.
*              : 22.11.2019 1.14     Added RX72N and RX66N.
*******************************************************************************/
#ifndef QSPI_SMSTR_PIN_CONFIG_H
#define QSPI_SMSTR_PIN_CONFIG_H

/******************************************************************************
SPECIFY PORTS USED FOR QSPI
******************************************************************************/
/* The #defines specify the ports used for Clock, Data Output and Data Input
   of QSPI individually. */

#if   defined(BSP_MCU_RX64M)
    /* ==== QSPI channel 0 ==== */
    /* ---- Clock port ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT   ('7')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT    ('7')     /* Bit Number  */

    /* ---- Data Input/Output port 0 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT     ('C')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT      ('3')     /* Bit Number  */

    /* ---- Data Input/Output port 1 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT     ('C')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT      ('4')     /* Bit Number  */

    /* ---- Data Input/Output port 2 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT     ('8')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT      ('0')     /* Bit Number  */

    /* ---- Data Input/Output port 3 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT     ('8')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT      ('1')     /* Bit Number  */

#elif defined(BSP_MCU_RX65N)
    /* ==== QSPI channel 0 ==== */
    /* ---- Clock port ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT   ('D')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT    ('5')     /* Bit Number  */

    /* ---- Data Input/Output port 0 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT     ('D')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT      ('6')     /* Bit Number  */

    /* ---- Data Input/Output port 1 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT     ('D')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT      ('7')     /* Bit Number  */

    /* ---- Data Input/Output port 2 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT     ('D')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT      ('2')     /* Bit Number  */

    /* ---- Data Input/Output port 3 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT     ('D')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT      ('3')     /* Bit Number  */

#elif defined(BSP_MCU_RX66N)
    /* ==== QSPI channel 0 ==== */
    /* ---- Clock port ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT   ('N')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT    ('4')     /* Bit Number  */

    /* ---- Data Input/Output port 0 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT     ('J')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT      ('3')     /* Bit Number  */

    /* ---- Data Input/Output port 1 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT     ('J')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT      ('5')     /* Bit Number  */

    /* ---- Data Input/Output port 2 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT     ('0')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT      ('0')     /* Bit Number  */

    /* ---- Data Input/Output port 3 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT     ('0')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT      ('1')     /* Bit Number  */

#elif defined(BSP_MCU_RX71M)
    /* ==== QSPI channel 0 ==== */
    /* ---- Clock port ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT   ('7')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT    ('7')     /* Bit Number  */

    /* ---- Data Input/Output port 0 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT     ('C')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT      ('3')     /* Bit Number  */

    /* ---- Data Input/Output port 1 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT     ('C')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT      ('4')     /* Bit Number  */

    /* ---- Data Input/Output port 2 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT     ('8')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT      ('0')     /* Bit Number  */

    /* ---- Data Input/Output port 3 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT     ('8')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT      ('1')     /* Bit Number  */

#elif defined(BSP_MCU_RX72M)
    /* ==== QSPI channel 0 ==== */
    /* ---- Clock port ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT   ('7')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT    ('7')     /* Bit Number  */

    /* ---- Data Input/Output port 0 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT     ('C')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT      ('3')     /* Bit Number  */

    /* ---- Data Input/Output port 1 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT     ('C')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT      ('4')     /* Bit Number  */

    /* ---- Data Input/Output port 2 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT     ('8')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT      ('0')     /* Bit Number  */

    /* ---- Data Input/Output port 3 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT     ('8')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT      ('1')     /* Bit Number  */

#elif defined(BSP_MCU_RX72N)
    /* ==== QSPI channel 0 ==== */
    /* ---- Clock port ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_PORT   ('N')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QSPCLK_BIT    ('4')     /* Bit Number  */

    /* ---- Data Input/Output port 0 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT     ('J')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT      ('3')     /* Bit Number  */

    /* ---- Data Input/Output port 1 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT     ('J')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT      ('5')     /* Bit Number  */

    /* ---- Data Input/Output port 2 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_PORT     ('0')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO2_BIT      ('0')     /* Bit Number  */

    /* ---- Data Input/Output port 3 ---- */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_PORT     ('0')     /* Port Number */
    #define R_QSPI_SMSTR_CFG_QSPI_QIO3_BIT      ('1')     /* Bit Number  */

#else
    #error "This MCU is not supported by the current r_qspi_smstr_rx module."
#endif  /* defined(BSP_MCU_RX64M) */

#endif /* _QSPI_API_PIN_CONFIG_H */


/* End of File */

