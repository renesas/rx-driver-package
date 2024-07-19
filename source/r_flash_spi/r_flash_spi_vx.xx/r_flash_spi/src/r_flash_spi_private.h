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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_flash_spi_private.h
* Version      : 3.40
* Description  : FLASH SPI driver private header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 23.07.2014 2.21     Created
*              : 29.05.2015 2.32     Revised functions of same as Ver.2.32 of EEPROM SPI FIT module.
*              : 11.02.2019 3.00     Added parameter check of  Configuration Options.
*              : 10.12.2020 3.02     Fixed a bug that build does not pass due to #error 
*                                    when Flash of 1Gbit or more is selected.
*              : 31.12.2021 3.03     Added variable "read_after_write" "read_after_write_add" and
*                                    "read_after_write_data" for controlling SPI bus.
*              : 16.03.2023 3.20     Added support for AT25QF641B-SHB with Indirect Access Mode
*                                    of QSPIX
*                                    Added support for QSPIX Memory Mapped Mode.
*              : 15.11.2023 3.40     Added support for reading across multiple banks in QSPIX
*                                    Memory Mapped Mode.
*                                    Updated according to GSCE Code Checker 6.50.
*                                    Added support for MX25U6432F.
*                                    Added features Advanced sector protection supporting
*                                    for MX66L1G45 and MX25U6432F.
*************************************************************************************************/
#ifndef FLASH_SPI_PRIVATE_H
#define FLASH_SPI_PRIVATE_H


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
/* FLASH driver flash memory type file */
#if   (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) || (FLASH_SPI_CFG_DEV0_MX25R == 1) || (FLASH_SPI_CFG_DEV0_MX25U == 1) || \
      (FLASH_SPI_CFG_DEV1_MX25L == 1) || (FLASH_SPI_CFG_DEV1_MX66L == 1) || (FLASH_SPI_CFG_DEV1_MX25R == 1) || (FLASH_SPI_CFG_DEV1_MX25U == 1)
#include "./src/flash_types/flash_mx/r_flash_spi_type_sub.h"
#elif (FLASH_SPI_CFG_DEV0_AT25QF == 1) || (FLASH_SPI_CFG_DEV1_AT25QF == 1)
#include "./src/flash_types/flash_at/r_flash_spi_type_sub.h"
#endif

/* FLASH driver port header file */
#include "./src/dev_port/r_flash_spi_dev_port.h"

/* MEMDRV driver header file and configuration file */
#include "r_memdrv_rx_if.h"
#include "r_memdrv_rx_config.h"

#if (FLASH_SPI_CFG_LONGQ_ENABLE == 1)
/* Use LONGQ driver header file. */
#include "r_longq_if.h"
#endif /* (FLASH_SPI_CFG_LONGQ_ENABLE == 1) */


/*============================================================================*/
/*  Parameter check of  Configuration Options                              */
/*============================================================================*/

/* check of FLASH_SPI_CFG_DEVx_INCLUDED */
#if ((FLASH_SPI_CFG_DEV0_INCLUDED == 1 ) || ( FLASH_SPI_CFG_DEV1_INCLUDED == 1 ) )
#else
    #error "ERROR - One of the FLASH_SPI_CFG_DEVx_INCLUDED must be set. -  Parameter error in configures file."
#endif

/* Checking FLASH_SPI_CFG_DEV0_xxx is set*/
#if (FLASH_SPI_CFG_DEV0_INCLUDED != 1 )
#elif (FLASH_SPI_CFG_DEV0_INCLUDED == 1 ) && (( FLASH_SPI_CFG_DEV0_MX25L == 1 ) || (FLASH_SPI_CFG_DEV0_MX66L == 1) || (FLASH_SPI_CFG_DEV0_MX25R == 1) || (FLASH_SPI_CFG_DEV0_AT25QF == 1) || ( FLASH_SPI_CFG_DEV0_MX25U == 1))
#else
    #error "ERROR - One of the FLASH_SPI_CFG_DEV0_xxx must be set. -  Parameter error in configures file."
#endif

/* Checking FLASH_SPI_CFG_DEV1_xxx is set*/
#if (FLASH_SPI_CFG_DEV1_INCLUDED != 1 )
#elif (FLASH_SPI_CFG_DEV1_INCLUDED == 1 ) && (( FLASH_SPI_CFG_DEV1_MX25L == 1 ) || (FLASH_SPI_CFG_DEV1_MX66L == 1) || (FLASH_SPI_CFG_DEV1_MX25R == 1) || (FLASH_SPI_CFG_DEV1_AT25QF == 1) || (FLASH_SPI_CFG_DEV1_MX25U == 1))
#else
    #error "ERROR - One of the FLASH_SPI_CFG_DEV1_xxx must be set. -  Parameter error in configures file."
#endif

/* Check only one of FLASH_SPI_CFG_DEV0_xxx is set*/
#if (FLASH_SPI_CFG_DEV0_INCLUDED != 1 )
#elif (FLASH_SPI_CFG_DEV0_INCLUDED == 1 ) && ( \
   ( ( FLASH_SPI_CFG_DEV0_MX25L == 1 ) &&  (FLASH_SPI_CFG_DEV0_MX66L != 1) && (FLASH_SPI_CFG_DEV0_MX25R != 1) && (FLASH_SPI_CFG_DEV0_AT25QF != 1) && (FLASH_SPI_CFG_DEV0_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV0_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV0_MX66L == 1) && (FLASH_SPI_CFG_DEV0_MX25R != 1) && (FLASH_SPI_CFG_DEV0_AT25QF != 1) && (FLASH_SPI_CFG_DEV0_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV0_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV0_MX66L != 1) && (FLASH_SPI_CFG_DEV0_MX25R == 1) && (FLASH_SPI_CFG_DEV0_AT25QF != 1) && (FLASH_SPI_CFG_DEV0_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV0_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV0_MX66L != 1) && (FLASH_SPI_CFG_DEV0_MX25R != 1) && (FLASH_SPI_CFG_DEV0_AT25QF == 1) && (FLASH_SPI_CFG_DEV0_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV0_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV0_MX66L != 1) && (FLASH_SPI_CFG_DEV0_MX25R != 1) && (FLASH_SPI_CFG_DEV0_AT25QF != 1) && (FLASH_SPI_CFG_DEV0_MX25U == 1)))
#else
    #error "ERROR - Only One FLASH_SPI_CFG_DEV0_xxx can be set. -  Parameter error in configures file."
#endif

/* Check only one of FLASH_SPI_CFG_DEV1_xxx is set*/
#if (FLASH_SPI_CFG_DEV1_INCLUDED != 1 )
#elif (FLASH_SPI_CFG_DEV1_INCLUDED == 1 ) && ( \
   ( ( FLASH_SPI_CFG_DEV1_MX25L == 1 ) &&  (FLASH_SPI_CFG_DEV1_MX66L != 1) && (FLASH_SPI_CFG_DEV1_MX25R != 1) && (FLASH_SPI_CFG_DEV1_AT25QF != 1) && (FLASH_SPI_CFG_DEV1_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV1_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV1_MX66L == 1) && (FLASH_SPI_CFG_DEV1_MX25R != 1) && (FLASH_SPI_CFG_DEV1_AT25QF != 1) && (FLASH_SPI_CFG_DEV1_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV1_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV1_MX66L != 1) && (FLASH_SPI_CFG_DEV1_MX25R == 1) && (FLASH_SPI_CFG_DEV1_AT25QF != 1) && (FLASH_SPI_CFG_DEV1_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV1_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV1_MX66L != 1) && (FLASH_SPI_CFG_DEV1_MX25R != 1) && (FLASH_SPI_CFG_DEV1_AT25QF == 1) && (FLASH_SPI_CFG_DEV1_MX25U != 1)) || \
   ( ( FLASH_SPI_CFG_DEV1_MX25L != 1 ) &&  (FLASH_SPI_CFG_DEV1_MX66L != 1) && (FLASH_SPI_CFG_DEV1_MX25R != 1) && (FLASH_SPI_CFG_DEV1_AT25QF != 1) && (FLASH_SPI_CFG_DEV1_MX25U == 1)))
#else
    #error "ERROR - Only One FLASH_SPI_CFG_DEV1_xxx can be set. -  Parameter error in configures file."
#endif

/* Check only one of FLASH_SPI_CFG_DEV0_SIZE_xxx is set*/
#if (FLASH_SPI_CFG_DEV0_INCLUDED != 1 )
#elif (FLASH_SPI_CFG_DEV0_INCLUDED == 1 ) &&  (FLASH_SPI_CFG_DEV0_AT25QF != 1 ) && ( \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K == 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M == 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M == 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M == 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M == 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M == 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M == 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M == 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M == 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M == 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV0_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV0_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV0_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV0_SIZE_1G == 1)))
#elif (FLASH_SPI_CFG_DEV0_INCLUDED == 1 ) &&  (FLASH_SPI_CFG_DEV0_AT25QF == 1 ) && (FLASH_SPI_CFG_DEV0_SIZE_64M == 1)
#else 
    #error "ERROR - Only One FLASH_SPI_CFG_DEV0_SIZE_xxx can be set. -  Parameter error in configures file."
#endif /* FLASH_SPI_CFG_DEV0_INCLUDED != 1  */

/* Check only one of FLASH_SPI_CFG_DEV1_SIZE_xxx is set*/
#if (FLASH_SPI_CFG_DEV1_INCLUDED != 1 )
#elif (FLASH_SPI_CFG_DEV1_INCLUDED == 1 ) &&  (FLASH_SPI_CFG_DEV1_AT25QF != 1 ) && ( \
    ((FLASH_SPI_CFG_DEV1_SIZE_512K == 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M == 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M == 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M == 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M == 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M == 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M == 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M == 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M == 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M == 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G != 1)) || \
     ((FLASH_SPI_CFG_DEV1_SIZE_512K != 1 ) &&  (FLASH_SPI_CFG_DEV1_SIZE_1M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_2M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_4M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_16M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_32M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_64M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_128M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_256M != 1) && \
     (FLASH_SPI_CFG_DEV1_SIZE_512M != 1) && (FLASH_SPI_CFG_DEV1_SIZE_1G == 1)))
#elif (FLASH_SPI_CFG_DEV1_INCLUDED == 1 ) &&  (FLASH_SPI_CFG_DEV1_AT25QF == 1 ) && (FLASH_SPI_CFG_DEV1_SIZE_64M == 1)
#else 
    #error "ERROR - Only One FLASH_SPI_CFG_DEV1_SIZE_xxx can be set. -  Parameter error in configures file."
#endif /* FLASH_SPI_CFG_DEV1_INCLUDED != 1  */

/************************************************************************************************
Macro definitions
*************************************************************************************************/
/* Definition of FLASH device number */
#if   (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
    #define FLASH_SPI_DEV_NUM           (2)
#elif (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
    #define FLASH_SPI_DEV_NUM           (1)
#else
    #define FLASH_SPI_DEV_NUM           (0)
#endif

/* Definition of unit of software wait timer */
#define FLASH_SPI_UINT_MICROSECS        ((uint8_t)(0))              /* Unit : us                            */
#define FLASH_SPI_UINT_MILLISECS        ((uint8_t)(1))              /* Unit : ms                            */

/* Access time */
#define FLASH_SPI_T_READY_WAIT          (FLASH_SPI_UINT_MILLISECS)  /* Write busy completion polling time   */
#define FLASH_SPI_T_CS_HOLD             (FLASH_SPI_UINT_MICROSECS)  /* CS stability waiting time            */
#define FLASH_SPI_T_R_ACCESS            (FLASH_SPI_UINT_MICROSECS)  /* Reading start waiting time           */

/* Definition of read max size */
#if   defined(FLASH_SPI_CFG_USE_FIT)
#define FLASH_SPI_R_DATA_SIZE           ((uint32_t)(0xffffffff))    /* Read Max size 4G-1 Bytes             */
#else
#define FLASH_SPI_R_DATA_SIZE           ((uint32_t)(0x00008000))    /* Read Max size 32K Bytes              */
#endif  /* #if   defined(FLASH_SPI_CFG_USE_FIT) */

/* Address Mode */
#define FLASH_SPI_MODE_3BYTE            ((uint8_t)(0))              /* 3-byte Address Mode                  */
#define FLASH_SPI_MODE_4BYTE            ((uint8_t)(1))              /* 4-byte Address Mode                  */

/* Definitions of flash memory types */
#define FLASH_SPI_TYPE_NONE_MEMORY      ((uint32_t)(0x00000000))    /* None memory device                   */
#define FLASH_SPI_TYPE_MX25L            ((uint32_t)(0x00000001))    /* Macronix MX25L                       */
#define FLASH_SPI_TYPE_MX66L            ((uint32_t)(0x00000002))    /* Macronix MX66L                       */
#define FLASH_SPI_TYPE_MX25R            ((uint32_t)(0x00000004))    /* Macronix MX25R                       */
#define FLASH_SPI_TYPE_AT25QF           ((uint32_t)(0x00000008))    /* Adesto AT25QF                        */
#define FLASH_SPI_TYPE_MX25U            ((uint32_t)(0x00000010))    /* Macronix MX25U                       */

/* Address boundary */
#define FLASH_SPI_ADDR_BOUNDARY         ((uint32_t)(0x00000003))

/* Definition of read max size in a Bank Memory Map */
#define FLASH_SPI_R_DATA_QSPIX_BANK_SIZE    ((uint32_t)(0x4000000)) /* Read Max size 64 MBytes             */

/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* Enumeration for error log */
#if (FLASH_SPI_CFG_LONGQ_ENABLE == 1)                               /* Use LONGQ driver.                    */
#define FLASH_SPI_DRIVER_ID             ((uint32_t)(8))             /* FLASH debug log id                   */
#define FLASH_SPI_DEBUG_ERR_ID          ((uint32_t)(1))             /* FLASH debug log error id             */
typedef enum e_flash_log
{
    FLASH_SPI_USER       =  11,
    FLASH_SPI_TYPE       =  12,
    FLASH_SPI_TYPE_SUB   =  13,
} flash_logid_t;
#endif /* (FLASH_SPI_CFG_LONGQ_ENABLE == 1) */


/************************************************************************************************
Exported global variables
*************************************************************************************************/


/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
#if (FLASH_SPI_CFG_LONGQ_ENABLE == 1)
/* It is a function for debugging. When invalidate definition "FLASH_SPI_CFG_LONGQ_ENABLE",
   the code is not generated. */
uint32_t           r_flash_spi_log(uint32_t flg, uint32_t fid, uint32_t line);
#define R_FLASH_SPI_Log_Func(x, y, z)   (r_flash_spi_log((x), (y), (z))) /* define a function for debugging */
#else
#define R_FLASH_SPI_Log_Func(x, y, z)    /* define a empty */
#endif /* (FLASH_SPI_CFG_LONGQ_ENABLE == 1) */

/* r_flash_spi_type.c */
/******************************************************************************
 * Function Name: r_flash_spi_init_port
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_init_port (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_reset_port
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_reset_port (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_read_status
 * Description  : .
 * Arguments    : devno
 *              : p_status
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_status (uint8_t devno, uint8_t * p_status);

/******************************************************************************
 * Function Name: r_flash_spi_set_write_protect
 * Description  : .
 * Arguments    : devno
 *              : wpsts
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_set_write_protect (uint8_t devno, uint8_t wpsts);

/******************************************************************************
 * Function Name: r_flash_spi_set_write_protect_advanced_sector
 * Description  : .
 * Arguments    : devno
 *              : flash_spi_protect_sector_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_set_write_protect_advanced_sector (uint8_t devno, flash_spi_protect_sector_info_t * flash_spi_protect_sector_info);

/******************************************************************************
 * Function Name: r_flash_spi_erase_write_protect_advanced_sector
 * Description  : .
 * Arguments    : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_erase_write_protect_advanced_sector (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_write_di
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_di (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_read_data
 * Description  : .
 * Arguments    : devno
 *              : p_flash_spi_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_data (uint8_t devno, flash_spi_info_t * p_flash_spi_info);

/******************************************************************************
 * Function Name: r_flash_spi_write_data_page
 * Description  : .
 * Arguments    : devno
 *              : p_flash_spi_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_data_page (uint8_t devno, flash_spi_info_t * p_flash_spi_info);

/******************************************************************************
 * Function Name: r_flash_spi_erase
 * Description  : .
 * Arguments    : devno
 *              : p_flash_spi_erase_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_erase (uint8_t devno, flash_spi_erase_info_t * p_flash_spi_erase_info);

/******************************************************************************
 * Function Name: r_flash_spi_polling
 * Description  : .
 * Arguments    : devno
 *              : mode
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_polling (uint8_t devno, flash_spi_poll_mode_t mode);

/******************************************************************************
 * Function Name: r_flash_spi_read_id
 * Description  : .
 * Arguments    : devno
 *              : p_data
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_id (uint8_t devno, uint8_t * p_data);

/******************************************************************************
 * Function Name: r_flash_spi_get_memory_info
 * Description  : .
 * Arguments    : devno
 *              : p_flash_spi_mem_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_get_memory_info (uint8_t devno, flash_spi_mem_info_t * p_flash_spi_mem_info);

/******************************************************************************
 * Function Name: r_flash_spi_read_configuration
 * Description  : .
 * Arguments    : devno
 *              : p_config_reg
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_configuration (uint8_t devno, uint8_t * p_config_reg);

/******************************************************************************
 * Function Name: r_flash_spi_read_status2
 * Description  : .
 * Arguments    : devno
 *              : p_status
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_status2 (uint8_t devno, uint8_t * p_status);

/******************************************************************************
 * Function Name: r_flash_spi_read_status3
 * Description  : .
 * Arguments    : devno
 *              : p_status
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_status3 (uint8_t devno, uint8_t * p_status);

/******************************************************************************
 * Function Name: r_flash_spi_write_configuration
 * Description  : .
 * Arguments    : devno
 *              : p_reg
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_configuration (uint8_t devno, flash_spi_reg_info_t * p_reg);

/******************************************************************************
 * Function Name: r_flash_spi_write_status
 * Description  : .
 * Arguments    : devno
 *              : p_reg
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_status (uint8_t devno, uint8_t * p_reg);

/******************************************************************************
 * Function Name: r_flash_spi_write_status2
 * Description  : .
 * Arguments    : devno
 *              : p_reg
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_status2 (uint8_t devno, uint8_t * p_reg);

/******************************************************************************
 * Function Name: r_flash_spi_write_status3
 * Description  : .
 * Arguments    : devno
 *              : p_reg
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_status3 (uint8_t devno, uint8_t * p_reg);

/******************************************************************************
 * Function Name: r_flash_spi_read_security
 * Description  : .
 * Arguments    : devno
 *              : p_scur
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_security (uint8_t devno, uint8_t * p_scur);

/******************************************************************************
 * Function Name: r_flash_spi_read_data_security_page
 * Description  : .
 * Arguments    : devno
 *              : p_flash_spi_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_read_data_security_page (uint8_t devno, flash_spi_info_t * p_flash_spi_info);

/******************************************************************************
 * Function Name: r_flash_spi_write_data_security_page
 * Description  : .
 * Arguments    : devno
 *              : p_flash_spi_info
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_write_data_security_page (uint8_t devno, flash_spi_info_t * p_flash_spi_info);

/******************************************************************************
 * Function Name: r_flash_spi_quad_enable
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_quad_enable (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_quad_disable
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_quad_disable (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_set_4byte_address_mode
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_set_4byte_address_mode (uint8_t devno);

/* r_flash_spi_drvif.c */
/******************************************************************************
 * Function Name: r_flash_spi_drvif_open
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_open (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_open_tx_data
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_open_tx_data (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_open_rx_data
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_open_rx_data (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_close
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_close (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_close_tx_data
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_close_tx_data (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_close_rx_data
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_close_rx_data (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_disable
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_disable (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_enable
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_enable (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_enable_tx_data
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_enable_tx_data (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_enable_rx_data
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_enable_rx_data (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_tx
 * Description  : .
 * Arguments    : devno
 *              : txcnt
 *              : p_data
 *              : read_after_write
 *              : read_in_memory_mapped
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_tx (uint8_t devno, uint32_t txcnt, uint8_t * p_data,
                                        bool read_after_write, bool read_in_memory_mapped);


/******************************************************************************
 * Function Name: r_flash_spi_drvif_tx_add
 * Description  : .
 * Arguments    : devno
 *              : txcnt
 *              : p_data
 *              : op_mode
 *              : read_after_write_add
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_tx_add (uint8_t devno, uint32_t txcnt, uint8_t * p_data,
                                            flash_spi_opmode_t op_mode, bool read_after_write_add);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_tx_data
 * Description  : .
 * Arguments    : devno
 *              : txcnt
 *              : p_data
 *              : op_mode
 *              : read_after_write_data
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_tx_data (uint8_t devno, uint32_t txcnt, uint8_t * p_data,
                                             flash_spi_opmode_t op_mode, bool read_after_write_data);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_rx
 * Description  : .
 * Arguments    : devno
 *              : rxcnt
 *              : p_data
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_rx (uint8_t devno, uint32_t rxcnt, uint8_t * p_data);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_rx_add
 * Description  : .
 * Arguments    : devno
 *              : rxcnt
 *              : p_data
 *              : op_mode
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_rx_add (uint8_t devno, uint32_t rxcnt, uint8_t * p_data,

        flash_spi_opmode_t op_mode);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_rx_data
 * Description  : .
 * Arguments    : devno
 *              : rxcnt
 *              : p_data
 *              : p_addr
 *              : addr_size
 *              : op_mode
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_rx_data (uint8_t devno, uint32_t rxcnt, uint8_t * p_data,
                                         uint32_t p_addr, uint8_t addr_size, flash_spi_opmode_t op_mode);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_1ms_interval
 * Description  : .
 * Return Value : .
 *****************************************************************************/
void               r_flash_spi_drvif_1ms_interval (void);

/******************************************************************************
 * Function Name: r_flash_spi_drvif_set_loghdladdress
 * Description  : .
 * Argument     : user_long_que
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_drvif_set_loghdladdress (uint32_t user_long_que);


/* r_flash_spi_dev_port_iodefine.c or r_flash_spi_dev_port_gpio.c */
/******************************************************************************
 * Function Name: r_flash_spi_cs_init
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
void               r_flash_spi_cs_init (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_cs_reset
 * Description  : .
 * Argument     : devno
 * Return Value : .
 *****************************************************************************/
void               r_flash_spi_cs_reset (uint8_t devno);

/******************************************************************************
 * Function Name: r_flash_spi_set_cs
 * Description  : .
 * Arguments    : devno
 *              : lv
 * Return Value : .
 *****************************************************************************/
void               r_flash_spi_set_cs (uint8_t devno, uint8_t lv);

/******************************************************************************
 * Function Name: r_flash_spi_softwaredelay
 * Description  : .
 * Arguments    : delay
 *              : units
 * Return Value : .
 *****************************************************************************/
bool               r_flash_spi_softwaredelay (uint32_t delay, bsp_delay_units_t units);

/******************************************************************************
 * Function Name: r_flash_spi_wait_lp
 * Description  : .
 * Argument     : unit
 * Return Value : .
 *****************************************************************************/
flash_spi_status_t r_flash_spi_wait_lp (uint8_t unit);


#endif /* FLASH_SPI_PRIVATE_H */

/* End of File */
