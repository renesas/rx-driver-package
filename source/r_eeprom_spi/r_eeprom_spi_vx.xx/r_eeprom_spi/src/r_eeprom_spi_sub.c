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
* File Name    : r_eeprom_spi_sub.c
* Version      : 2.31
* Device       : -
* Abstract     : Sub module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : SPI Renesas EEPROM Sub file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : Date          No.             Comment
*              : 2004/12/06                    wrote it.
*              : 2004/12/08                    eep_Read_StsReg()
*              :                                   Changed the error return value of Status register 
*              :                                   read processing to EEPROM_SPI_ERR_OTHER.
*              :                               eep_ExchgShort()
*              :                                   Changed to do nothing in case of big endian.
*              : 2004/12/10                    eep_Write_En(), eep_Write_Di()
*              :                                   Moved from eep_usr.c.
*              : 2004/12/27                    Modified for MISRA-C.
*              : 2005/06/09                    Changed ifdef according to EEPROM capacity definition.
*              : 2005/12/12                    eep_Uart_Tx()
*              :                                   Integrated into eep_Uart_DataOut() in order to overlook 
*              :                                   transmission complete flag in case of low speed.
*              :                               eep_Send_cmd(), eep_Write_StsReg(), eep_Write_Page()
*              :                                   Changed transmission operation to eep_Uart_DataOut().
*              :                               eep_Uart_DataOut()
*              :                                   Changed to check buffer empty flag for last data transmission
*              :                                   Refer to comment for the above eep_Uart_Tx()
*              :                                   Changed to check IR flag of SIO transmission interruption 
*              :                                   in order to detecting for SIO transmission buffer empty.
*              : 2006/01/27                    eep_Set_Interrupt_1(), eep_Set_Interrupt_2()
*              :                                   Added the above functions.
*              : 2006.01.27    Ver1.10 --------------------------------------------------------
*              : 2007.02.27    Ver1.20 --------------------------------------------------------
*              : 2007.11.07    #### BU-0004    eep_Open_Port()
*              :                                   Deletes port-open processing.
*              : 2007.11.07    Ver1.21 --------------------------------------------------------
*              : 2009.01.31    #### CF-0017    Divided between SIO processing and CRC calculation.
*              : 2009.03.06    Ver1.30 --------------------------------------------------------
*              : 2011.02.15                    Changed API.
*              : 2011.02.15    Ver2.00 --------------------------------------------------------
*              : 2011.04.15                    Added #pragma SFR
*              : 2011.04.15    Ver2.01 --------------------------------------------------------
*              : 2012.03.19    Changed History to English comment.
*              : 2012.03.19    Ver2.02 --------------------------------------------------------
*              : 2013.09.30    Ver2.03 Supported IAR Embedded Workbench for Renesas RL78.
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
/*---------- Definitions of EEPROM command -----------*/
#define EEPROM_SPI_CMD_WREN        (uint8_t)(0x06)         /* Write Enable                         */
#define EEPROM_SPI_CMD_WRDI        (uint8_t)(0x04)         /* Write Disable                        */
#define EEPROM_SPI_CMD_RDSR        (uint8_t)(0x05)         /* Read Status Register                 */
#define EEPROM_SPI_CMD_WRSR        (uint8_t)(0x01)         /* Write Status Register                */
#define EEPROM_SPI_CMD_READ        (uint8_t)(0x03)         /* Read for Memory Array                */
#define EEPROM_SPI_CMD_WRITE       (uint8_t)(0x02)         /* Write for Memory Array               */

/*--------- Command transmission processing ----------*/
/*                                                         devno  cmd                   addr        cmdsize   */
#define R_EEPROM_SPI_CMD_WREN(devno) r_eeprom_spi_send_cmd(devno, EEPROM_SPI_CMD_WREN, (uint32_t)0, EEPROM_SPI_CMD_SIZE)
#define R_EEPROM_SPI_CMD_WRDI(devno) r_eeprom_spi_send_cmd(devno, EEPROM_SPI_CMD_WRDI, (uint32_t)0, EEPROM_SPI_CMD_SIZE)
#define R_EEPROM_SPI_CMD_RDSR(devno) r_eeprom_spi_send_cmd(devno, EEPROM_SPI_CMD_RDSR, (uint32_t)0, EEPROM_SPI_CMD_SIZE)
#define R_EEPROM_SPI_CMD_WRSR(devno) r_eeprom_spi_send_cmd(devno, EEPROM_SPI_CMD_WRSR, (uint32_t)0, EEPROM_SPI_CMD_SIZE)
#define R_EEPROM_SPI_CMD_READ(devno, addr, addr_size)  \
                                     r_eeprom_spi_send_cmd(devno, EEPROM_SPI_CMD_READ, (uint32_t)addr, \
                                                                                          EEPROM_SPI_CMD_SIZE+addr_size)
#define R_EEPROM_SPI_CMD_WRITE(devno, addr, addr_size) \
                                     r_eeprom_spi_send_cmd(devno, EEPROM_SPI_CMD_WRITE, (uint32_t)addr, \
                                                                                          EEPROM_SPI_CMD_SIZE+addr_size)


/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* uint16_t <-> uint8_t conversion */
typedef union
{
    uint32_t    ul;
    uint8_t     uc[4];
} eeprom_exchg_long_t;                                  /* total 4 bytes                        */


/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/


/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
static uint32_t gs_eeprom_cmdbuf[EEPROM_SPI_DEV_NUM][4/sizeof(uint32_t)];
                                                        /* Command transmission buffer          */
static eeprom_status_t r_eeprom_spi_send_cmd(uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize);
                                                        /* Command transmission                 */
static eeprom_status_t r_eeprom_spi_write_en(uint8_t devno);  /* Writing enable                 */
static void         r_eeprom_spi_cmd_set(uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize);

#ifdef EEPROM_SPI_CFG_LONGQ_ENABLE
static longq_hdl_t       p_eeprom_long_que;             /* LONGQ handler                        */
#endif  /* EEPROM_SPI_CFG_LONGQ_ENABLE */


/************************************************************************************************
* Function Name: r_eeprom_spi_init_port
* Description  : Initializes the ports related to the specified device.
* Arguments    : uint8_t           devno               ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_eeprom_spi_init_port(uint8_t devno)
{
    r_eeprom_spi_cs_init(devno);                        /* SS#   "H" /   Output                 */
}


/************************************************************************************************
* Function Name: r_eeprom_spi_reset_port
* Description  : Resets the ports related to the specified device.
* Arguments    : uint8_t           devno               ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_eeprom_spi_reset_port(uint8_t devno)
{
    r_eeprom_spi_cs_reset(devno);                       /* SS#   "H" /   Output                 */
}


/************************************************************************************************
* Function Name: r_eeprom_spi_send_cmd
* Description  : Sends a EEPROM command.
* Arguments    : uint8_t            cmd                ;   Command data
*              : uint32_t           addr               ;   address data
*              : uint8_t            cmdsize            ;   Command size
* Return Value : EEPROM_SPI_SUCCESS                    ;   Successful operation
*              : EEPROM_SPI_ERR_HARD                   ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                  ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static eeprom_status_t r_eeprom_spi_send_cmd(uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    r_eeprom_spi_cmd_set(devno, cmd, addr, cmdsize);
    
    /* The upper layer software should set to the single mode. */
    /* Send a command using the single mode. */
    /* Cast from 8-bit data to 16-bit data. */
    ret = r_eeprom_spi_drvif_tx(devno, (uint16_t)cmdsize, (uint8_t *)&gs_eeprom_cmdbuf[devno]);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_read
* Description  : Reads data from the specified address (addr) for the specified number (cnt) of bytes
*              : and stores to the specified buffer (pData).
* Arguments    : uint8_t            devno              ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info      ;   EEPROM Information
*              :    uint32_t        addr               ;   Read start address
*              :    uint32_t        cnt                ;   Number of bytes to be read
*              :    uint32_t        data_cnt           ;   Temporary Number of bytes to be read
*              :    uint8_t       * pData              ;   Read data storage buffer pointer
* Return Value : EEPROM_SPI_SUCCESS                    ;   Successful operation
*              : EEPROM_SPI_ERR_HARD                   ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                  ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_read(uint8_t devno,  eeprom_info_t * p_eeprom_info)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    r_eeprom_spi_set_cs(devno, EEPROM_SPI_LOW);               /* SS# "L"                              */
    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the READ command using the single mode. */
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            ret = R_EEPROM_SPI_CMD_READ(devno, p_eeprom_info->addr, EEPROM_SPI_DEV0_ADDR_SIZE);
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            ret = R_EEPROM_SPI_CMD_READ(devno, p_eeprom_info->addr, EEPROM_SPI_DEV1_ADDR_SIZE);
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_R_ACCESS);

    /* Set SPI mode to mode N and bit rate for Data. */
    ret =r_eeprom_spi_drvif_enable_rx_data(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    do
    {
        /* Set data size to receive. */
        /* The data size is (4G - 1) bytes or less when using Firmware Integration Technology (FIT). */
        /* The data size is 32K bytes or less when not using Firmware Integration Technology (FIT). */
        if (EEPROM_SPI_R_DATA_SIZE <= p_eeprom_info->cnt)
        {
            p_eeprom_info->data_cnt = EEPROM_SPI_R_DATA_SIZE;
        }
        else
        {
            p_eeprom_info->data_cnt = p_eeprom_info->cnt;
        }

        /* Receive data from memory array using the single mode. */
        /* The lower layer software should use the data_cnt as receiving counter. */
        ret = r_eeprom_spi_drvif_rx_data(devno, p_eeprom_info->data_cnt, p_eeprom_info->p_data);
        if (EEPROM_SPI_SUCCESS > ret)
        {
            R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
            r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
            r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);        /* SS# "H"                              */
/*          r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
            return ret;
        }

        p_eeprom_info->cnt    -= p_eeprom_info->data_cnt;     /* Updates the cnt.                     */
        p_eeprom_info->p_data += p_eeprom_info->data_cnt;
        p_eeprom_info->addr   += p_eeprom_info->data_cnt;
    }
    while(0 != p_eeprom_info->cnt); /* WAIT_LOOP */


    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
    r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);                /* SS# "H"                              */

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_write_en
* Description  : Executes a Write Enable operation.
*              : Issues the WREN command and sets the WEL bit.
* Arguments    : uint8_t            devno              ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                    ;   Successful operation
*              : EEPROM_SPI_ERR_HARD                   ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                  ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static eeprom_status_t r_eeprom_spi_write_en(uint8_t devno)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    r_eeprom_spi_set_cs(devno, EEPROM_SPI_LOW);               /* SS# "L"                              */
    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Write Enable (WREN) command using the single mode. */
    ret = R_EEPROM_SPI_CMD_WREN(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
    r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);                /* SS# "H"                              */

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_write_di
* Description  : Executes a Write Disable operation.
*              : Issues the WRDI command and clears the WEL bit.
* Arguments    : uint8_t            devno              ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                    ;   Successful operation
*              : EEPROM_SPI_ERR_HARD                   ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                  ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_write_di(uint8_t devno)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    r_eeprom_spi_set_cs(devno, EEPROM_SPI_LOW);               /* SS# "L"                              */
    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Write Disable (WRDI) command using the single mode. */
    ret = R_EEPROM_SPI_CMD_WRDI(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
    r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);                /* SS# "H"                              */

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_read_stsreg
* Description  : Reads status from the status register and stores to the read status storage buffer(p_stsreg).
*              :
*              : Memory size <= 4K-bit
*              :   Bits 7 to 4 :   Reserved (All "1")
*              :   Bits 3, 2   :   BP1, BP0
*              :                   00: No protection
*              :                   01: Upper-quarter protection
*              :                   10: Upper-half protection
*              :                   11: Whole memory protection
*              :   Bit 1       :   WEL
*              :                   0: Write disabled
*              :                   1: Write enabled
*              :   Bit 0       :   WIP
*              :                   1: During write operation
*              : Memory size > 4K-bit
*              :   Bit 7       :   SRWD
*              :                   0: Status register can be changed
*              :                   1: Status register cannot be changed
*              :   Bits 6 to 4 :   Reserved (All "0")
*              :   Bits 3, 2   :   BP1, BP0
*              :                   00: No protection
*              :                   01: Upper-quarter protection
*              :                   10: Upper-half protection
*              :                   11: Whole memory protection
*              :   Bit 1       :   WEL
*              :                   0: Write disabled
*              :                   1: Write enabled
*              :   Bit 0       :   WIP
*              :                   1: During write operation
*              :
* Arguments    : uint8_t            devno              ;   Device No. (EEPROM_SPI_DEVn)
*              : uint8_t          * p_stsreg           ;   Read status storage buffer (1 byte)
* Return Value : EEPROM_SPI_SUCCESS                    ;   Successful operation
*              : EEPROM_SPI_ERR_HARD                   ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                  ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_read_stsreg(uint8_t devno, uint8_t * p_stsreg)
{
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    r_eeprom_spi_set_cs(devno, EEPROM_SPI_LOW);               /* SS# "L"                              */
    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Read Status Register (RDSR) command using the single mode. */
    ret = R_EEPROM_SPI_CMD_RDSR(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_R_ACCESS);

    /* Receive data from the status register using the single mode. */
    ret = r_eeprom_spi_drvif_rx(devno, EEPROM_SPI_STSREG_SIZE, p_stsreg);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
    r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);                /* SS# "H"                              */

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_write_stsreg
* Description  : Writes status from the write status storage buffer(p_stsreg) to the status register.
*              : 
*              : Memory size <= 4K-bit
*              :   Bits 7 to 4 :   Reserved (All "1")
*              :   Bits 3, 2   :   BP1, BP0
*              :                   00: No protection
*              :                   01: Upper-quarter protection
*              :                   10: Upper-half protection
*              :                   11: Whole memory protection
*              :   Bits 1, 0   :    Read-only (All "0")
*              : Memory size > 4K-bit
*              :   Bit 7       :   SRWD
*              :                   0: Status register can be changed
*              :                   1: Status register cannot be changed
*              :   Bits 6 to 4 :   Reserved (All "0")
*              :   Bits 3, 2   :   BP1, BP0
*              :                   00: No protection
*              :                   01: Upper-quarter protection
*              :                   10: Upper-half protection
*              :                   11: Whole memory protection
*              :   Bits 1, 0   :   Read-only (All "0")
*              :
* Arguments    : uint8_t            devno              ;   Device No. (EEPROM_SPI_DEVn)
*              : uint8_t          * p_stsreg           ;   Write status storage buffer (1 byte)
* Return Value : EEPROM_SPI_SUCCESS                    ;   Successful operation
*              : EEPROM_SPI_ERR_HARD                   ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                  ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_write_stsreg(uint8_t devno, uint8_t * p_stsreg)
{
#ifdef EEPROM_SPI_CFG_WEL_CHK
    uint8_t             stsreg = 0;                     /* Status buffer                        */
#endif  /* #ifdef EEPROM_SPI_CFG_WEL_CHK */
    eeprom_status_t        ret = EEPROM_SPI_SUCCESS;

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_eeprom_spi_write_en(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return ret;
    }

#ifdef EEPROM_SPI_CFG_WEL_CHK
    /* Execute the Read Status Register (RDSR) command operation using the single mode. */
    ret = r_eeprom_spi_read_stsreg(devno, &stsreg);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg & EEPROM_SPI_REG_WEL))             /* WEL bit =0 : In Write Disable State  */
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return EEPROM_SPI_ERR_OTHER;
    }
#endif  /* #ifdef EEPROM_SPI_CFG_WEL_CHK */

    r_eeprom_spi_set_cs(devno, EEPROM_SPI_LOW);               /* SS# "L"                              */
    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);

    /* Set fixed data to status. */
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
     #if (defined(EEPROM_SPI_CFG_DEV0_SIZE_002K) || defined(EEPROM_SPI_CFG_DEV0_SIZE_004K))
                *p_stsreg = (*p_stsreg & (uint8_t)0x0C) | (uint8_t)0xF0;
     #else
                *p_stsreg = *p_stsreg & (uint8_t)0x8C;
     #endif
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
     #if (defined(EEPROM_SPI_CFG_DEV1_SIZE_002K) || defined(EEPROM_SPI_CFG_DEV1_SIZE_004K))
                *p_stsreg = (*p_stsreg & (uint8_t)0x0C) | (uint8_t)0xF0;
     #else
                *p_stsreg = *p_stsreg & (uint8_t)0x8C;
     #endif
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }

    /* Issue the Writes Status Register (WRSR) command using the single mode. */
    ret = R_EEPROM_SPI_CMD_WRSR(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    /* Transmit data to the status register using the single mode. */
    ret = r_eeprom_spi_drvif_tx(devno, EEPROM_SPI_STSREG_SIZE, p_stsreg);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
    r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);                /* SS# "H"                              */

    return EEPROM_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_polling
* Description  : Checks busy for the WRITE command operation.
* Arguments    : uint8_t            devno               ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : EEPROM_SPI_SUCCESS                     ;   Successful operation (EERPOM is ready)
*              : EEPROM_SPI_SUCCESS_BUSY                ;   Successful operation (EERPOM is busy)
*              : EEPROM_SPI_ERR_HARD                    ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                   ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_polling(uint8_t devno)
{
    uint8_t             rxbuf;                          /* Receive temp buffer                  */
    eeprom_status_t     ret = EEPROM_SPI_SUCCESS;

    /* Execute the Read Status Register (RDSR) command operation using the single mode. */
    ret = r_eeprom_spi_read_stsreg(devno, (uint8_t *)&rxbuf);
    if (ret == EEPROM_SPI_SUCCESS)
    {
        /* Ready/Busy check */
        if (0x00 == (rxbuf & EEPROM_SPI_REG_WIP))
        {
            ret = EEPROM_SPI_SUCCESS;                   /* Ready                                */
        }
        else
        {
            ret = EEPROM_SPI_SUCCESS_BUSY;              /* Busy                                 */
        }
    }
    else
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_write_page
* Description  : Writes data from the specified buffer (pData)
*              : to the specified address (addr) for the specified number (data_cnt) of bytes.
* Arguments    : uint8_t            devno               ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info       ;   EEPROM Information
*              :    uint32_t        addr                ;   Write start address
*              :    uint32_t        cnt                 ;   Number of bytes to be written
*              :    uint32_t        data_cnt            ;   Number of bytes to be written in a page
*              :    uint8_t       * pData               ;   Write data storage buffer pointer
* Return Value : EEPROM_SPI_SUCCESS                     ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                   ;   Parameter error
*              : EEPROM_SPI_ERR_HARD                    ;   Hardware error
*              : EEPROM_SPI_ERR_OTHER                   ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A write exceeding the write page isn't allowed.
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_write_page(uint8_t devno, eeprom_info_t * p_eeprom_info)
{
#ifdef EEPROM_SPI_CFG_WEL_CHK
    uint8_t             stsreg = 0;                     /* Status buffer                        */
#endif  /* #ifdef EEPROM_SPI_CFG_WEL_CHK */
    eeprom_status_t     ret = EEPROM_SPI_SUCCESS;

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_eeprom_spi_write_en(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return ret;
    }

#ifdef EEPROM_SPI_CFG_WEL_CHK
    /* Execute the Read Status Register (RDSR) command operation using the single mode. */
    ret = r_eeprom_spi_read_stsreg(devno, (uint8_t *)&stsreg);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg & EEPROM_SPI_REG_WEL))             /* WEL bit =0 : In Write Disable State  */
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        return EEPROM_SPI_ERR_OTHER;
    }
#endif  /* #ifdef EEPROM_SPI_CFG_WEL_CHK */

    r_eeprom_spi_set_cs(devno, EEPROM_SPI_LOW);               /* SS# "L"                              */
    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);

    /* Issue the WRITE command using the single mode. */
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            ret = R_EEPROM_SPI_CMD_WRITE(devno, p_eeprom_info->addr, EEPROM_SPI_DEV0_ADDR_SIZE);
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            ret = R_EEPROM_SPI_CMD_WRITE(devno, p_eeprom_info->addr, EEPROM_SPI_DEV1_ADDR_SIZE);
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
        return ret;
    }

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    /* Set SPI mode to mode N and bit rate for Data. */
    ret = r_eeprom_spi_drvif_enable_tx_data(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    /* Transmit data to memory using the single mode. */
    ret = r_eeprom_spi_drvif_tx_data(devno, p_eeprom_info->data_cnt, p_eeprom_info->p_data);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
        r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
        r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);            /* SS# "H"                              */
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    r_eeprom_spi_wait_lp(EEPROM_SPI_T_CS_HOLD);
    r_eeprom_spi_set_cs(devno, EEPROM_SPI_HI);                /* SS# "H"                              */

    ret = r_eeprom_spi_drvif_disable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_eeprom_spi_drvif_enable(devno);
    if (EEPROM_SPI_SUCCESS > ret)
    {
        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
/*      r_eeprom_spi_drvif_disable(devno); */  /* Remove because the upper layer software executes. */
        return ret;
    }

    return ret;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_cmd_set
* Description  : Command and address setting.
*              : Converts little endian data or big endian data.
* Arguments    : uint8_t            devno               ;   Device No. (EEPROM_SPI_DEVn)
*              : uint8_t            cmd                 ;   Command data
*              : uint32_t           addr                ;   address data
*              : uint8_t            cmdsize             ;   Command size
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_eeprom_spi_cmd_set(uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize)
{
    eeprom_exchg_long_t    tmp;
    uint8_t              * p_cmdbuf = (uint8_t *)&gs_eeprom_cmdbuf[devno][0];

    tmp.ul = addr;

    if (EEPROM_SPI_CMD_SIZE == cmdsize)
    {
        *p_cmdbuf = cmd;
    }
    else if (2 ==  cmdsize)
    {
#if (EEPROM_SPI_LITTLE_ENDIAN)
        *p_cmdbuf++   = (cmd | ((tmp.uc[1] & 0x01) << 3));  /* MSB address setting          */
        *p_cmdbuf     = tmp.uc[0];
#else
        *p_cmdbuf++   = (cmd | ((tmp.uc[2] & 0x01) << 3));  /* MSB address setting          */
        *p_cmdbuf     = tmp.uc[3];
#endif  /* #if (EEPROM_SPI_LITTLE_ENDIAN) */
    }
    else if (3 ==  cmdsize)
    {
#if (EEPROM_SPI_LITTLE_ENDIAN)
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf   = tmp.uc[0];
#else
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf   = tmp.uc[3];
#endif  /* #if (EEPROM_SPI_LITTLE_ENDIAN) */
    }
    else
    {
#if (EEPROM_SPI_LITTLE_ENDIAN)
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf   = tmp.uc[0];
#else
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf   = tmp.uc[3];
#endif  /* #if (EEPROM_SPI_LITTLE_ENDIAN) */
    }
}


/************************************************************************************************
* Function Name: r_eeprom_spi_check_cnt
* Description  : Checks counter.
* Arguments    : uint8_t            devno               ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info       ;   EEPROM Information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : EEPROM_SPI_SUCCESS                     ;   Successful operation
*              : EEPROM_SPI_ERR_PARAM                   ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_check_cnt(uint8_t devno, eeprom_info_t  * p_eeprom_info)
{
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            if ((0 == p_eeprom_info->cnt) || (EEPROM_SPI_DEV0_MEM_SIZE < p_eeprom_info->cnt) ||
                ((EEPROM_SPI_DEV0_MEM_SIZE - p_eeprom_info->cnt) < p_eeprom_info->addr))
            {
                R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
                return EEPROM_SPI_ERR_PARAM;
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            if ((0 == p_eeprom_info->cnt) || (EEPROM_SPI_DEV1_MEM_SIZE < p_eeprom_info->cnt) ||
                ((EEPROM_SPI_DEV1_MEM_SIZE - p_eeprom_info->cnt) < p_eeprom_info->addr))
            {
                R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
                return EEPROM_SPI_ERR_PARAM;
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_SUB, __LINE__);
            return EEPROM_SPI_ERR_PARAM;
        break;
    }
    return EEPROM_SPI_SUCCESS;
}



/************************************************************************************************
* Function Name: r_eeprom_spi_check_wp
* Description  : Checks write protection.
* Arguments    : uint8_t            devno               ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info       ;   EEPROM Information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : EEPROM_SPI_SUCCESS                     ;   Successful operation
*              : EEPROM_SPI_ERR_WP                      ;   Write-protection error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_check_wp(uint8_t devno, eeprom_info_t  * p_eeprom_info)
{
    uint32_t            endwaddr = 0;                   /* Writing end address                  */
    uint8_t             stsreg = 0;

    endwaddr = p_eeprom_info->addr + p_eeprom_info->cnt - 1;
    stsreg = (stsreg >> 2) & (uint8_t)0x03;
    if (EEPROM_SPI_WP_NONE != stsreg)                   /* Write-protection state ?             */
    {
        if (EEPROM_SPI_WP_UPPER_QUART == stsreg)        /* Upper 1/4 protecting ?               */
        {
            switch (devno)
            {
                case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
                    if (EEPROM_SPI_DEV0_UPPER_QUART <= endwaddr)
                    {
                        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
                        return EEPROM_SPI_ERR_WP;
                    }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
                break;
                case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
                    if (EEPROM_SPI_DEV1_UPPER_QUART <= endwaddr)
                    {
                        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
                        return EEPROM_SPI_ERR_WP;
                    }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
                break;
                default:
                    /* Do nothing. */
                break;
            }
        }
        else if (EEPROM_SPI_WP_UPPER_HALF == stsreg)    /* Upper 1/2 protecting ?               */
        {
            switch (devno)
            {
                case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
                    if (EEPROM_SPI_DEV0_UPPER_HALF <= endwaddr)
                    {
                        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
                        return EEPROM_SPI_ERR_WP;
                    }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
                break;
                case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
                    if (EEPROM_SPI_DEV1_UPPER_HALF <= endwaddr)
                    {
                        R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
                        return EEPROM_SPI_ERR_WP;
                    }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
                break;
                default:
                    /* Do nothing. */
                break;
            }
        }
        else                                            /* Whole memory protecting              */
        {
            R_EEPROM_SPI_Log_Func(EEPROM_SPI_DEBUG_ERR_ID, (uint32_t)EEPROM_SPI_USER, __LINE__);
            return EEPROM_SPI_ERR_WP;
        }
    }
    return EEPROM_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_page_calc
* Description  : Calculates the writable bytes from the start address to the page boundary.
* Arguments    : uint8_t            devno               ;   Device No. (EEPROM_SPI_DEVn)
*              : eeprom_info_t    * p_eeprom_info       ;   EEPROM Information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : Writable size (bytes)
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_eeprom_spi_page_calc(uint8_t devno, eeprom_info_t  * p_eeprom_info)
{
    uint32_t    tmpcnt = 0;

    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            tmpcnt = ((p_eeprom_info->addr + EEPROM_SPI_DEV0_WPAG_SIZE) / EEPROM_SPI_DEV0_WPAG_SIZE) 
                     * EEPROM_SPI_DEV0_WPAG_SIZE - p_eeprom_info->addr;
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            tmpcnt = ((p_eeprom_info->addr + EEPROM_SPI_DEV1_WPAG_SIZE) / EEPROM_SPI_DEV1_WPAG_SIZE) 
                     * EEPROM_SPI_DEV1_WPAG_SIZE - p_eeprom_info->addr;
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
    return tmpcnt;
}


/************************************************************************************************
* Function Name: R_EEPROM_SPI_1ms_Interval
* Description  : 1ms Interval Timer call function.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : Please call the function when using DMAC or DTC.
*************************************************************************************************/
void R_EEPROM_SPI_1ms_Interval(void)
{
    r_eeprom_spi_drvif_1ms_interval();
}


#ifdef EEPROM_SPI_CFG_LONGQ_ENABLE
/************************************************************************************************
* Function Name: R_EEPROM_SPI_Set_LogHdlAddress
* Description  : Sets handler address.
* Arguments    : uint32_t           user_long_que       ;   Handler address
* Return Value : EEPROM_SPI_SUCCESS                     ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Set_LogHdlAddress(uint32_t user_long_que)
{
    p_eeprom_long_que = (longq_hdl_t)user_long_que;
    r_eeprom_spi_drvif_set_loghdladdress(user_long_que);
    return EEPROM_SPI_SUCCESS;
}


/************************************************************************************************
* Function Name: r_eeprom_spi_log
* Description  : Stores error information to LONGQ buffer.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   EEPROM middleware file No.
*              : uint32_t           line                ;   EEPROM middleware line No.
* Return Value : 0                                      ;   Successful operation
*              : 1                                      ;   Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_eeprom_spi_log(uint32_t flg, uint32_t fid, uint32_t line)
{
    longq_err_t err;
    uint32_t    ul_tmp;

    /* Long to Byte */
    ul_tmp = 0;                                         /* Reserved               */
    ul_tmp = (ul_tmp | (line << 8));                    /* LINE                   */
    ul_tmp = (ul_tmp | (fid  << 21));                   /* FID FILE No.           */
    ul_tmp = (ul_tmp | (EEPROM_SPI_DRIVER_ID << 27));   /* FID DRIVER No.         */
    ul_tmp = (ul_tmp | (flg  << 31));                   /* Breakpoint processing  */

    err = R_LONGQ_Put(p_eeprom_long_que, ul_tmp);
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
* Function Name: R_EEPROM_SPI_Set_LogHdlAddress
* Description  : Sets handler address.
* Arguments    : uint32_t           user_long_que       ;   Handler address
* Return Value : EEPROM_SPI_SUCCESS                     ;   Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Set_LogHdlAddress(uint32_t user_long_que)
{
    return EEPROM_SPI_SUCCESS;
}
#endif  /* EEPROM_SPI_CFG_LONGQ_ENABLE */


/************************************************************************************************
* Function Name: r_eeprom_spi_log
* Description  : Stores error information to LONGQ buffer.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   EEPROM middleware file No.
*              : uint32_t           line                ;   EEPROM middleware line No.
* Return Value : 0                                      ;   Successful operation
*              : 1                                      ;   Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t R_EEPROM_SPI_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
    R_EEPROM_SPI_Log_Func(flg, fid, line);
    return 0;
}


/* End of File */
