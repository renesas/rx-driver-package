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
* File Name    : r_sdc_sdmem_wrapper.c
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
* Outline      : Read Data from SD Memory
* Function Name: R_SDC_SDMEM_Read
* Description  : Reads the data from the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t    * p_sdc_sd_access   ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDMEM_Read(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
    
    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdmem_Read(card_no, p_sdc_sd_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDMEM_Read() */

/**********************************************************************************************************************
* Outline      : Read Data from SD Memory (Software_transfer only)
* Function Name: R_SDC_SDMEM_ReadSoftwareTrans
* Description  : Reads the data from the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t    * p_sdc_sd_access   ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDMEM_ReadSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdmem_ReadSoftwareTrans(card_no, p_sdc_sd_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDMEM_ReadSoftwareTrans() */

/**********************************************************************************************************************
* Outline      : Read Data from SD Memory (Software_transfer_Single_CMD only)
* Function Name: R_SDC_SDMEM_ReadSoftwareTransSingleCmd
* Description  : Reads the data from the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  * p_sdc_sd_access     ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDMEM_ReadSoftwareTransSingleCmd(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdmem_ReadSoftwareTransSingleCmd(card_no, p_sdc_sd_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDMEM_ReadSoftwareTransSingleCmd() */

/**********************************************************************************************************************
* Outline      : Write Data to SD Memory
* Function Name: R_SDC_SDMEM_Write
* Description  : Writes data to the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDMEM_Write(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdmem_Write(card_no, p_sdc_sd_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDMEM_Write() */

/**********************************************************************************************************************
* Outline      : Write Data to SD Memory (Software_transfer only)
* Function Name: R_SDC_SDMEM_WriteSoftwareTrans
* Description  : Writes data to the SD Memory.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_sd_access_t  *p_sdc_sd_access      ;   SDHI Driver transfer configuration
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDMEM_WriteSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access)
{
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_api_lock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }

    ret = r_sdc_sdmem_WriteSoftwareTrans(card_no, p_sdc_sd_access);
    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return ret;
    }

    if (r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]) != true)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_API_LOCK);
        r_sdc_sd_api_unlock(&g_sdc_sd_api_locks[card_no]);
        return SDC_SD_ERR_API_LOCK;
    }
    
    return ret;
} /* End of function R_SDC_SDMEM_WriteSoftwareTrans() */

/**********************************************************************************************************************
* Outline      : get card speed
* Function Name: R_SDC_SDMEM_GetSpeed
* Description  : Gets the card speed class and performance move
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint8_t            * p_clss            ;   Speed class
*              : uint8_t            * p_move            ;   Performance move
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SDMEM_GetSpeed(uint32_t card_no, uint8_t * p_clss, uint8_t * p_move)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (SDC_SD_CFG_CARD_NUM <= card_no)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_WRAPPER, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_sdc_sdmem_GetSpeed(card_no, p_clss, p_move);
} /* End of function R_SDC_SDMEM_GetSpeed() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */

/* End of File */
