/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011-2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name     : config_tcpudp.c
* Version       : 2.09
* Device(s)     : Renesas microcomputer
* Tool-Chain    : Renesas compilers
* OS            :
* H/W Platform  :
* Description   : T4 configuration file
* Limitations   :
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 30.08.2011 1.00    First release.
*               : 30.11.2016 1.01    DHCP relation add.
*               : 12.12.2017 2.07    Corresponding to Smart Configurator.
*               : 20.06.2019 2.09    Added TCP Keep-Alive function.
*               :                    Changed specification T_TCP_CCEP, T_UDP_CCEP.
******************************************************************************/

#include "r_t4_itcpip.h"
#include "r_t4_rx_config.h"

#if (T4_CFG_SYSTEM_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_SYSTEM_CALLBACK_FUNCTION_NAME(UB channel, UW eventid, VP param);
#endif
#if  (T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif

#if  (T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif
#if  (T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_USE != (0))
extern ER T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_NAME(ID cepid, FN fncd, VP p_parblk);
#endif

/****************************************************************************/
/**********************     TCP-related definition     **********************/
/****************************************************************************/
/* Number of LAN port, Number of Serial port */
const UB _t4_channel_num = T4_CFG_SYSTEM_CHANNEL_NUMBER;
const UB _t4_dhcp_enable = T4_CFG_SYSTEM_DHCP;
const UH _t4_dhcp_ip_reply_arp_delay = 300;       /* unit:10ms */

/***  Definition of TCP reception point (only port number needs to be set) ***/
const T_TCP_CREP tcp_crep[] =
{
    /* { attribute of reception point, {local IP address, local port number}} */
    { 0, { 0, T4_CFG_TCP_REPID1_PORT_NUMBER }},
    { 0, { 0, T4_CFG_TCP_REPID2_PORT_NUMBER }},
    { 0, { 0, T4_CFG_TCP_REPID3_PORT_NUMBER }},
    { 0, { 0, T4_CFG_TCP_REPID4_PORT_NUMBER }},
};

/* Total number of TCP reception points */
const H __tcprepn = sizeof(tcp_crep) / sizeof(T_TCP_CREP);

/***  Definition of TCP communication end point
      (only receive window size needs to be set) ***/
const T_TCP_CCEP tcp_ccep[] =
{
    /* { attribute of TCP communication end point,
         top address of transmit window buffer, size of transmit window buffer,
         top address of receive window buffer, size of receive window buffer,
         address of callback routine }
    */
    {
        0, 0, 0, 0, T4_CFG_TCP_CEPID1_RECEIVE_WINDOW_SIZE, T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_NAME,
        T4_CFG_TCP_CEPID1_CHANNEL, T4_CFG_TCP_CEPID1_KEEPALIVE_ENABLE
    },
    {
        0, 0, 0, 0, T4_CFG_TCP_CEPID2_RECEIVE_WINDOW_SIZE, T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_NAME,
        T4_CFG_TCP_CEPID2_CHANNEL, T4_CFG_TCP_CEPID2_KEEPALIVE_ENABLE
    },
    {
        0, 0, 0, 0, T4_CFG_TCP_CEPID3_RECEIVE_WINDOW_SIZE, T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_NAME,
        T4_CFG_TCP_CEPID3_CHANNEL, T4_CFG_TCP_CEPID3_KEEPALIVE_ENABLE
    },
    {
        0, 0, 0, 0, T4_CFG_TCP_CEPID4_RECEIVE_WINDOW_SIZE, T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_NAME,
        T4_CFG_TCP_CEPID4_CHANNEL, T4_CFG_TCP_CEPID4_KEEPALIVE_ENABLE
    },
    {
        0, 0, 0, 0, T4_CFG_TCP_CEPID5_RECEIVE_WINDOW_SIZE, T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_NAME,
        T4_CFG_TCP_CEPID5_CHANNEL, T4_CFG_TCP_CEPID5_KEEPALIVE_ENABLE
    },
    {
        0, 0, 0, 0, T4_CFG_TCP_CEPID6_RECEIVE_WINDOW_SIZE, T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_NAME,
        T4_CFG_TCP_CEPID6_CHANNEL, T4_CFG_TCP_CEPID6_KEEPALIVE_ENABLE
    },
};

/* Total number of TCP communication end points */
const H __tcpcepn = sizeof(tcp_ccep) / sizeof(T_TCP_CCEP);

/***  TCP MSS  ***/
const UH _tcp_mss[] =
{
    /* MAX:1460 bytes */
    T4_CFG_TCP_MSS,
    T4_CFG_TCP_MSS,
};

/***  2MSL wait time (unit:10ms)  ***/
const UH    _tcp_2msl[] =
{
    (T4_CFG_TCP_2MSL_TIME * (1000 / 10)),      /* 1 min */
    (T4_CFG_TCP_2MSL_TIME * (1000 / 10)),      /* 1 min */
};
/***  Maximum value of retransmission timeout period (unit:10ms)  ***/
const UH    _tcp_rt_tmo_rst[] =
{
    (T4_CFG_TCP_MAX_TIMEOUT_PERIOD * (1000 / 10)),     /* 10 min */
    (T4_CFG_TCP_MAX_TIMEOUT_PERIOD * (1000 / 10)),     /* 10 min */
};

/***  Transmit for delay ack (ON=1/OFF=0) ***/
const UB   _tcp_dack[] =
{
    T4_CFG_TCP_DIVIDE_SENDING_PACKET,
    T4_CFG_TCP_DIVIDE_SENDING_PACKET
};

/***  Time to first transmit Keepalive packet (unit:10ms)  ***/
const UW   _tcp_keepalive_start[] =
{
    (T4_CFG_TCP_KEEPALIVE_START * (1000 / 10)),
    (T4_CFG_TCP_KEEPALIVE_START * (1000 / 10)),
};

/***  Second Keepalive packets transmission interval (unit:10ms)  ***/
const UW   _tcp_keepalive_interval[] =
{
    (T4_CFG_TCP_KEEPALIVE_INTERVAL * (1000 / 10)),
    (T4_CFG_TCP_KEEPALIVE_INTERVAL * (1000 / 10)),
};

/***  Keepalive packet transmission count  ***/
const UW   _tcp_keepalive_count[] =
{
    T4_CFG_TCP_KEEPALIVE_COUNT,
    T4_CFG_TCP_KEEPALIVE_COUNT,
};

/****************************************************************************/
/**********************     UDP-related definition     **********************/
/****************************************************************************/
/***  Definition of UDP communication end point  ***/
const T_UDP_CCEP udp_ccep[] =
{
    { 0, { 0, T4_CFG_UDP_CEPID1_PORT_NUMBER }, T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_NAME, T4_CFG_UDP_CEPID1_CHANNEL },
    { 0, { 0, T4_CFG_UDP_CEPID2_PORT_NUMBER }, T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_NAME, T4_CFG_UDP_CEPID2_CHANNEL },
    { 0, { 0, T4_CFG_UDP_CEPID3_PORT_NUMBER }, T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_NAME, T4_CFG_UDP_CEPID3_CHANNEL },
    { 0, { 0, T4_CFG_UDP_CEPID4_PORT_NUMBER }, T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_NAME, T4_CFG_UDP_CEPID4_CHANNEL },
    { 0, { 0, T4_CFG_UDP_CEPID5_PORT_NUMBER }, T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_NAME, T4_CFG_UDP_CEPID5_CHANNEL },
    { 0, { 0, T4_CFG_UDP_CEPID6_PORT_NUMBER }, T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_NAME, T4_CFG_UDP_CEPID6_CHANNEL },
};
/* Total number of UDP communication end points */
const H __udpcepn = (sizeof(udp_ccep) / sizeof(T_UDP_CCEP));

/***  TTL for multicast transmission  ***/
const UB __multi_TTL[] = { T4_CFG_UDP_MULTICAST_TTL, T4_CFG_UDP_MULTICAST_TTL };

/*** Behavior of UDP zero checksum ***/
const UB _udp_enable_zerochecksum[] =
{
    T4_CFG_UDP_BEHAVIOR_OF_RECEIVED_ZERO_CHECKSUM,
    T4_CFG_UDP_BEHAVIOR_OF_RECEIVED_ZERO_CHECKSUM
}; /* 0 = disable, other = enable */

/****************************************************************************/
/**********************  SYSTEM-callback definition   ***********************/
/****************************************************************************/
const callback_from_system_t g_fp_user = T4_CFG_SYSTEM_CALLBACK_FUNCTION_NAME;

/****************************************************************************/
/**********************     IP-related definition     ***********************/
/****************************************************************************/
const UH _ip_tblcnt[] = { T4_CFG_IP_ARP_CACHE_TABLE_COUNT, T4_CFG_IP_ARP_CACHE_TABLE_COUNT };

#define MY_IP_ADDR0     T4_CFG_FIXED_IP_ADDRESS_CH0           /* Local IP address  */

#define GATEWAY_ADDR0   T4_CFG_FIXED_GATEWAY_ADDRESS_CH0      /* Gateway address (invalid if all 0s) */

#define SUBNET_MASK0    T4_CFG_FIXED_SABNET_MASK_CH0          /* Subnet mask  */

#define MY_IP_ADDR1     T4_CFG_FIXED_IP_ADDRESS_CH1           /* Local IP address  */

#define GATEWAY_ADDR1   T4_CFG_FIXED_GATEWAY_ADDRESS_CH1      /* Gateway address (invalid if all 0s) */

#define SUBNET_MASK1    T4_CFG_FIXED_SABNET_MASK_CH1          /* Subnet mask  */

TCPUDP_ENV tcpudp_env[] =
{
    {{MY_IP_ADDR0}, {SUBNET_MASK0}, {GATEWAY_ADDR0}},
    {{MY_IP_ADDR1}, {SUBNET_MASK1}, {GATEWAY_ADDR1}},
};

/****************************************************************************/
/**********************     Driver-related definition     *******************/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/*    Set of Ethernet-related                                               */
/*--------------------------------------------------------------------------*/
/* Local MAC address (Set all 0s when unspecified) */
#define MY_MAC_ADDR0    T4_CFG_ETHER_CH0_MAC_ADDRESS

#define MY_MAC_ADDR1    T4_CFG_ETHER_CH1_MAC_ADDRESS

UB _myethaddr[][6] =
{
    {MY_MAC_ADDR0},
    {MY_MAC_ADDR1},
};

/*--------------------------------------------------------------------------*/
/*    Set of PPP-related                                                    */
/*--------------------------------------------------------------------------*/
/* Authentication-related setting */
const UH ppp_auth  = AUTH_PAP;               /* PAP,No authentication enabled */
UB user_name[6]    = "abcde";                /* user name */
UB user_passwd[6]  = "abc00";                /* password  */

/* Dial up-related setting */
const UB peer_dial[]      = "0,123";                /* Destination telephone number */
const UB at_commands[]   = "ATW2S0=2&C0&D0&S0M0S10=255X3";    /* Modem initialization command */


/* Copyright 2019, RENESAS ELECTRONICS CORPORATION */

