/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lwip_driver_rx_config.h
 * Description  : Configures the lwIP driver.
 *********************************************************************************************************************/

#ifndef R_LWIP_DRIVER_RX_CONFIG_H
#define R_LWIP_DRIVER_RX_CONFIG_H

#define LWIP_DRIVER_CFG_ETH_MAC_ADDR0       (0x74)
#define LWIP_DRIVER_CFG_ETH_MAC_ADDR1       (0x90)
#define LWIP_DRIVER_CFG_ETH_MAC_ADDR2       (0x50)
#define LWIP_DRIVER_CFG_ETH_MAC_ADDR3       (0x10)
#define LWIP_DRIVER_CFG_ETH_MAC_ADDR4       (0xFE)
#define LWIP_DRIVER_CFG_ETH_MAC_ADDR5       (0x79)

#define LWIP_DRIVER_CFG_ETH_DRV_CH          (0)

#define LWIP_DRIVER_CFG_MTU                 (1500U)

/* Retry max count in the low_level_output()  */
#define LWIP_DRIVER_CFG_SEND_MAX_LOOP       (20)

/* Delay time when the send buffer full in the low_level_output(): usec */
#define LWIP_DRIVER_CFG_SEND_DELAY_US       (50)

#endif /* R_LWIP_DRIVER_RX_CONFIG_H */
