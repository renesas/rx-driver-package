/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_t4_rx_config_reference.h
* Version      : 2.09
* Description  : Configuration options for the r_t4_rx module.
* Website      : https://www.renesas.com/mw/t4
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.04.2014 1.00    First Release
*         : 30.11.2016 1.01    File Header maintenance
*         : 12.12.2017 2.07    Corresponding to Smart Configurator.
*         : 10.12.2018 2.08    Corresponding to Smart Configurator.
*         : 20.06.2019 2.09    Added TCP Keep-Alive Function.
***********************************************************************************************************************/
#ifndef T4_RX_CONFIG_HEADER_FILE
#define T4_RX_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
#define T4_CFG_SYSTEM_CHANNEL_NUMBER                      (1)
#define T4_CFG_SYSTEM_DHCP                                (1)
#define T4_CFG_FIXED_IP_ADDRESS_CH0                       192,168,0,3
#define T4_CFG_FIXED_SABNET_MASK_CH0                      255,255,255,0
#define T4_CFG_FIXED_GATEWAY_ADDRESS_CH0                  0,0,0,0
#define T4_CFG_FIXED_IP_ADDRESS_CH1                       192,168,0,10
#define T4_CFG_FIXED_SABNET_MASK_CH1                      255,255,255,0
#define T4_CFG_FIXED_GATEWAY_ADDRESS_CH1                  0,0,0,0
#define T4_CFG_ETHER_CH0_MAC_ADDRESS                      0x74,0x90,0x50,0x00,0x79,0x03
#define T4_CFG_ETHER_CH1_MAC_ADDRESS                      0x74,0x90,0x50,0x00,0x79,0x10
#define T4_CFG_SYSTEM_CALLBACK_FUNCTION_USE               (1)
#define T4_CFG_SYSTEM_CALLBACK_FUNCTION_NAME_TMP          (system_callback)
#define T4_CFG_TCP_REPID1_PORT_NUMBER                     (1024)
#define T4_CFG_TCP_REPID2_PORT_NUMBER                     (1025)
#define T4_CFG_TCP_REPID3_PORT_NUMBER                     (1026)
#define T4_CFG_TCP_REPID4_PORT_NUMBER                     (1027)
#define T4_CFG_TCP_CEPID1_CHANNEL                         (0)
#define T4_CFG_TCP_CEPID1_RECEIVE_WINDOW_SIZE             (1460)
#define T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_TCP_CEPID1_KEEPALIVE_ENABLE                (0)
#define T4_CFG_TCP_CEPID2_CHANNEL                         (0)
#define T4_CFG_TCP_CEPID2_RECEIVE_WINDOW_SIZE             (1460)
#define T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_TCP_CEPID2_KEEPALIVE_ENABLE                (0)
#define T4_CFG_TCP_CEPID3_CHANNEL                         (0)
#define T4_CFG_TCP_CEPID3_RECEIVE_WINDOW_SIZE             (1460)
#define T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_TCP_CEPID3_KEEPALIVE_ENABLE                (0)
#define T4_CFG_TCP_CEPID4_CHANNEL                         (0)
#define T4_CFG_TCP_CEPID4_RECEIVE_WINDOW_SIZE             (1460)
#define T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_TCP_CEPID4_KEEPALIVE_ENABLE                (0)
#define T4_CFG_TCP_CEPID5_CHANNEL                         (0)
#define T4_CFG_TCP_CEPID5_RECEIVE_WINDOW_SIZE             (1460)
#define T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_TCP_CEPID5_KEEPALIVE_ENABLE                (0)
#define T4_CFG_TCP_CEPID6_CHANNEL                         (0)
#define T4_CFG_TCP_CEPID6_RECEIVE_WINDOW_SIZE             (1460)
#define T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_TCP_CEPID6_KEEPALIVE_ENABLE                (0)
#define T4_CFG_TCP_MSS                                    (1460)
#define T4_CFG_TCP_2MSL_TIME                              (60)
#define T4_CFG_TCP_MAX_TIMEOUT_PERIOD                     (600)
#define T4_CFG_TCP_DIVIDE_SENDING_PACKET                  (1)
#define T4_CFG_TCP_KEEPALIVE_START                        (7200)
#define T4_CFG_TCP_KEEPALIVE_INTERVAL                     (10)
#define T4_CFG_TCP_KEEPALIVE_COUNT                        (10)
#define T4_CFG_UDP_CEPID1_CHANNEL                         (0)
#define T4_CFG_UDP_CEPID1_PORT_NUMBER                     (1365)
#define T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_UDP_CEPID2_CHANNEL                         (0)
#define T4_CFG_UDP_CEPID2_PORT_NUMBER                     (1366)
#define T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_UDP_CEPID3_CHANNEL                         (0)
#define T4_CFG_UDP_CEPID3_PORT_NUMBER                     (1367)
#define T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_UDP_CEPID4_CHANNEL                         (0)
#define T4_CFG_UDP_CEPID4_PORT_NUMBER                     (1368)
#define T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_UDP_CEPID5_CHANNEL                         (0)
#define T4_CFG_UDP_CEPID5_PORT_NUMBER                     (1369)
#define T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_UDP_CEPID6_CHANNEL                         (0)
#define T4_CFG_UDP_CEPID6_PORT_NUMBER                     (1370)
#define T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_USE           (0)
#define T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_NAME_TMP      (0)
#define T4_CFG_UDP_MULTICAST_TTL                          (1)
#define T4_CFG_UDP_BEHAVIOR_OF_RECEIVED_ZERO_CHECKSUM     (0)
#define T4_CFG_IP_ARP_CACHE_TABLE_COUNT                   (3)

#if (T4_CFG_SYSTEM_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_SYSTEM_CALLBACK_FUNCTION_NAME              (0)
#else
#define T4_CFG_SYSTEM_CALLBACK_FUNCTION_NAME              (T4_CFG_SYSTEM_CALLBACK_FUNCTION_NAME_TMP)
#endif

#if (T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_NAME          (T4_CFG_TCP_CEPID1_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_NAME          (T4_CFG_TCP_CEPID2_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_NAME          (T4_CFG_TCP_CEPID3_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_NAME          (T4_CFG_TCP_CEPID4_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_NAME          (T4_CFG_TCP_CEPID5_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_NAME          (T4_CFG_TCP_CEPID6_CALLBACK_FUNCTION_NAME_TMP)
#endif

#if (T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_NAME          (T4_CFG_UDP_CEPID1_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_NAME          (T4_CFG_UDP_CEPID2_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_NAME          (T4_CFG_UDP_CEPID3_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_NAME          (T4_CFG_UDP_CEPID4_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_NAME          (T4_CFG_UDP_CEPID5_CALLBACK_FUNCTION_NAME_TMP)
#endif
#if (T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_USE == (0))
#define T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_NAME          (0)
#else
#define T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_NAME          (T4_CFG_UDP_CEPID6_CALLBACK_FUNCTION_NAME_TMP)
#endif

#endif /* T4_RX_CONFIG_HEADER_FILE */


