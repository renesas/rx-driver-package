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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/******************************************************************************************************************//**
 * @addtogroup rm_comms_usb_pcdc_instance_rx
 * @{
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_smc_entry.h"
#include "rm_comms_usb_pcdc.h"
#include "../rm_comms_lock/rm_comms_lock.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* Callback function for USB PCDC Device */
void COMMS_USB_PCDC_CFG_CALLBACK(rm_comms_usb_pcdc_callback_args_t * p_args);

/* Callback function for Timer */
extern void rm_comms_usb_pcdc_timer_handler (void * pdata);

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#if BSP_CFG_RTOS == 0
cmt_instance_t g_comms_usb_pcdc0_timer_cfg =
{
    .freqency_hz = COMMS_USB_PCDC_CFG_TIMER_FREQUENCY,
    .channel     = COMMS_USB_PCDC_CFG_TIMER_CHANNEL,
    .priority    = COMMS_USB_PCDC_CFG_TIMER_PRIORITY,
    .callback    = rm_comms_usb_pcdc_timer_handler,
};
#endif

#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
usb_ctrl_t g_basic0_ctrl =
{
    .module     = USB_CFG_USE_USBIP,
    .type       = USB_PCDC,
};
#elif COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC_MINI
usb_ctrl_t g_basic0_ctrl =
{
    .type       = USB_PCDC,
};
#else
#endif

#if !defined(g_usb_descriptor)
extern usb_descriptor_t g_usb_descriptor;
#endif

usb_cfg_t g_basic0_cfg =
{
    .usb_mode = USB_CFG_MODE,
#if (defined(BSP_MCU_RX71M) && (USB_CFG_USE_USBIP == USB_CFG_IP1))
    .usb_speed = USB_HS,
#else
    .usb_speed = USB_FS,
#endif
#if defined(g_usb_descriptor)
    .p_usb_reg = g_usb_descriptor,
#else
    .p_usb_reg = &g_usb_descriptor,
#endif
};

/* Instance structure to use this module. */
usb_instance_t g_basic0 =
{
    .p_ctrl = &g_basic0_ctrl,
    .p_cfg = &g_basic0_cfg,
};

/* USB PCDC Communication Device */
rm_comms_usb_pcdc_instance_ctrl_t g_comms_usb_pcdc0_ctrl;

#if BSP_CFG_RTOS == 1 // ThreadX
 #if COMMS_USB_PCDC_CFG_RTOS_TX_MUTEX_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_usb_pcdc0_tx_mutex =
 {
     .p_name = "g_comms_usb_pcdc0 tx mutex",
 };
 #endif
 #if COMMS_USB_PCDC_CFG_RTOS_RX_MUTEX_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_usb_pcdc0_rx_mutex =
 {
     .p_name = "g_comms_usb_pcdc0 rx mutex",
 };
 #endif
 #if COMMS_USB_PCDC_CFG_RTOS_TX_SEMAPHORE_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_usb_pcdc0_tx_semaphore =
 {
     .p_name = "g_comms_usb_pcdc0 tx semaphore",
 };
 #endif
 #if COMMS_USB_PCDC_CFG_RTOS_RX_SEMAPHORE_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_usb_pcdc0_rx_semaphore =
 {
     .p_name = "g_comms_usb_pcdc0 rx semaphore",
 };
 #endif
#elif BSP_CFG_RTOS == 2 // FreeRTOS
 #if COMMS_USB_PCDC_CFG_RTOS_TX_MUTEX_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_usb_pcdc0_tx_mutex;
 #endif
 #if COMMS_USB_PCDC_CFG_RTOS_RX_MUTEX_SUPPORT_ENABLE
 rm_comms_mutex_t g_comms_usb_pcdc0_rx_mutex;
 #endif
 #if COMMS_USB_PCDC_CFG_RTOS_TX_SEMAPHORE_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_usb_pcdc0_tx_semaphore;
 #endif
 #if COMMS_USB_PCDC_CFG_RTOS_RX_SEMAPHORE_SUPPORT_ENABLE
 rm_comms_semaphore_t g_comms_usb_pcdc0_rx_semaphore;
 #endif
#else
#endif /* BSP_CFG_RTOS == 1 */

rm_comms_usb_pcdc_extended_cfg_t g_comms_usb_pcdc0_extended_cfg =
{
#if BSP_CFG_RTOS

#if COMMS_USB_PCDC_CFG_RTOS_TX_MUTEX_SUPPORT_ENABLE
    .p_tx_mutex = &g_comms_usb_pcdc0_tx_mutex,
#else
    .p_tx_mutex = NULL,
#endif

#if COMMS_USB_PCDC_CFG_RTOS_RX_MUTEX_SUPPORT_ENABLE
    .p_rx_mutex = &g_comms_usb_pcdc0_rx_mutex,
#else
    .p_rx_mutex = NULL,
#endif

#if COMMS_USB_PCDC_CFG_RTOS_TX_SEMAPHORE_SUPPORT_ENABLE
    .p_tx_semaphore = &g_comms_usb_pcdc0_tx_semaphore,
#else
    .p_tx_semaphore = NULL,
#endif

#if COMMS_USB_PCDC_CFG_RTOS_RX_SEMAPHORE_SUPPORT_ENABLE
    .p_rx_semaphore = &g_comms_usb_pcdc0_rx_semaphore,
#else
    .p_rx_semaphore = NULL,
#endif
    .mutex_timeout  = COMMS_USB_PCDC_CFG_MUTEX_TIMEOUT,
#endif
    .p_usb = &g_basic0,
#if BSP_CFG_RTOS == 0
    .p_cmt = &g_comms_usb_pcdc0_timer_cfg,
#endif
};

const rm_comms_usb_pcdc_cfg_t g_comms_usb_pcdc0_cfg =
{
    .semaphore_timeout = COMMS_USB_PCDC_CFG_SEMAPHORE_TIMEOUT,
    .p_lower_level_cfg = NULL,
    .p_extend = (void*) &g_comms_usb_pcdc0_extended_cfg,
    .p_callback = COMMS_USB_PCDC_CFG_CALLBACK,
};

const rm_comms_usb_pcdc_instance_t g_comms_usb_pcdc0 =
{
    .p_ctrl = &g_comms_usb_pcdc0_ctrl,
    .p_cfg  = &g_comms_usb_pcdc0_cfg,
    .p_api  = &g_comms_on_comms_usb_pcdc,
};
