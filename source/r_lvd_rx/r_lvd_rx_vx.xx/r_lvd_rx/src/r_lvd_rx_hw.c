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
* File Name    : r_lvd_rx_hw.c
* Description  : Functions for using LVD on RX devices.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Added RX65N support.
*              :                     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Added support for RX24U, RX24T(512KB).
*              :                     Deleted unnecessary header information.
*              :                     FIT_NO_PTR check added to NULL check.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*                                    Fix GSCE
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin
*              : 20.05.2019 3.00     Added support for GNUC and ICCRX.
*              : 28.06.2019 3.10     Added support for RX23W.
*              : 30.12.2019 3.40     Added support RX66N, RX72N.
                                     Fixed Bit Operation on LVD1CR0, LVD2CR0.
                                     Added macro LVD_GROUP_INT_ICUD.
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
Typedef definitions
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
/* Callback function pointer */
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH1)
static void (*p_lvd_cb_ch1)(void *p_data) = NULL;
#endif

#if (LVD_ENABLE == LVD_SUPPORT_MI_CH2)
static void (*p_lvd_cb_ch2)(void *p_data) = NULL;
#endif

/* Support information */
#ifndef BSP_MCU_RX23W
static const uint8_t lvd_support_cmpa[LVD_CHANNEL_NUM]    = {LVD_SUPPORT_CMPA_CH1,    LVD_SUPPORT_CMPA_CH2    };
static const uint8_t lvd_support_dfilter[LVD_CHANNEL_NUM] = {LVD_SUPPORT_DFILTER_CH1, LVD_SUPPORT_DFILTER_CH2 };
static const uint8_t lvd_support_mi[LVD_CHANNEL_NUM]      = {LVD_SUPPORT_MI_CH1,      LVD_SUPPORT_MI_CH2      };
static const uint8_t lvd_support_nmi[LVD_CHANNEL_NUM]     = {LVD_SUPPORT_NMI_CH1,     LVD_SUPPORT_NMI_CH2     };
#endif

#if defined(BSP_MCU_RX23W)
static const uint8_t lvd_support_cmpa[LVD_CHANNEL_NUM]    = {LVD_SUPPORT_CMPA_CH1};
static const uint8_t lvd_support_dfilter[LVD_CHANNEL_NUM] = {LVD_SUPPORT_DFILTER_CH1};
static const uint8_t lvd_support_mi[LVD_CHANNEL_NUM]      = {LVD_SUPPORT_MI_CH1};
static const uint8_t lvd_support_nmi[LVD_CHANNEL_NUM]     = {LVD_SUPPORT_NMI_CH1};
#endif

/***********************************************************************************************************************
* Outline      : Checks the specified channel.
* Function Name: lvd_hw_check_param_ch
* Description  : This function checks effectivity of the specified channel.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be checked.
*              : lvd_cfg_opt_t *p_cfg_opt   ; Address of the configuration.
* Return Value : LVD_SUCCESS                ; The value is effective.
*              : LVD ERR_INVALID_CHAN       ; The value is not effective.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_param_ch(lvd_channel_t ch, lvd_cfg_opt_t const *p_cfg_opt)
{
    lvd_err_t result_code = LVD_SUCCESS;
    
#if (LVD_ENABLE == LVD_CFG_PARAM_CHECKING_ENABLE)
    if (LVD_CHANNEL_NUM <= ch)
    {
        result_code = LVD_ERR_INVALID_CHAN;
        goto RETURN_LVD_HW_CHECK_PARAM_CH;
    }

    if (LVD_ENABLE != (p_cfg_opt + ch)->lvd_ch_used)
    {
        result_code = LVD_ERR_INVALID_CHAN;
        goto RETURN_LVD_HW_CHECK_PARAM_CH;
    }
#endif

RETURN_LVD_HW_CHECK_PARAM_CH:
{
    return result_code;
}
} /* End of function lvd_hw_check_param_ch() */

/***********************************************************************************************************************
* Outline      : Checks the specified address.
* Function Name: lvd_hw_check_ptr
* Description  : This function checks effectivity of the specified address.
* Arguments    : void *p_ptr                ; Address of the parameter.
* Return Value : LVD_SUCCESS                ; The address is effective.
*              : LVD LVD_ERR_INVALID_PTR    ; The address is not effective.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_ptr(void const *p_ptr)
{
    lvd_err_t result_code = LVD_SUCCESS;
    
    /* Check data pointer */
    if ((NULL == p_ptr) || (FIT_NO_PTR == p_ptr))
    {
        result_code = LVD_ERR_INVALID_PTR;
        goto RETURN_LVD_HW_CHECK_PTR;
    }
    
RETURN_LVD_HW_CHECK_PTR:
{
    return result_code;
}
} /* End of function lvd_hw_check_ptr() */

/***********************************************************************************************************************
* Outline      : Checks the LVD channel status.
* Function Name: lvd_hw_check_already_open
* Description  : This function checks the LVD FIT module channel status.
* Arguments    : bool b_open_flag           ; The LVD FIT module channel status.
* Return Value : LVD_SUCCESS                ; The status is not 'Opened'.
*              : LVD_ERR_ALREADY_OPEN       ; The status is already 'Opened'.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_already_open(bool b_open_flag)
{
    lvd_err_t result_code = LVD_SUCCESS;
    
    if (true == b_open_flag)
    {
        result_code = LVD_ERR_ALREADY_OPEN;
        goto RETURN_LVD_HW_CHECK_ALREADY_OPEN;
    }
    
RETURN_LVD_HW_CHECK_ALREADY_OPEN:
{
    return result_code;
}
} /* End of function lvd_hw_check_already_open() */

/***********************************************************************************************************************
* Outline      : Checks the LVD channel status.
* Function Name: lvd_hw_check_not_opened
* Description  : This function checks the LVD FIT module channel status.
* Arguments    : bool b_open_flag           ; The LVD FIT module channel status.
* Return Value : LVD_SUCCESS                ; The status is already 'Opened'.
*              : LVD_ERR_NOT_OPENED         ; The status is not 'Opened'.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_not_opened(bool b_open_flag)
{
    lvd_err_t result_code = LVD_SUCCESS;
    
    if (false == b_open_flag)
    {
        result_code = LVD_ERR_NOT_OPENED;
        goto RETURN_LVD_HW_CHECK_NOT_OPENED;
    }
    
RETURN_LVD_HW_CHECK_NOT_OPENED:
{
    return result_code;
}
} /* End of function lvd_hw_check_not_opened() */

/***********************************************************************************************************************
* Outline      : Checks a support status of the R_LVD_GetStatus function.
* Function Name: lvd_hw_check_getstatus
* Description  : This function checks a support status of the R_LVD_GetStatus function.
* Arguments    : none
* Return Value : LVD_SUCCESS                ; This function is supported.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_getstatus(void)
{
    return LVD_SUCCESS;
} /* End of function lvd_hw_check_getstatus() */

/***********************************************************************************************************************
* Outline      : Checks a support status of the R_LVD_ClearStatus function.
* Function Name: lvd_hw_check_clearstatus
* Description  : This function checks a support status of the R_LVD_ClearStatus function.
* Arguments    : none
* Return Value : LVD_SUCCESS                ; This function is supported.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_clearstatus(void)
{
    return LVD_SUCCESS;
} /* End of function lvd_hw_check_clearstatus() */

/***********************************************************************************************************************
* Outline      : Checks parameters used in the R_LVD_Open funcion.
* Function Name: lvd_hw_check_param_open
* Description  : This function checks parameters used in the R_LVD_Open function.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be checked.
*              : lvd_config_t *p_cfg        ; Address of the configuration.
*              : void (*p_cb)(void *)       ; Address of the callback function.
*              : lvd_cfg_opt_t *p_cfg_opt   ; Address of the configuration.
* Return Value : LVD_SUCCESS                ; parameters are effective.
*              : LVD_ERR_INVALID_DATA       ; The definition of the configuration option is invalid.
*              : LVD_ERR_UNSUPPORTED        ; Selected function not supported.
*              : LVD_ERR_INVALID_FUNC       ; Address in the p_callback parameter is invalid.
*              : LVD_ERR_INVALID_ARG        ; The argument of the p_cfg parameter is invalid.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_param_open(lvd_channel_t ch, 
                                  lvd_config_t const *p_cfg, 
                                  void (*p_cb)(void *), 
                                  lvd_cfg_opt_t const *p_cfg_opt)
{
    lvd_err_t result_code = LVD_SUCCESS;
    
#if (LVD_ENABLE == LVD_CFG_PARAM_CHECKING_ENABLE)
    if (LVD_VDET_TARGET_INVALID <= p_cfg_opt->lvd_target)
    {
        result_code = LVD_ERR_INVALID_DATA;
        goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
    }

    if (LVD_VDET_TARGET_CMPA == p_cfg_opt->lvd_target)
    {
        if (LVD_DISABLE == lvd_support_cmpa[ch])
        {
            result_code = LVD_ERR_UNSUPPORTED;
            goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
        }
    }
    
    if (LVD_INVALID_VALUE == p_cfg_opt->lvd_voltage_level_value)
    {
        result_code = LVD_ERR_INVALID_DATA;
        goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
    }
    
    if (LVD_ENABLE_INVALID <= p_cfg_opt->lvd_dfilter)
    {
        result_code = LVD_ERR_INVALID_DATA;
        goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
    }

    if (LVD_ENABLE == p_cfg_opt->lvd_dfilter)
    {
        if (LVD_DISABLE == lvd_support_dfilter[ch])
        {
            result_code = LVD_ERR_UNSUPPORTED;
            goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
        }

        if (LVD_INVALID_VALUE == p_cfg_opt->lvd_dfilter_div_value)
        {
            result_code = LVD_ERR_INVALID_DATA;
            goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
        }
    }
    
    if (LVD_ACTION_INVALID <= p_cfg_opt->lvd_action)
    {
        result_code = LVD_ERR_INVALID_DATA;
        goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
    }

    switch (p_cfg_opt->lvd_action)
    {
        case LVD_ACTION_RESET:
        {
            if (LVD_STAB_INVALID <= p_cfg_opt->lvd_reset_negate)
            {
                result_code = LVD_ERR_INVALID_DATA;
                goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
            }
        break;
        }
        case LVD_ACTION_MI:
        {
            if (LVD_DISABLE == lvd_support_mi[ch])
            {
                result_code = LVD_ERR_UNSUPPORTED;
                goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
            }
            if (LVD_MAX_MI_PRIO < p_cfg_opt->lvd_int_prio)
            {
                result_code = LVD_ERR_INVALID_DATA;
                goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
            }
            /* Check data pointer */
            if ((NULL == p_cb) || (FIT_NO_FUNC == p_cb))
            {
                result_code = LVD_ERR_INVALID_FUNC;
                goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
            }
        break;
        }
        case LVD_ACTION_NMI:
        {
            if (LVD_DISABLE == lvd_support_nmi[ch])
            {
                result_code = LVD_ERR_UNSUPPORTED;
                goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
            }
            /* Check data pointer */
            if ((NULL == p_cb) || (FIT_NO_FUNC == p_cb))
            {
                result_code = LVD_ERR_INVALID_FUNC;
                goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
            }
        break;
        }
        default:
        {
            /* do nothing */
            ;
        break;
        }
    }
    
    if (LVD_TRIGGER_INVALID <= p_cfg->trigger)
    {
        result_code = LVD_ERR_INVALID_ARG;
        goto RETURN_LVD_HW_CHECK_PARAM_OPEN;
    }

#endif /* LVD_CFG_PARAM_CHECKING_ENABLE */
    
RETURN_LVD_HW_CHECK_PARAM_OPEN:
{
    return result_code;
}
} /* End of function lvd_hw_check_param_open() */

/***********************************************************************************************************************
* Outline      : Checks the status of LOCO.
* Function Name: lvd_hw_check_loco_limitation
* Description  : This function checks the status of LOCO.
* Arguments    : lvd_cfg_opt_t *p_cfg_opt   ; Address of the configuration.
* Return Value : LVD_SUCCESS                ; LOCO status is effective.
*              : LVD_ERR_LOCO_STOPPED       ; Setting invalid while LOCO stops.
***********************************************************************************************************************/
lvd_err_t lvd_hw_check_loco_limitation(lvd_cfg_opt_t const *p_cfg_opt)
{
    lvd_err_t result_code = LVD_SUCCESS;
    
    /* LOCO */
    if (1 == SYSTEM.LOCOCR.BIT.LCSTP)
    {
        /*  */
        if (LVD_ACTION_RESET == p_cfg_opt->lvd_action)
        {
            if (LVD_STAB_RESET_ASSERT == p_cfg_opt->lvd_reset_negate)
            {
                result_code = LVD_ERR_LOCO_STOPPED;
                goto RETURN_LVD_HW_CHECK_LOCO_LIMITATION;
            }
        }

        /*  */
        if (LVD_ENABLE == p_cfg_opt->lvd_dfilter)
        {
            result_code = LVD_ERR_LOCO_STOPPED;
            goto RETURN_LVD_HW_CHECK_LOCO_LIMITATION;
        }
    }
    
RETURN_LVD_HW_CHECK_LOCO_LIMITATION:
{
    return result_code;
}
} /* End of function lvd_hw_check_loco_limitation() */

/***********************************************************************************************************************
* Outline      : Clears the voltage crossing status.
* Function Name: lvd_hw_clear_lvd_status
* Description  : This function clears the voltage crossing status.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to clear the voltage crossing status.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_clear_lvd_status(lvd_channel_t ch)
{
    uint8_t volatile dummy_read;

    if (LVD_CHANNEL_1 == ch)
    {
        /* Set LVD1DET to 0 */
        SYSTEM.LVD1SR.BIT.LVD1DET = 0;

        /* Wait for a value to be reflected to a register. */
        dummy_read = SYSTEM.LVD1SR.BIT.LVD1DET;
        
        /* Wait for PCLKB 2cycles time. */
        dummy_read = SYSTEM.LVDLVLR.BYTE;
    }
#ifndef  BSP_MCU_RX23W
    else
    {
            /* Set LVD2DET to 0 */
            SYSTEM.LVD2SR.BIT.LVD2DET = 0;

            /* Wait for a value to be reflected to a register. */
            dummy_read = SYSTEM.LVD2SR.BIT.LVD2DET;

            /* Wait for PCLKB 2cycles time. */
            dummy_read = SYSTEM.LVDLVLR.BYTE;
    }
#endif

    return ;
} /* End of function lvd_hw_clear_lvd_status() */

/***********************************************************************************************************************
* Outline      : Obtains the LVD status.
* Function Name: lvd_hw_get_lvd_status
* Description  : This function obtains the LVD status.
* Arguments    : lvd_channel_t ch            ; Enumerated channel number to obtain the status.
*              : lvd_status_position_t *p_pos; Address to store the enumerated voltage position status.
*              : lvd_status_cross_t *p_cross ; Address to store the enumerated voltage crossing status.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_get_lvd_status(lvd_channel_t ch, lvd_status_position_t *p_pos, lvd_status_cross_t *p_cross)
{
    if (LVD_CHANNEL_1 == ch)
    {
        /* Check LVD1DET = 0 */
        if (0 == SYSTEM.LVD1SR.BIT.LVD1DET)
        {
            *p_cross = LVD_STATUS_CROSS_NONE;
        }
        else
        {
            *p_cross = LVD_STATUS_CROSS_OVER;
        }
        /* Check LVD1MON = 0 */
        if (0 == SYSTEM.LVD1SR.BIT.LVD1MON)
        {
            *p_pos = LVD_STATUS_POSITION_BELOW;
        }
        else
        {
            *p_pos = LVD_STATUS_POSITION_ABOVE;
        }
    }
#ifndef  BSP_MCU_RX23W
    else
    {

    /* Check LVD2DET = 0 */
    if (0 == SYSTEM.LVD2SR.BIT.LVD2DET)
    {
        *p_cross = LVD_STATUS_CROSS_NONE;
    }
    else
    {
        *p_cross = LVD_STATUS_CROSS_OVER;
    }
    /* Check LVD2MON = 0 */
    if (0 == SYSTEM.LVD2SR.BIT.LVD2MON)
    {
        *p_pos = LVD_STATUS_POSITION_BELOW;
    }
    else
    {
        *p_pos = LVD_STATUS_POSITION_ABOVE;
    }
    }
#endif
    
    return ;
} /* End of function lvd_hw_get_lvd_status() */

/***********************************************************************************************************************
* Outline      : Set the voltage detection level.
* Function Name: lvd_hw_set_level
* Description  : This function set the voltage detection level.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : uint16_t level_value       ; Specified the voltage detection level.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_set_level(lvd_channel_t ch, uint16_t level_value)
{
    if (LVD_INVALID_VALUE != level_value)
    {
        if (LVD_CHANNEL_1 == ch)
        {
            /* Set LVD1LVL */
            SYSTEM.LVDLVLR.BIT.LVD1LVL = level_value;
        }
#ifndef  BSP_MCU_RX23W
        else
        {
            /* Set LVD2LVL */
            SYSTEM.LVDLVLR.BIT.LVD2LVL = level_value;
        }
#endif
    }

    return ;
} /* End of function lvd_hw_set_level() */

/***********************************************************************************************************************
* Outline      : Set the voltage detection conditions.
* Function Name: lvd_hw_set_trigger
* Description  : This function set the voltage detection conditions.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : lvd_trigger_t trigger      ; Enumerated voltage detection conditions.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_set_trigger(lvd_channel_t ch, lvd_trigger_t trigger)
{
    if (LVD_CHANNEL_1 == ch)
    {
        switch (trigger)
        {
            case LVD_TRIGGER_RISE:
            {
                /* Set LVD1IDTSEL */
                SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 0;

            break;
            }
            case LVD_TRIGGER_FALL:
            {
                /* Set LVD1IDTSEL */
                SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 1;
            break;
            }
            case LVD_TRIGGER_BOTH:
            {
                /* Set LVD1IDTSEL */
                SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 2;
            break;
            }
            default:
            {
                /* Set LVD1IDTSEL */
                SYSTEM.LVD1CR1.BIT.LVD1IDTSEL = 1;
            break;
            }
        }
    }
#ifndef  BSP_MCU_RX23W
    else
    {
        switch (trigger)
        {
            case LVD_TRIGGER_RISE:
            {
                /* Set LVD2IDTSEL */
                SYSTEM.LVD2CR1.BIT.LVD2IDTSEL = 0;
            break;
            }
            case LVD_TRIGGER_FALL:
            {
                /* Set LVD2IDTSEL */
                SYSTEM.LVD2CR1.BIT.LVD2IDTSEL = 1;
            break;
            }
            case LVD_TRIGGER_BOTH:
            {
                /* Set LVD2IDTSEL */
                SYSTEM.LVD2CR1.BIT.LVD2IDTSEL = 2;
            break;
            }
            default:
            {
                /* Set LVD2IDTSEL */
                SYSTEM.LVD2CR1.BIT.LVD2IDTSEL = 1;
            break;
            }
        }
    }
#endif /* endif of #ifndef  BSP_MCU_RX23W*/
    
    return ;
} /* End of function lvd_hw_set_trigger() */

/***********************************************************************************************************************
* Outline      : Set the target to be monitored.
* Function Name: lvd_hw_set_target
* Description  : This function set the target to be monitored.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : uint16_t target            ; Enumerated target to be monitored.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_set_target(lvd_channel_t ch, uint16_t target)
{
#ifndef  BSP_MCU_RX23W
    if (LVD_CHANNEL_2 == ch)
    {
     #if (LVD_ENABLE == LVD_SUPPORT_CMPA_CH2)
        if (LVD_VDET_TARGET_CMPA == target)
        {
            SYSTEM.LVCMPCR.BIT.EXVCCINP2 = 1;
        }
        else
        {
            SYSTEM.LVCMPCR.BIT.EXVCCINP2 = 0;
        }
      #endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_set_target() */

/***********************************************************************************************************************
* Outline      : Selects reset processing.
* Function Name: lvd_hw_select_reset
* Description  : This function selects reset processing.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_select_reset(lvd_channel_t ch)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH1)||\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH1))
        /* Set LVD1RI */
        SYSTEM.LVD1CR0.BYTE = (SYSTEM.LVD1CR0.BYTE & 0xF7U) | 0x40U;
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH2)||\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH2))
        /* Set LVD2RI */
        SYSTEM.LVD2CR0.BYTE = (SYSTEM.LVD2CR0.BYTE & 0xF7U) | 0x40U;
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_select_reset() */

/***********************************************************************************************************************
* Outline      : Setups reset processing.
* Function Name: lvd_hw_setup_reset
* Description  : This function setups reset processing.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : uint16_t lvd_reset_negate  ; Enumerated reset negation timing.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_setup_reset(lvd_channel_t ch, uint16_t lvd_reset_negate)
{
    if (LVD_CHANNEL_1 == ch)
    {
        if (LVD_STAB_RECOVARY == lvd_reset_negate)
        {
            /* Set LVD1RN */
            SYSTEM.LVD1CR0.BYTE = SYSTEM.LVD1CR0.BYTE & 0x77U;
        }
        else
        {
            /* Set LVD1RN */
            SYSTEM.LVD1CR0.BYTE = (SYSTEM.LVD1CR0.BYTE & 0xF7U) | 0x80U;
        }
    }
    #ifndef  BSP_MCU_RX23W
        else
        {
            if (LVD_STAB_RECOVARY == lvd_reset_negate)
            {
                /* Set LVD2RN */
                SYSTEM.LVD2CR0.BYTE = SYSTEM.LVD2CR0.BYTE & 0x77U;
            }
            else
            {
                /* Set LVD2RN */
                SYSTEM.LVD2CR0.BYTE = (SYSTEM.LVD2CR0.BYTE & 0xF7U) | 0x80U;
            }
        }
    #endif
    return ;
} /* End of function lvd_hw_setup_reset() */

/***********************************************************************************************************************
* Outline      : Selects interrupt processing.
* Function Name: lvd_hw_select_int
* Description  : This function selects interrupt processing.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_select_int(lvd_channel_t ch)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH1)||\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH1))
        /* Set LVD1RI */
        SYSTEM.LVD1CR0.BYTE = SYSTEM.LVD1CR0.BYTE & 0xB7U;
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH2)||\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH2))
        /* Set LVD2RI */
        SYSTEM.LVD2CR0.BYTE = SYSTEM.LVD2CR0.BYTE & 0xB7U;
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_select_int() */

/***********************************************************************************************************************
* Outline      : Selects maskable interrupt processing.
* Function Name: lvd_hw_select_mi
* Description  : This function selects maskable interrupt processing.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_select_mi(lvd_channel_t ch)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH1)&&\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH1))
        /* Set LVD1IRQSEL */
        SYSTEM.LVD1CR1.BIT.LVD1IRQSEL = 1;
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH2)&&\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH2))
        /* Set LVD2IRQSEL */
        SYSTEM.LVD2CR1.BIT.LVD2IRQSEL = 1;
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_select_mi() */

/***********************************************************************************************************************
* Outline      : Selects non-maskable interrupt processing.
* Function Name: lvd_hw_select_nmi
* Description  : This function selects non-maskable interrupt processing.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_select_nmi(lvd_channel_t ch)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH1)&&\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH1))
        /* Set LVD1IRQSEL */
        SYSTEM.LVD1CR1.BIT.LVD1IRQSEL = 0;
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if ((LVD_ENABLE == LVD_SUPPORT_MI_CH2)&&\
     (LVD_ENABLE == LVD_SUPPORT_NMI_CH2))
        /* Set LVD2IRQSEL */
        SYSTEM.LVD2CR1.BIT.LVD2IRQSEL = 0;
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_select_nmi() */

/***********************************************************************************************************************
* Outline      : Setupts digital filter.
* Function Name: lvd_hw_setup_dfilter
* Description  : This function setups digital filter.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : uint16_t clock_value       ; Specified the division ratio of LOCO as the sampling clock.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_setup_dfilter(lvd_channel_t ch, uint16_t clock_value)
{
    if (LVD_INVALID_VALUE != clock_value)
    {
        if (LVD_CHANNEL_1 == ch)
        {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH1)
            /* Set LVD1FSAMP */
            SYSTEM.LVD1CR0.BYTE = (SYSTEM.LVD1CR0.BYTE & 0xC7U) | ((clock_value & 0xFFU) << 4);
#endif
        }
#ifndef  BSP_MCU_RX23W
        else
        {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH2)
            /* Set LVD2FSAMP */
            SYSTEM.LVD2CR0.BYTE = (SYSTEM.LVD2CR0.BYTE & 0xC7U) | ((clock_value & 0xFFU) << 4);
#endif
        }
#endif
    }
    
    return ;
} /* End of function lvd_hw_setup_dfilter() */

/***********************************************************************************************************************
* Outline      : Obtains the LVD circuit status.
* Function Name: lvd_hw_get_circuit_enable
* Description  : This function obtains the LVD circuit status.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool *b_penable_flag       ; Address to store the status.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_get_circuit_enable(lvd_channel_t ch, bool *b_penable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
        /* Check LVD1E */
        if (0 == SYSTEM.LVCMPCR.BIT.LVD1E)
        {
            *b_penable_flag = false;
        }
        else
        {
            *b_penable_flag = true;
        }
    }
    #ifndef  BSP_MCU_RX23W
        else
        {
            /* Check LVD2E */
            if (0 == SYSTEM.LVCMPCR.BIT.LVD2E)
            {
                *b_penable_flag = false;
            }
            else
            {
                *b_penable_flag = true;
            }
        }
    #endif
    return ;
} /* End of function lvd_hw_get_circuit_enable() */

/***********************************************************************************************************************
* Outline      : Obtains the reset and interrupt status.
* Function Name: lvd_hw_get_reset_int_enable
* Description  : This function obtains the reset and interrupt status.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool *b_penable_flag       ; Address to store the status.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_get_reset_int_enable(lvd_channel_t ch, bool *b_penable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
        /* Check LVD1RIE */
        if (0 == SYSTEM.LVD1CR0.BIT.LVD1RIE)
        {
            *b_penable_flag = false;
        }
        else
        {
            *b_penable_flag = true;
        }
    }
    #ifndef  BSP_MCU_RX23W
        else
        {
            /* Check LVD2RIE */
            if (0 == SYSTEM.LVD2CR0.BIT.LVD2RIE)
            {
                *b_penable_flag = false;
            }
            else
            {
                *b_penable_flag = true;
            }
        }
    #endif

    return ;
} /* End of function lvd_hw_get_reset_int_enable() */

/***********************************************************************************************************************
* Outline      : Obtains the digital filter status.
* Function Name: lvd_hw_get_dfilter_enable
* Description  : This function obtains the digital filter status.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool *b_penable_flag       ; Address to store the status.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_get_dfilter_enable(lvd_channel_t ch, bool *b_penable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH1)
        /* Check LVD1DFDIS */
        if (0 == SYSTEM.LVD1CR0.BIT.LVD1DFDIS)
        {
            *b_penable_flag = true;
        }
        else
        {
            *b_penable_flag = false;
        }
#else
        *b_penable_flag = false;
#endif
    }
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH2)
        /* Check LVD2DFDIS */
        if (0 == SYSTEM.LVD2CR0.BIT.LVD2DFDIS)
        {
            *b_penable_flag = true;
        }
        else
        {
            *b_penable_flag = false;
        }
#else
        *b_penable_flag = false;
#endif
    }
    
    return ;
} /* End of function lvd_hw_get_dfilter_enable() */

/***********************************************************************************************************************
* Outline      : Enables or disables the LVD result output.
* Function Name: lvd_hw_enable_output
* Description  : This function enables or disables the LVD result output.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_enable_output(lvd_channel_t ch, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
        if (true == b_enable_flag)
        {
            /* Set LVD1CMPE */
            SYSTEM.LVD1CR0.BYTE = (SYSTEM.LVD1CR0.BYTE & 0xF7U) | 0x04U;
        }
        else
        {
            /* Set LVD1CMPE */
            SYSTEM.LVD1CR0.BYTE = SYSTEM.LVD1CR0.BYTE & 0xF3U;
        }
    }
#ifndef  BSP_MCU_RX23W
    else
    {
        if (true == b_enable_flag)
        {
            /* Set LVD2CMPE */
            SYSTEM.LVD2CR0.BYTE = (SYSTEM.LVD2CR0.BYTE & 0xF7U) | 0x04U;
        }
        else
        {
            /* Set LVD2CMPE */
            SYSTEM.LVD2CR0.BYTE = SYSTEM.LVD2CR0.BYTE & 0xF3U;
        }
    }
#endif
    return ;
} /* End of function lvd_hw_enable_output() */

/***********************************************************************************************************************
* Outline      : Enables or disables the LVD circuit.
* Function Name: lvd_hw_enable_circuit
* Description  : This function enables or disables the LVD circuit.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_enable_circuit(lvd_channel_t ch, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
        if (true == b_enable_flag)
        {
            /* Set LVD1E */
            SYSTEM.LVCMPCR.BIT.LVD1E = 1;
        }
        else
        {
            /* Set LVD1E */
            SYSTEM.LVCMPCR.BIT.LVD1E = 0;
        }
    }
    #ifndef BSP_MCU_RX23W
        else
        {
            if (true == b_enable_flag)
            {
                /* Set LVD2E */
                SYSTEM.LVCMPCR.BIT.LVD2E = 1;
            }
            else
            {
                /* Set LVD2E */
                SYSTEM.LVCMPCR.BIT.LVD2E = 0;
            }
        }
    #endif
    
    return ;
} /* End of function lvd_hw_enable_circuit() */

/***********************************************************************************************************************
* Outline      : Enables or disables the LVD reset and interrupt.
* Function Name: lvd_hw_enable_reset_int
* Description  : This function enables or disables the LVD reset and interrupt.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_enable_reset_int(lvd_channel_t ch, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
        if (true == b_enable_flag)
        {
            /* Set LVD1RIE */
            SYSTEM.LVD1CR0.BYTE = (SYSTEM.LVD1CR0.BYTE & 0xF7U) | 0x01U;
        }
        else
        {
            /* Set LVD1RIE */
            SYSTEM.LVD1CR0.BYTE = SYSTEM.LVD1CR0.BYTE & 0xF6U;
        }
    }
    #ifndef BSP_MCU_RX23W
        else
        {
            if (true == b_enable_flag)
            {
                /* Set LVD2RIE */
                SYSTEM.LVD2CR0.BYTE = (SYSTEM.LVD2CR0.BYTE & 0xF7U) | 0x01U;
            }
            else
            {
                /* Set LVD2RIE */
                SYSTEM.LVD2CR0.BYTE = SYSTEM.LVD2CR0.BYTE & 0xF6U;
            }
        }
    #endif
    
    return ;
} /* End of function lvd_hw_enable_reset_int() */

/***********************************************************************************************************************
* Outline      : Enables or disables digital filter.
* Function Name: lvd_hw_enable_dfilter
* Description  : This function enables or disables digital filter.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_enable_dfilter(lvd_channel_t ch, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH1)
        if (true == b_enable_flag)
        {
            /* Set LVD1DFDIS */
            SYSTEM.LVD1CR0.BYTE = SYSTEM.LVD1CR0.BYTE & 0xF5U;
        }
        else
        {
            /* Set LVD1DFDIS */
            SYSTEM.LVD1CR0.BYTE = (SYSTEM.LVD1CR0.BYTE & 0xF7U) | 0x02U;
        }
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH2)
        if (true == b_enable_flag)
        {
            /* Set LVD2DFDIS */
            SYSTEM.LVD2CR0.BYTE = SYSTEM.LVD2CR0.BYTE & 0xF5U;
        }
        else
        {
            /* Set LVD2DFDIS */
            SYSTEM.LVD2CR0.BYTE = (SYSTEM.LVD2CR0.BYTE & 0xF7U) | 0x02U;
        }
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_enable_dfilter() */

/***********************************************************************************************************************
* Outline      : Enables or disables maskable interrupt.
* Function Name: lvd_hw_enable_mi
* Description  : This function enables or disables maskable interrupt.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : uint16_t prio              ; The interrupt priority level.
*              : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
#if ((LVD_GROUP_INT_ICUA == LVD_GROUP_INT)||\
     (LVD_GROUP_INT_ICUB == LVD_GROUP_INT)||\
     (LVD_GROUP_INT_ICUD == LVD_GROUP_INT))
void lvd_hw_enable_mi(lvd_channel_t ch, uint16_t prio, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH1)
        if (true == b_enable_flag)
        {
            /* Set IR to 0 */
            IR(LVD1, LVD1)  = 0;
            /* Set IPR to prio */
            IPR(LVD1, LVD1) = prio;
            /* Set IEN to 1 */
            R_BSP_InterruptRequestEnable(VECT(LVD1, LVD1));
        }
        else
        {
            /* Set IEN to 0 */
            R_BSP_InterruptRequestDisable(VECT(LVD1, LVD1));
        }
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH2)
        if (true == b_enable_flag)
        {
            /* Set IR to 0 */
            IR(LVD2, LVD2)  = 0;
            /* Set IPR to prio */
            IPR(LVD2, LVD2) = prio;
            /* Set IEN to 1 */
            R_BSP_InterruptRequestEnable(VECT(LVD2, LVD2));
        }
        else
        {
            /* Set IEN to 0 */
            R_BSP_InterruptRequestDisable(VECT(LVD2, LVD2));
        }
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_enable_mi() */

#elif (LVD_GROUP_INT_ICUb == LVD_GROUP_INT)
void lvd_hw_enable_mi(lvd_channel_t ch, uint16_t prio, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH1)
        if (true == b_enable_flag)
        {
            /* casting is valid*/
            IR(LVD, LVD1)  = 0;

            /* casting is valid*/
            IPR(LVD, LVD1) = prio;
            R_BSP_InterruptRequestEnable(VECT(LVD, LVD1));
        }
        else
        {
            R_BSP_InterruptRequestDisable(VECT(LVD, LVD1));
        }
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH2)
        if (true == b_enable_flag)
        {
            IR(LVD, LVD2)  = 0;
            IPR(LVD, LVD2) = prio;
            R_BSP_InterruptRequestEnable(VECT(LVD, LVD2));
        }
        else
        {
            R_BSP_InterruptRequestDisable(VECT(LVD, LVD2));
        }
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_enable_mi() */

#else /* Error */
    #error "Error! Invalid setting for LVD_GROUP_INT in targets file."
#endif /* LVD_CFG_PARAM_CHECKING_ENABLE */

/***********************************************************************************************************************
* Outline      : Enables or disables the LVD non-maskable interrupt.
* Function Name: lvd_hw_enable_nmi
* Description  : This function enables or disables the LVD non-maskable interrupt.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be set.
*              : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_enable_nmi(lvd_channel_t ch, bool b_enable_flag)
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_NMI_CH1)
        if (true == b_enable_flag)
        {
            /* Set LVD1CLR = 1 */
            ICU.NMICLR.BIT.LVD1CLR = 1;
            /* Set LVD1EN = 1 */
            ICU.NMIER.BIT.LVD1EN   = 1;
        }
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_NMI_CH2)
        if (true == b_enable_flag)
        {
            /* Set LVD2CLR = 1 */
            ICU.NMICLR.BIT.LVD2CLR = 1;
            /* Set LVD2EN = 1 */
            ICU.NMIER.BIT.LVD2EN   = 1;
        }
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_enable_nmi() */

/***********************************************************************************************************************
* Outline      : Wait for a value to be reflected by a register.
* Function Name: lvd_hw_dummy_read_dfilter
* Description  : This function wait for a value to be reflected by a register.
*              : The target register is digital filter.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be waited.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_dummy_read_dfilter(lvd_channel_t ch)
{
    uint8_t volatile dummy_read;

    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH1)
        /* Read dummy */
        dummy_read = SYSTEM.LVD1CR0.BIT.LVD1DFDIS;
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_DFILTER_CH2)
        /* Read dummy */
        dummy_read = SYSTEM.LVD2CR0.BIT.LVD2DFDIS;
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_dummy_read_dfilter() */

/***********************************************************************************************************************
* Outline      : Wait for a value to be reflected by a register.
* Function Name: lvd_hw_dummy_read_circuit
* Description  : This function wait for a value to be reflected by a register.
*              : The target register is LVD circuit.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be waited.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_dummy_read_circuit(lvd_channel_t ch)
{
    uint8_t volatile dummy_read;

    if (LVD_CHANNEL_1 == ch)
    {
        /* Read dummy */
        dummy_read = SYSTEM.LVCMPCR.BIT.LVD1E;
    }
#ifndef BSP_MCU_RX23W
    else
    {
        /* Read dummy */
        dummy_read = SYSTEM.LVCMPCR.BIT.LVD2E;
    }
#endif
    
    return ;
} /* End of function lvd_hw_dummy_read_circuit() */

/***********************************************************************************************************************
* Outline      : Wait for a value to be reflected by a register.
* Function Name: lvd_hw_dummy_read_output
* Description  : This function wait for a value to be reflected by a register.
*              : The target register is LVD result output.
* Arguments    : lvd_channel_t ch           ; Enumerated channel number to be waited.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_dummy_read_output(lvd_channel_t ch)
{
    uint8_t volatile dummy_read;

    if (LVD_CHANNEL_1 == ch)
    {
        /* Read dummy */
        dummy_read = SYSTEM.LVD1CR0.BIT.LVD1CMPE;
    }
#ifndef BSP_MCU_RX23W
    else
    {
        /* Read dummy */
        dummy_read = SYSTEM.LVD2CR0.BIT.LVD2CMPE;
    }
#endif

    return ;
} /* End of function lvd_hw_dummy_read_output() */

/***********************************************************************************************************************
* Outline      : Set the callback function called by maskable interrup.
* Function Name: lvd_hw_set_mi_cb
* Description  : This function set the callback function called by maskable interrup.
* Arguments    : lvd_channel_t ch              ; Enumerated channel number to be set.
*              : void (*p_cb)(void *)          ; Address of the callback function.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_set_mi_cb(lvd_channel_t ch, void (*p_cb)(void *))
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH1)
        p_lvd_cb_ch1 = p_cb;
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH2)
        p_lvd_cb_ch2 = p_cb;
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_set_mi_cb() */

/***********************************************************************************************************************
* Outline      : Set the callback function called by non-maskable interrup.
* Function Name: lvd_hw_set_nmi_cb
* Description  : This function set the callback function called by non-maskable interrup.
* Arguments    : lvd_channel_t ch              ; Enumerated channel number to be set.
*              : void (*p_cb)(void *)          ; Address of the callback function.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_set_nmi_cb(lvd_channel_t ch, void (*p_cb)(void *))
{
    if (LVD_CHANNEL_1 == ch)
    {
#if (LVD_ENABLE == LVD_SUPPORT_NMI_CH1)
        /* Cast type void * to type bsp_int_cb_t */
        R_BSP_InterruptWrite(BSP_INT_SRC_LVD1, (bsp_int_cb_t)p_cb);
#endif
    }
#ifndef  BSP_MCU_RX23W
    else
    {
#if (LVD_ENABLE == LVD_SUPPORT_NMI_CH2)
        /* Cast type void * to type bsp_int_cb_t */
        R_BSP_InterruptWrite(BSP_INT_SRC_LVD2, (bsp_int_cb_t)p_cb);
#endif
    }
#endif
    
    return ;
} /* End of function lvd_hw_set_nmi_cb() */

/***********************************************************************************************************************
* Outline      : Enables or disables the LVD register protection.
* Function Name: lvd_hw_enable_reg_protect
* Description  : This function enables or disables the LVD register protection.
* Arguments    : bool b_enable_flag         ; The flag orders Enable or Disable.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_enable_reg_protect(bool b_enable_flag)
{
    if (true == b_enable_flag)
    {
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LVD);
    }
    else
    {
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LVD);
    }
    return ;
} /* End of function lvd_hw_enable_reg_protect() */

/***********************************************************************************************************************
* Outline      : Wait during some LOCO cycles.
* Function Name: lvd_hw_wait_delay_loco
* Description  : This function wait during some LOCO cycles.
* Arguments    : uint32_t loco_cycle        ; Specified the number of LOCO cycles.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_wait_delay_loco(uint32_t loco_cycle)
{
    uint32_t usec = 0;
    
    if (0 != loco_cycle)
    {
        usec = ((((loco_cycle) * 1000000) + (BSP_LOCO_HZ - 1)) / BSP_LOCO_HZ);
        R_BSP_SoftwareDelay(usec, BSP_DELAY_MICROSECS);
    }

    return ;
} /* End of function lvd_hw_wait_delay_loco() */

/***********************************************************************************************************************
* Outline      : Wait during some micro sec.
* Function Name: lvd_hw_wait_delay
* Description  : This function wait during some micro sec.
* Arguments    : uint32_t usec              ; Specified the number of micro sec.
* Return Value : none
***********************************************************************************************************************/
void lvd_hw_wait_delay(uint32_t usec)
{
    if (0 != usec)
    {
        R_BSP_SoftwareDelay(usec, BSP_DELAY_MICROSECS);
    }

    return ;
} /* End of function lvd_hw_wait_delay() */

/***********************************************************************************************************************
* Outline      : Interrupt LVD1 handler.
* Function Name: lvd_ch1_isr
* Description  : This function handle the LVD1 interrupt.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH1)
#if ((LVD_GROUP_INT_ICUA == LVD_GROUP_INT)||\
     (LVD_GROUP_INT_ICUB == LVD_GROUP_INT)||\
     (LVD_GROUP_INT_ICUD == LVD_GROUP_INT))
R_BSP_PRAGMA_STATIC_INTERRUPT(lvd_ch1_isr,VECT(LVD1,LVD1))
#elif (LVD_GROUP_INT_ICUb == LVD_GROUP_INT)
R_BSP_PRAGMA_STATIC_INTERRUPT(lvd_ch1_isr,VECT(LVD,LVD1))
#else
    #error "Error! Invalid setting for LVD_GROUP_INT in targets file."
#endif

R_BSP_ATTRIB_STATIC_INTERRUPT void lvd_ch1_isr(void)
{
    lvd_int_cb_args_t event_arg;
    
    /* Check data pointer */
    if ((NULL != p_lvd_cb_ch1) && (FIT_NO_FUNC != p_lvd_cb_ch1))
    {
        event_arg.vector = BSP_INT_SRC_LVD1;
        /* Cast type lvd_int_cb_args_t to type void * */
        p_lvd_cb_ch1((void *)&event_arg);
    }

    return ;
} /* End of function lvd_ch1_isr() */

#endif

/***********************************************************************************************************************
* Outline      : Interrupt LVD2 handler.
* Function Name: lvd_ch2_isr
* Description  : This function handle the LVD2 interrupt.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
#ifndef  BSP_MCU_RX23W
#if (LVD_ENABLE == LVD_SUPPORT_MI_CH2)
#if ((LVD_GROUP_INT_ICUA == LVD_GROUP_INT)||\
     (LVD_GROUP_INT_ICUB == LVD_GROUP_INT)||\
     (LVD_GROUP_INT_ICUD == LVD_GROUP_INT))
R_BSP_PRAGMA_STATIC_INTERRUPT(lvd_ch2_isr,VECT(LVD2,LVD2))
#elif (LVD_GROUP_INT_ICUb ==LVD_GROUP_INT)
R_BSP_PRAGMA_STATIC_INTERRUPT(lvd_ch2_isr,VECT(LVD,LVD2))
#else
    #error "Error! Invalid setting for LVD_GROUP_INT in targets file."
#endif
R_BSP_ATTRIB_STATIC_INTERRUPT void lvd_ch2_isr(void)
{
    lvd_int_cb_args_t event_arg;
    
    /* Check data pointer */
    if ((NULL != p_lvd_cb_ch2) && (FIT_NO_FUNC != p_lvd_cb_ch2))
    {
        event_arg.vector = BSP_INT_SRC_LVD2;
        /* Cast type lvd_int_cb_args_t to type void * */
        p_lvd_cb_ch2((void *)&event_arg);
    }

    return ;
} /* End of function lvd_ch2_isr() */

#endif
#endif
/* End of File */

