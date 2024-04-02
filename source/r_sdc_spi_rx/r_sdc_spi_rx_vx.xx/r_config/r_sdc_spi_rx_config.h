/* Generated configuration header file - do not edit */
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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_sdc_spi_rx_config.h
 * Version      : 1.0
 * Description  : Configures the SPI mode SDC card driver.
  *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/
#ifndef SDC_SPI_CONFIG_H
#define SDC_SPI_CONFIG_H

#define SDC_SPI_CFG_CH0_CD_ENABLE           (1)     /* CH0 CD pin use select */
#define SDC_SPI_CFG_CH0_CD_PORT             (0x00)  /* CH0 CD pin port number */
#define SDC_SPI_CFG_CH0_CD_BIT              (0)     /* CH0 CD pin port bit number */
#define SDC_SPI_CFG_CH0_CS_ENABLE           (1)     /* CH0 CS pin use select */
#define SDC_SPI_CFG_CH0_CS_PORT             (0x00)  /* CH0 CS pin port number */
#define SDC_SPI_CFG_CH0_CS_BIT              (0)     /* CH0 CS pin port bit number */
#define SDC_SPI_CFG_CH0_WP_ENABLE           (1)     /* CH0 WP pin use select */
#define SDC_SPI_CFG_CH0_WP_PORT             (0x00)  /* CH0 WP pin port number */
#define SDC_SPI_CFG_CH0_WP_BIT              (0)     /* CH0 WP pin port bit number */
#define SDC_SPI_CFG_CH1_CD_ENABLE           (1)     /* CH1 CD pin use select */
#define SDC_SPI_CFG_CH1_CD_PORT             (0x00)  /* CH1 CD pin port number */
#define SDC_SPI_CFG_CH1_CD_BIT              (0)     /* CH1 CD pin port bit number */
#define SDC_SPI_CFG_CH1_CS_ENABLE           (1)     /* CH1 CS pin use select */
#define SDC_SPI_CFG_CH1_CS_PORT             (0x00)  /* CH1 CS pin port bit number */
#define SDC_SPI_CFG_CH1_CS_BIT              (0)     /* CH1 CS pin port bit number */
#define SDC_SPI_CFG_CH1_WP_ENABLE           (1)     /* CH1 WP pin use select */
#define SDC_SPI_CFG_CH1_WP_PORT             (0x00)  /* CH1 WP pin port number */
#define SDC_SPI_CFG_CH1_WP_BIT              (0)     /* CH1 WP pin port bit number */
#define SDC_SPI_CFG_ERROR_LOG_ACQUISITION   (0)     /* Use Error log get select */
#define SDC_SPI_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE)
#define SDC_SPI_CFG_SBLK_NUM                (1)     /* Number of blocks accessed as single block */
#define SDC_SPI_CFG_USE_SC_CRC              (0)     /* Use CRC smart configration */
#endif /* SDC_SPI_CONFIG_H */
