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
* Copyright (C) 2017(2019) Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdhi_int.c
* Version      : 2.06
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDHI API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 31.07.2017 2.00    First Release
*              : 20.05.2019 2.04    Added support for GNUC and ICCRX.
*                                   Fixed coding style. 
*              : 22.11.2019 2.06    Modified comment of API function to Doxygen style.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdhi_rx_if.h"
#include ".\src\r_sdhi_rx_private.h"

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
 * Function Name: R_SDHI_SetIntMask
 *****************************************************************************************************************/ /**
 * @brief This function controls the SD interrupt mask registers to enable SD interrupts.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] mask1
 *             SD interrupt mask register 1 (SDIMSK1) control\n 
 *              To enable an interrupt, set the target bit to 1.\n 
 *              To not change the interrupt setting, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit is invalid.\n 
 *              For details of the SDIMSK1 register, refer to section 3.9 in application note.
 * @param[in] mask2
 *             SD interrupt mask register 2 (SDIMSK2) control\n 
 *              To enable an interrupt, set the target bit to 1.\n 
 *              To not change the interrupt setting, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit is invalid.\n 
 *              For details of the SDIMSK2 register, refer to section 3.9 in application note.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Controls SD interrupt mask register 1 (SDIMSK1) and SD interrupt mask register 2 (SDIMSK2) to enable 
 *            SD interrupts.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            After calling this function, call the R_SDHI_EnableIcuInt() function to enable SDHI ICU controller 
 *            interrupts. If not enabled, no SD interrupts will be generated.
 */
sdhi_status_t R_SDHI_SetIntMask(uint32_t channel, uint32_t mask1, uint32_t mask2)
{
    sdhi_sdhndl_t * p_hndl = 0;
    uint32_t mask1_reg = 0;
    uint32_t mask2_reg = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Set the sdimsk1 and sdimsk2 according to the specified mask1 and mask2. ---- */
    mask1_reg = (SDHI_INREG(p_hndl, SDHI_SDIMSK1) & (~mask1));
    mask2_reg = ((SDHI_INREG(p_hndl, SDHI_SDIMSK2) & (~mask2)) | SDHI_SDIMSK2_BIT11);

    /* ---- Set the SDIMSK1 and SDIMSK2 registers. ---- */
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK1, mask1_reg);
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK2, mask2_reg);
    if (SDHI_INREG(p_hndl, SDHI_SDIMSK2) == mask2_reg)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;
} /* End of function R_SDHI_SetIntMask() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_ClearIntMask
 *****************************************************************************************************************/ /**
 * @brief This function controls the SD interrupt mask registers to disable SD interrupts.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] mask1
 *             SD interrupt mask register 1 (SDIMSK1) control\n 
 *              To disable an interrupt, set the target bit to 1.\n 
 *              To not change the interrupt setting, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit is invalid.\n 
 *              For details of the SDIMSK1 register, refer to section 3.9 in application note.
 * @param[in] mask2
 *             SD interrupt mask register 2 (SDIMSK2) control\n 
 *              To disable an interrupt, set the target bit to 1.\n 
 *              To not change the interrupt setting, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit is invalid.\n 
 *              For details of the SDIMSK2 register, refer to section 3.9 in application note.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Controls SD interrupt mask register 1 (SDIMSK1) and SD interrupt mask register 2 (SDIMSK2) to disable
 *            SD interrupts.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            Before calling this function, call the R_SDHI_DisableIcuInt() function to disable SDHI ICU controller
 *            interrupts. If not disabled, SD interrupts may be generated at unintended times.
 */
sdhi_status_t R_SDHI_ClearIntMask(uint32_t channel, uint32_t mask1, uint32_t mask2)
{
    sdhi_sdhndl_t * p_hndl = 0;
    uint32_t mask1_reg = 0;
    uint32_t mask2_reg = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Clear the sdimsk1 and sdimsk2 according to the specified mask1 and mask2. ---- */
    mask1_reg = (SDHI_INREG(p_hndl, SDHI_SDIMSK1) | mask1);
    mask2_reg = ((SDHI_INREG(p_hndl, SDHI_SDIMSK2) | mask2) | SDHI_SDIMSK2_BIT11);

    /* ---- Clear the SDIMSK1 and SDIMSK2 registers. ---- */
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK1, mask1_reg);
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK2, mask2_reg);
    if (SDHI_INREG(p_hndl, SDHI_SDIMSK2) == mask2_reg)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;
} /* End of function R_SDHI_ClearIntMask() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_ClearSdstsReg
 *****************************************************************************************************************/ /**
 * @brief This function controls the SD status registers to clear interrupt flags.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] clear_sdsts1
 *             SD status register 1 (SDSTS1) control\n 
 *              To 0 clear an interrupt flag, set the target bit to 1.\n 
 *              To not change the interrupt flag, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit is invalid.\n 
 *              For details of the SDSTS1 register, refer to section 3.11 in application note.
 * @param[in] clear_sdsts2
 *             SD status register 2 (SDSTS2) control\n 
 *              To 0 clear an interrupt flag, set the target bit to 1.\n 
 *              To not change the interrupt flag, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit and b12 (Reserved bit) is invalid.\n 
 *              For details of the SDSTS2 register, refer to section 3.11 in application note.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Clears interrupt flags in SD status register 1 (SDSTS1) and SD status register 2 (SDSTS2).
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            Before calling this function, call the R_SDHI_DisableIcuInt() function to disable SDHI ICU controller
 *            interrupts. If not disabled, SD interrupts may be generated at unintended times.
 */
sdhi_status_t R_SDHI_ClearSdstsReg(uint32_t channel, uint32_t clear_sdsts1, uint32_t clear_sdsts2)
{
    sdhi_sdhndl_t   * p_hndl = 0;
    uint32_t            sdsts1 = 0;
    uint32_t            sdsts2 = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Clear the SDSTS1 and SDSTS2 according to the specified clear_sdsts1 and clear_sdsts2. ---- */
    sdsts1 = (SDHI_INREG(p_hndl, SDHI_SDSTS1) & (~clear_sdsts1));
    sdsts2 = ((SDHI_INREG(p_hndl, SDHI_SDSTS2) & (~clear_sdsts2)) | SDHI_SDIMSK2_BIT11);

    /* Clear the SDSTS1 and SDSTS2 registers. */
    SDHI_OUTREG(p_hndl, SDHI_SDSTS1, sdsts1);
    SDHI_OUTREG(p_hndl, SDHI_SDSTS2, sdsts2);

    if (SDHI_INREG(p_hndl, SDHI_SDSTS2) == sdsts2)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;
} /* End of function R_SDHI_ClearSdstsReg() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_SetSdioIntMask
 *****************************************************************************************************************/ /**
 * @brief This function controls the SDIO interrupt mask registers to enable SDIO interrupts.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] mask
 *             SDIO interrupt mask register (SDIOIMSK) control\n 
 *              To enable an interrupt, set the target bit to 1.\n 
 *              To not change the interrupt setting, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit and b2-b1 (Reserved bit) is invalid.\n 
 *              For details of the SDIOIMSK register, refer to section 3.12 in application note.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Controls the SDIO interrupt mask register (SDIOIMSK) to enable interrupts.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            After calling this function, call the R_SDHI_EnableIcuInt() function to enable SDHI ICU controller
 *            interrupts. If not enabled, no SDIO interrupts will be generated.
 */
sdhi_status_t R_SDHI_SetSdioIntMask(uint32_t channel, uint32_t mask)
{
    sdhi_sdhndl_t       * p_hndl = 0;
    uint32_t mask_reg = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Set the sdioimsk according to the specified mask. ---- */
    mask_reg = ((SDHI_INREG(p_hndl, SDHI_SDIOIMSK) & (~mask)) | SDHI_SDIOSTS_INIT);

    /* ---- Set the SDIOIMSK register. ---- */
    SDHI_OUTREG(p_hndl, SDHI_SDIOIMSK, mask_reg);
    if (SDHI_INREG(p_hndl, SDHI_SDIOIMSK) == mask_reg)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;

} /* End of function R_SDHI_SetSdioIntMask() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_ClearSdioIntMask
 *****************************************************************************************************************/ /**
 * @brief This function controls the SDIO interrupt mask registers to disable SDIO interrupts.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] mask
 *             SDIO interrupt mask register (SDIOIMSK) control\n 
 *              To disable an interrupt, set the target bit to 1.\n 
 *              To not change the interrupt setting, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit and b2-b1 (Reserved bit) is invalid.\n 
 *              For details of the SDIOIMSK register, refer to section 3.12 in application note.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Controls the SDIO interrupt mask register (SDIOIMSK) to disable interrupts.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            Before calling this function, call the R_SDHI_DisableIcuInt() function to disable SDHI ICU controller
 *            interrupts. If not disabled, SD interrupts may be generated at unintended times.
 */
sdhi_status_t R_SDHI_ClearSdioIntMask(uint32_t channel, uint32_t mask)
{
    sdhi_sdhndl_t       * p_hndl = 0;
    uint32_t mask_reg = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Clear the sdioimsk. ---- */
    mask_reg = ((SDHI_INREG(p_hndl, SDHI_SDIOIMSK) | mask) | SDHI_SDIOSTS_INIT);

    /* ---- Clear the SDIOIMSK register. ---- */
    SDHI_OUTREG(p_hndl, SDHI_SDIOIMSK, mask_reg);
    if (SDHI_INREG(p_hndl, SDHI_SDIOIMSK) == mask_reg)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;

} /* End of function R_SDHI_ClearSdioIntMask() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_ClearSdiostsReg
 *****************************************************************************************************************/ /**
 * @brief This function controls the SDIO status registers to clear interrupt flags.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] clear
 *             SDIO status register (SDIOSTS) control\n 
 *              To clear an interrupt flag, set the target bit to 1.\n 
 *              To not change the interrupt flag, clear the target bit to 0.\n 
 *              However, setting to the Read Only bit and b2-b1 (Reserved bit) is invalid.\n 
 *              For details of the SDIOSTS register, refer to section 3.14 in application note.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Clears interrupt flags in the SDIO status register (SDIOSTS).
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            Before calling this function, call the R_SDHI_DisableIcuInt() function to disable SDHI ICU controller
 *            interrupts. If not disabled, SD interrupts may be generated at unintended times.
 */
sdhi_status_t R_SDHI_ClearSdiostsReg(uint32_t channel, uint32_t clear)
{
    sdhi_sdhndl_t   * p_hndl = 0;
    uint32_t          sdiosts = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Clear the SDIOSTS according to the specified clear value. ---- */
    sdiosts = ((SDHI_INREG(p_hndl, SDHI_SDIOSTS) & (~clear)) | SDHI_SDIOSTS_INIT);

    /* Clear the SDIOSTS register. */
    SDHI_OUTREG(p_hndl, SDHI_SDIOSTS, sdiosts);
    if (SDHI_INREG(p_hndl, SDHI_SDIOSTS) == sdiosts)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;
} /* End of function R_SDHI_ClearSdiostsReg() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_IntCallback
 *****************************************************************************************************************/ /**
 * @brief This function registers the callback function for the card access interrupt (CACI) 
 *        and card detect interrupt (CDETI).
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] (*callback)(uint32_t,uint32_t)
 *             Callback function to be registered.\n 
 *             If set to a null pointer, no callback function is registered.\n 
 *             The first argument (uint32_t) contains the value of SD status register 1 (SDSTS1).\n 
 *             The second argument (uint32_t) contains the value of SD status register 2 (SDSTS2).
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Registers the callback function for the card access interrupt (CACI) and card detect interrupt (CDETI).
 *            The callback function registered by this function is called as a subroutine of the interrupt handler 
 *            when an interrupt is generated by a change in the SD protocol status.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required. \n 
 *            The callback function registered by this function is different from the callback functions of the SD 
 *            buffer access interrupt (SBFAI) and SDIO access interrupt (SDACI). Therefore, this callback function 
 *            is not called when the above-mentioned interrupts occur.
 */
sdhi_status_t R_SDHI_IntCallback(uint32_t channel, sdhi_status_t (* callback)(uint32_t, uint32_t))
{
    sdhi_sdhndl_t * p_hndl = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* Not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl->int_callback = callback;

    return SDHI_SUCCESS;
} /* End of function R_SDHI_IntCallback() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_IntSDBuffCallback
 *****************************************************************************************************************/ /**
 * @brief This function registers the callback function for the SD buffer access interrupt (SBFAI).
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] (\*callback)(void\*)
 *             Callback function to be registered.\n 
 *             If set to a null pointer, no callback function is registered.\n 
 *             The value of (void *) is always 0.
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Registers the callback function for the SD buffer access interrupt (SBFAI). 
 *            The callback function registered by this function is called as a subroutine of the DTC's data transfer
 *            end interrupt handler when an interrupt is generated at DTC transfer end.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            The callback function registered by this function is different from the callback function of the card
 *            access interrupt (CACI) and card detect interrupt (CDETI), and from the callback function of the SDIO
 *            access interrupt (SDACI). Therefore, this callback function is not called when the above-mentioned
 *            interrupts occur.
 */
sdhi_status_t R_SDHI_IntSDBuffCallback(uint32_t channel, sdhi_status_t (* callback)(void *))
{
    sdhi_sdhndl_t * p_hndl = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* Not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl->int_sdbuff_callback = callback;

    return SDHI_SUCCESS;
} /* End of function R_SDHI_IntSDBuffCallback() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_IntSdioCallback
 *****************************************************************************************************************/ /**
 * @brief This function registers the callback function for the SDIO access interrupt (SDACI).
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @param[in] (*callback)(uint32_t)
 *             Callback function to be registered.\n 
 *             If set to a null pointer, no callback function is registered.\n 
 *             The first argument (uint32_t) contains the value of the SDIO status register (SDIOSTS).
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Registers the callback function for the SDIO access interrupt (SDACI).\n 
 *            The callback function registered by this function is called as a subroutine of the interrupt handler
 *            when an SDHI SDIO interrupt is generated.
 * @note      Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            The callback function registered by this function is different from the callback function of
 *            the card access interrupt (CACI) and card detect interrupt (CDETI), and from the callback function of
 *            the SD buffer access interrupt (SBFAI). Therefore, this callback function is not called when 
 *            the above-mentioned interrupts occur.
 */
sdhi_status_t R_SDHI_IntSdioCallback(uint32_t channel, sdhi_status_t (* callback)(uint32_t))
{
    sdhi_sdhndl_t       * p_hndl = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_INT, __LINE__);
        return SDHI_ERR;  /* not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl->int_io_callback = callback;

    return SDHI_SUCCESS;

} /* End of function R_SDHI_IntSdioCallback() */

/* End of File */
