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
/***********************************************************************************************************************
* File Name    : r_sntp_client.h
* Version      : 1.00
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.02.2017 1.00    First Release
***********************************************************************************************************************/
#ifndef R_SNTP_CLIENT_H_
#define R_SNTP_CLIENT_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define SNTP_ERROR_COUNT    (2)
#define SNTP_DNS_INTERVAL_RESET    ((10 * 60 * 1000) /10)
#define SNTP_ERR_TIMEOUT_RESET    (0)
#define SNTP_DNS_TEN_MINUTES_PASSED    (1)
#define SNTP_DNS_TEN_MINUTES_NOT_PASSED    (0)
#define SNTP_ERR_TIMEOUT_15_SECONDS    ((15 * 1000) / 10)
#define SNTP_ERR_TIMEOUT_225_SECONDS    ((15 * 15)* 1000 / 10)
#define SNTP_ERR_TIMEOUT_3_SECONDS    ((3  * 1000) / 10)
#define SNTP_NORMAL_DATA    (1)
#define SNTP_ABNORMAL_DATA    (-1)
#define SNTP_SERVER_NO_CHOICE    (-1)
#define SNTP_REQUEST_SEND_TIME_10MS    (((SNTP_CFG_ACCURACY * 1000000) / SNTP_CFG_FREQUENCY_TOLERANCE) * 100)

#ifdef _UNIT_TEST
#define SNTP_CLIENT_STATIC
#else
#define SNTP_CLIENT_STATIC static
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#pragma bit_order left
#pragma pack
typedef struct _sntp{
    union{
        uint32_t headerall;
        struct{
            uint8_t LI :2;
            uint8_t VN :3;
            uint8_t mode :3;
            uint8_t stratum;
            uint8_t poll;
            uint8_t precision;
        }header;
    }field1;
    int32_t rootdelay;
    int32_t rootdispersion;
    int32_t refid;
    union{
        uint64_t timestamp;
        struct{
            uint32_t seconds;
            uint32_t fraction;
        }stamp32;
    }reference;
    union{
        uint64_t timestamp;
        struct{
            uint32_t seconds;
            uint32_t fraction;
        }stamp32;
    }originate;
    union{
        uint64_t timestamp;
        struct{
            uint32_t seconds;
            uint32_t fraction;
        }stamp32;
    }receive;
    union{
        uint64_t receive;
        struct{
            uint32_t seconds;
            uint32_t fraction;
        }stamp32;
    }transmit;
}sntp_packet_t;
#pragma unpack
#pragma bit_order

typedef enum _sntp_proc
{
    SNTP_PROC_REST = 0,                       /* A.rested                              */
    SNTP_PROC_STOP,                           /* B.stopped                             */
    SNTP_PROC_WAIT,                           /* C.waiting                             */
    SNTP_PROC_START_DNS,                      /* D.start DNS sending                   */
    SNTP_PROC_DNS_PROCESSING,                 /* E.processing DNS                      */
    SNTP_PROC_START,                          /* F.start SNTP sending                  */
    SNTP_PROC_CALLBACK_SEND_WAITING,          /* G.wait to complete sending request    */
    SNTP_PROC_CALLBACK_RECEIVE_WAITING,       /* H.wait to complete receiving request  */
    SNTP_PROC_FINISH,                         /* I.finished                            */
    SNTP_PROC_CANCELING,                      /* J.cancelling                          */
    SNTP_PROC_CANCELING_WAIT,                 /* K.wait to complete canceling          */
    SNTP_PROC_CANCELLED,                      /* L.cancelled SNTP                      */
    SNTP_PROC_NUM                             /* the number of process                 */
} sntp_proc_t;

typedef enum _sntp_event
{
    SNTP_EVENT_CALL_OPEN_FUNCTION,         /* 1.call R_TCPIP_SntpClientOpen  */
    SNTP_EVENT_CALL_START_FUNCTION,        /* 2.call R_TCPIP_SntpClientStart */
    SNTP_EVENT_PERIODIC,                   /* 3.periodic                     */
    SNTP_EVENT_DNS_ILLEGAL_FINISH,         /* 4.DNS illegal finished           */
    SNTP_EVENT_DNS_NORMAL_FINISH,          /* 5.DNS normal finished          */
    SNTP_EVENT_CALLBACK_SEND,              /* 6.complete sending callback    */
    SNTP_EVENT_CALLBACK_RECEIVE,           /* 7.complete receiving callback  */
    SNTP_EVENT_CALLBACK_CANCEL,            /* 8.complete canceling callback  */
    SNTP_EVENT_CALL_CLOSE_FUNCTION,        /* 9.call R_TCPIP_SntpClientOpen  */
    SNTP_EVENT_NUM                         /* the number of event            */
} sntp_event_t;

typedef enum _sntp_select
{
    SNTP_SELECT_LOCAL_PRIMARY,        /* select local primary SNTP server    */
    SNTP_SELECT_LOCAL_SECONDARY,      /* select local secondary SNTP server  */
    SNTP_SELECT_GLOBAL_PRIMARY,       /* select global primary SNTP server   */
    SNTP_SELECT_GLOBAL_SECONDARY,     /* select global secondary SNTP server */
    SNTP_SELECT_NUM                   /* the number of selecting SNTP server */
} sntp_select_t;

typedef struct _sntp_mng
{
    sntp_proc_t    proc;                   /* state of SNTP process          */
    sntp_select_t  selected_sntp_server;   /* selected SNTP server           */
    callback_from_sntp_t  cb_func;         /* callback function              */
    uint32_t    retry_cnt;                 /* retry counter                  */
    ID          cepid;                     /* the communication endpoint ID  */
    uint32_t    sntp_server_ipaddr;
    uint32_t    calcurated_sntp_timeout;
    int32_t     sntp_error_type;
} sntp_mng_t;

typedef sntp_err_t (*SNTP_PROC_FUNC)(void);

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
SNTP_CLIENT_STATIC int32_t  sntp_check_answer(uint32_t request_time);
SNTP_CLIENT_STATIC int32_t  sntp_send_request();
SNTP_CLIENT_STATIC int32_t  sntp_receive_response(void);
SNTP_CLIENT_STATIC void     calcurate_timeout(void);
SNTP_CLIENT_STATIC sntp_err_t  sntp_timer(void);
SNTP_CLIENT_STATIC sntp_err_t  sntp_server_select(void);
void   callback_from_dns(int32_t ercd, name_table_t *name_table);
ER     callback_from_udp(ID cepid, FN fncd , VP p_parblk);
static uint32_t convert_array_to_long(uint8_t *ipaddr);
SNTP_CLIENT_STATIC uint32_t htons32(uint32_t data);
SNTP_CLIENT_STATIC uint32_t convert_ntptime_to_unixtime(uint32_t ntp_time);
SNTP_CLIENT_STATIC sntp_err_t sntp_client_process(void);




#endif /* R_SNTP_CLIENT_H_ */
