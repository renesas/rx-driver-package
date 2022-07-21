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
#ifndef R_FS1015_CONFIG_H
#define R_FS1015_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_FS1015_CFG_PARAM_CHECKING_ENABLE             (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY NUMBER OF DEVICES */
#define RM_FS1015_CFG_DEVICE_NUM_MAX                    (1) /* Number (max.) of FS1015 Sensors */

/* SPECIFY USING FS1015-1015 */
#define RM_FS1015_CFG_DEVICE_TYPE                       (0) /* Use FS1015-1015 device */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE0 */
#define RM_FS1015_CFG_DEVICE0_COMMS_INSTANCE            (g_comms_i2c_device0)

/* SPECIFY CALLBACK FUNCTION NAME FOR DEVICE 0 */
#define RM_FS1015_CFG_DEVICE0_CALLBACK                  (fs1015_user_callback) /* Enable user callback for FS1015 Sensor Device0 */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOR DEVICE1 */
#define RM_FS1015_CFG_DEVICE1_COMMS_INSTANCE            (g_comms_i2c_device1)

/* SPECIFY CALLBACK FUNCTION NAME FOR DEVICE 1 */
#define RM_FS1015_CFG_DEVICE1_CALLBACK                  (fs1015_user_callback1) /* Enable user callback for FS1015 Sensor Device0 */

#endif  /* R_FS1015_CONFIG_H */
