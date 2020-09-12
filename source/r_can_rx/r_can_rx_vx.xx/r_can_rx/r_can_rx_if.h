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
* File Name    : r_can_rx_if.h
* Description  : The RX CAN FIT API.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 02.20.2015 2.01    For 64M, 71M. Originates from RX63N.
*         : 30.10.2015 2.02    FIT update, new R_CAN_Create interface.
*         : 3.3.2016   2.10    65N added. No significant changes.
*         : 1.30.2017  2.11    - const was added to CAN_port_map_t so that there is no need to add const to instantiations.
*                              - Replaced 0 with NULL for unused pointers to port config. registers. (p_CAN0_RX_PIN_MPC etc.)
*                              - Test ran with 65N 2MB.
*                              - Some GSCE coding guidelines implemented. Mulitple lines changed. (Plugin was used.)
*         : 8.14.2017  2.12    - RX65N 2MB MP release.
*         : 27.04.2018 2.13WS  - Added RX66T. Fixed faulty CANn RX port configuration #if statements
*         : 26.10.2018 2.13    - Added support for additional RX/TX ports on RX66T
*         : 08.01.2019 2.15    - Added RX72T
*         : 05.04.2019 3.00    - Added support for GCC and IAR compilers
*         : 30.04.2019 3.10    - Added RX72M
*         : 16.09.2019 3.11    - Added message to warn issue of CAN0, CAN1 and CAN2 interrupt sources are not assigned any
*                                interrupt vector number
*         : 30.12.2019 3.20    - Added support RX66N, RX72N.
***********************************************************************************************************************/
#ifndef CAN_INTERFACE_HEADER_FILE
#define CAN_INTERFACE_HEADER_FILE 

/***********************************************************************************************************************
Includes   <System Includes>, "Project Includes"
***********************************************************************************************************************/
#include <stdint.h>
#include "r_can_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define RCAN_RX_VERSION_MAJOR           (3)
#define RCAN_RX_VERSION_MINOR           (20)
/* The process of getting the version number is done through the macro below. The version number is encoded where the
   top 2 bytes are the major version number and the bottom 2 bytes are the minor version number. For example,
   Version 4.25 would be returned as 0x00040019. */
#define R_CAN_GetVersion()  ((((uint32_t)RCAN_RX_VERSION_MAJOR) << 16) | (uint32_t)RCAN_RX_VERSION_MINOR)



#if (CAN_USE_CAN1 == 1)
    #if ((BSP_MCU_RX66T == 1) || (BSP_MCU_RX72T == 1))
        #error " - CAN1  does not exist on this device! -"
    #endif
#endif

#if (CAN_USE_CAN2 == 1)
    #if ((BSP_MCU_RX65N == 1) || (BSP_MCU_RX66T == 1) || (BSP_MCU_RX72T == 1))
        #error " - CAN2 does not exist on this device! -"
    #endif
#endif

#if ((CAN_USE_CAN0 == 1)&&((VECT(CAN0,TXM0) == 0)||(VECT(CAN0,RXM0) == 0)))
        #error "Interrupt vector number of CAN0 should be defined in file r_bsp_interrupt_config.h."
#endif

#ifdef CAN1
    #if ((CAN_USE_CAN1 == 1)&&((VECT(CAN1,TXM1) == 0)||(VECT(CAN1,RXM1) == 0)))
        #error "Interrupt vector number of CAN1 should be defined in file r_bsp_interrupt_config.h."
    #endif
#endif

#ifdef CAN2
    #if ((CAN_USE_CAN2 == 1)&&((VECT(CAN2,TXM2) == 0)||(VECT(CAN2,RXM2) == 0)))
        #error "Interrupt vector number of CAN2 should be defined in file r_bsp_interrupt_config.h."
    #endif
#endif

/******************************************************************************
Definitions to make the port pin selection logic below work (needs be numericals).
******************************************************************************/
#define    P14        (1)
#define    P15        (2)
#define    P22        (3)
#define    P23        (4)
#define    P32        (5)
#define    P33        (6)
#define    P54        (7)
#define    P55        (8)
#define    P66        (9)
#define    P67        (10)
#define    PA0        (11)
#define    PA1        (12)
#define    PB5        (13)
#define    PB6        (14)
#define    PC5        (15)
#define    PC6        (16)
#define    PD1        (17)
#define    PD2        (18)
#define    PD7        (19)
#define    PE0        (20)
#define    PF2        (21)
#define    PF3        (22)

/* Define registers and information to fill the CAN pin map based on user selection
 * of port/pin numbers in r_can_rx_config.h */

/* CAN0 RX port configuration */
#if ((BSP_MCU_RX64M == 1) || (BSP_MCU_RX65N == 1) || (BSP_MCU_RX71M == 1) || (BSP_MCU_RX72M == 1)|| (BSP_MCU_RX72N == 1) || (BSP_MCU_RX66N == 1))
    #if (CAN0_RX_PORT == P33)
        #define p_CAN0_RX_PIN_MPC       (&MPC.P33PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORT3.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORT3.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORT3.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x08) /* bit 3 */
    #elif (CAN0_RX_PORT == PD2)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PD2PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTD.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTD.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTD.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x04) /* bit 2 */
    #else
        #error " - Assigned CAN0 RX port not supported by device! -"
    #endif
#elif ((BSP_MCU_RX66T == 1) || (BSP_MCU_RX72T == 1))
    #if (CAN0_RX_PORT == P22)
        #define p_CAN0_RX_PIN_MPC       (&MPC.P22PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORT2.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORT2.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORT2.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x04) /* bit 2 */
    #elif(CAN0_RX_PORT == PA1)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PA1PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTA.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTA.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTA.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x02) /* bit 1 */
    #elif (CAN0_RX_PORT == PA7)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PA7PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTA.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTA.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTA.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x80) /* bit 7 */
    #elif (CAN0_RX_PORT == PB6)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PB6PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTB.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTB.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTB.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x40) /* bit 6 */
    #elif (CAN0_RX_PORT == PC6)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PC6PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTC.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTC.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTC.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x40) /* bit 6 */
    #elif (CAN0_RX_PORT == PE0)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PE0PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTE.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTE.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTE.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x01) /* bit 0 */
    #elif (CAN0_RX_PORT == PF3)
        #define p_CAN0_RX_PIN_MPC       (&MPC.PF3PFS.BYTE)
        #define p_CAN0_RX_PIN_PMR       (&PORTF.PMR.BYTE)
        #define p_CAN0_RX_PIN_PDR       (&PORTF.PDR.BYTE)
        #define p_CAN0_RX_PIN_PIDR      (&PORTF.PIDR.BYTE)
        #define   CAN0_RX_PIN_MASK      (0x08) /* bit 3 */
    #else
        #error " - Assigned CAN0 RX port not supported by device! -"
    #endif
#endif

/* CAN0 TX port configuration */
#if ((BSP_MCU_RX64M == 1) || (BSP_MCU_RX65N == 1) || (BSP_MCU_RX71M == 1) || (BSP_MCU_RX72M == 1) || (BSP_MCU_RX72N == 1)|| (BSP_MCU_RX66N == 1))
    #if (CAN0_TX_PORT == P32)
        #define p_CAN0_TX_PIN_MPC       (&MPC.P32PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORT3.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORT3.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORT3.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x04) /* bit 2 */
    #elif (CAN0_TX_PORT == PD1)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PD1PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTD.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTD.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTD.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x02) /* bit 1 */
    #else
        #error " - Assigned CAN0 TX port not supported by device! -"
    #endif
#elif ((BSP_MCU_RX66T == 1) || (BSP_MCU_RX72T == 1))
    #if (CAN0_TX_PORT == P23)
        #define p_CAN0_TX_PIN_MPC       (&MPC.P23PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORT2.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORT2.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORT2.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x08) /* bit 3 */
    #elif (CAN0_TX_PORT == PA0)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PA0PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTA.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTA.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTA.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x01) /* bit 0 */
    #elif (CAN0_TX_PORT == PA6)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PA6PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTA.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTA.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTA.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x40) /* bit 6 */
    #elif (CAN0_TX_PORT == PB5)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PB5PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTB.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTB.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTB.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x20) /* bit 5 */
    #elif (CAN0_TX_PORT == PC5)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PC5PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTC.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTC.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTC.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x20) /* bit 5 */
    #elif (CAN0_TX_PORT == PD7)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PD7PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTD.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTD.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTD.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x80) /* bit 7 */
    #elif (CAN0_TX_PORT == PF2)
        #define p_CAN0_TX_PIN_MPC       (&MPC.PF2PFS.BYTE)
        #define p_CAN0_TX_PIN_PMR       (&PORTF.PMR.BYTE)
        #define p_CAN0_TX_PIN_PDR       (&PORTF.PDR.BYTE)
        #define p_CAN0_TX_PIN_PODR      (&PORTF.PODR.BYTE)
        #define   CAN0_TX_PIN_MASK      (0x04) /* bit 2 */
    #else
        #error " - Assigned CAN0 TX port not supported by device! -"
    #endif
#endif


/* CAN1 RX port configuration */
#if ((BSP_MCU_RX64M == 1) || (BSP_MCU_RX65N == 1) || (BSP_MCU_RX71M == 1) || (BSP_MCU_RX72M == 1) || (BSP_MCU_RX72N == 1)|| (BSP_MCU_RX66N == 1))
    #if (CAN1_RX_PORT == P15)
        #define p_CAN1_RX_PIN_MPC       (&MPC.P15PFS.BYTE)
        #define p_CAN1_RX_PIN_PMR       (&PORT1.PMR.BYTE)
        #define p_CAN1_RX_PIN_PDR       (&PORT1.PDR.BYTE)
        #define p_CAN1_RX_PIN_PIDR      (&PORT1.PIDR.BYTE)
        #define CAN1_RX_PIN_MASK        (0x20) /* bit 5 */
    #elif (CAN1_RX_PORT == P55)
        #define p_CAN1_RX_PIN_MPC       (&MPC.P55PFS.BYTE)
        #define p_CAN1_RX_PIN_PMR       (&PORT5.PMR.BYTE)
        #define p_CAN1_RX_PIN_PDR       (&PORT5.PDR.BYTE)
        #define p_CAN1_RX_PIN_PIDR      (&PORT5.PIDR.BYTE)
        #define CAN1_RX_PIN_MASK        (0x20) /* bit 5 */
    #else
        #error " - Assigned CAN1 RX port not supported by device! -"
    #endif
#endif

/* CAN1 TX port configuration */
#if ((BSP_MCU_RX64M == 1) || (BSP_MCU_RX65N == 1) || (BSP_MCU_RX71M == 1) || (BSP_MCU_RX72M == 1) || (BSP_MCU_RX72N == 1)|| (BSP_MCU_RX66N == 1))
    #if (CAN1_TX_PORT == P14)
        #define p_CAN1_TX_PIN_MPC       (&MPC.P14PFS.BYTE)
        #define p_CAN1_TX_PIN_PMR       (&PORT1.PMR.BYTE)
        #define p_CAN1_TX_PIN_PDR       (&PORT1.PDR.BYTE)
        #define p_CAN1_TX_PIN_PODR      (&PORT1.PODR.BYTE)
        #define   CAN1_TX_PIN_MASK      (0x10) /* bit 4 */
    #elif (CAN1_TX_PORT == P54)
        #define p_CAN1_TX_PIN_MPC       (&MPC.P54PFS.BYTE)
        #define p_CAN1_TX_PIN_PMR       (&PORT5.PMR.BYTE)
        #define p_CAN1_TX_PIN_PDR       (&PORT5.PDR.BYTE)
        #define p_CAN1_TX_PIN_PODR      (&PORT5.PODR.BYTE)
        #define   CAN1_TX_PIN_MASK      (0x10) /* bit 4 */
    #else
        #error " - Assigned CAN1 TX port not supported by device! -"
    #endif
#endif


/* CAN2 RX port configuration */
#if (CAN2_RX_PORT == P67)
    #define p_CAN2_RX_PIN_MPC       (&MPC.P67PFS.BYTE)
    #define p_CAN2_RX_PIN_PMR       (&PORT6.PMR.BYTE)
    #define p_CAN2_RX_PIN_PDR       (&PORT6.PDR.BYTE)
    #define p_CAN2_RX_PIN_PIDR      (&PORT6.PIDR.BYTE)
    #define   CAN2_RX_PIN_MASK      (0x80) /* bit 7 */
#else
    #error " - Assigned CAN2 RX port not supported by device! -"
#endif
/* CAN2 TX port configuration */        
#if (CAN2_TX_PORT == P66)
    #define p_CAN2_TX_PIN_MPC       (&MPC.P66PFS.BYTE)
    #define p_CAN2_TX_PIN_PMR       (&PORT6.PMR.BYTE)
    #define p_CAN2_TX_PIN_PDR       (&PORT6.PDR.BYTE)
    #define p_CAN2_TX_PIN_PODR      (&PORT6.PODR.BYTE)
    #define   CAN2_TX_PIN_MASK      (0x40) /* bit 6 */
#else
    #error " - Assigned CAN2 TX port not supported by device! -"
#endif



#define PINFUNC_CAN                 (0x10)    /* Value to set MPC PFS registers for CAN operation. */

/******************************************************************************
Macro definitions
******************************************************************************/
/*** CAN API ACTION TYPES ***/
#define DISABLE                         (0)
#define ENABLE                          (1)

/* Periph CAN modes */
#define EXITSLEEP_CANMODE               (2)
#define ENTERSLEEP_CANMODE              (3)
#define RESET_CANMODE                   (4)
#define HALT_CANMODE                    (5)
#define OPERATE_CANMODE                 (6)

/* Port mode actions */
#define CANPORT_TEST_LISTEN_ONLY        (7)
#define CANPORT_TEST_0_EXT_LOOPBACK     (8)
#define CANPORT_TEST_1_INT_LOOPBACK     (9)
#define CANPORT_RETURN_TO_NORMAL        (10)

/* Local sleep mode for CAN module */
#define CAN_NOT_SLEEP   (0)
#define CAN_SLEEP       (1)

/* Tranceiver port pin macros.  */
#define CAN_TRX_DDR(x, y)               (CAN_TRX_DDR_PREPROC(x, y))
#define CAN_TRX_DDR_PREPROC(x, y)       ((PORT ## x .PDR.BIT.B ## y))
#define CAN_TRX_DR(x, y)                (CAN_TRX_DR_PREPROC(x, y))
#define CAN_TRX_DR_PREPROC(x, y)        ((PORT ## x .PODR.BIT.B ## y))

/*** CAN API return values *****************************/
#define     R_CAN_OK                ((uint32_t)0x00000000)
#define     R_CAN_NOT_OK            ((uint32_t)0x00000001)
#define     R_CAN_MODULE_STOP_ERR   ((uint32_t)0x00000002)
#define     R_CAN_MSGLOST           ((uint32_t)0x00000004)
#define     R_CAN_NO_SENTDATA       ((uint32_t)0x00000008)
#define     R_CAN_RXPOLL_TMO        ((uint32_t)0x00000010)    /* 16 */
#define     R_CAN_BAD_CH_NR         ((uint32_t)0x00000020)    /* 32 */
#define     R_CAN_SW_BAD_MBX        ((uint32_t)0x00000040)    /* 64 */
#define     R_CAN_BAD_ACTION_TYPE   ((uint32_t)0x00000080)    /* 128 */

/* CAN peripheral timeout reasons. */
#define     R_CAN_SW_WAKEUP_ERR     ((uint32_t)0x00000100)    /* 256 */
#define     R_CAN_SW_SLEEP_ERR      ((uint32_t)0x00000200)    /* 512 */
#define     R_CAN_SW_HALT_ERR       ((uint32_t)0x00000400)    /* 1024 */
#define     R_CAN_SW_RST_ERR        ((uint32_t)0x00000800)    /* 2048 */
#define     R_CAN_SW_TSRC_ERR       ((uint32_t)0x00001000)    /* 4096 */
#define     R_CAN_SW_SET_TX_TMO     ((uint32_t)0x00002000)    /* 8192 */
#define     R_CAN_SW_SET_RX_TMO     ((uint32_t)0x00004000)    /* 16384 */
#define     R_CAN_SW_ABORT_ERR      ((uint32_t)0x00008000)    /* 32768 */

/* CAN STATE CODES */
#define     R_CAN_STATUS_INIT           ((uint32_t)0x0000000)
#define     R_CAN_STATUS_ERROR_ACTIVE   ((uint32_t)0x0000001)
#define     R_CAN_STATUS_ERROR_PASSIVE  ((uint32_t)0x0000002)
#define     R_CAN_STATUS_BUSOFF         ((uint32_t)0x0000004)
/*** API end *********************************************/

/* CAN Frame ID modes */
#define     STD_ID_MODE     (0)
#define     EXT_ID_MODE     (1)
#define     MIXED_ID_MODE   (2)

/* Mailbox search modes. */
#define     RX_SEARCH       (0)
#define     TX_SEARCH       (1)
#define     MSGLOST_SEARCH  (2)
#define     CHANNEL_SEARCH  (3)

/* CAN1 Control Register (CTLR) b9, b8 CANM[1:0] CAN Operating Mode Select. */
#define CAN_OPERATION       (0)    /* CAN operation mode */
#define CAN_RESET           (1)    /* CAN reset mode */
#define CAN_HALT            (2)    /* CAN halt mode */
#define CAN_RESET_FORCE     (3)    /* CAN reset mode (forcible transition) */

/* Frame types */
#define DATA_FRAME          (0)
#define REMOTE_FRAME        (1)

/* Bit set defines */
#define        MBX_0        (0x00000001)
#define        MBX_1        (0x00000002)
#define        MBX_2        (0x00000004)
#define        MBX_3        (0x00000008)
#define        MBX_4        (0x00000010)
#define        MBX_5        (0x00000020)
#define        MBX_6        (0x00000040)
#define        MBX_7        (0x00000080)
#define        MBX_8        (0x00000100)
#define        MBX_9        (0x00000200)
#define        MBX_10       (0x00000400)
#define        MBX_11       (0x00000800)
#define        MBX_12       (0x00001000)
#define        MBX_13       (0x00002000)
#define        MBX_14       (0x00004000)
#define        MBX_15       (0x00008000)
#define        MBX_16       (0x00010000)
#define        MBX_17       (0x00020000)
#define        MBX_18       (0x00040000)
#define        MBX_19       (0x00080000)
#define        MBX_20       (0x00100000)
#define        MBX_21       (0x00200000)
#define        MBX_22       (0x00400000)
#define        MBX_23       (0x00800000)
#define        MBX_24       (0x01000000)
#define        MBX_25       (0x02000000)
#define        MBX_26       (0x04000000)
#define        MBX_27       (0x08000000)
#define        MBX_28       (0x10000000)
#define        MBX_29       (0x20000000)
#define        MBX_30       (0x40000000)
#define        MBX_31       (0x80000000)

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Standard data frame message definition object. */
typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} can_frame_t;

/* A pointer to the CAN peripheral registers memory map structure. */
typedef volatile struct st_can R_BSP_EVENACCESS_SFR * can_st_ptr;

/* A set of pointers to the registers of the I/O ports assigned to CAN pins. */
typedef const struct
{
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Rx_Pin_MPC;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Rx_Pin_PMR;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Rx_Pin_PDR;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Rx_Pin_PIDR;
    uint8_t Rx_Pin_mask;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Tx_Pin_MPC;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Tx_Pin_PMR;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Tx_Pin_PDR;
    volatile R_BSP_EVENACCESS_SFR unsigned char * p_CAN_Tx_Pin_PODR;
    uint8_t Tx_Pin_mask;
} CAN_port_map_t;

/**************************************************************************************************
Exported global variables
***************************************************************************************************/
/**************************************************************************************************
Exported global functions (to be accessed by other files)
***************************************************************************************************/
/*****************************************************************
                R X   C A N   A P I 
******************************************************************/
/* INITIALIZATION */
uint32_t    R_CAN_Create(const uint32_t  ch_nr, void (*tx_cb_func)(void), void (*cb_rx_func)(void), void (*cb_err_func)(void));
uint32_t    R_CAN_PortSet(const uint32_t ch_nr, const uint32_t action_type);
uint32_t    R_CAN_Control(const uint32_t ch_nr, const uint32_t action_type);
void        R_CAN_SetBitrate(const uint32_t ch_nr);

/* TRANSMIT */
uint32_t    R_CAN_TxSet(const uint32_t ch_nr, const uint32_t mbox_nr, const can_frame_t* frame_p, const uint32_t frame_type);
uint32_t    R_CAN_TxSetXid(const uint32_t ch_nr, const uint32_t mbox_nr, can_frame_t* frame_p, const uint32_t frame_type);
uint32_t    R_CAN_Tx(const uint32_t ch_nr, const uint32_t mbox_nr);
uint32_t    R_CAN_TxCheck(const uint32_t ch_nr, const uint32_t mbox_nr);
uint32_t    R_CAN_TxStopMsg(const uint32_t ch_nr, const uint32_t mbox_nr);

/* RECEIVE */
uint32_t    R_CAN_RxSet(const uint32_t ch_nr, const uint32_t mbox_nr, const uint32_t sid, const uint32_t frame_type);
uint32_t    R_CAN_RxSetXid(const uint32_t ch_nr, const uint32_t mbox_nr, uint32_t xid, const uint32_t frame_type);
uint32_t    R_CAN_RxPoll(const uint32_t ch_nr, const uint32_t mbox_nr);
uint32_t    R_CAN_RxRead(const uint32_t ch_nr, const uint32_t mbox_nr, can_frame_t * const frame_p);
void        R_CAN_RxSetMask(const uint32_t ch_nr, const uint32_t mbox_nr, const uint32_t mask_value);

/* ERRORS */
uint32_t    R_CAN_CheckErr(const uint32_t ch_nr);

#endif    /* R_CAN_API.H */
/* file end */
