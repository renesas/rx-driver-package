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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsci_rx26t_private.h
* Description  : Functions for using RSCI on the RX26T device.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           15.08.2022 1.00    Initial Release
***********************************************************************************************************************/

#ifndef RSCI_RX26T_H
#define RSCI_RX26T_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "../../r_rsci_rx_private.h"

#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
#include "r_byteq_if.h"
#endif

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* RSCI channel include Check */
#if (RSCI_CFG_CH10_INCLUDED != 0)
    #error "ERROR - Unsupported channel chosen in r_sci_config.h"
#endif

/* RSCI SCR register masks */
#define RSCI_SCR0_TIE_MASK    (0x00100000U)     /* transmit interrupt enable */
#define RSCI_SCR0_RIE_MASK    (0x00010000U)     /* receive interrupt enable */
#define RSCI_SCR0_TE_MASK     (0x00000010U)     /* transmitter enable */
#define RSCI_SCR0_RE_MASK     (0x00000001U)     /* receiver enable */
#define RSCI_EN_XCVR_MASK     (RSCI_SCR0_RE_MASK | RSCI_SCR0_TE_MASK | RSCI_SCR0_RIE_MASK | RSCI_SCR0_TIE_MASK)

/* RSCI MMCR register masks */
#define RSCI_MMCR_PFERIE_MASK    (0x01000000U)     /* preface error interrupt enable */
#define RSCI_MMCR_SYERIE_MASK    (0x02000000U)     /* sync error interrupt enable */
#define RSCI_MMCR_SBERIE_MASK    (0x04000000U)     /* transmitter enable */
#define RSCI_EN_MMCR_ERR_MASK    (RSCI_MMCR_PFERIE_MASK | RSCI_MMCR_SYERIE_MASK | RSCI_MMCR_SBERIE_MASK )

/* RSCI SSR register receiver error masks */
#define RSCI_SSR_ORER_MASK         (0x01000000U)     /* overflow error */
#define RSCI_SSR_AFER_MASK         (0x10000000U)     /* framing error */
#define RSCI_SSR_APER_MASK         (0x08000000U)     /* parity err */
#define RSCI_RCVR_ERR_MASK         (RSCI_SSR_ORER_MASK | RSCI_SSR_AFER_MASK | RSCI_SSR_APER_MASK)

/* RSCI SSCR register receiver error masks */
#define RSCI_SSCR_ORERC_MASK        (0x01000000U)     /* overflow error */
#define RSCI_SSCR_AFERC_MASK        (0x10000000U)     /* framing error */
#define RSCI_SSCR_APERC_MASK        (0x08000000U)     /* parity err */
#define RSCI_SSCR_ERR_CLEAR_MASK    (RSCI_SSCR_ORERC_MASK | RSCI_SSCR_AFERC_MASK | RSCI_SSCR_APERC_MASK)

/* RSCI MMSR register receiver error masks */
#define RSCI_MMSR_PFER_MASK        (0x00000001U)     /* preface error */
#define RSCI_MMSR_SYER_MASK        (0X00000002U)     /* sync error */
#define RSCI_MMSR_SBER_MASK        (0x00000004U)     /* start bit error */
#define RSCI_MMSR_MCER_MASK        (0x00000010U)     /* manchester code error */
#define RSCI_MMSR_ERR_MASK         (RSCI_MMSR_PFER_MASK | RSCI_MMSR_SYER_MASK \
                                    | RSCI_MMSR_SBER_MASK | RSCI_MMSR_MCER_MASK)

/* RSCI MMSCR register receiver error clear masks */
#define RSCI_MMSCR_PFERC_MASK      (0x00000001U)     /* preface error clear */
#define RSCI_MMSCR_SYERC_MASK      (0X00000002U)     /* sync error clear */
#define RSCI_MMSCR_SBERC_MASK      (0x00000004U)     /* start bit error clear */
#define RSCI_MMSCR_MCERC_MASK      (0x00000010U)     /* manchester code error clear */
#define RSCI_MMSCR_ERR_CLEAR_MASK  (RSCI_MMSCR_PFERC_MASK | RSCI_MMSCR_SYERC_MASK \
                                    | RSCI_MMSCR_SBERC_MASK | RSCI_MMSCR_MCERC_MASK)

/* Macros to enable and disable ICU interrupts */
#define ENABLE_RXI_INT      (*hdl->rom->icu_rxi |= hdl->rom->rxi_en_mask)
#define DISABLE_RXI_INT     (*hdl->rom->icu_rxi &= (uint8_t)~hdl->rom->rxi_en_mask)
#define ENABLE_TXI_INT      (*hdl->rom->icu_txi |= hdl->rom->txi_en_mask)
#define DISABLE_TXI_INT     (*hdl->rom->icu_txi &= (uint8_t)~hdl->rom->txi_en_mask)

#define ENABLE_ERI_INT      (*hdl->rom->icu_grp |= hdl->rom->eri_ch_mask)
#define DISABLE_ERI_INT     (*hdl->rom->icu_grp &= ~hdl->rom->eri_ch_mask)
#define ENABLE_TEI_INT      (*hdl->rom->icu_grp |= hdl->rom->tei_ch_mask)
#define DISABLE_TEI_INT     (*hdl->rom->icu_grp &= ~hdl->rom->tei_ch_mask)

#define NUM_DIVISORS_ASYNC  (10)
#define NUM_DIVISORS_SYNC   (4)

/*****************************************************************************
Typedef definitions
******************************************************************************/

typedef struct st_rsci_fifo_ctrl
{
    uint8_t     *p_tx_buf;          /* user's buffer */
    uint8_t     *p_rx_buf;          /* user's buffer */
    uint16_t    tx_cnt;             /* bytes remaining to add to FIFO */
    uint16_t    rx_cnt;             /* bytes waiting to receive from FIFO */
    uint16_t    total_length;       /* used for DTC in rsci_txi_handler */
} rsci_fifo_ctrl_t;

/* CHANNEL CONTROL BLOCK */

/* ROM INFO */

typedef struct st_rsci_ch_rom    /* RSCI ROM info for channel control block */
{
    volatile  struct st_rsci11 R_BSP_EVENACCESS_SFR  *regs;  /* base ptr to ch registers */
    volatile  uint32_t R_BSP_EVENACCESS_SFR *mstp;          /* ptr to mstp register */
    uint32_t                        stop_mask;      /* mstp mask to disable ch */
#if RSCI_CFG_TEI_INCLUDED
    bsp_int_src_t                   tei_vector;
    bsp_int_cb_t                    tei_isr;
#endif
    bsp_int_src_t                   eri_vector;
    bsp_int_cb_t                    eri_isr;
    uint32_t                        tei_ch_mask;    /* ICU IR and IEN mask */
    uint32_t                        eri_ch_mask;    /* ICU IR and IEN mask */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ipr_rxi;       /* ptr to IPR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ipr_txi;       /* ptr to IPR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ir_rxi;        /* ptr to RXI IR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ir_txi;        /* ptr to TXI IR register */

    /* 
    * DO NOT use the enable/disable interrupt bits in the SCR 
    * register. Pending interrupts can be lost that way.
    */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *icu_rxi;       /* ptr to ICU register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *icu_txi;
    volatile  uint32_t R_BSP_EVENACCESS_SFR *icu_grp;
    uint8_t                         rxi_en_mask;    /* ICU enable/disable rxi mask */
    uint8_t                         txi_en_mask;    /* ICU enable/disable txi mask */
    uint8_t                         chan;           /* Channel RSCI is used*/
} rsci_ch_rom_t;


/* CHANNEL CONTROL BLOCK */

typedef struct st_rsci_ch_ctrl       /* RSCI channel control (for handle) */
{
    rsci_ch_rom_t const *rom;        /* pointer to rom info */
    rsci_mode_t      mode;           /* operational mode */
    uint32_t        baud_rate;      /* baud rate */
    void          (*callback)(void *p_args); /* function ptr for rcvr errs */
    union
    {
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
        byteq_hdl_t     que;        /* async/manc transmit queue handle */
#endif
        uint8_t         *buf;       /* sspi/sync tx buffer ptr */
    } u_tx_data;
    union
    {
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
        byteq_hdl_t     que;        /* async/manc receive queue handle */
#endif
        uint8_t         *buf;       /* sspi/sync rx buffer ptr */
    } u_rx_data;
    bool            tx_idle;        /* TDR is empty (async); TSR is empty (sync/sspi) */
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
    bool            save_rx_data;   /* save the data that is clocked in */
    uint16_t        tx_cnt;         /* number of bytes to transmit */
    uint16_t        rx_cnt;         /* number of bytes to receive */
    bool            tx_dummy;       /* transmit dummy byte, not buffer */
#endif
    uint32_t        pclk_speed;     /* saved peripheral clock speed for break generation */
#if RSCI_CFG_FIFO_INCLUDED
    uint8_t         fifo_ctrl;      /* fifo ctrl (enable/disable) flag */
    uint8_t         rx_dflt_thresh; /* RX FIFO threshold(default) */
    uint8_t         rx_curr_thresh; /* RX FIFO threshold(current) */
    uint8_t         tx_dflt_thresh; /* TX FIFO threshold(default) */
    uint8_t         tx_curr_thresh; /* TX FIFO threshold(current) */
#endif
#if RSCI_CFG_MANC_INCLUDED
    uint8_t         rx_decoding_pol;    /* Decoding conversion select */
    uint8_t         rx_preface_length;  /* RX Preface length */
    uint8_t         rx_preface_pattern; /* RX Preface pattern */
    uint8_t         tx_encoding_pol;    /* Encoding conversion select */
    uint8_t         tx_preface_length;  /* TX Preface length */
    uint8_t         tx_preface_pattern; /* TX Preface pattern */
#endif
} rsci_ch_ctrl_t;


/* BAUD DIVISOR INFO */

/* BRR = (PCLK/(divisor * baud)) - 1 */
/* when abcs=1, divisor = 32*pow(2,2n-1) */
/* when abcs=0, divisor = 64*pow(2,2n-1) */

typedef struct st_baud_divisor
{
    int16_t     divisor;    // clock divisor
    uint8_t     abcs;       // abcs value to get divisor
    uint8_t     bgdm;       // bdgm value to get divisor
    uint8_t     abcse;      // abcse value to get divisor
    uint8_t     cks;        // cks  value to get divisor (cks = n)
} baud_divisor_t;



/*****************************************************************************
Exported global variables and functions
******************************************************************************/
extern const rsci_hdl_t g_rsci_handles[];

#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
extern const baud_divisor_t rsci_async_baud[];
#endif
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
extern const baud_divisor_t rsci_sync_baud[];
#endif


#if (RSCI_CFG_CH8_INCLUDED)
extern const rsci_ch_rom_t      g_rsci_ch8_rom;
extern rsci_ch_ctrl_t           g_rsci_ch8_ctrl;
#endif

#if (RSCI_CFG_CH9_INCLUDED)
extern const rsci_ch_rom_t      g_rsci_ch9_rom;
extern rsci_ch_ctrl_t           g_rsci_ch9_ctrl;
#endif

#if (RSCI_CFG_CH11_INCLUDED)
extern const rsci_ch_rom_t      g_rsci_ch11_rom;
extern rsci_ch_ctrl_t           g_rsci_ch11_ctrl;
#endif

/*****************************************************************************
Exported global functions
******************************************************************************/
#if RSCI_CFG_TEI_INCLUDED
#if (RSCI_CFG_CH8_INCLUDED)
extern void rsci8_tei_isr (void *cb_args);
#endif

#if (RSCI_CFG_CH9_INCLUDED)
extern void rsci9_tei_isr (void *cb_args);
#endif

#if (RSCI_CFG_CH11_INCLUDED)
extern void rsci11_tei_isr (void *cb_args);
#endif
#endif

#if (RSCI_CFG_CH8_INCLUDED)
extern void rsci8_eri_isr (void *cb_args);
#endif

#if (RSCI_CFG_CH9_INCLUDED)
extern void rsci9_eri_isr (void *cb_args);
#endif

#if (RSCI_CFG_CH11_INCLUDED)
extern void rsci11_eri_isr (void *cb_args);
#endif

extern void rsci_init_register (rsci_hdl_t const hdl);

#if (RSCI_CFG_ASYNC_INCLUDED)
extern rsci_err_t rsci_async_cmds (rsci_hdl_t const hdl,
                                rsci_cmd_t const cmd,
                                void            *p_args);
#endif

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
extern rsci_err_t rsci_sync_cmds (rsci_hdl_t const hdl,
                                rsci_cmd_t const cmd,
                                void            *p_args);
#endif

#if (RSCI_CFG_MANC_INCLUDED)
extern rsci_err_t rsci_manc_cmds (rsci_hdl_t const hdl,
                                rsci_cmd_t const cmd,
                                void            *p_args);
#endif

extern rsci_err_t rsci_mcu_param_check (uint8_t const chan);

extern int32_t rsci_init_bit_rate (rsci_hdl_t const    hdl,
                                uint32_t const     pclk,
                                uint32_t const     baud);

extern void rsci_initialize_ints (rsci_hdl_t const hdl,
                                uint8_t const   priority);

extern void rsci_disable_ints (rsci_hdl_t const hdl);

#endif /* RSCI_RX26T_H */

