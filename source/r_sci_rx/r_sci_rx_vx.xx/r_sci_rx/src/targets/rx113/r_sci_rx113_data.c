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
* File Name    : r_sci_rx113_data.c
* Description  : Functions for using SCI on the RX113 device.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.06.2016 1.00    Initial Release.
*           20.05.2019 3.00    Added support for GNUC and ICCRX.
*           25.08.2020 3.60    Added feature using DTC/DMAC in SCI transfer.
*                              Merged IrDA functionality to SCI FIT.
*           31.03.2021 3.80    Updated macro definition enable and disable TXI, RXI, ERI, TEI.
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#include "r_sci_rx113_private.h"

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

#if (SCI_CFG_IRDA_INCLUDED)
/* Infrared Interface */
/* BRR = (PCLK/(divisor * baud)) - 1    */
/* abcs=0, divisor = 64*pow(2,2n-1)     */

const baud_divisor_t irda_async_baud[NUM_DIVISORS_IRDA]=
{
    /* divisor result, abcs, n */
    {32,   0, 0},
    {128,  0, 1},
    {512,  0, 2},
    {2048, 0, 3}
};

const out_width_divisor_t irda_ircks_div[NUM_WIDTH_DIVISORS_IRDA] =
{
    {16 ,3},        /* div B * 3/16 */
    {2  ,1},        /* div P /2     */
    {4  ,1},        /* div P /4     */
    {8  ,1},        /* div P /8     */
    {16 ,1},        /* div P /16    */
    {32 ,1},        /* div P /32    */
    {64 ,1},        /* div P /64    */
    {128,1}         /* div P /128   */
};

#endif

/* CHANNEL MEMORY ALLOCATIONS */

#if SCI_CFG_CH0_INCLUDED

/* rom info */
const sci_ch_rom_t  ch0_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI0,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT31_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
                                &ICU.IPR[IPR_SCI0_RXI0].BYTE,
                                &ICU.IR[IR_SCI0_RXI0].BYTE,
                                &ICU.IR[IR_SCI0_TXI0].BYTE,
                                &ICU.IR[IR_SCI0_TEI0].BYTE,
                                &ICU.IR[IR_SCI0_ERI0].BYTE,
                                &ICU.IER[IER_SCI0_RXI0].BYTE,
                                &ICU.IER[IER_SCI0_TXI0].BYTE,
                                &ICU.IER[IER_SCI0_TEI0].BYTE,
                                &ICU.IER[IER_SCI0_ERI0].BYTE,
                                SCI_BIT6, SCI_BIT7, SCI_BIT0, SCI_BIT1
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH0_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH0_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI0_TXI0
                                , DTCE_SCI0_RXI0
                                #endif
                                };

/* channel control block */
sci_ch_ctrl_t   ch0_ctrl = {&ch0_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            , NULL
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH0_INCLUDED */

#if SCI_CFG_CH1_INCLUDED

/* rom info */
const sci_ch_rom_t  ch1_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI1,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT30_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
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
sci_ch_ctrl_t   ch1_ctrl = {&ch1_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            , NULL
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH1_INCLUDED */

#if SCI_CFG_CH2_INCLUDED

/* rom info */
const sci_ch_rom_t  ch2_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI2,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT29_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
                                &ICU.IPR[IPR_SCI2_RXI2].BYTE,
                                &ICU.IR[IR_SCI2_RXI2].BYTE,
                                &ICU.IR[IR_SCI2_TXI2].BYTE,
                                &ICU.IR[IR_SCI2_TEI2].BYTE,
                                &ICU.IR[IR_SCI2_ERI2].BYTE,
                                &ICU.IER[IER_SCI2_RXI2].BYTE,
                                &ICU.IER[IER_SCI2_TXI2].BYTE,
                                &ICU.IER[IER_SCI2_TEI2].BYTE,
                                &ICU.IER[IER_SCI2_ERI2].BYTE,
                                SCI_BIT2, SCI_BIT3, SCI_BIT4, SCI_BIT5
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH2_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH2_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI2_TXI2
                                , DTCE_SCI2_RXI2
                                #endif
                                };

/* channel control block */
sci_ch_ctrl_t   ch2_ctrl = {&ch2_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            , NULL
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH2_INCLUDED */

#if SCI_CFG_CH5_INCLUDED

/* rom info */
const sci_ch_rom_t  ch5_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI5,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT26_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                (volatile struct st_irda R_BSP_EVENACCESS_SFR *)&IRDA,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRC.LONG, BIT20_MASK,
#endif
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

#if SCI_CFG_CH5_IRDA_INCLUDED
#if (SCI_CFG_IRDA_INCLUDED)
/* Port setting values for pins IRTXD and IRRXD */
const sci_irda_ch_port_rom_t g_sci_irda_ch5_portrom =
{
                                 /* IRTXD Pin Select */
                                SCI_CFG_CH5_IRDA_IRTXD_GP,              /* 0: Port group of IRTXD pin */
                                SCI_CFG_CH5_IRDA_IRTXD_PIN,             /* 1: Port bit of IRTXD pin */
                                1,                                      /* 2: Pin direction control (IN/OUT) when not selecting the IRTXD pin */
                                SCI_CFG_CH5_IRDA_IRTXD_INACTIVE_LEVEL,  /* 3: Pin level when  not selecting the IRTXD pin */
                                SCI_IRDA_MPC_IRTXD5_ENABLE,             /* 4: MPC setting data of IRTXD pin */

                                /* IRTXD Pin Select */
                                SCI_CFG_CH5_IRDA_IRRXD_GP,              /* 0: Port group of IRRXD pin */
                                SCI_CFG_CH5_IRDA_IRRXD_PIN,             /* 1: Port bit of IRRXD pin */
                                0,                                      /* 2: Pin direction control (IN/OUT) when not selecting the IRRXD pin */
                                SCI_CFG_CH5_IRDA_IRRXD_INACTIVE_LEVEL,  /* 3: Pin level when  not selecting the IRRXD pin */
                                SCI_IRDA_MPC_IRRXD5_ENABLE              /* 4: MPC setting data of IRRXD pin */
};
#endif
#endif

/* channel control block */
sci_ch_ctrl_t   ch5_ctrl = {&ch5_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            #if (SCI_CFG_CH5_IRDA_INCLUDED)
                            , &g_sci_irda_ch5_portrom
                            #else
                            , NULL
                            #endif
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH5_INCLUDED */

#if SCI_CFG_CH6_INCLUDED

/* rom info */
const sci_ch_rom_t  ch6_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI6,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT25_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
                                &ICU.IPR[IPR_SCI6_RXI6].BYTE,
                                &ICU.IR[IR_SCI6_RXI6].BYTE,
                                &ICU.IR[IR_SCI6_TXI6].BYTE,
                                &ICU.IR[IR_SCI6_TEI6].BYTE,
                                &ICU.IR[IR_SCI6_ERI6].BYTE,
                                &ICU.IER[IER_SCI6_RXI6].BYTE,
                                &ICU.IER[IER_SCI6_TXI6].BYTE,
                                &ICU.IER[IER_SCI6_TEI6].BYTE,
                                &ICU.IER[IER_SCI6_ERI6].BYTE,
                                SCI_BIT2, SCI_BIT3, SCI_BIT4, SCI_BIT5
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH6_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH6_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI6_TXI6
                                , DTCE_SCI6_RXI6
                                #endif
                                };

/* channel control block */
sci_ch_ctrl_t   ch6_ctrl = {&ch6_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            , NULL
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH6_INCLUDED */

#if SCI_CFG_CH8_INCLUDED

/* rom info */
const sci_ch_rom_t  ch8_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI8,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRC.LONG, BIT27_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
                                &ICU.IPR[IPR_SCI8_RXI8].BYTE,
                                &ICU.IR[IR_SCI8_RXI8].BYTE,
                                &ICU.IR[IR_SCI8_TXI8].BYTE,
                                &ICU.IR[IR_SCI8_TEI8].BYTE,
                                &ICU.IR[IR_SCI8_ERI8].BYTE,
                                &ICU.IER[IER_SCI8_RXI8].BYTE,
                                &ICU.IER[IER_SCI8_TXI8].BYTE,
                                &ICU.IER[IER_SCI8_TEI8].BYTE,
                                &ICU.IER[IER_SCI8_ERI8].BYTE,
                                SCI_BIT6, SCI_BIT7, SCI_BIT0, SCI_BIT1
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH8_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH8_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI8_TXI8
                                , DTCE_SCI8_RXI8
                                #endif
                                };

/* channel control block */
sci_ch_ctrl_t   ch8_ctrl = {&ch8_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            , NULL
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH8_INCLUDED */

#if SCI_CFG_CH9_INCLUDED

/* rom info */
const sci_ch_rom_t  ch9_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI9,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRC.LONG, BIT26_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
                                &ICU.IPR[IPR_SCI9_RXI9].BYTE,
                                &ICU.IR[IR_SCI9_RXI9].BYTE,
                                &ICU.IR[IR_SCI9_TXI9].BYTE,
                                &ICU.IR[IR_SCI9_TEI9].BYTE,
                                &ICU.IR[IR_SCI9_ERI9].BYTE,
                                &ICU.IER[IER_SCI9_RXI9].BYTE,
                                &ICU.IER[IER_SCI9_TXI9].BYTE,
                                &ICU.IER[IER_SCI9_TEI9].BYTE,
                                &ICU.IER[IER_SCI9_ERI9].BYTE,
                                SCI_BIT2, SCI_BIT3, SCI_BIT4, SCI_BIT5
                                #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                                , SCI_CFG_CH9_TX_DTC_DMACA_ENABLE
                                , SCI_CFG_CH9_RX_DTC_DMACA_ENABLE
                                #endif
                                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                                , DTCE_SCI9_TXI9
                                , DTCE_SCI9_RXI9
                                #endif
                                };

/* channel control block */
sci_ch_ctrl_t   ch9_ctrl = {&ch9_rom
                            #if (SCI_CFG_IRDA_INCLUDED)
                            , NULL
                            #endif
                            , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                            #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                            , true, 0, 0, false
                            #endif
                            , BSP_PCLKB_HZ
                            #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                           };
#endif /* End of SCI_CFG_CH9_INCLUDED */

#if SCI_CFG_CH12_INCLUDED

/* rom info */
const sci_ch_rom_t  ch12_rom = {(volatile struct st_sci12 R_BSP_EVENACCESS_SFR *)&SCI12,
                                (volatile uint32_t R_BSP_EVENACCESS_SFR*)&SYSTEM.MSTPCRB.LONG, BIT4_MASK,
#if (SCI_CFG_IRDA_INCLUDED)
                                NULL, NULL, 0,
#endif
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
sci_ch_ctrl_t   ch12_ctrl = {&ch12_rom
                             #if (SCI_CFG_IRDA_INCLUDED)
                             , NULL
                             #endif
                             , SCI_MODE_OFF, 0, NULL, NULL, NULL, true
                             #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
                             , true, 0, 0, false
                             #endif
                             , BSP_PCLKB_HZ
                             #if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
                            , 0, 0, 0, 0, 0
                            #endif
                            };
#endif /* End of SCI_CFG_CH12_INCLUDED */


/* SCI HANDLE-ARRAY DECLARATION */

const sci_hdl_t g_handles[SCI_NUM_CH] =
{
#if SCI_CFG_CH0_INCLUDED
            &ch0_ctrl,
#else
            NULL,
#endif

#if SCI_CFG_CH1_INCLUDED
            &ch1_ctrl,
#else
            NULL,
#endif

#if SCI_CFG_CH2_INCLUDED
            &ch2_ctrl,
#else
            NULL,
#endif
            NULL,        /* ch3 */
            NULL,        /* ch4 */

#if SCI_CFG_CH5_INCLUDED
            &ch5_ctrl,
#else
            NULL,
#endif

#if SCI_CFG_CH6_INCLUDED
            &ch6_ctrl,
#else
            NULL,
#endif

            NULL,        /* ch7 */

#if SCI_CFG_CH8_INCLUDED
            &ch8_ctrl,
#else
            NULL,
#endif

#if SCI_CFG_CH9_INCLUDED
            &ch9_ctrl,
#else
            NULL,
#endif

            NULL,        /* ch10 */
            NULL,        /* ch11 */

#if SCI_CFG_CH12_INCLUDED
            &ch12_ctrl
#else
            NULL
#endif
};

