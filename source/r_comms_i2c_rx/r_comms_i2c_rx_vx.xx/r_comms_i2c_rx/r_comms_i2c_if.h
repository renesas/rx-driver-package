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

#ifndef R_COMMS_I2C_IF_H_
#define R_COMMS_I2C_IF_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_comms_i2c.h"
#if COMMS_I2C_CFG_DRIVER_I2C
#include "r_riic_rx_if.h"
#endif
#if COMMS_I2C_CFG_DRIVER_SCI_I2C
#include "r_sci_iic_rx_if.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/* I2C device specific configuration */
typedef struct st_rm_comms_i2c_device_cfg
{
    uint32_t const slave_address;      ///< Slave address of I2C device
    uint32_t const slave_address_bits; ///< Indicates how slave fields should be interpreted
} rm_comms_i2c_device_cfg_t;

/* RX Driver Instance */
typedef struct st_i2c_master_instance
{
    uint32_t const     driver_type;
    uint32_t const     driver_channel;
    void const       * p_info;
    void (* callback)(void);
} i2c_master_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
 /* COMMS I2C Shared Bus */
#if (1 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus0_extended_cfg;
#endif
#if (2 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus1_extended_cfg;
#endif
#if (3 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus2_extended_cfg;
#endif
#if (4 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus3_extended_cfg;
#endif
#if (5 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus4_extended_cfg;
#endif
#if (6 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus5_extended_cfg;
#endif
#if (7 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus6_extended_cfg;
#endif
#if (8 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus7_extended_cfg;
#endif
#if (9 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus8_extended_cfg;
#endif
#if (10 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus9_extended_cfg;
#endif
#if (11 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus10_extended_cfg;
#endif
#if (12 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus11_extended_cfg;
#endif
#if (13 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus12_extended_cfg;
#endif
#if (14 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus13_extended_cfg;
#endif
#if (15 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus14_extended_cfg;
#endif
#if (16 <= COMMS_I2C_CFG_BUS_NUM_MAX)
extern rm_comms_i2c_bus_extended_cfg_t g_comms_i2c_bus15_extended_cfg;
#endif

 /* COMMS I2C Device */
#if (1 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device0;
#endif
#if (2 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device1;
#endif
#if (3 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device2;
#endif
#if (4 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device3;
#endif
#if (5 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device4;
#endif
#if (6 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device5;
#endif
#if (7 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device6;
#endif
#if (8 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device7;
#endif
#if (9 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device8;
#endif
#if (10 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device9;
#endif
#if (11 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device10;
#endif
#if (12 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device11;
#endif
#if (13 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device12;
#endif
#if (14 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device13;
#endif
#if (15 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device14;
#endif
#if (16 <= COMMS_I2C_CFG_DEVICE_NUM_MAX)
extern const rm_comms_instance_t g_comms_i2c_device15;
#endif

#endif /* R_COMMS_I2C_IF_H_ */
