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
 * File Name    : sqnscfg.c
 * Description  : Function to execute the AT command (SQNSCFG).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_sqnscfg
 ************************************************************************************************/
e_cellular_err_t atc_sqnscfg(st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no)
{
    uint8_t          str[5][10]                            = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

    sprintf((char *)str[0], "%d", socket_no);   // (&uint8_t[])->(char *)
    sprintf((char *)str[1], "%d",               // (&uint8_t[])->(char *)
            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].packet_data_size);
    sprintf((char *)str[2], "%d",               // (&uint8_t[])->(char *)
            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].exchange_timeout);
    sprintf((char *)str[3], "%d",               // (&uint8_t[])->(char *)
            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].connect_timeout);
    sprintf((char *)str[4], "%d",               // (&uint8_t[])->(char *)
            p_ctrl->p_socket_ctrl[socket_no - CELLULAR_START_SOCKET_NUMBER].send_timeout);

    p_command_arg[0] = str[0];
    p_command_arg[1] = str[1];
    p_command_arg[2] = str[2];
    p_command_arg[3] = str[3];
    p_command_arg[4] = str[4];

    atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_SOCKET_CONFIG_1], p_command_arg);

    ret = cellular_execute_at_command(p_ctrl, p_ctrl->sci_ctrl.atc_timeout, ATC_RETURN_OK, ATC_SOCKET_CONFIG_1);

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_sqnscfg
 *********************************************************************************************************************/
