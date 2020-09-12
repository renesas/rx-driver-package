/* Generated configuration header file - do not edit */
/***********************************************************************************************************************
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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_can_rx_config_reference.h
* Description  : Configures RX CAN FIT code.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 02.01.2015 1.10    For 64M, 71M, 65N.
*         : 30.10.2015 2.02    FIT update, new R_CAN_Create interface.
*         : 3.3.2016   2.10    65N added.
*         : 1.30.2017  2.11    Tested with 65N 2MB. (Only comments change.)
*         : 8.14.2017  2.12    Release version for 65N 2MB. Bug fix in r_can_rx.c.
*         : 27.04.2018 2.13WS  - Added RX66T
*         : 26.09.2018 2.13    Revised r_can_rx_config_reference.h for use with Smart Configurator
*         : 08.01.2019 2.15   - Added RX72T
*         : 30.04.2019 3.10   - Added RX72M
*         : 30.12.2019 3.20   - Added support RX66N, RX72N
*         :                   - Fixed to comply with GSCE Coding Standards Rev.6.00.
***********************************************************************************************************************/
#ifndef CAN_CONFIG_HEADER_FILE
#define CAN_CONFIG_HEADER_FILE

/* Includes board and MCU related header files. */
#include "platform.h"

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Set to 0 to use the CAN interrupts. Set to 1 if you want to poll CAN 
mailboxes for messages sent and received. */
#define USE_CAN_POLL        (0)   /* 1 polled mode, 0 use interrupts (=default). */

/* Level */
#define CAN0_INT_LVL        (2)
#define CAN1_INT_LVL        (2)
#define CAN2_INT_LVL        (2)

/**********************************************************************************************************************
 * Select CAN ID-type for the driver, that is, usage of 11-bit or 29-bit IDs. The FRAME_ID_MODE can be set to
 * STD_ID_MODE, EXT_ID_MODE, or MIXED_ID_MODE. The first two settings enable only those API functions belonging
 * to that ID mode. If it is set to mixed mode, the whole API becomes available.
 *
 * MIXED_ID_MODE *must* be used in a network with both types of frames. Otherwise messages can be misinterprted
 * by the CAN hardware. Choose ONLY ONE. Again; use MIXED_ID_MODE if there will be Standard AND Extended CAN frames
 * on the bus, else, unexpected data may result. 
 * #define FRAME_ID_MODE   (STD_ID_MODE)       11-bit CAN ID. default
 * #define FRAME_ID_MODE   EXT_ID_MODE         29-bit CAN ID. 
 * #define FRAME_ID_MODE   MIXED_ID_MODE
 ***********************************************************************************************************************/
#define FRAME_ID_MODE   (STD_ID_MODE)

/**********************************************************************************************************************
 * PORT SETUP *
 * Configure the CAN0 RX and TX pins to be used by the CAN port configuration
 * macros. The RX may have up to 3 CAN channels depending on version.
 * Check RX Group User Manual for port availability for your device
 * Pins are defined here whether the CAN channel is in use or not to allow driver compatibility with all supported devices.
 * Pins defined here for unused CAN channels are ignored and remain available for other uses.
 ***********************************************************************************************************************/
#define CAN_USE_CAN0    (1)
#define CAN0_RX_PORT    (P33) /* Options for RX64M, RX65N, RX71M, RX72M, RX72N, RX66N: P33, PD2. RX72T, RX66T: P22, PA1, PA7, PB6, PC6, PE0, PF3 */
#define CAN0_TX_PORT    (P32) /* Options for RX64M, RX65N, RX71M, RX72M, RX72N, RX66N: P32, PD1. RX72T, RX66T: P23, PA0, PA6, PB5, PC5, PD7, PF2 */

#define CAN_USE_CAN1    (0)
#define CAN1_RX_PORT    (P15) /* Options for RX64M, RX65N, RX71M, RX72M, RX72N, RX66N: P15, P55. */
#define CAN1_TX_PORT    (P14) /* Options for RX64M, RX65N, RX71M, RX72M, RX72N, RX66N: P14, P54. */

#define CAN_USE_CAN2    (0)
#define CAN2_RX_PORT    (P67) /* RX64M, RX71M, RX72M, RX72N, RX66N only */
#define CAN2_TX_PORT    (P66) /* RX64M, RX71M, RX72M, RX72N, RX66N only */


/**********************************************************************************************************************
 * CAN transceiver ICs that have Standby and Enable pins can be supported by this driver by using the following defines.
 * Set the defines as required for Port#, bit# and active level as in the examples.
 ***********************************************************************************************************************/
#define CAN_USE_CAN0_STANDBY_ENABLE_PINS (0)
#if (CAN_USE_CAN0_STANDBY_ENABLE_PINS == 1)
/* Configure CAN0 STBn GPIO output pin. */
    #define CAN0_TRX_STB_PORT       (0)
    #define CAN0_TRX_STB_PIN        (0)
    #define CAN0_TRX_STB_LVL        (0)  /* Choose High or Low active state for CAN Transceiver standby pin. */

    /* Configure CAN0 EN GPIO output pin. */
    #define CAN0_TRX_ENABLE_PORT    (0)
    #define CAN0_TRX_ENABLE_PIN     (0)
    #define CAN0_TRX_ENABLE_LVL     (0)   /* Choose High or Low active state for CAN Transceiver standby pin. */
#endif

#define CAN_USE_CAN1_STANDBY_ENABLE_PINS (0)
#if (CAN_USE_CAN1_STANDBY_ENABLE_PINS == 1)
    /* Configure CAN1 STBn GPIO output pin.
    Output. High = not standby. */
    #define CAN1_TRX_STB_PORT       (0)
    #define CAN1_TRX_STB_PIN        (0)
    #define CAN1_TRX_STB_LVL        (0)   /* Choose High or Low active state for CAN Transceiver standby pin. */

    /* Configure CAN1 EN GPIO output pin. */
    #define CAN1_TRX_ENABLE_PORT    (0)
    #define CAN1_TRX_ENABLE_PIN     (0)
    #define CAN1_TRX_ENABLE_LVL     (0)  /* Choose High or Low active state for CAN Transceiver standby pin. */
#endif

#define CAN_USE_CAN2_STANDBY_ENABLE_PINS (0)
#if (CAN_USE_CAN2_STANDBY_ENABLE_PINS == 1)
    /* Configure CAN2 STB GPIO output pin.
    Output. High = not standby. */
    #define CAN2_TRX_STB_PORT       (0)
    #define CAN2_TRX_STB_PIN        (0)
    #define CAN2_TRX_STB_LVL        (0)   /* Choose High or Low active state for CAN Transceiver standby pin. */

    /* Configure CAN2 EN GPIO output pin. */
    #define CAN2_TRX_ENABLE_PORT    (0)
    #define CAN2_TRX_ENABLE_PIN     (0)
    #define CAN2_TRX_ENABLE_LVL     (0)   /* Choose High or Low active state for CAN Transceiver standby pin. */
#endif


/**********************************************************************************************************************
 * CAN BITRATE Settings **
 * See 'CAN Communication Speed Setting', and 'Bit Rate' sections in HW-manual.
 *
 * CCLKS is 0 => running on PCLK which is PCLKB, that is,
 * FCAN = PCLK = PCLKB.
 * CAN_BRP = Baudrate prescaling.
 * FCANCLK = FCAN / CAN_BRP
 * P = value selected in BRP[9:0] bits in BCR (P = 0 to 1023). P + 1 = CAN_BRP.
 * TQTOT = Nr CAN clocks in one CAN bit = FCANCLK/BITRATE.
 *
 * With CCLKS = 0, and using r_bsp macros we get:
 * FCAN = (BSP_CFG_XTAL_HZ * BSP_CFG_PLL_MUL / BSP_CFG_PCKB_DIV)   (Eq. 1)
 * TQTOT = (FCAN/(CAN_BRP * BITRATE))                              (Eq. 2)
 *
 * Eq. (1) in (2):
 * TQTOT = (BSP_CFG_XTAL_HZ * BSP_CFG_PLL_MUL / BSP_CFG_PCKB_DIV)/(CAN_BRP * BITRATE)), or
 * TQTOT = (BSP_CFG_XTAL_HZ * BSP_CFG_PLL_MUL)/(CAN_BRP * BITRATE * BSP_CFG_PCKB_DIV) (Eq. 3)
 ***********************************************************************************************************************/
    /* Example A). Desired baudrate 500 kbps.
     * Try CAN_BRP = 4. Equation 3:
     * TQTOT = (24000000 * 10)/(4 * 500000 * 4) = 30. This is too large. TQTOT can be max 25.
     * Try CAN_BRP = 5.
     * TQTOT = (BSP_CFG_XTAL_HZ * BSP_CFG_PLL_MUL)/(CAN_BRP * BITRATE * BSP_CFG_PCKB_DIV) =
     * = (24000000 * 10)/(5 * 500000 * 4) = ***24***
     * TQTOT = 24 = TSEG1 + TSEG2 + SS:
     *  Try:
     *      SS = 1 Tq always.
     *      TSEG1 = 15 Tq
     *      TSEG2 = 8 Tq
     *      ============
     *      SUM = 24
     */
#define BITRATE    (500000)
#define CAN_BRP     (5)
#define CAN_TSEG1   (15)
#define CAN_TSEG2   (8)
#define CAN_SJW     (2) /* Re-synchronization Control (SJW) should be 1-4 Tq (must be <=TSEG2). */

/** End bitrate setting. **************************************************************************/


/* Max loops to poll a CAN register bit for expected value. If you are using 
 * polled mode, and If you wish to wait a certain time to check that a mailbox
 * has recieved a frame, increase this value. This can be set to a very low value,
 * but do not set to zero or the mailbox may not be checked at all!
 */
#define MAX_CANREG_POLLCYCLES   (5)

#endif    /* CAN_CONFIG_HEADER_FILE */
/* eof */
