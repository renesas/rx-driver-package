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
 * File Name    : at_command.c
 * Description  : Function to generate AT command.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
const uint8_t g_ryz014_echo_off[]                = RYZ014_ATC_ECHO_OFF;
const uint8_t g_ryz014_function_level_check[]    = RYZ014_ATC_FUNCTION_LEVEL_CHECK;
const uint8_t g_ryz014_function_level[]          = RYZ014_ATC_FUNCTION_LEVEL;
const uint8_t g_ryz014_pin_lock_check[]          = RYZ014_ATC_PIN_LOCK_CHECK;
const uint8_t g_ryz014_pin_lock_release[]        = RYZ014_ATC_PIN_LOCK_RELEASE;
const uint8_t g_ryz014_connect_socket[]          = RYZ014_ATC_CONNECT_SOCKET;
const uint8_t g_ryz014_close_socket[]            = RYZ014_ATC_CLOSE_SOCKET;
const uint8_t g_ryz014_send_socket[]             = RYZ014_ATC_SEND_SCOKET;
const uint8_t g_ryz014_recv_socket[]             = RYZ014_ATC_RECV_SCOKET;
const uint8_t g_ryz014_dns_lookup[]              = RYZ014_ATC_DNS_LOOKUP;
const uint8_t g_ryz014_ap_config[]               = RYZ014_ATC_AP_CONFIG;
const uint8_t g_ryz014_user_config[]             = RYZ014_ATC_USER_CONFIG;
const uint8_t g_ryz014_socket_config_1[]         = RYZ014_ATC_SOCKET_CONFIG_1;
const uint8_t g_ryz014_socket_config_2[]         = RYZ014_ATC_SOCKET_CONFIG_2;
const uint8_t g_ryz014_listening_socket[]        = RYZ014_ATC_LISTENING_SOCKET;
const uint8_t g_ryz014_connect_check[]           = RYZ014_ATC_CONNECT_CHECK;
const uint8_t g_ryz014_set_connect_status[]      = RYZ014_ATC_SET_CONNECT_STATUS;
const uint8_t g_ryz014_shutdown[]                = RYZ014_ATC_SHUTDOWN;
const uint8_t g_ryz014_get_time[]                = RYZ014_ATC_GET_TIME;
const uint8_t g_ryz014_set_time[]                = RYZ014_ATC_SET_TIME;
const uint8_t g_ryz014_reset[]                   = RYZ014_ATC_RESET;
const uint8_t g_ryz014_set_notice_level[]        = RYZ014_ATC_SET_NOTICE_LEVEL;
const uint8_t g_ryz014_get_notice_level[]        = RYZ014_ATC_GET_NOTICE_LEVEL;
const uint8_t g_ryz014_auto_connect[]            = RYZ014_ATC_AUTO_CONNECT;
const uint8_t g_ryz014_auto_connect_check[]      = RYZ014_ATC_AUTO_CONNECT_CHECK;
const uint8_t g_ryz014_sim_st_off[]              = RYZ014_ATC_SIM_ST_OFF;
const uint8_t g_ryz014_get_service_status[]      = RYZ014_ATC_GET_SERVICE_STATUS;
const uint8_t g_ryz014_set_provider[]            = RYZ014_ATC_SET_PROVIDER;
const uint8_t g_ryz014_get_pdn_state[]           = RYZ014_ATC_GET_PDN_STATE;
const uint8_t g_ryz014_activate_pdn[]            = RYZ014_ATC_ACTIVATE_PDN;
const uint8_t g_ryz014_atc_get_ipaddr[]          = RYZ014_ATC_GET_IPADDR;
const uint8_t g_ryz014_atc_get_psm[]             = RYZ014_ATC_GET_PSM;
const uint8_t g_ryz014_atc_set_psm[]             = RYZ014_ATC_SET_PSM;
const uint8_t g_ryz014_atc_get_edrx[]            = RYZ014_ATC_GET_EDRX;
const uint8_t g_ryz014_atc_set_edrx[]            = RYZ014_ATC_SET_EDRX;
const uint8_t g_ryz014_atc_get_signal[]          = RYZ014_ATC_GET_SIGNAL_STRENGTH;
const uint8_t g_ryz014_atc_get_sw_revision[]     = RYZ014_ATC_GET_SW_REVISION;
const uint8_t g_ryz014_atc_get_serial_num[]      = RYZ014_ATC_GET_SERIAL_NUMBER;
const uint8_t g_ryz014_atc_get_svn[]             = RYZ014_ATC_GET_SVN;
const uint8_t g_ryz014_atc_get_module_name[]     = RYZ014_ATC_GET_MODULE_NAME;
const uint8_t g_ryz014_atc_get_maker_name[]      = RYZ014_ATC_GET_MAKER_NAME;
const uint8_t g_ryz014_atc_get_imsi[]            = RYZ014_ATC_GET_IMSI;
const uint8_t g_ryz014_atc_send_command_sim[]    = RYZ014_ATC_SEND_COMMAND_TO_SIM;
const uint8_t g_ryz014_atc_set_inter_config[]    = RYZ014_ATC_SET_INTER_CONFIG;
const uint8_t g_ryz014_atc_set_ring_config[]     = RYZ014_ATC_SET_RING_CONFIG;
const uint8_t g_ryz014_atc_set_psm_config[]      = RYZ014_ATC_SET_PSM_CONFIG;
const uint8_t g_ryz014_atc_set_ind_notify[]      = RYZ014_ATC_SET_IND_NOTIFY;
const uint8_t g_ryz014_atc_get_phone_num[]       = RYZ014_ATC_GET_PHONE_NUM;
const uint8_t g_ryz014_atc_get_iccid[]           = RYZ014_ATC_GET_ICCID;
const uint8_t g_ryz014_atc_ping[]                = RYZ014_ATC_PING;
const uint8_t g_ryz014_atc_get_cellinfo[]        = RYZ014_ATC_GET_CELLINFO;
const uint8_t g_ryz014_atc_set_ctm[]             = RYZ014_ATC_SET_CTM;
const uint8_t g_ryz014_atc_get_ctm[]             = RYZ014_ATC_GET_CTM;
const uint8_t g_ryz014_atc_set_band[]            = RYZ014_ATC_SET_BAND;
const uint8_t g_ryz014_atc_factoryreset[]        = RYZ014_ATC_FACTORYRESET;
const uint8_t g_ryz014_atc_smcwrx[]              = RYZ014_ATC_SMCWRX;
const uint8_t g_ryz014_atc_smcwtx[]              = RYZ014_ATC_SMCWTX;
const uint8_t g_ryz014_atc_cgpiaf[]              = RYZ014_ATC_CGPIAF;
const uint8_t g_ryz014_no_command[]              = RYZ014_NO_COMMAND;
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
const uint8_t g_ryz014_write_certificate[]       = RYZ014_ATC_WRITE_CERTIFICATE;
const uint8_t g_ryz014_erase_certificate[]       = RYZ014_ATC_ERASE_CERTIFICATE;
const uint8_t g_ryz014_config_ssl_profile[]      = RYZ014_ATC_CONFIG_SSL_PROFILE;
const uint8_t g_ryz014_config_ssl_socket[]       = RYZ014_ATC_CONFIG_SSL_SOCKET;
#endif

const uint8_t * const gp_at_command[ATC_LIST_MAX] =
{
    g_ryz014_echo_off,
    g_ryz014_function_level_check,
    g_ryz014_function_level,
    g_ryz014_pin_lock_check,
    g_ryz014_pin_lock_release,
    g_ryz014_connect_socket,
    g_ryz014_close_socket,
    g_ryz014_send_socket,
    g_ryz014_recv_socket,
    g_ryz014_dns_lookup,
    g_ryz014_ap_config,
    g_ryz014_user_config,
    g_ryz014_socket_config_1,
    g_ryz014_socket_config_2,
    g_ryz014_listening_socket,
    g_ryz014_connect_check,
    g_ryz014_set_connect_status,
    g_ryz014_shutdown,
    g_ryz014_get_time,
    g_ryz014_set_time,
    g_ryz014_reset,
    g_ryz014_set_notice_level,
    g_ryz014_get_notice_level,
    g_ryz014_auto_connect,
    g_ryz014_auto_connect_check,
    g_ryz014_sim_st_off,
    g_ryz014_get_service_status,
    g_ryz014_set_provider,
    g_ryz014_get_pdn_state,
    g_ryz014_activate_pdn,
    g_ryz014_atc_get_ipaddr,
    g_ryz014_atc_get_psm,
    g_ryz014_atc_set_psm,
    g_ryz014_atc_get_edrx,
    g_ryz014_atc_set_edrx,
    g_ryz014_atc_get_signal,
    g_ryz014_atc_get_sw_revision,
    g_ryz014_atc_get_serial_num,
    g_ryz014_atc_get_svn,
    g_ryz014_atc_get_module_name,
    g_ryz014_atc_get_maker_name,
    g_ryz014_atc_get_imsi,
    g_ryz014_atc_send_command_sim,
    g_ryz014_atc_set_inter_config,
    g_ryz014_atc_set_ring_config,
    g_ryz014_atc_set_psm_config,
    g_ryz014_atc_set_ind_notify,
    g_ryz014_atc_get_phone_num,
    g_ryz014_atc_get_iccid,
    g_ryz014_atc_ping,
    g_ryz014_atc_get_cellinfo,
    g_ryz014_atc_set_ctm,
    g_ryz014_atc_get_ctm,
    g_ryz014_atc_set_band,
    g_ryz014_atc_factoryreset,
    g_ryz014_atc_smcwrx,
    g_ryz014_atc_smcwtx,
    g_ryz014_atc_cgpiaf,
    g_ryz014_no_command,
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
    g_ryz014_write_certificate,
    g_ryz014_erase_certificate,
    g_ryz014_config_ssl_profile,
    g_ryz014_config_ssl_socket,
#endif
};

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*****************************************************************************
 * Function Name  @fn            atc_generate
 * Description    @details       Generate the AT command.
 * Arguments      @param[in/out] p_command_buff -
 *                                  Pointer to store the command.
 *                @param[in]     pp_command -
 *                                  Pointer to AT command.
 *                @param[in]     pp_command_arg -
 *                                  Pointer to the argument of the AT command.
 ****************************************************************************/
void atc_generate(uint8_t * const p_command_buff, const uint8_t * const p_command,
                                                    const uint8_t ** const pp_command_arg)
{
    memset(p_command_buff, 0, CELLULAR_ATC_BUFF_SIZE);
    if (NULL == pp_command_arg)
    {
        sprintf((char *)p_command_buff,     // (uint8_t *) -> (char *)
                (const char *)p_command);   // (uint8_t *) -> (char *)
    }
    else
    {
        sprintf((char *)p_command_buff,     // (uint8_t *) -> (char *)
                (const char *)p_command,    // (uint8_t *) -> (char *)
                        pp_command_arg[0],
                        pp_command_arg[1],
                        pp_command_arg[2],
                        pp_command_arg[3],
                        pp_command_arg[4],
                        pp_command_arg[5],
                        pp_command_arg[6],
                        pp_command_arg[7]);
    }
    CELLULAR_LOG_DEBUG(("generated AT command: %s", p_command_buff));
    return;
}
/**********************************************************************************************************************
 * End of function atc_generate
 *********************************************************************************************************************/
