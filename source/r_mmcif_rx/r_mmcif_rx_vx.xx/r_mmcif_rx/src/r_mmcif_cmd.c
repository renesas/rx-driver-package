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
* System Name  : MMC Driver
* File Name    : r_mmcif_cmd.c
* Version      : 1.06
* Device       : RX64M (LQFP-176)
* Abstract     : API & Sub module
* Tool-Chain   : For RX64M Group
*              :  e2 studio (Version 7.4.0)
* OS           : not use
* H/W Platform : RSK board for RX64M
* Description  : Interface file for MMC API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 28.12.2016 1.01    Ignore bit17(UNDERRUN) and bit18(OVERRUN) by the card status error check.
*              : 20.05.2019 1.05    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
*              : 30.07.2019 1.06    Add WAIT_LOOP.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Public interface header file for this package. */
#include "r_mmcif_rx_if.h"
/* Private header file for this package. */
#include "./src/r_mmcif_rx_private.h"


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
/* ==== Response errors ==== */
static const mmc_status_t RespErrTbl[] =
{
    MMC_ERR_OUT_OF_RANGE,
    MMC_ERR_ADDRESS_ERROR,
    MMC_ERR_BLOCK_LEN_ERROR,
    MMC_ERR_CARD_ERASE,
    MMC_ERR_CARD_ERASE,
    MMC_ERR_WP,
    MMC_ERR_CARD_LOCK,
    MMC_ERR_CARD_UNLOCK,
    MMC_ERR_CARD_CRC,
    MMC_ERR_ILLEGAL_COMMAND,
    MMC_ERR_CARD_ECC,
    MMC_ERR_CARD_CC,
    MMC_ERR_CARD_ERROR,
    MMC_ERR_RESERVED_ERROR18,
    MMC_ERR_RESERVED_ERROR17,
    MMC_ERR_OVERWRITE
};


/* ==== MMC_CEINT errors table ==== */
static const mmc_status_t Info2ErrTbl[] =
{
    MMC_ERR_CMD,        /* CE_INT[b15] Command issuance error */
    MMC_ERR_BUFACC,     /* CE_INT[b14] Buffer access error */
    MMC_SUCCESS,        /* CE_INT[b13] Reserved */
    MMC_SUCCESS,        /* CE_INT[b12] Reserved */
    MMC_ERR_WRITE,      /* CE_INT[b11] Write data error */
    MMC_ERR_READ,       /* CE_INT[b10] Read data error */
    MMC_ERR_RESPIND,    /* CE_INT[b9 ] Response index error */
    MMC_ERR_RESP,       /* CE_INT[b8 ] Response error */
    MMC_SUCCESS,        /* CE_INT[b7 ] Reserved */
    MMC_SUCCESS,        /* CE_INT[b6 ] Reserved */
    MMC_SUCCESS,        /* CE_INT[b5 ] Reserved */
    MMC_ERR_CRC_TOE,    /* CE_INT[b4 ] CRC status timeout */
    MMC_ERR_WRITE_TOE,  /* CE_INT[b3 ] Write data timeout */
    MMC_ERR_READ_TOE,   /* CE_INT[b2 ] Read data timeout */
    MMC_ERR_RESPB_TOE,  /* CE_INT[b1 ] Response busy timeout */
    MMC_ERR_RESP_TOE,   /* CE_INT[b0 ] Response timeout 0x0001 */
};


/* ==== Transfer speed table ==== */
static const uint32_t TranSpeed[8] =
{
    1,      /* 100kbit/s */
    10,     /* 1Mbit/s */
    100,    /* 10Mbit/s */
    1000,   /* 100Mbit/s */
    1000,   /* Reserved */
    1000,   /* Reserved */
    1000,   /* Reserved */
    1000    /* Reserved */
};

static const uint8_t TimeValue[16] =
{
     0, 10, 12, 13, 15,
    20, 26, 30, 35, 40,
    45, 52, 55, 60, 70,
    80
};


/**********************************************************************************************************************
* Outline      : Issue MMC command
* Function Name: r_mmcif_send_cmd
* Description  : Issues a MMC command.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           cmd                 ;   Command code
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_send_cmd(uint32_t channel, uint32_t cmd)
{
    mmc_mmchndl_t      *p_hndl = 0;
    int32_t             time = 0;
    volatile int32_t    i = 0;
    uint32_t            reg = 0;

    p_hndl = MMC_GET_HNDL(channel);
    
    p_hndl->error = MMC_SUCCESS;

    /* ---- Set timeout ---- */
    switch (cmd)
    {
        case MMC_CMD38:                             /* Erase command */
            time = MMC_TIMEOUT_ERASE_CMD;           /* Extend timeout 1 sec */
        break;

        case MMC_CMD7:
        case MMC_CMD12:
            time = MMC_TIMEOUT_RESP;                /* Same as write timeout */
        break;

        default:
            time = MMC_TIMEOUT_CMD;
        break;
    }

    /* ---- Enable resp end interrupts. ---- */
    /* NOTE: Do not check the all error interrupt.
             Set MMC_ERR_HOST_TOE to the error code even if any error occurred. */
    r_mmcif_set_int_mask(channel, MMC_CEINT1_RESP_COMP, 0);

    /* ---- Set response check option and change HOST bus width. ---- */
    reg |= (uint32_t)p_hndl->if_mode;       /* Set bus width. */

    switch (cmd)
    {
        case MMC_CMD1:      /* R3 Response Type */
            reg |= 0x00005000;
        break;
        case MMC_CMD2:      /* R4 Response Type */
        case MMC_CMD9:
        case MMC_CMD10:
            /* NOTE: 
                Response CRC7 Check : Perform the error check on internal CRC7 field value (set the
                                      RTYP[1:0] bits to 10b).
                Response Index Check: Check to see if the check bits in the response are all 1. 
            */
            reg |= 0x00006000;
        break;
        case MMC_CMD14:     /* Not always exist CRC16 */
            reg |= 0x00000408;
        break;
        case MMC_CMD19:     /* Not always exist CRC16 */
            reg |= 0x00000100;
        break;
        default:
            /* NOP */
        break;
    }

    /* WAIT_LOOP */
    for (i = 0 ; i < MMC_CLKDIVEN_LOOP_COUNT ; i++)
    {
        if (0 == (MMC_INREG(p_hndl, MMC_CECLKCTRL) & MMC_CECLKCTRL_MASK_MMCBUSBSY))
        {
            break;
        }
    }

    if (MMC_CLKDIVEN_LOOP_COUNT == i)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CBSY_ERROR);       /* Treate as CBSY ERROR. */
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_RESP_COMP, 0);
        return p_hndl->error;
    }
    /* ---- Clear previous errors. ---- */
    r_mmcif_clear_info(channel,  MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

    /* ---- Issue a command. ---- */
    MMC_OUTREG(p_hndl, MMC_CECMDSET, (cmd | reg));

    if ((cmd | reg) == MMC_INREG(p_hndl, MMC_CECMDSET))
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    /* ---- Wait for the resp end. ---- */
    if (r_mmcif_dev_int_wait(channel, time) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_RESP_COMP, 0);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_RESP_COMP, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_RESP_COMP, MMC_CEINT2_ALL_ERR);
        return p_hndl->error;
    }

    /* Disable the resp end and the illegal access interrupts. */
    r_mmcif_clear_int_mask(channel, MMC_CEINT1_RESP_COMP, 0);

    r_mmcif_get_info2(channel);       /* Get the MMC_CEINT register. */

    r_mmcif_check_info2_err(channel);     /* Check the MMC_CEINT error bits. */

    if (0 == (p_hndl->ceint1 & MMC_CEINT1_RESP_COMP))
    {
        /* Set the error code in case of no error code. */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_NO_RESP_ERROR);      /* No response */
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_RESP_COMP, 0);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_RESP_COMP, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_RESP_COMP, MMC_CEINT2_ALL_ERR);
        return p_hndl->error;
    }

    /* ---- Clear previous errors. ---- */
    r_mmcif_clear_info(channel, MMC_CEINT1_RESP_COMP, MMC_CEINT2_ALL_ERR);

    if (cmd & 0x00200000ul)
    {
        /* ---- Check busy response. ---- */
        /* Enable response busy complete and all errors. */
        r_mmcif_set_int_mask(channel, MMC_CEINT1_RESPB_COMP, 0);

        /* ---- Wait response busy completion. ---- */
        if(r_mmcif_dev_int_wait(channel, MMC_TIMEOUT_BUSY_CMD) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
            r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
            r_mmcif_clear_int_mask(channel, MMC_CEINT1_RESPB_COMP, 0);
            r_mmcif_clear_info_reg(channel, MMC_CEINT1_RESPB_COMP, MMC_CEINT2_ALL_ERR);
            r_mmcif_clear_info(channel, MMC_CEINT1_RESPB_COMP, MMC_CEINT2_ALL_ERR);
            return p_hndl->error;
        }

        /* Disable response busy completion and all errors. */
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_RESPB_COMP, 0);

        r_mmcif_get_info2(channel);       /* Get the MMC_CEINT register. */

        r_mmcif_check_info2_err(channel);     /* Check the MMC_CEINT error bits. */

        /* ---- Clear previous errors. ---- */
        r_mmcif_clear_info(channel, MMC_CEINT1_RESPB_COMP, MMC_CEINT2_ALL_ERR);
    }

    return p_hndl->error;
}


/**********************************************************************************************************************
* Outline      : Issue General MMC Command
* Function Name: R_MMCIF_SendCmdArg
* Description  : Issues a command, and then waits for the receive response.
*              : After finishing this function, start the data transfer.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           cmd                 ;   Command code
*              : int32_t            resp                ;   Command response
*              : uint32_t           arg                 ;   Command argument [31:0]
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t R_MMCIF_SendCmdArg(uint32_t channel, uint32_t cmd, int32_t resp, uint32_t arg)
{
    mmc_mmchndl_t       *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Check the card is mounted. ---- */
    if (MMC_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Not mounted yet */
    }

    p_hndl->error = MMC_SUCCESS;

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_STOP);
        return p_hndl->error;
    }

    /* ---- Check the card insertion. ---- */
    if (R_MMCIF_Get_CardDetection(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_NO_CARD);
        return p_hndl->error;
    }

    /* ---- Supply the clock for data-transfer mode. ---- */
    if (MMC_SUCCESS == r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE))
    {
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

        /* ==== Issues a command to the MMC. ==== */
        r_mmcif_send_cmd_arg(channel, cmd, resp, arg);

        r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
    }
    else
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
    }

    /* ---- Halt the clock. ---- */
    r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);

    return p_hndl->error;
}


/**********************************************************************************************************************
* Outline      : Issue general MMC command
* Function Name: r_mmcif_send_cmd_arg
* Description  : Issues a command, and then waits for the receive response.
*              : After finishing this function, start the data transfer.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           cmd                 ;   Command code
*              : int32_t            resp                ;   Command response
*              : uint32_t           arg                 ;   Command argument [31:0]
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_send_cmd_arg(uint32_t channel, uint32_t cmd, int32_t resp, uint32_t arg)
{
    mmc_mmchndl_t *p_hndl = 0;
    mmc_status_t  ret = MMC_SUCCESS;

    p_hndl = MMC_GET_HNDL(channel);

    MMC_OUTREG(p_hndl, MMC_CEARG, arg);
    if (arg == MMC_INREG(p_hndl, MMC_CEARG))
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    /* ---- Issue a command. ---- */
    ret = r_mmcif_send_cmd(channel, cmd);
    if (MMC_SUCCESS == ret)
    {
        ret = r_mmcif_get_resp(channel, resp);        /* Get and check the response. */
    }
    else
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
    }

    return ret;
}


/**********************************************************************************************************************
* Outline      : Get OCR register
* Function Name: r_mmcif_send_ocr
* Description  : Gets OCR register and checks card operation voltage.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_send_ocr(uint32_t channel)
{
    mmc_mmchndl_t    *p_hndl = 0;
    int32_t           i = 0;
    mmc_status_t  ret = MMC_SUCCESS;

    p_hndl = MMC_GET_HNDL(channel);

    /* ==== Distinguish card type using CMD1. ==== */

    /* ---- Check sector mode ---- */
    /* WAIT_LOOP */
    for (i = 0 ; i < 200 ; i++)
    {
        /* ---- Issue the CMD1. ---- */
        ret = r_mmcif_send_cmd_arg(channel, MMC_CMD1, MMC_RESP_R3, (p_hndl->voltage | 0x40000000));
        if (MMC_SUCCESS == ret)
        {
            r_mmcif_get_resp(channel, MMC_RESP_R3);       /* Check R3 response. */
            /* ---- Poll the busy bit. ---- */
            if (p_hndl->ocr[0] & 0x80000000)              /* Busy cleared */
            {
                return ret;
            }
            else
            {
                ret = MMC_ERR;      /* Busy */
                r_mmcif_dev_wait(channel, 5);             /* Issue the wait function due to retry. */
            }
        }
    }

    /* ---- Check byte mode ---- */
    /* WAIT_LOOP */
    for (i = 0 ; i < 200 ; i++)
    {
        /* ---- Issue the CMD1. ---- */
        ret = r_mmcif_send_cmd_arg(channel, MMC_CMD1, MMC_RESP_R3, p_hndl->voltage);
        if (MMC_SUCCESS == ret)
        {
            r_mmcif_get_resp(channel, MMC_RESP_R3);       /* Check R3 response. */
            /* ---- Poll the busy bit. ---- */
            if (p_hndl->ocr[0] & 0x80000000)              /* Busy cleared */
            {
                return ret;
            }
            else
            {
                ret = MMC_ERR;      /* Busy */
                r_mmcif_dev_wait(channel, 5);             /* Issue the wait function due to retry. */
            }
        }
    }
    return ret;
}


/**********************************************************************************************************************
* Outline      : Check R1 Response Errors
* Function Name: r_mmcif_check_resp_error
* Description  : Distinguishes the error from the R1 response and then sets p_hndl->error to the error.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   No error detected
*              : MMC_ERR                                ;   Errors detected
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_check_resp_error(uint32_t channel)
{
    mmc_mmchndl_t    *p_hndl = 0;
    uint32_t          status = 0;
    int32_t           bit = 0;

    p_hndl = MMC_GET_HNDL(channel);

    status = (uint32_t)((p_hndl->resp_status >> 16) & 0xfff9u);
    
    /* ---- Search R1 error bit. ---- */
    bit = r_mmcif_bit_search(status);

    if (MMC_ERR != bit)
    {
        /* Detect the R1 errors, except for the AKE_SEQ_ERROR. */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, RespErrTbl[bit]);
        return MMC_ERR;
    }
    else if (p_hndl->resp_status & MMC_RES_SWITCH_ERROR)    /* Switch error */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_SWITCH);
        return MMC_ERR;
    }
    else
    {
        /* NOP */
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Get Response and Check Response Errors
* Function Name: r_mmcif_get_resp
* Description  : Gets the response value from the RESP register.
*              : After finishing this function, it is possible to start the data transfer. 
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : int32_t            resp                ;   Command response
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_get_resp(uint32_t channel, int32_t resp)
{
    mmc_mmchndl_t    *p_hndl = 0;
    uint32_t          status = 0;
    uint32_t          *p_ptr = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* Select the RESP register depend on the response type. */
    switch (resp)
    {
        case MMC_RESP_NON:   /* Non-response */
            /* NOP */
        break;

        case MMC_RESP_R1:    /* Standard response (32bits length) */
        case MMC_RESP_R1b:   /* Standard response (32bits length) with the busy signal */
            status = MMC_INREG(p_hndl, MMC_CERESP0);
            p_hndl->resp_status = status;

            /* Support to mount a locked card. */
            if (status & 0xfdf9e080)        /* Ignore the following card status error.
                                               CARD_IS_LOCKED, UNDERRUN, OVERRUN */
            {
                /* Any status error */
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
                return r_mmcif_check_resp_error(channel);
            }
        break;

        case MMC_RESP_R2_CID:    /* CID register (128-bit length) */
            p_ptr = p_hndl->cid;
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP3);
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP2);
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP1);
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP0);
        break;

        case MMC_RESP_R2_CSD:   /* CSD register (128-bit length) */
            p_ptr = p_hndl->csd;
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP3);
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP2);
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP1);
            *p_ptr++ = MMC_INREG(p_hndl, MMC_CERESP0);
        break;

        case MMC_RESP_R3:       /* OCR register (32-bit length) */
            p_hndl->ocr[0] = MMC_INREG(p_hndl, MMC_CERESP0);
        break;

        case MMC_RESP_R4:       /* 1byte register access */
            p_hndl->resp_r4[0] = MMC_INREG(p_hndl, MMC_CERESP0);
            if (0 == (p_hndl->resp_r4[0] & 0x00008000))
            {
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
                r_mmcif_set_err(channel, MMC_ERR_FAST_IO);
                return p_hndl->error;
            }
        break;

        case MMC_RESP_R5:       /* Interrupt request */
            p_hndl->resp_r5[0] = MMC_INREG(p_hndl, MMC_CERESP0);
        break;

        case MMC_RESP_AUTO12:   /* Auto CMD12 response */
            status = MMC_INREG(p_hndl, MMC_CERESPCMD12);
            p_hndl->resp_status = status;
            if (status & 0xfdffe080)
            {
                /* Any status error */
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
                return r_mmcif_check_resp_error(channel);
            }
        break;

        default:
            /* Unknown type */
            p_hndl->resp_status = 0;
            /* Set the error code. */
            p_hndl->error = MMC_ERR_INTERNAL;
            return MMC_ERR;
        break;
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Check CSD Register
* Function Name: r_mmcif_check_csd
* Description  : Checks the CSD register and then gets the information.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_check_csd(uint32_t channel)
{
    mmc_mmchndl_t          *p_hndl = 0;
    uint32_t                transpeed = 0;
    uint32_t                timevalue = 0;
    uint32_t                erase_sector_size = 0;
    uint32_t                erase_group_size = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ---- CSD Structure ---- */
    p_hndl->csd_structure = (uint8_t)((p_hndl->csd[0] & 0xc0000000u) >> 30u);

    /* ---- SPEC_VERS ---- */
    p_hndl->csd_spec = (uint8_t)((p_hndl->csd[0] & 0x3c000000u) >> 26u);
#if 0
    /* Modified, Correspondence to the MMC version v4.0 or more
                 but don't know how this program works if such devices
                 are connected at present time. */
    if(p_hndl->csd_spec > MMC_SPEC_40)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CSD_VER);
        return MMC_ERR;
    }
#endif  

    /* ---- TAAC/NSAC ---- */
    /* No check, set the fixed data. */

    /* ---- TRAN_SPEED  ---- */
    transpeed = (p_hndl->csd[0] & 0x00000007u);
    timevalue = ((p_hndl->csd[0] & 0x00000078u) >> 3u);
    
    transpeed = (uint32_t)(TranSpeed[transpeed] * TimeValue[timevalue]);
    
    /* ---- Set transfer speed for memory access. ---- */
    if (5200 <= transpeed)
    {
        p_hndl->csd_tran_speed = MMC_CLK_52MHz;
    }
    else if (2600 <= transpeed)
    {
        p_hndl->csd_tran_speed = MMC_CLK_26MHz;
    }
    else if (2000 <= transpeed)
    {
        p_hndl->csd_tran_speed = MMC_CLK_20MHz;
    }
    else if (1000 <= transpeed)
    {
        p_hndl->csd_tran_speed = MMC_CLK_10MHz;
    }
    else if (500 <= transpeed)
    {
        p_hndl->csd_tran_speed = MMC_CLK_5MHz;
    }
    else if (100 <= transpeed)
    {
        p_hndl->csd_tran_speed = MMC_CLK_1MHz;
    }
    else
    {
        p_hndl->csd_tran_speed = MMC_CLK_400kHz;
    }

    /* ---- CCC  ---- */
    p_hndl->csd_ccc = (uint32_t)(((p_hndl->csd[1] & 0xfff00000u) >> 20u));

    /* ---- READ_BL_LEN  ---- */
    p_hndl->csd_read_len = (uint16_t)(1u << ((p_hndl->csd[1] & 0x000f0000u) >> 16u));
    if (p_hndl->csd_read_len > 0x800)   /* not defined */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CSD_RLEN);
        return MMC_ERR;
    }
    
    /* ---- WRITE_BL_LEN  ---- */
    p_hndl->csd_write_len = (uint16_t)(1u << ((p_hndl->csd[3] & 0x03c00000u) >> 22u));
    if(p_hndl->csd_write_len > 0x800)   /* not defined */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CSD_WLEN);
        return MMC_ERR;
    }

    /* ---- Calculate the number of erase sectors. ---- */
    erase_sector_size = ((p_hndl->csd[2] & 0x00007c00u) >> 10u);
    erase_group_size  = ((p_hndl->csd[2] & 0x000003e0u) >> 5u); 

    p_hndl->erase_sect = (erase_sector_size + 1) * (erase_group_size + 1);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Check HOST error flag
* Function Name: r_mmcif_check_info2_err
* Description  : Checks error bits of the MMC_CEINT register. Sets the error bit to hndl->error.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_check_info2_err(uint32_t channel)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            info2 = 0;
    int32_t             bit = 0;

    p_hndl = MMC_GET_HNDL(channel);

    info2 = (uint32_t)(p_hndl->ceint2 & MMC_CEINT2_ALL_ERR);

    /* ---- Search error bit ---- */
    bit = r_mmcif_bit_search(info2);

    if (MMC_ERR == bit)     /* No error */
    {
        r_mmcif_set_err(channel, MMC_SUCCESS);
    }
    else if (1 == bit)      /* Buffer access error */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_stop(channel);
        r_mmcif_set_err(channel, Info2ErrTbl[bit]);
    }
    else                    /* other error */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CMD, __LINE__);
        r_mmcif_set_err(channel, Info2ErrTbl[bit]);
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Get HOST error flag
* Function Name: r_mmcif_get_info2
* Description  : Gets the error bit of the MMC_CEINT register. Sets the register value to hndl->ceint2.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void r_mmcif_get_info2(uint32_t channel)
{
    mmc_mmchndl_t    *p_hndl = 0;
    uint32_t          info2_reg = 0;

    p_hndl = MMC_GET_HNDL(channel);

    info2_reg = (uint32_t)(MMC_INREG(p_hndl, MMC_CEINT) & MMC_CEINT2_ALL_ERR);
    MMC_OUTREG(p_hndl, MMC_CEINT, (uint32_t)~info2_reg);
    if (info2_reg == MMC_INREG(p_hndl, MMC_CEINT))
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    p_hndl->ceint2 = (uint32_t)(p_hndl->ceint2 | info2_reg);
}


/* End of File */
