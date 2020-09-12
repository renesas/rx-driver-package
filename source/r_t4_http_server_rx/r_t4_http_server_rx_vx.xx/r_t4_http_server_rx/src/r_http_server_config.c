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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_http_server_config.c
* Version      : 1.0
* Device(s)    :
* Tool-Chain   :
* OS           : none
* H/W Platform :
* Description  : This is HTTP server configuration file.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.04.2011 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#if defined(__RX)
#include <stdint.h>
#else
typedef signed char    int8_t;
typedef signed short   int16_t;
typedef signed long    int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;
#endif
#include "r_http_server_config.h"

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
const uint8_t httpd_root_directry[] = {ROOT_DIR};
const uint8_t indexes = INDEXES;
const uint8_t default_file_name[] = {DEFAULT_FILE_NAME};
const EXTENSION_TYPE_TABLE extension_data[MAX_EXTENSION] = {EXTENSION_TYPE_TABLE_LIST};
const CGI_FILE_NAME_TABLE cgi_file_name_table[MAX_CGI_FILE] = {CGI_FILE_NAME_TABLE_LIST};

/* Copyright 2011 RENESAS ELECTRONICS CORPORATION */
