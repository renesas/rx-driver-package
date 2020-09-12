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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdc_sdio_int.c
* Version      : 2.00
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDC SD API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.09.2014 1.00    First Release
*              : 17.07.2015 1.10    Standardized by the combo.
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sd_rx_if.h"
#include ".\src\r_sdc_sd_rx_private.h"

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)

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
* Outline      : Set SDHI_SDIOSTS Interrupt Mask
* Function Name: r_sdc_sdio_set_int_mask
* Description  : Sets the sdioimsk according to the specified mask and sets the SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : uint32_t       mask                    ;   SDHI_SDIOIMSK bits value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_set_int_mask(uint32_t card_no, uint32_t mask)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Set the sdioimsk according to the specified mask. ---- */
    p_hndl->sdioimsk = (p_hndl->sdioimsk | mask);

    /* ---- Set the SDIOIMSK register. ---- */
    R_SDHI_SetSdioIntMask(p_hndl->channel, mask);

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_set_int_mask() */

/**********************************************************************************************************************
* Outline      : Clear SDHI_SDIOIMSK interrupt Mask
* Function Name: r_sdc_sdio_clear_int_mask
* Description  : Clears the sdioimsk according to the specified mask and clears the SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : uint32_t       mask                    ;   SDHI_SDIOIMSK bits value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_clear_int_mask(uint32_t card_no, uint32_t mask)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear the sdioimsk. ---- */
    p_hndl->sdioimsk = (p_hndl->sdioimsk & (~mask));

    /* ---- Clear the SDIOIMSK register. ---- */
    R_SDHI_ClearSdioIntMask(p_hndl->channel, mask);

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_clear_int_mask() */

/**********************************************************************************************************************
* Outline      : Clear Sdiosts Bits
* Function Name: r_sdc_sdio_clear_sdiosts
* Description  : Clear int_io_sdsts depend on the clear value
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : uint32_t       clear                   ;   sdiosts clear bits value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_clear_sdiosts(uint32_t card_no, uint32_t clear)
{
    sdc_sd_hndl_t  * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear the sdiosts according to the specified clear. ---- */
    p_hndl->sdiosts = (p_hndl->sdiosts & (~clear));

    return SDC_SD_SUCCESS;
    
} /* End of function r_sdc_sdio_clear_sdiosts() */

/**********************************************************************************************************************
* Outline      : Get SDHI_SDIOSTS Interrupt Sources.
* Function Name: r_sdc_sdio_get_int
* Description  : Get the SDIOSTS Interrupt sources, and then bits examine enabled elements hereafter,
*                clear SDHI_SDIOSTS bits save
* Arguments    : uint32_t       card_no                 ; SD Card No.
*              : uint32_t       sdiosts                 ; SDIO Status Register
* Return Value : SDC_SD_SUCCESS                         ; Successful operation
*              : SDC_SD_ERR                             ; Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_get_int(uint32_t card_no, uint32_t sdiosts)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* Store the interrupt sources. */
    p_hndl->sdiosts = sdiosts & p_hndl->sdioimsk;

    if (0 == (sdiosts & SDHI_SDIOSTS_IOIRQ))
    {
        /* Clear the SDIOSTS register. */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOSTS, ((uint32_t)~(sdiosts) | SDHI_SDIOSTS_INIT));
    }
    else
    {
        /* IOIRQ bit is the detection flag by SDIO interrupt.
           SDIO Interrupt has to perform flag clear after it was done negate.
           Therefore perform flag clear before enabling SDIO interrupt.
           Here, prohibit SDIO interrupt and stop the detection of the SDIO interrupt. */
        p_hndl->sdioimsk = (p_hndl->sdioimsk & (~SDHI_SDIOSTS_IOIRQ));
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOIMSK, ((uint32_t)~(p_hndl->sdioimsk) | SDHI_SDIOSTS_INIT));
    }

    if (sdiosts)
    {
        return SDC_SD_SUCCESS;   /* Any interrupt occurred */
    }

    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INT, __LINE__);

    return SDC_SD_ERR;  /* No interrupt occurred */

} /* End of function r_sdc_sdio_get_int() */

/**********************************************************************************************************************
* Outline      : Check SDHI_SDIOSTS Interrupt Sources
* Function Name: r_sdc_sdio_get_intstatus
* Description  : Check the SDIOSTS interrupt sources
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_get_intstatus(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    uint32_t              sdiosts = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INT, __LINE__);
        return SDC_SD_ERR;  /* not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (p_hndl->int_mode)
    {
        /* ---- Hardware interrupt mode ---- */
        if (p_hndl->sdiosts)
        {
            return SDC_SD_SUCCESS;
        }
        else
        {
            return SDC_SD_ERR;
        }
    }

    /* ---- Polling mode ---- */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS1, &sdiosts);
    
    return r_sdc_sdio_get_int(card_no, sdiosts);
    
} /* End of function r_sdc_sdio_get_intstatus() */

/**********************************************************************************************************************
* Outline      : Register SDHI_SDIOSTS Interrupt Callback Function
* Function Name: r_sdc_sdio_IntCallback
* Description  : Registers the callback function of SDIOSTS interrupt.
*              : If the SDIOSTS interrupt is occurred, call callback function.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t        (* callback)(int32_t)   ;   callback function
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t))
{
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INT, __LINE__);
        return SDC_SD_ERR;  /* not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl->sdc_sd_int_io_callback = callback;

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_IntCallback() */

/**********************************************************************************************************************
* Outline      : Enable SDIO Interrupt from IOIRQM Bit
* Function Name: r_sdc_sdio_enable_int_ioirqm
* Description  : Clear IOIRQ bit of SDIOSTS register and enables SDIO interrupt from IOIRQM bit of SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : Access the SDIO card, negate the SDIO interrupt from the SDIO card, and then call this function.
*              : If the SDIO interrupt from the SDIO card is not negated, the IOIRQ flag might become 1 again.
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_enable_int_ioirqm(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    uint32_t            sdhi_sdioimsk_ctrl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    p_hndl->sdiosts = (p_hndl->sdiosts & (~SDHI_SDIOSTS_IOIRQ));

    /* ---- Clear the IOIRQ. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOSTS, ((uint32_t)~(SDHI_SDIOSTS_IOIRQ) | SDHI_SDIOSTS_INIT));
    
    /* ---- Set the SDIOIMSK register. ---- */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDIOIMSK, &sdhi_sdioimsk_ctrl);
    sdhi_sdioimsk_ctrl = ((sdhi_sdioimsk_ctrl & (~SDHI_SDIOSTS_IOIRQ)) | SDHI_SDIOSTS_INIT);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOIMSK, sdhi_sdioimsk_ctrl);

    p_hndl->sdioimsk = (p_hndl->sdioimsk | SDHI_SDIOSTS_IOIRQ);

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_enable_int_ioirqm() */

/**********************************************************************************************************************
* Outline      : Disable SDIO Interrupt from IOIRQM Bit
* Function Name: r_sdc_sdio_disable_int_ioirqm
* Description  : Disables interrupt from IOIRQM bit of SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_disable_int_ioirqm(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    uint32_t            sdhi_sdioimsk_ctrl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear the SDIOIMSK register. ---- */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDIOIMSK, &sdhi_sdioimsk_ctrl);
    sdhi_sdioimsk_ctrl = ((sdhi_sdioimsk_ctrl | SDHI_SDIOSTS_IOIRQ) | SDHI_SDIOSTS_INIT);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOIMSK, sdhi_sdioimsk_ctrl);

    p_hndl->sdioimsk = (p_hndl->sdioimsk & (~SDHI_SDIOSTS_IOIRQ));

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_disable_int_ioirqm() */

/**********************************************************************************************************************
* Outline      : Enable SDIO Interrupt from IOIRQM Bit
* Function Name: r_sdc_sdio_EnableInt
* Description  : Clear IOIRQ bit of SDIOSTS register and enables SDIO interrupt from IOIRQM bit of SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : Access the SDIO card, negate the SDIO interrupt from the SDIO card, and then call this function.
*              : If the SDIO interrupt from the SDIO card is not negated, the IOIRQ flag might become 1 again.
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_EnableInt(uint32_t card_no)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INT, __LINE__);
        return SDC_SD_ERR;  /* not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sdio_enable_int_ioirqm(card_no);

} /* End of function r_sdc_sdio_EnableInt() */

/**********************************************************************************************************************
* Outline      : Disable SDIO Interrupt from IOIRQM Bit
* Function Name: r_sdc_sdio_DisableInt
* Description  : Disables SDIO interrupt from IOIRQM bit of SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_DisableInt(uint32_t card_no)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INT, __LINE__);
        return SDC_SD_ERR;  /* not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
    return r_sdc_sdio_disable_int_ioirqm(card_no);

} /* End of function r_sdc_sdio_DisableInt() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
