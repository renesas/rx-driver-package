/***********************************************************************************************************************
 * Copyright [2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

#ifndef RM_COMMS_USB_PCDC_API_COMPLEMENT_H
#define RM_COMMS_USB_PCDC_API_COMPLEMENT_H
/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define USB_PCDC_BASIC                        (0)
#define USB_PCDC_BASIC_MINI                   (1)

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_smc_entry.h"
#include "platform.h"

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
 #include "rm_comms_usb_pcdc_rx_config.h"
#else
 #include "rm_comms_usb_pcdc_cfg.h"
#endif

#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
 #include "r_usb_basic_if.h"
#elif COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC_MINI
 #include "r_usb_basic_mini_if.h"
#else
#endif

#if BSP_CFG_RTOS == 0
 #include "r_cmt_rx_if.h"
#endif

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#if BSP_CFG_RTOS == 0
/* CMT Driver Instance */
typedef struct st_cmt_instance
{
    uint32_t const          freqency_hz;            ///< Frequency [Hz]
    uint32_t const          channel;                ///< Channel
    cmt_priority_t const    priority;               ///< Desired priority level of timer interrupt
    void (* callback)       (void * pdata);         ///< Callback function
} cmt_instance_t;
#endif

/* This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_usb_instance
{
    usb_ctrl_t * p_ctrl;          ///< Pointer to the control structure for this instance
    usb_cfg_t  * p_cfg;           ///< Pointer to the configuration structure for this instance
} usb_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

#endif /* RM_COMMS_USB_PCDC_API_COMPLEMENT_H */
