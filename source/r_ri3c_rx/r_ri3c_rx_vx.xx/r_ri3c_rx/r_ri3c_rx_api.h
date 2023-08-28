/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
 * applicable laws, including copyright laws. 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO 
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the 
 * following link:
 * http://www.renesas.com/disclaimer 
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_ri3c_rx_api.h
 * Description  : Renesas FIT RX RI3C driver's API header file.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 15.08.2022 1.00    First release
 *                              Supported for RX26T.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#ifndef R_RI3C_API_H
#define R_RI3C_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/** Common Command Codes defined by MIPI I3C Specification v1.1. */
typedef enum e_ri3c_common_command_code
{
    /** Broadcast Common Command Codes */
    I3C_CCC_BROADCAST_ENEC     = (0x00), // Enable Target initiated events.
    I3C_CCC_BROADCAST_DISEC    = (0x01), // Disable Target initiated events.
    I3C_CCC_BROADCAST_ENTAS0   = (0x02), // Enter Activity State 0.
    I3C_CCC_BROADCAST_ENTAS1   = (0x03), // Enter Activity State 1.
    I3C_CCC_BROADCAST_ENTAS2   = (0x04), // Enter Activity State 2.
    I3C_CCC_BROADCAST_ENTAS3   = (0x05), // Enter Activity State 3.
    I3C_CCC_BROADCAST_RSTDAA   = (0x06), // Reset Dynamic Address Assignment.
    I3C_CCC_BROADCAST_ENTDAA   = (0x07), // Enter Dynamic Address Assignment.
    I3C_CCC_BROADCAST_DEFSVLS  = (0x08), // Define List of Targets.
    I3C_CCC_BROADCAST_SETMWL   = (0x09), // Set Max Write Length.
    I3C_CCC_BROADCAST_SETMRL   = (0x0A), // Set Max Read Length.
    I3C_CCC_BROADCAST_ENTTM    = (0x0B), // Enter Test Mode.
    I3C_CCC_BROADCAST_ENTHDR0  = (0x20), // Enter HDR Mode 0.
    I3C_CCC_BROADCAST_ENTHDR1  = (0x21), // Enter HDR Mode 1.
    I3C_CCC_BROADCAST_ENTHDR2  = (0x22), // Enter HDR Mode 2.
    I3C_CCC_BROADCAST_ENTHDR3  = (0x23), // Enter HDR Mode 3.
    I3C_CCC_BROADCAST_ENTHDR4  = (0x24), // Enter HDR Mode 4 (Reserved for future definition).
    I3C_CCC_BROADCAST_ENTHDR5  = (0x25), // Enter HDR Mode 5 (Reserved for future definition).
    I3C_CCC_BROADCAST_ENTHDR6  = (0x26), // Enter HDR Mode 6 (Reserved for future definition).
    I3C_CCC_BROADCAST_ENTHDR7  = (0x27), // Enter HDR Mode 7 (Reserved for future definition).
    I3C_CCC_BROADCAST_SETXTIME = (0x28), // Set Exchange Timing Info.
    I3C_CCC_BROADCAST_SETAASA  = (0x29), // Set All Addresses to Static Address.

    /** Direct Common Command Codes */
    I3C_CCC_DIRECT_ENEC      = (0x80),   // Enable Target initiated events.
    I3C_CCC_DIRECT_DISEC     = (0x81),   // Disable Target initiated events.
    I3C_CCC_DIRECT_ENTAS0    = (0x82),   // Enter Activity State 0.
    I3C_CCC_DIRECT_ENTAS1    = (0x83),   // Enter Activity State 1.
    I3C_CCC_DIRECT_ENTAS2    = (0x84),   // Enter Activity State 2.
    I3C_CCC_DIRECT_ENTAS3    = (0x85),   // Enter Activity State 3.
    I3C_CCC_DIRECT_RSTDAA    = (0x86),   // Reset Dynamic Address Assignment.
    I3C_CCC_DIRECT_SETDASA   = (0x87),   // Set Dynamic Address from Static Address.
    I3C_CCC_DIRECT_SETNEWDA  = (0x88),   // Set New Dynamic Address.
    I3C_CCC_DIRECT_SETMWL    = (0x89),   // Set Max Write Length.
    I3C_CCC_DIRECT_SETMRL    = (0x8A),   // Set Max Read Length.
    I3C_CCC_DIRECT_GETMWL    = (0x8B),   // Get Max Write Length.
    I3C_CCC_DIRECT_GETMRL    = (0x8C),   // Get Max Read Length.
    I3C_CCC_DIRECT_GETPID    = (0x8D),   // Get Provisional ID.
    I3C_CCC_DIRECT_GETBCR    = (0x8E),   // Get Bus Characteristic Register.
    I3C_CCC_DIRECT_GETDCR    = (0x8F),   // Get Device Characteristic Register.
    I3C_CCC_DIRECT_GETSTATUS = (0x90),   // Get Device Status.
    I3C_CCC_DIRECT_GETACCMST = (0x91),   // Get Accept Controller Role.
    I3C_CCC_DIRECT_GETMXDS   = (0x94),   // Get Max Data Speed.
    I3C_CCC_DIRECT_SETXTIME  = (0x98),   // Set Exchange Timing Information.
    I3C_CCC_DIRECT_GETXTIME  = (0x99),   // Get Exchange Timing Information.
} ri3c_common_command_code_t;

/** RI3C Events that result in a callback. */
typedef enum e_ri3c_event
{
    /** Events that only occur in Controller mode. */

    /**
     * A Target device has finished writing its PID, BCR, and DCR.
     */
    RI3C_EVENT_ENTDAA_ADDRESS_PHASE,
    RI3C_EVENT_IBI_READ_COMPLETE,       // An IBI has successfully been read.

    /**
     * There is no more space in the IBI read buffer. The application may provide another buffer by calling
     * ri3c_api_t::ibiRead.
     */
    RI3C_EVENT_IBI_READ_BUFFER_FULL,

    /** Events that only occur in Target mode. */

    /**
     * There is no more space in the read buffer.
     */
    RI3C_EVENT_READ_BUFFER_FULL,
    RI3C_EVENT_IBI_WRITE_COMPLETE,          // A IBI was written successfully.
    RI3C_EVENT_HDR_EXIT_PATTERN_DETECTED,   // The HDR exit pattern was detected on the bus.

    /** Events that are common to Controller and Target mode. */

    RI3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE, // Dynamic Address Assignment has completed.
    RI3C_EVENT_COMMAND_COMPLETE,            // A command was completed.
    RI3C_EVENT_WRITE_COMPLETE,              // A write transfer has completed.
    RI3C_EVENT_READ_COMPLETE,               // A read transfer has completed.
    RI3C_EVENT_TIMEOUT_DETECTED,            // SCL is stuck at the logic high or logic low level during a transfer.
    RI3C_EVENT_INTERNAL_ERROR,              // An internal error occurred.
} ri3c_event_t;

/** The type of device. */
typedef enum e_ri3c_type
{
    /** The main controller starts in controller mode and is responsible for configuring the bus. */
    RI3C_DEVICE_TYPE_PRIMARY_CONTROLLER,

    /** A target device listens to the bus for relevant I3C Commands (CCCs) sent by the current controller,
     *  and responds accordingly. Target devices may also initiate In-band interrupts and Hot-Join requests. */
    RI3C_DEVICE_TYPE_TARGET,
} ri3c_device_type_t;

/** Identifies the protocol for transferring data with the device on the bus. */
typedef enum e_ri3c_device_protocol
{
    /** Transfers will use legacy I2C protocal with open-drain output at a reduced baudrate. */
    RI3C_DEVICE_PROTOCOL_I2C,

    /** Transfers will use I3C SDR mode. */
    RI3C_DEVICE_PROTOCOL_I3C,
} ri3c_device_protocol_t;

/** Address Assignment Mode. */
typedef enum e_ri3c_address_assignment_mode
{
    /**
     * Send the ENTDAA command to enter Dynamic Address Assignment mode and assign dynamic addresses in order,
     * starting with the starting device index. The procedure is completed after the specified number of devices
     * have been configured. The callback will be called after the PID, DCR, and BCR registers have been read
     * for each device.
     */
    RI3C_ADDRESS_ASSIGNMENT_MODE_ENTDAA = I3C_CCC_BROADCAST_ENTDAA,

    /** Send the SETDASA command to the device selected by the starting device index (Set the device count to 0). */
    RI3C_ADDRESS_ASSIGNMENT_MODE_SETDASA = I3C_CCC_DIRECT_SETDASA,
} ri3c_address_assignment_mode_t;

/** The type of In-Band Interrupt. */
typedef enum e_ri3c_ibi_type
{
    /** Application specific In-Band Interrupt for notifying the controller when an event occurs. */
    RI3C_IBI_TYPE_INTERRUPT,

    /** Request the controller to perform the Dynamic Address Assignment process. */
    RI3C_IBI_TYPE_HOT_JOIN,

    /** Request the controller to give up control of the bus. */
    RI3C_IBI_TYPE_CONTROLLERROLE_REQUEST
} ri3c_ibi_type_t;

/** The current status of the target device (See GETSTATUS in the MIPI I3C Specification v1.0). */
typedef struct s_ri3c_device_status
{
    /** Contains the interrupt number of any pending interrupt, or 0 if no interrupts are pending. */
    uint8_t pending_interrupt;

    /** Reserved for vendor-specific meaning. */
    uint8_t vendor_status;
} ri3c_device_status_t;

/** Device characteristics that define the I3C capabilities of a target. */
typedef struct s_ri3c_target_info
{
    uint8_t pid[6]; /* Provisional ID. */

    union
    {
        uint8_t bcr; /* Bus Characteristics Register. */

        struct
        {
            /**
             * Max Data Speed Limitation:
             * - No limitation.
             * - Limitation (If this is set, the controller shall get the Max Data Speed using the GETMXDS command).
             */
            uint8_t max_data_speed_limitation : 1;

            /**
             * IBI Request Capable
             * - 0: Not Capable.
             * - 1: Capable.
             */
            uint8_t ibi_request_capable : 1;

            /**
             * IBI Payload:
             * - 0: No data byte follows the accepted IBI
             * - 1: Mandatory one or more data bytes follow the accepted IBI.
             */
            uint8_t ibi_payload : 1;

            /**
             * Offline Capable:
             * - 0: Not Capable.
             * - 1: Capable.
             */
            uint8_t offline_capable : 1;

            uint8_t                 : 2; /* Reserved */

            /**
             * Device Role:
             * - 0: I3C Target.
             * - 1: I3C Controller (Device acting as the main controller will set this to 1).
             * - 2: Reserved.
             * - 3: Reserved.
             */
            uint8_t device_role : 2;
        } bcr_b;
    };

    /** Device Characteristics Register. */
    uint8_t dcr;
} ri3c_target_info_t;

/**
 * Structure for configuring an entry in the device table when the driver is in controller mode
 * (See ri3c_api_t::controllerDeviceTableSet).
 */
typedef struct s_ri3c_device_table_cfg
{
    /** I3C Static address / I2C address for this device. */
    uint8_t static_address;

    /** Dynamic address for the device. This address will be assigned during Dynamic Address Assignment. */
    uint8_t dynamic_address;

    /** The protocol used to communicate with this device (I3C / I2C Legacy). */
    ri3c_device_protocol_t device_protocol;

    /** Accept or reject IBI requests from this device. */
    bool ibi_accept;

    /** Accept controller role requests from this device. */
    bool controllerrole_request_accept;

    /**
     * IBI requests from this device have a data payload.
     *
     * Note: When the device is configured using ENTDAA, the ibi_payload will automatically be updated
     *       based on the value of BCR.
     */
    bool ibi_payload;
} ri3c_device_table_cfg_t;

/** Structure for configuring a target address when the driver is in target mode. */
typedef struct s_ri3c_target_device_cfg
{
    /** I3C Static address. */
    uint8_t static_address;

    /** Dynamic address for this device. Note that the dynamic address will automatically be updated
     *  when ENTDAA is completed.*/
    uint8_t dynamic_address;

    /** PID, BCR, and DCR registers for the device (Target mode only). */
    ri3c_target_info_t target_info; 
} ri3c_device_cfg_t;

/** Descriptor for completing CCC transfers. */
typedef struct s_ri3c_command_descriptor
{
    uint8_t   command_code;            // Common Command Code for the transfer.
    uint8_t * p_buffer;                // Buffer for reading or writing data.
    uint32_t  length;                  // Length of the data portion of the command.
    bool      restart;                 // If true, issue a repeated-start after the transfer is completed.
    bool      rnw;                     // Set to true if the command type is Direct Get.
} ri3c_command_descriptor_t;

/** Arguments that are passed to the user callback when an event occurs. */
typedef struct s_ri3c_callback_args
{
    /** The type of event that has occurred. */
    ri3c_event_t               event;

    /** Status flags associated with the event. */
    uint32_t                   event_status;

    /** Number of bytes transferred. */
    uint32_t                   transfer_size;

    /** A pointer to the Characteristics Registers read during ENTDAA. */
    ri3c_target_info_t const * p_target_info;

    /** The dynamic address that was assigned to the target during ENTDAA. */
    uint8_t                    dynamic_address;

    /** The type of IBI that has been received. */
    ri3c_ibi_type_t            ibi_type;

    /** The address of the device that sent the IBI. */
    uint8_t                    ibi_address;

    /** The command code of the received command. */
    uint8_t                    command_code;

    /** User defined context. */
    void const               * p_context;
} ri3c_callback_args_t;

/** User configuration structure, used in open function */
typedef struct st_ri3c_cfg
{
    /** Select a channel corresponding to the channel number of the hardware. */
    uint32_t channel;

    /** The type of device. */
    ri3c_device_type_t device_type;

    /** Pointer to the user callback. */
    void (* p_callback)(ri3c_callback_args_t const * const p_args);

    void const * p_context;            // Pointer to the user-provided context

    /** Pointer to extended configuration. */
    void const * p_extend;
} ri3c_cfg_t;

typedef void ri3c_ctrl_t;

/** RI3C functions implemented will follow this API. */
typedef struct st_ri3c_api
{
    /** Initial configuration.
     *
     * @param[in]   p_ctrl     Pointer to control block. Must be declared by user. Elements set here.
     * @param[in]   p_cfg      Pointer to configuration structure. All elements of this structure must be set by user.
     */
    fsp_err_t (* open)(ri3c_ctrl_t * const p_ctrl, ri3c_cfg_t const * const p_cfg);

    /**
     * Enable the RI3C device.
     *
     * @param[in]   p_ctrl     Control block set this instance.
     */
    fsp_err_t (* enable)(ri3c_ctrl_t * const p_ctrl);

    /**
     * Set the configuration of this device.
     *
     * @param[in]   p_ctrl       Control block set this instance.
     * @param[in]   p_device_cfg Pointer to device configuration.
     */
    fsp_err_t (* deviceCfgSet)(ri3c_ctrl_t * const p_ctrl, ri3c_device_cfg_t const * const p_device_cfg);

    /**
     * Set the configuration for the device at the given index in the device table. The configuration will
     * be used by transfers when it is selected by deviceSelect.
     *
     * Note: This function is not used in target mode.
     *
     * @param[in]   p_ctrl             Control block set this instance.
     * @param[in]   device_index       Index into the device table.
     * @param[in]   p_device_table_cfg Pointer to the table settings for the entry in the controller device table.
     */
    fsp_err_t (* controllerDeviceTableSet)(ri3c_ctrl_t * const p_ctrl, uint32_t device_index,
                                       ri3c_device_table_cfg_t const * const p_device_table_cfg);

    /**
     * In controller mode, select the device for the next transfer.
     *
     * Note: This function is not used in target mode.
     *
     * @param[in]   p_ctrl          Control block set this instance.
     * @param[in]   device_index    Index into the device table.
     * @param[in]   bitrate_setting The bitrate settings for the selected device.
     */
    fsp_err_t (* deviceSelect)(ri3c_ctrl_t * const p_ctrl, uint32_t device_index, uint32_t bitrate_mode);

    /** Start Dynamic Address Assignment by sending either the ENTDAA or SETDASA command.
     *  See ri3c_address_assignment_mode_t for more information.
     *
     * Note: This function is not used in target mode.
     *
     * @param[in]   p_ctrl                  Control block set this instance.
     * @param[in]   address_assignment_mode The command to use for Dynamic Address Assignment.
     * @param[in]   starting_device_index   The device index that will be used to assign the first device during DAA.
     * @param[in]   device_count            The number of devices to assign
     *                                      (Only used with RI3C_ADDRESS_ASSIGNMENT_MODE_ENTDAA).
     */
    fsp_err_t (* dynamicAddressAssignmentStart)(ri3c_ctrl_t * const p_ctrl,
                                                ri3c_address_assignment_mode_t address_assignment_mode,
                                                uint32_t starting_device_index, uint32_t device_count);

    /**
     * Set the status returned to the controller in response to a GETSTATUS command.
     *
     * Note: This function is not used in controller mode.
     *
     * @param[in]   p_ctrl          Control block set this instance.
     * @param[in]   device_status   New status settings for responding to the GETSTATUS command code.
     */
    fsp_err_t (* targetStatusSet)(ri3c_ctrl_t * const p_ctrl, ri3c_device_status_t device_status);

    /**
     * Send a broadcast or directed command to target devices on the bus.
     *
     * Note: This function is not used in target mode.
     *
     * @param[in]   p_ctrl               Control block set this instance.
     * @param[in]   p_command_descriptor A descriptor for executing the command.
     */
    fsp_err_t (* commandSend)(ri3c_ctrl_t * const p_ctrl, ri3c_command_descriptor_t * p_command_descriptor);

    /**
     * In controller mode: Start a write transfer. When the transfer is completed, send a stop condition
     *                     or a repeated-start.
     * In target mode: Set the write buffer and configure the number of bytes that will be transferred before
     *                 the transfer is ended by the target via the 'T' bit or by the controller issuing
     *                 a stop condition.
     *
     * @param[in]   p_ctrl     Control block set this instance.
     * @param[in]   p_data     Pointer to a buffer to write.
     * @param[in]   length     Number of bytes to transfer.
     * @param[in]   restart    If true, issue a repeated-start after the transfer is completed (Controller only).
     */
    fsp_err_t (* write)(ri3c_ctrl_t * const p_ctrl, uint8_t const * const p_data, uint32_t length, bool restart);

    /**
     * In controller mode: Start a read transfer. When the transfer is completed, send a stop condition
     *                     or a repeated-start.
     * In target mode: Set the read buffer for storing data read during the transfer. When the buffer is full,
     *                 the application will receive a callback requesting a new read buffer.
     *                 If no buffer is provided by the application, the driver will discard any remaining bytes read
     *                 during the transfer.
     *
     * @param[in]   p_ctrl     Control block set this instance.
     * @param[in]   p_data     Pointer to a buffer to store the bytes read during the transfer.
     * @param[in]   length     Number of bytes to transfer.
     * @param[in]   restart    If true, issue a repeated-start after the transfer is completed (Controller only).
     */
    fsp_err_t (* read)(ri3c_ctrl_t * const p_ctrl, uint8_t * const p_data, uint32_t length, bool restart);

    /**
     * Initiate an IBI write operation.
     *
     * Note: This function is not used in controller mode.
     *
     * @param[in]   p_ctrl     Control block set this instance.
     * @param[in]   p_data     Pointer to a buffer to start the bytes read during the transfer.
     * @param[in]   length     Number of bytes to transfer.
     */
    fsp_err_t (* ibiWrite)(ri3c_ctrl_t * const p_ctrl, ri3c_ibi_type_t ibi_type, uint8_t const * const p_data,
                           uint32_t length);

    /**
     * Set the read buffer for storing received IBI data (This function is not used in target mode).
     *
     * @param[in]   p_ctrl     Control block set this instance.
     * @param[in]   p_data     Pointer to a buffer to store the bytes read during the transfer.
     * @param[in]   length     Number of bytes to transfer.
     */
    fsp_err_t (* ibiRead)(ri3c_ctrl_t * const p_ctrl, uint8_t * const p_data, uint32_t length);

    /** Allows driver to be reconfigured and may reduce power consumption.
     *
     * @param[in]   p_ctrl     Control block set this instance.
     */
    fsp_err_t (* close)(ri3c_ctrl_t * const p_ctrl);
} ri3c_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ri3c_instance
{
    ri3c_ctrl_t * p_ctrl;          // Pointer to the control structure for this instance
    ri3c_cfg_t  * p_cfg;           // Pointer to the configuration structure for this instance
    ri3c_api_t  * p_api;           // Pointer to the API structure for this instance
} ri3c_instance_t;

#ifdef __cplusplus
}
#endif
#endif /* R_RI3C_API_H */
