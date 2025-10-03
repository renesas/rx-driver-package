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
*         : 31.05.2024 1.00     First Release
*         : 26.02.2025 1.01     Changed the disclaimer.
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

/**********************************************************************************************************************
 * Function Name: R_BSP_SoftwareLock
 ******************************************************************************************************************//**
 * @brief Attempts to reserve a lock.
 * @param[out] plock Pointer to lock structure with lock to try and acquire.
 * @retval true Successful, lock was available and acquired.
 * @retval false Failure, lock was already acquired and is not available.
 * @details This function implements an atomic locking mechanism. Locks can be used in numerous ways. Two common uses
 * of locks are to protect critical sections of code and to protect against duplicate resource allocation.
 * For protecting critical sections of code the user would require that the code first obtain the critical section's
 * lock before executing. An example of protecting against duplicate resource allocation would be if the user had two
 * FIT modules that used the same peripheral. For example, the user may have one FIT module that uses the SCI
 * peripheral in UART mode and another FIT module that uses the SCI peripheral in I2C mode. To make sure that both
 * modules cannot use the same SCI channel, locks can be used.
 * Care should be taken when using locks as they do not provide advanced features one might expect from an RTOS
 * semaphore or mutex. If used improperly locks can lead to deadlock in the user's system.
 * Users can override the default locking mechanisms.
 */
bool R_BSP_SoftwareLock (BSP_CFG_USER_LOCKING_TYPE * const plock);

/**********************************************************************************************************************
 * Function Name: R_BSP_SoftwareUnlock
 ******************************************************************************************************************//**
 * @brief Releases a lock.
 * @param[out] plock Pointer to lock structure with lock to release.
 * @retval true Successful, lock was released. Or the lock has been already released.
 * @retval false Failure, lock could not be released.
 * @details This function releases a lock that was previously acquired using the R_BSP_SoftwareLock() function.
 */
bool R_BSP_SoftwareUnlock (BSP_CFG_USER_LOCKING_TYPE * const plock);

/**********************************************************************************************************************
 * Function Name: R_BSP_HardwareLock
 ******************************************************************************************************************//**
 * @brief Attempts to reserve a hardware peripheral lock.
 * @param[in] hw_index Index of lock to acquire from the hardware lock array.
 * @retval true Successful, lock was available and acquired.
 * @retval false Failure, lock was already acquired and is not available.
 * @details This function attempts to acquire the lock for a hardware resource of the MCU. Instead of sending in a
 * pointer to a lock as with the R_BSP_SoftwareLock() function, the user sends in an index to an array that holds 1
 * lock per MCU hardware resource. This array is shared amongst all FIT modules and user code therefore allowing
 * multiple FIT modules (and user code) to use the same locks. The user can see the available hardware resources by
 * looking at the mcu_lock_t enum in mcu_locks.h. These enum values are also the index into the hardware lock array.
 * The same atomic locking mechanisms from the R_BSP_SoftwareLock() function are used with this function as well.
 * @note Each entry in the mcu_lock_t enum in mcu_locks.h will be allocated a lock. On RX MCUs, each lock is required
 * to be 4-bytes. If RAM space is an issue then the user can remove the entries from the mcu_lock_t enum they are not
 * using. For example, if the user is not using the CRC peripheral then they could delete the BSP_LOCK_CRC entry. The
 * user will save 4-bytes per deleted entry.
 */
bool R_BSP_HardwareLock (mcu_lock_t const hw_index);

/**********************************************************************************************************************
 * Function Name: R_BSP_HardwareUnlock
 ******************************************************************************************************************//**
 * @brief Releases a hardware peripheral lock.
 * @param[in] hw_index Index of lock to release from the hardware lock array.
 * @retval true Successful, lock was released.
 * @retval false Failure, lock could not be released.
 * @details This function attempts to release the lock for a hardware resource of the MCU that was previously acquired
 * using the R_BSP_HardwareLock() function.
 * @note Each entry in the mcu_lock_t enum in mcu_locks.h will be allocated a lock. On RX MCUs, each lock is required
 * to be 4-bytes. If RAM space is an issue then the user can remove the entries from the mcu_lock_t enum that they are
 * not using. For example, if the user is not using the CRC peripheral then they could delete the BSP_LOCK_CRC entry.
 * The user will save 4-bytes per deleted entry.
 */
bool R_BSP_HardwareUnlock (mcu_lock_t const hw_index);

#if BSP_CFG_USER_LOCKING_ENABLED != 0
/* Is user is using their own lock functions then these are the prototypes. */
bool BSP_CFG_USER_LOCKING_SW_LOCK_FUNCTION(BSP_CFG_USER_LOCKING_TYPE * const plock);
bool BSP_CFG_USER_LOCKING_SW_UNLOCK_FUNCTION(BSP_CFG_USER_LOCKING_TYPE * const plock);
bool BSP_CFG_USER_LOCKING_HW_LOCK_FUNCTION(mcu_lock_t const hw_index);
bool BSP_CFG_USER_LOCKING_HW_UNLOCK_FUNCTION(mcu_lock_t const hw_index);
#endif

#endif /* LOCKING_H */

