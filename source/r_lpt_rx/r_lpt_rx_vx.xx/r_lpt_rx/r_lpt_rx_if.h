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
 * File Name    : r_lpt_rx_if.h
 * Description  : Functions for using Low Power Timer Module 
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           01.03.2016 1.00    First Release
 *           01.07.2016 1.10    Added support for RX130,RX230,RX231
 *                              Added Wait time to lpt_close() for divider circuit and counter reset
 *           04.08.2016 1.11    Added command LPT_CMD_COUNT_RESET to R_LPT_Control()
 *           01.10.2017 1.20    Added support for RX130-512KB
 *           31.10.2017 1.21    Added the demo for RX113, RX231
 *           16.11.2018 1.22    Added XML document number
 *           01.04.2019 1.23    Changed Minor version to 1.23.
  ***********************************************************************************************************************/
#ifndef LPT_IF_H
#define LPT_IF_H

/*******************************************************************************
  Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "./src/r_lpt_rx_platform.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
/* Version Number of API. */
#define LPT_VERSION_MAJOR  (1)
#define LPT_VERSION_MINOR  (23)

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
    LPT_ERR_CONDITION_NOT_MET  /* It does not meet the execution of conditions */
} lpt_err_t;

/*----- R_LPT_Control() ARGUMENT DEFINITIONS -----*/
/* commands */
typedef enum e_lpt_cmd
{
    LPT_CMD_START,          /* Start LPT count */
    LPT_CMD_STOP,           /* Stop LPT count */
    LPT_CMD_COUNT_RESET     /* Reset LPT count */
} lpt_cmd_t;

/*******************************************************************************
  Public Functions
*******************************************************************************/
lpt_err_t R_LPT_Open       (uint32_t const period);
lpt_err_t R_LPT_Close      (void);
lpt_err_t R_LPT_Control    (lpt_cmd_t const cmd);
uint32_t  R_LPT_GetVersion (void);

#endif /* LPT_IF_H */

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
