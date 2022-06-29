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
 * File Name    : r_cellular_apconnect.c
 * Description  : Function for connecting to an access point.
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
static e_cellular_err_t cellular_apconnect (st_cellular_ctrl_t * const p_ctrl);

/**************************************************************************
 * Function Name  @fn            R_CELLULAR_APConnect
 *************************************************************************/
e_cellular_err_t R_CELLULAR_APConnect(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semahore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if (NULL == p_ctrl)
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else if (CELLULAR_SYSTEM_CONNECT == p_ctrl->system_state)
        {
            ret =  CELLULAR_ERR_ALREADY_CONNECT;
        }
        else
        {
            R_BSP_NOP();
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        semahore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semahore_ret)
        {
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cgdcont(p_ctrl);
            }

            if ((CELLULAR_SUCCESS == ret) &&
                    (strlen((char *)p_ctrl->ap_user_name) && strlen((char *)p_ctrl->ap_pass)))    //(&uint8_t[]->char *)
            {
                ret = atc_cgauth(p_ctrl);
            }

            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cfun_check(p_ctrl);
            }

            if (CELLULAR_SUCCESS == ret)
            {
                if (CELLULAR_MODULE_OPERATING_LEVEL1 != p_ctrl->module_status)
                {
                    ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL1);
                }
            }
            if (CELLULAR_SUCCESS == ret)
            {
                ret = cellular_apconnect(p_ctrl);
            }
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
 * End of function R_CELLULAR_APConnect
 *********************************************************************************************************************/

/**************************************************************************
 * Function Name  @fn            cellular_apconnect
 *************************************************************************/
static e_cellular_err_t cellular_apconnect(st_cellular_ctrl_t * const p_ctrl)
{
    uint16_t cgatt_cnt = 0;
    e_cellular_err_t ret;

    CELLULAR_LOG_INFO(("Trying access point [%s] connecting.", p_ctrl->ap_name));

    while (1)
    {
        ret = atc_cgatt_check(p_ctrl);
        if (CELLULAR_SUCCESS != ret)
        {
            CELLULAR_LOG_INFO(("Remaining retry count [%d].", p_ctrl->ap_connect_retry - cgatt_cnt));
            cgatt_cnt++;
            atc_cgatt(p_ctrl, ATC_AP_CONNECT);
            cellular_delay_task(CELLULAR_AP_CONNECT_RETRY_WAIT);
        }

        if (CELLULAR_SYSTEM_CONNECT == p_ctrl->system_state)
        {
            CELLULAR_LOG_INFO(("Established connection to [%s].", p_ctrl->ap_name));
            break;
        }

        if (cgatt_cnt > p_ctrl->ap_connect_retry)
        {
            ret = CELLULAR_ERR_MODULE_COM;
            break;
        }
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_apconnect
 *********************************************************************************************************************/
