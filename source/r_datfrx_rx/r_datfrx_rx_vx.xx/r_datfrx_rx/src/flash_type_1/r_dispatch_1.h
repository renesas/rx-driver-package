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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_dispatch_1.h
* Version      : 2.00
* Description  : DATFRX interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_datfrx_rx_config.h"
#include "r_flash_rx_if.h"
#include "r_flash_dm_private.h"

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_DISPATCH_1_H
#define R_DISPATCH_1_H

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables
*************************************************************************************************/
extern uint32_t g_flash_dm_err_flg;

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
#if(FLASH_TYPE == FLASH_TYPE_1)
/* user callback */
extern p_flash_dm_callback r_flash_dm_callbackfunc;
extern flash_dm_ptr_t      r_flash_dm_drvif_get_dataaddr(uint8_t data_no);
extern uint32_t            r_flash_dm_drvif_get_dataaddr_of_addr(uint8_t data_no);
extern uint32_t            r_flash_dm_drvif_get_datanum(void);
extern uint16_t            r_flash_dm_drvif_get_datasize(uint32_t data_no);
extern void                r_flash_dm_block_set_program_addr(flash_dm_ptr_t p_addr);
extern flash_dm_ptr_t      r_flash_dm_block_get_program_addr(void);
e_flash_dm_status_t        r_flash_dm_dispatch(e_flash_dm_driver_status_t pc);

#endif /* FLASH_TYPE */

#endif /* R_DISPATCH_1_H */

/* End of File */

