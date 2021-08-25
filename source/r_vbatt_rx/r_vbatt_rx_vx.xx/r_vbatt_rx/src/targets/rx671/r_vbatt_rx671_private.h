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
*
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.03.2021 1.00     First Release
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx671_private.h
* Description  : Functions for using Battery Backup Function on RX devices. 
*******************************************************************************/
#ifndef R_VBATT_RX671_PRIVATE_H
#define R_VBATT_RX671_PRIVATE_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"
#ifdef BSP_MCU_RX671


/*******************************************************************************
Macro definitions
*******************************************************************************/

/*----------------------------------------------------------------------------*/
/*   Define                                                                   */
/*----------------------------------------------------------------------------*/
/* lvd0 of ofs1 setting judgment */
#define VBATT_OFS1_LVD0_BIT_MASK        ((uint32_t)(0x00000004))    /* mask for LVDAS bit of ofs1   */
#define VBATT_OFS1_LVD0_ENABLE          ((uint32_t)(0x00000000))    /* lvd0 enable */
#define VBATT_OFS1_VDSEL_BIT_MASK       ((uint32_t)(0x00000003))    /* mask for VDSEL bit of ofs1   */
#define VBATT_OFS1_VDSEL_DISABLE         ((uint32_t)(0x00000000))    /* VDSEL disable */

#define VBATT_MASK_TAMPIE                 ((uint8_t)(0x01)) /* Mask for bit TAMPIE */
#define VBATT_MASK_TAMPEE                 ((uint8_t)(0x10)) /* Mask for bit TAMPEE */
#define VBATT_MASK_TCES                   ((uint8_t)(0x01)) /* Mask for bit TCES */
#define VBATT_MASK_CHEN                   ((uint8_t)(0x01)) /* Mask for bit CHENE */
#define VBATT_MASK_CHNFE                  ((uint8_t)(0x01)) /* Mask for bit CHNFE */
#define VBATT_MASK_CHTRG                  ((uint8_t)(0x10)) /* Mask for bit CHTRG */
#define VBATT_MASK_TAMP_DETECT_FLAG       ((uint8_t)(0x01)) /* Mask for bit Detect flag */
#define VBATT_MASK_TAMP_MONITOR_FLAG      ((uint8_t)(0x01)) /* Mask for bit Monitor flag */

/* Maximum of BKR */
#define VBATT_BKR_MAX_VAL (127)
/* BKR register base address */

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*   Function For Target MCU                                                  */
/*----------------------------------------------------------------------------*/
extern vbatt_return_t r_vbatt_check_parameter(void * p_arg, vbatt_api_mode_t called_api);
extern vbatt_return_t r_vbatt_open_set_target_mcu(void);
extern uint8_t r_vbatt_open_check_target_mcu(void);
extern void r_vbatt_open_f_clear_target_mcu(void);
extern vbatt_return_t r_vbatt_control_target_mcu(vbatt_ctrl_info_t * p_vbatt_ctrl_info);
extern vbatt_return_t r_vbatt_getstatus_target_mcu(vbatt_status_t * p_vbatt_status);


#endif /* BSP_MCU_RX671 */
#endif /* R_VBATT_RX671_PRIVATE_H */

/* End of File */

