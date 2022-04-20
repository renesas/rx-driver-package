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
 /* COMMS I2C Shared Bus */
#if (1 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus0_callback(void);
#endif
#if (2 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus1_callback(void);
#endif
#if (3 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus2_callback(void);
#endif
#if (4 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus3_callback(void);
#endif
#if (5 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus4_callback(void);
#endif
#if (6 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus5_callback(void);
#endif
#if (7 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus6_callback(void);
#endif
#if (8 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus7_callback(void);
#endif
#if (9 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus8_callback(void);
#endif
#if (10 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus9_callback(void);
#endif
#if (11 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus10_callback(void);
#endif
#if (12 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus11_callback(void);
#endif
#if (13 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus12_callback(void);
#endif
#if (14 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus13_callback(void);
#endif
#if (15 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus14_callback(void);
#endif
#if (16 <= COMMS_I2C_CFG_BUS_NUM_MAX)
void rm_comms_i2c_bus15_callback(void);
#endif

 /* COMMS I2C Device */
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE0_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device0 */
#endif
#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE1_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device1 */
#endif
#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE2_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device2 */
#endif
#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE3_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device3 */
#endif
#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE4_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device4 */
#endif
#if (6 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE5_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device5 */
#endif
#if (7 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE6_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device6 */
#endif
#if (8 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE7_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device7 */
#endif
#if (9 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE8_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device8 */
#endif
#if (10 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE9_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device9 */
#endif
#if (11 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE10_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device10 */
#endif
#if (12 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE11_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device11 */
#endif
#if (13 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE12_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device12 */
#endif
#if (14 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE13_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device13 */
#endif
#if (15 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE14_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device14 */
#endif
#if (16 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
void COMMS_I2C_CFG_DEVICE15_CALLBACK(rm_comms_callback_args_t * p_args); /* Callback function for device15 */
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
 /* COMMS I2C Shared Bus */
#if (1 <= COMMS_I2C_CFG_BUS_NUM_MAX)
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
SemaphoreHandle_t g_comms_i2c_bus0_blocking_semaphore_handle;
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus0_recursive_mutex_handle;
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
const i2c_master_instance_t g_comms_i2c_bus0_driver_instance =
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
#endif

#if (2 <= COMMS_I2C_CFG_BUS_NUM_MAX)
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
SemaphoreHandle_t g_comms_i2c_bus1_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus1_recursive_mutex_handle;
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
const i2c_master_instance_t g_comms_i2c_bus1_driver_instance =
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
#endif

#if (3 <= COMMS_I2C_CFG_BUS_NUM_MAX)
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
SemaphoreHandle_t g_comms_i2c_bus2_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus2_recursive_mutex_handle;
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
const i2c_master_instance_t g_comms_i2c_bus2_driver_instance =
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
#endif

#if (4 <= COMMS_I2C_CFG_BUS_NUM_MAX)
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
SemaphoreHandle_t g_comms_i2c_bus3_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus3_recursive_mutex_handle;
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
const i2c_master_instance_t g_comms_i2c_bus3_driver_instance =
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
#endif

#if (5 <= COMMS_I2C_CFG_BUS_NUM_MAX)
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
SemaphoreHandle_t g_comms_i2c_bus4_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus4_recursive_mutex_handle;
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
const i2c_master_instance_t g_comms_i2c_bus4_driver_instance =
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
#endif

#if (6 <= COMMS_I2C_CFG_BUS_NUM_MAX)
 #if BSP_CFG_RTOS == 1
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus5_blocking_semaphore_handle;
CHAR g_comms_i2c_bus5_blocking_semaphore_name[] = "g_comms_i2c_bus5 blocking semaphore";
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus5_recursive_mutex_handle;
CHAR g_comms_i2c_bus5_recursive_mutex_name[] = "g_comms_i2c_bus5 recursive mutex";
   #endif
  #endif
 #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus5_blocking_semaphore_handle;
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus5_recursive_mutex_handle;
   #endif
  #endif
 #endif
 #if BSP_CFG_RTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus5_blocking_semaphore =
{
 .p_semaphore_handle = &g_comms_i2c_bus5_blocking_semaphore_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
 .p_semaphore_name = &g_comms_i2c_bus5_blocking_semaphore_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus5_recursive_mutex =
{
 .p_mutex_handle = &g_comms_i2c_bus5_recursive_mutex_handle,
    #if BSP_CFG_RTOS == 1 // ThreadX
 .p_mutex_name = &g_comms_i2c_bus5_recursive_mutex_name[0],
    #elif BSP_CFG_RTOS == 2 // FreeRTOS
    #endif
};
   #endif
  #endif
 #endif

uint8_t g_comms_i2c_bus5_slave_address;
 #if COMMS_I2C_CFG_BUS5_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus5_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus5_slave_address,
};
 #elif COMMS_I2C_CFG_BUS5_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus5_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus5_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus5_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS5_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS5_DRIVER_CH,
	.callback       = rm_comms_i2c_bus5_callback,
 #if COMMS_I2C_CFG_BUS5_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus5_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus5_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus5_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS5_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus5_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus5_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (7 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus6_blocking_semaphore_handle;
CHAR g_comms_i2c_bus6_blocking_semaphore_name[] = "g_comms_i2c_bus6 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus6_recursive_mutex_handle;
CHAR g_comms_i2c_bus6_recursive_mutex_name[] = "g_comms_i2c_bus6 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus6_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus6_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus6_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus6_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus6_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus6_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus6_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus6_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus6_slave_address;
 #if COMMS_I2C_CFG_BUS6_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus6_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus6_slave_address,
};
 #elif COMMS_I2C_CFG_BUS6_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus6_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus6_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus6_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS6_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS6_DRIVER_CH,
	.callback       = rm_comms_i2c_bus6_callback,
 #if COMMS_I2C_CFG_BUS6_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus6_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus6_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus6_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS6_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus6_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus6_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (8 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus7_blocking_semaphore_handle;
CHAR g_comms_i2c_bus7_blocking_semaphore_name[] = "g_comms_i2c_bus7 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus7_recursive_mutex_handle;
CHAR g_comms_i2c_bus7_recursive_mutex_name[] = "g_comms_i2c_bus7 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus7_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus7_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus7_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus7_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus7_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus7_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus7_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus7_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus7_slave_address;
 #if COMMS_I2C_CFG_BUS7_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus7_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus7_slave_address,
};
 #elif COMMS_I2C_CFG_BUS7_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus7_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus7_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus7_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS7_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS7_DRIVER_CH,
	.callback       = rm_comms_i2c_bus7_callback,
 #if COMMS_I2C_CFG_BUS7_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus7_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus7_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus7_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS7_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus7_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus7_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (9 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus8_blocking_semaphore_handle;
CHAR g_comms_i2c_bus8_blocking_semaphore_name[] = "g_comms_i2c_bus8 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus8_recursive_mutex_handle;
CHAR g_comms_i2c_bus8_recursive_mutex_name[] = "g_comms_i2c_bus8 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus8_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus8_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus8_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus8_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus8_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus8_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus8_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus8_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus8_slave_address;
 #if COMMS_I2C_CFG_BUS8_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus8_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus8_slave_address,
};
 #elif COMMS_I2C_CFG_BUS8_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus8_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus8_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus8_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS8_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS8_DRIVER_CH,
	.callback       = rm_comms_i2c_bus8_callback,
 #if COMMS_I2C_CFG_BUS8_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus8_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus8_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus8_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS8_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus8_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus8_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (10 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus9_blocking_semaphore_handle;
CHAR g_comms_i2c_bus9_blocking_semaphore_name[] = "g_comms_i2c_bus9 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus9_recursive_mutex_handle;
CHAR g_comms_i2c_bus9_recursive_mutex_name[] = "g_comms_i2c_bus9 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus9_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus9_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus9_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus9_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus9_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus9_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus9_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus9_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus9_slave_address;
 #if COMMS_I2C_CFG_BUS9_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus9_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus9_slave_address,
};
 #elif COMMS_I2C_CFG_BUS9_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus9_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus9_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus9_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS9_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS9_DRIVER_CH,
	.callback       = rm_comms_i2c_bus9_callback,
 #if COMMS_I2C_CFG_BUS9_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus9_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus9_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus9_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS9_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus9_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus9_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (11 <= COMMS_I2C_CFG_BUS_NUM_MAX)
 #if BSP_CFG_RTOS == 1
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus10_blocking_semaphore_handle;
CHAR g_comms_i2c_bus10_blocking_semaphore_name[] = "g_comms_i2c_bus10 blocking semaphore";
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus10_recursive_mutex_handle;
CHAR g_comms_i2c_bus10_recursive_mutex_name[] = "g_comms_i2c_bus10 recursive mutex";
   #endif
  #endif
 #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus10_blocking_semaphore_handle;
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus10_recursive_mutex_handle;
   #endif
  #endif
 #endif
 #if BSP_CFG_RTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus10_blocking_semaphore =
{
 .p_semaphore_handle = &g_comms_i2c_bus10_blocking_semaphore_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
 .p_semaphore_name = &g_comms_i2c_bus10_blocking_semaphore_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus10_recursive_mutex =
{
 .p_mutex_handle = &g_comms_i2c_bus10_recursive_mutex_handle,
    #if BSP_CFG_RTOS == 1 // ThreadX
 .p_mutex_name = &g_comms_i2c_bus10_recursive_mutex_name[0],
    #elif BSP_CFG_RTOS == 2 // FreeRTOS
    #endif
};
   #endif
  #endif
 #endif

uint8_t g_comms_i2c_bus10_slave_address;
 #if COMMS_I2C_CFG_BUS10_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus10_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus10_slave_address,
};
 #elif COMMS_I2C_CFG_BUS10_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus10_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus10_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus10_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS10_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS10_DRIVER_CH,
	.callback       = rm_comms_i2c_bus10_callback,
 #if COMMS_I2C_CFG_BUS10_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus10_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus10_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus10_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS10_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus10_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus10_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (12 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus11_blocking_semaphore_handle;
CHAR g_comms_i2c_bus11_blocking_semaphore_name[] = "g_comms_i2c_bus11 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus11_recursive_mutex_handle;
CHAR g_comms_i2c_bus11_recursive_mutex_name[] = "g_comms_i2c_bus11 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus11_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus11_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus11_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus11_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus11_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus11_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus11_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus11_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus11_slave_address;
 #if COMMS_I2C_CFG_BUS11_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus11_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus11_slave_address,
};
 #elif COMMS_I2C_CFG_BUS11_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus11_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus11_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus11_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS11_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS11_DRIVER_CH,
	.callback       = rm_comms_i2c_bus11_callback,
 #if COMMS_I2C_CFG_BUS11_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus11_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus11_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus11_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS11_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus11_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus11_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (13 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus12_blocking_semaphore_handle;
CHAR g_comms_i2c_bus12_blocking_semaphore_name[] = "g_comms_i2c_bus12 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus12_recursive_mutex_handle;
CHAR g_comms_i2c_bus12_recursive_mutex_name[] = "g_comms_i2c_bus12 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus12_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus12_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus12_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus12_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus12_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus12_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus12_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus12_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus12_slave_address;
 #if COMMS_I2C_CFG_BUS12_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus12_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus12_slave_address,
};
 #elif COMMS_I2C_CFG_BUS12_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus12_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus12_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus12_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS12_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS12_DRIVER_CH,
	.callback       = rm_comms_i2c_bus12_callback,
 #if COMMS_I2C_CFG_BUS12_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus12_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus12_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus12_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS12_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus12_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus12_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (14 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus13_blocking_semaphore_handle;
CHAR g_comms_i2c_bus13_blocking_semaphore_name[] = "g_comms_i2c_bus13 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus13_recursive_mutex_handle;
CHAR g_comms_i2c_bus13_recursive_mutex_name[] = "g_comms_i2c_bus13 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus13_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus13_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus13_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus13_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus13_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus13_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus13_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus13_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus13_slave_address;
 #if COMMS_I2C_CFG_BUS13_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus13_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus13_slave_address,
};
 #elif COMMS_I2C_CFG_BUS13_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus13_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus13_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus13_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS13_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS13_DRIVER_CH,
	.callback       = rm_comms_i2c_bus13_callback,
 #if COMMS_I2C_CFG_BUS13_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus13_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus13_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus13_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS13_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus13_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus13_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (15 <= COMMS_I2C_CFG_BUS_NUM_MAX)
#if BSP_CFG_RTOS == 1
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus14_blocking_semaphore_handle;
CHAR g_comms_i2c_bus14_blocking_semaphore_name[] = "g_comms_i2c_bus14 blocking semaphore";
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus14_recursive_mutex_handle;
CHAR g_comms_i2c_bus14_recursive_mutex_name[] = "g_comms_i2c_bus14 recursive mutex";
  #endif
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus14_blocking_semaphore_handle;
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus14_recursive_mutex_handle;
  #endif
 #endif
#endif
#if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus14_blocking_semaphore =
{
.p_semaphore_handle = &g_comms_i2c_bus14_blocking_semaphore_handle,
  #if BSP_CFG_RTOS == 1 // ThreadX
.p_semaphore_name = &g_comms_i2c_bus14_blocking_semaphore_name[0],
  #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #endif
};
  #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus14_recursive_mutex =
{
.p_mutex_handle = &g_comms_i2c_bus14_recursive_mutex_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
.p_mutex_name = &g_comms_i2c_bus14_recursive_mutex_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
  #endif
 #endif
#endif

uint8_t g_comms_i2c_bus14_slave_address;
 #if COMMS_I2C_CFG_BUS14_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus14_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus14_slave_address,
};
 #elif COMMS_I2C_CFG_BUS14_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus14_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus14_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus14_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS14_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS14_DRIVER_CH,
	.callback       = rm_comms_i2c_bus14_callback,
 #if COMMS_I2C_CFG_BUS14_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus14_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus14_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus14_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS14_TIMEOUT,
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus14_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus14_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

#if (16 <= COMMS_I2C_CFG_BUS_NUM_MAX)
 #if BSP_CFG_RTOS == 1
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
TX_SEMAPHORE g_comms_i2c_bus15_blocking_semaphore_handle;
CHAR g_comms_i2c_bus15_blocking_semaphore_name[] = "g_comms_i2c_bus15 blocking semaphore";
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
TX_MUTEX g_comms_i2c_bus15_recursive_mutex_handle;
CHAR g_comms_i2c_bus15_recursive_mutex_name[] = "g_comms_i2c_bus15 recursive mutex";
   #endif
  #endif
 #elif BSP_CFG_RTOS == 2 // FreeRTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus15_blocking_semaphore_handle;
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
SemaphoreHandle_t g_comms_i2c_bus15_recursive_mutex_handle;
   #endif
  #endif
 #endif
 #if BSP_CFG_RTOS
  #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
/* Semaphore for blocking */
rm_comms_i2c_semaphore_t g_comms_i2c_bus15_blocking_semaphore =
{
 .p_semaphore_handle = &g_comms_i2c_bus15_blocking_semaphore_handle,
   #if BSP_CFG_RTOS == 1 // ThreadX
 .p_semaphore_name = &g_comms_i2c_bus15_blocking_semaphore_name[0],
   #elif BSP_CFG_RTOS == 2 // FreeRTOS
   #endif
};
   #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
/* Recursive mutex for bus */
rm_comms_i2c_mutex_t g_comms_i2c_bus15_recursive_mutex =
{
 .p_mutex_handle = &g_comms_i2c_bus15_recursive_mutex_handle,
    #if BSP_CFG_RTOS == 1 // ThreadX
 .p_mutex_name = &g_comms_i2c_bus15_recursive_mutex_name[0],
    #elif BSP_CFG_RTOS == 2 // FreeRTOS
    #endif
};
   #endif
  #endif
 #endif

uint8_t g_comms_i2c_bus15_slave_address;
 #if COMMS_I2C_CFG_BUS15_DRIVER_TYPE == COMMS_DRIVER_I2C
riic_info_t g_comms_i2c_bus15_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus15_slave_address,
};
 #elif COMMS_I2C_CFG_BUS15_DRIVER_TYPE == COMMS_DRIVER_SCI_I2C
sci_iic_info_t g_comms_i2c_bus15_driver_info =
{
    .p_slv_adr      = &g_comms_i2c_bus15_slave_address,
};
 #endif
const i2c_master_instance_t g_comms_i2c_bus15_driver_instance =
{
    .driver_type    = COMMS_I2C_CFG_BUS15_DRIVER_TYPE,
	.driver_channel = COMMS_I2C_CFG_BUS15_DRIVER_CH,
	.callback       = rm_comms_i2c_bus15_callback,
 #if COMMS_I2C_CFG_BUS15_DRIVER_TYPE == COMM_DRIVER_NONE
    .p_info         = NULL,
 #else
    .p_info         = (void const *)&g_comms_i2c_bus15_driver_info,
 #endif
};
rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus15_extended_cfg =
{
    .p_driver_instance     = (void*)&g_comms_i2c_bus15_driver_instance,
    .p_current_ctrl        = NULL,
    .bus_timeout           = COMMS_I2C_CFG_BUS15_TIMEOUT, 
 #if BSP_CFG_RTOS
 #if COMMS_I2C_CFG_RTOS_BLOCKING_SUPPORT_ENABLE
  .p_blocking_semaphore  = &g_comms_i2c_bus15_blocking_semaphore,
 #if COMMS_I2C_CFG_RTOS_BUS_LOCK_SUPPORT_ENABLE
  .p_bus_recursive_mutex = &g_comms_i2c_bus15_recursive_mutex,
 #else
  .p_bus_recursive_mutex = NULL,
 #endif
 #else
  .p_blocking_semaphore  = NULL,
  .p_bus_recursive_mutex = NULL,
 #endif
 #endif
};
#endif

 /* COMMS I2C Device */
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device0_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device0_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE0_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE0_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device0_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE0_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device0_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE0_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE0_CALLBACK,
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
const rm_comms_i2c_device_cfg_t g_comms_i2c_device1_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE1_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE1_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device1_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE1_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device1_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE1_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE1_CALLBACK,
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
const rm_comms_i2c_device_cfg_t g_comms_i2c_device2_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE2_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE2_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device2_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE2_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device2_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE2_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE2_CALLBACK,
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
const rm_comms_i2c_device_cfg_t g_comms_i2c_device3_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE3_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE3_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device3_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE3_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device3_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE3_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE3_CALLBACK,
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
const rm_comms_i2c_device_cfg_t g_comms_i2c_device4_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE4_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE4_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device4_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE4_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device4_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE4_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE4_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device4 =
{
    .p_ctrl = &g_comms_i2c_device4_ctrl,
    .p_cfg  = &g_comms_i2c_device4_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (6 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device5_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device5_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE5_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE5_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device5_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE5_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device5_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE5_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE5_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device5 =
{
    .p_ctrl = &g_comms_i2c_device5_ctrl,
    .p_cfg  = &g_comms_i2c_device5_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (7 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device6_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device6_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE6_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE6_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device6_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE6_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device6_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE6_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE6_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device6 =
{
    .p_ctrl = &g_comms_i2c_device6_ctrl,
    .p_cfg  = &g_comms_i2c_device6_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (8 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device7_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device7_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE7_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE7_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device7_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE7_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device7_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE7_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE7_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device7 =
{
    .p_ctrl = &g_comms_i2c_device7_ctrl,
    .p_cfg  = &g_comms_i2c_device7_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (9 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device8_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device8_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE8_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE8_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device8_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE8_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device8_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE8_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE8_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device8 =
{
    .p_ctrl = &g_comms_i2c_device8_ctrl,
    .p_cfg  = &g_comms_i2c_device8_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (10 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device9_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device9_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE9_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE9_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device9_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE9_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device9_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE9_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE9_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device9 =
{
    .p_ctrl = &g_comms_i2c_device9_ctrl,
    .p_cfg  = &g_comms_i2c_device9_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (11 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device10_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device10_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE10_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE10_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device10_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE10_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device10_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE10_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE10_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device10 =
{
    .p_ctrl = &g_comms_i2c_device10_ctrl,
    .p_cfg  = &g_comms_i2c_device10_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (12 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device11_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device11_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE11_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE11_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device11_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE11_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device11_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE11_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE11_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device11 =
{
    .p_ctrl = &g_comms_i2c_device11_ctrl,
    .p_cfg  = &g_comms_i2c_device11_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (13 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device12_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device12_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE12_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE12_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device12_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE12_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device12_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE12_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE12_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device12 =
{
    .p_ctrl = &g_comms_i2c_device12_ctrl,
    .p_cfg  = &g_comms_i2c_device12_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (14 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device13_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device13_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE13_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE13_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device13_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE13_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device13_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE13_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE13_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device13 =
{
    .p_ctrl = &g_comms_i2c_device13_ctrl,
    .p_cfg  = &g_comms_i2c_device13_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (15 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device14_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device14_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE14_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE14_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device14_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE14_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device14_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE14_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE14_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device14 =
{
    .p_ctrl = &g_comms_i2c_device14_ctrl,
    .p_cfg  = &g_comms_i2c_device14_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (16 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
rm_comms_i2c_instance_ctrl_t g_comms_i2c_device15_ctrl;
const rm_comms_i2c_device_cfg_t g_comms_i2c_device15_lower_level_cfg =
{
    .slave_address      = COMMS_I2C_CFG_DEVICE15_SLAVE_ADDR,
    .slave_address_bits = COMMS_I2C_CFG_DEVICE15_ADDR_MODE, /* 7bit address mode */
};
const rm_comms_cfg_t g_comms_i2c_device15_cfg =
{
    .semaphore_timeout = COMMS_I2C_CFG_DEVICE15_BLOCKING_TIMEOUT, 
    .p_lower_level_cfg = (void *)&g_comms_i2c_device15_lower_level_cfg,
    .p_extend          = (void *)(&(COMMS_I2C_CFG_DEVICE15_BUS_CH)),
    .p_callback        = (void (*)(rm_comms_callback_args_t *))COMMS_I2C_CFG_DEVICE15_CALLBACK,
};
const rm_comms_instance_t g_comms_i2c_device15 =
{
    .p_ctrl = &g_comms_i2c_device15_ctrl,
    .p_cfg  = &g_comms_i2c_device15_cfg,
    .p_api  = &g_comms_on_comms_i2c,
};
#endif

#if (1 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus0_callback
 * Description  : The function callback by r_riic_rx or r_sci_iic_rx
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus0_callback(void)
{
    if (NULL != g_comms_i2c_bus0_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus0_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus0_callback
 *********************************************************************************************************************/
#endif

#if (2 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus1_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus1_callback(void)
{
    if (NULL != g_comms_i2c_bus1_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus1_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus1_callback
 *********************************************************************************************************************/
#endif

#if (3 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus2_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus2_callback(void)
{
    if (NULL != g_comms_i2c_bus2_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus2_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus2_callback
 *********************************************************************************************************************/
#endif

#if (4 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus3_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus3_callback(void)
{
    if (NULL != g_comms_i2c_bus3_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus3_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus3_callback
 *********************************************************************************************************************/
#endif

#if (5 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus4_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus4_callback(void)
{
    if (NULL != g_comms_i2c_bus4_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus4_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus4_callback
 *********************************************************************************************************************/
#endif

#if (6 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus5_callback
 * Description  : The function callback by r_riic_rx or r_sci_iic_rx
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus5_callback(void)
{
    if (NULL != g_comms_i2c_bus5_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus5_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus5_callback
 *********************************************************************************************************************/
#endif

#if (7 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus6_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus6_callback(void)
{
    if (NULL != g_comms_i2c_bus6_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus6_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus6_callback
 *********************************************************************************************************************/
#endif

#if (8 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus7_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus7_callback(void)
{
    if (NULL != g_comms_i2c_bus7_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus7_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus7_callback
 *********************************************************************************************************************/
#endif

#if (9 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus8_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus8_callback(void)
{
    if (NULL != g_comms_i2c_bus8_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus8_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus8_callback
 *********************************************************************************************************************/
#endif

#if (10 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus9_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus9_callback(void)
{
    if (NULL != g_comms_i2c_bus9_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus9_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus9_callback
 *********************************************************************************************************************/
#endif

#if (11 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus10_callback
 * Description  : The function callback by r_riic_rx or r_sci_iic_rx
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus10_callback(void)
{
    if (NULL != g_comms_i2c_bus10_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus10_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus10_callback
 *********************************************************************************************************************/
#endif

#if (12 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus11_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus11_callback(void)
{
    if (NULL != g_comms_i2c_bus11_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus11_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus11_callback
 *********************************************************************************************************************/
#endif

#if (13 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus12_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus12_callback(void)
{
    if (NULL != g_comms_i2c_bus12_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus12_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus12_callback
 *********************************************************************************************************************/
#endif

#if (14 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus13_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus13_callback(void)
{
    if (NULL != g_comms_i2c_bus13_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus13_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus13_callback
 *********************************************************************************************************************/
#endif

#if (15 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus14_callback
 * Description  : The function callback
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus14_callback(void)
{
    if (NULL != g_comms_i2c_bus14_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus14_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus14_callback
 *********************************************************************************************************************/
#endif

#if (16 <= COMMS_I2C_CFG_BUS_NUM_MAX)
/**********************************************************************************************************************
 * Function Name: rm_comms_i2c_bus15_callback
 * Description  : The function callback by r_riic_rx or r_sci_iic_rx
 * Arguments    :
 * Return Value :
 * Doxygen commenting is provided with prototype in this file.
 *********************************************************************************************************************/
void rm_comms_i2c_bus15_callback(void)
{
    if (NULL != g_comms_i2c_bus15_extended_cfg.p_current_ctrl)
    {
        rm_comms_i2c_callback (g_comms_i2c_bus15_extended_cfg.p_current_ctrl);
    }    
}
/**********************************************************************************************************************
 * End of function rm_comms_i2c_bus15_callback
 *********************************************************************************************************************/
#endif

/******************************************************************************************************************//**
 * @} (end addtogroup rm_comm_i2c_bus_rx_instance)
 *********************************************************************************************************************/
