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
* Copyright (C) 2004(2005-2015) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_eeprom_spi_pin_config.h
* Version      : 2.32
* Description  : EEPROM driver configuration header file
*************************************************************************************************/
/************************************************************************************************
*              : DD.MM.YYYY Version  Description
* History      : 28.11.2014 2.30     Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31     Added RX71M.
*              : 29.05.2015 2.32     Added RX231 and RX230.
*************************************************************************************************/
#ifndef __EEPROM_SPI_PIN_CONFIG_H__
#define __EEPROM_SPI_PIN_CONFIG_H__

/************************************************************************************************
PIN ASSIGNMENT
*************************************************************************************************/
/* The #defines specify the ports used for SS#. */
#define EEPROM_SPI_CS_DEV0_CFG_PORTNO   'C'     /* Device 0 Port Number : EEPROM SS#    */
#define EEPROM_SPI_CS_DEV0_CFG_BITNO    '0'     /* Device 0 Bit Number  : EEPROM SS#    */
#define EEPROM_SPI_CS_DEV1_CFG_PORTNO   'C'     /* Device 1 Port Number : EEPROM SS#    */
#define EEPROM_SPI_CS_DEV1_CFG_BITNO    '0'     /* Device 1 Bit Number  : EEPROM SS#    */


#endif /* __EEPROM_SPI_PIN_CONFIG_H__ */

/* End of File */
