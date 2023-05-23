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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_flash_spi_type_sub.h
* Version      : 1.00
* Description  : FLASH SPI driver memory type header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 16.03.2023 1.00     Created
*************************************************************************************************/
#ifndef FLASH_SPI_TYPE_SUB_H
#define FLASH_SPI_TYPE_SUB_H


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/*-------- Definitions of Command size ---------*/
#define FLASH_SPI_AT_CMD_SIZE           (uint8_t)(1)    /* Command size(Byte)                       */
#define FLASH_SPI_AT_STSREG1_SIZE       (uint32_t)(1)   /* Status register 1 size(Byte)             */
#define FLASH_SPI_AT_STSREG2_SIZE       (uint32_t)(1)   /* Status register 2 size(Byte)             */
#define FLASH_SPI_AT_STSREG3_SIZE       (uint32_t)(1)   /* Status register 3 size(Byte)             */
#define FLASH_SPI_AT_WSTSREG1_SIZE      (uint32_t)(1)   /* Status register 1 size (bytes) for Write */
#define FLASH_SPI_AT_WSTSREG2_SIZE      (uint32_t)(1)   /* Status register 2 size (bytes) for Write */
#define FLASH_SPI_AT_WSTSREG3_SIZE      (uint32_t)(1)   /* Status register 3 size (bytes) for Write */
#define FLASH_SPI_AT_IDDATA_SIZE        (uint32_t)(3)   /* ID data size (bytes)                     */
#define TRUE                            (bool)(true)    /* not close SPI bus cycle                  */
#define FALSE                           (bool)(false)   /* close SPI bus cycle                      */

/*-------- Definitions of device information ---------*/
/* ==== FLASH device 0 ==== */
#if (FLASH_SPI_CFG_DEV0_AT25QF == 1) && (FLASH_SPI_CFG_DEV0_SIZE_64M == 1)
#define FLASH_SPI_AT_DEV0_MEM_SIZE      (uint32_t)(8388608)     /* 8MByte (64Mbit)                      */
#define FLASH_SPI_AT_DEV0_B4K_ADDR      (uint32_t)(0xfffff000)  /* 4KB Block address setting            */
#define FLASH_SPI_AT_DEV0_B32K_ADDR     (uint32_t)(0xffff8000)  /* 32KB Block address setting           */
#define FLASH_SPI_AT_DEV0_B64K_ADDR     (uint32_t)(0xffff0000)  /* 64KB Block address setting           */
#define FLASH_SPI_AT_DEV0_SCUR_ADDR     (uint32_t)(0x00003000)  /* Mask three pages address setting     */
#define FLASH_SPI_AT_DEV0_PAGE_SIZE     (uint32_t)(256)         /* Page size when writing               */
#define FLASH_SPI_AT_DEV0_ADDR_SIZE     FLASH_SPI_ADDR_3BYTES   /* Address size (bytes)                 */
#define FLASH_SPI_AT_DEV0_WP_WHOLE_MEM  (uint8_t)(0x07)         /* Whole memory WP setting              */
#define FLASH_SPI_AT_DEV0_FULL_CHIP_ERASE   FLASH_SPI_MODE_C_ERASE
                                            /* Kind of full-chip erase command that the device supports */
#define FLASH_SPI_AT_DEV0_ADDR_MODE     FLASH_SPI_MODE_3BYTE    /* Addressability Mode                  */
#define FLASH_SPI_DEV0_TYPE             FLASH_SPI_TYPE_AT25QF   /* Flash memory type                    */
#endif  /* (FLASH_SPI_CFG_DEV0_AT25QF == 1) && (FLASH_SPI_CFG_DEV0_SIZE_64M == 1) */

/* ==== FLASH device 1 ==== */
#if (FLASH_SPI_CFG_DEV1_AT25QF == 1) && (FLASH_SPI_CFG_DEV1_SIZE_64M == 1)
#define FLASH_SPI_AT_DEV1_MEM_SIZE      (uint32_t)(8388608)     /* 8MByte (64Mbit)                      */
#define FLASH_SPI_AT_DEV1_B4K_ADDR      (uint32_t)(0xfffff000)  /* 4KB Block address setting            */
#define FLASH_SPI_AT_DEV1_B32K_ADDR     (uint32_t)(0xffff8000)  /* 32KB Block address setting           */
#define FLASH_SPI_AT_DEV1_B64K_ADDR     (uint32_t)(0xffff0000)  /* 64KB Block address setting           */
#define FLASH_SPI_AT_DEV1_SCUR_ADDR     (uint32_t)(0x00300000)  /* Mask three pages address setting     */
#define FLASH_SPI_AT_DEV1_PAGE_SIZE     (uint32_t)(256)         /* Page size when writing               */
#define FLASH_SPI_AT_DEV1_ADDR_SIZE     FLASH_SPI_ADDR_3BYTES   /* Address size (bytes)                 */
#define FLASH_SPI_AT_DEV1_WP_WHOLE_MEM  (uint8_t)(0x07)         /* Whole memory WP setting              */
#define FLASH_SPI_AT_DEV1_FULL_CHIP_ERASE   FLASH_SPI_MODE_C_ERASE
                                            /* Kind of full-chip erase command that the device supports */
#define FLASH_SPI_AT_DEV1_ADDR_MODE     FLASH_SPI_MODE_3BYTE    /* Addressability Mode                  */
#define FLASH_SPI_DEV1_TYPE             FLASH_SPI_TYPE_AT25QF   /* Flash memory type                    */
#endif  /* (FLASH_SPI_CFG_DEV1_AT25QF == 1) && (FLASH_SPI_CFG_DEV1_SIZE_64M == 1) */

/*------- Definitions of Status Register 1 value -------*/
#define FLASH_SPI_AT_REG1_SRP0           (uint8_t)(0x80)         /* Status Register Protection bit 0     */
#define FLASH_SPI_AT_REG1_SEC            (uint8_t)(0x40)         /* Block Protection                     */
#define FLASH_SPI_AT_REG1_TB             (uint8_t)(0x20)         /* Top/Bottom Protection                */
#define FLASH_SPI_AT_REG1_BP2            (uint8_t)(0x10)         /* Block Protect Bit2                   */
#define FLASH_SPI_AT_REG1_BP1            (uint8_t)(0x08)         /* Block Protect Bit1                   */
#define FLASH_SPI_AT_REG1_BP0            (uint8_t)(0x04)         /* Block Protect Bit0                   */
#define FLASH_SPI_AT_REG1_WEL            (uint8_t)(0x02)         /* Write Enable Latch Bit               */
#define FLASH_SPI_AT_REG1_WIP            (uint8_t)(0x01)         /* Write In Progress Bit                */
#define FLASH_SPI_AT_REG1_MASK           (uint8_t)(0xfc)         /* Write status fixed data              */
#define FLASH_SPI_AT_REG1_BPMASK         (uint8_t)(0x1c)         /* Block Protect Bit Mask               */

/*------- Definitions of Status Register 2 value -------*/
#define FLASH_SPI_AT_REG2_E_SUS          (uint8_t)(0x80)         /* Erase Suspend Status                  */
#define FLASH_SPI_AT_REG2_CMP            (uint8_t)(0x40)         /* Complement Block Protection           */
#define FLASH_SPI_AT_REG2_LB3            (uint8_t)(0x20)         /* Lock Security Register 3              */
#define FLASH_SPI_AT_REG2_LB2            (uint8_t)(0x10)         /* Lock Security Register 2              */
#define FLASH_SPI_AT_REG2_LB1            (uint8_t)(0x08)         /* Lock Security Register 1              */
#define FLASH_SPI_AT_REG2_P_SUS          (uint8_t)(0x04)         /* Program Suspend Status                */
#define FLASH_SPI_AT_REG2_QE             (uint8_t)(0x02)         /* Quad Enable                           */
#define FLASH_SPI_AT_REG2_SRP1           (uint8_t)(0x01)         /* Status Register Protection bit 0      */

/*------- Definitions of Status Register 3 value -------*/
#define FLASH_SPI_AT_REG3_DRV1           (uint8_t)(0x40)         /* Drive Strength 1   */
#define FLASH_SPI_AT_REG3_DRV0           (uint8_t)(0x20)         /* Drive Strength 0   */

/************************************************************************************************
Typedef definitions
*************************************************************************************************/


/************************************************************************************************
Exported global variables
*************************************************************************************************/


/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
/* r_flash_spi_sub.c */
void               r_flash_spi_at_init_port(uint8_t devno);
void               r_flash_spi_at_reset_port(uint8_t devno);
flash_spi_status_t r_flash_spi_at_write_di(uint8_t devno);
flash_spi_status_t r_flash_spi_at_read_stsreg1(uint8_t devno, uint8_t * p_status);
flash_spi_status_t r_flash_spi_at_read_stsreg2(uint8_t devno, uint8_t * p_status);
flash_spi_status_t r_flash_spi_at_read_stsreg3(uint8_t devno, uint8_t * p_status);
flash_spi_status_t r_flash_spi_at_set_write_protect(uint8_t devno, uint8_t wpsts, bool read_after_write);
flash_spi_status_t r_flash_spi_at_write_configuration(uint8_t devno, flash_spi_reg_info_t * p_reg, bool read_after_write);
flash_spi_status_t r_flash_spi_at_write_stsreg1(uint8_t devno, uint8_t * p_reg, bool read_after_write);
flash_spi_status_t r_flash_spi_at_write_stsreg2(uint8_t devno, uint8_t * p_reg, bool read_after_write);
flash_spi_status_t r_flash_spi_at_write_stsreg3(uint8_t devno, uint8_t * p_reg, bool read_after_write);
flash_spi_status_t r_flash_spi_at_quad_enable(uint8_t devno, bool read_after_write);
flash_spi_status_t r_flash_spi_at_quad_disable(uint8_t devno, bool read_after_write);
flash_spi_status_t r_flash_spi_at_read(uint8_t devno, flash_spi_info_t * p_flash_spi_info);
flash_spi_status_t r_flash_spi_at_read_scurreg_page(uint8_t devno, flash_spi_info_t * p_flash_spi_info);
flash_spi_status_t r_flash_spi_at_write_page(uint8_t devno, flash_spi_info_t  * p_flash_spi_info, bool read_after_write_add, bool read_after_write_data);
flash_spi_status_t r_flash_spi_at_write_scurreg_page(uint8_t devno, flash_spi_info_t  * p_flash_spi_info, bool read_after_write_add, bool read_after_write_data);
flash_spi_status_t r_flash_spi_at_erase(uint8_t devno, flash_spi_erase_info_t * p_flash_spi_erase_info);
flash_spi_status_t r_flash_spi_at_rdid(uint8_t devno, uint8_t * p_data);
flash_spi_status_t r_flash_spi_at_polling(uint8_t devno, flash_spi_poll_mode_t mode);
flash_spi_status_t r_flash_spi_at_get_memory_info(uint8_t devno, flash_spi_mem_info_t * p_flash_spi_mem_info);
flash_spi_status_t r_flash_spi_at_check_cnt(uint8_t devno, flash_spi_info_t  * p_flash_spi_info);
flash_spi_status_t r_flash_spi_at_check_scurreg_cnt(uint8_t devno, flash_spi_info_t * p_flash_spi_info);
uint32_t           r_flash_spi_at_page_calc(uint8_t devno, flash_spi_info_t  * p_flash_spi_info);


#endif /* FLASH_SPI_TYPE_SUB_H */

/* End of File */
