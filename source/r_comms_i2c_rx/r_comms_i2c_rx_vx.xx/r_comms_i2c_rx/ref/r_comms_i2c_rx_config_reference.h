/* Generated configuration header file - do not edit */
/* ${REA_DISCLAIMER_PLACEHOLDER} */
/***********************************************************************************************************************
* File Name    : r_comms_i2c_rx_config.h
* Device(s)    : RX
* Description  : The file r_comms_i2c_rx_config.h is used to configure your HS300x. r_comms_i2c_rx_config.h should be
*                included somewhere in your package so that the r_bsp code has access to it.
*                This file (r_bsp_config_reference.h) is just a reference file that the user can use to make their own
*                r_comms_i2c_rx_config.h file.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 30.09.2020 1.00    First Release
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

/* SPECIFY NUMBER OF DEVICES */
#define COMMS_I2C_CFG_DEVICE_NUM_MAX        (2)         /* Number (max.) of I2C devices */

/* SPECIFY BLOCKING OPERATION IN THE RTOS */
/* Setting to 1 enable blocking operation */
/* Setting to 0 disable blocking operation */
#define COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE  (0) /* Blocking operation */

/* SPECIFY BUS LOCK OPERATION IN THE RTOS */
/* Setting to 1 enable bus lock operation */
/* Setting to 0 disable bus lock operation */
#define COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE  (0) /* Bus lock operation */

/* SPECIFY DRIVER TYPE, CHANNEL NO. AND SLAVE ADDRESS EACH DEVICE */
/* For Bus No.0 */
#define COMMS_I2C_CFG_BUS0_DRIVER_TYPE      (COMMS_DRIVER_SCI_I2C) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS0_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS0_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_BUS0_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_BUS0_CALLBACK_ENABLE  (0) /* Callback function */
#define COMMS_I2C_CFG_BUS0_CALLBACK         (NULL) /* Callback function */
#define COMMS_I2C_CFG_BUS0_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */
#define COMMS_I2C_CFG_BUS0_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.1 */
#define COMMS_I2C_CFG_BUS1_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS1_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS1_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_BUS1_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_BUS1_CALLBACK_ENABLE  (0) /* Callback function */
#define COMMS_I2C_CFG_BUS1_CALLBACK         (NULL) /* Callback function */
#define COMMS_I2C_CFG_BUS1_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */
#define COMMS_I2C_CFG_BUS1_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.2 */
#define COMMS_I2C_CFG_BUS2_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS2_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS2_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_BUS2_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_BUS2_CALLBACK_ENABLE  (0) /* Callback function */
#define COMMS_I2C_CFG_BUS2_CALLBACK         (NULL) /* Callback function */
#define COMMS_I2C_CFG_BUS2_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */
#define COMMS_I2C_CFG_BUS2_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.3 */
#define COMMS_I2C_CFG_BUS3_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS3_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS3_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_BUS3_ADDR_MODE        (1) /* Address mode */
#define COMMS_I2C_CFG_BUS3_CALLBACK_ENABLE  (0) /* Callback function */
#define COMMS_I2C_CFG_BUS3_CALLBACK         (NULL) /* Callback function */
#define COMMS_I2C_CFG_BUS3_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */
#define COMMS_I2C_CFG_BUS3_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

/* For Bus No.4 */
#define COMMS_I2C_CFG_BUS4_DRIVER_TYPE      (COMMS_DRIVER_NONE) /* Driver type of I2C Bus */
#define COMMS_I2C_CFG_BUS4_DRIVER_CH        (0) /* Channel No. */
#define COMMS_I2C_CFG_BUS4_SLAVE_ADDR       (0x00) /* Slave address */
#define COMMS_I2C_CFG_BUS4_ADDR_MODE        (1)    /* Address mode */
#define COMMS_I2C_CFG_BUS4_CALLBACK_ENABLE  (0) /* Callback function */
#define COMMS_I2C_CFG_BUS4_CALLBACK         (NULL) /* Callback function */
#define COMMS_I2C_CFG_BUS4_BLOCKING_TIMEOUT (0xFFFFFFFF) /* Blocking timeout in the RTOS  (0x00 ~ 0xFFFFFFFF) */
#define COMMS_I2C_CFG_BUS4_TIMEOUT          (0xFFFFFFFF) /* Bus timeout in the RTOS (0x00 ~ 0xFFFFFFFF) */

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
