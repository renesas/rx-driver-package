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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpt_rx_private.h
 * Version      : 1.10
 * Description  : Functions for using Low Power Timer
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.03.2016 1.00    First Release
 *         : 01.07.2016 1.10    Added support for RX130,RX230,RX231
 *         : 04.08.2016 1.11    Added command LPT_CMD_COUNT_RESET to R_LPT_Control()
 ***********************************************************************************************************************/
#ifndef LPT_PRIVATE_H
#define LPT_PRIVATE_H

/*******************************************************************************
  Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Defines for LPT support */
#include "r_lpt_rx_if.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
/* Constants to determine whether the parameter checking is enabled or disabled  */
#define LPT_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE & LPT_CFG_PARAM_CHECKING)

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
bool r_lpt_mcu_hardware_lock(void);
void r_lpt_mcu_hardware_unlock(void);
lpt_err_t lpt_open(uint32_t const period);
void lpt_close(void);
void lpt_start(void);
void lpt_stop(void);
lpt_err_t lpt_count_reset(void);

#endif /* LPT_PRIVATE_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
