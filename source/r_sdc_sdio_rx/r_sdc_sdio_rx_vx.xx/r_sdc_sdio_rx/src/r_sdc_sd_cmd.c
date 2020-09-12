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
* File Name    : r_sdc_sd_cmd.c
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
/* ==== response errors ==== */
static const sdc_sd_status_t g_sdc_sd_resp_err_tbl[] =
{
    SDC_SD_ERR_OUT_OF_RANGE,
    SDC_SD_ERR_ADDRESS_ERROR,
    SDC_SD_ERR_BLOCK_LEN_ERROR,
    SDC_SD_ERR_CARD_ERASE,
    SDC_SD_ERR_CARD_ERASE,
    SDC_SD_ERR_WP,
    SDC_SD_ERR_CARD_LOCK,
    SDC_SD_ERR_CARD_UNLOCK,
    SDC_SD_ERR_CARD_CRC,
    SDC_SD_ERR_ILLEGAL_COMMAND,
    SDC_SD_ERR_CARD_ECC,
    SDC_SD_ERR_CARD_CC,
    SDC_SD_ERR_CARD_ERROR,
    SDC_SD_ERR_RESERVED_ERROR18,
    SDC_SD_ERR_RESERVED_ERROR17,
    SDC_SD_ERR_OVERWRITE
};

/* ==== IO errors ==== */
static const sdc_sd_status_t g_sdc_sd_io_err_tbl[] =
{
    SDC_SD_ERR_CARD_CRC,
    SDC_SD_ERR_ILLEGAL_COMMAND,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_CARD_ERROR,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_ILL_FUNC,
    SDC_SD_ERR_OUT_OF_RANGE,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL,
    SDC_SD_ERR_INTERNAL
};

/* ==== SDHI_SDSTS2 errors table ==== */
static const sdc_sd_status_t g_sdc_sd_info2_err_tbl[] =
{
    SDC_SD_ERR_RES_TOE,     /* SDHI_SDIMSK2_ERR6   0x0040 */
    SDC_SD_ERR_ILL_READ,    /* SDHI_SDIMSK2_ERR5   0x0020 */
    SDC_SD_ERR_ILL_WRITE,   /* SDHI_SDIMSK2_ERR4   0x0010 */
    SDC_SD_ERR_CARD_TOE,    /* SDHI_SDIMSK2_ERR3   0x0008 */
    SDC_SD_ERR_END_BIT,     /* SDHI_SDIMSK2_ERR2   0x0004 */
    SDC_SD_ERR_CRC,         /* SDHI_SDIMSK2_ERR1   0x0002 */
    SDC_SD_ERR_CMD_ERROR    /* SDHI_SDIMSK2_ERR0   0x0001 */
};

/* ==== transfer speed table ==== */
static const uint32_t g_sdc_sd_tran_speed[8] =
{
    1,      /* 100kbit/s */
    10,     /* 1Mbit/s */
    100,    /* 10Mbit/s */
    1000,   /* 100Mbit/s */
    1000,   /* reserved */
    1000,   /* reserved */
    1000,   /* reserved */
    1000    /* reserved */
};

static const uint8_t g_sdc_sd_time_value[16] =
{
     0, 10, 12, 13, 15,
    20, 25, 30, 35, 40,
    45, 50, 55, 60, 70,
    80
};

static void r_sdc_sd_get_sdsts2(uint32_t card_no);

/**********************************************************************************************************************
* Outline      : Issue SD Command
* Function Name: r_sdc_sd_send_cmd
* Description  : Issues a SD command, and then waits for the receive response.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           cmd                 ;   Command code
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_send_cmd(uint32_t card_no, uint32_t cmd)
{
    sdc_sd_hndl_t * p_hndl = 0;
    volatile int32_t  i = 0;
    int32_t           time = 0;
    uint32_t          reg = 0;

    switch (cmd)
    {
        case SDC_SD_CMD38:     /* Erase command */
            time = SDC_SD_TIMEOUT_ERASE_CMD;
        break;

        case SDC_SD_CMD7:
        case SDC_SD_CMD12:
            /* CMD7, CMD12 are contained. */
            time = SDC_SD_TIMEOUT_RESP; /* Treat same as write operation timeout. */
        break;

        default:
            time = SDC_SD_TIMEOUT_CMD;
        break;
    }

    p_hndl = SDC_SD_GET_HNDL(card_no);
    
    p_hndl->error = SDC_SD_SUCCESS;

    /* Enable the resp end. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_RESP, 0);
    r_sdc_sd_enable_icu_int(card_no);

    for (i = 0 ; i < SDC_SD_SCLKDIVEN_LOOP_COUNT ; i++)
    {
#ifdef USE_INFO2_CBSY
        R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS2, &reg);
        if (0 == reg & SDHI_SDIMSK2_CBSY)
        {
            break;
        }
#else
        R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS2, &reg);
        if (reg & SDHI_SDIMSK2_SCLKDIVEN)
        {
            break;
        }
#endif
    }

    if (SDC_SD_SCLKDIVEN_LOOP_COUNT == i)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CBSY_ERROR);       /* treat as CBSY ERROR */
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_RESP, 0);
        r_sdc_sd_enable_icu_int(card_no);
        return p_hndl->error;
    }

    /* ---- Clear previous errors. ---- */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_enable_icu_int(card_no);

    /* ---- Issue a command. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDCMD, cmd);

    /* ---- Wait for the resp end. ---- */
    if (r_sdc_sd_int_wait(card_no, time) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
        r_sdc_sd_disable_icu_int(card_no);
        r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_RESP, SDHI_SDIMSK2_ILA);
        R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_RESP, SDHI_SDIMSK2_ERR);
        r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_RESP, SDHI_SDIMSK2_ERR);
        r_sdc_sd_enable_icu_int(card_no);
        return p_hndl->error;
    }

    /* Disable the resp end and the illegal access interrupts. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_RESP, SDHI_SDIMSK2_ILA);
    r_sdc_sd_enable_icu_int(card_no);

    r_sdc_sd_get_sdsts2(card_no);    /* Get the SDHI_SDSTS2 register. */

    r_sdc_sd_check_sdsts2_err(card_no);  /* Check the SDHI_SDSTS2 error bits. */

    if (0 == (p_hndl->sdsts1 & SDHI_SDIMSK1_RESP))
    {
        /* Set the error code in case of no error code. */
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_NO_RESP_ERROR);        /* No response */
    }

    /* ---- Clear previous errors. ---- */
    /* SDHI_SDSTS1 and SDHI_SDSTS2 are already cleared in interrupt handler.*/
/*  R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_RESP, SDHI_SDIMSK2_ERR); */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_RESP, SDHI_SDIMSK2_ERR);
    r_sdc_sd_enable_icu_int(card_no);

    return p_hndl->error;
} /* End of function r_sdc_sd_send_cmd() */

/**********************************************************************************************************************
* Outline      : Issue Application Specific Command
* Function Name: r_sdc_sd_send_acmd
* Description  : Issues a application specific command, and then waits for the receive response.
*              : Issues a CMD55 preceding the application specific command.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           cmd                 ;   Command code
*              : uint32_t           arg                 ;   Command argument [31:0]
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_send_acmd(uint32_t card_no, uint32_t cmd, uint32_t arg)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Issue a CMD55. ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD55, SDC_SD_RESP_R1, p_hndl->rca[0]) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        return SDC_SD_ERR;
    }

    /* ---- Issue an ACMD. ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, cmd, SDC_SD_RESP_NON, arg) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_send_acmd() */

/**********************************************************************************************************************
* Outline      : Issue General SD Command
* Function Name: r_sdc_sd_send_cmd_arg
* Description  : Issues a command, and then waits for the receive response.
*              : After finishing this function, start the data transfer.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           cmd                 ;   Command code
*              : uint32_t           arg                 ;   Command argument [31:0]
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_send_cmd_arg(uint32_t card_no, uint32_t cmd, int32_t resp, uint32_t arg)
{
    sdc_sd_hndl_t * p_hndl = 0;
    sdc_sd_status_t ret = SDC_SD_SUCCESS;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    R_SDHI_OutReg(p_hndl->channel, SDHI_SDARG, arg);

    /* ---- Issue a command. ---- */
    ret = r_sdc_sd_send_cmd(card_no, cmd);
    if (SDC_SD_SUCCESS == ret)
    {
        ret = r_sdc_sd_get_resp(card_no, resp); /* Get and check the response. */
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
    }

    return ret;
} /* End of function r_sdc_sd_send_cmd_arg() */

/**********************************************************************************************************************
* Outline      : Get OCR Register
* Function Name: r_sdc_sd_send_ocr
* Description  : Gets a OCR register and checks card operation voltage.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            type                ;   Card type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_send_ocr(uint32_t card_no, int32_t type)
{
    sdc_sd_hndl_t * p_hndl = 0;
    int32_t           i = 0;
    sdc_sd_status_t ret = SDC_SD_SUCCESS;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ===== Distinguish card type using the CMD5, ACMD41. ==== */
    for (i = 0; i < 200; i++)
    {
        switch (type)
        {
            case SDC_SD_MEDIA_UNKNOWN:  /* Unknown media */
                /* ---- Issue CMD5 without argument. ---- */
                ret = r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD5, SDC_SD_RESP_R4, 0);
                return ret;
            break;

            case SDC_SD_MEDIA_SDIO:
                /* ---- Issue the CMD5. ---- */
                ret = r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD5, SDC_SD_RESP_NON, p_hndl->voltage);
            break;

            case SDC_SD_MEDIA_SDMEM:
            case SDC_SD_MEDIA_COMBO:
                /* SDC_SD_MEDIA_SDMEM and SDC_SD_MEDIA_COMBO are contained. */
                /* Set a HCS bit according the CMD8 response. */
                if (SDC_SD_SPEC_10 != p_hndl->sd_spec)
                {
                    /* Set the voltage pattern to a HCS bit. */
                    p_hndl->voltage = (p_hndl->voltage | SDC_SD_OCR_HCS);
                }
                /* ---- Issue the ACMD41. ---- */
                ret = r_sdc_sd_send_acmd(card_no, SDC_SD_ACMD41, p_hndl->voltage);
            break;

            default:
                p_hndl->resp_status = 0;
                /* Set the error code. */
                p_hndl->error = SDC_SD_ERR_INTERNAL;
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
                return SDC_SD_ERR;
            break;
        }

        if (SDC_SD_SUCCESS == ret)
        {
            if (SDC_SD_MEDIA_SDIO == type)    /* IO */
            {
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
                r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R4); /* Check the R4 response. */
                /* ---- Poll the busy bit. ---- */
                if (p_hndl->io_ocr[0] & SDC_SDIO_CMD5_R4_C)   /* Busy cleared */
                {
                    break;
                }
                else
                {
                    ret = SDC_SD_ERR;   /* Busy */
                    r_sdc_sd_wait(card_no, 5);  /* Issue the wait function due to retry. */
                }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */
            }
            else    /* Memory */
            {
                r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R3); /* Check the R3 response. */
                /* ---- Poll the busy bit. ---- */
                if (p_hndl->ocr[0] & SDC_SD_OCR_BUSY)  /* Busy cleared */
                {
                    break;
                }
                else
                {
                    ret = SDC_SD_ERR;   /* Busy */
                    r_sdc_sd_wait(card_no, 5);  /* Issue the wait function due to retry. */
                }
            }
        }
    }

    return ret;
} /* End of function r_sdc_sd_send_ocr() */

/**********************************************************************************************************************
* Outline      : Check R1 Response Errors
* Function Name: r_sdc_sd_check_resp_error
* Description  : Distinguishes the error from the R1 response and then sets p_hndl->error to the error.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   No error detected
*              : SDC_SD_ERR                             ;   Errors detected
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_check_resp_error(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;
    uint32_t          status = 0;
    int32_t           bit = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    status = (uint32_t)((p_hndl->resp_status & SDC_SD_RES_ERR_UPPER) >> SDC_SD_BIT_SHIFT_16);
    
    /* ---- Search the R1 error bits. ---- */
    bit = r_sdc_sd_bit_search(status);

    if (SDC_SD_ERR != bit)
    {
        /* Detect the R1 errors, except for the AKE_SEQ_ERROR. */
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, g_sdc_sd_resp_err_tbl[bit]);
        return SDC_SD_ERR;
    }
    else if (p_hndl->resp_status & SDC_SD_RES_AKE_SEQ_ERROR)
    {
        /* Authentication process sequence error */
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_AKE_SEQ);
        return SDC_SD_ERR;
    }
    else
    {
        /* NOP */
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_check_resp_error() */

/**********************************************************************************************************************
* Outline      : Check R5 Response Errors
* Function Name: r_sdc_sd_check_resp5_error
* Description  : Distinguishes the error from the R5 response and then sets p_hndl->error to the error.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   No error detected
*              : SDC_SD_ERR                             ;   Errors detected
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_check_resp5_error(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t          status = 0;
    int32_t           bit = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_MEDIA_SDIO & p_hndl->media_type)
    {
        /* SDIO */
        status = (uint32_t)(p_hndl->resp_status & SDC_SD_R5_CARD_STATUS_ERR);

        /* Search the R5 error bits. */
        bit = r_sdc_sd_bit_search(status);

        /* R5 errors detected */
        if (SDC_SD_ERR != bit)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
            r_sdc_sd_set_err(card_no, g_sdc_sd_io_err_tbl[bit]);
            return SDC_SD_ERR;
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_check_resp5_error() */

/**********************************************************************************************************************
* Outline      : Get Response and Check Response Errors
* Function Name: r_sdc_sd_get_resp
* Description  : Gets the response value from the RESP register.
*              : After finishing this function, start the data transfer. 
* Arguments    : uint32_t           card_no              ;   SD Card No.
*              : int32_t            resp                 ;   Command response
* Return Value : SDC_SD_SUCCESS                          ;   Successful operation
*              : SDC_SD_ERR                              ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_get_resp(uint32_t card_no, int32_t resp)
{
    sdc_sd_hndl_t * p_hndl = 0;
    uint32_t          status[4] = {0, 0, 0, 0};
    uint32_t          * p_ptr = 0;
    sdhi_get_resp_t   resp_reg;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* Select the RESP register depend on the response type. */
    switch (resp)
    {
        case SDC_SD_RESP_NON:   /* Non-response */
            /* NOP */
        break;

        case SDC_SD_RESP_R1:    /* Standard response (32-bit length) */
        case SDC_SD_RESP_R1b:   /* Standard response  (32-bit length) with the busy signal */
            /* SDC_SD_RESP_R1 and SDC_SD_RESP_R1b are contained. */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            status[0] = resp_reg.sdrsp10;
            p_hndl->resp_status = status[0];

            /* Support to mount a locked card. */
            if (status[0] & SDC_SD_RES_ERR_IGNORE_LOCK)        /* Ignore the card locked status. */
            {
                /* Any status error */
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
                return r_sdc_sd_check_resp_error(card_no);
            }
        break;

        case SDC_SD_RESP_R1_SCR:    /* Standard response (32-bit length) with the busy signal */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            p_hndl->scr[0] = resp_reg.sdrsp10;
        break;

        case SDC_SD_RESP_R2_CID:    /* CID register (128-bit length) */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            status[3] = resp_reg.sdrsp76;
            status[2] = resp_reg.sdrsp54;
            status[1] = resp_reg.sdrsp32;
            status[0] = resp_reg.sdrsp10;

            p_ptr = p_hndl->cid;
            *p_ptr++ = ((status[3] << SDC_SD_BIT_SHIFT_8) | (status[2] >> SDC_SD_BIT_SHIFT_24));
            *p_ptr++ = ((status[2] << SDC_SD_BIT_SHIFT_8) | (status[1] >> SDC_SD_BIT_SHIFT_24));
            *p_ptr++ = ((status[1] << SDC_SD_BIT_SHIFT_8) | (status[0] >> SDC_SD_BIT_SHIFT_24));
            *p_ptr++ = (status[0] << SDC_SD_BIT_SHIFT_8);
        break;

        case SDC_SD_RESP_R2_CSD:    /* CSD register (128-bit length) */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            status[3] = resp_reg.sdrsp76;
            status[2] = resp_reg.sdrsp54;
            status[1] = resp_reg.sdrsp32;
            status[0] = resp_reg.sdrsp10;

            p_ptr = p_hndl->csd;
            *p_ptr++ = ((status[3] << SDC_SD_BIT_SHIFT_8) | (status[2] >> SDC_SD_BIT_SHIFT_24));
            *p_ptr++ = ((status[2] << SDC_SD_BIT_SHIFT_8) | (status[1] >> SDC_SD_BIT_SHIFT_24));
            *p_ptr++ = ((status[1] << SDC_SD_BIT_SHIFT_8) | (status[0] >> SDC_SD_BIT_SHIFT_24));
            *p_ptr++ = (status[0] << SDC_SD_BIT_SHIFT_8);
        break;

        case SDC_SD_RESP_R3:    /* OCR register (32-bit length) */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            p_hndl->ocr[0] = resp_reg.sdrsp10;
        break;

        case SDC_SD_RESP_R4:    /* IO OCR register (24-bit length) */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            p_hndl->io_ocr[0] = resp_reg.sdrsp10;
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */
        break;

        case SDC_SD_RESP_R6:        /* Published RCA response (16-bit length) */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            p_hndl->rca[0] = (resp_reg.sdrsp10 & SDC_SD_R6_NEW_PUBLISHED_RCA);
        break;

        case SDC_SD_RESP_R5:        /* IO RW response */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            status[0] = resp_reg.sdrsp10;
            p_hndl->resp_status = status[0];

            if (status[0] & SDC_SD_R5_CARD_STATUS_ERR)
            {
                /* Any status error */
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
                return r_sdc_sd_check_resp5_error(card_no);
            }
        break;

        case SDC_SD_RESP_R7:       /* IF_COND response */
            R_SDHI_GetResp(p_hndl->channel, &resp_reg);
            p_hndl->if_cond[0] = resp_reg.sdrsp10;
        break;

        default:
            /* Unknown type */
            p_hndl->resp_status = 0;
            /* Set the error code. */
            p_hndl->error = SDC_SD_ERR_INTERNAL;
            return SDC_SD_ERR;
        break;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_get_resp() */

/**********************************************************************************************************************
* Outline      : Check CSD Register
* Function Name: r_sdc_sd_check_csd
* Description  : Checks the CSD register and then gets the information.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_check_csd(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;
    uint32_t          transpeed = 0;
    uint32_t          timevalue = 0;
    uint32_t          erase_sector_size = 0;
    uint32_t          erase_group_size = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- CSD Structure ---- */
    p_hndl->csd_structure = (uint8_t)((p_hndl->csd[0] & SDC_SD_CSD_STRUCTURE) >> SDC_SD_BIT_SHIFT_30);

    /* Use sd_spec to check cmd8 response status */
    if (1 <= p_hndl->csd_structure)
    {
        if(SDC_SD_SPEC_20 > p_hndl->sd_spec)
        {
            /* If csd_structure is 1 or more, sd_spec is SD Specifications Part 1 Physical Layer */
            /* Specification Ver.2.00 or more. */
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_CSD_VER);
            return SDC_SD_ERR;
        }
    }

    /* ---- TAAC/NSAC ---- */
    /* No check, set the fixed data. */

    /* ---- TRAN_SPEED  ---- */
    transpeed = (p_hndl->csd[0] & SDC_SD_CSD_TRAN_SPEED);
    timevalue = (p_hndl->csd[0] & SDC_SD_CSD_TIME_VALUE) >> SDC_SD_BIT_SHIFT_3;

    transpeed = (uint32_t)(g_sdc_sd_tran_speed[transpeed] * g_sdc_sd_time_value[timevalue]);

    /* ---- Set transfer speed for memory access. ---- */
    if (SDC_SD_CSD_TRAN_SPEED_5000 <= transpeed)
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_50MHz;
    }
    else if (SDC_SD_CSD_TRAN_SPEED_2500 <= transpeed)
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_25MHz;
    }
    else if (SDC_SD_CSD_TRAN_SPEED_2000 <= transpeed)
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_20MHz;
    }
    else if (SDC_SD_CSD_TRAN_SPEED_1000 <= transpeed)
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_10MHz;
    }
    else if (SDC_SD_CSD_TRAN_SPEED_500 <= transpeed)
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_5MHz;
    }
    else if (SDC_SD_CSD_TRAN_SPEED_100 <= transpeed)
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_1MHz;
    }
    else
    {
        p_hndl->csd_tran_speed = SDC_SD_CLK_400kHz;
    }

    /* ---- CCC  ---- */
    p_hndl->csd_ccc = (uint32_t)(((p_hndl->csd[1] & SDC_SD_CSD_CCC) >> SDC_SD_BIT_SHIFT_20));

    /* ---- PERM/TMP_WRITE_PROTECT ---- */
    p_hndl->write_protect = (p_hndl->write_protect | (uint8_t)((p_hndl->csd[3] & SDC_SD_CSD_PROTECT) >> SDC_SD_BIT_SHIFT_11));

    /* ---- Calculate the number of erase sectors ---- */
    erase_sector_size = ((p_hndl->csd[2] & SDC_SD_CSD_ERASE_SEC_SIZE) >> SDC_SD_BIT_SHIFT_7);
    erase_group_size = (p_hndl->csd[2] & SDC_SD_CSD_ERASE_GRP_SIZE);
    p_hndl->erase_sect = (erase_sector_size + 1) * (erase_group_size + 1);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_check_csd() */

/**********************************************************************************************************************
* Outline      : Check SDHI_SDSTS2 Register Errors
* Function Name: r_sdc_sd_check_sdsts2_err
* Description  : Checks error bits of the SDHI_SDSTS2 register.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_check_sdsts2_err(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;
    uint32_t          sdsts2 = 0;
    int32_t           bit = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    sdsts2 = (uint32_t)(p_hndl->sdsts2 & SDHI_SDIMSK2_ERR);

    /* ---- Search error bits. ---- */
    bit = r_sdc_sd_bit_search(sdsts2);

    if (SDC_SD_ERR == bit)  /* No error */
    {
        r_sdc_sd_set_err(card_no, SDC_SD_SUCCESS);
    }
    else if (0 == bit)  /* CMD error */
    {
        /* Set the error code. */
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CMD_ERROR);
    }
    else    /* Other error */
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CMD, __LINE__);
        r_sdc_sd_set_err(card_no, g_sdc_sd_info2_err_tbl[bit - 9]);
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_check_sdsts2_err() */

/**********************************************************************************************************************
* Outline      : Get SDHI_SDSTS2 Register
* Function Name: r_sdc_sd_get_sdsts2
* Description  : Gets the SDHI_SDSTS2 register, stores to p_hndl->sdsts2 and clears the SDHI_SDSTS error flags.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : none
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static void r_sdc_sd_get_sdsts2(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;
    uint32_t        sdsts2_reg = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    R_SDHI_InReg(p_hndl->channel, SDHI_SDSTS2, &sdsts2_reg);
    sdsts2_reg = (uint32_t)((sdsts2_reg & SDHI_SDIMSK2_ERR) & (~SDHI_SDIMSK2_BIT11));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTS2, (uint32_t)~sdsts2_reg);

    p_hndl->sdsts2 = (uint32_t)(p_hndl->sdsts2 | sdsts2_reg);

    return;
} /* End of function r_sdc_sd_get_sdsts2() */

/* End of File */
