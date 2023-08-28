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
 * File Name    : r_ri3c_rx_config.h
 * Description  : Configures the RI3C drivers
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 15.08.2022 1.00    First release
 *                              Support RX26T.
 *         : 30.06.2023 1.10    Support RX26T-256K.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "platform.h"
#include "r_ri3c_rx_if.h"

#ifndef R_RI3C_CFG_H_
 #define R_RI3C_CFG_H_

#if ((BSP_MCU_RX26T == 1) && ((BSP_CFG_MCU_PART_MEMORY_SIZE == 0x8) || (BSP_CFG_MCU_PART_MEMORY_SIZE == 0xA)))
#error  "Error - RX26T Product with 48 Kbytes RAM not support RI3C Module!"
#endif

/***********************************************************************************************************************
 * Configuration Options
 **********************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Specify whether to include code for API parameter checking
 * Set to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * Set to 1 includes parameter checking; 0 compiles out parameter checking
 * Use this option with caution.
 */
    #define RI3C_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Selects whether to support Unaligned Buffer in this device.
 * 1 = Enabled (System default)
 * 0 = Disabled
 */
    #define RI3C_CFG_UNALIGNED_BUFFER_SUPPORT (1)

/* Device operation mode.
 * RI3C_DEVICE_TYPE_PRIMARY_CONTROLLER
 * RI3C_DEVICE_TYPE_TARGET (System default)
 */
    #define RI3C_CFG_DEVICE_TYPE (RI3C_DEVICE_TYPE_TARGET)

/* Select enable/disable Controller mode of this device.
 * 1 = Enabled (System default)
 * 0 = Disabled
 * If only Controller mode is required, disable Target support to decrease code size.
 */
    #define RI3C_CFG_CONTROLLER_SUPPORT (1)

/* Select enable/disable Target mode of this device
 * 1 = Enabled (System default)
 * 0 = Disabled
 * If only Controller mode is required, disable Target support to decrease code size.
 */
    #define RI3C_CFG_TARGET_SUPPORT (1)

/* Peripheral Clock A Reference Value
 * 48 MHz (System default)
 * 64 MHz
 * FIT RI3C Modules works properly on 48 MHz or 64 MHz.
 */
    #define RI3C_CFG_PCLKA_REF_VALUE (48000000)

/**** Bitrate Settings ****
 **** These values has been evaluated to fit RX MCU hardware configurations, change with caution!
 */
/* The Logic High period of SCL during Standard Mode Open Drain transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_STANDARD_OPEN_DRAIN_LOGIC_HIGH_PERIOD (167)

/* The Frequency of SCL during Standard Mode Open Drain transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_STANDARD_OPEN_DRAIN_FREQUENCY (1000000)

/* The Logic High period of SCL during Standard Mode Open Drain transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_STANDARD_PUSH_PULL_LOGIC_HIGH_PERIOD (167)

/* The Frequency of SCL during Standard Mode Push Pull transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_STANDARD_PUSH_PULL_FREQUENCY (3400000)

/* The Logic High period of SCL during Extended Mode Push Pull transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_EXTENDED_OPEN_DRAIN_LOGIC_HIGH_PERIOD (167)

/* The Frequency of SCL during Extended Mode Open Drain transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_EXTENDED_OPEN_DRAIN_FREQUENCY (1000000)

/* The Logic High period of SCL during Extended Mode Open Drain transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_EXTENDED_PUSH_PULL_LOGIC_HIGH_PERIOD (167)

/* The Frequency of SCL during Extended Mode Open Drain transfers.
 * Value must be an integer
 * Value must be greater than 0
 */
    #define RI3C_CFG_EXTENDED_PUSH_PULL_FREQUENCY (3400000)

/* The Open Drain rising time in nanoseconds.
 * Value must be greater than or equal to 0
 */
    #define RI3C_CFG_OPEN_DRAIN_RISING_TIME (0)

/* The Open Drain falling time in nanoseconds.
 * Value must be greater than or equal to 0
 */
    #define RI3C_CFG_OPEN_DRAIN_FALLING_TIME (0)

/* The Push Pull rising time in nanoseconds.
 * Value must be greater than or equal to 0
 */
    #define RI3C_CFG_PUSH_PULL_RISING_TIME (0)

/* The Push Pull falling time in nanoseconds.
 * Value must be greater than or equal to 0
 */
    #define RI3C_CFG_PUSH_PULL_FALLING_TIME (0)

    /* Clock Stalling Configurations */
/* Enable clock stalling during the Address Assignment Phase of ENTDAA.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_ADDRESS_ASSIGNMENT_PHASE (0)

/* Enable clock stalling during the Transition Bit of a read transfer.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_TRANSITION_PHASE (0)

/* Enable clock stalling during the Parity Bit of a write transfer.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_PARITY_PHASE (0)

/* Enable clock stalling during the ACK phase of a transfer.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_ACK_PHASE (0)

/* The amount of time to stall the clock during the Address Assignment Phase, Transition Phase, Parity Phase, and ACK Phase.
 * Value must be an integer
 * Value must be great than or equal 0 and less than PCLKA
 * Default value is 0.
 */
    #define RI3C_CFG_CLOCK_STALLING_TIME (0)

    /* Controller Mode Settings */
/* If enabled, the RI3C instance will ACK Hot-Join Requests and notify the application.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_CONTROLLER_ACK_HOTJOIN_REQ (0)

/* If enabled, the application will get a callback when an IBI Hot-Join Request is rejected.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_CONTROLLER_NOTIFY_REJECTED_HOTJOIN_REQ (0)

/* If enabled, the application will get a callback when an IBI Controller Role Request is rejected.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_CONTROLLER_NOTIFY_REJECTED_CONTROLLERROLE_REQ (0)

/* If enabled, the application will get a callback when an IBI Interrupt Request is rejected.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_CONTROLLER_NOTIFY_REJECTED_INTERRUPT_REQ (0)

/* Configure whether the target can issue IBI requests.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_TARGET_IBI_REQ (0)

/* Configure whether the target can issue Hot-Join requests.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_TARGET_HOTJOIN_REQ (0)

/* Configure whether the target can issue Controller Role requests.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_TARGET_CONTROLLERROLE_REQ (0)

/* Configure whether the Max Read Turnaround time will be transmitted.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_TARGET_INCLUDE_MAX_READ_TURNAROUND_TIME (0)

/* Configure the starting activity state of the target.
 * RI3C_ACTIVITY_STATE_ENTAS0 = 1 microsecond: Latency-free operation (Default value)
 * RI3C_ACTIVITY_STATE_ENTAS1 = 100 microseconds
 * RI3C_ACTIVITY_STATE_ENTAS2 = 2 milliseconds
 * RI3C_ACTIVITY_STATE_ENTAS3 = 50 milliseconds: Lowest-activity operation
 */
    #define RI3C_CFG_TARGET_ENTER_ACTIVITY_STATE (RI3C_ACTIVITY_STATE_ENTAS0)

/* Set the max write length in Target Mode.
 * Write length must be in range of [8, 65535]
 */
    #define RI3C_CFG_TARGET_MAX_WRITE_LENGTH (65535)

/* Set the max read length in Target Mode.
 * Read length must be in range of [8, 65535]
 */
    #define RI3C_CFG_TARGET_MAX_READ_LENGTH (65535)

/* Set the max IBI payload length, or set it to 0 for unlimited.
 * Read length must be in range of [0, 255]
 */
    #define RI3C_CFG_TARGET_MAX_IBI_PAYLOAD_LENGTH (0)

/* Set the max write data rate in Target Mode.
 * RI3C_DATA_RATE_SETTING_FSCL_MAX = FSCL_MAX
 * RI3C_DATA_RATE_SETTING_8MHZ = 8 MHz
 * RI3C_DATA_RATE_SETTING_6MHZ = 6 MHz
 * RI3C_DATA_RATE_SETTING_4MHZ = 4 MHz
 * RI3C_DATA_RATE_SETTING_2MHZ = 2 MHz (Default)
 */
    #define RI3C_CFG_TARGET_WRITE_DATA_RATE (RI3C_DATA_RATE_SETTING_2MHZ)

/* Set the max read data rate in Target Mode.
 * RI3C_DATA_RATE_SETTING_FSCL_MAX = FSCL_MAX
 * RI3C_DATA_RATE_SETTING_8MHZ = 8 MHz
 * RI3C_DATA_RATE_SETTING_6MHZ = 6 MHz
 * RI3C_DATA_RATE_SETTING_4MHZ = 4 MHz
 * RI3C_DATA_RATE_SETTING_2MHZ = 2 MHz (Default)
 */
    #define RI3C_CFG_TARGET_READ_DATA_RATE (RI3C_DATA_RATE_SETTING_2MHZ)

/* Set the max clock to data turnaround time in Target Mode.
 * RI3C_CLOCK_DATA_TURNAROUND_8NS = 8 nanoseconds (Default)
 * RI3C_CLOCK_DATA_TURNAROUND_9NS = 9 nanoseconds
 * RI3C_CLOCK_DATA_TURNAROUND_10NS = 10 nanoseconds
 * RI3C_CLOCK_DATA_TURNAROUND_11NS = 11 nanoseconds
 * RI3C_CLOCK_DATA_TURNAROUND_12NS = 12 nanoseconds
 * RI3C_CLOCK_DATA_TURNAROUND_EXTENDED = Greater than 12 nanoseconds
 */
    #define RI3C_CFG_TARGET_CLK_TURNAROUND_RATE (RI3C_CLOCK_DATA_TURNAROUND_8NS)

/* Configure whether the Max Read Turnaround time will be transmitted in Target Mode.
 * 0 = Disable (System Default)
 * 1 = Enable
 */
    #define RI3C_CFG_TARGET_INCLUDE_MAX_READ_TURNAROUND_TIME (0)

/* Set max read turnaround time in Target Mode.
 * Value must be in the range of [0, 255]
 */
    #define RI3C_CFG_TARGET_MAX_READ_TURNAROUND_TIME (0)

/* Set the internal oscillator frequency in increments of 0.5 MHz in Target Mode.
 * Value must be in the range of [0, 255]
 */
    #define RI3C_CFG_TARGET_FREQUENCY_BYTE (0)

/* Set the oscillator inaccuracy byte in increments of 0.5% in Target Mode.
 * Value must be in the range of [0, 255]
 */
    #define RI3C_CFG_TARGET_INACCURACY_BYTE (0)

/* The minimum period occurring after a STOP and before a START.
 * Must be greater than or equal to 38.4 nanoseconds.
 */
    #define RI3C_CFG_BUS_FREE_DETECT_TIME (38.4)

/* The minimum period occurring after the Bus Free Condition when Targets can initiate IBI requests.
 * Must be greater than or equal to 1 microsecond.
 */
    #define RI3C_CFG_BUS_AVAILABLE_CONDITION_DETECT_TIME (1)

/* The minimum period occurring after the Bus Available Condition when Targets can initiate Hot-Join requests.
 * Must be greater than or equal to 1000 microsecond.
 */
    #define RI3C_CFG_BUS_IDLE_CONDITION_DETECT_TIME (1000)

/* If enabled, the application will get a callback if SCL is stuck at a logic high or logic low level 
 * for more than 65535 cycles of the RI3C source clock.
 * 0 = Disable (System default)
 * 1 = Enable
 */
    #define RI3C_CFG_TIMEOUT_DETECTION (0)

/* Set the interrupt priority level of the RI3C Module.
 * Value must be in the range of [0, 15]
 */
    #define RI3C_CFG_INTERRUPT_PRIORITY_LEVEL (2)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* R_RI3C_CFG_H_ */
