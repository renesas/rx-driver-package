/**********************************************************************************************************************
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
**********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_flash_type5_if.h
* Description  : This module implements equates/declarations specific to Flash Type 5 MCUs
**********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 24.01.2023 5.00    Support for Flash Type 5 MCUs
**********************************************************************************************************************/

#ifndef R_FLASH_TYPE5_IF_H
#define R_FLASH_TYPE5_IF_H

#include "r_flash_rx_if.h"
#if (FLASH_TYPE == FLASH_TYPE_5)

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_flash_rx.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define BANKSEL_ADDR            (0x00120090)
#define BANKSWP_MASK            (0x00000007)

#define FLASH_FREQ_LO           (4000000)
#define FLASH_FREQ_HI           (60000000)

#define FLASH_FCU_INT_ENABLE    FLASH.FAEINT.BYTE = 0x90;   \
                                FLASH.FRDYIE.BYTE = 0x01;
#define FLASH_FCU_INT_DISABLE   FLASH.FAEINT.BYTE = 0x00;   \
                                FLASH.FRDYIE.BYTE = 0x00;

#define FLASH_ACCESS_WINDOW_END_NEXT_REG_VALUE  (0x1000)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
extern flash_err_t R_CF_SetAccessWindow(flash_access_window_config_t  *pAccessInfo);
extern flash_err_t R_CF_GetAccessWindow(flash_access_window_config_t  *pAccessInfo);
extern uint8_t R_CF_GetCurrentStartupArea(void);
extern flash_err_t R_CF_ToggleStartupArea(void);
extern uint8_t R_CF_GetCurrentSwapState(void);
extern void R_CF_SetCurrentSwapState(uint8_t value);
extern flash_err_t get_cmdlk_err(void);
extern flash_interrupt_event_t get_cmdlk_err_event(void);
#if (FLASH_IN_DUAL_BANK_MODE == 1)
extern flash_err_t flash_toggle_banksel_reg(void);
#endif

#endif // FLASH_TYPE == 5
#endif // R_FLASH_TYPE5_IF_H
