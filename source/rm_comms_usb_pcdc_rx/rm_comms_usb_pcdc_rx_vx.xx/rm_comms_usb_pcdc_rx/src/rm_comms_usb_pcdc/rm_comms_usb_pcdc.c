/***********************************************************************************************************************
 * Copyright (C) 2024 Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_comms_usb_pcdc.h"
#include "../rm_comms_lock/rm_comms_lock.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* Definitions of Open flag "COUT" */
#define RM_COMMS_USB_PCDC_OPEN    (0x434F5554UL)

/* Number of bytes to transfer on data stage for Line Coding Class request */
#define RM_COMMS_USB_PCDC_LINE_CODING_LENGTH     (0x07U)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Extern functions
 **********************************************************************************************************************/
extern fsp_err_t rm_comms_usb_pcdc_bus_open ( usb_ctrl_t *p_ctrl, usb_cfg_t *p_cfg );
extern fsp_err_t rm_comms_usb_pcdc_bus_close (usb_ctrl_t *p_ctrl);
extern fsp_err_t rm_comms_usb_pcdc_bus_read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
extern fsp_err_t rm_comms_usb_pcdc_bus_write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
extern fsp_err_t rm_comms_usb_pcdc_timer_open (cmt_instance_t *p_ctrl);
extern fsp_err_t rm_comms_usb_pcdc_timer_start (cmt_instance_t *p_ctrl);
extern fsp_err_t rm_comms_usb_pcdc_timer_close (cmt_instance_t *p_ctrl);
/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
rm_comms_usb_pcdc_api_t const g_comms_on_comms_usb_pcdc =
{
    .open        = RM_COMMS_USB_PCDC_Open,
    .read        = RM_COMMS_USB_PCDC_Read,
    .write       = RM_COMMS_USB_PCDC_Write,
    .writeRead   = RM_COMMS_USB_PCDC_WriteRead,
    .callbackSet = RM_COMMS_USB_PCDC_CallbackSet,
    .close       = RM_COMMS_USB_PCDC_Close,
};

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/* Comms USB variable to store p_ctrl, which is used for invoking Comms Callback in callback handler */
rm_comms_usb_pcdc_instance_ctrl_t   *gp_comms_usb_pcdc_crtl;
/* Global variable to store Virtual COM port settings */
usb_pcdc_linecoding_t               g_comms_usb_pcdc_line_coding;
usb_status_t                        g_usb_pcdc_event;

/*******************************************************************************************************************//**
 * @addtogroup RM_COMMS_USB_PCDC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Opens and configures the USB PCDC Comms module. Implements @ref rm_comms_usb_pcdc_api_t::open.
 *
 *
 * @retval FSP_SUCCESS                  USB PCDC Comms module successfully configured.
 * @retval FSP_ERR_ASSERTION            Null pointer, or one or more configuration options is invalid.
 * @retval FSP_ERR_ALREADY_OPEN         Module is already open.  This module can only be opened once.
 *
 * @return See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_Open (rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl, rm_comms_usb_pcdc_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_usb_pcdc_instance_ctrl_t * p_ctrl = (rm_comms_usb_pcdc_instance_ctrl_t *) p_api_ctrl;

#if RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ERROR_RETURN(RM_COMMS_USB_PCDC_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    rm_comms_usb_pcdc_extended_cfg_t * p_extend = (rm_comms_usb_pcdc_extended_cfg_t *) p_cfg->p_extend;

#if RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_extend);
    FSP_ASSERT(NULL != p_extend->p_usb);
    FSP_ASSERT(NULL != p_extend->p_usb->p_cfg);
    FSP_ASSERT(NULL != p_extend->p_cmt);
#endif

    usb_instance_t * p_usb_instance = (usb_instance_t *) p_extend->p_usb;

    p_ctrl->p_cfg      = p_cfg;
    p_ctrl->p_extend   = p_extend;
    p_ctrl->p_callback = p_cfg->p_callback;
    p_ctrl->p_context  = p_cfg->p_context;

    /* Store rm_comms_ctrl to gp_comms_usb_pcdc_crtl */
    gp_comms_usb_pcdc_crtl = p_ctrl;

#if BSP_CFG_RTOS == 2
    if (NULL != p_extend->p_tx_mutex)
    {
        /* Init mutex for writing */
        err = rm_comms_recursive_mutex_initialize(p_extend->p_tx_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_rx_mutex)
    {
        /* Init mutex for reading */
        err = rm_comms_recursive_mutex_initialize(p_extend->p_rx_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_tx_semaphore)
    {
        /* Init semaphore for writing */
        err = rm_comms_semaphore_initialize(p_extend->p_tx_semaphore);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_rx_semaphore)
    {
        /* Init semaphore for reading */
        err = rm_comms_semaphore_initialize(p_extend->p_rx_semaphore);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    /* Calls open function of USB HAL driver */
    err = rm_comms_usb_pcdc_bus_open(p_usb_instance->p_ctrl, p_usb_instance->p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if BSP_CFG_RTOS == 2
    /* Set callback for Free Rtos */
    R_USB_Callback(rm_comms_usb_pcdc_callback_handler);
#endif

#if BSP_CFG_RTOS == 0
    cmt_instance_t * p_cmt_instance = (cmt_instance_t *) p_extend->p_cmt;

    /* Open timer */
    err = rm_comms_usb_pcdc_timer_open(p_cmt_instance);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Start timer */
    err = rm_comms_usb_pcdc_timer_start(p_cmt_instance);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Set open flag */
    p_ctrl->open = RM_COMMS_USB_PCDC_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Disables specified USB PCDC Comms module. Implements @ref rm_comms_api_t::close.
 *
 * @retval FSP_SUCCESS              Successfully closed.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 *
 * @return See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_Close (rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_usb_pcdc_instance_ctrl_t * p_ctrl = (rm_comms_usb_pcdc_instance_ctrl_t *) p_api_ctrl;

#if RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(RM_COMMS_USB_PCDC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    rm_comms_usb_pcdc_extended_cfg_t const * p_extend = (rm_comms_usb_pcdc_extended_cfg_t *) p_ctrl->p_extend;

#if BSP_CFG_RTOS == 0
    cmt_instance_t * p_cmt_instance = (cmt_instance_t *) p_extend->p_cmt;

    /* Close timer */
    err = rm_comms_usb_pcdc_timer_close(p_cmt_instance);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    err = rm_comms_usb_pcdc_bus_close(p_extend->p_usb->p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if BSP_CFG_RTOS == 2
    if (NULL != p_extend->p_tx_mutex)
    {
        err = rm_comms_recursive_mutex_destroy(p_extend->p_tx_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_rx_mutex)
    {
        err = rm_comms_recursive_mutex_destroy(p_extend->p_rx_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_tx_semaphore)
    {
        err = rm_comms_semaphore_destroy(p_extend->p_tx_semaphore);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_rx_semaphore)
    {
        err = rm_comms_semaphore_destroy(p_extend->p_rx_semaphore);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    /* Clear open flag */
    p_ctrl->open = 0;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Updates the USB PCDC Comms callback. Implements @ref rm_comms_api_t::callbackSet.
 *
 * @retval FSP_SUCCESS              Successfully set.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 *
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_CallbackSet (rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl,
                                         void (* p_callback)(rm_comms_usb_pcdc_callback_args_t *),
                                         void const * const p_context)
{
    rm_comms_usb_pcdc_instance_ctrl_t * p_ctrl = (rm_comms_usb_pcdc_instance_ctrl_t *) p_api_ctrl;

#if RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_callback);
    FSP_ERROR_RETURN(RM_COMMS_USB_PCDC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_ctrl->p_callback = p_callback;
    p_ctrl->p_context  = p_context;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Performs a read from the USB PCDC device. Implements @ref rm_comms_api_t::read.
 *
 * @retval FSP_SUCCESS              Successfully data decoded.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 *
 * @return See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_Read (rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_usb_pcdc_instance_ctrl_t * p_ctrl = (rm_comms_usb_pcdc_instance_ctrl_t *) p_api_ctrl;

#if RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_dest);
    FSP_ERROR_RETURN(RM_COMMS_USB_PCDC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    rm_comms_usb_pcdc_extended_cfg_t const * p_extend = p_ctrl->p_extend;

#if BSP_CFG_RTOS == 2
    if (NULL != p_extend->p_rx_mutex)
    {
        /* Acquire read mutex */
        err = rm_comms_recursive_mutex_acquire(p_extend->p_rx_mutex, p_extend->mutex_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    /* Use USB PDCD driver to read data */
    err = rm_comms_usb_pcdc_bus_read(p_extend->p_usb->p_ctrl, p_dest, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if BSP_CFG_RTOS == 2
    if (NULL != p_extend->p_rx_semaphore)
    {
        /* Wait for read to complete */
        err = rm_comms_semaphore_acquire(p_extend->p_rx_semaphore, p_ctrl->p_cfg->semaphore_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_rx_mutex)
    {
        /* Release read mutex */
        err = rm_comms_recursive_mutex_release(p_extend->p_rx_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Performs a write to the USB PCDC device. Implements @ref rm_comms_api_t::write.
 *
 * @retval FSP_SUCCESS              Successfully writing data .
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 *
 * @return See @ref RENESAS_ERROR_CODES or functions called by this function for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_Write (rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_usb_pcdc_instance_ctrl_t * p_ctrl = (rm_comms_usb_pcdc_instance_ctrl_t *) p_api_ctrl;

#if RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_src);
    FSP_ERROR_RETURN(RM_COMMS_USB_PCDC_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    rm_comms_usb_pcdc_extended_cfg_t const * p_extend = p_ctrl->p_extend;

#if BSP_CFG_RTOS == 2
    if (NULL != p_extend->p_tx_mutex)
    {
        /* Acquire write mutex */
        err = rm_comms_recursive_mutex_acquire(p_extend->p_tx_mutex, p_extend->mutex_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    /* Use USB PDCD driver to write data */
    err = rm_comms_usb_pcdc_bus_write(p_extend->p_usb->p_ctrl, p_src, bytes);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if BSP_CFG_RTOS == 2
    if (NULL != p_extend->p_tx_semaphore)
    {
        /* Wait for write to complete */
        err = rm_comms_semaphore_acquire(p_extend->p_tx_semaphore, p_ctrl->p_cfg->semaphore_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_extend->p_tx_mutex)
    {
        /* Release write mutex */
        err = rm_comms_recursive_mutex_release(p_extend->p_tx_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Performs a write to, then a read from the USB device. Implements @ref rm_comms_api_t::writeRead.
 *
 * @retval FSP_ERR_UNSUPPORTED      Not supported.
 *
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_WriteRead (rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl,
                                       rm_comms_usb_pcdc_write_read_params_t const write_read_params)
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(write_read_params);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup RM_COMMS_USB_PCDC)
 **********************************************************************************************************************/
void rm_comms_usb_pcdc_notify_application (rm_comms_usb_pcdc_instance_ctrl_t const * p_ctrl, rm_comms_usb_pcdc_event_t event)
{
    if (p_ctrl->p_callback)
    {
        rm_comms_usb_pcdc_callback_args_t args =
        {
            .p_context = p_ctrl->p_context,
            .event     = event,
        };

        p_ctrl->p_callback(&args);
    }
}

/*******************************************************************************************************************//**
 * @brief Common callback function called in the USB PCDC driver callback function.
 **********************************************************************************************************************/
#if BSP_CFG_RTOS == 2
void rm_comms_usb_pcdc_callback_handler (usb_ctrl_t * p_args,
                                        rtos_current_task_id_t usb_task_id,
                                        uint8_t usb_status)
#elif BSP_CFG_RTOS == 0
void rm_comms_usb_pcdc_callback_handler (usb_ctrl_t * p_args, usb_status_t event_info)
#endif
{
    rm_comms_usb_pcdc_instance_ctrl_t * p_ctrl = gp_comms_usb_pcdc_crtl;

#if BSP_CFG_RTOS == 2
    rm_comms_usb_pcdc_extended_cfg_t const * p_extend = p_ctrl->p_extend;
#endif

    p_ctrl->p_usb_args = p_args;

    switch(event_info)
    {
        case USB_STS_CONFIGURED:
        case USB_STS_RESUME:
        {
            rm_comms_usb_pcdc_notify_application(p_ctrl, RM_COMMS_USB_PCDC_EVENT_OPERATION_COMPLETE);
            break;
        }

        case USB_STS_WRITE_COMPLETE:
        {
#if BSP_CFG_RTOS == 2
            if (NULL != p_extend->p_tx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_tx_semaphore);
            }
#endif
            rm_comms_usb_pcdc_notify_application(p_ctrl, RM_COMMS_USB_PCDC_EVENT_TX_OPERATION_COMPLETE);
            break;
        }

        case USB_STS_READ_COMPLETE:
        {
#if BSP_CFG_RTOS == 2
            if (NULL != p_extend->p_rx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_rx_semaphore);
            }
#endif
            rm_comms_usb_pcdc_notify_application(p_ctrl, RM_COMMS_USB_PCDC_EVENT_RX_OPERATION_COMPLETE);
            break;
        }

        case USB_STS_REQUEST:   /* Receive Class Request */
        {
            /* Check for the specific CDC class request IDs */
            if (USB_PCDC_SET_LINE_CODING == (p_args->setup.type & USB_BREQUEST))
            {
                p_args->type = USB_REQUEST;
                R_USB_Read(p_args, (uint8_t *) &g_comms_usb_pcdc_line_coding, RM_COMMS_USB_PCDC_LINE_CODING_LENGTH);
            }
            else if (USB_PCDC_GET_LINE_CODING == (p_args->setup.type & USB_BREQUEST))
            {
                p_args->type = USB_REQUEST;
                R_USB_Write(p_args, (uint8_t *) &g_comms_usb_pcdc_line_coding, RM_COMMS_USB_PCDC_LINE_CODING_LENGTH);
            }
            else
            {
                p_args->type = USB_REQUEST;
                p_args->status = USB_ACK;
                R_USB_Write(p_args, (uint8_t*)USB_NULL, (uint32_t)USB_NULL);
            }
            break;
        }

        case USB_STS_REQUEST_COMPLETE:
        case USB_STS_DEFAULT:
        {
            break;
        }

        case USB_STS_SUSPEND:
        case USB_STS_DETACH:
        {
#if BSP_CFG_RTOS == 2
            if (NULL != p_extend->p_tx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_tx_semaphore);
            }

            if (NULL != p_extend->p_rx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_rx_semaphore);
            }
#endif
            break;
        }

        default:
        {
#if BSP_CFG_RTOS == 2
            if (NULL != p_extend->p_tx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_tx_semaphore);
            }

            if (NULL != p_extend->p_rx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_rx_semaphore);
            }
#endif
            break;
        }
    }
#if BSP_CFG_RTOS == 2
    FSP_PARAMETER_NOT_USED(usb_task_id);
    FSP_PARAMETER_NOT_USED(usb_status);
#endif
}

#if BSP_CFG_RTOS == 0
/*******************************************************************************************************************//**
  * @brief Timer callback function.
 **********************************************************************************************************************/
void rm_comms_usb_pcdc_timer_handler (void * pdata)
{
    usb_ctrl_t ctrl;
    g_usb_pcdc_event = R_USB_GetEvent(&ctrl);
    if (g_usb_pcdc_event != USB_STS_NONE)
    {
        rm_comms_usb_pcdc_callback_handler(&ctrl, g_usb_pcdc_event);
    }
}
#endif
