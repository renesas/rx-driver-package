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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_cellular_configsslprofile.c
 * Description  : Function to configurate security profile.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "at_command.h"

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
/************************************************************************
 * Function Name  @fn            R_CELLULAR_ConfigSSLProfile
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConfigSSLProfile(st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t security_profile_id,
                                    const e_cellular_cert_validate_level_t cert_valid_level,
                                    const uint8_t ca_certificate_id, 
                                    const uint8_t client_certificate_id, 
                                    const uint8_t client_privatekey_id)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if ((NULL == p_ctrl) ||
            (CELLULAR_SECURITY_PROFILE_ID_L > security_profile_id) ||
            (CELLULAR_SECURITY_PROFILE_ID_H < security_profile_id) ||
            ((CELLULAR_NO_CERT_VALIDATE != cert_valid_level) && (CELLULAR_VALIDATE_CERT_EXPDATE != cert_valid_level) &&
            (CELLULAR_VALIDATE_CERT_EXPDATE_CN != cert_valid_level)))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            ret = atc_sqnspcfg(p_ctrl, security_profile_id, cert_valid_level,
                    ca_certificate_id, client_certificate_id, client_privatekey_id);
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_ConfigSSLProfile
 *********************************************************************************************************************/
#endif /* (CELLULAR_IMPLEMENT_TYPE == 'B') */
