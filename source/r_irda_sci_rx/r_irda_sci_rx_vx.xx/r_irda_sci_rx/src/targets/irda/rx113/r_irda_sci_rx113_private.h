/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 27.01.2015 1.01     Chage symbol name of Arguments 
*                                    None change proceedures.
*******************************************************************************/
/*******************************************************************************
* System Name  : interface file for irda fit module.
* File Name    : r_irda_sci_rx113_private.h
* Version      : 1.01
* Device       : RX
* Abstract     : include file for using irda fit module.
* Tool-Chain   : Renesas RXC Toolchain v2.01.00
* OS           : not use
* H/W Platform : RSKRX113
* Description  : Functions for using SCI on RX devices. 
* Limitation   : none
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>

/* Include specifics for chosen MCU. */
#include "./../r_irda_sci_rx_private.h"

#ifdef BSP_MCU_RX113

#ifndef IRDA_SCI_RX113_PRIVATE_H

/*******************************************************************************
  Macro definitions
*******************************************************************************/
#define IRDA_SCI_RX113_PRIVATE_H

/* SCI SCR register masks */
#define IRDA_SCI_SCR_TEI_MASK    (0x80U)     // transmit interrupt enable
#define IRDA_SCI_SCR_REI_MASK    (0x40U)     // receive interrupt enable
#define IRDA_SCI_SCR_TE_MASK     (0x20U)     // transmitter enable
#define IRDA_SCI_SCR_RE_MASK     (0x10U)     // receiver enable
#define IRDA_SCI_EN_XCVR_MASK    (IRDA_SCI_SCR_RE_MASK | IRDA_SCI_SCR_TE_MASK)

/* SCI SSR register receiver error masks */
#define IRDA_SCI_SSR_ORER_MASK   (0x20U)     // overflow error
#define IRDA_SCI_SSR_FER_MASK    (0x10U)     // framing error
#define IRDA_SCI_SSR_PER_MASK    (0x08U)     // parity err
#define IRDA_SCI_RCVR_ERR_MASK   (IRDA_SCI_SSR_ORER_MASK | IRDA_SCI_SSR_FER_MASK | IRDA_SCI_SSR_PER_MASK)

/*------------------------------------------------------------------------------
  Parameter check of Configuration Options
------------------------------------------------------------------------------*/
/* Check the constant value of the parameter check */
#if ((0 != IRDA_SCI_CFG_PARAM_CHECKING_ENABLE) && (1 != IRDA_SCI_CFG_PARAM_CHECKING_ENABLE))
    #error "ERROR - IRDA_SCI_CFG_PARAM_CHECKING_ENABLE - Parameter error in configures file.  "
#endif

/* Parameter check is valid when */
#if IRDA_SCI_CFG_PARAM_CHECKING_ENABLE

/* check of channel 5 */
#if (1 == IRDA_SCI_CFG_CH5_INCLUDED)

    /* IRTXD Pin check data */
    #if ((1 > IRDA_SCI_CFG_CH5_IRTXD_SEL) || (2 < IRDA_SCI_CFG_CH5_IRTXD_SEL))
        #error "ERROR - IRDA_SCI_CFG_CH5_IRTXD_SEL - Parameter error in configures file.  "
    #endif

    #if (1 < IRDA_SCI_CFG_CH5_IRTXD_INACTIVE_LEVEL)
        #error "ERROR - IRDA_SCI_CFG_CH5_IRTXD_INACTIVE_LEVEL - Parameter error in configures file.  "
    #endif

    #if ((0x00 > IRDA_SCI_CFG_CH5_IRTXD_GP) || (0x14 < IRDA_SCI_CFG_CH5_IRTXD_GP))
        #error "ERROR - IRDA_SCI_CFG_CH5_IRTXD_GP - Parameter error in configures file.  "
    #endif
    #if ((0 > IRDA_SCI_CFG_CH5_IRTXD_PIN) || (7 < IRDA_SCI_CFG_CH5_IRTXD_PIN))
        #error "ERROR - IRDA_SCI_CFG_CH5_IRTXD_PIN - Parameter error in configures file.  "
    #endif

    /* IRRXD Pin check data */
    #if ((1 > IRDA_SCI_CFG_CH5_IRRXD_SEL) || (3 < IRDA_SCI_CFG_CH5_IRRXD_SEL))
        #error "ERROR - IRDA_SCI_CFG_CH5_IRRXD_SEL - Parameter error in configures file.  "
    #endif

    #if (1 < IRDA_SCI_CFG_CH5_IRRXD_INACTIVE_LEVEL)
        #error "ERROR - IRDA_SCI_CFG_CH5_IRRXD_INACTIVE_LEVEL - Parameter error in configures file.  "
    #endif

    #if ((0x00 > IRDA_SCI_CFG_CH5_IRRXD_GP) || (0x14 < IRDA_SCI_CFG_CH5_IRRXD_GP))
        #error "ERROR - IRDA_SCI_CFG_CH5_IRRXD_GP - Parameter error in configures file.  "
    #endif
    #if ((0 > IRDA_SCI_CFG_CH5_IRRXD_PIN) || (7 < IRDA_SCI_CFG_CH5_IRRXD_PIN))
        #error "ERROR - IRDA_SCI_CFG_CH5_IRRXD_PIN - Parameter error in configures file.  "
    #endif

#elif (0 == IRDA_SCI_CFG_CH5_INCLUDED)
    /* do not check */
#else
    /* Select parameter NG */
    #error "ERROR - IRDA_SCI_CFG_CH5_INCLUDED - Parameter error in configures file.  "
#endif  /* IRDA_SCI_CFG_CH5_INCLUDED */

#endif  /* IRDA_SCI_CFG_PARAM_CHECKING_ENABLE */

/* Sets to use multi-function pin controller. */
#if (1 == IRDA_SCI_CFG_CH5_INCLUDED)
#define IRDA_SCI_MPC_IRTXD5_ENABLE  ((uint8_t)(0x0A))   /* Pin function select to SCI5 IRTXD pin:b01010 */
#define IRDA_SCI_MPC_IRRXD5_ENABLE  ((uint8_t)(0x0A))   /* Pin function select to SCI5 IRRXD pin:b01010 */
#endif /* #ifdef IRDA_SCI_CFG_CH5_INCLUDED */

#define IRDA_SCI_MPC_IRTXD_DISABLE  ((uint8_t)(0x00))   /* Pin function select to PORT  pin:b00000 */
#define IRDA_SCI_MPC_IRRXD_DISABLE  ((uint8_t)(0x00))   /* Pin function select to PORT  pin:b00000 */

/* Interrupt register setting */
#define IRDA_SCI_IR_CLR             ((uint8_t)(0x00))   /* Clears interrupt request register.  */
#define IRDA_SCI_IR_SET             ((uint8_t)(0x01))   /* Sets interrupt request register.    */

/* Common register setting */
#define IRDA_SCI_SMR_INIT           ((uint8_t)(0x00))   /* Initializes SMR.                    */

#define IRDA_SCI_SCR_INIT           ((uint8_t)(0x00))   /* Initializes SCR.                    */
#define IRDA_SCI_SCR_ENABLE         ((uint8_t)(0xB4))   /* Enable bit:TEIE,TIE,TE,RE bit       */
#define IRDA_SCI_SCR_TE_RE          ((uint8_t)(0x30))   /* Enable bit:TE,RE bit                */

#define IRDA_SCI_SCMR_INIT          ((uint8_t)(0xF2))   /* Initializes SCMR.                   */

#define IRDA_SCI_BRR_INIT           ((uint8_t)(0xFF))   /* Initializes BRR.                    */

#define IRDA_SCI_SEMR_INIT          ((uint8_t)(0x00))   /* Initializes SEMR.                   */

#define IRDA_SCI_SNFR_INIT          ((uint8_t)(0x00))   /* Initializes SNFR.                   */

#define IRDA_SCI_SIMR1_INIT         ((uint8_t)(0x00))   /* Initializes SIMR1.                  */

#define IRDA_SCI_SIMR2_INIT         ((uint8_t)(0x00))   /* Initializes SIMR2.                  */

#define IRDA_SCI_SIMR3_INIT         ((uint8_t)(0x00))   /* Initializes SIMR3.                  */

#define IRDA_SCI_SISR_INIT          ((uint8_t)(0x00))   /* Initializes SISR.                   */

#define IRDA_SCI_SPMR_INIT          ((uint8_t)(0x00))   /* Initializes SPMR.                   */

#define IRDA_SCI_IRCR_INIT          ((uint8_t)(0x00))   /* Initializes IRCR.                   */

/* Set the port information that can be selected */
#if (1 == IRDA_SCI_CFG_CH5_INCLUDED)

/* Select the ports to used by IRTXD of SCI5 */
#if (1 == IRDA_SCI_CFG_CH5_IRTXD_SEL)    /* PC3 */
#define IRDA_SCI_CFG_CH5_IRTXD_GP     (PORTC_GR)
#define IRDA_SCI_CFG_CH5_IRTXD_PIN    (3)

#elif (2 == IRDA_SCI_CFG_CH5_IRTXD_SEL)  /* PA4 */
#define IRDA_SCI_CFG_CH5_IRTXD_GP     (PORTA_GR)
#define IRDA_SCI_CFG_CH5_IRTXD_PIN    (4)

#else
    #error "ERROR - IRDA_SCI_CFG_CH5_IRTXD_SEL - Parameter error in configures file.  "

#endif  /* IRDA_SCI_CFG_CH5_IRTXD_SEL */

/* Select the ports to used by IRRXD of SCI5 */
#if (1 == IRDA_SCI_CFG_CH5_IRTXD_SEL)    /* PC2 */
#define IRDA_SCI_CFG_CH5_IRRXD_GP     (PORTC_GR)
#define IRDA_SCI_CFG_CH5_IRRXD_PIN    (2)

#elif (2 == IRDA_SCI_CFG_CH5_IRTXD_SEL)  /* PA3 */
#define IRDA_SCI_CFG_CH5_IRRXD_GP     (PORTA_GR)
#define IRDA_SCI_CFG_CH5_IRRXD_PIN    (3)

#elif (3 == IRDA_SCI_CFG_CH5_IRTXD_SEL)  /* PA2 */
#define IRDA_SCI_CFG_CH5_IRRXD_GP     (PORTA_GR)
#define IRDA_SCI_CFG_CH5_IRRXD_PIN    (2)

#else
    #error "ERROR - IRDA_SCI_CFG_CH5_IRTXD_SEL - Parameter error in configures file.  "

#endif  /* IRDA_SCI_CFG_CH5_IRTXD_SEL */

#endif  /* IRDA_SCI_CFG_CH5_INCLUDED */

/*****************************************************************************
  Typedef definitions
******************************************************************************/
/* ROM INFO */
typedef struct st_irda_sci_ch_rom    /* Data and information for each channel */
{
    /* hardware locks. SCI, IrDA */
    mcu_lock_t                              hd_lock;        /* Hardware Lock number     */
    mcu_lock_t                              hd_lock_irda;

    /* Register struct. SCI, IrDA */
    volatile  __evenaccess struct st_sci12 *regs;           /* base ptr to ch registers */
    volatile  __evenaccess struct st_irda  *regs_irda;      /* base ptr to ch registers */

    /* Module stop Register and bit */
    volatile  __evenaccess uint32_t        *mstp;           /* ptr to mstp register     */
    volatile  __evenaccess uint32_t        *mstp_irda;      /* ptr to mstp register     */
    uint32_t                                stop_mask;      /* mstp mask to disable ch  */
    uint32_t                                stop_mask_irda; /* mstp mask to disable ch  */

    /* Interrupt Register */
    volatile  __evenaccess uint8_t         *ipr;            /* ptr to IPR register       */
    /* catin : The following data, Please be placed in the same size(32bit).         */
    /*         IR, IER, IER bit shall be arranged in the order RXI, TXI, TEI and ERI */
    volatile  __evenaccess uint8_t         *ir_rxi;         /* ptr to RXI IR register    */
    volatile  __evenaccess uint8_t         *ir_txi;         /* ptr to TXI IR register    */
    volatile  __evenaccess uint8_t         *ir_tei;         /* ptr to TEI IR register    */
    volatile  __evenaccess uint8_t         *ir_eri;         /* ptr to ERI IR register    */
    volatile  __evenaccess uint8_t         *icu_rxi;        /* ptr to ICU register (RXI) */
    volatile  __evenaccess uint8_t         *icu_txi;        /* ptr to ICU register (TXI) */
    volatile  __evenaccess uint8_t         *icu_tei;        /* ptr to ICU register (TEI) */
    volatile  __evenaccess uint8_t         *icu_eri;        /* ptr to ICU register (ERI) */
    uint32_t                                rxi_en_mask;    /* bit mask to enable/disable rxi INT in ICU */
    uint32_t                                txi_en_mask;    /* bit mask to enable/disable txi INT in ICU */
    uint32_t                                tei_en_mask;    /* bit mask to enable/disable tei INT in ICU */
    uint32_t                                eri_en_mask;    /* bit mask to enable/disable eri INT in ICU */
} irda_sci_ch_rom_t;

typedef struct st_irda_sci_ch_port_rom  /* Port setting values for pins IRTXD and IRRXD */
{
    uint8_t     irtxd_port_gr;                              /* 0: Port group of IRTXD pin */
    uint8_t     irtxd_port_pin;                             /* 1: Port bit of IRTXD pin */
    uint8_t     irtxd_pdr_val;                              /* 2: Pin direction control (IN/OUT) when not */
                                                            /*    selecting the IRTXD pin */
    uint8_t     irtxd_inactive_level;                       /* 3: Pin level when  not selecting the IRTXD pin */
    uint8_t     irtxd_mpc_setting;                          /* 4: MPC setting data of IRTXD pin */

    uint8_t     irrxd_port_gr;                              /* 0:  Port group of IRRXD pin */
    uint8_t     irrxd_port_pin;                             /* 1: Port bit of IRRXD pin */
    uint8_t     irrxd_pdr_val;                              /* 2: Pin direction control (IN/OUT) when not */
                                                            /*    selecting the IRRXD pin */
    uint8_t     irrxd_inactive_level;                       /* 3: Pin level when  not selecting the IRRXD pin */
    uint8_t     irrxd_mpc_setting;                          /* 4: MPC setting data of IRRXD pin */
}irda_sci_ch_port_rom_t;

/*****************************************************************************
  Global variables and functions
******************************************************************************/
/*------- global variables -------*/
extern const irda_sci_ch_rom_t  irda_ch5_rom;
extern const irda_sci_hdl_t g_irda_handles[];

#endif /* IRDA_SCI_RX113_PRIVATE_H */

#endif /* BSP_MCU_RX113 */

/* End of File */
