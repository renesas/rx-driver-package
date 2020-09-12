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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_ble_pf_functions.c
* Description  : The user configurable platform functions.
* History      : DD.MM.YYYY Version Description           
*              : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

#include <stdio.h>
#ifdef NO_USE_BSP
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#else /* NO_USE_BSP */
#include "platform.h"
#endif /* NO_USE_BSP */
#include "r_ble_rx23w_if.h"
#include "r_ble_rx23w_config.h"
#include "r_ble_pf_config_private.h"

#ifndef DISBALE_BLE_SECTION
#if defined(__CCRX__)
#pragma section P BLE_P
#pragma section C BLE_C
#pragma section D BLE_D
#pragma section B BLE_B
#endif /* defined(__CCRX__) */
#endif /* DISBALE_BLE_SECTION */

/* This API is called within BLE driver and notify fatal error of BLE driver.  */
BLE_SECTION_P void r_ble_rf_control_error(uint32_t err_no)
{
    switch( err_no )
    {
        case 0:
            break;
        default:
            break;
    }
    
   
    /* Disable protect bit */
    SYSTEM.PRCR.WORD = 0xA50FU;

    /* MCU Software Reset */
    SYSTEM.SWRR = 0xA501U;

    /**** never reached ****/

    return;
}

/* This API is called within BLE driver and specifies the feedback for power saving operation of BLE driver. */
BLE_SECTION_P uint8_t r_ble_rf_power_save_mode(void)
{
    uint8_t ret = BLE_CFG_RF_DEEP_SLEEP_EN;

    /* 
     * When this function returns 0x0, RF does not enter power saving mode. 
     * When this function returns 0x1, RF enter power saving mode.
     * 
     * When the application layer occupies more time than the RF event time, 
     * RF communication can be maintained by returning 0x0 with this function.
     */

    return ret;
}

#if ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_START_MASK) != 0)
/* This API is called within BLE driver and notify the RF event started. */
BLE_SECTION_P void r_ble_rf_notify_event_start(uint32_t param)
{
    /* Note: Do not processing long time here. */
}
#endif /* ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_START_MASK) != 0) */


#if ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_CLOSE_MASK) != 0)
/* This API is called within BLE driver and notify the RF event closed. */
BLE_SECTION_P void r_ble_rf_notify_event_close(uint32_t param)
{
    /* Note: Do not processing long time here. */
}
#endif /* ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_CLOSE_MASK) != 0) */


#if ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_DS_MASK) != 0)
/* This API is called within BLE driver and notify the RF_DEEP_SLEEP started or closed. */
BLE_SECTION_P void r_ble_rf_notify_deep_sleep(uint32_t param)
{
    /* Note: Do not processing long time here. */
}
#endif /* ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_DS_MASK) != 0) */


/***********************************************************************************************************************
* Function Name: r_ble_cmt_cmi2_interrupt_func
* Description  : Don't change this function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
BLE_PRAGMA_INTERRUPT (r_ble_cmt_cmi2_interrupt_func, VECT(CMT2, CMI2))
BLE_ATTRIB_INTERRUPT void r_ble_cmt_cmi2_interrupt_func(void)
{
    extern void r_ble_cmt_cmi2_interrupt(void);
    r_ble_cmt_cmi2_interrupt();
}


/***********************************************************************************************************************
* Function Name: r_ble_cmt_cmi3_interrupt_func
* Description  : Don't change this function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
BLE_PRAGMA_INTERRUPT (r_ble_cmt_cmi3_interrupt_func, VECT(CMT3, CMI3))
BLE_ATTRIB_INTERRUPT void r_ble_cmt_cmi3_interrupt_func(void)
{
    extern void r_ble_cmt_cmi3_interrupt(void);
    r_ble_cmt_cmi3_interrupt();
}

/***********************************************************************************************************************
* Function Name: r_ble_bleirq_interrupt_func
* Description  : Don't change this function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#ifdef VECT_BLE_BLEIRQ
BLE_PRAGMA_INTERRUPT (r_ble_bleirq_interrupt_func, VECT(BLE, BLEIRQ))
#else /* VECT_BLE_BLEIRQ */
BLE_PRAGMA_INTERRUPT (r_ble_bleirq_interrupt_func, VECT(ICU, BLEIRQ))
#endif /* VECT_BLE_BLEIRQ */
BLE_ATTRIB_INTERRUPT void r_ble_bleirq_interrupt_func(void)
{
    extern void r_ble_bleirq_interrupt(void);
    r_ble_bleirq_interrupt();
}


#ifndef DISBALE_BLE_SECTION
#if defined(__CCRX__)
#pragma section
#endif /* defined(__CCRX__) */
#endif /* DISBALE_BLE_SECTION */
