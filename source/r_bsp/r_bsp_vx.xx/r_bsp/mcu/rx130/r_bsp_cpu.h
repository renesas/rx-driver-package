/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp_cpu.h
* Description  : This module implements CPU specific functions. An example is enabling/disabling interrupts.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.12.2015 1.00     First Release
*         : 15.05.2017 1.01     Changed comments of the following enumeration.
*                               - bsp_reg_protect_t
*         : 28.02.2019 2.00     Added bsp_ram_initialize function.
*                               Fixed coding style.
*         : 26.07.2019 2.01     Added R_BSP_SoftwareReset function.
*         : 21.11.2023 2.02     Added bsp_bus_priority_initialize function.
*         : 26.02.2025 2.03     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef CPU_H
#define CPU_H

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* The different types of registers that can be protected. */
typedef enum
{
    /* PRC0
       Enables writing to the registers related to the clock generation circuit: SCKCR, SCKCR3,
       PLLCR, PLLCR2, MOSCCR, SOSCCR, LOCOCR, ILOCOCR, HOCOCR, HOFCR, OSTDCR, OSTDSR, CKOCR, LOCOTRR,
       ILOCOTRR, HOCOTRR0. */
    BSP_REG_PROTECT_CGC = 0,

    /* PRC1
       Enables writing to the registers related to operating modes, low power consumption,
       the clock generation circuit, and software reset: SYSCR1, SBYCR, MSTPCRA, MSTPCRB, MSTPCRC,
       MSTPCRD, OPCCR, RSTCKCR, SOPCCR, MOFCR, MOSCWTCR, SWRR. */
    BSP_REG_PROTECT_LPC_CGC_SWR,

    /* PRC2
       Enables writing to the registers related to the LPT: LPTCR1, LPTCR2, LPTCR3, LPTPRD, LPCMR0, LPWUCR. */
    BSP_REG_PROTECT_LPT,

    /* PRC3
       Enables writing to the registers related to the LVD: LVCMPCR, LVDLVLR, LVD1CR0, LVD1CR1, LVD1SR, LVD2CR0, 
       LVD2CR1, LVD2SR. */
    BSP_REG_PROTECT_LVD,

    /* MPC.PWPR
       Enables writing to MPC's PFS registers. */
    BSP_REG_PROTECT_MPC,

    /* This entry is used for getting the number of enum items. This must be the last entry. DO NOT REMOVE THIS ENTRY!*/
    BSP_REG_PROTECT_TOTAL_ITEMS
} bsp_reg_protect_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
void     R_BSP_InterruptsDisable(void);
void     R_BSP_InterruptsEnable(void);
uint32_t R_BSP_CpuInterruptLevelRead(void);
bool     R_BSP_CpuInterruptLevelWrite(uint32_t level);
void     R_BSP_RegisterProtectEnable(bsp_reg_protect_t regs_to_protect);
void     R_BSP_RegisterProtectDisable(bsp_reg_protect_t regs_to_unprotect);
void     R_BSP_SoftwareReset(void);

void     bsp_register_protect_open(void); //r_bsp internal function. DO NOT CALL.
void     bsp_ram_initialize(void);
#if BSP_CFG_BUS_PRIORITY_INITIALIZE_ENABLE == 1
void     bsp_bus_priority_initialize(void);
#endif

#endif /* CPU_H */

