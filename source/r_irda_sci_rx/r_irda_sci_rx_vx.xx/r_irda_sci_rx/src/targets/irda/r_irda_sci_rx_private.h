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
* File Name    : r_irda_sci_rx_private.h
* Version      : 1.01
* Device       : RX
* Abstract     : include file for using irda fit module.
* Tool-Chain   : Renesas RXC Toolchain v2.02.00
* OS           : not use
* H/W Platform : RSKRX113
* Description  : Functions for using IRDA on RX devices. 
* Limitation   : none
*******************************************************************************/
#ifndef IRDA_SCI_PRIVATE_H
#define IRDA_SCI_PRIVATE_H

/*******************************************************************************
  Macro definitions
*******************************************************************************/
/*Constants to determine whether the parameter checking is enabled or disabled  */
#define IRDA_SCI_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE & IRDA_SCI_CFG_PARAM_CHECKING)

/*******************************************************************************
  Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>

/* Defines for IRDA support */
#include "r_irda_sci_rx_if.h"
#include "r_irda_sci_rx_config.h"

/* Include specifics for chosen MCU.  */
#include "r_irda_sci_rx_platform.h"

#include "platform.h"
#include "r_byteq_if.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
/* Bit position masks (do not change values) */
#define BIT0_MASK   (0x01U)
#define BIT1_MASK   (0x02U)
#define BIT2_MASK   (0x04U)
#define BIT3_MASK   (0x08U)
#define BIT4_MASK   (0x10U)
#define BIT5_MASK   (0x20U)
#define BIT6_MASK   (0x40U)
#define BIT7_MASK   (0x80U)
#define BIT8_MASK   (0x0100U)
#define BIT9_MASK   (0x0200U)
#define BIT10_MASK  (0x0400U)
#define BIT11_MASK  (0x0800U)
#define BIT12_MASK  (0x1000U)
#define BIT13_MASK  (0x2000U)
#define BIT14_MASK  (0x4000U)
#define BIT15_MASK  (0x8000U)
#define BIT16_MASK  (0x010000U)
#define BIT17_MASK  (0x020000U)
#define BIT18_MASK  (0x040000U)
#define BIT19_MASK  (0x080000U)
#define BIT20_MASK  (0x100000U)
#define BIT21_MASK  (0x200000U)
#define BIT22_MASK  (0x400000U)
#define BIT23_MASK  (0x800000U)
#define BIT24_MASK  (0x01000000U)
#define BIT25_MASK  (0x02000000U)
#define BIT26_MASK  (0x04000000U)
#define BIT27_MASK  (0x08000000U)
#define BIT28_MASK  (0x10000000U)
#define BIT29_MASK  (0x20000000U)
#define BIT30_MASK  (0x40000000U)
#define BIT31_MASK  (0x80000000U)

/*------- Definitions for selecting ports (do not change values)-------*/
#define PORT0_GR    (0)
#define PORT1_GR    (1)
#define PORT2_GR    (2)
#define PORT3_GR    (3)
#define PORT4_GR    (4)
#define PORT5_GR    (5)
#define PORT6_GR    (6)
#define PORT7_GR    (7)
#define PORT8_GR    (8)
#define PORT9_GR    (9)
#define PORTA_GR    (0xA)
#define PORTB_GR    (0xB)
#define PORTC_GR    (0xC)
#define PORTD_GR    (0xD)
#define PORTE_GR    (0xE)
#define PORTF_GR    (0xF)
#define PORTG_GR    (0x10)
#define PORTH_GR    (0x11)
#define PORTI_GR    (0x12)
#define PORTJ_GR    (0x13)
#define PORT_GR_MAX (0x14)

/* Constants to select settings(do not change values) */
/*(use power_setting,select_ints,irda_sci_io_mode_setting) */
#define TARGET_STOP    (0)
#define TARGET_SETTING (1)

/*******************************************************************************
  Typedef definitions
*******************************************************************************/
/* Channel control handle block */
typedef struct st_irda_sci_ch_ctrl
{
    irda_sci_ch_rom_t const *rom;                       /* Module stop registers and bits in the registers */
    irda_sci_ch_port_rom_t const *port_rom;             /* Port setting values for pins IRTXD and IRRXD */
    void                     (*callback)(void *p_args); /* Callback function pointer */
    byteq_hdl_t              u_tx_data;                 /* Transmit queue handle     */
    byteq_hdl_t              u_rx_data;                 /* Receive queue handle      */
    bool                     tx_idle;                   /* Transmit status           */
} irda_sci_ch_ctrl_t;

/* Information table for all SCI channels */
/* BRR = (PCLK/(divisor * baud)) - 1     */
/* when abcs=0, divisor = 64*pow(2,2n-1) */
typedef struct st_baud_divisor
{
    int16_t     divisor;    /* clock divisor                       */
    uint8_t     cks;        /* cks  value to get divisor (cks = n) */
} baud_divisor_t;

/* The value selected with the IrDA output pulse width select bit (IRCKS) */
typedef struct st_out_width_divisor
{
    uint8_t     w_denominator;  /* denominator of width */
    uint8_t     w_numerator;    /* numerator of width   */
} out_width_divisor_t;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*------- global functions -------*/
extern void           irda_power_setting (irda_sci_hdl_t const hdl, bool init_or_setting);
extern irda_sci_err_t irda_sci_open (uint8_t const           chan,
                                     irda_sci_t * const      p_cfg,
                                     void(* const p_callback)(void *p_args),
                                     irda_sci_hdl_t * const  p_hdl);
extern void           irda_sci_close(irda_sci_hdl_t const hdl);
extern void           irda_rxi_handler   (irda_sci_hdl_t const);
extern void           irda_eri_handler   (irda_sci_hdl_t const);
extern void           irda_txi_handler   (irda_sci_hdl_t const);
extern void           irda_tei_handler   (irda_sci_hdl_t const);


#endif /* IRDA_SCI_PRIVATE_H */
