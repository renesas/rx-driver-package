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

/* SPECIFY WHETHER TO USE UART TRANSPORT LAYER INTERFACE */
/* Setting an equate to 1 includes code specific to that transport layer interface. */
#define BLE_CFG_TRANSPORT_INTERFACE_UART     1

/* SCI channel for DA14531 GTL command communication
Board dependent settings; please use the value for each setting listed below depending on the board you use.
1: RX65N Cloud Kit (PMOD)    - SCI Channel 0
2: RX65N Envision Kit (PMOD) - SCI Channel 2
3: RX65N RSK (2MB) (PMOD1)   - SCI Channel 6
4: RX65N RSK (2MB) (PMOD2)   - SCI Channel 1
5: CK-RX65N v1 (PMOD1)       - SCI Channel 6
6: CK-RX65N v1 (PMOD2)       - SCI Channel 0
7: RX671 Target Board (PMOD) - SCI Channel 5
8: RX66N Target Board (PMOD) - SCI Channel 5
*/
#define BLE_CFG_SCI_CHANNEL                         (6)

/* Interrupt Level for BLE_CFG_SCI_CHANNEL */
#define BLE_CFG_SCI_INTERRUPT_LEVEL                 (3)

/* Configure SCK Pin
Board dependent settings; please use the value for each setting listed below depending on the board you use.
1: RX65N Cloud Kit (PMOD)    - P22 (Port 2, Pin 2)
2: RX65N Envision Kit (PMOD) - P51 (Port 5, Pin 1)
3: RX65N RSK (2MB) (PMOD1)   - P02 (Port 0, Pin 2)
4: RX65N RSK (2MB) (PMOD2)   - PF1 (Port F, Pin 1)
5: CK-RX65N v1 (PMOD1)       - P02 (Port 0, Pin 2)
6: CK-RX65N v1 (PMOD2)       - P34 (Port 3, Pin 4)
7: RX671 Target Board (PMOD) - PC3 (Port C, Pin 3)
8: RX66N Target Board (PMOD) - PC3 (Port C, Pin 3)
 */
#define BLE_CFG_SCK_PORT                            0
#define BLE_CFG_SCK_PIN                             2

/* Configure reset pin
Board dependent settings; please use the value for each setting listed below depending on the board you use.
1: RX65N Cloud Kit (PMOD)    - PD0 (Port D, Pin 0)
2: RX65N Envision Kit (PMOD) - P55 (Port 5, Pin 5)
3: RX65N RSK (2MB) (PMOD1)   - PF5 (Port F, Pin 5)
4: RX65N RSK (2MB) (PMOD2)   - P20 (Port 2, Pin 0)
5: CK-RX65N v1 (PMOD1)       - P55 (Port 5, Pin 5)
6: CK-RX65N v1 (PMOD2)       - PA1 (Port A, Pin 1)
7: RX671 Target Board (PMOD) - PB1 (Port B, Pin 1)
8: RX66N Target Board (PMOD) - PB1 (Port B, Pin 1)
*/
#define BLE_CFG_RESET_PORT                          5
#define BLE_CFG_RESET_PIN                           5

/* Specify module reset polarity */
/* 1=Activa High, 0=Active Low */
#define BLE_CFG_RESET_POLARITY                      (0)

/* Boot SDK download from host MCU
0 = Disabled boot mode
1 = Boot SDK download from host MCU via 1-Wire UART
*/
#define BLE_CFG_HOST_BOOT_MODE                      (0)

/* Configure ABS Number Bonding */
#define BLE_CFG_ABS_NUMBER_BONDING                  (1)

/* Configure ABS  Timer number of slot */
#define BLE_CFG_ABS_TIMER_NUMBER_OF_SLOT            (10)

/* Configure ABS GATT MTU size */
#define BLE_CFG_ABS_GATT_MTU_SIZE                   (247)

/* Configure ABS RF connection maximum  */
#define BLE_CFG_ABS_RF_CONNECTION_MAXIMUM           (1)

/* Configure RF connnection maximum */
#define BLE_CFG_RF_CONN_MAX                         (1)

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
