/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.03.2021 1.00     First Release
*              : 20.09.2021 1.10     Fixed R_QSPIX_Write_Indirect()
*                                    Added the demo for RX671
*              : 29.11.2021 1.20     Supported to call R_QSPIX_Write_Indirect() 
*                                    multiple times
*******************************************************************************/
/*******************************************************************************
* File Name    : r_qspix_rx_if.h
* Description  : Functions for using QSPIX on RX devices.
*******************************************************************************/
#ifndef R_QSPIX_RX_IF_H
#define R_QSPIX_RX_IF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "r_qspix_rx_private.h"
#include "r_qspix_rx_config.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Version Number of API. */
#define QSPIX_VERSION_MAJOR           (1)
#define QSPIX_VERSION_MINOR           (20)

/* Definition of QSPIX channel */
#define QSPIX_CH0                         (0)
#define QSPIX_NUM_CH                      (1)

/* Definition address of QSPIX ROM window */
#define QSPI_DEVICE_START_ADDRESS   (0x70000000)
#define QSPIX_BANK_MASK             (26)
/*******************************************************************************
Typedef definitions
*******************************************************************************/
/* QSPIX  API error codes */
typedef enum e_qspix_err       /* QSPIX API error codes */
{
    QSPIX_SUCCESS,                /* QSPIX processing completed without problem. */
    QSPIX_ERR_OPENED,             /* QSPIX was initialized already. */
    QSPIX_ERR_NOT_OPEN,           /* QSPIX module is not initialized yet. */
    QSPIX_ERR_INVALID_ARG,        /* Arguments are invalid. */
    QSPIX_ERR_INVALID_COMMAND,    /* Command parameters are invalid. Or, forced data change failed. */
    QSPIX_ERR_NULL_PTR,           /* Argument pointers are NULL. */
    QSPIX_ERR_BUSY,               /* The QSPIX resources are locked by another process. */
    QSPIX_ERR_HW                  /* HW error. */
} qspix_err_t;

/* QSPIX_GetStatus() ARGUMENT DEFINITIONS */

/* commands */
typedef enum e_qspix_cmd
{
    QSPIX_GET_PREFETCH_BUFFER_FILL_LEVEL, /* Get Prefetch Buffer Fill Level */
    QSPIX_GET_PREFETCH_BUFFER_FULL_FLAG, /* Get Prefetch Buffer Full Flag */
    QSPIX_GET_PREFETCH_FUNCTION_OPERATING_STATUS_FLAG, /* Get Prefetch Function
                                                       Operating Status Flag */
    QSPIX_GET_BUS_BUSY_FLAG,         /* Get Bus Busy Flag */
    QSPIX_GET_ROM_ACCESS_ERROR_FLAG, /* Get ROM Access Error Flag */
    QSPIX_GET_XIP_STATUS_FLAG        /* Get XIP Status Flag */
} qspix_cmd_t;

/* Open() DEFINITIONS */
typedef enum e_qspix_read_mode /* Read Instruction Select */
{
    QSPIX_READ_MODE_STANDARD              = 0, /* Standard Read Mode */
    QSPIX_READ_MODE_FAST_READ             = 1, /* Fast Read Mode */
    QSPIX_READ_MODE_FAST_READ_DUAL_OUTPUT = 2, /* Fast Read Dual Output Mode */
    QSPIX_READ_MODE_FAST_READ_DUAL_IO     = 3, /* Fast Read Dual I/O Mode */
    QSPIX_READ_MODE_FAST_READ_QUAD_OUTPUT = 4, /* Fast Read Quad Output Mode */
    QSPIX_READ_MODE_FAST_READ_QUAD_IO     = 5  /* Fast Read Quad I/O Mode */
} qspix_read_mode_t;

typedef enum e_qspix_slave_select_extension /* Slave Select Extension */
{
    QSPIX_DO_NOT_EXTEND_QSSL        = 0, /* 0 0: Do not extend QSSL */
    QSPIX_EXTEND_QSSL_BY_33_QSPCLK  = 1, /* 0 1: Extend QSSL by 33 QSPCLK */
    QSPIX_EXTEND_QSSL_BY_129_QSPCLK = 2, /* 1 0: Extend QSSL by 129 QSPCLK */
    QSPIX_EXTEND_QSSL_INFINITELY    = 3  /* 1 1: Extend QSSL infinitely */
} qspix_slave_select_extension_t;

typedef enum e_qspix_prefetch_function
{
    QSPIX_PREFETCH_DISABLE = 0, /* 0: Disable function */
    QSPIX_PREFETCH_ENABLE = 1   /* 1: Enable function */
} qspix_prefetch_function_t;

typedef enum e_qspix_clock_mode /* Clock Mode Select */
{
    QSPIX_SPI_MODE_0 = 0, /* 0: SPI mode 0 */
    QSPIX_SPI_MODE_3 = 1  /* 1: SPI mode 3 */
} qspix_clock_mode_t;

typedef enum e_qspix_data_output_select /* Data Output Duration Extension */
{
    QSPIX_DO_NOT_EXTEND = 0,     /* 0: Do not extend */
    QSPIX_EXTEND_BY_1_QSPCLK = 1 /* 1: Extend by 1 QSPCLK */
} qspix_data_output_select_t;

typedef enum e_qspix_special_instruction_select /* Special Read Instruction Select */
{
    QSPIX_DEFAULT_INSTRUCTION_CODE = 0,          /* 0: Set default instruction
                                                    code for each instruction */
    QSPIX_INSTRUCTION_CODE_IN_SPRIR_REGISTER = 1 /* 1: Write instruction code in
                                                    the SPRIR register */
} qspix_special_instruction_select_t;

typedef enum e_qspix_slave_select_high_width /* Slave Select High Width Setting */
{
    QSPIX_1_CYCLE_QSPCLK = 0,   /* 0 0 0 0: 1 cycle of QSPCLK */
    QSPIX_2_CYCLE_QSPCLK = 1,   /* 0 0 0 1: 2 cycles of QSPCLK */
    QSPIX_3_CYCLE_QSPCLK = 2,   /* 0 0 1 0: 3 cycles of QSPCLK */
    QSPIX_4_CYCLE_QSPCLK = 3,   /* 0 0 1 1: 4 cycles of QSPCLK */
    QSPIX_5_CYCLE_QSPCLK = 4,   /* 0 1 0 0: 5 cycles of QSPCLK */
    QSPIX_6_CYCLE_QSPCLK = 5,   /* 0 1 0 1: 6 cycles of QSPCLK */
    QSPIX_7_CYCLE_QSPCLK = 6,   /* 0 1 1 0: 7 cycles of QSPCLK */
    QSPIX_8_CYCLE_QSPCLK = 7,   /* 0 1 1 1: 8 cycles of QSPCLK */
    QSPIX_9_CYCLE_QSPCLK = 8,   /* 1 0 0 0: 9 cycles of QSPCLK */
    QSPIX_10_CYCLE_QSPCLK = 9,  /* 1 0 0 1: 10 cycles of QSPCLK */
    QSPIX_11_CYCLE_QSPCLK = 10, /* 1 0 1 0: 11 cycles of QSPCLK */
    QSPIX_12_CYCLE_QSPCLK = 11, /* 1 0 1 1: 12 cycles of QSPCLK */
    QSPIX_13_CYCLE_QSPCLK = 12, /* 1 1 0 0: 13 cycles of QSPCLK */
    QSPIX_14_CYCLE_QSPCLK = 13, /* 1 1 0 1: 14 cycles of QSPCLK */
    QSPIX_15_CYCLE_QSPCLK = 14, /* 1 1 1 0: 15 cycles of QSPCLK */
    QSPIX_16_CYCLE_QSPCLK = 15  /* 1 1 1 1: 16 cycles of QSPCLK */
} qspix_slave_select_high_width_t;

typedef enum e_qspix_slave_select_hold_time /* Slave Select Hold Time Setting */
{
    QSPIX_RELEASE_QSSL_0_5_QSPCLK = 0, /* 0: Release QSSL 0.5 QSPCLK cycles after
                                        the last rising edge of QSPCLK */
    QSPIX_RELEASE_QSSL_1_5_QSPCLK = 1  /* 1: Release QSSL 1.5 QSPCLK cycles after
                                        the last rising edge of QSPCLK */
} qspix_slave_select_hold_time_t;


typedef enum e_qspix_slave_select_setup_time /* Slave Select Setup Time Setting */
{
    QSPIX_OUTPUT_QSSL_0_5_QSPCLK = 0, /* 0: Output QSSL 0.5 QSPCLK cycles before
                                        the first rising edge of QSPCLK */
    QSPIX_OUTPUT_QSSL_1_5_QSPCLK = 1  /* 1: Output QSSL 1.5 QSPCLK cycles before
                                        the first rising edge of QSPCLK */
} qspix_slave_select_setup_time_t;

typedef enum e_qspix_clock_divisor
 {
    QSPIX_ICLK_DIV_2 = 0,  /* ICLK / 2  */
    QSPIX_ICLK_DIV_3 = 1,  /* ICLK / 3  */
    QSPIX_ICLK_DIV_4 = 2,  /* ICLK / 4  */
    QSPIX_ICLK_DIV_5 = 3,  /* ICLK / 5  */
    QSPIX_ICLK_DIV_6 = 4,  /* ICLK / 6  */
    QSPIX_ICLK_DIV_7 = 5,  /* ICLK / 7  */
    QSPIX_ICLK_DIV_8 = 6,  /* ICLK / 8  */
    QSPIX_ICLK_DIV_9 = 7,  /* ICLK / 9  */
    QSPIX_ICLK_DIV_10 = 8, /* ICLK / 10 */
    QSPIX_ICLK_DIV_11 = 9, /* ICLK / 11 */
    QSPIX_ICLK_DIV_12 = 10,/* ICLK / 12 */
    QSPIX_ICLK_DIV_13 = 11,/* ICLK / 13 */
    QSPIX_ICLK_DIV_14 = 12,/* ICLK / 14 */
    QSPIX_ICLK_DIV_15 = 13,/* ICLK / 15 */
    QSPIX_ICLK_DIV_16 = 14,/* ICLK / 16 */
    QSPIX_ICLK_DIV_17 = 15,/* ICLK / 17 */
    QSPIX_ICLK_DIV_18 = 16,/* ICLK / 18 */
    QSPIX_ICLK_DIV_20 = 17,/* ICLK / 20 */
    QSPIX_ICLK_DIV_22 = 18,/* ICLK / 22 */
    QSPIX_ICLK_DIV_24 = 19,/* ICLK / 24 */
    QSPIX_ICLK_DIV_26 = 20,/* ICLK / 26 */
    QSPIX_ICLK_DIV_28 = 21,/* ICLK / 28 */
    QSPIX_ICLK_DIV_30 = 22,/* ICLK / 30 */
    QSPIX_ICLK_DIV_32 = 23,/* ICLK / 32 */
    QSPIX_ICLK_DIV_34 = 24,/* ICLK / 34 */
    QSPIX_ICLK_DIV_36 = 25,/* ICLK / 36 */
    QSPIX_ICLK_DIV_38 = 26,/* ICLK / 38 */
    QSPIX_ICLK_DIV_40 = 27,/* ICLK / 40 */
    QSPIX_ICLK_DIV_42 = 28,/* ICLK / 42 */
    QSPIX_ICLK_DIV_44 = 29,/* ICLK / 44 */
    QSPIX_ICLK_DIV_46 = 30,/* ICLK / 46 */
    QSPIX_ICLK_DIV_48 = 31 /* ICLK / 48 */
 } qspix_clock_divisor_t;

typedef enum e_qspix_duty_cycle_correction /* Duty Cycle Correction */
{
    QSPIX_MAKE_NO_CORRECTION = 0,    /* 0: Make no correction */
    QSPIX_CORRECT_THE_DUTY_CYCLE = 1 /* 1: Correct the duty cycle when
                                        the divisor is odd number */
} qspix_duty_cycle_correction_t;

typedef enum e_qspix_access_mode_select /* Flash Memory Access Mode Select */
{
    QSPIX_MEMORY_MAPPED_MODE = 0,  /* 0: Memory mapped mode */
    QSPIX_INDIRECT_ACCESS_MODE = 1 /* 1: Indirect access mode */
} qspix_access_mode_select_t;

typedef enum e_qspix_address_size /* Address Size Setting */
{
    QSPIX_1_BYTE = 0,  /* 0 0: 1 byte */
    QSPIX_2_BYTES = 1, /* 0 1: 2 bytes */
    QSPIX_3_BYTES = 2, /* 1 0: 3 bytes */
    QSPIX_4_BYTES = 3  /* 1 1: 4 bytes */
} qspix_address_size_t;

typedef enum e_qspix_instruction_4_Byte_address /* Instruction with 4-Byte AddressEnable */
{
    QSPIX_INSTRUCTION_4_BYTE_ADDRESS_DISABLE = 0, /* 0: Do not use 4-byte
                                                address read instruction code */
    QSPIX_INSTRUCTION_4_BYTE_ADDRESS_ENABLE = 1   /* 1: Use 4-byte address read
                                                            instruction code */
} qspix_instruction_4_Byte_address_t;

typedef enum e_qspix_dummy_clocks /* Number of Dummy Cycle */
{
    QSPIX_DEFAULT_DUMMY_CYCLES = 0, /* 0 0 0 0: Default dummy cycles
                                    for each instruction */
    QSPIX_3_QSPCLK = 1,   /* 0 0 0 1: 3 QSPCLK */
    QSPIX_4_QSPCLK = 2,   /* 0 0 1 0: 4 QSPCLK */
    QSPIX_5_QSPCLK = 3,   /* 0 0 1 1: 5 QSPCLK */
    QSPIX_6_QSPCLK = 4,   /* 0 1 0 0: 6 QSPCLK */
    QSPIX_7_QSPCLK = 5,   /* 0 1 0 1: 7 QSPCLK */
    QSPIX_8_QSPCLK = 6,   /* 0 1 1 0: 8 QSPCLK */
    QSPIX_9_QSPCLK = 7,   /* 0 1 1 1: 9 QSPCLK */
    QSPIX_10_QSPCLK = 8,  /* 1 0 0 0: 10 QSPCLK */
    QSPIX_11_QSPCLK = 9,  /* 1 0 0 1: 11 QSPCLK */
    QSPIX_12_QSPCLK = 10, /* 1 0 1 0: 12 QSPCLK */
    QSPIX_13_QSPCLK = 11, /* 1 0 1 1: 13 QSPCLK */
    QSPIX_14_QSPCLK = 12, /* 1 1 0 0: 14 QSPCLK */
    QSPIX_15_QSPCLK = 13, /* 1 1 0 1: 15 QSPCLK */
    QSPIX_16_QSPCLK = 14, /* 1 1 1 0: 16 QSPCLK */
    QSPIX_17_QSPCLK = 15  /* 1 1 1 1: 17 QSPCLK */
} qspix_dummy_clocks_t;

typedef enum e_qspix_xip_function
{
    QSPIX_XIP_DISABLE = 0,
    QSPIX_XIP_ENABLE = 1
} qspix_xip_function_t;

typedef enum e_qspix_protocol /* I/O Mode Select */
{
    QSPIX_EXTENDED_SPI_PROTOCOL = 0, /* 0 0: Extended SPI protocol */
    QSPIX_DUAL_SPI_PROTOCOL = 1,     /* 0 1: Dual SPI protocol */
    QSPIX_QUAD_SPI_PROTOCOL = 2      /* 1 0: Quad SPI protocol */
} qspix_protocol_t;

typedef enum e_qspix_WP_pin_control /* WP Pin Control */
{
    QSPIX_LOW_LEVEL = 0, /* 0: Low level */
    QSPIX_HIGH_LEVEL = 1 /* 1: High level */
} qspix_WP_pin_control_t;

/* Status operation of QSPIX */
typedef enum e_qspix_status
{
    /* QSPIX is closing */
    QSPIX_CLOSE = 0,

    /* QSPIX is opening */
    QSPIX_OPEN = 1

} qspix_status_t;



/*----- Structure type. -----*/
/* QSPIX config structure */
typedef struct st_qspix_cfg
{
    /* Read Instruction Select */
    qspix_read_mode_t                       read_mode;

    /* Slave Select Extension */
    qspix_slave_select_extension_t          slave_select_extension;

    /* Slave Select Extension */
    qspix_prefetch_function_t               prefetch_function;

    /* Clock Mode Select */
    qspix_clock_mode_t                      clock_mode;

    /* Data Output Duration Extension */
    qspix_data_output_select_t              data_output_select;

    /* Special Read Instruction Select */
    qspix_special_instruction_select_t      special_instruction_select;

    /* Slave Select High Width Setting */
    qspix_slave_select_high_width_t         slave_select_high_width;

    /* Slave Select Hold Time Setting */
    qspix_slave_select_hold_time_t          slave_select_hold_time;

    /* Slave Select Setup Time Setting */
    qspix_slave_select_setup_time_t         slave_select_setup_time;

    /* Clock Divisor Select */
    qspix_clock_divisor_t                   clock_divisor;

    /* Special Read Instruction Setting used when
       special_instruction_select = QSPIX_INSTRUCTION_CODE_IN_SPRIR_REGISTER */
    uint32_t qspix_special_read_instruction;

    /* Address Size Setting */
    qspix_address_size_t                    address_size;

    /* Instruction with 4-Byte Address Enable */
    qspix_instruction_4_Byte_address_t      instruction_4_Byte_address;

    /* Number of Dummy Cycle */
    qspix_dummy_clocks_t                    dummy_clocks;

    /* SPI protocol */
    qspix_protocol_t                        protocol;

    /* WP Pin Control */
    qspix_WP_pin_control_t                  WP_pin_control;

    void (*p_callback)(void *p_cbdat);    /* pointer to user callback function. */

} qspix_cfg_t;

typedef struct st_qspix_hdl
{
    qspix_cfg_t p_cfg;            // Pointer to initial configuration
    qspix_status_t open;             // Whether or not driver is open
} qspix_hdl_t;
/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
/* QSPIX Driver API functions */
qspix_err_t     R_QSPIX_Open(uint8_t channel, qspix_cfg_t *p_cfg);
qspix_err_t     R_QSPIX_Close (uint8_t channel);
qspix_err_t     R_QSPIX_Control(uint8_t channel, qspix_cfg_t *p_cfg);
qspix_err_t     R_QSPIX_Read_Indirect(uint8_t channel,
                                      uint8_t *p_src_addr,
                                      uint32_t bytes);
qspix_err_t     R_QSPIX_Write_Indirect(uint8_t channel,
                                       uint8_t *p_src_addr,
                                       uint32_t bytes,
                                       bool read_after_write);
qspix_err_t     R_QSPIX_Enter_XIP(uint8_t channel, uint8_t mode);
qspix_err_t     R_QSPIX_Exit_XIP(uint8_t channel, uint8_t mode);
qspix_err_t     R_QSPIX_BankSet (uint8_t channel, uint8_t bank);
qspix_err_t     R_QSPIX_Set_Spi_Protocol(uint8_t channel, qspix_protocol_t protocol);
qspix_err_t     R_QSPIX_Get_Status(qspix_cmd_t cmd, uint8_t *return_status);
uint32_t        R_QSPIX_GetVersion(void);

#endif /* R_QSPIX_RX_IF_H */

/* End of File */

