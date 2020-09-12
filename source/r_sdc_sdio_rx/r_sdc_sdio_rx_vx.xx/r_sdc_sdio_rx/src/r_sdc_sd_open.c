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
* File Name    : r_sdc_sd_open.c
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
* Outline      : Initialize SD Driver
* Function Name: r_sdc_sd_Open
* Description  : Initializes the SD Driver work memory specified by channel.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           channel             ;   SDHI FIT Module Channel No.
*              : void               * p_sdc_sd_workarea ;   SDC SD Driver work memory
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*              : SDC_SD_ERR_CPU_IF                      ;   CPU-IF function error
*              : SDC_SD_ERR_ADDRESS_BOUNDARY            ;   Not specified buffer address in 4-byte unit
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_Open(uint32_t card_no, uint32_t channel, void * p_sdc_sd_workarea)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t        * p_ptr = 0;
    int16_t           i = 0;

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    /*  Initialize the work memory.  */
    if (0 == (uint32_t)p_sdc_sd_workarea)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        return SDC_SD_ERR;
    }

    /*  Check the work memory boundary (4-byte unit)  */
    if ((uint32_t)p_sdc_sd_workarea & SDC_SD_ADDR_BOUNDARY)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        return SDC_SD_ERR_ADDRESS_BOUNDARY;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (0 != g_sdc_sd_handle[card_no])
    {
        return SDC_SD_SUCCESS;  /* Opend */
    }

    p_hndl = (sdc_sd_hndl_t *)p_sdc_sd_workarea;

    g_sdc_sd_handle[card_no] = p_hndl;

    /* ---- Clear the work memory to '0'. --- */
    p_ptr = (uint32_t *)p_hndl;

    for (i = (sizeof(sdc_sd_hndl_t) / sizeof(uint32_t)); i > 0; i--)
    {
        *p_ptr++ = 0;
    }

    p_hndl->channel = channel;

    /* Initialize the driver. */
    if (R_SDHI_Open(channel) != SDHI_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        R_SDHI_Close(channel);
        g_sdc_sd_handle[card_no] = 0;    /* Release the SD handle. */
        return SDC_SD_ERR_CPU_IF;
    }

    p_hndl->cd_port = (uint8_t)SDC_SD_CD_SOCKET;

    /* ---- Initialize the maximum block count. ---- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    p_hndl->sdmem_trans_sectors = SDC_SDMEM_MAX_TRANS;
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    p_hndl->sdio_trans_sectors = SDC_SDIO_MAX_TRANS;
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

    if (r_sdc_sd_set_int_callback(card_no, SDC_SD_CALLBACK_ENABLE) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        R_SDHI_Close(channel);
        g_sdc_sd_handle[card_no] = 0;    /* Release the SD handle. */
        return SDC_SD_ERR;
    }

    /* Enable all interrupts. */
    r_sdc_sd_enable_icu_int(card_no);

    return SDC_SD_SUCCESS;
    
} /* End of function r_sdc_sd_Open() */

/**********************************************************************************************************************
* Outline      : Finish SD Driver
* Function Name: r_sdc_sd_Close
* Description  : Finishes the SDHI Driver operation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : After this function finished, SD handle is unavailable.
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_Close(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;

    if (0 == g_sdc_sd_handle[card_no])
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (r_sdc_sd_set_int_callback(card_no, SDC_SD_CALLBACK_DISABLE) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        return SDC_SD_ERR;
    }

    if (R_SDHI_Close(p_hndl->channel) != SDHI_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_OPEN, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    g_sdc_sd_handle[card_no] = 0;    /* Destroy the SD Handle. */

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sd_Close() */

/**********************************************************************************************************************
* Outline      : Initialize SD Handle
* Function Name: r_sdc_sd_init_hndl
* Description  : Initializes the SD handle members.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           mode                ;   Driver mode
*              : uint32_t           voltage             ;   Working voltage
*              : uint32_t           type                ;   Initialize type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_init_hndl(uint32_t card_no, uint32_t mode, uint32_t voltage, uint32_t type)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t                i = 0;
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    int32_t                j = 0;
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_MODE_COMBO == type)
    {
        p_hndl->media_type &= (~SDC_SD_MEDIA_COMBO);
    }
    else if (SDC_SD_MODE_MEM == type)
    {
        p_hndl->media_type &= (~SDC_SD_MEDIA_MEM);
    }
    else    /* (SDC_SD_MODE_IO == type) */
    {
        p_hndl->media_type &= (~SDC_SD_MEDIA_SDIO);
    }

    if ((SDC_SD_MODE_MEM == type) || (SDC_SD_MODE_COMBO == type))
    {
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
        p_hndl->write_protect    = 0;
        p_hndl->prot_sector_size = 0;
        p_hndl->speed_mode       = 0;

        /* Initialize the card registers. */
        p_hndl->ocr[0]  = 0;
        p_hndl->dsr[0]  = 0;
        p_hndl->scr[0]  = 0;
        p_hndl->scr[1]  = 0;
        p_hndl->rca[0]  = 0;    /* Memory */

        for (i = 0; i < 4; ++i)
        {
            p_hndl->cid[i]          = 0;
            p_hndl->csd[i]          = 0;
            p_hndl->sdstatus[i]     = 0;
            p_hndl->status_data[i]  = 0;
        }

        p_hndl->status_data[4]      = 0;
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM */
    }

    if ((SDC_SD_MODE_IO == type) || (SDC_SD_MODE_COMBO == type))
    {
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
        p_hndl->rca[1]      = 0;    /* IO */
        p_hndl->io_tran_speed = 0;
        p_hndl->io_flag     = 0;
        p_hndl->io_info     = 0;
        p_hndl->io_ocr[0]   = 0;

        for (i = 0; i < ((int32_t)r_sdc_sdio_get_func_max_num()+1); ++i)
        {
            for (j = 0; j < (int32_t)r_sdc_sdio_get_io_reg_size(); ++j)
            {
                p_hndl->io_reg[i][j] = 0;
            }
            p_hndl->io_len[i]   = 0;
        }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */
    }
    
    p_hndl->resp_status         = SDC_SD_STATE_IDLE;
    p_hndl->error               = SDC_SD_SUCCESS;
    p_hndl->stop                = 0;
    p_hndl->voltage             = voltage;
    p_hndl->int_mode            = (uint8_t)(mode & SDC_SD_MODE_HWINT);
    p_hndl->trans_mode          = (uint8_t)(mode & (SDC_SD_MODE_DMA | SDC_SD_MODE_DTC));
    p_hndl->sup_card            = (uint8_t)(mode & SDC_SD_MODE_COMBO);
    p_hndl->io_speed_mode       = 0;

    if (mode & SDC_SD_MODE_1BIT)
    {
        p_hndl->sup_if_mode = SDC_SD_MODE_1BIT;
    }
    else
    {
        p_hndl->sup_if_mode = SDC_SD_MODE_4BIT;
    }

    /* Initialize the card registers. */
    p_hndl->if_cond[0]  = SDC_SD_R7_CHK_PATTERN;

    if (p_hndl->voltage & SDC_SD_R7_VOLT_ACCEPT_HIGH)
    {
        p_hndl->if_cond[0] = (p_hndl->if_cond[0] | SDC_SD_IF_COND_2_7_TO_3_6); /* High voltage : 2.7V-3.6V */
    }

    if (p_hndl->voltage & SDC_SD_R7_VOLT_ACCEPT_LOW)
    {
        p_hndl->if_cond[0] = (p_hndl->if_cond[0] | SDC_SD_IF_COND_LOW_RANGE); /* Low voltage : 1.65V-1.95V */
    }

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sd_init_hndl() */

/* End of File */
