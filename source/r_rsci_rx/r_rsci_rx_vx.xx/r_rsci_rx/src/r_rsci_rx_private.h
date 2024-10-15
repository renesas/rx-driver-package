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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsci_rx_private.h
* Description  : Functions for using RSCI on the RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2021 1.00    Initial Release
*                              Supported for RX671.
*           03.12.2021 2.00    Updated new features in Asynchronous mode
*                              and added support for Manchester mode.
*           15.08.2022 2.30    Supported for RX26T.
*                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
*           28.06.2024 2.60    Supported for RX260, RX261.
***********************************************************************************************************************/

#ifndef RSCI_RX_H
#define RSCI_RX_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "../r_rsci_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Bit number */
#define RSCI_BIT0    (0)
#define RSCI_BIT1    (1)
#define RSCI_BIT2    (2)
#define RSCI_BIT3    (3)
#define RSCI_BIT4    (4)
#define RSCI_BIT5    (5)
#define RSCI_BIT6    (6)
#define RSCI_BIT7    (7)
#define RSCI_BIT8    (8)
#define RSCI_BIT9    (9)
#define RSCI_BIT10   (10)
#define RSCI_BIT11   (11)
#define RSCI_BIT12   (12)
#define RSCI_BIT13   (13)
#define RSCI_BIT14   (14)
#define RSCI_BIT15   (15)
#define RSCI_BIT16   (16)
#define RSCI_BIT17   (17)
#define RSCI_BIT18   (18)
#define RSCI_BIT19   (19)
#define RSCI_BIT20   (20)
#define RSCI_BIT21   (21)
#define RSCI_BIT22   (22)
#define RSCI_BIT23   (23)
#define RSCI_BIT24   (24)
#define RSCI_BIT25   (25)
#define RSCI_BIT26   (26)
#define RSCI_BIT27   (27)
#define RSCI_BIT28   (28)
#define RSCI_BIT29   (29)
#define RSCI_BIT30   (30)
#define RSCI_BIT31   (31)


/* Bit position masks */
#define BIT0_MASK   (0x00000001U)
#define BIT1_MASK   (0x00000002U)
#define BIT2_MASK   (0x00000004U)
#define BIT3_MASK   (0x00000008U)
#define BIT4_MASK   (0x00000010U)
#define BIT5_MASK   (0x00000020U)
#define BIT6_MASK   (0x00000040U)
#define BIT7_MASK   (0x00000080U)
#define BIT8_MASK   (0x00000100U)
#define BIT9_MASK   (0x00000200U)
#define BIT10_MASK  (0x00000400U)
#define BIT11_MASK  (0x00000800U)
#define BIT12_MASK  (0x00001000U)
#define BIT13_MASK  (0x00002000U)
#define BIT14_MASK  (0x00004000U)
#define BIT15_MASK  (0x00008000U)
#define BIT16_MASK  (0x00010000U)
#define BIT17_MASK  (0x00020000U)
#define BIT18_MASK  (0x00040000U)
#define BIT19_MASK  (0x00080000U)
#define BIT20_MASK  (0x00100000U)
#define BIT21_MASK  (0x00200000U)
#define BIT22_MASK  (0x00400000U)
#define BIT23_MASK  (0x00800000U)
#define BIT24_MASK  (0x01000000U)
#define BIT25_MASK  (0x02000000U)
#define BIT26_MASK  (0x04000000U)
#define BIT27_MASK  (0x08000000U)
#define BIT28_MASK  (0x10000000U)
#define BIT29_MASK  (0x20000000U)
#define BIT30_MASK  (0x40000000U)
#define BIT31_MASK  (0x80000000U)

#ifndef NULL    /* Resolves e2studio code analyzer false error message. */
    #define NULL (0)
#endif

#if ((RSCI_CFG_CH10_FIFO_INCLUDED) || (RSCI_CFG_CH11_FIFO_INCLUDED))
    #define RSCI_CFG_FIFO_INCLUDED (1)
#endif

#if ((RSCI_CFG_CH0_DATA_MATCH_INCLUDED) || (RSCI_CFG_CH8_DATA_MATCH_INCLUDED) || \
        (RSCI_CFG_CH9_DATA_MATCH_INCLUDED) || (RSCI_CFG_CH10_DATA_MATCH_INCLUDED) || \
        (RSCI_CFG_CH11_DATA_MATCH_INCLUDED))
    #define RSCI_CFG_DATA_MATCH_INCLUDED (1)
#endif

#if ((RSCI_CFG_CH0_RX_DATA_SAMPLING_TIMING_INCLUDED)     ||   \
        (RSCI_CFG_CH8_RX_DATA_SAMPLING_TIMING_INCLUDED)  ||   \
        (RSCI_CFG_CH9_RX_DATA_SAMPLING_TIMING_INCLUDED)  ||   \
        (RSCI_CFG_CH10_RX_DATA_SAMPLING_TIMING_INCLUDED) ||   \
        (RSCI_CFG_CH11_RX_DATA_SAMPLING_TIMING_INCLUDED))
    #define RSCI_CFG_RX_DATA_SAMPLING_TIMING_INCLUDED (1)
#endif

#if ((RSCI_CFG_CH0_TX_SIGNAL_TRANSITION_TIMING_INCLUDED)     ||   \
        (RSCI_CFG_CH8_TX_SIGNAL_TRANSITION_TIMING_INCLUDED)  ||   \
        (RSCI_CFG_CH9_TX_SIGNAL_TRANSITION_TIMING_INCLUDED)  ||   \
        (RSCI_CFG_CH10_TX_SIGNAL_TRANSITION_TIMING_INCLUDED) ||   \
        (RSCI_CFG_CH11_TX_SIGNAL_TRANSITION_TIMING_INCLUDED))
    #define RSCI_CFG_TX_SIGNAL_TRANSITION_TIMING_INCLUDED (1)
#endif

#define RSCI_SSR_ORER      (hdl->rom->regs->SSR.BIT.ORER)
#define RSCI_SSR_APER      (hdl->rom->regs->SSR.BIT.APER)
#define RSCI_SSR_AFER      (hdl->rom->regs->SSR.BIT.AFER)
#define RSCI_SSR           (hdl->rom->regs->SSR.LONG)

#define RSCI_SSCR_ORERC      (hdl->rom->regs->SSCR.BIT.ORERC)
#define RSCI_SSCR_APERC      (hdl->rom->regs->SSCR.BIT.APERC)
#define RSCI_SSCR_AFERC      (hdl->rom->regs->SSCR.BIT.AFERC)
#define RSCI_SSCR            (hdl->rom->regs->SSCR.LONG)

#define RSCI_MMSR_PFER      (hdl->rom->regs->MMSR.BIT.PFER)
#define RSCI_MMSR_SYER      (hdl->rom->regs->MMSR.BIT.SYER)
#define RSCI_MMSR_SBER      (hdl->rom->regs->MMSR.BIT.SBER)
#define RSCI_MMSR_MCER      (hdl->rom->regs->MMSR.BIT.MCER)
#define RSCI_MMSR           (hdl->rom->regs->MMSR.LONG)

#define RSCI_MMSCR_PFER      (hdl->rom->regs->MMSCR.BIT.PFERC)
#define RSCI_MMSCR_SYER      (hdl->rom->regs->MMSCR.BIT.SYERC)
#define RSCI_MMSCR_SBER      (hdl->rom->regs->MMSCR.BIT.SBERC)
#define RSCI_MMSCR_MCER      (hdl->rom->regs->MMSCR.BIT.MCERC)
#define RSCI_MMSCR           (hdl->rom->regs->MMSCR.LONG)

#if RSCI_CFG_FIFO_INCLUDED
#define RSCI_FIFO_FRAME_SIZE (32)
#endif

/* SCR0 register dummy read */
#define RSCI_SCR0_DUMMY_READ                \
    if (0x00000000 == hdl->rom->regs->SCR0.LONG) \
    {                                     \
        R_BSP_NOP();                      \
    }

/* Interrupt Request register flag clear */
#define RSCI_IR_TXI_CLEAR (*hdl->rom->ir_txi = 0)

/* TDR register write access */
#define RSCI_TDR(byte)    (hdl->rom->regs->TDR.BIT.TDAT = (byte))

/* RDR register read access */
#define RSCI_RDR(byte)    ((byte) = hdl->rom->regs->RDR.BIT.RDAT)

/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
extern void rsci_txi_handler (rsci_hdl_t const hdl);
#endif

#if RSCI_CFG_TEI_INCLUDED
extern void rsci_tei_handler (rsci_hdl_t const hdl);
#endif

extern void rsci_rxi_handler (rsci_hdl_t const hdl);

extern void rsci_eri_handler (rsci_hdl_t const hdl);

#endif /* RSCI_RX_H */
