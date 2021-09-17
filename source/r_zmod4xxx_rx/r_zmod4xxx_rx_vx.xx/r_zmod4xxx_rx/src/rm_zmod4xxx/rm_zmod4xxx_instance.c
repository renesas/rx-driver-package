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
 #include "r_zmod4xxx_if.h"
 #if (RM_ZMOD4XXX_CFG_OPERATION_MODE == 1) || (RM_ZMOD4XXX_CFG_OPERATION_MODE == 2)
#include "iaq_1st_gen.h"
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 3)
#include "iaq_2nd_gen.h"
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 4)
#include "odor.h"
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 5)
#include "sulfur_odor.h"
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 6)
#include "oaq_1st_gen.h"
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 7)
#include "oaq_2nd_gen.h"
#else
#endif
#include "zmod4xxx_types.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
void rm_zmod4xxx_callback0(rm_comms_callback_args_t * p_args);
void RM_ZMOD4XXX_CFG_DEVICE0_COMMS_I2C_CALLBACK(rm_zmod4xxx_callback_args_t * p_args);

/* Timer */
void rm_zmod4xxx_timer_callback0(void * pdata);
/* IRQ */
#if (1 == RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE)
void rm_zmod4xxx_irq_callback0(void * pargs);
void RM_ZMOD4XXX_CFG_DEVICE0_IRQ_CALLBACK(rm_zmod4xxx_callback_args_t * p_args); 
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* Timer */
extern void rm_zmod4xxx_timer_callback (timer_callback_args_t * p_args);

timer_instance_t g_zmod4xxx_sensor0_timer_instance = 
{
    .channel    = 0,
    .p_callback = rm_zmod4xxx_timer_callback0,    
};

/* IRQ */
extern void rm_zmod4xxx_irq_callback (external_irq_callback_args_t * p_args);
#if (1 == RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE)
irq_handle_t g_zmod4xxx_sensor0_irq_handle;
external_irq_instance_t g_zmod4xxx_sensor0_irq_instance = 
{
    .number     = RM_ZMOD4XXX_CFG_DEVICE0_IRQ_NUMBER,
    .trigger    = RM_ZMOD4XXX_CFG_DEVICE0_IRQ_TRIGGER,
    .priority   = RM_ZMOD4XXX_CFG_DEVICE0_IRQ_PRIORITY,
    .p_handle   = &g_zmod4xxx_sensor0_irq_handle,
    .p_callback = rm_zmod4xxx_irq_callback0,
};
#endif

/* COMMS I2C */
extern void rm_zmod4xxx_comms_i2c_callback (rm_comms_callback_args_t * p_args);
rm_zmod4xxx_instance_ctrl_t g_zmod4xxx_sensor0_ctrl;
zmod4xxx_dev_t         g_zmod4xxx_sensor0_dev;
#if (RM_ZMOD4XXX_CFG_OPERATION_MODE == 1) || (RM_ZMOD4XXX_CFG_OPERATION_MODE == 2)
iaq_1st_gen_handle_t   g_zmod4xxx_sensor0_lib_handle;
iaq_1st_gen_results_t  g_zmod4xxx_sensor0_lib_results;
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 3)
iaq_2nd_gen_handle_t   g_zmod4xxx_sensor0_lib_handle;
iaq_2nd_gen_results_t  g_zmod4xxx_sensor0_lib_results;
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 4)
odor_params            g_zmod4xxx_sensor0_lib_handle;
odor_results_t         g_zmod4xxx_sensor0_lib_results;
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 5)
sulfur_odor_handle_t   g_zmod4xxx_sensor0_lib_handle;
sulfur_odor_results_t  g_zmod4xxx_sensor0_lib_results;
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 6)
oaq_base_handle_t      g_zmod4xxx_sensor0_lib_handle;
uint8_t                g_zmod4xxx_sensor0_lib_results;
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 7)
oaq_2nd_gen_handle_t   g_zmod4xxx_sensor0_lib_handle;
oaq_2nd_gen_results_t  g_zmod4xxx_sensor0_lib_results;
#else
#endif
const rm_zmod4xxx_cfg_t g_zmod4xxx_sensor0_cfg =
{
    .p_comms_instance = &(RM_ZMOD4XXX_CFG_DEVICE0_COMMS_INSTANCE),
    .p_timer_instance = &g_zmod4xxx_sensor0_timer_instance,
#if (0 == RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE)
    .p_irq_instance = NULL,
    .p_irq_callback = NULL,
#else
    .p_irq_instance = &g_zmod4xxx_sensor0_irq_instance,
    .p_irq_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_ZMOD4XXX_CFG_DEVICE0_IRQ_CALLBACK,
#endif
    .p_comms_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_ZMOD4XXX_CFG_DEVICE0_COMMS_I2C_CALLBACK,
    .p_zmod4xxx_device = (void *)&g_zmod4xxx_sensor0_dev,
    .p_zmod4xxx_handle = (void *)&g_zmod4xxx_sensor0_lib_handle,
    .p_zmod4xxx_results = (void *)&g_zmod4xxx_sensor0_lib_results,
    .p_context  = NULL,
};
const rm_zmod4xxx_instance_t g_zmod4xxx_sensor0 =
{
    .p_ctrl = &g_zmod4xxx_sensor0_ctrl,
    .p_cfg  = &g_zmod4xxx_sensor0_cfg,
    .p_api  = &g_zmod4xxx_on_zmod4xxx,
};
void rm_zmod4xxx_callback0(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_zmod4xxx_sensor0_ctrl;
    rm_zmod4xxx_comms_i2c_callback(p_args);
}

void rm_zmod4xxx_timer_callback0(void * pdata)
{
    timer_callback_args_t args;
    args.p_context = (void *)&g_zmod4xxx_sensor0_ctrl;
    rm_zmod4xxx_timer_callback(&args);
}

void rm_zmod4xxx_irq_callback0(void * pargs)
{
    external_irq_callback_args_t args;
    args.p_context = (void *)&g_zmod4xxx_sensor0_ctrl;
    rm_zmod4xxx_irq_callback(&args);
}


