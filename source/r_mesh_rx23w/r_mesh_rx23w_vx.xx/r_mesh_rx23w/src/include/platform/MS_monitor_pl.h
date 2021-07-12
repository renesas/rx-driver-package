/**
 *  \file MS_monitor_pl.h
 *
 *
 */

/*
 *  Copyright (C) 2020. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_MS_MONITOR_PL_
#define _H_MS_MONITOR_PL_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "EM_platform.h"
#include "MS_common.h"
#include "MS_net_api.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/**
 * \addtogroup ms_common_constants
 * \{
 */

/** \cond definitions for internal module */
/** Mesh Monitor PDU/SNB IDs */
#define MS_MONITOR_ID_ACCESS_PDU        (0)
#define MS_MONITOR_ID_TRANS_PDU         (1)
#define MS_MONITOR_ID_LTRANS_PDU        (2)
#define MS_MONITOR_ID_NET_PDU           (3)
#define MS_MONITOR_ID_NET_SNB           (4)
#define MS_MONITOR_ID_MAX               (5)
/** \endcond */

/** \name Mesh Monitor Configuration
 *  \{
 */
/** None */
#define MS_MONITOR_NONE                 (0)
/** All PDUs and SNB */
#define MS_MONITOR_ALL                  ((1 << MS_MONITOR_ID_MAX) - 1)
/** Access PDU */
#define MS_MONITOR_ACCESS_PDU           (1 << MS_MONITOR_ID_ACCESS_PDU)
/** Upper Transport PDU */
#define MS_MONITOR_TRANS_PDU            (1 << MS_MONITOR_ID_TRANS_PDU)
/** Lower Transport PDU */
#define MS_MONITOR_LTRANS_PDU           (1 << MS_MONITOR_ID_LTRANS_PDU)
/** Network PDU */
#define MS_MONITOR_NET_PDU              (1 << MS_MONITOR_ID_NET_PDU)
/** Network Secure Network Beacon */
#define MS_MONITOR_NET_SNB              (1 << MS_MONITOR_ID_NET_SNB)
/** \} */

/** \name Mesh Monitor Direction
 *  \{
 */
/** Outgoing PDU/SNB */
#define MS_MONITOR_DIR_TX               (0)
/** Incoming PDU/SNB */
#define MS_MONITOR_DIR_RX               (1)
/** \} */

/** \} */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \addtogroup ms_common_structures
 * \{
 * \name Mesh Monitor
 * \{
 */

typedef struct {
    /** Mesh Monitor Configuration */
    UCHAR cfg;

    /** Access PDU callback function */
    void (*access_pdu)(
                UCHAR           dir,
                UINT32          opcode,
                const UCHAR *   pdata,
                UINT16          datalen);

    /** Upper Transport PDU callback function */
    void (*trans_pdu)(
                UCHAR           dir,
                UINT16          appkey_handle,
                UCHAR           akf,
                UCHAR           aid,
                const UCHAR *   pdata,
                UINT16          datalen);

    /** Lower Transport PDU callback function */
    void (*ltrans_pdu)(
                UCHAR           dir,
                UCHAR           reliable,
                UCHAR           szmic,
                UINT16          seq_zero,
                const UCHAR *   pdata,
                UINT16          datalen);

    /** Network PDU callback function */
    void (*net_pdu)(
                UCHAR           dir,
                const MS_NET_HEADER * hdr,
                const UCHAR *   pdata,
                UINT16          datalen,
                UCHAR           is_relay);

    /** Network SNB callback function */
    void (*net_snb)(
                UCHAR           dir,
                MS_SUBNET_HANDLE subnet_handle,
                UINT32          iv_index,
                UCHAR           flags,
                UCHAR           is_new_key,
                const UCHAR *   pdata,
                UINT16          datalen);
} MS_MONITOR_PL;

/** \} */
/** \} */

/*******************************************************************************
* API declarations
*******************************************************************************/
/**
 * \addtogroup ms_common_api
 * \{
 * \name Mesh Monitor
 * \{
 */

/**
 *  \brief Register Mesh Monitor Callbacks with Mesh Stack.
 *
 *  \par Description
 *  API to register callback functions with Mesh Stack to monitor incoming and outgoing
 *  PDU(Protocol Data Unit) and SNB(Secure Network Beacon).
 *  The following PDU/SNB can be monitored.
 *    - Access PDU
 *    - Upper Transport PDU
 *    - Lower Transport PDU
 *    - Network PDU
 *    - Network SNB
 *
 *  \param [in] monitor     mesh monitor callbacks
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
void MS_monitor_register_pl(const MS_MONITOR_PL * monitor);

/** \} */
/** \} */

#endif /* _H_MS_MONITOR_PL_ */
