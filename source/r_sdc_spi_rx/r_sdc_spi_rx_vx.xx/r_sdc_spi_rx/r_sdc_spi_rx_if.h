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
 * File Name    : r_sdc_spi_rx_if.h
 * Version      : 1.10
 * Description  : SPI mode SD card driver interface header.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *         : 20.09.2024 1.10     Deleted #include "r_gpio_rx_if.h"
 *********************************************************************************************************************/
#ifndef R_SDC_SPI_RX_IF_H
#define R_SDC_SPI_RX_IF_H

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include "r_memdrv_rx_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define SDC_SPI_WRITE_WITH_PREERASE (0x00000000)    /* Write with pre erase */
#define SDC_SPI_WRITE_OVERWRITE     (0x00000001)    /* Write is over write */

/* Version Number of API. */
#define SDC_SPI_VERSION_MAJOR  (1)
#define SDC_SPI_VERSION_MINOR  (10)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* **** Error code **** */
typedef enum e_sdc_spi_status
{
    SDC_SPI_SUCCESS_IDLE         =   1,    /* OK */
    SDC_SPI_SUCCESS              =   0,    /* OK */
    SDC_SPI_ERR                  =  -1,    /* General error */
    SDC_SPI_ERR_WP               =  -2,    /* Write protect error */
    SDC_SPI_ERR_CRC              =  -7,    /* CRC error */
    SDC_SPI_ERR_ILLEGAL_COMMAND  = -83,    /* Command not legal */
    SDC_SPI_ERR_ADDRESS_BOUNDARY = -89,    /* Not specified buffer address in 4-byte unit */
    SDC_SPI_ERR_INTERNAL         = -99     /* Driver software internal error */
} sdc_spi_status_t;

/* **** SD card access information **** */
typedef struct
{
    uint8_t     * p_buff;       /* Buffer pointer */
    uint32_t    lbn;            /* First block number to access */
    int32_t     cnt;            /* read or write block count */
    uint32_t    write_mode;     /* write mode */
}sdc_spi_access_t;

/* **** Card status information **** */
typedef struct
{
    uint32_t card_sector_size;  /* Sector size */
    uint32_t max_block_number;  /* Max block number */
    uint8_t  write_protect;     /* Write protection */
    uint8_t  csd_structure;     /* CSD structure */
}sdc_spi_card_status_t;

/* **** Card register information **** */
typedef struct
{
    uint32_t    ocr[1];         /* OCR register */
    uint32_t    cid[4];         /* CID register */
    uint32_t    csd[4];         /* CSD register */
    uint32_t    scr[2];         /* SCR register */
    uint32_t    sdstatus[4];    /* SD status */
}sdc_spi_card_reg_t;

/* **** Initialize config **** */
typedef struct
{
    uint32_t    mode;       /* Initialize mode */
    uint32_t    voltage;    /* Voltage */
}sdc_spi_cfg_t;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_Open (uint32_t card_no, uint32_t dev_no, void * p_sdc_spi_workarea);
sdc_spi_status_t R_SDC_SPI_Close (uint32_t card_no);
sdc_spi_status_t R_SDC_SPI_GetCardDetection (uint32_t card_no);
sdc_spi_status_t R_SDC_SPI_Initialize (uint32_t card_no, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type);
sdc_spi_status_t R_SDC_SPI_End (uint32_t card_no);
sdc_spi_status_t R_SDC_SPI_Read (uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t R_SDC_SPI_Write (uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t R_SDC_SPI_GetCardStatus (uint32_t card_no, sdc_spi_card_status_t * p_sdc_spi_card_status);
sdc_spi_status_t R_SDC_SPI_GetCardInfo (uint32_t card_no, sdc_spi_card_reg_t * p_sdc_spi_card_reg);
sdc_spi_status_t R_SDC_SPI_SetLogHdlAddress (uint32_t user_long_que);
int32_t R_SDC_SPI_Log (uint32_t flg, uint32_t fid, uint32_t line);
uint32_t R_SDC_SPI_Getversion (void);

#endif
