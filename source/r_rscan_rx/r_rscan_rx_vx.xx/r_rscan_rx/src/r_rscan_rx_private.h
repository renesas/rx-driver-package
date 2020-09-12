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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rscan_rx_private.h
* Description  : Internal structures and definitions for the CAN driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.03.2015 1.00    Initial Release
*           06.12.2018 1.10    Fixed big endian bug. Added __evenaccess to structure and union definitions.
*           20.05.2019 2.00    Add support for GNUC and ICCRX.
***********************************************************************************************************************/

#ifndef CAN_PRIVATE_HEADER_FILE
#define CAN_PRIVATE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_rscan_rx_config.h"
#include "r_rscan_rx_if.h"              

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/*****************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
 * DRIVER CONTROL STRUCTURES
 ******************************************************************************/

typedef R_BSP_EVENACCESS struct st_can_dcb      // driver control block
{
    bool        opened;
    uint16_t    rule_count;     // number of total rules applied to module
    uint32_t    fifo_mask;      // mask of configured FIFOs
    void        (*p_callback)(can_cb_evt_t event, void *p_args);
} can_dcb_t;

typedef R_BSP_EVENACCESS struct st_can_ccb      // channel control block
{
    bool        initialized;
    void        (*p_chcallback)(uint8_t       chan,
                                can_cb_evt_t  event,
                                void          *p_args);
} can_ccb_t;


/******************************************************************************
 * GLOBAL OPERATION DEFINITIONS
 ******************************************************************************/
/*
 * GCTR: Global Control Register
 */
typedef enum e_can_global_mode
{
    CAN_GLOBAL_MODE_OPERATING = 0,
    CAN_GLOBAL_MODE_RESET     = 1,
    CAN_GLOBAL_MODE_TEST      = 2,
    CAN_GLOBAL_MODE_END_ENUM
} can_global_mode_t;


/******************************************************************************
 * CHANNEL OPERATION DEFINITIONS
 ******************************************************************************/

typedef enum e_can_chmode
{
    CAN_CHMODE_COMM = 0,
    CAN_CHMODE_RESET = 1,
    CAN_CHMODE_HALT = 2,
    CAN_CHMODE_END_ENUM
} can_chmode_t;

typedef enum e_bo_rmode
{
    CAN_BO_RECOVERY_ISO = 0,
    CAN_BO_RECOVERY_CH_HALT_AT_BO_ENTRY = 1,
    CAN_BO_RECOVERY_CH_HALT_AT_BO_END = 2,
    CAN_BO_RECOVERY_CH_HALT_DURING_BO = 3,
    CAN_BO_RECOVERY_END_ENUM
} can_bo_rmode_t;

typedef enum e_test_mode
{
    CAN_TEST_MODE_STD = 0,
    CAN_TEST_MODE_LISTEN = 1,
    CAN_TEST_MODE_EXT_LOOPBACK = 2,
    CAN_TEST_MODE_INT_LOOPBACK = 3,
    CAN_TEST_MODE_END_ENUM
} can_test_mode_t;


/******************************************************************************
 * MESSAGE STRUCTURE
 ******************************************************************************/
/*
 * Adapted from iodefine.h (removed TM, RM, RF, and CF prefixes; trailing buffer number)
 * arr[4]  at (can_msg_t *)&RSCAN0.TMIDL0;      // transmit mailboxes
 *            (can_msg_t *)&RSCAN0.CFIDL0;      // txfifo
 * arr[16] at (can_msg_t *)&RSCAN.RMIDL0;       // receive mailboxes
 * arr[2]  at (can_msg_t *)&RSCAN.RFIDL0;       // rxfifos
 */

typedef R_BSP_EVENACCESS struct st_can_msg
{
    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_1 (
            unsigned short ID:16
        ) BIT;
    } IDL;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_4 (
            unsigned short IDE:1,
            unsigned short RTR:1,
            unsigned short THLEN:1,
            unsigned short ID:13
        ) BIT;
    } IDH;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_1 (
            unsigned short TS:16
        ) BIT;
    } TS;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_2 (
            unsigned short DLC:4,
            unsigned short PTR:12      /* only 8-bits for transmit*/
        ) BIT;
    } PTR;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_2 (
            unsigned short DB1:8,
            unsigned short DB0:8
        ) BIT;
    } DF0;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_2 (
            unsigned short DB3:8,
            unsigned short DB2:8
        ) BIT;
    } DF1;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_2 (
            unsigned short DB5:8,
            unsigned short DB4:8
        ) BIT;
    } DF2;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_2 (
            unsigned short DB7:8,
            unsigned short DB6:8
        ) BIT;
    } DF3;
} can_msg_t;


/******************************************************************************
 * TRANSMIT BUFFER DEFINITIONS
 ******************************************************************************/
/*
 * TMCp: Transmit Buffer control register (p=0-3); adapted from iodefine.h
 *   array[4] at &RSCAN0.TMCP0
 */
typedef R_BSP_EVENACCESS union {
    unsigned char BYTE;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_4 (
        unsigned char :5,
        unsigned char TMOM:1,
        unsigned char TMTAR:1,
        unsigned char TMTR:1
    ) BIT;
} can_tmcp_t;

#define CAN_TMCP_REQ_PRESENT   (0x03)

/*
 * TMSTSp: Transmit Buffer status register (p=0-3); adapted from iodefine.h
 *   array[4] at &RSCAN0.TMSTS0
 */
typedef R_BSP_EVENACCESS union {
    unsigned char BYTE;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_5 (
        unsigned char :3,
        unsigned char TMTARM:1,
        unsigned char TMTRM:1,
        unsigned char TMTRF:2,
        unsigned char TMTSTS:1
    ) BIT;
} can_tmstsp_t;


/******************************************************************************
 * COMMON FIFO DEFINTIONS
 ******************************************************************************/

typedef enum e_fifo_depth
{
    CAN_FIFO_DEPTH_0   = 0,
    CAN_FIFO_DEPTH_4   = 1,
    CAN_FIFO_DEPTH_8   = 2,
    CAN_FIFO_DEPTH_16  = 3,
    CAN_FIFO_DEPTH_END_ENUM
} can_fifo_depth_t;

/*
 * All FIFO status (full, empty, overflow)
 */
typedef R_BSP_EVENACCESS union
{
    uint32_t        uint32;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_29
    (
        uint32_t    :4,
        /* history FIFOs */
        uint32_t    ch4_hist:1,
        uint32_t    ch3_hist:1,
        uint32_t    ch2_hist:1,
        uint32_t    ch1_hist:1,
        uint32_t    ch0_hist:1,         /* used*/
        /* bits 8-22 = TX/RX FIFOs 0-14 */
        uint32_t    ch4_gwfifo:1,
        uint32_t    ch4_txfifo_1:1,
        uint32_t    ch4_txfifo_0:1,
        uint32_t    ch3_gwfifo:1,
        uint32_t    ch3_txfifo_1:1,
        uint32_t    ch3_txfifo_0:1,
        uint32_t    ch2_gwfifo:1,
        uint32_t    ch2_txfifo_1:1,
        uint32_t    ch2_txfifo_0:1,
        uint32_t    ch1_gwfifo:1,
        uint32_t    ch1_txfifo_1:1,
        uint32_t    ch1_txfifo_0:1,
        uint32_t    ch0_gwfifo:1,
        uint32_t    ch0_txfifo_1:1,
        uint32_t    ch0_txfifo_0:1,     /* used*/
        /* bits 0-7 = RX FIFOs 0-7 */
        uint32_t    rxfifo_7:1,
        uint32_t    rxfifo_6:1,
        uint32_t    rxfifo_5:1,
        uint32_t    rxfifo_4:1,
        uint32_t    rxfifo_3:1,
        uint32_t    rxfifo_2:1,
        uint32_t    rxfifo_1:1,         /* used*/
        uint32_t    rxfifo_0:1         /* used*/
    ) bit;
} can_fifo_mask_t;


/*
 * Individual FIFO status; adapted from iodefine.h
 */

typedef R_BSP_EVENACCESS union {
    unsigned short WORD;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_8 (
        unsigned short :2,
        unsigned short MC:6,        /* message count (4 bits in HIST FIFO)*/
        unsigned short :3,
        unsigned short TXIF:1,      /* transmit interrupt flag (TXFIFO only)*/
        unsigned short IF:1,        /* interrupt flag*/
        unsigned short MLT:1,       /* message lost (overflow)*/
        unsigned short FLL:1,       /* FIFO full*/
        unsigned short EMP:1        /* FIFO empty*/
    ) BIT;
} can_fifo_stat_t;


/******************************************************************************
 * TX/RX FIFO DEFINITIONS (TX and GW FIFOs)
 ******************************************************************************/
/*
 * CFCCx: TX/RX FIFO configuration and control register (k=0-14; 3*5)
 *   array[15] at &RSCAN0CFCC0
 */
typedef enum e_fifo_mode
{
    CAN_FIFO_MODE_RECEIVE  = 0,
    CAN_FIFO_MODE_TRANSMIT = 1,
    CAN_FIFO_MODE_END_ENUM
} can_fifo_mode_t;


/******************************************************************************
 * TRANSMIT HISTORY BUFFER (FIFO) DEFINITIONS
 ******************************************************************************/

typedef enum e_can_hist_src
{
    HIST_SRC_TX_BUF = 1,
    HIST_SRC_TXRX_FIFO = 2,
    HIST_SRC_END_ENUM
} can_hist_src_t;


/******************************************************************************
 * RECEIVE RULE DEFINITIONS
 ******************************************************************************/
/*
 * Adapted from iodefine.h (removed trailing rule number)
 * arr[16] at (can_rxrule_t *)&RSCAN.GAFLIDL0.WORD;
 */

typedef R_BSP_EVENACCESS struct st_can_rxrule
{
    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_1 (
            unsigned short GAFLID:16
        ) BIT;
    } GAFLIDL;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_4 (
            unsigned short GAFLIDE:1,
            unsigned short GAFLRTR:1,
            unsigned short GAFLLB:1,
            unsigned short GAFLID:13
        ) BIT;
    } GAFLIDH;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_1 (
            unsigned short GAFLIDM:16
        ) BIT;
    } GAFLML;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_4 (
            unsigned short GAFLIDEM:1,
            unsigned short GAFLRTRM:1,
            unsigned short :1,
            unsigned short GAFLIDM:13
        ) BIT;
    } GAFLMH;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_7 (
            unsigned short GAFLRMV:1,
            unsigned short GAFLRMDP:7,
            unsigned short :3,
            unsigned short GAFLFDP4:1,
            unsigned short :2,
            unsigned short GAFLFDP1:1,
            unsigned short GAFLFDP0:1
        ) BIT;
    } GAFLPL;

    union {
        unsigned short WORD;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_2 (
            unsigned short GAFLDLC:4,
            unsigned short GAFLPTR:12
        ) BIT;
    } GAFLPH;
} can_rxrule_t;


#endif /* CAN_PRIVATE_HEADER_FILE*/
