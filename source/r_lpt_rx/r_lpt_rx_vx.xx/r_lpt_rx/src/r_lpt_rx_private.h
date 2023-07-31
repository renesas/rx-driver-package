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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lpt_rx_private.h
 * Version      : 3.00
 * Description  : Functions for using Low Power Timer
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.03.2016 1.00    First Release
 *         : 01.07.2016 1.10    Added support for RX130,RX230,RX231
 *         : 04.08.2016 1.11    Added command LPT_CMD_COUNT_RESET to R_LPT_Control()
 *         : 10.06.2020 2.00    Added support for RX23W
 *         : 31.03.2021 3.00    Added function R_LPT_InitChan, R_LPT_SetCMT, R_LPT_FinalChan, R_LPT_InitPWM
 *                              Added command LPT_CMD_PWM_START and LPT_CMD_PWM_STOP to R_LPT_Control()
 *         : 31.05.2023 4.00    Change parameter check definition.
 *********************************************************************************************************************/

/*******************************************************************************
  Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Defines for LPT support */
#include "r_lpt_rx_if.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
#ifndef LPT_PRIVATE_H
#define LPT_PRIVATE_H

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*****************************************************************************
* Function Name: r_lpt_mcu_hardware_lock
* Description  : Attempt to acquire the lock that has been sent in.
* Arguments    : none
* Return Value : true                   ; Successful operation
*              : false                  ; error operation
******************************************************************************/
bool r_lpt_mcu_hardware_lock (void);

/*****************************************************************************
* Function Name: r_lpt_mcu_hardware_unlock
* Description  : Release hold on lock.
* Arguments    : none
* Return Value : none
******************************************************************************/
void r_lpt_mcu_hardware_unlock (void);

/*****************************************************************************
* Function Name: lpt_open
* Description  : Initialization for using the LPT.
* Arguments    : period
* Return Value : LPT_SUCCESS
*                LPT_ERR_INVALID_ARG
******************************************************************************/
lpt_err_t lpt_open (uint32_t const period);

/*****************************************************************************
* Function Name: lpt_close
* Description  : Releases the LPT function.
* Arguments    : None
* Return Value : None
******************************************************************************/
void lpt_close (void);

/*****************************************************************************
* Function Name: lpt_start
* Description  : Starts the LPT.
* Arguments    : None
* Return Value : None
******************************************************************************/
void lpt_start (void);

/*****************************************************************************
* Function Name: lpt_stop
* Description  : Stops the LPT.
* Arguments    : None
* Return Value : None
******************************************************************************/
void lpt_stop (void);

/*****************************************************************************
* Function Name: lpt_count_reset
* Description  : Reset the LPT.
* Arguments    : None
* Return Value : LPT_SUCCESS
*                LPT_ERR_CONDITION_NOT_ERR
******************************************************************************/
lpt_err_t lpt_count_reset (void);

/* PWM feature supported from LPTa */
#if (LPT_VERSION == 0x0061)
/*****************************************************************************
* Function Name: lpt_pwm_start
* Description  : Starts the PWM output.
* Arguments    : None
* Return Value : LPT_SUCCESS
*                LPT_ERR_CONDITION_NOT_MET
******************************************************************************/
lpt_err_t lpt_pwm_start (void);

/*****************************************************************************
* Function Name: lpt_pwm_stop
* Description  : Stops the PWM output.
* Arguments    : None
* Return Value : LPT_SUCCESS
*                LPT_ERR_CONDITION_NOT_MET
******************************************************************************/
lpt_err_t lpt_pwm_stop (void);
#endif /* (LPT_VERSION == 0x0061) */

/*****************************************************************************
* Function Name: lpt_initchan
* Description  : Initialize the LPT configuration.
* Arguments    : chan
*                cmt_period
* Return Value : LPT_SUCCESS
*                LPT_ERR_INVALID_ARG
*                LPT_ERR_CONDITION_NOT_MET
*                LPT_ERR_INVALID_CH
******************************************************************************/
lpt_err_t lpt_initchan (lpt_ch_t chan, uint32_t const cmt_period);

/*****************************************************************************
* Function Name: lpt_setcmt
* Description  : Set the value of compare match 0 or 1.
* Arguments    : chan
*                cmt_period
* Return Value : LPT_SUCCESS
*                LPT_ERR_INVALID_ARG
*                LPT_ERR_CONDITION_NOT_MET
*                LPT_ERR_INVALID_CH
******************************************************************************/
lpt_err_t lpt_setcmt (lpt_ch_t chan, uint32_t const cmt_period);

/*****************************************************************************
* Function Name: lpt_finalchan
* Description  : Finalize the LPT configuration.
* Arguments    : chan
* Return Value : LPT_SUCCESS
*                LPT_ERR_INVALID_ARG
*                LPT_ERR_CONDITION_NOT_MET
*                LPT_ERR_INVALID_CH
******************************************************************************/
lpt_err_t lpt_finalchan (lpt_ch_t chan);

/* PWM feature supported from LPTa */
#if (LPT_VERSION == 0x0061)
/*****************************************************************************
* Function Name: lpt_initpwm
* Description  : Sets the PWM configuration.
* Arguments    : chan
*                p_config
* Return Value : LPT_SUCCESS
*                LPT_ERR_INVALID_CH
*                LPT_ERR_CONDITION_NOT_MET
******************************************************************************/
lpt_err_t lpt_initpwm (lpt_ch_t chan, lpt_pwm_cfg_t * const p_config);
#endif /* (LPT_VERSION == 0x0061) */

#endif /* LPT_PRIVATE_H */
/**********************************************************************************************************************
 * End of File
 *********************************************************************************************************************/
