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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_freertos.h
 * Description  : Configures the driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : xx.xx.xxxx 1.00     First Release
 *         : 02.09.2021 1.01     Fixed reset timing
 *         : 21.10.2021 1.02     Support for Azure RTOS
 *                               Support for GCC for Renesas GNURX Toolchain
 *         : 15.11.2021 1.03     Improved receiving behavior, removed socket buffers
 *         : 24.01.2022 1.04     R_CELLULAR_SetPSM and R_CELLULAR_SetEDRX have been added as new APIs
 *********************************************************************************************************************/

#ifndef CELLULAR_FREERTOS_H
#define CELLULAR_FREERTOS_H

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_cellular_if.h"

#if (CELLULAR_CFG_DEBUGLOG != 0)
/* print debug info */
#include "logging_levels.h"

/* Logging configuration */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "Renesas RYZ014A driver"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL   (CELLULAR_CFG_DEBUGLOG)
#endif

#if (LIBRARY_LOG_LEVEL != LOG_NONE)
#include "logging_stack.h"

/* These messages describe the situations when a library encounters
 * an error from which it cannot recover. */
#define CELLULAR_LOG_ERROR( message )   LogError( message )

/* These messages describe the situations when a library encounters
 * abnormal event that may be indicative of an error. Libraries continue
 * execution after logging a warning.*/
#define CELLULAR_LOG_WARN( message )    LogWarn( message )

/* These messages describe normal execution of a library. They provide
 * the progress of the program at a coarse-grained level. */
#define CELLULAR_LOG_INFO( message )    LogInfo( message )

/* Debug log messages are used to provide the
 * progress of the program at a fine-grained level. These are mostly used
 * for debugging and may contain excessive information such as internal
 * variables, buffers, or other specific information. */
#define CELLULAR_LOG_DEBUG( message )   LogDebug( message )
#endif
#else /* CELLULAR_CFG_DEBUGLOG != 0 */
/* Don't show logs */
#define CELLULAR_LOG_ERROR( message )   (NULL)

/* Don't show logs */
#define CELLULAR_LOG_WARN( message )    (NULL)

/* Don't show logs */
#define CELLULAR_LOG_INFO( message )    (NULL)

/* Don't show logs */
#define CELLULAR_LOG_DEBUG( message )   (NULL)
#endif /* CELLULAR_CFG_DEBUGLOG != 0 */

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == (5)
/* Convert time to milliseconds */
#define MS_TO_TICKS( time )    (( UINT )((( UINT)( time ) * ( UINT ) TX_TIMER_TICKS_PER_SECOND ) / ( UINT ) 1000U ))

/* Get the memory size of the socket management structure */
#define SOCKET_BLOCK_SIZE             (sizeof(st_cellular_socket_ctrl_t))

/* Each memory block contains a pointer to the overhead represented by "sizeof(void *)" */
#define TOTAL_SOCKET_BLOCK_SIZE       (CELLULAR_CREATABLE_SOCKETS * (SOCKET_BLOCK_SIZE  + sizeof(void *)))

/* Get the memory size of the event group handle */
#define EVENT_BLOCK_SIZE              (sizeof(TX_EVENT_FLAGS_GROUP))

/* Each memory block contains a pointer to the overhead represented by "sizeof(void *)" */
#define TOTAL_EVENT_BLOCK_SIZE        (EVENT_BLOCK_SIZE + sizeof(void *))

/* Get the memory size of the thread structure */
#define THREAD_BLOCK_SIZE             (sizeof(TX_THREAD))

/* Each memory block contains a pointer to the overhead represented by "sizeof(void *)" */
#define TOTAL_THREAD_BLOCK_SIZE       (THREAD_BLOCK_SIZE + sizeof(void *))

/* Get the memory size of the semaphore handle */
#define SEMAPHORE_BLOCK_SIZE          (sizeof(TX_SEMAPHORE))

/* Each memory block contains a pointer to the overhead represented by "sizeof(void *)" */
#define TOTAL_SEMAPHORE_BLOCK_SIZE    ((CELLULAR_CREATABLE_SOCKETS + 1) * (SEMAPHORE_BLOCK_SIZE  + sizeof(void *)))
#endif

/*****************************************************************************
 * Private Functions
 *****************************************************************************/
/****************************************************************************
 * Function Name  @fn            cellular_create_event_group
 * Description    @details       Create an event group.
 * Return Value   @retval        void * -
 *                                  Event group handle.
 ***************************************************************************/
void * cellular_create_event_group (const char * str);

/****************************************************************************
 * Function Name  @fn            cellular_delete_event_group
 * Description    @details       Delete event group.
 * Arguments      @param[in]     xEventGroup -
 *                                  Pointer to the event group to be deleted.
 ***************************************************************************/
void cellular_delete_event_group (void * const xEventGroup);

/*****************************************************************************************
 * Function Name  @fn            cellular_synchro_event_group
 * Description    @details       Synchronize between multiple tasks.
 * Arguments      @param[in]     xEventGroup -
 *                                  Pointer to the created event group.
 * Arguments      @param[in]     uxBitsToSet -
 *                                  Bits to set to event group.
 * Arguments      @param[in]     uxBitsToWaitFor -
 *                                  Bits waiting to be set.
 * Arguments      @param[in]     xTicksToWait -
 *                                  time out(millisecond).
 * Return Value   @retval        uint32_t(0) -
 *                                  time out.
 *                               uxBitsToWaitFor -
 *                                  Successful synchronization
 ****************************************************************************************/
uint32_t cellular_synchro_event_group (void * const xEventGroup,
                                        const uint32_t uxBitsToSet,
                                        const uint32_t uxBitsToWaitFor,
                                        const uint32_t xTicksToWait);

/****************************************************************************
 * Function Name  @fn            cellular_create_semaphore
 * Description    @details       Create a semaphore.
 * Return Value   @retval        void * -
 *                                  Semaphore handle.
 ***************************************************************************/
void * cellular_create_semaphore (const char * str);

/****************************************************************************
 * Function Name  @fn            cellular_delete_semaphore
 * Description    @details       Delete semaphore.
 * Arguments      @param[in]     xEventGroup -
 *                                  Pointer to the semaphore to be deleted.
 ***************************************************************************/
void cellular_delete_semaphore (void * xSemaphore);

/****************************************************************************
 * Function Name  @fn            cellular_create_task
 * Description    @details       Create a task.
 * Return Value   @retval        pdPASS(1) -
 *                                  Successful task creation.
 *                @retval        Except for [pdPASS] -
 *                                  Failed to create task.
 ***************************************************************************/
#if BSP_CFG_RTOS_USED == (1)
e_cellular_err_t cellular_create_task (void *(pxTaskCode)(void *),
                                        const char * const pcName,
                                        const uint16_t usStackDepth,
                                        void * const pvParameters,
                                        const uint32_t uxPriority,
                                        void * const pxCreatedTask);
#elif BSP_CFG_RTOS_USED == (5)
void * cellular_create_task (void *(pxTaskCode)(void *),
                                const char * const pcName,
                                const uint16_t usStackDepth,
                                void * const pvParameters,
                                const uint32_t uxPriority,
                                void * const pxCreatedTask);
#endif


/****************************************************************************
 * Function Name  @fn            cellular_delay_task
 * Description    @details       Delay task.
 * Arguments      @param[in]     delay_time -
 *                                  Time to perform delay(milliseconds).
 ***************************************************************************/
void cellular_delay_task (const uint32_t delay_time);

/****************************************************************************
 * Function Name  @fn            cellular_delete_task
 * Description    @details       Delete task.
 * Arguments      @param[in]     taskhandle -
 *                                  Pointer to the taskhandle to be deleted.
 ***************************************************************************/
void cellular_delete_task (void * taskhandle);

/*****************************************************************************************
 * Function Name  @fn            cellular_get_tickcount
 * Description    @details       Get the time that has elapsed since the task was started.
 * Return Value   @retval        uint32_t -
 *                                  Time elapsed(millisecond).
 ****************************************************************************************/
uint32_t cellular_get_tickcount (void);

/*****************************************************************************************
 * Function Name  @fn            cellular_malloc
 * Description    @details       Get memory.
 * Arguments      @param[in]     size -
 *                                  Memory acquisition size.
 * Return Value   @retval        void *
 *                                  Pointer to retrieved memory.
 ****************************************************************************************/
void * cellular_malloc (const size_t size);

/*****************************************************************************************
 * Function Name  @fn            cellular_free
 * Description    @details       Memory release.
 * Arguments      @param[in]     p_free -
 *                                  Pointer to memory.
 ****************************************************************************************/
void cellular_free (void * p_free);

/*****************************************************************************************
 * Function Name  @fn            cellular_give_semaphore
 * Description    @details       Return semaphore.
 * Arguments      @param[in]     xEventGroup -
 *                                  Pointer to the semaphore to be returned.
 * Return Value   @retval        CELLULAR_SEMAPHORE_SUCCESS
 *                                  Semaphore return successful.
 *                               CELLULAR_SEMAPHORE_ERR_GIVE
 *                                  Semaphore return failed.
 ****************************************************************************************/
e_cellular_err_semaphore_t cellular_give_semaphore (void * const xSemaphore);

/*****************************************************************************************
 * Function Name  @fn            cellular_take_semaphore
 * Description    @details       Return semaphore.
 * Arguments      @param[in]     xSemaphore -
 *                                  Pointer to get the semaphore.
 * Return Value   @retval        CELLULAR_SEMAPHORE_SUCCESS -
 *                                  Semaphore return successful.
 *                               CELLULAR_SEMAPHORE_ERR_TAKE -
 *                                  Semaphore acquisition failed.
 ****************************************************************************************/
e_cellular_err_semaphore_t cellular_take_semaphore (void * const xSemaphore);

#if BSP_CFG_RTOS_USED == (5)
/*****************************************************************************************
 * Function Name  @fn            cellular_block_pool_create
 * Description    @details       Create a memory pool.
 * Return Value   @retval        CELLULAR_SEMAPHORE_SUCCESS -
 *                                  Successfully created the memory pool.
 *                               CELLULAR_ERR_MEMORY_ALLOCATION -
 *                                  Failed to create the memory pool.
 ****************************************************************************************/
e_cellular_err_t cellular_block_pool_create (void);

/*****************************************************************************************
 * Function Name  @fn            cellular_block_pool_delete
 * Description    @details       Delete the memory pool.
 * Return Value   @retval        CELLULAR_SEMAPHORE_SUCCESS -
 *                                  Successfully deleted the memory pool.
 *                               CELLULAR_ERR_MEMORY_ALLOCATION -
 *                                  Failed to delete the memory pool.
 ****************************************************************************************/
e_cellular_err_t cellular_block_pool_delete (void);
#endif

#endif  /* CELLULAR_FREERTOS_H */
