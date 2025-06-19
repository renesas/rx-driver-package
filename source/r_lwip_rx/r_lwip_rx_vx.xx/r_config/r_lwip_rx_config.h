/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_lwip_rx_config.h
 * Description  : Configures the lwIP.
 *********************************************************************************************************************/

#ifndef R_LWIP_RX_CONFIG_H
#define R_LWIP_RX_CONFIG_H

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define NO_SYS                          (1)

/**
 * SYS_LIGHTWEIGHT_PROT==0: disable inter-task protection (and task-vs-interrupt
 * protection) for certain critical regions during buffer allocation, deallocation
 * and memory allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT            (0)

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT                   (4)

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                        (20*1024)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF                   (50)
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB                (6)
/* MEMP_NUM_TCP_PCB: the number of simultaneously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB                (10)
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN         (5)
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG                (16)
/* MEMP_NUM_SYS_TIMEOUT: the number of simultaneously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT            (LWIP_NUM_SYS_TIMEOUT_INTERNAL)


/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE                  (16)

/* ---------- ARP options ---------- */
#define LWIP_ARP                        (1)

/* ---------- IPv4 options ---------- */
#define LWIP_IPV4                       (1)

/* ---------- ICMP options ---------- */
#define LWIP_ICMP                       (1)

/* ---------- DHCP options ---------- */
#define LWIP_DHCP                       (0)

/* ---------- IGMP options ---------- */
#define LWIP_IGMP                       (1)

/* ---------- UDP options ---------- */
#define LWIP_UDP                        (1)
#define UDP_TTL                         (IP_DEFAULT_TTL)

/* ---------- TCP options ---------- */
#define LWIP_TCP                        (1)
#define TCP_TTL                         (IP_DEFAULT_TTL)

/* TCP receive window. */
#define TCP_WND                         (4 * TCP_MSS)

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ                 (0)

/* TCP Maximum segment size. */
#define TCP_MSS                         (1500 - 40) /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF                     (4 * TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */

#define TCP_SND_QUEUELEN                (2 * TCP_SND_BUF/TCP_MSS)

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE               (LWIP_MEM_ALIGN_SIZE(TCP_MSS+PBUF_IP_HLEN+PBUF_TRANSPORT_HLEN+PBUF_LINK_ENCAPSULATION_HLEN+PBUF_LINK_HLEN))

/* ---------- link callback options ---------- */
/* LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
#define LWIP_NETIF_LINK_CALLBACK        (0)

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    (0)

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     (0)

/* ---------- Statistics options ---------- */
#define LWIP_STATS                      (0)


#endif /* R_LWIP_RX_CONFIG_H */
