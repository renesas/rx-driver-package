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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_malloc.c
 * Description  : Function to get memory.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*****************************************************************************************
 * Function Name  @fn            cellular_malloc
 ****************************************************************************************/
void * cellular_malloc(const size_t size)
{
    void * p_ret = NULL;
#if BSP_CFG_RTOS_USED == (1)
    p_ret = pvPortMalloc(size);
#elif BSP_CFG_RTOS_USED == (5)
    switch (size)
    {
        case EVENT_BLOCK_SIZE:
            tx_block_allocate(&g_cellular_event_pool, &p_ret, TX_NO_WAIT);
            break;
        case THREAD_BLOCK_SIZE:
            tx_block_allocate(&g_cellular_thread_pool, &p_ret, TX_NO_WAIT);
            break;
        case SEMAPHORE_BLOCK_SIZE:
            tx_block_allocate(&g_cellular_semaphore_pool, &p_ret, TX_NO_WAIT);
            break;
        default:
            tx_block_allocate(&g_cellular_socket_pool, &p_ret, TX_NO_WAIT);
            break;
    }
#endif

    return p_ret;
}
/**********************************************************************************************************************
 * End of function cellular_malloc
 *********************************************************************************************************************/
