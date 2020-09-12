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
* System Name  : memdrv software
* File Name    : r_memdrv_rx.c
* Version      : 1.02
* Device       : -
* Abstract     : IO I/F module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : memdrv I/O file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.12.2018 1.00     Initial Release
*              : 04.04.2019 1.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*              : 22.11.2019 1.02     Modified comment of API function to Doxygen style.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */

/************************************************************************************************
Macro definitions
*************************************************************************************************/

/************************************************************************************************
Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/
const uint32_t g_memdrv_dummy_tx = 0xffffffff; /* @suppress("Source line ordering") */
uint32_t g_memdrv_dummy_rx;
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI))
extern rspi_handle_t g_rspi_handle;
extern volatile bool  g_transfer_busy;
#endif
/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
#if  MEMDRV_CFG_LONGQ_ENABLE == 1
static longq_hdl_t       p_memdrv_long_que;     // LONGQ handler    
#endif  /* MEMDRV_CFG_LONGQ_ENABLE */

static uint32_t r_memdrv_get_drv_type(uint8_t devno);
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
static memdrv_err_t memdrv_param_check(uint8_t devno, st_memdrv_info_t * memdrv_info);
#endif
static const st_memdrv_func_t r_memdrv_tbl[MEMDRV_INDX_DRVR_NUM][MEMDRV_INDX_FUNC_NUM] =
{
    /* RX RSPI FIT Module */
    {
        { r_memdrv_rspi_open            },
        { r_memdrv_rspi_close           },
        { r_memdrv_rspi_disable         },
        { r_memdrv_rspi_disable_tx_data },
        { r_memdrv_rspi_disable_rx_data },
        { r_memdrv_rspi_enable          },
        { r_memdrv_rspi_enable_tx_data  },
        { r_memdrv_rspi_enable_rx_data  },
        { r_memdrv_rspi_tx              },
        { r_memdrv_rspi_tx_data         },
        { r_memdrv_rspi_rx              },
        { r_memdrv_rspi_rx_data         },
    },
    /* RX QSPI SMSTR FIT Module */
    {
        { r_memdrv_qspi_open            },
        { r_memdrv_qspi_close           },
        { r_memdrv_qspi_disable         },
        { r_memdrv_qspi_disable_tx_data },
        { r_memdrv_qspi_disable_rx_data },
        { r_memdrv_qspi_enable          },
        { r_memdrv_qspi_enable_tx_data  },
        { r_memdrv_qspi_enable_rx_data  },
        { r_memdrv_qspi_tx              },
        { r_memdrv_qspi_tx_data         },
        { r_memdrv_qspi_rx              },
        { r_memdrv_qspi_rx_data         },
    },
    /* RX SCI SPI FIT Module */
    {
        { r_memdrv_sci_open            },
        { r_memdrv_sci_close           },
        { r_memdrv_sci_disable         },
        { r_memdrv_sci_disable_tx_data },
        { r_memdrv_sci_disable_rx_data },
        { r_memdrv_sci_enable          },
        { r_memdrv_sci_enable_tx_data  },
        { r_memdrv_sci_enable_rx_data  },
        { r_memdrv_sci_tx              },
        { r_memdrv_sci_tx_data         },
        { r_memdrv_sci_rx              },
        { r_memdrv_sci_rx_data         },
    },
};

/***********************************************************************************************************************
* Function Name: R_MEMDRV_ClearDMACFlagTx
*******************************************************************************************************************/ /**
* @brief This function clears the transmit empty interrupt flag set at DMAC transfer end.
* @param[in] channel Device channel number
* @details   Clears the transmit empty interrupt flag. Use an interrupt generated at DMAC transfer end.
* @note      None
*/
void R_MEMDRV_ClearDMACFlagTx(uint8_t channel)
{
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI))
    g_rspi_handle->channel = channel;

    R_RSPI_IntSptiIerClear(g_rspi_handle);
#endif
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR))
    R_QSPI_SMstr_Int_Spti_Ier_Clear(channel);
    R_QSPI_SMstr_Int_Spti_Dmacdtc_Flag_Set(channel, QSPI_SET_TRANS_STOP);
#endif
} /* End of function R_MEMDRV_ClearDMACFlagTx() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_ClearDMACFlagRx
*******************************************************************************************************************/ /**
* @brief This function clears the receive buffer full interrupt flag set at DMAC transfer end.
* @param[in] channel Device channel number
* @details   Clears the receive buffer full interrupt flag. Use an interrupt generated at DMAC transfer end.
* @note      None
*/
void R_MEMDRV_ClearDMACFlagRx(uint8_t channel)
{
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI))
    g_rspi_handle->channel = channel;
    
    R_RSPI_IntSptiIerClear(g_rspi_handle);
    R_RSPI_IntSpriIerClear(g_rspi_handle);
    if (0 == channel)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
    ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
        RSPI0.SPCR.BIT.SPE   = 0;  // Disable RSPI.
#if RSPI_CFG_REQUIRE_LOCK == 1
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPI0 + channel));
#endif
#endif
    }
    else if (1 == channel)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
    ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
        RSPI1.SPCR.BIT.SPE   = 0;  // Disable RSPI.
#if RSPI_CFG_REQUIRE_LOCK == 1
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPI0 + channel));
#endif
#endif
    }
    else if (2 == channel)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
    ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
        RSPI2.SPCR.BIT.SPE   = 0;  // Disable RSPI.
#if RSPI_CFG_REQUIRE_LOCK == 1
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPI0 + channel));
#endif
#endif
    }
    else
    {
    }
    g_transfer_busy = false;
#endif
#if (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPI_SMSTR) | \
    (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)
    R_QSPI_SMstr_Int_Spti_Ier_Clear(channel);
    R_QSPI_SMstr_Int_Spri_Ier_Clear(channel);
    R_QSPI_SMstr_Int_Spri_Dmacdtc_Flag_Set(channel, QSPI_SET_TRANS_STOP);
#endif
} /* End of function R_MEMDRV_ClearDMACFlagRx() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_1msInterval
*******************************************************************************************************************/ /**
* @brief This function performs interval timer count processing.
* @details   Increments the driver software's internal timer counter while waiting for DMAC or DTC transfer end.
* @note      None
*/
void R_MEMDRV_1msInterval(void)
{
#if (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) | \
    (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI)
    r_memdrv_rspi_1ms_interval();
#endif
#if (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPI_SMSTR) | \
    (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)
    R_QSPI_SMstr_1ms_Interval();
#endif
#if (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) | \
    (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI)
    r_memdrv_sci_1ms_interval();
#endif
} /* End of function R_MEMDRV_1msInterval() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_SetLogHdlAddress
*******************************************************************************************************************/ /**
* @brief This function processes setting of the LONGQ FIT module handler address.
* @param[in] user_long_que LONGQ FIT module handler address
* @retval    MEMDRV_SUCCESS: Normal end
* @details   Sets the handler address of the LONGQ FIT module in the memory driver.
* @note      If the MEMDRV_CFG_LONGQ_ENABLE == 0 and this function is called, this function does nothing.
*/
memdrv_err_t R_MEMDRV_SetLogHdlAddress(uint32_t user_long_que)
{
#if MEMDRV_CFG_LONGQ_ENABLE == 1
    p_memdrv_long_que = (longq_hdl_t)user_long_que;

#if (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) | \
    (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI)
    R_RSPI_SetLogHdlAddress(user_long_que);
#endif
#if (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPI_SMSTR) | \
    (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)
    R_QSPI_SMstr_Set_LogHdlAddress(user_long_que);
#endif
#endif  /* MEMDRV_CFG_LONGQ_ENABLE */

    return MEMDRV_SUCCESS;
}

#if MEMDRV_CFG_LONGQ_ENABLE == 1
/*******************************************************************************
* Function Name: r_memdrv_log
* Description  : Stores error information to LONGQ buffer.
* Arguments    : flg -
*                    Breakpoint processing
*                fid -
*                    MEMDRV driver file No.
*                line -
*                    MEMDRV driver line number
* Return Value : 0 -
*                    Successful operation
*                1 -
*                    Error
*******************************************************************************/
uint32_t r_memdrv_log(uint32_t flg, uint32_t fid, uint32_t line)
{
    longq_err_t err;
    uint32_t    ul_tmp;

    /* Long to Byte */
    ul_tmp = 0;                                         // log     Reserved
    ul_tmp = (ul_tmp | (line << 8));                    // log     LINE
    ul_tmp = (ul_tmp | (fid  << 21));                   // log     FID FILE No.
    ul_tmp = (ul_tmp | (MEMDRV_DRIVER_ID << 27));       // log     FID DRIVER No.
    ul_tmp = (ul_tmp | (flg  << 31));                   // log     Breakpoint processing

    err = R_LONGQ_Put(p_memdrv_long_que, ul_tmp);
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
}
#endif

/***********************************************************************************************************************
* Function Name: R_MEMDRV_GetVersion 
*******************************************************************************************************************/ /**
* @brief This function gets the memory driver version information.
* @return    Upper 2 bytes: Major version (decimal notation).\n
*            Lower 2 bytes: Minor version (decimal notation).
* @details   Returns the driver version information.
* @note      None
*/
uint32_t R_MEMDRV_GetVersion(void)
{
    uint32_t version_number = 0;
    /* Bring in major version number. */
    version_number = ((uint16_t)MEMDRV_VERSION_MAJOR) << 16;
    /* Bring in minor version number. */
    version_number |= (uint16_t)MEMDRV_VERSION_MINOR;
    return version_number;
}/* End of function R_MEMDRV_GetVersion() */

/************************************************************************************************
* Function Name: r_memdrv_get_drv_ch
* Description  : Gets driver channel no.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value : Channel No.
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_memdrv_get_drv_ch(uint8_t devno)
{
    uint8_t channel = 0;

#if (MEMDRV_CFG_DEV0_INCLUDED == 1)
    if (MEMDRV_DEV0 == devno)
    {
        channel = MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH;
    }
#endif /* (MEMDRV_CFG_DEV0_INCLUDED == 1) */

#if (MEMDRV_CFG_DEV1_INCLUDED == 1)
    if (MEMDRV_DEV1 == devno)
    {
        channel = MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH;
    }
#endif /* (MEMDRV_CFG_DEV1_INCLUDED == 1) */

    return channel;

} /* End of function r_memdrv_get_drv_ch() */

/************************************************************************************************
* Function Name: r_memdrv_get_drv_type
* Description  : Gets driver type.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value : driver type.
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static uint32_t r_memdrv_get_drv_type(uint8_t devno)
{
    uint32_t drv_type = 0;

#if (MEMDRV_CFG_DEV0_INCLUDED == 1)
    if (MEMDRV_DEV0 == devno)
    {
        switch (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_MASK_TYPE)
        {
            case MEMDRV_DRVR_RX_FIT_RSPI:
            {
                drv_type = MEMDRV_INDX_RSPI;
            }
            break;
            case MEMDRV_DRVR_RX_FIT_QSPI_SMSTR:
            {
                drv_type = MEMDRV_INDX_QSPI_SMSTR;
            }
            break;
            case MEMDRV_DRVR_RX_FIT_SCI_SPI:
            {
                drv_type = MEMDRV_INDX_SCI_SPI;
            }
            break;
            default:
            {
                /* Do nothing. */
                R_BSP_NOP();
            }
            break;
        }
    }
#endif /* (MEMDRV_CFG_DEV0_INCLUDED == 1) */
#if (MEMDRV_CFG_DEV1_INCLUDED == 1)
    if (MEMDRV_DEV1 == devno)
    {
        switch (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_MASK_TYPE)
        {
            case MEMDRV_DRVR_RX_FIT_RSPI:
            {
                drv_type = MEMDRV_INDX_RSPI;
            }
            break;
            case MEMDRV_DRVR_RX_FIT_QSPI_SMSTR:
            {
                drv_type = MEMDRV_INDX_QSPI_SMSTR;
            }
            break;
            case MEMDRV_DRVR_RX_FIT_SCI_SPI:
            {
                drv_type = MEMDRV_INDX_SCI_SPI;
            }
            default:
            {
                /* Do nothing. */
                R_BSP_NOP();
            }
            break;
        }
    }
#endif /* (MEMDRV_CFG_DEV1_INCLUDED) */

    return drv_type;
} /* End of function r_memdrv_get_drv_type() */

#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
/*******************************************************************************
* Function Name: memdrv_param_check
* Description  : parameter checking.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_PARAM                       ;   parameter error
*******************************************************************************/
static memdrv_err_t memdrv_param_check(uint8_t devno, st_memdrv_info_t * memdrv_info)
{
    if (((devno != MEMDRV_DEV0) && (devno != MEMDRV_DEV1)) || (NULL == memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
    return MEMDRV_SUCCESS;
} /* End of function memdrv_param_check() */
#endif
/***********************************************************************************************************************
* Function Name: R_MEMDRV_Open
*******************************************************************************************************************/ /**
* @brief This function opens the memory driver. This function must be run before calling other API functions.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Initializes the FIT module used for SPI/QSPI/SCI communication and data transfer (only when DTC/DMAC is
*            selected).
* @note      None
*/
memdrv_err_t R_MEMDRV_Open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_OPEN].p_func(devno, p_memdrv_info);

} /* End of function R_MEMDRV_Open() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_Close
*******************************************************************************************************************/ /**
* @brief This function closes the memory driver.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Closes the FIT module used for SPI/QSPI/SCI communication and data transfer (only when DTC/DMAC is
*            selected).
* @note      None
*/
memdrv_err_t R_MEMDRV_Close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_CLOSE].p_func(devno, p_memdrv_info);

} /* End of function R_MEMDRV_Close() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_Disable
*******************************************************************************************************************/ /**
* @brief This function disables the memory driver.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @details   Disables the memory driver communication settings.
* @note      None
*/
memdrv_err_t R_MEMDRV_Disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_DISABLE].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Disable() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_DisableTxData
*******************************************************************************************************************/ /**
* @brief This function disables data transmission by the memory driver.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Disables the DMAC/DTC FIT module settings used for data transmission.
* @note      None
*/
memdrv_err_t R_MEMDRV_DisableTxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_DISABLE_TX_DATA].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Disable_tx_data() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_DisableRxData
*******************************************************************************************************************/ /**
* @brief This function disables data reception by the memory driver.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Disables the DMAC/DTC FIT module settings used for data reception.
* @note      None
*/
memdrv_err_t R_MEMDRV_DisableRxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_DISABLE_RX_DATA].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Disable_rx_data() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_Enable
*******************************************************************************************************************/ /**
* @brief This function enables the memory driver.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Processing to enable the memory driver.
* @note      None
*/
memdrv_err_t R_MEMDRV_Enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_ENABLE].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Enable() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_EnableTxData
*******************************************************************************************************************/ /**
* @brief This function enables data transmission.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Enables the DMAC/DTC FIT module used for data transmission.
* @note      None
*/
memdrv_err_t R_MEMDRV_EnableTxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_ENABLE_TX_DATA].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Enable_tx_data() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_EnableRxData
*******************************************************************************************************************/ /**
* @brief This function enables data reception.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Enables the DMAC/DTC FIT module settings used for data reception.
* @note      None
*/
memdrv_err_t R_MEMDRV_EnableRxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_ENABLE_RX_DATA].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Enable_rx_data() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_Tx
*******************************************************************************************************************/ /**
* @brief This function performs command transmission processing.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_HARD: Hardware error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Transmits the data specified by the memory driver information structure. Supports CPU transfer only.
* @note      None
*/
memdrv_err_t R_MEMDRV_Tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_TX].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Tx() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_TxData
*******************************************************************************************************************/ /**
* @brief This function performs data transmission processing.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_HARD: Hardware error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Transmits the data specified by the memory driver information structure. Supports CPU, DMAC, and DTC
*            transfer.
* @note      None
*/
memdrv_err_t R_MEMDRV_TxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_TX_DATA].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Tx_data() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_Rx
*******************************************************************************************************************/ /**
* @brief This function performs processing to receive the state and ID.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_HARD: Hardware error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Receives the data specified by the memory driver information structure. Supports CPU transfer only.
* @note      None
*/
memdrv_err_t R_MEMDRV_Rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_RX].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Rx() */

/***********************************************************************************************************************
* Function Name: R_MEMDRV_RxData
*******************************************************************************************************************/ /**
* @brief This function performs data reception processing.
* @param[in] devno Device number
* @param[in] *p_memdrv_info Memory driver information structure
* @retval    MEMDRV_SUCCESS: Normal end
* @retval    MEMDRV_ERR_PARAM: parameter error
* @retval    MEMDRV_ERR_HARD: Hardware error
* @retval    MEMDRV_ERR_OTHER: Other error
* @details   Receives the data specified by the memory driver information structure. Supports CPU, DMAC, and DTC
*            transfer.
* @note      None
*/
memdrv_err_t R_MEMDRV_RxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
#if MEMDRV_CFG_PARAM_CHECKING_ENABLE == 1
    if (MEMDRV_SUCCESS != memdrv_param_check(devno, p_memdrv_info))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_PARAM;
    }
#endif
    return r_memdrv_tbl[r_memdrv_get_drv_type(devno)]
                       [MEMDRV_INDX_RX_DATA].p_func(devno, p_memdrv_info);
} /* End of function R_MEMDRV_Rx_data() */

/* End Of File */
