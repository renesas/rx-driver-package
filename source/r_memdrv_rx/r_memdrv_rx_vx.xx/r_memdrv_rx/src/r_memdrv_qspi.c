/************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : MEMDRV software
* File Name    : r_memdrv_qspi.c
* Version      : 1.02
* Device       : -
* Abstract     : IO I/F module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : MEMDRV I/O file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.12.2018 1.00     Initial Release
*              : 04.04.2019 1.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*              : 22.11.2019 1.02     Modified check driver interface.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */

/* Check driver interface. */
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR))

/************************************************************************************************
Macro definitions
*************************************************************************************************/

/************************************************************************************************
Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
static uint8_t qspi_baud_cal(uint8_t devno);

static qspi_smstr_tranmode_t r_memdrv_qspi_set_tran_mode(uint8_t devno);
static memdrv_err_t r_memdrv_qspi_disable_tx_data_dmac(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_disable_rx_data_dmac(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_enable_tx_data_dmac(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_enable_rx_data_dmac(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_disable_tx_data_dtc(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_disable_rx_data_dtc(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_enable_tx_data_dtc(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_qspi_enable_rx_data_dtc(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);

/************************************************************************************************
* Function Name: r_memdrv_qspi_open
* Description  : Initializes I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    uint8_t bps_result = qspi_baud_cal(devno);
    
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    if (MEMDRV_SUCCESS != r_memdrv_dmac_open())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    if (MEMDRV_SUCCESS != r_memdrv_dtc_open())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif
    
    if (QSPI_SMSTR_SUCCESS != R_QSPI_SMstr_Open(channel, bps_result))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_open() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_close
* Description  : Resets I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    if (MEMDRV_SUCCESS != r_memdrv_dmac_close())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    if (MEMDRV_SUCCESS != r_memdrv_dtc_close())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif
    
    if (QSPI_SMSTR_SUCCESS != R_QSPI_SMstr_Close(channel))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_close() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable
* Description  : Disables I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable_tx_data
* Description  : Disables I/O driver for writing data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_disable_tx_data(uint8_t devno, 
                                           st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else if (MEMDRV_CHK_MULT_OF_4 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 4, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else
    {
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable_tx_data_dmac
* Description  : Disables I/O driver for writing data and DMAC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_disable_tx_data_dmac(uint8_t devno,
                                                       st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_stat_t stat_dmac;

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dmac_info.p_stat_dmac   = &stat_dmac;

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx;
    }

    if (r_memdrv_dmac_disable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable_tx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable_tx_data_dtc
* Description  : Disables I/O driver for writing data and DTC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_disable_tx_data_dtc(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dtc_info_t dtc_info;

    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPTI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPTI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    if (r_memdrv_dtc_disable(&dtc_info) != MEMDRV_SUCCESS)
    {
        return MEMDRV_ERR_OTHER;
    }
#endif  /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable_tx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable_rx_data
* Description  : Disables I/O driver for reading data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else if (MEMDRV_CHK_MULT_OF_4 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 4, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else
    {
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable_rx_data_dmac
* Description  : Disables I/O driver for reading data and DMAC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_disable_rx_data_dmac(uint8_t devno,
                                                       st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC) | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_stat_t stat_dmac;

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dmac_info.p_stat_dmac   = &stat_dmac;

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx;
    }

    if (r_memdrv_dmac_disable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx;
    }

    if (r_memdrv_dmac_disable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC) | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable_rx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_disable_rx_data_dtc
* Description  : Disables I/O driver for reading data and DTC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_disable_rx_data_dtc(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dtc_info_t dtc_info;

    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPTI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPTI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    if (r_memdrv_dtc_disable(&dtc_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPRI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPRI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    if (r_memdrv_dtc_disable(&dtc_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif  /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_disable_rx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable
* Description  : Enables I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    uint8_t bps_result = qspi_baud_cal(devno);
    
    if (R_QSPI_SMstr_Control(channel,
                             3,
                             bps_result) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable_tx_data
* Description  : Enables I/O driver for writing data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    uint8_t bps_result = qspi_baud_cal(devno);
    
    if (R_QSPI_SMstr_Control(channel,
                             3,
                             bps_result) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else if (MEMDRV_CHK_MULT_OF_16 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 4, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else
    {
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable_tx_data_dmac
* Description  : Enables I/O driver for writing data and DMAC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_enable_tx_data_dmac(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_transfer_data_cfg_t dmac_cfg;
    uint32_t reg_adr = 0;
    dmaca_stat_t stat_dmac;

    if (R_QSPI_SMstr_Get_BuffRegAddress(channel, &reg_adr) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dmac_info.src_adr       = (uint32_t)p_memdrv_info->p_data;
    dmac_info.dst_adr       = reg_adr;
    dmac_info.block_size    = 4;
    dmac_info.cnt           = p_memdrv_info->cnt;
    dmac_info.p_dmac_cfg    = &dmac_cfg;
    dmac_info.p_stat_dmac   = &stat_dmac;

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx;
    }
    
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dmac_info.act_source = IR_QSPI_SPTI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_QSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_QSPI2_SPTI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dmac_set_tx(&dmac_info);

    if (r_memdrv_dmac_enable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    
    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dmac_info.src_adr       = reg_adr;
    dmac_info.dst_adr       = (uint32_t)&g_memdrv_dummy_rx;
    dmac_info.block_size    = 4;
    dmac_info.cnt           = p_memdrv_info->cnt;
    dmac_info.p_dmac_cfg    = &dmac_cfg;
    dmac_info.p_stat_dmac   = &stat_dmac;

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx;
    }
    
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dmac_info.act_source = IR_QSPI_SPRI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_QSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_QSPI2_SPRI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dmac_set_rx(&dmac_info);
    dmac_info.p_dmac_cfg->des_addr_mode = DMACA_DES_ADDR_FIXED;     // Dummy 

    if (r_memdrv_dmac_enable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable_tx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable_tx_data_dtc
* Description  : Enables I/O driver for writing data and DTC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_enable_tx_data_dtc(uint8_t devno,
                                                     st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dtc_info_t dtc_info;
    dtc_transfer_data_cfg_t dtc_cfg;
    uint32_t reg_adr = 0;

    if (R_QSPI_SMstr_Get_BuffRegAddress(channel, &reg_adr) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dtc_info.src_adr        = (uint32_t)p_memdrv_info->p_data;
    dtc_info.dst_adr        = reg_adr;
    dtc_info.block_size     = 4;
    dtc_info.cnt            = p_memdrv_info->cnt;
    dtc_info.p_dtc_cfg      = &dtc_cfg;
    dtc_info.p_dtc_transfer = &g_memdrv_dtc_info[devno][MEMDRV_INDEX_DTC_TX];
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPTI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPTI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dtc_set_tx(&dtc_info);

    if (r_memdrv_dtc_enable(&dtc_info) != MEMDRV_SUCCESS)
    {
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dtc_info.src_adr        = reg_adr;
    dtc_info.dst_adr        = (uint32_t)&g_memdrv_dummy_rx;
    dtc_info.block_size     = 0;
    dtc_info.cnt            = p_memdrv_info->cnt;
    dtc_info.p_dtc_cfg      = &dtc_cfg;
    dtc_info.p_dtc_transfer = &g_memdrv_dtc_info[devno][MEMDRV_INDEX_DTC_RX];
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPRI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPRI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dtc_set_rx(&dtc_info);
    dtc_info.p_dtc_cfg->dest_addr = DTC_DES_ADDR_FIXED;     // Dummy 

    if (r_memdrv_dtc_enable(&dtc_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif  /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable_tx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable_rx_data
* Description  : Enables I/O driver for reading data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_enable_rx_data(uint8_t devno,
                                                               st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    uint8_t bps_result = qspi_baud_cal(devno);
    
    if (R_QSPI_SMstr_Control(channel,
                             3,
                             bps_result) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else if (MEMDRV_CHK_MULT_OF_16 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 4, force to perform the CPU transfer. */
        R_BSP_NOP();
    }
    else
    {
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_rx_data_dmac(devno,
                                                                    p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_rx_data_dtc(devno,
                                                                   p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_rx_data_dmac(devno,
                                                                    p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_rx_data_dtc(devno,
                                                                   p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable_rx_data_dmac
* Description  : Enables I/O driver for reading data and DMAC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_enable_rx_data_dmac(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_transfer_data_cfg_t dmac_cfg;
    uint32_t reg_adr = 0;
    dmaca_stat_t stat_dmac;

    if (R_QSPI_SMstr_Get_BuffRegAddress(channel, &reg_adr) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dmac_info.src_adr       = (uint32_t)&g_memdrv_dummy_tx;
    dmac_info.dst_adr       = reg_adr;
    dmac_info.block_size    = 4;
    dmac_info.cnt           = p_memdrv_info->cnt;
    dmac_info.p_dmac_cfg    = &dmac_cfg;
    dmac_info.p_stat_dmac   = &stat_dmac;

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx;
    }
    
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dmac_info.act_source = IR_QSPI_SPTI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_QSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_QSPI2_SPTI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dmac_set_tx(&dmac_info);
    dmac_info.p_dmac_cfg->src_addr_mode = DMACA_SRC_ADDR_FIXED;     // Dummy 

    if (r_memdrv_dmac_enable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dmac_info.src_adr       = reg_adr;
    dmac_info.dst_adr       = (uint32_t)p_memdrv_info->p_data;
    dmac_info.block_size    = 4;
    dmac_info.cnt           = p_memdrv_info->cnt;
    dmac_info.p_dmac_cfg    = &dmac_cfg;
    dmac_info.p_stat_dmac   = &stat_dmac;

    if (MEMDRV_DEV0 == devno)
    {
        dmac_info.channel   = MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx;
    }
    else
    {
        dmac_info.channel   = MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx;
    }
    
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dmac_info.act_source = IR_QSPI_SPRI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_QSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_QSPI2_SPRI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dmac_set_rx(&dmac_info);

    if (r_memdrv_dmac_enable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable_rx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_enable_rx_data_dtc
* Description  : Enables I/O driver for reading data and DTC.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static memdrv_err_t r_memdrv_qspi_enable_rx_data_dtc(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dtc_info_t dtc_info;
    dtc_transfer_data_cfg_t dtc_cfg;
    uint32_t reg_adr = 0;

    if (R_QSPI_SMstr_Get_BuffRegAddress(channel, &reg_adr) != QSPI_SMSTR_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dtc_info.src_adr        = (uint32_t)&g_memdrv_dummy_tx;
    dtc_info.dst_adr        = reg_adr;
    dtc_info.block_size     = 0;
    dtc_info.cnt            = p_memdrv_info->cnt;
    dtc_info.p_dtc_cfg      = &dtc_cfg;
    dtc_info.p_dtc_transfer = &g_memdrv_dtc_info[devno][MEMDRV_INDEX_DTC_TX];
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPTI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPTI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dtc_set_tx(&dtc_info);
    dtc_info.p_dtc_cfg->src_addr_mode = DTC_SRC_ADDR_FIXED;     // Dummy 

    if (r_memdrv_dtc_enable(&dtc_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_QSPI_SMSTR;
    dtc_info.src_adr        = reg_adr;
    dtc_info.dst_adr        = (uint32_t)p_memdrv_info->p_data;
    dtc_info.block_size     = 0;
    dtc_info.cnt            = p_memdrv_info->cnt;
    dtc_info.p_dtc_cfg      = &dtc_cfg;
    dtc_info.p_dtc_transfer = &g_memdrv_dtc_info[devno][MEMDRV_INDEX_DTC_RX];
    switch (channel)
    {
        case 0:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
            dtc_info.act_source = DTCE_QSPI_SPRI;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_QSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_QSPI2_SPRI2;
    #endif
        }
        break;
        default:
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
        break;
    }

    r_memdrv_dtc_set_rx(&dtc_info);

    if (r_memdrv_dtc_enable(&dtc_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif  /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspi_enable_rx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_tx
* Description  : Transmits data for command and status register.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_HARD                      ;   Hardware error
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspi_smstr_info_t     tx_info;
    qspi_smstr_status_t   ret_drv = QSPI_SMSTR_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    tx_info.data_cnt  = p_memdrv_info->cnt;
    tx_info.p_tx_data = p_memdrv_info->p_data;
    tx_info.tran_mode = QSPI_SMSTR_SW;
    tx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_WRITE;

    ret_drv = R_QSPI_SMstr_Write(channel, &tx_info);
    if   (QSPI_SMSTR_SUCCESS == ret_drv)
    {
        return MEMDRV_SUCCESS;
    }
    else if (QSPI_SMSTR_ERR_HARD == ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_HARD;
    }
    else
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
} /* End of function r_memdrv_qspi_tx() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_tx_data
* Description  : Transmits data using the single mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_HARD                      ;   Hardware error
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
memdrv_err_t r_memdrv_qspi_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspi_smstr_info_t     tx_info;
    qspi_smstr_status_t   ret_drv = QSPI_SMSTR_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    tx_info.data_cnt  = p_memdrv_info->cnt;
    tx_info.p_tx_data = p_memdrv_info->p_data;

    /* Set transfer mode. */
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        tx_info.tran_mode = QSPI_SMSTR_SW;
    }
    else if (MEMDRV_CHK_MULT_OF_16 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 16, force to perform the CPU transfer. */
        tx_info.tran_mode = QSPI_SMSTR_SW;
    }
    else
    {
        tx_info.tran_mode = r_memdrv_qspi_set_tran_mode(devno);
    }

    switch (p_memdrv_info->io_mode)
    {
        case MEMDRV_MODE_SINGLE:
        {
            tx_info.op_mode = QSPI_SMSTR_SINGLE_SPI_WRITE;
        }
        break;
        case MEMDRV_MODE_DUAL:
        {
            tx_info.op_mode = QSPI_SMSTR_DUAL_SPI;
        }
        break;
        case MEMDRV_MODE_QUAD:
        {
            tx_info.op_mode = QSPI_SMSTR_QUAD_SPI;
        }
        break;
        default:
        break;
    }

    ret_drv = R_QSPI_SMstr_Write(channel, &tx_info);
    if   (QSPI_SMSTR_SUCCESS == ret_drv)
    {
        return MEMDRV_SUCCESS;
    }
    else if (QSPI_SMSTR_ERR_HARD == ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_HARD;
    }
    else
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
} /* End of function r_memdrv_qspi_tx_data() */
#elif (MEMDRV_CFG_DEV0_TYPE == 0)  | (MEMDRV_CFG_DEV1_TYPE == 0)
memdrv_err_t r_memdrv_qspi_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspi_smstr_info_t           tx_info;
    qspi_smstr_status_t         ret_drv      = QSPI_SMSTR_SUCCESS;
    uint32_t                    boundary_cnt = 0;
    uint32_t                    txcnt = p_memdrv_info->cnt;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    tx_info.p_tx_data = p_memdrv_info->p_data;

    /* Check the 4 bytes address boundary.
       When the address is not 4 bytes address boundary,
       transfer the data of 1-3 bytes using software transfer. */
    if (0 != ((uint32_t)tx_info.p_tx_data & 0x00000003))
    {
        boundary_cnt      = (0x00000004 - ((uint32_t)tx_info.p_tx_data & 0x00000003));
        tx_info.data_cnt  = boundary_cnt;
        tx_info.tran_mode = QSPI_SMSTR_SW;
        if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
        {
            tx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_WRITE;
        }
        else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
        {
            tx_info.op_mode   = QSPI_SMSTR_DUAL_SPI;
        }
        else
        {
            tx_info.op_mode   = QSPI_SMSTR_QUAD_SPI;
        }

        ret_drv = R_QSPI_SMstr_Write(channel, &tx_info);
        if (QSPI_SMSTR_ERR_HARD == ret_drv)
        {
            return MEMDRV_ERR_HARD;
        }
        else if (QSPI_SMSTR_ERR_OTHER == ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        else
        {
            /* QSPI_SMSTR_SUCCESS */
        }

        tx_info.p_tx_data = (uint8_t *)(tx_info.p_tx_data + boundary_cnt);
        txcnt -= boundary_cnt;
    }

    /* Check the data count is multiple of 16.
       When the data count is multiple of 16,
       transfer the data using DMAC transfer. */
    if (0 != (txcnt & 0xfffffff0))
    {
        tx_info.data_cnt  = (txcnt & 0xfffffff0);   /* Set the data count to multiple of 16. */
        tx_info.tran_mode = r_memdrv_qspi_set_tran_mode(devno);
        if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
        {
            tx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_WRITE;
        }
        else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
        {
            tx_info.op_mode   = QSPI_SMSTR_DUAL_SPI;
        }
        else
        {
            tx_info.op_mode   = QSPI_SMSTR_QUAD_SPI;
        }
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_tx_data_dtc(devno,
                                                    p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }

        ret_drv = R_QSPI_SMstr_Write(channel, &tx_info);
        if (QSPI_SMSTR_ERR_HARD == ret_drv)
        {
            return MEMDRV_ERR_HARD;
        }
        else if (QSPI_SMSTR_ERR_OTHER == ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        else
        {
            /* QSPI_SMSTR_SUCCESS */
        }

        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                   return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        tx_info.p_tx_data = (uint8_t *)(tx_info.p_tx_data + (txcnt & 0xfffffff0));
    }

    /* Check the data count is multiple of 16.
       When the data count is not multiple of 16,
       transfer the data of 1-15 bytes using software transfer. */
    if (0 != (txcnt & 0x0000000f))
    {
        tx_info.data_cnt  = (txcnt & 0x0000000f);   /* Set data count to 1-15 bytes. */
        tx_info.tran_mode = QSPI_SMSTR_SW;
        if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
        {
            tx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_WRITE;
        }
        else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
        {
            tx_info.op_mode   = QSPI_SMSTR_DUAL_SPI;
        }
        else
        {
            tx_info.op_mode   = QSPI_SMSTR_QUAD_SPI;
        }

        ret_drv = R_QSPI_SMstr_Write(channel, &tx_info);
        if (QSPI_SMSTR_ERR_HARD == ret_drv)
        {
            return MEMDRV_ERR_HARD;
        }
        else if (QSPI_SMSTR_ERR_OTHER == ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        else
        {
            /* QSPI_SMSTR_SUCCESS */
        }
    }

    return MEMDRV_SUCCESS;
}
#endif
/************************************************************************************************
* Function Name: r_memdrv_qspi_rx
* Description  : Receives data for status register and Read ID.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_HARD                      ;   Hardware error
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspi_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspi_smstr_info_t     rx_info;
    qspi_smstr_status_t   ret_drv = QSPI_SMSTR_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    
    rx_info.data_cnt  = p_memdrv_info->cnt;
    rx_info.p_rx_data = p_memdrv_info->p_data;
    rx_info.tran_mode = QSPI_SMSTR_SW;
    rx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_READ;

    ret_drv = R_QSPI_SMstr_Read(channel, &rx_info);
    if   (QSPI_SMSTR_SUCCESS == ret_drv)
    {
        return MEMDRV_SUCCESS;
    }
    else if (QSPI_SMSTR_ERR_HARD == ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_HARD;
    }
    else
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
} /* End of function r_memdrv_qspi_rx() */

/************************************************************************************************
* Function Name: r_memdrv_qspi_rx_data
* Description  : Receives data using the single mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_HARD                      ;   Hardware error
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
memdrv_err_t r_memdrv_qspi_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspi_smstr_info_t     rx_info;
    qspi_smstr_status_t   ret_drv = QSPI_SMSTR_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    
    rx_info.data_cnt  = p_memdrv_info->cnt;
    rx_info.p_rx_data = p_memdrv_info->p_data;

    /* Set transfer mode. */
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        rx_info.tran_mode = QSPI_SMSTR_SW;
    }
    else if (MEMDRV_CHK_MULT_OF_16 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 16, force to perform the CPU transfer. */
        rx_info.tran_mode = QSPI_SMSTR_SW;
    }
    else
    {
        rx_info.tran_mode = r_memdrv_qspi_set_tran_mode(devno);
    }

    switch (p_memdrv_info->io_mode)
    {
        case MEMDRV_MODE_SINGLE:
        {
            rx_info.op_mode = QSPI_SMSTR_SINGLE_SPI_READ;
        }
        break;
        case MEMDRV_MODE_DUAL:
        {
            rx_info.op_mode = QSPI_SMSTR_DUAL_SPI;
        }
        break;
        case MEMDRV_MODE_QUAD:
        {
            rx_info.op_mode = QSPI_SMSTR_QUAD_SPI;
        }
        break;
        default:
        break;
    }

    ret_drv = R_QSPI_SMstr_Read(channel, &rx_info);
    if   (QSPI_SMSTR_SUCCESS == ret_drv)
    {
        return MEMDRV_SUCCESS;
    }
    else if (QSPI_SMSTR_ERR_HARD == ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_HARD;
    }
    else
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

} /* End of function r_memdrv_qspi_rx_data() */
#elif (MEMDRV_CFG_DEV0_TYPE == 0)  | (MEMDRV_CFG_DEV1_TYPE == 0)
memdrv_err_t r_memdrv_qspi_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspi_smstr_info_t           rx_info;
    qspi_smstr_status_t         ret_drv      = QSPI_SMSTR_SUCCESS;
    uint32_t                    dummy_buffer = 0xffffffff;
    uint32_t                    boundary_cnt = 0;
    uint32_t                    rxcnt = p_memdrv_info->cnt;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    rx_info.p_tx_data = (uint8_t *)&dummy_buffer;
    rx_info.p_rx_data = p_memdrv_info->p_data;

    /* Check the 4 bytes address boundary.
       When the address is not 4 bytes address boundary,
       transfer the data of 1-3 bytes using software transfer. */
    if (0 != ((uint32_t)rx_info.p_rx_data & 0x00000003))
    {
        boundary_cnt      = (0x00000004 - ((uint32_t)rx_info.p_rx_data & 0x00000003));
        rx_info.data_cnt  = boundary_cnt;
        rx_info.tran_mode = QSPI_SMSTR_SW;
        if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
        {
            rx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_READ;
        }
        else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
        {
            rx_info.op_mode   = QSPI_SMSTR_DUAL_SPI;
        }
        else
        {
            rx_info.op_mode   = QSPI_SMSTR_QUAD_SPI;
        }

        ret_drv = R_QSPI_SMstr_Read(channel, &rx_info);
        if (QSPI_SMSTR_ERR_HARD == ret_drv)
        {
            return MEMDRV_ERR_HARD;
        }
        else if (QSPI_SMSTR_ERR_OTHER == ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        else
        {
            /* QSPI_SMSTR_SUCCESS */
        }

        rx_info.p_rx_data = (uint8_t *)(rx_info.p_rx_data + boundary_cnt);
        rxcnt -= boundary_cnt;
    }

    /* Check the data count is multiple of 16.
       When the data count is multiple of 16,
       transfer the data using DMAC transfer. */
    if (0 != (rxcnt & 0xfffffff0))
    {
        rx_info.data_cnt  = (rxcnt & 0xfffffff0);   /* Set the data count to multiple of 16. */
        rx_info.tran_mode = r_memdrv_qspi_set_tran_mode(devno);
        if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
        {
            rx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_READ;
        }
        else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
        {
            rx_info.op_mode   = QSPI_SMSTR_DUAL_SPI;
        }
        else
        {
            rx_info.op_mode   = QSPI_SMSTR_QUAD_SPI;
        }

        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_rx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_rx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_enable_rx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_enable_rx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }

        ret_drv = R_QSPI_SMstr_Read(channel, &rx_info);
        if (QSPI_SMSTR_ERR_HARD == ret_drv)
        {
            return MEMDRV_ERR_HARD;
        }
        else if (QSPI_SMSTR_ERR_OTHER == ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        else
        {
            /* QSPI_SMSTR_SUCCESS */
        }

        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_qspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_qspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
            }
        }


        rx_info.p_rx_data = (uint8_t *)(rx_info.p_rx_data + (rxcnt & 0xfffffff0));
    }

    /* Check the data count is multiple of 16.
       When the data count is not multiple of 16,
       transfer the data of 1-15 bytes using software transfer. */
    if (0 != (rxcnt & 0x0000000f))
    {
        rx_info.data_cnt  = (rxcnt & 0x0000000f);   /* Set data count to 1-15 bytes. */
        rx_info.tran_mode = QSPI_SMSTR_SW;
        if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
        {
            rx_info.op_mode   = QSPI_SMSTR_SINGLE_SPI_READ;
        }
        else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
        {
            rx_info.op_mode   = QSPI_SMSTR_DUAL_SPI;
        }
        else
        {
            rx_info.op_mode   = QSPI_SMSTR_QUAD_SPI;
        }

        ret_drv = R_QSPI_SMstr_Read(channel, &rx_info);
        if (QSPI_SMSTR_ERR_HARD == ret_drv)
        {
            return MEMDRV_ERR_HARD;
        }
        else if (QSPI_SMSTR_ERR_OTHER == ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        else
        {
            /* QSPI_SMSTR_SUCCESS */
        }
    }

    return MEMDRV_SUCCESS;
}
#endif

/************************************************************************************************
* Function Name: r_memdrv_qspi_set_tran_mode
* Description  : Sets transfer mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value : QSPI_SMSTR_SW
*              : QSPI_SMSTR_DMAC
*              : QSPI_SMSTR_DTC
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static qspi_smstr_tranmode_t r_memdrv_qspi_set_tran_mode(uint8_t devno)
{
    qspi_smstr_tranmode_t mode = QSPI_SMSTR_SW;

#if (MEMDRV_CFG_DEV0_INCLUDED == 1)
    if (MEMDRV_DEV0 == devno)
    {
        if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
        {
            mode = QSPI_SMSTR_DMAC;
        }
        else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
        {
            mode = QSPI_SMSTR_DTC;
        }
        else
        {
            mode = QSPI_SMSTR_SW;
        }
    }
#endif /* (MEMDRV_CFG_DEV0_INCLUDED == 1) */
#if (MEMDRV_CFG_DEV1_INCLUDED == 1)
    if (MEMDRV_DEV1 == devno)
    {
        if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
        {
            mode = QSPI_SMSTR_DMAC;
        }
        else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
        {
            mode = QSPI_SMSTR_DTC;
        }
        else
        {
            mode = QSPI_SMSTR_SW;
        }
    }
#endif /* (MEMDRV_CFG_DEV1_INCLUDED == 1) */

    return mode;
} /* End of function r_memdrv_qspi_set_tran_mode() */

/***********************************************************************************************************************
* Function Name: qspi_baud_cal
* Description  : Determines the QSPI channel SPBR register setting for the requested baud rate.
*                Returns the actual bit rate that the setting will achieve which may differ from requested.
*                If the requested bit rate cannot be exactly achieved, the next lower bit rate setting will be applied.
*                If successful, applies the calculated setting to the SPBR register.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value :
* Note: Target baud must be >= PCLK/4 to get anything out.
* Limitations   : Does not track dynamically changing PCLK. Relies on constant BSP_PCLKB_HZ
***********************************************************************************************************************/
static uint8_t qspi_baud_cal(uint8_t devno)
{
    uint8_t bps_result;
    uint32_t bit_rate;
    uint32_t bit_rate_cal = 0;
    int32_t     f;                      // Frequency
    int32_t     n;                      // n term in equation
    int32_t     bit_div;                // N term in equation

    if (MEMDRV_DEV0 == devno)
    {
        bit_rate = (uint32_t)MEMDRV_CFG_DEV0_BR;
    }
    else if (MEMDRV_DEV1 == devno)
    {
        bit_rate = (uint32_t)MEMDRV_CFG_DEV1_BR;
    }
    else
    {

    }

#if defined(BSP_MCU_RX64M) || defined(BSP_MCU_RX71M) || defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX66N) || \
    defined(BSP_MCU_RX72M) || defined(BSP_MCU_RX72N)
        f = BSP_PCLKB_HZ;
    #else
        f = BSP_PCLKA_HZ;
    #endif

    if ((f / bit_rate) < 2)
    {
        return 0;
    }
    
    bit_div = (uint32_t)((MEMDRV_TRNS_CMD & 0x00c0)>>2);
    
    n = ((f >> (bit_div + 1)) / (int32_t)bit_rate);
    if ((n > 0) && (n <= 0xff))
    {
        /* Now plug n back into the formula for BPS and check it. */
        bit_rate_cal = (uint32_t)(f / (2 *(n << bit_div)));

        if (bit_rate_cal > bit_rate)
        {
            n += 1;
            if (n > 0xff)
            {
                return 0; 
            }
        }
        bps_result = n;
    }
    else
    {
        return 0;
    }
    
    return bps_result;
} /* End of function qspi_baud_cal() */

#else
memdrv_err_t r_memdrv_qspi_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_open() */
memdrv_err_t r_memdrv_qspi_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_close() */
memdrv_err_t r_memdrv_qspi_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_disable() */
memdrv_err_t r_memdrv_qspi_disable_tx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_disable_tx_data() */
memdrv_err_t r_memdrv_qspi_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_disable_rx_data() */
memdrv_err_t r_memdrv_qspi_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_enable() */
memdrv_err_t r_memdrv_qspi_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_enable_tx_data() */
memdrv_err_t r_memdrv_qspi_enable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_enable_rx_data() */
memdrv_err_t r_memdrv_qspi_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_tx() */
memdrv_err_t r_memdrv_qspi_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_tx_data() */
memdrv_err_t r_memdrv_qspi_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_rx() */
memdrv_err_t r_memdrv_qspi_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspi_rx_data() */

#endif  /* ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)) || \
           ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR))
/* End of File */
