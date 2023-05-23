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
 * File Name    : cellular_power_down.c
 * Description  : Function to shut down the module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "at_command.h"
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CELLULAR_SHUTDOWN_LIMIT     (50)

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/****************************************************************************
 * Function Name  @fn            cellular_power_down
 ***************************************************************************/
e_cellular_err_t cellular_power_down(st_cellular_ctrl_t * const p_ctrl)
{
    volatile uint8_t           flg           = CELLULAR_FLG_OFF;
    uint8_t                    cnt           = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_ERR_TAKE;

    semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
    if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
    {
        p_ctrl->recv_data = (void *) &flg; //(&uint8_t)->(void *)
        ret               = atc_sqnsshdn(p_ctrl);
        if (CELLULAR_SUCCESS == ret)
        {
            ret = CELLULAR_ERR_MODULE_COM;
            do
            {
                if (CELLULAR_FLG_SHUTDOWN == flg)
                {
                    cnt = CELLULAR_SHUTDOWN_LIMIT;
                    ret = CELLULAR_SUCCESS;
                }
                else
                {
                    cellular_delay_task(1000);
                }
                cnt++;
            } while (cnt < CELLULAR_SHUTDOWN_LIMIT);
        }
        p_ctrl->recv_data = NULL;
        cellular_give_semaphore(p_ctrl->at_semaphore);
    }
    else
    {
        ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
    }

    return ret;
}
/**********************************************************************************************************************
 End of function cellular_power_down
 *********************************************************************************************************************/
