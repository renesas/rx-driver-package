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
* Copyright (C) 2016-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx_config.h
* Description  : Configures the LVD FIT module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Deleted unnecessary header information.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*                                    Corrected file name.
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin.
***********************************************************************************************************************/
#ifndef LVD_CONFIG_HEADER_FILE
#define LVD_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Specifies whether to include parameter checking in the code.                     */
/* 0 : Omit parameter checking from the build.                                      */
/* 1 : Include parameter checking in the build.                                     */
/* BSP_CFG_PARAM_CHECKING_ENABLE: Use the system default setting. (default value)   */
/* Note: Code size can be reduced by excluding parameter checking from the build.   */
#define LVD_CFG_PARAM_CHECKING_ENABLE       (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Specifies whether to use the corresponding channel.                              */
/* 0: The corresponding channel is not used.                                        */
/* 1: The corresponding channel is used. (default value)                            */
#define LVD_CFG_CHANNEL_1_USED              (1)
#define LVD_CFG_CHANNEL_2_USED              (1)

/* Specifies the target to be monitored for each channel.                           */
/* 0: VCC (default value)                                                           */
/* 1: CMPA2 pin                                                                     */
#define LVD_CFG_VDET_TARGET_CHANNEL_1       (0)
#define LVD_CFG_VDET_TARGET_CHANNEL_2       (0)

/* Specifies the voltage detection level for each channel. Set an integer value     */
/* which expresses the number up to two decimal places.                             */
/* The default value is with reference to the hardware initial value, thus it       */
/* varies depending on the product used.                                            */
/* Example:                                                                         */
/* To set the voltage detection level to 3.00 V, specify '300'.                     */
/* To set the voltage detection level to 4.29 V, specify '429'.                     */
/* To set the voltage detection level to default value, specify '0xFFFF' .          */
#define LVD_CFG_VOLTAGE_LEVEL_CHANNEL_1     (0xFFFF)
#define LVD_CFG_VOLTAGE_LEVEL_CHANNEL_2     (0xFFFF)

/* Specifies enable/disable of the digital filter for each channel.                 */
/* 0: Digital filter is disabled. (default value)                                   */
/* 1: Digital filter is enabled.                                                    */
#define LVD_CFG_DIGITAL_FILTER_CHANNEL_1    (0)
#define LVD_CFG_DIGITAL_FILTER_CHANNEL_2    (0)

/* With the digital filter enabled, specifies the division ratio of LOCO as the     */
/* sampling clock applied to each channel. Set an integer value which expresses     */
/* the division ratio.                                                              */
/* The default value is with reference to the hardware initial value, thus it       */
/* varies depending on the product used.                                            */
/* Example:                                                                         */
/* To set the division ratio to divided-by-1, specify '1'.                          */
/* To set the division ratio to divided-by-4, specify '4'.                          */
/* To set the division ratio to default value, specify '0xFFFF' .                   */
#define LVD_CFG_SAMPLING_CLOCK_CHANNEL_1    (0xFFFF)
#define LVD_CFG_SAMPLING_CLOCK_CHANNEL_2    (0xFFFF)

/* Specifies processing upon voltage detection for each channel.                    */
/* 0: Reset                                                                         */
/* 1: Non-maskable interrupt (default value)                                        */
/* 2: Maskable interrupt                                                            */
/* 3: No processing                                                                 */
/* Note: Reset here indicates device reset. When reset is selected as processing, a */
/* reset occurs when a monitored voltage is below the voltage detection level. When */
/* reset is selected with this definition, the operation of the reset is not        */
/* dependent on the voltage detection condition.                                    */
#define LVD_CFG_ACTION_CHANNEL_1            (1)
#define LVD_CFG_ACTION_CHANNEL_2            (1)

/* Specifies the interrupt priority level for each channel, with maskable interrupt */
/* selected as processing.                                                          */
/* Set the level with an integer value; setting 1 means that the priority level is  */
/* the lowest level and 15 means the highest level.                                 */
/* (default value = 3)                                                              */
/* Example:                                                                         */
/* To set the priority level to 3, specify '3'.                                     */
/* To set the priority level to 15, specify '15'.                                   */
#define LVD_CFG_INT_PRIORITY_CHANNEL_1      (3)
#define LVD_CFG_INT_PRIORITY_CHANNEL_2      (3)

/* Specifies the reset negation timing for each channel, with reset selected as     */
/* processing.                                                                      */
/* 0: After a LVD reset, negation occurs when a certain period elapses after the    */
/*    monitored voltage goes above the voltage detection level.                     */
/* 1: Negation occurs when a certain period elapses after the LVD reset assertion.  */
/* Note: "a certain period" here means a wait time after a voltage monitoring       */
/* reset. Refer to the User's Manual: Hardware for details.                         */
#define LVD_CFG_STABILIZATION_CHANNEL_1     (0)
#define LVD_CFG_STABILIZATION_CHANNEL_2     (0)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* LVD_CONFIG_HEADER_FILE */

/* End of File */

