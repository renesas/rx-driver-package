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
* File Name    : r_sdc_sdmem_util.c
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

/**********************************************************************************************************************
* Outline      : Control SD Bus Width
* Function Name: r_sdc_sdmem_set_port
* Description  : Changes the SD memory bus width.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            port                ;   SDHI_PORT_1BIT, SDHI_PORT_4BIT
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_set_port(uint32_t card_no, int32_t port)
{
    sdc_sd_hndl_t    * p_hndl = 0;
    uint32_t              arg = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Check the card state. ---- */
    if (SDC_SD_STATE_TRAN == (p_hndl->resp_status & SDC_SD_RES_STATE))  /* transfer state */
    {
        if (SDC_SD_MODE_1BIT == port)
        {
            arg = SDC_SD_ARG_ACMD6_1bit;
        }
        else
        {
            arg = SDC_SD_ARG_ACMD6_4bit;
        }
        /* ==== Change the SD bus width using ACMD6. ==== */
        if (r_sdc_sd_send_acmd(card_no, SDC_SD_ACMD6, arg) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_UTIL, __LINE__);
            return SDC_SD_ERR;
        }
        if (r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R1) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_UTIL, __LINE__);
            return SDC_SD_ERR;
        }
    }

    return p_hndl->error;

} /* End of function r_sdc_sdmem_set_port() */

/**********************************************************************************************************************
* Outline      : Read or Write Error Operation
* Function Name: r_sdc_sdmem_read_write_error
* Description  : Executes the read or write error operation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_read_write_error(uint32_t card_no)
{
    sdc_sd_hndl_t            * p_hndl = 0;
    volatile sdc_sd_status_t   error = SDC_SD_SUCCESS;
    uint32_t                 sdhi_option = 0;
    uint32_t                 sdhi_clk_ctrl = 0;
    uint32_t                 sdhi_sddma_ctrl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    error = p_hndl->error;

    R_SDC_SD_SetDmacDtcTransFlg(card_no, SDC_SD_SET_TRANS_STOP);

    r_sdc_sd_disable_icu_int(card_no);

    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, 0);

    r_sdc_sd_enable_icu_int(card_no);

    /* ---- Data transfer stop using CMD12. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_STP_STOP);

    /* ---- Wait for All end. ---- */
    r_sdc_sd_int_err_mem_wait(card_no, SDC_SD_TIMEOUT_RESP);

    r_sdc_sd_disable_icu_int(card_no);

    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_ClearSdstsReg(p_hndl->channel, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    R_SDHI_InReg(p_hndl->channel, SDHI_SDOPT, &sdhi_option);
    R_SDHI_InReg(p_hndl->channel, SDHI_SDCLKCR, &sdhi_clk_ctrl);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDRST, SDHI_SDRST_ENABLE);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDRST, SDHI_SDRST_DISABLE);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_INIT);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDOPT, sdhi_option);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDCLKCR, sdhi_clk_ctrl);

    /* Disable the DMA/DTC. */
    R_SDHI_InReg(p_hndl->channel, SDHI_SDDMAEN, &sdhi_sddma_ctrl);
    sdhi_sddma_ctrl = (uint32_t)(sdhi_sddma_ctrl & (~SDHI_SDDMAEN_DMASDRW));
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDDMAEN, sdhi_sddma_ctrl);

    r_sdc_sd_enable_icu_int(card_no);

    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_STP_STOP);
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_INIT);

    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1,p_hndl->rca[0]) == SDC_SD_SUCCESS)
    {
        if (SDC_SD_STATE_TRAN != (p_hndl->resp_status & SDC_SD_RES_STATE))
        {
            r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD12, SDC_SD_RESP_R1b, p_hndl->rca[0]);
        }
    }

    /* ---- Disable all interrupts. ---- */
    r_sdc_sd_disable_icu_int(card_no);

    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_CLEAR);

    r_sdc_sd_enable_icu_int(card_no);

    p_hndl->error = error;

    return p_hndl->error;

} /* End of function r_sdc_sdmem_read_write_error() */

/**********************************************************************************************************************
* Outline      : get card speed
* Function Name: r_sdc_sdmem_GetSpeed
* Description  : Gets the card speed class and performance move
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint8_t            * p_clss            ;   Speed class
*              : uint8_t            * p_move            ;   Performance move
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_GetSpeed(uint32_t card_no, uint8_t * p_clss, uint8_t * p_move)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;  /* not initialized */
    }

    /* ---- Check the card is mounted. ---- */
    if (SDC_SD_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_UTIL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (p_clss)
    {
        *p_clss = p_hndl->speed_class;
    }

    if (p_move)
    {
        *p_move = p_hndl->perform_move;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_GetSpeed() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */

/* End of File */
