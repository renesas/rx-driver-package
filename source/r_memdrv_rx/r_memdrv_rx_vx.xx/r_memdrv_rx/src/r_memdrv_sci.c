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
* Copyright (C) 2018(2019)  Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : MEMDRV software
* File Name    : r_memdrv_sci.c
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
*              : 22.11.2019 1.02     Modified the interface type of transmit and receive data.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */

/* Check driver interface. */
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_SCI_SPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_SCI_SPI))
#include "r_pinset.h"
/************************************************************************************************
Macro definitions
*************************************************************************************************/
#define SCI_TIMER_CH_FLG                 (0)
#define SCI_TIMER_CH_COUNT               (1)
#define SCI_TIMER_CH_MAX_COUNT           (2)
#define SCI_TIMER_MIN_TIME               (100)       /* 100ms */
#define SCI_SECTOR_SIZE                  (512)       /* 1 sector size */
#define SCI_EXCHG_MAX_COUNT              (65532)

#define SCI_MAX_CHANNELS                 (SCI_NUM_CH)
/************************************************************************************************
Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
static volatile bool  transfer_busy = false;
static sci_hdl_t sci_handle;
static volatile uint32_t memdrv_sci_timer_cnt_out[SCI_MAX_CHANNELS][SCI_TIMER_CH_MAX_COUNT + 1];

static void                 r_memdrv_sci_start_timer(uint8_t channel, uint32_t msec);
static memdrv_err_t         r_memdrv_sci_check_timer(uint8_t channel);
static void                 r_memdrv_sci_end_timer(uint8_t channel);
static memdrv_err_t         r_memdrv_sci_wait(uint8_t channel, uint32_t size);
static void                 r_memdrv_sci_callback(void *p_data);
static void                 sci_init_ports(void);

/************************************************************************************************
* Function Name: r_memdrv_sci_open
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
memdrv_err_t r_memdrv_sci_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    sci_cfg_t config;
    
    if (MEMDRV_DEV0 == devno)
    {
        config.sspi.bit_rate = (uint32_t)MEMDRV_CFG_DEV0_BR;
    }
    else if (MEMDRV_DEV1 == devno)
    {
        config.sspi.bit_rate = (uint32_t)MEMDRV_CFG_DEV1_BR;
    }

    config.sspi.spi_mode = SCI_SPI_MODE_3;
    config.sspi.msb_first = true;
    config.sspi.invert_data = false;
    config.sspi.int_priority = 4;
    
    sci_init_ports();

    /* Casting to a uint8_t type is valid. */
    if (SCI_SUCCESS != R_SCI_Open(channel, 
                                  SCI_MODE_SSPI,
                                  &config,
                                  &r_memdrv_sci_callback,
                                  &sci_handle))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_open() */

/************************************************************************************************
* Function Name: r_memdrv_sci_close
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
memdrv_err_t r_memdrv_sci_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    if (SCI_SUCCESS != R_SCI_Close(sci_handle))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_close() */

/************************************************************************************************
* Function Name: r_memdrv_sci_disable
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
memdrv_err_t r_memdrv_sci_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_disable() */

/************************************************************************************************
* Function Name: r_memdrv_sci_disable_tx_data
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
memdrv_err_t r_memdrv_sci_disable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_disable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_sci_disable_rx_data
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
memdrv_err_t r_memdrv_sci_disable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_disable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_sci_enable
* Description  : Enables I/O driver.
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
memdrv_err_t r_memdrv_sci_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    sci_baud_t baud;
    baud.pclk = BSP_PCLKB_HZ;
    baud.rate = MEMDRV_CFG_DEV0_BR;
    /* Casting to a uint32_t type is valid. */
    if (R_SCI_Control(sci_handle,SCI_CMD_CHANGE_BAUD,(void *)&baud) != SCI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_enable() */

/************************************************************************************************
* Function Name: r_memdrv_sci_enable_tx_data
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
memdrv_err_t r_memdrv_sci_enable_tx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info)
{
    sci_baud_t baud;
    baud.pclk = BSP_PCLKB_HZ;
    baud.rate = MEMDRV_CFG_DEV0_BR_WRITE_DATA;
    /* Casting to a uint32_t type is valid. */
    if (R_SCI_Control(sci_handle,SCI_CMD_CHANGE_BAUD,(void *)&baud) != SCI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_enable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_sci_enable_rx_data
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
memdrv_err_t r_memdrv_sci_enable_rx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info)
{
    sci_baud_t baud;
    baud.pclk = BSP_PCLKB_HZ;
    baud.rate = MEMDRV_CFG_DEV0_BR_READ_DATA;
    /* Casting to a uint32_t type is valid. */
    if (R_SCI_Control(sci_handle, SCI_CMD_CHANGE_BAUD, (void *)&baud) != SCI_SUCCESS)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_enable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_sci_tx
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
memdrv_err_t r_memdrv_sci_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    sci_err_t   ret_drv = SCI_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    transfer_busy = true;
    ret_drv = R_SCI_Send(sci_handle, p_memdrv_info->p_data, p_memdrv_info->cnt);
    if (SCI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    
    /* Wait for transmission completion. */
    if (0 > r_memdrv_sci_wait(channel, p_memdrv_info->cnt))
    {
        /* ---- Disable SCI transmission. ---- */
        if (SCI_SUCCESS != R_SCI_Close(sci_handle))
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
    
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_tx() */

/************************************************************************************************
* Function Name: r_memdrv_sci_tx_data
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
memdrv_err_t r_memdrv_sci_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    sci_err_t   ret_drv = SCI_SUCCESS;
    uint8_t     channel = r_memdrv_get_drv_ch(devno);
    uint32_t    cnt     = p_memdrv_info->cnt;
    uint32_t    rem_txcnt = cnt / SCI_EXCHG_MAX_COUNT;
    uint32_t    mod_txcnt = cnt % SCI_EXCHG_MAX_COUNT;

    transfer_busy = true;
    /* Turn 32 digits into 16 digits */
    if (rem_txcnt >= 1)
    {
        /* WAIT_LOOP */
        while(rem_txcnt)
        {
            ret_drv = R_SCI_Send(sci_handle, p_memdrv_info->p_data, SCI_EXCHG_MAX_COUNT);
            if (MEMDRV_SUCCESS != ret_drv)
            {
                return MEMDRV_ERR_OTHER;
            }
            p_memdrv_info->p_data = (uint8_t *)(p_memdrv_info->p_data + SCI_EXCHG_MAX_COUNT);
            rem_txcnt--;
        }
        ret_drv = R_SCI_Send(sci_handle, p_memdrv_info->p_data, mod_txcnt);
    }
    else
    {
        ret_drv = R_SCI_Send(sci_handle, p_memdrv_info->p_data, cnt);
    }

    if (SCI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    
    /* Wait for transmission completion. */
    if (0 > r_memdrv_sci_wait(channel, p_memdrv_info->cnt))
    {
        /* ---- Disable SCI transmission. ---- */
        if (SCI_SUCCESS != R_SCI_Close(sci_handle))
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
    
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_sci_rx
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
memdrv_err_t r_memdrv_sci_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    sci_err_t   ret_drv = SCI_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    
    transfer_busy = true;
    ret_drv = R_SCI_Receive(sci_handle, p_memdrv_info->p_data, p_memdrv_info->cnt);
    if (SCI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    
    /* Wait for transmission completion. */
    if (0 > r_memdrv_sci_wait(channel, p_memdrv_info->cnt))
    {
        /* ---- Disable SCI transmission. ---- */
        if (SCI_SUCCESS != R_SCI_Close(sci_handle))
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
    
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_rx() */

/************************************************************************************************
* Function Name: r_memdrv_sci_rx_data
* Description  : Receives data using the single mode.
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
memdrv_err_t r_memdrv_sci_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    sci_err_t   ret_drv = SCI_SUCCESS;
    uint8_t     channel = r_memdrv_get_drv_ch(devno);
    uint32_t    cnt     = p_memdrv_info->cnt;
    uint32_t    rem_rxcnt = cnt / SCI_EXCHG_MAX_COUNT;
    uint32_t    mod_rxcnt = cnt % SCI_EXCHG_MAX_COUNT;
    
    transfer_busy = true;
    /* Turn 32 digits into 16 digits */
    if (rem_rxcnt >= 1)
    {
        /* WAIT_LOOP */
        while(rem_rxcnt)
        {
            ret_drv = R_SCI_Receive(sci_handle, p_memdrv_info->p_data, SCI_EXCHG_MAX_COUNT);
            if (MEMDRV_SUCCESS != ret_drv)
            {
                return MEMDRV_ERR_OTHER;
            }
            p_memdrv_info->p_data = (uint8_t *)(p_memdrv_info->p_data + SCI_EXCHG_MAX_COUNT);
            rem_rxcnt--;
        }
        ret_drv = R_SCI_Receive(sci_handle, p_memdrv_info->p_data, mod_rxcnt);
    }
    else
    {
        ret_drv = R_SCI_Receive(sci_handle, p_memdrv_info->p_data, cnt);
    }
    if (SCI_SUCCESS != ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    /* Wait for transmission completion. */
    if (0 > r_memdrv_sci_wait(channel, p_memdrv_info->cnt))
    {
        /* ---- Disable SCI transmission. ---- */
        if (SCI_SUCCESS != R_SCI_Close(sci_handle))
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
    
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_rx_data() */

/*******************************************************************************
* Function Name: r_memdrv_sci_1ms_interval
* Description  : 1ms Interval Timer call function.
* Arguments    : none
* Return Value : none
*******************************************************************************/
memdrv_err_t r_memdrv_sci_1ms_interval(void)
{
    uint8_t channel = 0;
    /* WAIT_LOOP */
    for (channel = 0; channel < SCI_MAX_CHANNELS; channel++)
    {
        if (memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_FLG] != 0)
        {
            memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_COUNT]++;
        }
    }

    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_sci_1ms_interval() */

/*******************************************************************************
* Function Name: r_memdrv_sci_start_timer
* Description  : Sets timer for checking timeout and start timer.
* Arguments    : channel -
*                    Which channel to use
*              : msec -
*                    timeout(msec order)
* Return Value : none
*******************************************************************************/
static void r_memdrv_sci_start_timer(uint8_t channel, uint32_t msec)
{
    memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_FLG] = 1;
    memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_COUNT] = 0;
    memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_MAX_COUNT] = (uint32_t)(msec);
} /* End of function r_memdrv_sci_start_timer() */

/*******************************************************************************
* Function Name: r_memdrv_sci_check_timer
* Description  : Checks timeout to set r_memdrv_sci_start_timer function.
*              : If timeout,return MEMDRV_ERR_HARD. 
*              : In the case of others, return MEMDRV_SUCCESS.
* Arguments    : channel -
*                    Which channel to use
* Return Value : MEMDRV_SUCCESS -
*                    Successful operation
*                MEMDRV_ERR_HARD -
*                    Hardware error
*******************************************************************************/
static memdrv_err_t r_memdrv_sci_check_timer(uint8_t channel)
{
    memdrv_err_t ret = MEMDRV_SUCCESS;
    uint32_t memdrv_sci_timer_cnt_out1 = memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_COUNT];
    uint32_t memdrv_sci_timer_cnt_out2 = memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_MAX_COUNT];

    /* ---- Check compare ---- */
    if (memdrv_sci_timer_cnt_out1 >= memdrv_sci_timer_cnt_out2)
    {
        ret = MEMDRV_ERR_HARD;
    }
    else
    {
        ret = MEMDRV_SUCCESS;
    }

    return ret;
} /* End of function r_memdrv_sci_check_timer() */

/*******************************************************************************
* Function Name: r_memdrv_sci_end_timer
* Description  : Stops timer to set r_memdrv_sci_start_timer function.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
static void r_memdrv_sci_end_timer(uint8_t channel)
{
    memdrv_sci_timer_cnt_out[channel][SCI_TIMER_CH_FLG] = 0;
} /* End of function r_memdrv_sci_end_timer() */

/*******************************************************************************
* Function Name: r_memdrv_sci_wait
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
static memdrv_err_t r_memdrv_sci_wait(uint8_t channel, uint32_t size)
{
    memdrv_err_t    ret  = MEMDRV_SUCCESS;
    uint32_t               time = SCI_TIMER_MIN_TIME;     // 100ms        

    /* ---- Check DMAC/DTC transfer end.  --- */
    /* Timeout value depends on transfer size. 
       Minimum time is 100ms. When the data count exceeds 10 sectors (1 sector = 512 bytes),
       increase 10ms time for 1 sector increase.*/
    if ((SCI_SECTOR_SIZE * 10) < size)
    {
        time = (time + (((size / SCI_SECTOR_SIZE) - 10) * 10));
        if (0x0fffffff < time)
        {
            time = 0x0fffffff;
        }
    }

    /* Start timer. */
    r_memdrv_sci_start_timer(channel, time);
    /* WAIT_LOOP */
    while (1)
    {
        /* Check timeout. */
        if (MEMDRV_ERR_HARD == r_memdrv_sci_check_timer(channel))
        {
            ret = MEMDRV_ERR_HARD;
            break;
        }
        if (false == transfer_busy)
        {
            break;
        }
    }

    r_memdrv_sci_end_timer(channel);

    return ret;
} /* End of function r_memdrv_sci_wait() */

/*****************************************************************************
* Function Name: r_memdrv_sci_callback
* Description  : This is an example of an SCI callback function.
* Arguments    : p_data - pointer to SCI event code data.
* Return Value : None
******************************************************************************/
static void r_memdrv_sci_callback(void *p_data)
{
    transfer_busy = false;
} /* End of function r_memdrv_SCI_callback() */

/*****************************************************************************
* Function Name: sci_init_ports
* Description  : This function initializes the port pins associated with
*                SCI channel 6 using the macros defined in iodefine.h.
* Arguments    : None
* Return Value : None
******************************************************************************/
static void sci_init_ports(void)
{
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH0)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH0)))
    R_SCI_PinSet_SCI0();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH1)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH1)))
    R_SCI_PinSet_SCI1();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH2)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH2)))
    R_SCI_PinSet_SCI2();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH3)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH3)))
    R_SCI_PinSet_SCI3();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH4)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH4)))
    R_SCI_PinSet_SCI4();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH5)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH5)))
    R_SCI_PinSet_SCI5();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH6)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH6)))
    R_SCI_PinSet_SCI6();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH7)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH7)))
    R_SCI_PinSet_SCI7();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH8)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH8)))
    R_SCI_PinSet_SCI8();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH9)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)      && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH9)))
    R_SCI_PinSet_SCI9();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH10)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)     && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH10)))
    R_SCI_PinSet_SCI10();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH11)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)     && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH11)))
    R_SCI_PinSet_SCI11();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && \
      (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH12)) || ((MEMDRV_CFG_DEV1_INCLUDED == 1)     && \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_SCI_SPI) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH12)))
    R_SCI_PinSet_SCI12();
#endif
} /* End of function sci_init_ports() */

#else
memdrv_err_t r_memdrv_sci_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_open() */
memdrv_err_t r_memdrv_sci_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_close() */
memdrv_err_t r_memdrv_sci_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_disable() */
memdrv_err_t r_memdrv_sci_disable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_disable_tx_data() */
memdrv_err_t r_memdrv_sci_disable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_disable_rx_data() */
memdrv_err_t r_memdrv_sci_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_enable() */
memdrv_err_t r_memdrv_sci_enable_tx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_enable_tx_data() */
memdrv_err_t r_memdrv_sci_enable_rx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_enable_rx_data() */
memdrv_err_t r_memdrv_sci_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_tx() */
memdrv_err_t r_memdrv_sci_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_tx_data() */
memdrv_err_t r_memdrv_sci_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_rx() */
memdrv_err_t r_memdrv_sci_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_rx_data() */
memdrv_err_t r_memdrv_sci_1ms_interval(void)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_sci_1ms_interval() */
#endif  /* ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_SCI_SPI)) || \
           ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_SCI_SPI))

/* End of File */
