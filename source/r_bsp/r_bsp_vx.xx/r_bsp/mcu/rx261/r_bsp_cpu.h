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
*         : 31.05.2024 1.00     First Release
*         : 26.02.2025 1.01     Changed the disclaimer.
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
       Enables writing to the registers related to the clock generation circuit: SCKCR, SCKCR3, PLLCR, PLLCR2, PLL2CR,
       PLL2CR2, MOSCCR, SOSCCR, LOCOCR, ILOCOCR, HOCOCR, LOFCR, OSTDCR, OSTDSR, CKOCR, LOCOTRR2, ILOCOTRR, HOCOTRR0, 
       SOMCR, CANFDCKCR, CANFDCKDIVCR, USBCKCR. */
    BSP_REG_PROTECT_CGC = 0,

    /* PRC1
       Enables writing to the registers related to operating modes, low power consumption, the clock generation circuit,
       and software reset: SYSCR1, SBYCR, MSTPCRA, MSTPCRB, MSTPCRC, MSTPCRD, OPCCR, RSTCKCR, SOPCCR, RPSCR, SNZCR,
       SNZCR2, MOFCR, MOSCWTCR, SWRR. */
    BSP_REG_PROTECT_LPC_CGC_SWR,

    /* PRC2
       Enables writing to the registers related to low power timer: LPTCR1, LPTCR2, LPTCR3, LPTPRD, LPCMR0, LPCMR1,
       LPWUCR */
    BSP_REG_PROTECT_LPT,

    /* PRC3
       Enables writing to the registers related to the LVD:LVCMPCR, LVDLVLR, LVD1CR0, LVD1CR1, LVD1SR, LVD2CR0,
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

/**********************************************************************************************************************
 * Function Name: R_BSP_InterruptsDisable
 ******************************************************************************************************************//**
 * @brief Globally disables interrupts.
 * @details This function globally disables interrupts. This is performed by clearing the 'I' bit in the CPU's
 * Processor Status Word (PSW) register.
 * @note The 'I' bit of the PSW can only be modified when in Supervisor Mode. If the CPU is in User Mode and this
 * function is called, this function does nothing.
 */
void     R_BSP_InterruptsDisable (void);

/**********************************************************************************************************************
 * Function Name: R_BSP_InterruptsEnable
 ******************************************************************************************************************//**
 * @brief Globally enable interrupts.
 * @details This function globally enables interrupts. This is performed by setting the 'I' bit in the CPU's Processor
 * Status Word (PSW) register.
 * @note The 'I' bit of the PSW can only be modified when in Supervisor Mode. If the CPU is in User Mode and this
 * function is called, this function does nothing.
 */
void     R_BSP_InterruptsEnable (void);

/**********************************************************************************************************************
 * Function Name: R_BSP_CpuInterruptLevelRead
 ******************************************************************************************************************//**
 * @brief Reads the CPU's Interrupt Priority Level.
 * @return The CPU's Interrupt Priority Level.
 * @details This function reads the CPU's Interrupt Priority Level. This level is stored in the IPL bits of the
 * Processor Status Word (PSW) register.
 */
uint32_t R_BSP_CpuInterruptLevelRead (void);

/**********************************************************************************************************************
 * Function Name: R_BSP_CpuInterruptLevelWrite
 ******************************************************************************************************************//**
 * @brief Writes the CPU's Interrupt Priority Level.
 * @param[in] level The level to write to the CPU's IPL.
 * @retval true Successful, CPU's IPL has been written.
 * @retval false Failure, provided 'level' has invalid IPL value or called when the CPU is in User Mode.
 * @details This function writes the CPU's Interrupt Priority Level. This level is stored in the IPL bits of the
 * Processor Status Word (PSW) register. This function does check to make sure that the IPL being written is valid.
 * The maximum and minimum valid settings for the CPU IPL are defined in mcu_info.h using the BSP_MCU_IPL_MAX and
 * BSP_MCU_IPL_MIN macros.
 * @note The CPU's IPL can only be modified by the user when in Supervisor Mode. If the CPU is in User Mode and this
 * function is called, this function does not control IPL and return false.
 */
bool     R_BSP_CpuInterruptLevelWrite (uint32_t level);

/**********************************************************************************************************************
 * Function Name: R_BSP_RegisterProtectEnable
 ******************************************************************************************************************//**
 * @brief Enables write protection for selected registers.
 * @param[in] regs_to_protect Which registers to enable write protection for.
 * @details This function enables write protection for the input registers. Only certain MCU registers have the
 * ability to be write protected. To see which registers are available to be protected by this function look at the
 * bsp_reg_protect_t enum in r_bsp_cpu.h for your MCU.
 * This function, and R_BSP_RegisterProtectDisable(), use counters for each entry in the bsp_reg_protect_t enum so
 * that users can call these functions multiple times without problem. This function uses the interrupt disable /
 * enable function by controlling the Processor Interrupt Priority Level (IPL) of the R_BSP_InterruptControl function,
 * because counter control is the critical section. If the function is executed while the processor mode is supervisor
 * mode, interrupts that are at or below the specified interrupt priority level will be disabled by controlling the
 * IPL. If the function is executed while the processor mode is user mode, the IPL controlling does not execute. An
 * example of why this is needed is shown below in the Special Notes section below.
 * @note
 * (1) About why counters are needed. \n
 * See Section 5.7 in the application note for details.\n
 * (2) Notes on user mode \n
 * The R_BSP_InterruptControl function used to secure atomicity in the critical section of the counter control with
 * this function is valid only in supervisor mode. When this function is executed in user mode, the
 * R_BSP_InterruptControl function is executed but atomicity is not to secure.
 */
void     R_BSP_RegisterProtectEnable (bsp_reg_protect_t regs_to_protect);

/**********************************************************************************************************************
 * Function Name: R_BSP_RegisterProtectDisable
 ******************************************************************************************************************//**
 * @brief Disables write protection for selected registers.
 * @param[in] regs_to_unprotect Which registers to disable write protection for.
 * @details This function disables write protection for the input registers. Only certain MCU registers have the
 * ability to be write protected. To see which registers are available to be protected by this function look at the
 * bsp_reg_protect_t enum in r_bsp_cpu.h for your MCU.
 * This function, and R_BSP_RegisterProtectEnable(), use counters for each entry in the bsp_reg_protect_t enum so that
 * users can call these functions multiple times without problem. This function uses the interrupt disable /
 * enable function by controlling the Processor Interrupt Priority Level (IPL) of the R_BSP_InterruptControl function,
 * because counter control is the critical section. If the function is executed while the processor mode is supervisor
 * mode, interrupts that are at or below the specified interrupt priority level will be disabled by controlling the
 * IPL. If the function is executed while the processor mode is user mode, the IPL controlling does not execute.
 * @note The R_BSP_InterruptControl function used to secure atomicity in the critical section of the counter control
 * with this function is valid only in supervisor mode. When this function is executed in user mode, the
 * R_BSP_InterruptControl function is executed but atomicity is not to secure.
 */
void     R_BSP_RegisterProtectDisable (bsp_reg_protect_t regs_to_unprotect);

/**********************************************************************************************************************
 * Function Name: R_BSP_SoftwareReset
 ******************************************************************************************************************//**
 * @details Reset the MCU by Software Reset.
 */
void     R_BSP_SoftwareReset (void);

/***********************************************************************************************************************
* Function Name: bsp_register_protect_open
* Description  : Initializes variables needed for register protection functionality.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void     bsp_register_protect_open (void); //r_bsp internal function. DO NOT CALL.

/***********************************************************************************************************************
* Function Name: bsp_ram_initialize
* Description  : Initialize ram variable.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void     bsp_ram_initialize (void);
#if BSP_CFG_BUS_PRIORITY_INITIALIZE_ENABLE == 1
/***********************************************************************************************************************
* Function Name: bsp_bus_priority_initialize
* Description  : Initialize bus priority.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void     bsp_bus_priority_initialize (void);
#endif

#endif /* CPU_H */

