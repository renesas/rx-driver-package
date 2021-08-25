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
* File Name    : r_vbatt_rx671.c
* Description  : Functions for using Battery Backup Function on RX devices.
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

#ifdef BSP_MCU_RX671

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines for Battery Backup Function support */
#include "r_vbatt_rx_private.h"
#include "r_vbatt_rx671_private.h"
#include "r_vbatt_rx_if.h"
#include "r_vbatt_rx_config.h"

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*  internal functions                                                        */
/*----------------------------------------------------------------------------*/
static void r_vbatt_set_io_register(vbatt_ctrl_info_t * p_vbatt_set);


/************************************************************************************
* Function Name: r_vbatt_check_parameter
* Description  : Checks parameter.
* Arguments    : void * p_arg                       ;
*                vbatt_api_mode_t called_api        ; API mode
* Return Value : VBATT_SUCCESS                      ; Successful operation
*              : VBATT_ERR_LVD0_SETTING             ; LVD0 of OFS1 setting is invalid
*              ; VBATT_ERR_INVALID_ARG              ; Parameter error
*              : VBATT_ERR_FUNC_INVALID             ; Is called in VBATT detection function disable
************************************************************************************/
vbatt_return_t r_vbatt_check_parameter(void * p_arg, vbatt_api_mode_t called_api)
{
    /* ==== CHECKS POINTER OF ARGMENT ==== */
    if (NULL == p_arg)
    {
        return VBATT_ERR_INVALID_ARG;
    }

    /* ==== CHECKS ARGMENT ==== */
    switch (called_api)
    {
        case VBATT_TAMPER_MODE_OPEN:

            /* ---- checks LVD0 of OFS1 setting ---- */
            if ((VBATT_OFS1_LVD0_ENABLE != (BSP_CFG_OFS1_REG_VALUE & VBATT_OFS1_LVD0_BIT_MASK))||\
                (VBATT_OFS1_VDSEL_DISABLE == (BSP_CFG_OFS1_REG_VALUE & VBATT_OFS1_VDSEL_BIT_MASK)))
            {
                return VBATT_ERR_LVD0_SETTING;
            }

            /* ---- checks pointer of callback function ---- */
            if (NULL == ((vbatt_info_t *)p_arg)->callbackfunc)
            {
                return VBATT_ERR_INVALID_ARG;
            }

            break;

        case VBATT_TAMPER_MODE_CONTROL:

            /* ---- checks information struct ---- */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->tamper_channel) &&
            	(1 != ((vbatt_ctrl_info_t*)p_arg)->tamper_channel) &&
                (2 != ((vbatt_ctrl_info_t*)p_arg)->tamper_channel))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid tamper detection interrupt */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->tamper_detection_interrupt) &&
            	(1 != ((vbatt_ctrl_info_t*)p_arg)->tamper_detection_interrupt))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid tamper erase */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->tamper_erase) &&
                (1 != ((vbatt_ctrl_info_t*)p_arg)->tamper_erase))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid tamper capture source */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->time_capture_source) &&
                (1 != ((vbatt_ctrl_info_t*)p_arg)->time_capture_source))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid tamper channel input */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->channel_input) &&
                (1 != ((vbatt_ctrl_info_t*)p_arg)->channel_input))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid channel nosie filter */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->channel_noise_filter) &&
                (1 != ((vbatt_ctrl_info_t*)p_arg)->channel_noise_filter))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid tamper trigger select */
            if ((0 != ((vbatt_ctrl_info_t*)p_arg)->channel_trigger_select) &&
                (1 != ((vbatt_ctrl_info_t*)p_arg)->channel_trigger_select))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            /* Check valid tamper int priority */
            if ((1 > ((vbatt_ctrl_info_t*)p_arg)->tamper_int_priority) ||
                (15 < ((vbatt_ctrl_info_t*)p_arg)->tamper_int_priority))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            break;

        case VBATT_TAMPER_MODE_GETSTATUS:

            if ((0 != ((vbatt_status_t*)p_arg)->tamper_channel) &&
            	(1 != ((vbatt_status_t*)p_arg)->tamper_channel) &&
                (2 != ((vbatt_status_t*)p_arg)->tamper_channel))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            if ((true != ((vbatt_status_t*)p_arg)->action_clear) &&
                (false != ((vbatt_status_t*)p_arg)->action_clear))
            {
                return VBATT_ERR_INVALID_ARG;
            }

            break;

        default:

            R_BSP_NOP();/* Do nothing */
    }

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_check_parameter
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_open_set_target_mcu
* Description  : According to the setting of the configuration option,
*                This function sets the validity / invalidity of the Tamper detector function and
*                a detection level, an interrupt.
* Arguments    : none
* Return Value : VBATT_SUCCESS        Successful operation
************************************************************************************/
vbatt_return_t r_vbatt_open_set_target_mcu(void)
{
    vbatt_ctrl_info_t   vbatt_set;

    /* Set configuration Tamper 0 */
    if(VBATT_TAMPER_ENABLE == VBATT_CFG_TAMPER_CH0)
    {
    	/* Set Tamper Channel 0 */
        vbatt_set.tamper_channel = VBATT_TAMPER_CH0;

        /* Set Tamper Detection Interrupt */
        vbatt_set.tamper_detection_interrupt = VBATT_CFG_TAMPER_CH0_DETECT_INT;

        /* Set Tamper Erase */
        vbatt_set.tamper_erase                  = VBATT_CFG_TAMPER_CH0_ERASE;

        /* Set Time Capture Source */
        vbatt_set.time_capture_source           = VBATT_CFG_TAMPER_TCE_CH0_SELECT;

        /* Set Channel Input */
        vbatt_set.channel_input                 = VBATT_TAMPER_CHEN_INPUT_ENABLE;

        /* Set Channel Noise Filter */
        vbatt_set.channel_noise_filter          = VBATT_CFG_TAMPER_CHEN_CH0_NOISE_FILTER;

        /* Set Channel Trigger Select */
        vbatt_set.channel_trigger_select        = VBATT_CFG_TAMPER_CHEN_CH0_EDGE;

        /* Set Tamper Int Priority */
        vbatt_set.tamper_int_priority           = VBATT_CFG_INT_PRIORITY;
        r_vbatt_set_io_register(&vbatt_set);
    }

    /* Set configuration Tamper 1 */
    if(VBATT_TAMPER_ENABLE == VBATT_CFG_TAMPER_CH1)
    {
        /* Set Tamper Channel 1 */
        vbatt_set.tamper_channel = VBATT_TAMPER_CH1;

        /* Set Tamper Detection Interrupt */
        vbatt_set.tamper_detection_interrupt = VBATT_CFG_TAMPER_CH1_DETECT_INT;

        /* Set Tamper Erase */
        vbatt_set.tamper_erase                  = VBATT_CFG_TAMPER_CH1_ERASE;

        /* Set Time Capture Source */
        vbatt_set.time_capture_source           = VBATT_CFG_TAMPER_TCE_CH1_SELECT;

        /* Set Channel Input */
        vbatt_set.channel_input                 = VBATT_TAMPER_CHEN_INPUT_ENABLE;

        /* Set Channel Noise Filter */
        vbatt_set.channel_noise_filter          = VBATT_CFG_TAMPER_CHEN_CH1_NOISE_FILTER;

        /* Set Channel Trigger Select */
        vbatt_set.channel_trigger_select        = VBATT_CFG_TAMPER_CHEN_CH1_EDGE;

        /* Set Tamper Int Priority */
        vbatt_set.tamper_int_priority           = VBATT_CFG_INT_PRIORITY;
        r_vbatt_set_io_register(&vbatt_set);
    }

    /* Set configuration Tamper 2 */
    if(VBATT_TAMPER_ENABLE == VBATT_CFG_TAMPER_CH2)
    {
        /* Set Tamper Channel 2 */
        vbatt_set.tamper_channel = VBATT_TAMPER_CH2;

        /* Set Tamper Detection Interrupt */
        vbatt_set.tamper_detection_interrupt = VBATT_CFG_TAMPER_CH2_DETECT_INT;

        /* Set Tamper Erase */
        vbatt_set.tamper_erase                  = VBATT_CFG_TAMPER_CH2_ERASE;

        /* Set Time Capture Source */
        vbatt_set.time_capture_source           = VBATT_CFG_TAMPER_TCE_CH2_SELECT;

        /* Set Channel Input */
        vbatt_set.channel_input                 = VBATT_TAMPER_CHEN_INPUT_ENABLE;

        /* Set Channel Noise Filter */
        vbatt_set.channel_noise_filter          = VBATT_CFG_TAMPER_CHEN_CH2_NOISE_FILTER;

        /* Set Channel Trigger Select */
        vbatt_set.channel_trigger_select        = VBATT_CFG_TAMPER_CHEN_CH2_EDGE;

        /* Set Tamper Int Priority */
        vbatt_set.tamper_int_priority           = VBATT_CFG_INT_PRIORITY;
        r_vbatt_set_io_register(&vbatt_set);
    }

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_open_set_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_open_check_target_mcu
* Description  : Check Power-Down Detected flag
* Arguments    : none
* Return Value : SYSTEM.BKPSR.BIT.PDDF
************************************************************************************/
uint8_t r_vbatt_open_check_target_mcu(void)
{
    /* ==== RETURN Power-Down Detected Flag ==== */
    return SYSTEM.BKPSR.BIT.PDDF;
}
/******************************************************************************
End of function r_vbatt_open_check_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_open_f_clear_target_mcu
* Description  : This function clear Power-Down Detect flag.
* Arguments    : none
* Return Value : none
************************************************************************************/
void r_vbatt_open_f_clear_target_mcu(void)
{
    /* ==== Clear Power-Down Detected Flag ==== */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR); 
    SYSTEM.BKPSR.BIT.PDDF = 0;                 /* Power-Down Detected Flag */

    /* dummy read */
    if (SYSTEM.BKPSR.BIT.PDDF)
    {
        /* Do nothing */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);   /* lock */
}
/******************************************************************************
End of function r_vbatt_open_f_clear_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_control_target_mcu
* Description  : This function sets it according to an argument.
*                   - sets the validity / invalidity of the Tamper detection function, a detection level, an interrupt.
* Arguments    : vbatt_ctrl_info_t * p_vbatt_ctrl_info  ; Battery Backup Function Information
* Return Value : VBATT_SUCCESS                          ; Successful operation
************************************************************************************/
vbatt_return_t r_vbatt_control_target_mcu(vbatt_ctrl_info_t * p_vbatt_ctrl_info)
{
    r_vbatt_set_io_register(p_vbatt_ctrl_info);

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_control_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_set_io_register
* Description  : This function sets it according to an argument.
*                   - sets the validity / invalidity of the Tamper detector function, 
*                   a detection level, an interrupt.
* Arguments    : vbatt_ctrl_info_t * p_vbatt_set    ; Pointer to the data 
* structure store Tamper detector information
* Return Value : none
************************************************************************************/
static void r_vbatt_set_io_register(vbatt_ctrl_info_t * p_vbatt_set)
{
    uint8_t uc_Temp = 0;
    uint8_t uc_Backup_TAMPCR = 0;
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR); 
    
    /* Store original data for TAMPCR */
    uc_Backup_TAMPCR = SYSTEM.TAMPCR.BYTE;
    
    /* Clear TAMPCR */
    SYSTEM.TAMPCR.BYTE = 0x00;
    
    /* Set Input signal for Time capture */
    if (VBATT_TAMPER_TCE_TAMPER_EVENT == p_vbatt_set->time_capture_source)
    {
        /* Set Input signal for Time capture */
        uc_Temp = SYSTEM.TCECR.BYTE;
        SYSTEM.TCECR.BYTE = uc_Temp | (VBATT_MASK_TCES<<p_vbatt_set->tamper_channel);
        
        if (SYSTEM.TCECR.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }
    else
    {
        /* Set Input signal for Time capture */
        uc_Temp = SYSTEM.TCECR.BYTE;
        SYSTEM.TCECR.BYTE = uc_Temp & (~(VBATT_MASK_TCES<<p_vbatt_set->tamper_channel));
        
        if (SYSTEM.TCECR.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }

    /* Set The RTCICn signal input */
    if (VBATT_TAMPER_CHEN_INPUT_ENABLE == p_vbatt_set->channel_input)
    {
        /* Set RTCICn signal input */
        uc_Temp = SYSTEM.TAMPICR1.BYTE;
        SYSTEM.TAMPICR1.BYTE = uc_Temp | (VBATT_MASK_CHEN<<p_vbatt_set->tamper_channel);
        
        if (SYSTEM.TAMPICR1.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }
    else
    {
        /* Set RTCICn signal input */
        uc_Temp = SYSTEM.TAMPICR1.BYTE;
        SYSTEM.TAMPICR1.BYTE = uc_Temp & (~(VBATT_MASK_CHEN<<p_vbatt_set->tamper_channel));
        
        if (SYSTEM.TAMPICR1.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }

    /* Set Noise filter for pin RTCICn */
    if (VBATT_TAMPER_CHEN_NOISE_FILTER_ENABLE == p_vbatt_set->channel_noise_filter)
    {
        /* Set Noise filter for pin RTCICn */
        uc_Temp = SYSTEM.TAMPICR2.BYTE;
        SYSTEM.TAMPICR2.BYTE = uc_Temp | (VBATT_MASK_CHNFE<<p_vbatt_set->tamper_channel);
        
        /* Waiting for detection delay by noise filter */
        R_BSP_SoftwareDelay(153, BSP_DELAY_MICROSECS);        
        if (SYSTEM.TAMPICR2.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }
    else
    {
        /* Set Noise filter for pin RTCICn */
        uc_Temp = SYSTEM.TAMPICR2.BYTE;
        SYSTEM.TAMPICR2.BYTE = uc_Temp & (~(VBATT_MASK_CHNFE<<p_vbatt_set->tamper_channel));
        
        if (SYSTEM.TAMPICR2.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }

    /* Set trigger select */
    if (VBATT_TAMPER_CHEN_RISING_EDGE == p_vbatt_set->channel_trigger_select)
    {
        /* Set trigger select */
        uc_Temp = SYSTEM.TAMPICR2.BYTE;
        SYSTEM.TAMPICR2.BYTE = uc_Temp | (VBATT_MASK_CHTRG<<p_vbatt_set->tamper_channel);
        
        if (SYSTEM.TAMPICR2.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }
    else
    {
        /* Set trigger select */
        uc_Temp = SYSTEM.TAMPICR2.BYTE;
        SYSTEM.TAMPICR2.BYTE = uc_Temp & (~(VBATT_MASK_CHTRG<<p_vbatt_set->tamper_channel));
        
        if (SYSTEM.TAMPICR2.BYTE)                 /* dummy read */
        {
            /* Do nothing */
        }
       
    }    
        
    /* Check and Clear register TAMPSR */
    if(1 == SYSTEM.TAMPSR.BIT.TAMP0F)
    {
        SYSTEM.TAMPSR.BIT.TAMP0F = 0;
    }
    if(1 == SYSTEM.TAMPSR.BIT.TAMP1F)
    {
        SYSTEM.TAMPSR.BIT.TAMP1F = 0;
    }
    if(1 == SYSTEM.TAMPSR.BIT.TAMP2F)
    {
        SYSTEM.TAMPSR.BIT.TAMP2F = 0;
    }
    
    /* Set Tamper n detection interrupt */
    if (VBATT_TAMPER_DETECT_INT_ENABLE == p_vbatt_set->tamper_detection_interrupt)
    {
        /* Set tamper detection interrupt */
        uc_Backup_TAMPCR = uc_Backup_TAMPCR | (VBATT_MASK_TAMPIE << p_vbatt_set->tamper_channel);

        /* Set tamper interrupt priority */
        IPR(VBATT,TAMPDI) = p_vbatt_set->tamper_int_priority;
        R_BSP_InterruptRequestEnable(VECT(VBATT,TAMPDI));

    }
    else
    {
        /* Set tamper detection interrupt */
        uc_Backup_TAMPCR = uc_Backup_TAMPCR & (~(VBATT_MASK_TAMPIE<<p_vbatt_set->tamper_channel));
    }

    /* Set Backup registers erase event */
    if (VBATT_TAMPER_ERASE_ENABLE == p_vbatt_set->tamper_erase)
    {
        /* Set Backup registers erase event */
        uc_Backup_TAMPCR = uc_Backup_TAMPCR | (VBATT_MASK_TAMPEE<<p_vbatt_set->tamper_channel);
    }
    else
    {
        /* Set Backup registers erase event */
        uc_Backup_TAMPCR = uc_Backup_TAMPCR & (~(VBATT_MASK_TAMPEE<<p_vbatt_set->tamper_channel));
    }

    SYSTEM.TAMPCR.BYTE = uc_Backup_TAMPCR;
    if (SYSTEM.TAMPCR.BYTE)                 /* dummy read */
    {
        /* Do nothing */
    }
    
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);
}
/******************************************************************************
End of function r_vbatt_set_io_register
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_getstatus_target_mcu
* Description  : This function get the state of the Tamper detector register. 
*                This result is set to the pointer of the argument. 
* Arguments    : vbatt_status_t * p_vbatt_status    ; Pointer to a data structure for 
*                the battery backup status information.
* Return Value : VBATT_SUCCESS                      ; Successful operation
************************************************************************************/
vbatt_return_t r_vbatt_getstatus_target_mcu(vbatt_status_t * p_vbatt_status)
{
     uint8_t uc_Temp = 0;
    /* ==== READ THE VBATTSR REGISTER ==== */

    /* Tamper n Detection Flag */
    uc_Temp = SYSTEM.TAMPSR.BYTE;
    p_vbatt_status->tamper_detection_flag = ((uc_Temp >> p_vbatt_status->tamper_channel)\
    & VBATT_MASK_TAMP_DETECT_FLAG);

    if(true == p_vbatt_status->action_clear)
    {
        if(1 == p_vbatt_status->tamper_detection_flag)
        {
            /* Clear tamper status flag if action_clear is true */
            R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);
            uc_Temp = SYSTEM.TAMPSR.BYTE;
            SYSTEM.TAMPSR.BYTE = uc_Temp & (~(VBATT_MASK_TAMP_DETECT_FLAG << \
            p_vbatt_status->tamper_channel));

            if (SYSTEM.TAMPSR.BYTE)                 /* dummy read */
            {
                /* Do nothing */
            }
            
            R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);
        }
    }

    /* Channel n Level Monitoring Flag */
    uc_Temp = SYSTEM.TAMPIMR.BYTE;
    p_vbatt_status->tamper_level_monitoring_flag = ((uc_Temp >> \
    p_vbatt_status->tamper_channel) & VBATT_MASK_TAMP_MONITOR_FLAG);

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_getstatus_target_mcus
******************************************************************************/

/************************************************************************************
* Outline      : Tamper detector Function Maskable Interrupt
* Function Name: r_vbatt_isr
* Description  : Tamper detector Function Maskable Interrupt
* Arguments    : None
* Return Value : None
************************************************************************************/
/* ---- Tamper detector Function Maskable Interrupt ---- */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_vbatt_isr,VECT_VBATT_TAMPDI)
R_BSP_ATTRIB_STATIC_INTERRUPT void r_vbatt_isr(void)
{
    /* Calls battery backup function maskable interrupt processing function */
    r_vbatt_isr_processing();
}
/******************************************************************************
End of function r_vbatt_isr
******************************************************************************/

#endif /* BSP_MCU_RX671 */

/* End of File */

