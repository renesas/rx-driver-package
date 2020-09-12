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
* File Name    : r_flash_spi.c
* Version      : 2.32
* Device       : -
* Abstract     : User I/F file
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : SPI FLASH User I/F file
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
#if (FLASH_SPI_CFG_LONGQ_ENABLE == 1)
static longq_hdl_t       p_flash_long_que;             /* LONGQ handler                        */
#endif  /* (FLASH_SPI_CFG_LONGQ_ENABLE == 1) */


/************************************************************************************************
* Function Name: R_FLASH_SPI_Open
* Description  : Initializes the FLASH driver.
*              : Sets FLASH control ports.
*              : Call this function once at system activation.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Open(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_init_port(devno);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Close
* Description  : Stops FLASH control.
*                Resets setting of ports.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Close(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_reset_port(devno);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Read_Status
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
flash_spi_status_t R_FLASH_SPI_Read_Status(uint8_t devno, uint8_t * p_status)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_read_status(devno, p_status);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Set_Write_Protect
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
flash_spi_status_t R_FLASH_SPI_Set_Write_Protect(uint8_t devno, uint8_t wpsts)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_set_write_protect(devno, wpsts);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Write_Di
* Description  : Clears the WEL bit.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Write_Di(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_write_di(devno);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Read_Data
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
flash_spi_status_t R_FLASH_SPI_Read_Data(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_read_data(devno, p_flash_spi_info);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Write_Data_Page
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
flash_spi_status_t R_FLASH_SPI_Write_Data_Page(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_write_data_page(devno, p_flash_spi_info);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Erase
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
flash_spi_status_t R_FLASH_SPI_Erase(uint8_t devno, flash_spi_erase_info_t * p_flash_spi_erase_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_erase(devno, p_flash_spi_erase_info);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Polling
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
flash_spi_status_t R_FLASH_SPI_Polling(uint8_t devno, flash_spi_poll_mode_t mode)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_polling(devno, mode);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Read_ID
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
flash_spi_status_t R_FLASH_SPI_Read_ID(uint8_t devno, uint8_t * p_data)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_read_id(devno, p_data);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_GetMemoryInfo
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
flash_spi_status_t R_FLASH_SPI_GetMemoryInfo(uint8_t devno, flash_spi_mem_info_t * p_flash_spi_mem_info)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_get_memory_info(devno, p_flash_spi_mem_info);
    
    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Read_Configuration
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
flash_spi_status_t R_FLASH_SPI_Read_Configuration(uint8_t devno, uint8_t * p_config)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_read_configuration(devno, p_config);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Write_Configuration
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
flash_spi_status_t R_FLASH_SPI_Write_Configuration(uint8_t devno, flash_spi_reg_info_t * p_reg)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_write_configuration(devno, p_reg);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Set_4byte_Address_Mode
* Description  : Issues a ENTER 4-BYTE MODE (EN4B) command.
*              : Call this function once at system activation after calling R_FLASH_SPI_Open().
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : Please confirm that the "4 BYTE" bit in the configuration register is "1" after this processing.
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Set_4byte_Address_Mode(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_set_4byte_address_mode(devno);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Read_Security
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
flash_spi_status_t R_FLASH_SPI_Read_Security(uint8_t devno, uint8_t * p_scur)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_read_security(devno, p_scur);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Quad_Enable
* Description  : Makes a setting using Quad Enable (QE Bit) mode for MX25L/MX66L/MX25R.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register and configuration register after this processing.
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Quad_Enable(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_quad_enable(devno);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_Quad_Disable
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
flash_spi_status_t R_FLASH_SPI_Quad_Disable(uint8_t devno)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS;

    ret = r_flash_spi_quad_disable(devno);

    return ret;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_GetVersion
* Description  : Returns the version of FLASH SPI driver.
*                The version number is encoded such that the top two bytes are
*                the major version number and the bottom two bytes are the minor
*                version number.
* Arguments    : none
* Return Value : version number
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t R_FLASH_SPI_GetVersion(void)
{
    uint32_t const version = (FLASH_SPI_VERSION_MAJOR << 16) | FLASH_SPI_VERSION_MINOR;
    
    return version;
}


/************************************************************************************************
* Function Name: R_FLASH_SPI_1ms_Interval
* Description  : 1ms Interval Timer call function.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : Please call the function when using DMAC or DTC.
*************************************************************************************************/
void R_FLASH_SPI_1ms_Interval(void)
{
    r_flash_spi_drvif_1ms_interval();
}


#if (FLASH_SPI_CFG_LONGQ_ENABLE == 1)
/************************************************************************************************
* Function Name: R_FLASH_SPI_Set_LogHdlAddress
* Description  : Sets handler address.
* Arguments    : uint32_t           user_long_que       ;   Handler address
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Set_LogHdlAddress(uint32_t user_long_que)
{
    p_flash_long_que = (longq_hdl_t)user_long_que;
    r_flash_spi_drvif_set_loghdladdress(user_long_que);
    return FLASH_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_flash_spi_log
* Description  : Stores error information to LONGQ buffer.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   Flash middleware file No.
*              : uint32_t           line                ;   Flash middleware line No.
* Return Value : 0                                      ;   Successful operation
*              : 1                                      ;   Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_flash_spi_log(uint32_t flg, uint32_t fid, uint32_t line)
{
    longq_err_t err = LONGQ_SUCCESS;
    uint32_t    ul_tmp = 0;

    /* Long to Byte */
    ul_tmp = 0;                                         /* Reserved               */
    ul_tmp = (ul_tmp | (line << 8));                    /* LINE                   */
    ul_tmp = (ul_tmp | (fid  << 21));                   /* FID FILE No.           */
    ul_tmp = (ul_tmp | (FLASH_SPI_DRIVER_ID << 27));    /* FID DRIVER No.         */
    ul_tmp = (ul_tmp | (flg  << 31));                   /* Breakpoint processing  */

    err = R_LONGQ_Put(p_flash_long_que, ul_tmp);
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
#else
/************************************************************************************************
* Function Name: R_FLASH_SPI_Set_LogHdlAddress
* Description  : Sets handler address.
* Arguments    : uint32_t           user_long_que       ;   Handler address
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t R_FLASH_SPI_Set_LogHdlAddress(uint32_t user_long_que)
{
    return FLASH_SPI_SUCCESS;
}
#endif  /* (FLASH_SPI_CFG_LONGQ_ENABLE == 1) */


/************************************************************************************************
* Function Name: R_FLASH_SPI_Log
* Description  : Stores error information to LONGQ buffer.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   Flash middleware file No.
*              : uint32_t           line                ;   Flash middleware line No.
* Return Value : 0                                      ;   Successful operation
*              : 1                                      ;   Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t R_FLASH_SPI_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
    R_FLASH_SPI_Log_Func(flg, fid, line);
    return 0;
}


/* End of File */
