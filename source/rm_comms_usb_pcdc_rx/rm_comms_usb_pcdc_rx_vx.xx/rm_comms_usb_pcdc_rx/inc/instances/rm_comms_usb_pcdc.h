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


/*******************************************************************************************************************//**
 * @addtogroup RM_COMMS_USB_PCDC
 * @{
 **********************************************************************************************************************/

#ifndef RM_COMMS_USB_PCDC_H
#define RM_COMMS_USB_PCDC_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_comms_usb_pcdc_api.h"

#if   defined(__CCRX__) || defined(__ICCRX__)   || defined(__RX__)
 #include "rm_comms_usb_pcdc_api_complement.h"
 #include "rm_comms_usb_pcdc_rx_config.h"
#else
 #include "rm_comms_usb_pcdc_cfg.h"
#endif

#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
 #include "r_usb_basic_if.h"
 #include "r_usb_pcdc_if.h"
#elif COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC_MINI
 #include "r_usb_basic_mini_if.h"
 #include "r_usb_pcdc_mini_if.h"
#else
#endif
/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* USB bus configuration */
typedef struct st_rm_comms_usb_pcdc_extended_cfg
{
#if BSP_CFG_RTOS == 2
    void * const   p_tx_mutex;         ///< Lock device for writing.
    void * const   p_rx_mutex;         ///< Lock device for reading.
    uint32_t const mutex_timeout;      ///< Timeout for locking device.
    void * const   p_tx_semaphore;     ///< Block write operations. If this is NULL then operations will be non-blocking and require a callback.
    void * const   p_rx_semaphore;     ///< Block read operations. If this is NULL then operations will be non-blocking and require a callback.
#endif
    usb_instance_t * p_usb;            ///< Pointer to USB instance.
#if BSP_CFG_RTOS == 0
    cmt_instance_t const * p_cmt;      ///< Pointer to CMT instance.
#endif
} rm_comms_usb_pcdc_extended_cfg_t;

/* Communications middleware control structure. */
typedef struct st_rm_comms_usb_pcdc_instance_ctrl
{
    uint32_t                                 open;                      ///< Open flag.
    rm_comms_usb_pcdc_cfg_t const          * p_cfg;                     ///< Middleware configuration.
    rm_comms_usb_pcdc_extended_cfg_t const * p_extend;                  ///< Pointer to extended configuration structure
    usb_ctrl_t                             * p_usb_args;                ///< Pointer to usb callback args

    void (* p_callback)(rm_comms_usb_pcdc_callback_args_t * p_args);    ///< Pointer to callback that is called when a usb_status_t occurs.
    void const * p_context;                                             ///< Pointer to context passed into callback function
} rm_comms_usb_pcdc_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern rm_comms_usb_pcdc_api_t const        g_comms_on_comms_usb_pcdc;
/** @endcond */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t RM_COMMS_USB_PCDC_Open(rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl, rm_comms_usb_pcdc_cfg_t const * const p_cfg);
fsp_err_t RM_COMMS_USB_PCDC_Close(rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl);
fsp_err_t RM_COMMS_USB_PCDC_Read(rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t RM_COMMS_USB_PCDC_Write(rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes);
fsp_err_t RM_COMMS_USB_PCDC_WriteRead(rm_comms_usb_pcdc_ctrl_t * const            p_api_ctrl,
                                  rm_comms_usb_pcdc_write_read_params_t const write_read_params);
fsp_err_t RM_COMMS_USB_PCDC_CallbackSet(rm_comms_usb_pcdc_ctrl_t * const p_api_ctrl,
                                    void (                * p_callback)(rm_comms_usb_pcdc_callback_args_t *),
                                    void const * const      p_context);

void rm_comms_usb_pcdc_notify_application (rm_comms_usb_pcdc_instance_ctrl_t const * p_ctrl, rm_comms_usb_pcdc_event_t event);
#if BSP_CFG_RTOS == 2
void rm_comms_usb_pcdc_callback_handler (usb_ctrl_t * p_args, rtos_current_task_id_t usb_task_id, uint8_t usb_status);
#elif BSP_CFG_RTOS == 0
void rm_comms_usb_pcdc_callback_handler (usb_ctrl_t * p_args, usb_status_t event_info);
void rm_comms_usb_pcdc_timer_handler (void * pdata);
#endif

#endif                                 /* RM_COMMS_USB_PCDC_H */
/*******************************************************************************************************************//**
 * @} (end addtogroup RM_COMMS_USB_PCDC)
 **********************************************************************************************************************/
