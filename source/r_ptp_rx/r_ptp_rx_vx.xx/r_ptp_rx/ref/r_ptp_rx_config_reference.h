/**********************************************************************************************************************
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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_ptp_rx_config.h
* Description  : Configure the PTP driver
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.12    Corrected the internal operation of getting version functions
*                              Corrected wait operation to information retention of GETINFOR
*                              Corrected the array index deviation of ptp_tmr_isr.
*         : 31.03.2017 1.13    Corrected BC and P2P TC setting.
*                              Modified listening state operations of R_PTP_Init, R_PTP_Start and R_PTP_Stop functions.
*                              Added registering user function to MINT interrupt handler.
*                              Changed MINT interrupt handler operation.
*                              Changed TC&OC combined opeartion to TC only operation.
*                              Added my clockIdentity setting function.
*                              Added domainnumber field updating function.
*                              Added announce message fields updating function.
*                              Added and modified transmission interval setting function.
*                              Added offsetFromMaster updating wait function when device state is P2P slave.
*                              Added PTP port state setting function.
*         : 30.04.2017 1.14    Corrected underflow operation of R_PTP_GetSyncInfo function.
*                              Corrected logMessageInterval out of range judgment.
*		  : 31.07.2019 1.15    Added support for GNUC and ICCRX.
*                              Added "WAIT_LOOP" comments for every loop operation.
*		  : 31.08.2019 1.16    Supported RX72M device.
*                              Added Bypass setting.
**********************************************************************************************************************/

#ifndef _PTP_CONFIG_H
#define _PTP_CONFIG_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* ==== PTP driver mode ==== */
#define PTP_MODE_CH0    (0x01ul)    /* enable EPTPC CH0 */
#define PTP_MODE_CH1    (0x02ul)    /* enable EPTPC CH1 */
#define PTP_MODE_POLL   (0x10ul)    /* EPTPC status check is software polling */
#define PTP_MODE_HWINT  (0x20ul)    /* EPTPC status check is hardware interrupt */

//#define EPTPC_CFG_MODE (PTP_MODE_CH0 | PTP_MODE_CH1 | PTP_MODE_POLL)
#define PTP_CFG_MODE (PTP_MODE_CH0 | PTP_MODE_CH1 | PTP_MODE_HWINT)

/* ==== PTPIF driver mode ==== */
#define PTPIF_MODE_CH0   (0x01ul)   /* use ETHERC CH0 */
#define PTPIF_MODE_CH1   (0x02ul)   /* use ETHERC CH1 */
#define PTPIF_MODE_POLL  (0x10ul)   /* PTPEDMAC status check is software polling, not supported in this version */
#define PTPIF_MODE_HWINT (0x20ul)   /* PTPEDMAC status check is hardware interrupt */

//#define PTPIF_CFG_MODE (PTPIF_MODE_CH0 | PTPIF_MODE_CH1 | PTPIF_MODE_POLL)
#define PTPIF_CFG_MODE (PTPIF_MODE_CH0 | PTPIF_MODE_CH1 | PTPIF_MODE_HWINT)

/* ==== PTPIF buffer and descriptors ==== */
/* The number of Rx descriptors */
#define PTPIF_CFG_NUM_RX_DESCRIPTORS (4)

/* The number of Tx descriptors */
#define PTPIF_CFG_NUM_TX_DESCRIPTORS (4)

/* PTPEDMAC buffer size */
#define PTPIF_CFG_BUFSIZE 	(1536) /* Min (64) byte, Max (1536) byte, set 32 byte times */

/* ==== PTP interrupt level definition ==== */
#define PTP_CFG_INTERRUPT_LEVEL    (2) /* set (1) to (15) */

/* ==== PTPIF interrupt level definition ==== */
#define PTPIF_CFG_INTERRUPT_LEVEL  (2) /* set (1) to (15) */

/* ==== PTP message format ==== */
#define PTP_MSG_FORM_ETH       (0x00)
#define PTP_MSG_FORM_ETH_8023  (0x01) /* This is not supported in the RX64M */
#define PTP_MSG_FORM_UDP4      (0x02)
#define PTP_MSG_FORM_UDP4_8023 (0x03) /* This is not supported in the RX64M */

#define PTP_CFG_MSG_FORM PTP_MSG_FORM_ETH

/* ==== PTP synchronous mode ==== */
#define PTP_SYNC_MODE1        (0x00)
#define PTP_SYNC_MODE2_HW     (0x02) /* recommend setting */
#define PTP_SYNC_MODE2_SW     (0x03)

#define PTP_CFG_SYNC_MODE PTP_SYNC_MODE2_HW

/* ==== Sync message reception timeout value ==== */
/* No timeout detection */
#define PTP_CFG_SYNC_TIMEOUT (0x00000000) /* 1024 nsec unit */

/* ==== Number of timer channel ==== */
#define NUM_OF_TMR_CHANNEL    (6) /* CH0 to CH5 */

/* ==== MTU3 PWM output channel ==== */
#define MTU3_PWM_OUTPUT_CH0   (0)  /* recommend setting */
#define MTU3_PWM_OUTPUT_CH3   (3)
#define MTU3_PWM_OUTPUT_CH4   (4)

#define PTP_CFG_MTU3_OUTPUT MTU3_PWM_OUTPUT_CH0

/* ==== GPT PWM output channel ==== */
#define GPT_PWM_OUTPUT_CH0    (0)
#define GPT_PWM_OUTPUT_CH1    (1)  /* recommend setting */
#define GPT_PWM_OUTPUT_CH2    (2)
#define GPT_PWM_OUTPUT_CH3    (3)

#define PTP_CFG_GPT_OUTPUT GPT_PWM_OUTPUT_CH1

/* ==== Local clock counter initial value ==== */
/* PTP time: Annex B.3 */
#define CURRENT_UTC_OFFSET (0x0008) /* approx. 1588 spec AnnexB B.2 */
#define PTP2NTP_OFFSET (2208988800) /* 2,208,988,800 sec */
#define NTP_SEC (3668572800) /* NTP(Network Time Protocol) second, 2016/03/31 08:00:00, 3,668,572,800 sec */
#define PTP_SEC (NTP_SEC - PTP2NTP_OFFSET + CURRENT_UTC_OFFSET) /* PTP second */

#define PTP_CFG_LCCLK_SEC_HI (0x0000)     /* sec order high */
#define PTP_CFG_LCCLK_SEC_LO (PTP_SEC)    /* sec order low  */
#define PTP_CFG_LCCLK_NANO   (0x12345678) /* nano sec order, 305,419,896 nsec */

/* ==== Timestamp latency setting value ==== */
/* MII ingress and egress timestamp latency setting */
#define MII_TIMESTAMP_LATENCY (0x03D4024E) /* MII, 100Mbps and STCA = 20MHz: INGP = 980(=0x3D4), EGP = 590(=0x24E) */
/* RMII ingress and egress timestamp latency setting */
#define RMII_TIMESTAMP_LATENCY (0x04240302) /* RMII, 100Mbps and STCA = 20MHz: INGP = 1060(=0x424), EGP = 770(=0x302) */

#define PTP_CFG_TIMESTAMP_LATENCY MII_TIMESTAMP_LATENCY /* Default value is MII, 100Mbps and STCA = 20MHz */

/* ==== PTP data sets and messages related parameter ==== */
/* LLC-CTL filed of IEEE802.3 format */
#define PTP_CFG_LLC_CTL (3)

/* PTP version field */
#define PTP_CFG_PTP_VER_NUM (0x02) /* IEEE1588-2008 (version2) spec */

/* domainNumber field */
#define PTP_CFG_DOMAIN_NUM (0) /* 0: Default domain, 1 to 3: Alternate domain, 4 to 127: User defined */

/* Announce message's flag field */
/* PTP profile Specific 2(b14) = false, PTP profile Specific 1(b13) = false, 
unicastFlag(b10) = false (not support unicast PTP in the RX64M/71M), alternateMasterFlag(b8) = false, 
frequencyTraceable(b5) = false, timeTraceable(b4) = false, ptpTimescale(b3) = false, 
currentUtcOffsetValid(b2) = false, leap59(b1) = false, leap61(b0) = false */
#define PTP_CFG_ANNOUNCE_FLAG_FIELD (0x00000000)

/* Sync message's flag field */
/* PTP profile Specific 2(b14) = false, PTP profile Specific 1(b13) = false, 
unicastFlag(b10) = false (not support unicast PTP in the RX64M/71M), 
twoStepFlag(b9) = false (not support two step in the RX64M/71M), alternateMasterFlag(b8) = false */
#define PTP_CFG_SYNC_FLAG_FIELD (0x00000000)

/* Delay_Req/Pdelay_Req message's flag field */
/* PTP profile Specific 2(b14) = false, PTP profile Specific 1(b13) = false, 
unicastFlag(b10) = false (not support unicast PTP in the RX64M/71M) */
#define PTP_CFG_DELAY_REQ_FLAG_FIELD (0x00000000)

/* Delay_Resp/Pdelay_Resp message's flag field */
/* PTP profile Specific 2(b14) = false, PTP profile Specific 1(b13) = false, 
unicastFlag(b10) = false (not support unicast PTP in the RX64M/71M), 
twoStepFlag(b9) = false (not support two step in the RX64M/71M) */
#define PTP_CFG_DELAY_RESP_FLAG_FIELD (0x00000000)

/* PortIdentity Clock-ID: OUI field (upper 3byte) and Extension field (lower 5byte) */
/* Clock identity creation base */
#define CLK_ID_EUI48_BASE (0) /* Create clock-ID based on MAC (EUI-48) address */
#define CLK_ID_USR_DEFINE (1) /* Set user specific value */
#define PTP_CFG_CLK_ID CLK_ID_EUI48_BASE

#if (CLK_ID_USR_DEFINE == PTP_CFG_CLK_ID)
#define PORTID_CLK_ID0_U (0xFF050123) /* Clock-ID high (Port0) */
#define PORTID_CLK_ID0_L (0x456789AB) /* Clock-ID low  (Port0) */
#define PORTID_CLK_ID1_U (0xFF05CDEF) /* Clock-ID high (Port1) */
#define PORTID_CLK_ID1_L (0x01234567) /* Clock-ID low  (Port1) */
#endif

/* PortIdentity port number: 1, 2,,, N, all 0 and all 1 are reserved */
#define PTP_CFG_PORTID_PORT_NUM0 (1) /* (Port0) */
#define PTP_CFG_PORTID_PORT_NUM1 (2) /* (Port1) */

/* Master clock-ID: In general, equal to parentDS.parentPortIdentity.clockIdentity field */
#define PTP_CFG_MTCID_U (0x00000000) /* Clock-ID hi */
#define PTP_CFG_MTCID_L (0x00000000) /* Clock-ID lo */

/* Master port number: In general, equal to parentDS.parentPortIdentity.portNumber field */
#define PTP_CFG_MTPID (0x0000)

/* grandmasterPriority1: Equal to parentDS.grandmasterPriority1 field */
/* From 0 to 255 can be set and lower value has higher priority */
#define PTP_CFG_GM_PRIORITY10 (0x00) /* (Port0) */
#define PTP_CFG_GM_PRIORITY11 (0x00) /* (Port1) */

/* grandmasterPriority2: Equal to parentDS.grandmasterPriority2 field */
/* From 0 to 255 can be set and lower value has higher priority */
#define PTP_CFG_GM_PRIORITY20 (0x00) /* (Port0) */
#define PTP_CFG_GM_PRIORITY21 (0x00) /* (Port1) */

/* grandmasterClockQuality: Equal to parentDS.grandmasterClockQuality field */
/* b31 to b24: clockClass, default value(=248, = 0xF8), 255 is slave only clock */
/* b23 to b16: clockAccuracy, default value(=0x21)is within 100 nsec, 0x20 to 0x31 */
/* b15 to b0: offsetScaledLogVariance, default value(=0xFFFF) is not calculated yet */
#define PTP_CFG_GM_CLK_QUALITY0 (0xF821FFFF) /* (Port0) */
#define PTP_CFG_GM_CLK_QUALITY1 (0xF821FFFF) /* (Port1) */

/* grandmasterIdentity: Equal to parentDS.grandmasterIdentity field */
#define PTP_CFG_GM_CLK_ID0_U (0x00000000) /* Clock-ID hi (Port0) */
#define PTP_CFG_GM_CLK_ID0_L (0x00000000) /* Clock-ID lo (Port1) */
#define PTP_CFG_GM_CLK_ID1_U (0x00000000) /* Clock-ID hi (Port0) */
#define PTP_CFG_GM_CLK_ID1_L (0x00000000) /* Clock-ID lo (Port1) */

/* currentUtcOffset: Equal to timePropertiesDS.currentUtcOffset field */
#define PTP_CFG_CUR_UTC_OFFSET0 CURRENT_UTC_OFFSET /* (Port0) */
#define PTP_CFG_CUR_UTC_OFFSET1 CURRENT_UTC_OFFSET /* (Port1) */

/* timeSource: Equal to timePropertiesDS.timeSource field */
#define PTP_CFG_TIME_SOURCE0 (0xA0) /* Timesource is internal oscillator (Port0) */
#define PTP_CFG_TIME_SOURCE1 (0xA0) /* Timesource is internal oscillator (Port1) */

/* stepsRemoved: Equal to currentDS.stepsRemoved field */
#define PTP_CFG_STEPS_REMOVED0 (0x0000) /* No traversed (Port0) */
#define PTP_CFG_STEPS_REMOVED1 (0x0000) /* No traversed (Port1) */

/* PTP event message's TOS field */
#define PTP_CFG_PTP_EVENT_TOS0 (0x00) /* 0: Best effort (Port0) */
#define PTP_CFG_PTP_EVENT_TOS1 (0x00) /* 0: Best effort (Port1) */

/* PTP general message's TOS field */
#define PTP_CFG_PTP_GENERAL_TOS0 (0x00) /* 0: Best effort (Port0) */
#define PTP_CFG_PTP_GENERAL_TOS1 (0x00) /* 0: Best effort (Port1) */

/* PTP-primary message's TTL field */
#define PTP_CFG_PTP_PRIMARY_TTL0 (0x80) /* 128 (Port0) */
#define PTP_CFG_PTP_PRIMARY_TTL1 (0x80) /* 128 (Port1) */

/* PTP-pdelay message's TTL field */
#define PTP_CFG_PTP_PDELAY_TTL0 (0x01) /* 1 (Port0) */
#define PTP_CFG_PTP_PDELAY_TTL1 (0x01) /* 1 (Port1) */

/* ==== PTP message transmission interval ==== */
#define PTP_CFG_LOG_ANNOUNCE_INTERVAL0   		(0x01) /* 0x01: 2sec */
#define PTP_CFG_LOG_ANNOUNCE_INTERVAL1			(0x01) /* 0x01: 2sec */
#define PTP_CFG_LOG_SYNC_INTERVAL0				(0x00) /* 0x00: 1sec */
#define PTP_CFG_LOG_SYNC_INTERVAL1				(0x00) /* 0x00: 1sec */
#define PTP_CFG_LOG_MIN_DELAY_REQ_INTERVAL0		(0x00) /* 0x00: 1sec */
#define PTP_CFG_LOG_MIN_DELAY_REQ_INTERVAL1		(0x00) /* 0x00: 1sec */
#define PTP_CFG_LOG_MIN_PDELAY_REQ_INTERVAL0	(0x00) /* 0x00: 1sec */
#define PTP_CFG_LOG_MIN_PDELAY_REQ_INTERVAL1	(0x00) /* 0x00: 1sec */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* _PTP_CONFIG_H */

