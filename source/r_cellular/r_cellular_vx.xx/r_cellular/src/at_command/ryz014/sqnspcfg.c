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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : sqnspcfg.c
 * Description  : Function to execute the AT command (SQNSPCFG).
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : xx.xx.xxxx 1.00     First Release
 *         : 02.09.2021 1.01     Fixed reset timing
 *         : 21.10.2021 1.02     Support for Azure RTOS
 *                               Support for GCC for Renesas GNURX Toolchain
 *         : 15.11.2021 1.03     Improved receiving behavior, removed socket buffers
 *         : 24.01.2022 1.04     R_CELLULAR_SetPSM and R_CELLULAR_SetEDRX have been added as new APIs
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

#if (CELLULAR_IMPLEMENT_TYPE == 'B')
/*************************************************************************************************
 * Function Name  @fn            atc_sqnspcfg
 ************************************************************************************************/
e_cellular_err_t atc_sqnspcfg(st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t security_profile_id,
                                    const e_cellular_cert_validate_level_t cert_valid_level,
                                    const uint8_t ca_certificate_id, 
                                    const uint8_t client_certificate_id, 
                                    const uint8_t client_privatekey_id)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_atc_t at_ret = CELLULAR_ATC_OK;
    uint8_t str[5][4] = {0};

    sprintf((char *)str[0], "%d", security_profile_id);                   // (uint8_t *)->(char *)
    sprintf((char *)str[1], "%d", cert_valid_level);        // (uint8_t *)->(char *)
    if (CELLULAR_MAX_NVM_CERTIFICATE_INDEX >= ca_certificate_id)
    {
        sprintf((char *)str[2], "%d", ca_certificate_id);   // (uint8_t *)->(char *)
    }
    else
    {
        str[2][0] = '\0';
    }
    if (CELLULAR_MAX_NVM_CERTIFICATE_INDEX >= client_certificate_id)
    {
        sprintf((char *)str[3], "%d", client_certificate_id); // (uint8_t *)->(char *)
    }
    else
    {
        str[3][0] = '\0';
    }
    if (CELLULAR_MAX_NVM_CERTIFICATE_INDEX >= client_privatekey_id)
    {
        sprintf((char *)str[4], "%d", client_privatekey_id); // (uint8_t *)->(char *) / (uint32_t)->(uint8_t)
    }
    else
    {
        str[4][0] = '\0';
    }

    const uint8_t * const p_command_arg[CELLULAR_MAX_ARG_COUNT] =
                            {str[0], str[1], str[2], str[3], str[4]};

    atc_generate(p_ctrl->sci_ctrl.atc_buff,
        (const uint8_t *)&gp_at_command[ATC_CONFIG_SSL_PROFILE][0], // (const uint8_t *const *)->(const uint8_t **)
            (const uint8_t **)&p_command_arg);                   // (const uint8_t *const *)->(const uint8_t **)

    at_ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_CONFIG_SSL_PROFILE);

    if (CELLULAR_ATC_OK != at_ret)
    {
        ret = CELLULAR_ERR_MODULE_COM;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnspcfg
 *********************************************************************************************************************/
#endif /* (CELLULAR_IMPLEMENT_TYPE == 'B') */
