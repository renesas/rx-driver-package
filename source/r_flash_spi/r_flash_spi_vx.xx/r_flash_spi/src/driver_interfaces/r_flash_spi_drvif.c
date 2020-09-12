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
* Copyright (C) 2011(2012-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : FLASH SPI driver software
* File Name    : r_flash_spi_drvif.c
* Version      : 3.00
* Device       : -
* Abstract     : IO I/F module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : SPI FLASH I/O file
* Limitation   : none
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 23.07.2014 2.21     Created
*              : 29.05.2015 2.32     Revised functions of same as Ver.2.32 of EEPROM SPI FIT module.
*              : 21.12.2018 3.00     Change flash drive interface to Memory Access Driver Interface
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_flash_spi_if.h"                      /* FLASH driver I/F definitions                 */
#include "r_flash_spi_config.h"                  /* FLASH driver Configuration definitions       */
#include "r_memdrv_rx_if.h"                      /* MEMORY driver I/F definitions                */
#include "./src/r_flash_spi_private.h"           /* FLASH driver Private module definitions      */


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


/************************************************************************************************
* Function Name: r_flash_spi_drvif_open
* Description  : Initializes I/O driver.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_open(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Open(devno,&memdrv_info);

    if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_open_tx_data
* Description  : Initializes I/O driver for writing data.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_open_tx_data(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Open(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_open_rx_data
* Description  : Initializes I/O driver for reading data.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_open_rx_data(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Open(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_close
* Description  : Resets I/O driver.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_close(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Close(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_close_tx_data
* Description  : Resets I/O driver for writing data.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_close_tx_data(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Close(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_close_rx_data
* Description  : Resets I/O driver for reading data.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_close_rx_data(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Close(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_disable
* Description  : Disables I/O driver.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_disable(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Disable(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_enable
* Description  : Enables I/O driver.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_enable(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Enable(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_enable_tx_data
* Description  : Enables I/O driver for writing data.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_enable_tx_data(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt = 0;
    memdrv_info.p_data = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_EnableTxData(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_enable_rx_data
* Description  : Enables I/O driver for reading data.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_enable_rx_data(uint8_t devno)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = 0;
    memdrv_info.p_data  = NULL;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_EnableRxData(devno,&memdrv_info);

     if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_tx
* Description  : Transmits data for command and status register.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
*              : uint32_t        txcnt                       ;   Number of bytes to be write
*              : uint8_t       * p_data                      ;   Write data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_HARD                          ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_tx(uint8_t devno, uint32_t txcnt, uint8_t * p_data)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = txcnt;
    memdrv_info.p_data  = p_data;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Tx(devno,&memdrv_info);

    if (MEMDRV_SUCCESS == MEMDRV_ERR_HARD)
    {
        return FLASH_SPI_ERR_HARD;
    } 
    else if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_tx_add
* Description  : Transmits data for address.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
*              : uint32_t        txcnt                       ;   Number of bytes to be write
*              : uint8_t       * p_data                      ;   Write data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_HARD                          ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_tx_add(uint8_t devno, uint32_t txcnt, uint8_t * p_data, flash_spi_opmode_t op_mode)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = txcnt;
    memdrv_info.p_data  = p_data;

    switch (op_mode)
    {
        case FLASH_SPI_SINGLE:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
        case FLASH_SPI_DUAL:
            memdrv_info.io_mode = MEMDRV_MODE_DUAL;
        break;
        case FLASH_SPI_QUAD:
            memdrv_info.io_mode = MEMDRV_MODE_QUAD;
        break;
        default:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
    } 

    ret_drv = R_MEMDRV_TxData(devno,&memdrv_info);

    if (MEMDRV_SUCCESS == MEMDRV_ERR_HARD)
    {
        return FLASH_SPI_ERR_HARD;
    } 
    else if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_tx_data
* Description  : Transmits data using the single mode.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
*              : uint32_t        txcnt                       ;   Number of bytes to be write
*              : uint8_t       * p_data                      ;   Write data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_HARD                          ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_tx_data(uint8_t devno, uint32_t txcnt, uint8_t * p_data, flash_spi_opmode_t op_mode)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = txcnt;
    memdrv_info.p_data  = p_data;

    switch (op_mode)
    {
        case FLASH_SPI_SINGLE:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
        case FLASH_SPI_DUAL:
            memdrv_info.io_mode = MEMDRV_MODE_DUAL;
        break;
        case FLASH_SPI_QUAD:
            memdrv_info.io_mode = MEMDRV_MODE_QUAD;
        break;
        default:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
    }

    ret_drv = R_MEMDRV_TxData(devno,&memdrv_info);

    if (MEMDRV_SUCCESS == MEMDRV_ERR_HARD)
    {
        return FLASH_SPI_ERR_HARD;
    } 
    else if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_rx
* Description  : Receives data for status register and Read ID.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
*              : uint32_t        rxcnt                       ;   Number of bytes to be read
*              : uint8_t       * p_data                      ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_HARD                          ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_rx(uint8_t devno, uint32_t rxcnt, uint8_t * p_data)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = rxcnt;
    memdrv_info.p_data  = p_data;
    memdrv_info.io_mode = MEMDRV_MODE_SINGLE;

    ret_drv = R_MEMDRV_Rx(devno,&memdrv_info);

    if (MEMDRV_SUCCESS == MEMDRV_ERR_HARD)
    {
        return FLASH_SPI_ERR_HARD;
    } 
    else if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_rx_add
* Description  : Receives data for address.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
*              : uint32_t        rxcnt                       ;   Number of bytes to be read
*              : uint8_t       * p_data                      ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_HARD                          ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_rx_add(uint8_t devno, uint32_t rxcnt, uint8_t * p_data, flash_spi_opmode_t op_mode)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = rxcnt;
    memdrv_info.p_data  = p_data;

    switch (op_mode)
    {
        case FLASH_SPI_SINGLE:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
        case FLASH_SPI_DUAL:
            memdrv_info.io_mode = MEMDRV_MODE_DUAL;
        break;
        case FLASH_SPI_QUAD:
            memdrv_info.io_mode = MEMDRV_MODE_QUAD;
        break;
        default:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
    }

    ret_drv = R_MEMDRV_RxData(devno,&memdrv_info);

    if (MEMDRV_SUCCESS == MEMDRV_ERR_HARD)
    {
        return FLASH_SPI_ERR_HARD;
    } 
    else if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_rx_data
* Description  : Receives data using the single mode.
* Arguments    : uint8_t         devno                       ;   Device No. (FLASH_SPI_DEVn)
*              : uint32_t        rxcnt                       ;   Number of bytes to be read
*              : uint8_t       * p_data                      ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*              : FLASH_SPI_ERR_HARD                          ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                         ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_rx_data(uint8_t devno, uint32_t rxcnt, uint8_t * p_data, flash_spi_opmode_t op_mode)
{
    memdrv_err_t ret_drv = MEMDRV_SUCCESS;
    st_memdrv_info_t  memdrv_info;

    memdrv_info.cnt     = rxcnt;
    memdrv_info.p_data  = p_data;

    switch (op_mode)
    {
        case FLASH_SPI_SINGLE:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
        case FLASH_SPI_DUAL:
            memdrv_info.io_mode = MEMDRV_MODE_DUAL;
        break;
        case FLASH_SPI_QUAD:
            memdrv_info.io_mode = MEMDRV_MODE_QUAD;
        break;
        default:
            memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        break;
    }

    ret_drv = R_MEMDRV_RxData(devno,&memdrv_info);

    if (MEMDRV_SUCCESS == MEMDRV_ERR_HARD)
    {
        return FLASH_SPI_ERR_HARD;
    } 
    else if (MEMDRV_SUCCESS > ret_drv)
    {
        return FLASH_SPI_ERR_OTHER;
    }
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_1ms_interval
* Description  : 1ms Interval Timer call function for driver interface.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_spi_drvif_1ms_interval(void)
{
    R_MEMDRV_1msInterval();
}


/************************************************************************************************
* Function Name: r_flash_spi_drvif_set_loghdladdress
* Description  : Sets handler address for driver interface.
* Arguments    : uint32_t           user_long_que           ;   Handler address
* Return Value : FLASH_SPI_SUCCESS                           ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_drvif_set_loghdladdress(uint32_t user_long_que)
{
    R_MEMDRV_SetLogHdlAddress(user_long_que);
    return FLASH_SPI_SUCCESS;
}


/* End of File */
