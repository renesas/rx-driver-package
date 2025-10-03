/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : hwsetup.c
* Device(s)    : RX
* H/W Platform : GENERIC_RX660
* Description  : Defines the initialization routines used each time the MCU is restarted.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version   Description
*         : 22.04.2022 1.00      First Release
*         : 21.11.2023 1.01      Added compile switch of BSP_CFG_BOOTLOADER_PROJECT.
*         : 26.02.2025 1.02      Changed the disclaimer.
***********************************************************************************************************************/


/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* I/O Register and board definitions */
#include "platform.h"
#if BSP_CFG_CONFIGURATOR_SELECT == 1
#include "r_cg_macrodriver.h"
#endif

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
#if BSP_CFG_BOOTLOADER_PROJECT == 0
/* Disable the following functions in the bootloader project. */
/* MCU I/O port configuration function declaration */
static void output_ports_configure(void);

/* Interrupt configuration function declaration */
static void interrupts_configure(void);
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

/* MCU peripheral module configuration function declaration */
static void peripheral_modules_enable(void);

#if BSP_CFG_BOOTLOADER_PROJECT == 0
/* Disable the following functions in the bootloader project. */
/* VOLSR register initial configuration function declaration */
static void bsp_volsr_initial_configure(void);
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

/***********************************************************************************************************************
* Function name: hardware_setup
* Description  : Contains setup functions called at device restart
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void hardware_setup(void)
{
#if BSP_CFG_BOOTLOADER_PROJECT == 0
    /* Disable the following functions in the bootloader project. */
    output_ports_configure();
    interrupts_configure();
    bsp_volsr_initial_configure();
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */
    peripheral_modules_enable();
    bsp_non_existent_port_init();
} /* End of function hardware_setup() */

#if BSP_CFG_BOOTLOADER_PROJECT == 0
/* Disable the following functions in the bootloader project. */
/***********************************************************************************************************************
* Function name: output_ports_configure
* Description  : Configures the port and pin direction settings, and sets the pin outputs to a safe level.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void output_ports_configure(void)
{
    /* Add code here to setup additional output ports */
    R_BSP_NOP();
} /* End of function output_ports_configure() */

/***********************************************************************************************************************
* Function name: interrupts_configure
* Description  : Configures interrupts used
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void interrupts_configure(void)
{
    /* Add code here to setup additional interrupts */
    R_BSP_NOP();
} /* End of function interrupts_configure() */
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

/***********************************************************************************************************************
* Function name: peripheral_modules_enable
* Description  : Enables and configures peripheral devices on the MCU
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void peripheral_modules_enable(void)
{
    /* Add code here to enable peripherals used by the application */
#if BSP_CFG_CONFIGURATOR_SELECT == 1
    /* Smart Configurator initialization function */
    R_Systeminit();
#endif
} /* End of function peripheral_modules_enable() */

#if BSP_CFG_BOOTLOADER_PROJECT == 0
/* Disable the following functions in the bootloader project. */
/***********************************************************************************************************************
* Function name: bsp_volsr_initial_configure
* Description  : Initializes the VOLSR register.
* Arguments    : none
* Return value : none
* Note         : none
***********************************************************************************************************************/
static void bsp_volsr_initial_configure(void)
{
    /* Used for argument of R_BSP_VoltageLevelSetting function. */
    uint8_t tmp_arg = 0;

    /* Set the pattern of the VOLSR(RICVLS). */
#if (4500 <= BSP_CFG_MCU_VCC_MV) && (5500 >= BSP_CFG_MCU_VCC_MV)
    tmp_arg |= BSP_VOL_RIIC_4_5V_OROVER;
#elif (2700 <= BSP_CFG_MCU_VCC_MV) && (4500 > BSP_CFG_MCU_VCC_MV)
    tmp_arg |= BSP_VOL_RIIC_UNDER_4_5V;
#else
    #error "Error! Invalid setting for BSP_CFG_MCU_VCC_MV in r_bsp_config.h"
#endif

    R_BSP_VoltageLevelSetting(tmp_arg);
} /* End of function bsp_volsr_initial_configure() */
#endif /* BSP_CFG_BOOTLOADER_PROJECT == 0 */

