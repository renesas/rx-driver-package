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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_longq.c
* Description  : Functions for using 32-bit queues/circular buffers.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description  
*         : 11.12.2013 1.0     Initial Release
*         : 21.07.2014 1.10    Removed BSP dependency for parameter checking
*         : 30.09.2015 1.50    Added BSP dependency
*         : 29.01.2016 1.60    Fixed the initial setting process in the R_LONGQ_Open function.
*                              Fixed a program according to the Renesas coding rules.
*         : 01.06.2018 1.70    Added the comment to while statement.
*         : 07.02.2019 1.80    Deleted the inline expansion of the R_LONGQ_GetVersion function.
*         : 10.06.2020 1.81    Modified comment of API function to Doxygen style.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used functions of malloc() and the free() */
#include <stdlib.h>

/* Used the common type */
#include "platform.h"

/* Defines for LONGQ support */
#include "r_longq_private.h"
#include "r_longq_if.h"
#include "r_longq_config.h"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* QUEUE CONTROL BLOCK ALLOCATIONS */

#if (LONGQ_CFG_USE_HEAP_FOR_CTRL_BLKS == 0)
static longq_ctrl_t     g_qcb[LONGQ_CFG_MAX_CTRL_BLKS];
#endif


/***********************************************************************************************************************
* Function Name: R_LONGQ_Open
*******************************************************************************************************************/ /**
* @brief This function allocates and initializes a queue control block for a buffer provided by the user. A queue handle
*        is provided for use with other API functions.
* @param[in] p_buf Pointer to buffer.
* @param[in] size Buffer size in number of elements.
* @param[in] ignore_overflow true = continue to add entries when queue is full by overwriting oldest entry \n
*            false = return error when a Put() call is made and the queue is full
* @param[in,out] p_hdl Pointer to a handle for queue (value set here)
* @retval    LONGQ_SUCCESS: Successful; queue initialized
* @retval    LONGQ_ERR_NULL_PTR: Received null ptr; missing required argument
* @retval    LONGQ_ERR_INVALID_ARG: Size is less than or equal to 1.
* @retval    LONGQ_ERR_MALLOC_FAIL: Cannot allocate control block. Increase heap size.
* @retval    LONGQ_ERR_NO_MORE_CTRL_BLKS: Cannot assign control block. Increase LONGQ_MAX_CTRL_BLKS in config.h.
* @details   This function allocates or assigns a queue control block for the buffer pointed to by \e p_buf. Initializes
*            the queue to an empty state and provides a Handle to its control structure in \e p_hdl which is then used 
*            as a queue ID for the other API functions.
* @note None
*/
longq_err_t R_LONGQ_Open(uint32_t * const       p_buf,
                         uint16_t const         size,
                         bool const             ignore_overflow,
                         longq_hdl_t * const    p_hdl)
{
    longq_ctrl_t    *p_qcb = NULL;
#if (LONGQ_CFG_USE_HEAP_FOR_CTRL_BLKS == 0)
    uint32_t        i;
    static bool     qcb_init = false;
#endif

    /* CHECK ARGUMENTS */

#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if (NULL == p_hdl)
    {
        return LONGQ_ERR_NULL_PTR;   // return if invalid location
    }
    if (NULL == p_buf)
    {
        return LONGQ_ERR_NULL_PTR;      // return if no buffer pointer
    }
    if (size < 2)
    {
        return LONGQ_ERR_INVALID_ARG;   // return insufficient buffer size
    }
#endif

    /* GET QUEUE CONTROL BLOCK */

#if LONGQ_CFG_USE_HEAP_FOR_CTRL_BLKS

    /* allocate memory for a QCB */
    p_qcb = (longq_ctrl_t *)malloc(sizeof(longq_ctrl_t));
    if (NULL == p_qcb)
    {
        return LONGQ_ERR_MALLOC_FAIL;
    }
#else

    /* if first Open call, mark all QCBs as being available */
    if (false == qcb_init)
    {
        /* WAIT_LOOP */
        for (i=0; i < LONGQ_CFG_MAX_CTRL_BLKS; i++)
        {
            g_qcb[i].size = 0;
        }
        qcb_init = true;
    }

    /* locate first available QCB */
    /* WAIT_LOOP */
    for (i=0; i < LONGQ_CFG_MAX_CTRL_BLKS; i++)
    {
        if (0 == g_qcb[i].size)
        {
            p_qcb = &g_qcb[i];
            break;
        }
    }
    
    /* return error if none available */
    if (LONGQ_CFG_MAX_CTRL_BLKS == i)
    {
        return LONGQ_ERR_NO_MORE_CTRL_BLKS;
    }
#endif


    /* INITIALIZE QCB FIELDS */
    
    p_qcb->buffer = p_buf;
    p_qcb->size = size;
    p_qcb->count = 0;
    p_qcb->in_index = 0;
    p_qcb->out_index = 0;
    p_qcb->ignore_ovfl = ignore_overflow;
    
    
    /* SET HANDLE */
    
    *p_hdl = p_qcb;
    return LONGQ_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_LONGQ_Put
*******************************************************************************************************************/ /**
* @brief This function adds an entry to the queue.
* @param[in,out] hdl Handle for queue.
* @param[in] datum Entry to add to queue.
* @retval    LONGQ_SUCCESS: Successful; entry added to queue
* @retval    LONGQ_ERR_NULL_PTR: hdl is NULL
* @retval    LONGQ_ERR_QUEUE_FULL: Queue full; cannot add entry to queue.
* @details   This function adds the contents of \e datum to the queue associated with \e hdl. If the queue is full and 
*            \e ignore_overflow was set to \e false during Open(), then \e LONG_ERR_QUEUE_FULL is returned. If the queue
*            is full and \e ignore_overflow was set to \e true during Open(), then \e datum overwrites the oldest entry
*            in the queue and \e LONGQ_SUCCESS is returned.
* @note      If the queue is accessed at both the interrupt and application level, it is up to the user to disable and 
*            enable the associated interrupt before and after calling this function from the application level. If the 
*            queue is accessed by tasks of different priorities, it is up to the user to prevent task switching or to 
*            utilize a mutex or semaphore to reserve the queue.
*/
longq_err_t R_LONGQ_Put(longq_hdl_t const   hdl,
                        uint32_t const      datum)
{


#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if (NULL == hdl)
    {
        return LONGQ_ERR_NULL_PTR;          // return if no handle
    }
#endif

    if ((hdl->count >= hdl->size) && (false == hdl->ignore_ovfl))
    {
        return LONGQ_ERR_QUEUE_FULL;        // return if queue is full
    }

    /* load datum into queue */
    hdl->buffer[hdl->in_index++] = datum;   // add entry
    if (hdl->in_index >= hdl->size)         // adjust index
    {
        hdl->in_index = 0;
    }

    /* if queue is full but overflow allowed, adjust out index */
    if ((hdl->count >= hdl->size) && (true == hdl->ignore_ovfl))
    {
        if ((++hdl->out_index) >= hdl->size)  // adjust index
        {
            hdl->out_index = 0;
        }
    }
    else // otherwise adjust count
    {
        hdl->count++;
    }

    return LONGQ_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_LONGQ_Get
*******************************************************************************************************************/ /**
* @brief This function removes an entry from the queue.
* @param[in,out] hdl Handle for queue.
* @param[in,out] p_datum Pointer to load entry to.
* @retval    LONGQ_SUCCESS: Successful; entry removed from queue
* @retval    LONGQ_ERR_NULL_PTR: hdl or p_datum is NULL
* @retval    LONGQ_ERR_QUEUE_EMPTY: Queue empty; no data available to fetch
* @details   This function removes the oldest entry (if available) in the queue associated with \e hdl and loads it into 
*            the location pointed to by \e p_datum.
* @note      If the queue is accessed at both the interrupt and application level, it is up to the user to disable and 
*            enable the associated interrupt before and after calling this function from the application level. If the 
*            queue is accessed by tasks of different priorities, it is up to the user to prevent task switching or to 
*            utilize a mutex or semaphore to reserve the queue.
*/
longq_err_t R_LONGQ_Get(longq_hdl_t const   hdl,
                        uint32_t * const    p_datum)
{


#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == hdl) || (NULL == p_datum))
    {
        return LONGQ_ERR_NULL_PTR;          // return if no handle
    }
#endif

    if (0 == hdl->count)
    {
        return LONGQ_ERR_QUEUE_EMPTY;       // return if queue empty        
    }

    *p_datum = hdl->buffer[hdl->out_index++]; // get datum
    if (hdl->out_index >= hdl->size)        // adjust index
    {
        hdl->out_index = 0;
    }
    hdl->count--;                           // adjust count

    return LONGQ_SUCCESS;
}        


/***********************************************************************************************************************
* Function Name: R_LONGQ_Flush
*******************************************************************************************************************/ /**
* @brief This function resets a queue to an empty state.
* @param[in,out] hdl Handle for queue.
* @retval    LONGQ_SUCCESS: Successful; queue reset
* @retval    LONGQ_ERR_NULL_PTR: hdl is NULL
* @details   This function resets the queue identified by \e hdl to an empty state.
* @note      If the queue is accessed at both the interrupt and application level, it is up to the user to disable and 
*            enable the associated interrupt before and after calling this function from the application level. If the 
*            queue is accessed by tasks of different priorities, it is up to the user to prevent task switching or to 
*            utilize a mutex or semaphore to reserve the queue.
*/
longq_err_t R_LONGQ_Flush(longq_hdl_t const hdl)
{
#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if (NULL == hdl)
    {
        return LONGQ_ERR_NULL_PTR;
    }
#endif

    /* RESET QUEUE */
    
    hdl->in_index = 0;
    hdl->out_index = 0;
    hdl->count = 0;

    return LONGQ_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_LONGQ_Used
*******************************************************************************************************************/ /**
* @brief This function provides the number of entries in the queue.
* @param[in] hdl Handle for queue.
* @param[in,out] p_cnt Pointer to load queue data count to.
* @retval    LONGQ_SUCCESS: Successful; *p_cnt loaded with number of entries in queue
* @retval    LONGQ_ERR_NULL_PTR: hdl or p_cnt is NULL.
* @details   This function loads the number of entries in the queue associated with \e hdl and into the location pointed
*            to by \e p_cnt.
* @note      If the queue is accessed at both the interrupt and application level, it is up to the user to disable and 
*            enable the associated interrupt before and after calling this function from the application level. If the 
*            queue is accessed by tasks of different priorities, it is up to the user to prevent task switching or to 
*            utilize a mutex or semaphore to reserve the queue.
*/
longq_err_t R_LONGQ_Used(longq_hdl_t const  hdl,
                         uint16_t * const   p_cnt)
{
#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == hdl) || (NULL == p_cnt))
    {
        return LONGQ_ERR_NULL_PTR;
    }
#endif

    *p_cnt = hdl->count;
    return LONGQ_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_LONGQ_Unused
*******************************************************************************************************************/ /**
* @brief This function provides the number of elements available for storage in the queue.
* @param[in] hdl Handle for queue.
* @param[in,out] p_cnt Pointer to load queue unused element count to.
* @retval    LONGQ_SUCCESS: Successful; *p_cnt loaded with number of elements available in queue.
* @retval    LONGQ_ERR_NULL_PTR: hdl or p_cnt is NULL.
* @details   This function loads the number of unused elements in the queue associated with \e hdl and into the location 
*            pointed to by \e p_cnt.
* @note      If the queue is accessed at both the interrupt and application level, it is up to the user to disable and 
*            enable the associated interrupt before and after calling this function from the application level. If the 
*            queue is accessed by tasks of different priorities, it is up to the user to prevent task switching or to 
*            utilize a mutex or semaphore to reserve the queue.
*/
longq_err_t R_LONGQ_Unused(longq_hdl_t const  hdl,
                           uint16_t * const   p_cnt)
{
#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == hdl) || (NULL == p_cnt))
    {
        return LONGQ_ERR_NULL_PTR;
    }
#endif

    *p_cnt = (uint16_t) (hdl->size - hdl->count);
    return LONGQ_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_LONGQ_Close
*******************************************************************************************************************/ /**
* @brief This function releases the queue control block associated with a handle.
* @param[in,out] hdl Handle for queue.
* @retval    LONGQ_SUCCESS: Successful; control block freed
* @retval    LONGQ_ERR_NULL_PTR: hdl is NULL
* @details   If the control block associated with this Handle was allocated dynamically at run time 
*            (LONGQ_USE_HEAP_FOR_CTRL_BLKS set to 1 in config.h), then that memory is freed by this function. If the 
*            control block was statically allocated at compile time (LONGQ_USE_HEAP_FOR_CTRL_BLKS set to 0 in config.h),
*            then this function marks the control block as available for use by another buffer. Nothing is done to the 
*            contents of the buffer referenced by this Handle.
* @note      None
*/
longq_err_t R_LONGQ_Close(longq_hdl_t const hdl)
{

#if (LONGQ_CFG_PARAM_CHECKING_ENABLE == 1)
    if (NULL == hdl)
    {
        return LONGQ_ERR_NULL_PTR;
    }
#endif


#if LONGQ_CFG_USE_HEAP_FOR_CTRL_BLKS
    free(hdl);                          // free QCB memory
#else
    hdl->size = 0;                      // mark QCB as free
#endif
    
    return LONGQ_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_LONGQ_GetVersion
*******************************************************************************************************************/ /**
* @brief This function returns the driver version number at runtime.
* @returns Version number.
* @details Returns the version of this module. The version number is encoded such that the top 2 bytes are the major 
*          version number and the bottom 2 bytes are the minor version number.
* @note    None
*/
uint32_t  R_LONGQ_GetVersion(void)
{

    uint32_t const version = (LONGQ_VERSION_MAJOR << 16) | LONGQ_VERSION_MINOR;
    return version;
}
