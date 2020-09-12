/*******************************************************************************
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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/*******************************************************************************
 * File Name    : r_uid_ram.c
 * Description  : Flash Control Processing of Unique ID Read.
 * Limitation   : none
 ******************************************************************************/
/*******************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 01.12.2014 1.00     First Release
 *              : 20.05.2019 1.11     Supported the following compilers:
 *                                    - GCC for Renesas RX
 *                                    - IAR C/C++ Compiler for Renesas RX
 *                                    Added comments "WAIT_LOOP"
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "platform.h"
#include "r_uid_rx_if.h"
#include "r_uid_ram.h"

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void uid_pe_mode_enter(void);
static void uid_pe_mode_exit(void);
static uid_err_t uid_read(uint8_t *pdest_addr);
static void uid_write_fpmcr(uint8_t value);
static void uid_delay_us(unsigned long us, unsigned long khz);

#ifndef BSP_MCU_RX110
static uint8_t uid_enable_dataflashaccess(uint8_t dflen);
#endif


/*******************************************************************************
 * Function Name: uid_read_ram
 * Description  : Function will initialize the flash peripheral.
 * Arguments    : void
 * Return Value : UID_SUCCESS - API initialized successfully.
 *              : UID_ERR_FAILURE - Unique ID Read sequence failed.
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
uid_err_t uid_read_ram(uint8_t *pdest_addr)
{
    uid_err_t err;

#ifndef BSP_MCU_RX110
    uint8_t   backup_dflen;
#endif

    /* Transition to the ROM P/E mode */
    uid_pe_mode_enter();

#ifndef BSP_MCU_RX110
    /* E2 Data Flash Access Enable */
    backup_dflen = uid_enable_dataflashaccess(ENABLE);
#endif

    /* Execution of Unique ID Read */
    err = uid_read(pdest_addr);

#ifndef BSP_MCU_RX110
    uid_enable_dataflashaccess(backup_dflen);
#endif

    /* Transition to the Read mode */
    uid_pe_mode_exit();

    return err;
}

/*******************************************************************************
 * Outline      : Transition to P/E mode
 * Header       : r_uid_ram.h
 * Function Name: uid_pe_mode_enter
 * Description  : Executes the sequence to enter P/E mode.
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
static void uid_pe_mode_enter(void)
{
    /* FENTRYR - Flash P/E Mode Entry Register
    b15:b8 FEKEY - Key Code - AAh (set this bits to AAh at the same time (write this register in 16 bits).)
    b7     FENTRYD - E2 DataFlash P/E Mode Entry Bit 3 - read mode.
    b6:b1  Reserved - These bits are read as 0. The write value should be 0.
    b0     FENTRY0 - ROM P/E Mode Entry 0 - P/E mode. */
    FLASH.FENTRYR.WORD = FENTRYR_CODEFLASH_PE_MODE;

    /* FPMCR - Flash P/E Mode Control Register
    b7     FMS2 - Flash Operating Mode Select 2 - See the FMS0 bit.
    b6     LVPE - Low-Voltage P/E Mode Enable - Low-voltage P/E mode disabled.
    b5     Reserved - These bits are read as 0. The write value should be 0.
    b4     FMS1 - Flash Operating Mode Select 1 - See the FMS0 bit.
    b3     RPDIS - ROM P/E Disable - ROM programming/erasure enabled.
    b2     Reserved - These bits are read as 0. The write value should be 0.
    b1     FMS0 - Flash Operating Mode Select 0 - Discharge mode 1.
    b0     Reserved - These bits are read as 0. The write value should be 0. */
    uid_write_fpmcr(DISCHARGE_1);

    /* Wait for 2us over (tDIS) */
    uid_delay_us(WAIT_TDIS, ICLK_KHZ);

    if (OPCCR_HIGH_SPEED_MODE == SYSTEM.OPCCR.BIT.OPCM)
    {
        /* FPMCR - Flash P/E Mode Control Register
        b1     FMS0 - Flash Operating Mode Select 0 - Discharge mode 2. */
        uid_write_fpmcr(DISCHARGE_2);

        /* FPMCR - Flash P/E Mode Control Register
        b1     FMS0 - Flash Operating Mode Select 0 - ROM P/E mode. */
        uid_write_fpmcr(CODEFLASH_PE_MODE);

        /* Wait for 5us over (tMS) */
        uid_delay_us(WAIT_TMS_HIGH, ICLK_KHZ);
    }
    else
    {
        /* FPMCR - Flash P/E Mode Control Register
        b6     LVPE - Low-Voltage P/E Mode Enable - Low-voltage P/E mode enabled.
        b1     FMS0 - Flash Operating Mode Select 0 - Discharge mode 2. */
        uid_write_fpmcr(DISCHARGE_2 | LVPE_MODE);

        /* FPMCR - Flash P/E Mode Control Register
        b6     LVPE - Low-Voltage P/E Mode Enable - Low-voltage P/E mode enabled.
        b1     FMS0 - Flash Operating Mode Select 0 - ROM P/E mode. */
        uid_write_fpmcr(CODEFLASH_PE_MODE | LVPE_MODE);

        /* Wait for 3us over (tMS) */
        uid_delay_us(WAIT_TMS_MID, ICLK_KHZ);
    }

    /* FISR - Flash Initial Setting Register
    b7:b6  SAS - Start-Up Area Select - start-up area settings of the extra area.
    b5     Reserved - This bit is read as 0. The write value should be 0.
    b4:b0  PCKA - Peripheral Clock Notification -  FlashIF clock (FCLK). */
    FLASH.FISR.BIT.PCKA = FCLK_MHZ - 1;
}

/*******************************************************************************
 * Outline      : Transition to read mode
 * Header       : r_uid_ram.h
 * Function Name: uid_pe_mode_exit
 * Description  : Executes the sequence to enter read mode.
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
static void uid_pe_mode_exit(void)
{
    /* FPMCR - Flash P/E Mode Control Register
    b1     FMS0 - Flash Operating Mode Select 0 - Discharge mode 2. */
    uid_write_fpmcr(DISCHARGE_2);

    /* Wait for 2us over (tDIS) */
    uid_delay_us(WAIT_TDIS, ICLK_KHZ);

    /* FPMCR - Flash P/E Mode Control Register
    b1     FMS0 - Flash Operating Mode Select 0 - Discharge mode 1. */
    uid_write_fpmcr(DISCHARGE_1);

    /* FPMCR - Flash P/E Mode Control Register
    b1     FMS0 - Flash Operating Mode Select 0 - ROM read mode. */
    uid_write_fpmcr(READ_MODE);

    /* Wait for 5us over (tMS) */
    uid_delay_us(WAIT_TMS_HIGH, ICLK_KHZ);

    /* FENTRYR - Flash P/E Mode Entry Register
    b15:b8 FEKEY - Key Code - AAh (set this bits to AAh at the same time (write this register in 16 bits).)
    b0     FENTRY0 - ROM P/E Mode Entry 0 - Read mode. */
    FLASH.FENTRYR.WORD = FENTRYR_READ_MODE;

	/* WAIT_LOOP */
    while (0x0000 != FLASH.FENTRYR.WORD)
    {
        /* Confirm that the written value can be read correctly. */
    }
}

/*******************************************************************************
 * Outline      : Reading Unique ID
 * Header       : r_uid_ram.h
 * Function Name: uid_read
 * Description  : Read the Unique ID from extra area
 * Arguments    : pdest_addr : pointer of destination
 * Return Value : UID_SUCCESS - Unique ID read success
 *              : UID_ERR_FAILURE - Unique ID read failed
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
static uid_err_t uid_read(uint8_t *pdest_addr)
{
    uint16_t read_data;
    uid_err_t status;

    status = UID_SUCCESS;

    /* Select Extra Area */
    /* FASR - Flash Area Select Register
    b7:b1  Reserved - This bit is read as 0. The write value should be 0.
    b0     EXS - Extra Area Select - Extra area. */
    FLASH.FASR.BIT.EXS = 1;

    /* ---- Unique ID read start address setting ---- */
    /* FSARH - Flash Processing Start Address Register H
    target processing address or the start address in the flash memory.
    (Set bit 19 to bit 16 of the flash memory address) */
    FLASH.FSARH = 0;

    /* FSARL - Flash Processing Start Address Register L
    target processing address or the start address in the flash memory.
    (Set bit 15 to bit 0 of the address in this register.) */
    FLASH.FSARL = UNIQUE_ID_START;

    /* ---- Unique ID read end address setting ---- */
    /* FEARH - Flash Processing End Address Register H
    the end address in the flash memory.
    (Set bit 19 to bit 16 of the flash memory address in this register.)*/
    FLASH.FEARH = 0;

    /* FEARL - Flash Processing End Address Register L
    the end address in the flash memory
    (Set bit 15 to bit 0 of the flash memory address in this register.)*/
    FLASH.FEARL = UNIQUE_ID_END;

    /* ---- Execute Unique ID Read command ---- */
    /* FCR - Flash Control Register
    b7     OPST - Processing Start - Processing starts.
    b6     STOP - Forced Processing Stop - no stop.
    b5     Reserved - This bit is read as 0. The write value should be 0.
    b4     DRC - Data Read Completion - Data is not read or next data is requested.
    b3:b0  CMD - Software Command Setting - Unique ID Read. */
    FLASH.FCR.BYTE = FCR_IDREAD;

    /* WAIT_LOOP */
    while ((1 != FLASH.FSTATR1.BIT.DRRDY) && (1 != FLASH.FSTATR0.BIT.ILGLERR))
    {
        /* Check that execute command is completed. */
    }

    /* Check Read error */
    if (1 != FLASH.FSTATR0.BIT.ILGLERR)
    {

        do
        {
            /* Data Read */
            read_data = FLASH.FRBL;
            *(pdest_addr++) = (uint8_t) (read_data & 0x00FF);
            *(pdest_addr++) = (uint8_t) ((read_data >> 8) & 0x00FF);

            read_data = FLASH.FRBH;
            *(pdest_addr++) = (uint8_t) (read_data & 0x00FF);
            *(pdest_addr++) = (uint8_t) ((read_data >> 8) & 0x00FF);

            /* ---- Next Unique ID Read Setting ---- */
            /* FCR - Flash Control Register
            b4     DRC - Data Read Completion - Data reading is completed. */
            FLASH.FCR.BYTE = FCR_NEXT_IDREAD;

            /* WAIT_LOOP */
            while (0 != FLASH.FSTATR1.BIT.DRRDY)
            {
                /* Check that execute command is completed. */
            }

            /* ---- Execute Unique ID Read command ---- */
            /* FCR - Flash Control Register
            b4     DRC - Data Read Completion - Data is not read or next data is requested. */
            FLASH.FCR.BYTE = FCR_IDREAD;

            /* WAIT_LOOP */
            while ((1 != FLASH.FSTATR1.BIT.DRRDY) && (1 != FLASH.FSTATR1.BIT.FRDY))
            {
                /* Check that execute command is completed. */
            }

        }
        while (1 == FLASH.FSTATR1.BIT.DRRDY); /* WAIT_LOOP */

    }
    else
    {
        status = UID_ERR_FAILURE;
    }

    /* Clear FCR register */
    /* FCR - Flash Control Register
    b7     OPST - Processing Start - Processing stops.
    b4     DRC - Data Read Completion - Data is not read or next data is requested.
    b3:b0  CMD - Software Command Setting - 0h */
    FLASH.FCR.BYTE = FCR_CLEAR;

    /* WAIT_LOOP */
    while (0 != FLASH.FSTATR1.BIT.FRDY)
    {
        /* Check that execute command is completed. */
    }

    return status;
}

/*******************************************************************************
 * Outline      : Setting the FPMCR register
 * Header       : r_uid_ram.h
 * Function Name: uid_write_fpmcr
 * Description  : Specifies the argument set to the FPMCR register by the program sequence.
 * Arguments    : value     : Setting value for the FPMCR register
 * Return Value : none
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
static void uid_write_fpmcr(uint8_t value)
{
    /* FPR - Protection Unlock Register
    Write the "A5h" in order to rewrite the FPMCR register. */
    FLASH.FPR = 0xA5;

    /* FPMCR - Flash P/E Mode Control Register */
    FLASH.FPMCR.BYTE = value;
    FLASH.FPMCR.BYTE = (uint8_t) ~value;
    FLASH.FPMCR.BYTE = value;

    /* Order to secure the time of subsequent functions uid_delay_us. */
    if (value == FLASH.FPMCR.BYTE)
    {
        R_BSP_NOP();
    }
}

#ifndef BSP_MCU_RX110
/*******************************************************************************
 * Outline      : Setting the DFLEN bit
 * Header       : r_uid_ram.h
 * Function Name: uid_enable_dataflashaccess
 * Description  : rewrite E2 DataFlash Access enable bit
 * Arguments    : dflen     : Setting value for the DFLEN bit
 * Return Value : Value of DFLEN bit before the change
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
static uint8_t uid_enable_dataflashaccess(uint8_t dflen)
{
    uint8_t tmp_dflen;

    /* Backup E2 DataFlash Access enable bit */
    tmp_dflen = FLASH.DFLCTL.BIT.DFLEN;

    /* DFLCTL - E2 DataFlash Control Register
    b7:b1  Reserved - This bit is read as 0. The write value should be 0.
    b0     DFLEN - E2 DataFlash Access Enable - Access enabled or disable. */
    FLASH.DFLCTL.BIT.DFLEN = dflen;

    return tmp_dflen;
}

#endif

/*******************************************************************************
 * Outline      : Intrinsic function to specify the number of loops
 * Header       : none
 * Function Name: uid_delay
 * Description  : Wait processing that loops at a fixed five cycles.
 * Arguments    : R1 : Waiting loop counter
 * Return Value : none
 * note         : Warning occurs when you compile, because the argument of
 *              : the inline assembler function is passed to the R1 register.
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
R_BSP_PRAGMA_STATIC_INLINE_ASM(uid_delay)
void uid_delay(unsigned long loop_cnt)
{
    R_BSP_ASM_INTERNAL_USED(loop_cnt)
    R_BSP_ASM_BEGIN
    R_BSP_ASM(    BRA.B   R_BSP_ASM_LAB_NEXT(0)    )
    R_BSP_ASM(    NOP                              )
    R_BSP_ASM_LAB(0:                               )
    R_BSP_ASM(    NOP                              )
    R_BSP_ASM(    SUB     #01H,R1                  )
    R_BSP_ASM(    BNE.B   R_BSP_ASM_LAB_PREV(0)    )
    R_BSP_ASM_END

}

/*******************************************************************************
 * Outline      : Function that specifies the execution time
 * Header       : none
 * Function Name: uid_delay_us
 * Description  : The number of loops is calculated based on the execution time (ƒÊs)
 *              : and the system clock (ICLK) frequency, and the intrinsic function
 *              : that specifies the number of loops is called.
 * Arguments    : us  : Execution time
 : khz : ICLK frequency when calling the function
 * Return Value : none
 ******************************************************************************/
R_BSP_ATTRIB_SECTION_CHANGE(P,FRAM)
static void uid_delay_us(unsigned long us, unsigned long khz)
{

    signed long loop_cnt; /* Argument of R_DELAY function */

    /* Calculation of a loop count */
    loop_cnt = us * khz;
    loop_cnt = (loop_cnt / 5000); /* Division about cycle of 1 loop */
    loop_cnt = loop_cnt - 4; /* Overhead is reduced from a loop count. */

    /* R_DELAY function is performed when loop_cnt is 1 or more. */
    if (loop_cnt > 0)
    {
        uid_delay((unsigned long) loop_cnt);
    }
}

R_BSP_ATTRIB_SECTION_CHANGE_END /* end FRAM */

/* end of file */
