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

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
 #include "r_comms_i2c_if.h"
 #include "r_rrh46410_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
extern void rm_rrh46410_irq_callback (external_irq_callback_args_t * p_args);
extern void rm_rrh46410_comms_i2c_callback (rm_comms_callback_args_t * p_args);
/* RRH46410 Device0 */
#if (1 <= RM_RRH46410_CFG_DEVICE_NUM_MAX)
void rm_rrh46410_callback0(rm_comms_callback_args_t * p_args);
void RM_RRH46410_CFG_DEVICE0_COMMS_I2C_CALLBACK(rm_zmod4xxx_callback_args_t * p_args);
#if RM_RRH46410_CFG_DEVICE0_IRQ_ENABLE
void rm_rrh46410_irq_callback0(void * pargs);
void RM_RRH46410_CFG_DEVICE0_IRQ_CALLBACK(rm_zmod4xxx_callback_args_t * p_args); 
#endif
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* RRH46410 Device0 */
#if (1 <= RM_RRH46410_CFG_DEVICE_NUM_MAX)
/* IRQ */
#if RM_RRH46410_CFG_DEVICE0_IRQ_ENABLE
irq_handle_t g_rrh46410_sensor0_irq_handle;
external_irq_instance_t g_rrh46410_sensor0_irq_instance = 
{
    .number     = RM_RRH46410_CFG_DEVICE0_IRQ_NUMBER,
    .trigger    = RM_RRH46410_CFG_DEVICE0_IRQ_TRIGGER,
    .priority   = RM_RRH46410_CFG_DEVICE0_IRQ_PRIORITY,
    .p_handle   = &g_rrh46410_sensor0_irq_handle,
    .p_callback = rm_rrh46410_irq_callback0,
};
#endif

/* COMMS I2C */
rm_rrh46410_instance_ctrl_t g_rrh46410_sensor0_ctrl;
const rm_zmod4xxx_cfg_t g_rrh46410_sensor0_cfg =
{
    .p_comms_instance = &(RM_RRH46410_CFG_DEVICE0_COMMS_INSTANCE),
#if (0 == RM_RRH46410_CFG_DEVICE0_IRQ_ENABLE)
    .p_irq_instance   = NULL,
    .p_irq_callback   = NULL,
#else
    .p_irq_instance   = &g_rrh46410_sensor0_irq_instance,
    .p_irq_callback   = (void (*)(rm_zmod4xxx_callback_args_t *))RM_RRH46410_CFG_DEVICE0_IRQ_CALLBACK,
#endif
    .p_comms_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_RRH46410_CFG_DEVICE0_COMMS_I2C_CALLBACK,
    .p_context        = NULL,
};
const rm_zmod4xxx_instance_t g_rrh46410_sensor0 =
{
    .p_ctrl = &g_rrh46410_sensor0_ctrl,
    .p_cfg  = &g_rrh46410_sensor0_cfg,
    .p_api  = &g_zmod4xxx_on_rrh46410,
};

void rm_rrh46410_callback0(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_rrh46410_sensor0_ctrl;
    rm_rrh46410_comms_i2c_callback(p_args);
}

void rm_rrh46410_irq_callback0(void * pargs)
{
    external_irq_callback_args_t args;
    args.p_context = (void *)&g_rrh46410_sensor0_ctrl;
    rm_rrh46410_irq_callback(&args);
}
#endif