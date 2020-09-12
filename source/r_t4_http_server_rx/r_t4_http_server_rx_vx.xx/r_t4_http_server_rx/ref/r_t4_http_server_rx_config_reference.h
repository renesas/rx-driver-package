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
* Copyright (C) 2014-2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_t4_http_server_rx_config.h
* Description  : Configuration options for the r_t4_http_server_rx module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 09.05.2014 1.03    First Release
*         : 01.07.2014 1.04    Clean up source code
*         : 01.04.2016 1.05    XML file update
*         : 30.11.2016 1.06    Included r_sys_time_rx_if.h.
***********************************************************************************************************************/
#ifndef R_T4_HTTP_SERVER_RX_CONFIG_H
#define R_T4_HTTP_SERVER_RX_CONFIG_H

#include "r_t4_itcpip.h"

/******************************************************************************
Macro definitions
******************************************************************************/
// HTTPd version code
#define HTTPD_VERSION_CODE "Renesas Embedded Web Server/1.06"

// HTTPd root directory in memory card. FORMAT : "directory_name1/directory_name2..."

//#define ROOT_DIR "dir1/dir2"
//#define ROOT_DIR "dir1"
#define ROOT_DIR ""

// This definition decides the operation when the directory is specified from a browser.
// If user specifies directory, HTTPd ...
// 1: displays index page, 0: serches default_file_name
#define INDEXES 1

// If indexes == 0, HTTPd serches index.htm in specified directory.
// If index.htm dose not exist, HTTPd responses 404 Not found.
#define DEFAULT_FILE_NAME "index.htm"

#define MAX_EXTENSION   15

#define EXTENSION_TYPE_TABLE_LIST \
    {"def", "application/octet-stream"}, /* default      */\
    {"htm", "text/html"},                /* text         */\
    {"txt", "text/plain"},               /* text         */\
    {"exe", "application/octet-stream"}, /* application  */\
    {"com", "application/octet-stream"}, /* application  */\
    {"jpg", "image/jpeg"},               /* image        */\
    {"gif", "image/gif"},                /* image        */\
    {"png", "image/png"},                /* image        */\
    {"zip", "application/octet-stream"}, /* application  */\
    {"mp3", "audio/mpeg"},               /* audio        */\
    {"wav", "Nvaudio/x-wav"},            /* audio        */\
    {"dat", "application/octet-stream"}, /* application  */\
    {"xls", "application/vnd.ms-excel"}, /* application  */\
    {"doc", "application/msword"},       /* application  */\
    {"cgi", "text/html"},                /* text         */\

// If user uses CGI function, please specify cgi file name, and number of cgi files.
#define MAX_CGI_FILE   1

#ifndef NULL
#define NULL 0
#endif /* NULL */
#define CGI_FILE_NAME_TABLE_LIST \
    {"cgi_smpl.cgi", NULL, NULL},
//extern ER http_server_cgi_callback(ID cepid, void *res_info);
//#define CGI_FILE_NAME_TABLE_LIST \
//    {"cgi_smpl.cgi", http_server_cgi_callback, NULL},

#define HTTP_POST_CALLBACK_FUNCTION NULL
//#define HTTP_POST_CALLBACK_FUNCTION http_server_post_callback
//extern void http_server_post_callback(ID id, uint8_t event, uint8_t *file_name, uint8_t *receive_data, uint32_t receive_data_size, uint32_t uploaded_file_size);

// Line feed code which server uses.
#define LF_CODE   "\r\n"

// set same value number of CEPID in config_tcpudp.c
#define HTTP_TCP_CEP_NUM 4

// starting position of CEPID in config_tcpudp.c
#define HTTP_START_TCP_CEP 0

// max display number of files for indexes page
#define HTTP_MAX_FILE_LIST 16

// receive buffer size to receive : needs RCV_BUF_SIZE * HTTP_TCP_CEP_NUM byte RAM
#define RCV_BUF_SIZE 1024

// header buffer size to transmit : needs HDR_BUF_SIZE * HTTP_TCP_CEP_NUM byte RAM
#define HDR_BUF_SIZE 1460

// body buffer size  to transmit : needs BODY_BUF_SIZE * HTTP_TCP_CEP_NUM byte RAM
#define BODY_BUF_SIZE 2048

// no communication timeout timer initial value (unit = 1s)
#define NO_COMMUNICATION_TIMEOUT_TIMER 10

#endif /* R_T4_HTTP_SERVER_RX_CONFIG_H */


