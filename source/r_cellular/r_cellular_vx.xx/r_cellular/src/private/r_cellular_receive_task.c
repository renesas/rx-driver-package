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
 * File Name    : r_cellular_receive_task.c
 * Description  : Functions of the receiving task.
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
#include "r_cellular_if.h"
#include "r_irq_rx_pinset.h"
#include "cellular_freertos.h"
#include "cellular_receive_task.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CHAR_CHECK_1    ('+')
#define CHAR_CHECK_2    ('>')
#define CHAR_CHECK_3    (':')
#define CHAR_CHECK_4    ('\n')
#define CHAR_CHECK_5    (',')
#define CHAR_CHECK_6    (' ')

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == (5)
extern st_cellular_ctrl_t * gp_cellular_ctrl;
#endif

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static const uint8_t s_atc_res_go_send[]            = ATC_RES_GO_SEND;
static const uint8_t s_atc_res_ok[]                 = ATC_RES_OK;
static const uint8_t s_atc_res_error[]              = ATC_RES_ERROR;
static const uint8_t s_atc_res_no_carrier[]         = ATC_RES_NO_CARRIER;
static const uint8_t s_atc_res_connect[]            = ATC_RES_CONNECT;
static const uint8_t s_atc_res_cpin_ready[]         = ATC_RES_CPIN_READY;
static const uint8_t s_atc_res_spin_sim_lock[]      = ATC_RES_CPIN_SIM_LOCK;
static const uint8_t s_atc_res_data_receive[]       = ATC_RES_DATA_RECEIVE;
static const uint8_t s_atc_res_data_receive_qird[]  = ATC_RES_DATA_RECEIVE_QIRD;
static const uint8_t s_atc_res_read_dns[]           = ATC_RES_READ_DNS;
static const uint8_t s_atc_res_pin_lock_status[]    = ATC_RES_PIN_LOCK_STATUS;
static const uint8_t s_atc_res_socket_close[]       = ATC_RES_SOCKET_CLOSE;
static const uint8_t s_atc_res_system_start[]       = ATC_RES_SYSTEM_START;
static const uint8_t s_atc_res_attach_status[]      = ATC_RES_ATTACH_STATUS;
static const uint8_t s_atc_res_function_level[]     = ATC_RES_FUNCTION_LEVEL;
static const uint8_t s_atc_res_timezone[]           = ATC_RES_TIMEZONE;
static const uint8_t s_atc_res_regist_status_1[]    = ATC_RES_REGIST_STATUS_1;
static const uint8_t s_atc_res_regist_status_2[]    = ATC_RES_REGIST_STATUS_2;
static const uint8_t s_atc_res_get_time[]           = ATC_RES_GET_TIME;
static const uint8_t s_atc_res_faital_error[]       = ATC_RES_FATAL_ERROR;
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

static irq_handle_t s_irq_handle = NULL;

static const uint8_t * const sp_cellular_atc_res_tbl[CELLULAR_RES_MAX] =
{
    s_atc_res_go_send,
    s_atc_res_ok,
    s_atc_res_error,
    s_atc_res_no_carrier,
    s_atc_res_connect,
    s_atc_res_cpin_ready,
    s_atc_res_spin_sim_lock,
    s_atc_res_data_receive,
    s_atc_res_data_receive_qird,
    s_atc_res_read_dns,
    s_atc_res_pin_lock_status,
    s_atc_res_socket_close,
    s_atc_res_system_start,
    s_atc_res_attach_status,
    s_atc_res_function_level,
    s_atc_res_timezone,
    s_atc_res_regist_status_1,
    s_atc_res_regist_status_2,
    s_atc_res_get_time,
    s_atc_res_faital_error,
    s_atc_res_get_service_status,
    s_atc_res_get_pdp_status,
    s_atc_res_get_ip_addr,
    s_atc_res_get_psms,
    s_atc_res_get_edrxs,
    s_atc_res_set_edrxs,
    s_atc_res_get_signal,
    s_atc_res_command_send_sim,
    s_atc_res_timezene_info,
    s_atc_res_ind_info
};

static void cellular_job_check (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static e_atc_return_code_t cellular_response_string_check (uint8_t * p_string);

static void cellular_responce_check (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_data_send_command (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_data_reception (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_request_data (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_store_data (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_dns_result (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_ap_connect_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_station_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_control_level (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_cpin_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_time (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_disconnect_socket (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_timezone (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_service_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_pdp_status (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_ip_addr (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_psms (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_edrx (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_get_signal (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_res_command_send_sim (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_timezone_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_ind_info (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);
static void cellular_response_skip (st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive);

static e_atc_list_t cellular_get_at_command (st_cellular_ctrl_t * const p_ctrl);
static void cellular_set_atc_response (st_cellular_ctrl_t * const p_ctrl, const e_cellular_atc_return_t result);
static void irq_callback (void * const p_Args);

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

    sci_err_t sci_ret = SCI_SUCCESS;
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
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(&cellular_receive, 0, sizeof(st_cellular_receive_t));
                cellular_receive.job_no = CELLULAR_RES_NONE;
            }
            else
            {
                p_ctrl->sci_ctrl.receive_buff[cellular_receive.recv_count] = cellular_receive.data;
                cellular_receive.recv_count++;
            }
            switch (cellular_receive.job_no)
            {
                case CELLULAR_RES_NONE:
                    cellular_job_check(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_CHECK:
                    cellular_responce_check(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_GO_SEND:
                    cellular_data_send_command(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_DATA_RECEIVE:
                    cellular_get_data_reception(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_DATA_RECEIVE_QIRD:
                    cellular_request_data(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_PUT_CHAR:
                    cellular_store_data(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_READ_DNS:
                    cellular_dns_result(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_ATTACH_STATUS:
                    cellular_get_ap_connect_status(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_REGIST_STATUS_1:
                case CELLULAR_RES_REGIST_STATUS_2:
                    cellular_station_info(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_FUNCTION_LEVEL:
                    cellular_control_level(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_PIN_LOCK_STATUS:
                    cellular_cpin_status(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_GET_TIME:
                    cellular_get_time(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_SOCKET_CLOSE:
                    cellular_disconnect_socket(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_TIMEZONE:
                    cellular_get_timezone(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_GET_SERVICE_STATUS:
                    cellular_get_service_status(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_GET_PDP_STATUS:
                    cellular_get_pdp_status(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_GET_IP_ADDR:
                    cellular_get_ip_addr(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_GET_PSMS:
                    cellular_get_psms(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_GET_EDRXS:
                case CELLULAR_SET_EDRXS:
                    cellular_get_edrx(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_GET_SIGNAL_QUALITY:
                    cellular_get_signal(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_TO_COMMAND_SEND_SIM:
                    cellular_res_command_send_sim(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_TIMEZONE_INFO:
                    cellular_timezone_info(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_IND_INFO:
                    cellular_ind_info(p_ctrl, &cellular_receive);
                    break;
                case CELLULAR_RES_MAX:
                    cellular_response_skip(p_ctrl, &cellular_receive);
                    break;
                default:
                    memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                    memset(&cellular_receive, 0, sizeof(st_cellular_receive_t));
                    cellular_receive.job_no = CELLULAR_RES_NONE;
                    break;
            }
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
            if ((char)p_cellular_receive->data == (CHAR_CHECK_4))               // (uint8_t)->(char)
            {
                if (NULL != strstr((char *) &p_ctrl->sci_ctrl.receive_buff[0],  //(uint8_t *)->(char *)
                                        ATC_RES_BEGIN_OR_END))
                {
                    p_cellular_receive->job_status = JOB_STATUS_FIRST_CHAR_CHECK;
                    p_cellular_receive->tmp_recvcnt = p_cellular_receive->recv_count;
                }
                else
                {
                    memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                    memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                    p_cellular_receive->job_no = CELLULAR_RES_NONE;
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
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
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
            memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
            memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
            p_cellular_receive->job_no = CELLULAR_RES_NONE;
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
    uint8_t i = 0;
    e_atc_return_code_t res = CELLULAR_RES_MAX;

    while (i < CELLULAR_RES_MAX)
    {
        if (NULL != strstr((const char *)&p_string[0],                  //(uint8_t *)->(char *)
                            (const char *)sp_cellular_atc_res_tbl[i]))  //(uint8_t *)->(char *)
        {
            break;
        }
        else
        {
            i++;
        }
    }
    if (i < CELLULAR_RES_MAX)
    {
        res = (e_atc_return_code_t)i; //(uint8_t)->(e_atc_return_code_t)
    }

    return res;
}
/**********************************************************************************************************************
 * End of function cellular_response_string_check
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_responce_check
 * Description    @details       Parses the received string and returns the result.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_responce_check(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    if ((char)p_cellular_receive->data == (CHAR_CHECK_4))  //(uint8_t)->(char)
    {
        p_cellular_receive->job_no = cellular_response_string_check(&p_ctrl->sci_ctrl.receive_buff[0]);

        switch (p_cellular_receive->job_no)
        {
            case CELLULAR_RES_OK:
            case CELLULAR_RES_CONNECT:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_OK);
                break;
            }
            case CELLULAR_RES_ERROR:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
                break;
            }
            case CELLULAR_RES_NO_CARRIER:
            {
                cellular_set_atc_response(p_ctrl, ATC_RETURN_SEND_NO_CARRIER);
                break;
            }
            case CELLULAR_RES_FAITAL_ERROR:
            {
                CELLULAR_LOG_ERROR(("The module froze up\n"));
                break;
            }
            default:
            {
                if (ATC_GET_SW_VERSION == p_ctrl->sci_ctrl.at_command)
                {
                    R_BSP_NOP();
                }
                if (ATC_GET_SERIAL_NUMBER == p_ctrl->sci_ctrl.at_command)
                {
                    R_BSP_NOP();
                }
                if (ATC_GET_MODULE_NAME == p_ctrl->sci_ctrl.at_command)
                {
                    R_BSP_NOP();
                }
                if (ATC_GET_MAKER_NAME == p_ctrl->sci_ctrl.at_command)
                {
                    R_BSP_NOP();
                }
                if (ATC_GET_IMSI == p_ctrl->sci_ctrl.at_command)
                {
                    R_BSP_NOP();
                }
                break;
            }
        }
        memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
        memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
        p_cellular_receive->job_no = CELLULAR_RES_NONE;
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_responce_check
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

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_6:
        {
            cellular_set_atc_response(p_ctrl, ATC_RETURN_OK_GO_SEND);
            memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
            memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
            p_cellular_receive->job_no = CELLULAR_RES_NONE;
            break;
        }
        default:
        {
            break;
        }
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
    int32_t sscanf_ret = 0;
    int32_t socket_no = 0;
    int32_t length    = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
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
                    p_cellular_receive->socket_no = socket_no;
                    p_cellular_receive->socket_no -= CELLULAR_START_SOCKET_NUMBER;
                    p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size += length;
                    p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count = 0;
                    p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_flg = CELLULAR_RECEIVE_FLAG_ON;
                }
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
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
    int32_t sscanf_ret = 0;
    int32_t socket_no = 0;
    int32_t length = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
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
                    p_cellular_receive->socket_no = socket_no;
                    p_cellular_receive->socket_no -= CELLULAR_START_SOCKET_NUMBER;
                    p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size -= length;

                    if (0 >= p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size)
                    {
                        p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_unprocessed_size = 0;
                        p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_flg = CELLULAR_RECEIVE_FLAG_OFF;
                    }
                    p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_num = length;
                    p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count = 0;
                    p_cellular_receive->job_no = CELLULAR_RES_PUT_CHAR;
                }
                else
                {
                    memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                    memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                    p_cellular_receive->job_no = CELLULAR_RES_NONE;
                    cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
                }
            }
            break;
        }
        default:
        {
            break;
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
        if (4 <= p_cellular_receive->recv_count)
        {
            p_cellular_receive->tmp_recvcnt = p_cellular_receive->recv_count - 4;
        }
        else
        {
            p_cellular_receive->tmp_recvcnt = 0;
        }

        if (NULL != strstr((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            ATC_RES_OK))
        {
            memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
            memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
            p_cellular_receive->job_no = CELLULAR_RES_NONE;
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_count = 0;
            p_ctrl->p_socket_ctrl[p_cellular_receive->socket_no].receive_num = 0;
            cellular_set_atc_response(p_ctrl, ATC_RETURN_OK);
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
    int32_t sscanf_ret;
    int32_t dns_1 = 0;
    int32_t dns_2 = 0;
    int32_t dns_3 = 0;
    int32_t dns_4 = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_5:
        {
            p_cellular_receive->tmp_recvcnt = p_cellular_receive->recv_count;
            break;
        }
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_OK))
            {
                sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                                &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                    "%ld.%ld.%ld.%ld", &dns_1, &dns_2, &dns_3, &dns_4);
                if (4 == sscanf_ret)
                {
                    p_ctrl->dns_address[0] = dns_1;
                    p_ctrl->dns_address[1] = dns_2;
                    p_ctrl->dns_address[2] = dns_3;
                    p_ctrl->dns_address[3] = dns_4;
                    cellular_set_atc_response(p_ctrl, ATC_RETURN_OK);
                }
                else
                {
                    cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
                }
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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
    int32_t  sscanf_ret = 0;
    int32_t  status     = 0;
    e_atc_list_t active_atc;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_OK))
            {
                active_atc = cellular_get_at_command(p_ctrl);

                if (ATC_CONNECT_CHECK == active_atc)
                {
                    sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                                    &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                        " %ld" , &status);
                    if ((1 == sscanf_ret) && (1 == status))
                    {
                        p_ctrl->system_state = CELLULAR_SYSTEM_CONNECT;
                        cellular_set_atc_response(p_ctrl, ATC_RETURN_OK);
                    }
                    else
                    {
                        p_ctrl->system_state = CELLULAR_SYSTEM_CLOSE;
                        cellular_set_atc_response(p_ctrl, ATC_RETURN_AP_NOT_CONNECT);
                    }
                }
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_ap_connect_status
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
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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
    int32_t sscanf_ret = 0;
    int32_t status     = 0;
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_OK))
            {
                sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                                &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                    " %ld", &status);

                if ((1 == sscanf_ret) && (1 == status))
                {
                    p_ctrl->module_status = CELLULAR_MODULE_OPERATING_LEVEL1;
                }
                else if ((1 == sscanf_ret) && (4 == status))
                {
                    p_ctrl->module_status = CELLULAR_MODULE_OPERATING_LEVEL4;
                }
                else
                {
                    /* Do Nothing */
                    CELLULAR_LOG_DEBUG(("Received unexpected string"));
                }
                cellular_set_atc_response(p_ctrl, ATC_RETURN_OK);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_OK))
            {
                if (NULL != strstr((char *) //(uint8_t *)->(char *)
                                &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                    ATC_RES_CPIN_READY))
                {
                    cellular_set_atc_response(p_ctrl, ATC_RETURN_CPIN_READY);
                }
                else
                {
                    if (NULL != strstr((char *) //(uint8_t *)->(char *)
                                    &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                        ATC_RES_CPIN_SIM_LOCK))
                    {
                        cellular_set_atc_response(p_ctrl, ATC_RETURN_SIM_LOCK);
                    }
                    else
                    {
                        cellular_set_atc_response(p_ctrl, ATC_RETURN_ERROR);
                    }
                }
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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
    st_cellular_receive_t * p_cellular_receive = cellular_receive;
    int32_t year    = 0;
    int32_t month   = 0;
    int32_t day     = 0;
    int32_t hour    = 0;
    int32_t min     = 0;
    int32_t sec     = 0;
    int32_t timezone = 0;
    int32_t ret = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            memset((void *)&p_ctrl->sci_ctrl.time, //(st_cellular_datetime_t *)->(void *)
                    0, sizeof(p_ctrl->sci_ctrl.time));

            ret = sscanf((char *)&p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt], //(&uint8_t[]->char*)
                    " \"%2d/%2d/%2d,%2d:%2d:%2d%3d\"\r\n",
                    &year, &month, &day, &hour, &min, &sec, &timezone);

            if (7 == ret)
            {
                p_ctrl->sci_ctrl.time.year = year;
                p_ctrl->sci_ctrl.time.month = month;
                p_ctrl->sci_ctrl.time.day = day;
                p_ctrl->sci_ctrl.time.hour = hour;
                p_ctrl->sci_ctrl.time.min = min;
                p_ctrl->sci_ctrl.time.sec = sec;
                p_ctrl->sci_ctrl.time.timezone = timezone;
            }

            memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
            memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
            p_cellular_receive->job_no = CELLULAR_RES_NONE;
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_time
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
    st_cellular_receive_t * p_cellular_receive = cellular_receive;
    int32_t sscanf_ret = 0;
    int32_t socket_no = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf_ret = sscanf((char *) //(uint8_t *)->(char *)
                                &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                    " %ld" , &socket_no);
                if (0 != sscanf_ret)
                {
                    p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].socket_status
                    = CELLULAR_SOCKET_STATUS_SOCKET;
                }

                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_timezone
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_service_status
 * Description    @details       Processing of network service status notification from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_service_status(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    int32_t status_1 = 0;
    int32_t status_2 = 0;
    int32_t status_3 = 0;
    int32_t status_4 = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %ld,%ld,\"%ld\",%ld",
                        &status_1, &status_2, &status_3, &status_4);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_service_status
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_pdp_status
 * Description    @details       Processing of network service status notification from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_pdp_status(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    int32_t status_1 = 0;
    int32_t status_2 = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %ld,%ld", &status_1, &status_2);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_pdp_status
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_ip_addr
 * Description    @details       Processing of network service status notification from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_ip_addr(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    int32_t context_id = 0;
    int32_t ip_addr_1 = 0;
    int32_t ip_addr_2 = 0;
    int32_t ip_addr_3 = 0;
    int32_t ip_addr_4 = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %ld,\"%ld.%ld.%ld.%ld\"",
                        &context_id, &ip_addr_1, &ip_addr_2, &ip_addr_3, &ip_addr_4);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_ip_addr
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_psms
 * Description    @details       Processing of network service status notification from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_psms(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    int32_t status = 0;
    int32_t tau = 0;
    int32_t active_time = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %ld,,,\"%ld\",\"%ld\"",
                        &status, &tau, &active_time);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_psms
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            cellular_get_edrx
 * Description    @details       Processing of network service status notification from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_edrx(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    int32_t status = 0;
    int32_t act_type = 0;
    int32_t requested_edrx = 0;
    int32_t provided_edrx = 0;
    int32_t ptw = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                if (ATC_GET_EDRXS == p_ctrl->sci_ctrl.at_command)
                {
                    sscanf((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            " %ld,%ld,\"%ld\",\"%ld\"",
                            &status, &act_type, &provided_edrx, &ptw);

                }
                else
                {
                    sscanf((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                            " %ld,\"%ld\",\"%ld\",\"%ld\"",
                            &status, &requested_edrx, &provided_edrx, &ptw);

                }

                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_get_edrx
 *********************************************************************************************************************/


/***********************************************************************************************
 * Function Name  @fn            cellular_get_signal
 * Description    @details       Processing of network service status notification from the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] cellular_receive -
 *                                  Pointer to structure for analysis processing.
 **********************************************************************************************/
static void cellular_get_signal(st_cellular_ctrl_t * p_ctrl, st_cellular_receive_t * cellular_receive)
{
    st_cellular_receive_t * p_cellular_receive = cellular_receive;

    int32_t rssi = 0;
    int32_t ber = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %ld,%ld", &rssi, &ber);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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
    st_cellular_receive_t * p_cellular_receive = cellular_receive;
    uint8_t responce[100 + 1] = {0};

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %s", &responce);
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
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
    int32_t ind = 0;
    int32_t value = 0;

    switch (p_cellular_receive->data)
    {
        case CHAR_CHECK_4:
        {
            if (NULL != strstr((char *) //(uint8_t *)->(char *)
                            &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                                ATC_RES_BEGIN_OR_END))
            {
                sscanf((char *) //(uint8_t *)->(char *)
                        &p_ctrl->sci_ctrl.receive_buff[p_cellular_receive->tmp_recvcnt],
                        " %ld,%ld", &ind, &value);
                if ((0 == ind) && (0 == value))
                {
                    /* Start of Power Saving Mode */
                    CELLULAR_LOG_DEBUG(("PSM Mode ON\n"));
                    CELLULAR_SET_DR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = CELLULAR_PIN_DIRECTION_MODE_OUTPUT;
                    cellular_delay_task(1000);
                    R_ICU_PinSet();
                    if (R_IRQ_Open(IRQ_NUM_4, IRQ_TRIG_FALLING, IRQ_PRI_1, &s_irq_handle, irq_callback) != IRQ_SUCCESS)
                    {
                        CELLULAR_SET_DR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
                    }
                }
                p_cellular_receive = cellular_receive;
                memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
                memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
                p_cellular_receive->job_no = CELLULAR_RES_NONE;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_ind_info
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
        memset(p_ctrl->sci_ctrl.receive_buff, 0, sizeof(p_ctrl->sci_ctrl.receive_buff));
        memset(p_cellular_receive, 0, sizeof(st_cellular_receive_t));
        p_cellular_receive->job_no = CELLULAR_RES_NONE;
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_response_skip
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
    if (CELLULAR_ATC_RESPONCE_UNCONFIRMED == p_ctrl->sci_ctrl.atc_flg)
    {
        p_ctrl->sci_ctrl.atc_res = result;
        p_ctrl->sci_ctrl.atc_flg = CELLULAR_ATC_RESPONCE_CONFIRMED;
        CELLULAR_LOG_DEBUG(("received AT command response: %s", p_ctrl->sci_ctrl.receive_buff));
    }
    return;
}
/**********************************************************************************************************************
 * End of function cellular_set_atc_response
 *********************************************************************************************************************/

/***********************************************************************************************
 * Function Name  @fn            irq_callback
 * Description    @details       Callback function for irq.
 **********************************************************************************************/
static void irq_callback(void * const p_Args)
{
    /* End of Power Saving Mode */
    CELLULAR_SET_DR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
    R_IRQ_Close(s_irq_handle);
    CELLULAR_LOG_DEBUG(("PSM Mode OFF\n"));
}
/**********************************************************************************************************************
 * End of function irq_callback
 *********************************************************************************************************************/
