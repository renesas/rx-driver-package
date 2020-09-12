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
* Copyright (C) 2013-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_mpc_rx.c
* Description  : Multi-Function Pin Controller (MPC) driver. This module uses pin definitions from the r_gpio_rx module
*                to allow users to easily control pin functionality.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 17.07.2013 1.00    First Release
*         : 24.04.2014 1.20    Changed method of calculating PFS base register to make it compatible with more MCUs
*         : 13.06.2014 1.30    Removed unused pfs_reg_t typedef
*         : 04.12.2014 1.40    Added support for RX113
*         : 19.12.2016 1.50    Added support for RX24U, RX24T(512KB)
*         : 21.07.2017 2.30    Added support for RX65N-2M, RX130-512KB
*         : 28.09.2018 2.40    Added support for RX66T
*                              Update according to GSCE Code Checker
*         : 01.02.2019 2.50    Added support for RX72T, RX65N-64pin
*         : 20.05.2019 3.00    Added support for GNUC and ICCRX.
*         : 28.06.2019 3.10    Added support RX23W
*         : 15.08.2019 3.20    Added support RX72M
*         : 25.11.2019 3.30    Added support RX13T
*                              Modified comment of API function to Doxygen style
*         : 30.12.2019 3.40    Added support RX72N, RX66N
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Public interface header file for this package. */
#include "r_mpc_rx_if.h"
/* Configuration for this package. */
#include "r_mpc_rx_config.h"

/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
/* Base register of PFS used to calculate all PFS register addresses. This is constant for all supported MCUs */
#define     MPC_PRV_PFS_BASE_REG    ((uint8_t volatile *)(&MPC.PWPR.BYTE+33))

/* Bit masks for PFS registers. */
#define     MPC_PRV_PFS_BIT_ASEL            ((uint8_t)0x80)
#define     MPC_PRV_PFS_BIT_ISEL            ((uint8_t)0x40)
#define     MPC_PRV_PFS_BIT_PSEL            ((uint8_t)0x3F)

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static uint8_t volatile * mpc_base_addr_get(uint8_t volatile * base_addr, uint16_t index);

/***********************************************************************************************************************
* Function Name: R_MPC_Read
********************************************************************************************************************//**
* @brief This function reads the function configuration of a pin.
* @param[in] pin - Which pin to read configuration information for.
* @param[in] pconfig - Pointer to structure where pin configuration information will be stored. See Section 2.9.1, MPC
* Pin Configuration.
* @details This function will read the configuration information for a pin and store it in a structure supplied by the
* user.
* @note None.
*/
void R_MPC_Read (gpio_port_pin_t pin, mpc_config_t * pconfig)
{
    uint8_t volatile * pfs_reg;

    /* Get address of PFS register for this pin. */
    pfs_reg = mpc_base_addr_get(MPC_PRV_PFS_BASE_REG, (uint16_t)pin);

    /* Fill in pin info. */
    if (0 == ((*pfs_reg) & MPC_PRV_PFS_BIT_ASEL))
    {
        pconfig->analog_enable = false;
    }
    else
    {
        pconfig->analog_enable = true;
    }

    /* Fill in irq info */
    if (0 == ((*pfs_reg) & MPC_PRV_PFS_BIT_ISEL))
    {
        pconfig->irq_enable = false;
    }
    else
    {
        pconfig->irq_enable = true;
    }

    /* Fill in pin function info */
    pconfig->pin_function = (uint8_t)((*pfs_reg) & MPC_PRV_PFS_BIT_PSEL);
} /* End of function R_MPC_Read() */

/***********************************************************************************************************************
* Function Name: R_MPC_Write
********************************************************************************************************************//**
* @brief This function sets the function of a pin.
* @param[in] pin - Which pin to configure.
* @param[in] pconfig - Pointer to structure with pin configuration information. See section 2.9.1, MPC Pin
* Configuration.
* @retval [MPC_SUCCESS] Successful; pin configured.
* @retval [MPC_ERR_INVALID_CFG] Error; invalid configuration input.
* @details This function will configure a pin based on the information in the mpc_config_t structure. Not all pins
* support the same functionality. For example, not all pins are able to be configured as analog pins for ADC or DAC use.
* Also, not all combinations of functionality are capable. For example, a pin cannot be configured as an analog pin and
* for peripheral use at the same time.
* To see what functions are available for a pin, refer to the Multi-Function Pin Controller (MPC) section of your MCU’s
* hardware manual. Select the Pin Function Control Register for the port that your pin is on. On this page you will
* find a table with available functions for each pin on the selected port.
* Which pin is to be configured by this function is defined using the gpio_port_pin_t type from the r_gpio_rx module.
* @note None.
*/
mpc_err_t R_MPC_Write (gpio_port_pin_t pin, mpc_config_t * pconfig)
{
    uint8_t volatile * pfs_reg;
    uint8_t            write_value;

#if (MPC_CFG_PARAM_CHECKING_ENABLE == 1)
    if (true == pconfig->analog_enable)
    {
        if ((true == pconfig->irq_enable) || (0 != pconfig->pin_function))
        {
            return MPC_ERR_INVALID_CFG;
        }
    }
#endif


    /* Get address of PFS register for this pin. */
    pfs_reg = mpc_base_addr_get(MPC_PRV_PFS_BASE_REG, (uint16_t)pin);

    write_value = 0;

    /* Fill in pin info. */
    if (true == pconfig->analog_enable)
    {
        /* Fill in analog pin info */
        write_value = MPC_PRV_PFS_BIT_ASEL;
    }

    if (true == pconfig->irq_enable)
    {
        /* Fill in irq info */
        write_value = (uint8_t)(write_value | MPC_PRV_PFS_BIT_ISEL);
    }

    /* Fill in pin function info */
    write_value = (uint8_t)(write_value | (pconfig->pin_function & MPC_PRV_PFS_BIT_PSEL));

    /* Enable writing to MPC registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    *pfs_reg = write_value;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

    return MPC_SUCCESS;
} /* End of function R_MPC_Write() */

/***********************************************************************************************************************
* Function Name: R_MPC_GetVersion
********************************************************************************************************************//**
* @brief Returns the current version of this API.
* @return  Version of this API.
* @details This function will return the version of the currently running API. The version number is encoded where
* the top 2 bytes are the major version number and the bottom 2 bytes are the minor version number. For example,
* Version 4.25 would be returned as 0x00040019.
* @note None.
*/
uint32_t R_MPC_GetVersion (void)
{
    /* These version macros are defined in r_mpc_rx_if.h. */
    return ((((uint32_t)MPC_RX_VERSION_MAJOR) << 16) | (uint32_t)MPC_RX_VERSION_MINOR);
} /* End of function R_MPC_GetVersion() */

/***********************************************************************************************************************
* Function Name: mpc_base_addr_get
* Description  : Get the address of a PFS register.
* Arguments    : base_addr -
*                    First port register of this type (e.g. P00PFS)
*                index -
*                    Index off the base. (e.g. for PORT4 it would be 0x0400)
* Return Value : Address of the register that was requested
***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(mpc_base_addr_get)
uint8_t volatile * mpc_base_addr_get (uint8_t volatile * base_addr, uint16_t index)
{
    uint32_t port_offset;
    uint32_t pin_offset;

    /* Pin is the lower 8-bits of 'index' */
    pin_offset =  (uint32_t)(index & 0x00FFu);

    /* Port is the upper 8-bits of 'index'. It is only being shifted by 5 because in the calculation the value is
     * multiplied by 8 (which would result in a shift of 3 in the other direction). To save a couple of cycles
     * the shift is adjusted here. */
    port_offset = (((uint32_t)index >> 5) & 0x000000FFuL);

    /* Add port and pin offset to 'index' to correct register. */
    return (uint8_t volatile *)(pin_offset + port_offset + (uint32_t)base_addr);
} /* End of function mpc_base_addr_get() */

