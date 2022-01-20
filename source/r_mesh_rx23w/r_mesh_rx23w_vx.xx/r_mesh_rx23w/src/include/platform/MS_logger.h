/**
 *  \file MS_logger.h
 *
 *
 */

/*
 *  Copyright (C) 2020. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_MS_LOGGER_
#define _H_MS_LOGGER_

/* --------------------------------------------- Header File Inclusion */
#include "EM_platform.h"
#include "MS_common.h"
#include "MS_net_api.h"
#include "MS_trn_api.h"

/* --------------------------------------------- Global Definitions */
/** Defines to indicate the packet direction */
#define MS_LOGGER_TX                             0
#define MS_LOGGER_RX                             1

/** Logger Module IDs */
#define MS_LOGGER_NET_MODULE_ID                  0
#define MS_LOGGER_LTR_MODULE_ID                  1
#define MS_LOGGER_TRN_MODULE_ID                  2
#define MS_LOGGER_ACC_MODULE_ID                  3

#define MS_LOGGER_MAX_MODULES                    4

/** Bitmask Defines to enable/disable the trace and info levels */
#define MS_LOGGER_NET       (1 << MS_LOGGER_NET_MODULE_ID)
#define MS_LOGGER_LTR       (1 << MS_LOGGER_LTR_MODULE_ID)
#define MS_LOGGER_TRN       (1 << MS_LOGGER_TRN_MODULE_ID)
#define MS_LOGGER_ACC       (1 << MS_LOGGER_ACC_MODULE_ID)

/** Logger Module validity bitmasks */
#define MS_LOGGER_BITMASK   ((1 << MS_LOGGER_MAX_MODULES) - 1)

/* --------------------------------------------- Structures/Data Types */
/** Network Logging related Log Packet Info structure  */
typedef struct _MS_LOGGER_NET_LOG_PKT_INFO
{
    /* Network Header reference */
    MS_NET_HEADER * hdr;

    /* Payload Buffer */
    MS_BUFFER     data;

    /* Relay Flag */
    UCHAR         is_relay;

} MS_LOGGER_NET_LOG_PKT_INFO;

/** Network Logging related Log SNB Info structure  */
typedef struct _MS_LOGGER_NET_LOG_SNB_INFO
{
    /* Subnet Handle */
    MS_SUBNET_HANDLE sh;

    /* IV Index */
    UINT32           iv_index;

    /* Flags Field */
    UCHAR            flags;

    /* Is new Key Flag */
    UCHAR            is_new_key;

    /* Payload Buffer */
    MS_BUFFER        data;

} MS_LOGGER_NET_LOG_SNB_INFO;

/* Lower Transport Logging related Log Packet Info Structures */
typedef struct _MS_LOGGER_LTR_LOG_TX_PKT_INFO
{
    /* Network Header reference */
    MS_NET_HEADER    * hdr;

    /* Subnet Handle */
    MS_SUBNET_HANDLE sh;

    /* Message Type */
    UCHAR            trn_msg_type;

    /* LTR AKF */
    UINT8            akf;

    /* LTR AID */
    UINT8            aid;

    /* LTR Seq0 */
    UINT16           seq_zero;

    /* Payload Buffer */
    MS_BUFFER        data;

} MS_LOGGER_LTR_LOG_TX_PKT_INFO;

/* Lower Transport Logging related Log Packet Info Structures */
typedef struct _MS_LOGGER_LTR_LOG_RX_SEG_PKT_INFO
{
    /* Network Header reference */
    MS_NET_HEADER    * hdr;

    /* Subnet Handle */
    MS_SUBNET_HANDLE sh;

    /* LTR Seq0 */
    UINT16           seq_zero;

    /* LTR SZMIC */
    UINT8            szmic;

    /* LTR SEG_O */
    UINT8            seg_o;

    /* LTR SEG_N */
    UINT8            seg_n;

    /* LTR OBO */
    UINT8            obo;

    /* LTR SEQ AUTH */
    UINT32           seq_auth;

    /* Payload Buffer */
    MS_BUFFER        data;

} MS_LOGGER_LTR_LOG_RX_SEG_PKT_INFO;

/* Lower Transport Logging related Log Packet Info Structures */
typedef struct _MS_LOGGER_LTR_LOG_RX_UNSEG_PKT_INFO
{
    /* Network Header reference */
    MS_NET_HEADER    * hdr;

    /* Subnet Handle */
    MS_SUBNET_HANDLE sh;

    /* LTR Seq0 */
    UINT16           seq_zero;

    /* Payload Buffer */
    MS_BUFFER        data;

} MS_LOGGER_LTR_LOG_RX_UNSEG_PKT_INFO;

/* Upper Transport Logging related Log Packet Info Structures */
typedef struct _MS_LOGGER_TRN_LOG_PKT_INFO
{
    /* Network Header reference */
    MS_NET_HEADER    * hdr;

    /* Subnet Handle */
    MS_SUBNET_HANDLE sh;

    /* Appkey Handle */
    MS_APPKEY_HANDLE ah;

    /* TRN AKF */
    UINT8            akf;

    /* TRN AID */
    UINT8            aid;

    /* Payload Buffer */
    MS_BUFFER        data;

} MS_LOGGER_TRN_LOG_PKT_INFO;

typedef struct _MS_LOGGER_TRN_LOG_HEART_BEAT_INFO
{
    /* Network Header reference */
    MS_NET_HEADER          * hdr;

    /* Subnet Handle */
    MS_SUBNET_HANDLE       sh;

    /* Heart Beat Param */
    MS_TRN_HEARTBEAT_PARAM * hb;

} MS_LOGGER_TRN_LOG_HEART_BEAT_INFO;

/** Access Logging related Log Packet Info structure  */
typedef struct _MS_LOGGER_ACCESS_LOG_PKT_INFO
{
    /* Opcode */
    UINT32    opcode;

    /* Payload Buffer */
    MS_BUFFER data;

} MS_LOGGER_ACCESS_LOG_PKT_INFO;

/* --------------------------------------------- Macros */
/* Network Log Message ID */
#define MS_NET_LOG_ID_TX_PKT                         0x0001
#define MS_NET_LOG_ID_RX_PKT                         0x0002
#define MS_NET_LOG_ID_SNB_PKT                        0x0003

/* Lower Transport Log Message ID */
#define MS_LTRN_LOG_ID_TX_PKT                        0x0001
#define MS_LTRN_LOG_ID_RX_SEG_PKT                    0x0002
#define MS_LTRN_LOG_ID_RX_UNSEG_PKT                  0x0003

/* Upper Transport Log Message ID */
#define MS_TRN_LOG_ID_TX_PKT                         0x0001
#define MS_TRN_LOG_ID_RX_PKT                         0x0002
#define MS_TRN_LOG_ID_HEART_BEAT_PKT                 0x0003

/* Access Log Message ID */
#define MS_ACCESS_LOG_ID_TX_PKT                      0x0001
#define MS_ACCESS_LOG_ID_RX_PKT                      0x0002

/* Generic Log Message Types */
#define MS_NET_GEN_LOG_ID_QUEUE_FULL                 0x0001
#define MS_NET_GEN_LOG_ID_LOG_ID_INVALID_PKT         0x0002
#define MS_LTRN_GEN_LOG_ID_ACK_TIMER_INFO            0x0003
#define MS_LTRN_GEN_LOG_ID_SAR_INCOMPLETE_TIMER_INFO 0x0004
#define MS_LTRN_GEN_LOG_ID_SAR_TIMEOUT_INFO          0x0005

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

#ifdef MS_LOGGER
/** Protocol data logging interfaces */
/* Network Logging */
void MS_logger_net_pdu_pl
     (
         UCHAR   dir,
         UINT16  log_msg_id,
         void    * data
     );

/* Lower Transport Logging */
void MS_logger_ltrans_pdu_pl
     (
         UCHAR   dir,
         UINT16  log_msg_id,
         void    * data
     );

/* Upper Transport Logging */
void MS_logger_trans_pdu_pl
     (
         UCHAR   dir,
         UINT16  log_msg_id,
         void    * data
     );

/* Access Logging */
void MS_logger_access_pdu_pl
     (
         UCHAR   dir,
         UINT16  log_msg_id,
         void    * data
     );

/* Generic Logging */
void MS_logger_generic_log_msg_pl
     (
         UCHAR module_id,
         UINT16 log_msg_type,
         UINT16 status,
         void    * data
     );

void MS_logger_dump_bytes_pl
     (
         UCHAR module_id,
         UCHAR * buffer,
         UINT16 length
     );
void MS_logger_set_config_pl(UCHAR trace, UCHAR info, UCHAR gen);

#else /* MS_LOGGER */
#define MS_logger_access_pdu_pl(...)
#define MS_logger_trans_pdu_pl(...)
#define MS_logger_ltrans_pdu_pl(...)
#define MS_logger_net_pdu_pl(...)
#define MS_logger_generic_log_msg_pl (...)

#define MS_logger_dump_bytes_pl(...)
#define MS_logger_set_config_pl(...)

#endif /* MS_LOGGER */

#endif /* _H_MS_LOGGER_ */

