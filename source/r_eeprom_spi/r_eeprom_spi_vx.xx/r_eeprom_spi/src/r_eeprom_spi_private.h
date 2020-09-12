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
* Copyright (C) 2008(2009-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_eeprom_spi_private.h
* Version      : 3.00
* Description  : EEPROM driver private header file
*************************************************************************************************/
/************************************************************************************************
* History      : Date          No.             Comment
*              : 2008.09.09                    wrote it.
*              : 2009.01.31    #### CF-0005    Divided between SIO processing and CRC calculation.
*              : 2009.03.06    Ver1.30 --------------------------------------------------------
*              : 2011.02.15                    Changed API.
*              : 2011.02.15    Ver2.00 --------------------------------------------------------
*              : 2011.04.15    Ver2.01 --------------------------------------------------------
*              : 2012.03.19    Changed History to English comment.
*              : 2012.03.19    Ver2.02 --------------------------------------------------------
*              : 2013.09.30    Ver2.03 Corrected comments.
*
*              : DD.MM.YYYY Version  Description
*              : 28.11.2014 2.30     Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31     Added RX71M.
*              : 29.05.2015 2.32     Added RX231 and RX230.
*              : 21.12.2018 3.00     Delete r_eeprom_spi_drvif_dev0.c and r_eeprom_spi_drvif_dev1.c related statement
*************************************************************************************************/
#ifndef __EEPROM_SPI_PRIVATE_H__
#define __EEPROM_SPI_PRIVATE_H__


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/

/* EEPROM driver targets header file */
#include "./src/dev_port/r_eeprom_spi_dev_port.h"

#ifdef EEPROM_SPI_CFG_LONGQ_ENABLE
/* Use LONGQ driver header file. */
#include "r_longq_if.h"
#endif /* EEPROM_SPI_CFG_LONGQ_ENABLE */


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/* Definition of EEPROM device number */
#if   defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
    #define EEPROM_SPI_DEV_NUM     (2)
#elif defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
    #define EEPROM_SPI_DEV_NUM     (1)
#else
    #define EEPROM_SPI_DEV_NUM     (0)
#endif

/* Definition of read max size */
#if   defined(EEPROM_SPI_CFG_USE_FIT)
    #define EEPROM_SPI_R_DATA_SIZE (uint32_t)(0xffffffff)  /* Read Max size 4G-1 Bytes             */
#else
    #define EEPROM_SPI_R_DATA_SIZE (uint32_t)(0x00008000)  /* Read Max size 32K Bytes              */
#endif  /* #if   defined(EEPROM_SPI_CFG_USE_FIT) */

/*-------- Definitions of device information ---------*/
/* ==== EEPROM device 0 ==== */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_002K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x100)   /* 256B  (  2K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(16)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(1)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_002K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_004K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x200)   /* 512B  (  4K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(16)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(1)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_004K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_008K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x400)   /*  1KB  (  8K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(32)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_008K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_016K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x800)   /*  2KB  ( 16K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(32)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_016K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_032K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x1000)  /*  4KB  ( 32K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(32)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_032K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_064K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x2000)  /*  8KB  ( 64K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(32)      /* Writing page size                    */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_064K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_128K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x4000)  /* 16KB  (128K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(64)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_128K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_256K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x8000)  /* 32KB  (256K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(64)      /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_256K */
#ifdef EEPROM_SPI_CFG_DEV0_SIZE_512K
#define EEPROM_SPI_DEV0_MEM_SIZE       (uint32_t)(0x10000) /* 64KB  (512K-bit)                     */
#define EEPROM_SPI_DEV0_WPAG_SIZE      (uint32_t)(128)     /* Page size when writing               */
#define EEPROM_SPI_DEV0_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV0_SIZE_512K */

#define EEPROM_SPI_DEV0_UPPER_QUART    (EEPROM_SPI_DEV0_MEM_SIZE - (EEPROM_SPI_DEV0_MEM_SIZE / 4))
                                                       /* Start address of upper quarter setting   */
#define EEPROM_SPI_DEV0_UPPER_HALF     (EEPROM_SPI_DEV0_MEM_SIZE / 2)
                                                       /* Start address of upper half setting      */

/* ==== EEPROM device 1 ==== */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_002K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x100)   /* 256B  (  2K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(16)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(1)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_002K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_004K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x200)   /* 512B  (  4K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(16)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(1)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_004K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_008K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x400)   /*  1KB  (  8K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(32)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_008K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_016K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x800)   /*  2KB  ( 16K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(32)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_016K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_032K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x1000)  /*  4KB  ( 32K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(32)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_032K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_064K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x2000)  /*  8KB  ( 64K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(32)      /* Writing page size                    */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_064K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_128K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x4000)  /* 16KB  (128K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(64)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_128K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_256K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x8000)  /* 32KB  (256K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(64)      /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_256K */
#ifdef EEPROM_SPI_CFG_DEV1_SIZE_512K
#define EEPROM_SPI_DEV1_MEM_SIZE       (uint32_t)(0x10000) /* 64KB  (512K-bit)                     */
#define EEPROM_SPI_DEV1_WPAG_SIZE      (uint32_t)(128)     /* Page size when writing               */
#define EEPROM_SPI_DEV1_ADDR_SIZE      (uint8_t)(2)        /* Address size (Bytes)                 */
#endif  /* #ifdef EEPROM_SPI_CFG_DEV1_SIZE_512K */

#define EEPROM_SPI_DEV1_UPPER_QUART    (EEPROM_SPI_DEV1_MEM_SIZE - (EEPROM_SPI_DEV1_MEM_SIZE / 4))
                                                       /* Start address of upper quarter setting   */
#define EEPROM_SPI_DEV1_UPPER_HALF     (EEPROM_SPI_DEV1_MEM_SIZE / 2)
                                                       /* Start address of upper half setting      */


/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* Enumeration for error log */
#ifdef EEPROM_SPI_CFG_LONGQ_ENABLE                         /* Use LONGQ driver.                    */
#define EEPROM_SPI_DRIVER_ID       (uint32_t)(8)           /* EEPROM debug log id                  */
#define EEPROM_SPI_DEBUG_ERR_ID    (uint32_t)(1)           /* EEPROM debug log error id            */
typedef enum e_eeprom_log
{
    EEPROM_SPI_USER       =  11,
    EEPROM_SPI_SUB        =  12,
} eeprom_logid_t;
#endif /* EEPROM_SPI_CFG_LONGQ_ENABLE */


/************************************************************************************************
Exported global variables
*************************************************************************************************/


/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
/* r_eeprom_spi_sub.c */
void            r_eeprom_spi_init_port(uint8_t devno);
void            r_eeprom_spi_reset_port(uint8_t devno);
eeprom_status_t r_eeprom_spi_read(uint8_t devno,  eeprom_info_t * p_eeprom_info);
eeprom_status_t r_eeprom_spi_write_di(uint8_t devno);
eeprom_status_t r_eeprom_spi_read_stsreg(uint8_t devno, uint8_t * p_stsreg);
eeprom_status_t r_eeprom_spi_write_stsreg(uint8_t devno, uint8_t * p_stsreg);
eeprom_status_t r_eeprom_spi_polling(uint8_t devno);
eeprom_status_t r_eeprom_spi_write_page(uint8_t devno, eeprom_info_t  * p_eeprom_info);
eeprom_status_t r_eeprom_spi_check_cnt(uint8_t devno, eeprom_info_t  * p_eeprom_info);
eeprom_status_t r_eeprom_spi_check_wp(uint8_t devno, eeprom_info_t  * p_eeprom_info);
uint32_t        r_eeprom_spi_page_calc(uint8_t devno, eeprom_info_t  * p_eeprom_info);

#ifdef EEPROM_SPI_CFG_LONGQ_ENABLE
/* It is a function for debugging. When invalidate definition "EEPROM_SPI_CFG_LONGQ_ENABLE",
   the code is not generated. */
uint32_t        r_eeprom_spi_log(uint32_t flg, uint32_t fid, uint32_t line);
#define R_EEPROM_SPI_Log_Func(x, y, z)   r_eeprom_spi_log(x, y, z)
#else
#define R_EEPROM_SPI_Log_Func(x, y, z)
#endif /* EEPROM_SPI_CFG_LONGQ_ENABLE */

/* r_eeprom_spi_drvif.c */
eeprom_status_t r_eeprom_spi_drvif_open(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_open_tx_data(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_open_rx_data(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_close(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_close_tx_data(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_close_rx_data(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_disable(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_enable(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_enable_tx_data(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_enable_rx_data(uint8_t devno);
eeprom_status_t r_eeprom_spi_drvif_tx(uint8_t devno, uint32_t txcnt, uint8_t * p_data);
eeprom_status_t r_eeprom_spi_drvif_tx_data(uint8_t devno, uint32_t txcnt, uint8_t * p_data);
eeprom_status_t r_eeprom_spi_drvif_rx(uint8_t devno, uint32_t rxcnt, uint8_t * p_data);
eeprom_status_t r_eeprom_spi_drvif_rx_data(uint8_t devno, uint32_t rxcnt, uint8_t * p_data);
void            r_eeprom_spi_drvif_1ms_interval(void);
eeprom_status_t r_eeprom_spi_drvif_set_loghdladdress(uint32_t user_long_que);

/* r_eeprom_spi_dev_port.c */
void            r_eeprom_spi_cs_init(uint8_t devno);
void            r_eeprom_spi_cs_reset(uint8_t devno);
void            r_eeprom_spi_set_cs(uint8_t devno, uint8_t lv);
bool            r_eeprom_spi_softwaredelay(uint32_t delay, bsp_delay_units_t units);
eeprom_status_t r_eeprom_spi_wait_lp(uint8_t unit);


#endif /* __EEPROM_SPI_PRIVATE_H__ */

/* End of File */
