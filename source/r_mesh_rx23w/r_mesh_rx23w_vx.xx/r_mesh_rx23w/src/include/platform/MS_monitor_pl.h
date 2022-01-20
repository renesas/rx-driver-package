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
/** Mesh Monitor PDU/Logs IDs */
#define MS_MONITOR_ID_NET_PDU           (0)
#define MS_MONITOR_ID_LTRANS_PDU        (1)
#define MS_MONITOR_ID_TRANS_PDU         (2)
#define MS_MONITOR_ID_ACCESS_PDU        (3)
#define MS_MONITOR_ID_GENERIC_LOG       (4)
#define MS_MONITOR_ID_MAX               (5)
/** \endcond */

/** \name Mesh Monitor Configuration
 *  \{
 */
/** None */
#define MS_MONITOR_NONE                 (0)
/** All PDUs and Logs */
#define MS_MONITOR_ALL                  ((1 << MS_MONITOR_ID_MAX) - 1)
/** Network PDU */
#define MS_MONITOR_NET_PDU              (1 << MS_MONITOR_ID_NET_PDU)
/** Lower Transport PDU */
#define MS_MONITOR_LTRANS_PDU           (1 << MS_MONITOR_ID_LTRANS_PDU)
/** Upper Transport PDU */
#define MS_MONITOR_TRANS_PDU            (1 << MS_MONITOR_ID_TRANS_PDU)
/** Access PDU */
#define MS_MONITOR_ACCESS_PDU           (1 << MS_MONITOR_ID_ACCESS_PDU)
/** Generic Logging */
#define MS_MONITOR_GENERIC_LOG          (1 << MS_MONITOR_ID_GENERIC_LOG)
/** \} */

/** \name Mesh Monitor Direction
 *  \{
 */
/** Outgoing PDU */
#define MS_MONITOR_DIR_TX               (0)
/** Incoming PDU */
#define MS_MONITOR_DIR_RX               (1)
/** \} */

/** \} */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \addtogroup ms_common_structures
 * \{
 * \name Mesh Monitor Parameter
 * \{
 */

typedef struct
{
    /** Mesh Monitor Configuration */
    UCHAR cfg;

    /** Access PDU callback function */
    void (*access_pdu)(UCHAR dir, UINT16 log_msg_id, void * data);

    /** Upper Transport PDU callback function */
    void (*trans_pdu)(UCHAR dir, UINT16 log_msg_id, void * data);

    /** Lower Transport PDU callback function */
    void (*ltrans_pdu)(UCHAR dir, UINT16 log_msg_id, void * data);

    /** Network PDU callback function */
    void (*net_pdu)(UCHAR dir, UINT16 log_msg_id, void * data);

    /** Generic Logging callback function */
    void (*generic_log)(UCHAR module_id, UINT16 log_msg_type, UINT16 status, void * data);

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
 *    - Network PDU/SNB
 *
 *  \param [in] monitor     mesh monitor callbacks
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
void MS_monitor_register_pl(const MS_MONITOR_PL * monitor);

/** \} */
/** \} */

#endif /* _H_MS_MONITOR_PL_ */
