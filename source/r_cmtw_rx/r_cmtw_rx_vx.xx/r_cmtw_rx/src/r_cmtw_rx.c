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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_cmtw_rx.c
* Description  : This module creates a timer tick using a CMTW channel based on an input by the user.
*              : The module also supports output compare and input capture features to generate
*              : waveforms and detect external events.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.08.2014 1.00    First GSCE Release.
*         : 01.02.2019 1.40    Supported RX65N-64pin.
*                              Corrected interrupt priority level macro names.
*                              Fixed GSCE Code Checker errors.
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
*         : 15.08.2019 2.10    Fixed warnings in IAR.
*         : 30.12.2019 2.20    Added support RX66N, RX72N.
*                              Modified comment of API function to Doxygen style.
*                              Added support for atomic control.
*                              Removed support for Generation 1 devices.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdio.h>

#include "platform.h"
/* Configuration for this module. */
#include "r_cmtw_rx_config.h"
/* Header file for this module. */
#include "r_cmtw_rx_if.h"
/* Internal definitions */
#include "r_cmtw_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CMTW_COUNTF_MIN             (0x00000001)
#define CMTW_COUNTF_MAX             (0xFFFFFFFF)

#define CMTW_ACTION_ISR_ENABLE_MASK     (CMTW_ACTION_INTERRUPT | CMTW_ACTION_CALLBACK | CMTW_ACTION_ONESHOT)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
#if CMTW_CFG_USE_CH0 == 1
/* Channel ROM info */
const cmtw_prv_ch_rom_info_t g_cmtw_rom_ch0 =
{
    &(CMTW0),                           /* Pointer to the registers */
    CMTW_CHANNEL_0,                     /* Channel */
    BIT1_MASK,                          /* MSTP mask bit */
    CMTW_CFG_IPR_CM_CH0,                /* Compare match interrupt priority level */
    CMTW_CFG_IPR_OC0_CH0,               /* Output Compare 0 interrupt priority level */
    CMTW_CFG_IPR_OC1_CH0,               /* Output Compare 1 interrupt priority level */
    CMTW_CFG_IPR_IC0_CH0,               /* Input capture 0 interrupt priority level */
    CMTW_CFG_IPR_IC1_CH0,               /* Input capture 1 interrupt priority level */
    &ICU.IR[IR_CMTW0_CMWI0].BYTE,       /* Pointer to compare match ICU IR register */
    &ICU.IPR[IPR_CMTW0_CMWI0].BYTE,     /* Pointer to compare match ICU IPR register */
    &ICU.IER[IER_CMTW0_CMWI0].BYTE,     /* Pointer to compare match ICU IER register */
    IEN_CMTW0_CMWI0,                    /* Compare match IEN mask bit */
    &ICU.IR[IR_CMTW0_OC0I0].BYTE,       /* Pointer to output compare 0 ICU IR register */
    &ICU.IPR[IPR_CMTW0_OC0I0].BYTE,     /* Pointer to output compare 0 ICU IPR register */
    &ICU.IER[IER_CMTW0_OC0I0].BYTE,     /* Pointer to output compare 0 ICU IER register */
    IEN_CMTW0_OC0I0,                    /* Output compare 0 IEN mask bit */
    &ICU.IR[IR_CMTW0_OC1I0].BYTE,       /* Pointer to output compare 1 ICU IR register */
    &ICU.IPR[IPR_CMTW0_OC1I0].BYTE,     /* Pointer to output compare 1 ICU IPR register */
    &ICU.IER[IER_CMTW0_OC1I0].BYTE,     /* Pointer to output compare 1 ICU IER register */
    IEN_CMTW0_OC1I0,                    /* Output compare 1 IEN mask bit */
    &ICU.IR[IR_CMTW0_IC0I0].BYTE,       /* Pointer to input capture 0 ICU IR register */
    &ICU.IPR[IPR_CMTW0_IC0I0].BYTE,     /* Pointer to input capture 0 ICU IPR register */
    &ICU.IER[IER_CMTW0_IC0I0].BYTE,     /* Pointer to input capture 0 ICU IER register */
    IEN_CMTW0_IC0I0,                    /* Input capture 0 IEN mask bit */
    &ICU.IR[IR_CMTW0_IC1I0].BYTE,       /* Pointer to input capture 0 ICU IR register */
    &ICU.IPR[IPR_CMTW0_IC1I0].BYTE,     /* Pointer to input capture 0 ICU IPR register */
    &ICU.IER[IER_CMTW0_IC1I0].BYTE,     /* Pointer to input capture 0 ICU IER register */
    IEN_CMTW0_IC1I0,                    /* Input capture 1 IEN mask bit */
};

/* Channel control info */
static cmtw_prv_ch_ctrl_info_t g_cmtw_ctrl_ch0 =
{
    &g_cmtw_rom_ch0,                    /* Pointer to ROM info */
    NULL,                               /* Pointer to callback */
    CMTW_PRV_CHANNEL_STATE_CLOSED,      /* Channel state */
    CMTW_ACTION_NONE,                   /* Compare match action */
    CMTW_ACTION_NONE,                   /* Output compare 0 action */
    CMTW_ACTION_NONE,                   /* Output compare 1 action */
    CMTW_ACTION_NONE,                   /* Input capture 0 action */
    CMTW_ACTION_NONE,                   /* Input capture 1 action */
};
#endif /* End of #if CMTW_CFG_USE_CH0 == 1 */

#if CMTW_CFG_USE_CH1 == 1
/* Channel ROM info */
const cmtw_prv_ch_rom_info_t g_cmtw_rom_ch1 =
{
    &(CMTW1),                           /* Pointer to the registers */
    CMTW_CHANNEL_1,                     /* Channel */
    BIT0_MASK,                          /* MSTP mask bit */
    CMTW_CFG_IPR_CM_CH1,                /* Compare match interrupt priority level */
    CMTW_CFG_IPR_OC0_CH1,               /* Output Compare 0 interrupt priority level */
    CMTW_CFG_IPR_OC1_CH1,               /* Output Compare 1 interrupt priority level */
    CMTW_CFG_IPR_IC0_CH1,               /* Input capture 0 interrupt priority level */
    CMTW_CFG_IPR_IC1_CH1,               /* Input capture 1 interrupt priority level */
    &ICU.IR[IR_CMTW1_CMWI1].BYTE,       /* Pointer to compare match ICU IR register */
    &ICU.IPR[IPR_CMTW1_CMWI1].BYTE,     /* Pointer to compare match ICU IPR register */
    &ICU.IER[IER_CMTW1_CMWI1].BYTE,     /* Pointer to compare match ICU IER register */
    IEN_CMTW1_CMWI1,                    /* Compare match IEN mask bit */
    &ICU.IR[IR_CMTW1_OC0I1].BYTE,       /* Pointer to output compare 0 ICU IR register */
    &ICU.IPR[IPR_CMTW1_OC0I1].BYTE,     /* Pointer to output compare 0 ICU IPR register */
    &ICU.IER[IER_CMTW1_OC0I1].BYTE,     /* Pointer to output compare 0 ICU IER register */
    IEN_CMTW1_OC0I1,                    /* Output compare 0 IEN mask bit */
    &ICU.IR[IR_CMTW1_OC1I1].BYTE,       /* Pointer to output compare 1 ICU IR register */
    &ICU.IPR[IPR_CMTW1_OC1I1].BYTE,     /* Pointer to output compare 1 ICU IPR register */
    &ICU.IER[IER_CMTW1_OC1I1].BYTE,     /* Pointer to output compare 1 ICU IER register */
    IEN_CMTW1_OC1I1,                    /* Output compare 1 IEN mask bit */
    &ICU.IR[IR_CMTW1_IC0I1].BYTE,       /* Pointer to input capture 0 ICU IR register */
    &ICU.IPR[IPR_CMTW1_IC0I1].BYTE,     /* Pointer to input capture 0 ICU IPR register */
    &ICU.IER[IER_CMTW1_IC0I1].BYTE,     /* Pointer to input capture 0 ICU IER register */
    IEN_CMTW1_IC0I1,                    /* Input capture 0 IEN mask bit */
    &ICU.IR[IR_CMTW1_IC1I1].BYTE,       /* Pointer to input capture 0 ICU IR register */
    &ICU.IPR[IPR_CMTW1_IC1I1].BYTE,     /* Pointer to input capture 0 ICU IPR register */
    &ICU.IER[IER_CMTW1_IC1I1].BYTE,     /* Pointer to input capture 0 ICU IER register */
    IEN_CMTW1_IC1I1,                    /* Input capture 1 IEN mask bit */
};

/* Channel control info */
static cmtw_prv_ch_ctrl_info_t g_cmtw_ctrl_ch1 =
{
    &g_cmtw_rom_ch1,                    /* Pointer to ROM info */
    NULL,                               /* Pointer to callback */
    CMTW_PRV_CHANNEL_STATE_CLOSED,      /* Channel state */
    CMTW_ACTION_NONE,                   /* Compare match action */
    CMTW_ACTION_NONE,                   /* Output compare 0 action */
    CMTW_ACTION_NONE,                   /* Output compare 1 action */
    CMTW_ACTION_NONE,                   /* Input capture 0 action */
    CMTW_ACTION_NONE,                   /* Input capture 1 action */
};
#endif /* End of #if CMTW_CFG_USE_CH1 == 1 */

cmtw_prv_ch_ctrl_info_t * const g_cmtw_handles[] =
{
    #if CMTW_CFG_USE_CH0 == 1
    &g_cmtw_ctrl_ch0,
    #else
    NULL,
    #endif
    #if CMTW_CFG_USE_CH1 == 1
    &g_cmtw_ctrl_ch1,
    #else
    NULL,
    #endif
};

static const uint32_t g_cmtw_clock_divisors[] = {8, 32, 128, 512};
static const float g_cmtw_time_units[] = {1e-9, 1e-6, 1e-3, 1};

static void power_on(cmtw_prv_ch_ctrl_info_t  * const hdl);
static void power_off(cmtw_prv_ch_ctrl_info_t  * const hdl);
static uint32_t calculate_count(cmtw_time_unit_t time_unit, cmtw_clock_divisor_t clock_divisor, uint32_t time);
static void cmtw_isr_common(cmtw_event_t event, uint32_t count, cmtw_prv_ch_ctrl_info_t * const hdl);

/***********************************************************************************************************************
* Function Name: R_CMTW_Open
********************************************************************************************************************//**
* @brief This function powers up the specified CMTW channel, initializes the associated CMTW registers and enables
* interrupts if requested.  Takes a callback function pointer for responding to interrupt events.  After successful
* completion, the channel moves to open state.  This function must be called before calling any other API functions.
* @param[in] channel Number of the CMTW channel to initialize
* @param[in] pconfig Pointer to the CMTW channel settings data structure
* @param[in] pcallback Pointer to the user function to call from the interrupt
* @retval CMTW_SUCCESS Successful, channel is initialized
* @retval CMTW_ERR_BAD_CHAN Invalid channel number
* @retval CMTW_ERR_CH_NOT_ENABLED Channel is disabled by user configuration
* @retval CMTW_ERR_CH_NOT_CLOSED Channel currently in operation, perform R_CMTW_Close() first
* @retval CMTW_ERR_INVALID_ARG An element of the pconfig structure contains an invalid value
* @retval CMTW_ERR_OUT_OF_RANGE Calculated count value is not in range
* @retval CMTW_ERR_NULL_PTR Either pconfig or pcallback is null
* @retval CMTW_ERR_LOCK The lock could not be acquired, the channel is busy
* @details This function sets up a CMTW channel.  After completion of the open function the CMTW channel will be
* initialized and ready to be started by calling R_CMTW_Control().  This function must be called once prior to calling
* any other CMTW API functions. Once successfully completed, the status of the selected CMTW channel will be set
* to "open".  After that this function should not be called again for the same CMTW channel without first performing
* a "close" by calling R_CMTW_Close().
* @note None
*/
cmtw_err_t    R_CMTW_Open(cmtw_channel_t          channel,
                          cmtw_channel_settings_t *pconfig,
                          void                    (* const pcallback)(void *pdata))
{
    cmtw_prv_ch_ctrl_info_t *hdl;
    uint32_t                count;


#if CMTW_CGF_REQUIRE_LOCK == 1
    bool lock_result;
#endif

#if CMTW_CFG_PARAM_CHECKING_ENABLE == 1
    /* Parameter check - channel */
    if (CMTW_CHANNEL_MAX <= channel)
    {
        return(CMTW_ERR_BAD_CHAN);
    }

    /* Parameter check - pconfig */
    if ((NULL == (void *)pconfig) || (FIT_NO_FUNC == (void (*)(void *))pconfig))
    {
        return(CMTW_ERR_NULL_PTR);
    }

    /* Parameter check - time unit, clock divisor, clear source */
    if ((CMTW_TIME_MAX <= pconfig->time_unit) ||
        (CMTW_CLK_DIV_MAX <= pconfig->clock_divisor) ||
        (CMTW_CLR_MAX <= pconfig->clear_source) || (2 == pconfig->clear_source) || (3 == pconfig->clear_source))
    {
        return(CMTW_ERR_INVALID_ARG);
    }

    /* Parameter check - compare match */
    if (CMTW_ACTION_NONE != pconfig->cm_timer.actions)
    {
        /* Check callback pointer */
        if (CMTW_ACTION_CALLBACK & pconfig->cm_timer.actions)
        {
            /* Return CMTW_ERR_NULL_PTR if NULL or FIT_NO_FUNC */
            if ((NULL == pcallback) || (FIT_NO_FUNC == pcallback))
            {
                return(CMTW_ERR_NULL_PTR);
            }
        }
    }

    /* Parameter check - output compare 0 */
    if (CMTW_ACTION_NONE != pconfig->oc_timer_0.actions)
    {
        /* Check callback pointer */
        if (CMTW_ACTION_CALLBACK & pconfig->oc_timer_0.actions)
        {
            /* Return CMTW_ERR_NULL_PTR if NULL or FIT_NO_FUNC */
            if ((NULL == pcallback) || (FIT_NO_FUNC == pcallback))
            {
                return(CMTW_ERR_NULL_PTR);
            }
        }

        /* Check parameters */
        if (CMTW_OUTPUT_MAX <= pconfig->oc_timer_0.output)
        {
            return (CMTW_ERR_INVALID_ARG);
        }
    }

    /* Parameter check - output compare 1 */
    if (CMTW_ACTION_NONE != pconfig->oc_timer_1.actions)
    {
        /* Check callback pointer */
        if (CMTW_ACTION_CALLBACK & pconfig->oc_timer_1.actions)
        {
            /* Return CMTW_ERR_NULL_PTR if NULL or FIT_NO_FUNC */
            if ((NULL == pcallback) || (FIT_NO_FUNC == pcallback))
            {
                return (CMTW_ERR_NULL_PTR);
            }
        }

        /* Check parameters */
        if (CMTW_OUTPUT_MAX <= pconfig->oc_timer_1.output)
        {
            return (CMTW_ERR_INVALID_ARG);
        }

    }

    /* Parameter check - input capture 0 */
    if (CMTW_ACTION_NONE != pconfig->ic_timer_0.actions)
    {
        /* Check callback pointer */
        if (CMTW_ACTION_CALLBACK & pconfig->ic_timer_0.actions)
        {
            /* Return CMTW_ERR_NULL_PTR if NULL or FIT_NO_FUNC */
            if ((NULL == pcallback) || (FIT_NO_FUNC == pcallback))
             {
                 return (CMTW_ERR_NULL_PTR);
             }
        }

        /* Check parameters */
        if (CMTW_EDGE_MAX <= pconfig->ic_timer_0.edge)
        {
            return (CMTW_ERR_INVALID_ARG);
        }
    }

    /* Parameter check - input capture 1 */
    if (CMTW_ACTION_NONE != pconfig->ic_timer_1.actions)
    {
        /* Check callback pointer */
        if (CMTW_ACTION_CALLBACK & pconfig->ic_timer_1.actions)
        {
            /* Return CMTW_ERR_NULL_PTR if NULL or FIT_NO_FUNC */
            if ((NULL == pcallback) || (FIT_NO_FUNC == pcallback))
             {
                 return (CMTW_ERR_NULL_PTR);
             }
        }

        /* Check parameters */
        if (CMTW_EDGE_MAX <= pconfig->ic_timer_1.edge)
        {
            return (CMTW_ERR_INVALID_ARG);
        }
    }
#endif /* End of #if CMTW_CFG_PARAM_CHECKING_ENABLE == 1 */

#if CMTW_CGF_REQUIRE_LOCK == 1
    /* Get lock for this channel */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));

    if (false == lock_result)
    {
        return(CMTW_ERR_LOCK);
    }
#endif

    /* Get the handle for the channel - pointer to the channel control info */
    hdl = g_cmtw_handles[channel];

    /* Check if channel enabled */
    if (NULL == hdl)
    {
#if CMTW_CGF_REQUIRE_LOCK == 1
        /* Release lock for this channel */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
        return (CMTW_ERR_CH_NOT_ENABLED);
    }

    /* Check channel state */
    if (CMTW_PRV_CHANNEL_STATE_CLOSED != hdl->state)
    {
#if CMTW_CGF_REQUIRE_LOCK == 1
        /* Release lock for this channel */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
        return (CMTW_ERR_CH_NOT_CLOSED);
    }

    /* Turn power on to the channel */
    power_on(hdl);

    /* Set clock divisor, counter clear-source, 32-bit operation */
    hdl->rom->regs->CMWCR.BIT.CKS = pconfig->clock_divisor;
    hdl->rom->regs->CMWCR.BIT.CCLR = pconfig->clear_source;
    hdl->rom->regs->CMWCR.BIT.CMS = 0;

    /* Save the callback */
    hdl->pcallback = pcallback;


    /* COMPARE MATCH */
    if (CMTW_ACTION_NONE != pconfig->cm_timer.actions)
    {
        /* Calculate timer count */
        count = calculate_count(pconfig->time_unit,
                                pconfig->clock_divisor,
                                pconfig->cm_timer.time);

        /* Set timer counter if in range, otherwise return */
        if (0 == count)
        {
#if CMTW_CGF_REQUIRE_LOCK == 1
            /* Release lock for this channel */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
            R_CMTW_Close(channel);
            return(CMTW_ERR_OUT_OF_RANGE);
        }

        /* Set the compare match count */
        hdl->rom->regs->CMWCOR = count;

        /* Enable compare match */
        hdl->rom->regs->CMWIOR.BIT.CMWE = 1;

        if (CMTW_ACTION_ISR_ENABLE_MASK & pconfig->cm_timer.actions)
        {
            /* Save action for ISR processing */
            hdl->cm_action = pconfig->cm_timer.actions;

            /* Clear ICU interrupt request, set level, and enable it */
            *hdl->rom->ir_cm = 0;
            *hdl->rom->ipr_cm = hdl->rom->cm_priorty;
            (*hdl->rom->ier_cm) |= hdl->rom->cm_enable_mask;

            /* Enable compare match interrupt */
            hdl->rom->regs->CMWCR.BIT.CMWIE = 1;
        }

        /* Change state to open */
        hdl->state = CMTW_PRV_CHANNEL_STATE_OPENED;
    }

    /* OUTPUT COMPARE 0 */
    if (CMTW_ACTION_NONE != pconfig->oc_timer_0.actions)
    {
        /* Calculate timer count */
        count = calculate_count(pconfig->time_unit,
                                pconfig->clock_divisor,
                                pconfig->oc_timer_0.time);

        /* Set timer counter if in range, otherwise return */
        if (0 == count)
        {
#if CMTW_CGF_REQUIRE_LOCK == 1
            /* Release lock for this channel */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
            R_CMTW_Close(channel);
            return(CMTW_ERR_OUT_OF_RANGE);
        }

        /* Set the output compare count */
        hdl->rom->regs->CMWOCR0 = count;

        /* Enable output compare */
        hdl->rom->regs->CMWIOR.BIT.OC0E = 1;

        /* Set output compare pin state */
        hdl->rom->regs->CMWIOR.BIT.OC0 = pconfig->oc_timer_0.output;

        if (CMTW_ACTION_ISR_ENABLE_MASK & pconfig->oc_timer_0.actions)
        {
            /* Save action for ISR processing */
            hdl->oc0_action = pconfig->oc_timer_0.actions;

            /* Clear ICU interrupt request, set level, and enable it */
            *hdl->rom->ir_oc0 = 0;
            *hdl->rom->ipr_oc0 = hdl->rom->oc0_priorty;
            (*hdl->rom->ier_oc0) |= hdl->rom->oc0_enable_mask;

            /* Enable output compare 0 interrupt */
            hdl->rom->regs->CMWCR.BIT.OC0IE = 1;
        }

        /* Change state to open */
        hdl->state = CMTW_PRV_CHANNEL_STATE_OPENED;
    }

    /* OUTPUT COMPARE 1 */
    if (CMTW_ACTION_NONE != pconfig->oc_timer_1.actions)
    {
        /* Calculate timer count */
        count = calculate_count(pconfig->time_unit,
                                pconfig->clock_divisor,
                                pconfig->oc_timer_1.time);

        /* Set timer counter if in range, otherwise return */
        if (0 == count)
        {
#if CMTW_CGF_REQUIRE_LOCK == 1
            /* Release lock for this channel */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
            R_CMTW_Close(channel);
            return(CMTW_ERR_OUT_OF_RANGE);
        }

        /* Set the output compare count */
        hdl->rom->regs->CMWOCR1 = count;

        /* Enable output compare */
        hdl->rom->regs->CMWIOR.BIT.OC1E = 1;

        /* Set output compare pin state */
        hdl->rom->regs->CMWIOR.BIT.OC1 = pconfig->oc_timer_1.output;

        if (CMTW_ACTION_ISR_ENABLE_MASK & pconfig->oc_timer_1.actions)
        {
            /* Save action for ISR processing */
            hdl->oc1_action = pconfig->oc_timer_1.actions;

            /* Clear ICU interrupt request, set level, and enable it */
            *hdl->rom->ir_oc1 = 0;
            *hdl->rom->ipr_oc1 = hdl->rom->oc1_priorty;
            (*hdl->rom->ier_oc1) |= (hdl->rom->oc1_enable_mask);

            /* Enable output compare 0 interrupt */
            hdl->rom->regs->CMWCR.BIT.OC1IE = 1;
        }

        /* Change state to open */
        hdl->state = CMTW_PRV_CHANNEL_STATE_OPENED;
    }

    /* INPUT CAPTURE 0 */
    if (CMTW_ACTION_NONE != pconfig->ic_timer_0.actions)
    {
        /* Enable input capture */
        hdl->rom->regs->CMWIOR.BIT.IC0E = 1;

        /* Set pin edge to capture */
        hdl->rom->regs->CMWIOR.BIT.IC0 = pconfig->ic_timer_0.edge;

        if (CMTW_ACTION_ISR_ENABLE_MASK & pconfig->ic_timer_0.actions)
        {
            /* Save action for ISR processing */
            hdl->ic0_action = pconfig->ic_timer_0.actions;

            /* Clear ICU interrupt request, set level, and enable it */
            *hdl->rom->ir_ic0 = 0;
            *hdl->rom->ipr_ic0 = hdl->rom->ic0_priorty;
            (*hdl->rom->ier_ic0) |= hdl->rom->ic0_enable_mask;

            /* Enable input capture 0 interrupt */
            hdl->rom->regs->CMWCR.BIT.IC0IE = 1;
        }

        /* Change state to open */
        hdl->state = CMTW_PRV_CHANNEL_STATE_OPENED;
    }

    /* INPUT CAPTURE 1 */
    if (CMTW_ACTION_NONE != pconfig->ic_timer_1.actions)
    {
        /* Enable input capture */
        hdl->rom->regs->CMWIOR.BIT.IC1E = 1;

        /* Set pin edge to capture */
        hdl->rom->regs->CMWIOR.BIT.IC1 = pconfig->ic_timer_1.edge;

        if (CMTW_ACTION_ISR_ENABLE_MASK & pconfig->ic_timer_1.actions)
        {
            /* Save action for ISR processing */
            hdl->ic1_action = pconfig->ic_timer_1.actions;

            /* Clear ICU interrupt request, set level, and enable it */
            *hdl->rom->ir_ic1 = 0;
            *hdl->rom->ipr_ic1 = hdl->rom->ic1_priorty;
            (*hdl->rom->ier_ic1) |= hdl->rom->ic1_enable_mask;

            /* Enable input capture 1 interrupt */
            hdl->rom->regs->CMWCR.BIT.IC1IE = 1;
        }

        /* Change state to open */
        hdl->state = CMTW_PRV_CHANNEL_STATE_OPENED;
    }

#if CMTW_CGF_REQUIRE_LOCK == 1
    /* Release lock for this channel */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif

    return(CMTW_SUCCESS);
} /* End of function R_CMTW_Open() */

/***********************************************************************************************************************
* Function Name: R_CMTW_Close
********************************************************************************************************************//**
* @brief Disables the specified CMTW channel and powers it down. The channel moves to the closed state.
* @param[in] channel Number of the CMTW channel to close
* @retval CMTW_SUCCESS Successful, channel is closed
* @retval CMTW_ERR_BAD_CHAN Invalid channel number
* @retval CMTW_ERR_CH_NOT_ENABLED Channel is disabled by user configuration
* @retval CMTW_ERR_CH_NOT_OPENED Channel currently closed, perform R_CMTW_Open() first
* @retval CMTW_ERR_LOCK The lock could not be acquired, the channel is busy
* @details This function stops and disables a CMTW channel that is in the open or running state.  Once successfully
* completed, the status of the selected CMTW channel will be set to "stopped" and the channel is powered down to reduce
* power consumption.  The channel cannot be used again until it has been reopened with the R_CMTW_Open() function.
* @note None
*/
cmtw_err_t    R_CMTW_Close(cmtw_channel_t    channel)
{
    cmtw_prv_ch_ctrl_info_t *hdl;

#if CMTW_CGF_REQUIRE_LOCK == 1
    bool lock_result;
#endif

#if CMTW_CFG_PARAM_CHECKING_ENABLE == 1
/* Parameter check - channel */
    if (CMTW_CHANNEL_MAX <= channel)
    {
        return (CMTW_ERR_BAD_CHAN);
    }
#endif

#if CMTW_CGF_REQUIRE_LOCK == 1
    /* Get lock for this channel */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));

    if (false == lock_result)
    {
        return(CMTW_ERR_LOCK);
    }
#endif

    /* Get the handle for the channel - pointer to the channel control info */
    hdl = g_cmtw_handles[channel];

    /* Check if channel enabled */
    if (NULL == hdl)
    {
#if CMTW_CGF_REQUIRE_LOCK == 1
        /* Release lock for this channel */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
        return (CMTW_ERR_CH_NOT_ENABLED);
    }

    /* Check channel state */
    if (CMTW_PRV_CHANNEL_STATE_CLOSED == hdl->state)
    {
#if CMTW_CGF_REQUIRE_LOCK == 1
        /* Release lock for this channel */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
        return (CMTW_ERR_CH_NOT_OPENED);
    }

    /* Clear all registers */
    hdl->rom->regs->CMWSTR.WORD = 0;
    hdl->rom->regs->CMWCR.WORD = 0;
    hdl->rom->regs->CMWIOR.WORD = 0;
    hdl->rom->regs->CMWCNT = 0;
    hdl->rom->regs->CMWCOR = 0;
    hdl->rom->regs->CMWICR0 = 0;
    hdl->rom->regs->CMWICR1 = 0;
    hdl->rom->regs->CMWOCR0 = 0;
    hdl->rom->regs->CMWOCR1 = 0;

    /* Clear ICU interrupts */
    *hdl->rom->ir_cm = 0;
    *hdl->rom->ipr_cm = 0;
    (*hdl->rom->ier_cm) &= (~hdl->rom->cm_enable_mask);

    *hdl->rom->ir_oc0 = 0;
    *hdl->rom->ipr_oc0 = 0;
    (*hdl->rom->ier_oc0) &= (~hdl->rom->oc0_enable_mask);

    *hdl->rom->ir_oc1 = 0;
    *hdl->rom->ipr_oc1 = 0;
    (*hdl->rom->ier_oc1) &= (~hdl->rom->oc1_enable_mask);

    *hdl->rom->ir_ic0 = 0;
    *hdl->rom->ipr_ic0 = 0;
    (*hdl->rom->ier_ic0) &= (~hdl->rom->ic0_enable_mask);

    *hdl->rom->ir_ic1 = 0;
    *hdl->rom->ipr_ic1 = 0;
    (*hdl->rom->ier_ic1) &= (~hdl->rom->ic1_enable_mask);

    /* Change state to closed */
    hdl->state = CMTW_PRV_CHANNEL_STATE_CLOSED;

    /* Turn power off to the channel */
    power_off(hdl);

#if CMTW_CGF_REQUIRE_LOCK == 1
    /* Release lock for this channel */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif


    return(CMTW_SUCCESS);
}  /* End of function R_CMTW_Close() */

/***********************************************************************************************************************
* Function Name: R_CMTW_Control
********************************************************************************************************************//**
* @brief This function starts, stops, resumes, or restarts a CMTW channel that is in the open state.
* @param[in] channel Number of the CMTW channel to control
* @param[in] cmd Enumerated command codes:\n
* CMTW_CMD_START: Activate the timer\n
* CMTW_CMD_RESUME: Same as start\n
* CMTW_CMD_STOP: Pause the timer\n
* CMTW_CMD_RESTART: Zero the counter then activate the timer
* @retval CMTW_SUCCESS Successful, the command is executed
* @retval CMTW_ERR_BAD_CHAN Invalid channel number
* @retval CMTW_ERR_CH_NOT_ENABLED Channel is disabled by user configuration
* @retval CMTW_ERR_CH_NOT_OPENED Channel currently closed, perform R_CMTW_Open() first
* @retval CMTW_ERR_CH_NOT_RUNNIG Channel currently not started - perform R_CMTW_Control() to start
* @retval CMTW_ERR_CH_NOT_STOPPED Channel currently running - perform R_CMTW_Control() to stop
* @retval CMTW_ERR_UNKNOWN_CMD Invalid command
* @retval CMTW_ERR_LOCK The lock could not be acquired, the channel is busy
* @details This function starts or resumes a CMTW channel that is in the open state.  Once successfully completed, the
* status of the selected CMTW channel will be set to "running".  Stop command pauses a channel that is in a running state.
* In this state the timer registers retain their values immediately before executing the stop command.  Timer operations
* can be continued by start or resume commands.  The restart command clears the timer counter and resumes the operations.
* @note None
*/
cmtw_err_t    R_CMTW_Control(cmtw_channel_t     channel,
                             cmtw_cmd_t         cmd)
{
    cmtw_prv_ch_ctrl_info_t *hdl;

#if CMTW_CGF_REQUIRE_LOCK == 1
    bool lock_result;
#endif

#if CMTW_CFG_PARAM_CHECKING_ENABLE == 1
    /* Parameter check - channel */
    if (CMTW_CHANNEL_MAX <= channel)
    {
        return(CMTW_ERR_BAD_CHAN);
    }

    /* Parameter check - cmd */
    if (CMTW_CMD_MAX <= cmd)
    {
        return(CMTW_ERR_UNKNOWN_CMD);
    }
#endif

#if CMTW_CGF_REQUIRE_LOCK == 1
    /* Get lock for this channel */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));

    if (false == lock_result)
    {
        return(CMTW_ERR_LOCK);
    }
#endif

    /* Get the handle for the channel - pointer to the channel control info */
    hdl = g_cmtw_handles[channel];

    /* Check if channel enabled */
    if (NULL == hdl)
    {
#if CMTW_CGF_REQUIRE_LOCK == 1
        /* Release lock for this channel */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
        return (CMTW_ERR_CH_NOT_ENABLED);
    }

    /* Check channel state */
    if (CMTW_PRV_CHANNEL_STATE_CLOSED == hdl->state)
    {
#if CMTW_CGF_REQUIRE_LOCK == 1
        /* Release lock for this channel */
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
        return(CMTW_ERR_CH_NOT_OPENED);
    }
    else if (CMTW_PRV_CHANNEL_STATE_OPENED == hdl->state)
    {
        if (CMTW_CMD_STOP == cmd)
        {
#if CMTW_CGF_REQUIRE_LOCK == 1
            /* Release lock for this channel */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
            return (CMTW_ERR_CH_NOT_RUNNIG);
        }
    }
    else if (CMTW_PRV_CHANNEL_STATE_RUNNING == hdl->state)
    {
        if ((CMTW_CMD_START == cmd) || (CMTW_CMD_RESUME == cmd))
        {
#if CMTW_CGF_REQUIRE_LOCK == 1
            /* Release lock for this channel */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
            return (CMTW_ERR_CH_NOT_STOPPED);
        }
    }
    else
    {
        /* Do nothing */
        R_BSP_NOP();
    }

    /* Process the command */
    switch (cmd)
    {
        /* Both start and resume do the same thing - start the timers of the this channel */
        case CMTW_CMD_START:
        case CMTW_CMD_RESUME:
            {
            hdl->rom->regs->CMWSTR.BIT.STR = 1;
            hdl->state = CMTW_PRV_CHANNEL_STATE_RUNNING;
            }
        break;

        /* Stop the timers of the this channel */
        case CMTW_CMD_STOP:
            {
            hdl->rom->regs->CMWSTR.BIT.STR = 0;
            hdl->state = CMTW_PRV_CHANNEL_STATE_OPENED;
            }
        break;

        /* Clears the counter and starts the timers of this channel */
        case CMTW_CMD_RESTART:
            {
            hdl->rom->regs->CMWSTR.BIT.STR = 0;
            hdl->rom->regs->CMWCNT = 0;
            hdl->rom->regs->CMWSTR.BIT.STR = 1;
            hdl->state = CMTW_PRV_CHANNEL_STATE_RUNNING;
            }
        break;

        default:
            {
#if CMTW_CGF_REQUIRE_LOCK == 1
            /* Release lock for this channel */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif
            return(CMTW_ERR_UNKNOWN_CMD);
            }
        break;
    }

#if CMTW_CGF_REQUIRE_LOCK == 1
    /* Release lock for this channel */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_CMTW0 + (uint8_t)channel));
#endif

    return(CMTW_SUCCESS);
} /* End of function R_CMTW_Control() */

/***********************************************************************************************************************
* Function Name: R_CMTW_GetVersion
********************************************************************************************************************//**
* @brief This function returns the driver version number at runtime.
* @return Version_number with major and minor version digits packed into a single 32-bit value.
* @details This function returns the version of this module. The version number is encoded such that the top 2 bytes are
* the major version number and the bottom 2 bytes are the minor version number.
* @note None
*/
uint32_t R_CMTW_GetVersion (void)
{
    /* These version macros are defined in r_cmtw_rx_if.h. */
    return((((uint32_t)CMTW_RX_VERSION_MAJOR) << 16) | (uint32_t)CMTW_RX_VERSION_MINOR);
} /* End of function R_CMTW_GetVersion() */

/***********************************************************************************************************************
* Function Name: power_on
* Description  : Provides power to the channel specified by the handle.
* Arguments    : hdl -
*                   handle for the channel
* Return Value : none
***********************************************************************************************************************/
static void power_on(cmtw_prv_ch_ctrl_info_t  * const hdl)
{
    
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    /* Power on CMTW */
    SYSTEM.MSTPCRA.LONG &= (~hdl->rom->stop_mask);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
} /* End of function power_on() */

/***********************************************************************************************************************
* Function Name: power_off
* Description  : Removes power from the channel specified by the handle.
* Arguments    : hdl -
*                   handle for the channel
* Return Value : none
***********************************************************************************************************************/
static void power_off(cmtw_prv_ch_ctrl_info_t  * const hdl)
{
    
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    /* Power off CMTW */
    SYSTEM.MSTPCRA.LONG |= hdl->rom->stop_mask;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
} /* End of function power_off() */

/***********************************************************************************************************************
* Function Name: calculate_count
* Description  : Calculates the timer count.
* Arguments    : time_unit -
*                   time base
*                clock_divisor -
*                   time clock divisor
*                time -
*                   request time value
* Return Value : count
*                   timer tick count if in range, 0 otherwise
***********************************************************************************************************************/
static uint32_t calculate_count(cmtw_time_unit_t time_unit, cmtw_clock_divisor_t clock_divisor, uint32_t time)
{
    uint32_t divisor;
    uint32_t count;
    float countf;
    float unit;

    unit = g_cmtw_time_units[time_unit];
    divisor = g_cmtw_clock_divisors[clock_divisor];
    countf = ((BSP_PCLKB_HZ / divisor) * time * unit) - 1;

    /* Check if count is in range */
    if ((countf < CMTW_COUNTF_MIN) || (countf > CMTW_COUNTF_MAX))
    {
        count = 0;
    }
    else
    {
        /* Round up the count if necessary */
        if ((uint32_t)(countf*2) == ((uint32_t)(countf)*2))
        {
            /* Casting to uint32_t is valid */
            count = (uint32_t)countf;
        }
        else
        {
            /* Casting to uint32_t is valid */
            count = (uint32_t)(countf) + 1;
        }
    }

    return(count);
} /* End of function calculate_count() */

#if CMTW_CFG_USE_CH0 == 1
/***********************************************************************************************************************
* Function Name: cmtw_cm_ch0_isr
* Description  : Compare match interrupt handler for channel 0.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_cm_ch0_isr,VECT(CMTW0, CMWI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_cm_ch0_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_0];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->cm_action)
    {
        count = hdl->rom->regs->CMWCOR;
        cmtw_isr_common(CMTW_EVENT_CM, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->cm_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWCR.BIT.CMWIE = 0;
        hdl->rom->regs->CMWIOR.BIT.CMWE = 0;
    }
} /* End of function cmtw_cm_ch0_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_oc0_ch0_isr
* Description  : Output compare 0 interrupt handler for channel 0.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_oc0_ch0_isr, VECT_CMTW0_OC0I0)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_oc0_ch0_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_0];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->oc0_action)
    {
        count = hdl->rom->regs->CMWOCR0;
        cmtw_isr_common(CMTW_EVENT_OC0, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->oc0_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWIOR.BIT.OC0 = 0;
        hdl->rom->regs->CMWCR.BIT.OC0IE = 0;
        hdl->rom->regs->CMWIOR.BIT.OC0E = 0;
    }
} /* End of function cmtw_oc0_ch0_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_oc1_ch0_isr
* Description  : Output compare 1 interrupt handler for channel 0.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_oc1_ch0_isr,VECT_CMTW0_OC1I0)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_oc1_ch0_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_0];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->oc1_action)
    {
        count = hdl->rom->regs->CMWOCR1;
        cmtw_isr_common(CMTW_EVENT_OC1, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->oc1_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWIOR.BIT.OC1 = 0;
        hdl->rom->regs->CMWCR.BIT.OC1IE = 0;
        hdl->rom->regs->CMWIOR.BIT.OC1E = 0;
    }
} /* End of function cmtw_oc1_ch0_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_ic0_ch0_isr
* Description  : Input capture 0 interrupt handler for channel 0.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_ic0_ch0_isr, VECT_CMTW0_IC0I0)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_ic0_ch0_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_0];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->ic0_action)
    {
        count = hdl->rom->regs->CMWICR0;
        cmtw_isr_common(CMTW_EVENT_IC0, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->ic0_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWCR.BIT.IC0IE = 0;
        hdl->rom->regs->CMWIOR.BIT.IC0E = 0;
    }
} /* End of function cmtw_ic0_ch0_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_ic1_ch0_isr
* Description  : Input capture 1 interrupt handler for channel 0.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_ic1_ch0_isr, VECT_CMTW0_IC1I0)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_ic1_ch0_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_0];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->ic1_action)
    {
        count = hdl->rom->regs->CMWICR1;
        cmtw_isr_common(CMTW_EVENT_IC1, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->ic1_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWCR.BIT.IC1IE = 0;
        hdl->rom->regs->CMWIOR.BIT.IC1E = 0;
    }
} /* End of function cmtw_ic1_ch0_isr() */
#endif /* End of #if CMTW_CFG_USE_CH0 == 1 */

#if CMTW_CFG_USE_CH1 == 1
/***********************************************************************************************************************
* Function Name: cmtw_cm_ch1_isr
* Description  : Compare match interrupt handler for channel 1.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_cm_ch1_isr,VECT(CMTW1, CMWI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_cm_ch1_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_1];

     /* Call user callback if requested */
     if (CMTW_ACTION_CALLBACK & hdl->cm_action)
     {
         count = hdl->rom->regs->CMWCOR;
         cmtw_isr_common(CMTW_EVENT_CM, count, hdl);
     }

     /* Disable if repeat not requested e.g. one-shot */
     if (CMTW_ACTION_ONESHOT & hdl->cm_action)
     {
         /* Disable interrupt and timer */
         hdl->rom->regs->CMWCR.BIT.CMWIE = 0;
         hdl->rom->regs->CMWIOR.BIT.CMWE = 0;
     }
} /* End of function cmtw_cm_ch1_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_oc0_ch1_isr
* Description  : Output compare 0 interrupt handler for channel 1.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_oc0_ch1_isr, VECT_CMTW1_OC0I1)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_oc0_ch1_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_1];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->oc0_action)
    {
        count = hdl->rom->regs->CMWOCR0;
        cmtw_isr_common(CMTW_EVENT_OC0, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->oc0_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWIOR.BIT.OC0 = 0;
        hdl->rom->regs->CMWCR.BIT.OC0IE = 0;
        hdl->rom->regs->CMWIOR.BIT.OC0E = 0;
    }
} /* End of function cmtw_oc0_ch1_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_oc1_ch1_isr
* Description  : Output compare 1 interrupt handler for channel 1.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_oc1_ch1_isr, VECT_CMTW1_OC1I1)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_oc1_ch1_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_1];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->oc1_action)
    {
        count = hdl->rom->regs->CMWOCR1;
        cmtw_isr_common(CMTW_EVENT_OC1, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->oc1_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWIOR.BIT.OC1 = 0;
        hdl->rom->regs->CMWCR.BIT.OC1IE = 0;
        hdl->rom->regs->CMWIOR.BIT.OC1E = 0;
    }
} /* End of function cmtw_oc1_ch1_isr() */


/***********************************************************************************************************************
* Function Name: cmtw_ic0_ch1_isr
* Description  : Input capture 0 interrupt handler for channel 1.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_ic0_ch1_isr, VECT_CMTW1_IC0I1)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_ic0_ch1_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_1];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->ic0_action)
    {
        count = hdl->rom->regs->CMWICR0;
        cmtw_isr_common(CMTW_EVENT_IC0, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->ic0_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWCR.BIT.IC0IE = 0;
        hdl->rom->regs->CMWIOR.BIT.IC0E = 0;
    }
} /* End of function cmtw_ic0_ch1_isr() */

/***********************************************************************************************************************
* Function Name: cmtw_ic1_ch1_isr
* Description  : Input capture 1 interrupt handler for channel 1.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(cmtw_ic1_ch1_isr, VECT_CMTW1_IC1I1)
R_BSP_ATTRIB_STATIC_INTERRUPT void cmtw_ic1_ch1_isr(void)
{
    uint32_t                count;
    cmtw_prv_ch_ctrl_info_t *hdl;

    /* Get the handle for the channel */
     hdl = g_cmtw_handles[CMTW_CHANNEL_1];

    /* Call user callback if requested */
    if (CMTW_ACTION_CALLBACK & hdl->ic1_action)
    {
        count = hdl->rom->regs->CMWICR1;
        cmtw_isr_common(CMTW_EVENT_IC1, count, hdl);
    }

    /* Disable if repeat not requested e.g. one-shot */
    if (CMTW_ACTION_ONESHOT & hdl->ic1_action)
    {
        /* Disable interrupt and timer */
        hdl->rom->regs->CMWCR.BIT.IC1IE = 0;
        hdl->rom->regs->CMWIOR.BIT.IC1E = 0;
    }
} /* End of function cmtw_ic1_ch1_isr() */
#endif /* End of #if CMTW_CFG_USE_CH1 == 1 */

/***********************************************************************************************************************
* Function Name: cmtw_isr_common
* Description  : Common ISR code for handling all CMTW interrupts.
* Arguments    : event -
*                   interrupt source
*                count -
*                   timer count for the event
*                hdl -
*                   pointer to the channel control block
* Return Value : none
***********************************************************************************************************************/
static void cmtw_isr_common(cmtw_event_t event, uint32_t count, cmtw_prv_ch_ctrl_info_t * const hdl)
{
    cmtw_callback_data_t    cb_data;

    /* Call the callback if available */
    if ((NULL != hdl->pcallback) && (FIT_NO_FUNC != hdl->pcallback))
    {
        cb_data.channel = hdl->rom->channel;
        cb_data.event = event;
        cb_data.count = count;

        /* Execute registered callback function */
        hdl->pcallback((void *)&cb_data);
    }
} /* End of function cmtw_isr_common() */
