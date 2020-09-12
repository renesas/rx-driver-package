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
* Copyright (C) 2016(2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDSI Driver
* File Name    : r_sdsi_rx.c
* Version      : 1.00
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDSI API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 30.09.2016 1.00    First Release
*              : 31.07.2017 2.00    Supported RX65N-2MB.
*              :                    Fixed to correspond to Renesas coding rule.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "platform.h"
#include "r_sdsi_rx_if.h"
#include ".\src\r_sdsi_rx_private.h"
#ifdef SDSI_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
#include "r_longq_if.h"
#endif /* SDSI_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
sdsi_hndl_t * g_sdsi_hndl[((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t))];
static sdsi_status_t r_sdsi_init_hndl(uint32_t channel);

#ifdef SDSI_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
static longq_hdl_t     p_sdsi_long_que = 0;             /* LONGQ module                         */
#endif /* SDSI_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
* Function Name: R_SDSI_Open
* Description  : Opens SDSI Driver.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : void               * p_sdsi_workarea    ;   SDSI Driver work memory
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*              : SDSI_ERR_ADDRESS_BOUNDARY               ;   Not specified buffer address in 4-byte unit
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_Open(uint32_t channel, void * p_sdsi_workarea)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t    * p_ptr = 0;
    int16_t       i = 0;
#ifdef SDSI_CFG_USE_FIT
    bool          chk = false;
#endif /* SDSI_CFG_USE_FIT */

#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

#ifdef SDSI_CFG_USE_FIT
    /* Get the SDSI hardware resource. */
    chk = r_sdsi_dev_hardware_lock(channel);
    if (false == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_USE_FIT */

    /* Initialize the work memory. */
    if (0 == (uint32_t)p_sdsi_workarea)
    {
#ifdef SDSI_CFG_USE_FIT
        /* Release the SDSI hardware resource. */
        r_sdsi_dev_hardware_unlock(channel);
#endif /* SDSI_CFG_USE_FIT */
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        g_sdsi_hndl[channel] = 0;    /* Release the SDSI handle. */
        return SDSI_ERR;
    }

    /* Check the work memory boundary (4-byte unit) */
    if ((uint32_t)p_sdsi_workarea & SDSI_ADDR_BOUNDARY)
    {
#ifdef SDSI_CFG_USE_FIT
        /* Release the SDSI hardware resource. */
        r_sdsi_dev_hardware_unlock(channel);
#endif /* SDSI_CFG_USE_FIT */
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        g_sdsi_hndl[channel] = 0;    /* Release the SDSI handle. */
        return SDSI_ERR_ADDRESS_BOUNDARY;
    }

    p_hndl = (sdsi_hndl_t *)p_sdsi_workarea;
    g_sdsi_hndl[channel] = p_hndl;

    /* Clear the work memory to '0'. */
    p_ptr = (uint32_t *)p_hndl;
    for (i = (sizeof(sdsi_hndl_t) / sizeof(uint32_t)); i > 0; i--)
    {
        *p_ptr++ = 0;
    }

    /* Set the SDSI register base address. */
    p_hndl->reg_base = (uint32_t)g_sdsi_cfg_ip_base[channel];

    /* Set to disable all interrupts. */
    r_sdsi_dev_disable_system_int(channel);

    /* Enable the peripheral module. */
    r_sdsi_dev_module_enable(channel);

    /* Open setting of SDSI register. */
    r_sdsi_dev_open(channel);

    /* Clear to disable all interrupts. */
    r_sdsi_dev_enable_system_int(channel);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_Open() */

/**********************************************************************************************************************
* Function Name: R_SDSI_Close
* Description  : Closes SDSI Driver.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_Close(uint32_t channel)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == g_sdsi_hndl[channel])
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Disable all interrupts. */
    r_sdsi_dev_disable_int(channel);

    /* Close setting of SDSI register. */
    r_sdsi_dev_close(channel);

    /* Disable the peripheral module. */
    r_sdsi_dev_module_disable(channel);

    /* Destroy the SDSI Handle and SDSI driver trans status. */
    g_sdsi_hndl[channel] = 0;

#ifdef SDSI_CFG_USE_FIT
    /* Release the SDSI hardware resource. */
    r_sdsi_dev_hardware_unlock(channel);
#endif /* SDSI_CFG_USE_FIT */

    return SDSI_SUCCESS;

} /* End of function R_SDSI_Close() */

/**********************************************************************************************************************
* Function Name: R_SDSI_Initialize
* Description  : Initializes SDSI.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_Initialize(uint32_t channel)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Initializes the SDSI handle. */
    r_sdsi_init_hndl(channel);

    /* Disable ICU interrupts. */
    r_sdsi_dev_disable_int(channel);

    /* Initialize setting of SDSI register. */
    r_sdsi_dev_initialize(channel);

    /* Enable SDSI interrupts. */
    r_sdsi_dev_enable_int(channel);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_Initialize() */

/**********************************************************************************************************************
* Function Name: R_SDSI_End
* Description  : Ends SDSI.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_End(uint32_t channel)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Disable ICU interrupts. */
    r_sdsi_dev_disable_int(channel);

    /* End setting of SDSI register. */
    r_sdsi_dev_end(channel);

    /* Initializes the SDSI handle. */
    r_sdsi_init_hndl(channel);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_End() */

/**********************************************************************************************************************
* Function Name: R_SDSI_CflagPolling
* Description  : Check the C Flag.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   C Flag is 1 (Ready).
*              : SDSI_ERR_BUSY                           ;   C Flag is 0 (Busy).
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_CflagPolling(uint32_t channel)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)(sizeof(g_sdsi_cfg_ip_base) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Check the C Flag. */
    if (r_sdsi_dev_c_flag_polling(channel) != SDSI_SUCCESS)
    {
        return SDSI_ERR_BUSY;
    }
    return SDSI_SUCCESS;
} /* End of function R_SDSI_CflagPolling() */

/**********************************************************************************************************************
* Function Name: R_SDSI_WriteCisReg
* Description  : Writes CIS register.
* Arguments    : uint32_t       channel         ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg        ;
*              :    uint32_t    reg_no          ;   Ignore value
*              :    uint32_t    offset          ;   Offset from the targeted register.
*              :    uint32_t    * p_buff        ;   Register value
* Return Value : SDSI_SUCCESS                   ;   Successful operation
*              : SDSI_ERR                       ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_WriteCisReg(uint32_t channel,sdsi_cis_reg_t * p_sdsi_reg)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (SDSI_CIS_OFFSET_MAX < p_sdsi_reg->offset)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (SDSI_OFFSET_BOUNDARY & p_sdsi_reg->offset)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == p_sdsi_reg->p_buff)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Set SDSI CIS register. */
    r_sdsi_dev_write_cis_reg(channel, p_sdsi_reg);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_WriteCisReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_ReadCisReg
* Description  : Reads CIS register.
* Arguments    : uint32_t       channel         ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg        ;
*              :    uint32_t    reg_no          ;   Ignore value
*              :    uint32_t    offset          ;   Offset from the targeted register.
*              :    uint32_t    * p_buff        ;   Register value
* Return Value : SDSI_SUCCESS                   ;   Successful operation
*              : SDSI_ERR                       ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_ReadCisReg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (SDSI_CIS_OFFSET_MAX < p_sdsi_reg->offset)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (SDSI_OFFSET_BOUNDARY & p_sdsi_reg->offset)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == p_sdsi_reg->p_buff)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Get SDSI CIS register. */
    r_sdsi_dev_read_cis_reg(channel, p_sdsi_reg);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_ReadCisReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_WriteFuncReg
* Description  : Writes function 1 register N.
* Arguments    : uint32_t       channel         ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg        ;
*              :    uint32_t    reg_no          ;   Register No.
*              :                                ;       SDSI_FUNC1_REG1
*              :                                ;       SDSI_FUNC1_REG2
*              :                                ;       SDSI_FUNC1_REG5
*              :    uint32_t    offset          ;   Offset from the targeted register.
*              :    uint32_t    * p_buff        ;   Register value
* Return Value : SDSI_SUCCESS                   ;   Successful operation
*              : SDSI_ERR                       ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_WriteFuncReg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (0 == p_sdsi_reg->reg_no)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if ((SDSI_FUNC1_REG3 == p_sdsi_reg->reg_no) ||
        (SDSI_FUNC1_REG4 == p_sdsi_reg->reg_no))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (((SDSI_FUNC1_REG1 == p_sdsi_reg->reg_no) ||
         (SDSI_FUNC1_REG2 == p_sdsi_reg->reg_no)) &&
         (SDSI_OFFSET_256 <= p_sdsi_reg->offset))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if ((SDSI_FUNC1_REG5  == p_sdsi_reg->reg_no) &&
        (SDSI_OFFSET_1024 <= p_sdsi_reg->offset))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
/*
    if (SDSI_OFFSET_BOUNDARY & p_sdsi_reg->offset)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
*/
    if (0 == p_sdsi_reg->p_buff)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Set SDSI function 1 register N. */
    r_sdsi_dev_write_func_reg(channel, p_sdsi_reg);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_WriteFuncReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_ReadFuncReg
* Description  : Reads function 1 register N.
* Arguments    : uint32_t           channel     ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg        ;
*              :    uint32_t    reg_no          ;   Register No.
*              :                                ;       SDSI_FUNC1_REG1
*              :                                ;       SDSI_FUNC1_REG3
*              :                                ;       SDSI_FUNC1_REG5
*              :    uint32_t    offset          ;   Offset from the targeted register.
*              :    uint32_t    * p_buff        ;   Register value
* Return Value : SDSI_SUCCESS                   ;   Successful operation
*              : SDSI_ERR                       ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_ReadFuncReg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (0 == p_sdsi_reg->reg_no)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if ((SDSI_FUNC1_REG2 == p_sdsi_reg->reg_no) ||
        (SDSI_FUNC1_REG4 == p_sdsi_reg->reg_no))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (((SDSI_FUNC1_REG1 == p_sdsi_reg->reg_no) ||
         (SDSI_FUNC1_REG3 == p_sdsi_reg->reg_no)) &&
         (SDSI_OFFSET_256 <= p_sdsi_reg->offset))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if ((SDSI_FUNC1_REG5  == p_sdsi_reg->reg_no) &&
        (SDSI_OFFSET_1024 <= p_sdsi_reg->offset))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
/*
    if (SDSI_OFFSET_BOUNDARY & p_sdsi_reg->offset)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
*/
    if (0 == p_sdsi_reg->p_buff)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Get SDSI function 1 register N. */
    r_sdsi_dev_read_func_reg(channel, p_sdsi_reg);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_ReadFuncReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_WriteIntVectorReg
* Description  : Sets SDIO interrupt vector(FN1INTVECR).
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint8_t        vector                  ;   SDIO interrupt vector
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_WriteIntVectorReg(uint32_t channel, uint8_t vector)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)(sizeof(g_sdsi_cfg_ip_base) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Set SDIO interrupt vector. */
    r_sdsi_dev_write_int_vector_reg(channel, vector);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_WriteIntVectorReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_ReadIntVectorReg
* Description  : Gets SDIO interrupt vector(FN1INTVECR).
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint8_t        * p_vector              ;   SDIO interrupt vector buffer
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_ReadIntVectorReg(uint32_t channel, uint8_t * p_vector)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)(sizeof(g_sdsi_cfg_ip_base) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (0 == p_vector)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Get SDIO interrupt vector. */
    r_sdsi_dev_read_int_vector_reg(channel, p_vector);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_ReadIntVectorReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_ReadIntClearReg
* Description  : Gets SDIO interrupt vector clear register(FN1INTCLRR).
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint8_t        * p_vector              ;   SDIO interrupt vector clear buffer
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_ReadIntClearReg(uint32_t channel, uint8_t * p_vector)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)(sizeof(g_sdsi_cfg_ip_base) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (0 == p_vector)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* Get SDIO interrupt vector clear register. */
    r_sdsi_dev_read_int_clear_reg(channel, p_vector);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_ReadIntClearReg() */

/**********************************************************************************************************************
* Function Name: R_SDSI_EnableDirectTrans
* Description  : Sets direct transfer address and enable direct transfer.
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : sdsi_direct_trans_t    * p_sdsi_direct_trans   ;
*              :    uint32_t    adr                     ;   Direct transfer address
*              :    uint32_t    mode                    ;   Mode
*              :       SDSI_MODE_DIRECT_ADDR_FIXED      ;   Direct transfer address is fixed.
*              :       SDSI_MODE_DIRECT_ADDR_INC        ;   Direct transfer address is incremented after each transfer.
*              :       SDSI_MODE_DIRECT_BUS_LOCK        ;   Internal main bus is locked.
*              :       SDSI_MODE_DIRECT_BUS_UNLOCK      ;   Internal main bus is not locked.
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_EnableDirectTrans(uint32_t channel, sdsi_direct_trans_t * p_sdsi_direct_trans)
{
    sdsi_hndl_t * p_hndl = 0;
    
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    sdsi_status_t ret = SDSI_SUCCESS;

    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    /* Check direct transfer address. */
    ret = r_sdsi_dev_chk_direct_trans_adr(channel, p_sdsi_direct_trans->adr);
    if (SDSI_SUCCESS != ret)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return ret;
    }

    /* Check direct transfer mode. */
    if (0 == (p_sdsi_direct_trans->mode & SDSI_MODE_DIRECT_ADDR_MASK))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == (p_sdsi_direct_trans->mode & SDSI_MODE_DIRECT_BUS_MASK))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    p_hndl = SDSI_GET_HNDL(channel);

    /* Enable direct transfer. */
    if (r_sdsi_dev_enable_direct_trans(channel, p_sdsi_direct_trans) != SDSI_SUCCESS)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    /* Set address mode. */
    p_hndl->direct_trans_mode = p_sdsi_direct_trans->mode;

    return SDSI_SUCCESS;
} /* End of function R_SDSI_EnableDirectTrans() */

/**********************************************************************************************************************
* Function Name: R_SDSI_DisableDirectTrans
* Description  : Disables direct transfer.
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_DisableDirectTrans(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;
    
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    p_hndl = SDSI_GET_HNDL(channel);

    /* Disable direct transfer. */
    r_sdsi_dev_disable_direct_trans(channel);

    /* Clear address mode. */
    p_hndl->direct_trans_mode = SDSI_MODE_DIRECT_NONE;

    return SDSI_SUCCESS;
} /* End of function R_SDSI_DisableDirectTrans() */

/**********************************************************************************************************************
* Function Name: R_SDSI_SetDirectTransAdr
* Description  : Sets direct transfer address.
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint32_t       adr                     ;   Direct transfer address
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_SetDirectTransAdr(uint32_t channel, uint32_t adr)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    sdsi_status_t ret = SDSI_SUCCESS;

    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    /* Check direct transfer address. */
    ret = r_sdsi_dev_chk_direct_trans_adr(channel, adr);
    if (SDSI_SUCCESS != ret)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return ret;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    r_sdsi_dev_set_direct_trans_adr(channel, adr);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_SetDirectTransAdr() */

/**********************************************************************************************************************
* Function Name: R_SDSI_GetDirectTransAdr
* Description  : Gets direct transfer address.
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint32_t       * p_adr                 ;   Direct transfer address buffer
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*              : SDSI_ERR                               ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_GetDirectTransAdr(uint32_t channel, uint32_t * p_adr)
{
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }

    if (0 == p_adr)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    r_sdsi_dev_get_direct_trans_adr(channel, p_adr);

    return SDSI_SUCCESS;
} /* End of function R_SDSI_GetDirectTransAdr() */

/**********************************************************************************************************************
* Function Name: R_SDSI_RegistIntCallback
* Description  : Set SDSI CMD interrupt callback function.
* Arguments    : uint32_t       channel                  ;   SDSI Channel No.
*              : sdsi_status_t  (* callback)(* sdsi_cmd_t) ; Callback function
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_RegistIntCallback(uint32_t channel, sdsi_status_t (* callback)(sdsi_cmd_t *))
{
    sdsi_hndl_t * p_hndl = 0;
    
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    p_hndl = SDSI_GET_HNDL(channel);
    p_hndl->int_callback = callback;

    return SDSI_SUCCESS;
} /* End of function R_SDSI_RegistIntCallback() */

/**********************************************************************************************************************
* Function Name: R_SDSI_RegistCdIntCallback
* Description  : Set SDSI Card detection disable (rise/fall) interrupt callback function.
* Arguments    : uint32_t       channel                  ;   SDSI Channel No.
*              : sdsi_status_t  (* callback)(uint32_t)   ;   Callback function
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_RegistCdIntCallback(uint32_t channel, sdsi_status_t (* callback)(uint32_t))
{
    sdsi_hndl_t * p_hndl = 0;
    
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    p_hndl = SDSI_GET_HNDL(channel);
    p_hndl->int_cd_callback = callback;

    return SDSI_SUCCESS;
} /* End of function R_SDSI_RegistCdIntCallback() */

/**********************************************************************************************************************
* Function Name: R_SDSI_RegistDtIntCallback
* Description  : Set SDSI direct transfer end interrupt callback function.
* Arguments    : uint32_t       channel                  ;   SDSI Channel No.
*              : sdsi_status_t  (* callback)(* sdsi_cmd_t) ; Callback function
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_RegistDtIntCallback(uint32_t channel, sdsi_status_t (* callback)(sdsi_cmd_t *))
{
    sdsi_hndl_t * p_hndl = 0;
    
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdsi_cfg_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;
    }

    if (0 == SDSI_GET_HNDL(channel))
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_RX, __LINE__);
        return SDSI_ERR;  /* Not initialized */
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    p_hndl = SDSI_GET_HNDL(channel);
    p_hndl->int_dt_callback = callback;

    return SDSI_SUCCESS;
} /* End of function R_SDSI_RegistDtIntCallback() */

/**********************************************************************************************************************
* Function Name: R_SDSI_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t R_SDSI_GetVersion(void)
{
    uint32_t const version = ((RX_SDSI_VERSION_MAJOR << SDSI_BIT_SHIFT_16) | RX_SDSI_VERSION_MINOR);

    return version;
} /* End of function R_SDSI_GetVersion() */

/**********************************************************************************************************************
* Function Name: r_sdsi_init_hndl
* Description  : Initializes the SDSI handle.
* Arguments    : uint32_t       channel                  ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static sdsi_status_t r_sdsi_init_hndl(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    p_hndl->intsr1 = 0;
    p_hndl->intsr2 = 0;
    p_hndl->direct_trans_mode = 0;

    return SDSI_SUCCESS;
} /* End of function r_sdsi_init_hndl() */

#ifdef SDSI_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
/**********************************************************************************************************************
* Function Name: R_SDSI_SetLogHdlAddress
* Description  : Gets the Log HNDL Address.
* Arguments    : uint32_t           user_byte_que        ;   user_byte_que
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_SetLogHdlAddress(uint32_t user_long_que)
{
    p_sdsi_long_que = (longq_hdl_t)user_long_que;

    return SDSI_SUCCESS;
} /* End of function R_SDSI_SetLogHdlAddress() */


/**********************************************************************************************************************
* Function Name: r_sdsi_log
* Description  : Stores the debugging information.
* Arguments    : uint32_t           flg                  ;   Breakpoint processing
*              : uint32_t           fid                  ;   SDSI driver file No.
*              : uint32_t           line                 ;   SDSI driver line No.
* Return Value : 0      ; Successful
*              : 1      ; Error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdsi_log(uint32_t flg, uint32_t fid, uint32_t line)
{
    longq_err_t err = LONGQ_SUCCESS;
    uint32_t    ul_tmp = 0;

    /* Long to Byte */
    ul_tmp = 0;                                                        /* sdsi_log   Reserved               */
    ul_tmp = (ul_tmp | (line << SDSI_BIT_SHIFT_8));                    /* sdsi_log   LINE                   */
    ul_tmp = (ul_tmp | (fid  << SDSI_BIT_SHIFT_21));                   /* sdsi_log   FID FILE No.           */
    ul_tmp = (ul_tmp | (SDSI_DRIVER_ID << SDSI_BIT_SHIFT_27));         /* sdsi_log   FID DRIVER No.         */
    ul_tmp = (ul_tmp | (flg  << SDSI_BIT_SHIFT_31));                   /* sdsi_log   Breakpoint processing  */

    /* error log white */
    err = R_LONGQ_Put(p_sdsi_long_que, ul_tmp);

    if (err != LONGQ_SUCCESS)
    {
        return 1;
    }

    /* Breakpoint processing */
    if (0x80000000 == (ul_tmp & 0x80000000))
    {
        return 1;
    }

    return 0;
} /* End of function r_sdsi_log() */
#else
/**********************************************************************************************************************
* Function Name: R_SDSI_SetLogHdlAddress
* Description  : Gets the Log HNDL Address.
* Arguments    : uint32_t           user_byte_que        ;   user_byte_que
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t R_SDSI_SetLogHdlAddress(uint32_t user_long_que)
{
    return SDSI_SUCCESS;
} /* End of function R_SDSI_SetLogHdlAddress() */
#endif /* SDSI_CFG_LONGQ_ENABLE */


/**********************************************************************************************************************
* Function Name: R_SDSI_Log
* Description  : Stores the debugging information.
* Arguments    : uint32_t           flg                  ;   Breakpoint processing
*              : uint32_t           fid                  ;   SDSI driver file No.
*              : uint32_t           line                 ;   SDSI driver line No.
* Return Value : 0
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t R_SDSI_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
    SDSI_LOG_FUNC(flg, fid, line);
    return 0;
} /* End of function R_SDSI_Log() */


/* End of File */
