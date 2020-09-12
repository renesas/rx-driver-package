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
* Copyright (C) 2016-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx.c
* Description  : Functions for using LVD on RX devices.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Added support for RX24U, RX24T(512KB).
*              :                     Deleted unnecessary header information.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*                                    Add WAIT_LOOP comments
*                                    Fix GSCE coding checker errors
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin
*              : 20.05.2019 3.00     Added support for GNUC and ICCRX.
*              : 28.06.2019 3.10     Added support for RX23W.
*              : 25.11.2019 3.30     Modified comment of API function to Doxygen style.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

/* Public interface header file for this package. */
#include "r_lvd_rx_if.h"

/* Used for configuring the LVD module */
#include "r_lvd_rx_config.h"

/* Private header file for this package. */
#include "r_lvd_rx_private.h"

/* Private header file for this package. */
#include "r_lvd_rx_private_targets.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private variables and functions
***********************************************************************************************************************/
static const lvd_cfg_opt_t lvd_cfg_opt[LVD_CHANNEL_NUM] = {
{
    LVD_CFG_CHANNEL_1_USED,             /* lvd_ch_used                  */
    LVD_CFG_VDET_TARGET_CHANNEL_1,      /* lvd_target                   */
    LVD_VOLTAGE_LEVEL_VALUE_CH1,        /* lvd_voltage_level_value      */
    LVD_CFG_DIGITAL_FILTER_CHANNEL_1,   /* lvd_dfilter                  */
    LVD_DFILTER_DIV_VALUE_CH1,          /* lvd_dfilter_div_value        */
    LVD_DELAY_DFILTER_ENABLE_LOCO_CH1,  /* lvd_delay_dfilter_enable     */
    LVD_CFG_ACTION_CHANNEL_1,           /* lvd_action                   */
    LVD_CFG_INT_PRIORITY_CHANNEL_1,     /* lvd_int_prio                 */
    LVD_CFG_STABILIZATION_CHANNEL_1     /* lvd_reset_negate             */
},
#ifndef BSP_MCU_RX23W
{
    LVD_CFG_CHANNEL_2_USED,             /* lvd_ch_used                  */
    LVD_CFG_VDET_TARGET_CHANNEL_2,      /* lvd_target                   */
    LVD_VOLTAGE_LEVEL_VALUE_CH2,        /* lvd_voltage_level_value      */
    LVD_CFG_DIGITAL_FILTER_CHANNEL_2,   /* lvd_dfilter                  */
    LVD_DFILTER_DIV_VALUE_CH2,          /* lvd_dfilter_div_value        */
    LVD_DELAY_DFILTER_ENABLE_LOCO_CH2,  /* lvd_delay_dfilter_enable     */
    LVD_CFG_ACTION_CHANNEL_2,           /* lvd_action                   */
    LVD_CFG_INT_PRIORITY_CHANNEL_2,     /* lvd_int_prio                 */
    LVD_CFG_STABILIZATION_CHANNEL_2     /* lvd_reset_negate             */
}
#endif
};

#ifndef BSP_MCU_RX23W
static bool b_lvd_open[LVD_CHANNEL_NUM] = {false, false};
#endif
#if defined (BSP_MCU_RX23W)
static bool b_lvd_open[LVD_CHANNEL_NUM] = {false};
#endif

static void lvd_init_lvd(void);
static void lvd_start_lvd(lvd_channel_t ch, lvd_trigger_t trigger);
static void lvd_stop_lvd(lvd_channel_t ch);
static void lvd_start_int(lvd_channel_t ch, void (*p_callback)(void *));
static void lvd_stop_int(lvd_channel_t ch);


/***********************************************************************************************************************
* Function Name: R_LVD_Open
***********************************************************************************************************************//**
 * @brief This function initializes the specified channel and starts the LVD.
 * @param[in] channel Enumerated channel number to be initialized and for which the LVD starts.
 * @param[in] p_cfg Address of the configuration structure.
 * @param[in] p_callback Address of the function which is called from an interrupt upon the voltage detection.
 * @retval LVD_SUCCESS Successful: The LVD has been started.
 * @retval LVD_ERR_INVALID_PTR Error: Address in the p_cfg parameter is invalid.
 * @retval LVD_ERR_INVALID_FUNC Error: Address in the p_callback parameter is invalid.
 * @retval LVD_ERR_INVALID_DATA Error: The definition of the configuration option is invalid.
 * @retval LVD_ERR_INVALID_CHAN Error: The channel parameter is invalid.
 * @retval LVD_ERR_INVALID_ARG Error: The argument of the p_cfg parameter is invalid.
 * @retval LVD_ERR_UNSUPPORTED  Error: Selected function not supported.
 * @retval LVD_ERR_ALREADY_OPEN Error: The specified channel has already been open.
 * @retval LVD_ERR_LOCO_STOPPED Error: Setting during LOCO is stopped is invalid.
 * @details This function uses the p_cfg parameter and the configuration option settings to initialize the specified
 * channel and configure settings for processing upon voltage detection, and starts the LVD. When this function
 * completes its processing successfully, the status of the channel becomes ‘Opened’.
 * This function is executed for each channel, however, the configuration option settings for the voltage detection
 * level and the monitored voltage become effective only while all LVD circuits are stopped.
 * The callback function may or may not need to be registered in the p_callback parameter depending on processing upon
 * voltage detection, which is specified in the configuration option settings. For details, see the table below.
 * @note
 * See Section 3 in the application note for details.
 */
lvd_err_t R_LVD_Open(lvd_channel_t channel, lvd_config_t const *p_cfg, void (*p_callback)(void *))
{
    lvd_err_t result_code = LVD_SUCCESS;

    /* **** Error check **** */
    result_code = lvd_hw_check_param_ch(channel, &lvd_cfg_opt[0]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_OPEN;
    }
    result_code = lvd_hw_check_already_open(b_lvd_open[channel]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_OPEN;
    }
    /* **** Error check **** */
    result_code = lvd_hw_check_ptr((void *)p_cfg);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_OPEN;
    }
    result_code = lvd_hw_check_param_open(channel, p_cfg, p_callback, &lvd_cfg_opt[channel]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_OPEN;
    }
    result_code = lvd_hw_check_loco_limitation(&lvd_cfg_opt[channel]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_OPEN;
    }
    
    /* **** The main body of processing **** */
    lvd_hw_enable_reg_protect(false);
    lvd_stop_lvd(channel);
    lvd_init_lvd();
    lvd_start_int(channel, p_callback);
    lvd_start_lvd(channel, p_cfg->trigger);
    lvd_hw_enable_reg_protect(true);
    b_lvd_open[channel] = true;
    
RETURN_R_LVD_OPEN:
{
    return result_code;
}
} /* End of function R_LVD_Open() */


/***********************************************************************************************************************
* Function Name: R_LVD_Close
***********************************************************************************************************************//**
 * @brief This function stops the specified LVD channel.
 * @param[in] channel Enumerated channel number to be stopped.
 * @retval LVD_SUCCESS Successful: The LVD has been stopped.
 * @retval LVD_ERR_INVALID_CHAN Error: The channel parameter is invalid.
 * @details This function stops the specified LVD channel. When this function completes its processing successfully,
 * the status of the channel becomes ‘Not opened’.
 * @note
 * None.
 */
lvd_err_t R_LVD_Close(lvd_channel_t channel)
{
    lvd_err_t result_code = LVD_SUCCESS;

    /* **** Error check **** */
    result_code = lvd_hw_check_param_ch(channel, &lvd_cfg_opt[0]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_CLOSE;
    }
    
    /* **** The main body of processing **** */
    lvd_hw_enable_reg_protect(false);
    lvd_stop_lvd(channel);
    lvd_stop_int(channel);
    lvd_hw_enable_reg_protect(true);
    b_lvd_open[channel] = false;
    
RETURN_R_LVD_CLOSE:
{
    return result_code;
}
} /* End of function R_LVD_Close() */


/***********************************************************************************************************************
* Function Name: R_LVD_GetStatus
***********************************************************************************************************************//**
 * @brief This function obtains the LVD status of the specified channel.
 * @param[in] channel Enumerated channel number to obtain the status.
 * @param[in] p_status_position Address to store the enumerated voltage position status.
 * @param[in] p_status_cross Address to store the enumerated voltage crossing status.
 * @retval LVD_SUCCESS Successful: The LVD status has been obtained.
 * @retval LVD_ERR_INVALID_PTR Error: Addresses in the p_status_position and p_status_cross parameters are invalid.
 * @retval LVD_ERR_INVALID_CHAN Error: The channel parameter is invalid.
 * @retval LVD_ERR_NOT_OPENED Error: The specified channel is not opened.
 * @details This function stores the LVD statuses into parameters *p_status_position and *p_status_cross for the specified
 * channel. Refer to Figure 3.1for details on the statuses. The voltage position status stored in the *p_status_position
 * parameter can be obtained without dependence on the voltage detection condition. The voltage crossing status stored in
 * the *p_status_cross parameter is dependent on the voltage detection condition and the status becomes ‘Crossed’ only when
 * the condition is satisfied. Before this function is executed, the R_LVD_Open() function must be executed with the specified
 * channel to make the channel status ‘Opened’.
 * @note
 * None.
 */
lvd_err_t R_LVD_GetStatus(lvd_channel_t channel, 
                          lvd_status_position_t *p_status_position, 
                          lvd_status_cross_t *p_status_cross)
{
    lvd_err_t result_code = LVD_SUCCESS;

    /* **** Error check **** */
    result_code = lvd_hw_check_getstatus();
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_GETSTATUS;
    }
    result_code = lvd_hw_check_param_ch(channel, &lvd_cfg_opt[0]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_GETSTATUS;
    }
    result_code = lvd_hw_check_not_opened(b_lvd_open[channel]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_GETSTATUS;
    }
    /* **** Error check **** */
    result_code = lvd_hw_check_ptr((void *)p_status_position);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_GETSTATUS;
    }
    /* **** Error check **** */
    result_code = lvd_hw_check_ptr((void *)p_status_cross);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_GETSTATUS;
    }

    /* **** The main body of processing **** */
    lvd_hw_get_lvd_status(channel, p_status_position, p_status_cross);
    
RETURN_R_LVD_GETSTATUS:
{
    return result_code;
}
} /* End of function R_LVD_GetStatus() */


/***********************************************************************************************************************
* Function Name: R_LVD_ClearStatus
***********************************************************************************************************************//**
 * @brief This function clears the voltage crossing status for the specified channel.
 * @param[in] channel Enumerated channel number to clear the voltage crossing status.
 * @retval LVD_SUCCESS Successful: The voltage crossing status has been cleared.
 * @retval LVD_ERR_INVALID_CHAN Error: The channel parameter is invalid.
 * @retval LVD_ERR_NOT_OPENED   Error: The specified channel is not opened.
 * @details This function clears the voltage crossing status to ‘Not crossed’ for the specified channel. To clear the
 * status, interrupt and reset are temporarily disabled. Before executing this function, the R_LVD_Open() function must
 * be executed with the specified channel to make the channel status ‘Opened’.
 * @note Note that no interrupt or reset will occur if a voltage is detected while interrupt and reset are temporarily disabled
 * by this function.
 */
lvd_err_t R_LVD_ClearStatus(lvd_channel_t channel)
{
    lvd_err_t result_code = LVD_SUCCESS;
    bool b_enable_flag = false;
    
    /* **** Error check **** */
    result_code = lvd_hw_check_clearstatus();
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_CLEARSTATUS;
    }
    result_code = lvd_hw_check_param_ch(channel, &lvd_cfg_opt[0]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_CLEARSTATUS;
    }
    result_code = lvd_hw_check_not_opened(b_lvd_open[channel]);
    if (LVD_SUCCESS != result_code)
    {
        goto RETURN_R_LVD_CLEARSTATUS;
    }

    /* **** The main body of processing **** */
    lvd_hw_get_reset_int_enable(channel, &b_enable_flag);
    lvd_hw_enable_reg_protect(false);
    lvd_hw_enable_reset_int(channel, false);
    lvd_hw_clear_lvd_status(channel);
    lvd_hw_enable_reset_int(channel, b_enable_flag);
    lvd_hw_enable_reg_protect(true);
    
RETURN_R_LVD_CLEARSTATUS:
{
    return result_code;
}
} /* End of function R_LVD_ClearStatus() */


/***********************************************************************************************************************
* Function Name: R_LVD_GetVersion
***********************************************************************************************************************//**
 * @brief This function returns the current version of the LVD FIT module.
 * @return Version number
 * @details This function returns the version of the LVD FIT module. The version number is encoded where the top 2 bytes
 * are the major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25 would be
 * returned as 0x00040019.
 * @note
 * None.
 */
uint32_t R_LVD_GetVersion(void)
{
    uint32_t const version = (LVD_RX_VERSION_MAJOR << 16) | LVD_RX_VERSION_MINOR;

    return version;
} /* End of function R_LVD_GetVersion() */

/***********************************************************************************************************************
* Outline      : Initializes the voltage detection level and the monitored voltage.
* Function Name: lvd_init_lvd
* Description  : This function is executed for each channel, however, the configuration option settings for the voltage 
*              : detection level and the monitored voltage become effective only while all LVD circuits are stopped.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void lvd_init_lvd(void)
{
    uint32_t i;
    bool b_init_flag = true;
    bool b_enable_flag = false;
    
    /* When all LVD circuits are stopping, it is targeted for initialization */
    /* WAIT_LOOP */
    for (i = 0; i < LVD_CHANNEL_NUM; i++)
    {
        /* Cast type uint32_t to type lvd_channel_t */
        lvd_hw_get_circuit_enable((lvd_channel_t)i, &b_enable_flag);
        if (true == b_enable_flag)
        {
            b_init_flag = false;
        }
    }
    
    if (true == b_init_flag)
    {
         /* Initialize voltage detection level and target to monitored */
         /* WAIT_LOOP */
        for (i = 0; i < LVD_CHANNEL_NUM; i++)
        {
            /* Cast type uint32_t to type lvd_channel_t */
            lvd_hw_set_level((lvd_channel_t)i, lvd_cfg_opt[i].lvd_voltage_level_value);
            /* Cast type uint32_t to type lvd_channel_t */
            lvd_hw_set_target((lvd_channel_t)i, lvd_cfg_opt[i].lvd_target);
        }
    }
    
    return ;
} /* End of function lvd_init_lvd() */

/***********************************************************************************************************************
* Outline      : Starts the LVD.
* Function Name: lvd_start_lvd
* Description  : This function starts the specified LVD channel.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be started.
*              : lvd_trigger_t trigger      ; Enumerated voltage detection conditions.
* Return Value : none
***********************************************************************************************************************/
#if (LVD_GROUP_SETUP_LVDA_1 == LVD_GROUP_SETUP)
static void lvd_start_lvd(lvd_channel_t ch, lvd_trigger_t trigger)
{
    if (LVD_ENABLE == lvd_cfg_opt[ch].lvd_dfilter)
    {
        lvd_hw_setup_dfilter(ch, lvd_cfg_opt[ch].lvd_dfilter_div_value);
    }
    else
    {
        lvd_hw_enable_dfilter(ch, false);
    }
    
    if (LVD_ACTION_RESET == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_select_reset(ch);
        lvd_hw_setup_reset(ch, lvd_cfg_opt[ch].lvd_reset_negate);
    }
    else
    {
        lvd_hw_select_int(ch);
    }
    
    lvd_hw_set_trigger(ch, trigger);
    lvd_hw_enable_output(ch, true);

    if (LVD_ENABLE == lvd_cfg_opt[ch].lvd_dfilter)
    {
        lvd_hw_dummy_read_output(ch);
        lvd_hw_wait_delay_loco(1);
        lvd_hw_enable_dfilter(ch, true);
        lvd_hw_dummy_read_dfilter(ch);
        lvd_hw_wait_delay_loco(lvd_cfg_opt[ch].lvd_delay_dfilter_enable);
    }

    lvd_hw_clear_lvd_status(ch);

    if (LVD_ACTION_NONE != lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_enable_reset_int(ch, true);
    }

    lvd_hw_enable_circuit(ch, true);
    
    return ;
} /* End of function lvd_start_lvd() */

#elif (LVD_GROUP_SETUP_LVDA_2 == LVD_GROUP_SETUP)
static void lvd_start_lvd(lvd_channel_t ch, lvd_trigger_t trigger)
{
    lvd_hw_enable_circuit(ch, true);
    lvd_hw_dummy_read_circuit(ch);
    lvd_hw_wait_delay(LVD_DELAY_CIRCUIT_ENABLE_USEC);

    if (LVD_ENABLE == lvd_cfg_opt[ch].lvd_dfilter)
    {
        lvd_hw_setup_dfilter(ch, lvd_cfg_opt[ch].lvd_dfilter_div_value);
        lvd_hw_enable_dfilter(ch, true);
        lvd_hw_dummy_read_dfilter(ch);
        lvd_hw_wait_delay_loco(lvd_cfg_opt[ch].lvd_delay_dfilter_enable);
    }
    
    if (LVD_ACTION_RESET == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_select_reset(ch);
        lvd_hw_setup_reset(ch, lvd_cfg_opt[ch].lvd_reset_negate);
        lvd_hw_set_trigger(ch, trigger);
    }
    else
    {
        lvd_hw_select_int(ch);
        lvd_hw_set_trigger(ch, trigger);
        
        if (LVD_ACTION_MI == lvd_cfg_opt[ch].lvd_action)
        {
            lvd_hw_select_mi(ch);
        }
        else
        {
            lvd_hw_select_nmi(ch);
        }
    }

    lvd_hw_clear_lvd_status(ch);

    if (LVD_ACTION_NONE != lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_enable_reset_int(ch, true);
    }

    lvd_hw_enable_output(ch, true);

    return ;
} /* End of function lvd_start_lvd() */

#elif ((LVD_GROUP_SETUP_LVDAa_1 == LVD_GROUP_SETUP)||\
       (LVD_GROUP_SETUP_LVDAb_1 == LVD_GROUP_SETUP))
static void lvd_start_lvd(lvd_channel_t ch, lvd_trigger_t trigger)
{
    if (LVD_ACTION_RESET == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_select_reset(ch);
        lvd_hw_setup_reset(ch, lvd_cfg_opt[ch].lvd_reset_negate);
        lvd_hw_set_trigger(ch, trigger);
        lvd_hw_enable_reset_int(ch, true);
        lvd_hw_enable_circuit(ch, true);
        lvd_hw_dummy_read_circuit(ch);
        lvd_hw_wait_delay(LVD_DELAY_CIRCUIT_ENABLE_USEC);
        lvd_hw_enable_output(ch, true);
        lvd_hw_dummy_read_output(ch);
        lvd_hw_wait_delay(LVD_DELAY_CLEAR_STATUS_USEC);
        lvd_hw_clear_lvd_status(ch);
    }
    else
    {
        lvd_hw_select_int(ch);
        lvd_hw_set_trigger(ch, trigger);
        
        if (LVD_ACTION_MI == lvd_cfg_opt[ch].lvd_action)
        {
            lvd_hw_select_mi(ch);
        }
        else
        {
            lvd_hw_select_nmi(ch);
        }
        
        lvd_hw_enable_circuit(ch, true);
        lvd_hw_dummy_read_circuit(ch);
        lvd_hw_wait_delay(LVD_DELAY_CIRCUIT_ENABLE_USEC);
        lvd_hw_enable_output(ch, true);
        lvd_hw_dummy_read_output(ch);
        lvd_hw_wait_delay(LVD_DELAY_CLEAR_STATUS_USEC);
        lvd_hw_clear_lvd_status(ch);        

        if (LVD_ACTION_NONE != lvd_cfg_opt[ch].lvd_action)
        {
            lvd_hw_enable_reset_int(ch, true);
        }
    }
    
    return ;
} /* End of function lvd_start_lvd() */

#else /* Error */
    #error "Error! Invalid setting for LVD_GROUP_SETUP in targets file."
#endif
/***********************************************************************************************************************
* Outline      : Stops the LVD.
* Function Name: lvd_stop_lvd
* Description  : This function stops the specified LVD channel.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be stopped.
* Return Value : none
***********************************************************************************************************************/
#if (LVD_GROUP_SETUP_LVDA_1 == LVD_GROUP_SETUP)
static void lvd_stop_lvd(lvd_channel_t ch)
{
    lvd_hw_enable_circuit(ch, false);
    lvd_hw_dummy_read_circuit(ch);
    lvd_hw_wait_delay_loco(1);
    lvd_hw_enable_reset_int(ch, false);
    lvd_hw_enable_output(ch, false);
    lvd_hw_clear_lvd_status(ch);

    return ;
} /* End of function lvd_stop_lvd() */

#elif (LVD_GROUP_SETUP_LVDA_2 == LVD_GROUP_SETUP)
static void lvd_stop_lvd(lvd_channel_t ch)
{
    uint32_t wait_loco_cycle = 0;
    bool b_enable_flag = false;

    lvd_hw_get_dfilter_enable(ch, &b_enable_flag);
    if (true == b_enable_flag)
    {
        wait_loco_cycle = lvd_cfg_opt[ch].lvd_delay_dfilter_enable;
    }

    lvd_hw_enable_output(ch, false);
    lvd_hw_dummy_read_output(ch);
    lvd_hw_wait_delay_loco(wait_loco_cycle);
    lvd_hw_enable_reset_int(ch, false);
    lvd_hw_enable_dfilter(ch, false);
    lvd_hw_enable_circuit(ch, false);
    lvd_hw_clear_lvd_status(ch);

    return ;
} /* End of function lvd_stop_lvd() */

#elif ((LVD_GROUP_SETUP_LVDAa_1 == LVD_GROUP_SETUP)||\
       (LVD_GROUP_SETUP_LVDAb_1 == LVD_GROUP_SETUP))
static void lvd_stop_lvd(lvd_channel_t ch)
{
    if (LVD_ACTION_RESET == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_enable_output(ch, false);
        lvd_hw_enable_circuit(ch, false);
        lvd_hw_enable_reset_int(ch, false);
    }
    else
    {
        lvd_hw_enable_reset_int(ch, false);
        lvd_hw_enable_output(ch, false);
        lvd_hw_enable_circuit(ch, false);
    }
    lvd_hw_clear_lvd_status(ch);
} /* End of function lvd_stop_lvd() */

#else
    #error "Error! Invalid setting for LVD_GROUP_SETUP in targets file."
#endif
/***********************************************************************************************************************
* Outline      : Starts the LVD interrupt.
* Function Name: lvd_start_int
* Description  : This function enables the LVD interrupt.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be started.
*              : void (*p_callback)(void *) ; Address of the callback function.
* Return Value : none
***********************************************************************************************************************/
static void lvd_start_int(lvd_channel_t ch, void (*p_callback)(void *))
{
    if (LVD_ACTION_MI == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_set_mi_cb(ch, p_callback);
        lvd_hw_enable_mi(ch, lvd_cfg_opt[ch].lvd_int_prio, true);
    }
    else if (LVD_ACTION_NMI == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_set_nmi_cb(ch, p_callback);
        lvd_hw_enable_nmi(ch, true);
    }
    else
    {

        /* do nothing */
    }

    return ;
} /* End of function lvd_start_int() */

/***********************************************************************************************************************
* Outline      : Stops the LVD interrupt.
* Function Name: lvd_stop_int
* Description  : This function disables the LVD interrupt.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be stopped.
* Return Value : none
***********************************************************************************************************************/
static void lvd_stop_int(lvd_channel_t ch)
{
    if (LVD_ACTION_MI == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_enable_mi(ch, lvd_cfg_opt[ch].lvd_int_prio, false);
        lvd_hw_set_mi_cb(ch, NULL);
    }
    else if (LVD_ACTION_NMI == lvd_cfg_opt[ch].lvd_action)
    {
        lvd_hw_set_nmi_cb(ch, NULL);
    }
    else
    {
        /* do nothing */
    }

    return ;
}/* End of function lvd_stop_int() */

/* End of File */

