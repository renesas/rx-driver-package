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
* File Name    : r_sdc_sd_initialize.c
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
* Outline      : Initialize SD Card
* Function Name: r_sdc_sd_Initialize
* Description  : Initializes the SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_cfg_t     * p_sdc_sd_config     ;   SDHI Driver operation configuration
*              : uint32_t           init_type           ;   SDHI Initialize type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_Initialize(uint32_t card_no, sdc_sd_cfg_t * p_sdc_sd_config, uint32_t init_type)
{
    sdc_sd_hndl_t     * p_hndl = 0;
    uint32_t            sdsts1_back = 0;
    uint32_t            sdhi_sddma_ctrl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sd_config)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;  /* Not initialized */
    }

#if !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if (SDC_SD_MODE_MEM == init_type)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    if (SDC_SD_MODE_IO == init_type)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* ==== Initialize parameter. ==== */
    r_sdc_sd_init_hndl(card_no, p_sdc_sd_config->mode, p_sdc_sd_config->voltage, init_type);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ==== Check the card insertion. ==== */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return p_hndl->error;     /* Not inserted */
    }

    /* ==== Power-on sequence ==== */
    /* ---- Set the single port. ---- */
    R_SDHI_SetBus(p_hndl->channel, SDHI_PORT_1BIT);

    /* ---- Supply the clock for card-identification mode. ---- */
    if (R_SDHI_SetClock(p_hndl->channel, r_sdc_sd_get_div_init_speed(), SDHI_CLOCK_ENABLE) != SDHI_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CPU_IF);
        return p_hndl->error;
    }

    r_sdc_sd_wait(card_no, 3);  /* Wait for 74 clock or more. */
    r_sdc_sd_disable_icu_int(card_no);
    R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS1, &sdsts1_back);
    sdsts1_back = (sdsts1_back & (~SDHI_SDIMSK1_TRNS_RESP));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTS1, sdsts1_back);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTS2, SDHI_SDIMSK2_BIT11);

    /* Disable the DMA/DTC. */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
    sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl & (~SDHI_SDDMAEN_DMASDRW));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);
    r_sdc_sd_enable_icu_int(card_no);

    if (r_sdc_sd_initialize(card_no, init_type) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
        return p_hndl->error;
    }

    /* ---- Set the memory mount flag. ---- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if ((SDC_SD_MEDIA_SDMEM == p_hndl->media_type) || (SDC_SD_MEDIA_COMBO == p_hndl->media_type))
    {
        p_hndl->mount = SDC_SD_MOUNT_UNLOCKED_CARD;
    }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM */

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;
} /* End of function r_sdc_sd_Initialize() */

/**********************************************************************************************************************
* Outline      : End SD Card
* Function Name: r_sdc_sd_End
* Description  : Ends the SD Card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           end_type            ;   SDHI End type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_End(uint32_t card_no, uint32_t end_type)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

#if !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if (SDC_SD_MODE_MEM == end_type)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    if (SDC_SD_MODE_IO == end_type)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* !(SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Disable the SDIO interrupt. */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    if ((SDC_SD_MODE_COMBO == end_type) || (SDC_SD_MODE_IO == end_type))
    {
        r_sdc_sdio_disable_int_ioirqm(card_no);
    }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

    /* ---- Clear the memory mount flag. ---- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if ((SDC_SD_MODE_COMBO == end_type) || (SDC_SD_MODE_MEM == end_type))
    {
        p_hndl->mount = SDC_SD_UNMOUNT_CARD;
    }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM */

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    /* ---- Initialize the SD handle. ---- */
    r_sdc_sd_init_hndl(card_no, 0, p_hndl->voltage, end_type);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_End() */

/**********************************************************************************************************************
* Outline      : Initialize SD Card
* Function Name: r_sdc_sd_initialize
* Description  : Initializes the SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           init_type           ;   SDHI Initialize type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_initialize(uint32_t card_no, uint32_t init_type)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;

    switch (init_type)
    {
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
        case SDC_SD_MODE_MEM:
            ret = r_sdc_sdmem_initialize(card_no, init_type);
        break;
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
        case SDC_SD_MODE_IO:
            ret = r_sdc_sdio_initialize(card_no, init_type);
        break;
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) && (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO))
        case SDC_SD_MODE_COMBO:
            ret = r_sdc_sdcombo_initialize(card_no, init_type);
        break;
#endif /* ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) && (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)) */
        default:
            /* Do nothing. */
        break;
    }

    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INITIALIZE, __LINE__);
    }

    return ret;
} /* End of function r_sdc_sd_initialize() */

/* End of File */
