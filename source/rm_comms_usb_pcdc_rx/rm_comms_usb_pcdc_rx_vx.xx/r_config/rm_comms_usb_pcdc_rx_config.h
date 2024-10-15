/* Generated configuration header file - do not edit */
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
/* Guards against multiple inclusion */
#ifndef R_COMMS_USB_PCDC_CONFIG_H
#define R_COMMS_USB_PCDC_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking */
/* Setting to 0 does not include parameter checking */
#define RM_COMMS_USB_PCDC_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY THE DEVICE LOCK OPERATION IN THE RTOS */
/* Setting to 1 enable a device lock operation */
/* Setting to 0 disable a device lock operation */
#define COMMS_USB_PCDC_CFG_RTOS_TX_MUTEX_SUPPORT_ENABLE         (0)

/* SPECIFY THE DEVICE LOCK OPERATION IN THE RTOS */
/* Setting to 1 enable a device lock operation */
/* Setting to 0 disable a device lock operation */
#define COMMS_USB_PCDC_CFG_RTOS_RX_MUTEX_SUPPORT_ENABLE         (0)

/* Timeout for recursive mutex operation in using RTOS. */
#define COMMS_USB_PCDC_CFG_MUTEX_TIMEOUT                        (0xFFFFFFFF)

/* SPECIFY BLOCKING OPERATION IN THE RTOS */
/* Setting to 1 enable blocking operation */
/* Setting to 0 disable blocking operation */
#define COMMS_USB_PCDC_CFG_RTOS_TX_SEMAPHORE_SUPPORT_ENABLE     (0)

/* SPECIFY BLOCKING OPERATION IN THE RTOS */
/* Setting to 1 enable blocking operation */
/* Setting to 0 disable blocking operation */
#define COMMS_USB_PCDC_CFG_RTOS_RX_SEMAPHORE_SUPPORT_ENABLE     (0)

/* Timeout for semaphore operation in using RTOS. */
#define COMMS_USB_PCDC_CFG_SEMAPHORE_TIMEOUT                    (0xFFFFFFFF)

/* Callback function */
#define COMMS_USB_PCDC_CFG_CALLBACK                             (rm_comms_usb_pcdc_user_callback)

/* Specify the timer channel. */
#define COMMS_USB_PCDC_CFG_TIMER_CHANNEL                        (1)

/* Specify the timer frequency (Hz). */
#define COMMS_USB_PCDC_CFG_TIMER_FREQUENCY                      (100)

/* Specify the timer priority level. */
#define COMMS_USB_PCDC_CFG_TIMER_PRIORITY                       (CMT_PRIORITY_14)

/* Specify the module used for Rm_comms */
#define COMMS_USB_PCDC_DEVICE_USED                              (USB_PCDC_BASIC)

#endif  /* R_COMMS_USB_PCDC_CONFIG_H */
