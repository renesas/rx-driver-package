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
* File Name    : r_sdc_sd_wrapper.c
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
int32_t     g_sdc_sd_api_locks[SDC_SD_CFG_CARD_NUM];

/**********************************************************************************************************************
* Outline      : Initialize SD Driver
* Function Name: R_SDC_SD_Open
* Description  : Initializes the SD Driver work memory specified by channel.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           channel             ;   SDHI Driver Channel No.
*              : void               * p_sdc_sd_workarea ;   SDC SD Driver work memory
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*              : SDC_SD_ERR_CPU_IF                      ;   CPU-IF function error
*              : SDC_SD_ERR_ADDRESS_BOUNDARY            ;   Not specified buffer address in 4-byte unit
*              : SDC_SD_ERR_API_LOCK                    ;   Error when calling API during API call (For Combo Card)
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_Open(uint32_t card_no, uint32_t channel, void * p_sdc_sd_workarea)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sd_Open(card_no, channel, p_sdc_sd_workarea);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SD_Open() */

/**********************************************************************************************************************
* Outline      : Control SDHI driver
* Function Name: R_SDC_SD_Control
* Description  : Controls the SDHI driver using the specified command.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_cmd_t         * p_sdc_sd_cmd    ;   Command
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_Control(uint32_t card_no, sdc_sd_cmd_t * p_sdc_sd_cmd)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_Control(card_no, p_sdc_sd_cmd);
} /* End of function R_SDC_SD_Control() */

/**********************************************************************************************************************
* Outline      : Set Card Detection Interrupt
* Function Name: R_SDC_SD_CdInt
* Description  : Sets card detection interrupt.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            enable              ;   Card detection interrupt mode
*              :                                        ;    SDC_SD_CD_INT_ENABLE:Enables interrupt.
*              :                                        ;    SDC_SD_CD_INT_DISABLE:Disables interrupt.
*              : sdc_sd_status_t  ( *callback)(int32_t) ;   Interrupt callback function
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_CdInt(uint32_t card_no, int32_t enable, sdc_sd_status_t (* callback)(int32_t))
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sd_CdInt(card_no, enable, callback);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SD_CdInt() */

/**********************************************************************************************************************
* Outline      : Check Card Insertion
* Function Name: R_SDC_SD_GetCardDetection
* Description  : Checks card insertion.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   A card is inserted.
*              : SDC_SD_ERR                             ;   A Card is not inserted.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_GetCardDetection(uint32_t card_no)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_GetCardDetection(card_no);
} /* End of function R_SDC_SD_GetCardDetection() */

/**********************************************************************************************************************
* Outline      : Initialize SD Card
* Function Name: R_SDC_SD_Initialize
* Description  : Initializes the SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_cfg_t       * p_sdc_sd_config   ;   SDHI Driver operation configuration
*              : uint32_t           init_type           ;   SDHI Initialize type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_Initialize(uint32_t card_no, sdc_sd_cfg_t * p_sdc_sd_config, uint32_t init_type)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sd_Initialize(card_no, p_sdc_sd_config, init_type);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SD_Initialize() */

/**********************************************************************************************************************
* Outline      : Get Mode Status
* Function Name: R_SDC_SD_GetModeStatus
* Description  : Gets the mode status.
* Arguments    : uint32_t               card_no         ;   SD Card No.
*              : uint8_t                * p_mode        ;   Mode Status configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_GetModeStatus(uint32_t card_no, uint8_t * p_mode)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_GetModeStatus(card_no, p_mode);
} /* End of function R_SDC_SD_GetModeStatus() */

/**********************************************************************************************************************
* Outline      : Get Card Status
* Function Name: R_SDC_SD_GetCardStatus
* Description  : Gets the card status.
* Arguments    : uint32_t               card_no         ;   SD Card No.
*              : sdc_sd_card_status_t   * p_sdc_sd_card_status  ;   Card Status configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_GetCardStatus(uint32_t card_no, sdc_sd_card_status_t * p_sdc_sd_card_status)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_GetCardStatus(card_no, p_sdc_sd_card_status);
} /* End of function R_SDC_SD_GetCardStatus() */

/**********************************************************************************************************************
* Outline      : Get Card Information
* Function Name: R_SDC_SD_GetCardInfo
* Description  : Gets the card information.
* Arguments    : uint32_t            card_no            ;   SD Card No.
*              : sdc_sd_card_reg_t   * p_sdc_sd_card_reg   ;   CardReg configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_GetCardInfo(uint32_t card_no, sdc_sd_card_reg_t * p_sdc_sd_card_reg)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_GetCardInfo(card_no, p_sdc_sd_card_reg);
} /* End of function R_SDC_SD_GetCardInfo() */

/**********************************************************************************************************************
* Outline      : End SD Card
* Function Name: R_SDC_SD_End
* Description  : Ends the SD Card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           end_type            ;   SDHI End type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_End(uint32_t card_no, uint32_t end_type)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sd_End(card_no, end_type);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SD_End() */

/**********************************************************************************************************************
* Outline      : Register SDSTS1 or SDSTS2 Interrupt Callback Function
* Function Name: R_SDC_SD_IntCallback
* Description  : Registers the callback function of SDSTS1 or SDSTS2 interrupt.
*              : If the SDSTS1 or SDSTS2 interrupt are occurred, calls callback function.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_status_t  (* callback)(int32_t) ;   Callback function
* Return Value : SDC_SD_SUCCESS                         ;   Generate interrupt
*              : SDC_SD_ERR                             ;   Not generated interrupt
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t))
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_IntCallback(card_no, callback);
} /* End of function R_SDC_SD_IntCallback() */

/**********************************************************************************************************************
* Outline      : Get SD Driver Errors
* Function Name: R_SDC_SD_GetErrCode
* Description  : Gets the SD Driver errors (=ret) and returns it
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : Error code                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_GetErrCode(uint32_t card_no)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_GetErrCode(card_no);
} /* End of function R_SDC_SD_GetErrCode() */

/**********************************************************************************************************************
* Outline      : Finish SD Driver
* Function Name: R_SDC_SD_Close
* Description  : Finishes the SDHI Driver operation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : After this function finished, SD handle is unavailable.
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_Close(uint32_t card_no)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sd_Close(card_no);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SD_Close() */

/**********************************************************************************************************************
* Outline      : Get Buffer Register Address
* Function Name: R_SDC_SD_GetBuffRegAddress
* Description  : Gets the Buffer Register Address.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           * p_reg_buff        ;   Buffer Register Address
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_GetBuffRegAddress(uint32_t card_no, uint32_t * p_reg_buff)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_GetBuffRegAddress(card_no, p_reg_buff);
} /* End of function R_SDC_SD_GetBuffRegAddress() */

/**********************************************************************************************************************
* Outline      : Get Version
* Function Name: R_SDC_SD_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t  R_SDC_SD_GetVersion(void)
{
    return r_sdc_sd_GetVersion();
} /* End of function R_SDC_SD_GetVersion() */

/**********************************************************************************************************************
* Outline      : SDC_SD 1ms Interval
* Function Name: R_SDC_SD_1msInterval
* Description  : 1ms Interval Timer call function.
* Arguments    : None
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void R_SDC_SD_1msInterval(void)
{
    r_sdc_sd_1msInterval();
} /* End of function R_SDC_SD_1msInterval() */

/**********************************************************************************************************************
* Outline      : Set DMAC/DTC transfer End Flag
* Function Name: R_SDC_SD_SetDmacDtcTransFlg
* Description  : Sets DMAC/DTC transfer end flag.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           flg                 ;   Transfer: 1  No transfer: 0 
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_SetDmacDtcTransFlg(uint32_t card_no, uint32_t flg)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sd_SetDmacDtcTransFlg(card_no, flg);
} /* End of function R_SDC_SD_SetDmacDtcTransFlg() */

/**********************************************************************************************************************
* Outline      : Get Log HNDL Address
* Function Name: R_SDC_SD_SetLogHdlAddress
* Description  : Gets the Log HNDL Address.
* Arguments    : uint32_t           user_byte_que       ;   user_byte_que
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_SetLogHdlAddress(uint32_t user_long_que)
{
    return r_sdc_sd_SetLogHdlAddress(user_long_que);
} /* End of function R_SDC_SD_SetLogHdlAddress() */

/**********************************************************************************************************************
* Outline      : Store Debugging Information
* Function Name: R_SDC_SD_Log
* Description  : Stores the debugging information.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   SDHI driver file No.
*              : uint32_t           line                ;   SDHI driver line No.
* Return Value : 0
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t R_SDC_SD_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
    return r_sdc_sd_Log(flg, fid, line);
} /* End of function R_SDC_SD_Log() */

/* End of File */
