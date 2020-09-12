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
* File Name    : r_sdc_sdio_direct.c
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
*              : 05.09.2015 1.00    First Release
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
* Outline      : I/O Read Direct Operation
* Function Name: r_sdc_sdio_ReadDirect
* Description  : Executes I/O Read Direct operation from the specified address using CMD52.
* Arguments    : uint32_t       card_no                 ; SDHI Channel No
*              : sdc_sdio_daccess_t * p_sdc_sdio_daccess
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function No
*              :    uint32_t    adr                     ;   Register address
*              :    uint32_t    raw_flag                ;   Read after Write flag (Setting not required)
*              :    uint32_t    rw_flag                 ;   Read : 0, Write : 1 (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_ReadDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdio_chk_param_direct(card_no, p_sdc_sdio_daccess))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
        return SDC_SD_ERR_STOP;
    }

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
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

    /* ==== Execute I/O Read Direct Operation. ==== */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->io_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        /* ---- Check the card is in Command state (CMD) using CMD52. ---- */
        if (r_sdc_sdio_check_retry_cmd_state(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
            return SDC_SD_ERR;
        }

        p_sdc_sdio_daccess->rw_flag  = SDC_SDIO_RWFLAG_R;
        p_sdc_sdio_daccess->raw_flag = SDC_SDIO_SIMPLE_READ;

        /* ---- Executes I/O Read/Write Direct operation. ---- */
        r_sdc_sdio_direct(card_no, p_sdc_sdio_daccess);

        /* The State is in the Command State (CMD) because of CMD52, so that do not check the state. */
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;

} /* End of function r_sdc_sdio_ReadDirect() */

/**********************************************************************************************************************
* Outline      : I/O Write Direct Operation
* Function Name: r_sdc_sdio_WriteDirect
* Description  : Executes I/O Write Direct operation to the specified address using CMD52.
* Arguments    : uint32_t       card_no                 ; SDHI Channel No
*              : sdc_sdio_daccess_t * p_sdc_sdio_daccess
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function No
*              :    uint32_t    adr                     ;   Register address
*              :    uint32_t    raw_flag                ;   Read after Write flag
*              :    uint32_t    rw_flag                 ;   Read : 0, Write : 1 (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_WriteDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess)
{
    sdc_sd_hndl_t       * p_hndl = 0;

    if (SDC_SD_ERR == r_sdc_sdio_chk_param_direct(card_no, p_sdc_sdio_daccess))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        return SDC_SD_ERR;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    p_hndl->error = SDC_SD_SUCCESS;

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_STOP);
        return SDC_SD_ERR_STOP;
    }

    /* ---- Check the card insertion. ---- */
    if (R_SDC_SD_GetCardDetection(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
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

    /* ==== Execute I/O Write Direct operation. ---- */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->io_tran_speed, SDHI_CLOCK_ENABLE) == SDHI_SUCCESS)
    {
        /* ---- Check the card is in Command state (CMD) using CMD52. ---- */
        if (r_sdc_sdio_check_retry_cmd_state(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);
            return SDC_SD_ERR;
        }

        p_sdc_sdio_daccess->rw_flag  = SDC_SDIO_RWFLAG_W;
        
        /* ---- Executes I/O Read/Write Direct operation. ---- */
        r_sdc_sdio_direct(card_no, p_sdc_sdio_daccess);

        /* The State is in the Command State (CMD) because of CMD52, so that do not check the state. */
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
    }

    R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);

    return p_hndl->error;

} /* End of function r_sdc_sdio_WriteDirect() */

/**********************************************************************************************************************
* Outline      : Access a Single Register
* Function Name: r_sdc_sdio_direct
* Description  : Executes IO_RW_DIRECT Command (CMD52) operation and 
*                sets the Response Flags to p_sdc_sdio_daccess->p_buff.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : sdc_sdio_daccess_t * p_sdc_sdio_daccess    ;   SDHI Driver I/O transfer configuration
*              :    uint8_t     * p_buff                ;   Data buffer / Read or Write Data
*              :    uint32_t    func                    ;   Function No
*              :    uint32_t    adr                     ;   Register address
*              :    uint32_t    raw_flag                ;   Read after Write operation : 1
*              :    uint32_t    rw_flag                 ;   Read : 0, Write : 1
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : Read or Write Data;
*              : For I/O write with RAW =1 (raw_flag =1), the value is data read form the addressed register after write.
*              : For I/O write with RAW =0 (raw_flag =0), the value is data in the write command.
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_direct(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    uint32_t            arg = 0;
    uint32_t            cmd = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Set parameters to the arg. ---- */
    arg = ((p_sdc_sdio_daccess->rw_flag  << SDC_SD_BIT_SHIFT_31)
         | (p_sdc_sdio_daccess->func     << SDC_SD_BIT_SHIFT_28)
         | (p_sdc_sdio_daccess->raw_flag << SDC_SD_BIT_SHIFT_27)
         | (p_sdc_sdio_daccess->adr      << SDC_SD_BIT_SHIFT_9)
         | (uint8_t)*p_sdc_sdio_daccess->p_buff);

    if (p_sdc_sdio_daccess->rw_flag)   /* For Write */
    {
        cmd = SDC_SD_CMD52_W;
    }
    else    /* For Read */
    {
        cmd = SDC_SD_CMD52_R;
    }

    /* ---- Issue the command and get the response.  ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, cmd, SDC_SD_RESP_R5, arg) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_DIRECT, __LINE__);
        /* The error code is set in the r_sdc_sd_send_cmd_arg(). */
        return SDC_SD_ERR;
    }

    /* Store the Read or Write Data Flags (bit 7-0) of CMD52 Response to the buffer. */
    *p_sdc_sdio_daccess->p_buff = (uint8_t)p_hndl->resp_status;

    return p_hndl->error;
    
} /* End of function r_sdc_sdio_direct() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
