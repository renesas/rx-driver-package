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
* Copyright (C) 2016(2020) Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_cstd_rtos.h
* Version      : 1.0
* Description  : Create tasks, mailboxes, memory pool for USB in freeRTOS.
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.06.2016 1.00     First Release
*         : 16.11.2018 1.24     Supporting RTOS Thread safe
*         : 01.03.2020 1.30     RX72N/RX66N is added and uITRON is supported.
******************************************************************************/

#ifndef R_USB_RTOS_H
#define R_USB_RTOS_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_basic_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_rtos_abstract.h"

#if BSP_CFG_RTOS_USED == 4      /* Renesas RI600V4 & RI600PX */
#include "kernel.h"
#include "kernel_id.h"
#endif /* BSP_CFG_RTOS_USED == 4 */

#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_if.h"
#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc.h"
#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid.h"
#endif /* defined(USB_CFG_HHID_USE) */

#if BSP_CFG_RTOS_USED != 0      /* Use RTOS */
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define QUEUE_SIZE              (16)

#if BSP_CFG_RTOS_USED == 1      /* FreeRTOS */
/** USB task's priority **/
#define HCD_TSK_PRI             (11)
#define HUB_TSK_PRI             (9)
#define MGR_TSK_PRI             (10)
#define PCD_TSK_PRI             (11)
#define PMSC_TSK_PRI            (8)
#define HCDC_TSK_PRI            (8)
#define HHID_TSK_PRI            (8)

#define usb_rtos_err_t          rtos_err_t
#define UsbRtos_Success         RTOS_SUCCESS

#elif BSP_CFG_RTOS_USED == 2    /* SEGGER embOS */
/** USB task's priority **/
#elif BSP_CFG_RTOS_USED == 3    /* Micrium MicroC/OS */
/** USB task's priority **/

/** USB mailbox's pointer to a message queue control block**/

/** USB mailbox's pointer to the name of the message queue **/

/** USB mailbox's specifies the size of each message in the queue.
 * Message size range from 1 32-bit word to 16 32-bit words.
 * Valid message size options are defined as follows **/

/** USB mailbox's starting address of the message queue **/

/** USB mailbox's total number of bytes available for the message queue **/

#elif BSP_CFG_RTOS_USED == 4    /* Renesas RI600V4 & RI600PX */
/** USB task's priority **/
#define HCD_TSK_PRI             (1)
#define HUB_TSK_PRI             (3)
#define MGR_TSK_PRI             (2)
#define PCD_TSK_PRI             (1)
#define PMSC_TSK_PRI            (4)
#define HCDC_TSK_PRI            (4)
#define HHID_TSK_PRI            (4)
#else
#endif  /* BSP_CFG_RTOS_USED == 1 */

/** USB task stack size in words **/
#define HCD_STACK_SIZE          (512)
#define HUB_STACK_SIZE          (512)
#define MGR_STACK_SIZE          (512)
#define PCD_STACK_SIZE          (512)
#define PMSC_STACK_SIZE         (512)
#define HCDC_STACK_SIZE         (512)
#define HHID_STACK_SIZE         (512)

/** USB task stack size in words **/
#define STACK_SIZE              (512)

#define NUM_OF_BLOCK            (64u)
#define NUM_OF_MBX              (16u)
#define NUM_OF_TASK             (7)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

extern  rtos_mbx_id_t   g_rtos_usb_hcd_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_hcd_sub_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_hcd_sub_addr_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_mgr_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_hub_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_cls_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_hmsc_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_hmsc_req_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_hhid_mbx_id;

extern  rtos_mbx_id_t   g_rtos_usb_pcd_mbx_id;
extern  rtos_mbx_id_t   g_rtos_usb_pcd_sub_mbx_id;

extern  rtos_mbx_id_t   g_rtos_usb_pmsc_mbx_id;

extern  rtos_sem_id_t   g_rtos_usb_hmsc_sem_id;

extern  rtos_mem_id_t   g_rtos_usb_mpf_id;

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
rtos_err_t usb_rtos_configuration(void);
rtos_err_t usb_rtos_unconfiguration(void);
#endif /* BSP_CFG_RTOS_USED != 0 */
#endif /* R_USB_RTOS_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/


