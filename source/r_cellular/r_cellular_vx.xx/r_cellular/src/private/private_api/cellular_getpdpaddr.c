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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_getpdpaddr.c
 * Description  : Function to obtain a PDP address.
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

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_getpdpaddr
 ***********************************************************************/
void cellular_getpdpaddr(st_cellular_ctrl_t * const p_ctrl, st_cellular_ipaddr_t * const p_addr)
{
    uint32_t dns[4] = {0};
    uint8_t * p_add = NULL;

    p_add = (uint8_t *)strstr((char *)p_ctrl->recv_data, "\"");  //(uint8_t *)<->(char *)
    if (NULL != p_add)
    {
        sscanf((char *)p_add + 1,       //(uint8_t *)->(char *)
                "%ld.%ld.%ld.%ld",
                &dns[0], &dns[1], &dns[2], &dns[3]);

        sprintf((char *)p_addr->ipv4,   //(uint8_t *)->(char *)
                "%ld.%ld.%ld.%ld",
                dns[0], dns[1], dns[2], dns[3]);

        p_add = (uint8_t *)strstr((char *)p_add + 1, "\"");  //(uint8_t *)<->(char *)
    }

    if (NULL != p_add)
    {
        p_add = (uint8_t *)strstr((char *)p_add + 1, "\"");  //(uint8_t *)<->(char *)
    }

    if (NULL != p_add)
    {
        strncpy((char *)p_addr->ipv6, (char *)p_add + 1, CELLULAR_IPV6_ADDR_LENGTH);  //(uint8_t *)->(char *)
    }
}
/**********************************************************************************************************************
 * End of function cellular_getpdpaddr
 *********************************************************************************************************************/
