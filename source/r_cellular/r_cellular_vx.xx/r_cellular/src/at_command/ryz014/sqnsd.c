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
 * File Name    : sqnsd.c
 * Description  : Function to execute the AT command (SQNSD).
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

/*************************************************************************************************
 * Function Name  @fn            atc_sqnsd
 ************************************************************************************************/
e_cellular_err_t atc_sqnsd(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                            const uint32_t ip_address, const uint16_t port)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_atc_t at_ret = CELLULAR_ATC_OK;
    uint8_t str[CELLULAR_MAX_ARG_COUNT][10] = {0};

    sprintf((char *)str[0], "%d", socket_no);                   // (uint8_t *)->(char *)
    if (CELLULAR_SOCKET_IP_PROTOCOL_TCP == p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].protocol)
    {
        sprintf((char *)str[1], "%d", 0);                       // (uint8_t *)->(char *)
    }
    else
    {
        sprintf((char *)str[1], "%d", 1);                       // (uint8_t *)->(char *)
    }
    sprintf((char *)str[2], "%u", port);                        // (uint8_t *)->(char *)
    sprintf((char *)str[3], "%d", (uint8_t)(ip_address >> 24)); // (uint8_t *)->(char *) / (uint32_t)->(uint8_t)
    sprintf((char *)str[4], "%d", (uint8_t)(ip_address >> 16)); // (uint8_t *)->(char *) / (uint32_t)->(uint8_t)
    sprintf((char *)str[5], "%d", (uint8_t)(ip_address >> 8));  // (uint8_t *)->(char *) / (uint32_t)->(uint8_t)
    sprintf((char *)str[6], "%d", (uint8_t)(ip_address));       // (uint8_t *)->(char *) / (uint32_t)->(uint8_t)
    sprintf((char *)str[7], "%u", port);                        // (uint8_t *)->(char *)

    const uint8_t * const p_command_arg[CELLULAR_MAX_ARG_COUNT] =
                            {str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]};

    atc_generate(p_ctrl->sci_ctrl.atc_buff,
        (const uint8_t *)&gp_at_command[ATC_CONNECT_SOCKET][0], // (const uint8_t *const *)->(const uint8_t **)
            (const uint8_t **)&p_command_arg);                  // (const uint8_t *const *)->(const uint8_t **)

    if (p_ctrl->sci_ctrl.atc_timeout >
        ((p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout * 100)
                + CELLULAR_SOCKETCONNECT_DELAY))
    {
        at_ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_CONNECT_SOCKET);
    }
    else
    {
        at_ret = cellular_execute_at_command(p_ctrl,
                    ((uint32_t)p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout * 100)
                        + CELLULAR_SOCKETCONNECT_DELAY, ATC_RETURN_OK, ATC_CONNECT_SOCKET);
    }

    if (CELLULAR_ATC_OK != at_ret)
    {
        ret = CELLULAR_ERR_MODULE_COM;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsd
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_sqnsd_host
 ************************************************************************************************/
e_cellular_err_t atc_sqnsd_host(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                            const uint8_t * p_hostname, const uint16_t port)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_atc_t at_ret = CELLULAR_ATC_OK;
    uint8_t str[4][10] = {0};

    sprintf((char *)str[0], "%d", socket_no);                   // (uint8_t *)->(char *)
    if (CELLULAR_SOCKET_IP_PROTOCOL_TCP == p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].protocol)
    {
        sprintf((char *)str[1], "%d", 0);                       // (uint8_t *)->(char *)
    }
    else
    {
        sprintf((char *)str[1], "%d", 1);                       // (uint8_t *)->(char *)
    }
    sprintf((char *)str[2], "%u", port);                        // (uint8_t *)->(char *)
    sprintf((char *)str[3], "%u", port);                        // (uint8_t *)->(char *)

    const uint8_t * const p_command_arg[CELLULAR_MAX_ARG_COUNT] = {str[0], str[1], str[2], p_hostname, str[3]};

    atc_generate(p_ctrl->sci_ctrl.atc_buff,
        (const uint8_t *)&gp_at_command[ATC_CONNECT_SOCKET_TOHOST][0], // (const uint8_t *const *)->(const uint8_t **)
            (const uint8_t **)&p_command_arg);                         // (const uint8_t *const *)->(const uint8_t **)

    if (p_ctrl->sci_ctrl.atc_timeout >
        ((p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout * 100)
                + CELLULAR_SOCKETCONNECT_DELAY))
    {
        at_ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_CONNECT_SOCKET);
    }
    else
    {
        at_ret = cellular_execute_at_command(p_ctrl,
                    ((uint32_t)p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout * 100)
                        + CELLULAR_SOCKETCONNECT_DELAY,
                        ATC_RETURN_OK, ATC_CONNECT_SOCKET_TOHOST);
    }

    if (CELLULAR_ATC_OK != at_ret)
    {
        ret = CELLULAR_ERR_MODULE_COM;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnsd_host
 *********************************************************************************************************************/

