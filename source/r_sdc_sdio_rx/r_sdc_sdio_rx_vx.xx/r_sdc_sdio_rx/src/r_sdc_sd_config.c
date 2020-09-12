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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdc_sd_config.c
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
static sdc_sd_status_t r_sdc_sd_callback0(uint32_t sdsts1, uint32_t sdsts2);
static sdc_sd_status_t r_sdc_sd_dtc_callback0(void *);
static sdc_sd_status_t r_sdc_sdio_callback0(uint32_t sdiosts);
static sdc_sd_status_t r_sdc_sd_callback1(uint32_t sdsts1, uint32_t sdsts2);
static sdc_sd_status_t r_sdc_sd_dtc_callback1(void *);
static sdc_sd_status_t r_sdc_sdio_callback1(uint32_t sdiosts);

sdc_sd_hndl_t * g_sdc_sd_handle[SDC_SD_CFG_CARD_NUM];
uint32_t g_sdc_sd_int_dmacdtcflg[SDC_SD_CFG_CARD_NUM];

/**********************************************************************************************************************
* Outline      : Wait SD Card Interrupt
* Function Name: r_sdc_sd_int_wait
* Description  : Starts timer and waits for an interrupt generation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            time                ;   Timeout (msec).
* Return Value : SDC_SD_SUCCESS                         ;   Interrupt detection
*              : SDC_SD_ERR                             ;   Timeout
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_int_wait(uint32_t card_no, uint32_t time)
{
    sdc_sd_status_t    ret = SDC_SD_SUCCESS;

    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    while (1)
    {
        /* Get an interrupt generation. */
        if (r_sdc_sd_get_intstatus(card_no) == SDC_SD_SUCCESS)
        {
            ret = SDC_SD_SUCCESS;
            break;
        }

        /* Timeout? */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            ret = SDC_SD_ERR;
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return ret;
} /* End of function r_sdc_sd_int_wait() */

/**********************************************************************************************************************
* Outline      : Wait SD Memory Card Interrupt
* Function Name: r_sdc_sd_int_mem_wait
* Description  : Starts timer and waits for an interrupt generation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            time                ;   Timeout (msec).
* Return Value : SDC_SD_SUCCESS                         ;   Interrupt detection
*              : SDC_SD_ERR                             ;   Timeout
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_int_mem_wait(uint32_t card_no, uint32_t time)
{
    sdc_sd_status_t    ret = SDC_SD_SUCCESS;

    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    while (1)
    {
        /* Get an interrupt generation. */
        if (r_sdc_sd_get_intstatus(card_no) == SDC_SD_SUCCESS)
        {
            ret = SDC_SD_SUCCESS;
            break;
        }

        /* Timeout? */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            ret = SDC_SD_ERR;
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return ret;
} /* End of function r_sdc_sd_int_mem_wait() */

/**********************************************************************************************************************
* Outline      : Wait SDIO Card Interrupt
* Function Name: r_sdc_sd_int_io_wait
* Description  : Starts timer and waits for an interrupt generation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            time                ;   Timeout (msec).
*              : uint32_t           func                ;   Function No
* Return Value : SDC_SD_SUCCESS                         ;   Interrupt detection
*              : SDC_SD_ERR                             ;   Timeout
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_int_io_wait(uint32_t card_no, uint32_t time, uint32_t func)
{
    sdc_sd_status_t    ret = SDC_SD_SUCCESS;

    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    while (1)
    {
        /* Get an interrupt generation. */
        if (r_sdc_sd_get_intstatus(card_no) == SDC_SD_SUCCESS)
        {
            ret = SDC_SD_SUCCESS;
            break;
        }

        /* Timeout? */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            ret = SDC_SD_ERR;
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return ret;
} /* End of function r_sdc_sd_int_io_wait() */

/**********************************************************************************************************************
* Outline      : Wait SD Memory Card Interrupt
* Function Name: r_sdc_sd_int_err_mem_wait
* Description  : Starts timer and waits for an interrupt generation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            time                ;   Timeout (msec).
* Return Value : SDC_SD_SUCCESS                         ;   Interrupt detection
*              : SDC_SD_ERR                             ;   Timeout
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_int_err_mem_wait(uint32_t card_no, uint32_t time)
{
    sdc_sd_status_t    ret = SDC_SD_SUCCESS;

    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    while (1)
    {
        /* Get an interrupt generation. */
        if (r_sdc_sd_get_intstatus(card_no) == SDC_SD_SUCCESS)
        {
            ret = SDC_SD_SUCCESS;
            break;
        }

        /* Timeout? */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            ret = SDC_SD_ERR;
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return ret;
} /* End of function r_sdc_sd_int_err_mem_wait() */

/**********************************************************************************************************************
* Outline      : Wait SDIO Card Interrupt
* Function Name: r_sdc_sd_int_err_io_wait
* Description  : Starts timer and waits for an interrupt generation.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            time                ;   Timeout (msec).
*              : uint32_t           func                ;   Function No
* Return Value : SDC_SD_SUCCESS                         ;   Interrupt detection
*              : SDC_SD_ERR                             ;   Timeout
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_int_err_io_wait(uint32_t card_no, uint32_t time, uint32_t func)
{
    sdc_sd_status_t    ret = SDC_SD_SUCCESS;

    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    while (1)
    {
        /* Get an interrupt generation. */
        if (r_sdc_sd_get_intstatus(card_no) == SDC_SD_SUCCESS)
        {
            ret = SDC_SD_SUCCESS;
            break;
        }

        /* Timeout? */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            ret = SDC_SD_ERR;
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return ret;
} /* End of function r_sdc_sd_int_err_io_wait() */

/**********************************************************************************************************************
* Outline      : Wait
* Function Name: r_sdc_sd_wait
* Description  : Starts timer and waits.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : int32_t            time                ;   Timeout (msec).
* Return Value : SDC_SD_SUCCESS                         ;   Timeout
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_wait(uint32_t card_no, uint32_t time)
{
    if (r_sdc_sd_start_timer(card_no, time) == SDC_SD_ERR)
    {
        return SDC_SD_ERR;
    }

    while (1)
    {
        /* Timeout? */
        if (r_sdc_sd_check_timer(card_no) == SDC_SD_ERR)
        {
            break;
        }
    }

    r_sdc_sd_end_timer(card_no);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_wait() */

/**********************************************************************************************************************
* Outline      : Register SDHI FIT Module Callback Function
* Function Name: r_sdc_sd_set_int_callback
* Description  : Sets SDHI FIT module callback function.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           enable              ;   SDC_SD_CALLBACK_DISABLE
*              :                                        ;   SDC_SD_CALLBACK_ENABLE
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : Register the callback function for device number 0.
*              : User must create callback function registration process for device number 1 or later.
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_set_int_callback(uint32_t card_no, uint32_t enable)
{
    sdc_sd_hndl_t   * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_CALLBACK_DISABLE == enable)
    {
        if (R_SDHI_IntCallback(p_hndl->channel, 0) != SDHI_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
            R_SDHI_IntCallback(p_hndl->channel, 0);
            return SDC_SD_ERR;
        }

        if (R_SDHI_IntSDBuffCallback(p_hndl->channel, 0) != SDHI_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
            R_SDHI_IntCallback(p_hndl->channel, 0);
            R_SDHI_IntSDBuffCallback(p_hndl->channel, 0);
            return SDC_SD_ERR;
        }

        if (R_SDHI_IntSdioCallback(p_hndl->channel, 0) != SDHI_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
            R_SDHI_IntCallback(p_hndl->channel, 0);
            R_SDHI_IntSDBuffCallback(p_hndl->channel, 0);
            R_SDHI_IntSdioCallback(p_hndl->channel, 0);
            return SDC_SD_ERR;
        }
    }
    else
    {
        if (SDC_SD_CARD_NO0 == card_no)
        {
            if (R_SDHI_IntCallback(p_hndl->channel, r_sdc_sd_callback0) != SDHI_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
                R_SDHI_IntCallback(p_hndl->channel, 0);
                return SDC_SD_ERR;
            }

            if (R_SDHI_IntSDBuffCallback(p_hndl->channel, r_sdc_sd_dtc_callback0) != SDHI_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
                R_SDHI_IntCallback(p_hndl->channel, 0);
                R_SDHI_IntSDBuffCallback(p_hndl->channel, 0);
                return SDC_SD_ERR;
            }

            if (R_SDHI_IntSdioCallback(p_hndl->channel, r_sdc_sdio_callback0) != SDHI_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
                R_SDHI_IntCallback(p_hndl->channel, 0);
                R_SDHI_IntSDBuffCallback(p_hndl->channel, 0);
                R_SDHI_IntSdioCallback(p_hndl->channel, 0);
                return SDC_SD_ERR;
            }
        }
        else if (SDC_SD_CARD_NO1 == card_no)
        {
            if (R_SDHI_IntCallback(p_hndl->channel, r_sdc_sd_callback1) != SDHI_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
                R_SDHI_IntCallback(p_hndl->channel, 0);
                return SDC_SD_ERR;
            }

            if (R_SDHI_IntSDBuffCallback(p_hndl->channel, r_sdc_sd_dtc_callback1) != SDHI_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
                R_SDHI_IntCallback(p_hndl->channel, 0);
                R_SDHI_IntSDBuffCallback(p_hndl->channel, 0);
                return SDC_SD_ERR;
            }

            if (R_SDHI_IntSdioCallback(p_hndl->channel, r_sdc_sdio_callback1) != SDHI_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_CONFIG, __LINE__);
                R_SDHI_IntCallback(p_hndl->channel, 0);
                R_SDHI_IntSDBuffCallback(p_hndl->channel, 0);
                R_SDHI_IntSdioCallback(p_hndl->channel, 0);
                return SDC_SD_ERR;
            }
        }
        else
        {
            /* User must create callback function registration process for device number 1 or later. */
        }
    }

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sd_software_trans() */

/**********************************************************************************************************************
* Outline      : SDC Callback Processing
* Function Name: r_sdc_sd_callback0
* Description  : Executes the callback function of card access interrupt.
* Arguments    : uint32_t           sdsts1              ;   SDSTS1 register value
*              : uint32_t           sdsts2              ;   SDSTS2 register value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : For device no 0.
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sd_callback0(uint32_t sdsts1, uint32_t sdsts2)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t           cd = 0;

    p_hndl = SDC_SD_GET_HNDL(SDC_SD_CARD_NO0);  /* Card No. 0 */

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_get_int(SDC_SD_CARD_NO0, sdsts1, sdsts2) == SDC_SD_SUCCESS)    /* Card No. 0 */
    {
        /* Check card detect interrupt. */
        if (p_hndl->sdsts1 & (SDHI_SDIMSK1_DET_DAT3 | SDHI_SDIMSK1_DET_CD))
        {
            if (p_hndl->sdc_sd_int_cd_callback)
            {
                if (p_hndl->sdsts1 & (SDHI_SDIMSK1_INS_DAT3 | SDHI_SDIMSK1_INS_CD))
                {
                    cd = SDC_SD_CD_INSERT; /* Insertion */
                }
                else
                {
                    cd = SDC_SD_CD_REMOVE; /* Removal */
                }
                (*p_hndl->sdc_sd_int_cd_callback)(cd);
            }
            p_hndl->sdsts1 = (p_hndl->sdsts1 & 
                             (uint32_t)~(SDHI_SDIMSK1_DET_DAT3 | SDHI_SDIMSK1_DET_CD));
        }
        if (p_hndl->sdsts1 || p_hndl->sdsts2)
        {
            if (p_hndl->sdc_sd_int_callback)
            {
                (*p_hndl->sdc_sd_int_callback)(0);
            }
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_callback0() */

/**********************************************************************************************************************
* Outline      : SDC SD Buffer Access Callback Processing
* Function Name: r_sdc_sd_dtc_callback0
* Description  : Executes the callback function of SD buffer access interrupt.
* Arguments    : uint32_t           sdsts1              ;   SDSTS1 register value
*              : uint32_t           sdsts2              ;   SDSTS2 register value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : For device no 0.
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sd_dtc_callback0(void * vect)
{
    R_SDC_SD_SetDmacDtcTransFlg(SDC_SD_CARD_NO0, SDC_SD_SET_TRANS_STOP);  /* Card No. 0 */

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_dtc_callback0() */

/**********************************************************************************************************************
* Outline      : SDHI SDIO Callback Processing
* Function Name: r_sdc_sdio_callback0
* Description  : Executes the callback function of SDIO interrupt.
* Arguments    : uint32_t           sdiosts             ;   SDIOSTS register value.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : For device no 0.
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdio_callback0(uint32_t sdiosts)
{
    sdc_sd_hndl_t   * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(SDC_SD_CARD_NO0);  /* Card No. 0 */

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    if (r_sdc_sdio_get_int(SDC_SD_CARD_NO0, sdiosts) == SDC_SD_SUCCESS)     /* Card No. 0 */
    {
        if (p_hndl->sdc_sd_int_io_callback)
        {
            (*p_hndl->sdc_sd_int_io_callback)(0);
        }
    }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdio_callback0() */

/**********************************************************************************************************************
* Outline      : SDC Callback Processing
* Function Name: r_sdc_sd_callback1
* Description  : Executes the callback function of card access interrupt.
* Arguments    : uint32_t           sdsts1              ;   SDSTS1 register value
*              : uint32_t           sdsts2              ;   SDSTS2 register value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : For device no 1.
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sd_callback1(uint32_t sdsts1, uint32_t sdsts2)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t           cd = 0;

    p_hndl = SDC_SD_GET_HNDL(SDC_SD_CARD_NO1);  /* Card No. 1 */

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (r_sdc_sd_get_int(SDC_SD_CARD_NO1, sdsts1, sdsts2) == SDC_SD_SUCCESS)    /* Card No. 1 */
    {
        /* Check card detect interrupt. */
        if (p_hndl->sdsts1 & (SDHI_SDIMSK1_DET_DAT3 | SDHI_SDIMSK1_DET_CD))
        {
            if (p_hndl->sdc_sd_int_cd_callback)
            {
                if (p_hndl->sdsts1 & (SDHI_SDIMSK1_INS_DAT3 | SDHI_SDIMSK1_INS_CD))
                {
                    cd = SDC_SD_CD_INSERT; /* Insertion */
                }
                else
                {
                    cd = SDC_SD_CD_REMOVE; /* Removal */
                }
                (*p_hndl->sdc_sd_int_cd_callback)(cd);
            }
            p_hndl->sdsts1 = (p_hndl->sdsts1 & 
                             (uint32_t)~(SDHI_SDIMSK1_DET_DAT3 | SDHI_SDIMSK1_DET_CD));
        }
        if (p_hndl->sdsts1 || p_hndl->sdsts2)
        {
            if (p_hndl->sdc_sd_int_callback)
            {
                (*p_hndl->sdc_sd_int_callback)(0);
            }
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_callback1() */

/**********************************************************************************************************************
* Outline      : SDC SD Buffer Access Callback Processing
* Function Name: r_sdc_sd_dtc_callback1
* Description  : Executes the callback function of SD buffer access interrupt.
* Arguments    : uint32_t           sdsts1              ;   SDSTS1 register value
*              : uint32_t           sdsts2              ;   SDSTS2 register value
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : For device no 1.
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sd_dtc_callback1(void * vect)
{
    R_SDC_SD_SetDmacDtcTransFlg(SDC_SD_CARD_NO1, SDC_SD_SET_TRANS_STOP);  /* Card No. 1 */

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_dtc_callback1() */

/**********************************************************************************************************************
* Outline      : SDHI SDIO Callback Processing
* Function Name: r_sdc_sdio_callback1
* Description  : Executes the callback function of SDIO interrupt.
* Arguments    : uint32_t           sdiosts             ;   SDIOSTS register value.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : For device no 1.
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdio_callback1(uint32_t sdiosts)
{
    sdc_sd_hndl_t   * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(SDC_SD_CARD_NO1);  /* Card No. 1 */

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return SDC_SD_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    if (r_sdc_sdio_get_int(SDC_SD_CARD_NO1, sdiosts) == SDC_SD_SUCCESS)     /* Card No. 1 */
    {
        if (p_hndl->sdc_sd_int_io_callback)
        {
            (*p_hndl->sdc_sd_int_io_callback)(0);
        }
    }
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdio_callback1() */

/**********************************************************************************************************************
* Outline      : Disable ICU
* Function Name: r_sdc_sd_disable_icu_int
* Description  : Disables the Interrupt Request Enable (IENn).
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_disable_icu_int(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_HWINT)
    if (R_SDHI_DisableIcuInt(p_hndl->channel, SDHI_HWINT_ACCESS_CD | SDHI_HWINT_BUFFER) != SDHI_SUCCESS)
#else
    if (R_SDHI_DisableIcuInt(p_hndl->channel, SDHI_HWINT_BUFFER) != SDHI_SUCCESS)
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_HWINT */
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }
    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_disable_icu_int() */

/**********************************************************************************************************************
* Outline      : Enable ICU
* Function Name: r_sdc_sd_enable_icu_int
* Description  : Enables the Interrupt Request Enable (IENn).
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sd_enable_icu_int(uint32_t card_no)
{
    sdc_sd_hndl_t * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_HWINT)
    if (R_SDHI_EnableIcuInt(p_hndl->channel, SDHI_HWINT_ACCESS_CD | SDHI_HWINT_BUFFER) != SDHI_SUCCESS)
#else
    if (R_SDHI_EnableIcuInt(p_hndl->channel, SDHI_HWINT_BUFFER) != SDHI_SUCCESS)
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_HWINT */
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SD_INT, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }
    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sd_enable_icu_int() */

/**********************************************************************************************************************
* Outline      : Get initialization speed division of SDHI FIT module
* Function Name: r_sdc_sd_get_div_init_speed
* Description  : Gets initialization speed division of SDHI FIT module.
* Arguments    : None
* Return Value : Initialization speed division
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_get_div_init_speed(void)
{
    return SDHI_CFG_DIV_INIT_SPEED;
} /* End of function r_sdc_sd_get_div_init_speed() */

/**********************************************************************************************************************
* Outline      : Get default speed division of SDHI FIT module
* Function Name: r_sdc_sd_get_div_default_speed
* Description  : Gets default speed division of SDHI FIT module.
* Arguments    : None
* Return Value : Default speed division
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_get_div_default_speed(void)
{
    return SDHI_CFG_DIV_DEFAULT_SPEED;
} /* End of function r_sdc_sd_get_div_default_speed() */

/**********************************************************************************************************************
* Outline      : Get high speed division of SDHI FIT module
* Function Name: r_sdc_sd_get_div_high_speed
* Description  : Gets high speed division of SDHI FIT module.
* Arguments    : None
* Return Value : High speed division
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sd_get_div_high_speed(void)
{
    return SDHI_CFG_DIV_HIGH_SPEED;
} /* End of function r_sdc_sd_get_div_high_speed() */

/* End of File */
