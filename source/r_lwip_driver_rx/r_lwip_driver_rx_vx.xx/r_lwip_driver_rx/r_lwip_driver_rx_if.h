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
#include "r_bsp/platform.h"
#include "r_lwip_driver_rx_config.h"
#include "lwip/err.h"
#include "lwip/netif.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#if ((defined BSP_MCU_RX65N) && (BSP_CFG_MCU_PART_ENCRYPTION_INCLUDED == true)) ||\
    ((defined BSP_MCU_RX72M || defined BSP_MCU_RX72N || defined BSP_MCU_RX66N) && (BSP_CFG_MCU_PART_FUNCTION == 0x11))
#define R_LWIP_DRIVER_USE_TSIP      (1)
#else
#define R_LWIP_DRIVER_USE_TSIP      (0)
#endif

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    LWIP_DRV_SUCCESS        =  0,   /* Successful completion */
    LWIP_DRV_ERR_OPEN       = -1,   /* The R_LWIP_DRIVER_Open() function has already been executed. */
    LWIP_DRV_ERR_NOT_OPEN   = -2,   /* The R_LWIP_DRIVER_Open() function has not been executed. */
    LWIP_DRV_ERR_ARG        = -3,   /* An argument is incorrect. */
    LWIP_DRV_ERR_CMT        = -4,   /* An error has occurred in the CMT FIT module. */
    LWIP_DRV_ERR_RTOS       = -5    /* An error has occurred in RTOS. */
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

void r_lwip_driver_input_thread (void * arg);

#endif /* R_LWIP_DRIVER_RX_IF_H */
