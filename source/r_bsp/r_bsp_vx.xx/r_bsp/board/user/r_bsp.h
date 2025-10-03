/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp.h 
* Description  : Has the header files that should be included for this platform.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 13.01.2012 1.00     First Release
*         : 27.06.2012 1.10     Updated with new information to reflect udpated r_bsp structure.
*         : 26.02.2025 1.11     Changed the disclaimer.
***********************************************************************************************************************/

#ifndef BSP_BOARD_USER
#define BSP_BOARD_USER

/* Make sure that no other platforms have already been defined. Do not touch this! */
#ifdef  PLATFORM_DEFINED
#error  "Error - Multiple platforms defined in platform.h!"
#else
#define PLATFORM_DEFINED
#endif

/***********************************************************************************************************************
INCLUDE APPROPRIATE MCU AND BOARD FILES
***********************************************************************************************************************/
/* This is a user defined board. Start off by:
   1)Copy and rename one of the 'board' folders that most closely matches your system (same MCU Series and Group).
   2)Substitute in your MCU Group for the *MCU Group* option in the #include below for mcu_info.h.
   3)Copy the other #includes from the r_bsp.h in the 'board' folder that you copied earlier.
   4)Configure the BSP for your board by modifying the r_bsp_config_reference.h.
   5)Copy r_bsp_config_reference.h to your project directory and rename it r_bsp_config.h.
   You can also add your own include files here as well. */
#include    "r_bsp_config.h"
#include    "mcu/*MCU Group*/mcu_info.h"           

#endif /* BSP_BOARD_USER */


