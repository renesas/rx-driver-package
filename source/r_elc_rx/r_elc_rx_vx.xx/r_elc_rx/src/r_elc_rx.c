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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_elc_rx.c
 * Version      : 1.0
 * Description  : Functions for using Event Link Controller module
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description  
 *           01.07.2016 1.0     Initial Release
 *           16.06.2017 1.20    Added support for RX65N(2MB), RX130(512KB)
 *           04.22.2019 1.21    Delete inline definition of GetVersion function
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <stdio.h>
#include "platform.h"
#include "r_elc_rx_if.h"
#include "r_elc_rx_private.h"

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
#if (ELC_FUNCTION_ICU_LPT == 1)
elc_interrupt_set_t g_elc_callback_icu_lpt;
#endif
#if (ELC_FUNCTION_ICU1 == 1)
elc_interrupt_set_t g_elc_callback_icu1;
#endif
#if (ELC_FUNCTION_ICU2 == 1)
elc_interrupt_set_t g_elc_callback_icu2;
#endif

/***********************************************************************************************************************
 * Function Name: R_ELC_Open
 * Description  : This function initializes the event link module.
 * Arguments    : none
 * Return Value : ELC_SUCCESS
 *                   - A process is success.
 *              : ELC_ERR_LOCK_FUNC
 *                   - A process is failure.
 ***********************************************************************************************************************/
elc_err_t R_ELC_Open (void)
{

    /* Prevent multiple initialization */
    if (R_BSP_HardwareLock(BSP_LOCK_ELC) != true)
    {
        return ELC_ERR_LOCK_FUNC;
    }

    elc_module_control(ELC_MODULE_STOP_RELEASE);
    elc_open_init_register();

    return ELC_SUCCESS;
}
/***********************************************************************************************************************
 End of function R_ELC_Open
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_ELC_Set
 * Description  : This function sets the event link.
 * Arguments    : p_elc_event_signal
 *                  - Struct index of information about an event input.
 *              : p_elc_module
 *                  - Struct index of information about an event generation.
 * Return Value : ELC_SUCCESS
 *                   - A process is success.
 *              : ELC_ERR_INVALID_ARG
 *                   - A process is failure.
 ***********************************************************************************************************************/
elc_err_t R_ELC_Set (elc_event_signal_t * const p_elc_event_signal, elc_link_module_t * const p_elc_module)
{
    elc_err_t err = ELC_SUCCESS;

#if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == p_elc_event_signal) || (FIT_NO_PTR == p_elc_event_signal))
    {
        return ELC_ERR_INVALID_ARG;
    }
    if((NULL == p_elc_module) || (FIT_NO_PTR == p_elc_module))
    {
        return ELC_ERR_INVALID_ARG;
    }
    err = elc_check_validate_module(p_elc_module->link_module);
    if (ELC_SUCCESS != err)
    {
        return err;
    }

    err = elc_set_err_check_um_note(p_elc_event_signal, p_elc_module);
    if (ELC_SUCCESS != err)
    {
        return err;
    }
#endif

    switch (p_elc_module->link_module)
    {
#if (ELC_FUNCTION_ICU_LPT == 1)
        case ELC_ICU_LPT :
 #if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
        if (NULL == p_elc_module->link_module_callbackfunc)
        {
            return ELC_ERR_INVALID_ARG;
        }
        if (ELC_IPR_MAX < p_elc_module->link_module_interrupt_level)
        {
            return ELC_ERR_INVALID_ARG;
        }
 #endif
        g_elc_callback_icu_lpt = p_elc_module->link_module_callbackfunc;
        break;
#endif
#if (ELC_FUNCTION_ICU1 == 1)
        case ELC_ICU1 :
 #if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
        if (NULL == p_elc_module->link_module_callbackfunc)
        {
            return ELC_ERR_INVALID_ARG;
        }
        if (ELC_IPR_MAX < p_elc_module->link_module_interrupt_level)
        {
            return ELC_ERR_INVALID_ARG;
        }
 #endif
        g_elc_callback_icu1 = p_elc_module->link_module_callbackfunc;
        break;
#endif
#if (ELC_FUNCTION_ICU2 == 1)
        case ELC_ICU2 :
 #if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
        if (NULL == p_elc_module->link_module_callbackfunc)
        {
            return ELC_ERR_INVALID_ARG;
        }
        if (ELC_IPR_MAX < p_elc_module->link_module_interrupt_level)
        {
            return ELC_ERR_INVALID_ARG;
        }
 #endif
        g_elc_callback_icu2 = p_elc_module->link_module_callbackfunc;
        break;
#endif
        default :
        break;
    }

    elc_set_eventlink_setting(p_elc_event_signal, p_elc_module);

    return err;
}
/***********************************************************************************************************************
 End of function R_ELC_Set
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_ELC_Control
 * Description  : This function performs the operation specified in the command.
 * Arguments    : command
 *                   - The command of the processing.
 *              : pdata
 *                   - The value according to the command.
 * Return Value : ELC_SUCCESS
 *                   - A process is success.
 *              : ELC_ERR_INVALID_ARG
 *                   - A process is failure.
 ***********************************************************************************************************************/
elc_err_t R_ELC_Control (const elc_eventlink_cmd_t command, void *pdata)
{

    elc_err_t err = ELC_SUCCESS;
    elc_pdbf_access_t *pdbf_addr;
    elc_module_t clear_select;

#if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
    switch (command)
    {
        /* These 3 case are intentionally combined */
        case ELC_CMD_START :
        case ELC_CMD_STOP :
        case ELC_CMD_SOFTWARE_EVENT :
        break;

        default :
            if ((NULL == pdata) || (FIT_NO_PTR == pdata))
            {
                return ELC_ERR_INVALID_ARG;
            }
        break;
    }
    
    if (ELC_CMD_CLEAR_EVENTLINK == command)
    {
        clear_select = *(elc_module_t *) pdata;
        err = elc_check_validate_module(clear_select);
        if (ELC_SUCCESS != err)
        {
            return err;
        }
    }
    
#endif

    switch (command)
    {
        case ELC_CMD_START :
            elc_start_all_eventlink();
        break;

        case ELC_CMD_STOP :
            elc_stop_all_eventlink();
        break;

        case ELC_CMD_CLEAR_EVENTLINK :
            clear_select = *(elc_module_t *) pdata;
            elc_control_cmd_event_clear(clear_select);
        break;

        case ELC_CMD_WRITE_PORTBUFFER :
            pdbf_addr = (elc_pdbf_access_t *) pdata;
            elc_control_cmd_write_pdbf(pdbf_addr->select_group, pdbf_addr->value);
        break;
        
        case ELC_CMD_READ_PORTBUFFER :
            pdbf_addr = (elc_pdbf_access_t *) pdata;
            pdbf_addr->value = elc_control_cmd_read_pdbf(pdbf_addr->select_group);
        break;
        
        case ELC_CMD_SOFTWARE_EVENT :
            elc_control_cmd_soft_trigger();
        break;

        default :
#if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
            err = ELC_ERR_INVALID_ARG;
#endif
        break;
    }

    return err;
}
/***********************************************************************************************************************
 End of function R_ELC_Control
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_ELC_Close
 * Description  : This function stops the ELC module.
 * Arguments    : none
 * Return Value : ELC_SUCCESS -
 *                    Successful; function execution
 ***********************************************************************************************************************/
elc_err_t R_ELC_Close (void)
{
    elc_stop_all_eventlink();

    R_BSP_HardwareUnlock(BSP_LOCK_ELC);

    elc_module_control(ELC_MODULE_STOP_TRANSITION);

    return ELC_SUCCESS;
}
/***********************************************************************************************************************
 End of function R_ELC_Close
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_ELC_GetVersion
 * Description  : Returns the version of this module. The version number is
 *                encoded where the top 2 bytes are the major version number and
 *                the bottom 2 bytes are the minor version number.
 *                For example, Rev 4.25 would be 0x00040019.
 * NOTE         : This function is inlined using #pragma inline directive.
 * Arguments    : none
 * Return Value : Version Number
 ***********************************************************************************************************************/
uint32_t R_ELC_GetVersion (void)
{
    uint32_t version_number = 0;

    /* Bring in major version number. */
    version_number = ((uint32_t) ELC_RX_VERSION_MAJOR) << 16;

    /* Bring in minor version number. */
    version_number |= (uint32_t) ELC_RX_VERSION_MINOR;

    return version_number;
}
/***********************************************************************************************************************
 End of function R_ELC_GetVersion
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
