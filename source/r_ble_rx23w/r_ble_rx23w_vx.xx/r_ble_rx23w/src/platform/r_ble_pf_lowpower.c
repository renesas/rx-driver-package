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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_ble_pf_lowpower.c
* Description  : The user configurable low power functions.
* History      : DD.MM.YYYY Version Description           
*              : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

#include "platform.h"
#include "r_ble_rx23w_if.h"

#if (BLE_CFG_MCU_LPC_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0)

#include "r_lpc_rx_if.h"
#include "cli/r_ble_cli.h"
#include "board/r_ble_board.h"
#include "timer/r_ble_timer.h"

bool g_inhibit_software_standby = true;


/* Check whether MCU can enter software standby mode. */
static bool check_software_standby(void)
{
    if (g_inhibit_software_standby)
    {
        return false;
    }

#if BLE_CFG_CMD_LINE_EN
    if (g_cli_tx_flg)
    {
        return false;
    }
#endif /* BLE_CFG_CMD_LINE_EN */

    if( false != R_BLE_TIMER_IsActive() )
    {
    	return false;
    }

    /* If DTC/DMAC/DataFlash is in active, MCU can not enter software standby.
       This code is copied from r_lpc_rx23w.c lpc_lowpower_activate_check. */
    if ((0x0000 != (FLASH.FENTRYR.WORD & 0x0081)) ||
        ((0 == SYSTEM.MSTPCRA.BIT.MSTPA28) &&
         ((1 == DTC.DTCST.BIT.DTCST) || (1 == DMAC.DMAST.BIT.DMST))))
    {
        return false;
    }

    /* add check for other components */

    return true;
}

/* Check whether MCU can enter deep sleep mode. */
static bool check_deep_sleep(void)
{
    /* If DTC/DMAC/DataFlash is in active, MCU can not enter deep sleep.
       This code is copied from r_lpc_rx23w.c lpc_lowpower_activate_check. */
    if ((0x0000 != (FLASH.FENTRYR.WORD & 0x0081)) ||
        (0 == SYSTEM.MSTPCRA.BIT.MSTPA28))
    {
        return false;
    }

    /* add check for other components */

    return true;
}

/* Check whether MCU can enter sleep mode. */
static bool check_sleep(void)
{
    /* add check for other components */

    return true;
}

/* Make peripherals low power state. */
static void suspend_peripherals(lpc_low_power_mode_t mode)
{
    if (LPC_LP_SW_STANDBY == mode)
    {
        R_BLE_CLI_Terminate();

        /* add implementation for transiting the software standby mode. */
    }
    else if(LPC_LP_DEEP_SLEEP == mode)
    {
        /* add implementation for transiting the deep sleep mode. */
    }
    else if(LPC_LP_SLEEP == mode)
    {
        /* add implementation for transiting the sleep mode. */
    }
    else
    {

    }
}

/* Make peripherals active state. */
static void resume_peripherals(lpc_low_power_mode_t mode)
{
    if (LPC_LP_SW_STANDBY == mode)
    {
        R_BLE_CLI_Init();

        /* add implementation for transiting the active state. */
    }
    else if(LPC_LP_DEEP_SLEEP == mode)
    {
        /* add implementation for transiting the active state. */
    }
    else if(LPC_LP_SLEEP == mode)
    {
        /* add implementation for transiting the active state. */
    }
    else
    {
        
    }
}

/* Set wakeup trigger */
static void set_wakeup_trigger(lpc_low_power_mode_t mode)
{
    /* Wake up trigger SW2 is always enabled in rx_family_main.c. */
}

/* Unset wakeup trigger */
static void unset_wakeup_trigger(lpc_low_power_mode_t mode)
{
    /* Wake up trigger SW2 is always enabled in rx_family_main.c. */
}

void R_BLE_LPC_Init(void)
{
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Set to module stop state. */
    MSTP(DMAC) = 1;
    MSTP(DTC)  = 1;

    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

}

void R_BLE_LPC_EnterLowPowerMode(void)
{
    lpc_low_power_mode_t mode;

    /* Disable interrupt. */
    R_BSP_InterruptsDisable();

#if (BLE_CFG_CMD_LINE_EN == 1)
    /* If reception of a command occurs, execution of MCU low power is skipped once. */
    if (false != g_cli_rx_flg)
    {
        g_cli_rx_flg = false;
        R_BSP_InterruptsEnable();
        return;
    }
#endif /* (BLE_CFG_CMD_LINE_EN == 1) */

    /* Check if BLE task queue is free. */
    if(0 == R_BLE_IsTaskFree()) 
    {
        R_BSP_InterruptsEnable();
        return;
    }

    /* Check whether MCU can enter software standby mode. */
    if (check_software_standby())
    {
        R_LPC_LowPowerModeConfigure(LPC_LP_SW_STANDBY);
        mode = LPC_LP_SW_STANDBY;
    }
    /* Check whether MCU can enter deep sleep mode. */
    else if (check_deep_sleep())
    {
        R_LPC_LowPowerModeConfigure(LPC_LP_DEEP_SLEEP);
        mode = LPC_LP_DEEP_SLEEP;
    }
    /* Check whether MCU can enter sleep mode. */
    else if (check_sleep())
    {
        R_LPC_LowPowerModeConfigure(LPC_LP_SLEEP);
        mode = LPC_LP_SLEEP;
    }
    else
    {
        R_BSP_InterruptsEnable();
        return;
    }

    /* Set peripherals low power state, before MCU enter low power mode. */
    suspend_peripherals(mode);

    /* Set wake up trigger. */
    set_wakeup_trigger(mode);

    /* MCU enter low power mode. */
    R_LPC_LowPowerModeActivate(NULL);

    /* Unset wake up trigger. */
    unset_wakeup_trigger(mode);

    /* Set peripherals active state, after MCU resume from low power mode. */
    resume_peripherals(mode);
}

void R_BLE_LPC_SetInhibitSoftwareStandby(bool inhibit)
{
    g_inhibit_software_standby = inhibit;
}

bool R_BLE_LPC_GetInhibitSoftwareStandby(void)
{
    return g_inhibit_software_standby;
}

#else /* (BLE_CFG_MCU_LPC_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

void R_BLE_LPC_Init(void)
{
}

void R_BLE_LPC_EnterLowPowerMode(void)
{
}

void R_BLE_LPC_SetInhibitSoftwareStandby(bool inhibit)
{
    (void)inhibit;
}

bool R_BLE_LPC_GetInhibitSoftwareStandby(void)
{
    return false;
}

#endif /* (BLE_CFG_MCU_LPC_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */
