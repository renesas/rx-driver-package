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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : MEMDRV software
* File Name    : r_memdrv_qspix.c
* Version      : 1.05
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
*              : 30.10.2021 1.00     Initial Release
*              : 16.03.2023 1.05     Added support for QSPIX Memory Mapped Mode.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_if.h"                      /* MEMDRV driver I/F definitions                  */
#include "r_memdrv_rx_config.h"                  /* MEMDRV driver Configuration definitions        */
#include "./src/r_memdrv_rx_private.h"           /* MEMDRV driver Private module definitions       */

/* Check driver interface. */
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && ((MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
     (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM))) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && ((MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
     (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM)))
#include "r_pinset.h"

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#define QSPIX_TIMER_CH_FLG          (0)
#define QSPIX_TIMER_CH_COUNT        (1)
#define QSPIX_TIMER_CH_MAX_COUNT    (2)

#define QSPIX_MAX_CHANNELS              (QSPIX_NUM_CH)
/************************************************************************************************
Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/
static volatile uint32_t gs_memdrv_qspix_timer_cnt_out[QSPIX_NUM_CH][QSPIX_TIMER_CH_MAX_COUNT + 1];
/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
static void qspix_init_ports(void);
static void r_memdrv_qspix_callback(void *p_cbdat);
static uint8_t qspix_div_cal(uint8_t devno);
static uint8_t qspix_SPOCR_regset(uint8_t devno , qspix_cfg_t * cfg);
static bool enable_mapped_mode[] =
{
#if (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM)
true,
#else
false,
#endif
#if (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM)
true
#else
false
#endif
};

/************************************************************************************************
* Function Name: r_memdrv_qspix_open
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
memdrv_err_t r_memdrv_qspix_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspix_cfg_t cfg;
    uint8_t channel = r_memdrv_get_drv_ch(devno);
    uint8_t div_result = 0;

    /* Set read mode */
    cfg.read_mode = QSPIX_READ_MODE_STANDARD;

    /* Set Slave Select Extension */
    cfg.slave_select_extension = QSPIX_DO_NOT_EXTEND_QSSL;

    /* Set Prefetch Function Enable */
    cfg.prefetch_function = QSPIX_PREFETCH_ENABLE;

    /* Set Clock Mode */
    cfg.clock_mode = QSPIX_SPI_MODE_3;

    /* Set Data Output Duration Extension */
    cfg.data_output_select = QSPIX_DO_NOT_EXTEND;

    /* Set Special Read Instruction Select */
    cfg.special_instruction_select = QSPIX_DEFAULT_INSTRUCTION_CODE;

    /* Set Slave Select High Width Setting */
    cfg.slave_select_high_width = QSPIX_1_CYCLE_QSPCLK;

    /* Set Slave Select Hold Time Setting */
    cfg.slave_select_hold_time = QSPIX_RELEASE_QSSL_0_5_QSPCLK;

    /* Set Slave Select Setup Time Setting */
    cfg.slave_select_setup_time = QSPIX_OUTPUT_QSSL_0_5_QSPCLK;

    /* Set Address Size */
    cfg.address_size = QSPIX_3_BYTES;

    /* Set Instruction with 4-Byte Address Enable */
    cfg.instruction_4_Byte_address = QSPIX_INSTRUCTION_4_BYTE_ADDRESS_DISABLE;

    /* Set Number of Dummy Cycle */
    cfg.dummy_clocks = QSPIX_DEFAULT_DUMMY_CYCLES;

    /* Set Clock Divisor Select */
    div_result = qspix_SPOCR_regset(devno,&cfg);

    if (1 == div_result)
    {
        return MEMDRV_ERR_PARAM;
    }

    /* Set SPI protocol */
    if (MEMDRV_MODE_SINGLE == p_memdrv_info->io_mode)
    {
        cfg.protocol = QSPIX_EXTENDED_SPI_PROTOCOL;
    }
    else if (MEMDRV_MODE_DUAL == p_memdrv_info->io_mode)
    {
        cfg.protocol = QSPIX_DUAL_SPI_PROTOCOL;
    }
    else
    {
        cfg.protocol = QSPIX_QUAD_SPI_PROTOCOL;
    }
    /* Set WP Pin Control */
    cfg.WP_pin_control = QSPIX_HIGH_LEVEL;
    /* Set callback function */
    cfg.p_callback = r_memdrv_qspix_callback;
    if (QSPIX_SUCCESS != R_QSPIX_Open(channel, &cfg))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }

    qspix_init_ports();
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_open() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_close
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
memdrv_err_t r_memdrv_qspix_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    if (QSPIX_SUCCESS != R_QSPIX_Close(channel))
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_close() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_disable
* Description  : Disables I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspix_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_disable() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_disable_tx_data
* Description  : Disables I/O driver for writing data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspix_disable_tx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_disable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_disable_rx_data
* Description  : Disables I/O driver for reading data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspix_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_disable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_enable
* Description  : Enables I/O driver.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspix_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_enable() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_enable_tx_data
* Description  : Enables I/O driver for writing data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspix_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_enable_tx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_enable_rx_data
* Description  : Enables I/O driver for reading data.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
*              : st_memdrv_info_t * p_memdrv_info
*              :    uint32_t    cnt                     ;   Number of bytes
*              :    uint8_t   * p_data                  ;   Buffer pointer
*              :    uint8_t     io_mode                 ;   Single/Dual/Quad
*              :    uint8_t     rsv[3]                  ;   Reserved
* Return Value : MEMDRV_SUCCESS                         ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
memdrv_err_t r_memdrv_qspix_enable_rx_data(uint8_t devno,
                                        st_memdrv_info_t * p_memdrv_info)
{
    return MEMDRV_SUCCESS;
} /* End of function r_memdrv_qspix_enable_rx_data() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_tx
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
memdrv_err_t r_memdrv_qspix_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspix_err_t   ret_drv = QSPIX_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    if((true == p_memdrv_info->read_in_memory_mapped) && (true == enable_mapped_mode[devno]))
    {
        return MEMDRV_SUCCESS;
    }
    else
    {
        ret_drv = R_QSPIX_Write_Indirect(channel,
                                         p_memdrv_info->p_data,
                                         p_memdrv_info->cnt,
                                         p_memdrv_info->read_after_write);
        if   (QSPIX_SUCCESS == ret_drv)
        {
            return MEMDRV_SUCCESS;
        }
        else if (QSPIX_ERR_HW == ret_drv)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_HARD;
        }
        else
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
} /* End of function r_memdrv_qspix_tx() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_tx_data
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

memdrv_err_t r_memdrv_qspix_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspix_err_t   ret_drv = QSPIX_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    ret_drv = R_QSPIX_Write_Indirect(channel,
                                     p_memdrv_info->p_data,
                                     p_memdrv_info->cnt,
                                     p_memdrv_info->read_after_write);
    if   (QSPIX_SUCCESS == ret_drv)
    {
        return MEMDRV_SUCCESS;
    }
    else if (QSPIX_ERR_HW == ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_HARD;
    }
    else
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
}
/************************************************************************************************
* Function Name: r_memdrv_qspix_rx
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
memdrv_err_t r_memdrv_qspix_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspix_err_t   ret_drv = QSPIX_SUCCESS;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    ret_drv = R_QSPIX_Read_Indirect(channel,
                                     p_memdrv_info->p_data,
                                     p_memdrv_info->cnt);
    if   (QSPIX_SUCCESS == ret_drv)
    {
        return MEMDRV_SUCCESS;
    }
    else if (QSPIX_ERR_HW == ret_drv)
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_HARD;
    }
    else
    {
        R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
        return MEMDRV_ERR_OTHER;
    }
} /* End of function r_memdrv_qspix_rx() */

/************************************************************************************************
* Function Name: r_memdrv_qspix_rx_data
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
memdrv_err_t r_memdrv_qspix_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    qspix_err_t   ret_drv = QSPIX_SUCCESS;
    qspix_protocol_t protocol_ext;
    qspix_address_size_t addr_size;
    uint8_t channel = r_memdrv_get_drv_ch(devno);

    if(true == enable_mapped_mode[devno])
    {
        ret_drv = R_QSPIX_BankSet(channel, (uint8_t)(p_memdrv_info->p_addr/QSPI_DEVICE_OFFSET_ADDRESS));
        if(QSPIX_SUCCESS != ret_drv)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }

        /* Update start address at QSPI memory space */
        p_memdrv_info->p_addr = p_memdrv_info->p_addr - ((p_memdrv_info->p_addr/QSPI_DEVICE_OFFSET_ADDRESS) * QSPI_DEVICE_OFFSET_ADDRESS);

        switch(p_memdrv_info->io_mode)
        {
            case MEMDRV_MODE_SINGLE:
                protocol_ext = QSPIX_EXTENDED_SPI_PROTOCOL;
            break;
            case MEMDRV_MODE_DUAL:
                protocol_ext = QSPIX_DUAL_SPI_PROTOCOL;
            break;
            case MEMDRV_MODE_QUAD:
                protocol_ext = QSPIX_QUAD_SPI_PROTOCOL;
            break;
            default:
            break;
        }

        switch(p_memdrv_info->addr_size)
        {
            case MEMDRV_ADDR_4BYTES:
                addr_size = QSPIX_4_BYTES;
            break;
            case MEMDRV_ADDR_3BYTES:
                addr_size = QSPIX_3_BYTES;
            break;
            default:
            break;
        }

        ret_drv = R_QSPIX_Read_Memory_Map(channel,
                                     p_memdrv_info->p_data,
                                     p_memdrv_info->p_addr,
                                     protocol_ext,
                                     addr_size,
                                     p_memdrv_info->cnt);

        if   (QSPIX_SUCCESS == ret_drv)
        {
            return MEMDRV_SUCCESS;
        }
        else if (QSPIX_ERR_HW == ret_drv)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_HARD;
        }
        else
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
    else
    {
        ret_drv = R_QSPIX_Read_Indirect(channel,
                                         p_memdrv_info->p_data,
                                         p_memdrv_info->cnt);
        if   (QSPIX_SUCCESS == ret_drv)
        {
            return MEMDRV_SUCCESS;
        }
        else if (QSPIX_ERR_HW == ret_drv)
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_HARD;
        }
        else
        {
            R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
            return MEMDRV_ERR_OTHER;
        }
    }
}

/*******************************************************************************
* Function Name: r_memdrv_qspix_1ms_interval
* Description  : 1ms Interval Timer call function.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void r_memdrv_qspix_1ms_interval(void)
{
    uint8_t channel = 0;
    /* WAIT_LOOP */
    for (channel = 0; channel < QSPIX_MAX_CHANNELS; channel++)
    {
        if (gs_memdrv_qspix_timer_cnt_out[channel][QSPIX_TIMER_CH_FLG] != 0)
        {
            gs_memdrv_qspix_timer_cnt_out[channel][QSPIX_TIMER_CH_COUNT]++;
        }
    }
} /* End of function r_memdrv_qspix_1ms_interval() */

/*****************************************************************************
* Function Name: qspix_init_ports
* Description  : This function initializes the port pins associated with
*                QSPIX channel 0 using the macros defined in iodefine.h.
* Arguments    : None
* Return Value : None
******************************************************************************/
static void qspix_init_ports(void)
{
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && ((MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
      (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_MMM)) && (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH0)) || \
     ((MEMDRV_CFG_DEV1_INCLUDED == 1) && ((MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM)) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH0)))
    R_QSPIX_PinSet_QSPIX0();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && ((MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
      (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_MMM)) && (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH1)) || \
     ((MEMDRV_CFG_DEV1_INCLUDED == 1) && ((MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM)) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH1)))
    R_QSPIX_PinSet_QSPIX1();
#endif
#if (((MEMDRV_CFG_DEV0_INCLUDED == 1) && ((MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
      (MEMDRV_CFG_DEV0_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_MMM)) && (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_DRVR_CH2)) || \
     ((MEMDRV_CFG_DEV1_INCLUDED == 1) && ((MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
      (MEMDRV_CFG_DEV1_MODE_DRVR & MEMDRV_DRVR_RX_FIT_QSPIX_IAM)) && (MEMDRV_CFG_DEV1_MODE_DRVR_CH == MEMDRV_DRVR_CH2)))
    R_QSPIX_PinSet_QSPIX2();
#endif
} /* End of function qspix_init_ports() */

/*****************************************************************************
* Function Name: r_memdrv_qspix_callback
* Description  : This is an example of an QSPIX callback function.
* Arguments    : p_cbdat - Code for handle ROM error access.
* Return Value : None
******************************************************************************/
static void r_memdrv_qspix_callback(void *p_cbdat)
{
    /* Code for handle ROM error access */
    R_BSP_NOP();
} /* End of function r_memdrv_qspix_callback() */

/*****************************************************************************
* Function Name: qspix_div_cal
* Description  : Calculate the clock frequency division ratio.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value :
******************************************************************************/
static uint8_t qspix_div_cal(uint8_t devno)
{
    uint8_t bps_result;
    uint32_t bit_rate = 0;
    uint32_t f = BSP_ICLK_HZ;       // Frequency
    uint32_t bit_rate_cal = 0;
    int32_t n = 0;                  // n term in equation
    int32_t bit_div;                // N term in equation

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

    if ((f / bit_rate) < 4)
    {
        return 0;
    }

    bit_div = (uint32_t)((MEMDRV_TRNS_CMD & 0x00c0)>>2);

    n = ((f >> bit_div) / (uint32_t)bit_rate);
    if ((n > 0) && (n <= 0x30))
    {
        /* Now plug n back into the formula for BPS and check it. */
        bit_rate_cal = (uint32_t)(f / (n << bit_div));

        if (bit_rate_cal > bit_rate)
        {
            n += 1;
            if (n > 0x30)
            {
                return 48;
            }
        }
        bps_result = n;
    }
    else
    {
        return 0;
    }

    return bps_result;

} /* End of function qspix_div_cal() */

/*****************************************************************************
* Function Name: qspix_SPOCR_regset
* Description  : Set the value of the SPOCR register.
* Arguments    : uint8_t         devno                  ;   Device No. (MEMDRV_DEVn)
* Return Value :
******************************************************************************/
static uint8_t qspix_SPOCR_regset(uint8_t devno , qspix_cfg_t * cfg)
{
    switch(qspix_div_cal(devno))
    {
    case 0: return 1;
    break;
    case 4: cfg->clock_divisor = QSPIX_ICLK_DIV_4;
    break;
    case 5: cfg->clock_divisor = QSPIX_ICLK_DIV_5;
    break;
    case 6: cfg->clock_divisor = QSPIX_ICLK_DIV_6;
    break;
    case 7: cfg->clock_divisor = QSPIX_ICLK_DIV_7;
    break;
    case 8: cfg->clock_divisor = QSPIX_ICLK_DIV_8;
    break;
    case 9: cfg->clock_divisor = QSPIX_ICLK_DIV_9;
    break;
    case 10: cfg->clock_divisor = QSPIX_ICLK_DIV_10;
    break;
    case 11: cfg->clock_divisor = QSPIX_ICLK_DIV_11;
    break;
    case 12: cfg->clock_divisor = QSPIX_ICLK_DIV_12;
    break;
    case 13: cfg->clock_divisor = QSPIX_ICLK_DIV_13;
    break;
    case 14: cfg->clock_divisor = QSPIX_ICLK_DIV_14;
    break;
    case 15: cfg->clock_divisor = QSPIX_ICLK_DIV_15;
    break;
    case 16: cfg->clock_divisor = QSPIX_ICLK_DIV_16;
    break;
    case 17: cfg->clock_divisor = QSPIX_ICLK_DIV_17;
    break;
    case 18: cfg->clock_divisor = QSPIX_ICLK_DIV_18;
    break;
    case 19: cfg->clock_divisor = QSPIX_ICLK_DIV_20;
    break;
    case 20: cfg->clock_divisor = QSPIX_ICLK_DIV_20;
    break;
    case 21: cfg->clock_divisor = QSPIX_ICLK_DIV_22;
    break;
    case 22: cfg->clock_divisor = QSPIX_ICLK_DIV_22;
    break;
    case 23: cfg->clock_divisor = QSPIX_ICLK_DIV_24;
    break;
    case 24: cfg->clock_divisor = QSPIX_ICLK_DIV_24;
    break;
    case 25: cfg->clock_divisor = QSPIX_ICLK_DIV_26;
    break;
    case 26: cfg->clock_divisor = QSPIX_ICLK_DIV_26;
    break;
    case 27: cfg->clock_divisor = QSPIX_ICLK_DIV_28;
    break;
    case 28: cfg->clock_divisor = QSPIX_ICLK_DIV_28;
    break;
    case 29: cfg->clock_divisor = QSPIX_ICLK_DIV_30;
    break;
    case 30: cfg->clock_divisor = QSPIX_ICLK_DIV_30;
    break;
    case 31: cfg->clock_divisor = QSPIX_ICLK_DIV_32;
    break;
    case 32: cfg->clock_divisor = QSPIX_ICLK_DIV_32;
    break;
    case 33: cfg->clock_divisor = QSPIX_ICLK_DIV_34;
    break;
    case 34: cfg->clock_divisor = QSPIX_ICLK_DIV_34;
    break;
    case 35: cfg->clock_divisor = QSPIX_ICLK_DIV_36;
    break;
    case 36: cfg->clock_divisor = QSPIX_ICLK_DIV_36;
    break;
    case 37: cfg->clock_divisor = QSPIX_ICLK_DIV_38;
    break;
    case 38: cfg->clock_divisor = QSPIX_ICLK_DIV_38;
    break;
    case 39: cfg->clock_divisor = QSPIX_ICLK_DIV_40;
    break;
    case 40: cfg->clock_divisor = QSPIX_ICLK_DIV_40;
    break;
    case 41: cfg->clock_divisor = QSPIX_ICLK_DIV_42;
    break;
    case 42: cfg->clock_divisor = QSPIX_ICLK_DIV_42;
    break;
    case 43: cfg->clock_divisor = QSPIX_ICLK_DIV_44;
    break;
    case 44: cfg->clock_divisor = QSPIX_ICLK_DIV_44;
    break;
    case 45: cfg->clock_divisor = QSPIX_ICLK_DIV_46;
    break;
    case 46: cfg->clock_divisor = QSPIX_ICLK_DIV_46;
    break;
    case 47: cfg->clock_divisor = QSPIX_ICLK_DIV_48;
    break;
    case 48: cfg->clock_divisor = QSPIX_ICLK_DIV_48;
    break;
    default:
    break;
    }
    return 0;
}/* End of function qspix_SPOCR_regset() */
#else
memdrv_err_t r_memdrv_qspix_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_open() */
memdrv_err_t r_memdrv_qspix_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_close() */
memdrv_err_t r_memdrv_qspix_disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_disable() */
memdrv_err_t r_memdrv_qspix_disable_tx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_disable_tx_data() */
memdrv_err_t r_memdrv_qspix_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_disable_rx_data() */
memdrv_err_t r_memdrv_qspix_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_enable() */
memdrv_err_t r_memdrv_qspix_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_enable_tx_data() */
memdrv_err_t r_memdrv_qspix_enable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_enable_rx_data() */
memdrv_err_t r_memdrv_qspix_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_tx() */
memdrv_err_t r_memdrv_qspix_tx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_tx_data() */
memdrv_err_t r_memdrv_qspix_rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_rx() */
memdrv_err_t r_memdrv_qspix_rx_data(uint8_t devno, st_memdrv_info_t * p_memdrv_info)
{
    R_MEMDRV_Log(MEMDRV_DEBUG_ERR_ID, (uint32_t)MEMDRV_ERR_SUB, __LINE__);
    return MEMDRV_ERR_OTHER;
} /* End of function r_memdrv_qspix_rx_data() */

#endif  /* ((MEMDRV_CFG_DEV0_INCLUDED == 1) && ((MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
            (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM))) || \
           ((MEMDRV_CFG_DEV1_INCLUDED == 1) && ((MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_IAM) || \
            (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM))) */
/* End of File */
