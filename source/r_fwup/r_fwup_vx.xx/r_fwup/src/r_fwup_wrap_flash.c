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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup_wrap_flash.c
 * Description  : Functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 20.07.2023 2.00    First Release
 *         : 29.09.2023 2.01    Fixed log messages.
 *                              Add parameter checking.
 *                              Added arguments to R_FWUP_WriteImageProgram API.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_wrap_com.h"
#include "r_fwup_wrap_flash.h"

/**** Start user code ****/
#include "r_flash_rx_if.h"
/**** End user code   ****/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/**** Start user code ****/
/**** End user code   ****/

/*
 * Internal flash
 */
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_open
 * Description  : wrapper function for initializing Flash module.
 * Arguments    : None
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash open error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_open(void)
{
    /**** Start user code ****/
    if (FLASH_SUCCESS != R_FLASH_Open())
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_close
 * Description  : wrapper function for closing Flash module.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void r_fwup_wrap_flash_close(void)
{
    /**** Start user code ****/
    R_FLASH_Close();
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_erase
 * Description  : wrapper function for erasing Flash.
 * Arguments    : addr           : erasure destination address
 *                num_blocks     : number of blocks to erase
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash erase error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_erase(uint32_t addr, uint32_t num_blocks)
{
    /**** Start user code ****/
    uint32_t blk_addr;
#if (FLASH_TYPE == FLASH_TYPE_1)
    blk_addr = addr;
#else
    if((FLASH_DF_BLOCK_0 <= addr) && (addr < FLASH_DF_BLOCK_INVALID ))
    {
        blk_addr = addr;
    }
    else
    {
        blk_addr = addr + (FWUP_CFG_CF_BLK_SIZE * (num_blocks - 1));
    }
#endif
    r_fwup_wrap_disable_interrupt();
    flash_err_t ret = R_FLASH_Erase((flash_block_address_t )blk_addr, num_blocks);
    r_fwup_wrap_enable_interrupt();
    if (FLASH_SUCCESS != ret)
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_erase
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_write
 * Description  : wrapper function for writing Flash.
 * Arguments    : src_addr       : write source address
 *                dest_addr      : write destination address
 *                num_bytes      : number of bytes to write
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash write error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_write(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes)
{
    /**** Start user code ****/
    r_fwup_wrap_disable_interrupt();
    flash_err_t ret = R_FLASH_Write(src_addr, dest_addr, num_bytes);
    r_fwup_wrap_enable_interrupt();
    if (FLASH_SUCCESS != ret)
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_write
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_flash_read
 * Description  : wrapper function for reading Flash.
 * Arguments    : buf_addr     : storage destination address
 *                src_addr     : read source address
 *                size         : number of bytes to read
 * Return Value : FWUP_SUCCESS : success
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_flash_read(uint32_t buf_addr, uint32_t src_addr, uint32_t size)
{
    /**** Start user code ****/

    MEMCPY((void FWUP_FAR *)buf_addr, (void FWUP_FAR *)src_addr, size);
    return (FWUP_SUCCESS);

    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_flash_read
 *********************************************************************************************************************/

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_bank_swap
 * Description  : wrapper function for bank swap.
 * Arguments    : none
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash control error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_bank_swap(void)
{
    /**** Start user code ****/

    flash_err_t err;

    r_fwup_wrap_disable_interrupt();
    err = R_FLASH_Control(FLASH_CMD_BANK_TOGGLE, NULL);
    r_fwup_wrap_enable_interrupt();

    if (FLASH_SUCCESS != err)
    {
        return (FWUP_ERR_FLASH);
    }
    return (FWUP_SUCCESS);

    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_bank_swap
 *********************************************************************************************************************/
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */

#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
/*
 * External flash
 */
/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_open
 * Description  : wrapper function for opening external Flash.
 * Arguments    : None
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash open error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_open(void)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_close
 * Description  : wrapper function for closing external Flash.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void r_fwup_wrap_ext_flash_close(void)
{
    /**** Start user code ****/
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_erase
 * Description  : wrapper function for erasing external Flash.
 * Arguments    : addr           : erasure destination address
 *              : num_sectors    : number of sectors to erase
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash erase error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_erase(uint32_t addr, uint32_t num_sectors)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_erase
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_write
 * Description  : wrapper function for writing external Flash.
 * Arguments    : src_addr       : write source address
 *              : dest_addr      : write destination address
 *              : num_bytes      : number of bytes to write
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash write error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_write(uint32_t src_addr, uint32_t dest_addr, uint32_t num_bytes)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_write
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwup_wrap_ext_flash_read
 * Description  : wrapper function for reading external Flash.
 * Arguments    : buf_addr       : storage destination address
 *              : src_addr       : read source address
 *              : size           : number of bytes to read
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash read error
 *********************************************************************************************************************/
e_fwup_err_t r_fwup_wrap_ext_flash_read(uint32_t buf_addr, uint32_t src_addr, uint32_t size)
{
    /**** Start user code ****/
    return (FWUP_SUCCESS);
    /**** End user code   ****/
}
/**********************************************************************************************************************
 End of function r_fwup_wrap_ext_flash_read
 *********************************************************************************************************************/
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */

/*
 * static functions
 */
/**** Start user code ****/
/**** End user code   ****/

