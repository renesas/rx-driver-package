/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_elc_rx130_private.h
 * Version      : 1.0
 * Description  : Functions for using Event Link Controller module
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           01.07.2016 1.0     Initial Release
 ***********************************************************************************************************************/
#ifndef ELC_RX130_PRIVATE_H
    #define ELC_RX130_PRIVATE_H
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
    #include "r_elc_rx_config.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
    #define ELC_FUNCTION_ICU_LPT            (1)
    #define ELC_FUNCTION_ICU1               (1)
    #define ELC_FUNCTION_ICU2               (0)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
/* R_ELC_Control command */
typedef enum
{
    ELC_CMD_START,
    ELC_CMD_STOP,
    ELC_CMD_CLEAR_EVENTLINK,
    ELC_CMD_WRITE_PORTBUFFER,
    ELC_CMD_READ_PORTBUFFER,
    ELC_CMD_SOFTWARE_EVENT
} elc_eventlink_cmd_t;

typedef enum
{
    ELC_MTU1_CMP1A          = 0x08,
    ELC_MTU1_CMP1B          = 0x09,
    ELC_MTU1_OVF            = 0x0A,
    ELC_MTU1_UDF            = 0x0B,
    ELC_MTU2_CMP2A          = 0x0C,
    ELC_MTU2_CMP2B          = 0x0D,
    ELC_MTU2_OVF            = 0x0E,
    ELC_MTU2_UDF            = 0x0F,
    ELC_MTU3_CMP3A          = 0x10,
    ELC_MTU3_CMP3B          = 0x11,
    ELC_MTU3_CMP3C          = 0x12,
    ELC_MTU3_CMP3D          = 0x13,
    ELC_MTU3_OVF            = 0x14,
    ELC_MTU4_CMP4A          = 0x15,
    ELC_MTU4_CMP4B          = 0x16,
    ELC_MTU4_CMP4C          = 0x17,
    ELC_MTU4_CMP4D          = 0x18,
    ELC_MTU4_OVF            = 0x19,
    ELC_MTU4_UDF            = 0x1A,
    ELC_CMT_CMP1            = 0x1F,
    ELC_TMR0_CMPA0          = 0x22,
    ELC_TMR0_CMPB0          = 0x23,
    ELC_TMR0_OVF            = 0x24,
    ELC_TMR2_CMPA2          = 0x28,
    ELC_TMR2_CMPB2          = 0x29,
    ELC_TMR2_OVF            = 0x2A,
    ELC_LPT_CMP0            = 0x32,
    ELC_S12AD_WMELC         = 0x34,
    ELC_S12AD_WUMELC        = 0x35,
    ELC_SCI5_ER5            = 0x3A,
    ELC_SCI5_RX5            = 0x3B,
    ELC_SCI5_TX5            = 0x3C,
    ELC_SCI5_TE5            = 0x3D,
    ELC_RIIC0_ER0           = 0x4E,
    ELC_RIIC0_RX0           = 0x4F,
    ELC_RIIC0_TX0           = 0x50,
    ELC_RIIC0_TE0           = 0x51,
    ELC_S12AD_S12AD0        = 0x58,
    ELC_CMPB_CMPB0          = 0x59,
    ELC_CMPB_CMPB0_CMPB1    = 0x5A,
    ELC_LVD1_LVD1           = 0x5B,
    ELC_DTC_DTC             = 0x61,
    ELC_PORT_PGR1           = 0x63,
    ELC_PORT_PSP0           = 0x65,
    ELC_PORT_PSP1           = 0x66,
    ELC_ELC_SEG             = 0x69,
    ELC_DOC_DOPCF           = 0x6A
} elc_eventlink_signal_t;

typedef enum
{
    ELC_EDGE_RISING,
    ELC_EDGE_FALLING,
    ELC_EDGE_RISING_AND_FALLING
} elc_port_trigger_select_t;

typedef enum
{
    ELC_PSB_PB0 = 0x08,
    ELC_PSB_PB1 = 0x09,
    ELC_PSB_PB2 = 0x0A,
    ELC_PSB_PB3 = 0x0B,
    ELC_PSB_PB4 = 0x0C,
    ELC_PSB_PB5 = 0x0D,
    ELC_PSB_PB6 = 0x0E,
    ELC_PSB_PB7 = 0x0F,
} elc_single_port_select_t;

typedef enum
{
    ELC_MTU1       = 1,
    ELC_MTU2       = 2,
    ELC_MTU3       = 3,
    ELC_MTU4       = 4,
    ELC_CMT1       = 7,
    ELC_ICU_LPT    = 8,
    ELC_TMR0       = 10,
    ELC_TMR2       = 12,
    ELC_CTSU       = 14,
    ELC_S12AD      = 15,
    ELC_DA0        = 16,
    ELC_ICU1       = 18,
    ELC_OUT_PGR1   = 20,
    ELC_IN_PGR1    = 22,
    ELC_PSP0       = 24,
    ELC_PSP1       = 25,

    /* For the min/max limitation value */
    ELC_MODULE_MIN = ELC_MTU1,           /* Minimum value defined by elc_module_t */
    ELC_MODULE_MAX = ELC_PSP1            /* Maximum value defined by elc_module_t */
} elc_module_t;

typedef enum
{
    ELC_TIMER_START         = 0,
    ELC_TIMER_RESTART       = 1,
    ELC_TIMER_INPUT_CAPTURE = 2,
    ELC_TIMER_DISABLED      = 3
} elc_timer_operation_select_t;

typedef enum
{
    ELC_PORT_LOW        = 0,
    ELC_PORT_HIGH       = 1,
    ELC_PORT_TOGGLE     = 2,
    ELC_PORT_BUFFER     = 3,
    ELC_PORT_ROTATE     = 4
} elc_port_level_select_t;

typedef enum
{
    ELC_PDBF_OVERWRITE_DISABLE = 0,
    ELC_PDBF_OVERWRITE_ENABLE  = 1
} elc_port_buffer_select_t;

typedef void (*elc_interrupt_set_t) (void *pdata); /* Callback function type */

typedef enum
{
    ELC_PORT_GROUP1
} elc_portbuffer_t;

typedef enum
{
    ELC_EVT_ICU1, 
    ELC_EVT_ICU_LPT
} elc_icu_t;

#endif /* ELC_RX130_PRIVATE_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
