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
 * Version      : 2.00
 * Description  : Functions for using Event Link Controller module
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description  
 *           01.07.2016 1.0     Initial Release
 *           16.06.2017 1.20    Added support for RX65N(2MB), RX130(512KB)
 *           04.22.2019 1.21    Delete inline definition of GetVersion function
 *           10.06.2020 2.00    Added support for RX23W
 *                              Modified comment of API function to Doxygen style.
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
 ******************************************************************************************************************/ /**
 * @brief This function initializes the ELC FIT module and transitions the module from the ELC terminated state to the
 *        ELC stopped state. This function must be called before calling any other API functions.
 * @retval    ELC_SUCCESS: Normal completion
 * @retval    ELC_ERR_LOCK_FUNC: The ELC was already open
 * @details   Initializes an event link. Also, if the ELC interrupt is used, it sets the priority level of that 
 *            interrupt.
 * @note      When this function is called, all of the content set by the R_ELC_Set() function and R_ELC_Control()
 *            function is cleared.
 */
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
 ******************************************************************************************************************/ /**
 * @brief When this module is in the ELC stopped state, this function sets the event link source and event link target.
 * @param[in] *p_elc_event_signal
 *            Pointer to an event link source setup structure.\n
 *            The content set in the event link source setup structure should be refereed from Table "3.1 Content Set in
 *            the Event Link Source Setup Structure (*p_elc_event_signal)" in the application note.
 * @param[in] *p_elc_module
 *            Pointer to an event link target setup structure.\n
 *            The content set in the event link target setup structure should be refereed from Table "3.2 Content Set in
 *            the Event Link Target Setup Structure (*p_elc_module)" in the application note.
 * @retval    ELC_SUCCESS: Normal completion
 * @retval    ELC_ERR_INVALID_ARG: Illegal argument
 * @details   This function sets up an event link. The event link source and event link target are specified as 
 *            arguments.
 * @note      \li This function should be called when the ELC is in the stopped state.
 *            \li The event link signals and link target peripheral modules that can be used differ with the device 
 *            used.
 *            \li To start event link operation, set this module to the ELC operating state with the R_ELC_Control()
 *            function (ELC_CMD_START) described later in this document.
 *            \li See section 1.5, State Transition Diagram in the application note for details on the ELC FIT module 
 *            states.
 *            \li When an output port group is selected as the link target and bit rotate output is selected as the port
 *            group operation, an initial value must be written to the port buffers in advance.\n
 *
 *            See section 4.4, Case C Setup Example in the application note for the setup procedure.
 */
elc_err_t R_ELC_Set (elc_event_signal_t * const p_elc_event_signal, elc_link_module_t * const p_elc_module)
{
    elc_err_t err = ELC_SUCCESS;

#if (ELC_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == p_elc_event_signal) || (FIT_NO_PTR == p_elc_event_signal))
    {
        return ELC_ERR_INVALID_ARG;
    }
    if ((NULL == p_elc_module) || (FIT_NO_PTR == p_elc_module))
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
 ******************************************************************************************************************/ /**
 * @brief This function transitions this module to the ELC operating state, clears the event link settings, and 
 *        generates port buffer accesses and ELC software events.
 * @param[in] command 
 *            Specifies the command\n
 *            The commands can be specified from Table "3.3 Commands" in the application note.
 * @param[in] *pdata 
 *            Used as the pointer to the arguments for each command.\n
 *            The void pointer set to the argument is converted to the appropriate type according to the command used.\n
 *            The pointer settings for each command can be specified from Table "3.4 Pointer Settings Corresponding to
 *            Each Command" in the application note.
 * @retval    ELC_SUCCESS: Normal completion
 * @retval    ELC_ERR_INVALID_ARG: Illegal argument
 * @details   Performs the operation specified by the command. The following commands can be specified.
 *            \li Start event link\n
 *            Transitions the event link to the operating state. Only \e ELC_SUCCESS is returned.
 *            \li Stop event link\n
 *            Transitions the event link to the stopped state. Only \e ELC_SUCCESS is returned.
 *            \li Clear event link settings\n
 *            Clears an event link set up with the R_ELC_Set() function.
 *            \li Write port buffer\n
 *            Writes the specified value to the port buffer.
 *            \li Read port buffer\n
 *            Reads the value from the port buffer.\n
 *            The value read is stored in the value element of the elc_pdbf_access_t structure passed as an argument.\n
 *            Only use this value after confirming that the return value from the R_ELC_Control() function was
 *            \e ELC_SUCCESS.
 *            \li Generate software event\n
 *            Software events can be generated.\n
 *            When a software event is to be generated, first set the link source to software event with the
 *            R_ELC_Set() function. Only \e ELC_SUCCESS is returned.
 * @note      \li If event link start is specified as the command, call this function if the ELC is in the stopped 
 *            state.
 *            \li If event link stop is specified as the command, call this function if the ELC is in the operating 
 *            state.
 *            \li If software event is specified as the command, call this function if the ELC is in the operating 
 *            state.
 *            \li See section 1.5, State Transition Diagram in the application note for details on the ELC FIT module 
 *            states.
 */
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
 ******************************************************************************************************************/ /**
 * @brief Sets the ELC to the terminated state.
 * @retval    ELC_SUCCESS: Normal completion
 * @details   Closes the ELC module.
 * @note      None 
 */
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
 ******************************************************************************************************************/ /**
 * @brief Returns the version number of the API.
 * @return    Version Number
 * @details   Returns the version number of this API.
 * @note      None 
 */
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
