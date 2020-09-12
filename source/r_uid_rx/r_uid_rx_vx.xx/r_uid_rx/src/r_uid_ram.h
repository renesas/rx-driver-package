/*******************************************************************************
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
*******************************************************************************/
/*******************************************************************************
 * File Name    : r_uid_ram.h
 * Description  : This is a private header file used internally by the Unique ID Read API module.
 * Limitation   : none
 ******************************************************************************/
/*******************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 01.12.2014 1.00     First Release
 ******************************************************************************/

#ifndef RX_UID_RAM_PRIVATE_HEADER_FILE
#define RX_UID_RAM_PRIVATE_HEADER_FILE

/*******************************************************************************
 Macro definitions
 ******************************************************************************/
#define ENABLE            (1)

/*==== Wait Process definition ====*/
#define WAIT_TDIS       (3)
#define WAIT_TMS_MID    (4)
#define WAIT_TMS_HIGH   (6)
#define WAIT_TDSTOP     (6)

#define MHZ             (1000000)
#define KHZ             (1000)
#define FCLK_MHZ        ((BSP_FCLK_HZ + 999999) / MHZ)
#define ICLK_KHZ        (BSP_ICLK_HZ / KHZ)

/*  flash mode definition (FENTRYR Register setting)*/
#define FENTRYR_CODEFLASH_PE_MODE   (0xAA01)
#define FENTRYR_READ_MODE           (0xAA00)

/*  flash mode definition (FPMCR Register setting)*/
#define DISCHARGE_1                 (0x12)
#define DISCHARGE_2                 (0x92)
#define CODEFLASH_PE_MODE           (0x82)
#define READ_MODE                   (0x08)
#define LVPE_MODE                   (0x40)

/*  operation definition (FCR Register setting)*/
#define FCR_IDREAD       (0x85)
#define FCR_NEXT_IDREAD  (0x95)
#define FCR_CLEAR        (0x00)

/* Unique ID Address */
#define UNIQUE_ID_START  (0x850)
#define UNIQUE_ID_END    (0x86F)

#define OPCCR_HIGH_SPEED_MODE       (0x00)

/*******************************************************************************
 Frequency Check of FlashIF clock (FCLK)
 ******************************************************************************/
#define FCLK_MAX         (32000000)
#define FCLK_MIN         (1000000)

#if ((FCLK_MIN > BSP_FCLK_HZ) || (FCLK_MAX < BSP_FCLK_HZ))
#error "ERROR - FCLK_MHZ - FlashIF clock is out of range. "
#endif


/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
uid_err_t uid_read_ram(uint8_t *);

#endif /* RX_UID_RAM_PRIVATE_HEADER_FILE */

/* end of file */
