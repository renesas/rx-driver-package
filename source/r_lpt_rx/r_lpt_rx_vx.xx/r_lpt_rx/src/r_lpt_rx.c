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
 * Version      : 2.00
 * Description  : Functions for using Low Power Timer
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description  
 *         : 01.03.2016 1.00    First Release
 *         : 01.07.2016 1.10    Added support for RX130,RX230,RX231
 *         :                    Lock release then lpt_open() error occurred
 *         : 04.08.2016 1.11    Added command LPT_CMD_COUNT_RESET to R_LPT_Control()
 *         : 04.22.2019 1.23    Delete inline definition of GetVersion function
 *         : 10.06.2020 2.00    Added support for RX23W
 *                              Modified comment of API function to Doxygen style.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
  Includes <System Includes> , "Project Includes"
************************************************************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Defines for LPT support */
#include "r_lpt_rx_if.h"

/* Include specifics for chosen MCU. */
#include "r_lpt_rx_private.h"

/***********************************************************************************************************************
 * Function Name: R_LPT_Open
 ******************************************************************************************************************/ /**
 * @brief The function initializes the LPT FIT module. This function must be called before calling any other API        
 *        functions.
 * @param[in] period 
 *            LPT cycle (unit: microsecond)
 * @retval    LPT_SUCCESS: Processing completed successfully.
 * @retval    LPT_ERR_LOCK_FUNC: Operating. LPT has been used.
 * @retval    LPT_ERR_INVALID_ARG: Argument has an invalid value.
 * @details   The initialization is performed to start LPT operation and then the LPT cycle specified with the argument 
 *            is set.\n
 *            Operations included in the initialization are as follows:
 *            \li Enables exiting software standby mode using the LPT.
 *            \li Sets the LPT clock source and the division ratio.
 *            \li Enables compare match 0.
 *            \li Sets the LPT cycle.
 *            \li Sets the value of LPT compare match 0.
 *            \li Provides the LPT clock.
 *            \li Resets the LPT.
 * @note      Call this function while the LPT clock source oscillation is stabilized.\n
 *            When the sub-clock oscillator is selected as the LPT clock source, the LPT cycle must be specified from 
 *            153 to 64000488.\n
 *            When the IWDT-dedicated on-chip oscillator is selected as the LPT clock source, the LPT cycle must be
 *            specified from 334 to 139811199.\n
 *            When the IWDT-dedicated on-chip oscillator is selected as the LPT clock source, set the OFS0.IWDTSLCSTP 
 *            bit to 0 (counting stop is disabled) in IWDT auto-start mode, and set the IWDTCSTPR.SLCSTP bit to 0 
 *            (counting stop is disabled) in other modes.\n
 *            MCU executes the program after MCU waits for the stability time for Main Clock Oscillator Wait Control
 *            Register (SMOSCWTCR) when MCU is resumed from software standby mode.
 */
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
/***********************************************************************************************************************
 * End of function R_LPT_Open()
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LPT_Close
 ******************************************************************************************************************/ /**
 * @brief This function performs processing to stop the LPT.
 * @retval    LPT_SUCCESS: Processing completed successfully.
 * @details   The following operations are performed to stop the LPT.
 *            \li Stops the LPT.
 *            \li Resets the LPT if the LPT clock is provided.
 *            \li Stops the LPT clock.
 *            \li Resets the value of LPT compare match 0.
 *            \li Resets the LPT cycle.
 *            \li Resets low-power timer control register 1.
 *            \li Disables exiting software standby mode using the LPT.
 * @note      Configure the LPT settings in the R_LPT_Open function first, wait one or more cycles of the LPT clock
 *            source, and then call this function.
 */
lpt_err_t R_LPT_Close(void)
{
    /* Calls the API function. */
    lpt_close();

    /* ---- Release hardware locks for the LPT. ---- */
    r_lpt_mcu_hardware_unlock();

    return (LPT_SUCCESS);
}
/***********************************************************************************************************************
 * End of function R_LPT_Close()
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LPT_Control
 ******************************************************************************************************************/ /**
 * @brief This function performs processing to start, stop, or reset LPT count.
 * @param[in] cmd
 *            Command to be executed (see 2.9, Parameters in the application note).
 * @retval    LPT_SUCCESS: Processing completed successfully.
 * @retval    LPT_ERR_INVALID_ARG: Argument has an invalid value.
 * @retval    LPT_ERR_CONDITION_NOT_MET: Condition not met.
 * @details   This API function controls start/stop of LPT count.\n
 *            The counter reset command (LPT_CMD_COUNT_RESET) must be executed while LPT count stops. If
 *            LPT_CMD_COUNT_RESET is executed during counting, LPT_ERR_CONDITION_NOT_MET is returned.
 * @note      Call this function after the LPT have been configured in the R_LPT_Open function.
 */
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
/***********************************************************************************************************************
 * End of function R_LPT_Control()
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LPT_GetVersion
 ******************************************************************************************************************/ /**
 * @brief This function returns the module version.
 * @return    Version number
 * @details   Returns the module version number. The version number is encoded where the top 2 bytes are the major
 *            version number and the bottom 2 bytes are the minor version number.
 * @note      None
 */
uint32_t  R_LPT_GetVersion(void)
{
    uint32_t const version = (LPT_VERSION_MAJOR << 16) | LPT_VERSION_MINOR;

    return version;
}
/***********************************************************************************************************************
 * End of function R_LPT_GetVersion()
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
