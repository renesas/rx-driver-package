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
* Copyright (C) 2004(2005-2020) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : EEPROM driver software
* File Name    : r_eeprom_spi.c
* Version      : 3.02
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
*              : 10.12.2020 3.02     Modified comment of API function to Doxygen style.
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_eeprom_spi_if.h"                      /* EEPROM driver I/F definitions                 */
#include "r_eeprom_spi_config.h"                  /* EEPROM driver Configuration definitions       */
#include "r_eeprom_spi_private.h"                 /* EEPROM driver Private module definitions      */

/************************************************************************************************
Macro definitions
*************************************************************************************************/


/************************************************************************************************
Typedef definitions
*************************************************************************************************/


/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Open
 *****************************************************************************************************************/ /**
 * @brief This function is run first when using the APIs of the serial EEPROM control software.
 * @param[in] devno
 *             Device number (0, 1)
 * @retval    EEPROM_SPI_SUCCESS            Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM          Parameter error
 * @details   Initializes the slave device select pin of the device number specified by the argument devno.
 *            After initialization the pin is in the general output port high-output state.\n
 *            Do not call this function when communication is in progress.
 *            Communication cannot be guaranteed if the function is called when communication is in progress.
 * @note      After calling this user API function, it is recommended that R_EEPROM_SPI_Polling() be used to
 *            confirm that the EEPROM write cycle has completed. The next read or write processing will not
 *            be accepted while the EEPROM write cycle is in progress.\n
 *            However, it is possible to access the EEPROM during the write cycle by, for example, issuing a
 *            system reset while the EEPROM write cycle is in progress and restarting EEPROM control from the
 *            beginning.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Close
 *****************************************************************************************************************/ /**
 * @brief This function is used to close the serial EEPROM control software when it is in use.
 * @param[in] devno
 *             Device number (0, 1)
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @retval    EEPROM_SPI_ERR_OTHER  Other error
 * @details   Sets the slave device select pin of the device number specified by the argument devno to function as
 *            a general I/O port. After the function runs, the pin is in the general output port high-output state.\n
 *            Do not call this function when communication is in progress.
 *            Communication cannot be guaranteed if the function is called when communication is in progress.
 * @note      The state of the slave device select pin after this function is called is different from its state after
 *            a reset (general input port state). Review the pin settings if necessary.\n
 *            Before calling this user API function, it is recommended that R_EEPROM_SPI_Polling() be used to confirm
 *            that the EEPROM write cycle has completed. This makes it possible to restart EEPROM control because the
 *            EEPROM has not transitioned to the write cycle.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Read_Status
 *****************************************************************************************************************/ /**
 * @brief This function is used to read the status register.
 * @param[in] devno
 *             Device number (0, 1)
 * @param[in,out] *p_status
 *             Status register storage buffer (size: 1 byte)
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @retval    EEPROM_SPI_ERR_HARD   Hardware error
 * @retval    EEPROM_SPI_ERR_OTHER  Other task has acquired clock synchronous single master control software resources,
 *                                  or other error
 * @details   Reads the status register and stores the contents in p_status. See section 3.3 in the application note
 *            for details.
 * @note      The clock synchronous single master control software resources are acquired at the start of the
 *            processing, and the resources are released and the end of the processing.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Set_Write_Protect
 *****************************************************************************************************************/ /**
 * @brief This function is used to make write protect settings.
 * @param[in] devno
 *             Device number (0, 1)
 * @param[in] wpsts
 *             Write protect setting data
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @retval    EEPROM_SPI_ERR_HARD   Hardware error
 * @retval    EEPROM_SPI_ERR_OTHER  Other task has acquired clock synchronous single master control software resources,
 *                                  or other error
 * @details   Make write protect settings. SRWD is cleared to 0. \n
 *            After this user API function finishes successfully, the EEPROM transitions to the write cycle. Do not
 *            fail to confirm that the write has finished with R_EEPROM_SPI_Polling(). If an attempt is made to perform
 *            the next read or write processing while a write cycle is in progress, the EEPROM will not accept that
 *            processing.\n
 *            R_EEPROM_SPI_Polling() can be called at any time specified by the user. This makes it possible for the
 *            user application to perform other processing while a write cycle is in progress.See section 3.4 in the
 *            application note for details.
 * @note      The clock synchronous single master control software resources are acquired at the start of the
 *            processing, and the resources are released and the end of the processing.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Write_Di
 *****************************************************************************************************************/ /**
 * @brief This function is used to disable write operation.
 * @param[in] devno
 *             Device number (0, 1)
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @retval    EEPROM_SPI_ERR_HARD   Hardware error
 * @retval    EEPROM_SPI_ERR_OTHER  Other task has acquired clock synchronous single master control software resources,
 *                                  or other error
 * @details   Transmits the WRDI command and clears the WEL bit in the status register.
 * @note      The clock synchronous single master control software resources are acquired at the start of the
 *            processing, and the resources are released and the end of the processing.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Read_Data
 *****************************************************************************************************************/ /**
 * @brief This function is used to read data from the EEPROM.
 * @param[in] devno
 *             Device number (0, 1)
 * @param[in,out] *p_eeprom_info
 *             EEPROM information structure. Use the structure address aligned with a 4-byte boundary.
 *             See section 3.6 in the application note for details.
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @retval    EEPROM_SPI_ERR_HARD   Hardware error
 * @retval    EEPROM_SPI_ERR_OTHER  Other task has acquired clock synchronous single master control software resources,
 *                                  or other error
 * @details   Reads the specified number of bytes of data from the specified address in the EEPROM and stores the data
 *            in p_data.\n
 *            The maximum read address is the EEPROM capacity – 1.\n
 *            EEPROM_SPI_ERR_PARAM is returned if the total value of the read byte count, cnt, and specified address,
 *            addr, exceeds the maximum read address.\n
 *            DMAC transfer or DTC transfer occurs when the transfer size conditions of the clock synchronous single
 *            master control software are matched. Otherwise, operation switches to software transfer.
 * @note      To speed up data transfers, align the start address with a 4-byte boundary when specifying data storage
 *            buffer pointers.\n
 *            The clock synchronous single master control software resources are acquired at the start of the
 *            processing, and the resources are released and the end of the processing.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Write_Data_Page
 *****************************************************************************************************************/ /**
 * @brief This function is used to write data to the EEPROM in single-page units.
 * @param[in] devno
 *             Device number (0, 1)
 * @param[in, out] *p_eeprom_info
 *             EEPROM information structure. Use the structure address aligned with a 4-byte boundary.
 *             See section 3.7 in the application note for details.
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @retval    EEPROM_SPI_ERR_HARD   Hardware error
 * @retval    EEPROM_SPI_ERR_WP     Write protect error
 * @retval    EEPROM_SPI_ERR_OTHER  Other task has acquired clock synchronous single master control software resources,
 *                                  or other error
 * @details   Writes the specified number of bytes of data (up to a maximum size of 1 page) in p_data to the EEPROM,
 *            starting from the specified address.\n
 *            When writing a large volume of data, communication is divided into page units. This prevents a situation
 *            in which other processing is not possible while communication is in progress.\n
 *            Writing to the EEPROM is only possible when write protect has been canceled.\n
 *            It is not possible to write to a protected page. Attempting to do so returns the error EEPROM_SPI_ERR_WP.\n
 *            The maximum write address is the EEPROM capacity – 1.\n
 *            EEPROM_SPI_ERR_PARAM is returned if the total value of the write byte count, cnt, and specified address,
 *            addr, exceeds the maximum write address.\n
 *            DMAC transfer or DTC transfer occurs when the transfer size conditions of the clock synchronous single
 *            master control software are matched. Otherwise, operation switches to software transfer.\n
 *            When a byte count exceeding 1 page is specified, the remaining byte count and next address information
 *            remain in the EEPROM information stricture (p_eeprom_info) after processing of a single page write
 *            finishes. It is possible to write the remaining bytes by specifying p_eeprom_info unmodified in this
 *            API function again.\n
 *            After this user API function finishes successfully, the EEPROM transitions to the write cycle. Do not
 *            fail to confirm that the write has finished with R_EEPROM_SPI_Polling(). If an attempt is made to perform
 *            the next read or write processing while a write cycle is in progress, the EEPROM will not accept that
 *            processing. \n
 *            R_EEPROM_SPI_Polling() can be called at any time specified by the user. This makes it possible for the
 *            user application to perform other processing while a write cycle is in progress.\n
 *            See section 3.7 in the application note for details.
 * @note      To speed up data transfers, align the start address with a 4-byte boundary when specifying data storage
 *            buffer pointers.\n
 *            The clock synchronous single master control software resources are acquired at the start of the
 *            processing, and the resources are released and the end of the processing.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_Polling
 *****************************************************************************************************************/ /**
 * @brief This function is used to perform polling to determine if the write operation has finished.
 * @param[in] devno
 *             Device number (0, 1)
 * @retval    EEPROM_SPI_SUCCESS         Normal end, and write finished
 * @retval    EEPROM_SPI_SUCCESS_BUSY    Normal end, and write in progress
 * @retval    EEPROM_SPI_ERR_PARAM       Parameter error
 * @retval    EEPROM_SPI_ERR_HARD        Hardware error
 * @retval    EEPROM_SPI_ERR_OTHER       Other task has acquired clock synchronous single master control software
 *                                       resources, or other error
 * @details   Determines whether or not the write cycle has finished.
 * @note      R_EEPROM_SPI_Polling() can be called at any time specified by the user. This makes it possible for the
 *            user application to perform other processing while a write cycle is in progress.\n
 *            The clock synchronous single master control software resources are acquired at the start of the
 *            processing, and the resources are released and the end of the processing.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_GetMemoryInfo
 *****************************************************************************************************************/ /**
 * @brief This function is used to fetch the serial EEPROM size information.
 * @param[in] devno
 *             Device number (0, 1)
 * @param[in,out] p_eeprom_mem_info
 *             EEPROM size information structure. Use the structure address aligned with a 4-byte boundary.
 * @retval    EEPROM_SPI_SUCCESS    Successful operation
 * @retval    EEPROM_SPI_ERR_PARAM  Parameter error
 * @details   Fetches EEPROM size information for the device number specified by the argument devno.
 * @note      None.
 */
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


/**********************************************************************************************************************
 * Function Name: R_EEPROM_SPI_GetVersion
 *****************************************************************************************************************/ /**
 * @brief This function is used to fetch the serial EEPROM version information.
 * @return    Version number.   Upper 2 bytes: major version, lower 2 bytes: minor version.
 * @details   Returns the version information.
 * @note      None
 */
uint32_t R_EEPROM_SPI_GetVersion(void)
{
    uint32_t const version = (EEPROM_SPI_VERSION_MAJOR << 16) | EEPROM_SPI_VERSION_MINOR;
    
    return version;
}


/* End of File */
