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
 * File Name    : r_cellular_dnsquery.c
 * Description  : Function to execute DNS query.
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

/************************************************************************
 * Function Name  @fn            R_CELLULAR_DnsQuery
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_DnsQuery(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_domain_name,
                                    uint32_t * const p_ip_address)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if ((NULL == p_ctrl) || (NULL == p_domain_name) || (NULL == p_ip_address))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else if (CELLULAR_MAX_HOSTNAME_LENGTH < strlen((const char *)p_domain_name)) //(const uint8_t *) ->(const char *)
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else if (CELLULAR_SYSTEM_OPEN == p_ctrl->system_state)
        {
            ret =  CELLULAR_ERR_NOT_CONNECT;
        }
        else
        {
            R_BSP_NOP();
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            ret = atc_sqndnslkup(p_ctrl, p_domain_name);

            if (CELLULAR_SUCCESS == ret)
            {
                *p_ip_address = ((uint32_t)(p_ctrl->dns_address[0]) << 24) | //(uint8_t)->(uint32_t)
                                ((uint32_t)(p_ctrl->dns_address[1]) << 16) | //(uint8_t)->(uint32_t)
                                ((uint32_t)(p_ctrl->dns_address[2]) << 8) |  //(uint8_t)->(uint32_t)
                                ((uint32_t)p_ctrl->dns_address[3]);          //(uint8_t)->(uint32_t)
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
 * End of function R_CELLULAR_DnsQuery
 *********************************************************************************************************************/
