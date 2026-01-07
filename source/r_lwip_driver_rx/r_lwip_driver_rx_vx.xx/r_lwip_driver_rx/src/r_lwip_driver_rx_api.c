/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lwip_driver_rx_api.c
 * Description  : Software used as a porting layer for lwip.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "r_bsp/platform.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/sys.h"

#include "lwip/opt.h"
#include "lwip/memp.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/etharp.h"
#if LWIP_IPV6
#include "lwip/ethip6.h"
#endif /* LWIP_IPV6 */
#include "netif/ethernet.h"
#include "arch/sys_arch.h"

#if NO_SYS
#include "r_cmt_rx_if.h"
#endif
#include "src/r_ether_rx_private.h"
#include "r_ether_rx_pinset.h"
#include "r_lwip_driver_rx_private.h"
#if R_LWIP_DRIVER_USE_TSIP
#include "r_tsip_rx_if.h"
#endif /* R_LWIP_DRIVER_USE_TSIP */

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/
BSP_CFG_USER_LOCKING_TYPE g_lwip_driver_open = {false};

#if NO_SYS
uint32_t g_timer_count  = 0;
uint16_t g_timer_set_ms = 0;
#endif

#if !NO_SYS
sys_mutex_t g_lwip_driver_mutex = {0};

sys_mbox_t g_lwip_driver_read_complete_mbox[ETHER_CHANNEL_MAX] = {NULL};
#endif

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   sys_now
 *********************************************************************************************************************/
#if NO_SYS
u32_t sys_now(void)
{
    return g_timer_count;
}
#endif /* NO_SYS */
/**********************************************************************************************************************
 * End of function sys_now
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   R_LWIP_DRIVER_Open
 *********************************************************************************************************************/
e_lwip_drv_return_t R_LWIP_DRIVER_Open(uint32_t *p_cmt_ch, uint16_t ms)
{
    bool  res;
#if !NO_SYS
    err_t err = ERR_OK;
#endif /* !NO_SYS */

    res = R_BSP_SoftwareLock(&g_lwip_driver_open);

    if (false == res)
    {
        return LWIP_DRV_ERR_OPEN;
    }

#if NO_SYS
    if (NULL == p_cmt_ch)
    {
        R_BSP_SoftwareUnlock(&g_lwip_driver_open);
        return LWIP_DRV_ERR_ARG;
    }

    if ((LWIP_DRIVER_TIMER_SET_MIN > ms) || (LWIP_DRIVER_TIMER_SET_MAX < ms))
    {
        R_BSP_SoftwareUnlock(&g_lwip_driver_open);
        return LWIP_DRV_ERR_ARG;
    }

    /* Clear the counter for lwip */
    g_timer_count  = 0;
    g_timer_set_ms = ms;

    /* Create periodic timer */
    res = R_CMT_CreatePeriodic((1000/ms), r_lwip_driver_timer_callback, p_cmt_ch);

    if (true != res)
    {
        R_BSP_SoftwareUnlock(&g_lwip_driver_open);
        return LWIP_DRV_ERR_CMT;
    }
#else /* NO_SYS */
    LWIP_UNUSED_ARG(p_cmt_ch);
    LWIP_UNUSED_ARG(ms);

    err = sys_mutex_new(&g_lwip_driver_mutex);

    if (ERR_OK != err)
    {
        R_BSP_SoftwareUnlock(&g_lwip_driver_open);
        return LWIP_DRV_ERR_RTOS;
    }
#endif /* NO_SYS */

    /* Setup Ethernet hardware. */
    R_ETHER_Initial();

#if !R_LWIP_DRIVER_USE_TSIP
    uint32_t mac_xorshift_val = 0;
    uint32_t input_num        = 0;

    input_num =  (uint32_t)LWIP_DRIVER_CFG_ETH_MAC_ADDR5        |\
                ((uint32_t)LWIP_DRIVER_CFG_ETH_MAC_ADDR4 << 8 ) |\
                ((uint32_t)LWIP_DRIVER_CFG_ETH_MAC_ADDR3 << 16) |\
                ((uint32_t)LWIP_DRIVER_CFG_ETH_MAC_ADDR2 << 24);

    /* Create the seed value from MAC Address using the Xorshitf Algorithm */
    mac_xorshift_val = r_lwip_driver_Xorshift(input_num);

    /* Set initial seed value */
    srand(mac_xorshift_val);

#endif /* !R_LWIP_DRIVER_USE_TSIP */

    return LWIP_DRV_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_LWIP_DRIVER_Open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   R_LWIP_DRIVER_Close
 *********************************************************************************************************************/
e_lwip_drv_return_t R_LWIP_DRIVER_Close(uint32_t cmt_ch)
{
#if NO_SYS
    bool res;
#endif /* NO_SYS */

    if (true != g_lwip_driver_open.lock)
    {
        return LWIP_DRV_ERR_NOT_OPEN;
    }

#if NO_SYS
    res = R_CMT_Stop(cmt_ch);

    if (true != res)
    {
        return LWIP_DRV_ERR_CMT;
    }
#else /* NO_SYS */
    LWIP_UNUSED_ARG(cmt_ch);

    sys_mutex_free(&g_lwip_driver_mutex);
#endif /* NO_SYS */

    R_BSP_SoftwareUnlock(&g_lwip_driver_open);

    return LWIP_DRV_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_LWIP_DRIVER_Close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   R_LWIP_DRIVER_EthernetClose
 *********************************************************************************************************************/
e_lwip_drv_return_t R_LWIP_DRIVER_EthernetClose(struct netif *netif)
{
    ether_return_t ether_ret;
    uint32_t       eth_ch;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    eth_ch = (uint32_t)(netif->name[1] - '0');

    /* Please check the netif->name[1] setting value in r_lwip_driver_ethernetif_init func if asserted here */
    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);
    if (ETHER_CHANNEL_MAX <= eth_ch)
    {
        return LWIP_DRV_ERR_ARG;
    }

    /* Wait for sending the data */
    ether_ret = R_ETHER_CheckWrite(eth_ch);

    if (ETHER_SUCCESS != ether_ret)
    {
        return LWIP_DRV_ERR_ARG;
    }

    /* Close the Ethernet I/F */
    ether_ret = R_ETHER_Close_ZC2(eth_ch);

    if (ETHER_SUCCESS != ether_ret)
    {
        return LWIP_DRV_ERR_ARG;
    }

    return LWIP_DRV_SUCCESS;

}
/**********************************************************************************************************************
 * End of function R_LWIP_DRIVER_Ether_Close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   R_LWIP_DRIVER_EthernetLinkCheck
 *********************************************************************************************************************/
void R_LWIP_DRIVER_EthernetLinkCheck(struct netif *netif)
{
    uint32_t eth_ch;
    uint32_t linkstatus;
    uint8_t  link_up;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    eth_ch = (uint32_t)(netif->name[1] - '0');

    /* Please check the netif->name[1] setting value in r_lwip_driver_ethernetif_init func if asserted here */
    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    if (eth_ch < ETHER_CHANNEL_MAX)
    {
#if (ETHER_CFG_USE_LINKSTA == 1)
        if (LWIP_DRIVER_LINK_CHANGE_FLAG_ON == r_lwip_driver_get_linkchange(eth_ch))
        {
#endif /* ETHER_CFG_USE_LINKSTA == 1 */
#if !NO_SYS
            sys_mutex_lock(&g_lwip_driver_mutex);
#endif /* !NO_SYS */

            R_ETHER_LinkProcess(eth_ch);

#if !NO_SYS
            sys_mutex_unlock(&g_lwip_driver_mutex);
#endif /* !NO_SYS */
#if (ETHER_CFG_USE_LINKSTA == 1)
        }
#endif /* ETHER_CFG_USE_LINKSTA == 1 */

#if (ETHER_CFG_USE_LINKSTA == 0)
        if (LWIP_DRIVER_LINK_CHANGE_FLAG_ON == r_lwip_driver_get_linkchange(eth_ch))
        {
#endif /* ETHER_CFG_USE_LINKSTA == 0 */
            linkstatus = r_lwip_driver_get_linkstatus(eth_ch);
            link_up    = netif_is_link_up(netif);

            if ((LWIP_DRIVER_LINK_STATUS_FLAG_OFF == linkstatus) && (LWIP_DRIVER_LINK_STATUS_FLAG_ON == link_up))
            {
                netif_set_link_down(netif);
            }
            else if ((LWIP_DRIVER_LINK_STATUS_FLAG_ON == linkstatus) && (LWIP_DRIVER_LINK_STATUS_FLAG_OFF == link_up))
            {
                netif_set_link_up(netif);
            }
#if (ETHER_CFG_USE_LINKSTA == 0)
        }
#endif /* ETHER_CFG_USE_LINKSTA == 0 */
    }
}
/**********************************************************************************************************************
 * End of function R_LWIP_DRIVER_EthernetLinkCheck
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_ethernetif_init
 *********************************************************************************************************************/
err_t r_lwip_driver_ethernetif_init(struct netif *netif)
{
    err_t err = ERR_OK;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip_rx";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'c';
    netif->name[1] = (char) (LWIP_DRIVER_CFG_ETH_DRV_CH + '0');
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
#if LWIP_IPV4
    netif->output     = etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput = r_lwip_driver_low_level_output;

    /* initialize the hardware */
    err = r_lwip_driver_low_level_init(netif);

    return err;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_ethernetif_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   R_LWIP_DRIVER_Input
 *********************************************************************************************************************/
void R_LWIP_DRIVER_Input(struct netif *netif)
{
    struct pbuf *p  = NULL;
    err_t       err = ERR_OK;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    do
    {
#if !NO_SYS
        sys_mutex_lock(&g_lwip_driver_mutex);
#endif /* !NO_SYS */

        p = r_lwip_driver_low_level_input(netif);

#if !NO_SYS
        sys_mutex_unlock(&g_lwip_driver_mutex);
#endif /* !NO_SYS */

        if (NULL != p)
        {
            err = netif->input(p, netif);

            if (ERR_OK != err)
            {
                pbuf_free(p);
            }
        }
    }
    while (p != NULL); /* WAIT_LOOP */
}
/**********************************************************************************************************************
 * End of function R_LWIP_DRIVER_Input
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_low_level_output
 *********************************************************************************************************************/
err_t r_lwip_driver_low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t       err         = ERR_OK;
    struct pbuf *q          = NULL;
    uint8_t     *p_buf      = NULL;
    uint8_t     *p_dst      = NULL;
    uint32_t    total_len   = 0;

    uint32_t eth_ch;
    uint16_t buf_size;

    ether_return_t ether_ret;

    LWIP_ASSERT("netif != NULL", (netif != NULL));
    LWIP_ASSERT("p != NULL", (p != NULL));

    eth_ch = (uint32_t)(netif->name[1] - '0');

    /* Please check the netif->name[1] setting value in r_lwip_driver_ethernetif_init func if asserted here */
    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    if (eth_ch < ETHER_CHANNEL_MAX)
    {
#if ETH_PAD_SIZE
        pbuf_remove_header(p, ETH_PAD_SIZE); /* drop the padding word */
#endif

        uint32_t loop_cnt = 0;

        /* Send the data from the pbuf to the interface, one pbuf at a
         time. The size of the data in each pbuf is kept in the ->len
         variable. */

#if !NO_SYS
        sys_mutex_lock(&g_lwip_driver_mutex);
#endif /* !NO_SYS */

        do
        {
            /* Fetch a hardware buffer. */
            ether_ret = R_ETHER_Write_ZC2_GetBuf(eth_ch, (void *)&p_buf, &buf_size);
            loop_cnt++;

            if (ETHER_ERR_TACT == ether_ret)
            {
                /* Wait for send buffer empty */
#if NO_SYS
                R_BSP_SoftwareDelay(LWIP_DRIVER_CFG_SEND_DELAY_US, BSP_DELAY_MICROSECS);
#endif
            }
        }
        while ((ETHER_ERR_TACT == ether_ret ) && ( LWIP_DRIVER_CFG_SEND_MAX_LOOP > loop_cnt )); /* WAIT_LOOP */

        if (ETHER_SUCCESS != ether_ret)
        {
            /* An error occurred in R_ETHER_Write_ZC2_GetBuf. */
            err = ERR_IF;
        }
        else if (ETHER_BUFSIZE_MIN > buf_size)
        {
            err = ERR_BUF;
        }
        else if (p->tot_len > buf_size)
        {
            /* The total length of the pbuf exceeds the driver's buffer size. */
            err = ERR_BUF;
        }

        if (ERR_OK == err)
        {
            /* Set the write pointer to the beginning of the buffer. */
            p_dst = p_buf;

            /* WAIT_LOOP */
            for (q = p; q != NULL; q = q->next)
            {
                /* Copy data from the pbuf to the destination buffer. */
                MEMCPY(p_dst, q->payload, q->len);

                p_dst += q->len;
            }

            if (p->tot_len < ETHER_BUFSIZE_MIN)
            {
                memset(p_buf + p->tot_len, 0, ETHER_BUFSIZE_MIN - p->tot_len);
                total_len = ETHER_BUFSIZE_MIN;
            }
            else
            {
                total_len = p->tot_len;
            }

            ether_ret = R_ETHER_Write_ZC2_SetBuf(eth_ch, total_len);

            if (ETHER_SUCCESS != ether_ret)
            {
                err = ERR_IF;
            }
        }

#if !NO_SYS
        sys_mutex_unlock(&g_lwip_driver_mutex);
#endif /* !NO_SYS */

#if ETH_PAD_SIZE
        pbuf_add_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    }
    else
    {
        err = ERR_ARG;
    }

    return err;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_low_level_output
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_get_rand
 *********************************************************************************************************************/
uint32_t r_lwip_driver_get_rand(void)
{
    uint32_t ret_val = 0;

#if R_LWIP_DRIVER_USE_TSIP
    uint32_t random_num[4] = {0,0,0,0};

    R_TSIP_GenerateRandomNumber(random_num);

    ret_val = random_num[0];
#else
    /* Generate random number */
    ret_val = (uint32_t)rand();
#endif /* R_LWIP_DRIVER_USE_TSIP */

    return ret_val;
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_get_rand
 *********************************************************************************************************************/

#if !NO_SYS
/**********************************************************************************************************************
 * Function Name  @fn   r_lwip_driver_input_thread
 *********************************************************************************************************************/
void r_lwip_driver_input_thread(void * arg)
{
    struct netif * netif = (struct netif *)arg;
    uint32_t     * p_read_complete_flag = NULL;
    uint32_t       eth_ch = (uint32_t)(netif->name[1] - '0');

    /* Please check the netif->name[1] setting value in r_lwip_driver_ethernetif_init func if asserted here */
    LWIP_ASSERT("The channel number is wrong. ", eth_ch < ETHER_CHANNEL_MAX);

    sys_mbox_new(&g_lwip_driver_read_complete_mbox[eth_ch], ETHER_CFG_EMAC_RX_DESCRIPTORS);

    /* WAIT_LOOP */
    while (true)
    {
        sys_mbox_fetch(&g_lwip_driver_read_complete_mbox[eth_ch], (void **) &p_read_complete_flag);
        R_LWIP_DRIVER_Input(netif);
    }
}
/**********************************************************************************************************************
 * End of function r_lwip_driver_input_thread
 *********************************************************************************************************************/
#endif

