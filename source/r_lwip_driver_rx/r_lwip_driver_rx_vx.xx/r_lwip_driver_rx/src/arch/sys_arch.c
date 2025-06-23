/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/

#include "platform.h"
#include "arch/sys_arch.h"
#include "r_lwip_rx_config.h"
#include "r_lwip_driver_rx_config.h"

#if NO_SYS

/******************************************************************************
* Function Name:    sys_arch_protect
* Description  :    Clear and return the I FLAG of CPU of PWS register value.
* Arguments    :    -
*
* Return Value :    Previous I flag value.
*
******************************************************************************/

sys_prot_t sys_arch_protect(void)
{
    uint64_t psw_val = R_BSP_GET_PSW();
    sys_prot_t I_flag_val;


    /* Disable I flag(All Interrupts) */
    R_BSP_InterruptsDisable();

    /* get the I Flag of PSW register value */
    I_flag_val = (sys_prot_t)((psw_val & 0x00010000) >> 16);

    return I_flag_val;

}

/******************************************************************************
* Function Name:    sys_arch_unprotect
* Description  :    Resume the I FLAG of CPU of PWS register value.
* Arguments    :    sys_prot_t pval
*                       Resume value pval
* Return Value :    None
*
******************************************************************************/

void sys_arch_unprotect(sys_prot_t pval)
{
    if(pval)
    {
        /* Set I flag to enable when the Previous I flag status is enabled (Value is 1) */
        R_BSP_InterruptsEnable();
    }
}

#endif /* NO_SYS */
