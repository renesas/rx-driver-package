/***********************************************************************************************************************
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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpt_rx.c
 * Version      : 1.00
 * Description  : Functions for using Low Power Timer
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description  
 *         : 01.03.2016 1.00    First Release
 *         : 01.07.2016 1.10    Added support for RX130,RX230,RX231
 *         :                    Lock release then lpt_open() error occurred
 *         : 04.08.2016 1.11    Added command LPT_CMD_COUNT_RESET to R_LPT_Control()
 *         : 04.22.2019 1.23    Delete inline definition of GetVersion function
 ***********************************************************************************************************************/
/*******************************************************************************
  Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Defines for LPT support */
#include "r_lpt_rx_if.h"

/* Include specifics for chosen MCU.  */
#include "r_lpt_rx_private.h"

/*****************************************************************************
* Function Name: R_LPT_Open
* Description  : Initializes the I/O register for LPT control.
* Arguments    : period -
*                    LPT period (microsecond)
* Return Value : LPT_SUCCESS -
*                    Successful operation
*                LPT_ERR_LOCK_FUNC -
*                    Operating. The LPT has been used.
*                LPT_ERR_INVALID_ARG -
*                    An invalid argument included.
******************************************************************************/
lpt_err_t R_LPT_Open(uint32_t const period)
{
    lpt_err_t   err = LPT_SUCCESS;
    bool        chk;

    /*  ---- LPT HARDWARE LOCK ---- */
    chk = r_lpt_mcu_hardware_lock();

    if (false == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        return LPT_ERR_LOCK_FUNC;
    }
    else
    {
        err = lpt_open(period);

        if (LPT_SUCCESS != err)
        {
            r_lpt_mcu_hardware_unlock();
        }
    }
    return (err);
}
/******************************************************************************
 * End of function R_LPT_Open()
 ******************************************************************************/

/*****************************************************************************
* Function Name: R_LPT_Close
* Description  : Resets the LPT driver.
* Arguments    : none
* Return Value : LPT_SUCCESS -
*                    Successful operation
******************************************************************************/
lpt_err_t R_LPT_Close(void)
{
    /* Calls the API function. */
    lpt_close();

    /* ---- Release hardware locks for the LPT. ---- */
    r_lpt_mcu_hardware_unlock();

    return (LPT_SUCCESS);
}
/******************************************************************************
 * End of function R_LPT_Close()
 ******************************************************************************/

/*****************************************************************************
* Function Name: R_LPT_Control
* Description  : This function controls start/stop of the low power timer counter.
* Arguments    : cmd -
*                    command to run
* Return Value : LPT_SUCCESS -
*                    Command completed successfully.
*                LPT_ERR_INVALID_ARG -
*                    The cmd value contains an invalid value.
******************************************************************************/
lpt_err_t R_LPT_Control(lpt_cmd_t const cmd)
{
    lpt_err_t   err = LPT_SUCCESS;

    /* Command control */
    switch(cmd)
    {

    /* start LPT count */
    case (LPT_CMD_START):
        lpt_start();
    break;

    /* stop LPT count */
    case (LPT_CMD_STOP):
        lpt_stop();
    break;

    /* reset LPT count */
    case (LPT_CMD_COUNT_RESET):
        err = lpt_count_reset();
    break;

    /* other */
    default:
        err = LPT_ERR_INVALID_ARG;
    break;
    }

    return err;
}
/******************************************************************************
 * End of function R_LPT_Control()
 ******************************************************************************/

/*****************************************************************************
* Function Name: R_LPT_GetVersion
* Description  : Returns the version of this module. The version number is 
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
uint32_t  R_LPT_GetVersion(void)
{
    uint32_t const version = (LPT_VERSION_MAJOR << 16) | LPT_VERSION_MINOR;

    return version;
}
/******************************************************************************
 * End of function R_LPT_GetVersion()
 ******************************************************************************/

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
