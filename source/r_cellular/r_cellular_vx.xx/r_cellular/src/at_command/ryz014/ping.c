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
 * File Name    : ping.c
 * Description  : Function to execute the AT command (PING).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "at_command.h"
#include "cellular_private_api.h"
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == (5)
#define MAX_DOMAIN_NAME_LENGTH      (255)
#define TOTAL_OTHER_BLOCK_SIZE      (MAX_DOMAIN_NAME_LENGTH + 1 + sizeof(void *))
#endif

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
#if BSP_CFG_RTOS_USED == (5)
TX_BLOCK_POOL g_cellular_other_pool;
static uint8_t s_cellular_other_pool[MAX_DOMAIN_NAME_LENGTH + 1];
#endif

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            atc_ping
 ************************************************************************************************/
e_cellular_err_t atc_ping(st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_host,
                            const st_cellular_ping_cfg_t * const p_cfg)
{
    size_t           len                                   = 0;
    uint8_t          str[4][5]                             = {0};
    const uint8_t *  p_command_arg[CELLULAR_MAX_ARG_COUNT] = {0};
    void *           p_malloc_ret                          = NULL;
    e_cellular_err_t ret                                   = CELLULAR_SUCCESS;

#if BSP_CFG_RTOS_USED == (5)
    UINT rtos_ret = TX_DELETED;
#endif
    len = strlen((char *)p_host);   //(uint8_t *) -> (char *)

#if BSP_CFG_RTOS_USED == (1)
    p_malloc_ret = cellular_malloc((len + 1) *sizeof(uint8_t));
#elif BSP_CFG_RTOS_USED == (5)
    if (NULL != p_malloc_ret)
    {
        rtos_ret = tx_block_pool_create(&g_cellular_other_pool, "other pool",
                sizeof(MAX_DOMAIN_NAME_LENGTH + 1), s_cellular_other_pool, TOTAL_OTHER_BLOCK_SIZE);
    }

    if (TX_SUCCESS == rtos_ret)
    {
        rtos_ret = tx_block_allocate(&g_cellular_other_pool, &p_malloc_ret, TX_NO_WAIT);
    }
#endif

    if (NULL != p_malloc_ret)
    {
#if BSP_CFG_RTOS_USED == (5)
        if (TX_SUCCESS == rtos_ret)
#endif
        {
            strncpy((char *)p_malloc_ret, (char *)p_host, len + 1);   // (uint8_t *)->(char *)

            if (NULL != p_cfg)
            {
                sprintf((char *)str[0], "%d", p_cfg->count);    // (uint8_t *)->(char *)
                sprintf((char *)str[1], "%d", p_cfg->len);      // (uint8_t *)->(char *)
                sprintf((char *)str[2], "%d", p_cfg->interval); // (uint8_t *)->(char *)
                sprintf((char *)str[3], "%d", p_cfg->timeout);  // (uint8_t *)->(char *)
            }
            else
            {
                sprintf((char *)str[0], "%d", CELLULAR_PING_REQ_DEFAULT);       // (uint8_t *)->(char *)
                sprintf((char *)str[1], "%d", CELLULAR_PING_MES_MIN);           // (uint8_t *)->(char *)
                sprintf((char *)str[2], "%d", CELLULAR_PING_INTER_MIN);         // (uint8_t *)->(char *)
                sprintf((char *)str[3], "%d", CELLULAR_PING_TIMEOUT_DEFAULT);   // (uint8_t *)->(char *)
            }

            p_command_arg[0] = (uint8_t *)p_malloc_ret; //(void *)->(uint8_t *)
            p_command_arg[1] = str[0];
            p_command_arg[2] = str[1];
            p_command_arg[3] = str[2];
            p_command_arg[4] = str[3];

            atc_generate(p_ctrl->sci_ctrl.atc_buff, gp_at_command[ATC_PING], p_command_arg);

            if (NULL != p_cfg)
            {
                ret = cellular_execute_at_command(p_ctrl, p_cfg->count * p_cfg->timeout * 1000,
                                                    ATC_RETURN_OK, ATC_PING);
            }
            else
            {
                ret = cellular_execute_at_command(p_ctrl,
                                                    CELLULAR_PING_TIMEOUT_DEFAULT * CELLULAR_PING_REQ_DEFAULT * 1000,
                                                    ATC_RETURN_OK, ATC_PING);
            }

            cellular_free(p_malloc_ret);
#if BSP_CFG_RTOS_USED == (5)
            tx_block_pool_delete(&g_cellular_other_pool);
#endif
        }
    }
    else
    {
        ret = CELLULAR_ERR_MEMORY_ALLOCATION;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function atc_ping
 *********************************************************************************************************************/
