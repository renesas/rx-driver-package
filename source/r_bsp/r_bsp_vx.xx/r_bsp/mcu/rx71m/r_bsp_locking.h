/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp_locking.h
* Description  : This implements a locking mechanism that can be used by all code. The locking is done atomically so
*                common resources can be accessed safely.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 07.03.2012 1.00     First Release
*         : 20.09.2012 1.10     Added ability for user to implement their own locking if  
*                               BSP_CFG_USER_LOCKING_ENABLED != 0.
*         : 19.11.2012 1.20     Updated code to use 'BSP_' and 'BSP_CFG_' prefix for macros.
*         : 16.01.2013 1.30     Added const qualifiers to lock functions.
*         : 28.02.2019 1.31     Fixed coding style.
*         : 26.02.2025 1.32     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Lock types. */
#include "mcu_locks.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef LOCKING_H
#define LOCKING_H

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
bool R_BSP_SoftwareLock(BSP_CFG_USER_LOCKING_TYPE * const plock);
bool R_BSP_SoftwareUnlock(BSP_CFG_USER_LOCKING_TYPE * const plock);
bool R_BSP_HardwareLock(mcu_lock_t const hw_index);
bool R_BSP_HardwareUnlock(mcu_lock_t const hw_index);

#if BSP_CFG_USER_LOCKING_ENABLED != 0
/* Is user is using their own lock functions then these are the prototypes. */
bool BSP_CFG_USER_LOCKING_SW_LOCK_FUNCTION(BSP_CFG_USER_LOCKING_TYPE * const plock);
bool BSP_CFG_USER_LOCKING_SW_UNLOCK_FUNCTION(BSP_CFG_USER_LOCKING_TYPE * const plock);
bool BSP_CFG_USER_LOCKING_HW_LOCK_FUNCTION(mcu_lock_t const hw_index);
bool BSP_CFG_USER_LOCKING_HW_UNLOCK_FUNCTION(mcu_lock_t const hw_index);
#endif

#endif /* LOCKING_H */

