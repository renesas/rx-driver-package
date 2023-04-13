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
static void cellular_getip (st_cellular_ctrl_t * const p_ctrl, const uint8_t ip_version,
                                st_cellular_ipaddr_t * const p_addr);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_DnsQuery
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_DnsQuery(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_domain_name,
                                        const uint8_t ip_version, st_cellular_ipaddr_t * const p_addr)
{
    uint8_t                    cnt                         = 0;
    uint8_t                    addr[CELLULAR_DNS_LENGTH+1] = {0};
    uint32_t                   preemption                  = 0;
    e_cellular_err_t           ret                         = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret               = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) || (NULL == p_domain_name) || (NULL == p_addr) ||
            ((CELLULAR_PROTOCOL_IPV4 != ip_version) && (CELLULAR_PROTOCOL_IPV6 != ip_version)))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }

    if (CELLULAR_SUCCESS == ret)
    {
        memset(p_addr, 0, sizeof(st_cellular_ipaddr_t));

        if (0 != (p_ctrl->running_api_count % 2))
        {
            ret = CELLULAR_ERR_OTHER_API_RUNNING;
        }
        else if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else if (CELLULAR_SYSTEM_OPEN == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_CONNECT;
        }
        else
        {
            R_BSP_NOP();
        }

        if (CELLULAR_SUCCESS == ret)
        {
            if (CELLULAR_MAX_HOSTNAME_LENGTH < strlen((const char *)p_domain_name)) //(const uint8_t *)->(const char *)
            {
                ret = CELLULAR_ERR_PARAMETER;
            }
            else if ((CELLULAR_PROTOCOL_IPV6 == ip_version) &&
                    (CELLULAR_IPV6_ADDR_LENGTH > strlen((char *)p_ctrl->pdp_addr.ipv6))) //(uint8_t *)->(char *)
            {
                ret = CELLULAR_ERR_SIM_NOT_SUPPORT_IPV6;
            }
            else
            {
                p_ctrl->running_api_count += 2;
            }
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            p_ctrl->recv_data = addr;
            do
            {
                ret = atc_sqndnslkup(p_ctrl, p_domain_name, ip_version);

                if (CELLULAR_SUCCESS == ret)
                {
                    cellular_getip(p_ctrl, ip_version, p_addr);
                }
                cnt++;
                cellular_delay_task(1000);
            } while (((CELLULAR_PROTOCOL_IPV6 == ip_version) && (10 > cnt) &&
                        (CELLULAR_IPV6_ADDR_LENGTH > strlen((char *)p_addr->ipv6)))); //(uint8_t *)->(char *)
            p_ctrl->recv_data = NULL;

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
 * End of function R_CELLULAR_DnsQuery
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_getip
 ***********************************************************************/
static void cellular_getip(st_cellular_ctrl_t * const p_ctrl, const uint8_t ip_version,
                            st_cellular_ipaddr_t * const p_addr)
{
    uint32_t dns[4] = {0};

    if ((CELLULAR_PROTOCOL_IPV6 != ip_version) || (CELLULAR_IPV6_ADDR_LENGTH > strlen(p_ctrl->recv_data)))
    {
        sscanf((char *)p_ctrl->recv_data,   //(uint8_t *)->(char *)
                "%ld.%ld.%ld.%ld",
                &dns[0], &dns[1], &dns[2], &dns[3]);

        sprintf((char *)p_addr->ipv4,       //(uint8_t *)->(char *)
                "%ld.%ld.%ld.%ld",
                dns[0], dns[1], dns[2], dns[3]);
    }
    else
    {
        strncpy((char *)p_addr->ipv6, (char *)p_ctrl->recv_data, CELLULAR_IPV6_ADDR_LENGTH);    //(uint8_t *)->(char *)
    }
}
/**********************************************************************************************************************
 * End of function cellular_getip
 *********************************************************************************************************************/
