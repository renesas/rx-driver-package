/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lwip_driver_rx_if.h
 * Description  : lwIP Driver interface
 *********************************************************************************************************************/

#ifndef R_LWIP_DRIVER_RX_IF_H
#define R_LWIP_DRIVER_RX_IF_H

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "lwip/err.h"
#include "lwip/netif.h"

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    LWIP_DRV_SUCCESS        = 0,    // 
    LWIP_DRV_ERR_OPEN       = -1,   // 
    LWIP_DRV_ERR_NOT_OPEN   = -2,   // 
    LWIP_DRV_ERR_ARG        = -3,   // 
    LWIP_DRV_ERR_CMT        = -4    // 
} e_lwip_drv_return_t;

/**********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 *********************************************************************************************************************/

e_lwip_drv_return_t R_LWIP_DRIVER_Open (uint32_t *cmt_ch, uint16_t ms);

e_lwip_drv_return_t R_LWIP_DRIVER_Close (uint32_t cmt_ch);

e_lwip_drv_return_t R_LWIP_DRIVER_EthernetClose (struct netif *netif);

void R_LWIP_DRIVER_EthernetLinkCheck (struct netif *netif);

err_t r_lwip_driver_ethernetif_init (struct netif *netif);

void R_LWIP_DRIVER_Input (struct netif *netif);

err_t r_lwip_driver_low_level_output (struct netif *netif, struct pbuf *p);

uint32_t r_lwip_driver_get_rand (void);

#endif /* R_LWIP_DRIVER_RX_IF_H */
