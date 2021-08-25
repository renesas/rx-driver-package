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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_sci_rx111_data.c
* Description  : Functions for using SCI on the RX111 device.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           03.06.2016 1.00    Initial Release.
*           20.05.2019 3.00    Added support for GNUC and ICCRX.
*           25.08.2020 3.60    Added feature using DTC/DMAC in SCI transfer.
*           31.03.2021 3.80    Updated macro definition enable and disable TXI, RXI, ERI, TEI.
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#include "r_sci_rx111_private.h"

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

#if (SCI_CFG_ASYNC_INCLUDED)
/*
 * Asynchronous
 * BRR = (PCLK/(divisor * baud)) - 1
 * when abcs=1, divisor = 32*pow(2,2n-1)
 * when abcs=0, divisor = 64*pow(2,2n-1)
 */
const baud_divisor_t async_baud[NUM_DIVISORS_ASYNC]=
{
    /* divisor result, abcs, n */
    {16,   1, 0},
    {32,   0, 0},
    {64,   1, 1},
    {128,  0, 1},
    {256,  1, 2},
    {512,  0, 2},
    {1024, 1, 3},
    {2048, 0, 3}
};
#endif

#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
/* Synchronous and Simple SPI */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* abcs=0, divisor = 8*pow(2,2n-1) */

const baud_divisor_t sync_baud[NUM_DIVISORS_SYNC]=
{
    /* divisor result, abcs, n */
    {4,   0, 0},
    {16,  0, 1},
    {64,  0, 2},
    {256, 0, 3}
};
#endif


/* CHANNEL MEMORY ALLOCATIONS */

#if SCI_CFG_CH1_INCLUDED

/* rom info */
const sci_ch_rom_t  ch1_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI1,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT30_MASK,
                                &ICU.IPR[IPR_SCI1_RXI1].BYTE,
                                &ICU.IR[IR_SCI1_RXI1].BYTE,
                                &ICU.IR[IR_SCI1_TXI1].BYTE,
                                &ICU.IR[IR_SCI1_TEI1].BYTE,
                                &ICU.IR[IR_SCI1_ERI1].BYTE,
                                &ICU.IER[IER_SCI1_RXI1].BYTE,
                                &ICU.IER[IER_SCI1_TXI1].BYTE,
                                &ICU.IER[IER_SCI1_TEI1].BYTE,
                                &ICU.IER[IER_SCI1_ERI1].BYTE,
                                SCI_BIT2, SCI_BIT3, SCI_BIT4, SCI_BIT5
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH1_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH1_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI1_TXI1
                                , DTCE_SCI1_RXI1
                                #endif
                                };
/* channel control block */
sci_ch_ctrl_t   ch1_ctrl = {&ch1_rom, SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif


#if SCI_CFG_CH5_INCLUDED

/* rom info */
const sci_ch_rom_t  ch5_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI5,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT26_MASK,
                                &ICU.IPR[IPR_SCI5_RXI5].BYTE,
                                &ICU.IR[IR_SCI5_RXI5].BYTE,
                                &ICU.IR[IR_SCI5_TXI5].BYTE,
                                &ICU.IR[IR_SCI5_TEI5].BYTE,
                                &ICU.IR[IR_SCI5_ERI5].BYTE,
                                &ICU.IER[IER_SCI5_RXI5].BYTE,
                                &ICU.IER[IER_SCI5_TXI5].BYTE,
                                &ICU.IER[IER_SCI5_TEI5].BYTE,
                                &ICU.IER[IER_SCI5_ERI5].BYTE,
                                SCI_BIT6, SCI_BIT7, SCI_BIT0, SCI_BIT1
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH5_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH5_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI5_TXI5
                                , DTCE_SCI5_RXI5
                                #endif
                                };
/* channel control block */
sci_ch_ctrl_t   ch5_ctrl = {&ch5_rom, SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif


#if SCI_CFG_CH12_INCLUDED

/* rom info */
const sci_ch_rom_t  ch12_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI12,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT4_MASK,
                                &ICU.IPR[IPR_SCI12_RXI12].BYTE,
                                &ICU.IR[IR_SCI12_RXI12].BYTE,
                                &ICU.IR[IR_SCI12_TXI12].BYTE,
                                &ICU.IR[IR_SCI12_TEI12].BYTE,
                                &ICU.IR[IR_SCI12_ERI12].BYTE,
                                &ICU.IER[IER_SCI12_RXI12].BYTE,
                                &ICU.IER[IER_SCI12_TXI12].BYTE,
                                &ICU.IER[IER_SCI12_TEI12].BYTE,
                                &ICU.IER[IER_SCI12_ERI12].BYTE,
                                SCI_BIT6, SCI_BIT7, SCI_BIT0, SCI_BIT1
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH12_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH12_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI12_TXI12
                                , DTCE_SCI12_RXI12
                                #endif
                                };
/* channel control block */
sci_ch_ctrl_t   ch12_ctrl = {&ch12_rom, SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                             #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                             , true, 0, 0, false
                             #endif
                             , BSP_PCLKB_HZ
                             #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                             , 0, 0, 0, 0, 0
                             #endif
                            };
#endif


/* SCI HANDLE-ARRAY DECLARATION */

const sci_hdl_t g_handles[SCI_NUM_CH] =
{
            NULL,        /* ch0 */

#if SCI_CFG_CH1_INCLUDED
            &ch1_ctrl,
#else
            NULL,
#endif

            NULL,        /* ch2 */
            NULL,        /* ch3 */
            NULL,        /* ch4 */

#if SCI_CFG_CH5_INCLUDED
            &ch5_ctrl,
#else
            NULL,
#endif

            NULL,        /* ch6 */
            NULL,        /* ch7 */
            NULL,        /* ch8 */
            NULL,        /* ch9 */
            NULL,        /* ch10 */
            NULL,        /* ch11 */

#if SCI_CFG_CH12_INCLUDED
            &ch12_ctrl
#else
            NULL
#endif
};

