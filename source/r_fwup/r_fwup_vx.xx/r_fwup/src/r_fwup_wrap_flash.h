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
 * Copyright (C) 2023-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_flash.h
 * Description  : Functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 20.07.2023 2.00    First Release
 *         : 29.09.2023 2.01    Fixed log messages.
 *                              Add parameter checking.
 *                              Added arguments to R_FWUP_WriteImageProgram API.
 *         : 28.03.2024 2.02    Update wrapper functions.
 *         : 09.04.2024 2.03    Fixed wrapper function.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_FWUP_WRAP_FLASH_H_
#define R_FWUP_WRAP_FLASH_H_

/* Flash functions */
#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
#define FWUP_FUNC_BUFFER_AREA_ERASE               r_fwup_wrap_ext_flash_erase
#define FWUP_FUNC_BUFFER_AREA_WRITE               r_fwup_wrap_ext_flash_write
#define FWUP_FUNC_BUFFER_AREA_READ                r_fwup_wrap_ext_flash_read
#else
#define FWUP_FUNC_BUFFER_AREA_ERASE               r_fwup_wrap_flash_erase
#define FWUP_FUNC_BUFFER_AREA_WRITE               r_fwup_wrap_flash_write
#define FWUP_FUNC_BUFFER_AREA_READ                r_fwup_wrap_flash_read
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_open  (void);
void         r_fwup_wrap_flash_close (void);
e_fwup_err_t r_fwup_wrap_flash_erase (uint32_t addr, uint32_t num_blocks);
e_fwup_err_t r_fwup_wrap_flash_write (uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes);
e_fwup_err_t r_fwup_wrap_flash_read  (uint32_t buf_addr, uint32_t src_addr, uint32_t size);
#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
e_fwup_err_t r_fwup_wrap_bank_swap   (void);
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */

#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
e_fwup_err_t r_fwup_wrap_ext_flash_open  (void);
void         r_fwup_wrap_ext_flash_close (void);
e_fwup_err_t r_fwup_wrap_ext_flash_erase (uint32_t offsetadd, uint32_t num_sectors);
e_fwup_err_t r_fwup_wrap_ext_flash_write (uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes);
e_fwup_err_t r_fwup_wrap_ext_flash_read  (uint32_t buf_addr, uint32_t src_addr, uint32_t size);
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */

#endif /* R_FWUP_WRAP_FLASH_H_ */
