/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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
 #include "r_ob1203_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
extern void rm_ob1203_irq_callback (external_irq_callback_args_t * p_args);
extern void rm_ob1203_comms_i2c_callback (rm_comms_callback_args_t * p_args);
/* OB1203 Device0 */
#if (1 <= RM_OB1203_CFG_DEVICE_NUM_MAX)
void rm_ob1203_callback0(rm_comms_callback_args_t * p_args);
void RM_OB1203_CFG_DEVICE0_COMMS_I2C_CALLBACK(rm_ob1203_callback_args_t * p_args);
#if RM_OB1203_CFG_DEVICE0_IRQ_ENABLE
void rm_ob1203_irq_callback0(void * pargs);
void RM_OB1203_CFG_DEVICE0_IRQ_CALLBACK(rm_ob1203_callback_args_t * p_args); 
#endif
#endif

/* OB1203 Device1 */
#if (2 <= RM_OB1203_CFG_DEVICE_NUM_MAX)
void rm_ob1203_callback1(rm_comms_callback_args_t * p_args);
void RM_OB1203_CFG_DEVICE1_COMMS_I2C_CALLBACK(rm_ob1203_callback_args_t * p_args);
#if RM_OB1203_CFG_DEVICE1_IRQ_ENABLE
void rm_ob1203_irq_callback1(void * pargs);
void RM_OB1203_CFG_DEVICE1_IRQ_CALLBACK(rm_ob1203_callback_args_t * p_args); 
#endif
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* OB1203 Device0 */
#if (1 <= RM_OB1203_CFG_DEVICE_NUM_MAX)
/* IRQ */
#if RM_OB1203_CFG_DEVICE0_IRQ_ENABLE
irq_handle_t g_ob1203_sensor0_irq_handle;
external_irq_instance_t g_ob1203_sensor0_irq_instance = 
{
    .number     = RM_OB1203_CFG_DEVICE0_IRQ_NUMBER,
    .trigger    = RM_OB1203_CFG_DEVICE0_IRQ_TRIGGER,
    .priority   = RM_OB1203_CFG_DEVICE0_IRQ_PRIORITY,
    .p_handle   = &g_ob1203_sensor0_irq_handle,
    .p_callback = rm_ob1203_irq_callback0,
};
#endif

/* OB1203 */
#if BSP_CFG_RTOS
#if BSP_CFG_RTOS == 1
TX_SEMAPHORE g_ob1203_sensor0_semaphore_handle;
CHAR g_ob1203_sensor0_semaphore_name[] = "g_ob1203_sensor0 semaphore";
#elif BSP_CFG_RTOS == 2
SemaphoreHandle_t g_ob1203_sensor0_semaphore_handle;
#endif

/* Semaphore to wait for callback  */
rm_ob1203_semaphore_t g_ob1203_sensor0_semaphore =
{
    .p_semaphore_handle = &g_ob1203_sensor0_semaphore_handle,
#if BSP_CFG_RTOS == 1 // ThreadX
    .p_semaphore_name = &g_ob1203_sensor0_semaphore_name[0],
#elif BSP_CFG_RTOS == 2 // FreeRTOS
#endif
};
#endif
rm_ob1203_instance_ctrl_t     g_ob1203_sensor0_ctrl;
#if RM_OB1203_LIGHT_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 1)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_light;
#elif RM_OB1203_PROXIMITY_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 2)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_proximity;
#elif RM_OB1203_LIGHT_PROX_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 3)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_light_proximity;
#elif RM_OB1203_PPG_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 4)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_ppg;
#endif
rm_ob1203_mode_extended_cfg_t g_ob1203_sensor0_extended_cfg =
{
#if RM_OB1203_CFG_DEVICE0_IRQ_ENABLE && RM_OB1203_LIGHT_PROX_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 3)
    .mode_irq = RM_OB1203_CFG_DEVICE0_LIGHT_PROX_DEVICE_INTERRUPT,
#endif
#if RM_OB1203_LIGHT_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 1)
    .p_api = (void *)&g_ob1203_on_ob1203_light,
#elif RM_OB1203_PROXIMITY_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 2)
    .p_api = (void *)&g_ob1203_on_ob1203_proximity,
#elif RM_OB1203_LIGHT_PROX_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 3)
    .p_api = (void *)&g_ob1203_on_ob1203_light_proximity,
#elif RM_OB1203_PPG_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 4)
    .p_api = (void *)&g_ob1203_on_ob1203_ppg,
#endif
    .ppg_prox_gain = RM_OB1203_CFG_DEVICE0_PPG_PROX_GAIN,
    .led_order = RM_OB1203_CFG_DEVICE0_LED_ORDER,
#if (RM_OB1203_LIGHT_MODE_ENABLE || RM_OB1203_LIGHT_PROX_MODE_ENABLE) && ((RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 1) || (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 3))
    .light_sensor_mode = RM_OB1203_CFG_DEVICE0_LIGHT_SENSOR_MODE,
    .light_interrupt_type = RM_OB1203_CFG_DEVICE0_LIGHT_INTERRUPT_TYPE,
    .light_interrupt_source = RM_OB1203_CFG_DEVICE0_LIGHT_INTERRUPT_SOURCE,
    .light_interrupt_persist = RM_OB1203_CFG_DEVICE0_LIGHT_INTERRUPT_PERSIST,
    .light_sleep = RM_OB1203_CFG_DEVICE0_LIGHT_SLEEP_AFTER_INTERRUPT,
    .light_gain = RM_OB1203_CFG_DEVICE0_LIGHT_GAIN,
    .light_upper_threshold = RM_OB1203_CFG_DEVICE0_LIGHT_UPPER_THRESHOLD,
    .light_lower_threshold = RM_OB1203_CFG_DEVICE0_LIGHT_LOWER_THRESHOLD,
    .light_variance_threshold = RM_OB1203_CFG_DEVICE0_LIGHT_VARIANCE_THRESHOLD,
    .light_resolution_period = RM_OB1203_CFG_DEVICE0_LIGHT_RESOLUTION_PERIOD,
#endif
#if (RM_OB1203_PROXIMITY_MODE_ENABLE || RM_OB1203_LIGHT_PROX_MODE_ENABLE) && ((RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 2) || (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 3))
    .prox_sleep = RM_OB1203_CFG_DEVICE0_PROX_SLEEP_AFTER_INTERRUPT,
    .prox_interrupt_type = RM_OB1203_CFG_DEVICE0_PROX_INTERRUPT_TYPE,
    .prox_interrupt_persist = RM_OB1203_CFG_DEVICE0_PROX_INTERRUPT_PERSIST,
    .prox_led_current = RM_OB1203_CFG_DEVICE0_PROX_LED_CURRENT,
    .prox_ana_can = RM_OB1203_CFG_DEVICE0_PROX_ANA_CAN,
    .prox_dig_can = RM_OB1203_CFG_DEVICE0_PROX_DIG_CAN,
    .prox_num_led_pulses = RM_OB1203_CFG_DEVICE0_PROX_NUM_LED_PULSES,
    .prox_upper_threshold = RM_OB1203_CFG_DEVICE0_PROX_UPPER_THRESHOLD,
    .prox_lower_threshold = RM_OB1203_CFG_DEVICE0_PROX_LOWER_THRESHOLD,
    .prox_width_period = RM_OB1203_CFG_DEVICE0_PROX_WIDTH_PERIOD,
    .prox_moving_average = RM_OB1203_CFG_DEVICE0_PROX_MOVING_AVERAGE,
    .prox_hysteresis = RM_OB1203_CFG_DEVICE0_PROX_HYSTERESIS,
#endif
#if RM_OB1203_PPG_MODE_ENABLE && (RM_OB1203_CFG_DEVICE0_SENSOR_MODE == 4)
    .ppg_sensor_mode = RM_OB1203_CFG_DEVICE0_PPG_SENSOR_MODE,
    .ppg_interrupt_type = RM_OB1203_CFG_DEVICE0_PPG_INTERRUPT_TYPE,
    .ppg_ir_led_current = RM_OB1203_CFG_DEVICE0_PPG_IR_LED_CURRENT,
    .ppg_red_led_current = RM_OB1203_CFG_DEVICE0_PPG_RED_LED_CURRENT,
    .ppg_power_save_mode = RM_OB1203_CFG_DEVICE0_PPG_POWER_SAVE_MODE,
    .ppg_ir_led_ana_can = RM_OB1203_CFG_DEVICE0_PPG_IR_LED_ANA_CAN,
    .ppg_red_led_ana_can = RM_OB1203_CFG_DEVICE0_PPG_RED_LED_ANA_CAN,
    .ppg_num_averaged_samples = RM_OB1203_CFG_DEVICE0_PPG_NUM_AVERAGED_SAMPLES,
    .ppg_width_period = RM_OB1203_CFG_DEVICE0_PPG_WIDTH_PERIOD,
    .ppg_fifo_rollover = RM_OB1203_CFG_DEVICE0_PPG_FIFO_ROLLOVER,
    .ppg_fifo_empty_num = RM_OB1203_CFG_DEVICE0_PPG_FIFO_EMPTY_NUM,
#endif
};
const rm_ob1203_cfg_t g_ob1203_sensor0_cfg =
{
#if BSP_CFG_RTOS
    .p_semaphore = &g_ob1203_sensor0_semaphore,
#endif
    .semaphore_timeout = RM_OB1203_CFG_DEVICE0_SEMAPHORE_TIMEOUT,
    .p_comms_instance = &(RM_OB1203_CFG_DEVICE0_COMMS_INSTANCE),
#if (0 == RM_OB1203_CFG_DEVICE0_IRQ_ENABLE)
    .p_irq_instance = NULL,
    .p_irq_callback = NULL,
#else
    .p_irq_instance = &g_ob1203_sensor0_irq_instance,
    .p_irq_callback = (void (*)(rm_ob1203_callback_args_t *))RM_OB1203_CFG_DEVICE0_IRQ_CALLBACK,
#endif
    .p_comms_callback = (void (*)(rm_ob1203_callback_args_t *))RM_OB1203_CFG_DEVICE0_COMMS_I2C_CALLBACK,
    .p_context  = NULL,
    .p_extend = (void *)&g_ob1203_sensor0_extended_cfg,
};
const rm_ob1203_instance_t g_ob1203_sensor0 =
{
    .p_ctrl = &g_ob1203_sensor0_ctrl,
    .p_cfg  = &g_ob1203_sensor0_cfg,
    .p_api  = &g_ob1203_on_ob1203,
};

void rm_ob1203_callback0(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_ob1203_sensor0_ctrl;
    rm_ob1203_comms_i2c_callback(p_args);
}

void rm_ob1203_irq_callback0(void * pargs)
{
    external_irq_callback_args_t args;
    args.p_context = (void *)&g_ob1203_sensor0_ctrl;
    rm_ob1203_irq_callback(&args);
}
#endif

/* OB1203 Device1 */
#if (2 <= RM_OB1203_CFG_DEVICE_NUM_MAX)
/* IRQ */
#if RM_OB1203_CFG_DEVICE1_IRQ_ENABLE
irq_handle_t g_ob1203_sensor1_irq_handle;
external_irq_instance_t g_ob1203_sensor1_irq_instance = 
{
    .number     = RM_OB1203_CFG_DEVICE1_IRQ_NUMBER,
    .trigger    = RM_OB1203_CFG_DEVICE1_IRQ_TRIGGER,
    .priority   = RM_OB1203_CFG_DEVICE1_IRQ_PRIORITY,
    .p_handle   = &g_ob1203_sensor1_irq_handle,
    .p_callback = rm_ob1203_irq_callback1,
};
#endif

/* OB1203 */
#if BSP_CFG_RTOS
#if BSP_CFG_RTOS == 1
TX_SEMAPHORE g_ob1203_sensor1_semaphore_handle;
CHAR g_ob1203_sensor1_semaphore_name[] = "g_ob1203_sensor1 semaphore";
#elif BSP_CFG_RTOS == 2
SemaphoreHandle_t g_ob1203_sensor1_semaphore_handle;
#endif

/* Semaphore to wait for callback  */
rm_ob1203_semaphore_t g_ob1203_sensor1_semaphore =
{
    .p_semaphore_handle = &g_ob1203_sensor1_semaphore_handle,
#if BSP_CFG_RTOS == 1 // ThreadX
    .p_semaphore_name = &g_ob1203_sensor1_semaphore_name[0],
#elif BSP_CFG_RTOS == 2 // FreeRTOS
#endif
};
#endif
rm_ob1203_instance_ctrl_t     g_ob1203_sensor1_ctrl;
#if RM_OB1203_LIGHT_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 1)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_light;
#elif RM_OB1203_PROXIMITY_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 2)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_proximity;
#elif RM_OB1203_LIGHT_PROX_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 3)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_light_proximity;
#elif RM_OB1203_PPG_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 4)
extern rm_ob1203_api_t const  g_ob1203_on_ob1203_ppg;
#endif
rm_ob1203_mode_extended_cfg_t g_ob1203_sensor1_extended_cfg =
{
#if RM_OB1203_CFG_DEVICE1_IRQ_ENABLE && RM_OB1203_LIGHT_PROX_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 3)
    .mode_irq = RM_OB1203_CFG_DEVICE1_LIGHT_PROX_DEVICE_INTERRUPT,
#endif
#if RM_OB1203_LIGHT_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 1)
    .p_api = (void *)&g_ob1203_on_ob1203_light,
#elif RM_OB1203_PROXIMITY_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 2)
    .p_api = (void *)&g_ob1203_on_ob1203_proximity,
#elif RM_OB1203_LIGHT_PROX_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 3)
    .p_api = (void *)&g_ob1203_on_ob1203_light_proximity,
#elif RM_OB1203_PPG_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 4)
    .p_api = (void *)&g_ob1203_on_ob1203_ppg,
#endif
    .ppg_prox_gain = RM_OB1203_CFG_DEVICE1_PPG_PROX_GAIN,
    .led_order = RM_OB1203_CFG_DEVICE1_LED_ORDER,
#if (RM_OB1203_LIGHT_MODE_ENABLE || RM_OB1203_LIGHT_PROX_MODE_ENABLE) && ((RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 1) || (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 3))
    .light_sensor_mode = RM_OB1203_CFG_DEVICE1_LIGHT_SENSOR_MODE,
    .light_interrupt_type = RM_OB1203_CFG_DEVICE1_LIGHT_INTERRUPT_TYPE,
    .light_interrupt_source = RM_OB1203_CFG_DEVICE1_LIGHT_INTERRUPT_SOURCE,
    .light_interrupt_persist = RM_OB1203_CFG_DEVICE1_LIGHT_INTERRUPT_PERSIST,
    .light_sleep = RM_OB1203_CFG_DEVICE1_LIGHT_SLEEP_AFTER_INTERRUPT,
    .light_gain = RM_OB1203_CFG_DEVICE1_LIGHT_GAIN,
    .light_upper_threshold = RM_OB1203_CFG_DEVICE1_LIGHT_UPPER_THRESHOLD,
    .light_lower_threshold = RM_OB1203_CFG_DEVICE1_LIGHT_LOWER_THRESHOLD,
    .light_variance_threshold = RM_OB1203_CFG_DEVICE1_LIGHT_VARIANCE_THRESHOLD,
    .light_resolution_period = RM_OB1203_CFG_DEVICE1_LIGHT_RESOLUTION_PERIOD,
#endif
#if (RM_OB1203_PROXIMITY_MODE_ENABLE || RM_OB1203_LIGHT_PROX_MODE_ENABLE) && ((RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 2) || (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 3))
    .prox_sleep = RM_OB1203_CFG_DEVICE1_PROX_SLEEP_AFTER_INTERRUPT,
    .prox_interrupt_type = RM_OB1203_CFG_DEVICE1_PROX_INTERRUPT_TYPE,
    .prox_interrupt_persist = RM_OB1203_CFG_DEVICE1_PROX_INTERRUPT_PERSIST,
    .prox_led_current = RM_OB1203_CFG_DEVICE1_PROX_LED_CURRENT,
    .prox_ana_can = RM_OB1203_CFG_DEVICE1_PROX_ANA_CAN,
    .prox_dig_can = RM_OB1203_CFG_DEVICE1_PROX_DIG_CAN,
    .prox_num_led_pulses = RM_OB1203_CFG_DEVICE1_PROX_NUM_LED_PULSES,
    .prox_upper_threshold = RM_OB1203_CFG_DEVICE1_PROX_UPPER_THRESHOLD,
    .prox_lower_threshold = RM_OB1203_CFG_DEVICE1_PROX_LOWER_THRESHOLD,
    .prox_width_period = RM_OB1203_CFG_DEVICE1_PROX_WIDTH_PERIOD,
    .prox_moving_average = RM_OB1203_CFG_DEVICE1_PROX_MOVING_AVERAGE,
    .prox_hysteresis = RM_OB1203_CFG_DEVICE1_PROX_HYSTERESIS,
#endif
#if RM_OB1203_PPG_MODE_ENABLE && (RM_OB1203_CFG_DEVICE1_SENSOR_MODE == 4)
    .ppg_sensor_mode = RM_OB1203_CFG_DEVICE1_PPG_SENSOR_MODE,
    .ppg_interrupt_type = RM_OB1203_CFG_DEVICE1_PPG_INTERRUPT_TYPE,
    .ppg_ir_led_current = RM_OB1203_CFG_DEVICE1_PPG_IR_LED_CURRENT,
    .ppg_red_led_current = RM_OB1203_CFG_DEVICE1_PPG_RED_LED_CURRENT,
    .ppg_power_save_mode = RM_OB1203_CFG_DEVICE1_PPG_POWER_SAVE_MODE,
    .ppg_ir_led_ana_can = RM_OB1203_CFG_DEVICE1_PPG_IR_LED_ANA_CAN,
    .ppg_red_led_ana_can = RM_OB1203_CFG_DEVICE1_PPG_RED_LED_ANA_CAN,
    .ppg_num_averaged_samples = RM_OB1203_CFG_DEVICE1_PPG_NUM_AVERAGED_SAMPLES,
    .ppg_width_period = RM_OB1203_CFG_DEVICE1_PPG_WIDTH_PERIOD,
    .ppg_fifo_rollover = RM_OB1203_CFG_DEVICE1_PPG_FIFO_ROLLOVER,
    .ppg_fifo_empty_num = RM_OB1203_CFG_DEVICE1_PPG_FIFO_EMPTY_NUM,
#endif
};
const rm_ob1203_cfg_t g_ob1203_sensor1_cfg =
{
#if BSP_CFG_RTOS
    .p_semaphore = &g_ob1203_sensor1_semaphore,
#endif
    .semaphore_timeout = RM_OB1203_CFG_DEVICE1_SEMAPHORE_TIMEOUT,
    .p_comms_instance = &(RM_OB1203_CFG_DEVICE1_COMMS_INSTANCE),
#if (0 == RM_OB1203_CFG_DEVICE1_IRQ_ENABLE)
    .p_irq_instance = NULL,
    .p_irq_callback = NULL,
#else
    .p_irq_instance = &g_ob1203_sensor1_irq_instance,
    .p_irq_callback = (void (*)(rm_ob1203_callback_args_t *))RM_OB1203_CFG_DEVICE1_IRQ_CALLBACK,
#endif
    .p_comms_callback = (void (*)(rm_ob1203_callback_args_t *))RM_OB1203_CFG_DEVICE1_COMMS_I2C_CALLBACK,
    .p_context  = NULL,
    .p_extend = (void *)&g_ob1203_sensor1_extended_cfg,
};
const rm_ob1203_instance_t g_ob1203_sensor1 =
{
    .p_ctrl = &g_ob1203_sensor1_ctrl,
    .p_cfg  = &g_ob1203_sensor1_cfg,
    .p_api  = &g_ob1203_on_ob1203,
};

void rm_ob1203_callback1(rm_comms_callback_args_t * p_args)
{
    p_args->p_context = (void *)&g_ob1203_sensor1_ctrl;
    rm_ob1203_comms_i2c_callback(p_args);
}

void rm_ob1203_irq_callback1(void * pargs)
{
    external_irq_callback_args_t args;
    args.p_context = (void *)&g_ob1203_sensor1_ctrl;
    rm_ob1203_irq_callback(&args);
}
#endif

