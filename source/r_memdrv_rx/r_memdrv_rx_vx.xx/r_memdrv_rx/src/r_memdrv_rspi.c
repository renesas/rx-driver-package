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
* System Name  : MEMDRV  software
* File Name    : r_memdrv_rspi.c
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
*              : 22.11.2019 1.02     The module is updated to fix the software issue.
*                                    When r_memdrv_rspi_write_data function is called, 
*                                    there are cases when transmit data/receive data are not processed successfully.
*                                    The issue occurs when the number of transmit is set to a value of 1024 byte.
*                                    Corrected parameter type of the r_memdrv_rspi_write_data function.
*                                    Corrected parameter type of the r_memdrv_rspi_read_data function.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */
/* Check driver interface. */
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI))
#include "r_pinset.h"
/************************************************************************************************
Macro definitions
*************************************************************************************************/
#define RSPI_TIMER_CH_FLG          (0)
#define RSPI_TIMER_CH_COUNT        (1)
#define RSPI_TIMER_CH_MAX_COUNT    (2)
#define RSPI_TIMER_MIN_TIME        (100)       /* 100ms             */
#define RSPI_SECTOR_SIZE           (512)       /* 1 sector size     */
#define RSPI_TRAN_SIZE             (4)
#define RSPI_EXCHG_MAX_COUNT       (65532)
/************************************************************************************************
Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/
volatile bool  g_transfer_busy = false;
rspi_handle_t g_rspi_handle;
/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
static rspi_evt_t callback_event; 
static volatile uint32_t gs_memdrv_rspi_timer_cnt_out[RSPI_MAX_CHANNELS][RSPI_TIMER_CH_MAX_COUNT + 1];
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) | \
    (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)   | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
static uint8_t rev_buf[RSPI_SECTOR_SIZE];
#endif

static void                 r_memdrv_rspi_start_timer(uint8_t channel, uint32_t msec);
static memdrv_err_t         r_memdrv_rspi_check_timer(uint8_t channel);
static void                 r_memdrv_rspi_end_timer(uint8_t channel);
static memdrv_err_t         r_memdrv_rspi_wait(uint8_t channel, uint32_t size);
static void                 r_memdrv_rspi_callback(void *p_data);
static void                 rspi_init_ports(void);

static memdrv_err_t r_rspi_exchg(uint8_t * p_data, uint16_t size);
static rspi_str_tranmode_t r_memdrv_rspi_set_tran_mode(uint8_t devno);
static memdrv_err_t r_memdrv_rspi_disable_tx_data_dmac(uint8_t devno,
                                                       st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_disable_rx_data_dmac(uint8_t devno,
                                                       st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_enable_tx_data_dmac(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_enable_rx_data_dmac(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_disable_tx_data_dtc(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_disable_rx_data_dtc(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_enable_tx_data_dtc(uint8_t devno,
                                                     st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_enable_rx_data_dtc(uint8_t devno,
                                                     st_memdrv_info_t * p_memdrv_info);
static memdrv_err_t r_memdrv_rspi_write_data(uint8_t channel, uint16_t count,
                                             uint8_t * pdata, rspi_command_word_t cmd);
static memdrv_err_t r_memdrv_rspi_read_data(uint8_t channel, uint16_t count,
                                            uint8_t * pdata, rspi_command_word_t cmd);

/************************************************************************************************
* Function Name: r_memdrv_rspi_open
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
memdrv_err_t r_memdrv_rspi_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    rspi_chnl_settings_t config;
    rspi_command_word_t spcmd_cmd_word;
    
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    if (MEMDRV_DEV0 == devno)
    {
        config.bps_target = (uint32_t)MEMDRV_CFG_DEV0_BR;
    }
    else if (MEMDRV_DEV1 == devno)
    {
        config.bps_target = (uint32_t)MEMDRV_CFG_DEV1_BR;
    }

    config.master_slave_mode = RSPI_MS_MODE_MASTER;
    config.gpio_ssl = RSPI_IF_MODE_3WIRE;
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    config.tran_mode = RSPI_TRANS_MODE_DMAC;
#elif (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    config.tran_mode = RSPI_TRANS_MODE_DTC;
#else
    config.tran_mode = RSPI_TRANS_MODE_SW;
#endif

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif

#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    if (MEMDRV_SUCCESS != r_memdrv_dmac_open())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#elif (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    if (MEMDRV_SUCCESS != r_memdrv_dtc_open())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif

    if (RSPI_SUCCESS != R_RSPI_Open (channel,     
                               &config,     
                               spcmd_cmd_word,    
                               &r_memdrv_rspi_callback,   
                               &g_rspi_handle))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    rspi_init_ports();
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_open() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_close
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
memdrv_err_t r_memdrv_rspi_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);

#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    if (MEMDRV_SUCCESS != r_memdrv_dmac_close())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#elif (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    if (MEMDRV_SUCCESS != r_memdrv_dtc_close())
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif
    
    g_rspi_handle->channel = channel;
    if (RSPI_SUCCESS != R_RSPI_Close(g_rspi_handle))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_close() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable
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
memdrv_err_t r_memdrv_rspi_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_disable() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable_tx_data
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
memdrv_err_t r_memdrv_rspi_disable_tx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    /* Casting to a uint32_t type is valid. */
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
                if (r_memdrv_rspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_disable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable_tx_data_dmac
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
static memdrv_err_t r_memdrv_rspi_disable_tx_data_dmac(uint8_t devno,
                                                       st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_stat_t stat_dmac;

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_RSPI;
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
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_disable_tx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable_tx_data_dtc
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
static memdrv_err_t r_memdrv_rspi_disable_tx_data_dtc(uint8_t devno,
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
            dtc_info.act_source = DTCE_RSPI0_SPTI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPTI2;
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
            dtc_info.act_source = DTCE_RSPI0_SPRI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPRI2;
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
} /* End of function r_memdrv_rspi_disable_tx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable_rx_data
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
memdrv_err_t r_memdrv_rspi_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    /* Casting to a uint32_t type is valid. */
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
                if (r_memdrv_rspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_disable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable_rx_data_dmac
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
static memdrv_err_t r_memdrv_rspi_disable_rx_data_dmac(uint8_t devno,
                                                       st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC) | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_stat_t stat_dmac;

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_RSPI;
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
} /* End of function r_memdrv_rspi_disable_rx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_disable_rx_data_dtc
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
static memdrv_err_t r_memdrv_rspi_disable_rx_data_dtc(uint8_t devno,
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
            dtc_info.act_source = DTCE_RSPI0_SPTI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPTI2;
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
            dtc_info.act_source = DTCE_RSPI0_SPRI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPRI2;
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
} /* End of function r_memdrv_rspi_disable_rx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable
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
memdrv_err_t r_memdrv_rspi_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    rspi_cmd_baud_t rspi_bitrate;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    if (MEMDRV_DEV0 == devno)
    {
        rspi_bitrate.bps_target = (uint32_t)MEMDRV_CFG_DEV0_BR;
    }
    else if (MEMDRV_DEV1 == devno)
    {
        rspi_bitrate.bps_target = (uint32_t)MEMDRV_CFG_DEV1_BR;
    }

    g_rspi_handle->channel = channel;
    /* Casting to a uint32_t type is valid. */
    if (R_RSPI_Control(g_rspi_handle, RSPI_CMD_SET_BAUD, &rspi_bitrate) != RSPI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_enable() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable_tx_data
* Description  : Enables I/O driver for writing data.
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
memdrv_err_t r_memdrv_rspi_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    rspi_cmd_baud_t rspi_bitrate;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    if (MEMDRV_DEV0 == devno)
    {
        rspi_bitrate.bps_target = (uint32_t)MEMDRV_CFG_DEV0_BR_WRITE_DATA;
    }
    else if (MEMDRV_DEV1 == devno)
    {
        rspi_bitrate.bps_target = (uint32_t)MEMDRV_CFG_DEV1_BR_WRITE_DATA;
    }

    g_rspi_handle->channel = channel;
    /* Casting to a uint32_t type is valid. */
    if (R_RSPI_Control(g_rspi_handle, RSPI_CMD_SET_BAUD, &rspi_bitrate) != RSPI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    /* Casting to a uint32_t type is valid. */
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
                if (r_memdrv_rspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_enable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable_tx_data_dmac
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
static memdrv_err_t r_memdrv_rspi_enable_tx_data_dmac(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_transfer_data_cfg_t dmac_cfg;
    uint32_t reg_adr = 0;
    dmaca_stat_t stat_dmac;
    
    g_rspi_handle->channel = channel;
    if (R_RSPI_GetBuffRegAddress(g_rspi_handle, &reg_adr) != RSPI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_RSPI;
    dmac_info.src_adr       = (uint32_t)p_memdrv_info->p_data;
    dmac_info.dst_adr       = reg_adr;
    dmac_info.block_size    = 0;
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
            dmac_info.act_source = IR_RSPI0_SPTI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_RSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_RSPI2_SPTI2;
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
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_RSPI;
    dmac_info.src_adr       = reg_adr;
    dmac_info.dst_adr       = (uint32_t)&rev_buf;
    dmac_info.block_size    = 0;
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
            dmac_info.act_source = IR_RSPI0_SPRI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_RSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_RSPI2_SPRI2;
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
} /* End of function r_memdrv_rspi_enable_tx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable_tx_data_dtc
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
static memdrv_err_t r_memdrv_rspi_enable_tx_data_dtc(uint8_t devno,
                                                     st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dtc_info_t dtc_info;
    dtc_transfer_data_cfg_t dtc_cfg;
    uint32_t reg_adr = 0;
    
    g_rspi_handle->channel = channel;
    if (R_RSPI_GetBuffRegAddress(g_rspi_handle, &reg_adr) != RSPI_SUCCESS)
    {
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_RSPI;
    dtc_info.src_adr        = (uint32_t)p_memdrv_info->p_data;
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
            dtc_info.act_source = DTCE_RSPI0_SPTI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPTI2;
    #endif
        break;
        }
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
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_RSPI;
    dtc_info.src_adr        = reg_adr;
    dtc_info.dst_adr        = (uint32_t)&rev_buf;
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
            dtc_info.act_source = DTCE_RSPI0_SPRI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPRI2;
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
} /* End of function r_memdrv_rspi_enable_tx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable_rx_data
* Description  : Enables I/O driver for reading data.
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
memdrv_err_t r_memdrv_rspi_enable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    rspi_cmd_baud_t rspi_bitrate;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    if (MEMDRV_DEV0 == devno)
    {
        rspi_bitrate.bps_target = (uint32_t)MEMDRV_CFG_DEV0_BR_READ_DATA;
    }
    else if (MEMDRV_DEV1 == devno)
    {
        rspi_bitrate.bps_target = (uint32_t)MEMDRV_CFG_DEV1_BR_READ_DATA;
    }

    g_rspi_handle->channel = channel;
    /* Casting to a uint32_t type is valid. */
    if (R_RSPI_Control(g_rspi_handle, RSPI_CMD_SET_BAUD, &rspi_bitrate) != RSPI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
    /* Casting to a uint32_t type is valid. */
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
                if (r_memdrv_rspi_enable_rx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_rx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_enable_rx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_rx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
    }
#endif
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_enable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable_rx_data_dmac
* Description  : Enables I/O driver for reading data and DMAC.
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
static memdrv_err_t r_memdrv_rspi_enable_rx_data_dmac(uint8_t devno,
                                                      st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dmac_info_t dmac_info;
    dmaca_transfer_data_cfg_t dmac_cfg;
    uint32_t reg_adr = 0;
    dmaca_stat_t stat_dmac;
    
    g_rspi_handle->channel = channel;
    if (R_RSPI_GetBuffRegAddress(g_rspi_handle, &reg_adr) != RSPI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_RSPI;
    dmac_info.src_adr       = (uint32_t)&g_memdrv_dummy_tx;
    dmac_info.dst_adr       = reg_adr;
    dmac_info.block_size    = 0;
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
            dmac_info.act_source = IR_RSPI0_SPTI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_RSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_RSPI2_SPTI2;
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
    dmac_info.p_dmac_cfg->src_addr_mode = DMACA_SRC_ADDR_FIXED; // Dummy

    if (r_memdrv_dmac_enable(&dmac_info) != MEMDRV_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dmac_info.drv           = MEMDRV_DRVR_RX_FIT_RSPI;
    dmac_info.src_adr       = reg_adr;
    dmac_info.dst_adr       = (uint32_t)p_memdrv_info->p_data;
    dmac_info.block_size    = 0;
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
            dmac_info.act_source = IR_RSPI0_SPRI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dmac_info.act_source = IR_RSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dmac_info.act_source = IR_RSPI2_SPRI2;
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
} /* End of function r_memdrv_rspi_enable_rx_data_dmac() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_enable_rx_data_dtc
* Description  : Enables I/O driver for reading data and DTC.
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
static memdrv_err_t r_memdrv_rspi_enable_rx_data_dtc(uint8_t devno,
                                                     st_memdrv_info_t * p_memdrv_info)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    st_memdrv_dtc_info_t dtc_info;
    dtc_transfer_data_cfg_t dtc_cfg;
    uint32_t reg_adr = 0;
    
    g_rspi_handle->channel = channel;
    if (R_RSPI_GetBuffRegAddress(g_rspi_handle, &reg_adr) != RSPI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_RSPI;
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
            dtc_info.act_source = DTCE_RSPI0_SPTI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPTI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPTI2;
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
    dtc_info.p_dtc_cfg->src_addr_mode = DTC_SRC_ADDR_FIXED; // Dummy 

    if (r_memdrv_dtc_enable(&dtc_info) != MEMDRV_SUCCESS)
    {
        return MEMDRV_ERR_OTHER;
    }

    dtc_info.drv            = MEMDRV_DRVR_RX_FIT_RSPI;
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
            dtc_info.act_source = DTCE_RSPI0_SPRI0;
    #endif
        }
        break;
        case 1:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
            dtc_info.act_source = DTCE_RSPI1_SPRI1;
    #endif
        }
        break;
        case 2:
        {
    #if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
        ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
            dtc_info.act_source = DTCE_RSPI2_SPRI2;
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
} /* End of function r_memdrv_rspi_enable_rx_data_dtc() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_tx
* Description  : Transmits data for command and status register.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_HARD                        ;   Hardware error
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_rspi_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t             channel = r_memdrv_get_drv_ch(devno);
    memdrv_err_t        ret_memdrv = MEMDRV_SUCCESS;
    rspi_command_word_t spcmd_cmd_word;

#if ((MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)   | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC))
    rspi_cmd_trans_mode_t   mode;
    rspi_err_t   ret_drv = RSPI_SUCCESS;
#endif

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif

#if ((MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)   | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC))
    mode.transfer_mode = RSPI_TRANS_MODE_SW;
    ret_drv = R_RSPI_Control(g_rspi_handle,
                             RSPI_CMD_SET_TRANS_MODE,
                             (void *)&mode);
    if (RSPI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif
    ret_memdrv = r_memdrv_rspi_write_data(channel, p_memdrv_info->cnt, p_memdrv_info->p_data, spcmd_cmd_word);
    if (MEMDRV_SUCCESS != ret_memdrv)
    {
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_tx() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_tx_data
* Description  : Transmits data using the single mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_HARD                        ;   Hardware error
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
#if (MEMDRV_CFG_DEV0_TYPE == 1)  | (MEMDRV_CFG_DEV1_TYPE == 1)
memdrv_err_t r_memdrv_rspi_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t                 channel = r_memdrv_get_drv_ch(devno);
    rspi_command_word_t     spcmd_cmd_word;
    rspi_err_t              ret_drv = RSPI_SUCCESS;
    memdrv_err_t            ret_memdrv = MEMDRV_SUCCESS;
    rspi_cmd_trans_mode_t   mode;

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_DATA_CMD;
#endif /* (__ICCRX__) */
#endif

    /* Set transfer mode. */
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        mode.transfer_mode = RSPI_TRANS_MODE_SW;
#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
    }
    else if (MEMDRV_CHK_MULT_OF_4 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 4, force to perform the CPU transfer. */
        mode.transfer_mode = RSPI_TRANS_MODE_SW;
#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
    }
    else
    {
        mode.transfer_mode = r_memdrv_rspi_set_tran_mode(devno);
    }
    ret_drv = R_RSPI_Control(g_rspi_handle,
                             RSPI_CMD_SET_TRANS_MODE,
                             (void *)&mode);
    if (RSPI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    r_rspi_exchg(p_memdrv_info->p_data, p_memdrv_info->cnt);
    ret_memdrv = r_memdrv_rspi_write_data(channel, p_memdrv_info->cnt, p_memdrv_info->p_data, spcmd_cmd_word);
    if (MEMDRV_SUCCESS != ret_memdrv)
    {
        r_rspi_exchg(p_memdrv_info->p_data, p_memdrv_info->cnt);
        return MEMDRV_ERR_OTHER;
    }
    r_rspi_exchg(p_memdrv_info->p_data, p_memdrv_info->cnt);
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_tx_data() */
#elif (MEMDRV_CFG_DEV0_TYPE == 0)  | (MEMDRV_CFG_DEV1_TYPE == 0)
memdrv_err_t r_memdrv_rspi_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t                 channel = r_memdrv_get_drv_ch(devno);
    rspi_command_word_t     spcmd_cmd_word;
    rspi_err_t              ret_drv = RSPI_SUCCESS;
    memdrv_err_t            ret_memdrv = MEMDRV_SUCCESS;
    rspi_cmd_trans_mode_t   mode;

    uint32_t                bound_cnt = 0;
    uint8_t *               pdsrc = p_memdrv_info->p_data;
    uint32_t                txcnt = p_memdrv_info->cnt;

    uint32_t                rem_txcnt = txcnt / RSPI_EXCHG_MAX_COUNT;
    uint32_t                mod_txcnt = txcnt % RSPI_EXCHG_MAX_COUNT;


    if (0 != ((uint32_t)(p_memdrv_info->p_data) & MEMDRV_ADDR_BOUNDARY))
    {
        bound_cnt = (0x00000004 - ((uint32_t)(p_memdrv_info->p_data) & MEMDRV_ADDR_BOUNDARY));
        txcnt = bound_cnt;
        mode.transfer_mode = RSPI_TRANS_MODE_SW;
#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
        ret_drv = R_RSPI_Control(g_rspi_handle,
                                 RSPI_CMD_SET_TRANS_MODE,
                                 (void *)&mode);
        if (RSPI_SUCCESS != ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }

        ret_memdrv = r_memdrv_rspi_write_data(channel, txcnt, pdsrc, spcmd_cmd_word);
        if (MEMDRV_SUCCESS != ret_memdrv)
        {
            return MEMDRV_ERR_OTHER;
        }
        pdsrc = (uint8_t *)(p_memdrv_info->p_data + bound_cnt);
        txcnt = p_memdrv_info->cnt - bound_cnt;
    }
    if (0 != (txcnt & 0xfffffffc))
    {
        txcnt = (txcnt & 0xfffffffc);
        mode.transfer_mode = r_memdrv_rspi_set_tran_mode(devno);

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_DATA_CMD;
#endif /* (__ICCRX__) */
#endif
        ret_drv = R_RSPI_Control(g_rspi_handle,
                                 RSPI_CMD_SET_TRANS_MODE,
                                 (void *)&mode);
        if (RSPI_SUCCESS != ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_enable_tx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_tx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        r_rspi_exchg(pdsrc, txcnt);
        /* Turn 32 digits into 16 digits */
        if (rem_txcnt >= 1)
        {
            /* WAIT_LOOP */
            while(rem_txcnt)
            {
                ret_memdrv = r_memdrv_rspi_write_data(channel, RSPI_EXCHG_MAX_COUNT, pdsrc, spcmd_cmd_word);
                if (MEMDRV_SUCCESS != ret_memdrv)
                {
                    return ret_memdrv;
                }
                pdsrc = (uint8_t *)(pdsrc + RSPI_EXCHG_MAX_COUNT);
                rem_txcnt--;
            }
            ret_memdrv = r_memdrv_rspi_write_data(channel, mod_txcnt, pdsrc, spcmd_cmd_word);
        }
        else
        {
            ret_memdrv = r_memdrv_rspi_write_data(channel, txcnt, pdsrc, spcmd_cmd_word);
        }

        if (MEMDRV_SUCCESS != ret_memdrv)
        {
            r_rspi_exchg(pdsrc, txcnt);
            return MEMDRV_ERR_OTHER;
        }
        r_rspi_exchg(pdsrc, txcnt);
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_disable_tx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_tx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        pdsrc = (uint8_t *)(pdsrc + txcnt);
    }
    if (0 != ((uint32_t)(p_memdrv_info->cnt - bound_cnt) & MEMDRV_CHK_MULT_OF_4))
    {
        txcnt = (uint32_t)((p_memdrv_info->cnt - bound_cnt) & MEMDRV_CHK_MULT_OF_4);
        mode.transfer_mode = RSPI_TRANS_MODE_SW;

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
        ret_drv = R_RSPI_Control(g_rspi_handle,
                                 RSPI_CMD_SET_TRANS_MODE,
                                 (void *)&mode);
        if (RSPI_SUCCESS != ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }


        ret_memdrv = r_memdrv_rspi_write_data(channel, txcnt, pdsrc, spcmd_cmd_word);
        if (MEMDRV_SUCCESS != ret_memdrv)
        {
            return MEMDRV_ERR_OTHER;
        }
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_tx_data() */
#endif
/************************************************************************************************
* Function Name: r_memdrv_rspi_rx
* Description  : Receives data for status register and Read ID.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_HARD                        ;   Hardware error
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_rspi_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t                 channel = r_memdrv_get_drv_ch(devno);
    memdrv_err_t            ret_memdrv = MEMDRV_SUCCESS;
    rspi_command_word_t     spcmd_cmd_word;

#if ((MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)   | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC))
    rspi_cmd_trans_mode_t   mode;
    rspi_err_t              ret_drv = RSPI_SUCCESS;
#endif

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif

#if ((MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)  | \
    (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)   | \
    (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC))
    mode.transfer_mode = RSPI_TRANS_MODE_SW;
    ret_drv = R_RSPI_Control(g_rspi_handle,
                             RSPI_CMD_SET_TRANS_MODE,
                             (void *)&mode);
    if (RSPI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
#endif

    ret_memdrv = r_memdrv_rspi_read_data(channel, p_memdrv_info->cnt, p_memdrv_info->p_data, spcmd_cmd_word);
    if (MEMDRV_SUCCESS != ret_memdrv)
    {
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_rx() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_rx_data
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
#if (MEMDRV_CFG_DEV0_TYPE ==1)  | (MEMDRV_CFG_DEV1_TYPE ==1)
memdrv_err_t r_memdrv_rspi_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    rspi_command_word_t     spcmd_cmd_word;
    rspi_err_t     ret_drv = RSPI_SUCCESS;
    memdrv_err_t            ret_memdrv = MEMDRV_SUCCESS;
    rspi_cmd_trans_mode_t   mode;

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_DATA_CMD;
#endif /* (__ICCRX__) */
#endif

    /* Set transfer mode. */
    if (MEMDRV_ADDR_BOUNDARY & (uint32_t)p_memdrv_info->p_data)
    {
        /* If the buffer address is not 4 bytes alignment, force to perform the CPU transfer. */
        mode.transfer_mode = RSPI_TRANS_MODE_SW;
#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
    }
    else if (MEMDRV_CHK_MULT_OF_4 & p_memdrv_info->cnt)
    {
        /* If the data counter is not multiple of 4, force to perform the CPU transfer. */
        mode.transfer_mode = RSPI_TRANS_MODE_SW;
#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
    }
    else
    {
        mode.transfer_mode = r_memdrv_rspi_set_tran_mode(devno);
    }
    ret_drv = R_RSPI_Control(g_rspi_handle,
                             RSPI_CMD_SET_TRANS_MODE,
                             (void *)&mode);
    if (RSPI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    
    r_rspi_exchg(p_memdrv_info->p_data, p_memdrv_info->cnt);
    ret_memdrv = r_memdrv_rspi_read_data(channel, p_memdrv_info->cnt, p_memdrv_info->p_data, spcmd_cmd_word);
    if (MEMDRV_SUCCESS != ret_memdrv)
    {
        r_rspi_exchg(p_memdrv_info->p_data, p_memdrv_info->cnt);
        return MEMDRV_ERR_OTHER;
    }
    r_rspi_exchg(p_memdrv_info->p_data, p_memdrv_info->cnt);

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_rx_data() */
#elif (MEMDRV_CFG_DEV0_TYPE ==0)  | (MEMDRV_CFG_DEV1_TYPE ==0)
memdrv_err_t r_memdrv_rspi_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    rspi_command_word_t     spcmd_cmd_word;
    rspi_err_t     ret_drv = RSPI_SUCCESS;
    memdrv_err_t   ret_memdrv = MEMDRV_SUCCESS;
    rspi_cmd_trans_mode_t   mode;

    uint32_t                bound_cnt = 0;
    uint8_t *               pdest = p_memdrv_info->p_data;
    uint32_t                rxcnt = p_memdrv_info->cnt;
    uint32_t                rem_rxcnt = rxcnt / RSPI_EXCHG_MAX_COUNT;
    uint32_t                mod_rxcnt = rxcnt % RSPI_EXCHG_MAX_COUNT;

    if (0 != ((uint32_t)(p_memdrv_info->p_data) & MEMDRV_ADDR_BOUNDARY))
    {
        bound_cnt = (0x00000004 - ((uint32_t)(p_memdrv_info->p_data) & MEMDRV_ADDR_BOUNDARY));
        rxcnt = bound_cnt;
        mode.transfer_mode = RSPI_TRANS_MODE_SW;
#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
        ret_drv = R_RSPI_Control(g_rspi_handle,
                                 RSPI_CMD_SET_TRANS_MODE,
                                 (void *)&mode);
        if (RSPI_SUCCESS != ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }

        ret_memdrv = r_memdrv_rspi_read_data(channel, rxcnt, pdest, spcmd_cmd_word);
        if (MEMDRV_SUCCESS != ret_memdrv)
        {
            return MEMDRV_ERR_OTHER;
        }

        pdest = (uint8_t *)(p_memdrv_info->p_data + bound_cnt);
        rxcnt = p_memdrv_info->cnt - bound_cnt;
    }
    if (0 != (rxcnt & 0xfffffffc))
    {
        rxcnt = (rxcnt & 0xfffffffc);
        mode.transfer_mode = r_memdrv_rspi_set_tran_mode(devno);

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_DATA_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_DATA_CMD;
#endif /* (__ICCRX__) */
#endif
        ret_drv = R_RSPI_Control(g_rspi_handle,
                                 RSPI_CMD_SET_TRANS_MODE,
                                 (void *)&mode);
        if (RSPI_SUCCESS != ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }
        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_enable_rx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_rx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_enable_rx_data_dmac(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_enable_rx_data_dtc(devno,
                                                     p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        r_rspi_exchg(pdest, rxcnt);
        /* Turn 32 digits into 16 digits */
        if (rem_rxcnt >= 1)
        {
            /* WAIT_LOOP */
            while(rem_rxcnt)
            {
                ret_memdrv = r_memdrv_rspi_read_data(channel, RSPI_EXCHG_MAX_COUNT, pdest, spcmd_cmd_word);
                if (MEMDRV_SUCCESS != ret_memdrv)
                {
                    return ret_memdrv;
                }
                pdest = (uint8_t *)(pdest + RSPI_EXCHG_MAX_COUNT);
                rem_rxcnt--;
            }
            ret_memdrv = r_memdrv_rspi_read_data(channel, mod_rxcnt, pdest, spcmd_cmd_word);
        }
        else
        {
            ret_memdrv = r_memdrv_rspi_read_data(channel, rxcnt, pdest, spcmd_cmd_word);
        }

        if (MEMDRV_SUCCESS != ret_memdrv)
        {
            r_rspi_exchg(pdest, rxcnt);
            return MEMDRV_ERR_OTHER;
        }
        r_rspi_exchg(pdest, rxcnt);

        if (MEMDRV_DEV0 == devno)
        {
            if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        else
        {
            if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
            {
                if (r_memdrv_rspi_disable_rx_data_dmac(devno,
                                                       p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
            {
                if (r_memdrv_rspi_disable_rx_data_dtc(devno,
                                                      p_memdrv_info) != MEMDRV_SUCCESS)
                {
                    return MEMDRV_ERR_OTHER;
                }
            }
            else
            {
                /* CPU transfer */
                R_BSP_NOP();
            }
        }
        pdest = (uint8_t *)(pdest + rxcnt);
    }
    if (0 != ((uint32_t)(p_memdrv_info->cnt - bound_cnt) & MEMDRV_CHK_MULT_OF_4))
    {
        rxcnt = ((uint32_t)(p_memdrv_info->cnt - bound_cnt) & MEMDRV_CHK_MULT_OF_4);
        mode.transfer_mode = RSPI_TRANS_MODE_SW;

#if RSPI_LITTLE_ENDIAN == 1
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
#else
    #if defined (__ICCRX__)
    uint16_t temp;
    spcmd_cmd_word.word[0] = MEMDRV_TRNS_CMD;
    temp = (spcmd_cmd_word.word[0] >> 8);
    temp |= (spcmd_cmd_word.word[0] << 8);
    spcmd_cmd_word.word[0] = temp;
    #else
    spcmd_cmd_word.word[1] = MEMDRV_TRNS_CMD;
#endif /* (__ICCRX__) */
#endif
        ret_drv = R_RSPI_Control(g_rspi_handle,
                                 RSPI_CMD_SET_TRANS_MODE,
                                 (void *)&mode);
        if (RSPI_SUCCESS != ret_drv)
        {
            return MEMDRV_ERR_OTHER;
        }

        ret_memdrv = r_memdrv_rspi_read_data(channel, rxcnt, pdest, spcmd_cmd_word);
        if (MEMDRV_SUCCESS != ret_memdrv)
        {
            return MEMDRV_ERR_OTHER;
        }
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_rx_data() */
#endif
/*******************************************************************************
* Function Name: r_memdrv_rspi_1ms_interval
* Description  : 1ms Interval Timer call function.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void r_memdrv_rspi_1ms_interval(void)
{
    uint8_t channel = 0;
    /* WAIT_LOOP */
    for (channel = 0; channel < RSPI_MAX_CHANNELS; channel++)
    {
        if (gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_FLG] != 0)
        {
            gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_COUNT]++;
        }
    }
} /* End of function r_memdrv_rspi_1ms_interval() */

/************************************************************************************************
* Function Name: r_memdrv_rspi_set_tran_mode
* Description  : Sets transfer mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value : RSPI_TRANS_MODE_SW
*              : RSPI_TRANS_MODE_DMAC
*              : RSPI_TRANS_MODE_DTC
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static rspi_str_tranmode_t r_memdrv_rspi_set_tran_mode(uint8_t devno)
{
    rspi_str_tranmode_t mode = RSPI_TRANS_MODE_SW;

#if (MEMDRV_CFG_DEV0_INCLUDED == 1)
    if (MEMDRV_DEV0 == devno)
    {
        if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
        {
            mode = RSPI_TRANS_MODE_DMAC;
        }
        else if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)
        {
            mode = RSPI_TRANS_MODE_DTC;
        }
        else
        {
            mode = RSPI_TRANS_MODE_SW;
        }
    }
#endif /* (MEMDRV_CFG_DEV0_INCLUDED == 1) */
#if (MEMDRV_CFG_DEV1_INCLUDED == 1)
    if (MEMDRV_DEV1 == devno)
    {
        if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
        {
            mode = RSPI_TRANS_MODE_DMAC;
        }
        else if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
        {
            mode = RSPI_TRANS_MODE_DTC;
        }
        else
        {
            mode = RSPI_TRANS_MODE_SW;
        }
    }
#endif /* (MEMDRV_CFG_DEV1_INCLUDED == 1) */

    return mode;
} /* End of function r_memdrv_rspi_set_tran_mode() */
    
/*******************************************************************************
* Function Name: r_memdrv_rspi_start_timer
* Description  : Sets timer for checking timeout and start timer.
* Arguments    : channel -
*                    Which channel to use
*              : msec -
*                    timeout(msec order)
* Return Value : none
*******************************************************************************/
static void r_memdrv_rspi_start_timer(uint8_t channel, uint32_t msec)
{
    gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_FLG] = 1;
    gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_COUNT] = 0;
    gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_MAX_COUNT] = (uint32_t)(msec);
} /* End of function r_memdrv_rspi_start_timer() */

/*******************************************************************************
* Function Name: r_memdrv_rspi_check_timer
* Description  : Checks timeout to set r_memdrv_rspi_start_timer function.
*              : If timeout,return MEMDRV_ERR_HARD. 
*              : In the case of others, return MEMDRV_SUCCESS.
* Arguments    : channel -
*                    Which channel to use
* Return Value : MEMDRV_SUCCESS -
*                    Successful operation
*                MEMDRV_ERR_HARD -
*                    Hardware error
*******************************************************************************/
static memdrv_err_t r_memdrv_rspi_check_timer(uint8_t channel)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    uint32_t gs_memdrv_rspi_timer_cnt_out1 = gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_COUNT];
    uint32_t gs_memdrv_rspi_timer_cnt_out2 = gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_MAX_COUNT];

    /* ---- Check compare ---- */
    if (gs_memdrv_rspi_timer_cnt_out1 >= gs_memdrv_rspi_timer_cnt_out2)
    {
        ret_drv = MEMDRV_ERR_HARD;
    }
    else
    {
        ret_drv = MEMDRV_SUCCESS;
    }

    return ret_drv;
} /* End of function r_memdrv_rspi_check_timer() */

/*******************************************************************************
* Function Name: r_memdrv_rspi_end_timer
* Description  : Stops timer to set r_memdrv_rspi_start_timer function.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
static void r_memdrv_rspi_end_timer(uint8_t channel)
{
    gs_memdrv_rspi_timer_cnt_out[channel][RSPI_TIMER_CH_FLG] = 0;
} /* End of function r_memdrv_rspi_end_timer() */

/*******************************************************************************
* Function Name: r_memdrv_rspi_wait
* Description  : Waits for reception completion.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : MEMDRV_SUCCESS -
*                    Successful operation
*                MEMDRV_ERR_HARD -
*                    Hardware error
*******************************************************************************/
static memdrv_err_t r_memdrv_rspi_wait(uint8_t channel, uint32_t size)
{
    memdrv_err_t    ret_drv  = MEMDRV_SUCCESS;
    uint32_t               time = RSPI_TIMER_MIN_TIME;     // 100ms        

    /* ---- Check DMAC/DTC transfer end.  --- */
    /* Timeout value depends on transfer size. 
       Minimum time is 100ms. When the data count exceeds 10 sectors (1 sector = 512 bytes),
       increase 10ms time for 1 sector increase.*/
    if ((RSPI_SECTOR_SIZE * 10) < size)
    {
        time = (time + (((size / RSPI_SECTOR_SIZE) - 10) * 10));
        if (0x0fffffff < time)
        {
            time = 0x0fffffff;
        }
    }

    /* Start timer. */
    r_memdrv_rspi_start_timer(channel, time);
    /* WAIT_LOOP */
    while (1)
    {
        /* Check timeout. */
        if (MEMDRV_ERR_HARD == r_memdrv_rspi_check_timer(channel))
        {
            ret_drv = MEMDRV_ERR_HARD;
            break;
        }
        if (false == g_transfer_busy)
        {
            break;
        }
    }

    r_memdrv_rspi_end_timer(channel);

    return ret_drv;
}  /* End of function r_memdrv_rspi_wait() */

/*****************************************************************************
* Function Name: r_memdrv_rspi_callback
* Description  : This is an example of an RSPI callback function.
* Arguments    : p_data - pointer to RSPI event code data.
* Return Value : None
******************************************************************************/
static void r_memdrv_rspi_callback(void *p_data)
{
    callback_event = (*(rspi_callback_data_t *)p_data).event_code;
    g_transfer_busy = false;

#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
    R_RSPI_IntSpriIerClear(g_rspi_handle);
    R_RSPI_IntSptiIerClear(g_rspi_handle);
    if (0 == g_rspi_handle->channel)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0) | \
    ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH0)
        RSPI0.SPCR.BIT.SPE   = 0;  // Disable RSPI.
#if RSPI_CFG_REQUIRE_LOCK == 1
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPI0));
#endif
#endif
    }
    else if (1 == g_rspi_handle->channel)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1) | \
    ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH1)
    {
        RSPI1.SPCR.BIT.SPE   = 0;  // Disable RSPI.
#if RSPI_CFG_REQUIRE_LOCK == 1
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPI1));
#endif
#endif
    }
    else if (2 == g_rspi_handle->channel)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2) | \
    ((MEMDRV_CFG_DEV1_MODE_DRVR_CH & MEMDRV_DRVR_MASK_CH) == MEMDRV_DRVR_CH2)
        RSPI2.SPCR.BIT.SPE   = 0;  // Disable RSPI.
#if RSPI_CFG_REQUIRE_LOCK == 1
        R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_RSPI2));
#endif
#endif
    }
    else
    {
    }
#endif
} /* End of function r_memdrv_rspi_callback() */

/*****************************************************************************
* Function Name: rspi_init_ports
* Description  : This function initializes the port pins associated with
*                RSPI channel 0 using the macros defined in iodefine.h.
* Arguments    : None
* Return Value : None
******************************************************************************/
static void rspi_init_ports(void)
{
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH0)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)   && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH0)))
    R_RSPI_PinSet_RSPI0();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH1)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)   && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH1)))
    R_RSPI_PinSet_RSPI1();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH2)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)   && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_RSPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH2)))
    R_RSPI_PinSet_RSPI2();
#endif
} /* End of function rspi_init_ports() */

/*****************************************************************************
* Function Name: r_memdrv_rspi_write_data
* Description  : Transmits data to a SPI  device.
* Arguments    : uint8_t         channel                ;   Device No. (MEMDRV_DEVn)
*              : uint16_t        count                  ;   Indicates the number of data words to be transferred.
*              : uint8_t         * pdata                ;   Buffer pointer
*              : rspi_command_word_t  cmd               ;   Consisting of all the RSPI command register settings.
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
******************************************************************************/
static memdrv_err_t r_memdrv_rspi_write_data(uint8_t channel,
                                             uint16_t count,
                                             uint8_t * pdata,
                                             rspi_command_word_t cmd)
{
    rspi_err_t     ret_drv = RSPI_SUCCESS;

    g_transfer_busy = true;

#if RSPI_LITTLE_ENDIAN == 1
    if (MEMDRV_TRNS_DATA_CMD == cmd.word[0])
    {
        count = count >> 2;
    }
#else
    if (MEMDRV_TRNS_DATA_CMD == cmd.word[1])
    {
        count = count >> 2;
    }
#endif
    ret_drv = R_RSPI_Write(g_rspi_handle,
                           cmd,
                           pdata,
                           count);
    if   (RSPI_SUCCESS != ret_drv)
    {
        return MEMDRV_ERR_OTHER;
    }

    /* Wait for transmission completion. */
    if (0 > r_memdrv_rspi_wait(channel, count))
    {
        /* ---- Disable RSPI transmission. ---- */
        R_RSPI_IntSptiIerClear(g_rspi_handle);
        R_RSPI_IntSpriIerClear(g_rspi_handle);
        g_rspi_handle->channel = channel;
        if (RSPI_SUCCESS != R_RSPI_Close(g_rspi_handle))
        {
            return MEMDRV_ERR_OTHER;
        }
        return MEMDRV_ERR_OTHER;
    }

    if (RSPI_EVT_TRANSFER_COMPLETE != callback_event)
    {
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_write_data() */

/*****************************************************************************
* Function Name: r_memdrv_rspi_read_data
* Description  : Receives data from a SPI device.
* Arguments    : uint8_t         channel                ;   Device No. (MEMDRV_DEVn)
*              : uint16_t        count                  ;   Indicates the number of data words to be transferred.
*              : uint8_t         * pdata                ;   Buffer pointer
*              : rspi_command_word_t  cmd               ;   Consisting of all the RSPI command register settings.
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
******************************************************************************/
static memdrv_err_t r_memdrv_rspi_read_data(uint8_t channel,
                                            uint16_t count,
                                            uint8_t * pdata,
                                            rspi_command_word_t cmd)
{
    rspi_err_t     ret_drv = RSPI_SUCCESS;

    g_transfer_busy = true;

#if RSPI_LITTLE_ENDIAN == 1
    if (MEMDRV_TRNS_DATA_CMD == cmd.word[0])
    {
        count = count >> 2;
    }
#else
    if (MEMDRV_TRNS_DATA_CMD == cmd.word[1])
    {
        count = count >> 2;
    }
#endif
    ret_drv = R_RSPI_Read(g_rspi_handle,
                           cmd,
                           pdata,
                           count);
    if   (RSPI_SUCCESS != ret_drv)
    {
        return MEMDRV_ERR_OTHER;
    }

    /* Wait for transmission completion. */
    if (0 > r_memdrv_rspi_wait(channel, count))
    {
        /* ---- Disable RSPI transmission. ---- */
        R_RSPI_IntSptiIerClear(g_rspi_handle);
        R_RSPI_IntSpriIerClear(g_rspi_handle);
        g_rspi_handle->channel = channel;
        if (RSPI_SUCCESS != R_RSPI_Close(g_rspi_handle))
        {
            return MEMDRV_ERR_OTHER;
        }
        return MEMDRV_ERR_OTHER;
    }

    if (RSPI_EVT_TRANSFER_COMPLETE != callback_event)
    {
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_rspi_read_data() */

/*******************************************************************************
* Function Name: r_rspi_exchg
* Description  : Exchanges data according to endian for DMAC or DTC.
* Arguments    : * p_data -
*                    Pointer of data
*                size -
*                    Size of data
* Return Value : RSPI_SMSTR_SUCCESS -
*                    Successful operation
*******************************************************************************/
static memdrv_err_t r_rspi_exchg(uint8_t * p_data, uint16_t size)
{
#if (RSPI_LITTLE_ENDIAN)
    uint32_t                * p_dataadr;

    p_dataadr = (uint32_t *)p_data;
    size  = size - (size % 4);
    do
    {
        *p_dataadr = R_BSP_REVL(*p_dataadr);
        p_dataadr  += (RSPI_TRAN_SIZE/sizeof(uint32_t));
        size       -= RSPI_TRAN_SIZE;
    }
    while(0 != size);      /* WAIT_LOOP */
#endif /* (RSPI_LITTLE_ENDIAN)  */
    return MEMDRV_SUCCESS;
} /* End of function r_rspi_exchg() */

#else
memdrv_err_t r_memdrv_rspi_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_open() */
memdrv_err_t r_memdrv_rspi_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_close() */
memdrv_err_t r_memdrv_rspi_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_disable() */
memdrv_err_t r_memdrv_rspi_disable_tx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_disable_tx_data() */
memdrv_err_t r_memdrv_rspi_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_disable_rx_data() */
memdrv_err_t r_memdrv_rspi_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_enable() */
memdrv_err_t r_memdrv_rspi_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_enable_tx_data() */
memdrv_err_t r_memdrv_rspi_enable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_enable_rx_data() */
memdrv_err_t r_memdrv_rspi_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_tx() */
memdrv_err_t r_memdrv_rspi_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_tx_data() */
memdrv_err_t r_memdrv_rspi_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_rx() */
memdrv_err_t r_memdrv_rspi_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_rspi_rx_data() */
void r_memdrv_rspi_1ms_interval(void)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
} /* End of function r_memdrv_rspi_1ms_interval() */
#endif  /* ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI)) || \
           ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI))

/* End of File */
