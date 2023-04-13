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
 * File Name    : cgdcont.c
 * Description  : Function to execute the AT command (CGDCONT).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

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
 * Function Name  @fn            atc_cgdcont
 ************************************************************************************************/
e_cellular_err_t atc_cgdcont(st_cellular_ctrl_t * const p_ctrl, const st_cellular_ap_cfg_t * const p_ap_cfg)
{
    uint8_t          str[CELLULAR_MAX_AP_NAME_LENGTH+1]    = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    if (NULL == p_ap_cfg)
    {
        strncpy((char *)str, CELLULAR_STRING_CONVERT(CELLULAR_CFG_AP_NAME), sizeof(str));  // (uint8_t *)->(char *)
    }
    else
    {
        if (CELLULAR_MAX_AP_NAME_LENGTH < strlen((const char *)p_ap_cfg->ap_name))         // (uint8_t *)->(char *)
        {
            ret = CELLULAR_ERR_PARAMETER;
        }
        else
        {
            strncpy((char *)str, (char *)p_ap_cfg->ap_name, sizeof(str));  // (uint8_t *)->(char *)
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        p_command_arg[0] = str;

        atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_AP_CONFIG], p_command_arg);

        ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_AP_CONFIG);

        memset(p_ctrl->sci_ctrl.atc_buff, 0x00, CELLULAR_ATC_BUFF_SIZE);
        memset(str, 0x00, sizeof(str));
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cgdcont
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_cgdcont_check
 ************************************************************************************************/
e_cellular_err_t atc_cgdcont_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_AP_CONFIG_CHECK], NULL);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_AP_CONFIG_CHECK);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_cgdcont_check
 *********************************************************************************************************************/
