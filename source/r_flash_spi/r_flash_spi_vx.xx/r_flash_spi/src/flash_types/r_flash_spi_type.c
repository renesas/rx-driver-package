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
* Copyright (C) 2011(2012-2015) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : FLASH SPI driver software
* File Name    : r_flash_spi_type.c
* Version      : 2.32
* Device       : -
* Abstract     : User I/F file
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : Flash memory type I/F file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 23.07.2014 2.21     Created
*              : 29.05.2015 2.32     Revised functions of same as Ver.2.32 of EEPROM SPI FIT module.
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_flash_spi_if.h"                      /* FLASH driver I/F definitions                 */
#include "r_flash_spi_config.h"                  /* FLASH driver Configuration definitions       */
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
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
const uint32_t gc_flash_dev0_types = FLASH_SPI_DEV0_TYPE;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
const uint32_t gc_flash_dev1_types = FLASH_SPI_DEV1_TYPE;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */


/************************************************************************************************
* Function Name: r_flash_spi_init_port
* Description  : Sets FLASH control ports.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_init_port(uint8_t devno)
{
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            r_flash_spi_mx_init_port(devno);            /* SS# initialization                   */
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            r_flash_spi_n25q_init_port(devno);          /* SS# initialization                   */
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            r_flash_spi_m25p_init_port(devno);          /* SS# initialization                   */
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            r_flash_spi_m45pe_init_port(devno);         /* SS# initialization                   */
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            r_flash_spi_s25fl_init_port(devno);         /* SS# initialization                   */
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_reset_port
* Description  : Resets setting of ports.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_reset_port(uint8_t devno)
{
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            r_flash_spi_mx_reset_port(devno);           /* SS# reset                            */
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            r_flash_spi_n25q_reset_port(devno);         /* SS# reset                            */
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            r_flash_spi_m25p_reset_port(devno);         /* SS# reset                            */
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            r_flash_spi_m45pe_reset_port(devno);        /* SS# reset                            */
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            r_flash_spi_s25fl_reset_port(devno);        /* SS# reset                            */
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_read_status
* Description  : Reads status from the status register and stores to the read status storage buffer (p_status).
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t       * p_status               ;   Read status storage buffer (1 byte)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_read_status(uint8_t devno, uint8_t * p_status)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((FLASH_SPI_DEV_NUM <= devno) || (0 == p_status))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Read Status Register. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_read_stsreg(devno, p_status);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_read_stsreg(devno, p_status);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_read_stsreg(devno, p_status);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_read_stsreg(devno, p_status);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_read_stsreg(devno, p_status);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_set_write_protect
* Description  : Sets the write-protection setting to write-protection setting data (wpsts).
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t            wpsts               ;   Write-protection setting data
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_set_write_protect(uint8_t devno, uint8_t wpsts)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Use the Block lock (BP) protection mode as the write protection. */
    /* Execute the Write Status Register (WRSR) command operation using the single mode. */
    /* Execute the Read Status Register (RDSR) command operation using the single mode 
       in r_flash_spi_set_write_protect(). */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_set_write_protect(devno, wpsts);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_set_write_protect(devno, wpsts);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_set_write_protect(devno, wpsts);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_set_write_protect(devno, wpsts);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_set_write_protect(devno, wpsts);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_write_di
* Description  : Clears the WEL bit.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_write_di(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Execute the Write Disable (WRDI) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_write_di(devno);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_write_di(devno);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_write_di(devno);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_write_di(devno);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_write_di(devno);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }
    
    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_read_data
* Description  : Reads data from the specified address (addr) for the specified number (cnt) of bytes
*              : and stores to the specified buffer (p_data).
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : The maximum read address is Flash memory size - 1.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_read_data(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((0 == p_flash_spi_info) || (FLASH_SPI_DEV_NUM <= devno) || (0 == p_flash_spi_info->p_data))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }

    if (0 != (((uint32_t)p_flash_spi_info & FLASH_SPI_ADDR_BOUNDARY) || 
              ((uint32_t)p_flash_spi_info->p_data & FLASH_SPI_ADDR_BOUNDARY)))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return ret;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open_rx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Execute the READ command operation. */
    /* The SPI mode and bit rate should be set at the start of the following operation */
    /* and return to default at the end of the following operation. */
    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_read(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_read(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_read(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_read(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_read(devno, p_flash_spi_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close_rx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_write_data_page
* Description  : Writes data from the specified buffer (p_data)
*              : to the specified address (addr) for the specified number (cnt) of bytes.
* Arguments    : uint8_t            devno                 ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info      ;   Flash memory information
*              :    uint32_t        addr                  ;   Write start address
*              :    uint32_t        cnt                   ;   Number of bytes to be written
*              :    uint32_t        data_cnt              ;   Number of bytes to be written in a page
*              :    uint8_t       * p_data                ;   Write data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                        ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                      ;   Parameter error
*              : FLASH_SPI_ERR_HARD                       ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : FLASH can be written to only when write-protection has been canceled.
*              : The maximum write address is Flash memory size - 1.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_write_data_page(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t tmpcnt = 0;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((0 == p_flash_spi_info) || (FLASH_SPI_DEV_NUM <= devno) || (0 == p_flash_spi_info->p_data))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }

    /* Do not check the 4 bytes boundary of the buffer address in the Write Data operation. */
    if (0 != ((uint32_t)p_flash_spi_info & FLASH_SPI_ADDR_BOUNDARY))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_check_cnt(devno, p_flash_spi_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return ret;
    }

#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open_tx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Writing page calculation */
    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            tmpcnt = r_flash_spi_mx_page_calc(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            tmpcnt = r_flash_spi_n25q_page_calc(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            tmpcnt = r_flash_spi_m25p_page_calc(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            tmpcnt = r_flash_spi_m45pe_page_calc(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            tmpcnt = r_flash_spi_s25fl_page_calc(devno, p_flash_spi_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (tmpcnt > p_flash_spi_info->cnt)
    {
        p_flash_spi_info->data_cnt = p_flash_spi_info->cnt;
    }
    else
    {
        p_flash_spi_info->data_cnt = tmpcnt;
    }
        
    /* Execute the WRITE command operation using the single mode. */
    /* The SPI mode and bit rate should be set at the start of the following operation */
    /* and return to default at the end of the following operation. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_write_page(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_write_page(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_write_page(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_write_page(devno, p_flash_spi_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_write_page(devno, p_flash_spi_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Update the writing information. */
    p_flash_spi_info->cnt    -= p_flash_spi_info->data_cnt;
    p_flash_spi_info->p_data += p_flash_spi_info->data_cnt;
    p_flash_spi_info->addr   += p_flash_spi_info->data_cnt;

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close_tx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_erase
* Description  : Erases the data.
* Arguments    : uint8_t            devno         ;   Device No. (FLASH_DEVn)
*              : flash_spi_erase_info_t * p_flash_spi_erase_info    ; Flash memory erase information
*              :    uint32_t        addr          ;   First address of specified sector
*              :    uint8_t         mode          ;   Type of erase command
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_PARAM              ;   Parameter error
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : Flash memory can be erased to only when write-protection has been canceled.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_erase(uint8_t devno, flash_spi_erase_info_t * p_flash_spi_erase_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((0 == p_flash_spi_erase_info) || (FLASH_SPI_DEV_NUM <= devno))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }

    if (0 != ((uint32_t)p_flash_spi_erase_info & FLASH_SPI_ADDR_BOUNDARY))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Execute the Erase command operation. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_erase(devno, p_flash_spi_erase_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_erase(devno, p_flash_spi_erase_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_erase(devno, p_flash_spi_erase_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_erase(devno, p_flash_spi_erase_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_erase(devno, p_flash_spi_erase_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_polling
* Description  : Checks busy.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_poll_mode_t  mode            ;   Mode of error check
*              :                                        ;   FLASH_SPI_MODE_REG_WRITE_POLL
*              :                                        ;   FLASH_SPI_MODE_PROG_POLL
*              :                                        ;   FLASH_SPI_MODE_PROG_ERSAE
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation (FLASH is ready)
*              : FLASH_SPI_SUCCESS_BUSY                 ;   Successful operation (FLASH is busy)
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_polling(uint8_t devno, flash_spi_poll_mode_t mode)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    flash_spi_status_t ret_busy_chk = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Checks busy. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret_busy_chk = r_flash_spi_mx_polling(devno, mode);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret_busy_chk = r_flash_spi_n25q_polling(devno, mode);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret_busy_chk = r_flash_spi_m25p_polling(devno, mode);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret_busy_chk = r_flash_spi_m45pe_polling(devno, mode);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret_busy_chk = r_flash_spi_s25fl_polling(devno, mode);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret_busy_chk)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret_busy_chk;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret_busy_chk;
}


/************************************************************************************************
* Function Name: r_flash_spi_read_id
* Description  : Reads Manufacture ID and Device ID.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t          * p_data              ;   ID data storage buffer pointer
*              :                                        ;     MX25L/MX66L/MX25R : 3 bytes
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation (FLASH is ready)
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_read_id(uint8_t devno, uint8_t * p_data)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Executes the Read ID (RDID) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_rdid(devno, p_data);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_rdid(devno, p_data);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_rdid(devno, p_data);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_rdid(devno, p_data);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_rdid(devno, p_data);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_get_memoryInfo
* Description  : Gets the memory size and page size.
* Arguments    : uint8_t                devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_mem_info_t * p_flash_spi_mem_info;   Flash memory size information
*              :    uint32_t            mem_size            ;   Max memory size
*              :    uint32_t            wpag_size           ;   Write page size
* Return Value : FLASH_SPI_SUCCESS                          ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                        ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_get_memory_info(uint8_t devno, flash_spi_mem_info_t * p_flash_spi_mem_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((0 == p_flash_spi_mem_info) || (FLASH_SPI_DEV_NUM <= devno))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }

    if (0 != ((uint32_t)p_flash_spi_mem_info & FLASH_SPI_ADDR_BOUNDARY))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_get_memory_info(devno, p_flash_spi_mem_info);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_get_memory_info(devno, p_flash_spi_mem_info);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = r_flash_spi_m25p_get_memory_info(devno, p_flash_spi_mem_info);
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = r_flash_spi_m45pe_get_memory_info(devno, p_flash_spi_mem_info);
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_get_memory_info(devno, p_flash_spi_mem_info);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }
    
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_read_configuration
* Description  : Reads status from the configuration register and stores to the configuration storage buffer (p_config).
* Arguments    : uint8_t         devno            ;   Device No. (FLASH_DEVn)
*              : uint8_t       * p_config         ;   Read configuration storage buffer (1 byte or 2 bytes)
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_PARAM              ;   Parameter error
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_read_configuration(uint8_t devno, uint8_t * p_config_reg)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((FLASH_SPI_DEV_NUM <= devno) || (0 == p_config_reg))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Executes the Read Configurtion Register (RDCR) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_read_configreg(devno, p_config_reg);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_read_configreg(devno, p_config_reg);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_write_configuration
* Description  : Writes from the write configuration storage buffer to the configuration register.
* Arguments    : uint8_t         devno            ;   Device No. (FLASH_DEVn)
*              : flash_spi_reg_info_t * p_reg     ;   Flash memory register information
*              :     uint8_t     status           ;   Status register setting data
*              :     uint8_t     config1          ;   Configuration or Configuration-1 register setting data
*              :     uint8_t     config2          ;   Configuration-2 register setting data
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_PARAM              ;   Parameter error
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_write_configuration(uint8_t devno, flash_spi_reg_info_t * p_reg)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((FLASH_SPI_DEV_NUM <= devno) || (0 == p_reg))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }

    /* Do not check the 4 bytes boundary of the buffer address in the Write Data operation. */
    if (0 != ((uint32_t)p_reg & FLASH_SPI_ADDR_BOUNDARY))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Executes the Write Register (WRR) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_write_configuration(devno, p_reg);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = r_flash_spi_s25fl_write_configuration(devno, p_reg);
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_set_4byte_address_mode
* Description  : Issues a ENTER 4-BYTE MODE (EN4B) command.
*              : Call this function once at system activation after calling R_FLASH_SPI_Open().
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : In the case of MX25L/MX66L/MX25R,
:              : please confirm that the "4 BYTE" bit in the configuration register is "1" after this processing.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_set_4byte_address_mode(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Issues the Enter 4-byte Address Mode command using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_enter_4addr(devno);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = r_flash_spi_n25q_enter_4addr(devno);
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }
    
    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_read_security
* Description  : Reads status from the security register and stores to the security storage buffer (p_scur).
* Arguments    : uint8_t         devno            ;   Device No. (FLASH_DEVn)
*              : uint8_t FAR *   p_scur           ;   Security storage buffer (1 byte)
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_PARAM              ;   Parameter error
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_read_security(uint8_t devno, uint8_t * p_scur)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((FLASH_SPI_DEV_NUM <= devno) || (0 == p_scur))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Executes the Read Security Register (RDSCUR) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_read_scurreg(devno, p_scur);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_quad_enable
* Description  : Makes a setting using Quad Disable (QE Bit) mode for MX25L/MX66L/MX25R.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register and configuration register after this processing.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_quad_enable(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Executes the Write Status Register (WRSR) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_quad_enable(devno);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }
    
    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_flash_spi_quad_disable
* Description  : Makes a setting using Quad Disable (QE Bit) mode for MX25L/MX66L/MX25R.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register and configuration register after this processing.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_quad_disable(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;
    uint32_t flash_type = 0;

    /* Check parameters. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DEV_NUM <= devno)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Open driver interface. */
    ret = r_flash_spi_drvif_open(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    r_flash_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    /* Executes the Write Status Register (WRSR) command operation using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        flash_type = gc_flash_dev0_types;
#endif /* (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        flash_type = gc_flash_dev1_types;
#endif /* (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    switch (flash_type)
    {
        case FLASH_SPI_TYPE_MX25L:
        case FLASH_SPI_TYPE_MX66L:
        case FLASH_SPI_TYPE_MX25R:
#if (FLASH_SPI_CFG_DEV0_MX25L == 1) || (FLASH_SPI_CFG_DEV0_MX66L == 1) ||(FLASH_SPI_CFG_DEV0_MX25R == 1)
            ret = r_flash_spi_mx_quad_disable(devno);
#endif
        break;
        case FLASH_SPI_TYPE_N25Q:
#if (FLASH_SPI_CFG_DEV0_N25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M25P:
#if (FLASH_SPI_CFG_DEV0_M25Q == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_M45PE:
#if (FLASH_SPI_CFG_DEV0_M45PE == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        case FLASH_SPI_TYPE_S25FL:
#if (FLASH_SPI_CFG_DEV0_S25FL == 1)
            ret = FLASH_SPI_ERR_OTHER;
#endif
        break;
        default:
            /* Do nothing. */
        break;
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }
    
    /* Close driver interface. */
    ret = r_flash_spi_drvif_close(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE, __LINE__);
        r_flash_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_flash_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/* End of File */
