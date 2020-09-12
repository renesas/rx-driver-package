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
* File Name    : r_sdc_sdio_param.c
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
* Outline      : Check Parameter Of Extended Operation
* Function Name: r_sdc_sdio_chk_param_extend
* Description  : Checks parameters of extended operation.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : sdc_sdio_access_t * p_sdc_sdio_access
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function number
*              :    uint32_t    adr                     ;   Register address
*              :    int32_t     cnt                     ;   Number of read (byte)
*              :    uint32_t    op_code                 ;   Operation code
*              :    uint32_t    trans_mode              ;   Transfer mode (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_chk_param_extend(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        return SDC_SD_ERR;
    }

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sdio_access)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    if (0 == (uint32_t)p_sdc_sdio_access->p_buff)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    /* Check register address and function number. */
    if ((p_sdc_sdio_access->func > r_sdc_sdio_get_func_max_num()) ||
        (p_sdc_sdio_access->adr > SDC_SDIO_ADRS_MAX))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    if (0 >= p_sdc_sdio_access->cnt)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;
#else
    return SDC_SD_SUCCESS;
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
} /* End of function r_sdc_sdio_chk_param_extend() */

/**********************************************************************************************************************
* Outline      : Check Parameter Of Direct Operation
* Function Name: r_sdc_sdio_chk_param_direct
* Description  : Checks parameters of direct operation.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : sdc_sdio_daccess_t * p_sdc_sdio_daccess
*              :    uint8_t     * p_buff                ;   Data buffer
*              :    uint32_t    func                    ;   Function No
*              :    uint32_t    adr                     ;   Register address
*              :    uint32_t    raw_flag                ;   Read after Write flag (Setting not required)
*              :    uint32_t    rw_flag                 ;   Read : 0, Write : 1 (Setting not required)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_chk_param_direct(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        return SDC_SD_ERR;
    }

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_sdc_sdio_daccess)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    if (0 == (uint32_t)p_sdc_sdio_daccess->p_buff)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    /* Check register address and function number. */
    if ((p_sdc_sdio_daccess->func > r_sdc_sdio_get_func_max_num()) ||
        (p_sdc_sdio_daccess->adr > SDC_SDIO_ADRS_MAX))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;
#else
    return SDC_SD_SUCCESS;
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
} /* End of function r_sdc_sdio_chk_param_direct() */

/**********************************************************************************************************************
* Outline      : Check Parameter Of Block Length Operation
* Function Name: r_sdc_sdio_chk_param_blocklen
* Description  : Checks parameters of block length operation.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_chk_param_blocklen(uint32_t card_no, uint32_t func)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    sdc_sd_hndl_t       * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        return SDC_SD_ERR;
    }

    if (r_sdc_sdio_get_func_max_num() < func)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_PARAM, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;
#else
    return SDC_SD_SUCCESS;
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
} /* End of function r_sdc_sdio_chk_param_blocklen() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
