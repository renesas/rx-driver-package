
/**
 *  \file MS_ltrn_api.h
 *
 *  \brief This file defines the Mesh Lower Transport Application Interface - includes
 *  Data Structures and Methods.
 */

/*
 *  Copyright (C) 2017. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_LTRN_API_
#define _H_MS_LTRN_API_


/* --------------------------------------------- Header File Inclusion */
/* Network API Header File */
#include "MS_net_api.h"

/* --------------------------------------------- Global Definitions */

/**
 * \defgroup ltrn_module Lower Transport Layer (LTRN)
 * \ingroup mesh_core_block
 * \{
 *  \brief This section describes the interfaces & APIs offered by the EtherMind
 *  Mesh Lower Transport (LTRN) module to the Application and other upper
 *  layers of the stack.
 */

/**
 * \defgroup ltrn_constants Constants
 * \{
 * \brief This section describes the EtherMind Mesh Lower Transport Layer Constants.
 */

/**
 * \name format types
 * \{
 */
/** Unsgemented Access Message */
#define MS_UNSEGMENTED_ACCESS_MSG     0x00

/** Segmented Access Message */
#define MS_SEGMENTED_ACCESS_MSG       0x01

/** Unsegmented Control Message */
#define MS_UNSEGMENTED_CTRL_MSG       0x02

/** Segmented Control Message */
#define MS_SEGMENTED_CTRL_MSG         0x03
/** \} */

/** Transport Message Type */
typedef UCHAR   MS_TRN_MSG_TYPE;

/**
 * \name message types
 * \{
 */
/** Transport Layer Control Packet */
#define MS_TRN_CTRL_PKT            0x01

/** Access Layer Packet */
#define MS_TRN_ACCESS_PKT          0x00

/** \} */

/** \} */

/* --------------------------------------------- Data Types/ Structures */

/**
 *  \addtogroup ltrn_types_structures Types/Structures
 *  \{
 *  \brief This section describes the EtherMind Mesh Lower Transport Layer Types/Structures.
 */
/** LPN Handle */
typedef UINT8 LPN_HANDLE;

/** \} */

/**
 *  \defgroup ltrn_cb Application Callback
 *  \{
 *  \brief This section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 * Lower TRANSPORT Application Asynchronous Notification Callback.
 *
 * Lower TRANSPORT calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param net_hdr           Network Header.
 * \param subnet_handle     Associated Subnet Handle.
 * \param data_param        Data associated with the event if any or NULL.
 * \param data_len          Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (*LTRN_NTF_CB)
        (
            MS_NET_HEADER     * net_hdr,
            MS_SUBNET_HANDLE    subnet_handle,
            UCHAR               szmic,
            UCHAR             * data_param,
            UINT16              data_len
        ) DECL_REENTRANT;

/**
 * Lower TRANSPORT Friend Segment Ack Asynchronous Notification Callback for Transport Layer.
 *
 * Lower TRANSPORT calls the registered callback to indicate Friend Segment Ack event
 * to the transport layer.
 *
 * \param subnet_handle     Associated Subnet Handle.
 */
typedef void (*LTRN_FRND_HANDLE_SEGMENT_ACK_CB)
        (
            MS_SUBNET_HANDLE    subnet_handle
        ) DECL_REENTRANT;

/**
 * Lower TRANSPORT Add to Queue Asynchronous Notification Callback for Transport Layer.
 *
 * Lower TRANSPORT calls the registered callback to indicate add to queue to the
 * transport layer.
 *
 * \param lpn_index         LPN identifier.
 * \param pkt_type          Packet Type.
 * \param net_hdr           Network Header.
 * \param pdu               PDU associated with the event.
 * \param pdu_len           Size of the PDU.
 */
typedef API_RESULT (*LTRN_ADD_TO_QUEUE_CB)
        (
            LPN_HANDLE       lpn_index,
            UINT8            pkt_type,
            MS_NET_HEADER  * net_hdr,
            UINT8          * pdu,
            UINT8            pdu_len
        ) DECL_REENTRANT;

/* TODO: Move this also part of the callback structure */
/**
 * Lower TRANSPORT Transmit State Application Asynchronous Notification
 * Callback.
 *
 * Lower TRANSPORT calls the registered callback to to indicate the current
 * transmit state of the Lower TRANSPORT layer Segmentation & Reassembly
 * Context to the application.
 *
 * \param subnet_handle     Associated Subnet Handle.
 * \param status            status of the current Lower TRANSPORT layer
 *                          Segmentation & Reassembly Context State.
 */
typedef API_RESULT (*LTRN_TX_STATE_ACCESS_CB)
        (
            MS_SUBNET_HANDLE  subnet_handle,
            UINT16            status
        ) DECL_REENTRANT;

/** Lower Transport Callback Data Structure */
typedef struct _MS_LTRN_CB_STRUCT
{
    /** Data reception callback */
    LTRN_NTF_CB ntf_cb;

    /** Friend Segment Ack callback */
    LTRN_FRND_HANDLE_SEGMENT_ACK_CB seg_ack_cb;

    /** Add to Queue callback */
    LTRN_ADD_TO_QUEUE_CB add_to_queue_cb;

} MS_LTRN_CB_STRUCT;

/** \} */

/** TCF (Transport Control Field) - Transport Field Value */


/* --------------------------------------------- Function */

/**
 * \defgroup ltrn_api_defs API Definitions
 * \{
 * \brief This section describes the EtherMind Mesh Lower Transport Layer APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** \cond DOC_EXCLUDE */
/**
 *  \brief Register Inerface with Lower Transport Layer
 *
 *  \par Description
 *  This routine registers interface with the Lower Transport Layer.
 *  Transport Layer supports single Application, hence this rouine shall be called once.
 *
 *  \param [in] cb
 *         Pointer to data structure holding Upper Layer Notification Callbacks
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_ltrn_register
           (
               /* IN */ MS_LTRN_CB_STRUCT    * cb
           );
/** \endcond */

/**
 *  \brief API to send transport PDUs
 *
 *  \par Description
 *  This routine sends transport PDUs to peer device.
 *
 *  \param [in] src_addr
 *         Source Address
 *
 *  \param [in] dst_addr
 *         Destination Address
 *
 *  \param [in] subnet_handle
 *         Handle identifying the Subnet
 *
 *  \param [in] msg_type
 *         Transport Message Type
 *
 *  \param [in] ttl
 *         Time to Live
 *
 *  \param [in] akf
 *         Application Key Flag
 *
 *  \param [in] aid
 *         Application Key Identifier
 *
 *  \param [in] seq_num
 *         Sequence Number to be used for the Packet
 *
 *  \param [in] buffer
 *         Transport Packet
 *
 *  \param [in] buffer_len
 *         Transport Packet Length
 *
 *  \param [in] reliable
 *         If requires lower transport Ack, set reliable as TRUE
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_ltrn_send_pdu
           (
               /* IN */ MS_NET_ADDR               src_addr,
               /* IN */ MS_NET_ADDR               dst_addr,
               /* IN */ MS_SUBNET_HANDLE          subnet_handle,
               /* IN */ MS_TRN_MSG_TYPE           msg_type,
               /* IN */ UINT8                     ttl,
               /* IN */ UINT8                     akf,
               /* IN */ UINT8                     aid,
               /* IN */ UINT32                    seq_num,
               /* IN */ UCHAR                   * buffer,
               /* IN */ UINT16                    buffer_len,
               /* IN */ UINT8                     reliable
           );

/**
 *  \brief To clear all Segmentation and Reassembly Contexts
 *
 *  \par Description
 *  This routine clears all Segmentation and Reassembly Contexts.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_ltrn_clear_sar_contexts(void);

/**
 *  \brief To clear all Segmentation and Reassembly Contexts for a given Subnet
 *
 *  \par Description
 *  This routine clears all Segmentation and Reassembly Contexts.
 *
 *  \param [in] subnet_handle    Subnet Handle whose respective SAR Contexts
 *                               are to be cleared.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_ltrn_clear_subnet_sar_contexts
           (
               /* IN */ MS_SUBNET_HANDLE   subnet_handle
           );

/**
 *  \brief To reinitialize all Lower Transport Replay Cache Entries
 *
 *  \par Description
 *  This routine clears and reinitializes all Transport Replay Cache Entries.
 *  This needs to be invoked by the upper layer when the Network moves to a
 *  newer IV Index (Normal State) and the Sequence
 *  numbers are reset.
 *
 *  \return None
 */
void MS_ltrn_reinit_replay_cache(void);

/** \cond DOC_EXCLUDE */
/**
 *  \brief Register Inerface with Lower Transport Layer to fetch the current
 *         transmit state.
 *
 *  \par Description
 *  This routine registers interface with the Lower Transport Layer to fetch the current transmit
 *  state of the Lower Transport Layer.
 *
 *  \param [in] tx_state_cb
 *         Upper Layer Lower Transport TX state Notification Callback
 *
 *  \return API_SUCCESS when tx_state_cb is not null and the Lower Transport
 *          Tx state is engaged or,
 *          API_SUCCESS when tx_state_cb parameter is NULL or
 *          API_FAILURE when Lower Transport Tx state is free.
 */
API_RESULT MS_ltrn_register_tx_state_access
           (
               /* IN */ LTRN_TX_STATE_ACCESS_CB    tx_state_cb
           );

/**
 *  \brief To trigger any LTRN pending Transmissions.
 *
 *  \par Description
 *  Trigger pending transmits is an interface to check for pending segments in
 *  the tx queue and schedule for transmission, which is mainly used by the LPN
 *  operation.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_ltrn_trigger_pending_transmits(void);
/** \endcond */

#ifdef __cplusplus
};
#endif

/** \} */

/** \} */

#endif /* _H_MS_LTRN_API_ */

