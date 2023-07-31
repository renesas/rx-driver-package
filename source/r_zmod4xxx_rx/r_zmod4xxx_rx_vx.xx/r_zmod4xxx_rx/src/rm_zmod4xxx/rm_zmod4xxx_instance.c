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

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
 #include "r_comms_i2c_if.h"
 #include "r_zmod4xxx_if.h"
 #include "zmod4xxx_types.h"
#if RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE
#include "iaq_1st_gen/iaq_1st_gen.h"
#endif
#if RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE
#include "iaq_2nd_gen/iaq_2nd_gen.h"
#endif
#if RM_ZMOD4410_IAQ_2ND_GEN_ULP_CFG_LIB_ENABLE
#include "iaq_2nd_gen_ulp/iaq_2nd_gen_ulp.h"
#endif
#if RM_ZMOD4410_ODOR_CFG_LIB_ENABLE
#include "odor/odor.h"
#endif
#if RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE
#include "sulfur_odor/sulfur_odor.h"
#endif
#if RM_ZMOD4410_REL_IAQ_CFG_LIB_ENABLE
#include "rel_iaq/rel_iaq.h"
#endif
#if RM_ZMOD4410_REL_IAQ_ULP_CFG_LIB_ENABLE
#include "rel_iaq_ulp/rel_iaq_ulp.h"
#endif
#if RM_ZMOD4410_PBAQ_CFG_LIB_ENABLE
#include "pbaq/pbaq.h"
#endif
#if RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE
#include "oaq_1st_gen/oaq_1st_gen.h"
#endif
#if RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE
#include "oaq_2nd_gen/oaq_2nd_gen.h"
#endif
#if RM_ZMOD4450_RAQ_CFG_LIB_ENABLE
#include "raq/raq.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
extern void rm_zmod4xxx_irq_callback (external_irq_callback_args_t * p_args);
extern void rm_zmod4xxx_comms_i2c_callback (rm_comms_callback_args_t * p_args);
/* ZMOD4XXX Device0 */
#if (1 <= RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX)
void rm_zmod4xxx_callback0(rm_comms_callback_args_t * p_args);
void RM_ZMOD4XXX_CFG_DEVICE0_COMMS_I2C_CALLBACK(rm_zmod4xxx_callback_args_t * p_args);
#if RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE
void rm_zmod4xxx_irq_callback0(void * pargs);
void RM_ZMOD4XXX_CFG_DEVICE0_IRQ_CALLBACK(rm_zmod4xxx_callback_args_t * p_args); 
#endif
#endif

/* ZMOD4XXX Device1 */
#if (2 <= RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX)
void rm_zmod4xxx_callback1(rm_comms_callback_args_t * p_args);
void RM_ZMOD4XXX_CFG_DEVICE1_COMMS_I2C_CALLBACK(rm_zmod4xxx_callback_args_t * p_args);
#if RM_ZMOD4XXX_CFG_DEVICE1_IRQ_ENABLE
void rm_zmod4xxx_irq_callback1(void * pargs);
void RM_ZMOD4XXX_CFG_DEVICE1_IRQ_CALLBACK(rm_zmod4xxx_callback_args_t * p_args); 
#endif
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* ZMOD4XXX Device0 */
#if (1 <= RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX)
/* IRQ */
#if RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE
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
rm_zmod4xxx_instance_ctrl_t     g_zmod4xxx_sensor0_ctrl;
zmod4xxx_dev_t                  g_zmod4xxx_sensor0_dev;
#if RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE && \
((RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 1) || (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 2)) 
iaq_1st_gen_handle_t            g_zmod4xxx_sensor0_lib_handle;
iaq_1st_gen_results_t           g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[6];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_iaq_1st_gen;
extern zmod4xxx_conf            g_zmod4410_iaq_1st_gen_sensor_type[];
#elif RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 3)
iaq_2nd_gen_handle_t            g_zmod4xxx_sensor0_lib_handle;
iaq_2nd_gen_results_t           g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_iaq_2nd_gen;
extern zmod4xxx_conf            g_zmod4410_iaq_2nd_gen_sensor_type[];
#elif RM_ZMOD4410_IAQ_2ND_GEN_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 8)
iaq_2nd_gen_ulp_handle_t            g_zmod4xxx_sensor0_lib_handle;
iaq_2nd_gen_ulp_results_t           g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_iaq_2nd_gen_ulp;
extern zmod4xxx_conf            g_zmod4410_iaq_2nd_gen_ulp_sensor_type[];
#elif RM_ZMOD4410_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 4)
odor_params                     g_zmod4xxx_sensor0_lib_handle;
odor_results_t                  g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[6];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_odor;
extern zmod4xxx_conf            g_zmod4410_odor_sensor_type[];
#elif RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 5)
sulfur_odor_handle_t            g_zmod4xxx_sensor0_lib_handle;
sulfur_odor_results_t           g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_sulfur_odor;
extern zmod4xxx_conf            g_zmod4410_sulfur_odor_sensor_type[];
#elif RM_ZMOD4410_REL_IAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 10)
rel_iaq_handle_t                g_zmod4xxx_sensor0_lib_handle;
rel_iaq_results_t               g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_rel_iaq;
extern zmod4xxx_conf            g_zmod4410_rel_iaq_sensor_type[];
#elif RM_ZMOD4410_REL_IAQ_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 11)
rel_iaq_ulp_handle_t            g_zmod4xxx_sensor0_lib_handle;
rel_iaq_ulp_results_t           g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_rel_iaq_ulp;
extern zmod4xxx_conf            g_zmod4410_rel_iaq_ulp_sensor_type[];
#elif RM_ZMOD4410_PBAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 12)
pbaq_handle_t                   g_zmod4xxx_sensor0_lib_handle;
pbaq_results_t                  g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_pbaq;
extern zmod4xxx_conf            g_zmod4410_pbaq_sensor_type[];
#elif RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 6)
oaq_base_handle_t               g_zmod4xxx_sensor0_lib_handle;
uint8_t                         g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[9];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4510_oaq_1st_gen;
extern zmod4xxx_conf            g_zmod4510_oaq_1st_gen_sensor_type[];
#elif RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 7)
oaq_2nd_gen_handle_t            g_zmod4xxx_sensor0_lib_handle;
oaq_2nd_gen_results_t           g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[10];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4510_oaq_2nd_gen;
extern zmod4xxx_conf            g_zmod4510_oaq_2nd_gen_sensor_type[];
#elif RM_ZMOD4450_RAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 9)
raq_params_t                    g_zmod4xxx_sensor0_lib_handle;
raq_results_t                   g_zmod4xxx_sensor0_lib_results;
uint8_t                         g_zmod4xxx_sensor0_product_data[5];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4450_raq;
extern zmod4xxx_conf            g_zmod4450_raq_sensor_type[];
#else
uint8_t                         g_zmod4xxx_sensor0_product_data[1]; // Dummy
uint8_t                         g_zmod4xxx_sensor0_lib_handle;      // Dummy
uint8_t                         g_zmod4xxx_sensor0_lib_results;     // Dummy
#endif
rm_zmod4xxx_lib_extended_cfg_t g_zmod4xxx_sensor0_extended_cfg =
{
#if RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 1)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_1ST_GEN_CONTINUOUS,
    .sample_period = 2.0F,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_1st_gen,
    .p_data_set = (void *)g_zmod4410_iaq_1st_gen_sensor_type,
#elif RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 2)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_1ST_GEN_LOW_POWER,
    .sample_period = 6.0F,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_1st_gen,
    .p_data_set = (void *)g_zmod4410_iaq_1st_gen_sensor_type,
#elif RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 3)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_2ND_GEN,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_2nd_gen,
    .p_data_set = (void *)g_zmod4410_iaq_2nd_gen_sensor_type,
#elif RM_ZMOD4410_IAQ_2ND_GEN_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 8)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_2ND_GEN_ULP,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_2nd_gen_ulp,
    .p_data_set = (void *)g_zmod4410_iaq_2nd_gen_ulp_sensor_type,
#elif RM_ZMOD4410_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 4)
    .lib_type = RM_ZMOD4410_LIB_TYPE_ODOR,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_odor,
    .p_data_set = (void *)g_zmod4410_odor_sensor_type,
#elif RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 5)
    .lib_type = RM_ZMOD4410_LIB_TYPE_SULFUR_ODOR,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_sulfur_odor,
    .p_data_set = (void *)g_zmod4410_sulfur_odor_sensor_type,
#elif RM_ZMOD4410_REL_IAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 10)
    .lib_type = RM_ZMOD4410_LIB_TYPE_REL_IAQ,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_rel_iaq,
    .p_data_set = (void *)g_zmod4410_rel_iaq_sensor_type,
#elif RM_ZMOD4410_REL_IAQ_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 11)
    .lib_type = RM_ZMOD4410_LIB_TYPE_REL_IAQ_ULP,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_rel_iaq_ulp,
    .p_data_set = (void *)g_zmod4410_rel_iaq_ulp_sensor_type,
#elif RM_ZMOD4410_PBAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 12)
    .lib_type = RM_ZMOD4410_LIB_TYPE_PBAQ,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_pbaq,
    .p_data_set = (void *)g_zmod4410_pbaq_sensor_type,
#elif RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 6)
    .lib_type = RM_ZMOD4510_LIB_TYPE_OAQ_1ST_GEN,
    .product_id = 0x6320,
    .p_api = (void *)&g_zmod4xxx_on_zmod4510_oaq_1st_gen,
    .p_data_set = (void *)g_zmod4510_oaq_1st_gen_sensor_type,
#elif RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 7)
    .lib_type = RM_ZMOD4510_LIB_TYPE_OAQ_2ND_GEN,
    .product_id = 0x6320,
    .p_api = (void *)&g_zmod4xxx_on_zmod4510_oaq_2nd_gen,
    .p_data_set = (void *)g_zmod4510_oaq_2nd_gen_sensor_type,
#elif RM_ZMOD4450_RAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE0_OPERATION_MODE == 9)
    .lib_type = RM_ZMOD4450_LIB_TYPE_RAQ,
    .product_id = 0x7310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4450_raq,
    .p_data_set = (void *)g_zmod4450_raq_sensor_type,
#else
#endif
    .p_product_data = g_zmod4xxx_sensor0_product_data,
    .p_device = (void *)&g_zmod4xxx_sensor0_dev,
    .p_handle = (void *)&g_zmod4xxx_sensor0_lib_handle,
    .p_results = (void *)&g_zmod4xxx_sensor0_lib_results,
};
const rm_zmod4xxx_cfg_t g_zmod4xxx_sensor0_cfg =
{
    .p_comms_instance = &(RM_ZMOD4XXX_CFG_DEVICE0_COMMS_INSTANCE),
#if (0 == RM_ZMOD4XXX_CFG_DEVICE0_IRQ_ENABLE)
    .p_irq_instance = NULL,
    .p_irq_callback = NULL,
#else
    .p_irq_instance = &g_zmod4xxx_sensor0_irq_instance,
    .p_irq_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_ZMOD4XXX_CFG_DEVICE0_IRQ_CALLBACK,
#endif
    .p_comms_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_ZMOD4XXX_CFG_DEVICE0_COMMS_I2C_CALLBACK,
    .p_context  = NULL,
    .p_extend = (void *)&g_zmod4xxx_sensor0_extended_cfg,
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

void rm_zmod4xxx_irq_callback0(void * pargs)
{
    external_irq_callback_args_t args;
    args.p_context = (void *)&g_zmod4xxx_sensor0_ctrl;
    rm_zmod4xxx_irq_callback(&args);
}
#endif

/* ZMOD4XXX Device1 */
#if (2 <= RM_ZMOD4XXX_CFG_DEVICE_NUM_MAX)
/* IRQ */
#if RM_ZMOD4XXX_CFG_DEVICE1_IRQ_ENABLE
irq_handle_t g_zmod4xxx_sensor1_irq_handle;
external_irq_instance_t g_zmod4xxx_sensor1_irq_instance = 
{
    .number     = RM_ZMOD4XXX_CFG_DEVICE1_IRQ_NUMBER,
    .trigger    = RM_ZMOD4XXX_CFG_DEVICE1_IRQ_TRIGGER,
    .priority   = RM_ZMOD4XXX_CFG_DEVICE1_IRQ_PRIORITY,
    .p_handle   = &g_zmod4xxx_sensor1_irq_handle,
    .p_callback = rm_zmod4xxx_irq_callback1,
};
#endif

/* COMMS I2C */
rm_zmod4xxx_instance_ctrl_t     g_zmod4xxx_sensor1_ctrl;
zmod4xxx_dev_t                  g_zmod4xxx_sensor1_dev;
#if RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE && \
((RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 1) || (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 2)) 
iaq_1st_gen_handle_t            g_zmod4xxx_sensor1_lib_handle;
iaq_1st_gen_results_t           g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[6];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_iaq_1st_gen;
extern zmod4xxx_conf            g_zmod4410_iaq_1st_gen_sensor_type[];
#elif RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 3)
iaq_2nd_gen_handle_t            g_zmod4xxx_sensor1_lib_handle;
iaq_2nd_gen_results_t           g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_iaq_2nd_gen;
extern zmod4xxx_conf            g_zmod4410_iaq_2nd_gen_sensor_type[];
#elif RM_ZMOD4410_IAQ_2ND_GEN_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 8)
iaq_2nd_gen_ulp_handle_t            g_zmod4xxx_sensor1_lib_handle;
iaq_2nd_gen_ulp_results_t           g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_iaq_2nd_gen_ulp;
extern zmod4xxx_conf            g_zmod4410_iaq_2nd_gen_ulp_sensor_type[];
#elif RM_ZMOD4410_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 4)
odor_params                     g_zmod4xxx_sensor1_lib_handle;
odor_results_t                  g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[6];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_odor;
extern zmod4xxx_conf            g_zmod4410_odor_sensor_type[];
#elif RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 5)
sulfur_odor_handle_t            g_zmod4xxx_sensor1_lib_handle;
sulfur_odor_results_t           g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_sulfur_odor;
extern zmod4xxx_conf            g_zmod4410_sulfur_odor_sensor_type[];
#elif RM_ZMOD4410_REL_IAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 10)
rel_iaq_handle_t                g_zmod4xxx_sensor1_lib_handle;
rel_iaq_results_t               g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_rel_iaq;
extern zmod4xxx_conf            g_zmod4410_rel_iaq_sensor_type[];
#elif RM_ZMOD4410_REL_IAQ_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 11)
rel_iaq_ulp_handle_t            g_zmod4xxx_sensor1_lib_handle;
rel_iaq_ulp_results_t           g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_rel_iaq_ulp;
extern zmod4xxx_conf            g_zmod4410_rel_iaq_ulp_sensor_type[];
#elif RM_ZMOD4410_PBAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 12)
pbaq_handle_t                   g_zmod4xxx_sensor1_lib_handle;
pbaq_results_t                  g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[7];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4410_pbaq;
extern zmod4xxx_conf            g_zmod4410_pbaq_sensor_type[];
#elif RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 6)
oaq_base_handle_t               g_zmod4xxx_sensor1_lib_handle;
uint8_t                         g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[9];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4510_oaq_1st_gen;
extern zmod4xxx_conf            g_zmod4510_oaq_1st_gen_sensor_type[];
#elif RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 7)
oaq_2nd_gen_handle_t            g_zmod4xxx_sensor1_lib_handle;
oaq_2nd_gen_results_t           g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[10];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4510_oaq_2nd_gen;
extern zmod4xxx_conf            g_zmod4510_oaq_2nd_gen_sensor_type[];
#elif RM_ZMOD4450_RAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 9)
raq_params_t                    g_zmod4xxx_sensor1_lib_handle;
raq_results_t                   g_zmod4xxx_sensor1_lib_results;
uint8_t                         g_zmod4xxx_sensor1_product_data[5];
extern rm_zmod4xxx_api_t const  g_zmod4xxx_on_zmod4450_raq;
extern zmod4xxx_conf            g_zmod4450_raq_sensor_type[];
#else
uint8_t                         g_zmod4xxx_sensor1_product_data[1]; // Dummy
uint8_t                         g_zmod4xxx_sensor1_lib_handle;      // Dummy
uint8_t                         g_zmod4xxx_sensor1_lib_results;     // Dummy
#endif
rm_zmod4xxx_lib_extended_cfg_t g_zmod4xxx_sensor1_extended_cfg =
{
#if RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 1)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_1ST_GEN_CONTINUOUS,
    .sample_period = 2.0F,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_1st_gen,
    .p_data_set = (void *)g_zmod4410_iaq_1st_gen_sensor_type,
#elif RM_ZMOD4410_IAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 2)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_1ST_GEN_LOW_POWER,
    .sample_period = 6.0F,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_1st_gen,
    .p_data_set = (void *)g_zmod4410_iaq_1st_gen_sensor_type,
#elif RM_ZMOD4410_IAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 3)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_2ND_GEN,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_2nd_gen,
    .p_data_set = (void *)g_zmod4410_iaq_2nd_gen_sensor_type,
#elif RM_ZMOD4410_IAQ_2ND_GEN_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 8)
    .lib_type = RM_ZMOD4410_LIB_TYPE_IAQ_2ND_GEN_ULP,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_iaq_2nd_gen_ulp,
    .p_data_set = (void *)g_zmod4410_iaq_2nd_gen_ulp_sensor_type,
#elif RM_ZMOD4410_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 4)
    .lib_type = RM_ZMOD4410_LIB_TYPE_ODOR,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_odor,
    .p_data_set = (void *)g_zmod4410_odor_sensor_type,
#elif RM_ZMOD4410_SULFUR_ODOR_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 5)
    .lib_type = RM_ZMOD4410_LIB_TYPE_SULFUR_ODOR,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_sulfur_odor,
    .p_data_set = (void *)g_zmod4410_sulfur_odor_sensor_type,
#elif RM_ZMOD4410_REL_IAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 10)
    .lib_type = RM_ZMOD4410_LIB_TYPE_REL_IAQ,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_rel_iaq,
    .p_data_set = (void *)g_zmod4410_rel_iaq_sensor_type,
#elif RM_ZMOD4410_REL_IAQ_ULP_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 11)
    .lib_type = RM_ZMOD4410_LIB_TYPE_REL_IAQ_ULP,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_rel_iaq_ulp,
    .p_data_set = (void *)g_zmod4410_rel_iaq_ulp_sensor_type,
#elif RM_ZMOD4410_PBAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 12)
    .lib_type = RM_ZMOD4410_LIB_TYPE_PBAQ,
    .product_id = 0x2310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4410_pbaq,
    .p_data_set = (void *)g_zmod4410_pbaq_sensor_type,
#elif RM_ZMOD4510_OAQ_1ST_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 6)
    .lib_type = RM_ZMOD4510_LIB_TYPE_OAQ_1ST_GEN,
    .product_id = 0x6320,
    .p_api = (void *)&g_zmod4xxx_on_zmod4510_oaq_1st_gen,
    .p_data_set = (void *)g_zmod4510_oaq_1st_gen_sensor_type,
#elif RM_ZMOD4510_OAQ_2ND_GEN_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 7)
    .lib_type = RM_ZMOD4510_LIB_TYPE_OAQ_2ND_GEN,
    .product_id = 0x6320,
    .p_api = (void *)&g_zmod4xxx_on_zmod4510_oaq_2nd_gen,
    .p_data_set = (void *)g_zmod4510_oaq_2nd_gen_sensor_type,
#elif RM_ZMOD4450_RAQ_CFG_LIB_ENABLE && (RM_ZMOD4XXX_CFG_DEVICE1_OPERATION_MODE == 9)
    .lib_type = RM_ZMOD4450_LIB_TYPE_RAQ,
    .product_id = 0x7310,
    .p_api = (void *)&g_zmod4xxx_on_zmod4450_raq,
    .p_data_set = (void *)g_zmod4450_raq_sensor_type,
#else
#endif
    .p_product_data = g_zmod4xxx_sensor1_product_data,
    .p_device = (void *)&g_zmod4xxx_sensor1_dev,
    .p_handle = (void *)&g_zmod4xxx_sensor1_lib_handle,
    .p_results = (void *)&g_zmod4xxx_sensor1_lib_results,
};
const rm_zmod4xxx_cfg_t g_zmod4xxx_sensor1_cfg =
{
    .p_comms_instance = &(RM_ZMOD4XXX_CFG_DEVICE1_COMMS_INSTANCE),
#if (0 == RM_ZMOD4XXX_CFG_DEVICE1_IRQ_ENABLE)
    .p_irq_instance = NULL,
    .p_irq_callback = NULL,
#else
    .p_irq_instance = &g_zmod4xxx_sensor1_irq_instance,
    .p_irq_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_ZMOD4XXX_CFG_DEVICE1_IRQ_CALLBACK,
#endif
    .p_comms_callback = (void (*)(rm_zmod4xxx_callback_args_t *))RM_ZMOD4XXX_CFG_DEVICE1_COMMS_I2C_CALLBACK,
    .p_context  = NULL,
    .p_extend = (void *)&g_zmod4xxx_sensor1_extended_cfg,
};
const rm_zmod4xxx_instance_t g_zmod4xxx_sensor1 =
{
    .p_ctrl = &g_zmod4xxx_sensor1_ctrl,
    .p_cfg  = &g_zmod4xxx_sensor1_cfg,
    .p_api  = &g_zmod4xxx_on_zmod4xxx,
};

void rm_zmod4xxx_callback1(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_zmod4xxx_sensor1_ctrl;
    rm_zmod4xxx_comms_i2c_callback(p_args);
}

void rm_zmod4xxx_irq_callback1(void * pargs)
{
    external_irq_callback_args_t args;
    args.p_context = (void *)&g_zmod4xxx_sensor1_ctrl;
    rm_zmod4xxx_irq_callback(&args);
}
#endif

