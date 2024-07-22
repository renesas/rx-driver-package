/* Generated configuration header file - do not edit */
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
 * File Name    : r_wifi_da16xxx_config.h
 * Description  : DA16XXX WiFi driver Configuration.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_WIFI_DA16XXX_CONFIG_H
#define R_WIFI_DA16XXX_CONFIG_H

/* Enable DA16600 support
   0 = Disable
   1 = Enable
 */
#define WIFI_CFG_DA16600_SUPPORT              (0)

/* SCI Channel for AT command communication
   Set this option to match the SCI port to be controlled.
 */
#define WIFI_CFG_SCI_CHANNEL                  (6)

/* Interrupt priority of the serial module used for communication with the Wi-Fi module.
   Set this option to a value of 1 to 15 to match the system priority
 */
#define WIFI_CFG_SCI_INTERRUPT_LEVEL          (4)

/* Peripheral clock speed for WIFI_CFG_SCI_CHANNEL
 */
#define WIFI_CFG_SCI_PCLK_HZ                  (60000000)

/* Communication baud rate for WIFI_CFG_SCI_CHANNEL
   Value must be 115200/230400/460800/921600
 */
#define WIFI_CFG_SCI_BAUDRATE                 (115200)

/* UART hardware flow control
   0 = CTS(Hardware), RTS(Software)
   1 = RTS(Hardware), CTS(Software)
 */
#define WIFI_CFG_CTS_SW_CTRL                  (1)

/* Configure CTS pin
 */
#define WIFI_CFG_CTS_PORT                     J
#define WIFI_CFG_CTS_PIN                      3

/* Configure RTS pin
 */
#define WIFI_CFG_RTS_PORT                     J
#define WIFI_CFG_RTS_PIN                      3

/* RTS pin function set value
 */
#define WIFI_CFG_PFS_SET_VALUE                0x0AU

/* Configure RESET pin
 */
#define WIFI_CFG_RESET_PORT                   5
#define WIFI_CFG_RESET_PIN                    5

/* Board dependent settings; please use the value for each setting listed below depending on the board you use.

Preprocessors that define board dependent settings and the corresponding values to be set are as follows:
Confirmed board number          1      2      3      4      5      6      7      8      9      10
WIFI_CFG_SCI_CHANNEL            0      2      6      1      6      0      6      2      5      5
WIFI_CFG_SCI_PCLK_HZ (*note1)   60000000
WIFI_CFG_CTS_PORT               2      J      J      3      J      2      J      J      C      C
WIFI_CFG_CTS_PIN                3      5      3      1      3      3      3      5      0      0
WIFI_CFG_RTS_PORT               2      J      J      3      J      2      J      J      C      C
WIFI_CFG_RTS_PIN                3      5      3      1      3      3      3      5      0      0
WIFI_CFG_PFS_SET_VALUE (*note2) 0x0BU  0x0BU  0x0AU  0x0BU  0x0AU  0x0BU  0x0AU  0x0BU  0x0BU  0x0BU
WIFI_CFG_RESET_PORT             D      5      F      2      5      A      5      A      B      B
WIFI_CFG_RESET_PIN              0      5      5      0      5      1      5      1      1      1

where the confirmed board numbers listed in the first row above are as follows (*note3):
1: RX65N Cloud Kit (PMOD(CN5)),
2: RX65N Envision Kit (PMOD(CN14)),
3: RX65N RSK (2MB) (PMOD1),
4: RX65N RSK (2MB) (PMOD2),
5: Cloud Kit for RX65N v1, CK-RX65N v1 (PMOD1),
6: Cloud Kit for RX65N v1, CK-RX65N v1 (PMOD2),
7: Cloud Kit for RX65N v2, CK-RX65N v2 (PMOD1),
8: Cloud Kit for RX65N v2, CK-RX65N v2 (PMOD2),
9: RX671 Target Board (PMOD(CN1)) (not tested), *note4.
10: RX66N Target Board (PMOD(CN1)) (not tested), *note5.
In the above preprocessor list, please use one of the values listed on the right side.
On the right side, each column corresponds to each confirmed board number.

Note1: Check Peripheral clock speed in Clocks tab on Smart Configuration.
    For Peripheral module clock, check board group hardware manual, Serial Communications Interface section in Note2.
    e.g. if PCLKB speed is 60MHz then WIFI_CFG_SCI_PCLK_HZ is 60000000).

Note2: Check Pin Function Select (PSEL) of Pin Function Control Register (PxyPFS)
    in Multi-Function Pin Controller (MPC) section of board group hardware manual.
    Below is list of board group hardware manual:
    1: RX65N group: https://www.renesas.com/us/en/document/mah/rx65n-group-rx651-group-users-manualhardware-rev230
    2: RX671 group: https://www.renesas.com/tw/en/document/mah/rx671-group-users-manual-hardware-rev100
    3: RX66N group: https://www.renesas.com/us/en/document/mah/rx66n-group-users-manual-hardware-rev111

Note3: List of board user's manual:
    1: RX65N Cloud Kit:    https://www.renesas.com/us/en/document/mat/uses-manual-cloud-option-board
    2: RX65N Envision Kit: https://www.renesas.com/us/en/document/mat/rx65n-envision-kit-users-manual-rev100
    3: RX65N RSK (2MB):    https://www.renesas.com/us/en/document/mat/renesas-starter-kit-rx65n-2mb-users-manual
    4: CK-RX65N v1:        https://www.renesas.com/kr/en/document/mat/ck-rx65n-v1-users-manual
    5: RX671 Target Board: https://www.renesas.com/br/en/document/mat/target-board-rx671-users-manual-rev100
    6: RX66N Target Board: https://www.renesas.com/us/en/document/mah/target-board-rx66n-users-manual-rev100

Note4:
When you use RX671 Target Board, you need pattern cut and so on to use SCI channel 5(TXD5/RXD5/CTS5) and GPIO(PC1).
Please refer to User's Manual: https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/rtk5rx6710c00000bj-target-board-rx671

Note5:
When you use RX66N Target Board, you need remodeling of the board to use SCI channel 5(TXD5/RXD5/CTS5) and GPIO(PC1).
Please refer to User's Manual: https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/rtk5rx66n0c00000bj-target-board-rx66n

*/

/* AT command transfer buffer size
   Value must be in the range 1-8192
 */
#define WIFI_CFG_AT_CMD_TX_BUFFER_SIZE          (512)

/* AT command receive buffer size
   Value must be in the range 1-8192
 */
#define WIFI_CFG_AT_CMD_RX_BUFFER_SIZE          (3000)

/* Wi-Fi callback function use
   0 = Unused
   1 = Used
 */
#define WIFI_CFG_USE_CALLBACK_FUNCTION          (0)

#if WIFI_CFG_USE_CALLBACK_FUNCTION == 1

/* Wi-Fi callback function name
 */
#define WIFI_CFG_CALLBACK_FUNCTION_NAME         (NULL)

#endif

/* Max AP's SSID length
   Value must be an decimal value
 */
#define WIFI_CFG_MAX_SSID_LEN                   32

/* Max AP's BSSID length
   Value must be an decimal value
 */
#define WIFI_CFG_MAX_BSSID_LEN                  6

/* Max AP's password length
   Value must be an decimal value
 */
#define WIFI_CFG_MAX_PASS_LEN                   32

/* Use SNTP client service
   0 = Disable SNTP client service.
   1 = Enable SNTP client service.
 */
#define WIFI_CFG_SNTP_ENABLE                    0

#if WIFI_CFG_SNTP_ENABLE == 1

/* The SNTP server IP address string
 */
#define WIFI_CFG_SNTP_SERVER_IP                 "0.0.0.0"

/* Timezone offset in hours (-12 ~ 12)
 */
#define WIFI_CFG_SNTP_UTC_OFFSET                7

#endif

/* Country code
   Country code defined in ISO3166-1 alpha-2 standard.
 */
#define WIFI_CFG_COUNTRY_CODE                   ""

/* Use FreeRTOS logging functionality
   0 = Not use FreeRTOS logging functionality.
   1 = Use FreeRTOS logging functionality.
 */
#define WIFI_CFG_USE_FREERTOS_LOGGING           0

/* Debug log output level
   0: OFF
   1: ERROR
   2: +WARN
   3: +INFO
   4: +DEBUG(AT command data)
 */
#define WIFI_CFG_DEBUG_LOG                      0

/* TCP protocol support
   0 = Not use TCP protocol.
   1 = Use TCP protocol.
 */
#define WIFI_CFG_TCP_SUPPORT                    1

/* Creatable TCP Sockets number
   Value must be in the range 1-4
 */
#define WIFI_CFG_TCP_CREATABLE_SOCKETS          (1)

/* Socket Receive buffer size
   Value must be in the range 1-8192
 */
#define WIFI_CFG_TCP_SOCKET_RECEIVE_BUFFER_SIZE (4096)

/* MQTT protocol support
   0 = Not use MQTT protocol
   1 = Use MQTT protocol.
 */
#define WIFI_CFG_MQTT_SUPPORT                   0

/* Use MQTT Certificates
   0 = Not use MQTT Certificates.
   1 = Use MQTT Certificates.
 */
#define WIFI_CFG_MQTT_CERTS                     (0)

#if WIFI_CFG_MQTT_CERTS == 1

/* Certificates|Root CA
   Name of header file that will contain certificates (macros).
   User must create header file.
   Example: "cert_storage.h"
 */
#define WIFI_CFG_MQTT_CERTS_HEADER              /* Header file name */

/* Certificates|Root CA
   Links to user-defined macro of the same name for Root CA
   which user must define in application header.
 */
#define WIFI_CFG_MQTT_ROOT_CA                   /* Root CA */

/* Certificates|Client Certificate
   Links to user-defined macro of the same name for client certificate
   which user must define in application header.
 */
#define WIFI_CFG_MQTT_CLIENT_CERT               /* Client certificate */

/* Certificates|Private Key
   Links to user-defined macro of the same name for private key
   which user must define in application header
 */
#define WIFI_CFG_MQTT_PRIVATE_KEY               /* Client private key */

#endif

/* Size in bytes of the MQTT buffer used for sending commands and publishing data
   Value must be in the range 200~2064 and must be less than or equal to Wi-Fi TX buffer
 */
#define WIFI_CFG_MQTT_CMD_TX_BUF_SIZE           (512)

/* Size in bytes of the MQTT buffer used for receiving subscribed data
   Value must be in the range 1-3000 and must be less than or equal to Wi-Fi RX buffer
 */
#define WIFI_CFG_MQTT_CMD_RX_BUF_SIZE           (512)

/* Use MQTT protocol v3.1.1
   0 = Disable MQTT protocol v3.1.1.
   1 = Enable MQTT protocol v3.1.1.
 */
#define WIFI_CFG_MQTT_USE_MQTT_V311             (1)

/* MQTT Receive Maximum Timeout (ms)
   Timeout for the MQTT Receive function to check the buffer
   for incoming MQTT messages in milliseconds.
   Set between 0~65535
 */
#define WIFI_CFG_MQTT_RX_TIMEOUT                (1000)

/* MQTT Transit Maximum Timeout (ms)
   Timeout for publishing MQTT messages in milliseconds.
   Set between 0~65535
 */
#define WIFI_CFG_MQTT_TX_TIMEOUT                (1000)

/* Use Clean Session
   0 = Disable Clean Session.
   1 = Enable Clean Session.
 */
#define WIFI_CFG_MQTT_CLEAN_SESSION             (1)

/* ALPN1
   Select 1st Application Layer Protocol Negotiations (ALPNs).
 */
#define WIFI_CFG_MQTT_ALPN1                     /* Select ALPN1 */

/* ALPN2
   Select 2nd Application Layer Protocol Negotiations (ALPNs).
 */
#define WIFI_CFG_MQTT_ALPN2                     /* Select ALPN2 */

/* ALPN3
   Select 3rd Application Layer Protocol Negotiations (ALPNs).
 */
#define WIFI_CFG_MQTT_ALPN3                     /* Select ALPN3 */

/* UseKeep Alive (s)
   MQTT ping period to check if connection is still active.
 */
#define WIFI_CFG_MQTT_KEEP_ALIVE                (60)

/* Client Identifier
 */
#define WIFI_CFG_MQTT_CLIENT_IDENTIFIER         /* MQTT Client identifier */

/* Host Name (or IP address)
 */
#define WIFI_CFG_MQTT_HOST_NAME                 /* MQTT broker name */

/* MQTT Port
 */
#define WIFI_CFG_MQTT_PORT                      (1883)

/* MQTT User Name
 */
#define WIFI_CFG_MQTT_USER_NAME                 /* Client user name */

/* MQTT Password
 */
#define WIFI_CFG_MQTT_PASSWORD                  /* Client pass word */

/* Last Will Topic
 */
#define WIFI_CFG_MQTT_WILL_TOPIC                /* Will topic */

/* Last Will Message
 */
#define WIFI_CFG_MQTT_WILL_MESSAGE              /* Will message */

/* Server Name Indication (SNI)
 */
#define WIFI_CFG_MQTT_SNI_NAME                  /* SNI name */

/* Quality-of-Service for MQTT Last Will message
   0: At most once (QoS 0).
   1: At least once (QoS 1).
   2: Exactly once (QoS 2).
 */
#define WIFI_CFG_MQTT_WILL_QOS                  (0)

/* Flag to use TLS Cipher Suites.
   0 = Unused.
   1 = Used.
 */
#define WIFI_CFG_MQTT_TLS_CIPHER_SUITES         (0)

#if WIFI_CFG_MQTT_TLS_CIPHER_SUITES == 1

/* Select TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA        (0)

/* Select TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA Cipher.
   Unused: 0.
   Used: WIFI_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA        (0)

/* Select TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256     (0)

/* Select TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384     (0)

/* Select TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256     (0)

/* Select TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384     (0)

/* Select TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA      (0)

/* Select TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA      (0)

/* Select TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256   (0)

/* Select TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384   (0)

/* Select TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256   (0)

/* Select TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 Cipher.
    Unused: 0.
    Used: WIFI_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
 */
#define WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384   (0)

#endif

/* Enables or disables the user MQTT callback function.
    0 = Unused.
    1 = Used.
 */
#define WIFI_CFG_MQTT_P_CALLBACK                1

#if WIFI_CFG_MQTT_P_CALLBACK == 1

/* Specifies function name of the MQTT callback function called when receive data subscribed.
 */
#define WIFI_CFG_MQTT_P_CALLBACK_FUNCTION_NAME  mqtt_userCallback

#endif

/* Enables or disables TLS on-chip protocol.
    0 = disabled.
    1 = enabled.
 */
#define WIFI_CFG_TLS_SUPPORT                    0

/* Configures the number of TLS client socket.
    Set this value in range from 1 to 2.
 */
#define WIFI_CFG_TLS_CREATABLE_SOCKETS          (1)

/* Configures the receive buffer size for the socket.
    Set this value in range from 1 to 8192.
 */
#define WIFI_CFG_TLS_SOCKET_RECEIVE_BUFFER_SIZE (4096)

/* Flag to use CA certificates.
    0 = Unused.
    1 = Used.
 */
#define WIFI_CFG_TLS_USE_CA_CERT                1

/* Configures length for certificate’s name.
 */
#define WIFI_CFG_TLS_CERT_MAX_NAME              32

/* Configures CA certificate name.
 */
#define WIFI_CFG_TLS_CERT_CA_NAME               ""

/* Configures Client certificate name.
 */
#define WIFI_CFG_TLS_CERT_CLIENT_NAME           ""

/* Configures Private certificate name.
 */
#define WIFI_CFG_TLS_CERT_PRIVATE_NAME           ""

/* Enables or disables HTTP on-chip protocol.
    0 = disabled
    1 = enabled
 */
#define WIFI_CFG_HTTP_SUPPORT                    0

/* Configures Server Name Indication (SNI).
 */
#define WIFI_CFG_HTTP_SNI_NAME                  /* SNI name */

/* Select 1st Application Layer Protocol Negotiation (ALPN).
 */
#define WIFI_CFG_HTTP_ALPN1                     /* Select ALPN1 */

/* Select 2nd ALPN.
 */
#define WIFI_CFG_HTTP_ALPN2                     /* Select ALPN2 */

/* Select 3rd ALPN.
 */
#define WIFI_CFG_HTTP_ALPN3                     /* Select ALPN3 */

/* Configures HTTP TLS Authentication levels.
    0: None - No authentication required; accept connections without any form of authentication.
    1: Optional - Allow both authenticated and unauthenticated connections.
    2: Require - Demand authentication for connections.
 */
#define WIFI_CFG_HTTP_TLS_AUTH                  (0)

#if WIFI_CFG_HTTP_TLS_AUTH != 0

/* Name of header file that will contain certificates (macros).
    User must create header file.
    Example: "cert_storage.h"
 */
#define WIFI_CFG_HTTP_CERTS_HEADER              /* Header file name */

/* Root CA
    Links to user-defined macro of the same name for Root CA
    which user must define in application header.
 */
#define WIFI_CFG_HTTP_ROOT_CA                   /* Root CA */

/* Client certificate
    Links to user-defined macro of the same name for client certificate
    which user must define in application header.
 */
#define WIFI_CFG_HTTP_CLIENT_CERT               /* Client certificate */

/* Client private key
    Links to user-defined macro of the same name for private key
    which user must define in application header.
 */
#define WIFI_CFG_HTTP_PRIVATE_KEY               /* Client private key */

#endif

/* End HTTP macro definitions */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* R_WIFI_DA16XXX_CONFIG_H */
