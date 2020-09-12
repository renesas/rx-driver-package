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
* File Name    : r_sdc_sd_trans.c
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
* Outline      : Transfer Data using PIO Mode
* Function Name: r_sdc_sd_software_trans
* Description  : Transfers data to/from the SD card by PIO mode
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint8_t            * p_buff            ;   Destination/Source data buffer
*              : int32_t            cnt                 ;   Number of transfer bytes
*              : int32_t            dir                 ;   Transfer direction
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_software_trans(uint32_t card_no, uint8_t * p_buff, int32_t cnt, int32_t dir)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t             j = 0;
    sdc_sd_status_t   (* func)(uint8_t *, uint32_t, int32_t);
    uint32_t        sd_buff_addr = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_TRANS_READ == dir)
    {
        func = r_sdc_sd_read_data;
    }
    else
    {
        func = r_sdc_sd_write_data;
    }

    R_SDHI_GetBuffRegAddress(p_hndl->channel, &sd_buff_addr);

    /* WAIT_LOOP */
    for (j = cnt; j > 0; j--)
    {
        /* ---- Wait for BWE/BRE interrupt. ---- */
        if (r_sdc_sd_int_mem_wait(card_no, SDC_SD_TIMEOUT_MULTIPLE) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
            break;
        }

        if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_check_sdsts2_err(card_no);
            break;
        }
        if (SDC_SD_TRANS_READ == dir)
        {
            r_sdc_sd_clear_sdsts(card_no, 0, SDHI_SDIMSK2_RE); /* Clear a BRE bit. */
        }
        else
        {
            r_sdc_sd_clear_sdsts(card_no, 0, SDHI_SDIMSK2_WE); /* Clear a BWE bit. */
        }
        /* Write/Read to/from SDHI_BUF by 1 sector. */
        if ((*func)(p_buff, sd_buff_addr, SDC_SD_TRANS_BLOCK_SIZE) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_CPU_IF);
            break;
        }
        /* Update buffer. */
        p_buff = (p_buff + SDC_SD_TRANS_BLOCK_SIZE);
        
        /* ---- Check the stop flag. ---- */
        if (p_hndl->stop)
        {
            p_hndl->stop = 0;
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
            break;
        }
    }

    return p_hndl->error;
} /* End of function r_sdc_sd_software_trans() */

/**********************************************************************************************************************
* Outline      : Transfer Data using PIO Mode
* Function Name: r_sdc_sd_software_trans_blk
* Description  : Transfers data to/from the SD card by PIO mode
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint8_t            * p_buff            ;   Destination/Source data buffer
*              : int32_t            cnt                 ;   Number of transfer bytes
*              : int32_t            dir                 ;   Transfer direction
*              : int32_t            blk                 ;   block size
*              : uint32_t           func                ;   Function number
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_software_trans_blk(uint32_t card_no, uint8_t * p_buff, int32_t cnt, int32_t dir, int32_t blk,
                                            uint32_t func_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t                j = 0;
    sdc_sd_status_t   (* func)(uint8_t *, uint32_t, int32_t);
    uint32_t        sd_buff_addr = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_TRANS_READ == dir)
    {
        func = r_sdc_sd_read_data_byte;
    }
    else
    {
        func = r_sdc_sd_write_data_byte;
    }

    R_SDHI_GetBuffRegAddress(p_hndl->channel, &sd_buff_addr);

    /* WAIT_LOOP */
    for (j = cnt; j > 0; j--)
    {
        /* ---- Wait for BWE/BRE interrupt. ---- */
        if (r_sdc_sd_int_io_wait(card_no, SDC_SD_TIMEOUT_MULTIPLE, func_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
            break;
        }

        if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_check_sdsts2_err(card_no);
            break;
        }
        if (SDC_SD_TRANS_READ == dir)
        {
            r_sdc_sd_clear_sdsts(card_no, 0, SDHI_SDIMSK2_RE); /* Clear a BRE bit. */
        }
        else
        {
            r_sdc_sd_clear_sdsts(card_no, 0, SDHI_SDIMSK2_WE); /* Clear a BWE bit. */
        }
        /* Write/Read to/from SDHI_BUF by 1 sector. */
        if ((*func)(p_buff, sd_buff_addr, blk) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_CPU_IF);
            break;
        }
        /* Update buffer. */
        p_buff = (p_buff + blk);

        /* ---- Check the stop flag. ---- */
        if (p_hndl->stop)
        {
            p_hndl->stop = 0;
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_TRANS, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
            break;
        }
    }

    return p_hndl->error;
} /* End of function r_sdc_sd_software_trans_blk() */

/**********************************************************************************************************************
* Outline      : Transfer data by DMAC/DTC
* Function Name: r_sdc_sd_dmacdtc_trans
* Description  : Waits for the DMAC/DTC transfer completion.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            cnt                 ;   Number of data
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_dmacdtc_trans(uint32_t card_no, int32_t cnt)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t            time = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);
    /* ---- Check DTC transfer end.  --- */
    /* Timeout value depends on transfer size. */
    time = cnt * 100;

    if (time < 1000)            /* 1s */
    {
        time = 1000;
    }

    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    /* WAIT_LOOP */
    while (1)
    {
        /* Check timeout. */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_DEV, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
            break;
        }

        /* Check error. */
        if (r_sdc_sd_get_intstatus(card_no) == SDC_SD_SUCCESS)
        {
            if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_DEV, __LINE__);
                r_sdc_sd_check_sdsts2_err(card_no);
                break;
            }
        }

        /* Check DTC transfer end */
        if (SDC_SD_SET_TRANS_STOP == g_sdc_sd_int_dmacdtcflg[card_no])
        {
            break;
        }

        /* ---- Check the stop flag. ---- */
        if (p_hndl->stop)
        {
            p_hndl->stop = 0;
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_DEV, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return p_hndl->error;
} /* End of function r_sdc_sd_dmacdtc_trans() */

/* End of File */
