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
/***********************************************************************************************************************
* File Name    : r_canfd_rx_if.h
* Description  : The RX CANFD FIT API.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 22.11.2021 1.00    Initial Release
*                              Supported for RX660.
*         : 28.06.2022 1.10    Changed Minor version to 10.
*         : 06.01.2023 1.20    Changed Minor version to 20.
*         : 31.03.2023 1.30    Changed Minor version to 30.
*                              Supported for RX26T.
***********************************************************************************************************************/
#ifndef CAN_INTERFACE_HEADER_FILE
#define CAN_INTERFACE_HEADER_FILE 

/***********************************************************************************************************************
Includes   "Project Includes"
***********************************************************************************************************************/
#include <stdint.h>
#include "r_canfd.h"
#include "r_can_api.h"
#include "r_canfd_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if (R_BSP_VERSION_MAJOR < 7)
    #error "This module must use BSP module of Rev.7.00 or higher. Please use the BSP module of Rev.7.00 or higher."
#endif

/* Version Number of API. */
#define CANFD_RX_VERSION_MAJOR           (1)
#define CANFD_RX_VERSION_MINOR           (30)
/* The process of getting the version number is done through the macro below. The version number is encoded where the
   top 2 bytes are the major version number and the bottom 2 bytes are the minor version number. For example,
   Version 4.25 would be returned as 0x00040019. */
#define R_CANFD_GetVersion()  ((((uint32_t)CANFD_RX_VERSION_MAJOR) << 16) | (uint32_t)CANFD_RX_VERSION_MINOR)

/* A pointer to the CAN peripheral registers memory map structure. */
typedef volatile struct st_canfd0 R_BSP_EVENACCESS_SFR * canfd_ch_st_ptr;

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/**************************************************************************************************
Exported global variables
***************************************************************************************************/
/**************************************************************************************************
Exported global functions (to be accessed by other files)
***************************************************************************************************/

/** CANFD on CANFD Instance. */
extern const can_instance_t g_canfd0;

/** Access the CANFD instance using these structures when calling API functions directly (::p_api is not used). */
extern canfd_instance_ctrl_t g_canfd0_ctrl;
extern can_cfg_t g_canfd0_cfg;
extern canfd_extended_cfg_t g_canfd0_extended_cfg;
extern can_bit_timing_cfg_t g_canfd0_bit_timing_cfg;
extern can_bit_timing_cfg_t g_canfd0_data_timing_cfg;

/* Global configuration (referenced by all instances) */
extern canfd_global_cfg_t g_canfd_global_cfg;
#endif    /* CAN_INTERFACE_HEADER_FILE */
