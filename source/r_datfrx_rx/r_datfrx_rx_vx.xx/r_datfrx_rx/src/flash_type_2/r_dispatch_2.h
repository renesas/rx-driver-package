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
* Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_dispatch_2.h
* Version      : 2.01
* Description  : DATFRX interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing and Busy check procedure
*************************************************************************************************/

/***********************************************************************************************
Includes   <System Includes> , "Project Includes"
************************************************************************************************/
#include "r_flash_rx_if.h"
#include "r_flash_dm_rx_if.h"                 /* DATFRX I/F definitions              */
#include "r_flash_dm_private.h"               /* DATFRX Private module definitions   */
#include "r_datfrx_rx_config.h"               /* DATFRX Configuration definitions    */

#include <string.h>

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_DISPATCH_2_H
#define R_DISPATCH_2_H

#if(FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)

#include "r_datf_crc.h"

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables
*************************************************************************************************/
/* user callback */
extern p_flash_dm_callback r_flash_dm_callbackfunc;

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
void flash_cb_function(void *event);
void r_flash_dm_info_update_w_pass(void);
void r_flash_dm_info_update_w_error(void);
void r_flash_dm_data_set(uint8_t *p_Dest, const uint8_t *pData, uint16_t data_size);
void r_flash_dm_init_making_chain(uint16_t *erase_chain_block, uint16_t *invalid_chain_block);
void r_flash_dm_info_set(uint16_t *max_serial_no_blk);
void r_flash_dm_erase_block_blank_chk(uint32_t block_addr);
void r_flash_dm_valid_block_blank_chk(uint8_t data_type, uint32_t block_addr);
void r_flash_dm_drvif_read(st_flash_dm_info_t * p_flash_dm_info);
e_flash_dm_status_t r_flash_dm_command(e_dm_cmd_t command_id, flash_res_t *result);
e_flash_dm_status_t r_flash_dm_dispatch(uint8_t pc);
e_flash_dm_status_t r_flash_dm_1st_check(uint16_t criteria);
e_flash_dm_status_t r_flash_dm_init_set(void);
e_flash_dm_status_t r_flash_dm_init_set2(void);
uint8_t r_flash_dm_drvif_open(void);
uint8_t r_flash_dm_drvif_api_flag(void);

#endif /* (FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4) */
#endif /* R_DISPATCH_2_H */

/* End of File */
