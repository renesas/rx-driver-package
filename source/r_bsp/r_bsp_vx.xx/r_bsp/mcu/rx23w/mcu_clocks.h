/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : mcu_clocks.h
* Description  : Contains clock specific routines.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 29.03.2019 1.00     First Release
*         : 30.11.2021 2.00     Added the bsp_clkout_initial_configure function.
*         : 21.11.2023 2.01     Added definition of bsp_mcu_clock_reset_bootloader function.
*         : 26.02.2025 2.02     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef MCU_CLOCKS_H
#define MCU_CLOCKS_H

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint32_t get_iclk_freq_hz(void);
void mcu_clock_setup(void);
#if BSP_CFG_CLKOUT_RF_MAIN == 1
void operating_frequency_set(void);
#if BSP_CFG_BOOTLOADER_PROJECT == 0
void usb_lpc_clock_source_select(void);
#if BSP_CFG_CLKOUT_OUTPUT != 0
void bsp_clkout_initial_configure(void)
#endif /* BSP_CFG_CLKOUT_OUTPUT != 0 */
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */
#endif /* BSP_CFG_CLKOUT_RF_MAIN == 1 */

#if BSP_CFG_BOOTLOADER_PROJECT == 1
/* Enable the following functions in the bootloader project. */
void bsp_mcu_clock_reset_bootloader(void);
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 1 */

/* End of multiple inclusion prevention macro */
#endif

