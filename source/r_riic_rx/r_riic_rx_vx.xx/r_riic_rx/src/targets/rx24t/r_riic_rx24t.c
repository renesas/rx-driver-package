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
 * Copyright (C) 2016(2019) Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riic_rx24t.c
 * Description  : Functions for using RIIC on RX devices. 
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 04.03.2016 1.90     First Release
 *         : 01.10.2016 2.00     Fixed a program according to the Renesas coding rules.
 *         : 02.06.2017 2.10     Changed include path becuase changed file structure.
 *                               Changed about the calculation processing for address of PFS, PCR, 
 *                               PDR and PMR register.
 *         : 20.05.2019 2.41     Added support for GNUC and ICCRX.
 *                               Fixed coding style.
 *         : 10.10.2019 2.44     Added support for atomic control.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Defines for RIIC support */
#include "r_riic_rx_if.h"

/* Check MCU Group */
#if defined(BSP_MCU_RX24T)

    #include "r_riic_rx_private.h"

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
/* Array of bit rate for RIICn (n=0) */
const uint16_t g_riic_bps[] =
{
    (uint16_t) RIIC_CFG_CH0_kBPS
};

/* Array of data of Slave Address 0 Register (SARL0 and SARU0) for RIICn (n=0) */
const uint16_t g_riic_slv_ad0_val[] =
{
    RIIC_CFG_CH0_SLV_ADDR0
};

/* Array of data of Slave Address 1 Register (SARL1 and SARU1) for RIICn (n=0) */
const uint16_t g_riic_slv_ad1_val[] =
{
    RIIC_CFG_CH0_SLV_ADDR1
};

/* Array of data of Slave Address 2 Register (SARL2 and SARU2) for RIICn (n=0) */
const uint16_t g_riic_slv_ad2_val[] =
{
    RIIC_CFG_CH0_SLV_ADDR2
};

/* Array of slave address 0 format for RIICn (n=0) */
const uint8_t g_riic_slv_ad0_format[] =
{
    RIIC_CFG_CH0_SLV_ADDR0_FORMAT
};

/* Array of slave address 1 format for RIICn (n=0) */
const uint8_t g_riic_slv_ad1_format[] =
{
    RIIC_CFG_CH0_SLV_ADDR1_FORMAT
};

/* Array of slave address 2 format for RIICn (n=0) */
const uint8_t g_riic_slv_ad2_format[] =
{
    RIIC_CFG_CH0_SLV_ADDR2_FORMAT
};

/* Array of general call address enable for RIICn (n=0) */
const uint8_t g_riic_gca_enable[] =
{
    RIIC_CFG_CH0_SLV_GCA_ENABLE
};

/* Array of initialize data of I2C-bus Mode Register 2 (ICMR2) for RIICn (n=0) */
const uint8_t g_riic_icmr2_init[] =
{
    RIIC_CH0_ICMR2_INIT
};

/* Array of initialize data of I2C-bus Mode Register 3 (ICMR3) for RIICn (n=0) */
const uint8_t g_riic_icmr3_init[] =
{
    RIIC_CH0_ICMR3_INIT
};

/* Array of initialize data of I2C-bus Function Enable Register (ICFER) for RIICn (n=0) */
const uint8_t g_riic_icfer_init[] =
{
    RIIC_CH0_ICFER_INIT
};

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/
    #if (1U == RIIC_CFG_PORT_SET_PROCESSING)
/***********************************************************************************************************************
 * Function Name: riic_mcu_io_open
 * Description  : Sets ports to input mode.
 *              : Ports input pull-up becomes "Off".
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_io_open (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppcr = NULL;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppdr = NULL;
    uint8_t port_gr = 0;
    uint8_t pin_num = 0;

    /* Channel number? */
    switch (channel)
    {
        #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */

            /* Sets the port register of SCL pin. */
            port_gr = RIIC_CH0_SCL0_GP;
            pin_num = RIIC_CH0_SCL0_PIN;
            ppcr = (uint8_t *)((uint32_t)RIIC_PRV_PCR_BASE_REG + (uint32_t)port_gr);
            ppdr = (uint8_t *)((uint32_t)RIIC_PRV_PDR_BASE_REG + (uint32_t)port_gr);
            (*ppcr) &= (~(1U << pin_num)); /* SCL0 input pull-up resister : off */
            (*ppdr) &= (~(RIIC_IN << pin_num)); /* SCL0 input mode */

            /* Sets the port register of SDA pin. */
            port_gr = RIIC_CH0_SDA0_GP;
            pin_num = RIIC_CH0_SDA0_PIN;
            ppcr = (uint8_t *)((uint32_t)RIIC_PRV_PCR_BASE_REG + (uint32_t)port_gr);
            ppdr = (uint8_t *)((uint32_t)RIIC_PRV_PDR_BASE_REG + (uint32_t)port_gr);
            (*ppcr) &= (~(1U << pin_num)); /* SDA0 input pull-up resister : off */
            (*ppdr) &= (~(RIIC_IN << pin_num)); /* SDA0 input mode */
            uctmp = (*ppdr); /* Reads PDR. */
        break;
        #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riic_mcu_io_open() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_mpc_enable
 * Description  : Enables RIIC multi-function pin controller.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_mpc_enable (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppmr = NULL;
    R_BSP_VOLATILE_EVENACCESS uint8_t * ppfs = NULL;
    uint8_t port_gr = 0;
    uint8_t pin_num = 0;

    /* The upper layer software should call "riic_disable()." */
    /* The upper layer software should set SCL and SDA ports to input mode using PDR. */
    /* The upper layer software should perform RIIC reset or internal reset. */

    /* Channel number? */
    switch (channel)
    {
        #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */
            port_gr = RIIC_CH0_SCL0_GP;
            pin_num = RIIC_CH0_SCL0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIIC_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            port_gr = RIIC_CH0_SDA0_GP;
            pin_num = RIIC_CH0_SDA0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIIC_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            /* Specifies the assignments of input/output signals for peripheral functions to the desired pins.
             But SCL and SDA are already in a high-impedance state.
             Because the upper layer software called "riic_reset_set()". */

            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC); /* Enables the PFS register writing. */

            port_gr = RIIC_CH0_SCL0_GP;
            pin_num = RIIC_CH0_SCL0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIIC_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIIC_MPC_SCL0_ENABLE; /* Pin function select to RIIC SCL pin. */

            port_gr = RIIC_CH0_SDA0_GP;
            pin_num = RIIC_CH0_SDA0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIIC_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIIC_MPC_SDA0_ENABLE; /* Pin function select to RIIC SDA pin. */

            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC); /* Disables the PFS register writing. */

            port_gr = RIIC_CH0_SCL0_GP;
            pin_num = RIIC_CH0_SCL0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIIC_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) |= (1U << pin_num); /* Uses as RIIC (SCL). */
            uctmp = (*ppmr); /* Reads PMR. */

            port_gr = RIIC_CH0_SDA0_GP;
            pin_num = RIIC_CH0_SDA0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIIC_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) |= (1U << pin_num); /* Uses as RIIC (SDA). */
            uctmp = (*ppmr); /* Reads PMR. */
        break;
        #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riic_mcu_mpc_enable() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_mpc_disable
 * Description  : Disables RIIC multi-function pin controller.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_mpc_disable (uint8_t channel)
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
        #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */
            port_gr = RIIC_CH0_SCL0_GP;
            pin_num = RIIC_CH0_SCL0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIIC_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            port_gr = RIIC_CH0_SDA0_GP;
            pin_num = RIIC_CH0_SDA0_PIN;
            ppmr = (uint8_t *)((uint32_t)RIIC_PRV_PMR_BASE_REG + (uint32_t)port_gr);
            (*ppmr) &= (~(1U << pin_num)); /* Uses as a GPIO (Input port). */

            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC); /* Enables the PFS register writing. */

            port_gr = RIIC_CH0_SCL0_GP;
            pin_num = RIIC_CH0_SCL0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIIC_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIIC_MPC_SCL0_INIT; /* Pin function select to Hi-Z. */

            port_gr = RIIC_CH0_SDA0_GP;
            pin_num = RIIC_CH0_SDA0_PIN;
            ppfs = (uint8_t *)((uint32_t)RIIC_PRV_PFS_BASE_REG + (uint32_t)((port_gr * 8) + pin_num));
            (*ppfs) = RIIC_MPC_SDA0_INIT; /* Pin function select to Hi-Z. */

            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC); /* Disables the PFS register writing. */
        break;
        #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
    uctmp = RIIC_PWPR_BOWI; /* Reads PWPR. */
} /* End of function riic_mcu_mpc_disable() */
    #endif

/***********************************************************************************************************************
 * Function Name: riic_mcu_check_channel
 * Description  : Checks channel is valid or not.
 * Arguments    : uint8_t channel
 * Return Value : true                   : RIIC channel is valid.
 *                false                  : RIIC channel is invalid.
 **********************************************************************************************************************/
bool riic_mcu_check_channel (uint8_t channel)
{
    bool ret = false;

    switch (channel)
    {

    #if (1U == RIIC_CFG_CH0_INCLUDED)
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
} /* End of function riic_mcu_check_channel() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_hardware_lock
 * Description  : Attempt to acquire the lock that has been sent in.
 * Arguments    : uint8_t channel
 * Return Value : true                   ; Successful operation
 *              : false                  ; error operation
 **********************************************************************************************************************/
bool riic_mcu_hardware_lock (uint8_t channel)
{
    bool chk = false;

    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
            chk = R_BSP_HardwareLock(BSP_LOCK_RIIC0);
        break;
    #endif

        default :

            /* Do Nothing */
        break;
    }
    return chk;
} /* End of function riic_mcu_hardware_lock() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_hardware_unlock
 * Description  : Release hold on lock.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riic_mcu_hardware_unlock (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
            R_BSP_HardwareUnlock(BSP_LOCK_RIIC0);
        break;
    #endif

        default :

            /* Do Nothing */
        break;
    }
} /* End of function riic_mcu_hardware_unlock() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_power_on
 * Description  : Turns on power to a RIIC channel.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riic_mcu_power_on (uint8_t channel)
{
    #if (1U == RIIC_CFG_CH0_INCLUDED)
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
    #endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Enable selected RIIC Channel. */
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
        
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
        
            /* Bring module out of stop state. */
            MSTP(RIIC0) = 0U;
        
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
} /* End of function riic_mcu_power_on() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_power_off
 * Description  : Turns off power to a RIIC channel.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riic_mcu_power_off (uint8_t channel)
{
    #if (1U == RIIC_CFG_CH0_INCLUDED)
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
    #endif
    #endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Disable selected CMT Channel. */
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
        
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
        
            /* Put module in stop state. */
            MSTP(RIIC0) = 1U;
        
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
} /* End of function riic_mcu_power_off() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_int_init
 * Description  : Registers a callback function for supported TEI and EEI group interrupts.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_int_init (uint8_t channel)
{
    /* Do Nothing */
} /* End of function riic_mcu_int_init() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_check_ir_txi
 * Description  : Check TXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : RIIC_IR_CLR         : TXI Interrupt request (IR) register is cleared.
 *              : RIIC_IR_SET         : TXI Interrupt request (IR) register is set.
 **********************************************************************************************************************/
uint8_t riic_mcu_check_ir_txi (uint8_t channel)
{
    uint8_t ret = RIIC_IR_CLR;

    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
            ret = RIIC_IR_TXI0;
        break;
    #endif
        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }

    return ret;
} /* End of function riic_mcu_check_ir_txi() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_check_ir_rxi
 * Description  : Check RXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : RIIC_IR_CLR         : TXI Interrupt request (IR) register is cleared.
 *              : RIIC_IR_SET         : TXI Interrupt request (IR) register is set.
 **********************************************************************************************************************/
uint8_t riic_mcu_check_ir_rxi (uint8_t channel)
{
    uint8_t ret = RIIC_IR_CLR;

    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
            ret = RIIC_IR_RXI0;
        break;
    #endif
        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }

    return ret;
} /* End of function riic_mcu_check_ir_rxi() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_clear_ir_txi
 * Description  : Clear TXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riic_mcu_clear_ir_txi (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
            RIIC_IR_TXI0 = RIIC_IR_CLR; /* Clears TXI interrupt request register. */
            if (RIIC_IR_CLR == RIIC_IR_TXI0)
            {
                /* Do Nothing */
            }
        break;
    #endif
        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }
} /* End of function riic_mcu_clear_ir_txi() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_clear_ir_rxi
 * Description  : Clear RXI interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : none
 **********************************************************************************************************************/
void riic_mcu_clear_ir_rxi (uint8_t channel)
{
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 :
            RIIC_IR_RXI0 = RIIC_IR_CLR; /* Clears TXI interrupt request register. */
            if (RIIC_IR_CLR == RIIC_IR_RXI0)
            {
                /* Do Nothing */
            }
        break;
    #endif
        default :

            /* Should never get here. Valid channel number is checked above. */
        break;
    }
} /* End of function riic_mcu_clear_ir_rxi() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_int_icier_setting
 * Description  : Time out interrupt enable bit setting.
 * Arguments    : uint8_t channel
 *              : uint8_t New_icier       ; New ICIER value
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_int_icier_setting (uint8_t channel, uint8_t New_icier)
{
    volatile uint8_t uctmp = 0x00;

    /* Channel number? */
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */
        #if (1U == RIIC_CFG_CH0_TMO_ENABLE)
            RIIC0.ICIER.BYTE = (New_icier | RIIC_ICIER_TMO);
            uctmp = RIIC0.ICIER.BYTE; /* Reads ICIER. */
        #else
            RIIC0.ICIER.BYTE = New_icier;
            uctmp = RIIC0.ICIER.BYTE; /* Reads ICIER. */
        #endif
        break;
    #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riic_mcu_int_icier_setting() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_int_enable
 * Description  : Clears interrupt request register.
 *              : Enables interrupt.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_int_enable (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;

    /* Channel number? */
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0x00 : /* Channel 0 */

            /* Enables interrupt. */
            RIIC_IER_EEI0 = RIIC_IER_ENABLE; /* Enables EEI0 interrupt request enable register. */
            RIIC_IER_RXI0 = RIIC_IER_ENABLE; /* Enables RXI0 interrupt request enable register. */
            RIIC_IER_TXI0 = RIIC_IER_ENABLE; /* Enables TXI0 interrupt request enable register. */
            RIIC_IER_TEI0 = RIIC_IER_ENABLE; /* Enables TEI0 interrupt request enable register. */

            /* Sets interrupt source priority. */
            RIIC_IPR_EEI0 = RIIC_IPR_CH0_EEI_SET; /* Sets EEI0 interrupt source priority register. */
            RIIC_IPR_RXI0 = RIIC_IPR_CH0_RXI_SET; /* Sets RXI0 interrupt source priority register. */
            RIIC_IPR_TXI0 = RIIC_IPR_CH0_TXI_SET; /* Sets TXI0 interrupt source priority register. */
            RIIC_IPR_TEI0 = RIIC_IPR_CH0_TEI_SET; /* Sets TEI0 interrupt source priority register. */
            uctmp = RIIC_IER_TEI0; /* Reads IER. */
        break;
    #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riic_mcu_int_enable() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_int_disable
 * Description  : Disables interrupt.
 *              : Sets interrupt source priority.
 *              : Clears interrupt request register.
 * Arguments    : uint8_t channel
 * Return Value : None
 **********************************************************************************************************************/
void riic_mcu_int_disable (uint8_t channel)
{
    volatile uint8_t uctmp = 0x00;

    /* Channel number? */
    switch (channel)
    {
    #if (1U == RIIC_CFG_CH0_INCLUDED)
        case 0U : /* Channel 0 */

            /* Disables interrupt. */
            RIIC_IER_EEI0 = RIIC_IER_DISABLE; /* Disables EEI0 interrupt request enable register. */
            RIIC_IER_RXI0 = RIIC_IER_DISABLE; /* Disables RXI0 interrupt request enable register. */
            RIIC_IER_TXI0 = RIIC_IER_DISABLE; /* Disables TXI0 interrupt request enable register. */
            RIIC_IER_TEI0 = RIIC_IER_DISABLE; /* Disables TEI0 interrupt request enable register. */

            /* Sets interrupt source priority. */
            RIIC_IPR_EEI0 = RIIC_IPR_CH0_EEI_INIT; /* Sets EEI0 interrupt source priority register. */
            RIIC_IPR_RXI0 = RIIC_IPR_CH0_RXI_INIT; /* Sets RXI0 interrupt source priority register. */
            RIIC_IPR_TXI0 = RIIC_IPR_CH0_TXI_INIT; /* Sets TXI0 interrupt source priority register. */
            RIIC_IPR_TEI0 = RIIC_IPR_CH0_TEI_INIT; /* Sets TEI0 interrupt source priority register. */
            uctmp = RIIC_IPR_TEI0; /* Reads IPR. */
        break;
    #endif

        default :

            /* Please add a channel as needed. */
        break;
    }
} /* End of function riic_mcu_int_disable() */

/***********************************************************************************************************************
 * Function Name: riic_mcu_check_freq
 * Description  : check pclk frequency and return that value.
 * Arguments    : None
 * Return Value : PCLK frequency
 **********************************************************************************************************************/
double riic_mcu_check_freq (void)
{
    return BSP_PCLKB_HZ;
} /* End of function riic_mcu_check_freq() */

    #if (1U == RIIC_CFG_CH0_INCLUDED)
/***********************************************************************************************************************
 * Function Name: riic0_eei_isr
 * Description  : Interrupt EEI handler for channel 0.
 *                Types of interrupt requests transfer error or event generation.
 *                The event generations are arbitration-lost, NACK detection, timeout detection, 
 *                start condition detection, and stop condition detection.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(riic0_eei_isr, VECT(RIIC0, EEI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void riic0_eei_isr (void)
{
    riic0_eei_sub();
} /* End of function riic0_eei_isr() */

/***********************************************************************************************************************
 * Function Name: riic0_txi_isr
 * Description  : Interrupt TXI handler for channel 0.
 *                Types of interrupt requests transmission data empty.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(riic0_txi_isr, VECT(RIIC0, TXI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void riic0_txi_isr (void)
{
    riic0_txi_sub();
} /* End of function riic0_txi_isr() */

/***********************************************************************************************************************
 * Function Name: riic0_rxi_isr
 * Description  : Interrupt RXI handler for channel 0.
 *                Types of interrupt requests reception end.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(riic0_rxi_isr, VECT(RIIC0, RXI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void riic0_rxi_isr (void)
{
    riic0_rxi_sub();
} /* End of function riic0_rxi_isr() */

/***********************************************************************************************************************
 * Function Name: riic0_tei_isr
 * Description  : Interrupt TEI handler for channel 0.
 *                Types of interrupt requests transmission end.
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(riic0_tei_isr, VECT(RIIC0, TEI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void riic0_tei_isr (void)
{
    riic0_tei_sub();
} /* End of function riic0_tei_isr() */
    #endif

#endif /* defined(BSP_MCU_RX24T) */

