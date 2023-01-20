/**
 * \file MS_light_lc_api.h
 *
 * \brief This file defines the Mesh Light LC Model Application Interface
 * - includes Data Structures and Methods for both Server and Client.
 */

/*
 * Copyright (C) 2017. Mindtree Ltd.
 * All rights reserved.
 */

#ifndef _H_MS_LIGHT_LC_API_
#define _H_MS_LIGHT_LC_API_


/* --------------------------------------------- Header File Inclusion */
#include "MS_access_api.h"


/* --------------------------------------------- Global Definitions */
/**
 * \defgroup light_lc_module Light LC Model (LIGHT_LC)
 * \ingroup lighting_models
 * \{
 *  \brief This section describes the interfaces & APIs offered by the EtherMind
 *  Mesh Light LC Model (LIGHT_LC) module to the Application.
 */

/* --------------------------------------------- Data Types/ Structures */
/**
 *  \defgroup light_lc_cb Application Callback
 *  \{
 *  \brief This section describes the Notification Callback Interfaces offered
 *  to the application by EtherMind Mesh Light LC Model Layer.
 */

/**
 * Light LC Server application Asynchronous Notification Callback.
 *
 * Light LC Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_LIGHT_LC_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;

#ifdef MS_MODEL_SERVER_EXTENDED_INTERFACE
/**
 * Light LC Setup Server application Asynchronous Notification Callback.
 *
 * Light LC Setup Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_LIGHT_LC_SETUP_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;
#endif /* MS_MODEL_SERVER_EXTENDED_INTERFACE */

#ifdef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \brief Light LC Server State Machine Asynchronous Notification Callback.
 *
 *  \par Description
 *  Light LC Server State Machine calls this registered callback to indicate
 *  the current State of the Light LC State Machine. The application then can
 *  use the information present here to control any other associated state such
 *  as Lightness Out etc. The application can also choose to decide and Publish
 *  any state changes that might occur as resultant.
 *
 *  \param [in] handle        Model Handle.
 *  \param [in] event_type    Light LC Server Event Types.
 *  \param [in] event_param   Parameter associated with the event if any or NULL.
 *  \param [in] event_length  Size of the event parameter data. 0 if event param is NULL.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
typedef API_RESULT (* MS_LIGHT_LC_SERVER_SM_APPL_CB)
        (
            MS_ACCESS_MODEL_HANDLE * handle,
            UINT8                    event_type,
            void                   * event_param,
            UINT16                   event_length
        ) DECL_REENTRANT;
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

/**
 * Light LC Client application Asynchronous Notification Callback.
 *
 * Light LC Client calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] opcode        Opcode.
 * \param [in] data_param    Data associated with the event if any or NULL.
 * \param [in] data_len      Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* MS_LIGHT_LC_CLIENT_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT * ctx,
            UINT32                   opcode,
            UCHAR                  * data_param,
            UINT16                   data_len
        ) DECL_REENTRANT;
/** \} */

/**
 * \defgroup light_lc_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * Mesh Light LC Model Layer.
 */

/**
 *  \defgroup light_lc_structures Structures
 *  \{
 *  \brief This section describes the various Data-Types and Structures in
 *  EtherMind Mesh Light LC Mode Layer.
 */

/**
 * Light LC Mode Set/Status message parameters.
 */
typedef struct MS_light_lc_mode_struct
{
    /** The target value of the Light LC Mode state */
    UCHAR  mode;

} MS_LIGHT_LC_MODE_STRUCT;

/**
 * Light LC Occupancy Mode Set/Status message parameters.
 */
typedef struct MS_light_lc_om_struct
{
    /** The target value of the Light LC Occupancy Mode state */
    UCHAR  mode;

} MS_LIGHT_LC_OM_STRUCT;

/**
 * Light LC Light OnOff Set message parameters.
 */
typedef struct MS_light_lc_light_onoff_set_struct
{
    /** The target value of the Light LC Light OnOff state */
    UCHAR  light_onoff;

    /** Transaction Identifier */
    UCHAR  tid;

    /**
     * Transition Time is a 1-octet value that consists of two fields:
     * - a 2-bit bit field representing the step resolution
     * - a 6-bit bit field representing the number of transition steps.
     *
     * Field                      | Size (bits) | Description
     * ---------------------------|-------------|----------------
     * Transition Number of Steps | 6           | The number of Steps
     * Transition Step Resolution | 2           | The resolution of the Default Transition
     *                                          | Number of Steps field
     */
    UCHAR  transition_time;

    /** Message execution delay in 5 milliseconds steps */
    UCHAR  delay;

    /** Flag: To represent if optional Transaction time and Delay fields are valid */
    UCHAR optional_fields_present;

} MS_LIGHT_LC_LIGHT_ONOFF_SET_STRUCT;

/**
 * Light LC Light OnOff Status message parameters.
 */
typedef struct MS_light_lc_light_onoff_status_struct
{
    /** The present value of the Light LC Light OnOff state */
    UCHAR  present_light_onoff;

    /** The target value of the Light LC Light OnOff state (Optional) */
    UCHAR  target_light_onoff;

    /**
     * Remaining Time is a 1-octet value that consists of two fields:
     * - a 2-bit bit field representing the step resolution
     * - a 6-bit bit field representing the number of transition steps.
     *
     * Field                      | Size (bits) | Description
     * ---------------------------|-------------|----------------
     * Transition Number of Steps | 6           | The number of Steps
     * Transition Step Resolution | 2           | The resolution of the Default Transition
     *                                          | Number of Steps field
     */
    UCHAR  remaining_time;

    /** Flag: To represent if optional fields Target LC Light OnOff and Remaining Time are valid */
    UCHAR optional_fields_present;

} MS_LIGHT_LC_LIGHT_ONOFF_STATUS_STRUCT;

/**
 * Light LC Property Get message parameters.
 */
typedef struct MS_light_lc_property_get_struct
{
    /** Property ID identifying a Light LC Property. */
    UINT16 light_lc_property_id;

} MS_LIGHT_LC_PROPERTY_GET_STRUCT;

/**
 * Light LC Property Set message parameters.
 */
typedef struct MS_light_lc_property_set_struct
{
    /** Property ID identifying a Light LC Property. */
    UINT16 light_lc_property_id;

    /** Raw value for the Light LC Property */
    UCHAR *light_lc_property_value;

    /** Raw value length for the Light LC Property */
    UINT16 light_lc_property_value_len;

} MS_LIGHT_LC_PROPERTY_SET_STRUCT;

/**
 * Light LC Property Status message parameters.
 */
typedef struct MS_light_lc_property_status_struct
{
    /** Property ID identifying a Light LC Property. */
    UINT16 light_lc_property_id;

    /** Raw value for the Light LC Property */
    UCHAR *light_lc_property_value;

    /** Raw value length for the Light LC Property */
    UINT16 light_lc_property_value_len;

} MS_LIGHT_LC_PROPERTY_STATUS_STRUCT;

#ifdef MS_LIGHT_LC_SERVER_INTERNAL_FSM

/**
 * Light LC Server State Info
 */
typedef struct MS_light_lc_server_state_info_struct
{
    /** Light LC Server Current State */
    UINT32                                state;

    /** Remaining Time in current state */
    UINT32                                lc_rem_time_in_ms;

    /** Light LC OM Value */
    UCHAR                                 lc_om;

    /** Light LC Mode Value */
    UCHAR                                 lc_mode;

    /** Current Light LC ONOFF Value */
    UCHAR                                 lc_present_onoff;

    /** Target Light LC ONOFF Value */
    UCHAR                                 lc_target_onoff;

} MS_LIGHT_LC_SERVER_STATE_INFO;

#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

/** \} */

#ifdef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \defgroup light_lc_events Events
 *  \{
 *  \brief This section lists the Asynchronous Events notified to Application
 *  by EtherMind Mesh Light LC Server Model Layer.
 */

/**
 * \name Light LC Server Event Types
 * \{
 */
/** Light LC Server Event - LC_OFF */
#define MS_LIGHT_LC_SERVER_EVENT_LC_OFF                               0x01

/** Light LC Server Event - LC_STANDBY */
#define MS_LIGHT_LC_SERVER_EVENT_LC_STANDBY                           0x02

/** Light LC Server Event - LC_FADE_ON */
#define MS_LIGHT_LC_SERVER_EVENT_LC_FADE_ON                           0x03

/** Light LC Server Event - LC_RUN */
#define MS_LIGHT_LC_SERVER_EVENT_LC_RUN                               0x04

/** Light LC Server Event - LC_FADE */
#define MS_LIGHT_LC_SERVER_EVENT_LC_FADE                              0x05

/** Light LC Server Event - LC_PROLONG */
#define MS_LIGHT_LC_SERVER_EVENT_LC_PROLONG                           0x06

/** Light LC Server Event - LC_FADE_STANDBY_AUTO */
#define MS_LIGHT_LC_SERVER_EVENT_LC_FADE_STANDBY_AUTO                 0x07

/** Light LC Server Event - LC_FADE_STANDBY_MANUAL */
#define MS_LIGHT_LC_SERVER_EVENT_LC_FADE_STANDBY_MANUAL               0x08

/** \} */

/** \} */

/**
 *  \defgroup light_lc_states States
 *  \{
 *  \brief This section lists the States that are informed to Application
 *  by EtherMind Mesh Light LC Server Model Layer. The Application
 *  can also use these states to directly drive the EtherMind Mesh Light
 *  LC Server Model State Machine in scenarios such as Scene Recall/Restore
 *  or Power-Up Behaviour.
 */

/**
 * \name Light LC Server States
 * \{
 */
/** Light LC Server State - LC_OFF */
#define MS_LIGHT_LC_SERVER_STATE_LC_OFF                               0x00000001

/** Light LC Server State - LC_STANDBY */
#define MS_LIGHT_LC_SERVER_STATE_LC_STANDBY                           0x00000002

/** Light LC Server State - LC_FADE_ON */
#define MS_LIGHT_LC_SERVER_STATE_LC_FADE_ON                           0x00000004

/** Light LC Server State - LC_RUN */
#define MS_LIGHT_LC_SERVER_STATE_LC_RUN                               0x00000008

/** Light LC Server State - LC_FADE */
#define MS_LIGHT_LC_SERVER_STATE_LC_FADE                              0x00000010

/** Light LC Server State - LC_PROLONG */
#define MS_LIGHT_LC_SERVER_STATE_LC_PROLONG                           0x00000020

/** Light LC Server State - LC_FADE_STANDBY_AUTO */
#define MS_LIGHT_LC_SERVER_STATE_LC_FADE_STANDBY_AUTO                 0x00000040

/** Light LC Server State - LC_FADE_STANDBY_MANUAL */
#define MS_LIGHT_LC_SERVER_STATE_LC_FADE_STANDBY_MANUAL               0x00000080

/** \} */

/** \} */
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

/** \} */

/* --------------------------------------------- Function */
/**
 * \defgroup light_lc_api_defs API Definitions
 * \{
 * \brief This section describes the various APIs exposed by
 * EtherMind Mesh Light LC Model Layer to the Application.
 */

/**
 * \defgroup light_lc_ser_api_defs Light LC Server API Definitions
 * \{
 * \brief This section describes the EtherMind Mesh Light LC Server Model APIs.
 */

/**
 * \name Light LC Server Interfaces
 * \{
 */

#ifndef MS_MODEL_SERVER_EXTENDED_INTERFACE
#ifndef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \brief API to initialize Light_LC Server model
 *
 *  \par Description
 *  This is to initialize Light_LC Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] lc_model_handle
 *                   Model identifier associated with the Light LC model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] lc_setup_model_handle
 *                   Model identifier associated with the Light LC Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] lc_appl_cb    Application Callback to be used by the Light_LC Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE        element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_model_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_setup_model_handle,
               /* IN */    MS_LIGHT_LC_SERVER_CB           lc_appl_cb
           );
#else /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */
/**
 *  \brief API to initialize Light_LC Server model
 *
 *  \par Description
 *  This is to initialize Light_LC Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] lc_model_handle
 *                   Model identifier associated with the Light LC model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] lc_setup_model_handle
 *                   Model identifier associated with the Light LC Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] lc_appl_cb    Application Callback to be used by the Light_LC Server.
 *
 *  \param [in] lc_sm_appl_cb    Application Callback to be used by the Light_LC Server to inform about the LC State Machine Events.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE        element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_model_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_setup_model_handle,
               /* IN */    MS_LIGHT_LC_SERVER_CB           lc_appl_cb,
               /* IN */    MS_LIGHT_LC_SERVER_SM_APPL_CB   lc_sm_appl_cb
           );
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

#else /* MS_MODEL_SERVER_EXTENDED_INTERFACE */

#ifndef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \brief API to initialize Light_LC Server model
 *
 *  \par Description
 *  This is to initialize Light_LC Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] lc_model_handle
 *                   Model identifier associated with the Light LC model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] lc_setup_model_handle
 *                   Model identifier associated with the Light LC Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] lc_appl_cb    Application Callback to be used by the Light_LC Server.
 *
 *  \param [in] lc_setup_appl_cb    Application Callback to be used by the Light_LC_Setup Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_init_ext
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE        element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_model_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_setup_model_handle,
               /* IN */    MS_LIGHT_LC_SERVER_CB           lc_appl_cb,
               /* IN */    MS_LIGHT_LC_SETUP_SERVER_CB     lc_setup_appl_cb
           );
#else /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */
/**
 *  \brief API to initialize Light_LC Server model
 *
 *  \par Description
 *  This is to initialize Light_LC Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] lc_model_handle
 *                   Model identifier associated with the Light LC model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] lc_setup_model_handle
 *                   Model identifier associated with the Light LC Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] lc_appl_cb    Application Callback to be used by the Light_LC Server.
 *
 *  \param [in] lc_setup_appl_cb    Application Callback to be used by the Light_LC_Setup Server.
 *
 *  \param [in] lc_sm_appl_cb    Application Callback to be used by the Light_LC Server to inform about the LC State Machine Events.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_init_ext
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE        element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_model_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE        * lc_setup_model_handle,
               /* IN */    MS_LIGHT_LC_SERVER_CB           lc_appl_cb,
               /* IN */    MS_LIGHT_LC_SETUP_SERVER_CB     lc_setup_appl_cb,
               /* IN */    MS_LIGHT_LC_SERVER_SM_APPL_CB   lc_sm_appl_cb
           );
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */
#endif /* MS_MODEL_SERVER_EXTENDED_INTERFACE */

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
 * \param [in] reply                   If unicast response to be sent
 * \param [in] publish                 If state to be published
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#ifndef MS_MODEL_SERVER_EXTENDED_INTERFACE
API_RESULT MS_light_lc_server_state_update
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS    * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS    * target_state_params,
               /* IN */ UINT16                            remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS      * ext_params,
               /* IN */ UCHAR                             reply,
               /* IN */ UCHAR                             publish
           );
#else /* MS_MODEL_SERVER_EXTENDED_INTERFACE */
API_RESULT MS_light_lc_server_state_update_ext
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS    * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS    * target_state_params,
               /* IN */ UINT16                            remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS      * ext_params,
               /* IN */ UCHAR                             reply,
               /* IN */ UCHAR                             publish
           );
#endif /* MS_MODEL_SERVER_EXTENDED_INTERFACE */

#ifdef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \brief To De-initialize Light LC Server Model
 *
 *  \par Description
 *  This routine is a de-initialize interface for Light LC Server Model.
 *  This needs to be invoked by the upper layer prior to invoking other core
 *  Shutdown related interfaces. This interface will move the Light LC Server
 *  Module back to its uninitialized state.
 *
 *  \return None
 */
void MS_light_lc_server_deinit(void);

/**
 *  \brief API to set Light LC Sever Specific Time Properties
 *
 *  \par Description
 *  This is to update the Light LC Module with any Light Control Time
 *  Properties values. Typically these LC Time properties will be needed
 *  to be updated or informed to the Light LC Module during Power-Up,
 *  Scene Recall/Restore related scenario or when ever the Local application
 *  desires to enforce an update in these device properties values.
 *  Currently this interface handles only Time related Device Properties of
 *  Light LC Module like:
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_OCCUPANCY_DELAY
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE_ON
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_RUN_ON
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_PROLONG
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO
 *  \ref MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL
 *
 * \param [in] lc_model_handle Identifier for LC Server Model
 * \param [in] property_id     LC Server Device Property ID
 * \param [in] time_in_ms      Related Device Property Value,
 *                             time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_set_time_property
           (
               /* IN */ MS_ACCESS_MODEL_HANDLE * lc_model_handle,
               /* IN */ UINT32                   property_id,
               /* IN */ UINT32                   time_in_ms
           );

/**
 *  \brief API to locally trigger Light LC Sever state machine
 *
 *  \par Description
 *  This is to locally trigger the Light LC Server state machine from the
 *  application. This typically is used in Power-Up or Scene Recall/Restore
 *  Scenario where the Application could trigger the Light LC Server from
 *  any desired LC State.
 *
 * \param [in] lc_model_handle Identifier for LC Server Model
 * \param [in] lc_info reference of \ref MS_LIGHT_LC_SERVER_STATE_INFO where
 *             Light LC Server State Informations are present.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_set_lc_state_info
           (
               /* IN */ MS_ACCESS_MODEL_HANDLE        * lc_model_handle,
               /* IN */ MS_LIGHT_LC_SERVER_STATE_INFO * lc_info
           );

/**
 *  \brief API to locally fetch the current states of the Light LC Sever state
 *  machine
 *
 *  \par Description
 *  This is to locally fetch the Light LC Server state machine related
 *  information from the Light LC Server Module. This typically is used in
 *  Power-Up or Scene Recall/Restore Scenario where the Application uses this
 *  to fetch and persistently store the Light LC State related details.
 *
 * \param [in]  lc_model_handle Identifier for LC Server Model
 * \param [out] lc_info reference of \ref MS_LIGHT_LC_SERVER_STATE_INFO where
 *              Light LC Server State Informations are returned.
 *
 * \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_get_lc_state_info
           (
               /* IN */  MS_ACCESS_MODEL_HANDLE        * lc_model_handle,
               /* OUT */ MS_LIGHT_LC_SERVER_STATE_INFO * lc_info
           );

/**
 *  \brief API to locally report occupancy to Light LC Sever Model
 *
 *  \par Description
 *  This is to locally report any occupancy to Light LC Server Model.
 *
 * \param [in]  lc_model_handle Identifier for LC Server Model
 *
 * \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_report_occupancy
           (
               /* IN */ MS_ACCESS_MODEL_HANDLE        * lc_model_handle
           );

/**
 *  \brief API to locally report Light ON or OFF to Light LC Sever Model
 *
 *  \par Description
 *  This is to locally report any Light ON or OFF to Light LC Server Model.
 *  There could be many triggering factors to this Light related ON/OFF events
 *  from the upper layer. This API could be used for the following scenarios
 *  1. Manual/Physical trigger of Light ON/OFF
 *  2. State Binding with Generic ON/OFF
 *  3. Trigger of Light ON/OFF due to Ambient Light Sensor values
 *
 * \param [in] lc_model_handle Identifier for LC Server Model
 * \param [in] onoff Depicts the desired state value of Light LC ONOFF
 *
 * \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_server_report_light_onoff
           (
               /* IN */ MS_ACCESS_MODEL_HANDLE * lc_model_handle,
               /* IN */ UINT8                  onoff
           );
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

/** \} */

/** \} */

/**
 * \defgroup light_lc_cli_api_defs Light LC Client API Definitions
 * \{
 * \brief This section describes the EtherMind Mesh Light LC Client Model APIs.
 */

/**
 * \name Light LC Client Interfaces
 * \{
 */

/**
 *  \brief API to initialize Light_LC Client model
 *
 *  \par Description
 *  This is to initialize Light_LC Client model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Light_LC Client.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_client_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE   element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE   * model_handle,
               /* IN */    MS_LIGHT_LC_CLIENT_CB      appl_cb
           );

/**
 *  \brief API to get Light_LC client model handle
 *
 *  \par Description
 *  This is to get the handle of Light_LC client model.
 *
 *  \param [out] model_handle   Address of model handle to be filled/returned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_client_get_model_handle
           (
               /* OUT */ MS_ACCESS_MODEL_HANDLE  * model_handle
           );

/**
 *  \brief API to set Light_LC client model handle
 *
 *  \par Description
 *  This is to set the handle of Light_LC client model.
 *
 *  \param [in] model_handle   Model handle to be assigned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_light_lc_client_set_model_handle
           (
               /* IN */ MS_ACCESS_MODEL_HANDLE  model_handle
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
API_RESULT MS_light_lc_client_send_reliable_pdu
           (
               /* IN */ UINT32    req_opcode,
               /* IN */ void    * param,
               /* IN */ UINT32    rsp_opcode
           );
/** \} */

/** \} */

/** \} */

/**
 * \addtogroup light_lc_defines
 * \{
 */

/**
 * \defgroup light_lc_marcos Utility Macros
 * \{
 * \brief This section describes the various Utility Macros in EtherMind
 * Mesh Light LC Model Layer.
 */

/**
 * \name Light LC Client Macros
 * \{
 */

/**
 *  \brief API to get the Light LC Mode state of an element.
 *
 *  \par Description
 *  Light LC Mode Get is an acknowledged message used to get the Light LC Mode state of an element.
 *  The response to the Light LC Mode Get message is a Light LC Mode Status message.
 *  There are no parameters for this message.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_mode_get() \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_MODE_GET_OPCODE,\
            NULL,\
            MS_ACCESS_LIGHT_LC_MODE_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Mode state of an element.
 *
 *  \par Description
 *  The Light LC Mode Set is an acknowledged message used to set the Light LC Mode state of an element.
 *  The response to the Light LC Mode Set message is a Light LC Mode Status message.
 *
 *  \param [in] param Light LC Mode Set message parameter \ref MS_LIGHT_LC_MODE_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_mode_set(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_MODE_SET_OPCODE,\
            param,\
            MS_ACCESS_LIGHT_LC_MODE_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Mode state of an element.
 *
 *  \par Description
 *  The Light LC Mode Set Unacknowledged is an unacknowledged message used to
 *  set the Light LC Mode state of an element.
 *
 *  \param [in] param Light LC Mode Set message parameter \ref MS_LIGHT_LC_MODE_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_mode_set_unacknowledged(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_MODE_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )

/**
 *  \brief API to get the Light LC Occupancy Mode state of an element.
 *
 *  \par Description
 *  Light LC OM Get is an acknowledged message used to get the Light LC Occupancy Mode state of an element.
 *  The response to the Light LC OM Get message is a Light LC OM Status message.
 *  There are no parameters for this message.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_om_get() \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_OM_GET_OPCODE,\
            NULL,\
            MS_ACCESS_LIGHT_LC_OM_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Occupancy Mode state of an element.
 *
 *  \par Description
 *  The Light LC OM Set is an acknowledged message used to set the Light LC Occupancy Mode state of an element.
 *  The response to the Light LC OM Set message is a Light LC OM Status message.
 *
 *  \param [in] param Light LC OM Set message parameter \ref MS_LIGHT_LC_OM_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_om_set(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_OM_SET_OPCODE,\
            param,\
            MS_ACCESS_LIGHT_LC_OM_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Occupancy Mode state of an element.
 *
 *  \par Description
 *  The Light LC OM Set Unacknowledged is an unacknowledged message used to set
 *  the Light LC Occupancy Mode state of an element.
 *
 *  \param [in] param Light LC OM Set message parameter \ref MS_LIGHT_LC_OM_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_om_set_unacknowledged(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_OM_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )

/**
 *  \brief API to get the Light LC Light OnOff state of an element.
 *
 *  \par Description
 *  Light LC Light OnOff Get is an acknowledged message used to get the Light LC Light OnOff state of an element.
 *  The response to the Light LC Light OnOff Get message is a Light LC Light OnOff Status message.
 *  There are no parameters for this message.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_light_onoff_get() \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_LIGHT_ONOFF_GET_OPCODE,\
            NULL,\
            MS_ACCESS_LIGHT_LC_LIGHT_ONOFF_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Light OnOff state of an element.
 *
 *  \par Description
 *  The Light LC Light OnOff Set is an acknowledged message used to set the Light LC Light OnOff state of an element.
 *  The response to the Light LC Light OnOff Set message is a Light LC Light OnOff Status message.
 *
 *  \param [in] param Light LC Light OnOff Set message parameter \ref MS_LIGHT_LC_LIGHT_ONOFF_SET_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_light_onoff_set(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_LIGHT_ONOFF_SET_OPCODE,\
            param,\
            MS_ACCESS_LIGHT_LC_LIGHT_ONOFF_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Light OnOff state of an element.
 *
 *  \par Description
 *  The Light LC Light OnOff Set Unacknowledged is an unacknowledged message
 *  used to set the Light LC Light OnOff state of an element.
 *
 *  \param [in] param Light LC Light OnOff Set message parameter \ref MS_LIGHT_LC_LIGHT_ONOFF_SET_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_light_onoff_set_unacknowledged(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_LIGHT_ONOFF_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )

/**
 *  \brief API to to get the Light LC Property state of an element.
 *
 *  \par Description
 *  Light LC Property Get is an acknowledged message used to get the Light LC Property state of an element.
 *  The response to the Light LC Property Get message is a Light LC Property Status message.
 *
 *  \param [in] param Light LC Property Get message parameter \ref MS_LIGHT_LC_PROPERTY_GET_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_property_get(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_PROPERTY_GET_OPCODE,\
            param,\
            MS_ACCESS_LIGHT_LC_PROPERTY_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Property state of an element.
 *
 *  \par Description
 *  The Light LC Property Set is an acknowledged message used to set the Light LC Property state of an element.
 *  The response to the Light LC Property Set message is a Light LC Property Status message.
 *
 *  \param [in] param Light LC Property Set message parameter \ref MS_LIGHT_LC_PROPERTY_SET_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_property_set(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_PROPERTY_SET_OPCODE,\
            param,\
            MS_ACCESS_LIGHT_LC_PROPERTY_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Light LC Property state of an element.
 *
 *  \par Description
 *  The Light LC Property Set Unacknowledged is an unacknowledged message used
 *  to set the Light LC Property state of an element.
 *
 *  \param [in] param Light LC Property Set message parameter \ref MS_LIGHT_LC_PROPERTY_SET_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_property_set_unacknowledged(param) \
        MS_light_lc_client_send_reliable_pdu \
        (\
            MS_ACCESS_LIGHT_LC_PROPERTY_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )
/** \} */


/**
 * \name Light LC Server and Light LC setup Server Macros
 * \{
 */

#ifdef MS_MODEL_SERVER_EXTENDED_INTERFACE
#ifndef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \brief API to initialize Light_LC Server model
 *
 *  \par Description
 *  This is to initialize Light_LC Server model and to register with Access layer.
 *
 *  \param [in] eh
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] mh
 *                   Model identifier associated with the Light LC model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] smh
 *                   Model identifier associated with the Light LC Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] cb   Application Callback to be used by the Light_LC Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_init(eh,mh,smh,cb) \
        MS_light_lc_server_init_ext \
        (\
            (eh),\
            (mh),\
            (smh),\
            (cb),\
            (cb)\
        )
#else /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */
/**
 *  \brief API to initialize Light_LC Server model
 *
 *  \par Description
 *  This is to initialize Light_LC Server model and to register with Access layer.
 *
 *  \param [in] eh Element identifier to be associated with the model instance.
 *
 *  \param [in, out] mh
 *                   Model identifier associated with the Light LC model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] smh
 *                   Model identifier associated with the Light LC Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] cb Application Callback to be used by the Light_LC Server.
 *
 *  \param [in] smcb Application Callback to be used by the Light_LC Server to inform about the LC State Machine Events.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_init(eh,mh,smh,cb,smcb) \
        MS_light_lc_server_init_ext \
        (\
            (eh),\
            (mh),\
            (smh),\
            (cb),\
            (cb),\
            (smcb)\
        )
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

/**
 *  \brief API to send reply or to update state change
 *
 *  \par Description
 *  This is to send reply for a request or to inform change in state.
 *
 * \param [in] c   Context of the message.
 * \param [in] cs  Model specific current state parameters.
 * \param [in] ts  Model specific target state parameters (NULL: to be ignored).
 * \param [in] rt  Time from current state to target state (0: to be ignored).
 * \param [in] ex  Additional parameters (NULL: to be ignored).
 * \param [in] r   If unicast response to be sent
 * \param [in] p   If state to be published
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_state_update(c,cs,ts,rt,ex,r,p) \
        MS_light_lc_server_state_update_ext \
        (\
            (c),\
            (cs),\
            (ts),\
            (rt),\
            (ex),\
            (r),\
            (p)\
        )
#endif /* MS_MODEL_SERVER_EXTENDED_INTERFACE */

#ifdef MS_LIGHT_LC_SERVER_INTERNAL_FSM
/**
 *  \brief API to set the Light LC Time Fade Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Fade Property for a given instance of
 *  Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_fade_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE,\
            (t_ms)\
        )

/**
 *  \brief API to set the Light LC Time Fade On Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Fade On Property for a given instance of
 *  Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_fade_on_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE_ON,\
            (t_ms)\
        )

/**
 *  \brief API to set the Light LC Time Fade Standby Auto Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Fade Standby Auto Property for a given
 *  instance of Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_fade_standby_auto_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO,\
            (t_ms)\
        )

/**
 *  \brief API to set the Light LC Time Fade Standby Manual Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Fade Standby Manual Property for a given
 *  instance of Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_fade_standby_manual_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL,\
            (t_ms)\
        )

/**
 *  \brief API to set the Light LC Time Occupancy Delay Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Occupancy Delay Property for a given
 *  instance of Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_occupancy_delay_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_OCCUPANCY_DELAY,\
            (t_ms)\
        )

/**
 *  \brief API to set the Light LC Time Prolong Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Prolong Property for a given
 *  instance of Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_prolong_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_PROLONG,\
            (t_ms)\
        )

/**
 *  \brief API to set the Light LC Time Run On Property value.
 *
 *  \par Description
 *  This updates the Light LC Time Run On Property for a given
 *  instance of Light LC Server.
 *
 * \param [in] mh    Identifier for LC Server Model
 * \param [in] t_ms  Related Device Property Value,
 *                   time in milliseconds
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_set_run_on_time(mh,t_ms) \
        MS_light_lc_server_set_time_property \
        (\
            (mh),\
            MS_DEV_PROP_LIGHT_CONTROL_TIME_RUN_ON,\
            (t_ms)\
        )

/**
 *  \brief API to locally report Light ON to Light LC Sever Model
 *
 *  \par Description
 *  This is to locally report any Light ON to Light LC Server Model.
 *
 * \param [in] mh Identifier for LC Server Model
 *
 * \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_report_light_on(mh) \
        MS_light_lc_server_report_light_onoff \
        (\
            (mh),\
            MS_TRUE\
        )

/**
 *  \brief API to locally report Light OFF to Light LC Sever Model
 *
 *  \par Description
 *  This is to locally report any Light OFF to Light LC Server Model.
 *
 * \param [in] mh Identifier for LC Server Model
 *
 * \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_light_lc_server_report_light_off(mh) \
        MS_light_lc_server_report_light_onoff \
        (\
            (mh),\
            MS_TRUE\
        )
#endif /* MS_LIGHT_LC_SERVER_INTERNAL_FSM */

/** \} */

/** \} */

/** \} */

/** \} */

#endif /*_H_MS_LIGHT_LC_API_ */

