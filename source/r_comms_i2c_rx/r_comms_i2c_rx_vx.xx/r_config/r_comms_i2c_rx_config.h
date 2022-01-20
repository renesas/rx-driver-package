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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef R_COMMS_I2C_CONFIG_H
#define R_COMMS_I2C_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/
#define COMMS_DRIVER_NONE       (0)
#define COMMS_DRIVER_I2C        (1)
#define COMMS_DRIVER_SCI_I2C    (2)

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_COMMS_I2C_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY NUMBER OF BUSES */
#define COMMS_I2C_CFG_BUS_NUM_MAX           (1)         /* Number (max.) of I2C buses */

/* SPECIFY NUMBER OF DEVICES */
#define COMMS_I2C_CFG_DEVICE_NUM_MAX        (1)         /* Number (max.) of I2C devices */

/* SPECIFY BLOCKING OPERATION IN THE RTOS */
/* Setting to 1 enable blocking operation */
/* Setting to 0 disable blocking operation */
#define COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE  (0) /* Blocking operation */

/* SPECIFY BUS LOCK OPERATION IN THE RTOS */
/* Setting to 1 enable bus lock operation */
/* Setting to 0 disable bus lock operation */
#define COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE  (0) /* Bus lock operation */

/* SPECIFY DRIVER TYPE, CHANNEL NO. */
/* For Bus No.0 */
#define COMMS_I2C_CFG_BUS0_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS0_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS0_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.1 */
#define COMMS_I2C_CFG_BUS1_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS1_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS1_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.2 */
#define COMMS_I2C_CFG_BUS2_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS2_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS2_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.3 */
#define COMMS_I2C_CFG_BUS3_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS3_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS3_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.4 */
#define COMMS_I2C_CFG_BUS4_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS4_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS4_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* SPECIFY SLAVE ADDRESS AND CALLBACK FUNCTION EACH DEVICE */
/* For Device No.0 */
#define COMMS_I2C_CFG_DEVICE0_BUS_CH           (g_comms_i2c_bus0_extended_cfg)
#define COMMS_I2C_CFG_DEVICE0_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_DEVICE0_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_DEVICE0_CALLBACK         (comms_i2c_user_callback0) /* Callback function */
#define COMMS_I2C_CFG_DEVICE0_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */

/* For Device No.1 */
#define COMMS_I2C_CFG_DEVICE1_BUS_CH           (g_comms_i2c_bus0_extended_cfg)
#define COMMS_I2C_CFG_DEVICE1_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_DEVICE1_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_DEVICE1_CALLBACK         (comms_i2c_user_callback1) /* Callback function */
#define COMMS_I2C_CFG_DEVICE1_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */

/* For Device No.2 */
#define COMMS_I2C_CFG_DEVICE2_BUS_CH           (g_comms_i2c_bus0_extended_cfg)
#define COMMS_I2C_CFG_DEVICE2_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_DEVICE2_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_DEVICE2_CALLBACK         (comms_i2c_user_callback2) /* Callback function */
#define COMMS_I2C_CFG_DEVICE2_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */

/* For Device No.3 */
#define COMMS_I2C_CFG_DEVICE3_BUS_CH           (g_comms_i2c_bus0_extended_cfg)
#define COMMS_I2C_CFG_DEVICE3_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_DEVICE3_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_DEVICE3_CALLBACK         (comms_i2c_user_callback3) /* Callback function */
#define COMMS_I2C_CFG_DEVICE3_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */

/* For Device No.4 */
#define COMMS_I2C_CFG_DEVICE4_BUS_CH           (g_comms_i2c_bus0_extended_cfg)
#define COMMS_I2C_CFG_DEVICE4_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_DEVICE4_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_DEVICE4_CALLBACK         (comms_i2c_user_callback4) /* Callback function */
#define COMMS_I2C_CFG_DEVICE4_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */

#if (COMMS_I2C_CFG_BUS0_DRIVER_TYPE == COMMS_DRIVER_I2C ) \
||  (COMMS_I2C_CFG_BUS1_DRIVER_TYPE == COMMS_DRIVER_I2C ) \
||  (COMMS_I2C_CFG_BUS2_DRIVER_TYPE == COMMS_DRIVER_I2C ) \
||  (COMMS_I2C_CFG_BUS3_DRIVER_TYPE == COMMS_DRIVER_I2C ) \
||  (COMMS_I2C_CFG_BUS4_DRIVER_TYPE == COMMS_DRIVER_I2C )
#define COMMS_I2C_CFG_DRIVER_I2C            (1) /* 1: Used I2C */
#else
#define COMMS_I2C_CFG_DRIVER_I2C            (0) /* 0: Unused I2C*/
#endif
#if (COMMS_I2C_CFG_BUS0_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C ) \
||  (COMMS_I2C_CFG_BUS1_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C ) \
||  (COMMS_I2C_CFG_BUS2_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C ) \
||  (COMMS_I2C_CFG_BUS3_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C ) \
||  (COMMS_I2C_CFG_BUS4_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C )
#define COMMS_I2C_CFG_DRIVER_SCI_I2C        (1) /* 1: Used SCI I2C */
#else
#define COMMS_I2C_CFG_DRIVER_SCI_I2C        (0) /* 0: Unused SCI I2C */
#endif

#endif  /* R_COMM_I2C_DEVICE_CONFIG_H */
