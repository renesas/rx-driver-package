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
/**********************************************************************************************************************
* File Name    : r_rsci_rx660_data.c
* Description  : Functions for using RSCI on the RX660 device.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2022 1.00    Initial Release
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#include "r_rsci_rx660_private.h"

/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Macro definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/

/* BAUD DIVISOR INFO */

/* Asynchronous or Manchester */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* when abcs=0 & bgdm=0 & abcse =0, divisor = 64*pow(2,2n-1) */
/* when abcs=1 & bgdm=0 & abcse =0 OR abcs=0 & bgdm=1 & abcse =0, divisor = 32*pow(2,2n-1) */
/* when abcs=1 & bgdm=1 & abcse =0, divisor = 16*pow(2,2n-1) */
/* when abcs=(1 or 0) & bgdm= (1 or 0) & abcse =1, divisor = 12*pow(2,2n-1). */
/* In manchester mode, only ABCSE bit = 0 can be selected. */

#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
/* NOTE: diff than RSCI async baud table, but should provide same results */
const baud_divisor_t rsci_async_baud[NUM_DIVISORS_ASYNC]=
{
    /* divisor result, abcs, bgdm, abcse, n */
    {6,    1, 1, 1, 0},
    {8,    1, 1, 0, 0},
    {16,   0, 1, 0, 0},
    {32,   0, 0, 0, 0},
    {64,   0, 1, 0, 1},
    {128,  0, 0, 0, 1},
    {256,  0, 1, 0, 2},
    {512,  0, 0, 0, 2},
    {1024, 0, 1, 0, 3},
    {2048, 0, 0, 0, 3}
};
#endif

/* Synchronous and Simple SPI */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* abcs=0, bdgm=0, divisor = 8*pow(2,2n-1) */

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/* NOTE: Identical to RSCI sync baud table */
const baud_divisor_t rsci_sync_baud[NUM_DIVISORS_SYNC]=
{
    /* divisor result, abcs, bgdm, abcse, n */
    {4,   0, 0, 0, 0},
    {16,  0, 0, 0, 1},
    {64,  0, 0, 0, 2},
    {256, 0, 0, 0, 3}
};
#endif


/* CHANNEL MEMORY ALLOCATIONS */

#if RSCI_CFG_CH10_INCLUDED

/* rom info */
const rsci_ch_rom_t  g_rsci_ch10_rom = {(volatile struct st_rsci R_BSP_EVENACCESS_SFR *)&RSCI10,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRD.LONG, BIT3_MASK,
                                #if RSCI_CFG_TEI_INCLUDED
                                BSP_INT_SRC_AL0_RSCI10_TEI, rsci10_tei_isr,
                                #endif
                                BSP_INT_SRC_AL0_RSCI10_ERI, rsci10_eri_isr,
                                BIT24_MASK, BIT25_MASK,
                                &ICU.IPR[IPR_RSCI10_RXI].BYTE,
                                &ICU.IPR[IPR_RSCI10_TXI].BYTE,
                                &ICU.IR[IR_RSCI10_RXI].BYTE,
                                &ICU.IR[IR_RSCI10_TXI].BYTE,
                                &ICU.IER[IER_RSCI10_RXI].BYTE,
                                &ICU.IER[IER_RSCI10_TXI].BYTE,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&ICU.GENAL0.LONG,
                                BIT0_MASK, BIT1_MASK,
                                /* Casting to uint8_t type is valid */
                                (uint8_t)RSCI_CH10
                                };

/* channel control block */
rsci_ch_ctrl_t   g_rsci_ch10_ctrl = {&g_rsci_ch10_rom, RSCI_MODE_OFF, 0, NULL, NULL, NULL, true
                             #if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
                             , true, 0, 0, false
                             #endif
                             , BSP_PCLKA_HZ
                             #if RSCI_CFG_FIFO_INCLUDED
                             , RSCI_CFG_CH10_FIFO_INCLUDED
                             , RSCI_CFG_CH10_RX_FIFO_THRESH
                             , RSCI_CFG_CH10_RX_FIFO_THRESH
                             , RSCI_CFG_CH10_TX_FIFO_THRESH
                             , RSCI_CFG_CH10_TX_FIFO_THRESH
                             #endif
                             #if RSCI_CFG_MANC_INCLUDED
                             , RSCI_CFG_CH10_RX_SEL_DECODING_POL
                             , RSCI_CFG_CH10_RX_PREFACE_LENGTH
                             , RSCI_CFG_CH10_RX_PREFACE_PATTERN
                             , RSCI_CFG_CH10_TX_SEL_ENCODING_POL
                             , RSCI_CFG_CH10_TX_PREFACE_LENGTH
                             , RSCI_CFG_CH10_TX_PREFACE_PATTERN
                             #endif
                            };
#endif /* End of RSCI_CFG_CH10_INCLUDED */


#if RSCI_CFG_CH11_INCLUDED

/* rom info */
const rsci_ch_rom_t  g_rsci_ch11_rom = {(volatile struct st_rsci R_BSP_EVENACCESS_SFR *)&RSCI11,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRD.LONG, BIT2_MASK,
                                #if RSCI_CFG_TEI_INCLUDED
                                BSP_INT_SRC_AL0_RSCI11_TEI, rsci11_tei_isr,
                                #endif
                                BSP_INT_SRC_AL0_RSCI11_ERI, rsci11_eri_isr,
                                BIT27_MASK, BIT28_MASK,
                                &ICU.IPR[IPR_RSCI11_RXI].BYTE,
                                &ICU.IPR[IPR_RSCI11_TXI].BYTE,
                                &ICU.IR[IR_RSCI11_RXI].BYTE,
                                &ICU.IR[IR_RSCI11_TXI].BYTE,
                                &ICU.IER[IER_RSCI11_RXI].BYTE,
                                &ICU.IER[IER_RSCI11_TXI].BYTE,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&ICU.GENAL0.LONG,
                                BIT2_MASK, BIT3_MASK,
                                /* Casting to uint8_t type is valid */
                                (uint8_t)RSCI_CH11
                                };

/* channel control block */
rsci_ch_ctrl_t   g_rsci_ch11_ctrl = {&g_rsci_ch11_rom, RSCI_MODE_OFF, 0, NULL, NULL, NULL, true
                             #if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
                             , true, 0, 0, false
                             #endif
                             , BSP_PCLKA_HZ
                             #if RSCI_CFG_FIFO_INCLUDED
                             , RSCI_CFG_CH11_FIFO_INCLUDED
                             , RSCI_CFG_CH11_RX_FIFO_THRESH
                             , RSCI_CFG_CH11_RX_FIFO_THRESH
                             , RSCI_CFG_CH11_TX_FIFO_THRESH
                             , RSCI_CFG_CH11_TX_FIFO_THRESH
                             #endif
                             #if RSCI_CFG_MANC_INCLUDED
                             , RSCI_CFG_CH11_RX_SEL_DECODING_POL
                             , RSCI_CFG_CH11_RX_PREFACE_LENGTH
                             , RSCI_CFG_CH11_RX_PREFACE_PATTERN
                             , RSCI_CFG_CH11_TX_SEL_ENCODING_POL
                             , RSCI_CFG_CH11_TX_PREFACE_LENGTH
                             , RSCI_CFG_CH11_TX_PREFACE_PATTERN
                             #endif
                            };
#endif /* End of RSCI_CFG_CH11_INCLUDED */



/* RSCI HANDLE-ARRAY DECLARATION */

const rsci_hdl_t g_rsci_handles[RSCI_NUM_CH] =
{
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#if RSCI_CFG_CH10_INCLUDED
            &g_rsci_ch10_ctrl,
#else
            NULL,
#endif
#if RSCI_CFG_CH11_INCLUDED
            &g_rsci_ch11_ctrl,
#else
            NULL,
#endif
};

