/**
 * \file MS_sensor_api.h
 *
 * \brief This file defines the Mesh Sensor Model Application Interface
 * - includes Data Structures and Methods for both Server and Client.
 */

/*
 * Copyright (C) 2017. Mindtree Ltd.
 * All rights reserved.
 */

#ifndef _H_MS_SENSOR_API_
#define _H_MS_SENSOR_API_


/* --------------------------------------------- Header File Inclusion */
#include "MS_access_api.h"


/* --------------------------------------------- Global Definitions */
/**
 * \defgroup sensor_module SENSOR (Mesh Sensor Model)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Mesh Sensor Model (SENSOR) module to the Application.
 */



/* --------------------------------------------- Data Types/ Structures */
/**
 *  \defgroup sensor_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 * Sensor Server application Asynchronous Notification Callback.
 *
 * Sensor Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_SENSOR_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;

/**
 * Sensor Setup Server application Asynchronous Notification Callback.
 *
 * Sensor Setup Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_SENSOR_SETUP_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;

/**
 * Sensor Client application Asynchronous Notification Callback.
 *
 * Sensor Client calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param handle        Model Handle.
 * \param opcode        Opcode.
 * \param data_param    Data associated with the event if any or NULL.
 * \param data_len      Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* MS_SENSOR_CLIENT_CB)
        (
            MS_ACCESS_MODEL_HANDLE * handle,
            UINT32                   opcode,
            UCHAR                  * data_param,
            UINT16                   data_len
        ) DECL_REENTRANT;
/** \} */

/**
 *  \defgroup sensor_structures Structures
 *  \{
 */

/**
 * Sensor Descriptor Get message parameters.
 */
typedef struct MS_sensor_descriptor_get_struct
{
    /** Property ID for the sensor (Optional) */
    UINT16 property_id;

    /** Flag: To represent if optional field Property ID is valid */
    UCHAR optional_fields_present;

} MS_SENSOR_DESCRIPTOR_GET_STRUCT;

/**
 * Sensor Descriptor Status message parameters.
 */
typedef struct MS_sensor_descriptor_status_struct
{
    /** Sequence of 8-octet Sensor Descriptors (Optional) */
    UCHAR * descriptor;

    /** Flag: To represent if optional field Descriptors are valid */
    UCHAR optional_fields_present;

} MS_SENSOR_DESCRIPTOR_STATUS_STRUCT;

/**
 * Sensor Get message parameters.
 */
typedef struct MS_sensor_get_struct
{
    /** Property for the sensor. */
    UINT16 property_id;

    /** Flag: To represent if optional field Property ID is valid */
    UCHAR optional_fields_present;

} MS_SENSOR_GET_STRUCT;

/**
 * Sensor Status message parameters.
 */
typedef struct MS_sensor_status_struct
{
    /** The Marshaled Sensor Data. */
    UCHAR *marshalled_sensor_data;

    /** The Marshaled Sensor Data Length . */
    UINT16 marshalled_sensor_data_len;

    /** Flag: To represent if optional field Marshaled Sensor Data is valid */
    UCHAR optional_fields_present;

} MS_SENSOR_STATUS_STRUCT;

/**
 * Sensor Column Get message parameters.
 */
typedef struct MS_sensor_column_get_struct
{
    /** Property identifying a sensor */
    UINT16 property_id;

    /** Raw value identifying a column */
    UCHAR *raw_value_x;

    /** Raw value length identifying a column */
    UINT16 raw_value_x_len;

} MS_SENSOR_COLUMN_GET_STRUCT;


/**
 * Sensor Column Status message parameters.
 */
typedef struct MS_sensor_column_status_struct
{
    /** Property identifying a sensor and the Y axis. */
    UINT16 property_id;

    /** Raw value representing the left corner of the column on the X axis. */
    UCHAR *raw_value_x;

    /** Raw value length representing the left corner of the column on the X axis. */
    UINT16 raw_value_x_len;

    /** Raw value representing the width of the column. (Optional) */
    UCHAR *column_width;

    /** Raw value length representing the width of the column. (Optional) */
    UINT16 column_width_len;

    /** Raw value representing the height of the column on the Y axis. */
    UCHAR *raw_value_y;

    /** Raw value length representing the height of the column on the Y axis. */
    UINT16 raw_value_y_len;

    /** Flag: To represent if optional fields Column Width and Raw Value Y are valid */
    UCHAR optional_fields_present;

} MS_SENSOR_COLUMN_STATUS_STRUCT;

/**
 * Sensor Series Get message parameters.
 */
typedef struct MS_sensor_series_get_struct
{
    /** Property identifying a sensor. */
    UINT16 property_id;

    /** Raw value identifying a starting column. (Optional) */
    UCHAR *raw_value_x1;

    /** Raw value length identifying a starting column. (Optional) */
    UINT16 raw_value_x1_len;

    /** Raw value identifying an ending column. */
    UCHAR *raw_value_x2;

    /** Raw value length identifying an ending column. */
    UINT16 raw_value_x2_len;

    /** Flag: To represent if optional fields Raw Value X1 and X2 are valid */
    UCHAR optional_fields_present;

} MS_SENSOR_SERIES_GET_STRUCT;

/**
 * Sensor Series Status message parameters.
 */
typedef struct MS_sensor_series_status_struct
{
    /** Property identifying a sensor and the Y axis. */
    UINT16 property_id;

    /** Raw value representing the left corner of the nth column on the X axis. */
    UCHAR *raw_value_x;

    /** Number of values in raw_value_x */
    UINT16 raw_value_x_len;

    /** Raw value representing the width of the nth column. */
    UCHAR *column_width;

    /** Number of values in column_width */
    UINT16 column_width_len;

    /** Raw value representing the height of the nth column on the Y axis. */
    UCHAR *raw_value_y;

    /** Number of values in raw_value_y */
    UINT16 raw_value_y_len;

} MS_SENSOR_SERIES_STATUS_STRUCT;

/**
 * Sensor Cadence Get message parameters.
 */
typedef struct MS_sensor_cadence_get_struct
{
    /** Property ID for the sensor. */
    UINT16 property_id;

} MS_SENSOR_CADENCE_GET_STRUCT;

/**
 * Sensor Cadence Set message parameters.
 */
typedef struct MS_sensor_cadence_set_struct
{
    /** Property ID for the sensor. */
    UINT16 property_id;

    /** Divisor for the Publish Period */
    UCHAR  fast_cadence_period_divisor;

    /** Defines the unit and format of the Status Trigger Delta fields. */
    UCHAR  status_trigger_type;

    /** Delta down value that triggers a status message. */
    UCHAR *status_trigger_delta_down;

    /** Delta down value length that triggers a status message. */
    UINT16 status_trigger_delta_down_len;

    /** Delta up value that triggers a status message. */
    UCHAR *status_trigger_delta_up;

    /** Delta up value length that triggers a status message. */
    UINT16 status_trigger_delta_up_len;

    /** Minimum interval between two consecutive Status messages. */
    UCHAR  status_min_interval;

    /** Low value for the fast cadence range. */
    UCHAR *fast_cadence_low;

    /** Low value length for the fast cadence range. */
    UINT16 fast_cadence_low_len;

    /** High value for the fast cadence range. */
    UCHAR *fast_cadence_high;

    /** High value length for the fast cadence range. */
    UINT16 fast_cadence_high_len;

} MS_SENSOR_CADENCE_SET_STRUCT;

/**
 * Sensor Cadence Status message parameters.
 */
typedef struct MS_sensor_cadence_status_struct
{
    /** Property ID for the sensor. */
    UINT16 property_id;

    /** Divisor for the Publish Period */
    UCHAR  fast_cadence_period_divisor;

    /** Defines the unit and format of the Status Trigger Delta fields. */
    UCHAR  status_trigger_type;

    /** Delta down value that triggers a status message. */
    UCHAR *status_trigger_delta_down;

    /** Delta down value length that triggers a status message. */
    UINT16 status_trigger_delta_down_len;

    /** Delta up value that triggers a status message. */
    UCHAR *status_trigger_delta_up;

    /** Delta up value length that triggers a status message. */
    UINT16 status_trigger_delta_up_len;

    /** Minimum interval between two consecutive Status messages. */
    UCHAR  status_min_interval;

    /** Low value for the fast cadence range. */
    UCHAR *fast_cadence_low;

    /** Low value length for the fast cadence range. */
    UINT16 fast_cadence_low_len;

    /** High value for the fast cadence range. */
    UCHAR *fast_cadence_high;

    /** High value length for the fast cadence range. */
    UINT16 fast_cadence_high_len;

} MS_SENSOR_CADENCE_STATUS_STRUCT;

/**
 * Sensor Settings Get message parameters.
 */
typedef struct MS_sensor_settings_get_struct
{
    /** Property ID identifying a sensor. */
    UINT16 sensor_property_id;

} MS_SENSOR_SETTINGS_GET_STRUCT;

/**
 * Sensor Setting Status message parameters.
 */
typedef struct MS_sensor_settings_status_struct
{
    /** Property ID identifying a sensor. */
    UINT16 sensor_property_id;

    /**
     * A sequence of N Sensor Setting Property IDs identifying settings within a sensor,
     * where N is the number of property IDs included in the message. (Optional)
     */
    UINT16 *sensor_setting_property_ids;

    /** Number of Sensor Setting Property IDs */
    UINT16 sensor_setting_property_ids_count;

} MS_SENSOR_SETTINGS_STATUS_STRUCT;

/**
 * Sensor Setting Get message parameters.
 */
typedef struct MS_sensor_setting_get_struct
{
    /** Property ID identifying a sensor. */
    UINT16 sensor_property_id;

    /** Setting Property ID identifying a setting within a sensor. */
    UINT16 sensor_setting_property_id;

} MS_SENSOR_SETTING_GET_STRUCT;

/**
 * Sensor Setting Set message parameters.
 */
typedef struct MS_sensor_setting_set_struct
{
    /** Property ID identifying a sensor. */
    UINT16 sensor_property_id;

    /** Setting ID identifying a setting within a sensor. */
    UINT16 sensor_setting_property_id;

    /** Raw value for the setting. */
    UCHAR *sensor_setting_raw;

    /** Raw value length for the setting. */
    UINT16 sensor_setting_raw_len;

} MS_SENSOR_SETTING_SET_STRUCT;

/**
 * Sensor Setting Status message parameters.
 */
typedef struct MS_sensor_setting_status_struct
{
    /** Property ID identifying a sensor. */
    UINT16 sensor_property_id;

    /** Setting ID identifying a setting within a sensor. */
    UINT16 sensor_setting_property_id;

    /** Read / Write access rights for the setting. (Optional) */
    UCHAR  sensor_setting_access;

    /** Raw value for the setting. */
    UCHAR *sensor_setting_raw;

    /** Raw value length for the setting. */
    UINT16 sensor_setting_raw_len;

    /** Flag: To represent if optional fields Access and Setting Raw are valid */
    UCHAR optional_fields_present;

} MS_SENSOR_SETTING_STATUS_STRUCT;

/** \} */



/* --------------------------------------------- Function */
/**
 * \defgroup sensor_api_defs API Definitions
 * \{
 * This section describes the EtherMind Mesh Sensor Model APIs.
 */
/**
 * \defgroup sensor_ser_api_defs Sensor Server API Definitions
 * \{
 * This section describes the Sensor Server APIs.
 */

/**
 *  \brief API to initialize Sensor Server model
 *
 *  \par Description
 *  This is to initialize Sensor Server model and to register with Acess layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Sensor Server.
 *
 *  \param [in] pub_cb     Publication Callback to be used for the Sensor Server application.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_sensor_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE              element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE              * model_handle,
               /* IN */    MS_SENSOR_SERVER_CB                   appl_cb,
               /* IN */    MS_ACCESS_MODEL_PUBLISH_TIMEOUT_CB    pub_cb
           );

/**
 *  \brief API to initialize Sensor_Setup Server model
 *
 *  \par Description
 *  This is to initialize Sensor_Setup Server model and to register with Acess layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Sensor_Setup Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_sensor_setup_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * model_handle,
               /* IN */    MS_SENSOR_SETUP_SERVER_CB appl_cb
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
API_RESULT MS_sensor_server_state_update
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * target_state_params,
               /* IN */ UINT16                               remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS         * ext_params
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
API_RESULT MS_sensor_setup_server_state_update
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * target_state_params,
               /* IN */ UINT16                               remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS         * ext_params
           );
/** \} */

/**
 * \defgroup sensor_cli_api_defs Sensor Client API Definitions
 * \{
 * This section describes the Sensor Client APIs.
 */

/**
 *  \brief API to initialize Sensor Client model
 *
 *  \par Description
 *  This is to initialize Sensor Client model and to register with Acess layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Sensor Client.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_sensor_client_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * model_handle,
               /* IN */    MS_SENSOR_CLIENT_CB appl_cb
           );

/**
 *  \brief API to get Sensor client model handle
 *
 *  \par Description
 *  This is to get the handle of Sensor client model.
 *
 *  \param [out] model_handle   Address of model handle to be filled/returned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_sensor_client_get_model_handle
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
API_RESULT MS_sensor_client_send_reliable_pdu
           (
               /* IN */ UINT32    req_opcode,
               /* IN */ void    * param,
               /* IN */ UINT32    rsp_opcode
           );

/**
 *  \brief API to get the Sensor Descriptor state of all sensors within an element.
 *
 *  \par Description
 *  Sensor Descriptor Get is an acknowledged message used to get the Sensor Descriptor state of all sensors within an element.
 *  The response to a Sensor Descriptor Get message is a Sensor Descriptor Status message.
 *
 *  \param [in] param Sensor Descriptor Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_descriptor_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_DESCRIPTOR_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_DESCRIPTOR_STATUS_OPCODE\
        )

/**
 *  \brief API to get the Sensor Data state.
 *
 *  \par Description
 *  Sensor Get is an acknowledged message used to get the Sensor Data state.
 *  The response to the Sensor Get message is a Sensor Status message.
 *
 *  \param [in] param Sensor Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_STATUS_OPCODE\
        )

/**
 *  \brief API to get the Sensor Series Column state.
 *
 *  \par Description
 *  Sensor Column Get is an acknowledged message used to get the Sensor Series Column state.
 *  The response to the Sensor Column Get message is a Sensor Column Status message
 *
 *  \param [in] param Sensor Column Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_column_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_COLUMN_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_COLUMN_STATUS_OPCODE\
        )

/**
 *  \brief API to get a sequence of the Sensor Series Column states.
 *
 *  \par Description
 *  Sensor Series Get is an acknowledged message used to get a sequence of the Sensor Series Column states.
 *  The response to the Sensor Series Get message is a Sensor Series Status message.
 *
 *  \param [in] param Sensor Series Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_series_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_SERIES_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_SERIES_STATUS_OPCODE\
        )

/**
 *  \brief API to get the Sensor Cadence state of an element.
 *
 *  \par Description
 *  Sensor Cadence Get is an acknowledged message used to get the Sensor Cadence state of an element.
 *  The response to the Sensor Cadence Get message is a Sensor Cadence Status message.
 *
 *  \param [in] param Sensor Cadence Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_cadence_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_CADENCE_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_CADENCE_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Sensor Cadence state of an element.
 *
 *  \par Description
 *  Sensor Cadence Set is an acknowledged message used to set the Sensor Cadence state of an element.
 *  The response to the Sensor Cadence Set message is a Sensor Cadence Status message.
 *
 *  \param [in] param Sensor Cadence Set message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_cadence_set(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_CADENCE_SET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_CADENCE_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Sensor Cadence state of an element.
 *
 *  \par Description
 *  Sensor Cadence Set Unacknowledged is an unacknowledged message used to set the Sensor Cadence state of an element.
 *
 *  \param [in] param Sensor Cadence Set message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_cadence_set_unacknowledged(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_CADENCE_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )

/**
 *  \brief API to get the list of Sensor Setting states of an element.
 *
 *  \par Description
 *  Sensor Settings Get is an acknowledged message used to get the list of Sensor Setting states of an element.
 *  The response to the Sensor Settings Get message is a Sensor Settings Status message
 *
 *  \param [in] param Sensor Settings Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_settings_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_SETTINGS_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_SETTINGS_STATUS_OPCODE\
        )

/**
 *  \brief API to get the Sensor Setting state of an element.
 *
 *  \par Description
 *  Sensor Setting Get is an acknowledged message used to get the Sensor Setting state of an element.
 *  The response to the Sensor Setting Get message is a Sensor Setting Status message.
 *
 *  \param [in] param Sensor Setting Get message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_setting_get(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_SETTING_GET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_SETTING_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Sensor Setting state of an element.
 *
 *  \par Description
 *  Sensor Setting Set is an acknowledged message used to set the Sensor Setting state of an element.
 *  The response to the Sensor Setting Set message is a Sensor Setting Status message.
 *
 *  \param [in] param Sensor Setting Set message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_setting_set(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_SETTING_SET_OPCODE,\
            param,\
            MS_ACCESS_SENSOR_SETTING_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Sensor Setting state of an element.
 *
 *  \par Description
 *  Sensor Setting Set Unacknowledged is an unacknowledged message used to set the Sensor Setting state of an element.
 *
 *  \param [in] param Sensor Setting Set message
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_sensor_setting_set_unacknowledged(param) \
        MS_sensor_client_send_reliable_pdu \
        (\
            MS_ACCESS_SENSOR_SETTING_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )
/** \} */
/** \} */
/** \} */

#endif /*_H_MS_SENSOR_API_ */
