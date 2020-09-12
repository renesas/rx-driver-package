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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.06.2015 1.00     First Release
*              : 20.05.2019 1.03     Deleted the inline expansion of the 
*                                    R_VBATT_GetVersion function.
*              : 30.06.2019 1.04     Added support for RX23W
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx.c
* Device(s)    : RX231, RX230, RX23W
* Tool-Chain   : Renesas RX Standard Toolchain 3.01.00
*                GCC for Renesas RX 4.8.4.201801
*                IAR C/C++ Compiler for Renesas RX version 4.10.1
* OS           : None
* H/W Platform : RSKRX231, RSKRX230, RSSKRX23W
* Description  : Functions for using Battery Backup Function on RX devices. 
*******************************************************************************/
/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Defines for Battery Backup Function support */
#include "r_vbatt_rx_private.h"
#include "r_vbatt_rx_if.h"
#include "r_vbatt_rx_config.h"

/* Include specifics for chosen MCU.  */
#include "r_vbatt_rx_platform.h"

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*  Main processing of Battery Backup Function Driver API functions           */
/*----------------------------------------------------------------------------*/
static vbatt_return_t vbatt_open(vbatt_info_t * p_vbatt_info);
static vbatt_return_t vbatt_control(vbatt_ctrl_info_t * p_vbatt_ctrl_info);
static vbatt_return_t vbatt_getstatus(vbatt_status_t * p_vbatt_status);

/*----------------------------------------------------------------------------*/
/*  Private global variables (this file only)                                 */
/*----------------------------------------------------------------------------*/
static vbatt_info_t* volatile pvbatt_info;


/*****************************************************************************
* Function Name: R_VBATT_Open
* Description  : This function is the API function that a user calls.
*                This function checks setting of LVD0 in OFS1.
*                And this function checks a parameter.
*                Then calls main processing of R_VBATT_Open.
* Arguments    : vbatt_info_t * p_vbatt_info        ; Battery Backup Function Information
* Return Value : VBATT_SUCCESS                      ; Successful operation
*              : VBATT_ERR_LVD0_SETTING             ; LVD0 of OFS1 setting is invalid
*              ; VBATT_ERR_INVALID_ARG              ; Parameter error
******************************************************************************/
vbatt_return_t R_VBATT_Open(vbatt_info_t * p_vbatt_info)
{
    vbatt_return_t      ret;

#if ((1 == VBATT_CFG_PARAM_CHECKING_ENABLE) && (1 == BSP_CFG_PARAM_CHECKING_ENABLE))
    /* ==== CHECKS PARAMETER ==== */
    /* Because the structure of type according to the function that
       called r_vbatt_check_parameter function */
    ret = r_vbatt_check_parameter((void *)p_vbatt_info, VBATT_MODE_OPEN);

    if (VBATT_SUCCESS != ret)
    {
        return ret;
    }
#endif

    /* ==== CALLS THE API FUNCTION ==== */
    ret = vbatt_open(p_vbatt_info);

    return ret;
}
/******************************************************************************
End of function R_VBATT_Open
******************************************************************************/

/*****************************************************************************
* Function Name: vbatt_open
* Description  : This function calls VBATT pin voltage drop detection set function.
*                And this function judges having battery backup power supply voltage drop or not.
*                Then this function call a callback function.
*                When I call a callback function:
*                   - Battery backup power voltage drop not detected
*                       VBATT_NOT_DROP_VOLTAGE is set to an argument
*                   - Battery backup power voltage drop detected
*                       VBATT_DROP_VOLTAGE is set to an argument
* Arguments    : vbatt_info_t * p_vbatt_info        ; Battery Backup Function Information
* Return Value : VBATT_SUCCESS                      ; Successful operation
******************************************************************************/
static vbatt_return_t vbatt_open(vbatt_info_t * p_vbatt_info)
{
    vbatt_return_t      ret;
    vbatt_cb_evt_t      callback_arg;
    uint8_t             result;

    /* copy vbatt information for interrupt */
    pvbatt_info = p_vbatt_info;

    /* ==== CALLS THE FUNCTION FOR TARGET MCU ==== */
    ret = r_vbatt_open_set_target_mcu();

    /* ==== BATTERY BACKUP POWER VOLTAGE DROP DETECTION JUDGEMENT ==== */
    /* Check battery backup power voltage drop detection flag */
    result = r_vbatt_open_check_target_mcu();
    if (VBATT_VOLTAGE_DROP_DETECTED == result)
    {
        /* ---- Battery backup power voltage drop detected ---- */
        /* callback function calling */
        callback_arg = VBATT_DROP_VOLTAGE;
        p_vbatt_info->callbackfunc(&callback_arg);
        callback_arg = VBATT_NONE;

        /* Clear the battery backup power voltage drop detection flag */
        r_vbatt_open_f_clear_target_mcu();
    }
    else
    {
        /* ---- Battery backup power voltage drop not detected ---- */
        /* callback function calling */
        callback_arg = VBATT_NOT_DROP_VOLTAGE;
        p_vbatt_info->callbackfunc(&callback_arg);
        callback_arg = VBATT_NONE;
    }

    return ret;
}
/******************************************************************************
End of function vbatt_open
******************************************************************************/

/*****************************************************************************
* Function Name: R_VBATT_Control
* Description  : This function is the API function that a user calls.
*                This function checks a parameter.
*                Then calls main processing of R_VBATT_Control.
* Arguments    : vbatt_ctrl_info_t * p_vbatt_ctrl_info  ; VBATT Pin Voltage Drop Detection Function information
* Return Value : VBATT_SUCCESS                          ; Successful operation
*              ; VBATT_ERR_INVALID_ARG                  ; Parameter error
******************************************************************************/
vbatt_return_t R_VBATT_Control(vbatt_ctrl_info_t * p_vbatt_ctrl_info)
{
    vbatt_return_t      ret;

#if ((1 == VBATT_CFG_PARAM_CHECKING_ENABLE) && (1 == BSP_CFG_PARAM_CHECKING_ENABLE))
    /* ==== CHECKS PARAMETER ==== */
    /* Because the member of the structure varies according to the function that
       called r_vbatt_check_parameter function */
    ret = r_vbatt_check_parameter((void *)p_vbatt_ctrl_info, VBATT_MODE_CONTROL);

    if (VBATT_SUCCESS != ret)
    {
        return ret;
    }

#endif

    /* ==== CALLS THE API FUNCTION ==== */
    ret = vbatt_control(p_vbatt_ctrl_info);

    return ret;
}
/******************************************************************************
End of function R_VBATT_Control
******************************************************************************/

/*****************************************************************************
* Function Name: vbatt_control
* Description  : This function calls Battery Backup Function and VBATT pin voltage drop detection set function.
* Arguments    : vbatt_ctrl_info_t * p_vbatt_ctrl_info  ; VBATT Pin Voltage Drop Detection Function information
* Return Value : VBATT_SUCCESS                          ; Successful operation
******************************************************************************/
static vbatt_return_t vbatt_control(vbatt_ctrl_info_t * p_vbatt_ctrl_info)
{
    vbatt_return_t      ret;

    /* ==== CALLS THE FUNCTION FOR TARGET MCU ==== */
    ret = r_vbatt_control_target_mcu(p_vbatt_ctrl_info);

    return ret;
}
/******************************************************************************
End of function vbatt_control
******************************************************************************/

/*****************************************************************************
* Function Name: R_VBATT_GetStatus
* Description  : This function is the API function that a user calls.
*                This function checks the setting of the VBATTDIS bit of the VBATTCR register.
*                And this function checks a parameter.
*                Then calls main processing of R_VBATT_GetStatus.
* Arguments    : vbatt_status_t * p_vbatt_status    ; Battery Backup Function status information
* Return Value : VBATT_SUCCESS                      ; Successful operation
*              : VBATT_ERR_INVALID_ARG              ; Parameter error
*              : VBATT_ERR_FUNC_INVALID             ; Is called in VBATT detection function disable
******************************************************************************/
vbatt_return_t R_VBATT_GetStatus(vbatt_status_t * p_vbatt_status)
{
    vbatt_return_t      ret;

#if ((1 == VBATT_CFG_PARAM_CHECKING_ENABLE) && (1 == BSP_CFG_PARAM_CHECKING_ENABLE))
    /* ==== CHECKS PARAMETER ==== */
    /* Because the member of the structure varies according to the function that
       called r_vbatt_check_parameter function */
    ret = r_vbatt_check_parameter((void *)p_vbatt_status, VBATT_MODE_GETSTATUS);

    if (VBATT_SUCCESS != ret)
    {
        return ret;
    }
#endif

    /* ==== CALLS THE API FUNCTION ==== */
    ret = vbatt_getstatus(p_vbatt_status);

    return ret;
}
/******************************************************************************
End of function R_VBATT_GetStatus
******************************************************************************/

/*****************************************************************************
* Function Name: vbatt_getstatus
* Description  : This function calls get the state of the Battery Backup Function function.
* Arguments    : vbatt_status_t * p_vbatt_status    ; Battery Backup Function status information
* Return Value : VBATT_SUCCESS                      ; Successful operation
******************************************************************************/
static vbatt_return_t vbatt_getstatus(vbatt_status_t * p_vbatt_status)
{
    vbatt_return_t      ret;

    /* ==== CALLS THE FUNCTION FOR TARGET MCU ==== */
    ret = r_vbatt_getstatus_target_mcu(p_vbatt_status);

    return ret;
}
/******************************************************************************
End of function vbatt_getstatus
******************************************************************************/

/*****************************************************************************
* Function Name: R_VBATT_GetVersion
* Description  : Returns the version of this module. The version number is 
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
uint32_t  R_VBATT_GetVersion(void)
{
    uint32_t const  version = (VBATT_VERSION_MAJOR << 16) | VBATT_VERSION_MINOR;

    return version;
}
/******************************************************************************
End of function R_VBATT_GetVersion
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_isr_processing
* Description  : This function is called in Battery Backup Function Maskable Interrupt.
*                This function call a callback function.
*                When I call a callback function:
*                       VBATT_MASKABLE_INTERRUPT is set to an argument
* Arguments    : none
* Return Value : none
************************************************************************************/
void r_vbatt_isr_processing(void)
{
    vbatt_cb_evt_t      callback_arg;

    /* call back function calling */
    callback_arg = VBATT_MASKABLE_INTERRUPT;
    pvbatt_info->callbackfunc(&callback_arg);
    callback_arg = VBATT_NONE;

}
/******************************************************************************
End of function r_vbatt_isr_processing
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_nmi_isr_processing
* Description  : This function is called in Battery Backup Function Non-Maskable Interrupt.
*                This function call a callback function.
*                When I call a callback function:
*                       VBATT_NON_MASKABLE_INTERRUPT is set to an argument
* Arguments    : none
* Return Value : none
************************************************************************************/
void r_vbatt_nmi_isr_processing(void)
{
    vbatt_cb_evt_t      callback_arg;

    /* call back function calling */
    callback_arg = VBATT_NON_MASKABLE_INTERRUPT;
    pvbatt_info->callbackfunc(&callback_arg);
    callback_arg = VBATT_NONE;

}
/******************************************************************************
End of function r_vbatt_nmi_isr_processing
******************************************************************************/

/* End of File */

