/**********************************************************************************************************************
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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdhi_dev.c
* Version      : 2.10
* Device       : RX671
* Abstract     : API & Sub module
* Tool-Chain   : For RX671 Group e2_studio
* OS           : not use
* H/W Platform : RSK board for RX671
* Description  : Interface file for SDHI API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 30.06.2021 2.07    First Release
*              : 27.12.2022 2.10    Updated slash format of included header file paths for Linux compatibility.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "platform.h"
#if defined(BSP_MCU_RX671)

#include "r_sdhi_rx_if.h"
#include "./src/r_sdhi_rx_private.h"
#include "./src/targets/rx671/r_sdhi_rx_target.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/

/**********************************************************************************************************************
* Outline      : SD Buffer Access Interrupt Handler
* Function Name: r_sdhi_dev_sbfai_isr
* Description  : Call a callback function.
* Arguments    : None
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(r_sdhi_dev_sbfai_isr, VECT(SDHI, SBFAI))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_sdhi_dev_sbfai_isr(void)
{
    sdhi_sdhndl_t   * p_hndl = 0;
    uint32_t channel = 0;

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    if (p_hndl->int_sdbuff_callback)
    {
        (*p_hndl->int_sdbuff_callback)(0);
    }
} /* End of function r_sdhi_dev_sbfai_isr() */

/**********************************************************************************************************************
* Outline      : Initialize SDHI
* Function Name: r_sdhi_dev_init
* Description  : Initializes hardware to access SDHI.
* Arguments    : uint32_t           channel              ;   SDHI Channel No.
* Return Value : SDHI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdhi_status_t r_sdhi_dev_init(uint32_t channel)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    MSTP_SDHI = 0;              /* SDHI ON */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    if (0 == MSTP_SDHI)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    if (SDHI_CH0 == channel)    /* SDHI_CH0 */
    {
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_SDHI_CDETI, R_SDHI_IntHandler0);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_SDHI_CACI, R_SDHI_IntHandler0);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_SDHI_SDACI, R_SDHI_IntHandler0);
    }

    return SDHI_SUCCESS;
} /* End of function r_sdhi_dev_init() */

/**********************************************************************************************************************
* Outline      : Finalize SDHI Host IP
* Function Name: r_sdhi_dev_finalize
* Description  : Finalizes to access SDHI.
* Arguments    : uint32_t           channel              ;   SDHI Channel No.
* Return Value : SDHI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdhi_status_t r_sdhi_dev_finalize(uint32_t channel)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    MSTP_SDHI = 1;              /* SDHI OFF */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    if (1 == MSTP_SDHI)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    if (SDHI_CH0 == channel)    /* SDHI_CH0 */
    {
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_SDHI_CDETI, FIT_NO_FUNC);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_SDHI_CACI, FIT_NO_FUNC);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_SDHI_SDACI, FIT_NO_FUNC);
    }

    return SDHI_SUCCESS;
} /* End of function r_sdhi_dev_finalize() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_DisableIcuInt
 *****************************************************************************************************************/ /**
 * @brief Disables ICU controller interrupts for the SDHI.\n 
 *        note : The following interrupt is disabled. [SD buffer access interrupt (SBFAI), 
 *        Card detect interrupt (CDETI), Card access interrupt (CACI), SDIO access interrupt (SDACI)]
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] select
 *             Specify interrupt arguments. See section 3.8 in application note for details.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Makes settings to the ICU controller registers.\n 
 *            Clears the SDHI interrupt source priority register (IPR) to 0.\n 
 *            Sets the SDHI interrupt request enable register (IEN) to disable interrupts.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.
 */
sdhi_status_t R_SDHI_DisableIcuInt(uint32_t channel, uint32_t select)
{
    uint32_t ipl = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    sdhi_sdhndl_t * p_hndl = 0;

    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }

    p_hndl = SDHI_GET_HNDL(channel);
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }

    if (select & SDHI_HWINT_PARAM)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    if (SDHI_CH0 == channel)    /* SDHI_CH0 */
    {
        /* ---- Disable interrupt request (IENn) of IER. ---- */
        if (select & SDHI_HWINT_BUFFER)
        {
            R_BSP_InterruptRequestDisable(VECT(SDHI, SBFAI));
        }

        if (select & SDHI_HWINT_ACCESS_CD)
        {
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
            ICU.GENBL1.BIT.EN3 = 0;
            ICU.GENBL1.BIT.EN4 = 0;
            ICU.GENBL1.BIT.EN5 = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
            if (0 == ICU.GENBL1.BIT.EN5)
            {
                R_BSP_NOP();    /* Wait for the write completion. */
            }

            ipl = 0;
            R_BSP_InterruptControl(BSP_INT_SRC_BL1_SDHI_CDETI,
                                   BSP_INT_CMD_GROUP_INTERRUPT_DISABLE,
                                   (void *)&ipl);
        }

        if (select & SDHI_HWINT_BUFFER)
        {
            IPR(SDHI,SBFAI)    = 0;
            if (0 == IPR(SDHI,SBFAI))
            {
                R_BSP_NOP();    /* Wait for the write completion. */
            }
        }
    }

    return SDHI_SUCCESS;

} /* End of function R_SDHI_DisableIcuInt() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_EnableIcuInt
 *****************************************************************************************************************/ /**
 * @brief Enables ICU controller interrupts for the SDHI.\n 
 *        note : The following interrupt is enabled. [SD buffer access interrupt (SBFAI), 
 *        Card detect interrupt (CDETI), Card access interrupt (CACI), SDIO access interrupt (SDACI)]
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] select
 *             Specify interrupt arguments. See section 3.7 in application note for details.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Makes settings to the ICU controller registers. \n 
 *            Makes settings to the SDHI's interrupt source property register (IPR). The setting values are defined by
 *            #define SDHI_CHx_INT_LEVRL and #define SDHI_CFG_CHx_INT_LEVEL_DMADTC.\n 
 *            Sets the SDHI interrupt request enable register (IEN) to enable interrupts.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.
 */
sdhi_status_t R_SDHI_EnableIcuInt(uint32_t channel, uint32_t select)
{
    uint32_t ipl = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    sdhi_sdhndl_t * p_hndl = 0;

    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }

    p_hndl = SDHI_GET_HNDL(channel);
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }

    if (select & SDHI_HWINT_PARAM)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    if (SDHI_CH0 == channel)    /* SDHI_CH0 */
    {
        /* ---- Enable interrupt request (IENn) of IER. ---- */
        if (select & SDHI_HWINT_BUFFER)
        {
            IPR(SDHI,SBFAI)    = SDHI_CFG_CH0_INT_LEVEL_DMADTC;
        }

        if (select & SDHI_HWINT_ACCESS_CD)
        {
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
            ICU.GENBL1.BIT.EN3 = 1;
            ICU.GENBL1.BIT.EN4 = 1;
            ICU.GENBL1.BIT.EN5 = 1;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
            if (1 == ICU.GENBL1.BIT.EN5)
            {
                R_BSP_NOP();    /* Wait for the write completion. */
            }

            ipl = SDHI_CFG_CH0_INT_LEVEL;
            R_BSP_InterruptControl(BSP_INT_SRC_BL1_SDHI_CDETI,
                                   BSP_INT_CMD_GROUP_INTERRUPT_ENABLE,
                                   (void *)&ipl);
        }

        if (select & SDHI_HWINT_BUFFER)
        {
            R_BSP_InterruptRequestEnable(VECT(SDHI, SBFAI));
            if (1 == IEN(SDHI,SBFAI))
            {
                R_BSP_NOP();    /* Wait for the write completion. */
            }
        }
    }

    return SDHI_SUCCESS;

} /* End of function R_SDHI_EnableIcuInt() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_CDLayout
 *****************************************************************************************************************/ /**
 * @brief This function checks whether the SDHI_CD (SD card detection) pin is used.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @retval    SDHI_SUCCESS CD pin used.
 * @retval    SDHI_ERR     CD pin not used.
 * @details   Checks whether the CD pin is used.
 * @note      None
 */
sdhi_status_t R_SDHI_CDLayout(uint32_t channel)
{
    sdhi_status_t ret = SDHI_ERR;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

#if (SDHI_CFG_CH0_INCLUDED == 1)
    if (SDHI_CH0 == channel)
    {
        if (SDHI_CFG_CH0_CD_ACTIVE)
        {
            ret = SDHI_SUCCESS;
        }
    }
#endif  /* #if (SDHI_CFG_CH0_INCLUDED == 1) */

    return ret;
} /* End of function R_SDHI_CDLayout() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_WPLayout
 *****************************************************************************************************************/ /**
 * @brief This function checks whether the SDHI_WP (SD write protect) pin is used.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @retval    SDHI_SUCCESS WP pin used.
 * @retval    SDHI_ERR     WP pin not used.
 * @details   Checks whether the WP pin is used.
 * @note      None
 */
sdhi_status_t R_SDHI_WPLayout(uint32_t channel)
{
    sdhi_status_t ret = SDHI_ERR;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

#if (SDHI_CFG_CH0_INCLUDED == 1)
    if (SDHI_CH0 == channel)
    {
        if (SDHI_CFG_CH0_WP_ACTIVE)
        {
            ret = SDHI_SUCCESS;
        }
    }
#endif  /* #if (SDHI_CFG_CH0_INCLUDED == 1) */

    return ret;
} /* End of function R_SDHI_WPLayout() */

/**********************************************************************************************************************
* Outline      : Card Access Option Register Init
* Function Name: r_sdhi_dev_sdopt_init
* Description  : Card Access Option Register Init.
* Arguments    : void
* Return Value : uint32_t
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdhi_dev_sdopt_init(void)
{
    return ((SDHI_SDOPT_INIT | SDHI_CFG_SDOPT_TOP) | SDHI_CFG_SDOPT_CTOP);
} /* End of function r_sdhi_dev_sdopt_init() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_GetSpeedType
 *****************************************************************************************************************/ /**
 * @brief This function gets information on the speed modes supported by the target microcontroller.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @retval    SDHI_SUCCESS Compatible with default-speed and high-speed modes
 * @retval    SDHI_ERR     Compatible with default-speed mode
 * @details   Gets information on the speed modes supported by the target microcontroller.
 * @note      None.
 */
sdhi_status_t R_SDHI_GetSpeedType(uint32_t channel)
{
    sdhi_status_t ret = SDHI_ERR;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

#if (SDHI_CFG_CH0_INCLUDED == 1)
    if (SDHI_CH0 == channel)
    {
        ret = SDHI_SUCCESS;
    }
#endif  /* #if (SDHI_CFG_CH0_INCLUDED == 1) */

    return ret;
} /* End of function R_SDHI_GetSpeedType() */

/**********************************************************************************************************************
* Outline      : SDHI CLKSEL BIT check
* Function Name: r_sdhi_check_clksel
* Description  : Check SDHI CLKSEL BIT.
* Arguments    : uint32_t           channel              ;   SDHI Channel No.
* Return Value : SDHI_SUCCESS                            ;   CLKSEL BIT: PCLK division by 1 setting permitted
*              : SDHI_ERR                                ;   CLKSEL BIT: PCLK division by 1 setting prohibited
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdhi_status_t r_sdhi_check_clksel(uint32_t channel)
{
    sdhi_status_t ret = SDHI_ERR;

#if (SDHI_CFG_CH0_INCLUDED == 1)
    if (SDHI_CH0 == channel)
    {
        ret = SDHI_SUCCESS;
    }
#endif  /* #if (SDHI_CFG_CH0_INCLUDED == 1) */

    return ret;
} /* End of function r_sdhi_check_clksel() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_IntHandler0
 *****************************************************************************************************************/ /**
 * @brief This function is the interrupt handler.
 * @param[in] *vect
 *             vector table
 * @details   This is the interrupt handler of the SDHI FIT module.\n 
 *            It is incorporated into the system as a processing routine for interrupt sources supported by the SDHI.\n
 *            When a callback function for the card access interrupt (CACI) and card detect interrupt (CDETI), 
 *            and a callback function for the SDIO access interrupt (SDACI) have been registered, 
 *            the appropriate callback function is called by this function.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.
 */
void R_SDHI_IntHandler0(void * vect)
{
    sdhi_sdhndl_t   * p_hndl = 0;
    uint32_t          int_isr = 0;
    uint32_t          channel = 0;
    uint32_t          sdsts1 = 0;
    uint32_t          sdsts2 = 0;
    uint32_t          sdiosts = 0;

    channel = 0;
    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_DEV, __LINE__);
        return;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    int_isr = ICU.GRPBL1.LONG;

    /* SDHI_MEM */
    if (int_isr & (SDHI_CH0_INT_CDETI | SDHI_CH0_INT_CACI))
    {
        sdsts1 = SDHI_INREG(p_hndl, SDHI_SDSTS1);
        sdsts2 = SDHI_INREG(p_hndl, SDHI_SDSTS2);
        if (sdsts1 | sdsts2)
        {
            if (p_hndl->int_callback)
            {
                (*p_hndl->int_callback)(sdsts1, sdsts2);
            }
        }
    }

    /* SDHI_SDIO */
    if (int_isr & SDHI_CH0_INT_SDACI)
    {
        sdiosts = SDHI_INREG(p_hndl, SDHI_SDIOSTS);
        if (sdiosts)
        {
            if (p_hndl->int_io_callback)
            {
                (*p_hndl->int_io_callback)(sdiosts);
            }
        }
    }

    return;

} /* End of function R_SDHI_IntHandler0() */

/**********************************************************************************************************************
* Outline      : Hardware Lock
* Function Name: r_sdhi_dev_hardware_lock
* Description  : Hardware Lock.
* Arguments    : uint32_t           channel     ; Channel No.
* Return Value : true                           ; Lock was acquired.
*              : false                          ; Lock was not acquired.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_sdhi_dev_hardware_lock(uint32_t channel)
{
    return R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_SDHI + channel));
} /* End of function r_sdhi_dev_hardware_lock() */

/**********************************************************************************************************************
* Outline      : Hardware Unlock
* Function Name: r_sdhi_dev_hardware_unlock
* Description  : Hardware Unlock.
* Arguments    : uint32_t           channel     ; Channel No.
* Return Value : true                           ; Lock was released.
*              : false                          ; Lock was not released.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_sdhi_dev_hardware_unlock(uint32_t channel)
{
    return R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_SDHI + channel));
} /* End of function r_sdhi_dev_hardware_unlock() */

#endif /* defined(BSP_MCU_RX671) */

/* End of File */
