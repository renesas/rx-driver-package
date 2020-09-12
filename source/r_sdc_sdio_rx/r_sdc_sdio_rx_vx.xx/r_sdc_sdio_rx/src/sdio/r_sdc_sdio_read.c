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
* File Name    : r_sdc_sdio_read.c
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
* Outline      : I/O Read Extended Operation
* Function Name: r_sdc_sdio_Read
* Description  : Executes I/O Read Extended operation from the specified address using CMD53.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : sdc_sdio_access_t * p_sdc_sdio_access
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function number
*              :    uint32_t    adr                     ;   Register address
*              :    int32_t     cnt                     ;   Number of read (byte)
*              :    uint32_t    op_code                 ;   Operation code
*              :    uint32_t    trans_mode              ;   Transfer mode (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_Read(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    int32_t             block_size = 0;

    if (SDC_SD_ERR == r_sdc_sdio_chk_param_extend(card_no, p_sdc_sdio_access))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
        return SDC_SD_ERR_STOP;
    }

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return SDC_SD_ERR_NO_CARD;
    }

    /* ---- Check io_len[]. ---- */
    if ((p_sdc_sdio_access->op_code & SDC_SDIO_BLOCK_MODE))
    {
        if ((0 == p_hndl->io_len[p_sdc_sdio_access->func]) ||
            (SDC_SDIO_SDSIZE_INVALID == p_hndl->io_len[p_sdc_sdio_access->func]))
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_BLOCK_LEN_ERROR);
            return SDC_SD_ERR_BLOCK_LEN_ERROR;
        }
    }

    /* ---- Check media type. ---- */
    if (0 == (p_hndl->media_type & SDC_SD_MEDIA_SDIO))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_TYPE);
        return SDC_SD_ERR_CARD_TYPE;
    }

    /* ---- Check the mode, and set mode to PIO/DMA/DTC transfer. ---- */
    if ((p_hndl->trans_mode & SDC_SD_MODE_DMA) && (0 == ((uint32_t)p_sdc_sdio_access->p_buff & SDC_SD_ADDR_BOUNDARY)))
    {
        p_sdc_sdio_access->trans_mode = SDC_SD_MODE_DMA;
    }
    else if ((p_hndl->trans_mode & SDC_SD_MODE_DTC) && 
             (0 == ((uint32_t)p_sdc_sdio_access->p_buff & SDC_SD_ADDR_BOUNDARY)))
    {
        p_sdc_sdio_access->trans_mode = SDC_SD_MODE_DTC;
    }
    else
    {
        p_sdc_sdio_access->trans_mode = SDC_SD_MODE_SW;
    }

    if (SDC_SD_MODE_SW != p_sdc_sdio_access->trans_mode)  /* ==== DMA or DTC ==== */
    {
        if (SDC_SDIO_BLOCK_MODE == (p_sdc_sdio_access->op_code & SDC_SDIO_BLOCK_MODE))
        {
            if (0 != (p_sdc_sdio_access->cnt % p_hndl->io_len[p_sdc_sdio_access->func]))
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR);
                return p_hndl->error;
            }
        }
        else /* Byte Mode */
        {
            block_size = r_sdc_sdio_get_block_size(p_sdc_sdio_access->func);
            if (block_size != p_sdc_sdio_access->cnt)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR);
                return p_hndl->error;
            }
        }
    }

    /* ==== Execute I/O Read Extended operation. ---- */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->io_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        /* ---- Check the card is in Command state (CMD) using CMD52. ---- */
        if (r_sdc_sdio_check_retry_cmd_state(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
            return SDC_SD_ERR;
        }

        /* ---- Executes I/O Read Extended operation. ---- */
        if ((p_sdc_sdio_access->op_code & SDC_SDIO_BLOCK_MODE))
        {
            R_BSP_NOP();    /* Because a compiler doesn't put an assembler out. */
            r_sdc_sdio_read_block_sub(card_no, p_sdc_sdio_access);
        }
        else
        {
            R_BSP_NOP();    /* Because a compiler doesn't put an assembler out. */
            r_sdc_sdio_read_byte_sub(card_no, p_sdc_sdio_access);
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;
    
} /* End of function r_sdc_sdio_Read() */

/**********************************************************************************************************************
* Outline      : I/O Read Extended Operation (Software_transfer only)
* Function Name: r_sdc_sdio_ReadSoftwareTrans
* Description  : Executes I/O Read Extended operation from the specified address using CMD53 by Software transfer.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : sdc_sdio_access_t * p_sdc_sdio_access
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function number
*              :    uint32_t    adr                     ;   Register address
*              :    int32_t     cnt                     ;   Number of read (byte)
*              :    uint32_t    op_code                 ;   Operation code
*              :    uint32_t    trans_mode              ;   Transfer mode (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_ReadSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdio_chk_param_extend(card_no, p_sdc_sdio_access))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
        return SDC_SD_ERR_STOP;
    }

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return SDC_SD_ERR_NO_CARD;
    }

    /* ---- Check io_len[]. ---- */
    if ((p_sdc_sdio_access->op_code & SDC_SDIO_BLOCK_MODE))
    {
        if ((0 == p_hndl->io_len[p_sdc_sdio_access->func]) ||
            (SDC_SDIO_SDSIZE_INVALID == p_hndl->io_len[p_sdc_sdio_access->func]))
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_BLOCK_LEN_ERROR);
            return SDC_SD_ERR_BLOCK_LEN_ERROR;
        }
    }

    /* ---- Check media type. ---- */
    if (0 == (p_hndl->media_type & SDC_SD_MEDIA_SDIO))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_TYPE);
        return SDC_SD_ERR_CARD_TYPE;
    }

    p_sdc_sdio_access->trans_mode = SDC_SD_MODE_SW;

    /* ==== Execute I/O Read Extended operation. ---- */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->io_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        /* ---- Check the card is in Command state (CMD) using CMD52. ---- */
        if (r_sdc_sdio_check_retry_cmd_state(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
            return SDC_SD_ERR;
        }

        /* ---- Executes I/O Read Extended operation. ---- */
        if ((p_sdc_sdio_access->op_code & SDC_SDIO_BLOCK_MODE))
        {
            R_BSP_NOP();    /* Because a compiler doesn't put an assembler out. */
            r_sdc_sdio_read_block_sub(card_no, p_sdc_sdio_access);
        }
        else
        {
            R_BSP_NOP();    /* Because a compiler doesn't put an assembler out. */
            r_sdc_sdio_read_byte_sub(card_no, p_sdc_sdio_access);
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;

} /* End of function r_sdc_sdio_ReadSoftwareTrans() */

/**********************************************************************************************************************
* Outline      : I/O Read using Block Mode and Byte Mode
* Function Name: r_sdc_sdio_read_block_sub
* Description  : Reads data of the multiple block length using CMD53 Block mode and
*                reads the rest data using CMD53 Byte mode.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sdio_access_t * p_sdc_sdio_access
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function number
*              :    uint32_t    adr                     ;   Register address
*              :    int32_t     cnt                     ;   Number of read (byte)
*              :    uint32_t    op_code                 ;   Operation code
*              :    uint32_t    trans_mode              ;   Transfer mode (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_read_block_sub(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    int32_t             i = 0;
    int32_t             block_cnt = 0;
    int32_t             rem = 0;
    int32_t             len = 0;
    uint32_t            arg = 0;
    uint32_t            sdsts1_back = 0;
    uint32_t            sdhi_sddma_ctrl = 0;
    uint32_t            op_code_addr = 0;
    sdc_sd_status_t       trans_ret = SDC_SD_SUCCESS;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* User-defined size using r_sdc_sdio_SetBlocklen(). */
    len = p_hndl->io_len[p_sdc_sdio_access->func];

    /* Calculate block count and remaining bytes from len. */
    block_cnt = p_sdc_sdio_access->cnt / len;
    rem = p_sdc_sdio_access->cnt % len;

    if (block_cnt)    /* cnt >= Blocklen size */
    {
        /* ==== Multiple Block Read by Blocklen size ==== */
        /* ---- Executes read operation using CMD53 Block mode. ---- */
        /* Transfer size is fixed (len bytes). */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, (uint32_t)len);

        /* loop during remaining bytes are more than Blocklen size */
        for (i = block_cnt; i > 0; i = (i - SDC_SDIO_TRANS_SECTORS))
        {
            /* ---- Check the card insertion. ---- */
            if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);  /* no card */
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            /* Enable SDBLKCNTEN in order to execute I/O Abort operation for SDIO Multi-block transfer using CMD53.
               Refer to "Writing to the SDSTOP Register During a Multi-Block Read Sequence" of UM. */
            R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_SEC_ENABLE);

            /* Set transfer block numbers to SDHI_SDBLKCNT. */
            block_cnt = i - SDC_SDIO_TRANS_SECTORS;
            if (block_cnt < 0)    /* Remaining blocks are less than SDC_SDIO_TRANS_SECTORS. */
            {
                block_cnt = i;
            }
            else
            {
                block_cnt = SDC_SDIO_TRANS_SECTORS;
            }

            R_SDHI_OutReg(p_hndl->channel, SDHI_SDBLKCNT, (uint32_t)block_cnt);

            if (SDC_SD_MODE_SW != p_sdc_sdio_access->trans_mode)  /* ==== DMA or DTC ==== */
            {
                R_SDC_SD_SetDmacDtcTransFlg(card_no, SDC_SD_SET_TRANS_START);
                /* Enable the DMA/DTC. */
                R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
                sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl | SDHI_SDDMAEN_DMASDRW);
                R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);
            }

            /* Set parameters with Block Mode to arg. */
            op_code_addr = (p_sdc_sdio_access->op_code & SDC_SDIO_INCREMENT_ADDR);
            arg = ((p_sdc_sdio_access->func << SDC_SD_BIT_SHIFT_28)
                 | SDC_SDIO_BLOCKMODE_RWFLAG_R
                 | (op_code_addr << SDC_SD_BIT_SHIFT_26)
                 | (p_sdc_sdio_access->adr << SDC_SD_BIT_SHIFT_9)
                 | (uint32_t)block_cnt);

            /* Issue CMD53 Block Mode. */
            if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD53_R_BLOCK, SDC_SD_RESP_R5, arg) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            if (SDC_SD_MODE_SW == p_sdc_sdio_access->trans_mode)
            {
                /* Enable All end, BRE and errors. */
                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
                r_sdc_sd_enable_icu_int(card_no);
                /* Software data transfer
                   BRE bit is cleared in the following function. */
                trans_ret = r_sdc_sd_software_trans_blk(card_no,
                                                        p_sdc_sdio_access->p_buff,
                                                        block_cnt, SDC_SD_TRANS_READ,
                                                        len,
                                                        p_sdc_sdio_access->func);
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

                /* DMA or DTC transfer */
                trans_ret = r_sdc_sd_dmacdtc_trans(card_no, block_cnt);

                /* Disable the DMA/DTC. */
                R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
                sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl & (~SDHI_SDDMAEN_DMASDRW));
                R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);

                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_set_int_mask(card_no, (sdsts1_back | SDHI_SDIMSK1_DATA_TRNS), 0);
                r_sdc_sd_enable_icu_int(card_no);
            }

            if (SDC_SD_SUCCESS != trans_ret)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            /* ---- Wait All end interrupt. ---- */
            if (r_sdc_sd_int_io_wait(card_no, SDC_SD_TIMEOUT_RESP, p_sdc_sdio_access->func) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }
            /* ---- Check errors. ---- */
            if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_check_sdsts2_err(card_no);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            /* Disable All end, BRE and errors. */
            r_sdc_sd_disable_icu_int(card_no);
            r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
            r_sdc_sd_enable_icu_int(card_no);

            /* Clear All end bit. 
               This is no problem. Surely, clear BRE and errors, too. */
/*          r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, 0); */

            /* Clear All end, BRE and errors. 
               SDHI_SDSTS1 and SDHI_SDSTS2 are already cleared in interrupt handler.*/
/*          R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_BRE); */
            r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);

            if (SDC_SDIO_INCREMENT_ADDR == op_code_addr)
            {
                p_sdc_sdio_access->adr = (p_sdc_sdio_access->adr + (uint32_t)((uint32_t)block_cnt * (uint32_t)len));
            }
            p_sdc_sdio_access->p_buff = (p_sdc_sdio_access->p_buff + (uint32_t)((uint32_t)block_cnt * (uint32_t)len));

        }
    }

    if (rem)    /* The remaining bytes < Blocklen size */
    {
        /* ==== Multiple Byte Read ==== */
        /* ---- Executes read operation using CMD53 Byte mode. ---- */
        p_sdc_sdio_access->trans_mode = SDC_SD_MODE_SW;   /* just support PIO */

        /* Enable SDBLKCNTEN in order to execute I/O Abort operation for SDIO Multi-block transfer using CMD53.
           Refer to "Writing to the SDSTOP Register During a Multi-Block Read Sequence" of UM. */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_SEC_ENABLE);

        /* ---- Set transfer size. ---- */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, (uint32_t)rem);

        /* Set parameters with Byte Mode to arg. */
        op_code_addr = (p_sdc_sdio_access->op_code & SDC_SDIO_INCREMENT_ADDR);
        arg = ((p_sdc_sdio_access->func << SDC_SD_BIT_SHIFT_28)
             | SDC_SDIO_BYTEMODE_RWFLAG_R
             | (op_code_addr << SDC_SD_BIT_SHIFT_26)
             | (p_sdc_sdio_access->adr << SDC_SD_BIT_SHIFT_9)
             | (uint32_t)rem);
               
        /* Issue CMD53 Byte mode. */
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD53_R_BYTE, SDC_SD_RESP_R5, arg) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* Enable All end, BRE and errors. */
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
        r_sdc_sd_enable_icu_int(card_no);

        /* Software data transfer
           BRE bit is cleared in the following function. */
        trans_ret = r_sdc_sd_software_trans_blk(card_no, p_sdc_sdio_access->p_buff, 1, SDC_SD_TRANS_READ, rem,
                                              p_sdc_sdio_access->func);

        if (SDC_SD_SUCCESS != trans_ret)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* Wait All end interrupt. */
        if (r_sdc_sd_int_io_wait(card_no, SDC_SD_TIMEOUT_RESP, p_sdc_sdio_access->func) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* ---- Check errors. ---- */
        if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_check_sdsts2_err(card_no);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* Disable All end, BRE and errors */
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
        r_sdc_sd_enable_icu_int(card_no);

        /* Clear All end bit. 
           This is no problem. Surely, clear BRE and errors, too. */
/*      r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, 0); */

        /* Clear All end, BRE and errors. 
           SDHI_SDSTS1 and SDHI_SDSTS2 are already cleared in interrupt handler.*/
/*      R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_BRE); */
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
    }

    return p_hndl->error;

} /* End of function r_sdc_sdio_read_block_sub() */

/**********************************************************************************************************************
* Outline      : I/O Read using Byte Mode Only
* Function Name: r_sdc_sdio_read_byte_sub
* Description  : Reads data using CMD53 Byte mode only.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : sdc_sdio_access_t * p_sdc_sdio_access
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function number
*              :    uint32_t    adr                     ;   Register address
*              :    int32_t     cnt                     ;   Number of read (byte)
*              :    uint32_t    op_code                 ;   Operation code
*              :    uint32_t    trans_mode              ;   Transfer mode (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_read_byte_sub(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    int32_t             i = 0;
    int32_t             byte_cnt = 0;
    int32_t             rem = 0;
    int32_t             block_size = 0;
    uint32_t            arg = 0;
    uint32_t            sdsts1_back = 0;
    uint32_t            sdhi_sddma_ctrl = 0;
    uint32_t            op_code_addr = 0;
    sdc_sd_status_t       trans_ret = SDC_SD_SUCCESS;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* I/O Block Size of Tuple information */
    block_size = r_sdc_sdio_get_block_size(p_sdc_sdio_access->func);

    /* Calculate block count and remaining bytes. */
    byte_cnt = p_sdc_sdio_access->cnt / block_size;
    rem = p_sdc_sdio_access->cnt % block_size;

    if (byte_cnt)    /* cnt >= I/O Block Size */
    {
        /* ==== Multiple Byte Read by I/O Block_Size ==== */
        /* ---- Executes read operation using CMD53 Byte mode. ---- */
        /* Transfer size is fixed (block_size bytes). */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, (uint32_t)block_size);

        /* loop during remaining bytes are more than I/O Block Size */
        for (i = byte_cnt; i > 0; i--)
        {
            /* ---- Check the card insertion. ---- */
            if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            /* Enable SDBLKCNTEN in order to execute I/O Abort operation for SDIO Multi-block transfer using CMD53.
               Refer to "Writing to the SDSTOP Register During a Multi-Block Read Sequence" of UM. */
            R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_SEC_ENABLE);

            if (SDC_SD_MODE_SW != p_sdc_sdio_access->trans_mode)   /* ==== DMA or DTC ==== */
            {
                R_SDC_SD_SetDmacDtcTransFlg(card_no, SDC_SD_SET_TRANS_START);
                /* Enable the DMA/DTC. */
                R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
                sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl | SDHI_SDDMAEN_DMASDRW);
                R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);
            }

            /* Set argument with Byte Mode and Byte Count = I/O Block Size. */
            op_code_addr = (p_sdc_sdio_access->op_code & SDC_SDIO_INCREMENT_ADDR);
            arg = ((p_sdc_sdio_access->func << SDC_SD_BIT_SHIFT_28)
                 | SDC_SDIO_BYTEMODE_RWFLAG_R
                 | (op_code_addr << SDC_SD_BIT_SHIFT_26)
                 | (p_sdc_sdio_access->adr << SDC_SD_BIT_SHIFT_9));

            /* Set argument with the byte count values.
               If the count is the 512 bytes, set 000h to argument.
               If the count is less than 512 bytes, set the byte count values to argument.   */
            if (SDC_SDIO_SDSIZE_512 > block_size)
            {
                arg |= (uint32_t)block_size;
            }

            /* Issue CMD53 Byte mode. */
            if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD53_R_BYTE, SDC_SD_RESP_R5, arg) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            if (SDC_SD_MODE_SW == p_sdc_sdio_access->trans_mode)
            {
                /* Enable All end, BRE and errors. */
                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
                r_sdc_sd_enable_icu_int(card_no);
                /* Software data transfer
                   BRE bit is cleared in the following function. */
                trans_ret = r_sdc_sd_software_trans_blk(card_no,
                                                        p_sdc_sdio_access->p_buff,
                                                        1,
                                                        SDC_SD_TRANS_READ, block_size,
                                                        p_sdc_sdio_access->func);
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

                /* DMA or DTC transfer */
                trans_ret = r_sdc_sd_dmacdtc_trans(card_no, byte_cnt);

                /* Disable the DMA/DTC. */
                R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
                sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl & (~SDHI_SDDMAEN_DMASDRW));
                R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);

                r_sdc_sd_disable_icu_int(card_no);
                r_sdc_sd_set_int_mask(card_no, (sdsts1_back | SDHI_SDIMSK1_DATA_TRNS), 0);
                r_sdc_sd_enable_icu_int(card_no);
            }

            /* ---- Check the result of transfer. ---- */
            if (SDC_SD_SUCCESS != trans_ret)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            /* ---- Wait All end interrupt. ---- */
            if (r_sdc_sd_int_io_wait(card_no, SDC_SD_TIMEOUT_RESP, p_sdc_sdio_access->func) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }
            /* ---- Check errors. ---- */
            if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
                r_sdc_sd_check_sdsts2_err(card_no);
                r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
                return p_hndl->error;
            }

            /* Disable All end, BRE and errors */
            r_sdc_sd_disable_icu_int(card_no);
            r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
            r_sdc_sd_enable_icu_int(card_no);

            /* Clear All end bit. 
               This is no problem. Surely, clear BRE and errors, too. */
/*          r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, 0); */

            /* Clear All end, BRE and errors. 
               SDHI_SDSTS1 and SDHI_SDSTS2 are already cleared in interrupt handler.*/
/*          R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_BRE); */
            r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);

            if (SDC_SDIO_INCREMENT_ADDR == op_code_addr)
            {
                p_sdc_sdio_access->adr = (p_sdc_sdio_access->adr + (uint32_t)block_size);
            }
            p_sdc_sdio_access->p_buff = (p_sdc_sdio_access->p_buff + (uint32_t)block_size);
        }
    }

    if (rem)    /* The remaining bytes < I/O Block Size */
    {
        /* ==== Multiple Byte Read ==== */
        /* ---- Executes read operation using CMD53 Byte mode. ---- */
        p_sdc_sdio_access->trans_mode = SDC_SD_MODE_SW;   /* just support PIO */

        /* Enable SDBLKCNTEN in order to execute I/O Abort operation for SDIO Multi-block transfer using CMD53.
           Refer to "Writing to the SDSTOP Register During a Multi-Block Read Sequence" of UM. */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_SEC_ENABLE);

        /* ---- Set transfer size. ---- */
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, (uint32_t)rem);

        /* Set argument with Byte Mode. */
        op_code_addr = (p_sdc_sdio_access->op_code & SDC_SDIO_INCREMENT_ADDR);
        arg = ((p_sdc_sdio_access->func << SDC_SD_BIT_SHIFT_28)
             | SDC_SDIO_BYTEMODE_RWFLAG_R
             | (op_code_addr << SDC_SD_BIT_SHIFT_26)
             | (p_sdc_sdio_access->adr << SDC_SD_BIT_SHIFT_9)
             | (uint32_t)rem);
               
        /* Issue CMD53 Byte mode. */
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD53_R_BYTE, SDC_SD_RESP_R5, arg) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* Enable All end, BRE and errors. */
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
        r_sdc_sd_enable_icu_int(card_no);

        /* Software data transfer
           BRE bit is cleared in the following function. */
        trans_ret = r_sdc_sd_software_trans_blk(card_no,
                                                p_sdc_sdio_access->p_buff,
                                                1,
                                                SDC_SD_TRANS_READ,
                                                rem,
                                                p_sdc_sdio_access->func);

        if (SDC_SD_SUCCESS != trans_ret)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* Wait All end interrupt. */
        if (r_sdc_sd_int_io_wait(card_no, SDC_SD_TIMEOUT_RESP, p_sdc_sdio_access->func) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* ---- Check errors. ---- */
        if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_READ, __LINE__);
            r_sdc_sd_check_sdsts2_err(card_no);
            r_sdc_sdio_read_write_error(card_no, p_sdc_sdio_access->func);
            return p_hndl->error;
        }

        /* Disable All end, BRE and errors */
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
        r_sdc_sd_enable_icu_int(card_no);

        /* Clear All end bit. 
           This is no problem. Surely, clear BRE and errors, too. */
/*      r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, 0); */

        /* Clear All end, BRE and errors. 
           SDHI_SDSTS1 and SDHI_SDSTS2 are already cleared in interrupt handler.*/
/*      R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_BRE); */
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
    }

    return p_hndl->error;

} /* End of function r_sdc_sdio_read_byte_sub() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
