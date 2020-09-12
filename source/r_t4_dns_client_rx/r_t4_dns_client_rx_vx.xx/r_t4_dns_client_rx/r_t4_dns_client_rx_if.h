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
* File Name    : r_t4_dns_client_rx_if.h
* Version      : 1.03
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 04.04.2011 1.00    First Release
*         : 09.05.2014 1.01    Corresponded to FIT Modules
*         : 01.07.2014 1.02    Clean up source code
*         : 01.04.2016 1.03    XML file update
*         : 01.03.2017 1.04    Change API function name
***********************************************************************************************************************/
#ifndef R_T4_DNS_CLIENT_RX_IF_H
#define R_T4_DNS_CLIENT_RX_IF_H

#include "r_t4_dns_client_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define T4_DNS_CLIENT_VERSION_MAJOR       (1)
#define T4_DNS_CLIENT_VERSION_MINOR       (04)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum _dns_err
{
    DNS_SUCCESS = 0,                  /* Normally terminated.                    */
    DNS_ERR_MODULE_NOT_REGISTERED,    /* The related module is not registered    */
    DNS_ERR_SERVER_NOT_REGISTERED,    /* The DNS server is not registered        */
    DNS_ERR_NOT_OPENED,               /* DNS client is not opened                */
    DNS_ERR_ALREADY_OPENED,           /* DNS client is already opened            */
    DNS_ERR_PROCESSING,               /* Processing                              */
    DNS_ERR_TIMEOUT,                  /* DNS time out                            */
    DNS_ERR_BAD_UDP_SETTINGS,         /* Bad UDP settings are set                */
    DNS_ERR_INVALID_PACKET_RECEIVED   /* Received DNS packet is invalid          */
} dns_err_t;


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct _name_table
{
    char name[DNS_CFG_NAME_SIZE];
    unsigned char ipaddr[4];
}name_table_t;

typedef int32_t (*DNS_CB_FUNC)(int32_t ercd, name_table_t *table);

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
dns_err_t  R_TCPIP_DnsClientOpen(void);
dns_err_t  R_TCPIP_DnsClientRegisterServerAddress(uint32_t dns_ipaddr1, uint32_t dns_ipaddr2);
dns_err_t  R_TCPIP_DnsClientResolveName(char *name, DNS_CB_FUNC func);
uint32_t   R_TCPIP_DnsClientGetVersion(void);
ER         R_TCPIP_DnsClientCallback(ID cepid, FN fncd , VP p_parblk);
dns_err_t  R_TCPIP_DnsClientClose(void);
dns_err_t  dns_client_process(void);

#endif /* R_T4_DNS_CLIENT_RX_IF_H */
