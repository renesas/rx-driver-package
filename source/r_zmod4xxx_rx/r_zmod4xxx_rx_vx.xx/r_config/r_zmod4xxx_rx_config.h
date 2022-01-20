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

/* SPECIFY NUMBER OF DEVICES */
#define RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX                    (0) /* Number (max.) of ZMOD4xxx Sensors */

/* SPECIFY OPERATION MODE FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE            (1) /* Set operation mode of ZMOD4XXX Sensor device0 */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_COMMS_INSTANCE            (g_comms_i2c_device0)

/* SPECIFY I2C CALLBACK FUNCTION NAME FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_COMMS_I2C_CALLBACK        (zmod4xxx_user_i2c_callback0) /* Enable user I2C callback for ZMOD4XXX Sensor Device0 */

/* SPECIFY IRQ ENABLED FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE                (0)

/* SPECIFY IRQ CALLBACK FUNCTION NAME FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_CALLBACK              (zmod4xxx_user_irq_callback0) /* Enable user IRQ callback for ZMOD4XXX Sensor Device0 */

/* SPECIFY IRQ NUMBER FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_NUMBER                (IRQ_NUM_13)

/* SPECIFY IRQ TRIGGER FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_TRIGGER               (IRQ_TRIG_RISING)

/* SPECIFY IRQ PRIORITY FOR DEVICE0 */
#define RM_ZMOD4XXX_CFG_DEVICE0_IRQ_PRIORITY              (IRQ_PRI_10)

/* SPECIFY OPERATION MODE FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE            (0) /* Set operation mode of ZMOD4XXX Sensor device1 */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_COMMS_INSTANCE            (g_comms_i2c_device1)

/* SPECIFY I2C CALLBACK FUNCTION NAME FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_COMMS_I2C_CALLBACK        (zmod4xxx_user_i2c_callback1) /* Enable user I2C callback for ZMOD4XXX Sensor device1 */

/* SPECIFY IRQ ENABLED FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_IRQ_ENABLE                (0)

/* SPECIFY IRQ CALLBACK FUNCTION NAME FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_IRQ_CALLBACK              (zmod4xxx_user_irq_callback1) /* Enable user IRQ callback for ZMOD4XXX Sensor device1 */

/* SPECIFY IRQ NUMBER FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_IRQ_NUMBER                (IRQ_NUM_10)

/* SPECIFY IRQ TRIGGER FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_IRQ_TRIGGER               (IRQ_TRIG_RISING)

/* SPECIFY IRQ PRIORITY FOR DEVICE1 */
#define RM_ZMOD4XXX_CFG_DEVICE1_IRQ_PRIORITY              (IRQ_PRI_10)

#if RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE || RM_ZMOD4XXX_CFG_DEVICE1_IRQ_ENABLE
#define RM_ZMOD4XXX_CFG_IRQ_ENABLE (1)
#else
#define RM_ZMOD4XXX_CFG_IRQ_ENABLE (0)
#endif

#if (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 1) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 1) \
||  (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 2) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 2)
#define RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE (1)
#else
#define RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE (0)
#endif

#if (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 3) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 3)
#define RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE (1)
#else
#define RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE (0)
#endif

#if (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 4) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 4)
#define RM_ZMOD4410_ODOR_CFG_LIB_ENABLE (1)
#else
#define RM_ZMOD4410_ODOR_CFG_LIB_ENABLE (0)
#endif

#if (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 5) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 5)
#define RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE (1)
#else
#define RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE (0)
#endif

#if (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 6) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 6)
#define RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE (1)
#else
#define RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE (0)
#endif

#if (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 7) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 7)
#define RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE (1)
#else
#define RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE (0)
#endif

#endif  /* R_ZMOD4XXX_CONFIG_H */
