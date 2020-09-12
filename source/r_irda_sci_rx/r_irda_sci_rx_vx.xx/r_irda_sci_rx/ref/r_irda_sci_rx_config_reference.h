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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 27.01.2015 1.01     Chage symbol name of Arguments
*                                    None change proceedures.
*******************************************************************************/
/*******************************************************************************
* System Name  : interface file for irda fit module.
* File Name    : r_irda_sci_rx_config.h
* Version      : 1.01
* Device       : RX
* Abstract     : include file for using irda fit module.
* Tool-Chain   : Renesas RXC Toolchain v2.02.00
* OS           : not use
* H/W Platform : RSKRX113
* Description  : Configures the IrDA driver. 
* Limitation   : none
*******************************************************************************/
#ifndef IRDA_SCI_CONFIG_H
#define IRDA_SCI_CONFIG_H

/*******************************************************************************
  Configuration Options
*******************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define IRDA_SCI_CFG_PARAM_CHECKING  (1)

/* SPECIFY CHANNELS TO INCLUDE SOFTWARE SUPPORT FOR 1=included, 0=not */
                                               /* mcu supported channels  */
#define IRDA_SCI_CFG_CH0_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH1_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH2_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH3_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH4_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH5_INCLUDED    (1)       /* RX113                   */
#define IRDA_SCI_CFG_CH6_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH7_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH8_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH9_INCLUDED    (0)       /*                         */
#define IRDA_SCI_CFG_CH10_INCLUDED   (0)       /*                         */
#define IRDA_SCI_CFG_CH11_INCLUDED   (0)       /*                         */
#define IRDA_SCI_CFG_CH12_INCLUDED   (0)       /*                         */

/* Select the IRTXD pin to be used */
/* In each channel, port on the comments of the right can be selected.    */
/* Please set to a constant numeric value to the left of the port number. */
#define IRDA_SCI_CFG_CH5_IRTXD_SEL  (1)       /* 1:PC3, 2:PA4             */

/* Set the non-active level of the TXD pin */
/* 1=High , 0=Low */
#define IRDA_SCI_CFG_CH5_IRTXD_INACTIVE_LEVEL (0)

/* Select the IRRXD pin to be used */
/* In each channel, port on the comments of the right can be selected.    */
/* Please set to a constant numeric value to the left of the port number. */
#define IRDA_SCI_CFG_CH5_IRRXD_SEL  (1)       /* 1:PC2, 2:PA3, 3:PA2      */

/* Set the non-active level of the RXD pin */
/* 1=High , 0=Low */
#define IRDA_SCI_CFG_CH5_IRRXD_INACTIVE_LEVEL (1)

/* SPECIFY IRDA TX QUEUE BUFFER SIZES (will not allocate if channel not enabled) */
#define IRDA_SCI_CFG_CH5_TX_BUFSIZ   (80)

/* SPECIFY IRDA RX QUEUE BUFFER SIZES (will not allocate if channel not enabled) */
#define IRDA_SCI_CFG_CH5_RX_BUFSIZ   (80)

#endif /* IRDA_SCI_CONFIG_H */
