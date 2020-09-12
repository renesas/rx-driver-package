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
* File Name    : r_sdc_sd_cd.c
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
* Outline      : Set Card Detection Interrupt
* Function Name: r_sdc_sd_CdInt
* Description  : Sets card detection interrupt.
* Arguments    : uint32_t           card_no             ;    SD Card No.
*              : int32_t            enable              ;    Card detection interrupt mode
*              :                                        ;     SDC_SD_CD_INT_ENABLE:Enables interrupt.
*              :                                        ;     SDC_SD_CD_INT_DISABLE:Disables interrupt.
*              : sdc_sd_status_t  ( *callback)(int32_t) ;   Interrupt callback function
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_CdInt(uint32_t card_no, int32_t enable, sdc_sd_status_t (* callback)(int32_t))
{
    uint32_t         sdsts1 = 0;
    sdc_sd_hndl_t  * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CD, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    if ((SDC_SD_CD_INT_ENABLE != enable) && (SDC_SD_CD_INT_DISABLE != enable))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CD, __LINE__);
        return SDC_SD_ERR;  /* Parameter error */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Check the interrupt setting. */
    if (0 == (p_hndl->sdimsk1 & (SDHI_SDIMSK1_DET_DAT3 | SDHI_SDIMSK1_DET_CD)))
    {
        r_sdc_sd_disable_icu_int(card_no);
        R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS1, &sdsts1);
        sdsts1 = (sdsts1 & (uint32_t)~SDHI_SDIMSK1_DET_DAT3_CD);
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTS1, sdsts1); /* Clear the insertion and removal bits. */
        r_sdc_sd_enable_icu_int(card_no);
    }

    if (SDHI_SUCCESS == R_SDHI_CDLayout(p_hndl->channel))
    {
        if (SDC_SD_CD_INT_ENABLE == enable)
        {
            /* Enable the insertion and removal interrupts. */
            if (SDC_SD_CD_SOCKET == p_hndl->cd_port)  /* CD */
            {
                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DET_CD, 0);
                r_sdc_sd_enable_icu_int(card_no);
            }
            else    /* DAT3 */
            {
                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DET_DAT3, 0);
                r_sdc_sd_enable_icu_int(card_no);
            }
        }
        else    /* case SDC_SD_CD_INT_DISABLE */
        {
            /* Disable the insertion and removal interrupts. */
            if (SDC_SD_CD_SOCKET == p_hndl->cd_port)  /* CD */
            {
                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DET_CD, 0);
                r_sdc_sd_enable_icu_int(card_no);
            }
            else    /* DAT3 */
            {
                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DET_DAT3, 0);
                r_sdc_sd_enable_icu_int(card_no);
            }
        }
    }

    /* ---- Register the callback function. ---- */
    p_hndl->sdc_sd_int_cd_callback = callback;

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_CdInt() */

/**********************************************************************************************************************
* Outline      : Check Card Insertion
* Function Name: r_sdc_sd_GetCardDetection
* Description  : Checks card insertion.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   A card is inserted.
*              : SDC_SD_ERR                             ;   A Card is not inserted.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_GetCardDetection(uint32_t card_no)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t  * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CD, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_get_card_detection(card_no);
} /* End of function r_sdc_sd_GetCardDetection() */

/**********************************************************************************************************************
* Outline      : Check Card Insertion
* Function Name: r_sdc_sd_get_card_detection
* Description  : Checks card insertion.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   A card is inserted
*              : SDC_SD_ERR                             ;   A card is not inserted
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_get_card_detection(uint32_t card_no)
{
    sdc_sd_hndl_t  * p_hndl = 0;
    uint32_t           reg = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS1, &reg);

    if (SDHI_SUCCESS == R_SDHI_CDLayout(p_hndl->channel))
    {
        if (SDC_SD_CD_SOCKET == p_hndl->cd_port)
        {
            reg = (reg & SDHI_SDSTS1_ISD_CD); /* Check CD level. */
        }
        else
        {
            reg = (reg & SDHI_SDSTS1_DET_DAT3);   /* Check DAT3 level. */
        }
    }
    else
    {
        reg = 1;
    }

    if (reg)
    {
        return SDC_SD_SUCCESS;   /* Inserted */
    }

    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CD, __LINE__);

    return SDC_SD_ERR;  /* No card */
} /* End of function r_sdc_sd_get_card_detection() */

/* End of File */
