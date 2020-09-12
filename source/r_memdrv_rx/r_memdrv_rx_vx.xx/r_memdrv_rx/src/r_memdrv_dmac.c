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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : MEMDRV software
* File Name    : r_memdrv_dmac.c
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
*              : 22.11.2019 1.02     Fixed coding style.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */

/* Check driver interface. */
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)

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
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
static void r_memdrv_dmac_callback_tx_dev0(void);
static void r_memdrv_dmac_callback_rx_dev0(void);
#endif
#if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
static void r_memdrv_dmac_callback_tx_dev1(void);
static void r_memdrv_dmac_callback_rx_dev1(void);
#endif

/************************************************************************************************
* Function Name: r_memdrv_dmac_open
* Description  : Opens DMAC FIT module.
* Arguments    : None
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dmac_open(void)
{
    R_DMACA_Init();

#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
    /* Tx setting */
    if (R_DMACA_Open(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Callback(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx,
                             (void *)r_memdrv_dmac_callback_tx_dev0) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Enable(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx,
                           MEMDRV_CFG_DEV0_DMAC_INT_PRL_Tx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    /* Rx setting */
    if (R_DMACA_Open(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Callback(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx,
                             (void *)r_memdrv_dmac_callback_rx_dev0) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Enable(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx,
                           MEMDRV_CFG_DEV0_DMAC_INT_PRL_Rx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

#endif /* MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC */
#if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    /* Tx setting */
    if (R_DMACA_Open(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Callback(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx,
                             (void *)r_memdrv_dmac_callback_tx_dev1) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Enable(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx,
                           MEMDRV_CFG_DEV1_DMAC_INT_PRL_Tx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    /* Rx setting */
    if (R_DMACA_Open(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Callback(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx,
                             (void *)r_memdrv_dmac_callback_rx_dev1) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Int_Enable(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx,
                           MEMDRV_CFG_DEV1_DMAC_INT_PRL_Rx) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

#endif /* MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC */

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dmac_open() */

#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
/************************************************************************************************
* Function Name: r_memdrv_dmac_callback_tx_dev0
* Description  : Callback function for DMAC transmission for device 0.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_memdrv_dmac_callback_tx_dev0(void)
{
    uint8_t channel = r_memdrv_get_drv_ch(MEMDRV_DEV0);
    dmaca_stat_t stat_dmac;

    if (R_DMACA_Control(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx,
                        DMACA_CMD_STATUS_GET, 
                        &stat_dmac) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return;
    }

    if (true == (stat_dmac.dtif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx,
                            DMACA_CMD_DTIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
        R_MEMDRV_ClearDMACFlagTx(channel);
    }

    if (true == (stat_dmac.esif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx,
                            DMACA_CMD_ESIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
    }
    return;
} /* End of function r_memdrv_dmac_callback_tx_dev0() */

/************************************************************************************************
* Function Name: r_memdrv_dmac_callback_rx_dev0
* Description  : Callback function for DMAC reception for device 0.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_memdrv_dmac_callback_rx_dev0(void)
{
    uint8_t channel = r_memdrv_get_drv_ch(MEMDRV_DEV0);
    dmaca_stat_t stat_dmac;

    if (R_DMACA_Control(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx,
                        DMACA_CMD_STATUS_GET, 
                        &stat_dmac) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return;
    }

    if (true == (stat_dmac.dtif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx,
                            DMACA_CMD_DTIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
        R_MEMDRV_ClearDMACFlagRx(channel);
    }

    if (true == (stat_dmac.esif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx,
                            DMACA_CMD_ESIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
    }
    return;
} /* End of function r_memdrv_dmac_callback_rx_dev0() */
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC) */

#if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
/************************************************************************************************
* Function Name: r_memdrv_dmac_callback_tx_dev1
* Description  : Callback function for DMAC transmission for device 1.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_memdrv_dmac_callback_tx_dev1(void)
{
    uint8_t channel = r_memdrv_get_drv_ch(MEMDRV_DEV0);
    dmaca_stat_t stat_dmac;

    if (R_DMACA_Control(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx,
                        DMACA_CMD_STATUS_GET, 
                        &stat_dmac) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return;
    }

    if (true == (stat_dmac.dtif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx,
                            DMACA_CMD_DTIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
        R_MEMDRV_ClearDMACFlagTx(channel);
    }

    if (true == (stat_dmac.esif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx,
                            DMACA_CMD_ESIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
    }
    return;
} /* End of function r_memdrv_dmac_callback_tx_dev1() */

/************************************************************************************************
* Function Name: r_memdrv_dmac_callback_rx_dev1
* Description  : Callback function for DMAC reception for device 1.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_memdrv_dmac_callback_rx_dev1(void)
{
    uint8_t channel = r_memdrv_get_drv_ch(MEMDRV_DEV0);
    dmaca_stat_t stat_dmac;

    if (R_DMACA_Control(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx,
                        DMACA_CMD_STATUS_GET, 
                        &stat_dmac) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return;
    }

    if (true == (stat_dmac.dtif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx,
                            DMACA_CMD_DTIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
        R_MEMDRV_ClearDMACFlagRx(channel);
    }

    if (true == (stat_dmac.esif_stat))
    {
        if (R_DMACA_Control(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx,
                            DMACA_CMD_ESIF_STATUS_CLR,
                            &stat_dmac) != DMACA_SUCCESS)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return;
        }
    }
    return;
} /* End of function r_memdrv_dmac_callback_rx_dev1() */
#endif /* (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

/************************************************************************************************
* Function Name: r_memdrv_dmac_close
* Description  : Resets I/O driver.
* Arguments    : None
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dmac_close(void)
{
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)
    if (DMACA_SUCCESS < R_DMACA_Close(MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (DMACA_SUCCESS < R_DMACA_Close(MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

#endif
#if (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
    if (DMACA_SUCCESS < R_DMACA_Close(MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (DMACA_SUCCESS < R_DMACA_Close(MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

#endif

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dmac_close() */

/************************************************************************************************
* Function Name: r_memdrv_dmac_enable
* Description  : Enables DMAC FIT module.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_dmac_info_t * p_memdrv_dmac_info  ;   DMAC transfer information
*              :    uint32_t        addr                ;       Address
*              :    uint32_t        cnt                 ;       Number of bytes
*              :    uint8_t       * p_data              ;       Buffer pointer
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dmac_enable(st_memdrv_dmac_info_t * p_memdrv_dmac_info)
{
    if (R_DMACA_Create(p_memdrv_dmac_info->channel,
                       p_memdrv_dmac_info->p_dmac_cfg) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DMACA_Control(p_memdrv_dmac_info->channel,
                        DMACA_CMD_ENABLE,
                        p_memdrv_dmac_info->p_stat_dmac) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dmac_enable() */

/************************************************************************************************
* Function Name: r_memdrv_dmac_disable
* Description  : Disables I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dmac_disable(st_memdrv_dmac_info_t * p_memdrv_dmac_info)
{
    if (R_DMACA_Control(p_memdrv_dmac_info->channel,
                        DMACA_CMD_DISABLE,
                        p_memdrv_dmac_info->p_stat_dmac) != DMACA_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dmac_disable() */

/************************************************************************************************
* Function Name: r_memdrv_dmac_set_tx
* Description  : Receives data using the single mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : uint32_t        rxcnt                  ;   Number of bytes to be read
*              : uint8_t       * p_data                 ;   Read data storage buffer pointer
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_HARD                        ;   Hardware error
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_memdrv_dmac_set_tx(st_memdrv_dmac_info_t * p_memdrv_dmac_info)
{
    switch (p_memdrv_dmac_info->drv)
    {
        case MEMDRV_DRVR_RX_FIT_RSPI:
        {
            p_memdrv_dmac_info->p_dmac_cfg->transfer_mode = DMACA_TRANSFER_MODE_NORMAL;
            p_memdrv_dmac_info->p_dmac_cfg->transfer_count= (p_memdrv_dmac_info->cnt / 4);
            p_memdrv_dmac_info->p_dmac_cfg->block_size    = p_memdrv_dmac_info->block_size;
        }
        break;
        case MEMDRV_DRVR_RX_FIT_QSPI_SMSTR:
        {
            p_memdrv_dmac_info->p_dmac_cfg->transfer_mode = DMACA_TRANSFER_MODE_BLOCK;
            p_memdrv_dmac_info->p_dmac_cfg->transfer_count= (p_memdrv_dmac_info->cnt / 16);
            p_memdrv_dmac_info->p_dmac_cfg->block_size    = 4;
        }
        break;
        default:
        {
            p_memdrv_dmac_info->p_dmac_cfg->transfer_mode = DMACA_TRANSFER_MODE_NORMAL;
            p_memdrv_dmac_info->p_dmac_cfg->transfer_count= (p_memdrv_dmac_info->cnt / sizeof(uint32_t));
            p_memdrv_dmac_info->p_dmac_cfg->block_size    = p_memdrv_dmac_info->block_size;
       }
        break;
    }

    p_memdrv_dmac_info->p_dmac_cfg->repeat_block_side     = DMACA_REPEAT_BLOCK_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->data_size             = DMACA_DATA_SIZE_LWORD;

    p_memdrv_dmac_info->p_dmac_cfg->act_source            = p_memdrv_dmac_info->act_source;
    p_memdrv_dmac_info->p_dmac_cfg->request_source        = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    p_memdrv_dmac_info->p_dmac_cfg->dtie_request          = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    p_memdrv_dmac_info->p_dmac_cfg->esie_request          = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->rptie_request         = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->sarie_request         = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->darie_request         = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    
    p_memdrv_dmac_info->p_dmac_cfg->src_addr_mode         = DMACA_SRC_ADDR_INCR;
    p_memdrv_dmac_info->p_dmac_cfg->des_addr_mode         = DMACA_DES_ADDR_FIXED;
    
    p_memdrv_dmac_info->p_dmac_cfg->src_addr_repeat_area  = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    p_memdrv_dmac_info->p_dmac_cfg->des_addr_repeat_area  = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    p_memdrv_dmac_info->p_dmac_cfg->interrupt_sel         = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    
    p_memdrv_dmac_info->p_dmac_cfg->p_src_addr            = (void *)p_memdrv_dmac_info->src_adr;
    p_memdrv_dmac_info->p_dmac_cfg->p_des_addr            = (void *)p_memdrv_dmac_info->dst_adr;

    return;
} /* End of function r_memdrv_dmac_set_tx() */

/************************************************************************************************
* Function Name: r_memdrv_dmac_set_rx
* Description  : Transmits data using the single mode.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : uint32_t        txcnt                  ;   Number of bytes to be write
*              : uint8_t       * p_data                 ;   Write data storage buffer pointer
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_HARD                        ;   Hardware error
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_memdrv_dmac_set_rx(st_memdrv_dmac_info_t * p_memdrv_dmac_info)
{
    switch (p_memdrv_dmac_info->drv)
    {
        case MEMDRV_DRVR_RX_FIT_RSPI:
        {
            p_memdrv_dmac_info->p_dmac_cfg->transfer_mode = DMACA_TRANSFER_MODE_NORMAL;
            p_memdrv_dmac_info->p_dmac_cfg->transfer_count= (p_memdrv_dmac_info->cnt / 4);
            p_memdrv_dmac_info->p_dmac_cfg->block_size    = p_memdrv_dmac_info->block_size;
        }
        break;
        case MEMDRV_DRVR_RX_FIT_QSPI_SMSTR:
        {
            p_memdrv_dmac_info->p_dmac_cfg->transfer_mode = DMACA_TRANSFER_MODE_BLOCK;
            p_memdrv_dmac_info->p_dmac_cfg->transfer_count= (p_memdrv_dmac_info->cnt / 16);
            p_memdrv_dmac_info->p_dmac_cfg->block_size    = 4;
        }
        break;
        default:
        {
            p_memdrv_dmac_info->p_dmac_cfg->transfer_mode = DMACA_TRANSFER_MODE_NORMAL;
            p_memdrv_dmac_info->p_dmac_cfg->transfer_count= (p_memdrv_dmac_info->cnt / 4);
            p_memdrv_dmac_info->p_dmac_cfg->block_size    = p_memdrv_dmac_info->block_size;
        }
        break;
    }
    
    p_memdrv_dmac_info->p_dmac_cfg->repeat_block_side     = DMACA_REPEAT_BLOCK_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->data_size             = DMACA_DATA_SIZE_LWORD;

    p_memdrv_dmac_info->p_dmac_cfg->act_source            = p_memdrv_dmac_info->act_source;
    p_memdrv_dmac_info->p_dmac_cfg->request_source        = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    p_memdrv_dmac_info->p_dmac_cfg->dtie_request          = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    p_memdrv_dmac_info->p_dmac_cfg->esie_request          = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->rptie_request         = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->sarie_request         = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    p_memdrv_dmac_info->p_dmac_cfg->darie_request         = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    
    p_memdrv_dmac_info->p_dmac_cfg->src_addr_mode         = DMACA_SRC_ADDR_FIXED;
    p_memdrv_dmac_info->p_dmac_cfg->des_addr_mode         = DMACA_DES_ADDR_INCR;
    
    p_memdrv_dmac_info->p_dmac_cfg->src_addr_repeat_area  = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    p_memdrv_dmac_info->p_dmac_cfg->des_addr_repeat_area  = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    p_memdrv_dmac_info->p_dmac_cfg->interrupt_sel         = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    
    p_memdrv_dmac_info->p_dmac_cfg->p_src_addr            = (void *)p_memdrv_dmac_info->src_adr;
    p_memdrv_dmac_info->p_dmac_cfg->p_des_addr            = (void *)p_memdrv_dmac_info->dst_adr;

    return;
} /* End of function r_memdrv_dmac_set_rx() */

#endif  /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

/* End of File */
