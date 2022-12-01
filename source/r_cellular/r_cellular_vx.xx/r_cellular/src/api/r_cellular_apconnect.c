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
 * File Name    : r_cellular_apconnect.c
 * Description  : Function for connecting to an access point.
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
#define CELLULAR_SYNC_RETRY     (60)
#define CELLULAR_SYNC_DELAY     (1000)
#define CELLULAR_INIT_YEAR      (70)
#define CELLULAR_INIT_MONTH     (1)
#define CELLULAR_INIT_DAY       (1)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static e_cellular_err_t cellular_apconnect (st_cellular_ctrl_t * const p_ctrl,
                                                const st_cellular_ap_cfg_t * const p_ap_cfg);
static e_cellular_err_t cellular_sync_check (st_cellular_ctrl_t * const p_ctrl);

/**************************************************************************
 * Function Name  @fn            R_CELLULAR_APConnect
 *************************************************************************/
e_cellular_err_t R_CELLULAR_APConnect(st_cellular_ctrl_t * const p_ctrl, const st_cellular_ap_cfg_t * const p_ap_cfg)
{
    uint32_t preemption = 0;
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semahore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if (NULL == p_ctrl)
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (0 != (p_ctrl->running_api_count % 2))
        {
            ret = CELLULAR_ERR_OTHER_API_RUNNING;
        }
        else if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else if (CELLULAR_SYSTEM_CONNECT == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_ALREADY_CONNECT;
        }
        else
        {
            p_ctrl->running_api_count += 2;
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        semahore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semahore_ret)
        {
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cfun_check(p_ctrl);
            }
            if (CELLULAR_SUCCESS == ret)
            {
                if (CELLULAR_MODULE_OPERATING_LEVEL4 != p_ctrl->module_status)
                {
                    ret = atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
                }
            }
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cgpiaf(p_ctrl);
            }
            if (CELLULAR_SUCCESS == ret)
            {
                ret = atc_cgdcont(p_ctrl, p_ap_cfg);
            }

            /* A && (B || C)*/
            if ((CELLULAR_SUCCESS == ret) &&
                    (((NULL != p_ap_cfg) && (strlen((char *)p_ap_cfg->ap_user_name)) && //(uint8_t *) -> (char*)
                    (strlen((char *)p_ap_cfg->ap_pass))) ||                             //(uint8_t *) -> (char*)
                        ((NULL == p_ap_cfg) &&
                        (strlen(CELLULAR_STRING_CONVERT(CELLULAR_CFG_AP_USERID))) &&
                        (strlen(CELLULAR_STRING_CONVERT(CELLULAR_CFG_AP_PASSWORD))))))
            {
                ret = atc_cgauth(p_ctrl, p_ap_cfg);
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
                ret = cellular_apconnect(p_ctrl, p_ap_cfg);
                if (CELLULAR_SUCCESS != ret)
                {
                    atc_cfun(p_ctrl, CELLULAR_MODULE_OPERATING_LEVEL4);
                }
            }
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }

        p_ctrl->running_api_count -= 2;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_APConnect
 *********************************************************************************************************************/

/**************************************************************************
 * Function Name  @fn            cellular_apconnect
 *************************************************************************/
static e_cellular_err_t cellular_apconnect(st_cellular_ctrl_t * const p_ctrl,
                                                const st_cellular_ap_cfg_t * const p_ap_cfg)
{
    uint16_t cgatt_cnt = 0;
    e_cellular_err_t ret;

    if (NULL == p_ap_cfg)
    {
        CELLULAR_LOG_INFO(("Trying access point [%s] connecting.", CELLULAR_STRING_CONVERT(CELLULAR_CFG_AP_NAME)));
    }
    else
    {
        CELLULAR_LOG_INFO(("Trying access point [%s] connecting.", p_ap_cfg->ap_name));
    }

    while (1)
    {
        atc_cgatt_check(p_ctrl);
        if (CELLULAR_SYSTEM_CONNECT == p_ctrl->system_state)
        {
            if (NULL == p_ap_cfg)
            {
                CELLULAR_LOG_INFO(("Established connection to [%s].", CELLULAR_STRING_CONVERT(CELLULAR_CFG_AP_NAME)));
            }
            else
            {
                CELLULAR_LOG_INFO(("Established connection to [%s].", p_ap_cfg->ap_name));
            }
            ret = cellular_sync_check(p_ctrl);

            break;
        }
        else
        {
            CELLULAR_LOG_INFO(("Remaining retry count [%d].", p_ctrl->ap_connect_retry - cgatt_cnt));
            cgatt_cnt++;
            cellular_delay_task(CELLULAR_AP_CONNECT_RETRY_WAIT);
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

/**************************************************************************
 * Function Name  @fn            cellular_sync_check
 *************************************************************************/
static e_cellular_err_t cellular_sync_check(st_cellular_ctrl_t * const p_ctrl)
{
    uint8_t count = 0;
    uint8_t pdpaddr[70] = {0};
    e_cellular_err_t ret = CELLULAR_ERR_MODULE_TIMEOUT;
    st_cellular_datetime_t datetime = {0};

    p_ctrl->recv_data = &datetime;

    while (count < CELLULAR_SYNC_RETRY)
    {
        ret = atc_cclk_check(p_ctrl);

        if ((CELLULAR_INIT_YEAR != datetime.year) ||
                (CELLULAR_INIT_MONTH != datetime.month) || (CELLULAR_INIT_DAY != datetime.day))
        {
            ret = CELLULAR_SUCCESS;
            break;
        }
        else
        {
            count++;
            cellular_delay_task(CELLULAR_SYNC_DELAY);
        }
    }

    p_ctrl->recv_data = NULL;


    if (CELLULAR_SUCCESS == ret)
    {
        p_ctrl->recv_data = pdpaddr;
        ret = atc_cgpaddr(p_ctrl);
        if (CELLULAR_SUCCESS == ret)
        {
            cellular_getpdpaddr(p_ctrl, &p_ctrl->pdp_addr);
        }
        p_ctrl->recv_data = NULL;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_sync_check
 *********************************************************************************************************************/
