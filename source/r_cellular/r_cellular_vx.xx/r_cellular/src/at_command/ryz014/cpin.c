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
 * File Name    : cpin.c
 * Description  : Function to execute the AT command (CPIN).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define URC_CPIN_READY          "READY"
#define URC_CPIN_SIM_LOCK       "SIM PIN"

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
 * Function Name  @fn            atc_cpin
 ************************************************************************************************/
e_cellular_err_t atc_cpin(st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg)
{
    uint8_t          str[CELLULAR_MAX_SIM_PASS_LENGTH+1]   = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    if (NULL == p_cfg)
    {
        strncpy((char *)str, CELLULAR_STRING_CONVERT(CELLULAR_CFG_PIN_CODE), sizeof(str));    // (uint8_t *)->(char *)
    }
    else
    {
        strncpy((char *)str, (char *)p_cfg->sim_pin_code, sizeof(str));  // (uint8_t *)->(char *)
    }

    p_command_arg[0] = str;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_PIN_LOCK_RELEASE], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_PIN_LOCK_RELEASE);

    memset(p_ctrl->sci_ctrl.atc_buff, 0x00, CELLULAR_ATC_BUFF_SIZE);
    memset(str, 0x00, sizeof(str));

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cpin
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_cpin_check
 ************************************************************************************************/
e_cellular_err_t atc_cpin_check(st_cellular_ctrl_t * const p_ctrl)
{
    uint8_t          lock_state[CELLULAR_CPIN_STATUS_LENGTH+1] = {0};
    e_cellular_err_t ret                                       = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_PIN_LOCK_CHECK], NULL);

    p_ctrl->recv_data = lock_state;

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_PIN_LOCK_CHECK);

    if (CELLULAR_SUCCESS == ret)
    {
        if (NULL == strstr((char *)&lock_state, URC_CPIN_READY))//(uint8_t *)->(char *)
        {
            ret = CELLULAR_ERR_MODULE_COM;
        }
    }

    p_ctrl->recv_data = NULL;

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cpin_check
 *********************************************************************************************************************/
