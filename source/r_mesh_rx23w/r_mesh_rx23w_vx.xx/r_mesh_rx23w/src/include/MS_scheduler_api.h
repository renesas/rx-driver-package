/**
 * \file MS_scheduler_api.h
 *
 * \brief This file defines the Mesh Scheduler Model Application Interface
 * - includes Data Structures and Methods for both Server and Client.
 */

/*
 * Copyright (C) 2017. Mindtree Ltd.
 * All rights reserved.
 */

#ifndef _H_MS_SCHEDULER_API_
#define _H_MS_SCHEDULER_API_


/* --------------------------------------------- Header File Inclusion */
#include "MS_access_api.h"


/* --------------------------------------------- Global Definitions */
/**
 * \defgroup scheduler_module SCHEDULER (Mesh Scheduler Model)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Mesh Scheduler Model (SCHEDULER) module to the Application.
 */



/* --------------------------------------------- Data Types/ Structures */
/**
 *  \defgroup scheduler_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 * Scheduler Server application Asynchronous Notification Callback.
 *
 * Scheduler Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_SCHEDULER_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;

/**
 * Scheduler Client application Asynchronous Notification Callback.
 *
 * Scheduler Client calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param handle        Model Handle.
 * \param opcode        Opcode.
 * \param data_param    Data associated with the event if any or NULL.
 * \param data_len      Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* MS_SCHEDULER_CLIENT_CB)
        (
            MS_ACCESS_MODEL_HANDLE * handle,
            UINT32                   opcode,
            UCHAR                  * data_param,
            UINT16                   data_len
        ) DECL_REENTRANT;
/** \} */

/**
 *  \defgroup scheduler_structures Structures
 *  \{
 */

/**
 * Scheduler Status message parameters.
 */
typedef struct MS_scheduler_status_struct
{
    /** Bit field indicating defined Actions in the Schedule Register */
    UINT16 schedules;

} MS_SCHEDULER_STATUS_STRUCT;

/**
 * Scheduler Action Get message parameters.
 */
typedef struct MS_scheduler_action_get_struct
{
    /** Index of the Schedule Register entry to get */
    UCHAR  index;

} MS_SCHEDULER_ACTION_GET_STRUCT;

/**
 * Scheduler Action Status message parameters.
 *
 * TODO: Merge with MS_scheduler_action_set_struct
 */
typedef struct MS_scheduler_action_status_struct
{
    /** Enumerates (selects) a Schedule Register entry */
    UCHAR  index;

    /** Scheduled year for the action */
    UINT8    year;

    /** Scheduled month for the action */
    UINT16   month;

    /** Scheduled day of the month for the action */
    UINT8    day;

    /** Scheduled hour for the action */
    UINT8    hour;

    /** Scheduled minute for the action */
    UINT8    minute;

    /** Scheduled second for the action */
    UINT8    second;

    /** Schedule days of the week for the action */
    UINT8    day_of_week;

    /** Action to be performed at the scheduled time */
    UINT8    action;

    /** Transition time for this action */
    UINT8    transition_time;

    /** Scene number to be used for some actions */
    UINT16   scene_number;
} MS_SCHEDULER_ACTION_STATUS_STRUCT;

/**
 * Scheduler Action Set message parameters.
 */
typedef struct MS_scheduler_action_set_struct
{
    /** Index of the Schedule Register entry to set */
    UCHAR  index;

    /** Scheduled year for the action */
    UINT8    year;

    /** Scheduled month for the action */
    UINT16   month;

    /** Scheduled day of the month for the action */
    UINT8    day;

    /** Scheduled hour for the action */
    UINT8    hour;

    /** Scheduled minute for the action */
    UINT8    minute;

    /** Scheduled second for the action */
    UINT8    second;

    /** Schedule days of the week for the action */
    UINT8    day_of_week;

    /** Action to be performed at the scheduled time */
    UINT8    action;

    /** Transition time for this action */
    UINT8    transition_time;

    /** Scene number to be used for some actions */
    UINT16   scene_number;

} MS_SCHEDULER_ACTION_SET_STRUCT;

/** \} */



/* --------------------------------------------- Function */
/**
 * \defgroup scheduler_api_defs API Definitions
 * \{
 * This section describes the EtherMind Mesh Scheduler Model APIs.
 */
/**
 * \defgroup scheduler_ser_api_defs Scheduler Server API Definitions
 * \{
 * This section describes the Scheduler Server APIs.
 */

/**
 *  \brief API to initialize Scheduler Server model
 *
 *  \par Description
 *  This is to initialize Scheduler Server model and to register with Acess layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] scheduler_model_handle
 *                   Model identifier associated with the Scheduler model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] scheduler_setup_model_handle
 *                   Model identifier associated with the Scheduler Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Scheduler Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_scheduler_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * scheduler_model_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * scheduler_setup_model_handle,
               /* IN */    MS_SCHEDULER_SERVER_CB      appl_cb
           );

/**
 *  \brief API to send reply or to update state change
 *
 *  \par Description
 *  This is to send reply for a request or to inform change in state.
 *
 * \param [in] ctx                     Context of the message.
 * \param [in] current_state_params    Model specific current state parameters.
 * \param [in] target_state_params     Model specific target state parameters (NULL: to be ignored).
 * \param [in] remaining_time          Time from current state to target state (0: to be ignored).
 * \param [in] ext_params              Additional parameters (NULL: to be ignored).
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_scheduler_server_state_update
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * target_state_params,
               /* IN */ UINT16                               remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS         * ext_params
           );
/** \} */

/**
 * \defgroup scheduler_cli_api_defs Scheduler Client API Definitions
 * \{
 * This section describes the Scheduler Client APIs.
 */

/**
 *  \brief API to initialize Scheduler Client model
 *
 *  \par Description
 *  This is to initialize Scheduler Client model and to register with Acess layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Scheduler Client.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_scheduler_client_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * model_handle,
               /* IN */    MS_SCHEDULER_CLIENT_CB appl_cb
           );

/**
 *  \brief API to get Scheduler client model handle
 *
 *  \par Description
 *  This is to get the handle of Scheduler client model.
 *
 *  \param [out] model_handle   Address of model handle to be filled/returned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_scheduler_client_get_model_handle
           (
               /* OUT */ MS_ACCESS_MODEL_HANDLE  * model_handle
           );

/**
 *  \brief API to send acknowledged commands
 *
 *  \par Description
 *  This is to initialize sending acknowledged commands.
 *
 *  \param [in] req_opcode    Request Opcode.
 *  \param [in] param         Parameter associated with Request Opcode.
 *  \param [in] rsp_opcode    Response Opcode.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_scheduler_client_send_reliable_pdu
           (
               /* IN */ UINT32    req_opcode,
               /* IN */ void    * param,
               /* IN */ UINT32    rsp_opcode
           );

/**
 *  \brief API to get the current Schedule Register state of an element.
 *
 *  \par Description
 *  Scheduler Get is an acknowledged message used to get the current Schedule Register state of an element.
 *  The response to the Scheduler Get message is a Scheduler Status message.
 *  There are no parameters for this message.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_scheduler_get() \
        MS_scheduler_client_send_reliable_pdu \
        (\
            MS_ACCESS_SCHEDULER_GET_OPCODE,\
            NULL,\
            MS_ACCESS_SCHEDULER_STATUS_OPCODE\
        )

/**
 *  \brief API to report the action defined by the entry of the Schedule Register state of an element,
 *  identified by the Index field.
 *
 *  \par Description
 *  Scheduler Action Get is an acknowledged message used to report the action defined by the entry
 *  of the Schedule Register state of an element, identified by the Index field.
 *  The response to the Scheduler Action Get message is a Scheduler Action Status message.
 *
 *  \param [in] param Scheduler Action Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_scheduler_action_get(param) \
        MS_scheduler_client_send_reliable_pdu \
        (\
            MS_ACCESS_SCHEDULER_ACTION_GET_OPCODE,\
            param,\
            MS_ACCESS_SCHEDULER_ACTION_STATUS_OPCODE\
        )

/**
 *  \brief API to set the entry of the Schedule Register state of an element.
 *
 *  \par Description
 *  Scheduler Action Set is an acknowledged message used to set the entry of the Schedule Register state of an element,
 *  identified by the Index field.
 *  The response to the Scheduler Action Set message is a Scheduler Action Status message.
 *
 *  \param [in] param Scheduler Action Set message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_scheduler_action_set(param) \
        MS_scheduler_client_send_reliable_pdu \
        (\
            MS_ACCESS_SCHEDULER_ACTION_SET_OPCODE,\
            param,\
            MS_ACCESS_SCHEDULER_ACTION_STATUS_OPCODE\
        )

/**
 *  \brief API to set the entry of the Schedule Register state of an element.
 *
 *  \par Description
 *  Scheduler Action Set Unacknowledged is an unacknowledged message used to set the entry of the Schedule Register state of an element,
 *  identified by the Index field.
 *
 *  \param [in] param Scheduler Action Set message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_scheduler_action_set_unacknowledged(param) \
        MS_scheduler_client_send_reliable_pdu \
        (\
            MS_ACCESS_SCHEDULER_ACTION_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )
/** \} */
/** \} */
/** \} */

#endif /*_H_MS_SCHEDULER_API_ */
