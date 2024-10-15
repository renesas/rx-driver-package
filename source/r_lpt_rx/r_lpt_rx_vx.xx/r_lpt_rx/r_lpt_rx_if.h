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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.    
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lpt_rx_if.h
 * Description  : Functions for using Low Power Timer Module 
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           01.03.2016 1.00    First Release
 *           01.07.2016 1.10    Added support for RX130,RX230,RX231
 *                              Added Wait time to lpt_close() for divider circuit and counter reset
 *           04.08.2016 1.11    Added command LPT_CMD_COUNT_RESET to R_LPT_Control()
 *           01.10.2017 1.20    Added support for RX130-512KB
 *           31.10.2017 1.21    Added the demo for RX113, RX231
 *           16.11.2018 1.22    Added XML document number
 *           01.04.2019 1.23    Changed Minor version to 1.23.
 *           10.06.2020 2.00    Added support for RX23W
 *           25.09.2020 2.01    Added support for RX140
 *           31.03.2021 3.00    Added function R_LPT_InitChan, R_LPT_SetCMT, R_LPT_FinalChan, R_LPT_InitPWM
 *                              Added command LPT_CMD_PWM_START and LPT_CMD_PWM_STOP to R_LPT_Control()
 *           31.05.2023 4.00    Added support for RX23E-B
 *           31.07.2024 5.00    Added support for RX260, RX261
 *********************************************************************************************************************/

/*******************************************************************************
  Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "./src/r_lpt_rx_platform.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
#ifndef LPT_IF_H
#define LPT_IF_H

/* Version Number of API. */
#define LPT_VERSION_MAJOR  (5)
#define LPT_VERSION_MINOR  (00)

/* Wait time for Call R_LPT_Close after Called R_LPT_Open immediately */
#define LPT_RSTWAIT_USEC   (LPT_LPTSRCCLK_CYC)

/*******************************************************************************
  Typedef definitions
*******************************************************************************/
/* API error codes */
typedef enum e_lpt_err
{
    LPT_SUCCESS=0,
    LPT_ERR_LOCK_FUNC,         /* Hardware Lock */
    LPT_ERR_INVALID_ARG,       /* argument is not one of the predefined values */
    LPT_ERR_CONDITION_NOT_MET, /* It does not meet the execution of conditions */
    LPT_ERR_INVALID_CH,        /* Channel is invalid. */
    LPT_ERR_NULL_PTR           /* Received null ptr. missing required argument */
} lpt_err_t;

/*----- R_LPT_Control() ARGUMENT DEFINITIONS -----*/
/* commands */
typedef enum e_lpt_cmd
{
    LPT_CMD_START,          /* Start LPT count */
    LPT_CMD_STOP,           /* Stop LPT count */
    LPT_CMD_COUNT_RESET,    /* Reset LPT count */
    LPT_CMD_PWM_START,      /* Start PWM output */
    LPT_CMD_PWM_STOP        /* Stop PWM output */
} lpt_cmd_t;

/* LPT channel numbers */
typedef enum e_lpt_ch
{
    LPT_CH0=0,    /* LPT channel 0 */
    LPT_CH1,      /* LPT channel 1 */
    LPT_NUM_CH
} lpt_ch_t;

/* LPT PWM output polarity*/
typedef enum e_lpt_pwm_polarity
{
    output_polarity_low=0,    /* Output polarity low */
    output_polarity_high      /* Output polarity high */
} lpt_pwm_polarity_t;

/* LPT PWM output level*/
typedef enum e_lpt_pwm_level
{
    output_level_low=0,        /* Output level low */
    output_level_high          /* Output level high */
} lpt_pwm_level_t;

typedef struct st_lpt_pwm_cfg
{
    lpt_pwm_polarity_t output_polarity;    /* Output polarity */
    lpt_pwm_level_t output_level;          /* Output level */
}lpt_pwm_cfg_t;

/*******************************************************************************
  Public Functions
*******************************************************************************/
/******************************************************************************************************************
* Function Name: R_LPT_Open
* Description  : The function initializes the LPT FIT module. This function must be called before calling any
*                other API functions.
* Arguments    : lpt_period
* Return Value : LPT_SUCCESS: Processing completed successfully.
                 LPT_ERR_LOCK_FUNC: Operating. LPT has been used.
                 LPT_ERR_INVALID_ARG: Argument has an invalid value.
******************************************************************************************************************/
lpt_err_t R_LPT_Open       (uint32_t const lpt_period);

/******************************************************************************************************************
* Function Name: R_LPT_Close
* Description  : This function performs processing to stop the LPT.
* Arguments    : None
* Return Value : LPT_SUCCESS: Processing completed successfully.
******************************************************************************************************************/
lpt_err_t R_LPT_Close      (void);

/******************************************************************************************************************
* Function Name: R_LPT_Control
* Description  : This function performs processing to start, stop, or reset LPT count and start and stop PWM output.
* Arguments    : cmd
* Return Value : LPT_SUCCESS: Processing completed successfully.
                 LPT_ERR_INVALID_ARG: Argument has an invalid value.
                 LPT_ERR_CONDITION_NOT_MET: Condition not met.
                 LPT_ERR_INVALID_CH: Selected channel is invalid.
******************************************************************************************************************/
lpt_err_t R_LPT_Control    (lpt_cmd_t const cmd);

/******************************************************************************************************************
* Function Name: R_LPT_InitChan
* Description  : This function enable compare match and sets the value of LPT compare match.
* Arguments    : chan
*                cmt_period
* Return Value : LPT_SUCCESS: Processing completed successfully.
                 LPT_ERR_INVALID_ARG: Argument has an invalid value.
                 LPT_ERR_CONDITION_NOT_MET: Condition not met.
******************************************************************************************************************/
lpt_err_t R_LPT_InitChan    (lpt_ch_t chan, uint32_t const cmt_period);

/******************************************************************************************************************
* Function Name: R_LPT_SetCMT
* Description  : This function set the value of LPT compare match.
* Arguments    : chan
*                cmt_period
* Return Value : LPT_SUCCESS: Processing completed successfully.
                 LPT_ERR_INVALID_ARG: Argument has an invalid value.
                 LPT_ERR_CONDITION_NOT_MET: Condition not met.
                 LPT_ERR_INVALID_CH: Selected channel is invalid.
******************************************************************************************************************/
lpt_err_t R_LPT_SetCMT    (lpt_ch_t chan, uint32_t const cmt_period);

/******************************************************************************************************************
* Function Name: R_LPT_FinalChan
* Description  : This function finalize compare match.
* Arguments    : chan
* Return Value : LPT_SUCCESS: Processing completed successfully.
                 LPT_ERR_CONDITION_NOT_MET: Condition not met.
                 LPT_ERR_INVALID_CH: Selected channel is invalid.
******************************************************************************************************************/
lpt_err_t R_LPT_FinalChan    (lpt_ch_t chan);

/* PWM feature supported from LPTa */
#if (LPT_VERSION == 0x0061)
/******************************************************************************************************************
* Function Name: R_LPT_InitPWM
* Description  : This function sets the PWM configuration.
* Arguments    : chan
*                p_config
* Return Value : LPT_SUCCESS: Processing completed successfully.
                 LPT_ERR_CONDITION_NOT_MET: Condition not met.
                 LPT_ERR_INVALID_CH: Selected channel is invalid.
                 LPT_ERR_NULL_PTR: p_config is NULL.
******************************************************************************************************************/
lpt_err_t R_LPT_InitPWM    (lpt_ch_t chan, lpt_pwm_cfg_t * const p_config);
#endif

/******************************************************************************************************************
* Function Name: R_LPT_GetVersion
* Description  : This function returns the module version.
* Arguments    : None
* Return Value : Version number
******************************************************************************************************************/
uint32_t  R_LPT_GetVersion    (void);

#endif /* LPT_IF_H */

/**********************************************************************************************************************
 * End of File
 *********************************************************************************************************************/
