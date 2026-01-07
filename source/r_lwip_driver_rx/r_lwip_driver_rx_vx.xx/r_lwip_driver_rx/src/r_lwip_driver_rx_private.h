/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lwip_driver_rx_private.h
 * Description  : File that defines macro and structure seen only in "r_lwip_driver_rx_api.c" file.
 *********************************************************************************************************************/

#ifndef R_LWIP_DRIVER_RX_PRIVATE_H
#define R_LWIP_DRIVER_RX_PRIVATE_H

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_lwip_driver_rx_if.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define LWIP_DRIVER_TIMER_SET_MAX               (1000)
#define LWIP_DRIVER_TIMER_SET_MIN               (1)

#define LWIP_DRIVER_LINK_CHANGE_FLAG_OFF        (0)
#define LWIP_DRIVER_LINK_CHANGE_FLAG_ON         (1)

#define LWIP_DRIVER_LINK_STATUS_FLAG_OFF        (0)
#define LWIP_DRIVER_LINK_STATUS_FLAG_ON         (1)

/**********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 *********************************************************************************************************************/
#if NO_SYS
void r_lwip_driver_timer_callback (void * pdata);
#endif

uint32_t r_lwip_driver_get_linkstatus(uint32_t eth_ch);

uint32_t r_lwip_driver_get_linkchange(uint32_t eth_ch);

err_t r_lwip_driver_low_level_init(struct netif *netif);

struct pbuf* r_lwip_driver_low_level_input(struct netif *netif);

#if !R_LWIP_DRIVER_USE_TSIP
uint32_t r_lwip_driver_Xorshift(uint32_t z);
#endif /* !R_LWIP_DRIVER_USE_TSIP */

#endif /* R_LWIP_DRIVER_RX_PRIVATE_H */
