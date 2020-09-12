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
* File Name    : r_t4_sntp_client_rx_if.h
* Version      : 1.00
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.02.2017 1.00    First Release
***********************************************************************************************************************/

#ifndef R_T4_SNTP_CLIENT_RX_IF_H_
#define R_T4_SNTP_CLIENT_RX_IF_H_

#include "r_t4_sntp_client_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define T4_SNTP_CLIENT_VERSION_MAJOR       (1)
#define T4_SNTP_CLIENT_VERSION_MINOR       (00)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum _sntp_err
{
    SNTP_SUCCESS = 0,                /* Normally terminated.                    */
    SNTP_ERR_BAD_UDP_SETTINGS,       /* Bad UDP settings is set                 */
    SNTP_ERR_TIMEOUT,                /* SNTP timeout is occurred                */
    SNTP_ERR_DNS_NOT_WORKED,         /* DNS error is occurred                   */
    SNTP_ERR_SERVER_NOT_WORKED,      /* SNTP server is not registered or worked */
    SNTP_ERR_NOT_OPENED,             /* SNTP module is not opened               */
    SNTP_ERR_MODULE_NOT_REGISTERED,         /* The related module is not registered    */
    SNTP_ERR_REQUEST_ALREADY_STARTED /* SNTP module is already started          */
} sntp_err_t;

typedef int32_t (*callback_from_sntp_t)(int32_t ercd, int32_t stamp);
typedef struct _sntp_server
{
    uint8_t *local_primary_sntp_server_name;
    uint8_t *local_secondary_sntp_server_name;
    uint8_t *global_primary_sntp_server_name;
    uint8_t *global_secondary_sntp_server_name;
    uint8_t *time_offset;
} R_TCPIP_SNTP_CLIENT_SETTINGS;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
sntp_err_t R_TCPIP_SntpClientOpen(callback_from_sntp_t function_pointer, R_TCPIP_SNTP_CLIENT_SETTINGS *r_tcpip_sntp_client_settings);
sntp_err_t R_TCPIP_SntpClientStartRequest(void);
sntp_err_t R_TCPIP_SntpClientClose(void);
ER      R_TCPIP_SntpClientCallback(ID cepid, FN fncd , VP p_parblk);
uint32_t R_TCPIP_SntpClientGetVersion(void);

#endif /* R_T4_SNTP_CLIENT_RX_IF_H_ */


