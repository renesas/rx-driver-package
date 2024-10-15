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
 * Copyright (C) 2014(2024) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_basic_define.h
 * Description  : USB common macro define header
 ***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.09.2014 1.00 First Release
*         : 01.06.2015 1.01 Added RX231.
*         : 30.11.2018 1.10 Supporting Smart Configurator
*         : 31.05.2019 1.11 Added support for GNUC and ICCRX.
*         : 30.06.2019 1.12 RX23W is added.
*         : 30.06.2020 1.20 Added support for RTOS.
*         : 30.04.2024 1.30 Added support for RX261.
 ***********************************************************************************************************************/




/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* !!!!! WARNING--You can not edit this file. !!!!!*/
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#if defined(USB_DEBUG_ON)
    #include    <stdlib.h>          /* @@@MISRA del */
    #include    <stdio.h>           /* @@@MISRA del */
#endif  /* defined(USB_DEBUG_ON) */


/*****************************************************************************
 Macro definitions (for Preprocessor)
******************************************************************************/
#ifndef R_USB_BASIC_DEFINE_H
#define R_USB_BASIC_DEFINE_H

#define USB_CFG_LITTLE          (0u)
#define USB_CFG_BIG             (1u)

#if defined(__CCRX__)

  #ifdef __BIG
    #define    USB_CFG_ENDIAN   (USB_CFG_BIG)
  #else   /* __BIG */
    #define    USB_CFG_ENDIAN   (USB_CFG_LITTLE)
  #endif  /* __BIG */

#elif defined(__GNUC__)

  #ifdef __RX_BIG_ENDIAN__
    #define    USB_CFG_ENDIAN   (USB_CFG_BIG)
  #else   /* __RX_BIG_ENDIAN__ */
    #define    USB_CFG_ENDIAN   (USB_CFG_LITTLE)
  #endif  /* __RX_BIG_ENDIAN__ */

#elif defined(__ICCRX__)

  #if __BIG_ENDIAN__
    #define    USB_CFG_ENDIAN   (USB_CFG_BIG)
  #else   /* __BIG_ENDIAN__ */
    #define    USB_CFG_ENDIAN   (USB_CFG_LITTLE)
  #endif  /* __BIG_ENDIAN__ */

#endif /* defined(__CCRX__), defined(__GNUC__), defined(__ICCRX__) */

/* Select MCU type */
#ifdef BSP_MCU_RX111
    /* USB bus reset signal output waiting time */
    #define USB_TATTDB          (3000)    /* RX111 (Chapter 7.1.7.3 TATTDB) minimum duration of 100 ms */
#endif /* BSP_MCU_RX111 */
    
#ifdef BSP_MCU_RX113
    #define USB_TATTDB          (4500)    /* RX113 (Chapter 7.1.7.3 TATTDB) minimum duration of 100 ms */
#endif /* BSP_MCU_RX113 */

#ifdef BSP_MCU_RX231
    #define USB_TATTDB          (BSP_ICLK_HZ / 7000)     /* RX231(Chapter 7.1.7.3 TATTDB) minimum duration of 100 ms */
#endif /* BSP_MCU_RX231 */

#ifdef BSP_MCU_RX23W
    #define USB_TATTDB          (BSP_ICLK_HZ / 7000)     /* RX23W(Chapter 7.1.7.3 TATTDB) minimum duration of 100 ms */
#endif /* BSP_MCU_RX23W */

#ifdef BSP_MCU_RX261
    #define USB_TATTDB          (BSP_ICLK_HZ / 7000)     /* RX261(Chapter 7.1.7.3 TATTDB) minimum duration of 100 ms */
#endif /* BSP_MCU_RX261 */


/**********************************************************************************************************************
 * Macro definitions (USER DEFINE)
 **********************************************************************************************************************/
/* Version Number of API. */
#define USB_VERSION_MAJOR       (1)
#define USB_VERSION_MINOR       (20)

#define CLSDATASIZE             (255u)                  /* Transfer data size for Standard Request */
#if (BSP_CFG_RTOS_USED != 0)                            /* Use RTOS */
    /* The buffer size of interrupt info is increased to avoid overlapping interrupt events. */
    #define USB_INT_BUFSIZE     (64u)                   /* Size of Interrupt info buffer */
#else /* (BSP_CFG_RTOS_USED != 0) */
    #define USB_INT_BUFSIZE     (15u)                   /* Size of Interrupt info buffer */
#endif /* (BSP_CFG_RTOS_USED != 0) */
#define USB_EVENT_MAX           (15u)

/* Scheduler use define */
#define USB_FLGCLR              (0u)                    /* Flag clear */
#define USB_FLGSET              (1u)                    /* Flag set */
#define USB_IDCLR               (0xFFu)                 /* Priority clear */

/*******************************************************************************
 Task infomation
*******************************************************************************/
/* Please set with user system */
#define USB_IDMAX               ((uint8_t)5)            /* Maximum Task ID +1 */
#define USB_TABLEMAX            ((uint8_t)10)           /* Maximum priority table */
#define USB_BLKMAX              ((uint8_t)5)            /* Maximum block */
    
/* Task ID define */
#define USB_TID_0               (0u)                    /* Task ID 0 */
#define USB_TID_1               (1u)                    /* Task ID 1 */
#define USB_TID_2               (2u)                    /* Task ID 2 */
#define USB_TID_3               (3u)                    /* Task ID 3 */

/* Host Control Driver Task */
#define USB_HCD_TSK             (USB_TID_0)             /* Task ID */
#define USB_HCD_MBX             (USB_HCD_TSK)           /* Mailbox ID */

/* Host Manager Task */
#define USB_MGR_TSK             (USB_TID_1)             /* Task ID */
#define USB_MGR_MBX             (USB_MGR_TSK)           /* Mailbox ID */

/* Host Class Driver Task */
#define USB_HCLASS_TSK          (USB_TID_2)             /* Task ID */
#define USB_HCLASS_MBX          (USB_HCLASS_TSK)        /* Mailbox ID */

/******************************************************************************
 Macro definitions (Debug hook)
******************************************************************************/
/* Error discrimination */
#define USB_DEBUG_HOOK_HWR      (0x0100u)
#define USB_DEBUG_HOOK_HOST     (0x0200u)
#define USB_DEBUG_HOOK_PERI     (0x0400u)
#define USB_DEBUG_HOOK_STD      (0x0800u)
#define USB_DEBUG_HOOK_CLASS    (0x1000u)
#define USB_DEBUG_HOOK_APL      (0x2000u)

/* Error Code */
#define USB_DEBUG_HOOK_CODE1    (0x0001u)
#define USB_DEBUG_HOOK_CODE2    (0x0002u)
#define USB_DEBUG_HOOK_CODE3    (0x0003u)
#define USB_DEBUG_HOOK_CODE4    (0x0004u)
#define USB_DEBUG_HOOK_CODE5    (0x0005u)
#define USB_DEBUG_HOOK_CODE6    (0x0006u)
#define USB_DEBUG_HOOK_CODE7    (0x0007u)
#define USB_DEBUG_HOOK_CODE8    (0x0008u)
#define USB_DEBUG_HOOK_CODE9    (0x0009u)
#define USB_DEBUG_HOOK_CODE10   (0x000Au)
#define USB_DEBUG_HOOK_CODE11   (0x000Bu)
#define USB_DEBUG_HOOK_CODE12   (0x000Cu)
#define USB_DEBUG_HOOK_CODE13   (0x000Du)
#define USB_DEBUG_HOOK_CODE14   (0x000Eu)
#define USB_DEBUG_HOOK_CODE15   (0x000Fu)

#ifdef USB_DEBUG_HOOK_USE
    #define USB_DEBUG_HOOK(x)   (usb_cstd_debug_hook(x))
#else
    #define USB_DEBUG_HOOK(x)
#endif

/* H/W function type */
#define USB_HOST                (1u)     /* Host mode */
#define USB_PERI                (2u)     /* Peripheral mode */

#define USB_BITSET(x)           ((uint16_t)((uint16_t)1 << (x)))

/* Interrupt message num */
#define USB_INTMSGMAX           ((uint16_t)10u)

#define USB_STATIC  static      /* Not use #define USB_STATIC  */

#define USB_OK                  (0u)
#define USB_QOVR                (0xd5)          /* Submit overlap error */
#define USB_ERROR               (0xff)
#define USB_PAR                 (0xef)          /* parameter error */

#define USB_E_OK                (USB_OK)        /* Normal end */

#define USB_TRUE                (1u)
#define USB_FALSE               (0u)

#define USB_YES                 (1u)
#define USB_NO                  (0u)

#define USB_MAXPIPE_NUM         (9u)

/* Max device */
#define USB_MAXPIPE_BULK        (5u)
#define USB_MAXPIPE_ISO         (2u)
#define USB_MAXPIPE_NUM         (9u)

#define USB_BULK_PIPE_START     (1u)
#define USB_BULK_PIPE_END       (5u)
#define USB_INT_PIPE_START      (6u)
#define USB_INT_PIPE_END        (9u)
#define USB_ISO_PIPE_START      (1u)
#define USB_ISO_PIPE_END        (2u)

/* Reset Handshake result */
#define USB_NOCONNECT           (0xFFu)         /* Speed undecidedness */
#define USB_FSCONNECT           (0x01u)         /* Full-Speed connect */
#define USB_LSCONNECT           (0x03u)         /* Low-Speed connect */
#define USB_CONNECT             (0xF0u)

#define USB_SEQ_0               (0x00u)
#define USB_SEQ_1               (0x01u)
#define USB_SEQ_2               (0x02u)
#define USB_SEQ_3               (0x03u)
#define USB_SEQ_4               (0x04u)
#define USB_SEQ_5               (0x05u)
#define USB_SEQ_6               (0x06u)
#define USB_SEQ_7               (0x07u)
#define USB_SEQ_8               (0x08u)
#define USB_SEQ_9               (0x09u)
#define USB_SEQ_10              (0x0au)

#define USB_1                   (1u)
#define USB_2                   (2u)
#define USB_3                   (3u)
#define USB_4                   (4u)
#define USB_5                   (5u)
#define USB_6                   (6u)
#define USB_7                   (7u)
#define USB_8                   (8u)
#define USB_9                   (9u)

/* Root port */
#define USB_PORT0               (0u)

/* Device connect information */
#define USB_LNST_ATTACH         (0x0041u)
#define USB_LNST_DETACH         (0x0042u)
#define USB_RHST_ATTACH         (0x0043u)


/* Pipe define */
#define USB_IFISFIELD           (0x1000u)       /* Isochronous in-buf flash mode */
#define USB_IFISON              (0x1000u)
#define USB_IFISOFF             (0x0000u)
#define USB_IITVFIELD           (0x0007u)       /* Isochronous interval */
#define USB_IITV_TIME(x)        (x)

/* Start Pipe No */
#define USB_MIN_PIPE_NO         (1u)            /* PIPE1 ... */
#define USB_MAX_PIPE_NO         (9u)            /* ... PIPE9 */

/* USB Device address define */
#define USB_DEVICEADDR          (1u)            /* PORT0 USB Address (1 to 10) */

#define USB_PIPE_DIR_IN         (0u)
#define USB_PIPE_DIR_OUT        (1u)
#define USB_PIPE_DIR_MAX        (2u)

#define USB_CFG_ENABLE          (1u)
#define USB_CFG_DISABLE         (0u)

#define USB_CFG_HOST            (1)
#define USB_CFG_PERI            (2)

#define USB_CFG_CDC             (0)
#define USB_CFG_VEN             (1)

/* Channel Number */
#define USB_CFG_CH0             (0u)
#define USB_CFG_CH1             (1u)
#define USB_CFG_CH2             (2u)
#define USB_CFG_CH3             (3u)
#define USB_CFG_CH4             (4u)
#define USB_CFG_CH5             (5u)
#define USB_CFG_CH6             (6u)
#define USB_CFG_CH7             (7u)

#define USB_CFG_USE             (0u)
#define USB_CFG_NOUSE           (0xFFFFu)

#define USB_CFG_ON              (1u)
#define USB_CFG_OFF             (0u)

#define USB_CFG_HIGH            (0u)
#define USB_CFG_LOW             (1u)

#define USB_CFG_FS              (1)
#define USB_CFG_HS              (2)

#define USB_CFG_DBLBON          (0x0200u)
#define USB_CFG_DBLBOFF         (0x0000u)

/******************************************************************************
 USB specification define
 ******************************************************************************/

/* Standard Device Descriptor Define */
#define USB_DEV_B_LENGTH                    (0u)        /* Size of descriptor */
#define USB_DEV_B_DESCRIPTOR_TYPE           (1u)        /* Descriptor type */
#define USB_DEV_BCD_USB_L                   (2u)        /* USB Specification Release Number */
#define USB_DEV_BCD_USB_H                   (3u)        /* USB Specification Release Number */
#define USB_DEV_B_DEVICE_CLASS              (4u)        /* Class code */
#define USB_DEV_B_DEVICE_SUBCLASS           (5u)        /* Subclass code */
#define USB_DEV_B_DEVICE_PROTOCOL           (6u)        /* Protocol code */
#define USB_DEV_B_MAX_PACKET_SIZE_0         (7u)        /* Max packet size for EP0(only 8,16,32,64 are valid) */
#define USB_DEV_ID_VENDOR_L                 (8u)        /* Vendor ID */
#define USB_DEV_ID_VENDOR_H                 (9u)        /* Vendor ID */
#define USB_DEV_ID_PRODUCT_L                (10u)       /* Product ID */
#define USB_DEV_ID_PRODUCT_H                (11u)       /* Product ID */
#define USB_DEV_BCD_DEVICE_L                (12u)       /* Device release number */
#define USB_DEV_BCD_DEVICE_H                (13u)       /* Device release number */
#define USB_DEV_I_MANUFACTURER              (14u)       /* Index of string descriptor describing manufacturer */
#define USB_DEV_I_PRODUCT                   (15u)       /* Index of string descriptor describing product */
#define USB_DEV_I_SERIAL_NUMBER             (16u)       /* Device serial number */
#define USB_DEV_B_NUM_CONFIGURATION         (17u)       /* Number of possible configuration */

/* Standard Configuration Descriptor Define */
#define USB_DEV_W_TOTAL_LENGTH_L            (2u)        /* Total length of data returned for this configuration */
#define USB_DEV_W_TOTAL_LENGTH_H            (3u)        /* Total length of data returned for this configuration */
#define USB_DEV_B_NUM_INTERFACES            (4u)        /* Number of interfaces supported by this configuration */
#define USB_DEV_B_CONFIGURATION_VALUE       (5u)        /* Configuration value */
#define USB_DEV_I_CONFIGURATION             (6u)        /* Index of string descriptor describing this configuration */
#define USB_DEV_BM_ATTRIBUTES               (7u)        /* Configuration characteristics */
#define USB_DEV_B_MAX_POWER                 (8u)        /* Max power consumption of the USB device from the bus */

/* Endpoint Descriptor  Define */
#define USB_EP_DIRMASK                      (0x80u)     /* Endpoint direction mask [2] */
#define USB_EP_NUMMASK                      (0x0Fu)     /* Endpoint number mask [2] */
#define USB_EP_USGMASK                      (0x30u)     /* Usage type mask [2] */
#define USB_EP_SYNCMASK                     (0x0Cu)     /* Synchronization type mask [2] */
#define USB_EP_TRNSMASK                     (0x03u)     /* Transfer type mask [2] */
#define USB_EP_CNTRL                        (0x00u)     /* Control Transfer */

#define USB_EP_B_LENGTH                     (0u)      /* Size of descriptor */
#define USB_EP_B_DESCRIPTORTYPE             (1u)      /* Descriptor type */
#define USB_EP_B_ENDPOINTADDRESS            (2u)      /* Endpoint No. , Dir */
#define USB_EP_B_ATTRIBUTES                 (3u)      /* Transfer Type */
#define USB_EP_B_MAXPACKETSIZE_L            (4u)      /* Max packet size */
#define USB_EP_B_MAXPACKETSIZE_H            (5u)      /* Max packet size */
#define USB_EP_B_INTERVAL                   (6u)      /* Interval */

/* Standard Interface Descriptor Offset Define */
#define USB_IF_B_INTERFACENUMBER            (2u)      /* bInterfaceNumber */
#define USB_IF_B_ALTERNATESETTING           (3u)      /* bAlternateSetting */
#define USB_IF_B_NUMENDPOINTS               (4u)      /* bNumEndpoints */
#define USB_IF_B_INTERFACECLASS             (5u)      /* bInterfaceClass */
#define USB_IF_B_INTERFACESUBCLASS          (6u)      /* bInterfaceSubClass @@*/
#define USB_IF_B_INTERFACEPROTOCOL          (7u)      /* bInterfacePtorocol */
#define USB_IF_I_INTERFACE                  (8u)      /* iInterface */

/* GET_STATUS request information */
/* Standard Device status */
#define USB_GS_BUSPOWERD                    (0x0000u)
#define USB_GS_SELFPOWERD                   (0x0001u)
#define USB_GS_REMOTEWAKEUP                 (0x0002u)

/* Endpoint status */
#define USB_GS_NOTHALT                      (0x0000u)
#define USB_GS_HALT                         (0x0001u)

/* CLEAR_FEATURE/GET_FEATURE/SET_FEATURE request information */
/* Standard Feature Selector */
#define USB_ENDPOINT_HALT                   (0x0000u)
#define USB_DEV_REMOTE_WAKEUP               (0x0001u)
#define USB_TEST_MODE                       (0x0002u)

/* GET_DESCRIPTOR/SET_DESCRIPTOR request information */
/* Standard Descriptor type */
#define USB_HUB_DESCRIPTOR                  (0x0000u)
#define USB_DEV_DESCRIPTOR                  (0x0100u)
#define USB_CONF_DESCRIPTOR                 (0x0200u)
#define USB_STRING_DESCRIPTOR               (0x0300u)
#define USB_INTERFACE_DESCRIPTOR            (0x0400u)
#define USB_ENDPOINT_DESCRIPTOR             (0x0500u)
#define USB_DEV_QUALIFIER_DESCRIPTOR        (0x0600u)
#define USB_OTHER_SPEED_CONF_DESCRIPTOR     (0x0700u)
#define USB_INTERFACE_POWER_DESCRIPTOR      (0x0800u)

/* Device connect information */
#define USB_ATTACH                          (0x0040u)
#define USB_ATTACHL                         (0x0041u)
#define USB_ATTACHF                         (0x0042u)
#define USB_DETACH                          (0x0043u)

/* Reset Handshake result */
#define USB_DEVADD_NOCONNECT                (0x0000u)   /* Speed undecidedness */
#define USB_DEVADD_HSCONNECT                (0x00C0u)   /* Hi-Speed connect */
#define USB_DEVADD_FSCONNECT                (0x0080u)   /* Full-Speed connect */
#define USB_DEVADD_LSCONNECT                (0x0040u)   /* Low-Speed connect */

/* Pipe configuration table define */
#define USB_TYPFIELD                        (0xC000u)   /* Transfer type */
#define USB_PERIODIC                        (0x8000u)   /* Periodic pipe */
#define USB_TYPFIELD_ISO                    (0xC000u)   /* Isochronous */
#define USB_TYPFIELD_INT                    (0x8000u)   /* Interrupt */
#define USB_TYPFIELD_BULK                   (0x4000u)   /* Bulk */
#define USB_NOUSE                           (0x0000u)   /* Not configuration */
#define USB_BFREFIELD                       (0x0400u)   /* Buffer ready interrupt mode select */
#define USB_BFREON                          (0x0400u)
#define USB_BFREOFF                         (0x0000u)
#define USB_DBLBFIELD                       (0x0200u)   /* Double buffer mode select */
#define USB_SHTNAKFIELD                     (0x0080u)   /* Transfer end NAK */
#define USB_SHTNAKON                        (0x0080u)
#define USB_SHTNAKOFF                       (0x0000u)
#define USB_DIRFIELD                        (0x0010u)   /* Transfer direction select */
#define USB_DIR_H_OUT                       (0x0010u)   /* HOST OUT */
#define USB_DIR_P_IN                        (0x0010u)   /* PERI IN */
#define USB_DIR_H_IN                        (0x0000u)   /* HOST IN */
#define USB_DIR_P_OUT                       (0x0000u)   /* PERI OUT */
#define USB_BUF2FIFO                        (0x0010u)   /* Buffer --> FIFO */
#define USB_FIFO2BUF                        (0x0000u)   /* FIFO --> buffer */
#define USB_EPNUMFIELD                      (0x000Fu)   /* Endpoint number select */
#define USB_MAX_EP_NO                       (15u)       /* EP0 EP1 ... EP15 */

/* FIFO port & access define */
#define USB_CUSE                            (0u)  /* CFIFO  trans */
#define USB_D0USE                           (1u)  /* D0FIFO trans */
#define USB_D1USE                           (2u)  /* D1FIFO trans */
#define USB_FIFO_ACCESS_NUM_MAX             (3u)  /* MAX number for FIFO port & access define */

/******************************************************************************
 Another define
 ******************************************************************************/
/* FIFO read / write result */
#define USB_FIFOERROR                       (USB_ERROR)   /* FIFO not ready */
#define USB_WRITEEND                        (0x0000u)     /* End of write (but packet may not be outputting) */
#define USB_WRITESHRT                       (0x0001u)     /* End of write (send short packet) */
#define USB_WRITING                         (0x0002u)     /* Write continues */
#define USB_READEND                         (0x0000u)     /* End of read */
#define USB_READSHRT                        (0x0001u)     /* Insufficient (receive short packet) */
#define USB_READING                         (0x0002u)     /* Read continues */
#define USB_READOVER                        (0x0003u)     /* Buffer size over */

/* Pipe define table end code */
#define USB_PDTBLEND                        (0xFFFFu) /* End of table */

/* Transfer status Type */
#define USB_CTRL_END                        (0u)    /* Control transfer Normal termination */
#define USB_DATA_NONE                       (1u)
#define USB_DATA_OK                         (3u)    /* Data transfer Normal termination */
#define USB_DATA_SHT                        (4u)    /* Control transfer / Data transfer Buffer short reception */
#define USB_DATA_OVR                        (5u)    /* Control transfer / Data transfer Buffer over reception */
#define USB_DATA_STALL                      (6u)    /* Control transfer / Data transfer STALL detection */
#define USB_DATA_ERR                        (7u)    /* Exceptions of the FIFO access error, no response or etc. */
#define USB_DATA_STOP                       (8u)    /* Exceptions of the forced end */
#define USB_DATA_DTCH                       (9u)    /* Exceptions of the detach */
#define USB_DATA_TMO                        (10u)   /* Exceptions of the timeout */
#define USB_CTRL_READING                    (17u)
#define USB_CTRL_WRITING                    (18u)
#define USB_DATA_READING                    (19u)
#define USB_DATA_WRITING                    (20u)

/* Callback argument */
#define USB_NO_ARG                          (0u)

/* USB interrupt type (common)*/
#define USB_INT_UNKNOWN                     (0x00u)
#define USB_INT_BRDY                        (0x01u)
#define USB_INT_BEMP                        (0x02u)
#define USB_INT_NRDY                        (0x03u)
#define USB_INT_DXFIFO                      (0x04u)   /* BSP_CFG_RTOS_USED == 1 (FreeRTOS) */

/* USB interrupt type (PERI)*/
#define USB_INT_VBINT                       (0x0011u)
#define USB_INT_RESM                        (0x0012u)
#define USB_INT_SOFR                        (0x0013u)
#define USB_INT_DVST                        (0x0014u)
#define USB_INT_CTRT                        (0x0015u)
#define USB_INT_ATTACH                      (0x0016u)
#define USB_INT_DETACH                      (0x0017u)

/* USB interrupt type (HOST)*/
#define USB_INT_OVRCR0                      (0x41u)
#define USB_INT_BCHG0                       (0x42u)
#define USB_INT_DTCH0                       (0x43u)
#define USB_INT_ATTCH0                      (0x44u)
#define USB_INT_EOFERR0                     (0x45u)
#define USB_INT_PDDETINT0                   (0x46u)
#define USB_INT_OVRCR1                      (0x51u)
#define USB_INT_BCHG1                       (0x52u)
#define USB_INT_ATTCH1                      (0x53u)
#define USB_INT_DTCH1                       (0x54u)
#define USB_INT_EOFERR1                     (0x55u)
#define USB_INT_PDDETINT1                   (0x56u)
#define USB_INT_SACK                        (0x61u)
#define USB_INT_SIGN                        (0x62u)

#define USB_VBON                            (1u)
#define USB_VBOFF                           (0u)

#define USB_NOVENDOR                        (0xFFFFu) /* Vendor ID no check */
#define USB_NOPRODUCT                       (0xFFFFu) /* Product ID no check */

/* Root port */
#define USB_NOPORT                          (0xFFu) /* Not connect */

/* Max device */
#define USB_MAXDEVADDR                      (1u)

/* DEVICE address */
#define USB_DEVICE_0                        (0u)    /* Device address 0 */
#define USB_NODEVICE                        (0x0Fu) /* No device */
#define USB_DEVADDRBIT                      (12u)

/* DCP Max packetsize */
#define USB_MAXPFIELD                       (0x7Fu) /* Maxpacket size of DCP */

/* Device Address (DEVSEL) */
#define USB_ADDR2DEVSEL(x)                  ((uint16_t)((uint16_t)(x) << USB_DEVADDRBIT))
#define USB_DEVSEL2ADDR(x)                  ((usb_addr_t)((x) >> USB_DEVADDRBIT))

/* ControlPipe Max Packet size */
#define USB_DEFPACKET                       (0x0040u) /* Default DCP Max packet size */

/* Device state define */
#define USB_NONDEVICE                       (0u)
#define USB_NOTTPL                          (1u)
#define USB_DEVICEENUMERATION               (3u)
#define USB_COMPLETEPIPESET                 (10u)

/* Control Transfer Stage */
#define USB_IDLEST                          (0u)  /* Idle */
#define USB_SETUPNDC                        (1u)  /* Setup Stage No Data Control */
#define USB_SETUPWR                         (2u)  /* Setup Stage Control Write */
#define USB_SETUPRD                         (3u)  /* Setup Stage Control Read */
#define USB_DATAWR                          (4u)  /* Data Stage Control Write */
#define USB_DATARD                          (5u)  /* Data Stage Control Read */
#define USB_STATUSRD                        (6u)  /* Status stage */
#define USB_STATUSWR                        (7u)  /* Status stage */
#define USB_SETUPWRCNT                      (17u) /* Setup Stage Control Write */
#define USB_SETUPRDCNT                      (18u) /* Setup Stage Control Read */
#define USB_DATAWRCNT                       (19u) /* Data Stage Control Write */
#define USB_DATARDCNT                       (20u) /* Data Stage Control Read */

/*  USB Manager mode    */
#define USB_DETACHED                        (10u)     /* Disconnect(VBUSon) */
#define USB_POWERED                         (30u)     /* Start reset handshake */
#define USB_DEFAULT                         (40u)     /* Set device address */
#define USB_ADDRESS                         (50u)     /* Enumeration start */
#define USB_CONFIGURED                      (70u)     /* Detach detected */
#define USB_SUSPENDED                       (80u)     /* Device suspended */
#define USB_RESUME_PROCESS                  (103u)    /* Wait device resume */

/* HCD common task message command */
#define USB_MSG_HCD_ATTACH                  (0x0101u)
#define USB_MSG_HCD_DETACH                  (0x0102u)
#define USB_MSG_HCD_USBRESET                (0x0103u)
#define USB_MSG_HCD_SUSPEND                 (0x0104u)
#define USB_MSG_HCD_RESUME                  (0x0105u)
#define USB_MSG_HCD_REMOTE                  (0x0106u)
#define USB_MSG_HCD_VBON                    (0x0107u)
#define USB_MSG_HCD_VBOFF                   (0x0108u)
#define USB_MSG_HCD_CLR_STALL               (0x0109u)
#define USB_MSG_HCD_DETACH_MGR              (0x010Au)
#define USB_MSG_HCD_ATTACH_MGR              (0x010Bu)

#define USB_MSG_HCD_CLR_STALLBIT            (0x010Du)
#define USB_MSG_HCD_SQTGLBIT                (0x010Fu)

/* HCD task message command */
#define USB_MSG_HCD_SUBMITUTR               (0x0112u)
#define USB_MSG_HCD_TRANSEND1               (0x0113u)
#define USB_MSG_HCD_TRANSEND2               (0x0114u)
#define USB_MSG_HCD_CLRSEQBIT               (0x0115u)
#define USB_MSG_HCD_SETSEQBIT               (0x0116u)
#define USB_MSG_HCD_INT                     (0x0117u)

/* Davice state control command define (MGR2HCD) */
#define USB_HCD_USBRESET                    (0x41u)
#define USB_HCD_SUSPEND                     (0x42u)
#define USB_HCD_RESUME                      (0x43u)
#define USB_HCD_VBON                        (0x44u)
#define USB_HCD_VBOFF                       (0x45u)
#define USB_HCD_CLEAR_STALL                 (0x46u)

/* API function command define */
#define USB_INTERRUPT                       (0x01u)


/* USB Manager task message command */
#define USB_MSG_MGR_AORDETACH               (0x0121u)
#define USB_MSG_MGR_OVERCURRENT             (0x0122u)
#define USB_MSG_MGR_STATUSRESULT            (0x0123u)
#define USB_MSG_MGR_SUBMITRESULT            (0x0124u)

/* Davice state control command for API function(HOST/PERI common define) */
#define USB_DO_TRANSFERSTART                (0x11u)
#define USB_DO_TRANSFEREND                  (0x12u)
#define USB_DO_TRANSFER_STP                 (0x13u)     /* Transfer stop */

/* Alternate number */
#define USB_ALT_NO                          (255u)

/* USB Peripheral task message command */
#define USB_MSG_PCD_INT                     (0x0151u)
#define USB_MSG_PCD_SUBMITUTR               (0x0152u)
#define USB_MSG_PCD_TRANSEND1               (0x0153u)
#define USB_MSG_PCD_TRANSEND2               (0x0154u)
#define USB_MSG_PCD_REMOTEWAKEUP            (0x0155u)
#define USB_MSG_PCD_PCUTINT                 (0x0156u)
#define USB_MSG_PCD_DP_ENABLE               (0x0157u)
#define USB_MSG_PCD_DP_DISABLE              (0x0158u)
#define USB_MSG_PCD_DM_ENABLE               (0x0159u)
#define USB_MSG_PCD_DM_DISABLE              (0x015Au)
#define USB_MSG_PCD_DETACH                  (0x0161u)
#define USB_MSG_PCD_ATTACH                  (0x0162u)
#define USB_MSG_PCD_CLRSEQBIT               (0x0163u)
#define USB_MSG_PCD_SETSTALL                (0x0164u)

/* Davice state control command for API function(R_USB_PstdChangeDeviceState) */
#define USB_DO_REMOTEWAKEUP                 (0x21u)
#define USB_DP_ENABLE                       (USB_MSG_PCD_DP_ENABLE)
#define USB_DP_DISABLE                      (USB_MSG_PCD_DP_DISABLE)
#define USB_DM_ENABLE                       (USB_MSG_PCD_DM_ENABLE)
#define USB_DM_DISABLE                      (USB_MSG_PCD_DM_DISABLE)

#define USB_DO_STALL                        (USB_MSG_PCD_SETSTALL)

#if (BSP_CFG_RTOS_USED != 0)                            /* Use RTOS */
#define USB_DO_RESET_AND_ENUMERATION        (0x0202u) /* USB_MSG_HCD_ATTACH */
#define USB_PORT_ENABLE                     (0x0203u) /* USB_MSG_HCD_VBON */
#define USB_PORT_DISABLE                    (0x0204u) /* USB_MSG_HCD_VBOFF */
#define USB_DO_GLOBAL_SUSPEND               (0x0205u) /* USB_MSG_HCD_SUSPEND */
#define USB_DO_SELECTIVE_SUSPEND            (0x0206u) /* USB_MSG_HCD_SUSPEND */
#define USB_DO_GLOBAL_RESUME                (0x0207u) /* USB_MSG_HCD_RESUME */
#define USB_DO_SELECTIVE_RESUME             (0x0208u) /* USB_MSG_HCD_RESUME */
#define USB_DO_CLR_STALL                    (0x0209u) /* USB_MSG_HCD_CLR_STALL */
#define USB_DO_SET_SQTGL                    (0x020Au) /* USB_MSG_HCD_SQTGLBIT */
#define USB_DO_CLR_SQTGL                    (0x020Bu) /* USB_MSG_HCD_CLRSEQBIT */
#else /* (BSP_CFG_RTOS_USED != 0) */
/* Sequence control command define (HCD2MGR/UPL2MGR) */
#define USB_MGR_CONTINUE                    (0x30u)     /* Continues sequnce */
#define USB_RTP_DETACH                      (0x31u)     /* USB_INT_DTCH */
#define USB_RTP_ATTACH                      (0x32u)     /* USB_INT_ATTCH */
#define USB_RTP_OVERCURRENT                 (0x33u)     /* USB_INT_OVRCR */
#define USB_RTP_REMOTEWAKEUP                (0x34u)     /* USB_INT_BCHG */
#define USB_DO_SELECTIVE_SUSPEND            (0x35u)     /* SUSPEND control (device select)*/
#define USB_DO_SELECTIVE_RESUME             (0x36u)     /* RESUME control (device select) */
#define USB_DO_PORT_ENABLE                  (0x37u)     /* VBOUT control */
#define USB_DO_PORT_DISABLE                 (0x38u)     /* VBOUT control */
#define USB_DO_CLEAR_STALL                  (0x39u)     /* Clear_Feature request */
#define USB_DO_GLOBAL_SUSPEND               (0x3Au)     /* SUSPEND control (port select)*/
#define USB_DO_GLOBAL_RESUME                (0x3Bu)     /* RESUME control (port select) */
#define USB_MGR_NOSEQUENCE                  (0x3Cu)
#endif /* (BSP_CFG_RTOS_USED != 0) */

/******************************************************************************
 Sequence control
******************************************************************************/
#define USB_NOSEQE0         (0x00u)
#define USB_NOSEQE1         (0x01u)
#define USB_ATTACH0         (0x02u)
#define USB_ENUMER1         (0x03u)
#define USB_SUSPEN2         (0x04u)
#define USB_SUSPEN0         (0x05u)
#define USB_SUSPEN1         (0x06u)
#define USB_RESUME2         (0x07u)
#define USB_RESUME0         (0x08u)
#define USB_RESUME1         (0x09u)
#define USB_REMOTE0         (0x0Au)
#define USB_REMOTE1         (0x0Bu)
#define USB_DETACH0         (0x0Cu)
#define USB_DETACH1         (0x0Du)
#define USB_DETACH2         (0x0Eu)
#define USB_OVERCU0         (0x0Fu)
#define USB_PORT_E0         (0x10u)
#define USB_PORT_E1         (0x11u)
#define USB_PORT_D0         (0x12u)
#define USB_PORT_D1         (0x13u)
#define USB_CLRSTL0         (0x14u)
#define USB_CLRSTL1         (0x15u)


/******************************************************************************
 Host Battery Charging Define
******************************************************************************/
/* BC State Define */
#define USB_BC_STATE_INIT                   (0x00u)
#define USB_BC_STATE_DET                    (0x01u)
#define USB_BC_STATE_CDP                    (0x02u)
#define USB_BC_STATE_SDP                    (0x03u)
#define USB_BC_STATE_DCP                    (0x04u)
#define USB_BC_STATE_MAX                    (0x05u)

/* BC State Change Event Define */
#define USB_BC_EVENT_VB                     (0x00u)
#define USB_BC_EVENT_AT                     (0x01u)
#define USB_BC_EVENT_DT                     (0x02u)
#define USB_BC_EVENT_MAX                    (0x03u)


/******************************************************************************
 System call macro definitions
******************************************************************************/
#define USB_SND_MSG(ID, MESS)           (usb_hstd_snd_msg((uint8_t)(ID), (usb_msg_t*)(MESS)))
#define USB_ISND_MSG(ID, MESS)          (usb_hstd_isnd_msg((uint8_t)(ID), (usb_msg_t*)(MESS)))
#define USB_WAI_MSG(ID, MESS, TM)       (usb_hstd_wai_msg((uint8_t)(ID), (usb_msg_t*)(MESS), (usb_tm_t)(TM)))
#define R_USB_RCV_MSG(ID, MESS)         (usb_hstd_rec_msg((uint8_t)(ID), (usb_msg_t**)(MESS)))
#define USB_GET_SND(ID, MSG, CLB, KEY)  (usb_hstd_pget_send( (uint8_t)(ID), (usb_strct_t)(MSG), \
                                                               (usb_cbinfo_t) (CLB), (usb_strct_t)(KEY) ))
#define R_USB_REL_BLK(BLK)              (usb_hstd_rel_blk((uint8_t)(BLK)))

#define usb_hstd_scheduler              R_usb_cstd_Scheduler

/* Descriptor size */
#define USB_DEVICESIZE                  (20u)   /* Device Descriptor size */
#define USB_CONFIGSIZE                  (256u)  /* Configuration Descriptor size */

/* Number of software retries when a no-response condition occurs during a transfer */
#define USB_PIPEERROR                   (1u)

#endif  /* R_USB_BASIC_DEFINE_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/
