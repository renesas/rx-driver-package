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
 * File Name    : r_elc_rx65n_private.h
 * Version      : 1.0
 * Description  : Functions for using Event Link Controller module
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           12.09.2016 1.0     Initial Release
 ***********************************************************************************************************************/
#ifndef ELC_RX65N_PRIVATE_H
    #define ELC_RX65N_PRIVATE_H
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
    #include "r_elc_rx_config.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
    #define ELC_FUNCTION_ICU_LPT            (0)
    #define ELC_FUNCTION_ICU1               (1)
    #define ELC_FUNCTION_ICU2               (1)

    #define ELC_SFR_ADDR_OFFSET             (15)    /* sfr address offset for ESLR33-45 */
/******************************************************************************
 Typedef definitions
 ******************************************************************************/
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
    ELC_MTU0_CMP0A          = 0x01,
    ELC_MTU0_CMP0B          = 0x02,
    ELC_MTU0_CMP0C          = 0x03,
    ELC_MTU0_CMP0D          = 0x04,
    ELC_MTU0_CMP0E          = 0x05,
    ELC_MTU0_CMP0F          = 0x06,
    ELC_MTU0_OVF            = 0x07,
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
    ELC_TMR1_CMPA1          = 0x25,
    ELC_TMR1_CMPB1          = 0x26,
    ELC_TMR1_OVF            = 0x27,
    ELC_TMR2_CMPA2          = 0x28,
    ELC_TMR2_CMPB2          = 0x29,
    ELC_TMR2_OVF            = 0x2A,
    ELC_TMR3_CMPA3          = 0x2B,
    ELC_TMR3_CMPB3          = 0x2C,
    ELC_TMR3_OVF            = 0x2D,
    ELC_RTC_PRD             = 0x2E,
    ELC_IWDT_UDF            = 0x31,
    ELC_SCI5_ER5            = 0x3A,
    ELC_SCI5_RX5            = 0x3B,
    ELC_SCI5_TX5            = 0x3C,
    ELC_SCI5_TE5            = 0x3D,
    ELC_RIIC0_ER0           = 0x4E,
    ELC_RIIC0_RX0           = 0x4F,
    ELC_RIIC0_TX0           = 0x50,
    ELC_RIIC0_TE0           = 0x51,
    ELC_RSPI0_ER0           = 0x52,
    ELC_RSPI0_IDLE          = 0x53,
    ELC_RSPI0_RX0           = 0x54,
    ELC_RSPI0_TX0           = 0x55,
    ELC_RSPI0_TE0           = 0x56,
    ELC_S12AD_S12AD0        = 0x58,
    ELC_LVD1_LVD1           = 0x5B,
    ELC_LVD2_LVD2           = 0x5C,
    ELC_DMAC0_DMAC0         = 0x5D,
    ELC_DMAC1_DMAC1         = 0x5E,
    ELC_DMAC2_DMAC2         = 0x5F,
    ELC_DMAC3_DMAC3         = 0x60,
    ELC_DTC_DTC             = 0x61,
    ELC_CGC_OSTD            = 0x62,
    ELC_PORT_PGR1           = 0x63,
    ELC_PORT_PGR2           = 0x64,
    ELC_PORT_PSP0           = 0x65,
    ELC_PORT_PSP1           = 0x66,
    ELC_PORT_PSP2           = 0x67,
    ELC_PORT_PSP3           = 0x68,
    ELC_ELC_SEG             = 0x69,
    ELC_DOC_DOPCF           = 0x6A,
    ELC_S12AD_S12AD1        = 0x6C,
    ELC_CMT_CMPW            = 0x7E,
    ELC_TPU0_CMPA           = 0xAC,
    ELC_TPU0_CMPB           = 0xAD,
    ELC_TPU0_CMPC           = 0xAE,
    ELC_TPU0_CMPD           = 0xAF,
    ELC_TPU0_OVF            = 0xB0,
    ELC_TPU1_CMPA           = 0xB1,
    ELC_TPU1_CMPB           = 0xB2,
    ELC_TPU1_OVF            = 0xB3,
    ELC_TPU1_UDF            = 0xB4,
    ELC_TPU2_CMPA           = 0xB5,
    ELC_TPU2_CMPB           = 0xB6,
    ELC_TPU2_OVF            = 0xB7,
    ELC_TPU2_UDF            = 0xB8,
    ELC_TPU3_CMPA           = 0xB9,
    ELC_TPU3_CMPB           = 0xBA,
    ELC_TPU3_CMPC           = 0xBB,
    ELC_TPU3_CMPD           = 0xBC,
    ELC_TPU3_OVF            = 0xBD
    
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
    ELC_PSB_PE0 = 0x10,
    ELC_PSB_PE1 = 0x11,
    ELC_PSB_PE2 = 0x12,
    ELC_PSB_PE3 = 0x13,
    ELC_PSB_PE4 = 0x14,
    ELC_PSB_PE5 = 0x15,
    ELC_PSB_PE6 = 0x16,
    ELC_PSB_PE7 = 0x17
} elc_single_port_select_t;

typedef enum
{
    ELC_MTU0       = 0,
    ELC_MTU3       = 3,
    ELC_MTU4       = 4,
    ELC_CMT1       = 7,
    ELC_TMR0       = 10,
    ELC_TMR1       = 11,
    ELC_TMR2       = 12,
    ELC_TMR3       = 13,
    ELC_S12AD      = 15,
    ELC_DA0        = 16,
    ELC_ICU1       = 18,
    ELC_ICU2       = 19,
    ELC_OUT_PGR1   = 20,
    ELC_OUT_PGR2   = 21,
    ELC_IN_PGR1    = 22,
    ELC_IN_PGR2    = 23,
    ELC_PSP0       = 24,
    ELC_PSP1       = 25,
    ELC_PSP2       = 26,
    ELC_PSP3       = 27,
    ELC_CGC_LOCO   = 28,
    ELC_CMTW0      = (33 + ELC_SFR_ADDR_OFFSET),
    ELC_TPU0       = (35 + ELC_SFR_ADDR_OFFSET),
    ELC_TPU1       = (36 + ELC_SFR_ADDR_OFFSET),
    ELC_TPU2       = (37 + ELC_SFR_ADDR_OFFSET),
    ELC_TPU3       = (38 + ELC_SFR_ADDR_OFFSET),
    ELC_S12AD1     = (45 + ELC_SFR_ADDR_OFFSET),

    /* For the min/max limitation value */
    ELC_MODULE_MIN = ELC_MTU0,           /* Minimum value defined by elc_module_t */
    ELC_MODULE_MAX = ELC_S12AD1          /* Maximum value defined by elc_module_t */
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
    ELC_PORT_GROUP1,
    ELC_PORT_GROUP2
} elc_portbuffer_t;

typedef enum
{
    ELC_EVT_ICU1,
    ELC_EVT_ICU2
} elc_icu_t;

#endif /* ELC_RX65N_PRIVATE_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
