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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : FLASH SPI driver software
* File Name    : r_flash_spi_at_type_sub.c
* Version      : 3.40
* Device       : -
* Abstract     : Sub module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : SPI FLASH Sub file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 16.03.2023 1.00     Created
*              : 15.11.2023 3.40     Added support for reading across multiple banks in QSPIX
*                                    Memory Mapped Mode.
*                                    Updated according to GSCE Code Checker 6.50.
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_flash_spi_if.h"                     /* FLASH driver I/F definitions                 */
#include "r_flash_spi_config.h"                 /* FLASH driver Configuration definitions       */
#include "../../r_flash_spi_private.h"          /* FLASH driver Private module definitions      */


/* Check flash memory types. */
#if (FLASH_SPI_CFG_DEV0_AT25QF == 1) || (FLASH_SPI_CFG_DEV1_AT25QF == 1)


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/*---------- Definitions of FLASH command -----------*/
#define FLASH_SPI_AT_CMD_WREN   ((uint8_t)(0x06)) /* Write Enable                                 */
#define FLASH_SPI_AT_CMD_WRDI   ((uint8_t)(0x04)) /* Write Disable                                */
#define FLASH_SPI_AT_CMD_RDSR1  ((uint8_t)(0x05)) /* Read Status Register 1                       */
#define FLASH_SPI_AT_CMD_RDSR2  ((uint8_t)(0x35)) /* Read Status Register 2                       */
#define FLASH_SPI_AT_CMD_RDSR3  ((uint8_t)(0x15)) /* Read Status Register 3                       */
#define FLASH_SPI_AT_CMD_WRSR1  ((uint8_t)(0x01)) /* Write Status Register 1                      */
#define FLASH_SPI_AT_CMD_WRSR2  ((uint8_t)(0x31)) /* Write Status Register 2                      */
#define FLASH_SPI_AT_CMD_WRSR3  ((uint8_t)(0x11)) /* Write Status Register 3                      */
#define FLASH_SPI_AT_CMD_FREAD  ((uint8_t)(0x0b)) /* Read Data at Higher Speed                    */
#define FLASH_SPI_AT_CMD_DREAD  ((uint8_t)(0x3b)) /* Dual Read (Single -> Dual Out)               */
#define FLASH_SPI_AT_CMD_QREAD  ((uint8_t)(0x6b)) /* Quad Read (Single -> Quad Out)               */
#define FLASH_SPI_AT_CMD_RDSCUR ((uint8_t)(0x48)) /* Read Data at Security Register Page          */
#define FLASH_SPI_AT_CMD_PP     ((uint8_t)(0x02)) /* Page Program (Single -> Single Input)        */
#define FLASH_SPI_AT_CMD_QPP    ((uint8_t)(0x32)) /* Quad Page Program (Single -> Quad Input)     */
#define FLASH_SPI_AT_CMD_PSCUR  ((uint8_t)(0x42)) /* Program Security Register                    */
#define FLASH_SPI_AT_CMD_ESCUR  ((uint8_t)(0x44)) /* Erase Security Register                      */
#define FLASH_SPI_AT_CMD_BE4K   ((uint8_t)(0x20)) /* Block Erase (4KB)                            */
#define FLASH_SPI_AT_CMD_BE32K  ((uint8_t)(0x52)) /* Block Erase (32KB)                           */
#define FLASH_SPI_AT_CMD_BE64K  ((uint8_t)(0xd8)) /* Block Erase (64KB)                           */
#define FLASH_SPI_AT_CMD_CE     ((uint8_t)(0x60)) /* Chip Erase                                   */
#define FLASH_SPI_AT_CMD_RDID   ((uint8_t)(0x9f)) /* Read Identification                          */


/*--------- Command transmission processing ----------*/
#define R_FLASH_SPI_AT_CMD_WREN(devno, read_after_write)      (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_WREN,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_WRDI(devno, read_after_write)      (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_WRDI,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_RDSR1(devno, read_after_write)     (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_RDSR1,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_RDSR2(devno, read_after_write)     (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_RDSR2,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_RDSR3(devno, read_after_write)     (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_RDSR3,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_WRSR1(devno, read_after_write)     (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_WRSR1,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_WRSR2(devno, read_after_write)     (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_WRSR2,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_WRSR3(devno, read_after_write)     (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_WRSR3,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_ESCUR(devno, addr, addr_size, read_after_write)   (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_ESCUR, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_BE4K(devno, addr, addr_size, read_after_write)    (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_BE4K, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_BE32K(devno, addr, addr_size, read_after_write) (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_BE32K, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_BE64K(devno, addr, addr_size, read_after_write) (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_BE64K, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_CE(devno, addr, read_after_write)  (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_CE,   (uint32_t)addr, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
#define R_FLASH_SPI_AT_CMD_RDID(devno, read_after_write)      (r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_RDID,   (uint32_t)0, \
                                                            FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE))
/* READ Command */
/* 1 Dummy cycle is indicated as "1". */
#define R_FLASH_SPI_AT_CMD_FREAD(devno, addr, addr_size, read_after_write) r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_FREAD, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size+1, read_after_write, TRUE)
#define R_FLASH_SPI_AT_CMD_DREAD(devno, addr, addr_size, read_after_write) r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_DREAD, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size+1, read_after_write, TRUE)
#define R_FLASH_SPI_AT_CMD_QREAD(devno, addr, addr_size, read_after_write) r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_QREAD, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size+1, read_after_write, TRUE)
#define R_FLASH_SPI_AT_CMD_RDSCUR(devno, addr, addr_size, read_after_write) r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_RDSCUR, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE+addr_size+1, read_after_write, FALSE)
/* Page Program Command */
#define R_FLASH_SPI_AT_CMD_PP(devno, addr, addr_size, read_after_write)   r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_PP,  \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE)
#define R_FLASH_SPI_AT_CMD_QPP(devno, addr, addr_size, read_after_write)  r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_QPP, \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE)
#define R_FLASH_SPI_AT_CMD_PSCUR(devno, addr, addr_size, read_after_write)   r_flash_spi_at_send_cmd(devno, FLASH_SPI_AT_CMD_PSCUR,  \
                                                            (uint32_t)addr, FLASH_SPI_AT_CMD_SIZE, read_after_write, FALSE)


/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* uint32_t <-> uint8_t conversion */
typedef union
{
    uint32_t    ul;
    uint8_t     uc[4];
} flash_spi_at_exchg_long_t;                            /* total 4 bytes                       */


/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/


/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
static bool s_flash_at_read_memory_mapped[FLASH_SPI_DEV_NUM] =
{
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
#if (MEMDRV_DRVR_RX_FIT_QSPIX_MMM == MEMDRV_CFG_DEV0_MODE_DRVR)
    true,
#else
    false,
#endif
#if (MEMDRV_DRVR_RX_FIT_QSPIX_MMM == MEMDRV_CFG_DEV1_MODE_DRVR)
    true
#else
    false
#endif
#elif (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
#if (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPIX_MMM)
    true
#else
    false
#endif
#else
#endif
};

/* Command transmission buffer */
static uint32_t s_flash_at_cmdbuf[FLASH_SPI_DEV_NUM][8/sizeof(uint32_t)];

/* Command transmission */
static flash_spi_status_t r_flash_spi_at_send_cmd (uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize,
                                                  bool read_after_write, bool read_in_memory_mapped);

/* Writing enable */
static flash_spi_status_t r_flash_spi_at_write_en (uint8_t devno);

/* Wait for ready after busy by program or erase operation */
static flash_spi_status_t r_flash_spi_at_poll_prog_erase (uint8_t devno, flash_spi_poll_mode_t mode);

/* Wait for ready after register write operation */
static flash_spi_status_t r_flash_spi_at_poll_reg_write (uint8_t devno);

/* Set command */
static void               r_flash_spi_at_cmd_set (uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize);


/************************************************************************************************
* Function Name: r_flash_spi_at_init_port
* Description  : Initializes the ports related to the specified device.
* Arguments    : uint8_t           devno                ;   Device No. (FLASH_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_spi_at_init_port(uint8_t devno)
{
    r_flash_spi_cs_init(devno);                         /* SS# "H"                              */
}
/******************************************************************************
 End of function r_flash_spi_at_init_port
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_reset_port
* Description  : Resets the ports related to the specified device.
* Arguments    : uint8_t           devno                ;   Device No. (FLASH_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_spi_at_reset_port(uint8_t devno)
{
    r_flash_spi_cs_reset(devno);                        /* SS# "H"                              */
}
/******************************************************************************
 End of function r_flash_spi_at_reset_port
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_send_cmd
* Description  : Sends a FLASH command.
* Arguments    : uint8_t            cmd                   ;   Command data
*              : uint32_t           addr                  ;   Address data
*              : uint8_t            cmdsize               ;   Command size
*              : bool               read_after_write      ;   Control SPI bus
*              : bool               read_in_memory_mapped ;   Read access in memory-mapped mode
* Return Value : FLASH_SPI_SUCCESS                        ;   Successful operation
*              : FLASH_SPI_ERR_HARD                       ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static flash_spi_status_t r_flash_spi_at_send_cmd(uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize, bool read_after_write, bool read_in_memory_mapped)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;

    r_flash_spi_at_cmd_set(devno, cmd, addr, cmdsize);
    
    /* The upper layer software should set to the single mode. */
    /* Send a command using the single mode. */
    /* Cast from 8-bit data to 32-bit data. */
    ret = r_flash_spi_drvif_tx(devno, (uint32_t)cmdsize, (uint8_t *)&s_flash_at_cmdbuf[devno][0], read_after_write, read_in_memory_mapped);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_send_cmd
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_en
* Description  : Executes a Write Enable operation.
*              : Issues the WREN command and sets the WEL bit.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static flash_spi_status_t r_flash_spi_at_write_en(uint8_t devno)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Write Enable (WREN) command using the single mode. */
    ret = R_FLASH_SPI_AT_CMD_WREN(devno, FALSE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_write_en
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_di
* Description  : Executes a Write Disable operation.
*              : Issues the WRDI command and clears the WEL bit.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_write_di(uint8_t devno)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Write Disable (WRDI) command using the single mode. */
    ret = R_FLASH_SPI_AT_CMD_WRDI(devno, FALSE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_write_di
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_read_stsreg1
* Description  : Reads status from the status register 1 and stores to the read status storage buffer(p_status).
*              :
*              :        Bit 7: Status Register Protection (SRP0)
*              :        Bit 6: Block Protection (SEC)
*              :        Bit 5: Top or Bottom Protection (TB)
*              :        Bits 4 to 2: Block protect 2 - 0 (BP2 - BP0)
*              :                  Set to 1, a designated memory area is protected from PROGRAM and ERASE operations.
*              :        Bit 1: Write Enable Latch Status (WEL)
*              :                  1: Device is Write Enabled.
*              :                  0: Device is not Write Enabled (default).
*              :        Bit 0: Ready/Busy Status (RDY/BSY)
*              :                  1: Device is busy with an internal operation.
*              :                  0: Device is ready.
*              :
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t          * p_status            ;   Read status register 1 storage buffer (1 byte)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_read_stsreg1(uint8_t devno, uint8_t * p_status)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Read Status Register 1 (RDSR1) command using the single mode. */
    ret = R_FLASH_SPI_AT_CMD_RDSR1(devno, TRUE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_R_ACCESS);

    /* Receive data from the status register 1 using the single mode. */
    ret = r_flash_spi_drvif_rx(devno, FLASH_SPI_AT_STSREG1_SIZE, p_status);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_read_stsreg1
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_read_stsreg2
* Description  : Reads status from the status register 2 and stores to the read status storage buffer(p_status).
*              :
*              :        Bit 7: Erase Suspend Status (E_SUS)
*              :                  1: Erase operation is suspended.
*              :                  0: Erase operation is not suspended (default).
*              :        Bit 6: Complement Block Protection (CMP)
*              :        Bit 5: Lock Security Register 3 (LB3)
*              :                  1: Security Register page-3 cannot be erased/programmed.
*              :                  0: Security Register page-3 is not locked (default).
*              :        Bit 4: Lock Security Register 2 (LB2)
*              :                  1: Security Register page-2 cannot be erased/programmed.
*              :                  0: Security Register page-2 is not locked (default).
*              :        Bit 3: Lock Security Register 1 (LB1)
*              :                  1: Security Register page-1 cannot be erased/programmed.
*              :                  0: Security Register page-1 is not locked (default).
*              :        Bit 2: Program Suspend Status (P_SUS)
*              :                  1: Program operation is suspended.
*              :                  0: Program operation is not suspended (default).
*              :        Bit 1: Quad Enable (QE)
*              :                  1: HOLD and WP function are I/O pins (default).
*              :                  0: HOLD and WP function normally.
*              :        Bit 0: Status Register Protect bit 1 (SRP1)
*              :
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t          * p_status            ;   Read status register 2 storage buffer (1 byte)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_read_stsreg2(uint8_t devno, uint8_t * p_status)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Read Status Register 2 (RDSR2) command using the single mode. */
    ret = R_FLASH_SPI_AT_CMD_RDSR2(devno, TRUE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_R_ACCESS);

    /* Receive data from the status register 2 using the single mode. */
    ret = r_flash_spi_drvif_rx(devno, FLASH_SPI_AT_STSREG2_SIZE, p_status);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_read_stsreg2
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_read_stsreg3
* Description  : Reads status from the status register 3 and stores to the read status storage buffer(p_status).
*              :
*              :        Bit 7: Reserved
*              :        Bits 6 to 5: Drive Strength (DRV[1:0])
*              :                 11: Auto (7pF base on VCC level.
*              :                 10: 50% (15pF).
*              :                 01: 75% (22pF).
*              :                 00: 100% (30pF).
*              :        Bits 4 to 0: Reserved
*              :
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t          * p_status            ;   Read status register 3 storage buffer (1 byte)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_read_stsreg3(uint8_t devno, uint8_t * p_status)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Read Status Register 3 (RDSR3) command using the single mode. */
    ret = R_FLASH_SPI_AT_CMD_RDSR3(devno, TRUE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_R_ACCESS);

    /* Receive data from the status register 3 using the single mode. */
    ret = r_flash_spi_drvif_rx(devno, FLASH_SPI_AT_STSREG3_SIZE, p_status);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_read_stsreg3
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_set_write_protect
* Description  : Makes a write-protection setting using Block Lock (BP) protection mode.
*              : The BP2, BP1 and BP0 bit of status register are set as follows by
*              : write-protection setting data (wpsts).
*              :
*              :    wpsts=0x00  :   BP2=0, BP1=0, BP0=0
*              :    wpsts=0x01  :   BP2=0, BP1=0, BP0=1
*              :    wpsts=0x02  :   BP2=0, BP1=1, BP0=0
*              :    wpsts=0x03  :   BP2=0, BP1=1, BP0=1
*              :    wpsts=0x04  :   BP2=1, BP1=0, BP0=0
*              :    wpsts=0x05  :   BP2=1, BP1=0, BP0=1
*              :    wpsts=0x06  :   BP2=1, BP1=1, BP0=0
*              :    wpsts=0x07  :   BP2=1, BP1=1, BP0=1
*              :
*              : The TB bit of status register 1 is set.
*              :    TB (Top/Bottom)
*              :        1: Bottom area protect
*              :        0: Top area protect
*              : The SEC bit of status register 1 is set.
*              :    SEC (Block Protection)
*              :        1: Select small block size protection
*              :        0: Select large block size protection
*              :
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t            wpsts               ;   Write-protection setting data
*              : bool               read_after_write    ;   Control SPI bus
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_set_write_protect(uint8_t devno, uint8_t wpsts, bool read_after_write)
{
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;
    uint8_t                 reg;
    uint8_t                 stsreg1 = 0;

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        if (wpsts > FLASH_SPI_AT_DEV0_WP_WHOLE_MEM)
        {
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            return FLASH_SPI_ERR_PARAM;
        }
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        if (wpsts > FLASH_SPI_AT_DEV1_WP_WHOLE_MEM)
        {
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            return FLASH_SPI_ERR_PARAM;
        }
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, &stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Store the write-protection information. */
    reg = ((stsreg1 & (uint8_t)(~(FLASH_SPI_AT_REG1_BPMASK | FLASH_SPI_AT_REG1_SRP0))) | (wpsts << 2));

    /* Use the Block lock (BP) protection mode as the write protection. */
    /* Therefore set Status Register Protection bit 0 & 1 (SRP0, SRP1) to 0 (default). */
    /* Execute the Write Status Register 1 (WRSR1) command operation using the single mode. */
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode
       in r_flash_spi_at_write_stsreg1(). */
    ret = r_flash_spi_at_write_stsreg1(devno, &reg, read_after_write);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    
    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_set_write_protect
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_quad_enable
* Description  : Makes a setting using Quad Enable (QE Bit) mode.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
*              : bool            read_after_write       ;   Control SPI bus
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register and configuration register after this processing.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_quad_enable(uint8_t devno, bool read_after_write)
{
    flash_spi_status_t      ret     = FLASH_SPI_SUCCESS;
    uint8_t                 reg2    = 0;
    uint8_t                 stsreg2 = 0;

    /* Execute the Read Status Register 2 (RDSR2) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg2(devno, &stsreg2);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Store the write-protection information. */
    reg2  = (stsreg2 | FLASH_SPI_AT_REG2_QE);

    /* Use the Block lock (BP) protection mode as the write protection. */
    /* Therefore set Status Register Protection bit 0 & 1 (SRP0, SRP1) to 0 (default). */
    /* Execute the Write Status Register 2 (WRSR2) command operation using the single mode. */
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode
       in r_flash_spi_at_write_stsreg2(). */
    ret = r_flash_spi_at_write_stsreg2(devno, &reg2, read_after_write);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    
    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_quad_enable
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_quad_disable
* Description  : Makes a setting using Quad Disable (QE Bit) mode.
* Arguments    : uint8_t         devno                  ;   Device No. (FLASH_SPI_DEVn)
*              : bool            read_after_write       ;   Control SPI bus
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A SRWD bit is fixed to 0.
*              : Please confirm the status register and configuration register after this processing.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_quad_disable(uint8_t devno, bool read_after_write)
{
    flash_spi_status_t      ret     = FLASH_SPI_SUCCESS;
    uint8_t                 reg2    = 0;
    uint8_t                 stsreg2 = 0;

    /* Execute the Read Status Register (RDSR) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg2(devno, &stsreg2);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Store the write-protection information. */
    reg2 = (uint8_t)(stsreg2 & (uint8_t)~FLASH_SPI_AT_REG2_QE);

    /* Use the Block lock (BP) protection mode as the write protection. */
    /* Therefore set Status Register Protection bit 0 & 1 (SRP0, SRP1) to 0 (default). */
    /* Execute the Write Status Register (WRSR) command operation using the single mode. */
    /* Execute the Read Status Register (RDSR) command operation using the single mode 
       in r_flash_spi_at_write_stsreg2(). */
    ret = r_flash_spi_at_write_stsreg2(devno, &reg2, read_after_write);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    
    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_quad_disable
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_stsreg1
* Description  : Writes status from the write status storage buffer to the status register 1.
*              : 
*              :        Bit 7: Status Register Protection (SRP0)
*              :        Bit 6: Block Protection (SEC)
*              :        Bit 5: Top or Bottom Protection (TB)
*              :        Bits 4 to 2: Block protect 2 - 0 (BP2 - BP0)
*              :                  Set to 1, a designated memory area is protected from PROGRAM and ERASE operations.
*              :        Bit 1: Write Enable Latch Status (WEL)
*              :                  1: Device is Write Enabled.
*              :                  0: Device is not Write Enabled (default).
*              :        Bit 0: Ready/Busy Status (RDY/BSY)
*              :                  1: Device is busy with an internal operation.
*              :                  0: Device is ready.
*              :
* Arguments    : uint8_t     devno                ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t     * p_reg              ;   Status register 1 setting data buffer (1 byte)
*              : bool        read_after_write     ;   Control SPI bus
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_write_stsreg1(uint8_t devno, uint8_t * p_reg, bool read_after_write)
{
#if (FLASH_SPI_CFG_WEL_CHK == 1)
    uint8_t             stsreg1 = 0;                     /* Status buffer                        */
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             data_size = 0;
    uint8_t             data_buff[2];

    data_buff[0] = 0;
    data_buff[1] = 0;

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_flash_spi_at_write_en(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

#if (FLASH_SPI_CFG_WEL_CHK == 1)
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, &stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg1 & FLASH_SPI_AT_REG1_WEL))        /* WEL bit =0 : In Write Disable State  */
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_OTHER;
    }
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Sets configuration fixed data for writing. */
    data_buff[0] = FLASH_SPI_AT_CMD_WRSR1;
    data_buff[1] = *p_reg | FLASH_SPI_AT_REG1_WEL;
    data_size    = FLASH_SPI_AT_CMD_SIZE + FLASH_SPI_AT_WSTSREG1_SIZE;

    /* Transmit data to the status register 1 using the single mode. */
    ret = r_flash_spi_drvif_tx(devno, data_size, &data_buff[0], read_after_write, FALSE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return FLASH_SPI_SUCCESS;
}
/******************************************************************************
 End of function r_flash_spi_at_write_stsreg1
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_stsreg2
* Description  : Writes status from the write status storage buffer to the status register 2.
*              :
*              :        Bit 7: Erase Suspend Status (E_SUS)
*              :                  1: Erase operation is suspended.
*              :                  0: Erase operation is not suspended (default).
*              :        Bit 6: Complement Block Protection (CMP)
*              :        Bit 5: Lock Security Register 3 (LB3)
*              :                  1: Security Register page-3 cannot be erased/programmed.
*              :                  0: Security Register page-3 is not locked (default).
*              :        Bit 4: Lock Security Register 2 (LB2)
*              :                  1: Security Register page-2 cannot be erased/programmed.
*              :                  0: Security Register page-2 is not locked (default).
*              :        Bit 3: Lock Security Register 1 (LB1)
*              :                  1: Security Register page-1 cannot be erased/programmed.
*              :                  0: Security Register page-1 is not locked (default).
*              :        Bit 2: Program Suspend Status (P_SUS)
*              :                  1: Program operation is suspended.
*              :                  0: Program operation is not suspended (default).
*              :        Bit 1: Quad Enable (QE)
*              :                  1: HOLD and WP function are I/O pins (default).
*              :                  0: HOLD and WP function normally.
*              :        Bit 0: Status Register Protect bit 1 (SRP1)
*              :
* Arguments    : uint8_t     devno                ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t     * p_reg              ;   Status register 2 setting data buffer (1 byte)
*              : bool        read_after_write     ;   Control SPI bus
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_write_stsreg2(uint8_t devno, uint8_t * p_reg, bool read_after_write)
{
#if (FLASH_SPI_CFG_WEL_CHK == 1)
    uint8_t             stsreg1 = 0;                     /* Status buffer                        */
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             data_size = 0;
    uint8_t             data_buff[2];

    data_buff[0] = 0;
    data_buff[1] = 0;

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_flash_spi_at_write_en(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

#if (FLASH_SPI_CFG_WEL_CHK == 1)
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, &stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg1 & FLASH_SPI_AT_REG1_WEL))        /* WEL bit =0 : In Write Disable State  */
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_OTHER;
    }
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Sets configuration fixed data for writing. */
    data_buff[0] = FLASH_SPI_AT_CMD_WRSR2;
    data_buff[1] = *p_reg;
    data_size    = FLASH_SPI_AT_CMD_SIZE + FLASH_SPI_AT_WSTSREG2_SIZE;

    /* Transmit data to the status register 2 using the single mode. */
    ret = r_flash_spi_drvif_tx(devno, data_size, &data_buff[0], read_after_write, FALSE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return FLASH_SPI_SUCCESS;
}
/******************************************************************************
 End of function r_flash_spi_at_write_stsreg2
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_stsreg3
* Description  : Writes status from the write status storage buffer to the status register 3.
*              :
*              :        Bit 7: Reserved
*              :        Bits 6 to 5: Drive Strength (DRV[1:0])
*              :                 11: Auto (7pF base on VCC level.
*              :                 10: 50% (15pF).
*              :                 01: 75% (22pF).
*              :                 00: 100% (30pF).
*              :        Bits 4 to 0: Reserved
*              :
* Arguments    : uint8_t     devno                ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t     * p_reg              ;   Status register 3 setting data buffer (1 byte)
*              : bool        read_after_write     ;   Control SPI bus
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_write_stsreg3(uint8_t devno, uint8_t * p_reg, bool read_after_write)
{
#if (FLASH_SPI_CFG_WEL_CHK == 1)
    uint8_t             stsreg1 = 0;                     /* Status buffer                        */
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             data_size = 0;
    uint8_t             data_buff[2];

    data_buff[0] = 0;
    data_buff[1] = 0;

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_flash_spi_at_write_en(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

#if (FLASH_SPI_CFG_WEL_CHK == 1)
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, &stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg1 & FLASH_SPI_AT_REG1_WEL))        /* WEL bit =0 : In Write Disable State  */
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_OTHER;
    }
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Sets configuration fixed data for writing. */
    data_buff[0] = FLASH_SPI_AT_CMD_WRSR3;
    data_buff[1] = *p_reg;
    data_size    = FLASH_SPI_AT_CMD_SIZE + FLASH_SPI_AT_WSTSREG3_SIZE;

    /* Transmit data to the status register 3 using the single mode. */
    ret = r_flash_spi_drvif_tx(devno, data_size, &data_buff[0], read_after_write, FALSE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return FLASH_SPI_SUCCESS;
}
/******************************************************************************
 End of function r_flash_spi_at_write_stsreg3
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_read
* Description  : Reads data from the specified address (addr) for the specified number (cnt) of bytes
*              : and stores to the specified buffer (pData).
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Temporary Number of bytes to be read
*              :    uint8_t       * pData               ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_read(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;
    uint32_t    tmpcnt    = 0;
    uint8_t     addr_size = 0;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the READ command using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV0_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV1_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    if (FLASH_SPI_SINGLE == p_flash_spi_info->op_mode)
    {
        ret = R_FLASH_SPI_AT_CMD_FREAD(devno, p_flash_spi_info->addr, addr_size, TRUE);
    }
    else if (FLASH_SPI_DUAL == p_flash_spi_info->op_mode)
    {
        ret = R_FLASH_SPI_AT_CMD_DREAD(devno, p_flash_spi_info->addr, addr_size, TRUE);
    }
    else
    {
        ret = R_FLASH_SPI_AT_CMD_QREAD(devno, p_flash_spi_info->addr, addr_size, TRUE);
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_R_ACCESS);

    /* Set SPI mode to mode N and bit rate for Data. */
    ret =r_flash_spi_drvif_enable_rx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    do
    {
        if(true == s_flash_at_read_memory_mapped[devno])
        {
            /* Calculates the writable bytes from the start address to the bank boundary */
            tmpcnt = ((p_flash_spi_info->addr + FLASH_SPI_R_DATA_QSPIX_BANK_SIZE) / FLASH_SPI_R_DATA_QSPIX_BANK_SIZE)
                     * FLASH_SPI_R_DATA_QSPIX_BANK_SIZE - p_flash_spi_info->addr;

            /* Set data size to receive. */
            /* The data size is 64 Mbytes or less when using a read memory map in the QSPI area. */
            if (tmpcnt >= p_flash_spi_info->cnt)
            {
                p_flash_spi_info->data_cnt = p_flash_spi_info->cnt;
            }
            else
            {
                p_flash_spi_info->data_cnt = tmpcnt;
            }
        }
        else
        {
            /* Set data size to receive. */
            /* The data size is (4G - 1) bytes or less when using Firmware Integration Technology (FIT). */
            /* The data size is 32K bytes or less when not using Firmware Integration Technology (FIT). */
            if (FLASH_SPI_R_DATA_SIZE <= p_flash_spi_info->cnt)
            {
                p_flash_spi_info->data_cnt = FLASH_SPI_R_DATA_SIZE;
            }
            else
            {
                p_flash_spi_info->data_cnt = p_flash_spi_info->cnt;
            }
        }

        /* Receive data from memory array. */
        /* The lower layer software should use the data_cnt as receiving counter. */
        ret = r_flash_spi_drvif_rx_data(devno, p_flash_spi_info->data_cnt, p_flash_spi_info->p_data,
                                        p_flash_spi_info->addr, addr_size, p_flash_spi_info->op_mode);
        if (FLASH_SPI_SUCCESS > ret)
        {
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
            r_flash_spi_set_cs(devno, FLASH_SPI_HI);    /* SS# "H"                              */
            return ret;
        }

        if(true == s_flash_at_read_memory_mapped[devno])
        {
            r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
            r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */
        }

        p_flash_spi_info->cnt    -= p_flash_spi_info->data_cnt;     /* Updates the cnt.         */
        p_flash_spi_info->p_data += p_flash_spi_info->data_cnt;
        p_flash_spi_info->addr   += p_flash_spi_info->data_cnt;

        if((true == s_flash_at_read_memory_mapped[devno]) && (0 != p_flash_spi_info->cnt))
        {
            r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
            r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        }
    }
    while(0 != p_flash_spi_info->cnt); /* WAIT_LOOP */

    if(true != s_flash_at_read_memory_mapped[devno])
    {
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_read
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_read_scurreg_page
* Description  : Reads data from the specified address (addr) for the specified number (cnt) of bytes
*              : and stores to the specified buffer (pData).
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Temporary Number of bytes to be read
*              :    uint8_t       * pData               ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_read_scurreg_page(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    flash_spi_status_t        ret = FLASH_SPI_SUCCESS;
    uint8_t     addr_size = 0;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Issue the Read The Security Register command. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV0_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV1_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    ret = R_FLASH_SPI_AT_CMD_RDSCUR(devno, p_flash_spi_info->addr, addr_size, TRUE);

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_R_ACCESS);

    /* Set SPI mode to mode N and bit rate for Data. */
    ret =r_flash_spi_drvif_enable_rx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    do
    {
        /* Set data size to receive. */
        p_flash_spi_info->data_cnt = p_flash_spi_info->cnt;

        /* Receive data from memory array. */
        /* The lower layer software should use the data_cnt as receiving counter. */
        ret = r_flash_spi_drvif_rx_data(devno, p_flash_spi_info->data_cnt, p_flash_spi_info->p_data,
                                        p_flash_spi_info->addr, addr_size, FLASH_SPI_SINGLE);
        if (FLASH_SPI_SUCCESS > ret)
        {
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
            r_flash_spi_set_cs(devno, FLASH_SPI_HI);    /* SS# "H"                              */
            return ret;
        }

        p_flash_spi_info->cnt    -= p_flash_spi_info->data_cnt;     /* Updates the cnt.         */
        p_flash_spi_info->p_data += p_flash_spi_info->data_cnt;
        p_flash_spi_info->addr   += p_flash_spi_info->data_cnt;
    }
    while(0 != p_flash_spi_info->cnt); /* WAIT_LOOP */


    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_read_scurreg_page
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_page
* Description  : Writes data from the specified buffer (pData)
*              : to the specified address (addr) for the specified number (data_cnt) of bytes.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Write start address
*              :    uint32_t        cnt                 ;   Number of bytes to be written
*              :    uint32_t        data_cnt            ;   Number of bytes to be written in a page
*              :    uint8_t       * p_data              ;   Write data storage buffer pointer
*              :    bool            read_after_write_add;   Whether or not to close SPI bus cycle
*              :    bool            read_after_write_data;  Whether or not to close SPI bus cycle
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A write exceeding the write page isn't allowed.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_write_page(uint8_t devno, flash_spi_info_t * p_flash_spi_info, bool read_after_write_add, bool read_after_write_data)
{
#if (FLASH_SPI_CFG_WEL_CHK == 1)
    uint8_t             stsreg1 = 0;                     /* Status buffer                        */
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             addr_size = 0;
    uint8_t           * p_cmdbuf = (uint8_t *)&s_flash_at_cmdbuf[devno][0];

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (FLASH_SPI_DUAL == p_flash_spi_info->op_mode)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_flash_spi_at_write_en(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

#if (FLASH_SPI_CFG_WEL_CHK == 1)
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, (uint8_t *)&stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg1 & FLASH_SPI_AT_REG1_WEL))        /* WEL bit =0 : In Write Disable State  */
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_OTHER;
    }
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Issue the WRITE command using the single mode. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV0_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV1_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    if (FLASH_SPI_SINGLE == p_flash_spi_info->op_mode)
    {
        ret = R_FLASH_SPI_AT_CMD_PP(devno, p_flash_spi_info->addr, addr_size, TRUE);
    }
    else
    {
        ret = R_FLASH_SPI_AT_CMD_QPP(devno, p_flash_spi_info->addr, addr_size, TRUE);
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    /* CMD Dummy. */
    r_flash_spi_at_cmd_set(devno, 0, p_flash_spi_info->addr, FLASH_SPI_AT_CMD_SIZE+addr_size);
    if (FLASH_SPI_SINGLE == p_flash_spi_info->op_mode)
    {
        ret = r_flash_spi_drvif_tx_add(devno, addr_size, ++p_cmdbuf, FLASH_SPI_SINGLE, read_after_write_add);
    }
    else
    {
        ret = r_flash_spi_drvif_tx_add(devno, addr_size, ++p_cmdbuf, FLASH_SPI_QUAD, read_after_write_add);
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    /* Set SPI mode to mode N and bit rate for Data. */
    ret = r_flash_spi_drvif_enable_tx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    /* Transmit data to memory. */
    ret = r_flash_spi_drvif_tx_data(devno, p_flash_spi_info->data_cnt, p_flash_spi_info->p_data,
                                    p_flash_spi_info->op_mode, read_after_write_data);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_write_page
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_write_scurreg_page
* Description  : Writes data to security register page from the specified buffer (pData)
*              : to the specified address (addr) for the specified number (data_cnt) of bytes.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Write start address
*              :    uint32_t        cnt                 ;   Number of bytes to be written
*              :    uint32_t        data_cnt            ;   Number of bytes to be written in a page
*              :    uint8_t       * p_data              ;   Write data storage buffer pointer
*              :    bool            read_after_write_add;   Whether or not to close SPI bus cycle
*              :    bool            read_after_write_data;  Whether or not to close SPI bus cycle
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : A write exceeding the write page isn't allowed.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_write_scurreg_page(uint8_t devno, flash_spi_info_t * p_flash_spi_info, bool read_after_write_add, bool read_after_write_data)
{
#if (FLASH_SPI_CFG_WEL_CHK == 1)
    uint8_t             stsreg1 = 0;                     /* Status buffer                        */
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             addr_size = 0;
    uint8_t           * p_cmdbuf = (uint8_t *)&s_flash_at_cmdbuf[devno][0];

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_flash_spi_at_write_en(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

#if (FLASH_SPI_CFG_WEL_CHK == 1)
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, (uint8_t *)&stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg1 & FLASH_SPI_AT_REG1_WEL))        /* WEL bit =0 : In Write Disable State  */
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_OTHER;
    }
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Issue the Write The Security Register command. */
    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV0_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV1_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    ret = R_FLASH_SPI_AT_CMD_PSCUR(devno, p_flash_spi_info->addr, addr_size, TRUE);

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    /* CMD Dummy. */
    r_flash_spi_at_cmd_set(devno, 0, p_flash_spi_info->addr, FLASH_SPI_AT_CMD_SIZE+addr_size);
    ret = r_flash_spi_drvif_tx_add(devno, addr_size, ++p_cmdbuf, FLASH_SPI_SINGLE, read_after_write_add);

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    /* Set SPI mode to mode N and bit rate for Data. */
    ret = r_flash_spi_drvif_enable_tx_data(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    /* Transmit data to memory. */
    ret = r_flash_spi_drvif_tx_data(devno, p_flash_spi_info->data_cnt, p_flash_spi_info->p_data,
                                    FLASH_SPI_SINGLE, read_after_write_data);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    ret = r_flash_spi_drvif_disable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    /* Set SPI mode to mode 3 and bit rate for Command and Data as default. */
    ret = r_flash_spi_drvif_enable(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_write_scurreg_page
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_flash_erase
* Description  : FLASH_MODE_B4K_ERASE (4KB)       ;   Erases the data of specified block (Addr).
*              : FLASH_MODE_B32K_ERASE (32KB)     ;   Erases the data of specified block (Addr).
*              : FLASH_MODE_B64K_ERASE (64KB)     ;   Erases the data of specified block (Addr).
*              : FLASH_MODE_C_ERASE               ;   Erases the all data of specified chip (Addr).
* Arguments    : uint8_t            devno         ;   Device No. (FLASH_DEVn)
*              : flash_spi_erase_info_t * p_flash_spi_erase_info    ; Flash memory erase information
*              :    uint32_t        addr          ;   First address of specified sector
*              :    uint8_t         mode          ;   Type of erase command
*              :                                  ;   FLASH_SPI_MODE_B4K_ERASE   ; Block Erase (4KB)
*              :                                  ;   FLASH_SPI_MODE_B32K_ERASE  ; Block Erase (32KB)
*              :                                  ;   FLASH_SPI_MODE_B64K_ERASE  ; Block Erase (64KB)
*              :                                  ;   FLASH_SPI_MODE_C_ERASE     ; Chip Erase
* Return Value : FLASH_SPI_SUCCESS                ;   Successful operation
*              : FLASH_SPI_ERR_PARAM              ;   Parameter error
*              : FLASH_SPI_ERR_HARD               ;   Hardware error
*              : FLASH_SPI_ERR_OTHER              ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : Flash memory can be erased to only when write-protection has been canceled.
*              : (1) Block Erase
*              :   Data of the specified block in the protected block (64KB) can't be erased and
*              :   error result is returned.
*              : (2) Chip Erase
*              :   Data of the protected chip in the proteted block (64KB) can't be erased and
*              :   error result is returned.
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_erase(uint8_t devno, flash_spi_erase_info_t * p_flash_spi_erase_info)
{
    flash_spi_status_t     ret = FLASH_SPI_SUCCESS;
    uint32_t    mem_size = 0;
    uint8_t     addr_size = 0;
#if (FLASH_SPI_CFG_WEL_CHK == 1)
    uint8_t     stsreg1 = 0;                             /* Status buffer                        */
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        mem_size  = FLASH_SPI_AT_DEV0_MEM_SIZE;
        addr_size = FLASH_SPI_AT_DEV0_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        mem_size  = FLASH_SPI_AT_DEV1_MEM_SIZE;
        addr_size = FLASH_SPI_AT_DEV1_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    if (mem_size <= p_flash_spi_erase_info->addr)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((FLASH_SPI_MODE_B4K_ERASE  != p_flash_spi_erase_info->mode) &&
        (FLASH_SPI_MODE_B32K_ERASE != p_flash_spi_erase_info->mode) &&
        (FLASH_SPI_MODE_B64K_ERASE != p_flash_spi_erase_info->mode) &&
        (FLASH_SPI_MODE_SCUR_ERASE != p_flash_spi_erase_info->mode) &&
        (FLASH_SPI_MODE_C_ERASE    != p_flash_spi_erase_info->mode))
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_PARAM;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* The upper layer software should set to the single mode. */
    /* Execute the Write Enable (WREN) command operation using the single mode. */
    ret = r_flash_spi_at_write_en(devno);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

#if (FLASH_SPI_CFG_WEL_CHK == 1)
    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, (uint8_t *)&stsreg1);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }
    if (0x00 == (stsreg1 & FLASH_SPI_AT_REG1_WEL))        /* WEL bit =0 : In Write Disable State  */
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return FLASH_SPI_ERR_OTHER;
    }
#endif  /* #if (FLASH_SPI_CFG_WEL_CHK == 1) */

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* Issue the Erase command using the single mode. */
    /* The upper layer software should check and set the mode correctly to "p_flash_spi_erase_info->mode". */ 
    if (FLASH_SPI_MODE_B4K_ERASE == p_flash_spi_erase_info->mode)
    {
        if (FLASH_SPI_DEV0 == devno)
        {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV0_B4K_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        }
        else
        {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV1_B4K_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        }
        ret = R_FLASH_SPI_AT_CMD_BE4K(devno, p_flash_spi_erase_info->addr, addr_size, FALSE);
    }
    else if (FLASH_SPI_MODE_B32K_ERASE == p_flash_spi_erase_info->mode)
    {
        if (FLASH_SPI_DEV0 == devno)
        {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV0_B32K_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        }
        else
        {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV1_B32K_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        }
        ret = R_FLASH_SPI_AT_CMD_BE32K(devno, p_flash_spi_erase_info->addr, addr_size, FALSE);
    }
    else if (FLASH_SPI_MODE_B64K_ERASE == p_flash_spi_erase_info->mode)
    {
        if (FLASH_SPI_DEV0 == devno)
        {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV0_B64K_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        }
        else
        {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV1_B64K_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        }
        ret = R_FLASH_SPI_AT_CMD_BE64K(devno, p_flash_spi_erase_info->addr, addr_size, FALSE);
    }
    else if (FLASH_SPI_MODE_SCUR_ERASE == p_flash_spi_erase_info->mode)
    {
        if (FLASH_SPI_DEV0 == devno)
        {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV0_SCUR_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        }
        else
        {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            p_flash_spi_erase_info->addr &= FLASH_SPI_AT_DEV1_SCUR_ADDR;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        }
        ret = R_FLASH_SPI_AT_CMD_ESCUR(devno, p_flash_spi_erase_info->addr, addr_size, FALSE);
    }
    else
    {
        p_flash_spi_erase_info->addr &= 0x00000000;
        ret = R_FLASH_SPI_AT_CMD_CE(devno, p_flash_spi_erase_info->addr, FALSE);
    }

    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_erase
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_rdid
* Description  : Reads Manufacture ID, Device ID.
*              : ID data of 3 bytes are stored in the following order.
*              :        (1) Manufacturer ID - 1 byte
*              :        (2) Device ID - 2 bytes
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_DEVn)
*              : uint8_t          * p_data              ;   ID data storage buffer pointer (3 bytes)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_rdid(uint8_t devno, uint8_t * p_data)
{
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;

    r_flash_spi_set_cs(devno, FLASH_SPI_LOW);           /* SS# "L"                              */
    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);

    /* The upper layer software should set to the single mode. */
    /* Issue the Read Manufacturer and Device ID (RDID) command using the single mode. */
    ret = R_FLASH_SPI_AT_CMD_RDID(devno, TRUE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_R_ACCESS);

    /* Receive the Manufacture Identification, Identification and Memory capacity using the single mode. */
    ret = r_flash_spi_drvif_rx(devno, FLASH_SPI_AT_IDDATA_SIZE, p_data);
    if (FLASH_SPI_SUCCESS > ret)
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
        r_flash_spi_set_cs(devno, FLASH_SPI_HI);        /* SS# "H"                              */
        return ret;
    }

    r_flash_spi_wait_lp(FLASH_SPI_T_CS_HOLD);
    r_flash_spi_set_cs(devno, FLASH_SPI_HI);            /* SS# "H"                              */

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_rdid
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_polling
* Description  : Checks busy for the Page Program, Erase and Write Status Register command operation.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_poll_mode_t  mode            ;   Mode of error check
*              :                                        ;   FLASH_SPI_MODE_REG_WRITE_POLL
*              :                                        ;   FLASH_SPI_MODE_PROG_POLL
*              :                                        ;   FLASH_SPI_MODE_ERASE_POLL
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation (FLASH is ready)
*              : FLASH_SPI_SUCCESS_BUSY                 ;   Successful operation (FLASH is busy)
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_polling(uint8_t devno, flash_spi_poll_mode_t mode)
{
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;

    /* Checks mode of wait operation. */
    if ((FLASH_SPI_MODE_PROG_POLL == mode) || (FLASH_SPI_MODE_ERASE_POLL == mode))
    {
        /* Wait until the ready using single mode for the Page Program or Erase command operation. */
        ret = r_flash_spi_at_poll_prog_erase(devno, mode);
        if (FLASH_SPI_SUCCESS > ret)
        {
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            r_flash_spi_drvif_disable(devno);
            return ret;
        }
    }
    else
    {
        /* Wait until the ready using single mode for the Write Status Register 1 (WRSR1) command operation. */
        ret = r_flash_spi_at_poll_reg_write(devno);
        if (FLASH_SPI_SUCCESS > ret)
        {
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            r_flash_spi_drvif_disable(devno);
            return ret;
        }
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_polling
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_poll_prog_erase
* Description  : Waits for the ready after busy.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_DEVn)
*              : flash_spi_poll_mode_t  mode            ;   Mode of error check
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation (FLASH is ready)
*              : FLASH_SPI_SUCCESS_BUSY                 ;   Successful operation (FLASH is busy)
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : If uses "Delay Task", minimum unit is 1ms.
*              : This is for the Write Status Register 1 (WRSR1) command.
*************************************************************************************************/
static flash_spi_status_t r_flash_spi_at_poll_prog_erase(uint8_t devno, flash_spi_poll_mode_t mode)
{
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             rxbuf   = 0;          /* Receive temp buffer for Status Register 1 */
    uint8_t             rxbuf2  = 0;          /* Receive temp buffer for Status Register 2 */

    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, (uint8_t *)&rxbuf);
    if (FLASH_SPI_SUCCESS == ret)
    {
        /* Ready/Busy check */
        if (0x00 == (rxbuf & FLASH_SPI_AT_REG1_WIP))
        {
            ret = r_flash_spi_at_read_stsreg2(devno, &rxbuf2);
            if (FLASH_SPI_SUCCESS == ret)
            {
                if (FLASH_SPI_MODE_PROG_POLL == mode)
                {
                    /* Other error check */
                    if (0x00 != (rxbuf2 & FLASH_SPI_AT_REG2_P_SUS))
                    {
                        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
                        return FLASH_SPI_ERR_OTHER;
                    }
                }
                else
                {
                    /* Other error check */
                    if (0x00 != (rxbuf2 & FLASH_SPI_AT_REG2_E_SUS))
                    {
                        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
                        return FLASH_SPI_ERR_OTHER;
                    }
                }
                ret = FLASH_SPI_SUCCESS;                /* Ready                               */
            }
        }
        else
        {
            ret = FLASH_SPI_SUCCESS_BUSY;               /* Busy                                */
        }
    }
    else
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_poll_prog_erase
 *****************************************************************************/


/************************************************************************************************
* Function Name: r_flash_spi_at_poll_reg_write
* Description  : Waits for the ready after busy.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_DEVn)
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation (Flash memory is ready)
*              : FLASH_SPI_SUCCESS_BUSY                 ;   Successful operation (Flash memory is busy)
*              : FLASH_SPI_ERR_HARD                     ;   Hardware error
*              : FLASH_SPI_ERR_OTHER                    ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : If uses "Delay Task", minimum unit is 1ms.
*              : This is for the Write Status Register 1 (WRSR1) command.
*************************************************************************************************/
static flash_spi_status_t r_flash_spi_at_poll_reg_write(uint8_t devno)
{
    flash_spi_status_t  ret = FLASH_SPI_SUCCESS;
    uint8_t             rxbuf = 0;            /* Receive temp buffer for Status Register 1 */

    /* Execute the Read Status Register 1 (RDSR1) command operation using the single mode. */
    ret = r_flash_spi_at_read_stsreg1(devno, (uint8_t *)&rxbuf);
    if (FLASH_SPI_SUCCESS == ret)
    {
        /* Ready/Busy check */
        if (0x00 == (rxbuf & FLASH_SPI_AT_REG1_WIP))
        {
            ret = FLASH_SPI_SUCCESS;                   /* Ready                                */
        }
        else
        {
            ret = FLASH_SPI_SUCCESS_BUSY;              /* Busy                                 */
        }
    }
    else
    {
        R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
        return ret;
    }

    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_poll_reg_write
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_cmd_set
* Description  : Command and address setting.
*              : Converts little endian data or big endian data.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t            cmd                 ;   Command data
*              : uint32_t           addr                ;   Address data
*              : uint8_t            cmdsize             ;   Command size
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static void r_flash_spi_at_cmd_set(uint8_t devno, uint8_t cmd, uint32_t addr, uint8_t cmdsize)
{
    flash_spi_at_exchg_long_t     tmp;
    uint8_t     * p_cmdbuf = (uint8_t *)&s_flash_at_cmdbuf[devno][0];
    uint8_t     addr_size = 0;

    if (FLASH_SPI_DEV0 == devno)
    {
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV0_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
    }
    else
    {
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
        addr_size = FLASH_SPI_AT_DEV1_ADDR_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
    }

    tmp.ul = addr;

    if (FLASH_SPI_AT_CMD_SIZE == cmdsize)
    {
        *p_cmdbuf = cmd;
    }
    else if (4 == cmdsize)
    {
#if (FLASH_SPI_LITTLE_ENDIAN)
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf   = tmp.uc[0];
#else
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf   = tmp.uc[3];
#endif  /* #if (FLASH_SPI_LITTLE_ENDIAN) */
    }
    else if (5 == cmdsize)
    {
#if (FLASH_SPI_LITTLE_ENDIAN)
        if (3 == addr_size)
        {
            *p_cmdbuf++ = cmd;
            *p_cmdbuf++ = tmp.uc[2];
            *p_cmdbuf++ = tmp.uc[1];
            *p_cmdbuf++ = tmp.uc[0];
            *p_cmdbuf   = 0xFF;
        }
        else
        {
            *p_cmdbuf++ = cmd;
            *p_cmdbuf++ = tmp.uc[3];
            *p_cmdbuf++ = tmp.uc[2];
            *p_cmdbuf++ = tmp.uc[1];
            *p_cmdbuf   = tmp.uc[0];
        }
#else
        if (3 == addr_size)
        {
            *p_cmdbuf++ = cmd;
            *p_cmdbuf++ = tmp.uc[1];
            *p_cmdbuf++ = tmp.uc[2];
            *p_cmdbuf++ = tmp.uc[3];
            *p_cmdbuf   = 0xFF;
        }
        else
        {
            *p_cmdbuf++ = cmd;
            *p_cmdbuf++ = tmp.uc[0];
            *p_cmdbuf++ = tmp.uc[1];
            *p_cmdbuf++ = tmp.uc[2];
            *p_cmdbuf   = tmp.uc[3];
        }
#endif  /* #if (FLASH_SPI_LITTLE_ENDIAN) */
    }   
    else
    {
#if (FLASH_SPI_LITTLE_ENDIAN)
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[3];
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf++ = tmp.uc[0];
        *p_cmdbuf   = 0xFF;
#else
        *p_cmdbuf++ = cmd;
        *p_cmdbuf++ = tmp.uc[0];
        *p_cmdbuf++ = tmp.uc[1];
        *p_cmdbuf++ = tmp.uc[2];
        *p_cmdbuf++ = tmp.uc[3];
        *p_cmdbuf   = 0xFF;
#endif  /* #if (FLASH_SPI_LITTLE_ENDIAN) */
    }
}
/******************************************************************************
 End of function r_flash_spi_at_cmd_set
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_get_memory_info
* Description  : Gets the memory size and page size.
* Arguments    : uint8_t               devno                ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_mem_info_t * p_flash_spi_mem_info;   Flash memory size information
*              :    uint32_t            mem_size            ;   Max memory size
*              :    uint32_t            wpag_size           ;   Write page size
* Return Value : FLASH_SPI_SUCCESS                          ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                        ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_get_memory_info(uint8_t devno, flash_spi_mem_info_t * p_flash_spi_mem_info)
{
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;

    /* Get memory information. */
    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            p_flash_spi_mem_info->mem_size  = FLASH_SPI_AT_DEV0_MEM_SIZE;
            p_flash_spi_mem_info->wpag_size = FLASH_SPI_AT_DEV0_PAGE_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
            break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            p_flash_spi_mem_info->mem_size  = FLASH_SPI_AT_DEV1_MEM_SIZE;
            p_flash_spi_mem_info->wpag_size = FLASH_SPI_AT_DEV1_PAGE_SIZE;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
            break;
        default:

            /* Do nothing. */
            break;
    }
    
    return ret;
}
/******************************************************************************
 End of function r_flash_spi_at_get_memory_info
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_check_cnt
* Description  : Checks counter.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_check_cnt(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            if ((0 == p_flash_spi_info->cnt) || (FLASH_SPI_AT_DEV0_MEM_SIZE < p_flash_spi_info->cnt) ||
                ((FLASH_SPI_AT_DEV0_MEM_SIZE - p_flash_spi_info->cnt) < p_flash_spi_info->addr))
            {
                R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
                return FLASH_SPI_ERR_PARAM;
            }
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
            break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            if ((0 == p_flash_spi_info->cnt) || (FLASH_SPI_AT_DEV1_MEM_SIZE < p_flash_spi_info->cnt) ||
                ((FLASH_SPI_AT_DEV1_MEM_SIZE - p_flash_spi_info->cnt) < p_flash_spi_info->addr))
            {
                R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
                return FLASH_SPI_ERR_PARAM;
            }
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
            break;
        default:
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            return FLASH_SPI_ERR_PARAM;
            break;
    }
    return FLASH_SPI_SUCCESS;
}
/******************************************************************************
 End of function r_flash_spi_at_check_cnt
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_check_scurreg_cnt
* Description  : Checks counter of internal buffer within a security register page.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : FLASH_SPI_SUCCESS                      ;   Successful operation
*              : FLASH_SPI_ERR_PARAM                    ;   Parameter error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_at_check_scurreg_cnt(uint8_t devno, flash_spi_info_t * p_flash_spi_info)
{
    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            if ((0 == p_flash_spi_info->cnt) || (FLASH_SPI_AT_DEV0_PAGE_SIZE < p_flash_spi_info->cnt))
            {
                R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
                return FLASH_SPI_ERR_PARAM;
            }
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
            break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            if ((0 == p_flash_spi_info->cnt) || (FLASH_SPI_AT_DEV1_PAGE_SIZE < p_flash_spi_info->cnt))
            {
                R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
                return FLASH_SPI_ERR_PARAM;
            }
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
            break;
        default:
            R_FLASH_SPI_Log_Func(FLASH_SPI_DEBUG_ERR_ID, (uint32_t)FLASH_SPI_TYPE_SUB, __LINE__);
            return FLASH_SPI_ERR_PARAM;
            break;
    }
    return FLASH_SPI_SUCCESS;
}
/******************************************************************************
 End of function r_flash_spi_at_check_scurreg_cnt
 *****************************************************************************/

/************************************************************************************************
* Function Name: r_flash_spi_at_page_calc
* Description  : Calculates the writable bytes from the start address to the page boundary.
* Arguments    : uint8_t            devno               ;   Device No. (FLASH_SPI_DEVn)
*              : flash_spi_info_t * p_flash_spi_info    ;   Flash memory information
*              :    uint32_t        addr                ;   Read start address
*              :    uint32_t        cnt                 ;   Number of bytes to be read
*              :    uint32_t        data_cnt            ;   Not used
*              :    uint8_t       * p_data              ;   Read data storage buffer pointer
* Return Value : Writable size (bytes)
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_flash_spi_at_page_calc(uint8_t devno, flash_spi_info_t   * p_flash_spi_info)
{
    uint32_t    tmpcnt = 0;

    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            tmpcnt = ((p_flash_spi_info->addr + FLASH_SPI_AT_DEV0_PAGE_SIZE) / FLASH_SPI_AT_DEV0_PAGE_SIZE)
                     * FLASH_SPI_AT_DEV0_PAGE_SIZE - p_flash_spi_info->addr;
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
            break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            tmpcnt = ((p_flash_spi_info->addr + FLASH_SPI_AT_DEV1_PAGE_SIZE) / FLASH_SPI_AT_DEV1_PAGE_SIZE)
                     * FLASH_SPI_AT_DEV1_PAGE_SIZE - p_flash_spi_info->addr;
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
            break;
        default:

            /* Do nothing. */
            break;
    }
    return tmpcnt;
}
/******************************************************************************
 End of function r_flash_spi_at_page_calc
 *****************************************************************************/

#endif /* FLASH_SPI_CFG_DEV0_AT25QF == 1 || FLASH_SPI_CFG_DEV1_AT25QF == 1 */

/* End of File */
