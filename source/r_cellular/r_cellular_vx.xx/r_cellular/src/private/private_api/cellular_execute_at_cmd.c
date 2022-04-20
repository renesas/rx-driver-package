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
 * File Name    : cellular_execute_at_cmd.c
 * Description  : Function to send an AT command to a module.
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

/****************************************************************************
 * Function Name  @fn            cellular_execute_at_command
 ***************************************************************************/
e_cellular_err_atc_t cellular_execute_at_command(st_cellular_ctrl_t * const p_ctrl, const uint32_t timeout_ms,
                                                        const e_cellular_atc_return_t expect_code,
                                                            const e_atc_list_t command)
{
    sci_err_t                   sci_ret = SCI_SUCCESS;
    e_cellular_timeout_check_t  timeout_ret = CELLULAR_NOT_TIMEOUT;
    e_cellular_err_atc_t        ret = CELLULAR_ATC_OK;
    e_cellular_atc_return_t     res = ATC_RETURN_NONE;

    if (NULL == p_ctrl)
    {
        ret = CELLULAR_ATC_ERR_PARAMETER;
    }

    if (CELLULAR_ATC_OK == ret)
    {
        cellular_bytetimeout_init(&p_ctrl->sci_ctrl.timeout_ctrl, timeout_ms);
        cellular_set_atc_number(p_ctrl, command);
        p_ctrl->sci_ctrl.tx_end_flg = CELLULAR_TX_END_FLAG_OFF;

        sci_ret = R_SCI_Send(p_ctrl->sci_ctrl.sci_hdl, p_ctrl->sci_ctrl.atc_buff,
                                strlen((const char *)p_ctrl->sci_ctrl.atc_buff)); // (&uint8_t[])->(char*)

        if (SCI_SUCCESS == sci_ret)
        {
            while (1)
            {
                if (CELLULAR_TX_END_FLAG_ON == p_ctrl->sci_ctrl.tx_end_flg)
                {
                    break;
                }

                timeout_ret = cellular_check_bytetimeout(&p_ctrl->sci_ctrl.timeout_ctrl);
                if (CELLULAR_TIMEOUT == timeout_ret)
                {
                    ret = CELLULAR_ATC_ERR_TIMEOUT;
                    goto cellular_execute_at_command_fail;
                }
            }
        }
        else
        {
            ret = CELLULAR_ATC_ERR_MODULE_COM;
            goto cellular_execute_at_command_fail;
        }
    }

    if (CELLULAR_ATC_OK == ret)
    {
        while (1)
        {
            res = cellular_get_atc_response(p_ctrl);
            if (ATC_RETURN_NONE != res)
            {
                if (res == expect_code)
                {
                    break;
                }
                else
                {
                    ret = CELLULAR_ATC_ERR_COMPARE;
                    goto cellular_execute_at_command_fail;
                }
            }

            timeout_ret = cellular_check_bytetimeout(&p_ctrl->sci_ctrl.timeout_ctrl);
            if (CELLULAR_TIMEOUT == timeout_ret)
            {
                ret = CELLULAR_ATC_ERR_TIMEOUT;
                goto cellular_execute_at_command_fail;
            }
        }
    }

cellular_execute_at_command_fail:
    {
        return ret;
    }
}
/**********************************************************************************************************************
 * End of function cellular_execute_at_command
 *********************************************************************************************************************/
