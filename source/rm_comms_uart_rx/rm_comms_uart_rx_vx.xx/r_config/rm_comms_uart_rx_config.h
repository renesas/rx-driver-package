/* Generated configuration header file - do not edit */
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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef R_COMMS_UART_CONFIG_H
#define R_COMMS_UART_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_COMMS_UART_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY NUMBER OF BUSES */
#define COMMS_UART_CFG_BUS_NUM_MAX           (1)         /* Number (max.) of UART buses */

/* SPECIFY NUMBER OF DEVICES */
#define COMMS_UART_CFG_DEVICE_NUM_MAX        (1)         /* Number (max.) of UART devices */ 

/* SPECIFY BLOCKING OPERATION IN THE RTOS */
/* Setting to 1 enable blocking operation */
/* Setting to 0 disable blocking operation */
#define COMMS_UART_CFG_RTOS_TX_BLOCKING_SUPPORT_ENABLE  (1) /* Blocking operation */

/* SPECIFY BUS LOCK OPERATION IN THE RTOS */
/* Setting to 1 enable bus lock operation */
/* Setting to 0 disable bus lock operation */
#define COMMS_UART_CFG_RTOS_TX_BUS_LOCK_SUPPORT_ENABLE  (1) /* Bus lock operation */

/* SPECIFY BLOCKING OPERATION IN THE RTOS */
/* Setting to 1 enable blocking operation */
/* Setting to 0 disable blocking operation */
#define COMMS_UART_CFG_RTOS_RX_BLOCKING_SUPPORT_ENABLE  (1) /* Blocking operation */

/* SPECIFY BUS LOCK OPERATION IN THE RTOS */
/* Setting to 1 enable bus lock operation */
/* Setting to 0 disable bus lock operation */
#define COMMS_UART_CFG_RTOS_RX_BUS_LOCK_SUPPORT_ENABLE  (1) /* Bus lock operation */

/* SPECIFY DRIVER TYPE, CHANNEL NO. */
/* For Bus No.0 */
#define COMMS_UART_CFG_BUS0_SCI_UART_COMPONENT  Config_SCI0
#define COMMS_UART_CFG_BUS0_SCI_UART_CH         0
#define COMMS_UART_CFG_BUS0_CLK_SRC             (SCI_CLK_INT)
#define COMMS_UART_CFG_BUS0_DATA_SIZE           (SCI_DATA_8BIT)
#define COMMS_UART_CFG_BUS0_PARITY_EN           (SCI_PARITY_OFF)
#define COMMS_UART_CFG_BUS0_PARITY_TYPE         (SCI_EVEN_PARITY)
#define COMMS_UART_CFG_BUS0_STOP_BITS           (SCI_STOPBITS_1)
#define COMMS_UART_CFG_BUS0_PCLK                (60)
#define COMMS_UART_CFG_BUS0_RATE                (115200)
#define COMMS_UART_CFG_BUS0_TIMEOUT             (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* SPECIFY CALLBACK FUNCTION EACH DEVICE */
/* For Device No.0 */
#define COMMS_UART_CFG_DEVICE0_BUS_CH           (g_comms_uart_bus0_extended_cfg)
#define COMMS_UART_CFG_DEVICE0_CALLBACK_ENABLE  (1) /* Callback function */
#define COMMS_UART_CFG_DEVICE0_CALLBACK         (rm_comms_uart_callback0) /* Callback function */
#define COMMS_UART_CFG_DEVICE0_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */

#endif  /* R_COMMS_UART_CONFIG_H */
