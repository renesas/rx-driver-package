/**
 * \file MS_generic_power_onoff_api.h
 *
 * \brief This file defines the Mesh Generic Power OnOff Model Application Interface
 * - includes Data Structures and Methods for both Server and Client.
 */

/*
 * Copyright (C) 2017. Mindtree Ltd.
 * All rights reserved.
 */

#ifndef _H_MS_GENERIC_POWER_ONOFF_API_
#define _H_MS_GENERIC_POWER_ONOFF_API_


/* --------------------------------------------- Header File Inclusion */
#include "MS_access_api.h"


/* --------------------------------------------- Global Definitions */
/**
 * \defgroup generic_power_onoff_module Generic Power OnOff Model (GENERIC_POWER_ONOFF)
 * \ingroup generics_models
 * \{
 *  \brief This section describes the interfaces & APIs offered by the EtherMind
 *  Mesh Generic Power OfOff Model (GENERIC_POWER_ONOFF) module to the Application.
 */

/* --------------------------------------------- Data Types/ Structures */
/**
 *  \defgroup generic_power_onoff_cb Application Callback
 *  \{
 *  \brief This section describes the Notification Callback Interfaces offered
 *  to the application by EtherMind Mesh Generic Power OfOff Model Layer.
 */

/**
 * Generic Power OnOff Server application Asynchronous Notification Callback.
 *
 * Generic Power OnOff Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_GENERIC_POWER_ONOFF_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;

/**
 * Generic Power OnOff Client application Asynchronous Notification Callback.
 *
 * Generic Power OnOff Client calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] opcode        Opcode.
 * \param [in] data_param    Data associated with the event if any or NULL.
 * \param [in] data_len      Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* MS_GENERIC_POWER_ONOFF_CLIENT_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT * ctx,
            UINT32                   opcode,
            UCHAR                  * data_param,
            UINT16                   data_len
        ) DECL_REENTRANT;

/**
 * Generic Power OnOff Setup Server application Asynchronous Notification Callback.
 *
 * Generic Power OnOff Setup Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * \param [in] ctx           Context of the message received for a specific model instance.
 * \param [in] msg_raw       Uninterpreted/raw received message.
 * \param [in] req_type      Requested message type.
 * \param [in] state_params  Model specific state parameters.
 * \param [in] ext_params    Additional parameters.
 */
typedef API_RESULT (* MS_GENERIC_POWER_ONOFF_SETUP_SERVER_CB)
        (
            MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
            MS_ACCESS_MODEL_REQ_MSG_RAW        * msg_raw,
            MS_ACCESS_MODEL_REQ_MSG_T          * req_type,
            MS_ACCESS_MODEL_STATE_PARAMS       * state_params,
            MS_ACCESS_MODEL_EXT_PARAMS         * ext_params

        ) DECL_REENTRANT;
/** \} */

/**
 * \defgroup generic_power_onoff_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind
 * Mesh Generic Power OnOff Model Layer.
 */

/**
 *  \defgroup generic_power_onoff_structures Structures
 *  \{
 *  \brief This section describes the various Data-Types and Structures in
 *  EtherMind Mesh Generic Power OnOff Model Layer.
 */

/**
 * Generic OnPowerUp Set message parameters.
 */
typedef struct MS_generic_onpowerup_struct
{
    /**
     * The Generic OnPowerUp state is an enumeration representing the behavior of an element when powered up.
     *
     * Value     | Description
     * ----------|------------
     * 0x00      | Off. After being powered up, the element is in an off state.
     * 0x01      | Default. After being powered up, the element is in an On state and uses default state values.
     * 0x02      | Restore. If a transition was in progress when powered down, the element restores the target
     *             state when powered up. Otherwise the element restores the state it was in when powered down.
     * 0x03-0xFF | Prohibited
     */
    UCHAR  onpowerup;

} MS_GENERIC_ONPOWERUP_STRUCT;

/** \} */

/** \} */

/* --------------------------------------------- Function */
/**
 * \defgroup generic_power_onoff_api_defs API Definitions
 * \{
 * \brief This section describes the various APIs exposed by
 * EtherMind Mesh Generic Power OnOff Model Layer to the Application.
 */

/**
 * \defgroup generic_power_onoff_ser_api_defs Generic Power OnOff Server API Definitions
 * \{
 * \brief This section describes the EtherMind Mesh Generic Power OnOff Server
 * Model APIs.
 */

/**
 * \name Generic Power OnOff Server Interfaces
 * \{
 */

#ifdef MS_MODEL_SERVER_EXTENDED_INTERFACE
/**
 *  \brief API to initialize Generic_Power_OnOff Server model
 *
 *  \par Description
 *  This is to initialize Generic_Power_Onoff Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] power_onoff_model_handle
 *                   Model identifier associated with the Generic Power OnOff model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in, out] power_onoff_setup_model_handle
 *                   Model identifier associated with the Generic Power OnOff Setup model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] power_onoff_appl_cb          Application Callback to be used by the Generic_Power_OnOff Server.
 *
 *  \param [in] power_onoff_setup_appl_cb    Application Callback to be used by the Generic_Power_OnOff_Setup Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_server_init_ext
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE                element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE                * power_onoff_model_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE                * power_onoff_setup_model_handle,
               /* IN */    MS_GENERIC_POWER_ONOFF_SERVER_CB        power_onoff_appl_cb,
               /* IN */    MS_GENERIC_POWER_ONOFF_SETUP_SERVER_CB  power_onoff_setup_appl_cb
           );
#endif /* MS_MODEL_SERVER_EXTENDED_INTERFACE */

/**
 *  \brief API to initialize Generic_Power_OnOff Server model (deprecated)
 *
 *  \par Description
 *  This is to initialize Generic_Power_OnOff Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Generic_Power_OnOff Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * model_handle,
               /* IN */    MS_GENERIC_POWER_ONOFF_SERVER_CB appl_cb
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
 * \param [in] reply                   If unicast response to be sent
 * \param [in] publish                 If state to be published
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_server_state_update
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * target_state_params,
               /* IN */ UINT16                               remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS         * ext_params,
               /* IN */ UCHAR                                reply,
               /* IN */ UCHAR                                publish
           );

/**
 *  \brief API to initialize Generic_Power_OnOff_Setup Server model (deprecated)
 *
 *  \par Description
 *  This is to initialize Generic_Power_OnOff_Setup Server model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Generic_Power_OnOff_Setup Server.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_setup_server_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * model_handle,
               /* IN */    MS_GENERIC_POWER_ONOFF_SETUP_SERVER_CB appl_cb
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
 * \param [in] reply                   If unicast response to be sent
 * \param [in] publish                 If state to be published
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_setup_server_state_update
           (
               /* IN */ MS_ACCESS_MODEL_REQ_MSG_CONTEXT    * ctx,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * current_state_params,
               /* IN */ MS_ACCESS_MODEL_STATE_PARAMS       * target_state_params,
               /* IN */ UINT16                               remaining_time,
               /* IN */ MS_ACCESS_MODEL_EXT_PARAMS         * ext_params,
               /* IN */ UCHAR                                reply,
               /* IN */ UCHAR                                publish
           );
/** \} */

/** \} */

/**
 * \defgroup generic_power_onoff_cli_api_defs Generic Power OnOff Client API Definitions
 * \{
 * \brief This section describes the EtherMind Generic Power OnOff Client
 * Model APIs.
 */

/**
 * \name Generic Power OnOff Client Interfaces
 * \{
 */

/**
 *  \brief API to initialize Generic_Power_OnOff Client model
 *
 *  \par Description
 *  This is to initialize Generic_Power_OnOff Client model and to register with Access layer.
 *
 *  \param [in] element_handle
 *              Element identifier to be associated with the model instance.
 *
 *  \param [in, out] model_handle
 *                   Model identifier associated with the model instance on successful initialization.
 *                   After power cycle of an already provisioned node, the model handle will have
 *                   valid value and the same will be reused for registration.
 *
 *  \param [in] appl_cb    Application Callback to be used by the Generic_Power_OnOff Client.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_client_init
           (
               /* IN */    MS_ACCESS_ELEMENT_HANDLE    element_handle,
               /* INOUT */ MS_ACCESS_MODEL_HANDLE    * model_handle,
               /* IN */    MS_GENERIC_POWER_ONOFF_CLIENT_CB appl_cb
           );

/**
 *  \brief API to get Generic_Power_OnOff client model handle
 *
 *  \par Description
 *  This is to get the handle of Generic_Power_OnOff client model.
 *
 *  \param [out] model_handle   Address of model handle to be filled/returned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_client_get_model_handle
           (
               /* OUT */ MS_ACCESS_MODEL_HANDLE  * model_handle
           );

/**
 *  \brief API to set Generic_Power_OnOff client model handle
 *
 *  \par Description
 *  This is to set the handle of Generic_Power_OnOff client model.
 *
 *  \param [in] model_handle   Model handle to be assigned.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_generic_power_onoff_client_set_model_handle
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
API_RESULT MS_generic_power_onoff_client_send_reliable_pdu
           (
               /* IN */ UINT32    req_opcode,
               /* IN */ void    * param,
               /* IN */ UINT32    rsp_opcode
           );
/** \} */

/** \} */

/** \} */

/**
 * \addtogroup generic_power_onoff_defines
 * \{
 */

/**
 * \defgroup gneric_power_onoff_marcos Utility Macros
 * \{
 * \brief This section describes the various Utility Macros in EtherMind
 * Mesh Generic Power OnOff Model Layer.
 */

/**
 * \name Generic Power OnOff Client Macros
 * \{
 */

/**
 *  \brief API to get the Generic OnPowerUp state of an element.
 *
 *  \par Description
 *  Generic OnPowerUp Get is an acknowledged message used to get the Generic OnPowerUp state of an element.
 *  The response to the Generic OnPowerUp Get message is a Generic OnPowerUp Status message.
 *  There are no parameters for this message.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_generic_onpowerup_get() \
        MS_generic_power_onoff_client_send_reliable_pdu \
        (\
            MS_ACCESS_GENERIC_ONPOWERUP_GET_OPCODE,\
            NULL,\
            MS_ACCESS_GENERIC_ONPOWERUP_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Generic OnPowerUp state of an element.
 *
 *  \par Description
 *  Generic OnPowerUp Set is an acknowledged message used to set the Generic OnPowerUp state of an element.
 *  The response to the Generic OnPowerUp Set message is a Generic OnPowerUp Status message.
 *
 *  \param [in] param Generic OnPowerUp Set message parameter \ref MS_GENERIC_ONPOWERUP_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_generic_onpowerup_set(param) \
        MS_generic_power_onoff_client_send_reliable_pdu \
        (\
            MS_ACCESS_GENERIC_ONPOWERUP_SET_OPCODE,\
            param,\
            MS_ACCESS_GENERIC_ONPOWERUP_STATUS_OPCODE\
        )

/**
 *  \brief API to set the Generic OnPowerUp state of an element.
 *
 *  \par Description
 *  Generic OnPowerUp Set Unacknowledged is an unacknowledged message used to set
 *  the Generic OnPowerUp state of an element.
 *
 *  \param [in] param Generic OnPowerUp Set message parameter \ref MS_GENERIC_ONPOWERUP_STRUCT
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
#define MS_generic_onpowerup_set_unacknowledged(param) \
        MS_generic_power_onoff_client_send_reliable_pdu \
        (\
            MS_ACCESS_GENERIC_ONPOWERUP_SET_UNACKNOWLEDGED_OPCODE,\
            param,\
            0xFFFFFFFF\
        )
/** \} */

/**
 * \name Generic Power OnOff Server and Generic Power OnOff Setup Server Macros
 * \{
 */
#ifdef MS_MODEL_SERVER_EXTENDED_INTERFACE
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
#define MS_generic_power_onoff_server_state_update_ext(c,cs,ts,rt,ex,r,p) \
        MS_generic_power_onoff_server_state_update \
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

/** \} */

/** \} */

/** \} */

/** \} */

#endif /* _H_MS_GENERIC_POWER_ONOFF_API_ */
