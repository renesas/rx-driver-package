/***********************************************************************************************************************
* Copyright [2020-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
*
* This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
* of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
* sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
* of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
* right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
* reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
* IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
* PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
* DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
* EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
* (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
* WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
* OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
* OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
**********************************************************************************************************************/

/* Guards against multiple inclusion */
#ifndef R_RRH46410_CONFIG_H
#define R_RRH46410_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_RRH46410_CFG_PARAM_CHECKING_ENABLE             (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY NUMBER OF DEVICES */
#define RM_RRH46410_CFG_DEVICE_NUM_MAX                    (1) /* Number of RRH46410 Sensors */

/* SPECIFY OPERATION MODE FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_OPERATION_MODE            (1) /* Set operation mode of RRH46410 Sensor device0 */
                                                            /*
                                                            1 : IAQ 2nd Gen.
                                                            2 : IAQ 2nd Gen. Ultra-Low Power
                                                            5 : PBAQ
                                                             */

/* SPECIFY USING COMMUNICATION LINE INSTANCE FOP DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_COMMS_INSTANCE            (g_comms_i2c_device0)

/* SPECIFY I2C CALLBACK FUNCTION NAME FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_COMMS_I2C_CALLBACK        (rrh46410_user_i2c_callback0) /* Enable user I2C callback for RRH46410 Sensor Device0 */

/* SPECIFY IRQ ENABLED FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_IRQ_ENABLE                (0)

/* SPECIFY IRQ CALLBACK FUNCTION NAME FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_IRQ_CALLBACK              (rrh46410_user_irq_callback0) /* Enable user IRQ callback for RRH46410 Sensor Device0 */

/* SPECIFY IRQ NUMBER FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_IRQ_NUMBER                (IRQ_NUM_13)

/* SPECIFY IRQ TRIGGER FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_IRQ_TRIGGER               (IRQ_TRIG_RISING)

/* SPECIFY IRQ PRIORITY FOR DEVICE0 */
#define RM_RRH46410_CFG_DEVICE0_IRQ_PRIORITY              (IRQ_PRI_2)

#if RM_RRH46410_CFG_DEVICE0_IRQ_ENABLE
#define RM_RRH46410_CFG_IRQ_ENABLE (1)
#else
#define RM_RRH46410_CFG_IRQ_ENABLE (0)
#endif

 #endif  /* R_RRH46410_CONFIG_H */
