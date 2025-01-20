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
 * File Name    : r_ble_da14531_config.h
 * Description  : DA14531 BLE driver Configuration.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_BLE_DA14531_CONFIG_H_
#define R_BLE_DA14531_CONFIG_H_

/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define BLE_CFG_PARAM_CHECKING_ENABLE        (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Interrupt Level for BLE_CFG_SCI_CHANNEL */
#define BLE_CFG_SCI_INTERRUPT_LEVEL                 (3)

/* SCI channel for DA14531 GTL command communication*/
#define BLE_CFG_SCI_CHANNEL                         (6)

/* Configure SCK */
#define BLE_CFG_SCK_PORT                            0
#define BLE_CFG_SCK_PIN                             2

/* Configure reset */
#define BLE_CFG_RESET_PORT                          5
#define BLE_CFG_RESET_PIN                           5

/* Board dependent settings; please use the value for each setting listed below depending on the board you use.

Preprocessors that define board dependent settings and the corresponding values to be set are as follows:
Confirmed board number    1      2      3      4      5      6      7      8
BLE_CFG_SCI_CHANNEL       0      2      6      1      6      0      5      5
BLE_CFG_SCK_PORT          2      5      0      F      0      3      C      C
BLE_CFG_SCK_PIN           2      1      2      1      2      4      3      3
BLE_CFG_RESET_PORT        D      5      F      2      5      A      B      B
BLE_CFG_RESET_PIN         0      5      5      0      5      1      1      1

where the confirmed board numbers listed in the first row above are as follows (*Note1):
1: RX65N Cloud Kit (PMOD)
2: RX65N Envision Kit (PMOD)
3: RX65N RSK (2MB) (PMOD1)
4: RX65N RSK (2MB) (PMOD2)
5: CK-RX65N v1 (PMOD1)
6: CK-RX65N v1 (PMOD2)
7: RX671 Target Board (PMOD)
8: RX66N Target Board (PMOD)
In the above preprocessor list, please use one of the values listed on the right side.
On the right side, each column corresponds to each confirmed board number.

Note1: List of board user's manual:
1: RX65N Cloud Kit:    https://www.renesas.com/us/en/document/mat/uses-manual-cloud-option-board
2: RX65N Envision Kit: https://www.renesas.com/us/en/document/mat/rx65n-envision-kit-users-manual-rev100
3: RX65N RSK (2MB):    https://www.renesas.com/us/en/document/mat/renesas-starter-kit-rx65n-2mb-users-manual
4: CK-RX65N v1:        https://www.renesas.com/kr/en/document/mat/ck-rx65n-v1-users-manual
5: RX671 Target Board: https://www.renesas.com/br/en/document/mat/target-board-rx671-users-manual-rev100
6: RX66N Target Board: https://www.renesas.com/us/en/document/mah/target-board-rx66n-users-manual-rev100

Note2:
When you use RX671 Target Board, you need pattern cut and so on to use SCI channel 5(TXD5/RXD5/CTS5) and GPIO(PC1).
Please refer to User's Manual: https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/rtk5rx6710c00000bj-target-board-rx671

Note3:
When you use RX66N Target Board, you need remodeling of the board to use SCI channel 5(TXD5/RXD5/CTS5) and GPIO(PC1).
Please refer to User's Manual: https://www.renesas.com/products/microcontrollers-microprocessors/rx-32-bit-performance-efficiency-mcus/rtk5rx66n0c00000bj-target-board-rx66n

*/

/* Specify module reset polarity */
/* 1=Activa High, 0=Active Low */
#define BLE_CFG_RESET_POLARITY                      (0)

/* Boot SDK download from host MCU
0 = Disabled boot mode
1 = Boot SDK download from host MCU via 1-Wire UART
2 = Boot SDK download from host MCU via 2-Wire UART (*Note4)
Note4: Only be used with the DA14535 device
*/
#define BLE_CFG_HOST_BOOT_MODE                      (0)

/* Option BLE DA1453x
DA14531 = DA14531 device is selected
DA14535 = DA14535 device is selected
*/
#define BLE_CFG_DA1453x_DEVICE (DA14531_DEVICE)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* R_BLE_DA14531_CONFIG_H_ */
