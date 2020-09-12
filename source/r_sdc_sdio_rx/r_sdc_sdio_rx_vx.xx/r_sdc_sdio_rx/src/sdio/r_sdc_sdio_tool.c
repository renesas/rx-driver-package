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
* File Name    : r_sdc_sdio_tool.c
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
* Outline      : I/O Abort Operation
* Function Name: r_sdc_sdio_IOAbort
* Description  : Sets I/O Abort to the specified function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No, (0 to SDC_SDIO_FUNC_MAXNUM)
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_IOAbort(uint32_t card_no, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t       ret = SDC_SD_SUCCESS;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if (r_sdc_sdio_get_func_max_num() < func)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    io_buff[0]                 = (uint8_t)func;
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOABORT;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_WRITE;    /* Because of including W/O register. */

    ret = r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_IOAbort() */

/**********************************************************************************************************************
* Outline      : I/O Enable Setting Operation
* Function Name: r_sdc_sdio_SetIOEnable
* Description  : Sets the IOEn of I/O Enable using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No, (1 to SDC_SDIO_FUNC_MAXNUM)
*              :                                        ;   Do not set Function No 0.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_SetIOEnable(uint32_t card_no, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t       ret = SDC_SD_SUCCESS;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    uint8_t             func_data = 0;
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if ((0 == func) || (r_sdc_sdio_get_func_max_num() < func))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    ret = r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    }

    /* Convert from func to func_data. */
    func_data = 1;
    func_data <<= (uint8_t)func;
    
    io_buff[0] = (io_buff[0] | func_data);
    io_buff[1] = io_buff[0];                /* Store the value of IOE in order to verify. */
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

    r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);

    if (io_buff[0] != io_buff[1])           /* Verify error */
    {
        /* Retry to read */
        io_buff[0] = 0;
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOENABLE;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
        
        if (io_buff[0] != io_buff[1])           /* Verify error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_SetIOEnable() */

/**********************************************************************************************************************
* Outline      : I/O Enable Clearing Operation
* Function Name: r_sdc_sdio_ClearIOEnable
* Description  : Clears the IOEn of I/O Enable using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint32_t       func                    ;   Function No, (1 to SDC_SDIO_FUNC_MAXNUM)
*              :                                        ;   Do not set Function No 0.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_ClearIOEnable(uint32_t card_no, uint32_t func)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t       ret = SDC_SD_SUCCESS;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    uint8_t             func_data = 0;
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if ((0 == func) || (r_sdc_sdio_get_func_max_num() < func))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    ret = r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    }

    /* Convert from func to func_data. */
    func_data = 1;
    func_data <<= (uint8_t)func;

    io_buff[0] = (io_buff[0] & (~func_data));
    io_buff[1] = io_buff[0];                /* Store the value of IOE in order to verify. */
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

    r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);

    if (io_buff[0] != io_buff[1])           /* Verify error */
    {
        /* Retry to read */
        io_buff[0] = 0;
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOENABLE;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
        
        if (io_buff[0] != io_buff[1])           /* Verify error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_ClearIOEnable() */

/**********************************************************************************************************************
* Outline      : I/O Ready Operation
* Function Name: r_sdc_sdio_GetIOReady
* Description  : Gets I/O Ready to the specified function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint8_t      * p_data                  ;   Pointer to the data buffer to get I/O Ready register.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_GetIOReady(uint32_t card_no, uint8_t * p_data)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t       ret = SDC_SD_SUCCESS;
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if (0 == p_data)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    sdc_sdio_daccess.p_buff   = p_data;
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOREADY;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    ret = r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_GetIOReady() */

/**********************************************************************************************************************
* Outline      : I/O Reset Setting Operation
* Function Name: r_sdc_sdio_IOReset
* Description  : Sets the RES of I/O Abort using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_IOReset(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t ret = SDC_SD_SUCCESS;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    io_buff[0] = SDC_SDIO_IOABORT_RES;
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_IOABORT;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_WRITE;        /* for I/O Card Reset */

    ret = r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_IOReset() */

/**********************************************************************************************************************
* Outline      : Set Interrupt From IO Operation
* Function Name: r_sdc_sdio_SetIntEnable
* Description  : Sets the IENM of Int Enable.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_SetIntEnable(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t ret = SDC_SD_SUCCESS;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    ret = r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    }

    io_buff[0] |= SDC_SDIO_INTENABLE_IENM;
    io_buff[1] = io_buff[0];
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

    r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);

    if (io_buff[0] != io_buff[1])           /* Verify error */
    {
        /* Retry to read */
        io_buff[0] = 0;
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
        
        if (io_buff[0] != io_buff[1])           /* Verify error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
    }

    /* Save Int Enable register */
    p_hndl->io_reg[0][SDC_SDIO_ADRS_INTENABLE] = io_buff[0];

    return p_hndl->error;
} /* End of function r_sdc_sdio_SetIntEnable() */

/**********************************************************************************************************************
* Outline      : Clear Interrupt From IO Operation
* Function Name: r_sdc_sdio_ClearIntEnable
* Description  : Clears the IENM of Int Enable.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_ClearIntEnable(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t ret = SDC_SD_SUCCESS;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    ret = r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    }

    io_buff[0] &= (~SDC_SDIO_INTENABLE_IENM);
    io_buff[1] = io_buff[0];
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

    r_sdc_sdio_WriteDirect(card_no, &sdc_sdio_daccess);

    if (io_buff[0] != io_buff[1])           /* Verify error */
    {
        /* Retry to read */
        io_buff[0] = 0;
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
        
        if (io_buff[0] != io_buff[1])           /* Verify error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
    }

    /* Save Int Enable register */
    p_hndl->io_reg[0][SDC_SDIO_ADRS_INTENABLE] = io_buff[0];

    return p_hndl->error;
} /* End of function r_sdc_sdio_ClearIntEnable() */

/**********************************************************************************************************************
* Outline      : Int Enable Getting Operation
* Function Name: r_sdc_sdio_GetIntEnable
* Description  : Gets I/O Ready to the specified function using CMD52.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
*              : uint8_t      * p_data                  ;   Pointer to the data buffer to get I/O Ready register.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_GetIntEnable(uint32_t card_no, uint8_t * p_data)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sd_status_t       ret = SDC_SD_SUCCESS;
    
    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if (0 == p_data)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    sdc_sdio_daccess.p_buff   = p_data;
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;
    ret = r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    if (SDC_SD_ERR_STOP == ret)
    {
        p_hndl->error = SDC_SD_SUCCESS;
        r_sdc_sdio_ReadDirect(card_no, &sdc_sdio_daccess);
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_GetIntEnable() */

/**********************************************************************************************************************
* Outline      : Get CIS Operation
* Function Name: r_sdc_sdio_GetCIS
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
sdc_sd_status_t r_sdc_sdio_GetCIS(uint32_t card_no, sdc_sdio_cis_t * p_sdc_sdio_cis)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    sdc_sdio_access_t  sdc_sdio_access;
    int32_t             i = 0;
    int32_t             block_size = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ==== Check parameters. ==== */

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if ((p_sdc_sdio_cis->cnt & (p_sdc_sdio_cis->cnt-1)) ||
        (0 > p_sdc_sdio_cis->cnt) || (512 < p_sdc_sdio_cis->cnt))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        return SDC_SD_ERR;
    }

    if ((0 == p_sdc_sdio_cis->p_comm_cis_ptr) || (0 == (uint32_t)p_sdc_sdio_cis->p_cis_buff))
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    if ((uint32_t)p_sdc_sdio_cis->p_cis_buff & 0x3u)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Get the Function N CIS pointer and store it. */
    *(p_sdc_sdio_cis->p_comm_cis_ptr) = 
        (uint32_t)(((uint32_t)p_hndl->io_reg[p_sdc_sdio_cis->func][SDC_SDIO_ADRS_CISPTR2] << SDC_SD_BIT_SHIFT_16) |
        ((uint32_t)p_hndl->io_reg[p_sdc_sdio_cis->func][SDC_SDIO_ADRS_CISPTR1] << SDC_SD_BIT_SHIFT_8) |
        p_hndl->io_reg[p_sdc_sdio_cis->func][SDC_SDIO_ADRS_CISPTR0]);

    if (*(p_sdc_sdio_cis->p_comm_cis_ptr))
    {
        block_size = r_sdc_sdio_get_block_size(SDC_SDIO_FNO_0);
        if (block_size != 0) /* Use CMD53 byte mode */
        {
            sdc_sdio_access.p_buff  = p_sdc_sdio_cis->p_cis_buff;
            sdc_sdio_access.func    = SDC_SDIO_FNO_0;
            sdc_sdio_access.adr     = *(p_sdc_sdio_cis->p_comm_cis_ptr);
            sdc_sdio_access.op_code = (SDC_SDIO_BYTE_MODE | SDC_SDIO_INCREMENT_ADDR);
            for (i = p_sdc_sdio_cis->cnt; i > 0; i -= block_size)
            {
                sdc_sdio_access.cnt = block_size;
                if (R_SDC_SDIO_ReadSoftwareTrans(card_no, &sdc_sdio_access) != SDC_SD_SUCCESS)
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
                    return p_hndl->error;
                }
                sdc_sdio_access.p_buff += block_size;
                sdc_sdio_access.adr += block_size;
            }
        }
        else /* Use CMD52 */
        {
            sdc_sdio_daccess.p_buff   = p_sdc_sdio_cis->p_cis_buff;
            sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
            sdc_sdio_daccess.adr      = *(p_sdc_sdio_cis->p_comm_cis_ptr);
            for (i = 0; i < p_sdc_sdio_cis->cnt; i++)
            {
                if (R_SDC_SDIO_ReadDirect(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
                    return p_hndl->error;
                }
                sdc_sdio_daccess.p_buff++;
                sdc_sdio_daccess.adr++;
            }
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_TOOL, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_GetCIS() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
