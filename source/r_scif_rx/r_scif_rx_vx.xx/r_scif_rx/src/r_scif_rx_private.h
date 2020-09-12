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
* File Name    : r_scif_rx_private.h
* Description  : Internal structures and definitions for the SCIF driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.08.2014 1.00    Initial Release
*           11.11.2019 2.00    Added support for GNUC and ICCRX.
*                              Fixed coding style.
***********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "r_scif_rx_if.h"

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR < 50)) || (R_BSP_VERSION_MAJOR <= 4) 
    #error "This module must use BSP module of Rev.5.50 or higher. Please use the BSP module of Rev.5.50 or higher."
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef SCIF_PRIVATE_H
#define SCIF_PRIVATE_H

/* Bit position masks */
#define BIT0_MASK   (1<<0)
#define BIT1_MASK   (1<<1)
#define BIT2_MASK   (1<<2)
#define BIT3_MASK   (1<<3)
#define BIT4_MASK   (1<<4)
#define BIT5_MASK   (1<<5)
#define BIT6_MASK   (1<<6)
#define BIT7_MASK   (1<<7)
#define BIT8_MASK   (1<<8)
#define BIT9_MASK   (1<<9)
#define BIT10_MASK  (1<<10)
#define BIT11_MASK  (1<<11)
#define BIT12_MASK  (1<<12)
#define BIT13_MASK  (1<<13)
#define BIT14_MASK  (1<<14)
#define BIT15_MASK  (1<<15)
#define BIT16_MASK  (1<<16)
#define BIT17_MASK  (1<<17)
#define BIT18_MASK  (1<<18)
#define BIT19_MASK  (1<<19)
#define BIT20_MASK  (1<<20)
#define BIT21_MASK  (1<<21)
#define BIT22_MASK  (1<<22)
#define BIT23_MASK  (1<<23)
#define BIT24_MASK  (1<<24)
#define BIT25_MASK  (1<<25)
#define BIT26_MASK  (1<<26)
#define BIT27_MASK  (1<<27)
#define BIT28_MASK  (1<<28)
#define BIT29_MASK  (1<<29)
#define BIT30_MASK  (1<<30)
#define BIT31_MASK  (1<<31)

#define NUM_DIVISORS_ASYNC  (9)
#define NUM_DIVISORS_SYNC   (4)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/* ROM INFO */

typedef struct st_scif_ch_rom       /* SCIF ROM info for channel control block */
{
    volatile R_BSP_EVENACCESS_SFR struct st_scifa  *regs;  /* base ptr to ch registers */
    uint32_t                        stop_mask;      /* mstp mask to disable ch */
    bsp_int_src_t                   teif_vector;    /* for BSP_InterruptWrite() */
    bsp_int_cb_t                    teif_isr;       /*  " */
#if SCIF_CFG_ASYNC_INCLUDED
    bsp_int_src_t                   erif_vector;    /*  " */
    bsp_int_cb_t                    erif_isr;       /*  " */
    bsp_int_src_t                   brif_vector;    /*  " */
    bsp_int_cb_t                    brif_isr;       /*  " */
#endif
    uint32_t                        teif_ch_mask;   /* GENAL0 and GRPAL0 mask */
#if SCIF_CFG_ASYNC_INCLUDED
    uint32_t                        erif_ch_mask;   /*  " */
    uint32_t                        brif_ch_mask;   /*  " */
#endif
    volatile uint8_t R_BSP_EVENACCESS_SFR *p_ipr_rxif;       /* ptr to RXIF IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR *p_ipr_txif;       /* ptr to TXIF IPR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR *p_ir_rxif;        /* ptr to RXIF IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR *p_ir_txif;        /* ptr to TXIF IR register */
    volatile uint8_t R_BSP_EVENACCESS_SFR *p_ier_rxif;      /* ptr to RXIF IER register */
    volatile uint8_t R_BSP_EVENACCESS_SFR *p_ier_txif;      /* ptr to TXIF IER register */
    uint8_t                         rxif_en_mask;   /* RXIF IER enable mask */
    uint8_t                         txif_en_mask;   /* TXIF IER enable mask */
    uint8_t                         rx_dflt_thresh;
    uint8_t                         tx_dflt_thresh;
} scif_ch_rom_t;


/* CHANNEL CONTROL BLOCK */

typedef struct st_scif_fifo_ctrl
{
    uint8_t     *p_tx_buf;            /* user's buffer */
    uint8_t     *p_rx_buf;            /* user's buffer */
    uint16_t    tx_cnt;             /* bytes remaining to add to FIFO */
    uint16_t    rx_cnt;             /* bytes waiting to receive from FIFO */
} scif_fifo_ctrl_t;

typedef struct st_scif_ch_ctrl      /* SCIF Channel Control Block */
{
    scif_ch_rom_t const *p_rom;       /* pointer to rom info */
    scif_mode_t         mode;       /* operational mode (async or sync) */
    uint32_t            baud_rate;  /* baud/bit rate */
    void              (*callback)(void *p_args); /* function ptr from Open() */
    uint8_t             qindex_app_tx;
    uint8_t             qindex_int_tx;
    uint8_t             qindex_app_rx;
    uint8_t             qindex_int_rx;
    scif_fifo_ctrl_t    queue[2];
    bool                tx_idle;    /* true = tx shift register is empty */
    bool                tx_break;   /* true = sending break */
} scif_ch_ctrl_t;


/* BAUD DIVISOR INFO */

/* BRR = (PCLK/(divisor * baud)) - 1
 * when abcs=1, divisor = 32*pow(2,2n-1)
 * when abcs=0, divisor = 64*pow(2,2n-1) */

typedef struct st_baud_divisor
{
    int16_t     divisor;    /* clock divisor */
    uint8_t     abcs;       /* abcs value to get divisor */
    uint8_t     bgdm;       /* bdgm value to get divisor */
    uint8_t     cks;        /* cks  value to get divisor (cks = n) */
} baud_divisor_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
#if (SCIF_CFG_ASYNC_INCLUDED)
extern const baud_divisor_t g_scif_async_baud[];
#endif
#if (SCIF_CFG_SYNC_INCLUDED)
extern const baud_divisor_t g_scif_sync_baud[];
#endif

#if SCIF_CFG_CH8_INCLUDED
extern const scif_ch_rom_t      g_scif_ch8_rom;
extern scif_ch_ctrl_t           g_scif_ch8_ctrl;
#endif

#if SCIF_CFG_CH9_INCLUDED
extern const scif_ch_rom_t      g_scif_ch9_rom;
extern scif_ch_ctrl_t           g_scif_ch9_ctrl;
#endif

#if SCIF_CFG_CH10_INCLUDED
extern const scif_ch_rom_t      g_scif_ch10_rom;
extern scif_ch_ctrl_t           g_scif_ch10_ctrl;
#endif

#if SCIF_CFG_CH11_INCLUDED
extern const scif_ch_rom_t      g_scif_ch11_rom;
extern scif_ch_ctrl_t           g_scif_ch11_ctrl;
#endif

extern const scif_hdl_t g_scif_handles[];

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
extern void scif8_teif8_isr(void *cb_args);
extern void scif9_teif9_isr(void *cb_args);
extern void scif10_teif10_isr(void *cb_args);
extern void scif11_teif11_isr(void *cb_args);

#if (SCIF_CFG_ASYNC_INCLUDED)
extern void scif8_erif8_isr(void *cb_args);
extern void scif9_erif9_isr(void *cb_args);
extern void scif10_erif10_isr(void *cb_args);
extern void scif11_erif11_isr(void *cb_args);

extern void scif8_brif8_isr(void *cb_args);
extern void scif9_brif9_isr(void *cb_args);
extern void scif10_brif10_isr(void *cb_args);
extern void scif11_brif11_isr(void *cb_args);

extern void scif8_drif8_isr(void *cb_args);
extern void scif9_drif9_isr(void *cb_args);
extern void scif10_drif10_isr(void *cb_args);
extern void scif11_drif11_isr(void *cb_args);
#endif

void scif_init_prologue(scif_hdl_t const hdl);
void scif_init_epilogue(scif_hdl_t const hdl, bool msb_first);

#endif /* SCIF_PRIVATE_H */

