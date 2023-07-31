/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

#ifndef R_ZMOD4XXX_IF_H_
#define R_ZMOD4XXX_IF_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_zmod4xxx.h"
#include "r_zmod4xxx_rx_config.h"
#if RM_ZMOD4XXX_CFG_IRQ_ENABLE
#include "r_irq_rx_if.h"
#include "r_irq_rx_pinset.h"
#endif

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
/* IRQ */
#ifndef R_SENSOR_IRQ_DEFINE
#define R_SENSOR_IRQ_DEFINE
#if RM_ZMOD4XXX_CFG_IRQ_ENABLE
typedef struct st_external_irq_instance
{
    irq_number_t number;
    irq_trigger_t trigger;
    irq_prio_t priority;
    irq_handle_t * p_handle;
    void (* p_callback)(void *pargs);
} external_irq_instance_t;
#endif
typedef struct st_external_irq_callback_args
{
    void * p_context;
} external_irq_callback_args_t;
#endif /* R_SENSOR_IRQ_DEFINE */

 /**********************************************************************************************************************
 * Exported global functions
 *********************************************************************************************************************/
#if (1 <= RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX)
extern const rm_zmod4xxx_instance_t         g_zmod4xxx_sensor0;
extern       rm_zmod4xxx_lib_extended_cfg_t g_zmod4xxx_sensor0_extended_cfg;
#endif
#if (2 <= RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX)
extern const rm_zmod4xxx_instance_t         g_zmod4xxx_sensor1;
extern       rm_zmod4xxx_lib_extended_cfg_t g_zmod4xxx_sensor1_extended_cfg;
#endif

#endif /* R_ZMOD4XXX_IF_H_ */
