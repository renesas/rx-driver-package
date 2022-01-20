/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_flash_nofcu.c
 * Description  : This module implements functions common to Flash Types 1.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           23.04.2021 4.80    First Release. (Changed the structure of FLASH_TYPE_1 code with the addition of RX140.)
 *           12.12.2021 4.81    Modified some global variables problems.
 *           10.12.2021 4.81    Added support for Tool News R20TS0772, and removed unnecessary code in flash_write().
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_flash_rx_if.h"

#ifndef FLASH_HAS_FCU
#include "r_flash_nofcu.h"
#include "r_flash_group.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
#ifndef FLASH_NO_DATA_FLASH
static void flash_df_pe_mode_enter(void);
static void flash_df_read_mode_enter(void);
static void flash_df_erase(const uint32_t start_addr, const uint32_t num_blocks);
static void flash_df_blankcheck(const uint32_t start_addr, const uint32_t end_addr);
static void flash_df_write(const uint32_t src_addr, const uint32_t dest_addr);
#endif

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
static void flash_cf_pe_mode_enter(void);
static void flash_cf_read_mode_enter(void);
static void flash_cf_erase(const uint32_t start_addr, const uint32_t num_blocks);
static void flash_cf_blankcheck(const uint32_t start_addr, const uint32_t end_addr);
static void flash_cf_write(const uint32_t src_addr,  const uint32_t dest_addr);
#endif

static void flash_write_fpmcr (uint8_t value);
static flash_err_t flash_wait_frdy(void);

#ifndef FLASH_NO_DATA_FLASH
/**********************************************************************************************************************
 * Function Name: flash_df_access_enable
 * Description  : Enable the E2 Data Flash Access and wait for the DataFlash STOP recovery time
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void flash_df_access_enable(void)
{
#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* E2 DataFlash Wait Cycle Setting */
    if (MCU_CFG_ICLK_HZ > FLASH_DFLWAITR_ICLK_FREQ)
    {
        FLASH.DFLWAITR.WORD = 0xAA01;

        while (0x0001 != FLASH.DFLWAITR.WORD)
        {
            R_BSP_NOP();
        }
    }
#endif

    /* E2 DataFlash Access enable */
    FLASH.DFLCTL.BIT.DFLEN = 1;

    if(1 == FLASH.DFLCTL.BIT.DFLEN)
    {
        R_BSP_NOP();
    }

    /* Wait for 5us over (tDSTOP) */
    flash_delay_us(WAIT_TDSTOP, ICLK_KHZ);
}

/**********************************************************************************************************************
 * Function Name: flash_df_pe_mode_enter
 * Description  : Executes the sequence to enter P/E mode.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
static void flash_df_pe_mode_enter(void)
{
    FLASH.FENTRYR.WORD = FENTRYR_DATAFLASH_PE_MODE;

    /* Wait for 5us over (tDSTOP) */
    flash_delay_us(WAIT_TDSTOP, ICLK_KHZ);

#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    flash_write_fpmcr(DATAFLASH_PE_MODE);

    FLASH.FISR.BIT.PCKA = (FLASH_FISR_FCLK_FREQ < MCU_CFG_FCLK_HZ) ? 
        (FCLK_MHZ - (((FCLK_MHZ - (FLASH_FISR_FCLK_FREQ / MHZ)) / 2)) - 1) : FCLK_MHZ - 1;
#else
    if (OPCCR_HIGH_SPEED_MODE == SYSTEM.OPCCR.BIT.OPCM)
    {
        flash_write_fpmcr(DATAFLASH_PE_MODE);
    }
    else
    {
        flash_write_fpmcr(DATAFLASH_PE_MODE | LVPE_MODE);
    }

    FLASH.FISR.BIT.PCKA = FCLK_MHZ - 1;
#endif
}

/**********************************************************************************************************************
 * Function Name: flash_df_read_mode_enter
 * Description  : Executes the sequence to enter read mode.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
static void flash_df_read_mode_enter(void)
{
    flash_write_fpmcr(READ_MODE);

    /* Wait for 5us over (tMS) */
    flash_delay_us(WAIT_TMS_HIGH, ICLK_KHZ);

    FLASH.FENTRYR.WORD = FENTRYR_READ_MODE;

    while (0x0000 != FLASH.FENTRYR.WORD)
    {
        /* Confirm that the written value can be read correctly. */
    }
}

/**********************************************************************************************************************
 * Function Name: flash_df_erase
 * Description  : Performs block erasing between specified addresses. Address validation
 *                has already been performed by the caller.
 * Arguments    : start_addr   : Start address for erasing
 *              : num_blocks   : # of blocks to erase
 * Return Value : None
 *********************************************************************************************************************/
static void flash_df_erase(const uint32_t start_addr, const uint32_t num_blocks)
{
    uint32_t block_start_addr;
    uint32_t block_end_addr;

    block_start_addr = start_addr - DATAFLASH_ADDR_OFFSET;  /* Conversion to the P/E address from the read address */
    block_end_addr   = ((start_addr + (num_blocks * FLASH_DF_BLOCK_SIZE)) - DATAFLASH_ADDR_OFFSET) - 1;

    /* Select User Area */
    FLASH.FASR.BIT.EXS = 0;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* Erase start address setting */
    FLASH.FSARH = (uint16_t)(block_start_addr >> 16);
    FLASH.FSARL = (uint16_t)(block_start_addr & 0xFFFF);

    /* Erase end address setting */
    FLASH.FEARH = (uint16_t)(block_end_addr >> 16);
    FLASH.FEARL = (uint16_t)(block_end_addr & 0xFFFF);
#else
    /* Erase start address setting */
    FLASH.FSARH = (uint8_t)((block_start_addr >> 16) & 0x0F);
    FLASH.FSARL = (uint16_t)(block_start_addr & 0xFFFF);

    /* Erase end address setting */
    FLASH.FEARH = (uint8_t)((block_end_addr >> 16) & 0x0F);
    FLASH.FEARL = (uint16_t)(block_end_addr & 0xFFFF);
#endif

    /* Execute Erase command */
    FLASH.FCR.BYTE = FCR_ERASE;
}

/**********************************************************************************************************************
 * Function Name: flash_df_blankcheck
 * Description  : Performs blank check for the area between specified addresses.
 * Arguments    : start_addr   : Start address for blank check
 *              : end_addr     : End address for blank check
 * Return Value : FLASH_SUCCESS     - The checked addresses are all blank
 *              : FLASH_ERR_FAILURE - Blank check error
 *********************************************************************************************************************/
static void flash_df_blankcheck(const uint32_t start_addr, const uint32_t end_addr)
{
    uint32_t start_addr_idx;
    uint32_t end_addr_idx;

    start_addr_idx = start_addr - DATAFLASH_ADDR_OFFSET;  /* Conversion to the P/E address from the read address */
    end_addr_idx   = end_addr - DATAFLASH_ADDR_OFFSET;    /* Conversion to the P/E address from the read address */

    /* Select User Area */
    FLASH.FASR.BIT.EXS = 0;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* BlankCheck start address setting */
    FLASH.FSARH = (uint16_t)(start_addr_idx >> 16);
    FLASH.FSARL = (uint16_t)(start_addr_idx & 0xFFFF);

    /* BlankCheck end address setting */
    FLASH.FEARH = (uint16_t)(end_addr_idx >> 16);
    FLASH.FEARL = (uint16_t)(end_addr_idx & 0xFFFF);
#else
    /* BlankCheck start address setting */
    FLASH.FSARH = (uint8_t)((start_addr_idx >> 16) & 0x0F);
    FLASH.FSARL = (uint16_t)(start_addr_idx & 0xFFFF);

    /* BlankCheck end address setting */
    FLASH.FEARH = (uint8_t)((end_addr_idx >> 16) & 0x0F);
    FLASH.FEARL = (uint16_t)(end_addr_idx & 0xFFFF);
#endif

    /* Execute BlankCheck command */
    FLASH.FCR.BYTE = FCR_BLANKCHECK;
}

/**********************************************************************************************************************
 * Function Name: flash_df_write
 * Description  : Programs the specified data to the E2 Data Flash data.
 * Arguments    : block_start_addr   : Start address (read form) for erasing
 *              : block_end_addr     : End address (read form) for erasing
 * Return Value : None
 *********************************************************************************************************************/
static void flash_df_write(const uint32_t src_addr,  const uint32_t dest_addr)
{
    uint32_t dest_addr_idx;
    uint8_t  *write_data = (uint8_t *)src_addr;

    dest_addr_idx = dest_addr - DATAFLASH_ADDR_OFFSET;  /* Conversion to the P/E address from the read address */

    /* Select User Area */
    FLASH.FASR.BIT.EXS = 0;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* Write start address setting */
    FLASH.FSARH = (uint16_t)(dest_addr_idx >> 16);
    FLASH.FSARL = (uint16_t)(dest_addr_idx & 0xFFFF);

    FLASH.FWB3 = (uint16_t)0x0000;
    FLASH.FWB2 = (uint16_t)0x0000;
    FLASH.FWB1 = (uint16_t)0x0000;
    FLASH.FWB0 = (uint16_t)(*write_data);
#else
    /* Write start address setting */
    FLASH.FSARH = (uint8_t)((dest_addr_idx >> 16) & 0x0F);
    FLASH.FSARL = (uint16_t)(dest_addr_idx & 0xFFFF);

    /* Write data setting */
    FLASH.FWBH = (uint16_t)0x0000;
    FLASH.FWBL = (uint16_t)(*write_data);
#endif

    /* Execute Write command */
    FLASH.FCR.BYTE = FCR_WRITE;
}
#endif /* #ifndef FLASH_NO_DATA_FLASH */

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
/* All the functions below need to be placed in RAM if Code Flash programming is to be supported */
#define FLASH_PE_MODE_SECTION    R_BSP_ATTRIB_SECTION_CHANGE(P, FRAM)
#define FLASH_SECTION_CHANGE_END R_BSP_ATTRIB_SECTION_CHANGE_END
#else
#define FLASH_PE_MODE_SECTION
#define FLASH_SECTION_CHANGE_END
#endif

/**********************************************************************************************************************
 * Function Name: flash_get_status
 * Description  : Returns the current state of the flash
 *                NOTE1: This function does not have to execute from in RAM. It must be in RAM though if
 *                CF BGO is enabled and this function is called during a ROM P/E operation.
 * Arguments    : None
 * Return Value : FLASH_SUCCESS -
 *                    Flash is ready to use
 *                FLASH_ERR_BUSY -
 *                    Flash is busy with another operation or is uninitialized
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_get_status (void)
{

    /* Return flash status */
    if( g_flash_state == FLASH_READY )
    {
        return FLASH_SUCCESS;
    }
    else
    {
        return FLASH_ERR_BUSY;
    }
}

/**********************************************************************************************************************
 * Function Name: flash_reset
 * Description  : Function resets the Flash peripheral
 * Arguments    : None
 * Return Value : FLASH_SUCCESS -
 *                Flash circuit successfully reset.
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
void flash_reset() 
{
    /* Reset Flash */
    FLASH.FRESETR.BIT.FRESET = 1;
    FLASH.FRESETR.BIT.FRESET = 0;
}

/**********************************************************************************************************************
 * Function Name: flash_stop
 * Description  : Function issues the STOP command
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
void flash_stop(void)
{
    FLASH.FCR.BIT.STOP = 1;
    while (FLASH.FSTATR1.BIT.FRDY == 0)     // wait for FRDY
        ;

    FLASH.FCR.BYTE = 0;
    while (FLASH.FSTATR1.BIT.FRDY == 1)     // wait for FRDY
        ;
}

/**********************************************************************************************************************
 * Function Name: flash_delay
 * Description  : Wait processing that loops at a fixed five cycles.
 * Arguments    : R1 : Waiting loop counter
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
R_BSP_PRAGMA_STATIC_INLINE_ASM(flash_delay)
void flash_delay(unsigned long loop_cnt)
{
    R_BSP_ASM_INTERNAL_USED(loop_cnt)
    R_BSP_ASM_BEGIN
    R_BSP_ASM(    BRA.B   R_BSP_ASM_LAB_NEXT(0)    )
    R_BSP_ASM(    NOP                              )
    R_BSP_ASM_LAB(0:                               )
    R_BSP_ASM(    NOP                              )
    R_BSP_ASM(    SUB     #01H, R1                 )
    R_BSP_ASM(    BNE.B   R_BSP_ASM_LAB_PREV(0)    )
    R_BSP_ASM_END
}

/**********************************************************************************************************************
 * Function Name: flash_delay_us
 * Description  : The number of loops is calculated based on the execution time (us)
 *              : and the sytem clock (ICLK) frequency, and the intrinsic function
 *              : that specifies the number of loops is called.
 * Arguments    : us  : Execution time
                : khz : ICLK frequency when calling the function
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
void flash_delay_us(unsigned long us, unsigned long khz)
{
    signed long loop_cnt; /* Argument of R_DELAY function */

    /* Calculation of a loop count */
    loop_cnt = us * khz;
    loop_cnt = (loop_cnt / WAIT_DIV_LOOP_CYCLE );      /* Division about cycle of 1 loop */
    loop_cnt = loop_cnt - WAIT_OVERHEAD_COUNT;         /* Overhead is reduced from a loop count. */

    /* R_DELAY function is performed when loop_cnt is 1 or more. */
    if(loop_cnt > 0)
    {
        flash_delay((unsigned long)loop_cnt);
    }
}

/**********************************************************************************************************************
 * Function Name: flash_pe_mode_enter
 * Description  : Function switches the peripheral to P/E mode for Code Flash or Data Flash.
 *              : And Flash Ready Interrupt enable (BGO mode)
 * Arguments    : flash_type_t : Specify Code Flash or Data Flash
 * Return Value : FLASH_SUCCESS -
 *                Switched successfully.
 *                FLASH_ERR_FAILURE -
 *                Unable to Switch to P/E Mode.
 *                FLASH_ERR_PARAM -
 *                Illegal parameter passed
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_pe_mode_enter(flash_type_t flash_type)
{
    flash_err_t err = FLASH_SUCCESS;

    if (flash_type == FLASH_TYPE_DATA_FLASH)
    {
#ifndef FLASH_NO_DATA_FLASH
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
        /* Enable Flash Ready Interrupt */
        IR(FCU,FRDYI) = 0;
        flash_InterruptRequestEnable(VECT(FCU,FRDYI));
#endif
        flash_df_pe_mode_enter();           /* Sets PCKA clock */
#endif
    }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    else if (flash_type == FLASH_TYPE_CODE_FLASH)
    {
#if (FLASH_CFG_CODE_FLASH_BGO == 1)
        /* Enable Flash Ready Interrupt */
        IR(FCU,FRDYI) = 0;
        flash_InterruptRequestEnable(VECT(FCU,FRDYI));
#endif
        flash_cf_pe_mode_enter();
    }
#endif
    else
    {
        err = FLASH_ERR_PARAM;
    }

    return err;
}

/**********************************************************************************************************************
 * Function Name: flash_pe_mode_exit
 * Description  : Exit PE mode to either DF or CF Read
 *              : And Flash Ready Interrupt disable (BGO mode)
 * Arguments    : None
 * Return Value : FLASH_SUCCESS -
 *                Switched successfully.
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_pe_mode_exit()
{
    if (FLASH.FENTRYR.WORD == 0x0080)
    {
#ifndef FLASH_NO_DATA_FLASH
        flash_df_read_mode_enter();
#endif
    }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    else if (FLASH.FENTRYR.WORD == 0x0001)
    {
        flash_cf_read_mode_enter();
    }
#endif

#if ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1) || (FLASH_CFG_DATA_FLASH_BGO == 1))
    /* Disable Flash Ready Interrupt */
    flash_InterruptRequestDisable(VECT(FCU,FRDYI));
#endif

    return(FLASH_SUCCESS);
}

/**********************************************************************************************************************
 * Function Name: flash_erase
 * Description  : Function erases a block of Code or Data Flash
 * Arguments    : uint32_t block address
 * Return Value : FLASH_SUCCESS -
 *                Block Erased successfully.
 *                FLASH_ERR_TIMEOUT
 *                Erase operation timed out.
 *                FLASH_ERR_FAILURE
 *                Erase operation failed for some other reason
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_erase(const uint32_t block_address, const uint32_t num_blocks)
{
    flash_err_t err = FLASH_SUCCESS;

    if (FLASH.FENTRYR.WORD == 0x0080)
    {
#ifndef FLASH_NO_DATA_FLASH
        flash_df_erase(block_address, num_blocks);
#endif
    }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    else if (FLASH.FENTRYR.WORD == 0x0001)
    {
        flash_cf_erase(block_address, num_blocks);
    }
#endif
    else
    {
        /* should never get here */
        return FLASH_ERR_FAILURE;
    }

    /* Return if in BGO mode. Processing will finish in FRDYI interrupt */
    if ((g_current_parameters.bgo_enabled_cf == true)
     || (g_current_parameters.bgo_enabled_df == true))
    {
        return err;
    }

    /* In blocking mode, wait for FRDY or timeout. Return if error. */
    err = flash_wait_frdy();
    if (err != FLASH_SUCCESS)
    {
        return err;
    }

    return err;
}

/**********************************************************************************************************************
 * Function Name: flash_blankcheck
 * Description  : Function checks if the specified Data Flash address range is blank
 * Arguments    : uint32_t address -
 *                 Start address to perform blank check. Actual address or entry from "flash_block_address_t" enum can be used
 *                uint32_t num_blocks -
 *                 Number of bytes to perform blank check operation for.
 *                flash_res_t *result
 *                 Returns the result of the blank check operation. This field is valid only in non-BGO mode
 *                  operation
 * Return Value : FLASH_ERR_TIMEOUT
 *                Operation timed out.
 *                FLASH_SUCCESS
 *                Operation started successfully (BGO/polling)
 *                Operation completed (Blocking)
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_blankcheck(const uint32_t start_address, const uint32_t num_bytes, flash_res_t *result)
{
    flash_err_t err = FLASH_SUCCESS;

    if (FLASH.FENTRYR.WORD == 0x0080)
    {
#ifndef FLASH_NO_DATA_FLASH
        flash_df_blankcheck(start_address, (start_address + num_bytes) - 1);
#endif
    }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    else if (FLASH.FENTRYR.WORD == 0x0001)
    {
        flash_cf_blankcheck(start_address, (start_address + num_bytes) - 1);
    }
#endif
    else
    {
        /* should never get here */
        return FLASH_ERR_FAILURE;
    }

    /* Return if in BGO mode. Processing will finish in FRDYI interrupt */
    if ((g_current_parameters.bgo_enabled_cf == true)
     || (g_current_parameters.bgo_enabled_df == true))
    {
        return err;
    }

    /* In blocking mode, wait for FRDY or timeout. Return if error. */
    err = flash_wait_frdy();
    if (FLASH_SUCCESS == err)
    {
        *result = FLASH_RES_BLANK;
    }
    else if (FLASH_ERR_FAILURE == err)
    {
        *result = FLASH_RES_NOT_BLANK;
        err = FLASH_SUCCESS;
    }
    else
    {
        /* timeout occurs */
    }

    return err;
}

/**********************************************************************************************************************
 * Function Name: flash_write
 * Description  : Function writes of Code or Data Flash
 * Arguments    : uint32_t src_address - the RAM area to take the data from
 *                uint32_t dest_address - the Flash address to write the data to
 *                uint32_t num - the count of bytes to write
 * Return Value : FLASH_SUCCESS -
 *                Block Erased successfully.
 *                FLASH_ERR_TIMEOUT
 *                Erase operation timed out.
 *                FLASH_ERR_FAILURE
 *                Erase operation failed for some other reason
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_write(const uint32_t src_address, const uint32_t dest_address, uint32_t num)
{
    flash_err_t err = FLASH_SUCCESS;

    g_current_parameters.src_addr = src_address;
    g_current_parameters.dest_addr = dest_address;

    if (FLASH.FENTRYR.WORD == 0x0080)
    {
#ifndef FLASH_NO_DATA_FLASH
        g_current_parameters.total_count = num / FLASH_DF_MIN_PGM_SIZE;
#endif
    }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    else if (FLASH.FENTRYR.WORD == 0x0001)
    {
        g_current_parameters.total_count = num / FLASH_CF_MIN_PGM_SIZE;
    }
#endif
    else
    {
        /* should never get here */
        return FLASH_ERR_FAILURE;
    }

    while(g_current_parameters.total_count > 0)
    {
        g_current_parameters.total_count--;

        /* Conversion to the P/E address from the read address */
        if (FLASH.FENTRYR.WORD == 0x0080)
        {
#ifndef FLASH_NO_DATA_FLASH
            flash_df_write(g_current_parameters.src_addr, g_current_parameters.dest_addr);
#endif
        }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
        else if (FLASH.FENTRYR.WORD == 0x0001)
        {
            flash_cf_write(g_current_parameters.src_addr, g_current_parameters.dest_addr);
        }
#endif

        /* Return if in BGO mode. Processing will finish in FRDYI interrupt */
        if ((g_current_parameters.bgo_enabled_cf == true)
         || (g_current_parameters.bgo_enabled_df == true))
        {
            break;
        }

        /* In blocking mode, wait for FRDY or timeout. Return if error. */
        err = flash_wait_frdy();
        if (err != FLASH_SUCCESS)
        {
            break;
        }

        /* timeout counter reset */
        if (FLASH.FENTRYR.WORD == 0x0080)
        {
#ifndef FLASH_NO_DATA_FLASH
            g_current_parameters.src_addr  += FLASH_DF_MIN_PGM_SIZE;
            g_current_parameters.dest_addr += FLASH_DF_MIN_PGM_SIZE;
            g_current_parameters.wait_cnt  =  WAIT_MAX_DF_WRITE;
#endif
        }
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
        else if (FLASH.FENTRYR.WORD == 0x0001)
        {
            g_current_parameters.src_addr  += FLASH_CF_MIN_PGM_SIZE;
            g_current_parameters.dest_addr += FLASH_CF_MIN_PGM_SIZE;
            g_current_parameters.wait_cnt  =  WAIT_MAX_ROM_WRITE;
        }
#endif
    }

    return err;
}

FLASH_SECTION_CHANGE_END /* end FLASH_SECTION_ROM */

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
/**********************************************************************************************************************
 * Function Name: flash_cf_pe_mode_enter
 * Description  : Executes the sequence to enter P/E mode.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_pe_mode_enter(void)
{
    FLASH.FENTRYR.WORD = FENTRYR_CODEFLASH_PE_MODE;

#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    flash_write_fpmcr(CODEFLASH_PE_MODE);

    FLASH.FISR.BIT.PCKA = (FLASH_FISR_FCLK_FREQ < MCU_CFG_FCLK_HZ) ? 
        (FCLK_MHZ - (((FCLK_MHZ - (FLASH_FISR_FCLK_FREQ / MHZ)) / 2)) - 1) : FCLK_MHZ - 1;
#else
    flash_write_fpmcr(DISCHARGE_1);

    /* Wait for 2us over (tDIS) */
    flash_delay_us(WAIT_TDIS, ICLK_KHZ);

    if (OPCCR_HIGH_SPEED_MODE == SYSTEM.OPCCR.BIT.OPCM)
    {
        flash_write_fpmcr(DISCHARGE_2);
        flash_write_fpmcr(CODEFLASH_PE_MODE);

        /* Wait for 5us over (tMS) */
        flash_delay_us(WAIT_TMS_HIGH, ICLK_KHZ);
    }
    else
    {
        flash_write_fpmcr(DISCHARGE_2 | LVPE_MODE);
        flash_write_fpmcr(CODEFLASH_PE_MODE | LVPE_MODE);

        /* Wait for 3us over (tMS) */
        flash_delay_us(WAIT_TMS_MID, ICLK_KHZ);
    }

    FLASH.FISR.BIT.PCKA = FCLK_MHZ - 1;
#endif
}

/**********************************************************************************************************************
 * Function Name: flash_cf_read_mode_enter
 * Description  : Executes the sequence to enter read mode.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_read_mode_enter(void)
{
#if (FLASH_TYPE_VARIETY != FLASH_TYPE_VARIETY_A)
    flash_write_fpmcr(DISCHARGE_2);

    /* Wait for 2us over (tDIS) */
    flash_delay_us(WAIT_TDIS, ICLK_KHZ);

    flash_write_fpmcr(DISCHARGE_1);
#endif

    flash_write_fpmcr(READ_MODE);

    /* Wait for 5us over (tMS) */
    flash_delay_us(WAIT_TMS_HIGH, ICLK_KHZ);

    FLASH.FENTRYR.WORD = FENTRYR_READ_MODE;

    while (0x0000 != FLASH.FENTRYR.WORD)
    {
        /* Confirm that the written value can be read correctly. */
    }
}

/**********************************************************************************************************************
 * Function Name: flash_cf_erase
 * Description  : Performs block erasing between specified addresses.
 * Arguments    : start_addr   : Start address for erasing
 *              : num          : End address for erasing
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_erase(const uint32_t start_addr, const uint32_t num_blocks)
{
    uint32_t block_start_addr;
    uint32_t block_end_addr;

    block_start_addr = start_addr - CODEFLASH_ADDR_OFFSET;  /* Conversion to the P/E address from the read address */
    block_end_addr   = ((start_addr + (num_blocks * FLASH_CF_BLOCK_SIZE)) - CODEFLASH_ADDR_OFFSET) - 1;

    /* Select User Area */
    FLASH.FASR.BIT.EXS = 0;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* Erase start address setting */
    FLASH.FSARH = (uint16_t)(block_start_addr >> 16);
    FLASH.FSARL = (uint16_t)(block_start_addr & 0xFFF8);

    /* Erase end address setting */
    FLASH.FEARH = (uint16_t)(block_end_addr >> 16);
    FLASH.FEARL = (uint16_t)(block_end_addr & 0xFFF8);
#else
    /* Erase start address setting */
    FLASH.FSARH = (uint8_t)((block_start_addr >> 16) & 0x0F);
    FLASH.FSARL = (uint16_t)(block_start_addr & 0xFFFC);

    /* Erase end address setting */
    FLASH.FEARH = (uint8_t)((block_end_addr >> 16) & 0x0F);
    FLASH.FEARL = (uint16_t)(block_end_addr & 0xFFFC);
#endif

    /* Execute Erase command */
    FLASH.FCR.BYTE = FCR_ERASE;
}

/**********************************************************************************************************************
 * Function Name: flash_cf_blankcheck
 * Description  : Performs blank check for the area between specified addresses.
 * Arguments    : start_addr   : Start address for blank check
 *              : end_addr     : End address for blank check
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_blankcheck(const uint32_t start_addr, const uint32_t end_addr)
{
    uint32_t start_addr_idx;
    uint32_t end_addr_idx;

    start_addr_idx = start_addr - CODEFLASH_ADDR_OFFSET;    /* Conversion to the P/E address from the read address */
    end_addr_idx   = end_addr - CODEFLASH_ADDR_OFFSET;      /* Conversion to the P/E address from the read address */

    /* Select User Area */
    FLASH.FASR.BIT.EXS = 0;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* BlankCheck start address setting */
    FLASH.FSARH = (uint16_t)(start_addr_idx >> 16);
    FLASH.FSARL = (uint16_t)(start_addr_idx & 0xFFF8);

    /* BlankCheck end address setting */
    FLASH.FEARH = (uint16_t)(end_addr_idx >> 16);
    FLASH.FEARL = (uint16_t)(end_addr_idx & 0xFFF8);
#else
    /* BlankCheck start address setting */
    FLASH.FSARH = (uint8_t)((start_addr_idx >> 16) & 0x0F);
    FLASH.FSARL = (uint16_t)(start_addr_idx & 0xFFFC);

    /* BlankCheck end address setting */
    FLASH.FEARH = (uint8_t)((end_addr_idx >> 16) & 0x0F);
    FLASH.FEARL = (uint16_t)(end_addr_idx & 0xFFFC);
#endif

    /* Execute BlankCheck command */
    FLASH.FCR.BYTE = FCR_BLANKCHECK;
}

/**********************************************************************************************************************
 * Function Name: flash_cf_write
 * Description  : Programs the specified data to the Code Flash data.
 * Arguments    : block_start_addr   : Source address for data to be written
 *              : block_end_addr     : End address (read form) for writing
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_write(const uint32_t src_addr, const uint32_t dest_addr)
{
    uint32_t dest_addr_idx;
    uint32_t *psrc_addr = (uint32_t *)src_addr;

    dest_addr_idx = dest_addr - CODEFLASH_ADDR_OFFSET;  /* Conversion to the P/E address from the read address */

    /* Select User Area */
    FLASH.FASR.BIT.EXS = 0;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    /* Write start address setting */
    FLASH.FSARH = (uint16_t)(dest_addr_idx >> 16);
    FLASH.FSARL = (uint16_t)(dest_addr_idx & 0xFFF8);

    FLASH.FWB0 = (uint16_t)(*psrc_addr & 0xFFFF);
    FLASH.FWB1 = (uint16_t)(*psrc_addr >> 16);

    psrc_addr ++;

    FLASH.FWB2 = (uint16_t)(*psrc_addr & 0xFFFF);
    FLASH.FWB3 = (uint16_t)(*psrc_addr >> 16);
#else
    /* Write start address setting */
    FLASH.FSARH = (uint8_t)((dest_addr_idx >> 16) & 0x0F);
    FLASH.FSARL = (uint16_t)(dest_addr_idx & 0xFFFC);

    /* Write data setting */
    FLASH.FWBH = (uint16_t)(*psrc_addr >> 16);
    FLASH.FWBL = (uint16_t)(*psrc_addr & 0xFFFF);
#endif

    /* Execute Write command */
    FLASH.FCR.BYTE = FCR_WRITE;
}

#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

/**********************************************************************************************************************
 * Function Name: flash_write_fpmcr
 * Description  : Specifies the argument set to the FPMCR register by the program sequence.
 * Arguments    : value     : Setting value for the FPMCR register
 * Return Value : None
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_write_fpmcr (uint8_t value)
{
    FLASH.FPR        = 0xA5;
    FLASH.FPMCR.BYTE = value;
    FLASH.FPMCR.BYTE = (uint8_t)~value;
    FLASH.FPMCR.BYTE = value;

    if(value == FLASH.FPMCR.BYTE)
    {
        R_BSP_NOP();
    }
}

/**********************************************************************************************************************
 * Function Name: flash_wait_frdy
 * Description  : Waits for the erase, blankcheck, and write command to be completed 
 *                and verifies the result of the command execution.
 * Arguments    : None.
 * Return Value : FLASH_SUCCESS     - Command executed successfully. (for blankcheck, the area is blank.)
 *                FLASH_ERR_TIMEOUT - Command timed out.
 *                FLASH_ERR_FAILURE - Command failed for some reason. (for blankcheck, the area is not blank.)
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static flash_err_t flash_wait_frdy(void)
{
    /* Check FREADY Flag bit*/
    while (1 != FLASH.FSTATR1.BIT.FRDY)
    {
        /* Check that execute command is completed. */
        /* Wait until FRDY is 0 unless timeout occurs. */
        g_current_parameters.wait_cnt--;

        if (g_current_parameters.wait_cnt <= 0)
        {
            /* if FRDY is not set to 1 after max timeout, return error*/
            return FLASH_ERR_TIMEOUT;
        }
    }

    /* Clear FCR register */
    FLASH.FCR.BYTE = FCR_CLEAR;

    while (0 != FLASH.FSTATR1.BIT.FRDY)
    {
        /* Check that execute command is completed. */
    }

    if ((0 != FLASH.FSTATR0.BIT.ILGLERR) || (0 != FLASH.FSTATR0.BIT.ERERR)
     || (0 != FLASH.FSTATR0.BIT.PRGERR)  || (0 != FLASH.FSTATR0.BIT.BCERR))
    {
        flash_reset();
        return FLASH_ERR_FAILURE;
    }

    return FLASH_SUCCESS;
}

#if ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)) || (FLASH_CFG_DATA_FLASH_BGO == 1)
/**********************************************************************************************************************
 * Function Name: Excep_FCU_FRDYI
 * Description  : ISR that is called when FCU is done with flash operation
 *                NOTE: This function MUST execute from RAM only when
 *                      FLASH_CFG_CODE_FLASH_BGO is enabled.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(Excep_FCU_FRDYI,VECT(FCU,FRDYI))
FLASH_PE_MODE_SECTION
R_BSP_ATTRIB_STATIC_INTERRUPT void Excep_FCU_FRDYI(void)
{
    flash_err_t err = FLASH_SUCCESS;

#if ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1))

    if (FLASH_CUR_CF_BGO_ERASE  == g_current_parameters.current_operation)
    {
        err = flash_wait_frdy();
        if (FLASH_SUCCESS == err)
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERASE_COMPLETE;
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERR_FAILURE;
        }
    }
    else if (FLASH_CUR_CF_BGO_WRITE  == g_current_parameters.current_operation)
    {
        err = flash_wait_frdy();
        if (FLASH_SUCCESS == err)
        {
            if (g_current_parameters.total_count > 0)
            {
                g_current_parameters.src_addr  += FLASH_CF_MIN_PGM_SIZE;
                g_current_parameters.dest_addr += FLASH_CF_MIN_PGM_SIZE;
                g_current_parameters.wait_cnt  =  WAIT_MAX_ROM_WRITE;
                g_current_parameters.total_count--;

                flash_cf_write(g_current_parameters.src_addr, g_current_parameters.dest_addr);
            
                return; 
            }
            else
            {
                g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_WRITE_COMPLETE;
            }
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERR_FAILURE;
        }
    }
    else if (FLASH_CUR_CF_BGO_BLANKCHECK == g_current_parameters.current_operation)
    {
        err = flash_wait_frdy();
        if (FLASH_SUCCESS == err)
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_BLANK;
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_NOT_BLANK;
        }
    }
    else if (FLASH_CUR_CF_ACCESSWINDOW == g_current_parameters.current_operation)
    {
        err = flash_wait_exrdy();
        if (FLASH_SUCCESS == err)
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_SET_ACCESSWINDOW;
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERR_FAILURE;
        }
    }
    else if (FLASH_CUR_CF_TOGGLE_STARTUPAREA == g_current_parameters.current_operation)
    {
        err = flash_wait_exrdy();
        if (FLASH_SUCCESS == err)
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_TOGGLE_STARTUPAREA;
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERR_FAILURE;
        }
    }
    else
    {
        /* Nothing to do */
    }
#endif  // #if ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1))


#ifndef FLASH_NO_DATA_FLASH
#if (FLASH_CFG_DATA_FLASH_BGO == 1)
    if (FLASH_CUR_DF_BGO_ERASE == g_current_parameters.current_operation)
    {
        err = flash_wait_frdy();
        if (FLASH_SUCCESS == err)
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERASE_COMPLETE;
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERR_FAILURE;
        }
    }
    else if (FLASH_CUR_DF_BGO_WRITE  == g_current_parameters.current_operation)
    {
        err = flash_wait_frdy();
        if (FLASH_SUCCESS == err)
        {
            if (g_current_parameters.total_count > 0)
            {
                g_current_parameters.src_addr  += FLASH_DF_MIN_PGM_SIZE;
                g_current_parameters.dest_addr += FLASH_DF_MIN_PGM_SIZE;
                g_current_parameters.wait_cnt  =  WAIT_MAX_DF_WRITE;
                g_current_parameters.total_count--;

                flash_df_write(g_current_parameters.src_addr, g_current_parameters.dest_addr);
            
                return; 
            }
            else
            {
                g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_WRITE_COMPLETE;
            }
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_ERR_FAILURE;
        }
    }
    else if (FLASH_CUR_DF_BGO_BLANKCHECK  == g_current_parameters.current_operation)
    {
        err = flash_wait_frdy();
        if (FLASH_SUCCESS == err)
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_BLANK;
        }
        else
        {
            g_flash_int_ready_cb_args.event = FLASH_INT_EVENT_NOT_BLANK;
        }
    }
    else
    {
        /* Nothing to do */
    }
#endif  // FLASH_CFG_DATA_FLASH_BGO
#endif  // #ifndef FLASH_NO_DATA_FLASH

    /* Release lock and Set current state to Idle */
    flash_pe_mode_exit();
    flash_release_state();
    g_current_parameters.current_operation = FLASH_CUR_IDLE;

    /* call back function execute */
    flash_ready_isr_handler((void *) &g_flash_int_ready_cb_args);
}

#endif  // ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)) || (FLASH_CFG_DATA_FLASH_BGO == 1)

FLASH_SECTION_CHANGE_END /* end FLASH_SECTION_ROM */

#endif /* #ifndef FLASH_HAS_FCU */

/* end of file */
