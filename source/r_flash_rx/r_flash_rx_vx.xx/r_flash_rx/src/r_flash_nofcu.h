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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_flash_nofcu.h
 * Description  : This module implements equates/declarations specific to Flash Type 1 MCUs
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           23.04.2021 4.80    First Release. (Changed the structure of FLASH_TYPE_1 code with the addition of RX140.)
 *********************************************************************************************************************/

#ifndef R_FLASH_NOFCU_H
#define R_FLASH_NOFCU_H

#ifndef FLASH_HAS_FCU

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/*==== Flash information ====*/
/* Used for DataFlash */
#define DATAFLASH_READ_BASE_END_ADDR        (0x00101FFF)

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
#define DATAFLASH_WRITE_BASE_END_ADDR       (0xFE001FFF)
#elif defined(MCU_RX13T)
#undef DATAFLASH_READ_BASE_END_ADDR
#define DATAFLASH_READ_BASE_END_ADDR        (0x00100FFF)
#define DATAFLASH_WRITE_BASE_END_ADDR       (0x000F1FFF)
#else
#define DATAFLASH_WRITE_BASE_END_ADDR       (0x000F2FFF)
#endif

#define DATAFLASH_ADDR_OFFSET               (DATAFLASH_READ_BASE_END_ADDR - DATAFLASH_WRITE_BASE_END_ADDR)

/* Used for CodeFlash */
#define CODEFLASH_READ_BASE_END_ADDR        (0xFFFFFFFF)

#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
#define CODEFLASH_WRITE_BASE_END_ADDR       (0x001FFFFF)
#elif defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL)
#define CODEFLASH_WRITE_BASE_END_ADDR       (0xFC1FFFFF)
#else
#define CODEFLASH_WRITE_BASE_END_ADDR       (0x0007FFFF)
#endif

#define CODEFLASH_ADDR_OFFSET               (CODEFLASH_READ_BASE_END_ADDR - CODEFLASH_WRITE_BASE_END_ADDR)

/* flash mode definition (FENTRYR Register setting) */
#define FENTRYR_DATAFLASH_PE_MODE           (0xAA80)
#define FENTRYR_CODEFLASH_PE_MODE           (0xAA01)
#define FENTRYR_READ_MODE                   (0xAA00)

/* flash mode definition (FPMCR Register setting) */
#define DATAFLASH_PE_MODE                   (0x10)
#define READ_MODE                           (0x08)
#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
#define CODEFLASH_PE_MODE                   (0x02)
#else
#define DISCHARGE_1                         (0x12)
#define DISCHARGE_2                         (0x92)
#define CODEFLASH_PE_MODE                   (0x82)
#define LVPE_MODE                           (0x40)
#endif

/* operation definition (FCR Register setting) */
#define FCR_WRITE                           (0x81)
#define FCR_ERASE                           (0x84)
#define FCR_BLANKCHECK                      (0x83)
#define FCR_CLEAR                           (0x00)

/* Definition of high-speed operation mode of OPCCR register */
#if (FLASH_TYPE_VARIETY != FLASH_TYPE_VARIETY_A)
#define OPCCR_HIGH_SPEED_MODE               (0x00)
#endif

#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
/* E2 DataFlash Wait Cycle Setting (Height of ICLK frequency) */
#define FLASH_DFLWAITR_ICLK_FREQ            (32000000)

/* Boundary frequency of FCLK register (Peripheral Clock Notification bits) */
#define FLASH_FISR_FCLK_FREQ                (32000000)
#endif

/*==== Wait Process definition ====*/
#define WAIT_TDIS                           (3)
#define WAIT_TMS_MID                        (4)
#define WAIT_TMS_HIGH                       (6)
#define WAIT_TDSTOP                         (6)

#define MHZ                                 (1000000)
#define KHZ                                 (1000)
#define FCLK_MHZ                            ((MCU_CFG_FCLK_HZ + 999999) / MHZ)
#define ICLK_KHZ                            (MCU_CFG_ICLK_HZ / KHZ)

/* Definition for function name compatibility */
#define R_DF_Enable_DataFlashAccess flash_df_access_enable

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 *********************************************************************************************************************/
void flash_df_access_enable(void);
flash_err_t flash_get_status (void);
void flash_stop(void);
void flash_reset();
void flash_delay_us (unsigned long us, unsigned long khz);
flash_err_t flash_pe_mode_enter(flash_type_t flash_type);
flash_err_t flash_pe_mode_exit();
flash_err_t flash_erase(const uint32_t block_address, const uint32_t num_blocks);
flash_err_t flash_blankcheck(const uint32_t start_address, const uint32_t num_bytes, flash_res_t *result);
flash_err_t flash_write(const uint32_t src_address, const uint32_t dest_address, uint32_t num);

#endif /* #ifndef FLASH_HAS_FCU */

#endif  /* R_FLASH_NOFCU_H */
/* end of file */
