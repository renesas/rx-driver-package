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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
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

/* Version Number of API. */
#define WIFI_VERSION_MAJOR  (1)
#define WIFI_VERSION_MINOR  (20)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/*  WiFi API error code */
typedef enum
{
    WIFI_SUCCESS                = 0,      // success
    WIFI_ERR_PARAMETER          = -1,     // invalid parameter
    WIFI_ERR_ALREADY_OPEN       = -2,     // already WIFI module opened
    WIFI_ERR_NOT_OPEN           = -3,     // WIFI module is not opened
    WIFI_ERR_SERIAL_OPEN        = -4,     // serial open failed
    WIFI_ERR_MODULE_COM         = -5,     // communicate with module failed
    WIFI_ERR_MODULE_TIMEOUT     = -6,     // communicate with module timed out
    WIFI_ERR_NOT_CONNECT        = -7,     // not connect to access point
    WIFI_ERR_SOCKET_NUM         = -8,     // no available sockets
    WIFI_ERR_SOCKET_CREATE      = -9,    // create socket failed
    WIFI_ERR_CHANGE_SOCKET      = -10,    // cannot change socket
    WIFI_ERR_SOCKET_CONNECT     = -11,    // cannot connect socket
    WIFI_ERR_BYTEQ_OPEN         = -12,    // cannot assigned BYTEQ
    WIFI_ERR_SOCKET_TIMEOUT     = -13,    // socket timeout
    WIFI_ERR_TAKE_MUTEX         = -14,    // cannot take mutex

    /* For MQTT */
    WIFI_ERR_MQTT_ALREADY_OPEN  = -15,    // already WIFI MQTT opened
    WIFI_ERR_MQTT_NOT_OPEN      = -16,    // WIFI MQTT module is not opened
    WIFI_ERR_MQTT_NOT_CONNECT   = -17,    // not connect to MQTT channel
    WIFI_ERR_MQTT_CONNECTED     = -18,    // already connected to MQTT channel
    WIFI_ERR_MQTT_INVALID_DATA  = -19,    // invalid data to send/receive
    WIFI_ERR_MQTT_OUT_OF_MEMORY = -20,    // out of memory for MQTT communication

    /* For HTTP */
    WIFI_ERR_HTTP_ALREADY_OPEN  = -21,    // already WIFI HTTP opened
    WIFI_ERR_HTTP_NOT_OPEN      = -22,    // WIFI HTTP module is not opened
    WIFI_ERR_HTTP_INVALID_DATA  = -23,    // invalid data to send/receive
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
    WIFI_SOCKET_TYPE_TCP_SERVER = 0, // TCP server
    WIFI_SOCKET_TYPE_TCP_CLIENT,     // TCP client
    WIFI_SOCKET_TYPE_UDP,            // UDP
    WIFI_SOCKET_TYPE_TLS             // TLS client
} wifi_socket_type_t;

/* Certificate type */
typedef enum
{
    WIFI_TLS_TYPE_CA_CERT = 0,           // CA Certificate
    WIFI_TLS_TYPE_CLIENT_CERT,           // Client Certificate
    WIFI_TLS_TYPE_CLIENT_PRIVATE_KEY,    // Client Private Key
    WIFI_TLS_TYPE_UNDEFINED              // Unknown Encryption
} wifi_tls_key_type_t;

/* Query current socket status */
typedef enum
{
    WIFI_SOCKET_STATUS_CLOSED = 0,   // "CLOSED"
    WIFI_SOCKET_STATUS_SOCKET,       // "SOCKET"
    WIFI_SOCKET_STATUS_BOUND,        // "BOUND"
    WIFI_SOCKET_STATUS_LISTEN,       // "LISTEN"
    WIFI_SOCKET_STATUS_CONNECTED     // "CONNECTED"
} wifi_socket_status_t;

/* AT response type */
typedef enum
{
    WIFI_RESP_NONE,          // none
    WIFI_RESP_WFJAP,         // AP connect status
    WIFI_RESP_WFDAP,         // AP disconnect status
    WIFI_RESP_TRDTC,         // incomming TCP socket data
    WIFI_RESP_TRXTC,         // TCP socket disconnect notification
    WIFI_RESP_NWMQCL,        // MQTT connect status
    WIFI_RESP_NWMQMSG,       // MQTT message
    WIFI_RESP_NWMQMSGSND,    // MQTT send message status
    WIFI_RESP_TRSSLDTC,      // incomming TLS socket data
    WIFI_RESP_NWHTCSTATUS,   // HTTP response status code
    WIFI_RESP_NWHTCDATA      // HTTP response data
} wifi_resp_type_t;

/* Socket receive state */
typedef enum
{
    WIFI_RECV_PREFIX,         // +
    WIFI_RECV_CMD,            // command
    WIFI_RECV_SUFFIX,         // :
    WIFI_RECV_PARAM_CID,      // cid parameter
    WIFI_RECV_PARAM_IP,       // ip parameter
    WIFI_RECV_PARAM_PORT,     // port parameter
    WIFI_RECV_PARAM_LEN,      // length parameter
    WIFI_RECV_DATA
} wifi_recv_state_t;

/* MQTT Quality-of-service (QoS) levels */
typedef enum
{
    WIFI_MQTT_QOS_0 = 0,     // Delivery at most once.
    WIFI_MQTT_QOS_1 = 1,     // Delivery at least once.
    WIFI_MQTT_QOS_2 = 2      // Delivery exactly once.
} wifi_mqtt_qos_t;

/* MQTT TLS Cipher Suites */
typedef enum
{
    WIFI_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA      = 0xC011, // TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA protocol.
    WIFI_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA      = 0xC014, // TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA protocol.
    WIFI_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256   = 0xC027, // TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 protocol.
    WIFI_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384   = 0xC028, // TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 protocol.
    WIFI_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256   = 0xC02F, // TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 protocol.
    WIFI_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384   = 0xC030, // TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 protocol.
    WIFI_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA    = 0xC009, // TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA protocol.
    WIFI_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA    = 0xC00A, // TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA protocol.
    WIFI_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 = 0xC023, // TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 protocol.
    WIFI_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 = 0xC024, // TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 protocol.
    WIFI_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 = 0xC02B, // TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 protocol.
    WIFI_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 = 0xC02C, // TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 protocol.
} wifi_tls_cipher_suites_t;

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

/* MQTT SUBSCRIBE packet parameters */
typedef struct st_wifi_mqtt_sub_info
{
    wifi_mqtt_qos_t    qos;                  // Quality of Service for subscription.
    const char       * p_topic_filter;       // Topic filter to subscribe to.
    uint16_t           topic_filter_length;  // Length of subscription topic filter.
} wifi_mqtt_sub_info_t;

/* MQTT PUBLISH packet parameters */
typedef struct st_wifi_mqtt_pub_info
{
    wifi_mqtt_qos_t    qos;                // Quality of Service for subscription.
    const char       * p_topic_name;       // Topic name on which the message is published.
    uint16_t           topic_name_Length;  // Length of topic name.
    const char *       p_payload;          // Message payload.
    uint32_t           payload_length;     // Message payload length.
} wifi_mqtt_pub_info_t;

/* MQTT Packet info structure to be passed to user callback */
typedef struct st_wifi_mqtt_callback_args
{
    uint8_t    * p_data;               // Payload received from subscribed MQTT topic.
    const char * p_topic;              // Topic to which the message payload belongs to.
    uint32_t     data_length;          // Length of the MQTT payload.
    void const * p_context;            // Placeholder for user data.
} wifi_mqtt_callback_args_t;

/* TCP TLS certificate information */
typedef struct {
    uint8_t cert_ca[WIFI_CFG_TLS_CERT_MAX_NAME];
    uint8_t cert_name[WIFI_CFG_TLS_CERT_MAX_NAME];
} wifi_tls_cert_info_t;

/* HTTP methods */
typedef enum
{
    WIFI_HTTP_GET  = 0,     // GET method
    WIFI_HTTP_POST = 1,     // POST method
    WIFI_HTTP_PUT  = 2      // PUT method
} wifi_http_method_t;

/* HTTP TLS authentication */
typedef enum
{
    WIFI_HTTP_TLS_VERIFY_NONE     = 0,     // No needed verify client certification
    WIFI_HTTP_TLS_VERIFY_OPTIONAL = 1,     // Request client certification but not mandatory
    WIFI_HTTP_TLS_VERIFY_REQUIRED = 2      // Require client certification
} wifi_http_tls_auth_t;

/* HTTP request packet parameters */
typedef struct st_wifi_http_request
{
    const char          * http_endpoint;      // HTTP endpoint
    wifi_http_method_t    method;             // HTTP request method
    const char          * request_body;       // HTTP request header
    uint32_t              length;             // HTTP request length
} wifi_http_request_t;

typedef struct st_wifi_http_buffer
{
    char   * response_buffer;
    uint32_t resp_length;
} wifi_http_buffer_t;

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
 * Function Name: R_WIFI_DA16XXX_IsOpened
 * Description  : Check wifi is opened.
 * Arguments    : none.
 * Return Value : 0  - opened
 *                -1 - not opened
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_IsOpened (void);

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
wifi_err_t R_WIFI_DA16XXX_SntpTimeZoneSet (int32_t utc_offset_in_hours);

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

/******************************************************************************************************************//**
 * Function Name: R_WIFI_DA16XXX_GetVersion
 * Description  : Get the Wi-Fi FIT module version.
 * Arguments    : none
 * Return Value : The version of Wi-Fi module. The version number is encoded such that the top 2 bytes are the major
 *                version number and the bottom 2 bytes are the minor version number.
 **********************************************************************************************************************/
uint32_t R_WIFI_DA16XXX_GetVersion(void);

#if WIFI_CFG_TCP_SUPPORT == 1
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
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetSocketStatus (uint32_t socket_number, wifi_socket_status_t *socket_status);

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
wifi_err_t R_WIFI_DA16XXX_CreateSocket (uint32_t socket_number, wifi_socket_type_t type, uint8_t ip_version);

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
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_SendSocket (uint32_t socket_number, uint8_t * data,
                                   uint32_t length, uint32_t timeout_ms);

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
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_ReceiveSocket (uint32_t socket_number, uint8_t * data,
                                      uint32_t length, uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CloseSocket
 * Description  : Disconnect a specific socket connection.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CloseSocket (uint32_t socket_number);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TcpReconnect
 * Description  : Reconnect TCP socket.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TcpReconnect (uint32_t socket_number);
#endif /* WIFI_CFG_TCP_SUPPORT */

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

#if WIFI_CFG_MQTT_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttOpen
 * Description  : Initialize the DA16XXX on-chip MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_ALREADY_OPEN
 *                WIFI_ERR_MQTT_INVALID_DATA
 *                WIFI_ERR_MQTT_OUT_OF_MEMORY
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttOpen (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttDisconnect
 * Description  : Disconnect from DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttDisconnect (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttConnect
 * Description  : Configure and connect the DA16XXX MQTT Client service.
 * Arguments    : timeout_ms
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *                WIFI_ERR_MQTT_CONNECTED
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttConnect (uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttPublish
 * Description  : Publish a message for a given MQTT topic.
 * Arguments    : p_pub_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *                WIFI_ERR_MQTT_INVALID_DATA
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttPublish (wifi_mqtt_pub_info_t * const p_pub_info);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttSubscribe
 * Description  : Subscribe to DA16XXX MQTT topics.
 * Arguments    : p_sub_info
 *                subscription_count
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *                WIFI_ERR_MQTT_INVALID_DATA
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttSubscribe (wifi_mqtt_sub_info_t * const p_sub_info,
                                         size_t subscription_count);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttUnSubscribe
 * Description  : Unsubscribe from DA16XXX MQTT topics.
 * Arguments    : p_sub_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *                WIFI_ERR_MQTT_INVALID_DATA
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttUnSubscribe (wifi_mqtt_sub_info_t * const p_sub_info);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttReceive
 * Description  : Receive data subscribed to on DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MQTT_INVALID_DATA
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttReceive (void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttClose
 * Description  : Close the DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttClose (void);
#endif /* WIFI_CFG_MQTT_SUPPORT */

#if WIFI_CFG_TLS_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetAvailableTlsSocket
 * Description  : Get the next available socket ID.
 * Arguments    : socket_id
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetAvailableTlsSocket (uint32_t * socket_id);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetTlsSocketStatus
 * Description  : Get the socket status.
 * Arguments    : socket_number
 *                socket_status
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetTlsSocketStatus (uint32_t socket_number, wifi_socket_status_t *socket_status);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CreateTlsSocket
 * Description  : Create a new socket instance.
 * Arguments    : socket_number
 *                type
 *                ip_version
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CreateTlsSocket (uint32_t socket_number, wifi_socket_type_t type, uint8_t ip_version);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TlsConnect
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
wifi_err_t R_WIFI_DA16XXX_TlsConnect (uint32_t socket_number, uint8_t * ip_address, uint16_t port);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SendTlsSocket
 * Description  : Send data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of sent data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_SendTlsSocket (uint32_t socket_number, uint8_t * data,
                                      uint32_t length, uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_ReceiveTlsSocket
 * Description  : Receive data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of received data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_ReceiveTlsSocket (uint32_t socket_number, uint8_t * data,
                                         uint32_t length, uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CloseTlsSocket
 * Description  : Disconnect a specific socket connection.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CloseTlsSocket (uint32_t socket_number);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TlsReconnect
 * Description  : Reconnect TLS socket.
 * Arguments    : socket_number - UINT8_MAX for reconnect all disconnected sockets.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TlsReconnect (uint32_t socket_number);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_RegistServerCertificate
 * Description  : Register server certificate on WiFi module.
 * Arguments    : socket_num
 *                cert_info
 *                trans_buf_size
 *                recv_buf_size
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_RegistServerCertificate(uint8_t socket_num,
                                                  wifi_tls_cert_info_t * cert_info,
                                                  uint32_t trans_buf_size, uint32_t recv_buf_size);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_RequestTlsSocket
 * Description  : Request TLS socket communication.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_RequestTlsSocket(uint32_t socket_number);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetServerCertificate
 * Description  : Get stored server certificate on WiFi module.
 * Arguments    : cert_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetServerCertificate(wifi_tls_cert_info_t * cert_info);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_WriteCertificate
 * Description  : Write certificate on WiFi module.
 * Arguments    : name
 *                type_key
 *                p_data
 *                len
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_WriteCertificate(const uint8_t * name, wifi_tls_key_type_t type_key,
                                           const uint8_t * p_data, uint32_t len);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_DeleteCertificate
 * Description  : Delete certificate on WiFi module.
 * Arguments    : type_key
 *                cert_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_DeleteCertificate(wifi_tls_key_type_t type_key,
                                            wifi_tls_cert_info_t * cert_info);
#endif /* WIFI_CFG_TLS_SUPPORT */

#if WIFI_CFG_HTTP_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HttpOpen
 * Description  : Initialize the DA16XXX on-chip HTTP Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *                WIFI_ERR_HTTP_ALREADY_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HttpOpen(void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HttpClose
 * Description  : Close the DA16XXX HTTP Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_HTTP_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HttpClose(void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HttpSend
 * Description  : Send the HTTP request with the configured buffers.
 * Arguments    : request
 *                buffer
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *                WIFI_ERR_HTTP_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HttpSend (wifi_http_request_t request, wifi_http_buffer_t *buffer);
#endif /* WIFI_CFG_HTTP_SUPPORT */

#endif /* R_WIFI_DA16XXX_CFG_IF_H */
