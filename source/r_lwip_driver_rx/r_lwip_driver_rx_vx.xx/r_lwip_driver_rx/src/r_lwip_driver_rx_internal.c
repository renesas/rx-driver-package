/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lwip_driver_rx_internal.c
 * Description  : Software used as a porting layer for lwip.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "r_cmt_rx_if.h"
#include "r_bsp/platform.h"
#include "src/r_ether_rx_private.h"
#include "r_ether_rx_if.h"
#include "r_ether_rx_pinset.h"

#include "lwip/init.h"
#include "lwip/netif.h"

#include "lwip/opt.h"
#include "lwip/memp.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"
#include "r_lwip_driver_rx_config.h"
#include "r_lwip_driver_rx_if.h"
#include "r_lwip_driver_rx_private.h"
#include "arch/sys_arch.h"


/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
uint8_t g_link_change[ETHER_CHANNEL_MAX] = {0};
uint8_t g_link_status[ETHER_CHANNEL_MAX] = {0};

extern uint32_t g_timer_count;
extern uint16_t g_timer_set_ms;

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
void r_lwip_driver_rx_ether_cb (void *p_arg);
void r_lwip_driver_rx_ether_int_cb (void *p_arg);

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_timer_callback
 *********************************************************************************************************************/
void r_lwip_driver_timer_callback(void * pdata)
{
    /* call this function per ms */
    g_timer_count += g_timer_set_ms;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_timer_callback
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_get_linkstatus
 *********************************************************************************************************************/
uint32_t r_lwip_driver_get_linkstatus(uint32_t eth_ch)
{
    LWIP_ASSERT("The channel number is wrong. " , eth_ch < ETHER_CHANNEL_MAX);

    return g_link_status[eth_ch];
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_get_linkstatus
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_get_linkchange
 *********************************************************************************************************************/
uint32_t r_lwip_driver_get_linkchange(uint32_t eth_ch)
{
    uint32_t linkchange = LWIP_DRIVER_LINK_CHANGE_FLAG_OFF;

#if ETHER_CFG_USE_LINKSTA == (1)
    uint32_t save_iflag;

    /* Secure the critical section from checking g_link_change clear to clear it */
    /* save the I FLAG for resuming it */
    save_iflag = sys_arch_protect();
#endif /* ETHER_CFG_USE_LINKSTA == (1) */

    /* Please check the netif->name[1] setting value in r_lwip_driver_ethernetif_init func if asserted here */
    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    if(eth_ch < ETHER_CHANNEL_MAX)
    {
        if(LWIP_DRIVER_LINK_CHANGE_FLAG_ON == g_link_change[eth_ch])
        {
            g_link_change[eth_ch] = LWIP_DRIVER_LINK_CHANGE_FLAG_OFF;
            linkchange            = LWIP_DRIVER_LINK_CHANGE_FLAG_ON;
        }
        else
        {
            linkchange            = LWIP_DRIVER_LINK_CHANGE_FLAG_OFF;
        }
    }

#if ETHER_CFG_USE_LINKSTA == (1)
    /* exit Critical section */
    sys_arch_unprotect(save_iflag);
#endif /* ETHER_CFG_USE_LINKSTA == (1) */

    return linkchange;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_get_linkchange
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_low_level_init
 *********************************************************************************************************************/
err_t r_lwip_driver_low_level_init(struct netif *netif)
{
    ether_param_t  eth_param  = { 0 };
    ether_return_t ether_ret;
    uint32_t       eth_ch     = LWIP_DRIVER_CFG_ETH_DRV_CH;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    /* Clear the link status flags */
    g_link_change[eth_ch] =  LWIP_DRIVER_LINK_CHANGE_FLAG_OFF;
    g_link_status[eth_ch] =  LWIP_DRIVER_LINK_STATUS_FLAG_OFF;

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = LWIP_DRIVER_CFG_ETH_MAC_ADDR0;
    netif->hwaddr[1] = LWIP_DRIVER_CFG_ETH_MAC_ADDR1;
    netif->hwaddr[2] = LWIP_DRIVER_CFG_ETH_MAC_ADDR2;
    netif->hwaddr[3] = LWIP_DRIVER_CFG_ETH_MAC_ADDR3;
    netif->hwaddr[4] = LWIP_DRIVER_CFG_ETH_MAC_ADDR4;
    netif->hwaddr[5] = LWIP_DRIVER_CFG_ETH_MAC_ADDR5;

    /* maximum transfer unit */
    netif->mtu = LWIP_DRIVER_CFG_MTU;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    eth_param.ether_callback.pcb_func = r_lwip_driver_rx_ether_cb;

    ether_ret = R_ETHER_Control(CONTROL_SET_CALLBACK, eth_param);

    if (ETHER_SUCCESS != ether_ret)
    {
        return ERR_IF;
    }

    /* Set callback function called by EINT status interrupts. */
    eth_param.ether_int_hnd.pcb_int_hnd = r_lwip_driver_rx_ether_int_cb;

    ether_ret = R_ETHER_Control(CONTROL_SET_INT_HANDLER, eth_param);

    if (ETHER_SUCCESS != ether_ret)
    {
        return ERR_IF;
    }

    eth_param.channel = eth_ch;

    R_ETHER_Control(CONTROL_POWER_ON, eth_param);

#if (LWIP_DRIVER_CFG_ETH_DRV_CH == 0)
#if (ETHER_CFG_MODE_SEL == 0)
    R_ETHER_PinSet_ETHERC0_MII();
#if (ETHER_CFG_CH0_PHY_ACCESS == 1)
    R_ETHER_PinSet_ETHERC1_MII();
#endif /* ETHER_CFG_CH0_PHY_ACCESS == 1 */
#elif (ETHER_CFG_MODE_SEL == 1)
    R_ETHER_PinSet_ETHERC0_RMII();
#if (ETHER_CFG_CH0_PHY_ACCESS == 1)
    R_ETHER_PinSet_ETHERC1_RMII();
#endif /* ETHER_CFG_CH0_PHY_ACCESS == 1 */
#endif /* ETHER_CFG_MODE_SEL == 0 */

#elif (LWIP_DRIVER_CFG_ETH_DRV_CH == 1)
#if (ETHER_CFG_MODE_SEL == 0)
    R_ETHER_PinSet_ETHERC1_MII();
#if (ETHER_CFG_CH1_PHY_ACCESS == 0)
    R_ETHER_PinSet_ETHERC0_MII();
#endif
#elif (ETHER_CFG_MODE_SEL == 1)
    R_ETHER_PinSet_ETHERC1_RMII();
#if (ETHER_CFG_CH1_PHY_ACCESS == 0)
    R_ETHER_PinSet_ETHERC0_RMII();
#endif /* ETHER_CFG_CH1_PHY_ACCESS == 0 */
#endif /* ETHER_CFG_MODE_SEL == 0 */

#endif /* LWIP_DRIVER_CFG_ETH_DRV_CH == 0 */

    /* Open the Ethernet channel.
     Note that the API function R_ETHER_Initial must be called in advance. */
    ether_ret = R_ETHER_Open_ZC2(eth_ch, netif->hwaddr, ETHER_FLAG_OFF);

    if (ETHER_SUCCESS != ether_ret)
    {
        return ERR_IF;
    }

    return ERR_OK;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_low_level_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_low_level_input
 *********************************************************************************************************************/
struct pbuf* r_lwip_driver_low_level_input(struct netif *netif)
{
    uint32_t    eth_ch;
    void        *p_buf = NULL;
    int32_t     len;
    struct pbuf *p     = NULL;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    eth_ch = (uint32_t)(netif->name[1] - '0');

    /* Please check the netif->name[1] setting value in r_lwip_driver_ethernetif_init func if asserted here */
    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    if (ETHER_CHANNEL_MAX <= eth_ch)
    {
        return p;
    }

    /* Get the received frame from the Ethernet driver. */
    len = R_ETHER_Read_ZC2(eth_ch, &p_buf);

    if (len <= 0)
    {
        /* Unexpected error, return. */
        return p;
    }

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (NULL != p)
    {
#if ETH_PAD_SIZE
        pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

        MEMCPY(p->payload, p_buf, len);
        p->len = len;

#if ETH_PAD_SIZE
        pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        R_ETHER_Read_ZC2_BufRelease(eth_ch);
    }

    return p;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_low_level_input
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_rx_ether_cb
 *********************************************************************************************************************/
void r_lwip_driver_rx_ether_cb(void *p_arg)
{
    ether_cb_arg_t *p_cb_arg = (ether_cb_arg_t *)p_arg;
    uint32_t       eth_ch    = p_cb_arg->channel;

    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    switch (p_cb_arg->event_id)
    {
        /* Callback function that notifies user to have detected magic packet. */
        case ETHER_CB_EVENT_ID_WAKEON_LAN:
            /* Not supported. */
            break;

        /* Callback function that notifies user to have become Link up or down. */
        case ETHER_CB_EVENT_ID_LINK_ON:
            g_link_status[eth_ch] = LWIP_DRIVER_LINK_STATUS_FLAG_ON;
#if (ETHER_CFG_USE_LINKSTA == 0)
            g_link_change[eth_ch] = LWIP_DRIVER_LINK_CHANGE_FLAG_ON;
#endif
            break;
        case ETHER_CB_EVENT_ID_LINK_OFF:
            g_link_status[eth_ch] = LWIP_DRIVER_LINK_STATUS_FLAG_OFF;
#if (ETHER_CFG_USE_LINKSTA == 0)
            g_link_change[eth_ch] = LWIP_DRIVER_LINK_CHANGE_FLAG_ON;
#endif
            break;
        default:
            break;
    }
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_rx_ether_cb
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_rx_ether_int_cb
 *********************************************************************************************************************/
void r_lwip_driver_rx_ether_int_cb(void *p_arg)
{
    ether_cb_arg_t *p_cb_arg = (ether_cb_arg_t *)p_arg;
    uint32_t       eth_ch    = p_cb_arg->channel;

    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    if((p_cb_arg->status_ecsr & (1u << 2)) != 0u)
    {
        /* Link change detected, signal Ethernet processing thread. */
        g_link_change[eth_ch] = LWIP_DRIVER_LINK_CHANGE_FLAG_ON;
    }
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_rx_ether_int_cb
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_Xorshift
 *********************************************************************************************************************/
#if BSP_CFG_MCU_PART_ENCRYPTION_INCLUDED != true &&  BSP_CFG_MCU_PART_FUNCTION != (0x11)
uint32_t r_lwip_driver_Xorshift(uint32_t seed)
{
    uint32_t y = seed;
    y = y ^ (y << 13);
    y = y ^ (y >> 17);
    y = y ^ (y <<  5);

    return y;
}
#endif /* BSP_CFG_MCU_PART_ENCRYPTION_INCLUDED != true &&  BSP_CFG_MCU_PART_FUNCTION != (0x11) */
/**********************************************************************************************************************
 * End of function r_lwip_driver_Xorshift
 *********************************************************************************************************************/
