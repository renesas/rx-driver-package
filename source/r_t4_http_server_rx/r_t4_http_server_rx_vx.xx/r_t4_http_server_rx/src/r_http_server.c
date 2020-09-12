/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011-2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_http_server.c
* Version      : 1.06
* Device(s)    : Renesas MCUs
* Tool-Chain   : CC-RX v.2.05.00
* OS           : none
* H/W Platform : -
* Description  : HTTP server code.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.04.2011 1.00    First Release
*         : 09.05.2014 1.03    Corresponded to FIT Modules
*         : 09.05.2014 1.04    Clean up source code
*         : 15.08.2016 1.05    Added POST method for file upload.
*         : 30.11.2016 1.06    Included r_sys_time_rx_if.h.
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "r_t4_itcpip.h"
#include "r_t4_file_driver_rx_if.h"
#include "r_http_server_config.h"
#include "r_t4_http_server_rx_if.h"
#include "r_sys_time_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define CONST const

#define RESTYPE_NON     0x00
#define RESTYPE_CGI     0x01
#define RESTYPE_FILE    0x02
#define RESTYPE_INDEXES 0x03
#define HTTP_SRV

#define HTTPD_CLOSED                    0
#define HTTPD_ACCEPT                    1
#define HTTPD_WAIT_REQUEST              2
#define HTTPD_RECEIVE_POST_PARAMETER    3
#define HTTPD_RECEIVE_POST_BODY         4
#define HTTPD_CGI_PENDING               5
#define HTTPD_CGI_GEN_RESPONSE          6
#define HTTPD_SEND_RESPONSE_HEADER      7
#define HTTPD_SEND_RESPONSE_BODY        8
#define HTTPD_CLOSING                   9
#define HTTPD_CANCELING_BY_API          10

#define HTTPD_COMPLETED                     0
#define HTTPD_ERROR                         -1
#define HTTPD_PEND_SENDING                  -2
#define HTTPD_CONTINUE_RECEIVING_FOR_POST   -3
#define HTTPD_NO_POST_PHASE                 -4

#define METHOD_GET     1
#define METHOD_HEAD    2
#define METHOD_POST    3
#define METHOD_ERR     -1

#define HTTP_ERROR_CODE_404 404
#define HTTP_ERROR_CODE_500 500
#define HTTP_ERROR_CODE_501 501

#define SNAME_LEN 12
#define SNAME_LIMIT_LEN 10

/******************************************************************************
Typedef definitions
******************************************************************************/

typedef struct http_post_
{
    int32_t current_part_data_size;
    int32_t buffered_data_size;
    int32_t content_length;
    int32_t uploaded_file_size;
    int32_t initialize_flag;
    uint8_t file_name[FILE_MAX_NAME_SIZE];
    uint8_t mime_boundary[256];
} HTTP_POST;

typedef struct http_cep_
{
    uint8_t status;
    uint8_t current_cgi_func_index;
    uint8_t hdr_buff[HDR_BUF_SIZE];
    uint8_t body_buff[BODY_BUF_SIZE];
    uint8_t rcv_buff[RCV_BUF_SIZE];
    int32_t remain_data;
    int32_t now_data;
    int32_t total_rcv_len;
    int32_t fileid;
    int32_t no_communication_timeout_timer;
    T_IPV4EP dstaddr;
    HTTPD_RESOURCE_INFO res_info;
    FILE_LIST file_list[HTTP_MAX_FILE_LIST];
    DATE_INFO current_file_date_info;
    HTTP_POST http_post;
} HTTP_CEP;

typedef void (*HTTP_POST_CB_FUNC_PTR)(ID id, uint8_t event, uint8_t *file_name,
                                      uint8_t *revceived_data, uint32_t received_data_size, uint32_t uploaded_file_size);


/******************************************************************************
Private global variables and functions
******************************************************************************/
static ER httpd_rcv_request(ID cepid, uint8_t *buf, int16_t buf_len, HTTPD_RESOURCE_INFO *res_info, ER rcv_dat_size);
static ER httpd_parse_request(ID cepid, uint8_t *buf, HTTPD_RESOURCE_INFO *res_info);
static ER httpd_parse_request_not_found(ID cepid);
static ER httpd_parse_request_internal_server_error(ID cepid);
static ER httpd_parse_request_not_implemented(ID cepid);
static ER httpd_parse_post_header(ID cepid, uint8_t *buf, ER rcv_dat_size);
static ER httpd_gen_response_cgi(ID cepid, ER ercd);

static ER gen_response_header(ID cepid, uint8_t *uri, uint8_t *buf, uint32_t content_length);
static ER gen_response_error_header(uint8_t *buf, int32_t error_code);
static ER gen_response_indexes(ID cepid, uint8_t *dir_name);
static ER gen_response_cgi(ID cepid, uint8_t *buf);

static ER set_http_resource(ID cepid, uint32_t hdr_size, uint8_t *hdr_buff, uint32_t body_size, uint8_t *body_buff);
static ER set_lastmodified_date(ID cepid, uint8_t *buf, DATE_INFO *date_info);
static ER set_content_type(int16_t extension_no, uint8_t *buf);
static ER set_content_length(uint32_t content_length, uint8_t *buf);
static ER set_allow(uint8_t *buf);

static ER get_extension_no(uint8_t *buf);
static ER align_file_name_space(const uint8_t *src, uint8_t *dst, uint8_t **space);
static ER extension_check(uint8_t *buf);
static ER method_check(uint8_t *buf);
static ER strcat_with_check_len(uint8_t *src, uint8_t *concat, int32_t size);
static ER is_request_cgi(uint8_t *buf);

static uint8_t *get_time(void);
static uint8_t *conv_month_for_systime(int32_t file_month);
static uint8_t *conv_day_for_systime(int32_t tmp_year, int32_t tmp_month, int32_t tmp_day);

static const uint8_t day_data[7][4] = {{"Sun"}, {"Mon"}, {"Tue"}, {"Wed"}, {"Thu"}, {"Fri"}, {"Sat"}};
static const uint8_t month_data[12][4] = {{"Jan"}, {"Feb"}, {"Mar"}, {"Apr"}, {"May"}, {"Jun"}, {"Jul"}, {"Aug"}, {"Sep"}, {"Oct"}, {"Nov"}, {"Dec"}};

static const uint8_t get_t4_http_server_version_string[256] = {HTTPD_VERSION_CODE};

const HTTP_POST_CB_FUNC_PTR p_http_post_callback = HTTP_POST_CALLBACK_FUNCTION;

//static const void *p_http_post_callback(ID id, uint8_t event, uint8_t *file_name, uint8_t *revceived_data, uint32_t received_data_size, uint32_t uploaded_file_size) = HTTP_POST_CALLBACK_FUNCTION;
/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
HTTP_CEP http_cep[HTTP_TCP_CEP_NUM];
uint8_t lf_code[] = LF_CODE;
ER R_TCPIP_HttpServerCallback(ID cepid, FN fncd , VP p_parblk);

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void http_server_post_callback(ID id, uint8_t event, uint8_t *file_name, uint8_t *revceived_data, uint32_t received_data_size, uint32_t uploaded_file_size);
extern T_TCP_CCEP tcp_ccep[];
extern void timer_interrupt(void);
/*------------ PROCESS -----------*/

/******************************************************************************
* Function Name : R_TCPIP_HttpServerOpen
* Description   : Open the HTTP server.
* Argument      : none
* Return Value  : none
******************************************************************************/
int32_t R_TCPIP_HttpServerOpen(void)
{
    int i;
    if(true != R_SYS_TIME_IsPeriodicCallbackRegistered(timer_interrupt))
    {
        return -1;
    }
    if(SYS_TIME_SUCCESS != R_SYS_TIME_RegisterPeriodicCallback(R_TCPIP_HttpServerProcess, 1))
    {
        return -1;
    }
    /* Initialize process */
    memset(http_cep, 0, sizeof(http_cep));
    for (i = 0; i < HTTP_TCP_CEP_NUM; i++)
    {
        http_cep[i].fileid = -1;
    }
    return 0;
}

/******************************************************************************
* Function Name : R_TCPIP_HttpServerClose
* Description   : Open the HTTP server.
* Argument      : none
* Return Value  : none
******************************************************************************/
int32_t R_TCPIP_HttpServerClose(void)
{
    int i;
    int32_t close_count;

    R_SYS_TIME_UnregisterPeriodicCallback(R_TCPIP_HttpServerProcess);


    /* Initialize process */
    close_count = 0;

    while(close_count < HTTP_TCP_CEP_NUM)
    {
        for (i = 0, close_count = 0; i < HTTP_TCP_CEP_NUM; i++)
        {
            if(http_cep[i].status == HTTPD_CLOSED)
            {
                close_count ++;
            }
            else
            {
                if(E_OK == tcp_can_cep(i + HTTP_START_TCP_CEP + 1, TFN_TCP_ALL))
                {
                    http_cep[i].status = HTTPD_CANCELING_BY_API;
                }
                else
                {
                    http_cep[i].status = HTTPD_CLOSING;
                    tcp_cls_cep(i + HTTP_START_TCP_CEP + 1, TMO_NBLK);
                }
            }
        }
    }
    memset(http_cep, 0, sizeof(http_cep));

    return 0;
}


/******************************************************************************
* Function Name : R_TCPIP_HttpServerProcess
* Description   : Watch the HTTP server status.
* Argument      : none
* Return Value  : none
******************************************************************************/
void R_TCPIP_HttpServerProcess(void)
{
    ER   ercd;
    volatile uint32_t  i;
    static int32_t init_flag = 0;
    static SYS_TIME previous_sys_time;
    SYS_TIME sys_time;
    uint32_t dummy_i;
    uint8_t *dummy_p;


    /* This function checks all communication endpoint */
    for (i = 0; i < HTTP_TCP_CEP_NUM; i++)
    {
        switch (http_cep[i].status)
        {
            case HTTPD_CLOSED:
                /* accept */
                ercd = tcp_acp_cep(i + HTTP_START_TCP_CEP + 1, i + HTTP_START_TCP_CEP + 1, &http_cep[i].dstaddr, TMO_NBLK);
                if (ercd != E_WBLK)
                {
                    while (1);
                }
                http_cep[i].status = HTTPD_ACCEPT;
                break;
            case HTTPD_CGI_GEN_RESPONSE:
                if (cgi_file_name_table[http_cep[i].current_cgi_func_index].cgi_cb_func != NULL)
                {
                    ercd = cgi_file_name_table[http_cep[i].current_cgi_func_index].cgi_cb_func(i + 1, &http_cep[i].res_info.res);

                    ercd = httpd_gen_response_cgi(i + 1, ercd);
                    if (ercd == 0)
                    {
                        /* start transmitting response header */
                        tcp_snd_dat(i + HTTP_START_TCP_CEP + 1, (VP)http_cep[i].res_info.res.hdr, http_cep[i].res_info.res.hdr_size, TMO_NBLK);
                        http_cep[i].status = HTTPD_SEND_RESPONSE_HEADER;
                        http_cep[i].now_data = 0;
                        http_cep[i].remain_data = http_cep[i].res_info.res.hdr_size;
                    }
                    else
                    {
                        /* no transmit when there is no body data */
                        http_cep[i].now_data = 0;
                        http_cep[i].remain_data = 0;
                        http_cep[i].status = HTTPD_CLOSING;
                        tcp_cls_cep(i + HTTP_START_TCP_CEP + 1, TMO_NBLK);
                    }
                }
                else
                {
                    while (1);
                }
                break;
            default:
                break;
        }
    }

    /* detect timeout */
    R_SYS_TIME_GetCurrentTime(&sys_time);
    if ( sys_time.unix_time != previous_sys_time.unix_time )
    {
        for (i = 0; i < HTTP_TCP_CEP_NUM; i++)
        {
            if(http_cep[i].status > HTTPD_ACCEPT)
            {
                http_cep[i].no_communication_timeout_timer -= (sys_time.unix_time - previous_sys_time.unix_time);
                if(http_cep[i].no_communication_timeout_timer <= 0)
                {
                    if(E_OK != tcp_can_cep(i + HTTP_START_TCP_CEP + 1, TFN_TCP_ALL))
                    {
                        tcp_cls_cep(i + HTTP_START_TCP_CEP + 1, TMO_NBLK);
                        http_cep[i].status = HTTPD_CLOSING;
                        p_http_post_callback(i + HTTP_START_TCP_CEP + 1, HTTP_POST_CALLBACK_EVENT_ABORT, dummy_p, dummy_p, dummy_i, dummy_i);
                    }
                }
            }
        }
        memcpy(&previous_sys_time, &sys_time, sizeof(SYS_TIME));
    }
}

void R_TCPIP_HttpServerPendingReleaseRequest(ID cepid)
{
    if (cepid <= 0)
    {
        while (1);  /* System error !! */
    }

    http_cep[cepid - 1].status = HTTPD_CGI_GEN_RESPONSE;
}

static ER httpd_gen_response_cgi(ID cepid, ER ercd)
{
    if (ercd < 0 || http_cep[cepid - 1].res_info.res.body == 0 || http_cep[cepid - 1].res_info.res.body_size == 0)
    {
        return httpd_parse_request_internal_server_error(cepid);
    }

    /* generate response header */
    if (gen_response_cgi(cepid, http_cep[cepid - 1].hdr_buff) < 0)
    {
        return httpd_parse_request_internal_server_error(cepid);
    }
    /* set response information */
    set_http_resource(cepid, strlen((char*)http_cep[cepid - 1].hdr_buff), http_cep[cepid - 1].hdr_buff, strlen((char*)http_cep[cepid - 1].body_buff), http_cep[cepid - 1].body_buff);

    return 0;
}

static ER httpd_parse_post_header(ID cepid, uint8_t *buf, ER rcv_dat_size)
{
    char *str_p, *last_str_p;

    if (http_cep[cepid - 1].status == HTTPD_RECEIVE_POST_PARAMETER)
    {
        str_p = strstr((char *)buf, "Content-Disposition: ");
        if(str_p == 0)
        {
            return httpd_parse_request_internal_server_error(cepid);
        }
        str_p = strstr((char *)buf, "filename=");
        if(str_p == 0)
        {
            return httpd_parse_request_internal_server_error(cepid);
        }
        strncpy((char *)http_cep[cepid - 1].http_post.file_name, str_p + strlen("filename=\""), sizeof(http_cep[cepid - 1].http_post.file_name) - 1);

        if (http_cep[cepid - 1].http_post.file_name[0] == 0)
        {
            return httpd_parse_request_internal_server_error(cepid);;
        }

        str_p = strtok((char *)http_cep[cepid - 1].http_post.file_name, "\\");
        while(str_p != NULL)
        {
            last_str_p = str_p;
            str_p = strtok(NULL, "\\");
        }
        strcpy((char *)http_cep[cepid - 1].http_post.file_name, last_str_p);

        /* strtok() remove '"' character from filename. */
        strtok((char *)http_cep[cepid - 1].http_post.file_name, "\"");

        /* extract file pointer and size (removing MIME Multipart Media Encapsulation header length from POST content-length) */
        str_p = strstr((char *)buf, "\r\n");
        if(str_p == 0)
        {
            return httpd_parse_request_internal_server_error(cepid);;
        }

        str_p = strstr((char *)buf, "\r\n\r\n") + strlen("\r\n\r\n");
        http_cep[cepid - 1].http_post.current_part_data_size = rcv_dat_size - (str_p - (char *)buf);
        http_cep[cepid - 1].http_post.uploaded_file_size = http_cep[cepid - 1].http_post.content_length - (str_p - (char *)buf) - (strlen((char *)http_cep[cepid - 1].http_post.mime_boundary) + 8); /* +8 means crlf (2bytes) and "--" (2bytes) at each side of boundary */
        memcpy(buf, str_p, http_cep[cepid - 1].http_post.current_part_data_size);

        http_cep[cepid - 1].status = HTTPD_RECEIVE_POST_BODY;
        return HTTPD_CONTINUE_RECEIVING_FOR_POST;
    }
    else if (http_cep[cepid - 1].status == HTTPD_RECEIVE_POST_BODY)
    {
        http_cep[cepid - 1].http_post.current_part_data_size = rcv_dat_size;
        return HTTPD_CONTINUE_RECEIVING_FOR_POST;
    }
    return HTTPD_NO_POST_PHASE;
}

static ER httpd_rcv_request(ID cepid, uint8_t *buf, int16_t buf_len, HTTPD_RESOURCE_INFO *res_info, ER rcv_dat_size)
{
    uint8_t *ptr;
    ER ercd;

    http_cep[cepid - 1].total_rcv_len += rcv_dat_size;

    if (rcv_dat_size <= 0)
    {
        http_cep[cepid - 1].total_rcv_len = 0;
        return httpd_parse_request_internal_server_error(cepid);
    }

    ercd = httpd_parse_post_header(cepid, buf, rcv_dat_size);
    if( ercd != HTTPD_NO_POST_PHASE )
    {
        return ercd;
    }
    if (http_cep[cepid - 1].total_rcv_len > (buf_len - 1))
    {
        /* stop receiving when buffer is full */
        http_cep[cepid - 1].total_rcv_len = 0;
        return httpd_parse_request_internal_server_error(cepid);
    }
    buf[http_cep[cepid - 1].total_rcv_len] = '\0';

    ptr = (uint8_t *)strstr((char*)buf, "\r\n\r\n");
    if (ptr == 0)
    {
        /* buffer overflow -> error */
        return httpd_parse_request_internal_server_error(cepid);
    }

    ercd = httpd_parse_request(cepid, buf, res_info);
    http_cep[cepid - 1].total_rcv_len = 0;
    return ercd;
}


static ER httpd_parse_request(ID cepid, uint8_t *buf, HTTPD_RESOURCE_INFO *res_info)
{
    uint8_t  *p, *q;
    uint8_t  ch = 0;

    int8_t method_no = 0;
    uint8_t tmp_path[256];

    uint32_t i;
    ER ercd;

    char *str_p;

    memset(&res_info->res, 0, sizeof(HTTPD_RESOURCE));

    /* check method */
    method_no = method_check(buf);
    if (method_no == METHOD_ERR)
    {
        return httpd_parse_request_not_implemented(cepid);
    }
    if (method_no == METHOD_GET)
    {
        buf += (sizeof("GET ") - 1);
    }
    else if (method_no == METHOD_HEAD)
    {
        buf += (sizeof("HEAD ") - 1);
    }
    else if (method_no == METHOD_POST)
    {
        if(p_http_post_callback == NULL)
        {
            return httpd_parse_request_not_implemented(cepid);
        }
        buf += (sizeof("POST ") - 1);
        /* get content length when method is POST */
        if (method_no == METHOD_POST)
        {
            str_p = strstr((char *)buf, "Content-Length: ");
            if(str_p == 0)
            {
                return httpd_parse_request_internal_server_error(cepid);
            }
            sscanf(str_p, "Content-Length: %d", &http_cep[cepid - 1].http_post.content_length);

            str_p = strstr((char *)buf, "Content-Type: multipart/form-data; boundary=");
            if(str_p == 0)
            {
                return httpd_parse_request_internal_server_error(cepid);
            }
            sscanf(str_p, "Content-Type: multipart/form-data; boundary=%256s", http_cep[cepid - 1].http_post.mime_boundary);

            str_p = strstr((char *)buf, "\r\n\r\n");
            if(str_p == 0)
            {
                return httpd_parse_request_internal_server_error(cepid);
            }
            str_p += strlen("\r\n\r\n");
            if(*str_p != 0)
            {
                ercd = httpd_parse_post_header(cepid, (uint8_t *)str_p, http_cep[cepid - 1].total_rcv_len - ((uint8_t *)str_p - buf));
                if(ercd != HTTPD_CONTINUE_RECEIVING_FOR_POST)
                {
                    return httpd_parse_request_internal_server_error(cepid);
                }
            }
            http_cep[cepid - 1].status = HTTPD_RECEIVE_POST_PARAMETER;
        }
    }
    else
    {
        return httpd_parse_request_internal_server_error(cepid);
    }

    p = buf;
    q = NULL;
    while (1)
    {
        ch = *p++;
        if (ch == ' ')
        {
            *(p - 1) = '\0';
            break;
        }
        else if (ch == '?')
        {
            q = p;
        }
        else if (ch == '\0')
        {
            return httpd_parse_request_internal_server_error(cepid);
        }
    }
    if (q != NULL)
    {
        strncpy((char*)res_info->param, (const char*)q, QUERY_PARAM_SIZE - 1);
        res_info->param[QUERY_PARAM_SIZE - 1] = '\0';
    }

    /* generate file path */
    tmp_path[0] = 0;
    strcat_with_check_len(tmp_path, (uint8_t*)httpd_root_directry, sizeof(tmp_path));
    strcat_with_check_len(tmp_path, (uint8_t*)buf, sizeof(tmp_path));

    /* case : specified directory */
    if ((uint8_t)(tmp_path[strlen((char*)tmp_path) - 1]) == '/')
    {
        if (indexes == 0)
        {
            strcpy((char*)tmp_path, (const char *)httpd_root_directry);
            strcat((char*)tmp_path, (const char *)default_file_name);
        }
        else
        {
            /* generate indexes page */
            if (gen_response_indexes(cepid, (uint8_t*)tmp_path) == 0)
            {
                /* set response information */
                set_http_resource(cepid, strlen((char*)http_cep[cepid - 1].hdr_buff), http_cep[cepid - 1].hdr_buff, strlen((char*)http_cep[cepid - 1].body_buff), http_cep[cepid - 1].body_buff);
                http_cep[cepid - 1].res_info.res.type = RESTYPE_INDEXES;
                return HTTPD_COMPLETED;
            }
            else
            {
                return httpd_parse_request_internal_server_error(cepid);
            }
        }
    }
    if (is_request_cgi(tmp_path) != 0)
    {
        /* search resources */
        http_cep[cepid - 1].fileid = file_open((uint8_t *)tmp_path, FILE_READ);
        if (http_cep[cepid - 1].fileid < 0)
        {
            return httpd_parse_request_not_found(cepid);
        }
        get_file_info(http_cep[cepid - 1].fileid, &http_cep[cepid - 1].current_file_date_info);
        http_cep[cepid - 1].res_info.res.type = RESTYPE_FILE;

        /* generate response header */
        if (gen_response_header(cepid, buf, http_cep[cepid - 1].hdr_buff, get_file_size(http_cep[cepid - 1].fileid)) < 0)
        {
            return httpd_parse_request_internal_server_error(cepid);
        }
        /* set response information */
        set_http_resource(cepid, strlen((char*)http_cep[cepid - 1].hdr_buff), http_cep[cepid - 1].hdr_buff, get_file_size(http_cep[cepid - 1].fileid), http_cep[cepid - 1].body_buff);
        return HTTPD_COMPLETED;
    }

    else
    {
        /* search resources for CGI */
        for (i = 0; i < MAX_CGI_FILE; i++)
        {
            if (strstr((const char*)tmp_path, (const char*)cgi_file_name_table[i].file_name))
            {
                break; // exist cgi file
            }
        }
        if (i == MAX_CGI_FILE)
        {
            return httpd_parse_request_not_found(cepid);
        }

        /* CGI function pointer null check */
        if (NULL == cgi_file_name_table[i].cgi_func)
        {
            return httpd_parse_request_not_found(cepid);
        }

        /* Register CGI function */
        http_cep[cepid - 1].res_info.res.type = RESTYPE_CGI;
        http_cep[cepid - 1].res_info.res.cgi_func = cgi_file_name_table[i].cgi_func;
        http_cep[cepid - 1].res_info.res.body = http_cep[cepid - 1].body_buff;

        /* execute cgi function */
        ercd = (*http_cep[cepid - 1].res_info.res.cgi_func)(cepid, &http_cep[cepid - 1].res_info.res);

        /* HTTPD_PEND_SENDING: CGI pending */
        if (ercd == HTTPD_PEND_SENDING)
        {
            http_cep[cepid - 1].current_cgi_func_index = i;
            return HTTPD_PEND_SENDING;
        }
        /* 0: Normal termination, -1: Internal error */
        else
        {
            ercd = httpd_gen_response_cgi(cepid, ercd);
        }

        return ercd;
    }

    /* error path */
    return httpd_parse_request_internal_server_error(cepid);
}

static ER httpd_parse_request_not_found(ID cepid)
{
    /* generate 404 Not Found response header */
    if (gen_response_error_header(http_cep[cepid - 1].hdr_buff, HTTP_ERROR_CODE_404) < 0)
    {
        return httpd_parse_request_internal_server_error(cepid);
    }

    /* set response information */
    set_http_resource(cepid, strlen((char*)http_cep[cepid - 1].hdr_buff), http_cep[cepid - 1].hdr_buff, 0, http_cep[cepid - 1].body_buff);

    return HTTPD_COMPLETED;
}

/* default error */
static ER httpd_parse_request_internal_server_error(ID cepid)
{

    /* generate 500 Internal Server Error response header */
    if (gen_response_error_header(http_cep[cepid - 1].hdr_buff, HTTP_ERROR_CODE_500) < 0)
    {
        return HTTPD_ERROR;
    }

    /* set response information */
    set_http_resource(cepid, strlen((char*)http_cep[cepid - 1].hdr_buff), http_cep[cepid - 1].hdr_buff, 0, http_cep[cepid - 1].body_buff);

    return HTTPD_COMPLETED;
}

static ER httpd_parse_request_not_implemented(ID cepid)
{
    /* generate 501 Not Implemented response header */
    if (gen_response_error_header(http_cep[cepid - 1].hdr_buff, HTTP_ERROR_CODE_501) < 0)
    {
        return httpd_parse_request_internal_server_error(cepid);
    }

    /* set response information */
    set_http_resource(cepid, strlen((char*)http_cep[cepid - 1].hdr_buff), http_cep[cepid - 1].hdr_buff, 0, http_cep[cepid - 1].body_buff);

    return HTTPD_COMPLETED;

}

static ER method_check(uint8_t *buf)
{

    int8_t ret = METHOD_ERR;

    if (strncmp((char*)buf, "GET ", 4) == 0)
    {
        ret = METHOD_GET;
    }
    if (strncmp((char*)buf, "HEAD ", 5) == 0)
    {
        ret = METHOD_HEAD;
    }
    if (strncmp((char*)buf, "POST ", 5) == 0)
    {
        ret = METHOD_POST;
    }
    return ret;
}


static ER extension_check(uint8_t *buf)
{

    int16_t loop;
    int32_t ret;
    uint8_t tmp_extension[4];

    tmp_extension[0] = (uint8_t)tolower((int32_t) * buf);
    tmp_extension[1] = (uint8_t)tolower((int32_t) * (buf + 1));
    tmp_extension[2] = (uint8_t)tolower((int32_t) * (buf + 2));
    tmp_extension[3] = 0;

    for (loop = 0; loop < MAX_EXTENSION; loop++)
    {
        if (strcmp((const char*)tmp_extension, (const char *)extension_data[loop].extension_type) == 0)
        {
            ret = loop;
            break;
        }
    }
    if (loop == MAX_EXTENSION)
    {
        ret = 0;
    }
    return ret;
}


static ER set_allow(uint8_t *buf)
{
    sprintf((char *)buf, "Allow: GET, HEAD\r\n");
    return(12);
}

static ER set_content_length(uint32_t content_length, uint8_t *buf)
{
    sprintf((char *)buf, "Content-Length: %d\r\n", content_length);

    return strlen((const char *)buf);

}

static ER set_content_type(int16_t extension_no, uint8_t *buf)
{
    if (extension_no < 0)
    {
        return -1;
    }

    sprintf((char *)buf, "Content-Type: %s\r\n", &extension_data[extension_no].content_type_buf[0]);

    return strlen((const char *)buf);

}

static ER set_lastmodified_date(ID cepid, uint8_t *buf, DATE_INFO *date_info)
{
    sprintf((char *)buf, "Last-Modified: %s, %d %s %02d %02d:%02d:%02d GMT\r\n",
            date_info->day_of_the_week,
            date_info->day,
            date_info->month,
            date_info->year,
            date_info->hour,
            date_info->min,
            date_info->sec
           );
    return 0;
}

static ER set_http_resource(ID cepid, uint32_t hdr_size, uint8_t *hdr_buff, uint32_t body_size, uint8_t *body_buff)
{
    http_cep[cepid - 1].res_info.res.hdr_size = hdr_size;
    http_cep[cepid - 1].res_info.res.hdr = hdr_buff;
    http_cep[cepid - 1].res_info.res.body_size = body_size;
    http_cep[cepid - 1].res_info.res.body = body_buff;
    return 0;
}

static ER get_extension_no(uint8_t *buf)
{
    ER extension_no;
    uint8_t *ptr;

    ptr = (uint8_t *)strchr((char*)buf, '.');
    extension_no = extension_check(ptr + 1);
    return extension_no;

}

static ER gen_response_header(ID cepid, uint8_t *uri, uint8_t *buf, uint32_t content_length)
{
    uint8_t tmp_buf[128];
    uint8_t crlf[] = "\r\n";
    ER ercd = 0;

    buf[0] = 0;

    sprintf((char *)tmp_buf, "HTTP/1.0 200 OK\r\n");
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    sprintf((char *)tmp_buf, "Date: %s\r\n", get_time());
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Server:
    sprintf((char *)tmp_buf, "Server: %s\r\n", get_t4_http_server_version_string);
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Allow:
    set_allow(tmp_buf);
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Last-Modified:
    set_lastmodified_date(cepid, tmp_buf, &http_cep[cepid - 1].current_file_date_info);
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Content-Type:
    set_content_type(get_extension_no((uint8_t *)uri), tmp_buf);
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Content-Length:
    set_content_length(content_length, tmp_buf);
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Connection:
    sprintf((char *)tmp_buf, "Connection: Close\r\n");
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // End of header
    ercd += strcat_with_check_len((uint8_t*)buf, crlf, HDR_BUF_SIZE);

    return 0;
}

static ER gen_response_error_header(uint8_t *buf, int32_t error_code)
{
    uint8_t tmp_buf[128];
    uint8_t crlf[] = "\r\n";
    ER ercd = 0;

    buf[0] = 0;

    switch (error_code)
    {
        case HTTP_ERROR_CODE_404:
            sprintf((char *)tmp_buf, "HTTP/1.0 404 Not Found\r\n");
            ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);
            break;

        case HTTP_ERROR_CODE_500:
            sprintf((char *)tmp_buf, "HTTP/1.0 500 Internal Server Error\r\n");
            ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);
            break;

        case HTTP_ERROR_CODE_501:
            sprintf((char *)tmp_buf, "HTTP/1.0 501 Not Implemented \r\n");
            ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);
            break;
    }

    sprintf((char *)tmp_buf, "Date: %s\r\n", get_time());
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Server:
    sprintf((char *)tmp_buf, "Server: %s\r\n", get_t4_http_server_version_string);
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // Connection:
    sprintf((char *)tmp_buf, "Connection: Close\r\n");
    ercd += strcat_with_check_len((uint8_t*)buf, tmp_buf, HDR_BUF_SIZE);

    // End of header
    ercd += strcat_with_check_len((uint8_t*)buf, crlf, HDR_BUF_SIZE);

    return ercd;
}


static ER align_file_name_space(const uint8_t *src, uint8_t *dst, uint8_t **space)
{
    static const uint8_t c_space[SNAME_LEN + 1] = "            ";
    int32_t src_len;

    src_len = strlen((const char *)src);
    if (src_len > SNAME_LEN)
    {
        strncpy((char *)dst, (const char *)src, SNAME_LIMIT_LEN);
        dst[SNAME_LIMIT_LEN + 1] = '~';
        dst[SNAME_LIMIT_LEN + 1] = src[src_len - 1];
        *space = (uint8_t *) & c_space[SNAME_LEN - 1];
    }
    else
    {
        strcpy((char *)dst, (const char *)src);
        *space = (uint8_t *) & c_space[src_len];
    }

    return 0;
}

static ER gen_response_indexes(ID cepid, uint8_t *dir_name)
{
    uint8_t tmp_buf[512];
    uint8_t parent_dir_name[256];
    uint8_t crlf[] = "\r\n";
    int32_t i = 0;
    uint8_t shortname[13];
    uint8_t *p_space;
    ER ercd = 0;
    uint8_t *user_path;
    int32_t filenum;

    http_cep[cepid - 1].hdr_buff[0] = 0;
    http_cep[cepid - 1].body_buff[0] = 0;

    // generate body
    filenum = get_file_list_info((uint8_t *)dir_name, http_cep[cepid - 1].file_list, HTTP_MAX_FILE_LIST, 0);
    if (filenum < 0)
    {
        return -1;
    }

    sprintf((char *)tmp_buf, "<html>%s<body>%s<pre>%s", lf_code, lf_code, lf_code);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    user_path = (uint8_t *)strstr((const char *)dir_name, (const char *)httpd_root_directry);
    user_path += strlen((const char *)httpd_root_directry);
    sprintf((char *)tmp_buf, "Index of %s %s%s", user_path, lf_code, lf_code, lf_code);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    sprintf((char *)tmp_buf, "        Name            Last modified       Size%s", lf_code);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    sprintf((char *)tmp_buf, "<hr>%s", lf_code);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    /* display parent directory */
    if (strlen((const char *)user_path) != 1)
    {
        strcpy((char *)parent_dir_name, (const char *)user_path);
        parent_dir_name[strlen((const char *)parent_dir_name) - 1] = 0;
        *(strrchr((const char *)parent_dir_name, '/')) = 0;
        sprintf((char *)tmp_buf, "<a href=%s/>Parent Directory</a> ", parent_dir_name);
        ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));
        /* line feed code */
        sprintf((char *)tmp_buf, "%s", lf_code);
        ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));
    }

    while (i < HTTP_MAX_FILE_LIST)
    {
        if (http_cep[cepid - 1].file_list[i].file_name[0] == 0)
        {
            break;
        }
        else
        {
            /* directory name */
            if ((http_cep[cepid - 1].file_list[i].file_attr & FILE_ATTR_DIR) == FILE_ATTR_DIR)
            {
                align_file_name_space((const uint8_t *)http_cep[cepid - 1].file_list[i].file_name, shortname, &p_space);
                sprintf((char *)tmp_buf, "<a href=%s%s/>%s</a>%s     ", user_path, http_cep[cepid - 1].file_list[i].file_name, shortname, p_space);
            }
            /* file name */
            else
            {
                align_file_name_space((const uint8_t *)http_cep[cepid - 1].file_list[i].file_name, shortname, &p_space);
                sprintf((char *)tmp_buf, "<a href=%s%s>%s</a>%s     ", user_path, http_cep[cepid - 1].file_list[i].file_name, shortname, p_space);

            }
            ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

            /* Last modified */
            sprintf((char *)tmp_buf, "%2d-%s-%02d %02d:%02d:%02d ",
                    http_cep[cepid - 1].file_list[i].date_info.day,
                    http_cep[cepid - 1].file_list[i].date_info.month,
                    http_cep[cepid - 1].file_list[i].date_info.year,
                    http_cep[cepid - 1].file_list[i].date_info.hour,
                    http_cep[cepid - 1].file_list[i].date_info.min,
                    http_cep[cepid - 1].file_list[i].date_info.sec
                   );
            ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

            /* directory disp */
            if ((http_cep[cepid - 1].file_list[i].file_attr & FILE_ATTR_DIR) == FILE_ATTR_DIR)
            {
                sprintf((char *)tmp_buf, "     (dir)");
            }
            /* file size disp*/
            else
            {
                sprintf((char *)tmp_buf, "%10ld", http_cep[cepid - 1].file_list[i].file_size);
            }
            ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

            /* line feed code */
            sprintf((char *)tmp_buf, "%s", lf_code);
            ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));
        }
        i++;
    }

    sprintf((char *)tmp_buf, "<hr>%s", lf_code);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    sprintf((char *)tmp_buf, "%s", get_t4_http_server_version_string);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    sprintf((char *)tmp_buf, "</pre>%s</body>%s</html>", lf_code, lf_code, lf_code);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].body_buff, tmp_buf, sizeof(http_cep[cepid - 1].body_buff));

    // generate header
    sprintf((char *)tmp_buf, "HTTP/1.0 200 OK\r\n");
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    sprintf((char *)tmp_buf, "Date: %s\r\n", get_time());
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // Server:
    sprintf((char *)tmp_buf, "Server: %s\r\n", get_t4_http_server_version_string);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // Content-Length:
    set_content_length(strlen((char*)http_cep[cepid - 1].body_buff), tmp_buf);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // Connection:
    sprintf((char *)tmp_buf, "Connection: Close\r\n");
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // End of header
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, crlf, sizeof(http_cep[cepid - 1].hdr_buff));

    return ercd;
}

static ER gen_response_cgi(ID cepid, uint8_t *buf)
{
    ER ercd = 0;
    uint8_t tmp_buf[256];
    uint8_t crlf[] = "\r\n";

    buf[0] = 0;

    // generate header
    sprintf((char *)tmp_buf, "HTTP/1.0 200 OK\r\n");
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    sprintf((char *)tmp_buf, "Date: %s\r\n", get_time());
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // Server:
    sprintf((char *)tmp_buf, "Server: %s\r\n", get_t4_http_server_version_string);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // Content-Length:
    set_content_length(strlen((char*)http_cep[cepid - 1].body_buff), tmp_buf);
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // Connection:
    sprintf((char *)tmp_buf, "Connection: Close\r\n");
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, tmp_buf, sizeof(http_cep[cepid - 1].hdr_buff));

    // End of header
    ercd += strcat_with_check_len((uint8_t*)http_cep[cepid - 1].hdr_buff, crlf, sizeof(http_cep[cepid - 1].hdr_buff));

    return ercd;
}

static ER strcat_with_check_len(uint8_t *src, uint8_t *concat, int32_t size)
{
    if (strlen((const char *)src) + strlen((const char *)concat) >= size)
    {
        return -1;
    }
    strcat((char *)src, (const char *)concat);
    return 0;
}

static ER is_request_cgi(uint8_t *buf)
{
    char *strstr_return;
    ER return_value;

    strstr_return = strstr((const char*)buf, (const char *)".cgi");

    if (strstr_return == 0)
    {
        return_value = -1;
    }
    else
    {
        return_value = 0;
    }
    return return_value;
}

static uint8_t *get_time(void)
{
    static uint8_t date[] = {"Mon, 07 Feb 2011 12:00:00 GMT"};
    SYS_TIME ltime;
    SYS_TIME *ptime;

    ptime = &ltime;
    R_SYS_TIME_GetCurrentTime(ptime);
    if (NULL != ptime)
    {
        sprintf((char *)date, "%s, %02d %s %4d %02d:%02d:%02d GMT",
                conv_day_for_systime(ptime->year, ptime->month, ptime->day),
                ptime->day,
                conv_month_for_systime(ptime->month),
                ptime->year,
                ptime->hour,
                ptime->min,
                ptime->sec
               );
    }

    return date;
}

static uint8_t *conv_month_for_systime(int32_t file_month)
{
    return (uint8_t*)month_data[file_month - 1];
}


static uint8_t *conv_day_for_systime(int32_t tmp_year, int32_t tmp_month, int32_t tmp_day)
{
    if ((tmp_month == 1) || (tmp_month == 2))
    {
        tmp_year -= 1;
        tmp_month += 12;
    }
    return (uint8_t*)day_data[((tmp_year + (tmp_year / 4) - (tmp_year / 100) + (tmp_year / 400) + ((13 * tmp_month + 8) / 5) + tmp_day) % 7)];
}

ER R_TCPIP_HttpServerCallback(ID cepid, FN fncd , VP p_parblk)
{

    ER   parblk = *(ER *)p_parblk;
    ER   ercd;
    ID   cepid_oft;
    uint8_t *dummy_p;
    uint32_t dummy_i;

    if (cepid > HTTP_START_TCP_CEP)
    {
        cepid_oft = cepid - HTTP_START_TCP_CEP;
    }
    else /* if (cepid > HTTP_START_TCP_CEP) */
    {
        while (1);  /* System error !! */
    }

    switch (fncd)
    {
        case TFN_TCP_ACP_CEP:
            if (parblk < 0)
            {
                http_cep[cepid_oft - 1].status = HTTPD_CLOSED;
                file_close(http_cep[cepid_oft - 1].fileid);
                http_cep[cepid_oft - 1].fileid = -1;
            }
            else
            {
                if(http_cep[cepid_oft - 1].fileid != -1)
                {
                    file_close(http_cep[cepid_oft - 1].fileid);
                    http_cep[cepid_oft - 1].fileid = -1;
                }
                http_cep[cepid_oft - 1].no_communication_timeout_timer = NO_COMMUNICATION_TIMEOUT_TIMER;
                http_cep[cepid_oft - 1].status = HTTPD_WAIT_REQUEST;
                tcp_rcv_dat(cepid, http_cep[cepid_oft - 1].rcv_buff, sizeof(http_cep[cepid_oft - 1].rcv_buff), TMO_NBLK);
            }
            break;
        case TFN_TCP_SND_DAT:
            http_cep[cepid_oft - 1].no_communication_timeout_timer = NO_COMMUNICATION_TIMEOUT_TIMER;
            if (parblk < 0)
            {
                /* In case : FORCE DISCONNECT */
                http_cep[cepid_oft - 1].now_data = 0;
                http_cep[cepid_oft - 1].remain_data = 0;
                tcp_cls_cep(cepid, TMO_NBLK);
                http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
            }
            if (http_cep[cepid_oft - 1].status == HTTPD_SEND_RESPONSE_HEADER)
            {
                http_cep[cepid_oft - 1].now_data += parblk;
                http_cep[cepid_oft - 1].remain_data -= parblk;
                if (http_cep[cepid_oft - 1].remain_data == 0)
                {
                    /* get file data */
                    if (http_cep[cepid_oft - 1].res_info.res.type == RESTYPE_FILE)
                    {
                        if (file_read(http_cep[cepid_oft - 1].fileid, http_cep[cepid_oft - 1].body_buff, BODY_BUF_SIZE) < 0)
                        {
                            /* no more transmit when error has occurred */
                            http_cep[cepid_oft - 1].now_data = 0;
                            http_cep[cepid_oft - 1].remain_data = 0;
                            tcp_cls_cep(cepid, TMO_NBLK);
                            http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
                            break;
                        }
                    }
                    http_cep[cepid_oft - 1].status = HTTPD_SEND_RESPONSE_BODY;
                    http_cep[cepid_oft - 1].now_data = 0;
                    http_cep[cepid_oft - 1].remain_data = http_cep[cepid_oft - 1].res_info.res.body_size;
                    if (http_cep[cepid_oft - 1].remain_data > BODY_BUF_SIZE)
                    {
                        tcp_snd_dat(cepid, (VP)(http_cep[cepid_oft - 1].res_info.res.body), BODY_BUF_SIZE, TMO_NBLK);
                    }
                    else if (http_cep[cepid_oft - 1].remain_data > 0)
                    {
                        tcp_snd_dat(cepid, (VP)(http_cep[cepid_oft - 1].res_info.res.body), http_cep[cepid_oft - 1].res_info.res.body_size, TMO_NBLK);
                    }
                    else
                    {
                        /* no transmit when there is no body data */
                        http_cep[cepid_oft - 1].now_data = 0;
                        http_cep[cepid_oft - 1].remain_data = 0;
                        tcp_cls_cep(cepid, TMO_NBLK);
                        http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
                        break;
                    }
                }
                else
                {
                    if (http_cep[cepid_oft - 1].remain_data > BODY_BUF_SIZE)
                    {
                        tcp_snd_dat(cepid, (VP)(http_cep[cepid_oft - 1].res_info.res.hdr + http_cep[cepid_oft - 1].now_data), BODY_BUF_SIZE, TMO_NBLK);
                    }
                    else
                    {
                        tcp_snd_dat(cepid, (VP)(http_cep[cepid_oft - 1].res_info.res.hdr + http_cep[cepid_oft - 1].now_data), http_cep[cepid_oft - 1].res_info.res.hdr_size, TMO_NBLK);
                    }
                }
            }
            else if (http_cep[cepid_oft - 1].status == HTTPD_SEND_RESPONSE_BODY)
            {
                http_cep[cepid_oft - 1].now_data += parblk;
                http_cep[cepid_oft - 1].remain_data -= parblk;
                if (http_cep[cepid_oft - 1].remain_data == 0)
                {
                    http_cep[cepid_oft - 1].now_data = 0;
                    http_cep[cepid_oft - 1].remain_data = 0;
                    tcp_cls_cep(cepid, TMO_NBLK);
                    http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
                    break;
                }
                else
                {
                    if (file_read(http_cep[cepid_oft - 1].fileid, http_cep[cepid_oft - 1].body_buff, BODY_BUF_SIZE) < 0)
                    {
                        /* no more transmit when error has occurred */
                        http_cep[cepid_oft - 1].now_data = 0;
                        http_cep[cepid_oft - 1].remain_data = 0;
                        tcp_cls_cep(cepid, TMO_NBLK);
                        http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
                        break;
                    }
                    if (http_cep[cepid_oft - 1].remain_data > BODY_BUF_SIZE)
                    {
                        tcp_snd_dat(cepid, (VP)(http_cep[cepid_oft - 1].res_info.res.body), BODY_BUF_SIZE, TMO_NBLK);
                    }
                    else
                    {
                        tcp_snd_dat(cepid, (VP)(http_cep[cepid_oft - 1].res_info.res.body), http_cep[cepid_oft - 1].remain_data, TMO_NBLK);
                    }
                }
            }
            break;
        case TFN_TCP_RCV_DAT:
            http_cep[cepid_oft - 1].no_communication_timeout_timer = NO_COMMUNICATION_TIMEOUT_TIMER;
            if (parblk < 0)
            {
                /* In case : FORCE DISCONNECT */
                http_cep[cepid_oft - 1].now_data = 0;
                http_cep[cepid_oft - 1].remain_data = 0;
                tcp_cls_cep(cepid, TMO_NBLK);
                http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
                break;
            }
            /* receive request */
            ercd = httpd_rcv_request(cepid_oft, http_cep[cepid_oft - 1].rcv_buff, sizeof(http_cep[cepid_oft - 1].rcv_buff), &http_cep[cepid_oft - 1].res_info, *(ER *)p_parblk);

            /* complete */
            if (ercd == HTTPD_COMPLETED)
            {
                /* start transmitting response header */
                tcp_snd_dat(cepid, (VP)http_cep[cepid_oft - 1].res_info.res.hdr, http_cep[cepid_oft - 1].res_info.res.hdr_size, TMO_NBLK);
                http_cep[cepid_oft - 1].status = HTTPD_SEND_RESPONSE_HEADER;
                http_cep[cepid_oft - 1].now_data = 0;
                http_cep[cepid_oft - 1].remain_data = http_cep[cepid_oft - 1].res_info.res.hdr_size;
            }
            /* pend sending */
            else if (ercd == HTTPD_PEND_SENDING)
            {
                if((http_cep[cepid - 1].status == HTTPD_RECEIVE_POST_PARAMETER) || (http_cep[cepid - 1].status == HTTPD_RECEIVE_POST_BODY))
                {
                    tcp_rcv_dat(cepid, http_cep[cepid_oft - 1].rcv_buff, sizeof(http_cep[cepid_oft - 1].rcv_buff), TMO_NBLK);
                }
                else
                {
                    http_cep[cepid_oft - 1].status = HTTPD_CGI_PENDING;
                }
                break;
            }
            /* continue for post parameter */
            else if(ercd == HTTPD_CONTINUE_RECEIVING_FOR_POST)
            {
                if (http_cep[cepid_oft - 1].total_rcv_len == http_cep[cepid_oft - 1].http_post.content_length)
                {
                    /* case: last 1 packet includes all mime_boundary */
                    if (sizeof(http_cep[cepid_oft - 1].rcv_buff) - http_cep[cepid_oft - 1].http_post.current_part_data_size >= strlen((char *)http_cep[cepid_oft - 1].http_post.mime_boundary))
                    {
                        if(http_cep[cepid_oft - 1].http_post.current_part_data_size > (strlen((char *)http_cep[cepid_oft - 1].http_post.mime_boundary) + 8)) /* +8 means crlf (2bytes) and "--" (2bytes) at each side of boundary */
                        {
                            /* for tiny file is uploaded like smaller than 1 receive buffer */
                            if(http_cep[cepid_oft - 1].http_post.initialize_flag == 0)
                            {
                                http_cep[cepid_oft - 1].http_post.initialize_flag = 1;
                                p_http_post_callback(cepid, HTTP_POST_CALLBACK_EVENT_INITIALIZE, http_cep[cepid_oft - 1].http_post.file_name, dummy_p, dummy_i, dummy_i);
                            }
                            http_cep[cepid_oft - 1].http_post.current_part_data_size -= (strlen((char *)http_cep[cepid_oft - 1].http_post.mime_boundary) + 8); /* +8 means crlf (2bytes) and "--" (2bytes) at each side of boundary */
                            p_http_post_callback(cepid, HTTP_POST_CALLBACK_EVENT_PROCEEDING, http_cep[cepid_oft - 1].http_post.file_name, http_cep[cepid_oft - 1].rcv_buff, http_cep[cepid_oft - 1].http_post.current_part_data_size + http_cep[cepid_oft - 1].http_post.buffered_data_size, http_cep[cepid_oft - 1].http_post.uploaded_file_size);
                        }
                    }
                    p_http_post_callback(cepid, HTTP_POST_CALLBACK_EVENT_FINALIZE, dummy_p, dummy_p, dummy_i, dummy_i);
                    /* start transmitting response header */
                    http_cep[cepid_oft - 1].now_data = 0;
                    http_cep[cepid_oft - 1].remain_data = http_cep[cepid_oft - 1].res_info.res.hdr_size;
                    http_cep[cepid_oft - 1].total_rcv_len = 0;
                }
                else
                {
                    /* case: mime_boundary is separated into last 2 packets */
                    if ((http_cep[cepid_oft - 1].http_post.content_length - http_cep[cepid_oft - 1].total_rcv_len) <= strlen((char *)http_cep[cepid_oft - 1].http_post.mime_boundary) + 8) /* +8 means crlf (2bytes) and "--" (2bytes) at each side of boundary */
                    {
                        http_cep[cepid_oft - 1].http_post.current_part_data_size -= ((strlen((char *)http_cep[cepid_oft - 1].http_post.mime_boundary) + 8) - (http_cep[cepid_oft - 1].http_post.content_length - http_cep[cepid_oft - 1].total_rcv_len)); /* +8 means crlf (2bytes) and "--" (2bytes) at each side of boundary */
                        p_http_post_callback(cepid, HTTP_POST_CALLBACK_EVENT_PROCEEDING, http_cep[cepid_oft - 1].http_post.file_name, http_cep[cepid_oft - 1].rcv_buff, http_cep[cepid_oft - 1].http_post.current_part_data_size + http_cep[cepid_oft - 1].http_post.buffered_data_size, http_cep[cepid_oft - 1].http_post.uploaded_file_size);
                    }
                    /* callback and write to file when buffer is full (filesystem writes the data to storage in part of 512 bytes, so buffer size is multiple by 512 bytes has good effect for performance. */
                    if((http_cep[cepid_oft - 1].http_post.buffered_data_size + http_cep[cepid_oft - 1].http_post.current_part_data_size) == sizeof(http_cep[cepid_oft - 1].rcv_buff))
                    {
                        if(http_cep[cepid_oft - 1].http_post.initialize_flag == 0)
                        {
                            http_cep[cepid_oft - 1].http_post.initialize_flag = 1;
                            p_http_post_callback(cepid, HTTP_POST_CALLBACK_EVENT_INITIALIZE, http_cep[cepid_oft - 1].http_post.file_name, dummy_p, dummy_i, dummy_i);
                        }
                        p_http_post_callback(cepid, HTTP_POST_CALLBACK_EVENT_PROCEEDING, http_cep[cepid_oft - 1].http_post.file_name, http_cep[cepid_oft - 1].rcv_buff, sizeof(http_cep[cepid_oft - 1].rcv_buff), http_cep[cepid_oft - 1].http_post.uploaded_file_size);
                        http_cep[cepid_oft - 1].http_post.buffered_data_size = 0;
                    }
                    else
                    {
                        http_cep[cepid_oft - 1].http_post.buffered_data_size += http_cep[cepid_oft - 1].http_post.current_part_data_size;
                    }
                    tcp_rcv_dat(cepid, http_cep[cepid_oft - 1].rcv_buff + http_cep[cepid_oft - 1].http_post.buffered_data_size, sizeof(http_cep[cepid_oft - 1].rcv_buff) - http_cep[cepid_oft - 1].http_post.buffered_data_size, TMO_NBLK);
                }
                break;
            }
            /* error */
            else
            {
                tcp_cls_cep(cepid, TMO_NBLK);
                http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
                break;
            }
            break;
        case TFN_TCP_SHT_CEP:
            break;
        case TFN_TCP_CLS_CEP:
            http_cep[cepid_oft - 1].status = HTTPD_CLOSED;
            file_close(http_cep[cepid_oft - 1].fileid);
            http_cep[cepid_oft - 1].fileid = -1;
            memset(&http_cep[cepid_oft - 1].http_post, 0, sizeof(HTTP_POST));
            break;
        case TFN_TCP_CAN_CEP:
            tcp_cls_cep(cepid, TMO_NBLK);
            http_cep[cepid_oft - 1].status = HTTPD_CLOSING;
            memset(&http_cep[cepid_oft - 1].http_post, 0, sizeof(HTTP_POST));
            break;
        default:
            break;
    }
    return(0);
}

/*****************************************************************************
* Function Name: R_TCPIP_HttpServerGetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline(R_TCPIP_HttpServerGetVersion)
uint32_t  R_TCPIP_HttpServerGetVersion(void)
{
    uint32_t const version = (T4_HTTP_SERVER_VERSION_MAJOR << 16) | T4_HTTP_SERVER_VERSION_MINOR;

    return version;
}

/******************************************************************************
* Function Name : R_TCPIP_HttpServerReset
* Description   : Reset the HTTP server.
* Argument      : uint8_t channel ;    RJ45 interface
* Return Value  : none
******************************************************************************/
void R_TCPIP_HttpServerReset(uint8_t channel)
{
    uint32_t i;
    int32_t fileid;
    for(i = 0; i < HTTP_TCP_CEP_NUM; i++)
    {
        if(channel == tcp_ccep[i + HTTP_START_TCP_CEP].cepatr)
        {
            fileid = http_cep[i].fileid;
            memset(&http_cep[i], 0, sizeof(HTTP_CEP));
            http_cep[i].fileid = fileid;
        }
    }
}

/* Copyright 2005-2016 RENESAS ELECTRONICS CORPORATION */







