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
* File Name    : r_sdc_sdio_wrapper.c
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
* Outline      : I/O Abort Operation
* Function Name: R_SDC_SDIO_IOAbort
* Description  : Sets I/O Abort to the specified function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No, (0 to SDC_SDIO_FUNC_MAXNUM)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_IOAbort(uint32_t card_no, uint32_t func)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_IOAbort(card_no, func);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_IOAbort() */

/**********************************************************************************************************************
* Outline      : I/O Enable Setting Operation
* Function Name: R_SDC_SDIO_SetIOEnable
* Description  : Sets the IOEn of I/O Enable using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No, (1 to SDC_SDIO_FUNC_MAXNUM)
*              :                                        ;   Do not set Function No 0.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_SetIOEnable(uint32_t card_no, uint32_t func)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_SetIOEnable(card_no, func);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_SetIOEnable() */

/**********************************************************************************************************************
* Outline      : I/O Enable Clearing Operation
* Function Name: R_SDC_SDIO_ClearIOEnable
* Description  : Clears the IOEn of I/O Enable using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No, (1 to SDC_SDIO_FUNC_MAXNUM)
*              :                                        ;   Do not set Function No 0.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_ClearIOEnable(uint32_t card_no, uint32_t func)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_ClearIOEnable(card_no, func);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_ClearIOEnable() */

/**********************************************************************************************************************
* Outline      : I/O Ready Operation
* Function Name: R_SDC_SDIO_GetIOReady
* Description  : Gets I/O Ready to the specified function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint8_t      * p_data                  ;   Pointer to the data buffer to get I/O Ready register.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_GetIOReady(uint32_t card_no, uint8_t * p_data)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_GetIOReady(card_no, p_data);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_GetIOReady() */

/**********************************************************************************************************************
* Outline      : I/O Reset Setting Operation
* Function Name: R_SDC_SDIO_IOReset
* Description  : Sets the RES of I/O Abort using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_IOReset(uint32_t card_no)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_IOReset(card_no);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_IOReset() */

/**********************************************************************************************************************
* Outline      : Set Interrupt From IO Operation
* Function Name: R_SDC_SDIO_SetIntEnable
* Description  : Sets the IENM of Int Enable.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_SetIntEnable(uint32_t card_no)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_SetIntEnable(card_no);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_SetIntEnable() */

/**********************************************************************************************************************
* Outline      : Clear Interrupt From IO Operation
* Function Name: R_SDC_SDIO_ClearIntEnable
* Description  : Clears the IENM of Int Enable.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_ClearIntEnable(uint32_t card_no)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_ClearIntEnable(card_no);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_ClearIntEnable() */

/**********************************************************************************************************************
* Outline      : Int Enable Getting Operation
* Function Name: R_SDC_SDIO_GetIntEnable
* Description  : Gets I/O Ready to the specified function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint8_t      * p_data                  ;   Pointer to the data buffer to get I/O Ready register.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_GetIntEnable(uint32_t card_no, uint8_t * p_data)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_GetIntEnable(card_no, p_data);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_GetIntEnable() */

/**********************************************************************************************************************
* Outline      : I/O Read Direct Operation
* Function Name: R_SDC_SDIO_ReadDirect
* Description  : Executes I/O Read Direct operation from the specified address using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
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
sdc_sd_status_t R_SDC_SDIO_ReadDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_ReadDirect(card_no, p_sdc_sdio_daccess);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_ReadDirect() */

/**********************************************************************************************************************
* Outline      : I/O Write Direct Operation
* Function Name: R_SDC_SDIO_WriteDirect
* Description  : Executes I/O Write Direct operation to the specified address using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
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
sdc_sd_status_t R_SDC_SDIO_WriteDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_WriteDirect(card_no, p_sdc_sdio_daccess);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_WriteDirect() */

/**********************************************************************************************************************
* Outline      : Register SDHI_SDIOSTS Interrupt Callback Function
* Function Name: R_SDC_SDIO_IntCallback
* Description  : Registers the callback function of SDIOSTS interrupt.
*              : If the SDIOSTS interrupt is occurred, call callback function.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t        (* callback)(int32_t)   ;   callback function
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t))
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_IntCallback(card_no, callback);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_IntCallback() */

/**********************************************************************************************************************
* Outline      : I/O Read Extended Operation
* Function Name: R_SDC_SDIO_Read
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
sdc_sd_status_t R_SDC_SDIO_Read(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_Read(card_no, p_sdc_sdio_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_Read() */

/**********************************************************************************************************************
* Outline      : I/O Read Extended Operation (Software_transfer only)
* Function Name: R_SDC_SDIO_ReadSoftwareTrans
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
sdc_sd_status_t R_SDC_SDIO_ReadSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_ReadSoftwareTrans(card_no, p_sdc_sdio_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_ReadSoftwareTrans() */

/**********************************************************************************************************************
* Outline      : I/O Write Extended Operation
* Function Name: R_SDC_SDIO_Write
* Description  : Executes I/O Write Extended operation to the specified address using CMD53.
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
sdc_sd_status_t R_SDC_SDIO_Write(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_Write(card_no, p_sdc_sdio_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_Write() */

/**********************************************************************************************************************
* Outline      : I/O Write Extended Operation (Software_transfer only)
* Function Name: R_SDC_SDIO_WriteSoftwareTrans
* Description  : Executes I/O Write Extended operation to the specified address using CMD53 by Software transfer.
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
sdc_sd_status_t R_SDC_SDIO_WriteSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_WriteSoftwareTrans(card_no, p_sdc_sdio_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_WriteSoftwareTrans() */

/**********************************************************************************************************************
* Outline      : Set I/O Functions Block Size
* Function Name: R_SDC_SDIO_SetBlocklen
* Description  : Sets I/O block size each function.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t        len                     ;   Block size
*              : uint32_t       func                    ;   Function No, (0 to SDC_SDIO_FUNC_MAXNUM)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_SetBlocklen(uint32_t card_no, int32_t len, uint32_t func)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_SetBlocklen(card_no, len, func);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_SetBlocklen() */

/**********************************************************************************************************************
* Outline      : Get I/O Functions Block Size
* Function Name: R_SDC_SDIO_GetBlocklen
* Description  : Gets I/O block size for each function.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
*              : int32_t      * p_len                   ;   Block size
*              : uint32_t       func                    ;   Function No, (0 - 7)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_GetBlocklen(uint32_t card_no, int32_t * p_len, uint32_t func)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_GetBlocklen(card_no, p_len, func);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_GetBlocklen() */

/**********************************************************************************************************************
* Outline      : Enable SDIO Interrupt from IOIRQM Bit
* Function Name: R_SDC_SDIO_EnableInt
* Description  : Clear IOIRQ bit of SDIOSTS register and enables SDIO interrupt from IOIRQM bit of SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : Access the SDIO card, negate the SDIO interrupt from the SDIO card, and then call this function.
*              : If the SDIO interrupt from the SDIO card is not negated, the IOIRQ flag might become 1 again.
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_EnableInt(uint32_t card_no)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_EnableInt(card_no);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_EnableInt() */

/**********************************************************************************************************************
* Outline      : Disable SDIO Interrupt from IOIRQM Bit
* Function Name: R_SDC_SDIO_DisableInt
* Description  : Disables SDIO interrupt from IOIRQM bit of SDIOIMSK register.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_DisableInt(uint32_t card_no)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_DisableInt(card_no);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_DisableInt() */

/**********************************************************************************************************************
* Outline      : Get CIS Operation
* Function Name: R_SDC_SDIO_GetCIS
* Description  : Gets CIS to the specified function.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : sdc_sdio_cis_t * p_sdc_sdio_cis
*              :    uint32_t    func                    ;   Function No, (0 to SDC_SDIO_FUNC_MAXNUM)
*              :    int32_t     cnt                     ;   Get data size
*              :    uint32_t    * p_comm_cis_ptr        ;   Pointer of Common CIS Pointer to the specified function
*              :    uint8_t     * p_cis_buff            ;   Pointer of CIS buffer
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDIO_GetCIS(uint32_t card_no, sdc_sdio_cis_t * p_sdc_sdio_cis)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdio_GetCIS(card_no, p_sdc_sdio_cis);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDIO_GetCIS() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
