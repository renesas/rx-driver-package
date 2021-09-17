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
#ifndef R_ZMOD4XXX_CONFIG_H
#define R_ZMOD4XXX_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_ZMOD4XXX_CFG_PARAM_CHECKING_ENABLE             (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY OPERATION MODE */
#define RM_ZMOD4XXX_CFG_OPERATION_MODE                    (1) /* Set operation mode of ZMOD4XXX Sensors */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_COMMS_INSTANCE            (g_comms_i2c_device0)

/* SPECIFY I2C CALLBACK FUNCTION NAME FOR DEVICE 0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_COMMS_I2C_CALLBACK        (zmod4xxx_user_i2c_callback0) /* Enable user I2C callback for ZMOD4XXX Sensor Device0 */

/* SPECIFY IRQ ENABLED FOR DEVICE 0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE                (1)

/* SPECIFY IRQ CALLBACK FUNCTION NAME FOR DEVICE 0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_CALLBACK              (zmod4xxx_user_irq_callback0) /* Enable user IRQ callback for ZMOD4XXX Sensor Device0 */

/* SPECIFY IRQ NUMBER FOR DEVICE 0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_NUMBER                (IRQ_NUM_13)

/* SPECIFY IRQ TRIGGER FOR DEVICE 0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_TRIGGER               (IRQ_TRIG_RISING)

/* SPECIFY IRQ PRIORITY FOR DEVICE 0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_PRIORITY              (IRQ_PRI_10)

#endif  /* R_ZMOD4XXX_CONFIG_H */
