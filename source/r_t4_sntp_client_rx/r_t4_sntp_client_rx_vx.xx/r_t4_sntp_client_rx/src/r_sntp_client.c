
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/*******************************************************************************
* File Name     : r_sntp_client.c
* Version       : 1.00
* Device(s)     : Renesas microcomputer
* Tool-Chain    : Renesas compilers
* OS            :
* H/W Platform  :
* Description   :
* Limitations   :
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 01.02.2017 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include <stdint.h>
#include "r_t4_itcpip.h"
#include "r_t4_dns_client_rx_if.h"
#include "r_t4_sntp_client_rx_if.h"
#include "r_t4_sntp_client_rx_config.h"
#include "r_sntp_client.h"
#include "r_sys_time_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#if (SNTP_REQUEST_SEND_TIME_10MS < 1500)
    #error "Error! Invalid setting for SNTP_CFG_ACCURACY or SNTP_CFG_FREQUENCY_TOLERANCE in r_t4_sntp_config.h"
#endif


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct _CEP
{
    T_IPV4EP    dst_addr;
    T_IPV4EP    src_addr;
    UB          rcv_buf[48];
    UB          snd_buf[48];
    uint32_t    sntp_callback_timeout_timer;
    uint8_t     dns_flag_10min_passed;
    uint32_t    dns_interval_timer;
}CEP;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
extern void timer_interrupt(void* pdata);
extern dns_err_t  dns_client_process(void);

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

SNTP_CLIENT_STATIC sntp_packet_t sntp_packet;
SNTP_CLIENT_STATIC sntp_mng_t  sntp_mng;
SNTP_CLIENT_STATIC R_TCPIP_SNTP_CLIENT_SETTINGS sntp_client_settings;
SNTP_CLIENT_STATIC CEP cep[1];

#ifdef _UNIT_TEST
extern uint32_t count;
extern uint8_t flag_proc_C3;
extern uint8_t flag_proc_E5;
extern uint8_t flag_proc_F3;
extern uint8_t flag_proc_L3;
#endif

SNTP_CLIENT_STATIC sntp_err_t sntp_no_proc(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_error_proc(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_already_proc(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_close_proc(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_A1(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_B2(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_C3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_D3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_E4(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_E5(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_F3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_G3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_G6(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_H3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_H7(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_I3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_J3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_K3(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_K8(void);
SNTP_CLIENT_STATIC sntp_err_t sntp_proc_L3(void);

static const SNTP_PROC_FUNC sntp_proc_table[SNTP_PROC_NUM][SNTP_EVENT_NUM] =
{
    /* A. resting      */
    {
        sntp_proc_A1,       /* 1.call Open function           */
        sntp_error_proc,    /* 2.call Start function          */
        sntp_error_proc,    /* 3.periodic                     */
        sntp_error_proc,    /* 4.illegal DNS finished callback*/
        sntp_error_proc,    /* 5.normal DNS finished callback */
        sntp_error_proc,    /* 6.complete sending callback    */
        sntp_error_proc,    /* 7.complete receiving callback  */
        sntp_error_proc,    /* 8.complete canceling callback  */
        sntp_error_proc     /* 9.call Close function          */
    },

    /* B. stopping    */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_proc_B2,       /* 2.call Start function          */
        sntp_no_proc,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
     },


    /* C.waiting      */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_C3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* D.start DNS sending      */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_D3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* E.DNS Processing    */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_no_proc,       /* 3.periodic                     */
        sntp_proc_E4,       /* 4.illegal DNS finished callback*/
        sntp_proc_E5,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* F.start SNTP sending */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_F3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* G.wait to complete sending request */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_G3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_proc_G6,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* H.wait to complete receiving request        */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_H3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_proc_H7,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* I.finished      */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_I3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc        /* 8.complete canceling callback  */
    },

    /* J.cancelling */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_J3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* K.wait to complete canceling      */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_K3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_proc_K8,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    },

    /* L.cancelled          */
    {
        sntp_no_proc,       /* 1.call Open function           */
        sntp_already_proc,  /* 2.call Start function          */
        sntp_proc_L3,       /* 3.periodic                     */
        sntp_no_proc,       /* 4.illegal DNS finished callback*/
        sntp_no_proc,       /* 5.normal DNS finished callback */
        sntp_no_proc,       /* 6.complete sending callback    */
        sntp_no_proc,       /* 7.complete receiving callback  */
        sntp_no_proc,       /* 8.complete canceling callback  */
        sntp_close_proc     /* 9.call Close function          */
    }
};

SNTP_CLIENT_STATIC sntp_err_t sntp_no_proc(void)
{
    /* None */
    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_error_proc(void)
{
    /* None */
    return SNTP_ERR_NOT_OPENED;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_already_proc(void)
{
    /*None*/
    return SNTP_ERR_REQUEST_ALREADY_STARTED;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_close_proc(void)
{

    sntp_mng.proc = SNTP_PROC_REST;   /* proc <- A.rested */
    if (SYS_TIME_BAD_FUNCTION_POINTER == R_SYS_TIME_UnregisterPeriodicCallback((callback_from_sys_time_t)&sntp_client_process))
    {
        return SNTP_ERR_NOT_OPENED;
    }

    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_A1(void)
{
    sntp_mng.proc = SNTP_PROC_STOP;    /* proc <- B.stopped */
    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_B2(void)
{
    if (sntp_mng.proc == SNTP_PROC_STOP)
    {
        sntp_mng.proc = SNTP_PROC_START_DNS;    /* proc <- C.waited */
        return SNTP_SUCCESS;
    }
    else    /*if (sntp_mng.proc == SNTP_PROC_STOP)*/
    {
        sntp_mng.proc = SNTP_PROC_START_DNS;    /* proc <- C.waited */
        return SNTP_ERR_REQUEST_ALREADY_STARTED;
    }
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_C3(void)
{
#ifndef _UNIT_TEST
    static uint32_t count = SNTP_REQUEST_SEND_TIME_10MS;
#else
    flag_proc_C3 = 1;
#endif
    count--;
    sntp_mng.proc = SNTP_PROC_WAIT;    /* proc <- C.waited */
    if (count == 0)
    {
        if (cep[0].dns_flag_10min_passed == SNTP_DNS_TEN_MINUTES_NOT_PASSED)
        {
            sntp_mng.proc = SNTP_PROC_START;   /* proc <- F.start SNTP sending  */
        }
        else   /*if (cep[0].dns_flag_10min_passed == SNTP_DNS_TEN_MINUTES_NOT_PASSED)*/
        {
            sntp_mng.proc = SNTP_PROC_START_DNS;    /* proc <- D.start DNS sending  */
        }
#ifndef _UNIT_TEST
        count = SNTP_REQUEST_SEND_TIME_10MS;
#else
        count = 6000;
#endif

    }

    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_D3(void)
{
    uint8_t * server;
    sntp_err_t error_code;

    error_code = SNTP_SUCCESS;
    if (sntp_mng.selected_sntp_server ==  SNTP_SELECT_LOCAL_PRIMARY)
    {
        server = sntp_client_settings.local_primary_sntp_server_name;
    }
    else if (sntp_mng.selected_sntp_server ==  SNTP_SELECT_LOCAL_SECONDARY)
    {
        server = sntp_client_settings.local_secondary_sntp_server_name;
    }
    else if (sntp_mng.selected_sntp_server ==  SNTP_SELECT_GLOBAL_PRIMARY)
    {
        server = sntp_client_settings.global_primary_sntp_server_name;
    }
    else    /*if (sntp_mng.selected_sntp_server ==  SNTP_SELECT_LOCAL_PRIMARY)*/
    {
        server = sntp_client_settings.global_secondary_sntp_server_name;
    }
    R_TCPIP_DnsClientResolveName((char*)server, (DNS_CB_FUNC)callback_from_dns);
    cep[0].dns_flag_10min_passed = SNTP_DNS_TEN_MINUTES_NOT_PASSED;
    cep[0].dns_interval_timer = SNTP_DNS_INTERVAL_RESET;

    sntp_mng.proc = SNTP_PROC_DNS_PROCESSING;    /* proc <- E.processing DNS  */
    return error_code;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_E4(void)
{

    sntp_mng.sntp_error_type = SNTP_ERR_DNS_NOT_WORKED;
    sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- C.waited */
    sntp_mng.retry_cnt = 0;    /* no retry */

    return SNTP_ERR_DNS_NOT_WORKED;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_E5(void)
{
#ifdef _UNIT_TEST
    flag_proc_E5 = 1;
#endif
    sntp_mng.proc = SNTP_PROC_START;        /* proc <- F.start SNTP sending */

    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_F3(void)
{
    sntp_err_t error_code;

#ifdef _UNIT_TEST
    flag_proc_F3 = 1;
#endif

    calcurate_timeout();
    if (E_WBLK == sntp_send_request())
    {
        sntp_mng.proc = SNTP_PROC_CALLBACK_SEND_WAITING;  /* proc <- G.wait to complete sending request */
        error_code = SNTP_SUCCESS;
    }
    else /* if (E_WBLK == error_code) */
    {
        sntp_mng.proc = SNTP_PROC_CANCELLED;  /* proc <- M.cancelled SNTP */
        sntp_mng.retry_cnt = 0;     /* no retry */
        error_code = SNTP_ERR_BAD_UDP_SETTINGS;
    }
    return error_code;
}


SNTP_CLIENT_STATIC sntp_err_t sntp_proc_G3(void)
{
    sntp_err_t error_code;

    error_code = SNTP_SUCCESS;
    error_code = sntp_timer();

    if (SNTP_ERR_TIMEOUT == sntp_timer())
    {
        sntp_mng.proc = SNTP_PROC_CANCELING;    /* proc <- J.cancelling */
        error_code = SNTP_ERR_TIMEOUT;
    }
    return error_code;
}

sntp_err_t sntp_proc_G6(void)
{
    sntp_err_t error_code;

    calcurate_timeout();

    if (E_WBLK == sntp_receive_response())
    {
        sntp_mng.proc = SNTP_PROC_CALLBACK_RECEIVE_WAITING;   /* proc <- H.wait to complete receiving request */
        error_code = SNTP_SUCCESS;
    }
    else /* (error_code == E_WBLK) */
    {
        sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- M.cancelled SNTP */
        sntp_mng.retry_cnt = 0;     /* no retry */
        error_code = SNTP_ERR_BAD_UDP_SETTINGS;
    }

    return error_code;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_H3(void)
{
    sntp_err_t error_code;

    error_code = SNTP_SUCCESS;
    if (SNTP_ERR_TIMEOUT == sntp_timer())
    {
        sntp_mng.proc = SNTP_PROC_CANCELING;    /* proc <- J.cancelling */
        error_code = SNTP_ERR_TIMEOUT;
    }
    return error_code;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_H7(void)
{
    uint32_t client_request_timestamp;

    client_request_timestamp = sntp_packet.transmit.stamp32.seconds;
    memcpy(&sntp_packet, cep[0].rcv_buf,sizeof(cep[0].rcv_buf));

    if (SNTP_ABNORMAL_DATA == sntp_check_answer(client_request_timestamp))
    {
        sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- M.cancelled SNTP*/
        sntp_mng.retry_cnt = 0;     /* no retry */
        return SNTP_ERR_SERVER_NOT_WORKED;
    }
    else  /*  if (error_code == SNTP_ABNORMAL_DATA)  */
    {
        sntp_mng.proc = SNTP_PROC_FINISH;    /* proc <- I.finished*/
        return SNTP_SUCCESS;
    }
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_I3(void)
{
    uint32_t timestamp;
    uint32_t unix_time;
    SYS_TIME sys_time;
    uint8_t systime_error_code;
    sntp_err_t error_code;

    timestamp = htons32(sntp_packet.transmit.stamp32.seconds);

    unix_time = convert_ntptime_to_unixtime(timestamp);
    R_SYS_TIME_ConvertUnixTimeToSystemTime(unix_time, &sys_time, sntp_client_settings.time_offset);
    systime_error_code = R_SYS_TIME_SetCurrentTime(&sys_time);

    if (systime_error_code == SYS_TIME_SUCCESS)
    {
        sntp_mng.proc = SNTP_PROC_WAIT;    /* proc <- C.waited */
        error_code = SNTP_SUCCESS;
        sntp_mng.retry_cnt = SNTP_ERROR_COUNT;                          /* Initialize retry counter       */

        if (NULL != sntp_mng.cb_func)
        {
            sntp_mng.cb_func(error_code, timestamp);
        }
    }
    else    /*if (systime_error_code == SYS_TIME_SUCCESS)*/
    {
        sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- M.cancelled SNTP */
        error_code = SNTP_ERR_SERVER_NOT_WORKED;
    }

    return error_code;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_J3(void)
{
    sntp_err_t error_code;

    sntp_mng.calcurated_sntp_timeout = SNTP_ERR_TIMEOUT_RESET;
    calcurate_timeout();

    if (SNTP_SUCCESS == udp_can_cep(sntp_mng.cepid, TFN_UDP_ALL))
    {
        sntp_mng.proc = SNTP_PROC_CANCELING_WAIT;    /* proc <- K.wait to complete canceling */
        error_code = SNTP_ERR_TIMEOUT;
    }
    else    /* if (SNTP_SUCCESS == udp_can_cep(sntp_mng.cepid, TFN_UDP_ALL)) */
    {
        error_code = SNTP_ERR_BAD_UDP_SETTINGS;
        sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- C.waited */
        sntp_mng.retry_cnt = 0;    /* no retry */
    }
    return error_code;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_K3(void)
{

    if (SNTP_ERR_TIMEOUT == sntp_timer())
    {
        sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- M.cancelled SNTP */
        sntp_mng.retry_cnt = 0;     /* no retry */
    }
    return SNTP_ERR_TIMEOUT;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_K8(void)
{
    sntp_err_t error_code;

    sntp_mng.proc = SNTP_PROC_CANCELLED;    /* proc <- M.cancelled SNTP */
    error_code = SNTP_ERR_TIMEOUT;

    return error_code;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_proc_L3(void)
{

#ifdef _UNIT_TEST
    flag_proc_L3 = 1;
#endif
    if (sntp_mng.retry_cnt > 0)
    {
        sntp_mng.retry_cnt--;
        sntp_mng.proc = SNTP_PROC_START;   /* proc <- F.start SNTP sending */
    }
    else    /* if (sntp_mng.retry_cnt > 0) */
    {
        if (sntp_mng.sntp_error_type == SNTP_ERR_DNS_NOT_WORKED)
        {
            sntp_mng.proc = SNTP_PROC_WAIT;    /* proc <- C.waited */
            if (NULL != sntp_mng.cb_func)
            {
                sntp_mng.cb_func(sntp_mng.sntp_error_type, NULL);
            }

        }
        else if (sntp_mng.sntp_error_type == SNTP_ERR_BAD_UDP_SETTINGS)
        {
            sntp_mng.proc = SNTP_PROC_WAIT;    /* proc <- C.waited */
            if (NULL != sntp_mng.cb_func)
            {
                sntp_mng.cb_func(sntp_mng.sntp_error_type, NULL);
            }
        }
        else if (sntp_mng.sntp_error_type == SNTP_ERR_TIMEOUT)
        {
            if (SNTP_SUCCESS == sntp_server_select())
            {
                sntp_mng.retry_cnt = SNTP_ERROR_COUNT;
                cep[0].dns_flag_10min_passed = SNTP_DNS_TEN_MINUTES_PASSED;
                sntp_mng.proc = SNTP_PROC_START_DNS;    /* proc <- D.start DNS sending  */
            }
            else    /* if (SNTP_SUCCESS == sntp_server_select()) */
            {
                sntp_mng.sntp_error_type = SNTP_ERR_TIMEOUT;
                sntp_mng.proc = SNTP_PROC_WAIT;    /* proc <- C.waited */

                if (NULL != sntp_mng.cb_func)
                {
                    sntp_mng.cb_func(sntp_mng.sntp_error_type, NULL);
                }
            }
        }
        else if (sntp_mng.sntp_error_type == SNTP_ERR_SERVER_NOT_WORKED)    /*if (sntp_mng.sntp_error_type == E_SNTP_ERR_DNS_NOT_WORKED)*/
        {
            if (SNTP_SUCCESS == sntp_server_select())
            {
                sntp_mng.retry_cnt = SNTP_ERROR_COUNT;
                cep[0].dns_flag_10min_passed = SNTP_DNS_TEN_MINUTES_PASSED;
                sntp_mng.proc = SNTP_PROC_START_DNS;   /* proc <- D.start DNS sending  */
            }
            else    /* if (SNTP_SUCCESS == sntp_server_select()) */
            {
                sntp_mng.sntp_error_type = SNTP_ERR_SERVER_NOT_WORKED;
                sntp_mng.proc = SNTP_PROC_WAIT;  /* proc <- C.waited */

                if (NULL != sntp_mng.cb_func)
                {
                    sntp_mng.cb_func(sntp_mng.sntp_error_type, NULL);
                }
            }
        }
        else    /*if (sntp_mng.sntp_error_type == E_SNTP_ERR_DNS_NOT_WORKED)*/
        {
            /* none */
        }
    }
    return SNTP_SUCCESS;
}

void callback_from_dns(int32_t ercd, name_table_t *name_table)
{

    if (ercd == SNTP_SUCCESS)
    {
        ercd = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_DNS_NORMAL_FINISH]();
        sntp_mng.sntp_server_ipaddr = convert_array_to_long(name_table->ipaddr);
    }
    else  /* if (ercd == SNTP_SUCCESS) */
    {
        ercd = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_DNS_ILLEGAL_FINISH]();
    }
}

SNTP_CLIENT_STATIC uint32_t convert_array_to_long(uint8_t *ipaddr)
{
    uint32_t tmp = 0;

    tmp = ((((uint32_t)ipaddr[0]) << 24) & 0xFF000000);
    tmp |= ((((uint32_t)ipaddr[1]) << 16) & 0x00FF0000);
    tmp |= ((((uint32_t)ipaddr[2]) << 8)  & 0x0000FF00);
    tmp |= (((uint32_t)ipaddr[3])      & 0x000000FF);

    return tmp;
}

SNTP_CLIENT_STATIC uint32_t htons32(uint32_t data)
{
#ifndef __BIG
    uint32_t tmp = 0;
    tmp  = (data & 0x000000ff) << 24;
    tmp |= (data & 0x0000ff00) << 8;
    tmp |= (data & 0x00ff0000) >> 8;
    tmp |= (data & 0xff000000) >> 24;
    data = tmp;
#endif
    return data;
}

SNTP_CLIENT_STATIC void calcurate_timeout(void)
{

    if (sntp_mng.calcurated_sntp_timeout == 0)
    {
        sntp_mng.calcurated_sntp_timeout = SNTP_ERR_TIMEOUT_15_SECONDS;
    }
    else if (sntp_mng.calcurated_sntp_timeout == SNTP_ERR_TIMEOUT_15_SECONDS)    /* if (sntp_mng.calcurated_sntp_timeout == 0) */
    {
        sntp_mng.calcurated_sntp_timeout = SNTP_ERR_TIMEOUT_225_SECONDS;
    }
    else    /* if (sntp_mng.calcurated_sntp_timeout == 0) */
    {
        sntp_mng.calcurated_sntp_timeout = SNTP_ERR_TIMEOUT_3_SECONDS;
    }
    cep[0].sntp_callback_timeout_timer = sntp_mng.calcurated_sntp_timeout;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_timer(void)
{
    cep[0].sntp_callback_timeout_timer--;

    if (cep[0].sntp_callback_timeout_timer == 0)
    {
        return SNTP_ERR_TIMEOUT;
    }
    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC void interval_dns_timer(void)
{
    cep[0].dns_interval_timer--;
    if (cep[0].dns_interval_timer == 0)
    {
        cep[0].dns_flag_10min_passed = SNTP_DNS_TEN_MINUTES_PASSED;
    }
}

SNTP_CLIENT_STATIC int32_t sntp_send_request(void)
{
    ER len;
    SYS_TIME sys_time;

    cep[0].dst_addr.ipaddr = sntp_mng.sntp_server_ipaddr;
    cep[0].dst_addr.portno = 123;

    sntp_packet.field1.header.LI = 0;
    sntp_packet.field1.header.VN = 4;
    sntp_packet.field1.header.mode = 3;
    sntp_packet.field1.header.stratum = 0;
    sntp_packet.field1.header.poll = 0x00;
    sntp_packet.field1.header.precision = 0x00;

    sntp_packet.rootdelay = htons32(0x00000000);
    sntp_packet.rootdispersion = htons32(0x00000000);
    sntp_packet.refid = NULL;
    sntp_packet.reference.stamp32.fraction = htons32(0x00000000);
    sntp_packet.reference.stamp32.seconds = htons32(0x00000000);
    sntp_packet.originate.stamp32.fraction = htons32(0x00000000);
    sntp_packet.originate.stamp32.seconds = htons32(0x00000000);
    sntp_packet.receive.stamp32.fraction = htons32(0x00000000);
    sntp_packet.receive.stamp32.seconds = htons32(0x00000000);
    sntp_packet.transmit.stamp32.fraction = htons32(0x00000000);

#ifdef _UNIT_TEST
    sys_time.year = 2036;
    sys_time.month = 2;
    sys_time.day = 7;
    sys_time.hour = 6;
    sys_time.min = 28;
    sys_time.sec = 16;
    R_SYS_TIME_SetCurrentTime(&sys_time);
#endif

    R_SYS_TIME_GetCurrentTime(&sys_time);
    if (sys_time.unix_time < 0x7C558180)
    {
        sntp_packet.transmit.stamp32.seconds = htons32(sys_time.unix_time + 0x83AA7E80);    /* transmit timestamp is the total number of seconds from 1970/01/01 to current time */
    }
    else
    {
        sntp_packet.transmit.stamp32.seconds = htons32(sys_time.unix_time - 0x7C558180);    /* transmit timestamp is the total number of seconds from 2036/02/07 to current time */
    }



    memcpy(cep[0].snd_buf, &sntp_packet, sizeof(sntp_packet));

    len = udp_snd_dat(sntp_mng.cepid, &cep[0].dst_addr, cep[0].snd_buf, sizeof(sntp_packet) , TMO_NBLK);

    return len;
}

SNTP_CLIENT_STATIC int32_t sntp_receive_response(void)
{
    int32_t len;

    len = udp_rcv_dat(sntp_mng.cepid, &cep[0].dst_addr, cep[0].rcv_buf, sizeof(cep[0].rcv_buf) , TMO_NBLK);

    return len;
}

SNTP_CLIENT_STATIC int32_t sntp_check_answer(uint32_t request_time)
{
    if (sntp_packet.field1.header.stratum == 0 || sntp_packet.field1.header.LI == 3 || sntp_packet.field1.header.mode != 4 || sntp_packet.transmit.receive == 0)
    {
        return SNTP_ABNORMAL_DATA;
    }
    if (cep[0].dst_addr.ipaddr != sntp_mng.sntp_server_ipaddr)
    {
        return SNTP_ABNORMAL_DATA;
    }
    if (sntp_packet.originate.stamp32.seconds != request_time)
    {
        return SNTP_ABNORMAL_DATA;
    }
    return SNTP_NORMAL_DATA;
}

SNTP_CLIENT_STATIC sntp_err_t sntp_server_select(void)
{
    if (sntp_mng.selected_sntp_server == SNTP_SELECT_NUM)
    {
        sntp_mng.selected_sntp_server = SNTP_SELECT_LOCAL_PRIMARY;           /* selected sntp server */
        if (sntp_client_settings.local_primary_sntp_server_name != NULL)
        {
            return SNTP_SUCCESS;
        }
    }
    if (sntp_mng.selected_sntp_server == SNTP_SELECT_LOCAL_PRIMARY)
    {
        sntp_mng.selected_sntp_server = SNTP_SELECT_LOCAL_SECONDARY;           /* selected sntp server */
        if (sntp_client_settings.local_secondary_sntp_server_name != NULL)
        {
            return SNTP_SUCCESS;
        }
    }
    if (sntp_mng.selected_sntp_server == SNTP_SELECT_LOCAL_SECONDARY)
    {
        sntp_mng.selected_sntp_server = SNTP_SELECT_GLOBAL_PRIMARY;           /* selected sntp server */
        if (sntp_client_settings.global_primary_sntp_server_name != NULL)
        {
            return SNTP_SUCCESS;
        }
    }
    if (sntp_mng.selected_sntp_server == SNTP_SELECT_GLOBAL_PRIMARY)
    {
        sntp_mng.selected_sntp_server = SNTP_SELECT_GLOBAL_SECONDARY;          /* selected sntp server */
        if (sntp_client_settings.global_secondary_sntp_server_name != NULL)
        {
            return SNTP_SUCCESS;
        }
    }
    if (sntp_mng.selected_sntp_server == SNTP_SELECT_GLOBAL_SECONDARY)
    {
        return SNTP_ERR_SERVER_NOT_WORKED;
    }

    return SNTP_SUCCESS;
}

SNTP_CLIENT_STATIC uint32_t convert_ntptime_to_unixtime(uint32_t ntp_time)
{
    uint32_t detect_2036;
    uint32_t unix_time;

    detect_2036 = ntp_time >> 31;

    if (detect_2036 != 0)
    {
        unix_time = ntp_time - 0x83AA7E80;    /* ntp time is the total number of seconds from 1970/01/01 0:00:00 to current time */
    }
    else  /* if (detect_2036 != 0) */
    {
        unix_time = ntp_time + 0x7C558180;    /* ntp time is the total number of seconds from 2036/02/07 6:28:16 to current time */
    }
    return unix_time;
}

/***********************************************************************************************************************
* Function Name: sntp_client_process
* Description  : SNTP process function
* Arguments    : none
* Return Value : error code
***********************************************************************************************************************/
sntp_err_t sntp_client_process(void)
{
    sntp_err_t error_code;

    error_code = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_PERIODIC]();
    interval_dns_timer();
    if (error_code == SNTP_ERR_TIMEOUT)
    {
        sntp_mng.sntp_error_type = SNTP_ERR_TIMEOUT;
    }
    else if (error_code == SNTP_ERR_BAD_UDP_SETTINGS)
    {
        sntp_mng.sntp_error_type = SNTP_ERR_BAD_UDP_SETTINGS;
    }
    else if (error_code == SNTP_ERR_SERVER_NOT_WORKED)
    {
        sntp_mng.sntp_error_type = SNTP_ERR_SERVER_NOT_WORKED;
    }
    else if (error_code == SNTP_ERR_DNS_NOT_WORKED)
    {
        sntp_mng.sntp_error_type = SNTP_ERR_DNS_NOT_WORKED;
    }
    else
    {
        /* None */
    }

    return SNTP_SUCCESS;
}

/***********************************************************************************************************************
* Function Name: R_TCPIP_SntpClientOpen
* Description  : SNTP opening function
* Arguments    : function_pointer -
*                    callback function from SNTP
*                r_tcpip_sntp_client_settings-
*                    SNTP sever domain name
* Return Value : error code
***********************************************************************************************************************/

sntp_err_t R_TCPIP_SntpClientOpen(callback_from_sntp_t function_pointer, R_TCPIP_SNTP_CLIENT_SETTINGS *tcpip_sntp_client_settings)
{
    sntp_err_t error_code;

    if (false == R_SYS_TIME_IsPeriodicCallbackRegistered((callback_from_sys_time_t) &timer_interrupt))
    {
        return SNTP_ERR_MODULE_NOT_REGISTERED;
    }

    if (false == R_SYS_TIME_IsPeriodicCallbackRegistered((callback_from_sys_time_t) &dns_client_process))
    {
        return SNTP_ERR_MODULE_NOT_REGISTERED;
    }

    memcpy(&sntp_client_settings, tcpip_sntp_client_settings, sizeof(*tcpip_sntp_client_settings));
    sntp_mng.selected_sntp_server = SNTP_SELECT_NUM;

    if (SNTP_ERR_SERVER_NOT_WORKED == sntp_server_select())
    {
        return SNTP_ERR_SERVER_NOT_WORKED;
    }

    cep[0].dns_flag_10min_passed = SNTP_DNS_TEN_MINUTES_PASSED;
    cep[0].dns_interval_timer = SNTP_DNS_INTERVAL_RESET;
    sntp_mng.proc = SNTP_PROC_REST;                                 /* proc <- A.rested  */
    sntp_mng.cb_func = function_pointer;                            /* callback function <- functon_pointer      */
    sntp_mng.retry_cnt = SNTP_ERROR_COUNT;                          /* Initialize retry counter       */
    sntp_mng.cepid = SNTP_CFG_UDP_CEP_ID;                           /* invalid ID                     */
    error_code = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALL_OPEN_FUNCTION]();
    R_SYS_TIME_RegisterPeriodicCallback((callback_from_sys_time_t)&sntp_client_process, 1);

    return error_code;
 }

/***********************************************************************************************************************
* Function Name: R_TCPIP_SntpClientStartRequest
* Description  : SNTP start function
* Arguments    : none
* Return Value : error code
***********************************************************************************************************************/
sntp_err_t R_TCPIP_SntpClientStartRequest(void)
{
    sntp_err_t error_code;

    error_code = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALL_START_FUNCTION]();

    return error_code;
}

/***********************************************************************************************************************
* Function Name:  R_TCPIP_SntpClientClose
* Description  : SNTP closeing function
* Arguments    : none
* Return Value : error code
***********************************************************************************************************************/
sntp_err_t R_TCPIP_SntpClientClose(void)
{
    sntp_err_t error_code;

    error_code = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALL_CLOSE_FUNCTION]();

    return error_code;
}

/***********************************************************************************************************************
* Function Name: R_TCPIP_SntpClientCallback
* Description  : SNTP callback function
* Arguments    : cepid -
*                    the communication endpoint ID
*                fncd -
*                    function code
*                p_parblk -
*                    error code pointer
* Return Value : error code
***********************************************************************************************************************/
ER   R_TCPIP_SntpClientCallback(ID cepid, FN fncd , VP p_parblk)
{
    ER parblk;
    sntp_err_t error_code;

    parblk = *(ER *)p_parblk;

    switch (fncd)
     {
        case TEV_UDP_RCV_DAT:

            break;
        case TFN_UDP_RCV_DAT:
            if (0 < parblk)
            {
                sntp_mng.calcurated_sntp_timeout = SNTP_ERR_TIMEOUT_RESET;
                sntp_mng.sntp_error_type = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALLBACK_RECEIVE]();
            }
            else if (E_RLWAI == parblk)    /*if (0 < parblk)*/
            {
                sntp_mng.sntp_error_type = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALLBACK_CANCEL]();
            }
            else
            {
                /* None */
            }
            break;
        case TFN_UDP_SND_DAT:
            if (0 < parblk)
            {
                sntp_mng.calcurated_sntp_timeout = SNTP_ERR_TIMEOUT_RESET;
                error_code = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALLBACK_SEND]();
                if (error_code == SNTP_ERR_BAD_UDP_SETTINGS)
                {
                    sntp_mng.sntp_error_type = SNTP_ERR_BAD_UDP_SETTINGS;
                }
            }
            else if (E_RLWAI == parblk)    /*if (0 < parblk)*/
            {
                error_code = sntp_proc_table[sntp_mng.proc][SNTP_EVENT_CALLBACK_CANCEL]();
                if (error_code == SNTP_ERR_BAD_UDP_SETTINGS)
                {
                    sntp_mng.sntp_error_type = SNTP_ERR_BAD_UDP_SETTINGS;
                }
                else if (error_code == SNTP_ERR_TIMEOUT)
                {
                    sntp_mng.sntp_error_type = SNTP_ERR_TIMEOUT;
                }
                else
                {
                    /* None */
                }
            }
            else   /* if (0 < parblk) */
            {
                /* None */
            }
            break;
        default:  /* default */
            break;
    }
    return E_OK;
}

/*****************************************************************************
* Function Name: R_TCPIP_SntpClientGetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline(R_TCPIP_SntpClientGetVersion)
uint32_t  R_TCPIP_SntpClientGetVersion(void)
{
    uint32_t const version = (T4_SNTP_CLIENT_VERSION_MAJOR << 16) | T4_SNTP_CLIENT_VERSION_MINOR;

    return version;
}
