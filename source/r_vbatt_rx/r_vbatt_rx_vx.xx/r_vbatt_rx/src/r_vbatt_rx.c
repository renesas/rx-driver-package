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
*              : 10.06.2020 1.05     Modified comment of API function to Doxygen style.
*              : 31.03.2021 2.00     Added support for RX671
*                                    Removed duplicated information in description.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx.c
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


/**********************************************************************************************************************
 * Function Name: R_VBATT_Open
 *****************************************************************************************************************/ /**
 * @brief This function sets up the VBATT pin voltage drop detection function and determines whether or not the battery
 *        backup supply voltage has fallen. \n
 *        This function is called once and only once after a reset start. The processing performed when a drop in the
 *        battery backup supply voltage is detected and when no drop is detected are performed by a callback function
 *        called by this function.
 * @param[in] p_vbatt_info
 *             Pointer to a data structure for the battery backup information. See section 2.9, Arguments, for details
 *             in application note. \n
 * @retval    VBATT_SUCCESS              Processing completed without problem
 * @retval    VBATT_ERR_LVD0_SETTING     Illegal voltage monitoring 0 settings in option function selection register 1
 *                                       (OFS1)
 * @retval    VBATT_ERR_INVALID_ARG      Invalid argument
 * @details   This function sets the enabled or disabled state of the VBATT pin voltage drop detection function, the
 *            detection level, and the interrupts according to settings in the configuration options. After that, it
 *            determines whether or not the battery backup supply voltage has dropped and calls the callback
 *            function. \n
 *            When calling the callback function: \n
 *            \li If a battery backup supply voltage drop has not been detected: \n
 *                A pointer to a variable that has been set to VBATT_NOT_DROP_VOLTAGE is passed as an argument.
 *            \li If a battery backup supply voltage drop has been detected: \n
 *                A pointer to a variable that has been set to VBATT_DROP_VOLTAGE is passed as an argument.
 * @note      None
 */
vbatt_return_t R_VBATT_Open(vbatt_info_t * p_vbatt_info)
{
    vbatt_return_t      ret;

#if  (1 == VBATT_CFG_PARAM_CHECKING_ENABLE)
    /* ==== CHECKS PARAMETER ==== */
    /* Because the structure of type according to the function that
       called r_vbatt_check_parameter function */
#ifdef GROUP_VBATT
    ret = r_vbatt_check_parameter((void *)p_vbatt_info, VBATT_MODE_OPEN);
#endif

#ifdef GROUP_VBATTB
    /* Check parameter */
    ret = r_vbatt_check_parameter((void *)p_vbatt_info, VBATT_TAMPER_MODE_OPEN);
#endif

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

/**********************************************************************************************************************
 * Function Name: R_VBATT_Control
 *****************************************************************************************************************/ /**
 * @brief This function sets the enabled or disabled state of the battery backup function and sets up the VBATT pin
 *        voltage drop detection function. This function is used when changing the content of these settings from those
 *        made with the R_VBATT_Open() function.
 * @param[in] p_vbatt_ctrl_info
 *             Pointer to the data structure used by the VBATT pin voltage drop detection function. See section 2.9,
 *             Arguments, for details in application note. \n
 * @retval    VBATT_SUCCESS              Processing completed without problem
 * @retval    VBATT_ERR_INVALID_ARG      Invalid argument
 * @details   This function sets the enabled or disabled state of the battery backup function, the enabled or disabled
 *            state of the VBATT pin voltage drop detection function, the detection level, and the interrupts according
 *            to the settings of the arguments.
 * @note      The table lists and describes the range of values the arguments may be set to and the meanings of those
 *            arguments see section 3 in application note for details.
 */
vbatt_return_t R_VBATT_Control(vbatt_ctrl_info_t * p_vbatt_ctrl_info)
{
    vbatt_return_t      ret;

#if  (1 == VBATT_CFG_PARAM_CHECKING_ENABLE)
    /* ==== CHECKS PARAMETER ==== */
    /* Because the member of the structure varies according to the function that
       called r_vbatt_check_parameter function */
#ifdef GROUP_VBATT
    ret = r_vbatt_check_parameter((void *)p_vbatt_ctrl_info, VBATT_MODE_CONTROL);
#endif
#ifdef GROUP_VBATTB
    /* Check parameter */
    ret = r_vbatt_check_parameter((void *)p_vbatt_ctrl_info, VBATT_TAMPER_MODE_CONTROL);
#endif
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
* Description  : This function calls Battery Backup Function and 
*                VBATT pin voltage drop detection set function.
* Arguments    : vbatt_ctrl_info_t * p_vbatt_ctrl_info  ; 
*                VBATT Pin Voltage Drop Detection Function information
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

/**********************************************************************************************************************
 * Function Name: R_VBATT_GetStatus
 *****************************************************************************************************************/ /**
 * @brief This function acquires the status for the battery backup function. This function is used to check the status
 *        of the battery backup function.
 * @param[in,out] p_vbatt_status
 *             Pointer to a variable to hold the battery backup function status. See section 2.9, Arguments, for
 *             details in application note. \n
 * @retval    VBATT_SUCCESS            Processing completed without problem
 * @retval    VBATT_ERR_INVALID_ARG    Invalid argument
 * @retval    VBATT_ERR_FUNC_INVALID   R_VBATT_GetStatus() was called when VBATT pin voltage drop detection was invalid
 * @details   This function reads out the VBATT status register (VBATTSR) to acquire the status of the battery backup
 *            function. It then stores that information at the address passed as an argument.
 * @note      The table shows the layout of the status flags see section 3 in application note for details.
 */
vbatt_return_t R_VBATT_GetStatus(vbatt_status_t * p_vbatt_status)
{
    vbatt_return_t      ret;

#if (1 == VBATT_CFG_PARAM_CHECKING_ENABLE)
    /* ==== CHECKS PARAMETER ==== */
    /* Because the member of the structure varies according to the function that
       called r_vbatt_check_parameter function */
#ifdef GROUP_VBATT
    ret = r_vbatt_check_parameter((void *)p_vbatt_status, VBATT_MODE_GETSTATUS);
#endif
#ifdef GROUP_VBATTB
    /* Check parameter */
    ret = r_vbatt_check_parameter((void *)p_vbatt_status, VBATT_TAMPER_MODE_GETSTATUS);
#endif
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

/**********************************************************************************************************************
 * Function Name: R_VBATT_GetVersion
 *****************************************************************************************************************/ /**
 * @brief This function returns the version number of the API.
 * @return    Version number
 * @details   This function returns the version number of this API.
 * @note      None
 */
uint32_t  R_VBATT_GetVersion(void)
{
    uint32_t const  version = (VBATT_VERSION_MAJOR << 16) | VBATT_VERSION_MINOR;

    return version;
}
/******************************************************************************
End of function R_VBATT_GetVersion
******************************************************************************/

#ifdef GROUP_VBATTB
/**********************************************************************************************************************
* Function Name: R_VBATT_ReadBackupData
 *****************************************************************************************************************/ /**
 * @brief This function read data from Backup Registers.
 * @param[in] uint8_t index     Index of Backup register need to read.
 * @param[in] pint8_t * p_data  Pointer to a variable to store data read\n
 * from Backup Registers.
 * @retval    VBATT_SUCCESS            Processing completed without problem
 * @retval    VBATT_ERR_INVALID_ARG    Invalid argument
 */
vbatt_return_t R_VBATT_ReadBackupData(uint8_t index, uint8_t * p_data)
{
    vbatt_return_t      ret;
    ret = VBATT_SUCCESS;
#if  (1 == VBATT_CFG_PARAM_CHECKING_ENABLE)
/* Check whether index is in of a range of BKRs
   and Check pointer of argument POINTER OF ARGMENT */
    if (VBATT_BKR_MAX_VAL < index)
    {
        ret =  VBATT_ERR_INVALID_ARG; 
    }
    
    if (NULL == p_data)
    {
        ret =  VBATT_ERR_INVALID_ARG;
    }

    if (VBATT_SUCCESS != ret)
    {
        return ret;
    }
#endif
     /* Return backup data */
    *p_data = SYSTEM.BKR[index];
    
return(ret);
}

/**********************************************************************************************************************
* Function Name: R_VBATT_WriteBackupData
 *****************************************************************************************************************/ /**
 * @brief This function write data to Backup Registers.
 * @param[in] uint8_t index;  Index of Backup register need to write.
 * @param[in] uint8_t * p_data; Pointer to a variable to store data\n
 * write to Backup Registers.
 * @retval    VBATT_SUCCESS            Processing completed without problem
 * @retval    VBATT_ERR_INVALID_ARG    Invalid argument
 */
vbatt_return_t R_VBATT_WriteBackupData(uint8_t index, uint8_t * p_data)
{
    vbatt_return_t      ret;
    ret = VBATT_SUCCESS;

#if (1 == VBATT_CFG_PARAM_CHECKING_ENABLE)
/* Check whether index is in of a range of BKRs
   and Check pointer of argument POINTER OF ARGMENT */
    if (VBATT_BKR_MAX_VAL < index)
    {
        ret =  VBATT_ERR_INVALID_ARG; 
    }
    
    if (NULL == p_data)
    {
        ret =  VBATT_ERR_INVALID_ARG;
    }

    if (VBATT_SUCCESS != ret)
    {
        return ret;
    }
#endif
    /* Set backup data */
    SYSTEM.BKR[index] = *p_data;
    
    return(ret);
}
#endif /* #ifdef GROUP_VBATTB */

/************************************************************************************
* Function Name: r_vbatt_isr_processing
* Description  : This function is called in Battery Backup Function Maskable Interrupt.
*                In RX671 this function is called in Tamper detection Maskable Interrupt.
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
#ifdef GROUP_VBATT
    callback_arg = VBATT_MASKABLE_INTERRUPT;
    
    pvbatt_info->callbackfunc(&callback_arg);
#endif

#ifdef GROUP_VBATTB

    if(1 == SYSTEM.TAMPSR.BIT.TAMP0F)
    {
        callback_arg = VBATT_TAMPER_CH0_INTERRUPT;
        pvbatt_info->callbackfunc(&callback_arg);
    }

    if(1 == SYSTEM.TAMPSR.BIT.TAMP1F)
    {
        callback_arg = VBATT_TAMPER_CH1_INTERRUPT;
        pvbatt_info->callbackfunc(&callback_arg);
    }

    if(1 == SYSTEM.TAMPSR.BIT.TAMP2F)
    {
        callback_arg = VBATT_TAMPER_CH2_INTERRUPT;
        pvbatt_info->callbackfunc(&callback_arg);
    }

#endif

    callback_arg = VBATT_NONE;
}
/******************************************************************************
End of function r_vbatt_isr_processing
******************************************************************************/

#ifdef GROUP_VBATT
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
#endif
/* End of File */

