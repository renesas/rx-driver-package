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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_wifi_da16xxx_if.h
 * Description  : API functions definition for DA16XXX of RX65N.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "r_wifi_da16xxx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_WIFI_DA16XXX_CFG_IF_H
#define R_WIFI_DA16XXX_CFG_IF_H

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/*  WiFi API error code */
typedef enum
{
    WIFI_SUCCESS            = 0,      // success
    WIFI_ERR_PARAMETER      = -1,     // invalid parameter
    WIFI_ERR_ALREADY_OPEN   = -2,     // already WIFI module opened
    WIFI_ERR_NOT_OPEN       = -3,     // WIFI module is not opened
    WIFI_ERR_SERIAL_OPEN    = -4,     // serial open failed
    WIFI_ERR_MODULE_COM     = -5,     // cannot communicate WiFi module
    WIFI_ERR_NOT_CONNECT    = -6,     // not connect to access point
    WIFI_ERR_SOCKET_NUM     = -7,     // no available sockets
    WIFI_ERR_SOCKET_CREATE  = -8,     // create socket failed
    WIFI_ERR_CHANGE_SOCKET  = -9,     // cannot change socket
    WIFI_ERR_SOCKET_CONNECT = -10,    // cannot connect socket
    WIFI_ERR_BYTEQ_OPEN     = -11,    // cannot assigned BYTEQ
    WIFI_ERR_SOCKET_TIMEOUT = -12,    // socket timeout
    WIFI_ERR_TAKE_MUTEX     = -13,    // cannot take mutex

    /* For MQTT */
    WIFI_MQTT_ERR_ALREADY_OPEN  = -14,     // already WIFI MQTT opened
    WIFI_MQTT_ERR_NOT_OPEN      = -15,     // WIFI MQTT module is not opened
    WIFI_MQTT_ERR_NOT_CONNECT   = -16,     // not connect to MQTT channel
    WIFI_MQTT_ERR_CONNECTED     = -17,     // already connected to MQTT channel
    WIFI_MQTT_ERR_INVALID_DATA  = -18,     // invalid data to send/receive
    WIFI_MQTT_ERR_COM           = -19,     // cannot communicate WIFI MQTT
    WIFI_MQTT_ERR_OUT_OF_MEMORY = -20,     // out of memory for MQTT communication
} wifi_err_t;

/* Security type */
typedef enum
{
    WIFI_SECURITY_OPEN = 0,           // Open - No Security
    WIFI_SECURITY_WEP,                // WEP Security
    WIFI_SECURITY_WPA,                // WPA Security
    WIFI_SECURITY_WPA2,               // WPA2 Security
    WIFI_SECURITY_WPA2_ENT,           // WPA2 enterprise Security
    WIFI_SECURITY_WPA3,               // WPA3 Security
    WIFI_SECURITY_UNDEFINED           // Unknown Security
} wifi_security_t;

/* Encryption type */
typedef enum
{
    WIFI_ENCRYPTION_TKIP = 0,         // TKIP
    WIFI_ENCRYPTION_AES,              // AES
    WIFI_ENCRYPTION_TKIP_AES,         // TKIP+AES
    WIFI_ENCRYPTION_UNDEFINED,        // Unknow Encryption
} wifi_encryption_t;

/* Socket type */
typedef enum
{
    DA16XXX_SOCKET_TYPE_TCP_SERVER = 0, // TCP server
    DA16XXX_SOCKET_TYPE_TCP_CLIENT,     // TCP client
    DA16XXX_SOCKET_TYPE_UDP             // UDP
} da16xxx_socket_type_t;

/* Query current socket status */
typedef enum
{
    DA16XXX_SOCKET_STATUS_CLOSED = 0,   // "CLOSED"
    DA16XXX_SOCKET_STATUS_SOCKET,       // "SOCKET"
    DA16XXX_SOCKET_STATUS_BOUND,        // "BOUND"
    DA16XXX_SOCKET_STATUS_LISTEN,       // "LISTEN"
    DA16XXX_SOCKET_STATUS_CONNECTED     // "CONNECTED"
} da16xxx_socket_status_t;

/* AT response type */
typedef enum
{
    DA16XXX_RESP_WFJAP,         // AP connect status
    DA16XXX_RESP_WFDAP,         // AP disconnect status
    DA16XXX_RESP_TRDTC,         // incomming TCP socket data
    DA16XXX_RESP_TRXTC          // TCP socket disconnect notification
} da16xxx_resp_type_t;

/* Socket receive state */
typedef enum
{
    DA16XXX_RECV_PREFIX,         // +
    DA16XXX_RECV_CMD,            // command
    DA16XXX_RECV_SUFFIX,         // :
    DA16XXX_RECV_PARAM_CID,      // cid parameter
    DA16XXX_RECV_PARAM_IP,       // ip parameter
    DA16XXX_RECV_PARAM_PORT,     // port parameter
    DA16XXX_RECV_PARAM_LEN,      // length parameter
    DA16XXX_RECV_DATA
} da16xxx_recv_state_t;

/* Error event for user callback */
typedef enum
{
    WIFI_EVENT_WIFI_REBOOT = 0,       // reboot WIFI
    WIFI_EVENT_WIFI_DISCONNECT,       // disconnected WIFI
    WIFI_EVENT_SERIAL_OVF_ERR,        // serial : overflow error
    WIFI_EVENT_SERIAL_FLM_ERR,        // serial : flaming error
    WIFI_EVENT_SERIAL_RXQ_OVF_ERR,    // serial : receiving queue overflow
    WIFI_EVENT_RCV_TASK_RXB_OVF_ERR,  // receiving task : receive buffer overflow
    WIFI_EVENT_SOCKET_CLOSED,         // socket is closed
    WIFI_EVENT_SOCKET_RXQ_OVF_ERR     // socket : receiving queue overflow
} wifi_err_event_enum_t;

/* Enable/disable for SNTP */
typedef enum
{
    WIFI_SNTP_DISABLE = 0,
    WIFI_SNTP_ENABLE  = 1
} wifi_sntp_enable_t;

typedef struct
{
    wifi_err_event_enum_t event;      // Error event
    uint8_t socket_number;            // Socket number
} wifi_err_event_t;

/* AP scan result */
typedef struct
{
    uint8_t             ssid[WIFI_CFG_MAX_SSID_LEN];    // SSID
    uint8_t             bssid[WIFI_CFG_MAX_BSSID_LEN];  // BSSID
    wifi_security_t     security;                       // security type
    wifi_encryption_t   encryption;                     // encryption type
    int8_t              rssi;                           // RSSI
    uint8_t             hidden;                         // Hidden channel
} wifi_scan_result_t;

/* IP configurations */
typedef struct
{
    uint8_t ipaddress[4];             // IP address
    uint8_t subnetmask[4];            // subnet mask
    uint8_t gateway[4];               // gateway
} wifi_ip_configuration_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Open
 * Description  : Open WIFI Module.
 * Arguments    : none.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_ALREADY_OPEN
 *                WIFI_ERR_SERIAL_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_BYTEQ_OPEN
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Open (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Close
 * Description  : Close WIFI Module.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Close (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Ping
 * Description  : Execute Ping command.
 * Arguments    : ip_address
 *                count
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Ping (uint8_t *ip_address, uint16_t count);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Scan
 * Description  : Scan Access points.
 * Arguments    : ap_results
 *                max_networks
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Scan (wifi_scan_result_t * ap_results, uint32_t max_networks);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Connect
 * Description  : Connect to Access Point.
 * Arguments    : ssid
 *                pass
 *                security
 *                enc_type
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Connect (const uint8_t * ssid, const uint8_t * pass, wifi_security_t security,
        wifi_encryption_t enc_type);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_Disconnect
 * Description  : Disconnect from Access Point.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_Disconnect (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_IsConnected
 * Description  : Check connected access point.
 * Arguments    : none
 * Return Value : 0  - connected
 *                -1 - disconnected
 *********************************************************************************************************************/
int32_t    R_WIFI_DA16XXX_IsConnected (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_DnsQuery
 * Description  : Execute DNS query.
 * Arguments    : domain_name
 *                ip_address
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_DnsQuery (uint8_t * domain_name, uint8_t * ip_address);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SntpServerIpAddressSet
 * Description  : Set SNTP server IP address.
 * Arguments    : ip_address
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SntpServerIpAddressSet (uint8_t * ip_address);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SntpEnableSet
 * Description  : Enable or disable SNTP client service.
 * Arguments    : enable
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SntpEnableSet (wifi_sntp_enable_t enable);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SntpTimeZoneSet
 * Description  : Set SNTP timezone.
 * Arguments    : utc_offset_in_hours
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SntpTimeZoneSet (int utc_offset_in_hours);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_LocalTimeGet
 * Description  : Get the current local time based on current timezone in a string . Exp: YYYY-MM-DD,HOUR:MIN:SECS.
 * Arguments    : local_time
 *                size_string
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_LocalTimeGet (uint8_t * local_time, uint32_t size_string);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SetDnsServerAddress
 * Description  : Set DNS Server Address.
 * Arguments    : dns_address
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_SetDnsServerAddress (uint8_t * dns_address);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetMacAddress
 * Description  : Get MAC Address.
 * Arguments    : mac_address.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetMacAddress (uint8_t * mac_address);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetIpAddress
 * Description  : Get IP Address.
 * Arguments    : ip_config.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetIpAddress (wifi_ip_configuration_t * ip_config);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetAvailableSocket
 * Description  : Get the next available socket ID.
 * Arguments    : socket_id
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetAvailableSocket (uint32_t * socket_id);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetSocketStatus
 * Description  : Get the socket status.
 * Arguments    : socket_number
 *                socket_status
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetSocketStatus (uint32_t socket_number, da16xxx_socket_status_t *socket_status);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CreateSocket
 * Description  : Create a new socket instance.
 * Arguments    : socket_number
 *                type
 *                ip_version
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CreateSocket (uint32_t socket_number, da16xxx_socket_type_t type, uint8_t ip_version);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TcpConnect
 * Description  : Connect to a specific IP and Port using socket.
 * Arguments    : socket_number
 *                ip_address
 *                port
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TcpConnect (uint32_t socket_number, uint8_t * ip_address, uint16_t port);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SendSocket
 * Description  : Send data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of sent data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_SendSocket (uint32_t socket_number, uint8_t * data, uint32_t length, uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_ReceiveSocket
 * Description  : Receive data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of received data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_ReceiveSocket (uint32_t socket_number, uint8_t * data, uint32_t length, uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CloseSocket
 * Description  : Disconnect a specific socket connection.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CloseSocket (uint32_t socket_number);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HardwareReset
 * Description  : Reset the module.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_SERIAL_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_BYTEQ_OPEN
 *                WIFI_ERR_TAKE_MUTEX
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HardwareReset (void);

#endif /* R_WIFI_DA16XXX_CFG_IF_H */
