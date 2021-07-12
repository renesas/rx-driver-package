/* Generated configuration header file - do not edit */
/* ${REA_DISCLAIMER_PLACEHOLDER} */
/***********************************************************************************************************************
* File Name    : r_hs300x_rx_config.h
* Device(s)    : RX
* Description  : The file r_hs300x_rx_config.h is used to configure your HS300X. r_hs300x_rx_config.h should be included
*                somewhere in your package so that the r_bsp code has access to it. This file (r_bsp_config_reference.h)
*                is just a reference file that the user can use to make their own r_hs300x_rx_config.h file.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 30.09.2020 1.00    First Release
***********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef R_HS300X_CONFIG_H
#define R_HS300X_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_HS300X_CFG_PARAM_CHECKING_ENABLE             (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY NUMBER OF DEVICES */
#define RM_HS300X_CFG_DEVICE_NUM_MAX                    (1) /* Number (max.) of HS300x Sensors */

/* SPECIFY DATA TYPE */
#define RM_HS300X_CFG_DATA_BOTH_HUMIDITY_TEMPERATURE    (1) /* Getting data type of HS300x Sensors */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE0 */
#define RM_HS300X_CFG_DEVICE0_COMMS_INSTANCE            (g_comms_i2c_device0)

/* SPECIFY CALLBACK ENABLE FOR DEVICE 0 */
#define RM_HS300X_CFG_DEVICE0_CALLBACK_ENABLE           (1) /* Enable user callback for HS300x Sensor Device0 */

/* SPECIFY CALLBACK FUNCTION NAME FOR DEVICE 0 */
#define RM_HS300X_CFG_DEVICE0_CALLBACK                  (hs300x_user_callback0) /* Enable user callback for HS300x Sensor Device0 */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE1 */
#define RM_HS300X_CFG_DEVICE1_COMMS_INSTANCE            (g_comms_i2c_device1)

/* SPECIFY CALLBACK ENABLE FOR DEVICE 1 */
#define RM_HS300X_CFG_DEVICE1_CALLBACK_ENABLE           (0) /* Enable user callback for HS300x Sensor Device0 */

/* SPECIFY CALLBACK FUNCTION NAME FOR DEVICE 1 */
#define RM_HS300X_CFG_DEVICE1_CALLBACK                  (hs300x_user_callback1) /* Enable user callback for HS300x Sensor Device0 */

#endif  /* R_HS300X_CONFIG_H */
