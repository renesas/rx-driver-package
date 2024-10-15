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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_elc_rx261_private.h
 * Version      : 1.0
 * Description  : Functions for using Event Link Controller module
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           31.07.2023 1.0     Initial Release
 ***********************************************************************************************************************/
#ifndef ELC_RX261_PRIVATE_H
    #define ELC_RX261_PRIVATE_H
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
    #include "r_elc_rx_config.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
    #define ELC_FUNCTION_ICU_LPT            (1)
    #define ELC_FUNCTION_ICU1               (1)
    #define ELC_FUNCTION_ICU2               (1)

    #define ELC_SFR_ADDR_OFFSET             (21)    /* sfr address offset for ELSR46 */

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
    ELC_CMT_CMP1            = 0x1F,
    ELC_TMR0_CMPA0          = 0x22,
    ELC_TMR0_CMPB0          = 0x23,
    ELC_TMR0_OVF            = 0x24,
    ELC_TMR2_CMPA2          = 0x28,
    ELC_TMR2_CMPB2          = 0x29,
    ELC_TMR2_OVF            = 0x2A,
    ELC_RTC_PRD             = 0x2E,
    ELC_IWDT_UDF            = 0x31,
    ELC_LPT_CMP0            = 0x32,
    ELC_LPT_CMP1            = 0x33,
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
    ELC_RSPI0_ER0           = 0x52,
    ELC_RSPI0_IDLE          = 0x53,
    ELC_RSPI0_RX0           = 0x54,
    ELC_RSPI0_TX0           = 0x55,
    ELC_RSPI0_TE0           = 0x56,
    ELC_S12AD_S12AD0        = 0x58,
    ELC_CMPB_CMPB0          = 0x59,
    ELC_CMPB_CMPB0_CMPB1    = 0x5A,
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
    ELC_GPTW0_CMPA          = 0x80,
    ELC_GPTW0_CMPB          = 0x81,
    ELC_GPTW0_CMPC          = 0x82,
    ELC_GPTW0_CMPD          = 0x83,
    ELC_GPTW0_CMPE          = 0x84,
    ELC_GPTW0_CMPF          = 0x85,
    ELC_GPTW0_OVF           = 0x86,
    ELC_GPTW0_UDF           = 0x87,
    ELC_GPTW0_ADTRA         = 0x88,
    ELC_GPTW0_ADTRB         = 0x89,
    ELC_GPTW1_CMPA          = 0x8A,
    ELC_GPTW1_CMPB          = 0x8B,
    ELC_GPTW1_CMPC          = 0x8C,
    ELC_GPTW1_CMPD          = 0x8D,
    ELC_GPTW1_CMPE          = 0x8E,
    ELC_GPTW1_CMPF          = 0x8F,
    ELC_GPTW1_OVF           = 0x90,
    ELC_GPTW1_UDF           = 0x91,
    ELC_GPTW1_ADTRA         = 0x92,
    ELC_GPTW1_ADTRB         = 0x93,
    ELC_GPTW2_CMPA          = 0x94,
    ELC_GPTW2_CMPB          = 0x95,
    ELC_GPTW2_CMPC          = 0x96,
    ELC_GPTW2_CMPD          = 0x97,
    ELC_GPTW2_CMPE          = 0x98,
    ELC_GPTW2_CMPF          = 0x99,
    ELC_GPTW2_OVF           = 0x9A,
    ELC_GPTW2_UDF           = 0x9B,
    ELC_GPTW2_ADTRA         = 0x9C,
    ELC_GPTW2_ADTRB         = 0x9D,
    ELC_GPTW3_CMPA          = 0x9E,
    ELC_GPTW3_CMPB          = 0x9F,
    ELC_GPTW3_CMPC          = 0xA0,
    ELC_GPTW3_CMPD          = 0xA1,
    ELC_GPTW3_CMPE          = 0xA2,
    ELC_GPTW3_CMPF          = 0xA3,
    ELC_GPTW3_OVF           = 0xA4,
    ELC_GPTW3_UDF           = 0xA5,
    ELC_GPTW4_CMPA          = 0xA6,
    ELC_GPTW4_CMPB          = 0xA7,
    ELC_GPTW4_CMPC          = 0xA8,
    ELC_GPTW4_CMPD          = 0xA9,
    ELC_GPTW4_CMPE          = 0xAA,
    ELC_GPTW4_CMPF          = 0xAB,
    ELC_GPTW4_OVF           = 0xAC,
    ELC_GPTW4_UDF           = 0xAD,
    ELC_GPTW5_CMPA          = 0xAE,
    ELC_GPTW5_CMPB          = 0xAF,
    ELC_GPTW5_CMPC          = 0xB0,
    ELC_GPTW5_CMPD          = 0xB1,
    ELC_GPTW5_CMPE          = 0xB2,
    ELC_GPTW5_CMPF          = 0xB3,
    ELC_GPTW5_OVF           = 0xB4,
    ELC_GPTW5_UDF           = 0xB5,
    ELC_GPTW6_CMPA          = 0xAE,
    ELC_GPTW6_CMPB          = 0xAF,
    ELC_GPTW6_CMPC          = 0xB0,
    ELC_GPTW6_CMPD          = 0xB1,
    ELC_GPTW6_CMPE          = 0xB2,
    ELC_GPTW6_CMPF          = 0xB3,
    ELC_GPTW6_OVF           = 0xB4,
    ELC_GPTW6_UDF           = 0xB5,
    ELC_GPTW7_CMPA          = 0xBE,
    ELC_GPTW7_CMPB          = 0xBF,
    ELC_GPTW7_CMPC          = 0xC0,
    ELC_GPTW7_CMPD          = 0xC1,
    ELC_GPTW7_CMPE          = 0xC2,
    ELC_GPTW7_CMPF          = 0xC3,
    ELC_GPTW7_OVF           = 0xC4,
    ELC_GPTW7_UDF           = 0xC5,
	ELC_GPTWOPS_UVW         = 0xC6
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
    ELC_CMT1       = 7, 
    ELC_ICU_LPT    = 8, 
    ELC_TMR0       = 10,
    ELC_TMR2       = 12,
    ELC_CTSU       = 14,
    ELC_S12AD      = 15,    /* ELC_S12AD For previous version compatibility */
    ELC_S12AD_TRG00= 15,
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
    ELC_GPTWA      = (48 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWB      = (49 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWC      = (50 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWD      = (51 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWE      = (52 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWF      = (53 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWG      = (54 + ELC_SFR_ADDR_OFFSET),
    ELC_GPTWH      = (55 + ELC_SFR_ADDR_OFFSET),
	ELC_S12AD_TRG01= (56 + ELC_SFR_ADDR_OFFSET),

    /* For the min/max limitation value */
    ELC_MODULE_MIN = ELC_CMT1,           /* Minimum value defined by elc_module_t */
    ELC_MODULE_MAX = ELC_S12AD_TRG01           /* Maximum value defined by elc_module_t */
} elc_module_t;

typedef enum
{
    ELC_TIMER_START         = 0,
    ELC_TIMER_RESTART       = 1,
    ELC_TIMER_EVENT_COUNTER = 2,
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
    ELC_EVT_ICU2, 
    ELC_EVT_ICU_LPT
} elc_icu_t;

#endif /* ELC_RX261_PRIVATE_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
