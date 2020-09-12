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
 * Copyright (C) 2015(2019) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_basic_config_reference.h
 * Description  : USB User definition
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 31.08.2015 1.00    First Release
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ***********************************************************************************************************************/

#ifndef R_USB_BASIC_CONFIG_H
#define R_USB_BASIC_CONFIG_H

/******************************************************************************/
/** Common Settings in USB Host/USB Peripheral Mode                           */
/******************************************************************************/

/** [USB operating mode setting]
 *  USB_CFG_HOST        : USB Host mode.
 *  USB_CFG_PERI        : USB Peri mode.
 */
#define USB_CFG_MODE                (USB_CFG_HOST)

/** [Argument check setting]
 *  USB_CFG_ENABLE      :Checks arguments.
 *  USB_CFG_DISABLE     :Does not check arguments.
 */
#define USB_CFG_PARAM_CHECKING      (USB_CFG_ENABLE)

/** [Device class setting]
 *  #define USB_CFG_HCDC_USE : Host Communication Device Class
 *  #define USB_CFG_HHID_USE : Host Human Interface Device Class
 *  #define USB_CFG_HMSC_USE : Host Mass Storage Class
 *  #define USB_CFG_HVND_USE : Host VENDER Class
 *  #define USB_CFG_PCDC_USE : Peripheral Communication Device Class
 *  #define USB_CFG_PHID_USE : Peripheral Human Interface Device Class
 *  #define USB_CFG_PMSC_USE : Peripheral Mass Storage Class
 *  #define USB_CFG_PVND_USE : Peripheral VENDER Class
 * */
#define USB_CFG_HCDC_USE    /* USB_CFG_DEVICE_CLASS */

/**  [DTC use setting]
 * USB_CFG_ENABLE       : Uses DTC
 * USB_CFG_DISABLE      : Does not use DTC
 */
#define    USB_CFG_DTC              (USB_CFG_DISABLE)

/**  [DMA use setting]
 * USB_CFG_ENABLE       : Uses DMA
 * USB_CFG_DISABLE      : Does not use DMA
 */
#define    USB_CFG_DMA              (USB_CFG_DISABLE)

/** [DMA Channel setting(USB0 module Send transfer)]
 * USB_CFG_CH0          : Uses DMAC0
 * USB_CFG_CH1          : Uses DMAC1
 * USB_CFG_CH2          : Uses DMAC2
 * USB_CFG_CH3          : Uses DMAC3
 */
#define     USB_CFG_USB0_DMA_TX     (USB_CFG_CH0)

/** [DMA Channel setting(USB0 module Receive transfer)]
 * USB_CFG_CH0          : Uses DMAC0
 * USB_CFG_CH1          : Uses DMAC1
 * USB_CFG_CH2          : Uses DMAC2
 * USB_CFG_CH3          : Uses DMAC3
 */
#define     USB_CFG_USB0_DMA_RX     (USB_CFG_CH1)

/** [Setting Battery Charging (BC) function]
 * USB_CFG_ENABLE       : Uses BC function.
 * USB_CFG_DISABLE      : Does not use BC function.
 */
#define USB_CFG_BC                  (USB_CFG_DISABLE)

/**
 * SET USB INTERRUPT PRIORITY; 
 *  1                   : lowest
 *  15                  : highest
 */
#define USB_CFG_INTERRUPT_PRIORITY  (3)

/** [USB Regulator use setting]
 * USB_CFG_ENABLE       : Uses USB Regulator
 * USB_CFG_DISABLE      : Does not use USB Regulator
 */
#define     USB_CFG_REGULATOR       (USB_CFG_DISABLE)



/******************************************************************************/
/** Settings in USB Host Mode                                                 */
/******************************************************************************/

/** [Setting power source IC for USB Host]
 * USB_CFG_HIGH         : High assert
 * USB_CFG_LOW          : Low assert
 */
#define USB_CFG_VBUS                (USB_CFG_HIGH)

/** [Setting USB port operation when using Battery Charging (BC) function]
 * USB_CFG_ENABLE       : DCP enabled.
 * USB_CFG_DISABLE      : DCP disabled.
 */
#define USB_CFG_DCP                 (USB_CFG_DISABLE)

/** [Setting whther to use Type-C]
 * USB_CFG_ENABLE       : Using Type-C
 * USB_CFG_DISABLE      : Not using Type-C
 */
#define USB_CFG_TYPEC               (UCB_CFG_DISABLE)

/** [Setting Compliance Test mode]
 * USB_CFG_ENABLE       : Compliance Test supported.
 * USB_CFG_DISABLE      : Compliance Test not supported.
 */
#define USB_CFG_COMPLIANCE          (USB_CFG_DISABLE)

/** [Setting a Targeted Peripheral List (TPL)]
 * USB_CFG_TPLCNT       : Number of the USB devices to be connected.
 * USB_CFG_TPL          : Set the VID and PID pairs for the USB device to be connected.
 * */
#define USB_CFG_TPLCNT              (1)
#define USB_CFG_TPL                 USB_NOVENDOR, USB_NOPRODUCT



/******************************************************************************/
/** Settings in USB Peripheral Mode                                                 */
/******************************************************************************/

/** [Setting notification of request(SET_INTERFACE/SET_FEATURE/CLEAR_FEATURE) ]
 * USB_CFG_ENABLE       : Notification
 * USB_CFG_DISABLE      : Not Notification
 */
#define USB_CFG_REQUEST             (USB_CFG_DISABLE)

/******************************************************************************/
/** OTHER DEFINITIONS SETTING                                                 */
/******************************************************************************/
/** [DBLB bit setting]
 * USB_CFG_DBLBON       : DBLB bit set.
 * USB_CFG_DBLBOFF      : DBLB bit cleared.
 */
#define USB_CFG_DBLB                (USB_CFG_DBLBON)

#endif  /* R_USB_BASIC_CONFIG_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/

