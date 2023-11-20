/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_fwup_config.h
* Description   : Configures the Firmware update module.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.07.2023 2.00    First Release
*         : 29.09.2023 2.01    Fixed log messages.
*                              Add parameter checking.
*                              Added arguments to R_FWUP_WriteImageProgram API.
***********************************************************************************************************************/
#include "platform.h"

#ifndef R_FWUP_CONFIG_H
#define R_FWUP_CONFIG_H

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Select the update mode.
    0 = Dual bank
    1 = Single bank with buffer. (default)
    2 = Single bank without buffer.
    3 = Single bank with ext-buffer.
*/
#define FWUP_CFG_UPDATE_MODE                        (1)

/* Select the function mode.
    0 = use for Boot loader
    1 = use for User program
*/
#define FWUP_CFG_FUNCTION_MODE                      (0)

/* Area configuration */
#define FWUP_CFG_MAIN_AREA_ADDR_L                   (0xFFE00000U)      /* Main area start address     */
#define FWUP_CFG_BUF_AREA_ADDR_L                    (0xFFEF8000U)      /* Buffer area start address   */
#define FWUP_CFG_AREA_SIZE                          (0xF8000U)         /* Install area size           */

/* Internal flash */
#define FWUP_CFG_CF_BLK_SIZE                        (0x8000U)          /* Code flash block size       */
#define FWUP_CFG_CF_W_UNIT_SIZE                     (128U)             /* Code flash write unit size  */

/* External flash */
#define FWUP_CFG_EXT_BUF_AREA_ADDR_L                (0x00000U)         /* External Buffer area Start address */
#define FWUP_CFG_EXT_BUF_AREA_BLK_SIZE              (4096U)            /* Block(Sector) size  */

/* Data flash */
#define FWUP_CFG_DF_ADDR_L                          (0x00100000)       /* DF Start address    */
#define FWUP_CFG_DF_BLK_SIZE                        (64U)              /* DF Block size       */
#define FWUP_CFG_DF_NUM_BLKS                        (512U)             /* DF number of blocks */

/* FWUP v1 compatible */
#define FWUP_CFG_FWUPV1_COMPATIBLE                  (0)                /* 0:Disable, 1:Enable */

/* Select the algorithm of signature verification.
    0 = ECDSA. (default)
    1 = SHA256
*/
#define FWUP_CFG_SIGNATURE_VERIFICATION             (0)

/* Disable Printf Output Setting.
    Disables the character output by printf to the terminal software.
    0 = Enable (default)
    1 = Disable.
 */
#define FWUP_CFG_PRINTF_DISABLE                     (0)

#endif /* R_FWUP_CONFIG_H */
