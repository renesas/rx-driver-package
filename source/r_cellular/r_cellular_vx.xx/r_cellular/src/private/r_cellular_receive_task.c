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
 * File Name    : r_cellular_receive_task.c
 * Description  : Functions of the receiving task.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_cellular_if.h"
#include "cellular_freertos.h"
#include "cellular_receive_task.h"
#include "cellular_private_api.h"
#include "at_command.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CHAR_CHECK_1    ('+')
#define CHAR_CHECK_2    ('>')
#define CHAR_CHECK_3    (':')
#define CHAR_CHECK_4    ('\n')
#define CHAR_CHECK_5    (',')
#define CHAR_CHECK_6    (' ')
#define CHAR_CHECK_7    ('\r')
#define CHAR_END        ('\0')

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static const uint8_t s_atc_res_go_send[]            = ATC_RES_GO_SEND;
static const uint8_t s_atc_res_ok[]                 = ATC_RES_OK;
static const uint8_t s_atc_res_error[]              = ATC_RES_ERROR;
static const uint8_t s_atc_res_no_carrier[]         = ATC_RES_NO_CARRIER;
static const uint8_t s_atc_res_connect[]            = ATC_RES_CONNECT;
static const uint8_t s_atc_res_exit[]               = ATC_RES_EXIT;
static const uint8_t s_atc_res_data_receive[]       = ATC_RES_DATA_RECEIVE;
static const uint8_t s_atc_res_data_receive_qird[]  = ATC_RES_DATA_RECEIVE_QIRD;
static const uint8_t s_atc_res_read_dns[]           = ATC_RES_READ_DNS;
static const uint8_t s_atc_res_pin_lock_status[]    = ATC_RES_PIN_LOCK_STATUS;
static const uint8_t s_atc_res_socket_close[]       = ATC_RES_SOCKET_CLOSE;
static const uint8_t s_atc_res_system_start[]       = ATC_RES_SYSTEM_START;
static const uint8_t s_atc_res_attach_status[]      = ATC_RES_ATTACH_STATUS;
static const uint8_t s_atc_res_ap_connect_config[]  = ATC_RES_AP_CONNECT_CONFIG;
static const uint8_t s_atc_res_function_level[]     = ATC_RES_FUNCTION_LEVEL;
static const uint8_t s_atc_res_timezone[]           = ATC_RES_TIMEZONE;
static const uint8_t s_atc_res_creg_status[]        = ATC_RES_CREG_STATUS;
static const uint8_t s_atc_res_cereg_status[]       = ATC_RES_CEREG_STATUS;
static const uint8_t s_atc_res_get_time[]           = ATC_RES_GET_TIME;
static const uint8_t s_atc_res_get_service_status[] = ATC_RES_GET_SERVICE_STATUS;
static const uint8_t s_atc_res_get_pdp_status[]     = ATC_RES_GET_PDP_STATUS;
static const uint8_t s_atc_res_get_ip_addr[]        = ATC_RES_GET_IP_ADDR;
static const uint8_t s_atc_res_get_psms[]           = ATC_RES_GET_PSMS;
static const uint8_t s_atc_res_get_edrxs[]          = ATC_RES_GET_EDRXS;
static const uint8_t s_atc_res_set_edrxs[]          = ATC_RES_SET_EDRXS;
static const uint8_t s_atc_res_get_signal[]         = ATC_RES_GET_SIGNAL;
static const uint8_t s_atc_res_command_send_sim[]   = ATC_RES_COMMAND_SEND_SIM;
static const uint8_t s_atc_res_timezene_info[]      = ATC_RES_TIMEZONE_INFO;
static const uint8_t s_atc_res_ind_info[]           = ATC_RES_IND_INFO;
static const uint8_t s_atc_res_svn[]                = ATC_RES_SVN;
static const uint8_t s_atc_res_get_phone_num[]      = ATC_RES_GET_PHONE_NUM;
static const uint8_t s_atc_res_get_iccid[]          = ATC_RES_GET_ICCID;
static const uint8_t s_atc_res_ping[]               = ATC_RES_PING;
static const uint8_t s_atc_res_get_cellinfo[]       = ATC_RES_GET_CELLINFO;
static const uint8_t s_atc_res_get_autoconnect[]    = ATC_RES_GET_AUTOCONNECT;
static const uint8_t s_atc_res_get_ctm[]            = ATC_RES_GET_CTM;
static const uint8_t s_atc_res_smcwrx[]             = ATC_RES_SMCWRX;
static const uint8_t s_atc_res_smcwtx[]             = ATC_RES_SMCWTX;
static const uint8_t s_atc_res_shutdown[]           = ATC_RES_SHUTDOWN;
static const uint8_t s_atc_res_firmupgrade[]        = ATC_RES_FIRMUPGRADE;
static const uint8_t s_atc_res_get_certificate[]    = ATC_RES_GET_CERTIFICATE;

static const uint8_t * const sp_cellular_atc_res_tbl[CELLULAR_RES_MAX] =
{
    s_atc_res_go_send,
    s_atc_res_ok,
    s_atc_res_error,
    s_atc_res_no_carrier,
    s_atc_res_connect,
    s_atc_res_exit,
    s_atc_res_data_receive,
    s_atc_res_data_receive_qird,
    s_atc_res_read_dns,
    s_atc_res_pin_lock_status,
    s_atc_res_socket_close,
    s_atc_res_system_start,
    s_atc_res_attach_status,
    s_atc_res_ap_connect_config,
    s_atc_res_function_level,
    s_atc_res_timezone,
    s_atc_res_creg_status,
    s_atc_res_cereg_status,
    s_atc_res_get_time,
    s_atc_res_get_service_status,
    s_atc_res_get_pdp_status,
    s_atc_res_get_ip_addr,
    s_atc_res_get_psms,
    s_atc_res_get_edrxs,
    s_atc_res_set_edrxs,
    s_atc_res_get_signal,
    s_atc_res_command_send_sim,
    s_atc_res_timezene_info,
    s_atc_res_ind_info,
    s_atc_res_svn,
    s_atc_res_get_phone_num,
    s_atc_res_get_iccid,
    s_atc_res_ping,
    s_atc_res_get_cellinfo,
    s_atc_res_get_autoconnect,
    s_atc_res_get_ctm,
    s_atc_res_smcwrx,
    s_atc_res_smcwtx,
    s_atc_res_shutdown,
    s_atc_res_firmupgrade,
    s_atc_res_get_certificate,
};

static void                cellular_job_check (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static e_atc_return_code_t cellular_response_string_check (uint8_t * p_string);

static void cellular_response_check (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_data_send_command (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_data_reception (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_request_data (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_store_data (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_dns_result (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_ap_connect_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_ap_connect_config (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_station_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_control_level (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_cpin_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_time (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_imei (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_imsi (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_system_start (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_disconnect_socket (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_timezone (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_service_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_pdp_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_pdp_addr (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_psms (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_edrx (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_signal (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_res_command_send_sim (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_timezone_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_ind_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_svn (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_lrsvn (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_phone_number (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_iccid (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_ping (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_cellinfo (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_autoconnect (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_ctm (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_set_smcwrx (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_set_smcwtx (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_shutdown_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_firmupgrade_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_certificate (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_revision (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_response_skip (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_memclear (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_exit (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);

static void         cellular_system_state_change (st_cellular_ctrl_t * p_ctrl, int32_t stat);
static e_atc_list_t cellular_get_at_command (st_cellular_ctrl_t * const p_ctrl);

static void    cellular_set_atc_response (st_cellular_ctrl_t * const p_ctrl, const e_cellular_atc_return_t result);
static void    cellular_cleardata (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void    cellular_charcheck (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static int32_t binary_conversion (int32_t binary);

static void(* p_cellular_recvtask_api[])(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive) =
{
        cellular_data_send_command,
        cellular_memclear,
        cellular_memclear,
        cellular_memclear,
        cellular_memclear,
        cellular_exit,
        cellular_get_data_reception,
        cellular_request_data,
        cellular_dns_result,
        cellular_cpin_status,
        cellular_disconnect_socket,
        cellular_system_start,
        cellular_get_ap_connect_status,
        cellular_get_ap_connect_config,
        cellular_control_level,
        cellular_get_timezone,
        cellular_station_info,
        cellular_station_info,
        cellular_get_time,
        cellular_get_service_status,
        cellular_get_pdp_status,
        cellular_get_pdp_addr,
        cellular_get_psms,
        cellular_get_edrx,
        cellular_get_edrx,
        cellular_get_signal,
        cellular_res_command_send_sim,
        cellular_timezone_info,
        cellular_ind_info,
        cellular_get_svn,
        cellular_get_phone_number,
        cellular_get_iccid,
        cellular_ping,
        cellular_get_cellinfo,
        cellular_get_autoconnect,
        cellular_get_ctm,
        cellular_set_smcwrx,
        cellular_set_smcwtx,
        cellular_shutdown_info,
        cellular_firmupgrade_info,
        cellular_get_certificate,
        cellular_response_skip,
        cellular_store_data,
        cellular_response_check,
        cellular_job_check
};

/***********************************************************************************************
 * Function Name  @fn            cellular_recv_task
 * Description    @details       Process incoming data from the module.
 * Arguments      @param[in/out] p_pvParameters -
 *                                  Pointer to the parameter given at the time of task creation.
 **********************************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
void cellular_recv_task(void * const p_pvParameters)
#elif BSP_CFG_RTOS_USED == (5)
void cellular_recv_task(ULONG p_pvParameters)
#endif
{
#if BSP_CFG_RTOS_USED == (1)
    st_cellular_ctrl_t * const p_ctrl = p_pvParameters;
#elif BSP_CFG_RTOS_USED == (5)
    st_cellular_ctrl_t * const p_ctrl = gp_cellular_ctrl;
#endif

    sci_err_t             sci_ret          = SCI_SUCCESS;
    st_cellular_receive_t cellular_receive = {0,0,CELLULAR_RES_NONE,JOB_STATUS_NONE,0,0,0};

    memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));

    cellular_synchro_event_group(p_ctrl->eventgroup, CELLULAR_RECV_TASK_BIT,
                                (CELLULAR_MAIN_TASK_BIT | CELLULAR_RECV_TASK_BIT), CELLULAR_TIME_WAIT_TASK_START);

    while (1)
    {
        sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &cellular_receive.data, 1);
        if (SCI_SUCCESS != sci_ret)
        {
            cellular_delay_task(1);
        }
        else
        {
            if (cellular_receive.recv_count > CELLULAR_ATC_RESPONSE_BUFF_SIZE)
            {
                cellular_receive.overflow_flag = 1;
                CELLULAR_LOG_ERROR(("sci buffer overflow\n"));
                cellular_memclear(p_ctrl, &cellular_receive);
            }
            else
            {
                p_ctrl->sci_ctrl.receive_buff[cellular_receive.recv_count] = cellular_receive.data;
                cellular_receive.recv_count++;
            }
            (* p_cellular_recvtask_api[cellular_receive.job_no])(p_ctrl, &cellular_receive);
        }
        if (CELLULAR_MODULE_OPERATING_RESET == p_ctrl->module_status)
        {
            memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
            memset(&cellular_receive, 0, sizeof(st_cellular_receive_t));
            cellular_receive.job_no = CELLULAR_RES_NONE;
            p_ctrl->module_status   = CELLULAR_MODULE_OPERATING_LEVEL0;
        }
    }
}
/**********************************************************************************************************************
 * End of function cellular_recv_task
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_job_check
 * Description    @details       Analyze incoming data.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_job_check(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->job_status)
    {
        case JOB_STATUS_NONE:
        {
            if ((char)p_cellular_receive->data == (CHAR_CHECK_4))   // (uint8_t)->(char)
            {
                if ((NULL != strstr((char *)p_ctrl->sci_ctrl.receive_buff, ATC_RES_BEGIN_OR_END)) && //cast
                    (2    == p_cellular_receive->recv_count))
                {
                    p_cellular_receive->job_status  = JOB_STATUS_FIRST_CHAR_CHECK;
                    p_cellular_receive->tmp_recvcnt = p_cellular_receive->recv_count;
                }
                else
                {
                    cellular_memclear(p_ctrl, p_cellular_receive);
                }
            }
            break;
        }
        case JOB_STATUS_FIRST_CHAR_CHECK:
        {
            if ((char)p_cellular_receive->data == (CHAR_CHECK_1))          //(uint8_t)->(char)
            {
                p_cellular_receive->job_status = JOB_STATUS_COLON_CHECK;
            }
            else if ((char)p_cellular_receive->data == (CHAR_CHECK_2))     //(uint8_t)->(char)
            {
                p_cellular_receive->job_no = CELLULAR_RES_GO_SEND;
            }
            else
            {
                p_cellular_receive->job_no = CELLULAR_RES_CHECK;
            }
            break;
        }
        case JOB_STATUS_COLON_CHECK:
        {
            if ((char)p_cellular_receive->data == (CHAR_CHECK_3))          //(uint8_t)->(char)
            {
                p_cellular_receive->job_no
                = cellular_response_string_check(&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);
                p_cellular_receive->tmp_recvcnt = p_cellular_receive->recv_count;
            }
            else if ((char)p_cellular_receive->data == (CHAR_CHECK_4))     //(uint8_t)->(char)
            {
                p_cellular_receive->job_no
                = cellular_response_string_check(&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);
                (* p_cellular_recvtask_api[cellular_receive->job_no])(p_ctrl, cellular_receive);
            }
            else
            {
                /* Do Nothing */
                R_BSP_NOP();
            }
            break;
        }
        default :
        {
            cellular_memclear(p_ctrl, p_cellular_receive);
            break;
        }
    }
    return;
}
/**********************************************************************************************************************
 * End of function cellular_job_check
 *********************************************************************************************************************/

/************************************************************************************************
 * Function Name  @fn            cellular_response_string_check
 * Description    @details       Parse a string.
 * Arguments      @param[in]     p_string -
 *                                  Pointer to parsing string.
 * Return Value   @retval        e_atc_return_code_t -
 *                                  Returns the e-num value corresponding to the analysis result.
 ***********************************************************************************************/
static e_atc_return_code_t cellular_response_string_check(uint8_t * p_string)
{
    uint8_t             cnt = 0;
    e_atc_return_code_t res = CELLULAR_RES_MAX;

    while (cnt < CELLULAR_RES_MAX)
    {
        if (NULL != strstr((const char *)p_string,                          //(uint8_t *)->(char *)
                            (const char *)sp_cellular_atc_res_tbl[cnt]))    //(uint8_t *)->(char *)
        {
            break;
        }
        else
        {
            cnt++;
        }
    }
    if (cnt < CELLULAR_RES_MAX)
    {
        res = (e_atc_return_code_t)cnt; //(uint8_t)->(e_atc_return_code_t)
    }

    return res;
}
/**********************************************************************************************************************
 * End of function cellular_response_string_check
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_response_check
 * Description    @details       Parses the received string and returns the result.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_response_check(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if ((char)p_cellular_receive->data == (CHAR_CHECK_4))  //(uint8_t)->(char)
    {
        p_cellular_receive->job_no = cellular_response_string_check(p_ctrl->sci_ctrl.receive_buff);

        switch (p_cellular_receive->job_no)
        {
            case CELLULAR_RES_OK:
            case CELLULAR_RES_CONNECT:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_OK);
                cellular_memclear(p_ctrl, p_cellular_receive);
                break;
            }
            case CELLULAR_RES_ERROR:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
                cellular_memclear(p_ctrl, p_cellular_receive);
                break;
            }
            case CELLULAR_RES_NO_CARRIER:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_SEND_NO_CARRIER);
                cellular_memclear(p_ctrl, p_cellular_receive);
                break;
            }
            case CELLULAR_RES_EXIT:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
                cellular_exit(p_ctrl, cellular_receive);
                break;
            }
            default:
            {
                if (ATC_GET_SERIAL_NUMBER == p_ctrl->sci_ctrl.at_command)
                {
                    cellular_get_imei(p_ctrl, cellular_receive);
                }
                if (ATC_GET_MODULE_NAME == p_ctrl->sci_ctrl.at_command)
                {
                    cellular_memclear(p_ctrl, p_cellular_receive);
                }
                if (ATC_GET_MAKER_NAME == p_ctrl->sci_ctrl.at_command)
                {
                    cellular_memclear(p_ctrl, p_cellular_receive);
                }
                if (ATC_GET_IMSI == p_ctrl->sci_ctrl.at_command)
                {
                    cellular_get_imsi(p_ctrl, cellular_receive);
                }
                if (ATC_GET_SW_REVISION == p_ctrl->sci_ctrl.at_command)
                {
                    cellular_get_revision(p_ctrl, cellular_receive);
                }
                if (ATC_GET_LR_SVN == p_ctrl->sci_ctrl.at_command)
                {
                    cellular_get_lrsvn(p_ctrl, cellular_receive);
                }
                break;
            }
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_response_check
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_data_send_command
 * Description    @details       Process of sending data to the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_data_send_command(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_6 == p_cellular_receive->data)
    {
        cellular_set_atc_response(p_ctrl, ATC_RETURN_OK_GO_SEND);
        cellular_memclear(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_data_send_command
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_data_reception
 * Description    @details       Get data reception notification from module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_data_reception(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 sscanf_ret         = 0;
    int32_t                 socket_no          = 0;
    int32_t                 length             = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != strstr((char *)         //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            ATC_RES_BEGIN_OR_END))
        {
            sscanf_ret = sscanf((char *)    //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                " %ld,%ld", &socket_no, &length);
            if (2 == sscanf_ret)
            {
                p_cellular_receive->socket_no  = (uint8_t)socket_no;    // int32_t -> uint8_t
                p_cellular_receive->socket_no -= CELLULAR_START_SOCKET_NUMBER;

                p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size += (int16_t)length;//cast
                p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count             = 0;

                p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_flg = CELLULAR_RECEIVE_FLAG_ON;
            }
            cellular_cleardata(p_ctrl, p_cellular_receive);
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_data_reception
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_request_data
 * Description    @details       Processing of requests to pass data to the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_request_data(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 sscanf_ret         = 0;
    int32_t                 socket_no          = 0;
    int32_t                 length             = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf_ret = sscanf((char *)    //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            " %ld,%ld", &socket_no, &length);
        if (2 == sscanf_ret)
        {
            p_cellular_receive->socket_no  = (uint8_t)socket_no;    // int32_t -> uint8_t
            p_cellular_receive->socket_no -= CELLULAR_START_SOCKET_NUMBER;

            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size -= (int16_t)length; //cast

            if (0 >= p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size)
            {
                p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size = 0;

                p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_flg = CELLULAR_RECEIVE_FLAG_OFF;
            }
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_num   = (uint32_t)length; //cast
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count = 0;
            p_cellular_receive->job_no                                         = CELLULAR_RES_PUT_CHAR;
        }
        else
        {
            cellular_memclear(p_ctrl, p_cellular_receive);
            cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
            CELLULAR_LOG_ERROR(("Incoming data request failed.\n"));
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_request_data
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_store_data
 * Description    @details       Store data received from the module in the byte queue.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_store_data(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count <
        p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_num)
    {
        *(p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].p_recv +
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].total_recv_count) = p_cellular_receive->data;
        p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count++;
        p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].total_recv_count++;
    }
    else
    {
        if (CHAR_CHECK_4 == p_cellular_receive->data)
        {
            cellular_cleardata(p_ctrl, p_cellular_receive);
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count = 0;
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_num   = 0;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_store_data
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_dns_result
 * Description    @details       Process the results of dns query execution.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_dns_result(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_5 == p_cellular_receive->data)
    {
        p_cellular_receive->tmp_recvcnt = p_cellular_receive->recv_count;
    }
    else if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_ctrl->recv_data)
        {
            len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);    //cast
            if (CELLULAR_DNS_LENGTH >= len)
            {
                sprintf(p_ctrl->recv_data, "%s", &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);
            }
            else
            {
                CELLULAR_LOG_ERROR(("DNS Result Buffer Size Error\n"));
            }
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }
    else
    {
        /* Do Nothing */
        R_BSP_NOP();
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_dns_result
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_ap_connect_status
 * Description    @details       Process connection status notification to the access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_ap_connect_status(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 sscanf_ret         = 0;
    int32_t                 status             = 0;
    e_atc_list_t            active_atc;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        active_atc = cellular_get_at_command(p_ctrl);

        if (ATC_CONNECT_CHECK == active_atc)
        {
            sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                " %ld" , &status);
            if ((1 == sscanf_ret) && (1 == status) && (CELLULAR_SYSTEM_CLOSE != p_ctrl->system_state))
            {
                p_ctrl->system_state = CELLULAR_SYSTEM_CONNECT;
            }
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_ap_connect_status
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_ap_connect_config
 * Description    @details       Processes connection setup notifications to access points.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_ap_connect_config(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint8_t                 pdp_type[7]        = {0};
    uint8_t                 apn[6]             = {0};
    uint8_t *               p_flg[1][2]        = {0};
    int32_t                 cid                = 0;
    int32_t                 sscanf_ret         = 0;
    sci_err_t               sci_ret;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            " %ld,\"%6[0-9A-Z]\",\"%5[a-zA-Z.]\"" , &cid, pdp_type, apn);
        if ((0 < sscanf_ret) && (NULL != p_ctrl->recv_data))
        {
            p_flg[0][0]  = (uint8_t *)p_ctrl->recv_data + (7 *(cid - 1));       //(void *)->(uint8_t *)
            *p_flg[0][0] = 1;
            p_flg[0][1]  = (uint8_t *)p_ctrl->recv_data + 1 + (7 *(cid - 1));   //(void *)->(uint8_t *)
            sprintf((char *)p_flg[0][1], "%5s", apn);                           //(uint8_t *)->(char *)
        }

        cellular_cleardata(p_ctrl, p_cellular_receive);

        do
        {
            sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
            cellular_delay_task(1);
        } while (SCI_SUCCESS != sci_ret);

        p_ctrl->sci_ctrl.receive_buff[0] = p_cellular_receive->data;
        p_cellular_receive->recv_count++;
        if (CHAR_CHECK_1 == p_cellular_receive->data)
        {
            p_cellular_receive->job_no      = CELLULAR_GET_AP_CONNECT_CONFIG;
            p_cellular_receive->tmp_recvcnt = 9;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_ap_connect_config
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_station_info
 * Description    @details       Notification of connected base station information.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_station_info(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 tec                = 0;
    int32_t                 stat               = 0;
    int32_t                 level              = 0;
    e_atc_list_t            ret                = ATC_LIST_MAX;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;
    st_cellular_notice_t *  p_cellular_notice  = p_ctrl->recv_data;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        ret = cellular_get_at_command(p_ctrl);

        if ((ATC_GET_NOTICE_LEVEL == ret) && (NULL != p_cellular_notice))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[]->char*)
                    " %ld,%ld,\"%4s\",\"%8s\",%ld,,,\"%8s\",\"%8s\"",
                    &level,
                    &stat,
                    (char *)p_cellular_notice->ta_code,     //(&uint8_t[]->char*)
                    (char *)p_cellular_notice->cell_id,     //(&uint8_t[]->char*)
                    &tec,
                    (char *)p_cellular_notice->active_time, //(&uint8_t[]->char*)
                    (char *)p_cellular_notice->tau);        //(&uint8_t[]->char*)

            p_cellular_notice->level      = (e_cellular_network_result_t)level;//(int32_t->e_cellular_network_result_t)
            p_cellular_notice->stat       = (e_cellular_reg_stat_t)stat;       //(int32_t->e_cellular_reg_stat_t)
            p_cellular_notice->access_tec = (e_cellular_access_tec_t)tec;      //(int32_t->e_cellular_access_tec_t)
        }
        else
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[]->char*)
                                " %ld", &stat);
        }
        cellular_system_state_change(p_ctrl, stat);
        cellular_cleardata(p_ctrl, p_cellular_receive);
        memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
        memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
        p_cellular_receive->job_no = CELLULAR_RES_NONE;
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_station_info
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_control_level
 * Description    @details       Process module control level notifications.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_control_level(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 sscanf_ret         = 0;
    int32_t                 status             = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            " %ld", &status);

        if ((1 == sscanf_ret))
        {
            p_ctrl->module_status = (e_cellular_module_status_t)status; //cast
        }
        else
        {
            /* Do Nothing */
            CELLULAR_LOG_DEBUG(("Received unexpected string"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_control_level
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_cpin_status
 * Description    @details       Handle notification of sim card lock status.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_cpin_status(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_ctrl->recv_data)
        {
            len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);    //cast
            if (CELLULAR_CPIN_STATUS_LENGTH >= len)
            {
                sprintf(p_ctrl->recv_data, "%s", &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);
            }
            else
            {
                CELLULAR_LOG_ERROR(("CPIN Status Buffer Size Error\n"));
            }
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_cpin_status
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_time
 * Description    @details       Process the results of getting the date and time.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_time(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                  year               = 0;
    int32_t                  month              = 0;
    int32_t                  day                = 0;
    int32_t                  hour               = 0;
    int32_t                  min                = 0;
    int32_t                  sec                = 0;
    int32_t                  timezone           = 0;
    st_cellular_receive_t *  p_cellular_receive = cellular_receive;
    st_cellular_datetime_t * p_date_time        = p_ctrl->recv_data;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_date_time)
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[]->char*)
                    " \"%2d/%2d/%2d,%2d:%2d:%2d%3d\"\r\n",
                    &year, &month, &day, &hour, &min, &sec, &timezone);
            p_date_time->year     = (uint8_t)year;      // int32_t -> uint8_t
            p_date_time->month    = (uint8_t)month;     // int32_t -> uint8_t
            p_date_time->day      = (uint8_t)day;       // int32_t -> uint8_t
            p_date_time->hour     = (uint8_t)hour;      // int32_t -> uint8_t
            p_date_time->min      = (uint8_t)min;       // int32_t -> uint8_t
            p_date_time->sec      = (uint8_t)sec;       // int32_t -> uint8_t
            p_date_time->timezone = (int8_t)timezone;   // int32_t -> int8_t
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_time
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_imei
 * Description    @details       Process the results of getting the IMEI.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_imei(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_imei_t *    p_imei             = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);//cast
        if ((CELLULAR_MAX_IMEI_LENGTH >= (len - CELLULAR_IMEI_USELESS_CHAR)) && (NULL != p_imei))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[] -> char *)
                    "%15[0-9]", (char *)p_imei->imei);                                      //(&uint8_t[] -> char *)
        }
        else
        {
            CELLULAR_LOG_ERROR(("IMEI Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_imei
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_imsi
 * Description    @details       Process the results of getting the IMSI.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_imsi(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_imsi_t *    p_imsi             = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);    //cast
        if ((CELLULAR_MAX_IMSI_LENGTH >= (len - CELLULAR_IMSI_USELESS_CHAR)) && (NULL != p_imsi))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],     //(&uint8_t[] -> char *)
                    "%15[0-9]", (char *)p_imsi->imsi);                                          //(&uint8_t[] -> char *)
        }
        else
        {
            CELLULAR_LOG_ERROR(("IMSI Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_imsi
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_system_start
 * Description    @details       Module start-up notification.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_system_start(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (NULL != p_ctrl->recv_data)
    {
        *(uint8_t *)p_ctrl->recv_data = CELLULAR_FLG_START;    //(void *)->(uint8_t *)
    }

    cellular_cleardata(p_ctrl, p_cellular_receive);

    return;
}
/**********************************************************************************************************************
 * End of function cellular_system_start
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_disconnect_socket
 * Description    @details       Handling of Socket Disconnection Notification.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_disconnect_socket(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 sscanf_ret         = 0;
    int32_t                 socket_no          = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            " %ld" , &socket_no);
        if (0 != sscanf_ret)
        {
            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].socket_status
            = CELLULAR_SOCKET_STATUS_SOCKET;
        }

        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_disconnect_socket
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_timezone
 * Description    @details       Processing of notification of timezone information from modules.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_timezone(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_timezone
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_service_status
 * Description    @details       Get access technology and network operators.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_service_status(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 status_1           = 0;
    int32_t                 status_2           = 0;
    int32_t                 status_3           = 0;
    int32_t                 status_4           = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                " %ld,%ld,\"%ld\",%ld",
                &status_1, &status_2, &status_3, &status_4);
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_service_status
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_pdp_status
 * Description    @details       Get the state of the PDP context.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_pdp_status(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 status_1           = 0;
    int32_t                 status_2           = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                " %ld,%ld", &status_1, &status_2);
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_pdp_status
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_pdp_addr
 * Description    @details       Process for obtaining PDP addresses.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_pdp_addr(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_ctrl->recv_data)
        {
            len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);    //cast
            if (CELLULAR_PDP_ADDR_LENGTH >= len)
            {
                sprintf(p_ctrl->recv_data, "%s", &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);
            }
            else
            {
                CELLULAR_LOG_ERROR(("Get PDP Addr Buffer Size Error\n"));
            }

        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_pdp_addr
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_psms
 * Description    @details       Get PSM configuration status.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_psms(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                    status             = 0;
    int32_t                    tau_cycle          = 0;
    int32_t                    tau_multiplier     = 0;
    int32_t                    active_cycle       = 0;
    int32_t                    active_multiplier  = 0;
    st_cellular_receive_t *    p_cellular_receive = cellular_receive;
    st_cellular_psm_config_t * p_psm_ret          = p_ctrl->recv_data;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_psm_ret)
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                    " %ld,,,\"%3d%5d\",\"%3d%5d\"",
                    &status, &tau_cycle, &tau_multiplier, &active_cycle, &active_multiplier);
            p_psm_ret->psm_mode          = (e_cellular_psm_mode_t)status;                                       //cast
            p_psm_ret->tau_cycle         = (e_cellular_tau_cycle_t)binary_conversion(tau_cycle);                //cast
            p_psm_ret->tau_multiplier    = (e_cellular_cycle_multiplier_t)binary_conversion(tau_multiplier);    //cast
            p_psm_ret->active_cycle      = (e_cellular_active_cycle_t)binary_conversion(active_cycle);          //cast
            p_psm_ret->active_multiplier = (e_cellular_cycle_multiplier_t)binary_conversion(active_multiplier); //cast
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_psms
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_edrx
 * Description    @details       Get edrx configuration status.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_edrx(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                     status             = 0;
    int32_t                     act_type           = 0;
    int32_t                     requested_edrx     = 0;
    int32_t                     provided_edrx      = 0;
    int32_t                     ptw                = 0;
    st_cellular_receive_t *     p_cellular_receive = cellular_receive;
    st_cellular_edrx_config_t * p_edrx_ret         = p_ctrl->recv_data;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if ((ATC_GET_EDRXS == p_ctrl->sci_ctrl.at_command) && (NULL != p_edrx_ret))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                    " %ld,%ld,\"%ld\",\"%ld\"",
                    &status, &act_type, &provided_edrx, &ptw);
            p_edrx_ret->edrx_mode  = (e_cellular_edrx_mode_t)status;                            //cast
            p_edrx_ret->edrx_cycle = (e_cellular_edrx_cycle_t)binary_conversion(provided_edrx); //cast
            p_edrx_ret->ptw_cycle  = (e_cellular_ptw_cycle_t)binary_conversion(ptw);            //cast
        }
        else
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                    " %ld,\"%ld\",\"%ld\",\"%ld\"",
                    &status, &requested_edrx, &provided_edrx, &ptw);
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_edrx
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_signal
 * Description    @details       Get signal strength levels.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_signal(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_rssi_t *    p_rssi             = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_rssi)
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *) -> (char *)
                    " %2s,%2s", (char *)p_rssi->rssi, (char *)p_rssi->ber);                 //(&uint8_t[]) -> (char *)
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_signal
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_res_command_send_sim
 * Description    @details       Get the number of the AT command being executed.
 * Arguments      @param[in]     p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        e_atc_list_t -
 *                                  AT command number being executed.
 **********************************************************************************************/
static void cellular_res_command_send_sim(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint8_t                 response[100+1]    = {0};
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                " %100s", &response);

        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_res_command_send_sim
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_timezone_info
 * Description    @details       Processing of notification of timezone information from modules.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_timezone_info(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_timezone_info
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_ind_info
 * Description    @details       Processing of notification of indicator information from modules.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_ind_info(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        p_cellular_receive = cellular_receive;
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_ind_info
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_svn
 * Description    @details       Get Software Version Number.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_svn(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_svn_t *     p_svn              = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
        if ((CELLULAR_MAX_SVN_LENGTH >= (len - CELLULAR_SVN_USELESS_CHAR)) && (NULL != p_svn))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[] -> char *)
                    " \"%2[a-zA-Z0-9]", (char *)p_svn->svn);                                //(&uint8_t[] -> char *)
        }
        else
        {
            CELLULAR_LOG_ERROR(("SVN Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_svn
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_revision
 * Description    @details       Get Revision.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_revision(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_svn_t *     p_svn              = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
        if ((CELLULAR_MAX_REVISION_LENGTH >= (len - CELLULAR_REVISION_USELESS_CHAR)) && (NULL != p_svn))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[] -> char *)
                    "%9[a-zA-Z0-9.]", (char *)p_svn->revision);                             //(&uint8_t[] -> char *)
        }
        else
        {
            CELLULAR_LOG_ERROR(("Revision Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_revision
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_lrsvn
 * Description    @details       Get UE and LR software versions.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_lrsvn(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    uint8_t                 ue[]               = "UE";
    uint8_t                 lr[]               = "LR";
    st_cellular_svn_t *     p_svn              = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != strstr((char *)p_ctrl->sci_ctrl.receive_buff, (char *)ue))  //(uint8_t *)->(char *)
        {
            len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
            if ((CELLULAR_MAX_REVISION_LENGTH >= (len - CELLULAR_REVISION_USELESS_CHAR)) && (NULL != p_svn))
            {
                sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                        "%9[a-zA-Z0-9.]", (char *)p_svn->revision);                             //(uint8_t *)->(char *)
            }
            else
            {
                CELLULAR_LOG_ERROR(("Revision Buffer Size Error\n"));
            }
            cellular_cleardata(p_ctrl, p_cellular_receive);
            p_cellular_receive->job_no = CELLULAR_RES_CHECK;
        }
        if (NULL != strstr((char *)p_ctrl->sci_ctrl.receive_buff, (char *)lr))  //(uint8_t *)->(char *)
        {
            len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
            if ((CELLULAR_MAX_LR_SVN_LENGTH >= (len - CELLULAR_LR_SVN_USELESS_CHAR)) && (NULL != p_svn))
            {
                sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *)->(char *)
                        "%15[a-zA-Z0-9-.]", (char *)p_svn->lr_svn);                             //(uint8_t *)->(char *)
            }
            else
            {
                CELLULAR_LOG_ERROR(("Revision Buffer Size Error\n"));
            }
            cellular_cleardata(p_ctrl, p_cellular_receive);
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_lrsvn
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_phone_number
 * Description    @details       Get phone number.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_phone_number(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                 len                = 0;
    st_cellular_receive_t *  p_cellular_receive = cellular_receive;
    st_cellular_phonenum_t   phonenum           = {0};
    st_cellular_phonenum_t * p_phonenum         = p_ctrl->recv_data;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
        if ((CELLULAR_MAX_PHONENUM_LENGTH >= (len - CELLULAR_PHONENUM_USELESS_CHAR)) && (NULL != p_phonenum))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],     //cast
                    " %15[^,],\"%15[0-9]", (char *)&phonenum, (char *)p_phonenum->phonenum);    //cast
        }
        else
        {
            CELLULAR_LOG_ERROR(("Revision Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_phone_number
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_iccid
 * Description    @details       Get ICCID.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_iccid(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_iccid_t *   p_iccid            = p_ctrl->recv_data;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
        if ((CELLULAR_MAX_ICCID_LENGTH >= (len - CELLULAR_ICCID_USELESS_CHAR)) && (NULL != p_iccid))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t * -> char *)
                    " \"%22[0-9]", (char *)p_iccid->iccid);                                 //(&uint8_t[] -> char *)
        }
        else
        {
            CELLULAR_LOG_ERROR(("ICCID Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_iccid
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_ping
 * Description    @details       PING results.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_ping(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    sci_err_t               sci_ret;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);

        do
        {
            sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
            cellular_delay_task(1);
        } while (SCI_SUCCESS != sci_ret);

        p_ctrl->sci_ctrl.receive_buff[0] = p_cellular_receive->data;
        p_cellular_receive->recv_count++;
        if (CHAR_CHECK_1 == p_cellular_receive->data)
        {
            p_cellular_receive->job_no      = CELLULAR_PING;
            p_cellular_receive->tmp_recvcnt = 6;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_ping
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_cellinfo
 * Description    @details       Notification of cell information.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_cellinfo(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    sci_err_t               sci_ret;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);

        do
        {
            sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
            cellular_delay_task(1);
        } while (SCI_SUCCESS != sci_ret);

        p_ctrl->sci_ctrl.receive_buff[0] = p_cellular_receive->data;
        p_cellular_receive->recv_count++;
        if (CHAR_CHECK_1 == p_cellular_receive->data)
        {
            p_cellular_receive->job_no      = CELLULAR_GET_CELLINFO;
            p_cellular_receive->tmp_recvcnt = 9;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_cellinfo
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_autoconnect
 * Description    @details       Get ICCID.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_autoconnect(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int32_t                 type               = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_ctrl->recv_data)
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],     //(uint8_t * -> char *)
                    " %ld", &type);
            *(e_cellular_auto_connect_t *)p_ctrl->recv_data = (e_cellular_auto_connect_t)type;  //cast
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_autoconnect
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_ctm
 * Description    @details       Get Conformance Test Mode.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_ctm(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt]);  //cast
        if ((CELLULAR_MAX_CTM_LENGTH >= (len - CELLULAR_CTM_USELESS_CHAR)) && (NULL != p_ctrl->recv_data))
        {
            sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(uint8_t *) -> (char *)
                    " %20[a-zA-Z0-9-]", (char *)p_ctrl->recv_data);                         //(void *) -> (char *)
        }
        else
        {
            CELLULAR_LOG_ERROR(("CTM Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_ctm
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_set_smcwrx
 * Description    @details       Starts a downlink continuous wave service.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_set_smcwrx(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);
        p_cellular_receive->job_no = CELLULAR_RES_CHECK;
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_set_smcwrx
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_set_smcwtx
 * Description    @details       Starts an uplink continuous wave service.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_set_smcwtx(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);
        p_cellular_receive->job_no = CELLULAR_RES_CHECK;
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_set_smcwtx
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_shutdown_info
 * Description    @details       Shutdown notification from module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_shutdown_info(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (NULL != p_ctrl->recv_data)
    {
        *(uint8_t *)p_ctrl->recv_data = CELLULAR_FLG_SHUTDOWN;   //(void *)->(uint8_t *)
    }

    cellular_cleardata(p_ctrl, p_cellular_receive);

    return;
}
/**********************************************************************************************************************
 * End of function cellular_shutdown_info
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_firmupgrade_info
 * Description    @details       Firm Upgrade Status Notification.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_firmupgrade_info(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint32_t                len                = 0;
    sci_err_t               sci_ret;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        if (NULL != p_ctrl->recv_data)
        {
            if (NULL != strstr((char *)p_ctrl->recv_data, CELLULAR_GET_UPDATE_STATE))   //cast
            {
                len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[2]);      //cast
                if (CELLULAR_UPDATE_STATE_LENGTH >= len)
                {
                    sprintf(p_ctrl->recv_data, "%s", &p_ctrl->sci_ctrl.receive_buff[2]);
                }
                else
                {
                    CELLULAR_LOG_ERROR(("Firm Upgrade info Buffer Size Error\n"));
                }
            }
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);

        while (1)
        {
            do
            {
                sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
                cellular_delay_task(1);
            } while (SCI_SUCCESS != sci_ret);

            p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->recv_count] = p_cellular_receive->data;
            p_cellular_receive->recv_count++;

            if (CHAR_CHECK_4 == p_cellular_receive->data)
            {
                do
                {
                    sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
                    cellular_delay_task(1);
                } while (SCI_SUCCESS != sci_ret);

                cellular_charcheck(p_ctrl, p_cellular_receive);

                break;
            }
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_firmupgrade_info
 *********************************************************************************************************************/


/***********************************************************************************************
 * Function Name  @fn            cellular_get_certificate
 * Description    @details       Retrieve the written certificate.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_certificate(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    uint8_t *               p_recv             = p_ctrl->recv_data;
    uint32_t                len                = 0;
    sci_err_t               sci_ret;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        len = (uint32_t)strlen((char *)&p_ctrl->sci_ctrl.receive_buff[2]);    //cast
        if ((CELLULAR_ATC_RESPONSE_BUFF_SIZE >= len) && (NULL != p_ctrl->recv_data))
        {
            sprintf(p_ctrl->recv_data, "%s", &p_ctrl->sci_ctrl.receive_buff[2]);
        }
        else
        {
            CELLULAR_LOG_ERROR(("Get certificate Buffer Size Error\n"));
        }
        cellular_cleardata(p_ctrl, p_cellular_receive);

        while (1)
        {
            do
            {
                sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
                cellular_delay_task(1);
            } while (SCI_SUCCESS != sci_ret);

            if ('\r' != p_cellular_receive->data)
            {
                if (CELLULAR_ATC_RESPONSE_BUFF_SIZE > len)
                {
                    p_recv[len] = p_cellular_receive->data;
                    len++;
                }

                do
                {
                    sci_ret = R_SCI_Receive(p_ctrl->sci_ctrl.sci_hdl, &p_cellular_receive->data, 1);
                    cellular_delay_task(1);
                    if (SCI_SUCCESS == sci_ret)
                    {
                        if (CELLULAR_ATC_RESPONSE_BUFF_SIZE > len)
                        {
                            p_recv[len] = p_cellular_receive->data;
                            len++;
                        }
                    }
                } while ((SCI_SUCCESS != sci_ret) || (CHAR_CHECK_4 != p_cellular_receive->data));
            }
            else
            {
                if (CELLULAR_ATC_RESPONSE_BUFF_SIZE > p_cellular_receive->recv_count)
                {
                    p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->recv_count] = p_cellular_receive->data;
                    p_cellular_receive->recv_count++;
                }
                else
                {
                    CELLULAR_LOG_ERROR(("Get Certificate Buffer Size Error\n"));
                }
                break;
            }
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_certificate
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_response_skip
 * Description    @details       Processing of notification of time information from modules.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_response_skip(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        CELLULAR_LOG_DEBUG(("SKIP URC\n"));
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_response_skip
 *********************************************************************************************************************/

/***************************************************************************************************
 * Function Name  @fn            cellular_exit
 * Description    @details       Detecting Exit.
 *                               After this error detection, the module is automatically restarted.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **************************************************************************************************/
static void cellular_exit(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_4 == p_cellular_receive->data)
    {
        cellular_cleardata(p_ctrl, p_cellular_receive);
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_exit
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_memclear
 * Description    @details       Clear memory.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_memclear(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    CELLULAR_LOG_DEBUG(("clear buff = %s\n", p_ctrl->sci_ctrl.receive_buff));

    memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
    memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
    p_cellular_receive->job_no = CELLULAR_RES_NONE;

    return;
}
/**********************************************************************************************************************
 * End of function cellular_memclear
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_system_state_change
 * Description    @details       Update system_state.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     stat -
 *                                  Updated system_state.
 **********************************************************************************************/
static void cellular_system_state_change(st_cellular_ctrl_t * p_ctrl, int32_t stat)
{
    if (CELLULAR_SYSTEM_CLOSE != p_ctrl->system_state)
    {
        if ((1 == stat) || (5 == stat))
        {
            p_ctrl->system_state = CELLULAR_SYSTEM_CONNECT;
        }
        else if ((0 == stat) || (4 == stat) || (80 == stat))
        {
            p_ctrl->system_state = CELLULAR_SYSTEM_OPEN;
        }
        else
        {
            p_ctrl->system_state = CELLULAR_SYSTEM_OPEN;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_system_state_change
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_at_command
 * Description    @details       Get the number of the AT command being executed.
 * Arguments      @param[in]     p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        e_atc_list_t -
 *                                  AT command number being executed.
 **********************************************************************************************/
static e_atc_list_t cellular_get_at_command(st_cellular_ctrl_t * const p_ctrl)
{
    return p_ctrl->sci_ctrl.at_command;
}
/**********************************************************************************************************************
 * End of function cellular_get_at_command
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_set_atc_response
 * Description    @details       Stores responses to AT commands.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     result -
 *                                  Response from module.
 **********************************************************************************************/
static void cellular_set_atc_response(st_cellular_ctrl_t * const p_ctrl, const e_cellular_atc_return_t result)
{
    if (CELLULAR_ATC_RESPONSE_UNCONFIRMED == p_ctrl->sci_ctrl.atc_flg)
    {
        p_ctrl->sci_ctrl.atc_res = result;
        p_ctrl->sci_ctrl.atc_flg = CELLULAR_ATC_RESPONSE_CONFIRMED;
        CELLULAR_LOG_DEBUG(("received AT command response: %s", p_ctrl->sci_ctrl.receive_buff));
    }
    return;
}
/**********************************************************************************************************************
 * End of function cellular_set_atc_response
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_cleardata
 * Description    @details       Clear memory.
 * Arguments      @param[in]     p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_cleardata(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    int16_t                 cnt;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

#if CELLULAR_CFG_URC_CHARGET_ENABLED == 1
    CELLULAR_CFG_URC_CHARGET_FUNCTION(p_ctrl->sci_ctrl.receive_buff);
#endif

    for (cnt = 0; cnt < CELLULAR_ATC_RESPONSE_BUFF_SIZE; cnt++)
    {
        if ((p_ctrl->sci_ctrl.receive_buff[cnt] < 0x20) || (p_ctrl->sci_ctrl.receive_buff[cnt] > 0x7e))
        {
            if ('\0' == p_ctrl->sci_ctrl.receive_buff[cnt])
            {
                cnt = CELLULAR_ATC_RESPONSE_BUFF_SIZE;
            }
            else if (('\n' != p_ctrl->sci_ctrl.receive_buff[cnt]) && ('\r' != p_ctrl->sci_ctrl.receive_buff[cnt]))
            {
                p_ctrl->sci_ctrl.receive_buff[cnt] = 0;
            }
            else
            {
                /* Do Nothing */
                R_BSP_NOP();
            }
        }
    }
    p_ctrl->sci_ctrl.receive_buff[CELLULAR_ATC_RESPONSE_BUFF_SIZE-1] = (uint8_t)CHAR_END;   //cast
    CELLULAR_LOG_DEBUG(("URC = %s\n", p_ctrl->sci_ctrl.receive_buff));

    memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
    memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
    p_cellular_receive->job_no = CELLULAR_RES_NONE;

    return;
}
/**********************************************************************************************************************
 * End of function cellular_cleardata
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_charcheck
 * Description    @details       Character check.
 * Arguments      @param[in]     p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_charcheck(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if (CHAR_CHECK_1 == p_cellular_receive->data)
    {
        p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->recv_count] = p_cellular_receive->data;
        p_cellular_receive->recv_count++;
        p_cellular_receive->job_status = JOB_STATUS_COLON_CHECK;
    }
    else if (CHAR_CHECK_2 == p_cellular_receive->data)
    {
        p_cellular_receive->job_no = CELLULAR_RES_GO_SEND;
    }
    else if (CHAR_CHECK_7 == p_cellular_receive->data)
    {
        p_cellular_receive->recv_count = 1;
    }
    else
    {
        p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->recv_count] = p_cellular_receive->data;
        p_cellular_receive->recv_count++;
        p_cellular_receive->job_no = CELLULAR_RES_CHECK;
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_charcheck
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            binary_conversion
 * Description    @details       Convert binary numbers to decimal numbers.
 **********************************************************************************************/
static int32_t binary_conversion(int32_t binary)
{
    int32_t decimal = 0;
    int32_t base    = 1;

    while (binary > 0)
    {
        decimal = decimal + ((binary % 10) * base);
        binary  = binary / 10;
        base    = base * 2;
    }

    return decimal;
}
/**********************************************************************************************************************
 * End of function binary_conversion
 *********************************************************************************************************************/
