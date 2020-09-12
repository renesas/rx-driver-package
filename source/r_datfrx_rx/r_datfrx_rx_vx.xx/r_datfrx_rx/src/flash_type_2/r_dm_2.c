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
* Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_dm_2.c
* Version      : 2.00
* Description  : DATFRX interface source file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing and Busy check procedure
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_dm_2.h"

#if(FLASH_TYPE == FLASH_TYPE_2)
/************************************************************************************************
Macro definitions
*************************************************************************************************/

/************************************************************************************************
Local Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
extern st_flash_dispatch_2_hndl_t* (g_flash_dispatch_2_hndl);

/* Data size of each id */
const uint16_t gc_dm_data_size[] =
{
    FLASH_DM_CFG_DF_SIZE_NO0,   FLASH_DM_CFG_DF_SIZE_NO1,   FLASH_DM_CFG_DF_SIZE_NO2,   FLASH_DM_CFG_DF_SIZE_NO3,
    FLASH_DM_CFG_DF_SIZE_NO4,   FLASH_DM_CFG_DF_SIZE_NO5,   FLASH_DM_CFG_DF_SIZE_NO6,   FLASH_DM_CFG_DF_SIZE_NO7,
    FLASH_DM_CFG_DF_SIZE_NO8,   FLASH_DM_CFG_DF_SIZE_NO9,   FLASH_DM_CFG_DF_SIZE_NO10,  FLASH_DM_CFG_DF_SIZE_NO11,
    FLASH_DM_CFG_DF_SIZE_NO12,  FLASH_DM_CFG_DF_SIZE_NO13,  FLASH_DM_CFG_DF_SIZE_NO14,  FLASH_DM_CFG_DF_SIZE_NO15,
    FLASH_DM_CFG_DF_SIZE_NO16,  FLASH_DM_CFG_DF_SIZE_NO17,  FLASH_DM_CFG_DF_SIZE_NO18,  FLASH_DM_CFG_DF_SIZE_NO19,
    FLASH_DM_CFG_DF_SIZE_NO20,  FLASH_DM_CFG_DF_SIZE_NO21,  FLASH_DM_CFG_DF_SIZE_NO22,  FLASH_DM_CFG_DF_SIZE_NO23,
    FLASH_DM_CFG_DF_SIZE_NO24,  FLASH_DM_CFG_DF_SIZE_NO25,  FLASH_DM_CFG_DF_SIZE_NO26,  FLASH_DM_CFG_DF_SIZE_NO27,
    FLASH_DM_CFG_DF_SIZE_NO28,  FLASH_DM_CFG_DF_SIZE_NO29,  FLASH_DM_CFG_DF_SIZE_NO30,  FLASH_DM_CFG_DF_SIZE_NO31,
    FLASH_DM_CFG_DF_SIZE_NO32,  FLASH_DM_CFG_DF_SIZE_NO33,  FLASH_DM_CFG_DF_SIZE_NO34,  FLASH_DM_CFG_DF_SIZE_NO35,
    FLASH_DM_CFG_DF_SIZE_NO36,  FLASH_DM_CFG_DF_SIZE_NO37,  FLASH_DM_CFG_DF_SIZE_NO38,  FLASH_DM_CFG_DF_SIZE_NO39,
    /*  FLASH_DM_CFG_DF_SIZE_NO40,  FLASH_DM_CFG_DF_SIZE_NO41,  FLASH_DM_CFG_DF_SIZE_NO42,  FLASH_DM_CFG_DF_SIZE_NO43, */
    /*  FLASH_DM_CFG_DF_SIZE_NO44,  FLASH_DM_CFG_DF_SIZE_NO45,  FLASH_DM_CFG_DF_SIZE_NO46,  FLASH_DM_CFG_DF_SIZE_NO47, */
    /*  FLASH_DM_CFG_DF_SIZE_NO48,  FLASH_DM_CFG_DF_SIZE_NO49,  FLASH_DM_CFG_DF_SIZE_NO50,  FLASH_DM_CFG_DF_SIZE_NO51, */
    /*  FLASH_DM_CFG_DF_SIZE_NO52,  FLASH_DM_CFG_DF_SIZE_NO53,  FLASH_DM_CFG_DF_SIZE_NO54,  FLASH_DM_CFG_DF_SIZE_NO55, */
    /*  FLASH_DM_CFG_DF_SIZE_NO56,  FLASH_DM_CFG_DF_SIZE_NO57,  FLASH_DM_CFG_DF_SIZE_NO58,  FLASH_DM_CFG_DF_SIZE_NO59, */
    /*  FLASH_DM_CFG_DF_SIZE_NO60,  FLASH_DM_CFG_DF_SIZE_NO61,  FLASH_DM_CFG_DF_SIZE_NO62,  FLASH_DM_CFG_DF_SIZE_NO63, */
    /*  FLASH_DM_CFG_DF_SIZE_NO64,  FLASH_DM_CFG_DF_SIZE_NO65,  FLASH_DM_CFG_DF_SIZE_NO66,  FLASH_DM_CFG_DF_SIZE_NO67, */
    /*  FLASH_DM_CFG_DF_SIZE_NO68,  FLASH_DM_CFG_DF_SIZE_NO69,  FLASH_DM_CFG_DF_SIZE_NO70,  FLASH_DM_CFG_DF_SIZE_NO71, */
    /*  FLASH_DM_CFG_DF_SIZE_NO72,  FLASH_DM_CFG_DF_SIZE_NO73,  FLASH_DM_CFG_DF_SIZE_NO74,  FLASH_DM_CFG_DF_SIZE_NO75, */
    /*  FLASH_DM_CFG_DF_SIZE_NO76,  FLASH_DM_CFG_DF_SIZE_NO77,  FLASH_DM_CFG_DF_SIZE_NO78,  FLASH_DM_CFG_DF_SIZE_NO79, */
    /*  FLASH_DM_CFG_DF_SIZE_NO80,  FLASH_DM_CFG_DF_SIZE_NO81,  FLASH_DM_CFG_DF_SIZE_NO82,  FLASH_DM_CFG_DF_SIZE_NO83, */
    /*  FLASH_DM_CFG_DF_SIZE_NO84,  FLASH_DM_CFG_DF_SIZE_NO85,  FLASH_DM_CFG_DF_SIZE_NO86,  FLASH_DM_CFG_DF_SIZE_NO87, */
    /*  FLASH_DM_CFG_DF_SIZE_NO88,  FLASH_DM_CFG_DF_SIZE_NO89,  FLASH_DM_CFG_DF_SIZE_NO90,  FLASH_DM_CFG_DF_SIZE_NO91, */
    /*  FLASH_DM_CFG_DF_SIZE_NO92,  FLASH_DM_CFG_DF_SIZE_NO93,  FLASH_DM_CFG_DF_SIZE_NO94,  FLASH_DM_CFG_DF_SIZE_NO95, */
    /*  FLASH_DM_CFG_DF_SIZE_NO96,  FLASH_DM_CFG_DF_SIZE_NO97,  FLASH_DM_CFG_DF_SIZE_NO98,  FLASH_DM_CFG_DF_SIZE_NO99, */
    /*  FLASH_DM_CFG_DF_SIZE_NO100, FLASH_DM_CFG_DF_SIZE_NO101, FLASH_DM_CFG_DF_SIZE_NO102, FLASH_DM_CFG_DF_SIZE_NO103, */
    /*  FLASH_DM_CFG_DF_SIZE_NO104, FLASH_DM_CFG_DF_SIZE_NO105, FLASH_DM_CFG_DF_SIZE_NO106, FLASH_DM_CFG_DF_SIZE_NO107, */
    /*  FLASH_DM_CFG_DF_SIZE_NO108, FLASH_DM_CFG_DF_SIZE_NO109, FLASH_DM_CFG_DF_SIZE_NO110, FLASH_DM_CFG_DF_SIZE_NO111, */
    /*  FLASH_DM_CFG_DF_SIZE_NO112, FLASH_DM_CFG_DF_SIZE_NO113, FLASH_DM_CFG_DF_SIZE_NO114, FLASH_DM_CFG_DF_SIZE_NO115, */
    /*  FLASH_DM_CFG_DF_SIZE_NO116, FLASH_DM_CFG_DF_SIZE_NO117, FLASH_DM_CFG_DF_SIZE_NO118, FLASH_DM_CFG_DF_SIZE_NO119, */
    /*  FLASH_DM_CFG_DF_SIZE_NO120, FLASH_DM_CFG_DF_SIZE_NO121, FLASH_DM_CFG_DF_SIZE_NO122, FLASH_DM_CFG_DF_SIZE_NO123, */
    /*  FLASH_DM_CFG_DF_SIZE_NO124, FLASH_DM_CFG_DF_SIZE_NO125, FLASH_DM_CFG_DF_SIZE_NO126, FLASH_DM_CFG_DF_SIZE_NO127, */
    /*  FLASH_DM_CFG_DF_SIZE_NO128, FLASH_DM_CFG_DF_SIZE_NO129, FLASH_DM_CFG_DF_SIZE_NO130, FLASH_DM_CFG_DF_SIZE_NO131, */
    /*  FLASH_DM_CFG_DF_SIZE_NO132, FLASH_DM_CFG_DF_SIZE_NO133, FLASH_DM_CFG_DF_SIZE_NO134, FLASH_DM_CFG_DF_SIZE_NO135, */
    /*  FLASH_DM_CFG_DF_SIZE_NO136, FLASH_DM_CFG_DF_SIZE_NO137, FLASH_DM_CFG_DF_SIZE_NO138, FLASH_DM_CFG_DF_SIZE_NO139, */
    /*  FLASH_DM_CFG_DF_SIZE_NO140, FLASH_DM_CFG_DF_SIZE_NO141, FLASH_DM_CFG_DF_SIZE_NO142, FLASH_DM_CFG_DF_SIZE_NO143, */
    /*  FLASH_DM_CFG_DF_SIZE_NO144, FLASH_DM_CFG_DF_SIZE_NO145, FLASH_DM_CFG_DF_SIZE_NO146, FLASH_DM_CFG_DF_SIZE_NO147, */
    /*  FLASH_DM_CFG_DF_SIZE_NO148, FLASH_DM_CFG_DF_SIZE_NO149, FLASH_DM_CFG_DF_SIZE_NO150, FLASH_DM_CFG_DF_SIZE_NO151, */
    /*  FLASH_DM_CFG_DF_SIZE_NO152, FLASH_DM_CFG_DF_SIZE_NO153, FLASH_DM_CFG_DF_SIZE_NO154, FLASH_DM_CFG_DF_SIZE_NO155, */
    /*  FLASH_DM_CFG_DF_SIZE_NO156, FLASH_DM_CFG_DF_SIZE_NO157, FLASH_DM_CFG_DF_SIZE_NO158, FLASH_DM_CFG_DF_SIZE_NO159, */
    /*  FLASH_DM_CFG_DF_SIZE_NO160, FLASH_DM_CFG_DF_SIZE_NO161, FLASH_DM_CFG_DF_SIZE_NO162, FLASH_DM_CFG_DF_SIZE_NO163, */
    /*  FLASH_DM_CFG_DF_SIZE_NO164, FLASH_DM_CFG_DF_SIZE_NO165, FLASH_DM_CFG_DF_SIZE_NO166, FLASH_DM_CFG_DF_SIZE_NO167, */
    /*  FLASH_DM_CFG_DF_SIZE_NO168, FLASH_DM_CFG_DF_SIZE_NO169, FLASH_DM_CFG_DF_SIZE_NO170, FLASH_DM_CFG_DF_SIZE_NO171, */
    /*  FLASH_DM_CFG_DF_SIZE_NO172, FLASH_DM_CFG_DF_SIZE_NO173, FLASH_DM_CFG_DF_SIZE_NO174, FLASH_DM_CFG_DF_SIZE_NO175, */
    /*  FLASH_DM_CFG_DF_SIZE_NO176, FLASH_DM_CFG_DF_SIZE_NO177, FLASH_DM_CFG_DF_SIZE_NO178, FLASH_DM_CFG_DF_SIZE_NO179, */
    /*  FLASH_DM_CFG_DF_SIZE_NO180, FLASH_DM_CFG_DF_SIZE_NO181, FLASH_DM_CFG_DF_SIZE_NO182, FLASH_DM_CFG_DF_SIZE_NO183, */
    /*  FLASH_DM_CFG_DF_SIZE_NO184, FLASH_DM_CFG_DF_SIZE_NO185, FLASH_DM_CFG_DF_SIZE_NO186, FLASH_DM_CFG_DF_SIZE_NO187, */
    /*  FLASH_DM_CFG_DF_SIZE_NO188, FLASH_DM_CFG_DF_SIZE_NO189, FLASH_DM_CFG_DF_SIZE_NO190, FLASH_DM_CFG_DF_SIZE_NO191, */
    /*  FLASH_DM_CFG_DF_SIZE_NO192, FLASH_DM_CFG_DF_SIZE_NO193, FLASH_DM_CFG_DF_SIZE_NO194, FLASH_DM_CFG_DF_SIZE_NO195, */
    /*  FLASH_DM_CFG_DF_SIZE_NO196, FLASH_DM_CFG_DF_SIZE_NO197, FLASH_DM_CFG_DF_SIZE_NO198, FLASH_DM_CFG_DF_SIZE_NO199, */
    /*  FLASH_DM_CFG_DF_SIZE_NO200, FLASH_DM_CFG_DF_SIZE_NO201, FLASH_DM_CFG_DF_SIZE_NO202, FLASH_DM_CFG_DF_SIZE_NO203, */
    /*  FLASH_DM_CFG_DF_SIZE_NO204, FLASH_DM_CFG_DF_SIZE_NO205, FLASH_DM_CFG_DF_SIZE_NO206, FLASH_DM_CFG_DF_SIZE_NO207, */
    /*  FLASH_DM_CFG_DF_SIZE_NO208, FLASH_DM_CFG_DF_SIZE_NO209, FLASH_DM_CFG_DF_SIZE_NO210, FLASH_DM_CFG_DF_SIZE_NO211, */
    /*  FLASH_DM_CFG_DF_SIZE_NO212, FLASH_DM_CFG_DF_SIZE_NO213, FLASH_DM_CFG_DF_SIZE_NO214, FLASH_DM_CFG_DF_SIZE_NO215, */
    /*  FLASH_DM_CFG_DF_SIZE_NO216, FLASH_DM_CFG_DF_SIZE_NO217, FLASH_DM_CFG_DF_SIZE_NO218, FLASH_DM_CFG_DF_SIZE_NO219, */
    /*  FLASH_DM_CFG_DF_SIZE_NO220, FLASH_DM_CFG_DF_SIZE_NO221, FLASH_DM_CFG_DF_SIZE_NO222, FLASH_DM_CFG_DF_SIZE_NO223, */
    /*  FLASH_DM_CFG_DF_SIZE_NO224, FLASH_DM_CFG_DF_SIZE_NO225, FLASH_DM_CFG_DF_SIZE_NO226, FLASH_DM_CFG_DF_SIZE_NO227, */
    /*  FLASH_DM_CFG_DF_SIZE_NO228, FLASH_DM_CFG_DF_SIZE_NO229, FLASH_DM_CFG_DF_SIZE_NO230, FLASH_DM_CFG_DF_SIZE_NO231, */
    /*  FLASH_DM_CFG_DF_SIZE_NO232, FLASH_DM_CFG_DF_SIZE_NO233, FLASH_DM_CFG_DF_SIZE_NO234, FLASH_DM_CFG_DF_SIZE_NO235, */
    /*  FLASH_DM_CFG_DF_SIZE_NO236, FLASH_DM_CFG_DF_SIZE_NO237, FLASH_DM_CFG_DF_SIZE_NO238, FLASH_DM_CFG_DF_SIZE_NO239, */
    /*  FLASH_DM_CFG_DF_SIZE_NO240, FLASH_DM_CFG_DF_SIZE_NO241, FLASH_DM_CFG_DF_SIZE_NO242, FLASH_DM_CFG_DF_SIZE_NO243, */
    /*  FLASH_DM_CFG_DF_SIZE_NO244, FLASH_DM_CFG_DF_SIZE_NO245, FLASH_DM_CFG_DF_SIZE_NO246, FLASH_DM_CFG_DF_SIZE_NO247, */
    /*  FLASH_DM_CFG_DF_SIZE_NO248, FLASH_DM_CFG_DF_SIZE_NO249, FLASH_DM_CFG_DF_SIZE_NO250, FLASH_DM_CFG_DF_SIZE_NO251, */
    /*  FLASH_DM_CFG_DF_SIZE_NO252, FLASH_DM_CFG_DF_SIZE_NO253, FLASH_DM_CFG_DF_SIZE_NO254  */
};

/************************************************************************************************
* Function Name: r_flash_dm_command
* Description  : Mode change to data flash P/E mode. Command call.
* Arguments    : e_dm_cmd_t command_id          ; DATFRX command ID
*              : flash_res_t *result            ; Result storage variable.
* Return values: FLASH_DM_ACCEPT
*              : FLASH_DM_ERR
*------------------------------------------------------------------------------------------------
* Notes        : Command call process
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_command(e_dm_cmd_t command_id, flash_res_t *result)
{
    flash_err_t flash_err = FLASH_SUCCESS;
    volatile e_flash_dm_status_t ret = FLASH_DM_ACCEPT;

    /* cast from uint32_t to uint16_t */
    g_flash_dispatch_2_hndl->flash_state = (uint16_t)DATF_FLASH_CALL;

    if(command_id == DATF_PROGRAM_CMD)
    {
        /* cast from uint32_t to uint32_t */
        flash_err = R_FLASH_Write((uint32_t)g_flash_dispatch_2_hndl->program_data \
                                  , (uint32_t)g_flash_dispatch_2_hndl->command_addr \
                                  , g_flash_dispatch_2_hndl->program_counter);
    }
    else if(command_id == DATF_ERASE_CMD)
    {
        /* cast from uint32_t to flash_block_address_t and uint32_t */
        flash_err = R_FLASH_Erase((flash_block_address_t)g_flash_dispatch_2_hndl->command_addr, (uint32_t)1);
    }
    else if(command_id == DATF_BLANK_CHECK_CMD)
    {
        if((g_flash_dispatch_2_hndl->activity == FLASH_DM_ACT_FORMATTING)
        ||(g_flash_dispatch_2_hndl->activity == FLASH_DM_ACT_ERASING))
        {
            /* Multiple interrupt avoidance processing */
            FlashBlankCheckDone(FLASH_RES_NOT_BLANK);
        }
        else
        {
            /* cast from uint32_t to uint32_t */
            flash_err = R_FLASH_BlankCheck((uint32_t)g_flash_dispatch_2_hndl->command_addr \
                                           , (uint32_t)BLANK_CHECK_SMALLEST \
                                           , result);
            /* WAIT_LOOP */
            if(g_flash_dispatch_2_hndl->activity == FLASH_DM_ACT_INITIALIZING)
            {
                while(r_flash_dm_drvif_flash_busy() != 0);
            }
        }
    }
    else
    {
        /* Do nothing */
    }

    if(flash_err == FLASH_SUCCESS)
    {
        ret = FLASH_DM_ACCEPT;
    }
    else
    {
        ret = FLASH_DM_ERR;
    }

    return ret;
}

/************************************************************************************************
 * Outline          : Enable to issue command to dataflash.
 *-----------------------------------------------------------------------------------------------
 * Declaration      : void r_flash_dm_enable_rw(void)
 *-----------------------------------------------------------------------------------------------
 * Functions        : Change mode to dataflash P/E mode
 *                  : Set register to P/E enable
 *                  : Read permission.
 *                  : Write permission.
 *-----------------------------------------------------------------------------------------------
 * Arguments        : void
 *-----------------------------------------------------------------------------------------------
 * Return values    : void
 ************************************************************************************************/
void r_flash_dm_enable_rw(void)
{
    flash_access_window_config_t df_access;

    df_access.read_en_mask = 0x2D0F;
    df_access.write_en_mask = 0x1E0F;

    R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_SET, (void *)&df_access);
}

/************************************************************************************************
 * Outline          : Disable to issue command to dataflash.
 *-----------------------------------------------------------------------------------------------
 * Declaration      : void r_flash_dm_disable_rw(void)
 *-----------------------------------------------------------------------------------------------
 * Functions        : Change mode to dataflash read mode
 *                  : Set register to read enable
 *                  : Read permission.
 *                  : Write permission.
 *-----------------------------------------------------------------------------------------------
 * Arguments        : void
 *-----------------------------------------------------------------------------------------------
 * Return values    : void
 ************************************************************************************************/
void r_flash_dm_disable_rw(void)
{
    flash_access_window_config_t df_access;

    df_access.read_en_mask = 0x2D00;
    df_access.write_en_mask = 0x1E00;

    R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_SET, (void *)&df_access);
}

/************************************************************************************************
 * Outline          : These functions are only used when BGO (non-blocking) mode is enabled
 *-----------------------------------------------------------------------------------------------
 * Declaration      : void FlashEraseDone(void)
 *-----------------------------------------------------------------------------------------------
 * Functions        : Callback function to call when flash DATF_STATUS_ERASE is finished
 *-----------------------------------------------------------------------------------------------
 * Arguments        : void
 *-----------------------------------------------------------------------------------------------
 * Return values    : void
 ************************************************************************************************/
void FlashEraseDone(void)
{
    g_flash_dispatch_2_hndl->flash_state = DATF_FLASH_IDLE;
    g_flash_dispatch_2_hndl->error_code = (e_flash_dm_status_t)r_flash_dm_advance();

    if (0 != r_flash_dm_callbackfunc)
    {
        if(g_flash_dispatch_2_hndl->error_code == FLASH_DM_ERR_REQUEST_INIT)
        {
            switch(g_flash_dispatch_2_hndl->activity)
            {
                case    FLASH_DM_ACT_FORMATTING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_FORMAT);
                }
                break;

                case    FLASH_DM_ACT_ERASING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_ERASE);
                }
                break;

                default :
                {
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_FORMAT);
                }
            }
        }
    }

}
/* Callback function to call when flash IDLE is finished */

/************************************************************************************************
 * Outline          : These functions are only used when BGO (non-blocking) mode is enabled
 *-----------------------------------------------------------------------------------------------
 * Declaration      : void FlashWriteDone(void)
 *-----------------------------------------------------------------------------------------------
 * Functions        : Callback function to call when flash IDLE is finished
 *-----------------------------------------------------------------------------------------------
 * Arguments        : void
 *-----------------------------------------------------------------------------------------------
 * Return values    : void
 ************************************************************************************************/
void FlashWriteDone(void)
{
    g_flash_dispatch_2_hndl->flash_state = DATF_FLASH_IDLE;
    g_flash_dispatch_2_hndl->error_code = (e_flash_dm_status_t)r_flash_dm_advance();

    if (0 != r_flash_dm_callbackfunc)
    {
        if(g_flash_dispatch_2_hndl->error_code == FLASH_DM_ERR_REQUEST_INIT)
        {
            switch(g_flash_dispatch_2_hndl->activity)
            {
                case    FLASH_DM_ACT_FORMATTING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_FORMAT);
                }
                break;

                case    FLASH_DM_ACT_WRITING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_WRITE);
                }
                break;

                default :
                {
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_FORMAT);
                }
            }
        }
    }

}
/* Function to take care of flash errors */

/************************************************************************************************
 * Outline          : These functions are only used when BGO (non-blocking) mode is enabled
 *-----------------------------------------------------------------------------------------------
 * Declaration      : void FlashError(void)
 *-----------------------------------------------------------------------------------------------
 * Functions        : Function to take care of flash errors
 *-----------------------------------------------------------------------------------------------
 * Arguments        : void
 *-----------------------------------------------------------------------------------------------
 * Return values    : void
 ************************************************************************************************/
void FlashError(void)
{
    g_flash_dispatch_2_hndl->flash_state = DATF_FLASH_IDLE;
}
/* Callback function to call when flash blank check is finished. 'result'
   argument is 0 if block was blank and 1 if it was not */

/************************************************************************************************
 * Outline          : These functions are only used when BGO (non-blocking) mode is enabled
 *-----------------------------------------------------------------------------------------------
 * Declaration      : void FlashBlankCheckDone(uint8_t result)
 *-----------------------------------------------------------------------------------------------
 * Functions        : Callback function to call when flash blank check is finished.
 *                    'result' argument is 0 if block was blank and 1 if it was not
 *-----------------------------------------------------------------------------------------------
 * Arguments        : void
 *-----------------------------------------------------------------------------------------------
 * Return values    : void
 ************************************************************************************************/
void FlashBlankCheckDone(uint8_t result)
{

    g_flash_dispatch_2_hndl->flash_state = DATF_FLASH_IDLE;

    if(result == FLASH_RES_BLANK)
    {
        g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_BLANK;
    }
    else if(result == FLASH_RES_NOT_BLANK)
    {
        g_flash_dispatch_2_hndl->blankcheck_result = DATF_BLANKCHECK_NONBLANK;
    }
    else
    {
        /* Do Nothing */
    }

    if(g_flash_dispatch_2_hndl->activity != FLASH_DM_ACT_INITIALIZING)
    {
        g_flash_dispatch_2_hndl->error_code = (e_flash_dm_status_t)r_flash_dm_advance();
    }
}

/************************************************************************************************
* Function Name: r_flash_dm_set_dm
* Description  : Copy the handle's address.
* Arguments    : uint32_t   handle   ; Current handle
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : Note.
*************************************************************************************************/
void r_flash_dm_set_dm(uint32_t handle)
{
    /* cast from uint32_t to st_flash_dispatch_2_hndl_t* */
    g_flash_dispatch_2_hndl = (st_flash_dispatch_2_hndl_t*)handle;
}

#endif /* (FLASH_TYPE == FLASH_TYPE_2) */

/* End of File */
