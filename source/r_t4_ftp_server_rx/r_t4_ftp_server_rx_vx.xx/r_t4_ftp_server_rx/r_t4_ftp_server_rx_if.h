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
* File Name    : r_t4_ftp_server_rx_if.h
* Version      : 1.04
* Description  : This is FTP function header file.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.03.2011 1.00    First Release
*         : 09.05.2014 1.02    Corresponded to FIT Modules
*         : 01.07.2014 1.03    Clean up source code
*         : 01.04.2016 1.04    XML file update
***********************************************************************************************************************/
#ifndef _R_T4_FTP_SERVER_RX_IF_H_
#define _R_T4_FTP_SERVER_RX_IF_H_

/******************************************************************************
Macro definitions
******************************************************************************/
/* Version Number of API. */
#define T4_FTP_SERVER_VERSION_MAJOR       (1)
#define T4_FTP_SERVER_VERSION_MINOR       (04)

/* The response message from the FTP server */
/* Do not make it the message which is longer than CMD_BUF_SIZE! */
#define MSG_OPEN_DATA_CON     "150 Opening %s data connection.\r\n"
#define MSG_TYPE_SET          "200 TYPE set to %c.\r\n"
#define MSG_CMD_ACCEPTED      "200 %s command successful.\r\n"
#define MSG_SERVER_READY      "220 RX62N FTP server ready.\r\n"
#define MSG_GOODBY            "221 Goodbye.\r\n"
#define MSG_TRNS_CMPLT        "226 Transfer complete.\r\n"
#define MSG_TRNS_ABORT        "226 Transfer aborted. \r\n"
#define MSG_PASV_RES          "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\r\n"
#define MSG_AUTHENTICATION_OK "230 User %s logged in.\r\n"
#define MSG_CMD_SUCCESS       "250 %s command successful.\r\n"
#define MSG_PWD_RESPONSE      "257 \"%s\" is current directory\r\n"
#define MSG_MKD_RESPONSE      "257 MKD command successful.\r\n"
#define MSG_PASSWD_REQ        "331 Password required for %s.\r\n"
#define MSG_RNFR_RES          "350 Requested file action pending further information. \r\n"
#define MSG_NOT_AVAILABLE     "421 Service not available, closing control connection. \r\n"
#define MSG_ABORT             "426 Transfer aborting. \r\n"
#define MSG_INVALID_PATH      "450 Requested file action not taken. \r\n"
#define MSG_EXEC_ERROR        "451 Requested action aborted. Local error in processing.\r\n"
#define MSG_DISK_FULL         "452 Requested action not taken. \r\n"
#define MSG_PARAM_ERROR       "500 Syntax error in parameters or arguments.\r\n"
#define MSG_CMD_FAILED        "502 %s command not implemented.\r\n"
#define MSG_CMD_BAD_SEQ       "503 Bad sequence of commands.\r\n"
#define MSG_AUTHENTICATION_NG "530 Not logged in.\r\n"
#define MSG_NO_SUCH_FILE      "550 No such file or directory.\r\n"

#define USER_NAME_SIZE (32)   /* The size of the user name: To 32 characters   */

#define MAX_ARGC       (8)    /* maximum of the argument */

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern void R_ftpd(void);
extern void R_ftp_srv_open(void);
extern void R_ftp_srv_close(void);
extern uint32_t R_T4_FTP_SERVER_GetVersion(void);

#endif /* _R_T4_FTP_SERVER_RX_IF_H_ */

/* Copyright 2011 RENESAS ELECTRONICS CORPORATION */
