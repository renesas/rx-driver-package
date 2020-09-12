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
* File Name    : r_sdc_sdmem_write.c
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
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sd_rx_if.h"
#include ".\src\r_sdc_sd_rx_private.h"

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdmem_write_sect(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
static sdc_sd_status_t r_sdc_sdmem_write_sect_sub(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);

/**********************************************************************************************************************
* Outline      : Write Data to SD Memory
* Function Name: r_sdc_sdmem_Write
* Description  : Writes data to the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_Write(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_hndl_t * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdmem_chk_param(card_no, p_sdc_sd_access))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Check the buffer boundary (4-byte unit). ---- */
    if (0 != ((uint32_t)p_sdc_sd_access->p_buff & SDC_SD_ADDR_BOUNDARY))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_ADDRESS_BOUNDARY);
        return SDC_SD_ERR_ADDRESS_BOUNDARY;
    }

#if defined(SDC_SD_CFG_FREE_LIB)
    /* ---- Check access area. ---- */
    if ((p_sdc_sd_access->lbn >= SDC_SD_SIZE_CARD_1024MB) ||
        ((p_sdc_sd_access->lbn + p_sdc_sd_access->cnt) > SDC_SD_SIZE_CARD_1024MB))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR; /* Out of area */
    }
#endif /* SDC_SD_CFG_FREE_LIB */

    /* ---- Check the card is mounted. ---- */
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR; /* not mounted yet */
    }

    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check write protect. ---- */
    if (p_hndl->write_protect)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_WP);
        return SDC_SD_ERR_WP;
    }

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
        return SDC_SD_ERR_STOP;
    }

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return SDC_SD_ERR_NO_CARD;
    }

    /* ---- Check the cnt, and set mode to CPU/DMA/DTC transfer. ---- */
    if (p_hndl->trans_mode & SDC_SD_MODE_DMA)
    {
        if (2 == p_sdc_sd_access->cnt)
        {
            p_sdc_sd_access->mode = SDC_SD_MODE_SW;
        }
        else
        {
            p_sdc_sd_access->mode = SDC_SD_MODE_DMA;
        }
    }
    else if (p_hndl->trans_mode & SDC_SD_MODE_DTC)
    {
        if (2 == p_sdc_sd_access->cnt)
        {
            p_sdc_sd_access->mode = SDC_SD_MODE_SW;
        }
        else
        {
            p_sdc_sd_access->mode = SDC_SD_MODE_DTC;
        }
    }
    else
    {
        p_sdc_sd_access->mode = SDC_SD_MODE_SW;
    }

    /* ---- Supply the clock for data-transfer mode. ---- */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->mem_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_enable_icu_int(card_no);

        /* ==== Write data to the SD Memory card. ==== */
        r_sdc_sdmem_write_sect(card_no, p_sdc_sd_access);

        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_enable_icu_int(card_no);
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;
} /* End of function r_sdc_sdmem_Write() */

/**********************************************************************************************************************
* Outline      : Write Data to SD Memory (Software_transfer only)
* Function Name: r_sdc_sdmem_WriteSoftwareTrans
* Description  : Writes data to the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_WriteSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_hndl_t * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdmem_chk_param(card_no, p_sdc_sd_access))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if defined(SDC_SD_CFG_FREE_LIB)
    /* ---- Check access area. ---- */
    if ((p_sdc_sd_access->lbn >= SDC_SD_SIZE_CARD_1024MB) ||
        ((p_sdc_sd_access->lbn + p_sdc_sd_access->cnt) > SDC_SD_SIZE_CARD_1024MB))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR; /* Out of area */
    }
#endif /* SDC_SD_CFG_FREE_LIB */

    /* ---- Check the card is mounted. ---- */
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR; /* not mounted yet */
    }

    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check write protect. ---- */
    if (p_hndl->write_protect)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_WP);
        return SDC_SD_ERR_WP;
    }

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
        return SDC_SD_ERR_STOP;
    }

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return SDC_SD_ERR_NO_CARD;
    }

    p_sdc_sd_access->mode = SDC_SD_MODE_SW;

    /* ---- Supply the clock for data-transfer mode. ---- */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->mem_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_enable_icu_int(card_no);

        /* ==== Write data to the SD Memory card. ==== */
        r_sdc_sdmem_write_sect(card_no, p_sdc_sd_access);

        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
        r_sdc_sd_enable_icu_int(card_no);
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;
} /* End of function r_sdc_sdmem_WriteSoftwareTrans() */

/**********************************************************************************************************************
* Outline      : Write Block Data to SD Memory.
* Function Name: r_sdc_sdmem_write_sect
* Description  : Writes block data to the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdmem_write_sect(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t             j = 0;
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ==== Check the status precede write operation. ==== */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1, p_hndl->rca[0]) == SDC_SD_SUCCESS)
    {
        if (SDC_SD_STATE_TRAN != (p_hndl->resp_status & SDC_SD_RES_STATE))  /* Not in transfer state */
        {
            p_hndl->error = SDC_SD_ERR;
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
    }
    else    /* SDHI error */
    {
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1, p_hndl->rca[0]) == SDC_SD_SUCCESS)
        {
            if (SDC_SD_STATE_TRAN != (p_hndl->resp_status & SDC_SD_RES_STATE))  /* Not in transfer state */
            {
                p_hndl->error = SDC_SD_ERR;
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
                r_sdc_sdmem_read_write_error(card_no);
                return p_hndl->error;
            }
        }
        else    /* SDHI error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
    }

    /* Issue ACMD23 in case of Pre-Erase mode. */
    if ((p_hndl->media_type & SDC_SD_MEDIA_SDMEM) && (SDC_SD_WRITE_WITH_PREERASE == p_sdc_sd_access->write_mode))
    {
        if (r_sdc_sd_send_acmd(card_no, SDC_SD_ACMD23, p_sdc_sd_access->cnt) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
        if (r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R1) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
    }

    /* Transfer size is fixed (SDC_SD_TRANS_BLOCK_SIZE bytes). */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, SDC_SD_SECTOR_SIZE);

    /* In case of 1 or 2 blocks, execute the single write operation. */
    if (2 >= p_sdc_sd_access->cnt)
    {
        /* ---- Clear SDBLKCNTEN in order to disable SDHI_SDBLKCNT. ---- */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_INIT);

        /* WAIT_LOOP */
        for (j = p_sdc_sd_access->cnt; j > 0; j--, p_sdc_sd_access->lbn++,
             p_sdc_sd_access->p_buff = (p_sdc_sd_access->p_buff + SDC_SD_SECTOR_SIZE))
        {
            p_sdc_sd_access->cnt = 0x01;
            ret = r_sdc_sdmem_write_sect_sub(card_no, p_sdc_sd_access);
            if (SDC_SD_SUCCESS != ret)
            {
                break;
            }
        }
    }
    else
    {
        /* ---- Set SDBLKCNTEN in order to enable SDHI_SDBLKCNT. ---- */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_SEC_ENABLE);
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDBLKCNT, (uint32_t)p_sdc_sd_access->cnt);

        ret = r_sdc_sdmem_write_sect_sub(card_no, p_sdc_sd_access);
        if (SDC_SD_SUCCESS != ret)
        {
            return p_hndl->error;
        }
    }

    return p_hndl->error;
} /* End of function r_sdc_sdmem_write_sect() */

/**********************************************************************************************************************
* Outline      : Write Block Data to the SD Memory (Sub-module)
* Function Name: r_sdc_sdmem_write_sect_sub
* Description  : Writes block data to the SD Memory using CMD24 or CMD25.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdmem_write_sect_sub(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    sdc_sd_status_t   trans_ret = SDC_SD_SUCCESS;
    uint32_t        sdsts1_back = 0;
    uint8_t         wb[4] = {0, 0, 0, 0};
    uint32_t        writeblock = 0;
    uint32_t        set_acc_addr = 0;
    uint32_t        sdhi_sddma_ctrl = 0;
    p_hndl = SDC_SD_GET_HNDL(card_no);

    if ((SDC_SD_MODE_DMA | SDC_SD_MODE_DTC) & p_sdc_sd_access->mode)
    {
        R_SDC_SD_SetDmacDtcTransFlg(card_no, SDC_SD_SET_TRANS_START);

        /* Enable the DMA/DTC. */
        R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
        sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl | SDHI_SDDMAEN_DMASDRW);
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);
    }

    /* ---- Sets block address in case of SDHC/SDXC card. ---- */
    if (SDC_SD_CSD_STR_VER_2_0 <= p_hndl->csd_structure)
    {
        set_acc_addr = p_sdc_sd_access->lbn;
    }
    else
    {
        set_acc_addr = (p_sdc_sd_access->lbn * SDC_SD_TRANS_BLOCK_SIZE);
    }

    if (0x01 == p_sdc_sd_access->cnt)
    {
        /* Issue CMD24 (WRITE_SINGLE_BLOCK). */
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD24, SDC_SD_RESP_R1, set_acc_addr) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
    }
    else
    {
        /* Issue CMD25 (WRITE_MULTIPLE_BLOCK). */
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD25, SDC_SD_RESP_R1, set_acc_addr) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
    }

    if (SDC_SD_MODE_SW == p_sdc_sd_access->mode)
    {
        /* Enable All end, BWE and errors. */
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BWE);
        r_sdc_sd_enable_icu_int(card_no);
        /* PIO */
        trans_ret = r_sdc_sd_software_trans(card_no, p_sdc_sd_access->p_buff, p_sdc_sd_access->cnt, SDC_SD_TRANS_WRITE);
    }
    else    /* ==== DMA or DTC ==== */
    {
        /* Disable card ins&rem interrupt. */
        r_sdc_sd_disable_icu_int(card_no);
        sdsts1_back = (uint32_t)(p_hndl->sdimsk1 & SDHI_SDIMSK1_DET_CD);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DET_CD, 0);
        /* Enable All errors. */
        r_sdc_sd_set_int_mask(card_no, 0, SDHI_SDIMSK2_ERR);
        r_sdc_sd_enable_icu_int(card_no);

        /* DMA or DTC data transfer */
        trans_ret = r_sdc_sd_dmacdtc_trans(card_no, p_sdc_sd_access->cnt);
        /* Disable the DMA/DTC. */
        R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
        sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl & (~SDHI_SDDMAEN_DMASDRW));
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);

        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, 0);
        r_sdc_sd_enable_icu_int(card_no);
    }

    /* ---- Wait All end interrupt. ---- */
    ret = r_sdc_sd_int_mem_wait(card_no, SDC_SD_TIMEOUT_RESP);

    if ((SDC_SD_MODE_DMA | SDC_SD_MODE_DTC) & p_sdc_sd_access->mode)
    {
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_set_int_mask(card_no, sdsts1_back, 0);
        r_sdc_sd_enable_icu_int(card_no);
    }

    /* ---- Check the result of transfer. ---- */
    if (SDC_SD_SUCCESS != trans_ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return p_hndl->error;
    }

    /* ---- Check the result of wait All end interrupt. ---- */
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
        r_sdc_sdmem_read_write_error(card_no);
        return p_hndl->error;
    }

    /* ---- Check errors. ---- */
    if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sd_check_sdsts2_err(card_no);
        r_sdc_sdmem_read_write_error(card_no);
        return p_hndl->error;
    }

    /* Disable All end, BWE and errors. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BWE);
    r_sdc_sd_enable_icu_int(card_no);

    /* Clear All end bits. */
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, 0);

    if (0x01 != p_sdc_sd_access->cnt)
    {
        if (p_hndl->media_type & SDC_SD_MEDIA_SDMEM)
        {
            /* Check the response after CMD12. */
            if(r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R1) != SDC_SD_SUCCESS)
            {
                /* Check the number of write complete block. */
                if(r_sdc_sdmem_read_byte(card_no, SDC_SD_ACMD22, 0, wb, 4) != SDC_SD_SUCCESS)
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
                    r_sdc_sdmem_read_write_error(card_no);
                    return p_hndl->error;
                }
                /* Type cast (uint32_t) remove compiler dependence. */
                writeblock = ((uint32_t)wb[0] << SDC_SD_BIT_SHIFT_24) | 
                             ((uint32_t)wb[1] << SDC_SD_BIT_SHIFT_16) | 
                             ((uint32_t)wb[2] << SDC_SD_BIT_SHIFT_8)  |
                             (uint32_t)wb[3];
                if (p_sdc_sd_access->cnt != writeblock)  /* No write complete block */
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
                    r_sdc_sd_set_err(card_no, SDC_SD_ERR);
                    r_sdc_sdmem_read_write_error(card_no);
                    return p_hndl->error;
                }
            }
        }
    }

    /* ==== Check the status after write operation. ==== */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1, p_hndl->rca[0]) != SDC_SD_SUCCESS)
    {
        /* Check OUT_OF_RANGE error. */
        /* ignore errors during last block access */
        if (p_hndl->resp_status & SDC_SD_RES_ERR)
        {
            if ((p_sdc_sd_access->lbn + p_sdc_sd_access->cnt) != p_hndl->card_sector_size)
            {
                /* Not last block */
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
                r_sdc_sdmem_read_write_error(card_no);
                return p_hndl->error;
            }
            if (p_hndl->resp_status & SDC_SD_RES_ERR_IGNORE_OUT_OF_RAGE)
            {
                /* Not OUT_OF_RANGE error */
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
                r_sdc_sdmem_read_write_error(card_no);
                return p_hndl->error;
            }
            /* Clear OUT_OF_RANGE error. */
            p_hndl->resp_status = (p_hndl->resp_status & (SDC_SD_RES_STATE | SDC_SD_RES_READY_FOR_DATA));
            p_hndl->error = SDC_SD_SUCCESS;
        }
        else    /* SDHI error, ex)timeout error so on */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return p_hndl->error;
        }
    }

    if (SDC_SD_STATE_TRAN != (p_hndl->resp_status & SDC_SD_RES_STATE))
    {
        p_hndl->error = SDC_SD_ERR;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRITE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
    }

    return p_hndl->error;
} /* End of function r_sdc_sdmem_write_sect_sub() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */

/* End of File */
