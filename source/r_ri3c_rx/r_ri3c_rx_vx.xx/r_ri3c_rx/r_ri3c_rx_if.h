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
 * File Name    : r_ri3c_rx_if.h
 * Description  : The RX RI3C Interface Header File.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 15.08.2022 1.00    First release
 *                              Support RX26T.
 **********************************************************************************************************************/

#ifndef R_RI3C_IF_H
#define R_RI3C_IF_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "platform.h"
#include "r_ri3c_rx_config.h"
#include "r_ri3c_rx_api.h"
#include "r_ri3c_rx_reg.h"

#if (R_BSP_VERSION_MAJOR < 7) && (R_BSP_VERSION_MINOR < 30)
    #error "This module must use BSP module of Rev.7.30 or higher. Please use the BSP module of Rev.7.30 or higher."
#endif

#define RI3C_RX_VERSION_MAJOR           (1)
#define RI3C_RX_VERSION_MINOR           (00)
#define R_RI3C_GetVersion() ((((uint32_t)RI3C_RX_VERSION_MAJOR) << 16) | (uint32_t)RI3C_RX_VERSION_MINOR)

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Macro for RI3C initialization */
#define RI3C_OPEN (('I' << 16U) | ('3' << 8U) | ('C' << 0U))

/** RI3C Data type macros */
#define RI3C0_Type st_ri3c
#define RI3C0_Base 0xEC000

/** RI3C Specifications */
#define BSP_FEATURE_RI3C_NUM_CHANNELS                      (1U)
#define BSP_FEATURE_RI3C_MAX_DEV_COUNT                     (4U)
#define BSP_FEATURE_RI3C_ICDR_DEPTH                        (2U)

/** Index for selecting the device defined in the extended address table. */
#define RI3C_DEVICE_INDEX_EXTENDED_DEVICE      (1U << 5U)

/** Event Status Provided by the callback. */
#define RI3C_EVENT_STATUS_SUCCESS              (0x0)  ///< The transfer was completed as expected.
#define RI3C_EVENT_STATUS_PARITY               (0x2)  ///< A parity error was detected.
#define RI3C_EVENT_STATUS_FRAME                (0x3)  ///< A frame error was detected.
#define RI3C_EVENT_STATUS_ADDRESS_HEADER       (0x4)  ///< An Address Header error wasdetected.
#define RI3C_EVENT_STATUS_NACK                 (0x5)  ///< The transfer was NACK'd.
#define RI3C_EVENT_STATUS_OVERFLOW             (0x6)  ///< A Receive FIFO overflow or Transmit FIFO underflow occurred.
#define RI3C_EVENT_STATUS_ABORTED_TO_CONTROLLER    (0x7)  ///< In target mode, the write transfer was ended via the 'T' bit.
#define RI3C_EVENT_STATUS_ABORTED              (0x8)  ///< In controller mode, the transfer was aborted.
#define RI3C_EVENT_STATUS_IBI_NACK_DISABLED    (0x20) ///< An IBI was NACK'd and the a DISEC command was sent.

/* Bitfield definitions for Command Descriptor fields. */
#define RI3C_CMD_DESC_CMD_ATTR_Pos (0U)
#define RI3C_CMD_DESC_CMD_ATTR_Msk (7U << RI3C_CMD_DESC_CMD_ATTR_Pos)
#define RI3C_CMD_DESC_CND_ATTR_XFER (0U)
#define RI3C_CMD_DESC_CMD_ATTR_ADDR_ASSGN_CMD (2U)
#define RI3C_CMD_DESC_TID_Pos (3U)
#define RI3C_CMD_DESC_TID_Msk (0x0FU << RI3C_CMD_DESC_TID_Pos)
#define RI3C_CMD_DESC_CMD_Pos (7U)
#define RI3C_CMD_DESC_CMD_Msk (0xFFU << RI3C_CMD_DESC_CMD_Pos)
#define RI3C_CMD_DESC_DEV_INDEX_Pos (16U)
#define RI3C_CMD_DESC_DEV_INDEX_Msk (0x01FFU << RI3C_CMD_DESC_DEV_INDEX_Pos)
#define RI3C_CMD_DESC_ROC_Pos (30U)
#define RI3C_CMD_DESC_ROC_Msk (1U << RI3C_CMD_DESC_ROC_Pos)
#define RI3C_CMD_DESC_TOC_Pos (31U)
#define RI3C_CMD_DESC_TOC_Msk (1U << RI3C_CMD_DESC_TOC_Pos)

/* Command Descriptor definitions for Address Assign Commands. */
#define RI3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Pos (26U)
#define RI3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Msk (0x0FU << RI3C_CMD_DESC_ADDR_ASSGN_DEV_COUNT_Pos)

/* Command Descriptor definitions for Regular transfer Commands. */
#define RI3C_CMD_DESC_XFER_CP_Pos (15U)
#define RI3C_CMD_DESC_XFER_CP_Msk (1U << RI3C_CMD_DESC_XFER_CP_Pos)
#define RI3C_CMD_DESC_XFER_HJ_Pos (15U)
#define RI3C_CMD_DESC_XFER_HJ_Msk (1U << RI3C_CMD_DESC_XFER_HJ_Pos)
#define RI3C_CMD_DESC_XFER_LENGTH_Pos (16U)
#define RI3C_CMD_DESC_XFER_LENGTH_Msk (0xFFFFU << RI3C_CMD_DESC_XFER_LENGTH_Pos)
#define RI3C_CMD_DESC_XFER_MODE_Pos (26U)
#define RI3C_CMD_DESC_XFER_MODE_Msk (0x07U << RI3C_CMD_DESC_XFER_MODE_Pos)
#define RI3C_CMD_DESC_XFER_RNW_Pos (29U)
#define RI3C_CMD_DESC_XFER_RNW_Msk (1U << RI3C_CMD_DESC_XFER_RNW_Pos)
#define RI3C_CMD_DESC_XFER_MODE_I2C_STDBR (0U)
#define RI3C_CMD_DESC_XFER_MODE_I2C_EXDBR (1U)
#define RI3C_CMD_DESC_XFER_MODE_RI3C_SDR_ICEBR_X2 (3U)
#define RI3C_CMD_DESC_XFER_MODE_RI3C_SDR_ICEBR_X4 (4U)

/* Bitfield definitions for the Response Status Descriptor fields. */
#define RI3C_RESP_STATUS_DESC_DATA_LENGTH_Pos (0U)
#define RI3C_RESP_STATUS_DESC_DATA_LENGTH_Msk (0xFFFFU << RI3C_RESP_STATUS_DESC_DATA_LENGTH_Pos)
#define RI3C_RESP_STATUS_DESC_TID_Pos (24U)
#define RI3C_RESP_STATUS_DESC_TID_Msk (0x0FU << RI3C_RESP_STATUS_DESC_TID_Pos)
#define RI3C_RESP_STATUS_DESC_ERR_STATUS_Pos (28U)
#define RI3C_RESP_STATUS_DESC_ERR_STATUS_Msk (0x0FU << RI3C_RESP_STATUS_DESC_ERR_STATUS_Pos)

/* Bitfield definitions for Receive Status Descriptor fields. */
#define RI3C_RECV_STATUS_DESC_DATA_LENGTH_Pos (0U)
#define RI3C_RECV_STATUS_DESC_DATA_LENGTH_Msk (0xFFFFU << RI3C_RECV_STATUS_DESC_DATA_LENGTH_Pos)
#define RI3C_RECV_STATUS_DESC_CMD_Pos (16U)
#define RI3C_RECV_STATUS_DESC_CMD_Msk (0xFFU << RI3C_RECV_STATUS_DESC_CMD_Pos)
#define RI3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Pos (23U)
#define RI3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Msk (0x01U << RI3C_RECV_STATUS_DESC_SDR_R_W_TYPE_Pos)
#define RI3C_RECV_STATUS_DESC_ERR_STATUS_Pos (24U)
#define RI3C_RECV_STATUS_DESC_ERR_STATUS_Msk (0x07U << RI3C_RECV_STATUS_DESC_ERR_STATUS_Pos)
#define RI3C_RECV_STATUS_DESC_TRANSFER_TYPE_Pos (27U)
#define RI3C_RECV_STATUS_DESC_TRANSFER_TYPE_Msk (0x03U << RI3C_RECV_STATUS_DESC_TRANSFER_TYPE_Pos)
#define RI3C_RECV_STATUS_DESC_DEV_INDEX_Pos (29U)
#define RI3C_RECV_STATUS_DESC_DEV_INDEX_Msk (0x07U << RI3C_RECV_STATUS_DESC_DEV_INDEX_Pos)

/* Bitfield definitions for IBI Status Descriptor fields. */
#define RI3C_IBI_STATUS_DESC_LENGTH_Pos (0U)
#define RI3C_IBI_STATUS_DESC_LENGTH_Msk (0xFFU << RI3C_IBI_STATUS_DESC_LENGTH_Pos)
#define RI3C_IBI_STATUS_DESC_IBI_ID_Pos (8U)
#define RI3C_IBI_STATUS_DESC_IBI_ID_Msk (0xFFU << RI3C_IBI_STATUS_DESC_IBI_ID_Pos)
#define RI3C_IBI_STATUS_DESC_LAST_STATUS_Pos (24U)
#define RI3C_IBI_STATUS_DESC_LAST_STATUS_Msk (1U << RI3C_IBI_STATUS_DESC_LAST_STATUS_Pos)
#define RI3C_IBI_STATUS_DESC_TS_Pos (25U)
#define RI3C_IBI_STATUS_DESC_TS_Msk (1U << RI3C_IBI_STATUS_DESC_TS_Pos)
#define RI3C_IBI_STATUS_DESC_ERR_STATUS_Pos (26U)
#define RI3C_IBI_STATUS_DESC_ERR_STATUS_Msk (0x7U << RI3C_IBI_STATUS_DESC_ERR_STATUS_Pos)
#define RI3C_IBI_STATUS_DESC_IBI_ST_Pos (31U)
#define RI3C_IBI_STATUS_DESC_IBI_ST_Msk (1U << RI3C_IBI_STATUS_DESC_IBI_ST_Pos)

/* Address that is written by the target during a Hot Join request. */
#define RI3C_HOT_JOIN_ADDRESS (2U)

/* Mask for flushing the Read, Write, and Command FIFO. */
#define RI3C_ICRCR_FIFO_FLUSH_Msk (R_RI3C0_ICRCR_CQRST_Msk | R_RI3C0_ICRCR_TBRST_Msk | \
                                     R_RI3C0_ICRCR_RBRST_Msk)

#define RI3C_MAX_PUSH_PULL_PERIOD (0x3FU)

/* Mask for converting the internal state into an I3C event. */
#define RI3C_INTERNAL_EVENT_MASK (0x7FU)

/** Constants of Push Pull Periods */
#define RI3C_PUSH_PULL_LOW_PERIOD_48MHz (104)
#define RI3C_PUSH_PULL_LOW_DIGITAL_PERIOD_48MHz (112)
#define RI3C_PUSH_PULL_HIGH_PERIOD_48MHz (148)
#define RI3C_PUSH_PULL_HIGH_DIGITAL_PERIOD_48MHz (156)

#define RI3C_PUSH_PULL_LOW_PERIOD_64MHz (80)
#define RI3C_PUSH_PULL_LOW_DIGITAL_PERIOD_64MHz (88)
#define RI3C_PUSH_PULL_HIGH_PERIOD_64MHz (112)
#define RI3C_PUSH_PULL_HIGH_DIGITAL_PERIOD_64MHz (120)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Bitrate settings that can be selected at run-time using @ref ri3c_api_t::deviceSelect. */
typedef enum e_ri3c_bitrate_setting
{
    RI3C_BITRATE_MODE_I2C_ICSBR          = 0U, ///< Use the period settings defined in ICSBR H/L.
    RI3C_BITRATE_MODE_I2C_ICEBR          = 1U, ///< Use the period settings defined in ICEBR H/L.
    RI3C_BITRATE_MODE_RI3C_SDR0_ICSBR    = 0U, ///< Use the period settings defined in ICSBR H/L (I3C Devices only).
    RI3C_BITRATE_MODE_RI3C_SDR1_ICEBR    = 1U, ///< Use the period settings defined in ICEBR H/L (I3C Devices only).
    RI3C_BITRATE_MODE_RI3C_SDR2_ICSBR_X2 = 2U, ///< Use the period settings defined in ICSBR H/L x 2 (I3C Devices only).
    RI3C_BITRATE_MODE_RI3C_SDR3_ICEBR_X2 = 3U, ///< Use the period settings defined in ICEBR H/L x 2 (I3C Devices only).
    RI3C_BITRATE_MODE_RI3C_SDR4_ICEBR_X4 = 4U, ///< Use the period settings defined in ICEBR H/L x 4 (I3C Devices only).
} ri3c_bitrate_mode_t;

/** Supported activity states for ENTASn Command (See ENTASn in the MIPI I3C Specification v1.0). */
typedef enum e_ri3c_activity_state
{
    RI3C_ACTIVITY_STATE_ENTAS0 = 0U,    ///< Activity Interval (1 microsecond).
    RI3C_ACTIVITY_STATE_ENTAS1 = 1U,    ///< Activity Interval (100 microseconds).
    RI3C_ACTIVITY_STATE_ENTAS2 = 2U,    ///< Activity Interval (2 milliseconds).
    RI3C_ACTIVITY_STATE_ENTAS3 = 3U,    ///< Activity Interval (50 milliseconds).
} ri3c_activity_state_t;

/** Maximum Sustained Data Rate for non-CCC messages sent by Controller Device to Target Device (See GETMXDS in the MIPI I3C Specification v1.0). */
typedef enum e_ri3c_data_rate_setting
{
    RI3C_DATA_RATE_SETTING_FSCL_MAX = 0, ///< There is no data rate limit.
    RI3C_DATA_RATE_SETTING_8MHZ     = 1, ///< The max sustained data rate is 8 Mhz.
    RI3C_DATA_RATE_SETTING_6MHZ     = 2, ///< The max sustained data rate is 6 Mhz.
    RI3C_DATA_RATE_SETTING_4MHZ     = 3, ///< The max sustained data rate is 4 Mhz.
    RI3C_DATA_RATE_SETTING_2MHZ     = 4, ///< The max sustained data rate is 2 Mhz.
} ri3c_data_rate_setting_t;

/** Clock to Data Turnaround Time (See GETMXDS in the MIPI I3C Specification v1.0). */
typedef enum e_ri3c_clock_data_turnaround
{
    RI3C_CLOCK_DATA_TURNAROUND_8NS      = 0, ///< Clock to turnaround time is 8 nanoseconds or less.
    RI3C_CLOCK_DATA_TURNAROUND_9NS      = 1, ///< Clock to turnaround time is 9 nanoseconds or less.
    RI3C_CLOCK_DATA_TURNAROUND_10NS     = 2, ///< Clock to turnaround time is 10 nanoseconds or less.
    RI3C_CLOCK_DATA_TURNAROUND_11NS     = 3, ///< Clock to turnaround time is 11 nanoseconds or less.
    RI3C_CLOCK_DATA_TURNAROUND_12NS     = 4, ///< Clock to turnaround time is 12 nanoseconds or less.
    RI3C_CLOCK_DATA_TURNAROUND_EXTENDED = 7, ///< Clock to turnaround time is greater than 12 nanoseconds.
} ri3c_clock_data_turnaround_t;

typedef enum e_ri3c_internal_state
{
    RI3C_INTERNAL_STATE_DISABLED,
    RI3C_INTERNAL_STATE_CONTROLLER_IDLE,
    RI3C_INTERNAL_STATE_CONTROLLER_ENTDAA = RI3C_EVENT_ADDRESS_ASSIGNMENT_COMPLETE,
    RI3C_INTERNAL_STATE_CONTROLLER_WRITE = RI3C_EVENT_WRITE_COMPLETE,
    RI3C_INTERNAL_STATE_CONTROLLER_READ = RI3C_EVENT_READ_COMPLETE,
    RI3C_INTERNAL_STATE_CONTROLLER_COMMAND_WRITE = RI3C_EVENT_COMMAND_COMPLETE,
    RI3C_INTERNAL_STATE_CONTROLLER_COMMAND_READ = (RI3C_EVENT_COMMAND_COMPLETE | (0x80U)),
    RI3C_INTERNAL_STATE_TARGET_IDLE,
    RI3C_INTERNAL_STATE_TARGET_IBI = RI3C_EVENT_IBI_WRITE_COMPLETE,
} ri3c_internal_state_t;

typedef enum e_ri3c_target_error_recovery_type
{
    RI3C_TARGET_ERROR_RECOVERY_TYPE_WRITE = 0,
    RI3C_TARGET_ERROR_RECOVERY_TYPE_READ  = 1,
    RI3C_TARGET_ERROR_RECOVERY_TYPE_IBI   = 2,
    RI3C_TARGET_ERROR_RECOVERY_TYPE_OTHER = 2,
} ri3c_target_error_recovery_type_t;

/** Clock stalling settings. */
typedef struct s_ri3c_clock_stalling
{
    uint32_t assigned_address_phase_enable : 1; ///< Enable Clock Stalling during the address phase of the ENTDAA command.
    uint32_t transition_phase_enable       : 1; ///< Enable Clock Stalling during the transition bit in read transfers.
    uint32_t parity_phase_enable           : 1; ///< Enable Clock Stalling during the parity bit period in write transfers.
    uint32_t ack_phase_enable              : 1; ///< Enable Clock Stalling during the ACK/NACK phase.
    uint16_t clock_stalling_time;               ///< The amount of time to stall the clock in I3C source clock ticks.
} ri3c_clock_stalling_t;

/** Bitrate settings for configuring the SCL clock frequency. */
typedef struct s_ri3c_bitrate_settings
{
    uint32_t icsbr;                    ///< The standard bitrate settings.
    uint32_t icebr;                    ///< The extended bitrate settings.

    /** Clock Stalling settings (See Controller Clock Stalling in the MIPI I3C Specification v1.0). */
    ri3c_clock_stalling_t clock_stalling;
} ri3c_bitrate_settings_t;

/** Settings for controlling the drivers behavior in response to IBIs. */
typedef struct s_ri3c_ibi_control
{
    uint32_t hot_join_acknowledge                : 1;     ///< If false, NACK all Hot Join requests.
    uint32_t notify_rejected_hot_join_requests   : 1;     ///< Notify the application when an IBI Hot-Join request has been NACK'd.
    uint32_t notify_rejected_controllerrole_requests : 1; ///< Notify the application when an IBI Controller Role request has been NACK'd.
    uint32_t notify_rejected_interrupt_requests  : 1;     ///< Notify the application when an IBI Interrupt request has been NACK'd.
} ri3c_ibi_control_t;

/** Default configuration settings for the target response to Direct Get Common Command Codes. */
typedef struct s_target_command_response_info
{
    /** Target Event Settings (See ENEC and DISEC in the MIPI I3C Specification v1.0). */
    bool inband_interrupt_enable;          ///< Enable IBI interrupts.
    bool controllerrole_request_enable;    ///< Enable Controller Role requests.
    bool hotjoin_request_enable;           ///< Enable Hot-Join requests.

    /** Starting Activity State (See ENTASn in the MIPI I3C Specification v1.0). */
    ri3c_activity_state_t activity_state;

    /** Max Write Length (See SETMWL and GETMWL in the MIPI I3C Specification v1.0). */
    uint16_t write_length;

    /** Max Read Length (See SETMRL and GETMRL in the MIPI I3C Specification v1.0). */
    uint16_t read_length;

    /** Number of bytes that will be written by an IBI (See SETMRL and GETMRL in the MIPI I3C Specification v1.0). */
    uint8_t ibi_payload_length;

    /** Max Data Rate Settings (See GETMXDS in the MIPI I3C Specification v1.0). */
    ri3c_data_rate_setting_t     write_data_rate;       ///< Max Write Data Rate.
    ri3c_data_rate_setting_t     read_data_rate;        ///< Max Read Data Rate.
    ri3c_clock_data_turnaround_t clock_data_turnaround; ///< Max Data Speed Turnaround.
    bool     read_turnaround_time_enable;              ///< Enable transmission of the of the Max Read Max Read Turnaround Time.
    uint32_t read_turnaround_time;                     ///< Max Read Turnaround Time.

    /** This byte represents the Target’s internal oscillator frequency in increments of 0.5 MHz (500kHz), up to 127.5 MHz.
     * (See GETXTIME in the MIPI I3C Specification v1.1). */
    uint8_t oscillator_frequency;

    /** Oscillator inaccuracy in 0.5% increments of 0% up to 25.5% (See GETXTIME in the MIPI I3C Specification v1.1). */
    uint8_t oscillator_inaccuracy;
} ri3c_target_command_response_info_t;

/* Buffer descriptor for keeping track of a read transfer. */
typedef struct s_ri3c_read_buffer_descriptor
{
    uint8_t * p_buffer;                ///< Pointer to a buffer for storing data that has been read.
    uint32_t  count;                   ///< Number of bytes that have been read.
    uint32_t  buffer_size;             ///< Total size of the buffer.

    /** Flag for keeping track of whether the @ref RI3C_EVENT_READ_BUFFER_FULL callback has been issued. */
    bool read_request_issued;
} ri3c_read_buffer_descriptor_t;

/* Buffer descriptor for keeping track of a write transfer. */
typedef struct s_ri3c_write_buffer_descriptor
{
    uint8_t * p_buffer;                ///< Pointer to a buffer that will be read during a write transfer.
    uint32_t  count;                   ///< Number of bytes that have been written.
    uint32_t  buffer_size;             ///< Total size of the buffer.
} ri3c_write_buffer_descriptor_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when @ref ri3c_api_t::open is called. */
typedef struct st_ri3c_instance_ctrl
{
    uint32_t           open;                                            ///< Indicates whether the open() API has been successfully called.

#if((__CCRX__) || (__GNUC__))                                           ///< Base register for this channel
    volatile struct RI3C0_Type R_BSP_EVENACCESS * p_reg;                ///< CCRX and GCC version
#elif(__ICCRX__)
    struct RI3C0_Type R_BSP_VOLATILE_SFR * p_reg;                       ///< IAR version
#endif

    volatile uint32_t  internal_state;                                  ///< Used to track the current state of the driver.
    uint8_t            current_command_code;                            ///< The current Common Command Code that is being transferred.
    uint32_t           device_index;                                    ///< The device index selected using @ref ri3c_api_t::deviceSelect.
    ri3c_bitrate_mode_t device_bitrate_mode;                            ///< Runtime bitrate settings to use for the next transfer.
    ri3c_target_info_t  current_target_info;                            ///< The last @ref ri3c_target_info_t read during ENTDAA.
    uint32_t next_word;                                                 ///< The next word that will be written to the FIFO.
    uint32_t ibi_next_word;                                             ///< The next word that will be written to the IBI FIFO.
    ri3c_write_buffer_descriptor_t write_buffer_descriptor;             ///< Buffer descriptor for keeping track of a write transfer.
    ri3c_read_buffer_descriptor_t  read_buffer_descriptor;              ///< Buffer descriptor for keeping track of a read transfer.
    ri3c_read_buffer_descriptor_t  ibi_buffer_descriptor;               ///< Buffer descriptor for keeping track of an IBI read/write transfer.
    volatile uint32_t              read_transfer_count_final;           ///< The total number of bytes read during a read transfer.
    volatile uint32_t              ibi_transfer_count_final;            ///< The total number of bytes read during an IBI transfer.
    ri3c_cfg_t const             * p_cfg;                               ///< A pointer to the configuration structure provided during open.
} ri3c_instance_ctrl_t;

/** Extended configuration for r_ri3c. */
typedef struct s_ri3c_extended_cfg
{
    ri3c_bitrate_settings_t bitrate_settings; ///< Bitrate settings configuring the frequency and duty cycle for SCL.
    ri3c_ibi_control_t      ibi_control;      ///< Configure the driver's behavior in response to IBIs.

    /**
     * The time in I3C reference clock ticks needed in order to detect the bus free condition
     * (See "Bus Free Condition" in the MIPI I3C Specification v1.0).
     */
    uint32_t bus_free_detection_time;

    /**
     * The time in I3C reference clock ticks needed in order to detect the bus available condition
     * (See "Bus Available Condition" in the MIPI I3C Specification v1.0).
     */
    uint32_t bus_available_detection_time;

    /**
     * The time in I3C reference clock ticks needed in order to detect the bus idle condition
     * (See "Bus Idle Condition" in the MIPI I3C Specification v1.0).
     */
    uint32_t bus_idle_detection_time;

    bool timeout_detection_enable;     ///< Notify the application if SCL is stuck high or low.

    /** Initial settings for configuring the target's responses to received commands. */
    ri3c_target_command_response_info_t target_command_response_info;

    uint8_t   ipl;                     ///< Interrupt Priority for RSPI, CMDI, RXI, TXI, and RCVI Interrupts.
    uint8_t   eei_ipl;                 ///< Error and Event Interrupt Priority.
} ri3c_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** Filled in Interface API structure for this Instance. */
extern ri3c_api_t g_ri3c0_on_ri3c;

/* Extended configuration for this instance of RI3C. */
extern ri3c_extended_cfg_t g_ri3c0_cfg_extend;

/* Global context pointer */
extern void * gp_fit_isr_context;

/** RI3C0 on RI3C Instance. */
/** Access the RI3C instance using these structures when calling API functions directly (::p_api is not used). */
extern ri3c_instance_ctrl_t g_ri3c0_ctrl;
extern ri3c_cfg_t g_ri3c0_cfg;
extern ri3c_instance_t g_ri3c0;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_RI3C_Open(ri3c_ctrl_t * const p_api_ctrl, ri3c_cfg_t const * const p_cfg);
fsp_err_t R_RI3C_Enable(ri3c_ctrl_t * const p_api_ctrl);
fsp_err_t R_RI3C_DeviceCfgSet(ri3c_ctrl_t * const p_api_ctrl, ri3c_device_cfg_t const * const p_device_cfg);
fsp_err_t R_RI3C_ControllerDeviceTableSet(ri3c_ctrl_t * const                   p_api_ctrl,
                                          uint32_t                              device_index,
                                          ri3c_device_table_cfg_t const * const p_device_table_cfg);
fsp_err_t R_RI3C_TargetStatusSet(ri3c_ctrl_t * const p_api_ctrl, ri3c_device_status_t status);
fsp_err_t R_RI3C_DeviceSelect(ri3c_ctrl_t * const p_api_ctrl, uint32_t device_index, uint32_t bitrate_mode);
fsp_err_t R_RI3C_DynamicAddressAssignmentStart(ri3c_ctrl_t * const            p_api_ctrl,
                                               ri3c_address_assignment_mode_t address_assignment_mode,
                                               uint32_t                       starting_device_index,
                                               uint32_t                       device_count);
fsp_err_t R_RI3C_CommandSend(ri3c_ctrl_t * const p_api_ctrl, ri3c_command_descriptor_t * p_command_descriptor);
fsp_err_t R_RI3C_Write(ri3c_ctrl_t * const p_api_ctrl, uint8_t const * const p_data, uint32_t length, bool restart);
fsp_err_t R_RI3C_Read(ri3c_ctrl_t * const p_api_ctrl, uint8_t * const p_data, uint32_t length, bool restart);
fsp_err_t R_RI3C_IbiWrite(ri3c_ctrl_t * const    p_api_ctrl,
                          ri3c_ibi_type_t        ibi_type,
                          uint8_t const * const  p_data,
                          uint32_t               length);
fsp_err_t R_RI3C_IbiRead(ri3c_ctrl_t * const p_api_ctrl, uint8_t * const p_data, uint32_t length);
fsp_err_t R_RI3C_Close(ri3c_ctrl_t * const p_api_ctrl);

/***********************************************************************************************************************
 * @} (end defgroup RI3C)
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* R_RI3C_IF_H */
