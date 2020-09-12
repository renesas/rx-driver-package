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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.06.2019 1.00     First Release
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx23w.c
* Device(s)    : RX23W
* Tool-Chain   : Renesas RX Standard Toolchain 3.01.00
*                GCC for Renesas RX 4.8.4.201801
*                IAR C/C++ Compiler for Renesas RX version 4.10.1
* OS           : None
* H/W Platform : RSSKRX23W
* Description  : Functions for using Battery Backup Function on RX devices. 
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

#ifdef BSP_MCU_RX23W

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines for Battery Backup Function support */
#include "../r_vbatt_rx_private.h"
#include "r_vbatt_rx23w_private.h"
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
    uint32_t * pofs1;

    /* ==== CHECKS POINTER OF ARGMENT ==== */
    if (NULL == p_arg)
    {
        return VBATT_ERR_INVALID_ARG;
    }

    /* ==== CHECKS ARGMENT ==== */
    switch (called_api)
    {
        case VBATT_MODE_OPEN:
            pofs1 = VBATT_OFS1_ADDRESS;             /* sets ofs1 address */

            /* ---- checks LVD0 of OFS1 setting ---- */
            if (VBATT_OFS1_LVD0_2_51_V_ENABLE != ((*pofs1) & VBATT_OFS1_LVD0_BIT_MASK))
            {
                return VBATT_ERR_LVD0_SETTING;
            }

            /* ---- checks pointer of callback function ---- */
            if (NULL == ((vbatt_info_t *)p_arg)->callbackfunc)
            {
                return VBATT_ERR_INVALID_ARG;
            }

        break;

        case VBATT_MODE_CONTROL:

            /* ---- checks information struct ---- */
            if (1 < ((vbatt_ctrl_info_t*)p_arg)->vbatt_ctrl.bit.lvd_level)
            {
                return VBATT_ERR_INVALID_ARG;
            }

            if ((1 > ((vbatt_ctrl_info_t*)p_arg)->vbatt_int_priority) ||
                (15 < ((vbatt_ctrl_info_t*)p_arg)->vbatt_int_priority))
            {
                return VBATT_ERR_INVALID_ARG;
            }

        break;

        case VBATT_MODE_GETSTATUS:

            /* ---- checks VBATT detection enable ---- */
            if (VBATT_VBATTCR_VBTLVDEN_DISABLE == SYSTEM.VBATTCR.BIT.VBTLVDEN)
            {
                return VBATT_ERR_FUNC_INVALID;
            }

        break;

        default:

            /* Do nothing */

        break;
    }

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_check_parameter
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_open_set_target_mcu
* Description  : According to the setting of the configuration option,  
*                This function sets the validity / invalidity of the VBATT pin 
*                voltage drop detection function and a detection level, an interrupt.
* Arguments    : none
* Return Value : VBATT_SUCCESS                      ; Successful operation
************************************************************************************/
vbatt_return_t r_vbatt_open_set_target_mcu(void)
{
    vbatt_ctrl_info_t   vbatt_set;

    vbatt_set.vbatt_ctrl.bit.func           = VBATT_ENABLE;
    vbatt_set.vbatt_ctrl.bit.lvd_level      = VBATT_CFG_DETECT_LEVEL;
    vbatt_set.vbatt_ctrl.bit.lvd_detect     = VBATT_CFG_DETECT_FUNCTION;
    vbatt_set.vbatt_int_priority            = VBATT_CFG_INT_PRIORITY;

    r_vbatt_set_io_register(&vbatt_set);

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_open_set_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_open_check_target_mcu
* Description  : Check battery backup power voltage drop detection flag
* Arguments    : none
* Return Value : SYSTEM.VBATTSR.BIT.VBATRLVDETF
************************************************************************************/
uint8_t r_vbatt_open_check_target_mcu(void)
{
    /* ==== RETURN BATTERY BACKUP POWER VOLTAGE DROP DETECTION FLAG ==== */
    return SYSTEM.VBATTSR.BIT.VBATRLVDETF;
}
/******************************************************************************
End of function r_vbatt_open_check_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_open_f_clear_target_mcu
* Description  : This function clear the battery backup power voltage drop detection flag.
* Arguments    : none
* Return Value : none
************************************************************************************/
void r_vbatt_open_f_clear_target_mcu(void)
{
    /* ==== CLEAR THE VBATRLVDETF BIT OF THE VBATTSR REGISTER ==== */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LVD);  /* unlock */
    SYSTEM.VBATTSR.BIT.VBATRLVDETF = 0;                 /* battery backup power voltage drop detection flag */

    if (SYSTEM.VBATTSR.BIT.VBATRLVDETF)                 /* dummy read                               */
    {
        /* Do nothing */
    }

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LVD);   /* lock */
}
/******************************************************************************
End of function r_vbatt_open_f_clear_target_mcu
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_control_target_mcu
* Description  : This function sets it according to an argument.
*                   - sets the validity / invalidity of the battery backup function.
*                   - sets the effective / invalidity of the VBATT pin voltage drop detection function,
*                     a detection level, an interrupt.
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
*                   - sets the validity / invalidity of the battery backup function.
*                   - sets the effective / invalidity of the VBATT pin voltage drop detection function,
*                     a detection level, an interrupt.
* Arguments    : vbatt_ctrl_info_t * p_vbatt_set    ; VBATT Pin Voltage Drop Detection Function information
* Return Value : none
************************************************************************************/
static void r_vbatt_set_io_register(vbatt_ctrl_info_t * p_vbatt_set)
{
    /* ==== VBATT PIN VOLTAGE DROP DETECTION SETTING ==== */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LVD);  /* unlock */

    /* ---- Battery Backup Function ---- */
    if (VBATT_ENABLE == p_vbatt_set->vbatt_ctrl.bit.func)
    {
        /* Battery Backup Function is enable */
        SYSTEM.VBATTCR.BIT.VBATTDIS = VBATT_VBATTCR_VBATTDIS_ENABLE;
    }
    else
    {
        /* Battery Backup Function is disable */
        SYSTEM.VBATTCR.BIT.VBATTDIS = VBATT_VBATTCR_VBATTDIS_DISABLE;
    }

    /* ---- VBATT pin voltage drop detection level ---- */
    switch (p_vbatt_set->vbatt_ctrl.bit.lvd_level)
    {
        case VBATT_DTCT_LEVEL_2_20_V:

            SYSTEM.VBATTCR.BIT.VBTLVDLVL = VBATT_VBATTCR_VBTLVDLVL_2_20V;   /* 2.20V */
        break;

        case VBATT_DTCT_LEVEL_2_00_V:

            SYSTEM.VBATTCR.BIT.VBTLVDLVL = VBATT_VBATTCR_VBTLVDLVL_2_00V;   /* 2.00V */
        break;

        default:

            /* Do nothing */
        break;
    }

    /* ---- VBATT pin voltage drop detection function enable, interrupt enable, and interrupt type ---- */
    switch (p_vbatt_set->vbatt_ctrl.bit.lvd_detect)
    {
        case VBATT_DTCT_DISABLE:

            /* VBATT pin voltage drop detection disabled
               VBATT pin voltage drop detection interrupt disabled */
            SYSTEM.VBATTCR.BIT.VBTLVDEN = VBATT_VBATTCR_VBTLVDEN_DISABLE;
            SYSTEM.VBTLVDICR.BIT.VBTLVDIE = VBATT_VBTLVDICR_VBTLVDIE_DISABLE;
        break;

        case VBATT_DTCT_ENABLE_INT_DISABLE:

            /* VBATT pin voltage drop detection enabled
               VBATT pin voltage drop detection interrupt disabled */
            R_BSP_InterruptRequestDisable(VECT(VBATT,VBTLVDI));
            IPR(VBATT,VBTLVDI) = 0;
            SYSTEM.VBATTCR.BIT.VBTLVDEN = VBATT_VBATTCR_VBTLVDEN_ENABLE;
            SYSTEM.VBTLVDICR.BIT.VBTLVDIE = VBATT_VBTLVDICR_VBTLVDIE_DISABLE;
        break;

        case VBATT_DTCT_ENABLE_NMI_ENABLE:

            /* VBATT pin voltage drop detection enabled
               VBATT pin voltage drop detection interrupt enabled
               VBATT pin voltage drop detection interrupt enable type is non-maskable interrupt */
            SYSTEM.VBATTCR.BIT.VBTLVDEN = VBATT_VBATTCR_VBTLVDEN_ENABLE;
            SYSTEM.VBTLVDICR.BIT.VBTLVDIE = VBATT_VBTLVDICR_VBTLVDIE_ENABLE;
            SYSTEM.VBTLVDICR.BIT.VBTLVDISEL = VBATT_VBTLVDICR_VBTLVDISEL_NON_MASKABLE;
            R_BSP_InterruptWrite(BSP_INT_SRC_VBATT, (bsp_int_cb_t)r_vbatt_nmi_isr_processing);
            ICU.NMIER.BIT.VBATEN = VBATT_NMIER_VBATEN_ENABLE;
        break;

        case VBATT_DTCT_ENABLE_INT_ENABLE:

            /* VBATT pin voltage drop detection enabled
               VBATT pin voltage drop detection interrupt enabled
               VBATT pin voltage drop detection interrupt enable type is maskable interrupt */
            SYSTEM.VBATTCR.BIT.VBTLVDEN = VBATT_VBATTCR_VBTLVDEN_ENABLE;
            SYSTEM.VBTLVDICR.BIT.VBTLVDIE = VBATT_VBTLVDICR_VBTLVDIE_ENABLE;
            SYSTEM.VBTLVDICR.BIT.VBTLVDISEL = VBATT_VBTLVDICR_VBTLVDISEL_MASKABLE;
            IPR(VBATT,VBTLVDI) = p_vbatt_set->vbatt_int_priority;
            R_BSP_InterruptRequestEnable(VECT(VBATT,VBTLVDI));
        break;

        default:

            /* Do nothing */
        break;
    }

    if (SYSTEM.VBATTCR.BIT.VBTLVDEN)                    /* dummy read                               */
    {
        /* Do nothing */
    }

    R_BSP_SoftwareDelay(350, BSP_DELAY_MICROSECS);      /* Delay 350 microseconds before returning  */

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LVD);   /* lock */
}
/******************************************************************************
End of function r_vbatt_set_io_register
******************************************************************************/

/************************************************************************************
* Function Name: r_vbatt_getstatus_target_mcu
* Description  : This function get the state of the battery backup function
*                from VBATTSR register.
*                This result is set to the pointer of the argument.
* Arguments    : vbatt_status_t * p_vbatt_status    ; Battery Backup Function status information
* Return Value : VBATT_SUCCESS                      ; Successful operation
************************************************************************************/
vbatt_return_t r_vbatt_getstatus_target_mcu(vbatt_status_t * p_vbatt_status)
{
    /* ==== READ THE VBATTSR REGISTER ==== */
    p_vbatt_status->vbatt_status.bit.pwr_drp_dtct   = SYSTEM.VBATTSR.BIT.VBATRLVDETF;
    p_vbatt_status->vbatt_status.bit.vbatt_mon      = SYSTEM.VBATTSR.BIT.VBTLVDMON;
    p_vbatt_status->vbatt_status.bit.rsv            = VBATT_RSV_INIT;

    return VBATT_SUCCESS;
}
/******************************************************************************
End of function r_vbatt_getstatus_target_mcu
******************************************************************************/

/************************************************************************************
* Outline      : Battery Backup Function Maskable Interrupt
* Function Name: r_vbatt_isr
* Description  : Battery Backup Function Maskable Interrupt
* Arguments    : None
* Return Value : None
************************************************************************************/
/* ---- Battery Backup Function Maskable Interrupt ---- */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_vbatt_isr,VECT_VBATT_VBTLVDI)
R_BSP_ATTRIB_STATIC_INTERRUPT void r_vbatt_isr(void)
{
    /* Calls battery backup function maskable interrupt processing function */
    r_vbatt_isr_processing();
}
/******************************************************************************
End of function r_vbatt_isr
******************************************************************************/

#endif /* BSP_MCU_RX23W */

/* End of File */

