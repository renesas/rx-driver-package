/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp_mcu_startup.c
* Description  : This module implements user startup specific functions.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2019 2.00     Merged processing of all devices.
*                               Fixed coding style.
*         : 26.07.2019 2.01     Modified comment of API function to Doxygen style.
*                               Added Initialization the trigonometric function unit in R_BSP_StartupOpen function.
*         : 31.05.2024 2.02     Fixed coding style.
*         : 26.02.2025 2.03     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Platform support. */
#include "platform.h"

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE != 0

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Error checking
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_BSP_StartupOpen
 ******************************************************************************************************************//**
 * @brief Specifies settings to use the BSP and peripheral FIT modules. Call this function only when the BSP startup 
 * is disabled.
 * @details This function performs initialization for the interrupt callback, register protection, and the hardware 
 * and pins. These processing are needed for using the BSP and peripheral FIT modules. Thus, this function must be 
 * called in the beginning of the main function. Call this function only when the BSP startup is disabled.
 * @note The R_BSP_StartupOpen function performs a part of processing in the startup function.
 * See Section 5.18 in the application note for more information.
 */
void R_BSP_StartupOpen(void)
{
    /* Initializes the trigonometric function unit. */
#ifdef BSP_MCU_TRIGONOMETRIC
#ifdef __TFU
    R_BSP_INIT_TFU();
#endif
#endif
    /* Initialize RAM. */
    bsp_ram_initialize();

    /* Initialize MCU interrupt callbacks. */
    bsp_interrupt_open();

    /* Initialize register protection functionality. */
    bsp_register_protect_open();

    /* Configure the MCU and board hardware */
    hardware_setup();
} /* End of function R_BSP_StartupOpen() */

#endif /* BSP_CFG_STARTUP_DISABLE != 0 */

