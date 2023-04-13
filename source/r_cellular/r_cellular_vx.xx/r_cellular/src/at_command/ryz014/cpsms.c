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
 * File Name    : cpsms.c
 * Description  : Function to execute the AT command (CPSMS).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CHAR_0  (0x30u)
#define BIT_1   (0x01u << 0)
#define BIT_2   (0x01u << 1)
#define BIT_4   (0x01u << 2)
#define BIT_8   (0x01u << 3)
#define BIT_16  (0x01u << 4)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_cpsms
 ************************************************************************************************/
e_cellular_err_t atc_cpsms(st_cellular_ctrl_t * const p_ctrl, const st_cellular_psm_config_t * const p_config)
{
    uint8_t          value;
    uint8_t          str[3][9]                             = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    str[0][0] = (uint8_t)((uint8_t)p_config->psm_mode + (uint8_t)CHAR_0);   //cast

    value     = (uint8_t)(p_config->tau_cycle);                //cast
    str[1][0] = (uint8_t)(((value & BIT_4) >> 2) + CHAR_0);    //cast
    str[1][1] = (uint8_t)(((value & BIT_2) >> 1) + CHAR_0);    //cast
    str[1][2] = (uint8_t)((value & BIT_1) + CHAR_0);           //cast

    value     = (uint8_t)(p_config->tau_multiplier);           //cast
    str[1][3] = (uint8_t)(((value & BIT_16) >> 4) + CHAR_0);   //cast
    str[1][4] = (uint8_t)(((value & BIT_8) >> 3) + CHAR_0);    //cast
    str[1][5] = (uint8_t)(((value & BIT_4) >> 2) + CHAR_0);    //cast
    str[1][6] = (uint8_t)(((value & BIT_2) >> 1) + CHAR_0);    //cast
    str[1][7] = (uint8_t)((value & BIT_1) + CHAR_0);           //cast

    value     = (uint8_t)(p_config->active_cycle);             //cast
    str[2][0] = (uint8_t)(((value & BIT_4) >> 2) + CHAR_0);    //cast
    str[2][1] = (uint8_t)(((value & BIT_2) >> 1) + CHAR_0);    //cast
    str[2][2] = (uint8_t)((value & BIT_1) + CHAR_0);           //cast

    value     = (uint8_t)(p_config->active_multiplier);        //cast
    str[2][3] = (uint8_t)(((value & BIT_16) >> 4) + CHAR_0);   //cast
    str[2][4] = (uint8_t)(((value & BIT_8) >> 3) + CHAR_0);    //cast
    str[2][5] = (uint8_t)(((value & BIT_4) >> 2) + CHAR_0);    //cast
    str[2][6] = (uint8_t)(((value & BIT_2) >> 1) + CHAR_0);    //cast
    str[2][7] = (uint8_t)((value & BIT_1) + CHAR_0);           //cast

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_SET_PSM], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SET_PSM);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cpsms
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_cpsms_check
 ************************************************************************************************/
e_cellular_err_t atc_cpsms_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_GET_PSM], NULL);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_GET_PSM);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cpsms_check
 *********************************************************************************************************************/
