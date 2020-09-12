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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_http_server_config.h
* Version      : 1.00
* Description  :  This is HTTP server configuration setting header file.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.04.2011 1.00    First Release
***********************************************************************************************************************/
#ifndef _R_HTTP_SERVER_CONFIG_H_
#define _R_HTTP_SERVER_CONFIG_H_

#include "r_t4_itcpip.h"
#include "r_t4_http_server_rx_config.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
// Extension data table for HTTPd
typedef struct extension_setting
{
    const uint8_t extension_type[4];
    const uint8_t content_type_buf[25];
} EXTENSION_TYPE_TABLE;

// CGI file name table for HTTPd
typedef struct cgi_file_name_setting
{
    const uint8_t file_name[256];
    ER(*cgi_func)(ID cepid, void *res_info);
    ER(*cgi_cb_func)(ID cepid, void *res_info);
} CGI_FILE_NAME_TABLE;

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern const uint8_t httpd_root_directry[];
extern const uint8_t indexes;
extern const uint8_t default_file_name[];
extern const EXTENSION_TYPE_TABLE extension_data[MAX_EXTENSION];
extern const CGI_FILE_NAME_TABLE cgi_file_name_table[MAX_CGI_FILE];

#endif /* _R_HTTP_SERVER_CONFIG_H_ */

/* Copyright 2011 RENESAS ELECTRONICS CORPORATION */
