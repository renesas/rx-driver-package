/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_hmsc.h
* Description  : USB User definition Pre-pro
******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.09.2014 1.00    First Release
 *         : 01.06.2015 1.01    Added RX231.
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"

#ifndef R_USB_HMSC_H
#define R_USB_HMSC_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Host Sample Task */
#define USB_HSTRG_TSK                   (USB_TID_3)           /* Task ID */
#define USB_HSTRG_MBX                   (USB_HSTRG_TSK)       /* Mailbox ID */

#define R_usb_hmsc_Task                 (usb_hstd_class_task)
#define R_usb_hstd_HcdTask              (usb_hstd_hcd_task)
#define R_usb_hstd_MgrTask              (usb_hstd_mgr_task)
#define R_usb_hstd_DeviceInformation    (usb_hstd_device_information)
#define R_usb_hmsc_StrgReadSector       (usb_hmsc_strg_read_sector)
#define R_usb_hmsc_StrgWriteSector      (usb_hmsc_strg_write_sector)
#define usb_ghmsc_RootDevaddr           (g_usb_hmsc_root_devaddr)

/* CBW Structure define.    */
#define USB_MSC_CBWLENGTH               (31u)
#define USB_MSC_CBWCB_LENGTH            (12u)
#define USB_MSC_CSW_LENGTH              (13u)

/* CPU bit endian select (BIT_LITTLE:little, BIT_BIG:big) */
#if USB_CFG_ENDIAN == USB_CFG_BIG
 #define    USB_MSC_CBW_SIGNATURE       ((uint32_t)0x55534243)
 #define    USB_MSC_CSW_SIGNATURE       ((uint32_t)0x55534253)
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
 #define    USB_MSC_CBW_SIGNATURE       ((uint32_t)0x43425355)
 #define    USB_MSC_CSW_SIGNATURE       ((uint32_t)0x53425355)
#endif /* USB_CFG_ENDIAN == USB_CFG_BIG */

#define USB_MSG_HMSC_NO_DATA                (0x51u)
#define USB_MSG_HMSC_DATA_IN                (0x52u)
#define USB_MSG_HMSC_DATA_OUT               (0x53u)
#define USB_MSG_HMSC_CSW_ERR                (0x54u)
#define USB_MSG_HMSC_CSW_PHASE_ERR          (0x55u)
#define USB_MSG_HMSC_CBW_ERR                (0x56u)

#define USB_MSG_HMSC_STRG_DRIVE_SEARCH      (0x61u)
#define USB_MSG_HMSC_STRG_DRIVE_SEARCH_END  (0x62u)
#define USB_MSG_HMSC_STRG_USER_COMMAND      (0x67u)
#define USB_MSG_HMSC_STRG_RW_END            (0x66u)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

typedef void    (*usb_hmsc_cb_t)(usb_tskinfo_t * p_complete);

/* CBW Structure define.    */
R_BSP_PRAGMA_UNPACK
typedef struct
{
    uint32_t    dcbw_signature;
    uint32_t    dcbw_tag;
    uint8_t     dcbw_dtl_lo;
    uint8_t     dcbw_dtl_ml;
    uint8_t     dcbw_dtl_mh;
    uint8_t     dcbw_dtl_hi;
    union {
        uint8_t    BYTE;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_2(
            uint8_t reserved7:7,
            uint8_t cbw_dir:1
        )BIT;
    } bm_cbw_flags;
    union {
        uint8_t    BYTE;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_2(
            uint8_t bcbw_lun:4,
            uint8_t reserved4:4
        )BIT;
    } bcbw_lun;
    union {
        uint8_t    BYTE;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_2(
            uint8_t bcbwcb_length:5,
            uint8_t reserved3:3
        )BIT;
    } bcbwcb_length;
    uint8_t cbwcb[(16 + 33)];
}
USB_MSC_CBW_t;
R_BSP_PRAGMA_PACKOPTION

/* CSW Structure define define. */
typedef struct
{
    uint32_t    dcsw_signature;
    uint32_t    dcsw_tag;
    uint8_t     dcsw_data_residue_lo;
    uint8_t     dcsw_data_residue_ml;
    uint8_t     dcsw_data_residue_mh;
    uint8_t     dcsw_data_residue_hi;
    uint8_t     dcsw_status;
    uint8_t     dummy[51];      /* DTC Receive Size dummy (MAX Packet size = 64Byte) */
}
USB_MSC_CSW_t;

/* CSW STATUS */
enum usb_gcmsc_CswSts
{
    USB_MSC_CSW_OK          = (0x00u),
    USB_MSC_CSW_NG          = (0x01u),
    USB_MSC_CSW_PHASE_ERR   = (0x02u)
};


/* ERROR CODE */
enum usb_ghmsc_Error
{
    USB_HMSC_OK             = (0),
    USB_HMSC_STALL          = (0xff),
    USB_HMSC_CBW_ERR        = (0xfe),     /* CBR error */
    USB_HMSC_DAT_RD_ERR     = (0xfd),     /* Data IN error */
    USB_HMSC_DAT_WR_ERR     = (0xfc),     /* Data OUT error */
    USB_HMSC_CSW_ERR        = (0xfb),     /* CSW error */
    USB_HMSC_CSW_PHASE_ERR  = (0xfa),     /* Phase error */
    USB_HMSC_SUBMIT_ERR     = (0xf9),     /* submit error */
};

/* USB Mass Storage Device Class Request code define. */
enum usb_gpmsc_Request
{
    USB_MASS_STORAGE_RESET  = (0xFF00u), /* Mass dtorage reset request. */
    USB_GET_MAX_LUN         = (0xFE00u), /* Get max logical unit number */
};

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

extern uint8_t          g_usb_hmsc_data[];


/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

extern uint16_t     usb_hmsc_get_device_status (void);
extern uint16_t     usb_hmsc_read10 (uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
extern uint16_t     usb_hmsc_write10 (uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
extern uint16_t     usb_hmsc_strg_drive_search (usb_hmsc_cb_t complete);
extern uint16_t     usb_hmsc_strg_drive_close (void);
extern void         usb_hmsc_strg_drive_task (void);
extern usb_er_t     usb_hmsc_get_max_unit (usb_hcb_t complete);
extern uint16_t     usb_hmsc_inquiry (uint8_t *p_buff);
extern uint16_t     usb_hmsc_mode_select10 (uint8_t *p_buff);
extern uint16_t     usb_hmsc_mode_sense10 (uint8_t *p_buff);
extern uint16_t     usb_hmsc_prevent_allow (uint8_t *p_buff);
extern uint16_t     usb_hmsc_read_capacity (uint8_t *p_buff);
extern uint16_t     usb_hmsc_read_format_capacity (uint8_t *p_buff);
extern uint16_t     usb_hmsc_request_sense (uint8_t *p_buff);
extern uint16_t     usb_hmsc_test_unit (void);
extern uint16_t     usb_hmsc_strg_user_command (uint16_t command, uint8_t *p_buff, usb_hmsc_cb_t complete);
extern void         usb_hstd_device_information (usb_addr_t addr, uint16_t *p_table);
extern void         usb_cstd_pipe_reg_set (void);
extern uint8_t      usb_cstd_pipe_table_set (uint16_t usb_class, uint8_t *p_descriptor);
extern void         usb_hstd_class_check (uint8_t **pp_table);

extern void         usb_hstd_mgr_task(void);

#endif  /* R_USB_HMSC_H */
/******************************************************************************
End  Of File
******************************************************************************/
