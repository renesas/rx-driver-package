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
* File Name    : r_memdrv_dtc.c
* Version      : 1.00
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
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */

/* Check driver interface. */
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)

/************************************************************************************************
Macro definitions
*************************************************************************************************/

/************************************************************************************************
Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/
dtc_transfer_data_t g_memdrv_dtc_info[MEMDRV_DEV_NUM][MEMDRV_INDEX_DTC_NUM];

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/

/************************************************************************************************
* Function Name: r_memdrv_dtc_open
* Description  : Opens DTC FIT module.
* Arguments    : None
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dtc_open(void)
{
    if (R_DTC_Open() != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dtc_open() */

/************************************************************************************************
* Function Name: r_memdrv_dtc_close
* Description  : Resets I/O driver.
* Arguments    : None
* Return Value : MEMDRV_SUCCESS                       ;   Successful operation
*              : MEMDRV_ERR_OTHER                     ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dtc_close(void)
{
    if (R_DTC_Close() != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dtc_close() */

/************************************************************************************************
* Function Name: r_memdrv_dtc_enable
* Description  : Enables DTC FIT module.
* Arguments    : st_memdrv_dtc_info_t * p_memdrv_dtc_info  ;   DTC transfer information
*              :    uint32_t    drv                     ;   Driver Type
*              :    uint32_t    src_adr                 ;   Source address
*              :    uint32_t    dst_adr                 ;   Destination address
*              :    uint32_t    block_size              ;   Block size
*              :    uint32_t    cnt                     ;   Transfer count
*              :    dtc_transfer_data_cfg_t * p_dtc_cfg ;   DTC config
*              :    dtc_activation_source_t act_source  ;   DTC act source
*              :    dtc_transfer_data_t * p_dtc_transfer;   DTC transfer data
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dtc_enable(st_memdrv_dtc_info_t * p_memdrv_dtc_info)
{
    uint32_t chain_transfer_nr;
    dtc_stat_t stat;
    dtc_cmd_arg_t args;

    chain_transfer_nr = 0;

    if (R_DTC_Create(p_memdrv_dtc_info->act_source,
                     p_memdrv_dtc_info->p_dtc_transfer,
                     p_memdrv_dtc_info->p_dtc_cfg,
                     chain_transfer_nr) != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    args.act_src = p_memdrv_dtc_info->act_source;
    args.chain_transfer_nr = 0;
    if (R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, &stat, &args) != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DTC_Control(DTC_CMD_DTC_START, &stat, &args) != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dtc_enable() */

/************************************************************************************************
* Function Name: r_memdrv_dtc_disable
* Description  : Disables I/O driver.
* Arguments    : st_memdrv_dtc_info_t * p_memdrv_dtc_info  ;   DTC transfer information
*              :    uint32_t    drv                     ;   Driver Type
*              :    uint32_t    src_adr                 ;   Source address
*              :    uint32_t    dst_adr                 ;   Destination address
*              :    uint32_t    block_size              ;   Block size
*              :    uint32_t    cnt                     ;   Transfer count
*              :    dtc_transfer_data_cfg_t * p_dtc_cfg ;   DTC config
*              :    dtc_activation_source_t act_source  ;   DTC act source
*              :    dtc_transfer_data_t * p_dtc_transfer;   DTC transfer data
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*              : MEMDRV_ERR_OTHER                       ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_dtc_disable(st_memdrv_dtc_info_t * p_memdrv_dtc_info)
{
    dtc_stat_t stat;
    dtc_cmd_arg_t args;

    args.act_src = p_memdrv_dtc_info->act_source;
    args.chain_transfer_nr = 0;
    if (R_DTC_Control(DTC_CMD_DTC_STOP, &stat, &args) != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    if (R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, &stat, &args) != DTC_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_dtc_disable() */

/************************************************************************************************
* Function Name: r_memdrv_dtc_set_tx
* Description  : Receives data using the single mode.
* Arguments    : st_memdrv_dtc_info_t * p_memdrv_dtc_info  ;   DTC transfer information
*              :    uint32_t    drv                     ;   Driver Type
*              :    uint32_t    src_adr                 ;   Source address
*              :    uint32_t    dst_adr                 ;   Destination address
*              :    uint32_t    block_size              ;   Block size
*              :    uint32_t    cnt                     ;   Transfer count
*              :    dtc_transfer_data_cfg_t * p_dtc_cfg ;   DTC config
*              :    dtc_activation_source_t act_source  ;   DTC act source
*              :    dtc_transfer_data_t * p_dtc_transfer;   DTC transfer data
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_memdrv_dtc_set_tx(st_memdrv_dtc_info_t * p_memdrv_dtc_info)
{
    switch (p_memdrv_dtc_info->drv)
    {
        case MEMDRV_DRVR_RX_FIT_RSPI:
        {
            p_memdrv_dtc_info->p_dtc_cfg->transfer_mode = DTC_TRANSFER_MODE_NORMAL;
            p_memdrv_dtc_info->p_dtc_cfg->transfer_count= (p_memdrv_dtc_info->cnt / 4);
            p_memdrv_dtc_info->p_dtc_cfg->block_size    = p_memdrv_dtc_info->block_size;
        }
        break;
        case MEMDRV_DRVR_RX_FIT_QSPI_SMSTR:
        {
            p_memdrv_dtc_info->p_dtc_cfg->transfer_mode = DTC_TRANSFER_MODE_BLOCK;
            p_memdrv_dtc_info->p_dtc_cfg->transfer_count= (p_memdrv_dtc_info->cnt / 16);
            p_memdrv_dtc_info->p_dtc_cfg->block_size    = 4;
        }
        break;
        default:
        {
            p_memdrv_dtc_info->p_dtc_cfg->transfer_mode = DTC_TRANSFER_MODE_NORMAL;
            p_memdrv_dtc_info->p_dtc_cfg->transfer_count= (p_memdrv_dtc_info->cnt / 4);
            p_memdrv_dtc_info->p_dtc_cfg->block_size    = p_memdrv_dtc_info->block_size;
        }
        break;
    }
    
    p_memdrv_dtc_info->p_dtc_cfg->data_size             = DTC_DATA_SIZE_LWORD;
    p_memdrv_dtc_info->p_dtc_cfg->src_addr_mode         = DTC_SRC_ADDR_INCR;
    p_memdrv_dtc_info->p_dtc_cfg->chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;
    p_memdrv_dtc_info->p_dtc_cfg->chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    p_memdrv_dtc_info->p_dtc_cfg->response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    p_memdrv_dtc_info->p_dtc_cfg->repeat_block_side     = DTC_REPEAT_BLOCK_DESTINATION;
    p_memdrv_dtc_info->p_dtc_cfg->dest_addr_mode        = DTC_DES_ADDR_FIXED;
    p_memdrv_dtc_info->p_dtc_cfg->source_addr           = p_memdrv_dtc_info->src_adr;
    p_memdrv_dtc_info->p_dtc_cfg->dest_addr             = p_memdrv_dtc_info->dst_adr;
#if defined(BSP_MCU_RX65N)
    p_memdrv_dtc_info->p_dtc_cfg->writeback_disable     = DTC_WRITEBACK_ENABLE;
    p_memdrv_dtc_info->p_dtc_cfg->sequence_end          = DTC_SEQUENCE_TRANSFER_CONTINUE;
    p_memdrv_dtc_info->p_dtc_cfg->refer_index_table_enable = DTC_REFER_INDEX_TABLE_DISABLE;
    p_memdrv_dtc_info->p_dtc_cfg->disp_add_enable       = DTC_SRC_ADDR_DISP_ADD_DISABLE;
#endif /* defined(BSP_MCU_RX65N) */

    return;
} /* End of function r_memdrv_dtc_set_tx() */

/************************************************************************************************
* Function Name: r_memdrv_dtc_set_rx
* Description  : Transmits data using the single mode.
* Arguments    : st_memdrv_dtc_info_t * p_memdrv_dtc_info  ;   DTC transfer information
*              :    uint32_t    drv                     ;   Driver Type
*              :    uint32_t    src_adr                 ;   Source address
*              :    uint32_t    dst_adr                 ;   Destination address
*              :    uint32_t    block_size              ;   Block size
*              :    uint32_t    cnt                     ;   Transfer count
*              :    dtc_transfer_data_cfg_t * p_dtc_cfg ;   DTC config
*              :    dtc_activation_source_t act_source  ;   DTC act source
*              :    dtc_transfer_data_t * p_dtc_transfer;   DTC transfer data
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_memdrv_dtc_set_rx(st_memdrv_dtc_info_t * p_memdrv_dtc_info)
{
    switch (p_memdrv_dtc_info->drv)
    {
        case MEMDRV_DRVR_RX_FIT_RSPI:
        {
            p_memdrv_dtc_info->p_dtc_cfg->transfer_mode = DTC_TRANSFER_MODE_NORMAL;
            p_memdrv_dtc_info->p_dtc_cfg->transfer_count= (p_memdrv_dtc_info->cnt / 4);
            p_memdrv_dtc_info->p_dtc_cfg->block_size    = p_memdrv_dtc_info->block_size;
        }
        break;
        case MEMDRV_DRVR_RX_FIT_QSPI_SMSTR:
        {
            p_memdrv_dtc_info->p_dtc_cfg->transfer_mode = DTC_TRANSFER_MODE_BLOCK;
            p_memdrv_dtc_info->p_dtc_cfg->transfer_count= (p_memdrv_dtc_info->cnt / 16);
            p_memdrv_dtc_info->p_dtc_cfg->block_size    = 4;
        }
        break;
        default:
        {
            p_memdrv_dtc_info->p_dtc_cfg->transfer_mode = DTC_TRANSFER_MODE_NORMAL;
            p_memdrv_dtc_info->p_dtc_cfg->transfer_count= (p_memdrv_dtc_info->cnt / 4);
            p_memdrv_dtc_info->p_dtc_cfg->block_size    = p_memdrv_dtc_info->block_size;
        }
        break;
    }
    
    p_memdrv_dtc_info->p_dtc_cfg->data_size             = DTC_DATA_SIZE_LWORD;
    p_memdrv_dtc_info->p_dtc_cfg->src_addr_mode         = DTC_SRC_ADDR_FIXED;
    p_memdrv_dtc_info->p_dtc_cfg->chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;
    p_memdrv_dtc_info->p_dtc_cfg->chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    p_memdrv_dtc_info->p_dtc_cfg->response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    p_memdrv_dtc_info->p_dtc_cfg->repeat_block_side     = DTC_REPEAT_BLOCK_SOURCE;
    p_memdrv_dtc_info->p_dtc_cfg->dest_addr_mode        = DTC_DES_ADDR_INCR;
    p_memdrv_dtc_info->p_dtc_cfg->source_addr           = p_memdrv_dtc_info->src_adr;
    p_memdrv_dtc_info->p_dtc_cfg->dest_addr             = p_memdrv_dtc_info->dst_adr;
#if defined(BSP_MCU_RX65N)
    p_memdrv_dtc_info->p_dtc_cfg->writeback_disable     = DTC_WRITEBACK_ENABLE;
    p_memdrv_dtc_info->p_dtc_cfg->sequence_end          = DTC_SEQUENCE_TRANSFER_CONTINUE;
    p_memdrv_dtc_info->p_dtc_cfg->refer_index_table_enable = DTC_REFER_INDEX_TABLE_DISABLE;
    p_memdrv_dtc_info->p_dtc_cfg->disp_add_enable       = DTC_SRC_ADDR_DISP_ADD_DISABLE;
#endif /* defined(BSP_MCU_RX65N) */

    return;
} /* End of function r_memdrv_dtc_set_rx() */

#endif  /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

/* End of File */
