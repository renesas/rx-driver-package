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
* File Name    : r_sdc_sdio_util.c
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
*              : 30.09.2016 1.21    Changed the judgment of "Read After Write".
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
* Outline      : Set I/O Functions Block Size
* Function Name: r_sdc_sdio_SetBlocklen
* Description  : Sets I/O block size each function.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t        len                     ;   Block size
*              : uint32_t       func                    ;   Function No, (0 to SDC_SDIO_FUNC_MAXNUM)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_SetBlocklen(uint32_t card_no, int32_t len, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdio_chk_param_blocklen(card_no, func))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return SDC_SD_ERR_NO_CARD;
    }

    /* ---- Check media type. ---- */
    if (0 == (p_hndl->media_type & SDC_SD_MEDIA_SDIO))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_TYPE);
        return SDC_SD_ERR_CARD_TYPE;
    }

    /* ---- Check the Support Multiple Block (SMB bit of Card Capability Register in CCCR). ---- */
    if (0 == (p_hndl->io_reg[SDC_SDIO_FNO_0][SDC_SDIO_ADRS_CARDCAPAB] & SDC_SDIO_CARDCAPAB_SMB))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    /* ---- Check the value (32, 64, 128, 256 or 512 bytes) for the SDSIZE register. ---- */
    switch(len)
    {
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
            p_hndl->io_len[func] = len;
        break;

        default:
            p_hndl->io_len[func] = SDC_SDIO_SDSIZE_INVALID;         /* Set to the invalid value. */
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return SDC_SD_ERR;
        break;
    }

    /* ==== Supply the clock. ==== */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->io_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        /* ---- Check the card is in Command state (CMD) using CMD52. ---- */
        if (r_sdc_sdio_check_retry_cmd_state(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
            return SDC_SD_ERR;
        }

        /* ==== Execute I/O Block Size setting operation. ==== */
        if (r_sdc_sdio_set_blocklen_sub(card_no, len, func) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
            return p_hndl->error;
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;
    
} /* End of function r_sdc_sdio_SetBlocklen() */

/**********************************************************************************************************************
* Outline      : I/O Block Size Setting Operation
* Function Name: r_sdc_sdio_set_blocklen_sub
* Description  : Sets the block size each function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t        len                     ;   Block Size
*              : uint32_t       func                    ;   Function No. (0 - 7)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_set_blocklen_sub(uint32_t card_no, int32_t len, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    uint32_t            adr = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    uint8_t             tmp[2] = {0, 0};

    p_hndl = SDC_SD_GET_HNDL(card_no);

    tmp[0] = p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE0];
    tmp[1] = p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE1];

    for (adr = SDC_SDIO_ADRS_BLOCKSIZE0; SDC_SDIO_ADRS_BLOCKSIZE1 >= adr; adr++)
    {
        /* Set the value by Block Size 0 and Block Size 1. */
        io_buff[0] = (uint8_t)len;
        io_buff[1] = io_buff[0];
        len >>= SDC_SD_BIT_SHIFT_8;
        
        /* ---- Set I/O Block Size to block size in CCCR and FBR. ---- */
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = (uint32_t)SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = (uint32_t)(adr + (uint32_t)(0x100 * func));
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE0] = tmp[0];
            p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE1] = tmp[1];
            return p_hndl->error;
        }

        if (io_buff[1] != io_buff[0])
        {
            /* Retry to read */
            io_buff[0] = 0;
            sdc_sdio_daccess.p_buff   = &io_buff[0];
            sdc_sdio_daccess.func     = (uint32_t)SDC_SDIO_FNO_0;
            sdc_sdio_daccess.adr      = (uint32_t)(adr + (uint32_t)(0x100 * func));
            sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
            sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

            if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
                p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE0] = tmp[0];
                p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE1] = tmp[1];
                return p_hndl->error;
            }
            
            if (io_buff[1] != io_buff[0])
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR);
                p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE0] = tmp[0];
                p_hndl->io_reg[func][SDC_SDIO_ADRS_BLOCKSIZE1] = tmp[1];
                return p_hndl->error;
            }
        }

        /* The State is in the Command State (CMD) because of CMD52, so that do not check the state. */
        /* Store the block size. */
        p_hndl->io_reg[func][adr] = io_buff[0];
    }

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_set_blocklen_sub() */

/**********************************************************************************************************************
* Outline      : Get I/O Functions Block Size
* Function Name: r_sdc_sdio_GetBlocklen
* Description  : Gets I/O block size for each function.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t      * p_len                   ;   Block size
*              : uint32_t       func                    ;   Function No, (0 - 7)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_GetBlocklen(uint32_t card_no, int32_t * p_len, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdio_chk_param_blocklen(card_no, func))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_CARD);
        return SDC_SD_ERR_NO_CARD;
    }

    /* ---- Check media type. ---- */
    if (0 == (p_hndl->media_type & SDC_SD_MEDIA_SDIO))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_TYPE);
        return SDC_SD_ERR_CARD_TYPE;
    }

    /* At power-up or reset, default 0000h. */
    /* ---- Check the Support Multiple Block (SMB bit of Card Capability Register in CCCR for all functions). ---- */
    if (0 == (p_hndl->io_reg[SDC_SDIO_FNO_0][SDC_SDIO_ADRS_CARDCAPAB] & SDC_SDIO_CARDCAPAB_SMB))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    /* Store the block size to the buffer. */
    *p_len = p_hndl->io_len[func];

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_GetBlocklen() */

/**********************************************************************************************************************
* Outline      : Read or Write Error Operation for SDIO
* Function Name: r_sdc_sdio_read_write_error
* Description  : Executes the read or write error operation for SDIO.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : uint32_t       func                    ;   Function No, (0 - 7)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_read_write_error(uint32_t card_no, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    volatile sdc_sd_status_t      error = SDC_SD_SUCCESS;
    uint32_t            sdhi_option = 0;
    uint32_t            sdhi_clk_ctrl = 0;
    uint32_t            sdhi_sddma_ctrl = 0;
    uint32_t            sdhi_sdiomd_ctrl = 0;
    uint32_t            arg = 0;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    sdc_sdio_daccess_t sdc_sdio_daccess;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    error = p_hndl->error;

    R_SDC_SD_SetDmacDtcTransFlg(card_no, SDC_SD_SET_TRANS_STOP);

    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    /* ---- Enable All end. ---- */
    r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, 0);
    r_sdc_sd_enable_icu_int(card_no);

    /* ---- Stop data transfer using SDIOMD register for CMD53 operation. ---- */
    io_buff[0]               = (uint8_t)func;
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOABORT;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_WRITE;    /* Because of including W/O register. */

    arg = ((sdc_sdio_daccess.rw_flag  << SDC_SD_BIT_SHIFT_31)
         | (sdc_sdio_daccess.func     << SDC_SD_BIT_SHIFT_28)
         | (sdc_sdio_daccess.raw_flag << SDC_SD_BIT_SHIFT_27)
         | (sdc_sdio_daccess.adr      << SDC_SD_BIT_SHIFT_9)
         | (uint8_t)*sdc_sdio_daccess.p_buff);

    R_SDHI_OutReg(p_hndl->channel, SDHI_SDARG, arg);

    /* Set a IOABT bit using SDIOMD register. */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOMD, (SDHI_SDIOMD_IOABT | SDHI_SDIOMD_INTEN));

    /* ---- Wait for All end. ---- */
    r_sdc_sd_int_err_io_wait(card_no, SDC_SD_TIMEOUT_RESP, sdc_sdio_daccess.func);

    r_sdc_sd_disable_icu_int(card_no);

    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_InReg(p_hndl->channel, SDHI_SDOPT, &sdhi_option);
    R_SDHI_InReg(p_hndl->channel, SDHI_SDCLKCR, &sdhi_clk_ctrl);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDRST, SDHI_SDRST_ENABLE);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDRST, SDHI_SDRST_DISABLE);

    R_SDHI_InReg(p_hndl->channel, SDHI_SDIOMD, &sdhi_sdiomd_ctrl);
    sdhi_sdiomd_ctrl = (sdhi_sdiomd_ctrl & (~SDHI_SDIOMD_IOABT));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOMD, sdhi_sdiomd_ctrl);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_INIT);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDOPT, sdhi_option);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDCLKCR, sdhi_clk_ctrl);

    /* Disable the DMA/DTC. */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
    sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl & (~SDHI_SDDMAEN_DMASDRW));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);

    r_sdc_sd_enable_icu_int(card_no);

    /* ---- Execute I/O Abort (CMD52) operation in order to put into the Command State (CMD). ---- */
    io_buff[0]                 = (uint8_t)func;
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOABORT;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_WRITE;    /* Because of including W/O register. */
    
    r_sdc_sdio_direct(card_no, &sdc_sdio_daccess);

    /* Check I/O Current Status by reading the I/O Ready. */
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOREADY;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) == SDC_SD_SUCCESS)
    {
        /* Check the state is in Command State (CMD). */
        if (SDC_SDIO_STATE_CMD != (p_hndl->resp_status & SDC_SDIO_STATE_RFU))
        {
            /* ---- Put into the Command State (CMD) using I/O Abort (CMD52). ---- */
            io_buff[0]               = (uint8_t)func;
            sdc_sdio_daccess.p_buff   = &io_buff[0];
            sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
            sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOABORT;
            sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
            sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_WRITE;    /* Because of including W/O register. */
            
            r_sdc_sdio_direct(card_no, &sdc_sdio_daccess);
        }
    }

    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_enable_icu_int(card_no);

    p_hndl->error = error;

    return p_hndl->error;

} /* End of function r_sdc_sdio_read_write_error() */

/**********************************************************************************************************************
* Outline      : Check and Re-try Command status (CMD)
* Function Name: r_sdc_sdio_check_retry_cmd_state
* Description  : Checks the card is in the Command status (CMD).
*              : If the error is returned, re-tries checking the card is in the Command status (CMD).
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_check_retry_cmd_state(uint32_t card_no)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    sdc_sdio_daccess_t sdc_sdio_daccess;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Check the card is in Command state (CMD). ---- */
    /* Check I/O Current Status by reading the I/O Ready. */
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOREADY;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) == SDC_SD_SUCCESS)
    {
        /* Check the state is in Command State (CMD). */
        if (SDC_SDIO_STATE_CMD != (p_hndl->resp_status & SDC_SDIO_STATE_RFU))
        {
            p_hndl->error = SDC_SD_ERR;
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            r_sdc_sdio_read_write_error(card_no, sdc_sdio_daccess.func);
            return p_hndl->error;
        }
    }
    else    /* SDHI error */
    {
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOREADY;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) == SDC_SD_SUCCESS)
        {
            /* Check the state is in Command State (CMD). */
            if (SDC_SDIO_STATE_CMD != (p_hndl->resp_status & SDC_SDIO_STATE_RFU))
            {
                p_hndl->error = SDC_SD_ERR;
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
                r_sdc_sdio_read_write_error(card_no, sdc_sdio_daccess.func);
                return p_hndl->error;
            }
        }
        else    /* SDHI error */
        {
             SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
             r_sdc_sdio_read_write_error(card_no, sdc_sdio_daccess.func);
             return p_hndl->error;
        }
    }

    p_hndl->error = SDC_SD_SUCCESS;
    
    return p_hndl->error;
#else

/* When issue the command that SDIO accesses CID, CSD or SCR status
   becomes the SDIO error state.
   This error state is detected when the next command issue and
   returns from the SDIO error state.
   Because these commands is not issued before the call
   of this function, the following command issue to return is unnecessary.
*/

/* #if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_COMBO)         */
/*    uint8_t             io_buff[4] = {0, 0, 0, 0};        */
/*    sdc_sdio_daccess_t sdc_sdio_daccess;                  */
/*    sdc_sdio_daccess.p_buff   = &io_buff[0];              */
/*    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;           */
/*    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOREADY;    */
/*    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;        */
/*    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;     */
/*    r_sdc_sdio_direct(card_no, &sdc_sdio_daccess);        */
/* #endif                                                   */

    return SDC_SD_SUCCESS;
#endif
} /* End of function r_sdc_sdio_check_retry_cmd_state() */

/**********************************************************************************************************************
* Outline      : Control SD Bus Width
* Function Name: r_sdc_sdio_set_port
* Description  : Changes the SDIO bus width.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            port                ;   SDHI_PORT_1BIT, SDHI_PORT_4BIT
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_set_port(uint32_t card_no, int32_t port)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t   sdc_sdio_daccess;
    uint8_t               io_buff[4] = {0, 0, 0, 0};

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ==== Change SDIO bus width and clear pull-up DAT3 using CMD52. ==== */
    if (SDC_SD_MODE_1BIT == port)
    {
        /* Data:80'h */
        io_buff[0] = SDC_SDIO_CD_DIS_BUS_1BIT;     /* CD_Disable & 1-bit bus */
    }
    else
    {
        /* Data:82'h */
        io_buff[0] = SDC_SDIO_CD_DIS_BUS_4BIT;     /* CD_Disable & 4-bit bus */
    }
    io_buff[1] = io_buff[0];
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_BUSINTRFACE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;
    /* Data:80'h or 82'h, Func:0 address:07'h, Verify write */
    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
        return SDC_SD_ERR;
    }

    if (io_buff[1] != (io_buff[0] & SDC_SDIO_MASK_CD_DIS_BUS_WIDTH))
    {
        /* Retry to read */
        io_buff[0] = 0;
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_BUSINTRFACE;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            return p_hndl->error;
        }
        
        if (io_buff[1] != (io_buff[0] & SDC_SDIO_MASK_CD_DIS_BUS_WIDTH))
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_UTIL, __LINE__);
            return p_hndl->error;
        }
    }

    return p_hndl->error;

} /* End of function r_sdc_sdio_set_port() */

#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
