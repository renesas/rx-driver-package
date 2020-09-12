/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_scif_rx_data.c
* Description  : Channel control block data and baud rate tables for the SCIF driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.08.2014 1.00    Initial Release
*           11.11.2019 2.00    Added support for GNUC and ICCRX.
*                              Fixed coding style.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_scif_rx_if.h"
#include "r_scif_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/

/* BAUD DIVISOR INFO */

/* Asynchronous */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* when abcs=0 & bgdm=0, divisor = 64*pow(2,2n-1) */
/* when abcs=1 & bgdm=0 OR abcs=0 & bgdm=1, divisor = 32*pow(2,2n-1) */
/* when abcs=1 & bgdm=1, divisor = 16*pow(2,2n-1) */

#if (SCIF_CFG_ASYNC_INCLUDED)
/* NOTE: diff than SCI async baud table, but should provide same results */
const baud_divisor_t g_scif_async_baud[NUM_DIVISORS_ASYNC]=
{

    {8,    1, 1, 0},
    {16,   0, 1, 0},    /* divisor result, abcs, bgdm, n */
    {32,   0, 0, 0},
    {64,   0, 1, 1},
    {128,  0, 0, 1},
    {256,  0, 1, 2},
    {512,  0, 0, 2},
    {1024, 0, 1, 3},
    {2048, 0, 0, 3}
};
#endif

/* Synchronous and Simple SPI */
/* BRR = (PCLK/(divisor * baud)) - 1 */
/* abcs=0, bdgm=0, divisor = 8*pow(2,2n-1) */

#if (SCIF_CFG_SYNC_INCLUDED)
/* NOTE: Identical to SCI sync baud table */
const baud_divisor_t g_scif_sync_baud[NUM_DIVISORS_SYNC]=
{
    {4,   0, 0, 0},    /* divisor result, abcs, bgdm, n */
    {16,  0, 0, 1},
    {64,  0, 0, 2},
    {256, 0, 0, 3}
};
#endif


/* CHANNEL MEMORY ALLOCATIONS */


#if SCIF_CFG_CH8_INCLUDED

#if ((SCIF_CFG_CH8_RX_FIFO_THRESHOLD < 1) || (SCIF_CFG_CH8_RX_FIFO_THRESHOLD > 16))
#error "SCIF_CFG_CH8_RX_FIFO_THRESHOLD in r_scif_rx_config.h must be 1-16."
#endif
#if ((SCIF_CFG_CH8_TX_FIFO_THRESHOLD < 0) || SCIF_CFG_CH8_TX_FIFO_THRESHOLD > 15)
#error "SCIF_CFG_CH8_TX_FIFO_THRESHOLD in r_scif_rx_config.h must be 0-15."
#endif

/* rom info */
const scif_ch_rom_t g_scif_ch8_rom = {(volatile R_BSP_EVENACCESS_SFR struct st_scifa *)&SCIFA8,
                                BIT27_MASK,
                                BSP_INT_SRC_AL0_SCIFA8_TEIF8, scif8_teif8_isr,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BSP_INT_SRC_AL0_SCIFA8_ERIF8, scif8_erif8_isr,
                                BSP_INT_SRC_AL0_SCIFA8_BRIF8, scif8_brif8_isr,
                                #endif
                                BIT0_MASK,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BIT1_MASK, BIT2_MASK,
                                #endif
                                &ICU.IPR[IPR_SCIFA8_RXIF8].BYTE,
                                &ICU.IPR[IPR_SCIFA8_TXIF8].BYTE,
                                &ICU.IR[IR_SCIFA8_RXIF8].BYTE,
                                &ICU.IR[IR_SCIFA8_TXIF8].BYTE,
                                &ICU.IER[IER_SCIFA8_RXIF8].BYTE,
                                &ICU.IER[IER_SCIFA8_TXIF8].BYTE,
                                BIT4_MASK, BIT5_MASK,
                                SCIF_CFG_CH8_RX_FIFO_THRESHOLD,
                                SCIF_CFG_CH8_TX_FIFO_THRESHOLD
                                };

/* channel control block */
scif_ch_ctrl_t g_scif_ch8_ctrl = {&g_scif_ch8_rom, SCIF_MODE_OFF, 0, NULL,
                                0, 0, 0, 0,
                                NULL, NULL, 0, 0,
                                NULL, NULL, 0, 0,
                                true, false
                                };
#endif /* SCIF_CFG_CH8_INCLUDED */


#if SCIF_CFG_CH9_INCLUDED

#if ((SCIF_CFG_CH9_RX_FIFO_THRESHOLD < 1) || (SCIF_CFG_CH9_RX_FIFO_THRESHOLD > 16))
#error "SCIF_CFG_CH9_RX_FIFO_THRESHOLD in r_scif_rx_config.h must be 1-16."
#endif
#if ((SCIF_CFG_CH9_TX_FIFO_THRESHOLD < 0) || SCIF_CFG_CH9_TX_FIFO_THRESHOLD > 15)
#error "SCIF_CFG_CH9_TX_FIFO_THRESHOLD in r_scif_rx_config.h must be 0-15."
#endif

/* rom info */
const scif_ch_rom_t g_scif_ch9_rom = {(volatile R_BSP_EVENACCESS_SFR struct st_scifa *)&SCIFA9,
                                BIT26_MASK,
                                BSP_INT_SRC_AL0_SCIFA9_TEIF9, scif9_teif9_isr,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BSP_INT_SRC_AL0_SCIFA9_ERIF9, scif9_erif9_isr,
                                BSP_INT_SRC_AL0_SCIFA9_BRIF9, scif9_brif9_isr,
                                #endif
                                BIT4_MASK,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BIT5_MASK, BIT6_MASK,
                                #endif
                                &ICU.IPR[IPR_SCIFA9_RXIF9].BYTE,
                                &ICU.IPR[IPR_SCIFA9_TXIF9].BYTE,
                                &ICU.IR[IR_SCIFA9_RXIF9].BYTE,
                                &ICU.IR[IR_SCIFA9_TXIF9].BYTE,
                                &ICU.IER[IER_SCIFA9_RXIF9].BYTE,
                                &ICU.IER[IER_SCIFA9_TXIF9].BYTE,
                                BIT6_MASK, BIT7_MASK,
                                SCIF_CFG_CH9_RX_FIFO_THRESHOLD,
                                SCIF_CFG_CH9_TX_FIFO_THRESHOLD
                                };

/* channel control block */
scif_ch_ctrl_t  g_scif_ch9_ctrl = {&g_scif_ch9_rom, SCIF_MODE_OFF, 0, NULL,
                                0, 0, 0, 0,
                                NULL, NULL, 0, 0,
                                NULL, NULL, 0, 0,
                                true, false
                                };
#endif /* SCIF_CFG_CH9_INCLUDED */


#if SCIF_CFG_CH10_INCLUDED

#if ((SCIF_CFG_CH10_RX_FIFO_THRESHOLD < 1) || (SCIF_CFG_CH10_RX_FIFO_THRESHOLD > 16))
#error "SCIF_CFG_CH10_RX_FIFO_THRESHOLD in r_scif_rx_config.h must be 1-16."
#endif
#if ((SCIF_CFG_CH10_TX_FIFO_THRESHOLD < 0) || SCIF_CFG_CH10_TX_FIFO_THRESHOLD > 15)
#error "SCIF_CFG_CH10_TX_FIFO_THRESHOLD in r_scif_rx_config.h must be 0-15."
#endif

/* rom info */
const scif_ch_rom_t g_scif_ch10_rom = {(volatile R_BSP_EVENACCESS_SFR struct st_scifa *)&SCIFA10,
                                BIT25_MASK,
                                BSP_INT_SRC_AL0_SCIFA10_TEIF10, scif10_teif10_isr,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BSP_INT_SRC_AL0_SCIFA10_ERIF10, scif10_erif10_isr,
                                BSP_INT_SRC_AL0_SCIFA10_BRIF10, scif10_brif10_isr,
                                #endif
                                BIT8_MASK,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BIT9_MASK, BIT10_MASK,
                                #endif
                                &ICU.IPR[IPR_SCIFA10_RXIF10].BYTE,
                                &ICU.IPR[IPR_SCIFA10_TXIF10].BYTE,
                                &ICU.IR[IR_SCIFA10_RXIF10].BYTE,
                                &ICU.IR[IR_SCIFA10_TXIF10].BYTE,
                                &ICU.IER[IER_SCIFA10_RXIF10].BYTE,
                                &ICU.IER[IER_SCIFA10_TXIF10].BYTE,
                                BIT0_MASK, BIT1_MASK,
                                SCIF_CFG_CH10_RX_FIFO_THRESHOLD,
                                SCIF_CFG_CH10_TX_FIFO_THRESHOLD
                                };
/* channel control block */
scif_ch_ctrl_t  g_scif_ch10_ctrl = {&g_scif_ch10_rom, SCIF_MODE_OFF, 0, NULL,
                                0, 0, 0, 0,
                                NULL, NULL, 0, 0,
                                NULL, NULL, 0, 0,
                                true, false
                                };
#endif /* SCIF_CFG_CH10_INCLUDED */


#if SCIF_CFG_CH11_INCLUDED

#if ((SCIF_CFG_CH11_RX_FIFO_THRESHOLD < 1) || (SCIF_CFG_CH11_RX_FIFO_THRESHOLD > 16))
#error "SCIF_CFG_CH11_RX_FIFO_THRESHOLD in r_scif_rx_config.h must be 1-16."
#endif
#if ((SCIF_CFG_CH11_TX_FIFO_THRESHOLD < 0) || SCIF_CFG_CH11_TX_FIFO_THRESHOLD > 15)
#error "SCIF_CFG_CH11_TX_FIFO_THRESHOLD in r_scif_rx_config.h must be 0-15."
#endif

/* rom info */
const scif_ch_rom_t g_scif_ch11_rom = {(volatile R_BSP_EVENACCESS_SFR struct st_scifa *)&SCIFA11,
                                BIT24_MASK,
                                BSP_INT_SRC_AL0_SCIFA11_TEIF11, scif11_teif11_isr,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BSP_INT_SRC_AL0_SCIFA11_ERIF11, scif11_erif11_isr,
                                BSP_INT_SRC_AL0_SCIFA11_BRIF11, scif11_brif11_isr,
                                #endif
                                BIT12_MASK,
                                #if SCIF_CFG_ASYNC_INCLUDED
                                BIT13_MASK, BIT14_MASK,
                                #endif
                                &ICU.IPR[IPR_SCIFA11_RXIF11].BYTE,
                                &ICU.IPR[IPR_SCIFA11_TXIF11].BYTE,
                                &ICU.IR[IR_SCIFA11_RXIF11].BYTE,
                                &ICU.IR[IR_SCIFA11_TXIF11].BYTE,
                                &ICU.IER[IER_SCIFA11_RXIF11].BYTE,
                                &ICU.IER[IER_SCIFA11_TXIF11].BYTE,
                                BIT2_MASK, BIT3_MASK,
                                SCIF_CFG_CH11_RX_FIFO_THRESHOLD,
                                SCIF_CFG_CH11_TX_FIFO_THRESHOLD
                                };
/* channel control block */
scif_ch_ctrl_t  g_scif_ch11_ctrl = {&g_scif_ch11_rom, SCIF_MODE_OFF, 0, NULL,
                                0, 0, 0, 0,
                                NULL, NULL, 0, 0,
                                NULL, NULL, 0, 0,
                                true, false
                                };
#endif /* SCIF_CFG_CH11_INCLUDED */



/* SCI HANDLE-ARRAY DECLARATION */

/* static const scif_hdl_t g_scif_handles[SCI_NUM_CH] = */
const scif_hdl_t g_scif_handles[SCIF_NUM_CH] =
{
#if SCIF_CFG_CH8_INCLUDED
            &g_scif_ch8_ctrl,
#else
            NULL,
#endif /* SCIF_CFG_CH8_INCLUDED */
#if SCIF_CFG_CH9_INCLUDED
            &g_scif_ch9_ctrl,
#else
            NULL,
#endif /* SCIF_CFG_CH9_INCLUDED */
#if SCIF_CFG_CH10_INCLUDED
            &g_scif_ch10_ctrl,
#else
            NULL,
#endif /* SCIF_CFG_CH10_INCLUDED */
#if SCIF_CFG_CH11_INCLUDED
            &g_scif_ch11_ctrl,
#else
            NULL,
#endif /* SCIF_CFG_CH11_INCLUDED */
};
