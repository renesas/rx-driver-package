/************************************************************************************************
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
* Copyright (C) 2004(2005-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_eeprom_spi_config.h
* Version      : 3.00
* Description  : EEPROM driver configuration header file
*************************************************************************************************/
/************************************************************************************************
*              : DD.MM.YYYY Version  Description
* History      : 28.11.2014 2.30     Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31     Added RX71M.
*              : 21.12.2018 3.00     Change eeprom drive interface to Memory Access Driver Interface
*************************************************************************************************/
#ifndef __EEPROM_SPI_CONFIG_H__
#define __EEPROM_SPI_CONFIG_H__

/*###############################################################################################
!!Attention!!
The following definitions are necessary for this software.
Set the definitions to use environment.

(1) Common setting
(2) Control Device
################################################################################################*/

/*===============================================================================================
(1) Common setting
================================================================================================*/
/************************************************************************************************
ENABLE CHECKING OF THE WEL BIT.
*************************************************************************************************/
/* Define the check of WEL bit after WREN command issue. */
#define EEPROM_SPI_CFG_WEL_CHK /* SC_EEPROM_SPI_CFG_WEL_CHK */

/************************************************************************************************
ENABLE DEBUGGING INFORMATION
*************************************************************************************************/
/* Define using Firmware Integration Technology (FIT) for RX family MCU,
   Stores error log information using LONGQ driver. */
/* #define EEPROM_SPI_CFG_LONGQ_ENABLE */ /* SC_EEPROM_SPI_CFG_LONGQ_ENABLE */

/************************************************************************************************
ENABLE GPIO MODULE AND MPC MODULE OF FIT FOR RX FAMILY MCU
*************************************************************************************************/
/* When perform terminal setting with GPIO and MPC module of FIT, please validate the definition. */
/* #define EEPROM_SPI_CFG_USE_GPIO_MPC_FIT */ /* SC_EEPROM_SPI_CFG_USE_GPIO_MPC_FIT */

/*===============================================================================================
(2) Setting of depending on Device No.
================================================================================================*/
/************************************************************************************************
SPECIFY DEVICES TO INCLUDE SOFTWARE SUPPORT
*************************************************************************************************/
/* If these are defined, then the code for the specified device is valid.
   If the #define which device is not supported on the code is uncommented,
   then the compile error occurs. */
#define EEPROM_SPI_CFG_DEV0_INCLUDED /* SC_EEPROM_SPI_CFG_DEV0_INCLUDED */
/* #define EEPROM_SPI_CFG_DEV1_INCLUDED */ /* SC_EEPROM_SPI_CFG_DEV1_INCLUDED */

/************************************************************************************************
SELECT THE SERIAL EEPROM DEVICE
002K :   2K-bit  (256 Bytes)
004K :   4K-bit  (512 Bytes)
008K :   8K-bit  ( 1K Bytes)
016K :  16K-bit  ( 2K Bytes)
032K :  32K-bit  ( 4K Bytes)
064K :  64K-bit  ( 8K Bytes)
128K : 128K-bit  (16K Bytes)
256K : 256K-bit  (32K Bytes)
512K : 512K-bit  (64K Bytes)
*************************************************************************************************/

/* #define EEPROM_SPI_CFG_DEV0_SIZE_004K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_004K */
#define EEPROM_SPI_CFG_DEV0_SIZE_008K        /* SC_EEPROM_SPI_CFG_DEV0_SIZE_008K */
/* #define EEPROM_SPI_CFG_DEV0_SIZE_016K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_016K */
/* #define EEPROM_SPI_CFG_DEV0_SIZE_032K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_032K */
/* #define EEPROM_SPI_CFG_DEV0_SIZE_064K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_064K */
/* #define EEPROM_SPI_CFG_DEV0_SIZE_128K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_128K */
/* #define EEPROM_SPI_CFG_DEV0_SIZE_256K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_256K */
/* #define EEPROM_SPI_CFG_DEV0_SIZE_512K */  /* SC_EEPROM_SPI_CFG_DEV0_SIZE_512K */

/* #define EEPROM_SPI_CFG_DEV1_SIZE_002K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_002K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_004K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_004K */ 
#define EEPROM_SPI_CFG_DEV1_SIZE_008K       /* SC_EEPROM_SPI_CFG_DEV1_SIZE_008K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_016K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_016K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_032K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_032K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_064K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_064K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_128K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_128K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_256K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_256K */ 
/* #define EEPROM_SPI_CFG_DEV1_SIZE_512K */ /* SC_EEPROM_SPI_CFG_DEV1_SIZE_512K */ 

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_eeprom_spi_pin_config.h"

#ifdef EEPROM_SPI_CFG_LONGQ_ENABLE
#include "r_longq_if.h"
#endif

#ifdef EEPROM_SPI_CFG_USE_GPIO_MPC_FIT
#include "r_gpio_rx_if.h"
#include "r_mpc_rx_if.h"
#endif

#endif /* __EEPROM_SPI_CONFIG_H__ */

/* End of File */
