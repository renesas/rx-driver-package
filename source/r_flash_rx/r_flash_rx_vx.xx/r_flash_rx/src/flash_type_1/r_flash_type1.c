/********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014-2021 Renesas Electronics Corporation. All rights reserved.
********************************************************************************************************************/
/*******************************************************************************************************************
* File Name : r_flash_type1.c
* Description  : This module implements the Flash API layer functions for the RX1xx and RX23x
********************************************************************************************************************/
/*******************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           12.11.2014 1.10    Support for RX110, RX111, RX113
*                              Changed WAIT_MAX_ROM_WRITE to WAIT_MAX_ERASE_DF_1K in flash_api_erase().
*           19.12.2014 1.20    Replaced some equate names, modified for FLASH_TYPE_1, changed some wait_cnt settings.
*           12.01.2015 1.30    Updates for RX231
*           01.09.2015 1.40    Modified to use FLASH_NO_DATA_FLASH for RX23T and RX110.
*           13.11.2015 1.50    Added FLASH_CMD_CACHE_xxx for RX24T.
*           22.07.2016 2.00    Modified for BSPless flash.
*           17.11.2016 2.10    Added flash_stop() before issuing a FLASH_CMD_RESET.
*                              Added DF and CF block boundary check to flash_api_erase().
*                              Added CF block boundary check to FLASH_CMD_ACCESSWINDOW_SET.
*                              Added non-NULL argument check for FLASH_CMD_SWAPSTATE_SET and FLASH_CMD_SWAPSTATE_GET.
*                              Added valid SAS value check for FLASH_CMD_SWAPSTATE_SET.
*                              Added check for BUSY in flash_api_control() when in BGO mode.
*                              Added check in Open() for another operation in progress when in BGO mode.
*           18.11.2016 3.00    Removed functions common to other MCUs for new merged source code.
*           02.08.2017 3.10    Removed #include "r_mcu_config.h". Now in targets.h (r_flash_rx_if.h includes)
*           19.04.2019 4.00    Added support for GNUC and ICCRX.
*           23.04.2021 4.80    Added RX140.
********************************************************************************************************************/

/********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "r_flash_rx_if.h"

#if (FLASH_TYPE == FLASH_TYPE_1)
#include "r_flash_nofcu.h"
#include "r_flash_type1_if.h"
#include "r_flash_group.h"

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)

static void flash_cf_set_startup_area(uint32_t value);
static void flash_cf_set_access_window(const uint32_t start_addr_startup_value, const uint32_t end_addr);

#define FLASH_PE_MODE_SECTION    R_BSP_ATTRIB_SECTION_CHANGE(P, FRAM)
#define FLASH_SECTION_CHANGE_END R_BSP_ATTRIB_SECTION_CHANGE_END

/**********************************************************************************************************************
 * Function Name: flash_get_current_swap_state
 * Description  : Return which startup area (Default or Alternate) is active
 * Arguments    : none
 * Return Value : startup_area_select - 0 ==> The start-up area is selected
 *                                            according to the start-up area
 *                                            settings of the extra area
 *                                      2 ==> The start-up area is switched to
 *                                            the default area temporarily.
 *                                      3 ==> The start-up area is switched to
 *                                            the alternate area temporarily.
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
uint8_t flash_get_current_swap_state(void)
{
    uint8_t startup_area_select;

    startup_area_select = FLASH.FISR.BIT.SAS;
    return(startup_area_select);
}

/**********************************************************************************************************************
 * Function Name: flash_set_current_swap_state
 * Description  : Setting for switching the start-up area
 * Arguments    : value for SAS bits; switch startup area if value = SAS_SWITCH_AREA
 * Return Value : none
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
void flash_set_current_swap_state(uint8_t value)
{
    uint8_t sas_flag;

    if (FLASH_SAS_SWITCH_AREA == value)                 // switch startup areas
    {
        if(FLASH_SAS_EXTRA == FLASH.FISR.BIT.SAS)       // switch based upon FISR.SAS reset area
        {
            if(SASMF_DEFAULT == FLASH.FSCMR.BIT.SASMF)
            {
                sas_flag = FLASH_SAS_ALTERNATE;
            }
            else
            {
                sas_flag = FLASH_SAS_DEFAULT;
            }
        }
        else
        {
            if(FLASH_SAS_ALTERNATE == FLASH.FISR.BIT.SAS)   // switch based upon current state
            {
                sas_flag = FLASH_SAS_DEFAULT;
            }
            else
            {
                sas_flag = FLASH_SAS_ALTERNATE;
            }
        }
    }
    else
    {
        sas_flag = value;       /* to set SAS to desired area */
    }

    flash_pe_mode_enter(FLASH_TYPE_CODE_FLASH);

    FLASH.FISR.BIT.SAS = sas_flag;

    while(sas_flag != FLASH.FISR.BIT.SAS)
    {
        /* Confirm that the written value can be read correctly. */
    }

    flash_pe_mode_exit();
}

/**********************************************************************************************************************
 * Function Name: flash_get_current_startup_area
 * Description  : Return which startup area (Default or Alternate) is active
 * Arguments    : none
 * Return Value : startup_area_flag - 0 ==> Alternate area
 *                                    1 ==> Default area
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
uint8_t flash_get_current_startup_area(void)
{
    uint8_t startup_area_flag;
    uint16_t reg_fscmr;

    reg_fscmr = FLASH.FSCMR.WORD;
    startup_area_flag  = (uint8_t)((reg_fscmr >> 8) & 0x01);
    return(startup_area_flag);
}

/**********************************************************************************************************************
 * Function Name: flash_toggle_startup_area
 * Description  : Check the current start-up area setting
 *                and specifies the area currently not used as the start-up area.
 * Arguments    : none
 * Return Value : FLASH_SUCCESS -
 *                Switched successfully.
 *                FLASH_ERR_FAILURE -
 *                Unable to Switch to P/E Mode.
 *                FLASH_ERR_PARAM -
 *                Illegal parameter passed
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_toggle_startup_area(void)
{
    uint8_t startup_area_flag;
    uint16_t reg_fscmr;
    flash_err_t err = FLASH_SUCCESS;

    err = flash_pe_mode_enter(FLASH_TYPE_CODE_FLASH);
    if (FLASH_SUCCESS != err)
    {
        return(err);
    }

    g_current_parameters.current_operation = FLASH_CUR_CF_TOGGLE_STARTUPAREA;
    g_current_parameters.wait_cnt          = WAIT_MAX_EXRDY_CMD_TIMEOUT;
    
    reg_fscmr = FLASH.FSCMR.WORD;
    startup_area_flag  = (uint8_t)((reg_fscmr >> 8) & 0x01);

    if (DEFAULT_AREA == startup_area_flag)
    {
        flash_cf_set_startup_area(STARTUP_AREA_VALUE);
    }
    else
    {
        flash_cf_set_startup_area(DEFAULT_AREA_VALUE);
    }

    reg_fscmr = FLASH.FSCMR.WORD;

    /* Return if in BGO mode. Processing will finish in FRDYI interrupt */
    if ((g_current_parameters.bgo_enabled_df == true)
     || (g_current_parameters.bgo_enabled_cf == true))
    {
        return err;
    }

    /* In blocking mode, wait for EXRDY or timeout. */
    err = flash_wait_exrdy();

    /* In blocking mode, The return value from flash_wait_exrdy() is in any case, do the flash_pe_mode_exit(). */
    flash_pe_mode_exit();

    return err;
}

/**********************************************************************************************************************
 * Function Name: flash_cf_set_startup_area
 * Description  : Set the start-up area information that is specified by the argument.
 * Arguments    : value : Setting information for the FWBH register
 * Return Value : none
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_set_startup_area (uint32_t value)
{
    /* Select Extra Area */
    FLASH.FASR.BIT.EXS = 1;

#if defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL) || (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    FLASH.FWB3 = (uint16_t)(0xFFFF);
    FLASH.FWB2 = (uint16_t)(0xFFFF);
    FLASH.FWB1 = (uint16_t)(0xFFFF);
    FLASH.FWB0 = (uint16_t)(value);
#else
    FLASH.FWBH = (uint16_t)(0xFFFF);
    FLASH.FWBL = (uint16_t)(value);
#endif

    /* Execute Startup Area Flag command */
    FLASH.FEXCR.BYTE = FEXCR_STARTUP;
}

/**********************************************************************************************************************
 * Function Name: flash_get_access_window
 * Description  : Return the read address form of the current access window area setting
 * Arguments    : none
 * Return Value : FLASH_SUCCESS
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_get_access_window(flash_access_window_config_t *pAccessInfo)
{
    if (FLASH_ACCESS_WINDOW_END_VALUE == FLASH.FAWSMR)
    {
        pAccessInfo->start_addr = (uint32_t)FLASH_CF_BLOCK_END;
    }
    else
    {
#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
        pAccessInfo->start_addr = ((FLASH.FAWSMR << 11) | CODEFLASH_ADDR_OFFSET);
#else
        pAccessInfo->start_addr = ((FLASH.FAWSMR << 10) | CODEFLASH_ADDR_OFFSET | 0xFC000000);
#endif
    }
    if (FLASH_ACCESS_WINDOW_END_VALUE == FLASH.FAWEMR)
    {
        pAccessInfo->end_addr = (uint32_t)FLASH_CF_BLOCK_END;
    }
    else
    {
#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
        pAccessInfo->end_addr = ((FLASH.FAWEMR << 11) | CODEFLASH_ADDR_OFFSET);
#else
        pAccessInfo->end_addr = ((FLASH.FAWEMR << 10) | CODEFLASH_ADDR_OFFSET | 0xFC000000);
#endif
    }

    return FLASH_SUCCESS;
}

/**********************************************************************************************************************
 * Function Name: flash_set_access_window
 * Description  : Specifies the setting for the access window.
 * Arguments    : start_addr : start address of Access Window Setting
 *                end_addr   : end address of Access Window Setting. This should be one
 *                            beyond the actual last byte to allow write access for.
 *                            here as required by the spec.
 * Return Value : FLASH_SUCCESS            - Command executed successfully
 *                FLASH_ERR_ACCESSW        - AccessWindow setting error
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_set_access_window(flash_access_window_config_t  *pAccessInfo)
{
    flash_err_t err = FLASH_SUCCESS;
    uint32_t    start_addr_idx;
    uint32_t    end_addr_idx;

    err = flash_pe_mode_enter(FLASH_TYPE_CODE_FLASH);
    if (FLASH_SUCCESS != err)
    {
        return(err);
    }

    g_current_parameters.current_operation = FLASH_CUR_CF_ACCESSWINDOW;
    g_current_parameters.wait_cnt          = WAIT_MAX_EXRDY_CMD_TIMEOUT;
    
    /* Conversion to the P/E address from the read address */
    if ((uint32_t)FLASH_CF_BLOCK_END == pAccessInfo->start_addr)
    {
#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
        start_addr_idx = FLASH_ACCESS_WINDOW_END_VALUE << 11;
#else
        start_addr_idx = FLASH_ACCESS_WINDOW_END_VALUE << 10;
#endif
    }
    else
    {
        start_addr_idx = (pAccessInfo->start_addr) - CODEFLASH_ADDR_OFFSET;
    }
    if ((uint32_t)FLASH_CF_BLOCK_END == pAccessInfo->end_addr)
    {
#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
        end_addr_idx = FLASH_ACCESS_WINDOW_END_VALUE << 11;
#else
        end_addr_idx = FLASH_ACCESS_WINDOW_END_VALUE << 10;
#endif
    }
    else
    {
        end_addr_idx = (pAccessInfo->end_addr) - CODEFLASH_ADDR_OFFSET;
    }

    flash_cf_set_access_window(start_addr_idx, end_addr_idx);

    /* Return if in BGO mode. Processing will finish in FRDYI interrupt */
    if ((g_current_parameters.bgo_enabled_df == true)
     || (g_current_parameters.bgo_enabled_cf == true))
    {
        return err;
    }

    /* In blocking mode, wait for EXRDY or timeout. */
    err = flash_wait_exrdy();

    /* In blocking mode, The return value from flash_wait_exrdy() is in any case, do the flash_pe_mode_exit(). */
    flash_pe_mode_exit();

    return err;
}

/**********************************************************************************************************************
 * Function Name: flash_cf_set_access_window
 * Description  : Issues a command to the extra area.
 * Arguments    : start_addr_startup_value : start Address of the AccessWindow, or StartupAreaflag setting value
 *                end_addr : end Address of AccessWindow Setting, or Dummy value (start-up area information program)
 * Return Value : none
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
static void flash_cf_set_access_window(const uint32_t start_addr_startup_value, const uint32_t end_addr)
{
    /* Select Extra Area */
    FLASH.FASR.BIT.EXS = 1;

#if (FLASH_TYPE_VARIETY == FLASH_TYPE_VARIETY_A)
    FLASH.FWB1 = (uint16_t)((end_addr >> 11) & 0xFFF);
    FLASH.FWB0 = (uint16_t)((start_addr_startup_value >> 11) & 0xFFF);
#elif defined(MCU_RX23_ALL) || defined(MCU_RX24_ALL)
    FLASH.FWB1 = (uint16_t)((end_addr >> 10) & 0xFFF);
    FLASH.FWB0 = (uint16_t)((start_addr_startup_value >> 10) & 0xFFF);
#else
    FLASH.FWBH = (uint16_t)(end_addr >> 10);
    FLASH.FWBL = (uint16_t)(start_addr_startup_value >> 10);
#endif

    /* Execute Access Window command */
    FLASH.FEXCR.BYTE = FEXCR_AW;
}

/**********************************************************************************************************************
 * Function Name: flash_wait_exrdy
 * Description  : Waits for completing of the command execution
 *                and verifies the result of the execution.
 * Arguments    : none
 * Return Value : FLASH_SUCCESS          - Command executed successfully
 *                FLASH_ERR_TIMEOUT      - Command timed out.
 *                FLASH_ERR_ACCESSW      - Extra area command error
 *********************************************************************************************************************/
FLASH_PE_MODE_SECTION
flash_err_t flash_wait_exrdy(void)
{
    /* Check EXFREADY Flag bit*/
    while (1 != FLASH.FSTATR1.BIT.EXRDY)
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

    /* Clear FEXCR register */
    FLASH.FEXCR.BYTE = FEXCR_CLEAR;

    while (0 != FLASH.FSTATR1.BIT.EXRDY)
    {
        /* Check that execute command is completed. */
    }

    if ((0 != FLASH.FSTATR0.BIT.EILGLERR) || (0 != FLASH.FSTATR0.BIT.PRGERR))
    {
        flash_reset();
        return FLASH_ERR_ACCESSW;
    }

    return FLASH_SUCCESS;
}

FLASH_SECTION_CHANGE_END /* end FLASH_SECTION_ROM */

#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

#endif /* #if (FLASH_TYPE == FLASH_TYPE_1) */

/* end of file */
