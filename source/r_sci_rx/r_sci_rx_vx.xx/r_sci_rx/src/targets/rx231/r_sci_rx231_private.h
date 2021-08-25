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
/***********************************************************************************************************************
* File Name    : r_sci_rx231.h
* Description  : Functions for using SCI on the RX231 device.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           26.05.2016 1.00    Initial Release.
*           20.05.2019 3.00    Added support for GNUC and ICCRX.
*           25.08.2020 3.60    Added feature using DTC/DMAC in SCI transfer.
*                              Merged IrDA functionality to SCI FIT.
*           31.03.2021 3.80    Updated macro definition enable and disable TXI, RXI, ERI, TEI.
***********************************************************************************************************************/

#ifndef SCI_RX231_H
#define SCI_RX231_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "../../r_sci_rx_private.h"

#if (SCI_CFG_ASYNC_INCLUDED || SCI_CFG_IRDA_INCLUDED)
#include "r_byteq_if.h"
#endif

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/*------- Definitions for selecting ports (do not change values)-------*/
#define PORT0_GR    (0x00)
#define PORT1_GR    (0x01)
#define PORT2_GR    (0x02)
#define PORT3_GR    (0x03)
#define PORT4_GR    (0x04)
#define PORT5_GR    (0x05)
#define PORT6_GR    (0x06)
#define PORT7_GR    (0x07)
#define PORT8_GR    (0x08)
#define PORT9_GR    (0x09)
#define PORTA_GR    (0x0A)
#define PORTB_GR    (0x0B)
#define PORTC_GR    (0x0C)
#define PORTD_GR    (0x0D)
#define PORTE_GR    (0x0E)
#define PORTF_GR    (0x0F)
#define PORTG_GR    (0x10)
#define PORTH_GR    (0x11)
#define PORTI_GR    (0x12)
#define PORTJ_GR    (0x13)
#define PORT_GR_MAX (0x14)

/* Constants to select settings(do not change values) */
/*(sci_irda_io_mode_setting) */
#define TARGET_STOP    (0)
#define TARGET_SETTING (1)

/* Select the IRTXD pin to be used */
/* In each channel, port on the comments of the right can be selected.    */
/* Please set to a constant numeric value to the left of the port number. */
#define SCI_CFG_CH5_IRDA_IRTXD_SEL  (1)       /* 1:PC3, 2:PA4             */

/* Select the IRRXD pin to be used */
/* In each channel, port on the comments of the right can be selected.    */
/* Please set to a constant numeric value to the left of the port number. */
#define SCI_CFG_CH5_IRDA_IRRXD_SEL  (1)       /* 1:PC2, 2:PA3, 3:PA2      */

/* SCI channel include Check */
#if (SCI_CFG_CH2_INCLUDED  != 0) || (SCI_CFG_CH3_INCLUDED  != 0) || \
    (SCI_CFG_CH4_INCLUDED  != 0) || (SCI_CFG_CH7_INCLUDED  != 0) || \
    (SCI_CFG_CH10_INCLUDED != 0) || (SCI_CFG_CH11_INCLUDED != 0)
    #error "ERROR - Unsupported channel chosen in r_sci_config.h"
#endif

/* Mask of all active channels */
#define SCI_CFG_CH_INCLUDED_MASK ((SCI_CFG_CH0_INCLUDED << 0)   |   \
                                  (SCI_CFG_CH1_INCLUDED << 1)   |   \
                                  (SCI_CFG_CH5_INCLUDED << 5)   |   \
                                  (SCI_CFG_CH6_INCLUDED << 6)   |   \
                                  (SCI_CFG_CH8_INCLUDED << 8)   |   \
                                  (SCI_CFG_CH9_INCLUDED << 9)   |   \
                                  (SCI_CFG_CH12_INCLUDED << 12))

#if SCI_CFG_IRDA_INCLUDED

/* check of channel 5 */
#if (1 == SCI_CFG_CH5_IRDA_INCLUDED)

    /* IRTXD Pin check data */
    #if ((1 > SCI_CFG_CH5_IRDA_IRTXD_SEL) || (2 < SCI_CFG_CH5_IRDA_IRTXD_SEL))
        #error "ERROR - SCI_CFG_CH5_IRDA_IRTXD_SEL - Parameter error in configures file.  "
    #endif
    #if ((0x00 > SCI_CFG_CH5_IRDA_IRTXD_GP) || (0x14 < SCI_CFG_CH5_IRDA_IRTXD_GP))
        #error "ERROR - SCI_CFG_CH5_IRDA_IRTXD_GP - Parameter error in configures file.  "
    #endif
    #if ((0 > SCI_CFG_CH5_IRDA_IRTXD_PIN) || (7 < SCI_CFG_CH5_IRDA_IRTXD_PIN))
        #error "ERROR - SCI_CFG_CH5_IRDA_IRTXD_PIN - Parameter error in configures file.  "
    #endif

    /* IRRXD Pin check data */
    #if ((1 > SCI_CFG_CH5_IRDA_IRRXD_SEL) || (3 < SCI_CFG_CH5_IRDA_IRRXD_SEL))
        #error "ERROR - SCI_CFG_CH5_IRDA_IRRXD_SEL - Parameter error in configures file.  "
    #endif
    #if ((0x00 > SCI_CFG_CH5_IRDA_IRRXD_GP) || (0x14 < SCI_CFG_CH5_IRDA_IRRXD_GP))
        #error "ERROR - SCI_CFG_CH5_IRDA_IRRXD_GP - Parameter error in configures file.  "
    #endif
    #if ((0 > SCI_CFG_CH5_IRDA_IRRXD_PIN) || (7 < SCI_CFG_CH5_IRDA_IRRXD_PIN))
        #error "ERROR - SCI_CFG_CH5_IRDA_IRRXD_PIN - Parameter error in configures file.  "
    #endif

#elif (0 == SCI_CFG_CH5_IRDA_INCLUDED)
    /* do not check */
#else
    /* Select parameter NG */
    #error "ERROR - SCI_CFG_CH5_IRDA_INCLUDED - Parameter error in configures file.  "
#endif  /* SCI_IRDA_CFG_CH5_INCLUDED */

/* Sets to use multi-function pin controller. */
#if (1 == SCI_CFG_CH5_IRDA_INCLUDED)
#define SCI_IRDA_MPC_IRTXD5_ENABLE  ((uint8_t)(0x0A))   /* Pin function select to SCI5 IRTXD pin:b01010 */
#define SCI_IRDA_MPC_IRRXD5_ENABLE  ((uint8_t)(0x0A))   /* Pin function select to SCI5 IRRXD pin:b01010 */
#endif /* #ifdef IRDA_SCI_CFG_CH5_INCLUDED */

#define SCI_IRDA_MPC_IRTXD_DISABLE  ((uint8_t)(0x00))   /* Pin function select to PORT  pin:b00000 */
#define SCI_IRDA_MPC_IRRXD_DISABLE  ((uint8_t)(0x00))   /* Pin function select to PORT  pin:b00000 */

/* Common register setting */
#define SCI_IRDA_SMR_INIT           ((uint8_t)(0x00))   /* Initializes SMR.                    */

#define SCI_IRDA_SCR_INIT           ((uint8_t)(0x00))   /* Initializes SCR.                    */

#define SCI_IRDA_SCMR_INIT          ((uint8_t)(0xF2))   /* Initializes SCMR.                   */

#define SCI_IRDA_BRR_INIT           ((uint8_t)(0xFF))   /* Initializes BRR.                    */

#define SCI_IRDA_SEMR_INIT          ((uint8_t)(0x00))   /* Initializes SEMR.                   */

#define SCI_IRDA_SNFR_INIT          ((uint8_t)(0x00))   /* Initializes SNFR.                   */

#define SCI_IRDA_SIMR1_INIT         ((uint8_t)(0x00))   /* Initializes SIMR1.                  */

#define SCI_IRDA_SIMR2_INIT         ((uint8_t)(0x00))   /* Initializes SIMR2.                  */

#define SCI_IRDA_SIMR3_INIT         ((uint8_t)(0x00))   /* Initializes SIMR3.                  */

#define SCI_IRDA_SISR_INIT          ((uint8_t)(0x00))   /* Initializes SISR.                   */

#define SCI_IRDA_SPMR_INIT          ((uint8_t)(0x00))   /* Initializes SPMR.                   */

#define SCI_IRDA_IRCR_INIT          ((uint8_t)(0x00))   /* Initializes IRCR.                   */

/* Set the port information that can be selected */
#if (1 == SCI_CFG_CH5_IRDA_INCLUDED)

/* Select the ports to used by IRTXD of SCI5 */
#if (1 == SCI_CFG_CH5_IRDA_IRTXD_SEL)    /* PC3 */
#define SCI_CFG_CH5_IRDA_IRTXD_GP     (PORTC_GR)
#define SCI_CFG_CH5_IRDA_IRTXD_PIN    (3)

#elif (2 == SCI_CFG_CH5_IRDA_IRTXD_SEL)  /* PA4 */
#define SCI_CFG_CH5_IRDA_IRTXD_GP     (PORTA_GR)
#define SCI_CFG_CH5_IRDA_IRTXD_PIN    (4)

#else
    #error "ERROR - SCI_CFG_CH5_IRDA_IRTXD_SEL - Parameter error in configures file.  "

#endif  /* SCI_CFG_CH5_IRDA_IRTXD_SEL */

/* Select the ports to used by IRRXD of SCI5 */
#if (1 == SCI_CFG_CH5_IRDA_IRTXD_SEL)    /* PC2 */
#define SCI_CFG_CH5_IRDA_IRRXD_GP     (PORTC_GR)
#define SCI_CFG_CH5_IRDA_IRRXD_PIN    (2)

#elif (2 == SCI_CFG_CH5_IRDA_IRTXD_SEL)  /* PA3 */
#define SCI_CFG_CH5_IRDA_IRRXD_GP     (PORTA_GR)
#define SCI_CFG_CH5_IRDA_IRRXD_PIN    (3)

#elif (3 == SCI_CFG_CH5_IRDA_IRTXD_SEL)  /* PA2 */
#define SCI_CFG_CH5_IRDA_IRRXD_GP     (PORTA_GR)
#define SCI_CFG_CH5_IRDA_IRRXD_PIN    (2)

#else
    #error "ERROR - SCI_CFG_CH5_IRDA_IRTXD_SEL - Parameter error in configures file.  "

#endif  /* SCI_CFG_CH5_IRDA_IRTXD_SEL */

#endif  /* SCI_CFG_CH5_IRDA_INCLUDED */

#endif  /* SCI_IRDA_CFG_INCLUDED */

/* SCI SCR register masks */
#define SCI_SCR_TEI_MASK    (0x80U)     /* transmit interrupt enable */
#define SCI_SCR_REI_MASK    (0x40U)     /* receive interrupt enable */
#define SCI_SCR_TE_MASK     (0x20U)     /* transmitter enable */
#define SCI_SCR_RE_MASK     (0x10U)     /* receiver enable */
#define SCI_EN_XCVR_MASK    (SCI_SCR_RE_MASK | SCI_SCR_TE_MASK | SCI_SCR_REI_MASK | SCI_SCR_TEI_MASK)

/* SCI SSR register receiver error masks */
#define SCI_SSR_ORER_MASK         (0x20U)     /* overflow error */
#define SCI_SSR_FER_MASK          (0x10U)     /* framing error */
#define SCI_SSR_PER_MASK          (0x08U)     /* parity err */
#define SCI_RCVR_ERR_MASK         (SCI_SSR_ORER_MASK | SCI_SSR_FER_MASK | SCI_SSR_PER_MASK)
#define SCI_SSR_CLR_MASK          (0xC0U)     /* SSR register cleare mask (11000000b) */

/* Macros to enable and disable ICU interrupts */
#define ENABLE_RXI_INT      (R_BSP_BIT_SET(hdl->rom->icu_rxi, hdl->rom->rxi_bit_num))
#define DISABLE_RXI_INT     (R_BSP_BIT_CLEAR(hdl->rom->icu_rxi, hdl->rom->rxi_bit_num))
#define ENABLE_TXI_INT      (R_BSP_BIT_SET(hdl->rom->icu_txi, hdl->rom->txi_bit_num))
#define DISABLE_TXI_INT     (R_BSP_BIT_CLEAR(hdl->rom->icu_txi, hdl->rom->txi_bit_num))

#define ENABLE_ERI_INT      (R_BSP_BIT_SET(hdl->rom->icu_eri, hdl->rom->eri_bit_num))
#define DISABLE_ERI_INT     (R_BSP_BIT_CLEAR(hdl->rom->icu_eri, hdl->rom->eri_bit_num))
#define ENABLE_TEI_INT      (R_BSP_BIT_SET(hdl->rom->icu_tei, hdl->rom->tei_bit_num))
#define DISABLE_TEI_INT     (R_BSP_BIT_CLEAR(hdl->rom->icu_tei, hdl->rom->tei_bit_num))

/*****************************************************************************
Typedef definitions
******************************************************************************/

typedef struct st_scif_fifo_ctrl
{
    uint8_t     *p_tx_buf;            /* user's buffer */
    uint8_t     *p_rx_buf;            /* user's buffer */
    uint16_t    tx_cnt;             /* bytes remaining to add to FIFO */
    uint16_t    rx_cnt;             /* bytes waiting to receive from FIFO */
#if (TX_DTC_DMACA_ENABLE) || (RX_DTC_DMACA_ENABLE)
    uint8_t     *p_tx_fraction_buf;
    uint8_t     *p_rx_fraction_buf;
    uint16_t    tx_fraction;
    uint16_t    rx_fraction;
#endif
    uint16_t    total_length;       /* used for DTC in txi_handler */
} sci_fifo_ctrl_t;

/* CHANNEL CONTROL BLOCK */

/* ROM INFO */

typedef struct st_sci_ch_rom    /* SCI ROM info for channel control block */
{
    volatile  struct st_sci12 R_BSP_EVENACCESS_SFR  *regs;  /* base ptr to ch registers */
    volatile  uint32_t R_BSP_EVENACCESS_SFR *mstp;          /* ptr to mstp register */
    uint32_t                        stop_mask;      /* mstp mask to disable ch */
#if (SCI_CFG_IRDA_INCLUDED)
    volatile  struct st_irda R_BSP_EVENACCESS_SFR  *regs_irda;      /* base ptr to ch registers */
    volatile  uint32_t R_BSP_EVENACCESS_SFR        *mstp_irda;      /* ptr to mstp register     */
    uint32_t                        stop_mask_irda;                 /* mstp mask to disable ch  */
#endif
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ipr;           /* ptr to IPR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ir_rxi;        /* ptr to RXI IR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ir_txi;        /* ptr to TXI IR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ir_tei;        /* ptr to TEI IR register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *ir_eri;        /* ptr to ERI IR register */

    /* 
    * DO NOT use the enable/disable interrupt bits in the SCR 
    * register. Pending interrupts can be lost that way.
    */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *icu_rxi;       /* ptr to ICU register */
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *icu_txi;
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *icu_tei;
    volatile  uint8_t R_BSP_EVENACCESS_SFR  *icu_eri;
    uint8_t                         eri_bit_num;    /* ICU enable/disable eri bit number */
    uint8_t                         rxi_bit_num;    /* ICU enable/disable rxi bit number */
    uint8_t                         txi_bit_num;    /* ICU enable/disable txi bit number */
    uint8_t                         tei_bit_num;    /* ICU enable/disable tei bit number */

    /*
        * In case using DTC/DMAC
     */
#if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
    uint8_t                         dtc_dmaca_tx_enable;
    uint8_t                         dtc_dmaca_rx_enable;
#endif
#if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
    dtc_activation_source_t         dtc_tx_act_src;
    dtc_activation_source_t         dtc_rx_act_src;
#endif
#if ((TX_DTC_DMACA_ENABLE & 0x02) || (RX_DTC_DMACA_ENABLE & 0x02))
    dmaca_activation_source_t       dmaca_tx_act_src;
    dmaca_activation_source_t       dmaca_rx_act_src;
    uint8_t                         dmaca_tx_channel;
    uint8_t                         dmaca_rx_channel;
    uint8_t                         chan;
#endif
} sci_ch_rom_t;

#if (SCI_CFG_IRDA_INCLUDED)
typedef struct st_sci_irda_ch_port_rom  /* Port setting values for pins IRTXD and IRRXD */
{
    uint8_t     irtxd_port_gr;                              /* 0: Port group of IRTXD pin */
    uint8_t     irtxd_port_pin;                             /* 1: Port bit of IRTXD pin */
    uint8_t     irtxd_pdr_val;                              /* 2: Pin direction control (IN/OUT) when not */

                                                            /*    selecting the IRTXD pin */
    uint8_t     irtxd_inactive_level;                       /* 3: Pin level when  not selecting the IRTXD pin */
    uint8_t     irtxd_mpc_setting;                          /* 4: MPC setting data of IRTXD pin */

    uint8_t     irrxd_port_gr;                              /* 0: Port group of IRRXD pin */
    uint8_t     irrxd_port_pin;                             /* 1: Port bit of IRRXD pin */
    uint8_t     irrxd_pdr_val;                              /* 2: Pin direction control (IN/OUT) when not */

                                                            /*    selecting the IRRXD pin */
    uint8_t     irrxd_inactive_level;                       /* 3: Pin level when  not selecting the IRRXD pin */
    uint8_t     irrxd_mpc_setting;                          /* 4: MPC setting data of IRRXD pin */
}sci_irda_ch_port_rom_t;
#endif

/* CHANNEL CONTROL BLOCK */

typedef struct st_sci_ch_ctrl       /* SCI channel control (for handle) */
{
    sci_ch_rom_t const *rom;        /* pointer to rom info */
#if (SCI_CFG_IRDA_INCLUDED)
    sci_irda_ch_port_rom_t const *port_rom;             /* Port setting values for pins IRTXD and IRRXD */
#endif
    sci_mode_t      mode;           /* operational mode */
    uint32_t        baud_rate;      /* baud rate */
    void          (*callback)(void *p_args); /* function ptr for rcvr errs */
    union
    {
#if (SCI_CFG_ASYNC_INCLUDED || SCI_CFG_IRDA_INCLUDED)
        byteq_hdl_t     que;        /* async transmit queue handle */
#endif
        uint8_t         *buf;       /* sspi/sync tx buffer ptr */
    } u_tx_data;
    union
    {
#if (SCI_CFG_ASYNC_INCLUDED || SCI_CFG_IRDA_INCLUDED)
        byteq_hdl_t     que;        /* async receive queue handle */
#endif
        uint8_t         *buf;       /* sspi/sync rx buffer ptr */
    } u_rx_data;
    bool            tx_idle;        /* TDR is empty (async); TSR is empty (sync/sspi) */
#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
    bool            save_rx_data;   /* save the data that is clocked in */
    uint16_t        tx_cnt;         /* number of bytes to transmit */
    uint16_t        rx_cnt;         /* number of bytes to receive */
    bool            tx_dummy;       /* transmit dummy byte, not buffer */
#endif
    uint32_t        pclk_speed;     /* saved peripheral clock speed for break generation */
#if ((TX_DTC_DMACA_ENABLE || RX_DTC_DMACA_ENABLE))
    uint8_t                         qindex_app_tx;
    uint8_t                         qindex_int_tx;
    uint8_t                         qindex_app_rx;
    uint8_t                         qindex_int_rx;
    sci_fifo_ctrl_t                queue[2];
#endif
} sci_ch_ctrl_t;


/* BAUD DIVISOR INFO */

/* BRR = (PCLK/(divisor * baud)) - 1 */
/* when: bgdm,abcs=11, divisor = 16*pow(2,2n-1) */
/*       bgdm,abcs=01, divisor = 32*pow(2,2n-1) */
/*       bgdm,abcs=10, divisor = 32*pow(2,2n-1) */
/*       bgdm,abcs=00, divisor = 64*pow(2,2n-1) */

typedef struct st_baud_divisor
{
    int16_t     divisor;    // clock divisor
    uint8_t     abcs;       // abcs value to get divisor
    uint8_t     bgdm;       // bdgm value to get divisor
    uint8_t     cks;        // cks  value to get divisor (cks = n)
} baud_divisor_t;

/* The value selected with the IrDA output pulse width select bit (IRCKS) */
typedef struct st_out_width_divisor
{
    uint8_t     w_denominator;  /* denominator of width */
    uint8_t     w_numerator;    /* numerator of width   */
} out_width_divisor_t;

#define NUM_DIVISORS_ASYNC        (9)
#define NUM_DIVISORS_SYNC         (4)
#define NUM_DIVISORS_IRDA         (4)
#define NUM_WIDTH_DIVISORS_IRDA   (8)


/*****************************************************************************
Private global variables and functions
******************************************************************************/
#if (SCI_CFG_ASYNC_INCLUDED)
extern const baud_divisor_t async_baud[];
#endif
#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
extern const baud_divisor_t sync_baud[];
#endif
#if (SCI_CFG_IRDA_INCLUDED)
extern const baud_divisor_t irda_async_baud[];
extern const out_width_divisor_t irda_ircks_div[];
#endif

#if (SCI_CFG_CH0_INCLUDED)
extern const sci_ch_rom_t      ch0_rom;
extern sci_ch_ctrl_t           ch0_ctrl;
#endif

#if (SCI_CFG_CH1_INCLUDED)
extern const sci_ch_rom_t      ch1_rom;
extern sci_ch_ctrl_t           ch1_ctrl;
#endif

#if (SCI_CFG_CH5_INCLUDED)
extern const sci_ch_rom_t      ch5_rom;
#if (SCI_CFG_CH5_IRDA_INCLUDED)
#if (SCI_CFG_IRDA_INCLUDED)
extern const sci_irda_ch_port_rom_t   g_sci_irda_ch5_portrom;
#endif
#endif
extern sci_ch_ctrl_t           ch5_ctrl;
#endif

#if (SCI_CFG_CH6_INCLUDED)
extern const sci_ch_rom_t      ch6_rom;
extern sci_ch_ctrl_t           ch6_ctrl;
#endif

#if (SCI_CFG_CH8_INCLUDED)
extern const sci_ch_rom_t      ch8_rom;
extern sci_ch_ctrl_t           ch8_ctrl;
#endif

#if (SCI_CFG_CH9_INCLUDED)
extern const sci_ch_rom_t      ch9_rom;
extern sci_ch_ctrl_t           ch9_ctrl;
#endif

#if (SCI_CFG_CH12_INCLUDED)
extern const sci_ch_rom_t      ch12_rom;
extern sci_ch_ctrl_t           ch12_ctrl;
#endif

extern const sci_hdl_t g_sci_handles[];

extern const sci_hdl_t g_handles[];

extern void sci_init_register(sci_hdl_t const hdl);

#if (SCI_CFG_ASYNC_INCLUDED)
extern sci_err_t sci_async_cmds(sci_hdl_t const hdl,
                         sci_cmd_t const cmd,
                         void            *p_args);
#endif

#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
extern sci_err_t sci_sync_cmds(sci_hdl_t const hdl,
                        sci_cmd_t const cmd,
                        void            *p_args);
#endif

extern sci_err_t sci_mcu_param_check(uint8_t const chan);

extern int32_t sci_init_bit_rate(sci_hdl_t const    hdl,
                          uint32_t const     pclk,
                          uint32_t const     baud);

extern void sci_initialize_ints(sci_hdl_t const hdl,
                                uint8_t const   priority);

extern void sci_disable_ints(sci_hdl_t const hdl);

#if (SCI_CFG_IRDA_INCLUDED)
extern sci_err_t sci_irda_cmds(sci_hdl_t const hdl,
                               sci_cmd_t const cmd,
                               void            *p_args);
extern sci_err_t sci_irda_open(uint8_t const      chan,
                               sci_irda_t * const p_cfg,
                               uint8_t * const      p_priority,
                               sci_hdl_t  const   hdl);
extern void sci_irda_close(sci_hdl_t const hdl);

#endif

#endif /* SCI_RX231_H */

