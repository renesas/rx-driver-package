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
* Copyright (C) 2013 (2014-2019) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rspi_rx_if.h
* Description  : Functions for using RSPI on RX devices. 
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 21.02.2013 1.00     First Release
*         : 02.05.2014 1.10     Changed some comments and removed commented-out code.
*         : 05.05.2014 1.20     Version number change
*         : 05.16.2014 1.21     Added RX62T
*         : 19.01.2014 1.30     Added support for RX113, RX64M, and RX71M
*         : 26.06.2015 1.40     Added support for RX231
*         : 30.09.2016 1.50     Added support for RX130, RX230, RX23T, RX24T and RX65N. 
*         :                     Added typedef of rspi_spcmd_dummy_t.
*         :                     Modified typedef of rspi_command_word_t.
*         : 31.03.2017 1.60     Added support for RX24U.
*         : 31.07.2017 1.70     Added support for RX65N-2MB and RX130-512KB.
*         : 20.09.2018 1.80     Supported RX66T.
*         : 20.12.2018 2.00     Added double buffer and dmadtc transfer mode.
*         : 20.05.2019 2.01     Added support for GNUC and ICCRX.
*                               Fixed coding style. 
*         : 20.06.2019 2.02     Supported RX23W.
*         : 30.07.2019 2.03     Supported RX72M.
*         : 22.11.2019 2.04     Supported RX72N and RX66N.
***********************************************************************************************************************/

#ifndef RSPI_API_HEADER_FILE
#define RSPI_API_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* User settable configuration options for the RSPI code */
#include "r_rspi_rx_config.h"

/* Defines of default settings used by the RSPI code */
#include "./src/r_rspi_defaults.h"
/* Internal definitions. */
#include "./src/r_rspi_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define RSPI_RX_VERSION_MAJOR           (2)
#define RSPI_RX_VERSION_MINOR           (04)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* The following enumeration lists the possible error codes that can be returned by the API functions. */
typedef enum rspi_err_e       // RSPI API error codes
{
    RSPI_SUCCESS = 0,
    RSPI_ERR_BAD_CHAN,       // Invalid channel number.
    RSPI_ERR_CH_NOT_OPENED,  // Channel not yet opened.
    RSPI_ERR_CH_NOT_CLOSED,  // Channel still open from previous open.
    RSPI_ERR_UNKNOWN_CMD,    // Control command is not recognized.
    RSPI_ERR_INVALID_ARG,    // Argument is not valid for parameter.
    RSPI_ERR_ARG_RANGE,      // Argument is out of range for parameter.
    RSPI_ERR_NULL_PTR,       // Received null pointer; missing required argument.
    RSPI_ERR_LOCK,           // The lock procedure failed.
    RSPI_ERR_UNDEF,          // Undefined/unknown error
} rspi_err_t;

typedef enum {
    RSPI_EVT_TRANSFER_COMPLETE,  // The data transfer completed.
    RSPI_EVT_TRANSFER_ABORTED,   // The data transfer was aborted.
    RSPI_EVT_ERR_MODE_FAULT,     // Mode fault error.
    RSPI_EVT_ERR_READ_OVF,       // Read overflow.
    RSPI_EVT_ERR_PARITY,         // Parity error.
    RSPI_EVT_ERR_UNDER_RUN,      // Under run error.
    RSPI_EVT_ERR_UNDEF           // Undefined/unknown error event.
} rspi_evt_t;

/************ Type defines used with the R_RSPI_Open function. ***************/
/* Define for data transfer mode */
typedef enum e_rspi_str_tranmode
{
    RSPI_TRANS_MODE_SW         = 0,            // Data transfer mode is software.
    RSPI_TRANS_MODE_DMAC,                      // Data transfer mode is DMAC.
    RSPI_TRANS_MODE_DTC                        // Data transfer mode is DTC.
} rspi_str_tranmode_t;
/* Hardware interface mode configuration settings. */
typedef enum
{
    RSPI_IF_MODE_3WIRE  =  RSPI_SPCR_SPMS, // Use GPIO for slave select.
    RSPI_IF_MODE_4WIRE  = ~RSPI_SPCR_SPMS  // Use slave select signals controlled by RSPI.
} rspi_interface_mode_t;

/* Master or slave operating mode configuration settings. */
typedef enum
{
    RSPI_MS_MODE_MASTER =  RSPI_SPCR_MSTR, // Channel operates as SPI master
    RSPI_MS_MODE_SLAVE  = ~RSPI_SPCR_MSTR, // Channel operates as SPI slave
} rspi_master_slave_mode_t;

/* Abstraction of channel handle data structure.
 * User application will use this as a reference to an opened channel. */
typedef struct rspi_config_block_s *rspi_handle_t;

typedef struct rspi_callback_data_s
{
    rspi_handle_t handle;
    rspi_evt_t    event_code;
}rspi_callback_data_t;

typedef struct rspi_chnl_settings_s
{
    rspi_interface_mode_t gpio_ssl; // RSPI_IF_MODE_4WIRE: RSPI slave selects, RSPI_IF_MODE_3WIRE: GPIO slave selects.
    rspi_master_slave_mode_t master_slave_mode; // RSPI_MS_MODE_MASTER or RSPI_MS_MODE_SLAVE.
    uint32_t    bps_target;         // The target bits per second setting value for the channel.
    rspi_str_tranmode_t tran_mode;  // Data transfer mode
} rspi_chnl_settings_t;

/************ Type defines used with the R_RSPI_Control function. ***************/
/* Control function command codes. */
typedef enum rspi_cmd_e
{
    RSPI_CMD_SET_BAUD = 1,
    RSPI_CMD_ABORT,     // Stop the current read or write operation immediately.
    RSPI_CMD_SETREGS,   // Set all supported RSPI regs in one operation. Expert use only!
    RSPI_CMD_SET_TRANS_MODE,// Set the data transfer mode.
    RSPI_CMD_UNKNOWN    // Not a valid command.
} rspi_cmd_t;

/* Data structure for the Set Baud command. */
typedef struct rspi_cmd_baud_s
{
    uint32_t    bps_target;    // The target bits per second setting value for the channel.
} rspi_cmd_baud_t;

/* Advanced use! Entries for use in setting RSPI registers that control operation.
 * To be used with the R_RSPI_Control()-RSPI_CMD_SETREGS command.
 * Values will be copied directly into the associated register when used.
 * To use this with the RSPI_CMD_SETREGS command, user creates an instance of this with
 * settings as required, and passes a pointer to it as an argument in the call to R_RSPI_Control(). */
typedef struct rspi_cmd_setregs_s
{
    uint8_t sslp_val;   // RSPI Slave Select Polarity Register (SSLP)
    uint8_t sppcr_val;  // RSPI Pin Control Register (SPPCR)
    uint8_t spckd_val;  // RSPI Clock Delay Register (SPCKD)
    uint8_t sslnd_val;  // RSPI Slave Select Negation Delay Register (SSLND)
    uint8_t spnd_val;   // RSPI Next-Access Delay Register (SPND)
    uint8_t spcr2_val;  // RSPI Control Register 2 (SPCR2)
    uint8_t spdcr2_val; // RSPI Data Control Register 2 (SPDCR2)
} rspi_cmd_setregs_t;

/* Data structure for the Set Transfer mode command. */
typedef struct rspi_cmd_trans_mode_s
{
    rspi_str_tranmode_t    transfer_mode;    // The transfer mode setting value for the channel.
} rspi_cmd_trans_mode_t;

/*************************************************************************************************
 * Type defines used with the R_RSPI_Write, R_RSPI_Read, and R_RSPI_WriteRead functions.
 * Enums are provided for each field of the command word for write and read operations.
 * A command word must be formed by complete initialization of a structure of these.
 * This word is passed as an argument to the R_RSPI_Write, R_RSPI_Read, and R_RSPI_WriteRead functions
 * and will get copied to the SPCMD register with each call. */

/* Clock phase and polarity */
typedef enum {
    RSPI_SPCMD_CPHA_SAMPLE_ODD = (0x0),  // 0: Data sampling on odd edge, data variation on even edge.
    RSPI_SPCMD_CPHA_SAMPLE_EVEN  = (0x1)   // 1: Data variation on odd edge, data sampling on even edge.
} rspi_spcmd_cpha_t;

typedef enum {
    RSPI_SPCMD_CPOL_IDLE_LO = (0x0),      // 0: RSPCK is low when idle.
    RSPI_SPCMD_CPOL_IDLE_HI = (0x1)       // 1: RSPCK is high when idle.
} rspi_spcmd_cpol_t;

/* Clock base rate division */
typedef enum
{
    RSPI_SPCMD_BR_DIV_1 = (0x0),    // Select the base bit rate
    RSPI_SPCMD_BR_DIV_2 = (0x1),    // Select the base bit rate divided by 2
    RSPI_SPCMD_BR_DIV_4 = (0x2),    // Select the base bit rate divided by 4
    RSPI_SPCMD_BR_DIV_8 = (0x3)     // Select the base bit rate divided by 8
} rspi_spcmd_br_div_t;

/* Slave select to be asserted during transfer operation. */
typedef enum
{
    RSPI_SPCMD_ASSERT_SSL0 = (0x0),    // Select SSL0
    RSPI_SPCMD_ASSERT_SSL1 = (0x1),    // Select SSL1
    RSPI_SPCMD_ASSERT_SSL2 = (0x2),    // Select SSL2
    RSPI_SPCMD_ASSERT_SSL3 = (0x3)     // Select SSL3
} rspi_spcmd_ssl_assert_t;

/* Slave select negation. */
typedef enum
{
    RSPI_SPCMD_SSL_NEGATE = (0x0),   // 0: Negates all SSL signals upon completion of transfer
    RSPI_SPCMD_SSL_KEEP  = (0x1)    // 1: Keep SSL signal level from end of transfer until start of next.
} rspi_spcmd_ssl_negation_t;

/* Frame data length */
typedef enum
{
    RSPI_SPCMD_BIT_LENGTH_8  = (0x7),    // 0100 to 0111 = 8 bits data length
    RSPI_SPCMD_BIT_LENGTH_9  = (0x8),    // 1000 = 9 bits data length
    RSPI_SPCMD_BIT_LENGTH_10 = (0x9),    // 1001 = 10 bits data length
    RSPI_SPCMD_BIT_LENGTH_11 = (0xA),    // 1010 = 11 bits data length
    RSPI_SPCMD_BIT_LENGTH_12 = (0xB),    // 1011 = 12 bits data length
    RSPI_SPCMD_BIT_LENGTH_13 = (0xC),    // 1100 = 13 bits data length
    RSPI_SPCMD_BIT_LENGTH_14 = (0xD),    // 1101 = 14 bits data length
    RSPI_SPCMD_BIT_LENGTH_15 = (0xE),    // 1110 = 15 bits data length
    RSPI_SPCMD_BIT_LENGTH_16 = (0xF),    // 1111 = 16 bits data length
    RSPI_SPCMD_BIT_LENGTH_20 = (0x0),    // 0000 = 20 bits data length
    RSPI_SPCMD_BIT_LENGTH_24 = (0x1),    // 0001 = 24 bits data length
    RSPI_SPCMD_BIT_LENGTH_32 = (0x3)     // 0011 = 32 bits data length
} rspi_spcmd_bit_length_t;

/* Data transfer bit order. */
typedef enum
{
    RSPI_SPCMD_ORDER_MSB_FIRST = (0x0),    // 0: MSB first.
    RSPI_SPCMD_ORDER_LSB_FIRST = (0x1)    // 1: LSB first.
} rspi_spcmd_bit_order_t;

/* RSPI signal delays */
typedef enum
{
    RSPI_SPCMD_NEXT_DLY_1     = (0x0),  // 0: A next-access delay of 1 RSPCK +2 PCLK.
    RSPI_SPCMD_NEXT_DLY_SSLND = (0x1),  // 1: Next-access delay = RSPI nextaccess delay register (SPND)
} rspi_spcmd_spnden_t;

typedef enum
{
    RSPI_SPCMD_SSL_NEG_DLY_1     = (0x0),  // 0: An SSL negation delay of 1 RSPCK.
    RSPI_SPCMD_SSL_NEG_DLY_SSLND = (0x1),  // 1: Delay = slave select negation delay register (SSLND)
} rspi_spcmd_slnden_t;

typedef enum
{
    RSPI_SPCMD_CLK_DLY_1         = (0x0),  // 0: An RSPCK delay of 1 RSPCK.
    RSPI_SPCMD_CLK_DLY_SPCKD     = (0x1)   // 1: Delay = setting of RSPI clock delay register (SPCKD).
} rspi_spcmd_sckden_t;

typedef enum
{
    RSPI_SPCMD_DUMMY = (0xffff)
} rspi_spcmd_dummy_t;

/* The complete command word data structure. This contains one from each of the
 * above fields in the correct order to set all the bits of the SPCMD register. */
typedef union rspi_command_word_s
{
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_11(
        rspi_spcmd_cpha_t         cpha          :1,
        rspi_spcmd_cpol_t         cpol          :1,
        rspi_spcmd_br_div_t       br_div        :2,
        rspi_spcmd_ssl_assert_t   ssl_assert    :3,
        rspi_spcmd_ssl_negation_t ssl_negate    :1,
        rspi_spcmd_bit_length_t   bit_length    :4,
        rspi_spcmd_bit_order_t    bit_order     :1,
        rspi_spcmd_spnden_t       next_delay    :1,
        rspi_spcmd_slnden_t       ssl_neg_delay :1,
        rspi_spcmd_sckden_t       clock_delay   :1,
        rspi_spcmd_dummy_t        dummy         :16
    );
    uint16_t word[2];
} rspi_command_word_t;

/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
rspi_err_t   R_RSPI_Open(uint8_t               channel,
                         rspi_chnl_settings_t *pconfig,
                         rspi_command_word_t  spcmd_command_word,
                         void                (*pcallback)(void *pcbdat),
                         rspi_handle_t        *phandle);

rspi_err_t  R_RSPI_Control(rspi_handle_t  handle,
                           rspi_cmd_t     cmd,
                           void          *pcmd_data);

rspi_err_t  R_RSPI_Read(rspi_handle_t        handle,
                        rspi_command_word_t  spcmd_command_word,
                        void                *pdest,
                        uint16_t             length);

rspi_err_t  R_RSPI_Write(rspi_handle_t        handle,
                         rspi_command_word_t  spcmd_command_word,
                         void                *psrc,
                         uint16_t             length);

rspi_err_t  R_RSPI_WriteRead(rspi_handle_t        handle,
                             rspi_command_word_t  spcmd_command_word,
                             void                *psrc,
                             void                *pdest,
                             uint16_t             length);

rspi_err_t  R_RSPI_Close(rspi_handle_t handle);


uint32_t  R_RSPI_GetVersion(void);

rspi_err_t R_RSPI_GetBuffRegAddress(rspi_handle_t handle,
                                    uint32_t *p_spdr_adr);

rspi_err_t R_RSPI_IntSptiIerClear(rspi_handle_t handle);

rspi_err_t R_RSPI_IntSpriIerClear(rspi_handle_t handle);
                                        
rspi_err_t R_RSPI_SetLogHdlAddress(uint32_t user_long_que);

#endif /* RSPI_API_HEADER_FILE */
