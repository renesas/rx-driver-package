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
* File Name    : r_sdc_sd_int.c
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
* Outline      : Set SDIMSK1 and SDIMSK2 Interrupt Mask
* Function Name: r_sdc_sd_set_int_mask
* Description  : Sets the sdimsk1 and sdimsk2 according to the specified mask1 and mask2.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           mask1               ;   SDHI_SDIMSK1 bits value
*              : uint32_t           mask2               ;   SDHI_SDIMSK2 bits value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_set_int_mask(uint32_t card_no, uint32_t mask1, uint32_t mask2)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Set the sdimsk1 and sdimsk2 according to the specified mask1 and mask2. ---- */
    p_hndl->sdimsk1 = (p_hndl->sdimsk1 | mask1);
    p_hndl->sdimsk2 = ((p_hndl->sdimsk2 | mask2) & (~SDHI_SDIMSK2_BIT11));

    /* ---- Set the SDIMSK1 and SDIMSK2 registers. ---- */
    R_SDHI_SetIntMask(p_hndl->channel, mask1, mask2);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_set_int_mask() */

/**********************************************************************************************************************
* Outline      : Clear SDIMSK1 and SDIMSK2 Interrupt Mask
* Function Name: r_sdc_sd_clear_int_mask
* Description  : Clears the sdimsk1 and sdimsk2 according to the specified mask1 and mask2.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           mask1               ;   SDHI_SDIMSK1 bits value
*              : uint32_t           mask2               ;   SDHI_SDIMSK2 bits value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_clear_int_mask(uint32_t card_no, uint32_t mask1, uint32_t mask2)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear the sdimsk1 and sdimsk2 according to the specified mask1 and mask2. ---- */
    p_hndl->sdimsk1 = (p_hndl->sdimsk1 & (~mask1));
    p_hndl->sdimsk2 = ((p_hndl->sdimsk2 & (~mask2)) & (~SDHI_SDIMSK2_BIT11));

    /* ---- Clear the SDIMSK1 and SDIMSK2 registers. ---- */
    R_SDHI_ClearIntMask(p_hndl->channel, mask1, mask2);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_clear_int_mask() */

/**********************************************************************************************************************
* Outline      : Clear Sdsts Bits
* Function Name: r_sdc_sd_clear_sdsts
* Description  : Clears the sdsts1 and sdsts2 according to the specified clear_sdsts1 and clear_sdsts2.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           clear_sdsts1        ;   sdsts1 clear bits value
*              : uint32_t           clear_sdsts2        ;   sdsts2 clear bits value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_clear_sdsts(uint32_t card_no, uint32_t clear_sdsts1, uint32_t clear_sdsts2)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear the sdsts1 and sdsts2 according to the specified clear_sdsts1 and clear_sdsts2. ---- */
    p_hndl->sdsts1 = (p_hndl->sdsts1 & (~clear_sdsts1));
    p_hndl->sdsts2 = ((p_hndl->sdsts2 & (~clear_sdsts2)) & (~SDHI_SDIMSK2_BIT11));

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_clear_sdsts() */

/**********************************************************************************************************************
* Outline      : Get SDSTS1 and SDSTS2 Interrupt Sources.
* Function Name: r_sdc_sd_get_int
* Description  : Gets the SDSTS1 and SDSTS2 Interrupt sources, and then bits examine enabled elements hereafter
*              : clears SDSTS1 and SDSTS2 bits and saves those bits to sdsts1 or sdsts2.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           sdsts1              ;   SDSTS1 register value
*              : uint32_t           sdsts2              ;   SDSTS2 register value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_get_int(uint32_t card_no, uint32_t sdsts1, uint32_t sdsts2)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t          sdsts1_reg = 0;
    uint32_t          sdsts2_reg = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* Get the SDSTS1 and SDSTS2 registers. */
    sdsts1_reg = (uint32_t)(sdsts1 & p_hndl->sdimsk1);
    sdsts2_reg = (uint32_t)((sdsts2 & p_hndl->sdimsk2) & (~SDHI_SDIMSK2_BIT11));

    /* Clear the SDSTS1 and SDSTS2 registers. */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTS1, (~sdsts1_reg));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTS2, (~sdsts2_reg));

    /* Stores the interrupt sources to the SDSTS1 and SDSTS2. */
    p_hndl->sdsts1 = (p_hndl->sdsts1 | sdsts1_reg);
    p_hndl->sdsts2 = (p_hndl->sdsts2 | sdsts2_reg);

    if (sdsts1_reg || sdsts2_reg)
    {
        return SDC_SD_SUCCESS;   /* Any interrupt occurred */
    }

    return SDC_SD_ERR;  /* No interrupt occurred */
} /* End of function r_sdc_sd_get_int() */

/**********************************************************************************************************************
* Outline      : Check SDSTS1 and SDSTS2 Interrupt Sources
* Function Name: r_sdc_sd_get_intstatus
* Description  : Checks the SDSTS1 and SDSTS2 interrupt sources.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_get_intstatus(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;
    uint32_t        sdsts1 = 0;
    uint32_t        sdsts2 = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (p_hndl->int_mode)
    {
        /* ---- Hardware interrupt mode ---- */
        if (p_hndl->sdsts1 || p_hndl->sdsts2)
        {
            return SDC_SD_SUCCESS;
        }
        else
        {
            return SDC_SD_ERR;
        }
    }

    /* ---- Polling mode ---- */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS1, &sdsts1);
    R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS2, &sdsts2);
    
    return r_sdc_sd_get_int(card_no, sdsts1, sdsts2);
} /* End of function r_sdc_sd_get_intstatus() */

/**********************************************************************************************************************
* Outline      : Register SDSTS1 or SDSTS2 Interrupt Callback Function
* Function Name: r_sdc_sd_IntCallback
* Description  : Registers the callback function of SDSTS1 or SDSTS2 interrupt.
*              : If the SDSTS1 or SDSTS2 interrupt are occurred, calls callback function.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_status_t  (* callback)(int32_t) ;   Callback function
* Return Value : SDC_SD_SUCCESS                         ;   Generate interrupt
*              : SDC_SD_ERR                             ;   Not generated interrupt
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t))
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INT, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl->sdc_sd_int_callback = callback;

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_IntCallback() */

/* End of File */
