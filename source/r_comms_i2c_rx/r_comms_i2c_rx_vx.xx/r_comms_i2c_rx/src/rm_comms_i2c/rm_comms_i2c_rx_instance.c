/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/******************************************************************************************************************//**
 * @file          rm_comms_i2c_rx_instance.c
 * Version      : 1.00
 * @brief         This source file includes the API functions, global variables and the sub-modules.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.09.2020  1.00    First Release
 *********************************************************************************************************************/

/******************************************************************************************************************//**
 * @addtogroup rm_comm_i2c_bus_rx_instance
 * @{
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_comms_i2c_if.h"

#if COMMS_I2C_CFG_DRIVER_I2C
#include "r_riic_rx_if.h"
#endif
#if COMMS_I2C_CFG_DRIVER_SCI_I2C
#include "r_sci_iic_rx_if.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void rm_comms_i2c_bus0_callback(void);
 #if (1 == COMMS_I2C_CFG_BUS0_CALLBACK_ENABLE)
 void COMMS_I2C_CFG_BUS0_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for bus0 */
 #endif
#endif
#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void rm_comms_i2c_bus1_callback(void);
 #if (1 == COMMS_I2C_CFG_BUS1_CALLBACK_ENABLE)
 void COMMS_I2C_CFG_BUS1_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for bus1 */
 #endif
#endif
#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void rm_comms_i2c_bus2_callback(void);
 #if (1 == COMMS_I2C_CFG_BUS2_CALLBACK_ENABLE)
 void COMMS_I2C_CFG_BUS2_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for bus2 */
 #endif
#endif
#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void rm_comms_i2c_bus3_callback(void);
 #if (1 == COMMS_I2C_CFG_BUS3_CALLBACK_ENABLE)
 void COMMS_I2C_CFG_BUS3_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for bus3 */
 #endif
#endif
#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void rm_comms_i2c_bus4_callback(void);
 #if (1 == COMMS_I2C_CFG_BUS4_CALLBACK_ENABLE)
 void COMMS_I2C_CFG_BUS4_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for bus4 */
 #endif
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl;
 #if BSP_CFG_RTOS == 1
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus0_blocking_semaphore_handle;
CHAR g_comms_i2c_bus0_blocking_semaphore_name[] = "g_comms_i2c_bus0 blocking semaphore";
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus0_recursive_mutex_handle;
CHAR g_comms_i2c_bus0_recursive_mutex_name[] = "g_comms_i2c_bus0 recursive mutex";
   #endif
  #endif
 #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus0_blocking_semaphore_handle;
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus0_recursive_mutex_handle;
   #endif
  #endif
 #endif
 #if BSP_CFG_RTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus0_blocking_semaphore =
{
 .p_semaphore_handle = &g_comms_i2c_bus0_blocking_semaphore_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
 .p_semaphore_name = &g_comms_i2c_bus0_blocking_semaphore_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
 .p_semaphore_memory = NULL,
   #endif
};
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus0_recursive_mutex =
{
 .p_mutex_handle = &g_comms_i2c_bus0_recursive_mutex_handle,
    #if BSP_CFG_RTOS == 1 // ThreadX
 .p_mutex_name = &g_comms_i2c_bus0_recursive_mutex_name[0],
    #elif BSP_CFG_RTOS == 2 // FreeRTOS
 .p_mutex_memory = NULL,
    #endif
};
   #endif
  #endif
 #endif

uint8_t g_comms_i2c_bus0_slave_address;
 #if COMMS_I2C_CFG_BUS0_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus0_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus0_slave_address,
};
 #elif COMMS_I2C_CFG_BUS0_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus0_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus0_slave_address,
};
 #endif
const i2c_driver_instance_t g_comms_i2c_bus0_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS0_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS0_DRIVER_CH,
	.callback       = rm_comms_i2c_bus0_callback,
 #if COMMS_I2C_CFG_BUS0_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus0_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus0_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus0_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS0_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus0_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus0_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
const rm_comms_i2c_device_cfg_t g_comms_i2c_device0_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_BUS0_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_BUS0_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device0_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_BUS0_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device0_lower_level_cfg,
    .p_extend          = (void *)&g_comms_i2c_bus0_extended_cfg,
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_BUS0_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device0 =
{
    .p_ctrl = &g_comms_i2c_device0_ctrl,
    .p_cfg  = &g_comms_i2c_device0_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device1_ctrl;
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus1_blocking_semaphore_handle;
CHAR g_comms_i2c_bus1_blocking_semaphore_name[] = "g_comms_i2c_bus1 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus1_recursive_mutex_handle;
CHAR g_comms_i2c_bus1_recursive_mutex_name[] = "g_comms_i2c_bus1 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus1_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus1_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus1_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus1_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus1_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_semaphore_memory = NULL,
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus1_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus1_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus1_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_mutex_memory = NULL,
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus1_slave_address;
 #if COMMS_I2C_CFG_BUS1_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus1_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus1_slave_address,
};
 #elif COMMS_I2C_CFG_BUS1_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus1_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus1_slave_address,
};
 #endif
const i2c_driver_instance_t g_comms_i2c_bus1_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS1_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS1_DRIVER_CH,
	.callback       = rm_comms_i2c_bus1_callback,
 #if COMMS_I2C_CFG_BUS1_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus1_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus1_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus1_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS1_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus1_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus1_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
const rm_comms_i2c_device_cfg_t g_comms_i2c_device1_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_BUS1_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_BUS1_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device1_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_BUS1_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device1_lower_level_cfg,
    .p_extend          = (void *)&g_comms_i2c_bus1_extended_cfg,
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_BUS1_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device1 =
{
    .p_ctrl = &g_comms_i2c_device1_ctrl,
    .p_cfg  = &g_comms_i2c_device1_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device2_ctrl;
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus2_blocking_semaphore_handle;
CHAR g_comms_i2c_bus2_blocking_semaphore_name[] = "g_comms_i2c_bus2 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus2_recursive_mutex_handle;
CHAR g_comms_i2c_bus2_recursive_mutex_name[] = "g_comms_i2c_bus2 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus2_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus2_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus2_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus2_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus2_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_semaphore_memory = NULL,
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus2_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus2_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus2_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_mutex_memory = NULL,
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus2_slave_address;
 #if COMMS_I2C_CFG_BUS2_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus2_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus2_slave_address,
};
 #elif COMMS_I2C_CFG_BUS2_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus2_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus2_slave_address,
};
 #endif
const i2c_driver_instance_t g_comms_i2c_bus2_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS2_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS2_DRIVER_CH,
	.callback       = rm_comms_i2c_bus2_callback,
 #if COMMS_I2C_CFG_BUS2_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus2_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus2_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus2_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS2_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus2_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus2_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
const rm_comms_i2c_device_cfg_t g_comms_i2c_device2_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_BUS2_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_BUS2_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device2_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_BUS2_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device2_lower_level_cfg,
    .p_extend          = (void *)&g_comms_i2c_bus2_extended_cfg,
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_BUS2_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device2 =
{
    .p_ctrl = &g_comms_i2c_device2_ctrl,
    .p_cfg  = &g_comms_i2c_device2_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device3_ctrl;
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus3_blocking_semaphore_handle;
CHAR g_comms_i2c_bus3_blocking_semaphore_name[] = "g_comms_i2c_bus3 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus3_recursive_mutex_handle;
CHAR g_comms_i2c_bus3_recursive_mutex_name[] = "g_comms_i2c_bus3 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus3_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus3_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus3_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus3_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus3_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_semaphore_memory = NULL,
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus3_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus3_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus3_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_mutex_memory = NULL,
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus3_slave_address;
 #if COMMS_I2C_CFG_BUS3_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus3_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus3_slave_address,
};
 #elif COMMS_I2C_CFG_BUS3_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus3_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus3_slave_address,
};
 #endif
const i2c_driver_instance_t g_comms_i2c_bus3_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS3_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS3_DRIVER_CH,
	.callback       = rm_comms_i2c_bus3_callback,
 #if COMMS_I2C_CFG_BUS3_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus3_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus3_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus3_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS3_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus3_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus3_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
const rm_comms_i2c_device_cfg_t g_comms_i2c_device3_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_BUS3_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_BUS3_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device3_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_BUS3_BLOCKING_TIMEOUT,
    .p_lower_level_cfg = (void *)&g_comms_i2c_device3_lower_level_cfg,
    .p_extend          = (void *)&g_comms_i2c_bus3_extended_cfg,
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_BUS3_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device3 =
{
    .p_ctrl = &g_comms_i2c_device3_ctrl,
    .p_cfg  = &g_comms_i2c_device3_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device4_ctrl;
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus4_blocking_semaphore_handle;
CHAR g_comms_i2c_bus4_blocking_semaphore_name[] = "g_comms_i2c_bus4 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus4_recursive_mutex_handle;
CHAR g_comms_i2c_bus4_recursive_mutex_name[] = "g_comms_i2c_bus4 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus4_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
extern SemaphoreHandle_t g_comms_i2c_bus4_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus4_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus4_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus4_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_semaphore_memory = NULL,
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus4_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus4_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus4_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
.p_mutex_memory = NULL,
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus4_slave_address;
 #if COMMS_I2C_CFG_BUS4_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus4_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus4_slave_address,
};
 #elif COMMS_I2C_CFG_BUS4_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus4_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus4_slave_address,
};
 #endif
const i2c_driver_instance_t g_comms_i2c_bus4_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS4_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS4_DRIVER_CH,
	.callback       = rm_comms_i2c_bus4_callback,
 #if COMMS_I2C_CFG_BUS4_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus4_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus4_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus4_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS4_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus4_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus4_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
const rm_comms_i2c_device_cfg_t g_comms_i2c_device4_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_BUS4_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_BUS4_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device4_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_BUS4_BLOCKING_TIMEOUT,
    .p_lower_level_cfg = (void *)&g_comms_i2c_device4_lower_level_cfg,
    .p_extend          = (void *)&g_comms_i2c_bus4_extended_cfg,
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_BUS4_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device4 =
{
    .p_ctrl = &g_comms_i2c_device4_ctrl,
    .p_cfg  = &g_comms_i2c_device4_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus0_callback
 * Description  : The function callback by r_riic_rx or r_sci_iic_rx
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus0_callback(void)
{
    rm_comms_i2c_callback (&g_comms_i2c_device0_ctrl);
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus0_callback
 *********************************************************************************************************************/
#endif

#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus1_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus1_callback(void)
{
    rm_comms_i2c_callback (&g_comms_i2c_device1_ctrl);
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus1_callback
 *********************************************************************************************************************/
#endif

#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus2_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus2_callback(void)
{
    rm_comms_i2c_callback (&g_comms_i2c_device2_ctrl);
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus2_callback
 *********************************************************************************************************************/
#endif

#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus3_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus3_callback(void)
{
    rm_comms_i2c_callback (&g_comms_i2c_device3_ctrl);
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus3_callback
 *********************************************************************************************************************/
#endif

#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus4_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus4_callback(void)
{
    rm_comms_i2c_callback (&g_comms_i2c_device4_ctrl);
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus4_callback
 *********************************************************************************************************************/
#endif


/******************************************************************************************************************//**
 * @} (end addtogroup rm_comm_i2c_bus_rx_instance)
 *********************************************************************************************************************/
