/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_psm_config.c
 * Description  : Function to configure PSM.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "at_command.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define PHASE_1    (0x01u << 0)
#define PHASE_2    (0x01u << 1)
#define PHASE_3    (0x01u << 2)
#define PHASE_4    (0x01u << 3)
#define PHASE_5    (0x01u << 4)
#define PHASE_6    (0x01u << 5)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static void cellular_psm_config_fail (st_cellular_ctrl_t * const p_ctrl, const uint8_t open_phase);

/************************************************************************
 * Function Name  @fn            cellular_psm_config
 ***********************************************************************/
e_cellular_err_t cellular_psm_config(st_cellular_ctrl_t * const p_ctrl, const e_cellular_psm_mode_t mode)
{
    uint8_t          open_phase = 0;
    e_cellular_err_t ret        = CELLULAR_SUCCESS;

    if (CELLULAR_PSM_MODE_ACTIVE == mode)
    {
        ret = atc_sqnricfg(p_ctrl, CELLULAR_SQNRICFG_MODE);
        if (CELLULAR_SUCCESS == ret)
        {
            open_phase |= PHASE_1;
            ret         = atc_sqnipscfg(p_ctrl, CELLULAR_SQNIPSCFG_MODE);
        }

        if (CELLULAR_SUCCESS == ret)
        {
            open_phase |= PHASE_2;
            ret         = atc_sqnpscfg(p_ctrl);
        }

        if (CELLULAR_SUCCESS == ret)
        {
            p_ctrl->ring_ctrl.ring_event = cellular_create_event_group("ring_event");
        }

        if (NULL != p_ctrl->ring_ctrl.ring_event)
        {
            open_phase                     |= PHASE_3;
            p_ctrl->ring_ctrl.rts_semaphore = cellular_create_semaphore("rts_semaphore");
        }

        if (NULL != p_ctrl->ring_ctrl.rts_semaphore)
        {
            open_phase |= PHASE_4;
            ret         = cellular_start_ring_task(p_ctrl);
        }

        if (CELLULAR_SUCCESS == ret)
        {
            open_phase |= PHASE_5;
            ret         = cellular_irq_open(p_ctrl);
        }

        if (CELLULAR_SUCCESS == ret)
        {
            open_phase           |= PHASE_6;
            p_ctrl->ring_ctrl.psm = CELLULAR_PSM_ACTIVE;
#if CELLULAR_CFG_CTS_SW_CTRL == 1
            cellular_rts_hw_flow_disable();
#endif
            cellular_rts_ctrl(1);
        }
        else
        {
            cellular_psm_config_fail(p_ctrl, open_phase);
        }
    }
    else
    {
        ret = atc_sqnricfg(p_ctrl, (uint8_t)CELLULAR_PSM_MODE_INVALID);         //cast

        if (CELLULAR_SUCCESS == ret)
        {
            ret = atc_sqnipscfg(p_ctrl, (uint8_t)CELLULAR_PSM_MODE_INVALID);    //cast
        }

        cellular_irq_close(p_ctrl);

        if (NULL != p_ctrl->ring_ctrl.ring_taskhandle)
        {
            cellular_delete_task(p_ctrl->ring_ctrl.ring_taskhandle);
            p_ctrl->ring_ctrl.ring_taskhandle = NULL;
        }
        if (NULL != p_ctrl->ring_ctrl.rts_semaphore)
        {
            cellular_delete_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
            p_ctrl->ring_ctrl.rts_semaphore = NULL;
        }
        if (NULL != p_ctrl->ring_ctrl.ring_event)
        {
            cellular_delete_event_group(p_ctrl->ring_ctrl.ring_event);
            p_ctrl->ring_ctrl.ring_event = NULL;
        }
        p_ctrl->ring_ctrl.psm = CELLULAR_PSM_DEACTIVE;

#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_enable();
#else
        cellular_rts_ctrl(0);
#endif
#ifdef CELLULAR_RTS_DELAY
        cellular_delay_task(CELLULAR_RTS_DELAYTIME);
#endif
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_psm_config
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_psm_config_fail
 ***********************************************************************/
static void cellular_psm_config_fail(st_cellular_ctrl_t * const p_ctrl, const uint8_t open_phase)
{
    if ((open_phase & PHASE_6) == PHASE_6)
    {
        cellular_irq_close(p_ctrl);
    }

    if ((open_phase & PHASE_5) == PHASE_5)
    {
        cellular_delete_task(p_ctrl->ring_ctrl.ring_taskhandle);
        p_ctrl->ring_ctrl.ring_taskhandle = NULL;
    }

    if ((open_phase & PHASE_4) == PHASE_4)
    {
        cellular_delete_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
        p_ctrl->ring_ctrl.rts_semaphore = NULL;
    }

    if ((open_phase & PHASE_3) == PHASE_3)
    {
        cellular_delete_event_group(p_ctrl->ring_ctrl.ring_event);
        p_ctrl->ring_ctrl.ring_event = NULL;
    }

    if ((open_phase & PHASE_2) == PHASE_2)
    {
        atc_sqnipscfg(p_ctrl, (uint8_t)CELLULAR_PSM_MODE_INVALID);  //cast
    }

    if ((open_phase & PHASE_1) == PHASE_1)
    {
        atc_sqnricfg(p_ctrl, (uint8_t)CELLULAR_PSM_MODE_INVALID);   //cast
    }

    return;
}
/**********************************************************************************************************************
 * End of function cellular_psm_config_fail
 *********************************************************************************************************************/
