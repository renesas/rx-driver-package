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
* Copyright (C) 2014(2015-2019) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdc_sd_util.c
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
*              : 10.02.2020 3.00    Added comment "WAIT_LOOP".
*                                   Added support for GNUC and ICCRX.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sd_rx_if.h"
#include ".\src\r_sdc_sd_rx_private.h"

#ifdef SDC_SD_CFG_LONGQ_ENABLE
#include "r_longq_if.h"
#endif /* SDC_SD_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
static uint32_t g_sdc_sd_next = 0;      /* Volume ID Number */

#ifdef SDC_SD_CFG_LONGQ_ENABLE
static longq_hdl_t     p_sdc_sd_long_que = 0;
#endif /* SDC_SD_CFG_LONGQ_ENABLE */

static uint32_t g_sdc_sd_timer_cnt_out[SDC_SD_TIMER_CARD_MAX_NUM][SDC_SD_TIMER_CARD_MAX_COUNT + 1];

/**********************************************************************************************************************
* Outline      : Check Hardware Write Protect
* Function Name: r_sdc_sd_iswp
* Description  : Checks the hardware write protect using the SDHI register.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : 0                                      ;   WP pin = High (Non Write Protection)
*              : 1                                      ;   WP pin = Low  (Write Protection)
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_iswp(uint32_t card_no)
{
    sdc_sd_hndl_t  * p_hndl = 0;
    uint32_t            wp = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDHI_SUCCESS == R_SDHI_WPLayout(p_hndl->channel))
    {
        /*  Check the SDHI_SDSTS1 WP bit.  */
        R_SDHI_GetWP(p_hndl->channel, &wp);
    }
    else
    {
        wp = 0;
    }

    return wp;
} /* End of function r_sdc_sd_iswp() */

/**********************************************************************************************************************
* Outline      : Get Bit Information
* Function Name: r_sdc_sd_bit_search
* Description  : Checks the bits of argument (data) from LSB.
* Arguments    : uint32_t           data                ;   Checked data
* Return Value : -1                                     ;   no bit has 1'b
*              : other                                  ;   bit number has 1'b
*----------------------------------------------------------------------------------------------------------------------
* Notes        : All 16bits value can be applied.
**********************************************************************************************************************/
int32_t r_sdc_sd_bit_search(uint32_t data)
{
    int32_t i = 0;

    /* WAIT_LOOP */
    for (i = 15; 0 <= i; i--)
    {
        if (data & 1u)
        {
            return i;
        }
        data >>= SDC_SD_BIT_SHIFT_1;
    }

    return -1;
} /* End of function r_sdc_sd_bit_search() */

/**********************************************************************************************************************
* Outline      : Set Errors Information
* Function Name: r_sdc_sd_set_err
* Description  : Sets the error information (=error) to SD Handle member (=p_hndl->error).
* Arguments    : uint32_t               card_no         ;   SD Card No.
*              : sdc_sd_status_t      error             ;   Error information
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_set_err(uint32_t card_no, sdc_sd_status_t error)
{
    sdc_sd_hndl_t  * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_SUCCESS == p_hndl->error)
    {
        p_hndl->error = error;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_set_err() */

/**********************************************************************************************************************
* Outline      : Get Mode Status
* Function Name: r_sdc_sd_GetModeStatus
* Description  : Gets the mode status.
* Arguments    : uint32_t               card_no         ;   SD Card No.
*              : uint8_t                * p_mode        ;   Mode Status configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_GetModeStatus(uint32_t card_no, uint8_t * p_mode)
{
    sdc_sd_hndl_t * p_hndl = 0;
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    uint8_t         tmp_io_flag = 0;
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    /* ---- Check the card is mounted. ---- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    tmp_io_flag = p_hndl->io_flag & (SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT);
    if ((SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT) != tmp_io_flag)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_mode)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (p_hndl->trans_mode & SDC_SD_MODE_DMA)
    {
        *p_mode = SDC_SD_MODE_DMA;
    }
    else if (p_hndl->trans_mode & SDC_SD_MODE_DTC)
    {
        *p_mode = SDC_SD_MODE_DTC;
    }
    else
    {
        *p_mode = SDC_SD_MODE_SW;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_GetModeStatus() */

/**********************************************************************************************************************
* Outline      : Get Card Status
* Function Name: r_sdc_sd_GetCardStatus
* Description  : Gets the card status.
* Arguments    : uint32_t               card_no         ;   SD Card No.
*              : sdc_sd_card_status_t * p_sdc_sd_card_status    ;   Card Status configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_GetCardStatus(uint32_t card_no, sdc_sd_card_status_t * p_sdc_sd_card_status)
{
    sdc_sd_hndl_t * p_hndl = 0;
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    uint8_t         tmp_io_flag = 0;
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    tmp_io_flag = p_hndl->io_flag & (SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT);
    if ((SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT) != tmp_io_flag)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sd_card_status)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_sdc_sd_card_status->media_type       = p_hndl->media_type;
    p_sdc_sd_card_status->csd_structure    = p_hndl->csd_structure;
    p_sdc_sd_card_status->speed_mode       = p_hndl->speed_mode;
    p_sdc_sd_card_status->io_speed_mode    = p_hndl->io_speed_mode;
    p_sdc_sd_card_status->card_sector_size = p_hndl->card_sector_size;
    p_sdc_sd_card_status->prot_sector_size = p_hndl->prot_sector_size;
    p_sdc_sd_card_status->write_protect    = p_hndl->write_protect;

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_GetCardStatus() */

/**********************************************************************************************************************
* Outline      : Get Card Information
* Function Name: r_sdc_sd_GetCardInfo
* Description  : Gets the card information.
* Arguments    : uint32_t            card_no            ;   SD Card No.
*              : sdc_sd_card_reg_t * p_sdc_sd_card_reg  ;   CardReg configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_GetCardInfo(uint32_t card_no, sdc_sd_card_reg_t * p_sdc_sd_card_reg)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t            i = 0;
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    uint8_t         tmp_io_flag = 0;
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }

    /* ---- Check the card is mounted. ---- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    tmp_io_flag = p_hndl->io_flag & (SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT);
    if ((SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT) != tmp_io_flag)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sd_card_reg)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    p_sdc_sd_card_reg->sdio_ocr[0] = p_hndl->io_ocr[0];
#else
    p_sdc_sd_card_reg->sdio_ocr[0] = 0;
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */
    p_sdc_sd_card_reg->ocr[0] = p_hndl->ocr[0];
    p_sdc_sd_card_reg->dsr[0] = p_hndl->dsr[0];
    
    /* WAIT_LOOP */
    for (i = 0; i < 2; ++i)
    {
        p_sdc_sd_card_reg->rca[i] = p_hndl->rca[i];
        p_sdc_sd_card_reg->scr[i] = p_hndl->scr[i];
    }
    /* WAIT_LOOP */
    for (i = 0; i < 4; ++i)
    {
        p_sdc_sd_card_reg->cid[i] = p_hndl->cid[i];
        p_sdc_sd_card_reg->csd[i] = p_hndl->csd[i];
        p_sdc_sd_card_reg->sdstatus[i] = p_hndl->sdstatus[i];
    }
    /* WAIT_LOOP */
    for (i = 0; i < 5; ++i)
    {
        p_sdc_sd_card_reg->switch_func_status[i] = p_hndl->status_data[i];
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_GetCardInfo() */

/**********************************************************************************************************************
* Outline      : Get Card Size
* Function Name: r_sdc_sd_get_size
* Description  : Gets the memory card size.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            area                ;   Memory area (bit0:user area, bit1:protect area)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_get_size(uint32_t card_no, uint32_t area)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t            c_mult = 0;
    uint32_t            c_size = 0;
    uint32_t            read_bl_len = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- READ BL LEN ---- */
    read_bl_len = ((p_hndl->csd[1] & SDC_SD_CSD_READ_BL_LEN) >> SDC_SD_BIT_SHIFT_16);
    /* ---- C_SIZE_MULT ---- */
    c_mult = ((p_hndl->csd[2] & SDC_SD_CSD_C_SIZE_MULT) >> SDC_SD_BIT_SHIFT_15);

    if (area & SDC_SD_PROT_AREA)
    {
        /* Calculate the number of all sectors. */
        if (SDC_SD_CSD_STR_VER_2_0 <= p_hndl->csd_structure)
        {
            p_hndl->prot_sector_size = (p_hndl->sdstatus[1]) / SDC_SD_TRANS_BLOCK_SIZE;
        }
        else
        {
            p_hndl->prot_sector_size = ((p_hndl->sdstatus[1] * ((uint32_t)1 << (c_mult + 2))) *
                                        ((uint32_t)1 << read_bl_len)) / SDC_SD_TRANS_BLOCK_SIZE;
        }
    }

    if (area & SDC_SD_USER_AREA)
    {
        if (SDC_SD_CSD_STR_VER_2_0 <= p_hndl->csd_structure)
        {
            c_size = ((p_hndl->csd[1] & SDC_SD_CSD_2_0_C_SIZE_UPPER) << SDC_SD_BIT_SHIFT_16);
            c_size = (c_size | ((p_hndl->csd[2] & SDC_SD_CSD_2_0_C_SIZE_LOWER) >> SDC_SD_BIT_SHIFT_16));
            /* Memory capacity = C_SIZE*SDC_SD_TRANS_BLOCK_SIZE Kbyte */
            /* Sector_size = memory capacity/SDC_SD_TRANS_BLOCK_SIZE */
            p_hndl->card_sector_size = ((c_size + 1) << SDC_SD_BIT_SHIFT_10);
        }
        else
        {
            /* ---- C_SIZE ---- */
            c_size = ((p_hndl->csd[1] & SDC_SD_CSD_1_0_C_SIZE_UPPER) << SDC_SD_BIT_SHIFT_2);
            c_size = (c_size | ((p_hndl->csd[2] & SDC_SD_CSD_1_0_C_SIZE_LOWER) >> SDC_SD_BIT_SHIFT_30));
            /* Calculate the number of all sectors. */
            p_hndl->card_sector_size = (((uint32_t)(c_size + 1) * ((uint32_t)1 << (c_mult + 2))) *
                                        ((uint32_t)1 << read_bl_len)) / SDC_SD_TRANS_BLOCK_SIZE;
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_get_size() */

/**********************************************************************************************************************
* Outline      : Get SD Driver Errors
* Function Name: r_sdc_sd_GetErrCode
* Description  : Gets the SD Driver errors (=p_hndl->error) and returns it
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : Error code                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_GetErrCode(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return p_hndl->error;
} /* End of function r_sdc_sd_GetErrCode() */

/**********************************************************************************************************************
* Outline      : Control SDHI driver
* Function Name: r_sdc_sd_Control
* Description  : Controls the SDHI driver using the specified command.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_cmd_t         * p_sdc_sd_cmd    ;   Command
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_Control(uint32_t card_no, sdc_sd_cmd_t * p_sdc_sd_cmd)
{
    sdc_sd_hndl_t  * p_hndl = 0;
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    uint8_t         tmp_io_flag = 0;
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return SDC_SD_ERR;  /* Not initialized */
    }

    /* ---- Check the card is mounted. ---- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    tmp_io_flag = p_hndl->io_flag & (SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT);
    if ((SDC_SDIO_FUNC_INIT | SDC_SDIO_POWER_INIT) != tmp_io_flag)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sd_cmd)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    switch (p_sdc_sd_cmd->cmd)
    {
        case SDC_SD_SET_STOP:  /* Stop the operation compulsory. */
            p_hndl->stop = 1;
        break;

        default:
            /* NOP */
        break;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_Control() */

/**********************************************************************************************************************
* Outline      : Set Memory
* Function Name: r_sdc_sdmemset
* Description  : Sets memory to the specified data of the specified size from start address.
* Arguments    : uint8_t            * p_ptr             ;   Start address of memory
*              : uint8_t            data                ;   Filling data
*              : uint32_t           cnt                 ;   Filling size
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmemset(uint8_t * p_ptr, uint8_t data, uint32_t cnt)
{
    /* WAIT_LOOP */
    while (cnt--)
    {
        *p_ptr++ = data;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmemset() */

/**********************************************************************************************************************
* Outline      : Copy Memory
* Function Name: r_sdc_sdmemcpy
* Description  : Copies data of the specified size from the specified source address to the specified destination address.
* Arguments    : uint8_t            * p_dst             ;   Destination address
*              : uint8_t            * p_src             ;   Source address
*              : uint32_t           cnt                 ;   Copy size
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmemcpy(uint8_t * p_dst, uint8_t * p_src, uint32_t cnt)
{
    /* WAIT_LOOP */
    while (cnt--)
    {
        *p_dst++ = *p_src++;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmemcpy() */

/**********************************************************************************************************************
* Outline      : Create Volume ID Number
* Function Name: r_sdc_sd_rand
* Description  : Gets the Volume ID Number. The Volume ID number is created by pseudo random number.
* Arguments    : None
* Return Value : uint32_t                               ;   Created Volume ID Number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_rand(void)
{
    g_sdc_sd_next = (g_sdc_sd_next * 1103515245L) + 12345;

    return (uint32_t)g_sdc_sd_next;
} /* End of function r_sdc_sd_rand() */

/**********************************************************************************************************************
* Outline      : Set Volume ID Number
* Function Name: r_sdc_sd_srand
* Description  : Sets the initial value of Volume ID Number
* Arguments    : uint32_t           seed                ;   Initial setting value
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void r_sdc_sd_srand(uint32_t seed)
{
    if (0 == g_sdc_sd_next)
    {
        g_sdc_sd_next = seed;
    }

    return;
} /* End of function r_sdc_sd_srand() */

/**********************************************************************************************************************
* Outline      : Wait for Busy End
* Function Name: r_sdc_sd_wait_rbusy
* Description  : Waits for busy end.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           time                ;   Busy wait interval
* Return Value : SDC_SD_SUCCESS                         ;   Busy finished
*              : SDC_SD_ERR                             ;   Busy
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_wait_rbusy(uint32_t card_no, uint32_t time)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t               i = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* WAIT_LOOP */
    for (i = 0; i < time; ++i) 
    {
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1, p_hndl->rca[0]) == SDC_SD_SUCCESS)
        {
            if (SDC_SD_STATE_TRAN == (p_hndl->resp_status & SDC_SD_RES_STATE))  /* Transfer state */
            {
                return SDC_SD_SUCCESS;
            }
        }
        else    /* SDHI error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
            break;
        }
        if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
            break;
        }
        r_sdc_sd_wait(card_no, 1);
    }

    r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);

    return SDC_SD_ERR;
} /* End of function r_sdc_sd_wait_rbusy() */

/**********************************************************************************************************************
* Outline      : Get Buffer Register Address
* Function Name: r_sdc_sd_GetBuffRegAddress
* Description  : Gets the Buffer Register Address.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           * p_reg_buff        ;   Buffer Register Address
* Return Value : SDC_SD_SUCCES                          ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_GetBuffRegAddress(uint32_t card_no, uint32_t * p_reg_buff)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return SDC_SD_ERR;  /* Not initialized */
    }

    /*  Check the parameter.  */
    if (0 == p_reg_buff)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (R_SDHI_GetBuffRegAddress(p_hndl->channel, p_reg_buff) != SDHI_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_GetBuffRegAddress() */

/**********************************************************************************************************************
* Outline      : Get Version
* Function Name: r_sdc_sd_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t  r_sdc_sd_GetVersion(void)
{
    uint32_t const version = ((RX_SDHI_VERSION_MAJOR << SDC_SD_BIT_SHIFT_16) | RX_SDHI_VERSION_MINOR);

    return version;
} /* End of function r_sdc_sd_GetVersion() */

#ifdef SDC_SD_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
/**********************************************************************************************************************
* Outline      : Get Log HNDL Address
* Function Name: r_sdc_sd_SetLogHdlAddress
* Description  : Gets the Log HNDL Address.
* Arguments    : uint32_t           user_byte_que       ;   user_byte_que
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_SetLogHdlAddress(uint32_t user_long_que)
{
    p_sdc_sd_long_que = (longq_hdl_t)user_long_que;

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_SetLogHdlAddress() */

/**********************************************************************************************************************
* Outline      : Store Debugging Information
* Function Name: r_sdc_sd_log
* Description  : Stores the debugging information.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   SDHI driver file No.
*              : uint32_t           line                ;   SDHI driver line No.
* Return Value : 0      ; Successful
*              : 1      ; Error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_log(uint32_t flg, uint32_t fid, uint32_t line)
{
    longq_err_t err = LONGQ_SUCCESS;
    uint32_t    ul_tmp = 0;

    /* Long to Byte */
    ul_tmp = 0;                                                     /* sdhi_log  Reserved */
    ul_tmp = (ul_tmp | (line << SDC_SD_BIT_SHIFT_8));               /* sdhi_log  LINE */
    ul_tmp = (ul_tmp | (fid  << SDC_SD_BIT_SHIFT_21));              /* sdhi_log  FID FILE No. */
    ul_tmp = (ul_tmp | (SDC_SD_DRIVER_ID << SDC_SD_BIT_SHIFT_27));  /* sdhi_log  FID DRIVER No. */
    ul_tmp = (ul_tmp | (flg  << SDC_SD_BIT_SHIFT_31));              /* sdhi_log  Breakpoint processing */

    /* error log white */
    err = R_LONGQ_Put(p_sdc_sd_long_que, ul_tmp);

    if (err != LONGQ_SUCCESS)
    {
        return 1;
    }

    /* Breakpoint processing */
    if (0x80000000 == (ul_tmp & 0x80000000))
    {
        return 1;
    }

    return 0;
} /* End of function r_sdc_sd_log() */
#else
/**********************************************************************************************************************
* Outline      : Get Log HNDL Address
* Function Name: r_sdc_sd_SetLogHdlAddress
* Description  : Gets the Log HNDL Address.
* Arguments    : uint32_t           user_byte_que       ;   user_byte_que
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_SetLogHdlAddress(uint32_t user_long_que)
{
    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_SetLogHdlAddress() */
#endif /* SDC_SD_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
* Outline      : Store Debugging Information
* Function Name: r_sdc_sd_log
* Description  : Stores the debugging information.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   SDHI driver file No.
*              : uint32_t           line                ;   SDHI driver line No.
* Return Value : 0
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
    SDC_SD_LOG_FUNC(flg, fid, line);
    return 0;
} /* End of function r_sdc_sd_Log() */

/**********************************************************************************************************************
* Outline      : Read from SD Card
* Function Name: r_sdc_sd_read_data
* Description  : Reads the sector data from the data register.
* Arguments    : uint8_t            * p_buff            ;   Read data buffer
*              : uint32_t           reg_addr            ;   Register address
*              : int32_t            num                 ;   Number of data
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_read_data(uint8_t * p_buff, uint32_t reg_addr, int32_t num)
{
    int32_t  i = 0;
    uint32_t * p_reg = (uint32_t *)(reg_addr);
    uint32_t * p_ptr = (uint32_t *)p_buff;
    uint32_t tmp = 0;

    num = (num + 3);
    num = (num / 4);

    if ((uint32_t)p_buff & 0x3)
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
#if (SDC_SD_BIG_ENDIAN)
            tmp = *p_reg;
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
            *p_buff++ = (uint8_t)tmp;
#else
            tmp = *p_reg;
            *p_buff++ = (uint8_t)tmp;
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
#endif
        }
    }
    else
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
            *p_ptr++ = *p_reg;
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_read_data() */

/**********************************************************************************************************************
* Outline      : Write to SD Card
* Function Name: r_sdc_sd_write_data
* Description  : Writes the sector data to the data register.
* Arguments    : uint8_t            * p_buff            ;   Write data buffer
*              : uint32_t           reg_addr            ;   Register address
*              : int32_t            num                 ;   Number of data
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_write_data(uint8_t * p_buff, uint32_t reg_addr, int32_t num)
{
    int32_t  i = 0;
    uint32_t * p_reg = (uint32_t *)(reg_addr);
    uint32_t * p_ptr = (uint32_t *)p_buff;
    uint32_t tmp = 0;

    num = (num + 3);
    num = (num / 4);

    if ((uint32_t)p_buff & 0x3)
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
#if (SDC_SD_BIG_ENDIAN)
            tmp = (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24);
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
            tmp = (tmp | (uint32_t)(*p_buff++));
            *p_reg = tmp;
#else
            tmp = (uint32_t)(*p_buff++);
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24));
            *p_reg = tmp;
#endif
        }
    }
    else
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
            *p_reg = *p_ptr++;
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_write_data() */

/**********************************************************************************************************************
* Outline      : Read from SD Card
* Function Name: r_sdc_sd_read_data_byte
* Description  : Reads the byte data from the data register.
* Arguments    : uint8_t            * p_buff            ;   Read data buffer
*              : uint32_t           reg_addr            ;   Register address
*              : int32_t            num                 ;   Number of data
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_read_data_byte(uint8_t * p_buff, uint32_t reg_addr, int32_t num)
{
    int32_t  i = 0;
    uint32_t * p_reg = (uint32_t *)(reg_addr);
    uint32_t * p_ptr = (uint32_t *)p_buff;
    uint32_t tmp = 0;
    int32_t  franc = 0;

    franc = (num & 0x03);
    num   = (num / 4);

    if (0 != num)
    {
        if ((uint32_t)p_buff & 0x3)
        {
            /* WAIT_LOOP */
            for (i = num ; i > 0 ; i--)
            {
#if (SDC_SD_BIG_ENDIAN)
                tmp = *p_reg;
                *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
                *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
                *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
                *p_buff++ = (uint8_t)tmp;
#else
                tmp = *p_reg;
                *p_buff++ = (uint8_t)tmp;
                *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
                *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
                *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
#endif
            }
        }
        else
        {
            /* WAIT_LOOP */
            for (i = num ; i > 0 ; i--)
            {
                *p_ptr++ = *p_reg;
                p_buff += 4;
            }
        }
    }

    if (0 != franc)
    {
        tmp = *p_reg;
        if (1 == franc)
        {
#if (SDC_SD_BIG_ENDIAN)
            *p_buff = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
#else
            *p_buff = (uint8_t)tmp;
#endif
        }
        else if (2 == franc)
        {
#if (SDC_SD_BIG_ENDIAN)
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
            *p_buff   = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
#else
            *p_buff++ = (uint8_t)tmp;
            *p_buff   = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
#endif
        }
        else
        {
#if (SDC_SD_BIG_ENDIAN)
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_24);
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
            *p_buff   = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
#else
            *p_buff++ = (uint8_t)tmp;
            *p_buff++ = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_8);
            *p_buff   = (uint8_t)(tmp >> SDC_SD_BIT_SHIFT_16);
#endif
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_read_data_byte() */

/**********************************************************************************************************************
* Outline      : Write to SD Card
* Function Name: r_sdc_sd_write_data_byte
* Description  : Writes the byte data to the data register.
* Arguments    : uint8_t            * p_buff            ;   Write data buffer
*              : uint32_t           reg_addr            ;   Register address
*              : int32_t            num                 ;   Number of data
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_write_data_byte(uint8_t * p_buff, uint32_t reg_addr, int32_t num)
{
    int32_t  i = 0;
    uint32_t * p_reg = (uint32_t *)(reg_addr);
    uint32_t * p_ptr = (uint32_t *)p_buff;
    uint32_t tmp = 0;
    int32_t  franc = 0;

    franc = (num & 0x03);
    num   = (num / 4);

    if (0 != num)
    {
        if ((uint32_t)p_buff & 0x3)
        {
            /* WAIT_LOOP */
            for (i = num ; i > 0 ; i--)
            {
#if (SDC_SD_BIG_ENDIAN)
                tmp = (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24);
                tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
                tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
                tmp = (tmp | (uint32_t)(*p_buff++));
                *p_reg = tmp;
#else
                tmp = (uint32_t)(*p_buff++);
                tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
                tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
                tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24));
                *p_reg = tmp;
#endif
            }
        }
        else
        {
            /* WAIT_LOOP */
            for (i = num ; i > 0 ; i--)
            {
                *p_reg = *p_ptr++;
                p_buff += 4;
            }
        }
    }

    if (0 != franc)
    {
        if (1 == franc)
        {
#if (SDC_SD_BIG_ENDIAN)
            tmp = (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24);
#else
            tmp = (uint32_t)(*p_buff++);
#endif
        }
        else if (2 == franc)
        {
#if (SDC_SD_BIG_ENDIAN)
            tmp = (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24);
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
#else
            tmp = (uint32_t)(*p_buff++);
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
#endif
        }
        else
        {
#if (SDC_SD_BIG_ENDIAN)
            tmp = (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_24);
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
#else
            tmp = (uint32_t)(*p_buff++);
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_8));
            tmp = (tmp | (uint32_t)((*p_buff++) << SDC_SD_BIT_SHIFT_16));
#endif
        }
        *p_reg = tmp;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_write_data_byte() */

/**********************************************************************************************************************
* Outline      : Set DMAC/DTC transfer End Flag
* Function Name: r_sdc_sd_SetDmacDtcTransFlg
* Description  : Sets DMAC/DTC transfer end flag.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           flg                 ;   Transfer: 1  No transfer: 0 
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_SetDmacDtcTransFlg(uint32_t card_no, uint32_t flg)
{
    g_sdc_sd_int_dmacdtcflg[card_no] = flg;

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_SetDmacDtcTransFlg() */

/**********************************************************************************************************************
* Outline      : SDHI 1ms Interval
* Function Name: r_sdc_sd_1msInterval
* Description  : 1ms Interval Timer call function.
* Arguments    : None
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void r_sdc_sd_1msInterval(void)
{
    uint32_t card_no = 0;

    /* WAIT_LOOP */
    for (card_no = 0; card_no < SDC_SD_TIMER_CARD_MAX_NUM; card_no++)
    {
        if (0 != g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_FLG])
        {
            g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_COUNT]++;
        }
    }
    return;
} /* End of function r_sdc_sd_1msInterval() */

/**********************************************************************************************************************
* Outline      : SDHI Start timer for timeout
* Function Name: r_sdc_sd_start_timer
* Description  : Set timer for checking timeout and start timer.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           msec                ;   timeout(msec order)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_start_timer(uint32_t card_no, uint32_t msec)
{
    if (SDC_SD_TIMER_CARD_MAX_NUM <= card_no)
    {
        return SDC_SD_ERR;
    }

    /* ---- Check argument ---- */
    if ((0 >= msec) || (msec > 0x0fffffff))
    {
        return SDC_SD_ERR;
    }
    g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_FLG] = 1;
    g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_COUNT] = 0;
    g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_MAX_COUNT] = (uint32_t)(msec);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_start_timer() */

/**********************************************************************************************************************
* Outline      : SDHI Check timeout
* Function Name: r_sdc_sd_check_timer
* Description  : Check timeout set r_sdc_sd_start_timer function.
*              : If timeout,return SDC_SD_ERR.In the case of others, return SDC_SD_SUCCESS.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           msec                ;   timeout(msec order)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_check_timer(uint32_t card_no)
{
    sdc_sd_status_t ret = SDC_SD_SUCCESS;

    /* ---- Check compare ---- */
    if (g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_COUNT] >= 
        g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_MAX_COUNT])
    {
        ret = SDC_SD_ERR;
    }
    else
    {
        ret = SDC_SD_SUCCESS;
    }

    return ret;
} /* End of function r_sdc_sd_check_timer() */

/**********************************************************************************************************************
* Outline      : SDHI End timer for timeout
* Function Name: r_sdc_sd_end_timer
* Description  : End timer set r_sdc_sd_start_timer function.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void r_sdc_sd_end_timer(uint32_t card_no)
{

    g_sdc_sd_timer_cnt_out[card_no][SDC_SD_TIMER_CARD_FLG] = 0;

    return;
} /* End of function r_sdc_sd_end_timer() */

/**********************************************************************************************************************
* Outline      : SDHI API Lock
* Function Name: r_sdc_sd_api_lock
* Description  : Attempts to acquire the lock that has been sent in.
* Arguments    : int32_t    * p_lock                    ;   Pointer to SDHI API lock.
* Return Value : true                                   ;   Lock was acquired.
*              : false                                  ;   Lock was not acquired.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_sdc_sd_api_lock(int32_t * p_lock)
{
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) && (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO))
    bool ret = false;

    /* Variable used in trying to acquire lock. Using the xchg instruction makes this atomic */
    int32_t is_locked = true;

    /* Try to acquire semaphore to obtain lock */
    R_BSP_EXCHANGE(&is_locked, p_lock);

    /* Check to see if semaphore was successfully taken */
    if (is_locked == false)
    {
        /* Lock obtained, return success. */
        ret = true;
    }
    else
    {
        /* Lock was not obtained, another task already has it. */
    }

    return ret;
#else
    return true;
#endif /* ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) && (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)) */
} /* End of function r_sdc_sd_api_lock() */

/**********************************************************************************************************************
* Outline      : SDHI API UnLock
* Function Name: r_sdc_sd_api_unlock
* Description  : Releases hold on lock.
* Arguments    : int32_t    * p_lock                    ;   Pointer to SDHI API lock.
* Return Value : true                                   ;   Lock was released.
*              : false                                  ;   Lock was not released.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_sdc_sd_api_unlock(int32_t * p_lock)
{
#if ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) && (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO))
   /* Set lock back to unlocked. */
    *p_lock = false;

    return true;
#else
    return true;
#endif /* ((SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) && (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)) */
} /* End of function r_sdc_sd_api_unlock() */

/* End of File */
