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
* Copyright (C) 2004(2005-2015) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : EEPROM driver software
* File Name    : r_eeprom_spi.c
* Version      : 2.31
* Device       : -
* Abstract     : User I/F file
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : SPI Renesas EEPROM User I/F file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : Date          No.             Comment
*              : 2004/12/06                    wrote it.
*              : 2004/12/08                    eep_Write_Status()
*              :                                   Deleted review result.
*              :                               eep_Write_Data()
*              :                                   Changed from checking RAM to checking status register
*              :                                   for write-protection check.
*              :                                   register for write-protection check.
*              : 2004/12/10                    Moved eep_PWrite_En(), eep_Write_Di()
*              :                                   to eep_io.c.
*              :                               eep_Init_Driver()
*              :                                   Changed port initialization processing to port-open processing.
*              : 2004/12/27                    Modified for MISRA-C.
*              : 2006/01/27                    eep_Init_Driver(), eep_Read_Status(), eep_Write_Protect()
*              :                               eep_Read_Data(), eep_Write_Data()
*              :                                   Added interruption setting.
*              : 2006.01.27    Ver1.10 --------------------------------------------------------
*              : 2007.02.27    #### FF-0001    eep_Read_Data()
*              :                                   For RENESAS TECHNICAL UPDATE "TN-16C-A162A/J"
*              :                                   Added UART continuous reception prohibition mode.
*              : 2007.02.27    Ver1.20 --------------------------------------------------------
*              : 2007.11.07    #### BU-0004    eep_Init_Driver(), eep_Read_Status(), eep_Write_Protect()
*              :                               eep_Read_Data(), eep_Write_Data()
*              :                                   Deleted port-open processing.
*              : 2007.11.07    Ver1.21 --------------------------------------------------------
*              : 2009.01.31    #### CF-0005    Divided between SIO processing and CRC calculation.
*              : 2009.03.06    Ver1.30 --------------------------------------------------------
*              : 2011.02.15                    Changed API.
*              : 2011.02.15    Ver2.00 --------------------------------------------------------
*              : 2011.04.15    Ver2.01 --------------------------------------------------------
*              : 2012.03.19    Changed History to English comment.
*              : 2012.03.19    Ver2.02 --------------------------------------------------------
*              : 2013.09.30    Ver2.03 Modified R_EEPROM_SPI_Write_Protect().
*              : 2013.09.30    Ver2.03 Modified R_EEPROM_SPI_Write_Data().
*
*              : DD.MM.YYYY Version  Description
*              : 28.11.2014 2.30     Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31     Added RX71M.
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_eeprom_spi_if.h"                      /* EEPROM driver I/F definitions                 */
#include "r_eeprom_spi_config.h"                  /* EEPROM driver Configuration definitions       */
#include "./src/r_eeprom_spi_private.h"           /* EEPROM driver Private module definitions      */

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
* Function Name: R_EEPROM_SPI_Open
* Description  : Initializes the EEPROM driver.
*              : Sets EEPROM control ports.
*              : Call this function once at system activation.
* Arguments    : uint8_t         devno                     ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Open(uint8_t devno)
{
    /* Check parameters. */
    if (EEPROM_SPI_DEV_NUM <= devno)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    r_eeprom_spi_init_port(devno);                         /* SS# initialization                   */

    return EEPROM_SPI_SUCCESS;

}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Close
* Description  : Stops EEPROM control.
*                Resets setting of ports.
* Arguments    : uint8_t         devno                     ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Close(uint8_t devno)
{
    /* Check parameters. */
    if (EEPROM_SPI_DEV_NUM <= devno)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    r_eeprom_spi_reset_port(devno);                        /* SS# reset                            */

    return EEPROM_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Read_Status
* Description  : Reads status from the status register and stores to the read status storage buffer (p_status).
*              :
*              : Memory size <= 4K-bit
*              :   Bits 7 to 4 :   Reserved (All "1")
*              :   Bits 3, 2   :   BP1, BP0
*              :                   00: No protection
*              :                   01: Upper-quarter protection
*              :                   10: Upper-half protection
*              :                   11: Whole memory protection
*              :   Bit 1   : WEL
*              :                   0: Write disabled
*              :                    1: Write enabled
*              :   Bit 0   : WIP
*              :                   1: During write operation
*              : Memory size > 4K-bit
*              :   Bit 7   : SRWD
*              :                   0: Status register can be changed
*              :                   1: Status register cannot be changed
*              :   Bits 6 to 4 :   Reserved (All "0")
*              :   Bits 3, 2   :   BP1, BP0
*              :                   00: No protection
*              :                   01: Upper-quarter protection
*              :                   10: Upper-half protection
*              :                   11: Whole memory protection
*              :   Bit 1   : WEL
*              :                   0: Write disabled
*              :                   1: Write enabled
*              :   Bit 0   : WIP
*              :                   1: During write operation
*              :
* Arguments    : uint8_t         devno                     ;   Device No. (EEPROM_SPI_DEVn)
*              : uint8_t       * p_status                  ;   Read status storage buffer (1 byte)
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                       ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Read_Status(uint8_t devno, uint8_t * p_status)
{
    eeprom_status_t          ret = EEPROM_SPI_SUCCESS;

    /* Check parameters. */
    if ((EEPROM_SPI_DEV_NUM <= devno) || (NULL == p_status))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    /* Open driver interface. */
    ret = r_eeprom_spi_drvif_open(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    r_eeprom_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Read Status Register. */
    ret = r_eeprom_spi_read_stsreg(devno, p_status);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_eeprom_spi_drvif_close(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Set_Write_Protect
* Description  : Sets the write-protection setting to write-protection setting data (wpsts).
*              :
*              : EEPROM_SPI_WP_NONE           : No protection
*              : EEPROM_SPI_WP_UPPER_QUART    : Upper-quarter protection setting
*              : EEPROM_SPI_WP_UPPER_HALF     : Upper-half protection setting
*              : EEPROM_SPI_WP_WHOLE_MEM      : Whole memory protection setting
*              :
* Arguments    : uint8_t            devno                  ;   Device No. (EEPROM_SPI_DEVn)
*              : uint8_t            wpsts                  ;   Write-protection setting data
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                       ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register.
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Set_Write_Protect(uint8_t devno, uint8_t wpsts)
{
    eeprom_status_t     ret = EEPROM_SPI_SUCCESS;
    uint8_t             stsreg = 0;                     /* Write status buffer                  */

    /* Check parameters. */
    if ((EEPROM_SPI_DEV_NUM <= devno) || (wpsts > EEPROM_SPI_WP_WHOLE_MEM))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    /* Open driver interface. */
    ret = r_eeprom_spi_drvif_open(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Store a write-protection setting value. */
    stsreg = (wpsts << 2) & (uint8_t)(~EEPROM_SPI_REG_SRWD);   /* SRWD is fixed "0"                */

    r_eeprom_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);                /* SIO enable                           */
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Use the Block lock (BP) protection mode as the write protection. */
    /* Execute the Write Status Register (WRSR) command operation using the single mode. */
    /* Execute the Read Status Register (RDSR) command operation using the single mode 
       in r_eeprom_spi_write_stsreg(). */
    ret = r_eeprom_spi_write_stsreg(devno, (uint8_t *)&stsreg);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_eeprom_spi_drvif_close(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Write_Di
* Description  : Clears the WEL bit.
* Arguments    : uint8_t         devno                     ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                       ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Write_Di(uint8_t devno)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    /* Check parameters. */
    if (EEPROM_SPI_DEV_NUM <= devno)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    /* Open driver interface. */
    ret = r_eeprom_spi_drvif_open(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    r_eeprom_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Execute the Write Disable (WRDI) command operation using the single mode. */
    ret = r_eeprom_spi_write_di(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }
    
    /* Close driver interface. */
    ret = r_eeprom_spi_drvif_close(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Read_Data
* Description  : Reads data from the specified address (addr) for the specified number (cnt) of bytes
*              : and stores to the specified buffer (p_data).
* Arguments    : uint8_t            devno                  ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info          ;   EEPROM Information
*              :    uint32_t        addr                   ;   Read start address
*              :    uint32_t        cnt                    ;   Number of bytes to be read
*              :    uint32_t        data_cnt               ;   Not used
*              :    uint8_t       * p_data                 ;   Read data storage buffer pointer
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                       ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : The maximum read address is EEPROM size - 1.
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Read_Data(uint8_t devno, eeprom_info_t * p_eeprom_info)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    /* Check parameters. */
    if ((NULL == p_eeprom_info) || (EEPROM_SPI_DEV_NUM <= devno) || (NULL == p_eeprom_info->p_data))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    if (0 != (((uint32_t)p_eeprom_info & 0x00000003) || ((uint32_t)p_eeprom_info->p_data & 0x00000003)))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    ret = r_eeprom_spi_check_cnt(devno, p_eeprom_info);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return ret;
    }

    /* Open driver interface. */
    ret = r_eeprom_spi_drvif_open_rx_data(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    r_eeprom_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Execute the READ command operation. */
    /* The SPI mode and bit rate should be set at the start of the following operation */
    /* and return to default at the end of the following operation. */
    ret = r_eeprom_spi_read(devno, p_eeprom_info);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_eeprom_spi_drvif_close_rx_data(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Write_Data_Page
* Description  : Writes data from the specified buffer (p_data)
*              : to the specified address (addr) for the specified number (cnt) of bytes.
* Arguments    : uint8_t            devno                  ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info          ;   EEPROM Information
*              :    uint32_t        addr                   ;   Write start address
*              :    uint32_t        cnt                    ;   Number of bytes to be written
*              :    uint32_t        data_cnt               ;   Number of bytes to be written in a page
*              :    uint8_t       * p_data                 ;   Write data storage buffer pointer
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                       ;   Hardware error
*              : EEPROM_SPI_ERR_WP                         ;   Write-protection error
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : EEPROM can be written to only when write-protection has been canceled.
*              : The maximum write address is EEPROM size - 1.
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Write_Data_Page(uint8_t devno, eeprom_info_t * p_eeprom_info)
{
    eeprom_status_t     ret = EEPROM_SPI_SUCCESS;
    uint32_t            tmpcnt = 0;
    uint8_t             stsreg = 0;

    /* Check parameters. */
    if ((NULL == p_eeprom_info) || (EEPROM_SPI_DEV_NUM <= devno) || (NULL == p_eeprom_info->p_data))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    /* Do not check the 4 bytes boundary of the buffer address in the Write Data operation. */
    if (0 != ((uint32_t)p_eeprom_info & 0x00000003))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    ret = r_eeprom_spi_check_cnt(devno, p_eeprom_info);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return ret;
    }

    /* Open driver interface. */
    ret = r_eeprom_spi_drvif_open_tx_data(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    r_eeprom_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);                /* SIO enable                           */
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Check write-protection. */
    ret = r_eeprom_spi_read_stsreg(devno, (uint8_t *)&stsreg);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    ret = r_eeprom_spi_check_wp(devno, p_eeprom_info);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Writing page calculation */
    tmpcnt = r_eeprom_spi_page_calc(devno, p_eeprom_info);

    if (tmpcnt > p_eeprom_info->cnt)
    {
        p_eeprom_info->data_cnt = p_eeprom_info->cnt;
    }
    else
    {
        p_eeprom_info->data_cnt = tmpcnt;
    }
        
    /* Execute the WRITE command operation using the single mode. */
    /* The SPI mode and bit rate should be set at the start of the following operation */
    /* and return to default at the end of the following operation. */
    ret = r_eeprom_spi_write_page(devno, p_eeprom_info);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Update the writing information. */
    p_eeprom_info->cnt    -= p_eeprom_info->data_cnt;
    p_eeprom_info->p_data += p_eeprom_info->data_cnt;
    p_eeprom_info->addr   += p_eeprom_info->data_cnt;

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_eeprom_spi_drvif_close_tx_data(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_Polling
* Description  : Checks busy for the WRITE command operation.
* Arguments    : uint8_t            devno                  ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation (EERPOM is ready)
*              : EEPROM_SPI_SUCCESS_BUSY                   ;   Successful operation (EERPOM is busy)
*              : EEPROM_SPI_ERR_PARAM                      ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                       ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Polling(uint8_t devno)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;
    eeprom_status_t        ret_busy_chk = EEPROM_SPI_SUCCESS;

    /* Check parameters. */
    if (EEPROM_SPI_DEV_NUM <= devno)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    /* Open driver interface. */
    ret = r_eeprom_spi_drvif_open(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);      /* Unified error processing for ROM compression.   */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    r_eeprom_spi_init_port(devno);                         /* Port initialization                  */

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Check busy for the WRITE command operation. */
    ret_busy_chk = r_eeprom_spi_polling(devno);
    if (EEPROM_SPI_SUCCESS > ret_busy_chk)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);
        r_eeprom_spi_drvif_close(devno);
        return ret_busy_chk;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    /* Close driver interface. */
    ret = r_eeprom_spi_drvif_close(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        r_eeprom_spi_drvif_disable(devno);     /* Unified error processing for ROM compression.    */
        r_eeprom_spi_drvif_close(devno);
        return ret;
    }

    return ret_busy_chk;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_GetMemoryInfo
* Description  : Gets the memory size and page size.
* Arguments    : uint8_t               devno                   ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_mem_info_t   * p_eeprom_mem_info       ;   EEPROM information
* Return Value : EEPROM_SPI_SUCCESS                            ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                          ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_GetMemoryInfo(uint8_t devno, eeprom_mem_info_t * p_eeprom_mem_info)
{
    /* Check parameters. */
    if ((NULL == p_eeprom_mem_info) || (EEPROM_SPI_DEV_NUM <= devno))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    if (0 != ((uint32_t)p_eeprom_mem_info & 0x00000003))
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
        return EEPROM_SPI_ERR_PARAM;
    }

    /* Get memory information. */
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            p_eeprom_mem_info->mem_size  = EEPROM_SPI_DEV0_MEM_SIZE;
            p_eeprom_mem_info->wpag_size = EEPROM_SPI_DEV0_WPAG_SIZE;
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            p_eeprom_mem_info->mem_size  = EEPROM_SPI_DEV1_MEM_SIZE;
            p_eeprom_mem_info->wpag_size = EEPROM_SPI_DEV1_WPAG_SIZE;
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
    
    return EEPROM_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_GetVersion
* Description  : Returns the version of EEPROM SPI driver.
*                The version number is encoded such that the top two bytes are
*                the major version number and the bottom two bytes are the minor
*                version number.
* Arguments    : none
* Return Value : version number
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t R_EEPROM_SPI_GetVersion(void)
{
    uint32_t const version = (EEPROM_SPI_VERSION_MAJOR << 16) | EEPROM_SPI_VERSION_MINOR;
    
    return version;
}


/* End of File */
