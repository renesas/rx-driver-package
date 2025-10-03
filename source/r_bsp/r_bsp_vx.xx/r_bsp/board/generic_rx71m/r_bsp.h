/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp.h
* H/W Platform : GENERIC_RX71M
* Description  : Has the header files that should be included for this platform.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 07.08.2013 1.00     First Release
*         : 01.11.2017 2.00     Added include path of "r_bsp_mcu_startup.h".
*         : 01.07.2018 2.01     Additional RTOS header file.
*         : 28.02.2019 3.00     Added the following include path.
*                                - r_rx_compiler.h
*                                - r_rtos.h
*                                - r_bsp_interrupts.h
*                                - mcu_clocks.h
*                                - r_rx_intrinsic_functions.h
*                               Modified the following include path.
*                                - lowsrc.h
*                                - r_bsp_mcu_startup.h
*                                - vecttbl.h
*                               Added support for GNUC and ICCRX.
*                               Fixed coding style.
*         : 08.10.2019 3.01     Added the following include path.
*                                - r_bsp_software_interrupt.h
*                               Changed include of r_bsp_config.h for added support of Renesas RTOS(RI600V4 or RI600PX).
*         : 23.04.2021 3.02     Added the include of fsp_common_api.h and r_fsp_error.h.
*         : 30.11.2021 3.03     Modified the include file.
*         : 26.02.2025 3.04     Changed the disclaimer.
*         : 28.05.2025 3.05     Deleted the include of lowsrc.h.
***********************************************************************************************************************/

/* Make sure that no other platforms have already been defined. Do not touch this! */
#ifdef  PLATFORM_DEFINED
#error  "Error - Multiple platforms defined in platform.h!"
#else
#define PLATFORM_DEFINED
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
INCLUDE APPROPRIATE MCU AND BOARD FILES
***********************************************************************************************************************/
#include    "r_bsp_config.h"
#include    "mcu/all/r_bsp_common.h"
#include    "mcu/all/r_rx_compiler.h"

#include    "r_bsp_interrupt_config.h"

#include    "mcu/all/lowlvl.h"
#include    "mcu/all/r_bsp_mcu_startup.h"

#if defined(__CCRX__)
#include    "mcu/rx71m/register_access/ccrx/iodefine.h"
#elif defined(__GNUC__)
#include    "mcu/rx71m/register_access/gnuc/iodefine.h"
#elif defined(__ICCRX__)
#include    "mcu/rx71m/register_access/iccrx/iodefine.h"
#endif /* defined(__CCRX__), defined(__GNUC__), defined(__ICCRX__) */
#include    "mcu/rx71m/r_bsp_cpu.h"
#include    "mcu/rx71m/r_bsp_locking.h"
#include    "mcu/rx71m/mcu_clocks.h"
#include    "mcu/rx71m/mcu_info.h"
#include    "mcu/rx71m/mcu_init.h"
#include    "mcu/rx71m/mcu_interrupts.h"
#include    "mcu/rx71m/mcu_locks.h"
#include    "mcu/rx71m/mcu_mapped_interrupts_private.h"
#include    "mcu/rx71m/mcu_mapped_interrupts.h"
#include    "mcu/rx71m/vecttbl.h"

#include    "board/generic_rx71m/hwsetup.h"

#include    "mcu/all/r_bsp_interrupts.h"
#include    "mcu/all/r_bsp_software_interrupt.h"
#include    "mcu/all/r_rx_intrinsic_functions.h"
#include    "mcu/all/r_rtos.h"

#include    "mcu/all/fsp_common_api.h"
#include    "mcu/all/r_fsp_error.h"

#ifdef __cplusplus
}
#endif

#ifndef BSP_BOARD_GENERIC_RX71M
#define BSP_BOARD_GENERIC_RX71M

#endif /* BSP_BOARD_GENERIC_RX71M */

