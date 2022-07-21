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

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
 #include "r_comms_i2c_if.h"
 #include "r_hs400x_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Definitions of HS400X measurement type */
#define RM_HS400X_HOLD_MEASUREMENT                       (1)
#define RM_HS400X_NO_HOLD_MEASUREMENT                    (2)
#define RM_HS400X_PERIODIC_MEASUREMENT                   (3)
/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (1 <= RM_HS400X_CFG_DEVICE_NUM_MAX)
void rm_hs400x_callback0(rm_comms_callback_args_t * p_args);
void RM_HS400X_CFG_DEVICE0_CALLBACK(rm_hs400x_callback_args_t * p_args);
#endif
#if (2 <= RM_HS400X_CFG_DEVICE_NUM_MAX)
void rm_hs400x_callback1(rm_comms_callback_args_t * p_args);
void RM_HS400X_CFG_DEVICE1_CALLBACK(rm_hs400x_callback_args_t * p_args);
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
extern void rm_hs400x_comms_i2c_callback (rm_comms_callback_args_t * p_args);
#if (1 <= RM_HS400X_CFG_DEVICE_NUM_MAX)
rm_hs400x_instance_ctrl_t g_hs400x_sensor0_ctrl;
const rm_hs400x_cfg_t g_hs400x_sensor0_cfg =
{
    .p_comms_instance = &(RM_HS400X_CFG_DEVICE0_COMMS_INSTANCE),
    .temperature_resolution = RM_HS400X_CFG_DEVICE0_TEMPERATURE_RESOLUTION,
    .humidity_resolution = RM_HS400X_CFG_DEVICE0_HUMIDITY_RESOLUTION,
#if RM_HS400X_PERIODIC_MEASUREMENT == RM_HS400X_CFG_MEASUREMENT_TYPE
    .frequency = RM_HS400X_CFG_DEVICE0_PERIODIC_MEASUREMENT_FREQUENCY,
#endif
    .p_comms_callback = (void (*)(rm_hs400x_callback_args_t *))RM_HS400X_CFG_DEVICE0_CALLBACK,
    .p_context  = NULL,
};

const rm_hs400x_instance_t g_hs400x_sensor0 =
{
    .p_ctrl = &g_hs400x_sensor0_ctrl,
    .p_cfg  = &g_hs400x_sensor0_cfg,
    .p_api  = &g_hs400x_on_hs400x,
};
void rm_hs400x_callback0(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_hs400x_sensor0_ctrl;
    rm_hs400x_comms_i2c_callback(p_args);
}
#endif
#if (2 <= RM_HS400X_CFG_DEVICE_NUM_MAX)
rm_hs400x_instance_ctrl_t g_hs400x_sensor1_ctrl;
const rm_hs400x_cfg_t g_hs400x_sensor1_cfg =
{
	.p_comms_instance = &(RM_HS400X_CFG_DEVICE1_COMMS_INSTANCE),
    .temperature_resolution = RM_HS400X_CFG_DEVICE1_TEMPERATURE_RESOLUTION,
    .humidity_resolution = RM_HS400X_CFG_DEVICE1_HUMIDITY_RESOLUTION,
#if RM_HS400X_PERIODIC_MEASUREMENT == RM_HS400X_CFG_MEASUREMENT_TYPE
    .frequency = RM_HS400X_CFG_DEVICE1_PERIODIC_MEASUREMENT_FREQUENCY,
#endif
    .p_comms_callback = (void (*)(rm_hs400x_callback_args_t *))RM_HS400X_CFG_DEVICE1_CALLBACK,
    .p_context  = NULL,
};
const rm_hs400x_instance_t g_hs400x_sensor1 =
{
    .p_ctrl = &g_hs400x_sensor1_ctrl,
    .p_cfg  = &g_hs400x_sensor1_cfg,
    .p_api  = &g_hs400x_on_hs400x,
};
void rm_hs400x_callback1(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_hs400x_sensor1_ctrl;
    rm_hs400x_comms_i2c_callback(p_args);
}
#endif
/**********************************************************************************************************************
 * End of function rm_hs4001_callback1
 *********************************************************************************************************************/


