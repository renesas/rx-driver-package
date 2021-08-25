/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riichs_rx671.c
 * Description  : Functions for using RIICHS on RX devices. 
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.06.2021 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Defines for RIICHS support */
#include "r_riichs_rx_if.h"

/* Check MCU Group */
#if defined(BSP_MCU_RX671)

    #include "r_riichs_rx_private.h"

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/
    #if (1U == RIICHS_CFG_PORT_SET_PROCESSING)
/***********************************************************************************************************************
 * Function Name: riichs_mcu_io_open
 * Description  : Sets ports to input mode.
 *              : Ports input pull-up becomes "Off".
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_io_open (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppcr = NULL;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppdr = NULL;
    uint8_t port_gr = 0;
    uint8_t pin_num = 0;

    /* Channel number? */
    switch (channel)
    {
        #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */

            /* Sets the port register of SCL pin. */
            port_gr = RIICHS_CH0_SCL0_GP;
            pin_num = RIICHS_CH0_SCL0_PIN;
            ppcr = (uint8_t *)((uint32_t)RIICHS_PRV_PCR_BASE_REG + (uint32_t)port_gr);
            ppdr = (uint8_t *)((uint32_t)RIICHS_PRV_PDR_BASE_REG + (uint32_t)port_gr);
            (*ppcr) &= (~(1U << pin_num)); /* SCL0 input pull-up resister : off */
            (*ppdr) &= (~(RIICHS_IN << pin_num)); /* SCL0 input mode */

            /* Sets the port register of SDA pin. */
            port_gr = RIICHS_CH0_SDA0_GP;
            pin_num = RIICHS_CH0_SDA0_PIN;
            ppcr = (uint8_t *)((uint32_t)RIICHS_PRV_PCR_BASE_REG + (uint32_t)port_gr);
            ppdr = (uint8_t *)((uint32_t)RIICHS_PRV_PDR_BASE_REG + (uint32_t)port_gr);
            (*ppcr) &= (~(1U << pin_num)); /* SDA0 input pull-up resister : off */
            (*ppdr) &= (~(RIICHS_IN << pin_num)); /* SDA0 input mode */
            uctmp = (*ppdr); /* Reads PDR. */
        break;
        #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riichs_mcu_io_open() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_mpc_enable
 * Description  : Enables RIICHS multi-function pin controller.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_mpc_enable (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppmr = NULL;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppfs = NULL;
    uint8_t port_gr = 0;
    uint8_t pin_num = 0;

    /* The upper layer software should call "riichs_disable()." */
    /* The upper layer software should set SCL and SDA ports to input mode using PDR. */
    /* The upper layer software should perform RIICHS reset or internal reset. */

    /* Channel number? */
    switch (channel)
    {
        #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */
            port_gr = RIICHS_CH0_SCL0_GP;
            pin_num = RIICHS_CH0_SCL0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIICHS_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            port_gr = RIICHS_CH0_SDA0_GP;
            pin_num = RIICHS_CH0_SDA0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIICHS_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            /* Specifies the assignments of input/output signals for peripheral functions to the desired pins.
             But SCL and SDA are already in a high-impedance state.
             Because the upper layer software called "riichs_reset_set()". */

            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC); /* Enables the PFS register writing. */

            port_gr = RIICHS_CH0_SCL0_GP;
            pin_num = RIICHS_CH0_SCL0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIICHS_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIICHS_MPC_SCL0_ENABLE; /* Pin function select to RIICHS SCL pin. */

            port_gr = RIICHS_CH0_SDA0_GP;
            pin_num = RIICHS_CH0_SDA0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIICHS_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIICHS_MPC_SDA0_ENABLE; /* Pin function select to RIICHS SDA pin. */

            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC); /* Disables the PFS register writing. */

            port_gr = RIICHS_CH0_SCL0_GP;
            pin_num = RIICHS_CH0_SCL0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIICHS_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) |= (1U << pin_num); /* Uses as RIICHS (SCL). */
            uctmp = (*ppmr); /* Reads PMR. */

            port_gr = RIICHS_CH0_SDA0_GP;
            pin_num = RIICHS_CH0_SDA0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIICHS_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) |= (1U << pin_num); /* Uses as RIICHS (SDA). */
            uctmp = (*ppmr); /* Reads PMR. */
        break;
        #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riichs_mcu_mpc_enable() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_mpc_disable
 * Description  : Disables RIICHS multi-function pin controller.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_mpc_disable (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppmr = NULL;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppfs = NULL;
    uint8_t port_gr = 0;
    uint8_t pin_num = 0;

    /* The upper layer software should set SCL snd SDA ports to input mode using PDR. */

    /* Channel number? */
    switch (channel)
    {
        #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */
            port_gr = RIICHS_CH0_SCL0_GP;
            pin_num = RIICHS_CH0_SCL0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIICHS_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            port_gr = RIICHS_CH0_SDA0_GP;
            pin_num = RIICHS_CH0_SDA0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIICHS_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC); /* Enables the PFS register writing. */

            port_gr = RIICHS_CH0_SCL0_GP;
            pin_num = RIICHS_CH0_SCL0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIICHS_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIICHS_MPC_SCL0_INIT; /* Pin function select to Hi-Z. */

            port_gr = RIICHS_CH0_SDA0_GP;
            pin_num = RIICHS_CH0_SDA0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIICHS_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIICHS_MPC_SDA0_INIT; /* Pin function select to Hi-Z. */

            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC); /* Disables the PFS register writing. */
        break;
        #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
    uctmp = RIICHS_PWPR_BOWI; /* Reads PWPR. */
} /* End of function riichs_mcu_mpc_disable() */
    #endif

/***********************************************************************************************************************
 * Function Name: riichs_param_check
 * Description  : Checks NULL for pointer variable.
 * Arguments    : uint8_t channel
 * Return Value : true                   : pointer variable is not NULL.
 *                false                  : pointer variable is NULL.
 **********************************************************************************************************************/
bool riichs_param_check (riichs_info_t * p_riichs_info)
{
    bool ret = true;

    if (NULL == p_riichs_info)
    {
        ret = false;
    }

    return ret;
} /* End of function riichs_param_check() */

/***********************************************************************************************************************
 * Function Name: riichs_slave_address_check
 * Description  : Checks range of slave address.
 * Arguments    : uint8_t format
 *              : uint16_t address
 * Return Value : true                   : address is in range.
 *                false                  : address is out of range.
 **********************************************************************************************************************/
bool riichs_slave_address_check (uint8_t format, uint16_t address)
{
    bool ret = false;

    if (1U == format)
    {
        if (0x007FU >= address)
        {
            ret = true;
        }
    }
    else if (2U == format)
    {
        if (0x03FF >= address)
        {
            ret = true;
        }
    }
    else if (0U == format)
    {
        ret = true;
    }
    return ret;
} /* End of function riichs_slave_address_check() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_check_channel
 * Description  : Checks channel is valid or not.
 * Arguments    : uint8_t channel
 * Return Value : true                   : RIICHS channel is valid.
 *                false                  : RIICHS channel is invalid.
 **********************************************************************************************************************/
bool riichs_mcu_check_channel (uint8_t channel)
{
    bool ret = false;

    switch (channel)
    {

    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            ret = true;
        break;
    #endif

        default :

            /* The channel number is invalid. */
            ret = false;
        break;
    }

    return ret;
} /* End of function riichs_mcu_check_channel() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_int_init
 * Description  : Registers a callback function for supported TEI and EEI group interrupts.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_int_init (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            R_BSP_InterruptWrite(BSP_INT_SRC_AL1_RIICHS0_EEI, (bsp_int_cb_t) riichs0_eei_isr);
            R_BSP_InterruptWrite(BSP_INT_SRC_AL1_RIICHS0_TEI, (bsp_int_cb_t) riichs0_tei_isr);
        break;
    #endif

        default :

            /* Do Nothing */
        break;
    }
} /* End of function riichs_mcu_int_init() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_check_ir_txi
 * Description  : Check TXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : RIICHS_IR_CLR         : TXI Interrupt request (IR) register is cleared.
 *              : RIICHS_IR_SET         : TXI Interrupt request (IR) register is set.
 **********************************************************************************************************************/
uint8_t riichs_mcu_check_ir_txi (uint8_t channel)
{
    uint8_t ret = RIICHS_IR_CLR;

    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            ret = RIICHS_IR_TXI0;
        break;
    #endif

        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }

    return ret;
} /* End of function riichs_mcu_check_ir_txi() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_check_ir_rxi
 * Description  : Check RXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : RIICHS_IR_CLR         : TXI Interrupt request (IR) register is cleared.
 *              : RIICHS_IR_SET         : TXI Interrupt request (IR) register is set.
 **********************************************************************************************************************/
uint8_t riichs_mcu_check_ir_rxi (uint8_t channel)
{
    uint8_t ret = RIICHS_IR_CLR;

    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            ret = RIICHS_IR_RXI0;
        break;
    #endif

        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }

    return ret;
} /* End of function riichs_mcu_check_ir_rxi() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_clear_ir_txi
 * Description  : Clear TXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riichs_mcu_clear_ir_txi (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            RIICHS_IR_TXI0 = RIICHS_IR_CLR; /* Clears TXI interrupt request register. */
            if (RIICHS_IR_CLR == RIICHS_IR_TXI0)
            {
                /* Do Nothing */
            }
        break;
    #endif

        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }
} /* End of function riichs_mcu_clear_ir_txi() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_clear_ir_rxi
 * Description  : Clear RXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riichs_mcu_clear_ir_rxi (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            RIICHS_IR_RXI0 = RIICHS_IR_CLR; /* Clears TXI interrupt request register. */
            if (RIICHS_IR_CLR == RIICHS_IR_RXI0)
            {
                /* Do Nothing */
            }
        break;
    #endif

        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }
} /* End of function riichs_mcu_clear_ir_rxi() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_hardware_lock
 * Description  : Attempt to acquire the lock that has been sent in.
 * Arguments    : uint8_t channel
 * Return Value : true                   ; Successful operation
 *              : false                  ; error operation
 **********************************************************************************************************************/
bool riichs_mcu_hardware_lock (uint8_t channel)
{
    bool chk = false;

    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            chk = R_BSP_HardwareLock(BSP_LOCK_RIICHS0);
        break;
    #endif

        default :

            /* Do Nothing */
        break;
    }
    return chk;
} /* End of function riichs_mcu_hardware_lock() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_hardware_unlock
 * Description  : Release hold on lock.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riichs_mcu_hardware_unlock (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :
            R_BSP_HardwareUnlock(BSP_LOCK_RIICHS0);
        break;
    #endif

        default :

            /* Do Nothing */
        break;
    }
} /* End of function riichs_mcu_hardware_unlock() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_power_on
 * Description  : Turns on power to a RIICHS channel.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riichs_mcu_power_on (uint8_t channel)
{
    #if ((1U == RIICHS_CFG_CH0_INCLUDED))
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
    #endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Enable selected RIICHS Channel. */
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :

        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif

            /* Bring module out of stop state. */
            MSTP(RIICHS0) = 0U;

        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif
        break;
    #endif

        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }

    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
} /* End of function riichs_mcu_power_on() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_power_off
 * Description  : Turns off power to a RIICHS channel.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riichs_mcu_power_off (uint8_t channel)
{
    #if ((1U == RIICHS_CFG_CH0_INCLUDED))
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
    #endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Disable selected CMT Channel. */
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 :

        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            
            /* Put module in stop state. */
            MSTP(RIICHS0) = 1U;
            
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif
        break;
    #endif

        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }

    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
} /* End of function riichs_mcu_power_off() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_int_icsier_iccsier_setting
 * Description  : Time out interrupt enable bit setting.
 * Arguments    : uint8_t channel
 *              : uint32_t New_icsier       ; New ICSIER value
 *              : uint32_t New_iccsier      ; New ICCSIER value
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_int_icsier_iccsier_setting (uint8_t channel, uint32_t New_icsier,uint32_t New_iccsier)
{
    volatile uint32_t uctmp = 0x00000000;

    /* Channel number? */
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */
        if (1U == g_riichs_timeout_enable[0])
        {
            RIICHS0.ICSIER.LONG = (New_icsier | RIICHS_ICSIER_TMO);
            RIICHS0.ICCSIER.LONG = New_iccsier;
            uctmp = RIICHS0.ICSIER.LONG; /* Reads ICSIER. */
            uctmp = RIICHS0.ICCSIER.LONG; /* Reads ICCSIER. */
        }

        else
        {
            RIICHS0.ICSIER.LONG = New_icsier;
            RIICHS0.ICCSIER.LONG = New_iccsier;
            uctmp = RIICHS0.ICSIER.LONG; /* Reads ICSIER. */
            uctmp = RIICHS0.ICCSIER.LONG; /* Reads ICCSIER. */
        }

        break;
    #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riichs_mcu_int_icsier_iccsier_setting() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_int_enable
 * Description  : Clears interrupt request register.
 *              : Enables interrupt.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_int_enable (uint8_t channel)
{
    #if ((1U == RIICHS_CFG_CH0_INCLUDED))
    volatile uint8_t uctmp = 0x00;
    volatile bsp_int_ctrl_t group_priority;
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
    group_priority.ipl = 0x00000000;

        #if (1U == RIICHS_CFG_CH0_INCLUDED)
    if (group_priority.ipl < g_riichs_eei_int_priority[0])
    {
        group_priority.ipl = (uint32_t) g_riichs_eei_int_priority[0];
    }
    if (group_priority.ipl < g_riichs_tei_int_priority[0])
    {
        group_priority.ipl = (uint32_t) g_riichs_tei_int_priority[0];
    }
        #endif

    R_BSP_InterruptControl(BSP_INT_SRC_AL1_RIICHS0_TEI, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *) &group_priority);
    #endif

    /* Channel number? */
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */

            /* Enables interrupt. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            RIICHS_IER_EEI0_GPAL1 = RIICHS_IER_ENABLE; /* Enables EEI0 groupAL1 interrupt request enable register. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif
            RIICHS_IER_RXI0 = RIICHS_IER_ENABLE; /* Enables RXI0 interrupt request enable register. */
            RIICHS_IER_TXI0 = RIICHS_IER_ENABLE; /* Enables TXI0 interrupt request enable register. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            RIICHS_IER_TEI0_GPAL1 = RIICHS_IER_ENABLE; /* Enables TEI0 groupAL1 interrupt request enable register. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif

            /* Sets interrupt source priority. */
            RIICHS_IPR_RXI0 = g_riichs_rxi_int_priority[0]; /* Sets RXI0 interrupt source priority register. */
            RIICHS_IPR_TXI0 = g_riichs_txi_int_priority[0]; /* Sets TXI0 interrupt source priority register. */
            uctmp = RIICHS_IPR_TXI0; /* Reads IPR. */
        break;
    #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riichs_mcu_int_enable() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_int_disable
 * Description  : Disables interrupt.
 *              : Sets interrupt source priority.
 *              : Clears interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riichs_mcu_int_disable (uint8_t channel)
{
    #if ((1U == RIICHS_CFG_CH0_INCLUDED))
    volatile uint8_t uctmp = 0x00;
    volatile bsp_int_ctrl_t group_priority;
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
    #endif

    /* Channel number? */
    switch (channel)
    {
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */

            /* Disables interrupt. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            RIICHS_IER_EEI0_GPAL1 = RIICHS_IER_DISABLE; /* Disables TEI0 groupAL1 interrupt request enable register. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif
            RIICHS_IER_RXI0 = RIICHS_IER_DISABLE; /* Disables RXI0 interrupt request enable register. */
            RIICHS_IER_TXI0 = RIICHS_IER_DISABLE; /* Disables TXI0 interrupt request enable register. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            RIICHS_IER_TEI0_GPAL1 = RIICHS_IER_DISABLE; /* Disables TEI0 groupAL1 interrupt request enable register. */
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif

            /* Sets interrupt source priority. */
            RIICHS_IPR_RXI0 = g_riichs_rxi_int_priority[0]; /* Sets RXI0 interrupt source priority register. */
            RIICHS_IPR_TXI0 = g_riichs_txi_int_priority[0]; /* Sets TXI0 interrupt source priority register. */
            uctmp = RIICHS_IPR_TXI0; /* Reads IPR. */
        break;
    #endif

        default :

            /* Please add a channel as needed. */
        break;
    }

    group_priority.ipl = 0x00000000;
    R_BSP_InterruptControl(BSP_INT_SRC_AL1_RIICHS0_TEI, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, (void *) &group_priority);
} /* End of function riichs_mcu_int_disable() */

/***********************************************************************************************************************
 * Function Name: riichs_mcu_check_freq
 * Description  : check pclk frequency and return that value.
 * Arguments    : None
 * Return Value : PCLK frequency
 **********************************************************************************************************************/
double riichs_mcu_check_freq (void)
{
    return BSP_PCLKA_HZ;
} /* End of function riichs_mcu_check_freq() */

    #if (1U == RIICHS_CFG_CH0_INCLUDED)
/***********************************************************************************************************************
 * Function Name: riichs0_eei_isr
 * Description  : Interrupt EEI handler for channel 0.
 *                Types of interrupt requests transfer error or event generation.
 *                The event generations are arbitration-lost, NACK detection, timeout detection, 
 *                start condition detection, and stop condition detection.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void riichs0_eei_isr (void * vect)
{
    riichs0_eei_sub();
} /* End of function riichs0_eei_isr() */

/***********************************************************************************************************************
 * Function Name: riichs0_txi_isr
 * Description  : Interrupt TXI handler for channel 0.
 *                Types of interrupt requests transmission data empty.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(riichs0_txi_isr, VECT(RIICHS0, TXI))
R_BSP_ATTRIB_STATIC_INTERRUPT void riichs0_txi_isr (void)
{
    riichs0_txi_sub();
} /* End of function riichs0_txi_isr() */

/***********************************************************************************************************************
 * Function Name: riichs0_rxi_isr
 * Description  : Interrupt RXI handler for channel 0.
 *                Types of interrupt requests reception end.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(riichs0_rxi_isr, VECT(RIICHS0, RXI))
R_BSP_ATTRIB_STATIC_INTERRUPT void riichs0_rxi_isr (void)
{
    riichs0_rxi_sub();
} /* End of function riichs0_rxi_isr() */

/***********************************************************************************************************************
 * Function Name: riichs0_tei_isr
 * Description  : Interrupt TEI handler for channel 0.
 *                Types of interrupt requests transmission end.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void riichs0_tei_isr (void * vect)
{
    riichs0_tei_sub();
} /* End of function riichs0_tei_isr() */
    #endif

#endif /* defined(BSP_MCU_RX671) */

