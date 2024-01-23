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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
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

#define WIFI_CFG_DA16600_SUPPORT              0

#define WIFI_CFG_SCI_CHANNEL                  (0)

#define WIFI_CFG_SCI_INTERRUPT_LEVEL          (4)

#define WIFI_CFG_SCI_PCLK_HZ                  (60000000)  // peripheral clock speed in Clocks tab of Smart Configuration

#define WIFI_CFG_SCI_BAUDRATE                 (115200)

#define WIFI_CFG_CTS_SW_CTRL                  (1)

#define WIFI_CFG_CTS_PORT                     2
#define WIFI_CFG_CTS_PIN                      3

#define WIFI_CFG_RTS_PORT                     2
#define WIFI_CFG_RTS_PIN                      3
#define WIFI_CFG_PFS_SET_VALUE                0x0BU

#define WIFI_CFG_RESET_PORT                   A
#define WIFI_CFG_RESET_PIN                    1

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

#define WIFI_CFG_CREATABLE_SOCKETS            (1)

#define WIFI_CFG_SOCKETS_RECEIVE_BUFFER_SIZE  (8192)

#define WIFI_CFG_AT_CMD_TX_BUFFER_SIZE        (512)

#define WIFI_CFG_AT_CMD_RX_BUFFER_SIZE        (3000)

#define WIFI_CFG_USE_CALLBACK_FUNCTION        (0)

#if WIFI_CFG_USE_CALLBACK_FUNCTION == 1

#define WIFI_CFG_CALLBACK_FUNCTION_NAME       (NULL)

#endif

#define WIFI_CFG_COUNTRY_CODE                 "VN"

#define WIFI_CFG_MAX_SSID_LEN                 32

#define WIFI_CFG_MAX_BSSID_LEN                6

#define WIFI_CFG_MAX_PASS_LEN                 32

#define WIFI_CFG_SNTP_ENABLE                  0

#if WIFI_CFG_SNTP_ENABLE == 1

#define WIFI_CFG_SNTP_SERVER_IP               "0.0.0.0"

#define WIFI_CFG_SNTP_UTC_OFFSET              7

#endif

#define WIFI_CFG_USE_FREERTOS_LOGGING         0

#define WIFI_CFG_DEBUG_LOG                    3

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
