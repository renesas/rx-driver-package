/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_config.h
* Description  : Configuration header file for the fwupcomm module.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/

#ifndef FWUPCOMM_CONFIG_H_
#define FWUPCOMM_CONFIG_H_

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/*
    0: Checking of parameters in the code at the time of building is omitted.
    1: Checking of parameters in the code at the time of building is included.
    BSP_CFG_PARAM_CHECKING_ENABLE: Use of the default setting for the system.
*/
#define FWUPCOMM_CFG_PARAM_CHECKING_ENABLE          (BSP_CFG_PARAM_CHECKING_ENABLE)

/*
    0: Secondary MCU
    1: Primary MCU
*/
#define FWUPCOMM_CFG_DEVICE_PRIMARY                 (1)

/*
    1 : SCI UART
    10: SCI SPI (Primary Only)
    11: RSPI SPI
 */
#define FWUPCOMM_CFG_CH_INTERFACE                   (1)

#if (FWUPCOMM_CFG_CH_INTERFACE == 1)  /* UART */
/* Sets the SCI channel number to be used for communications. */
#define FWUPCOMM_CFG_SCI_UART_CHANNEL               (1)

/* Sets the bit rate for UART communications. */
#define FWUPCOMM_CFG_SCI_UART_BITRATE               (1000000)

/* Sets the priority of interrupts from the SCI channel to be used for communications. */
#define FWUPCOMM_CFG_SCI_UART_INT_PRIORITY          (15)
#endif  /* UART */

#if (FWUPCOMM_CFG_CH_INTERFACE == 10) || (FWUPCOMM_CFG_CH_INTERFACE == 11)  /* SPI */
/* Sets the SPI channel number to be used for communications. */
#define FWUPCOMM_CFG_SPI_CHANNEL                    (0)

/* Sets the bit rate for SPI communications. */
#define FWUPCOMM_CFG_SPI_BITRATE                    (1000000)

/* Sets the Port and Bit of the SPI MISO pin */
#define FWUPCOMM_CFG_SPI_MISO_PORTNO                'A'
#define FWUPCOMM_CFG_SPI_MISO_BITNO                 '0'

#if (FWUPCOMM_CFG_CH_INTERFACE == 10)  /* SCI SPI only */
/* Sets the priority of interrupts from the SPI channel to be used for communications. */
#define FWUPCOMM_CFG_SPI_INT_PRIORITY               (15)
#endif  /* SCI SPI only */
#endif  /* SPI */

/* Sets the size of the transmission buffer for commands. Set to multiples of 4. */
#define FWUPCOMM_CFG_SEND_PACKET_BUFFER_SIZE        (1048U)

/* Sets the size of the reception buffer for commands. Set to multiples of 4. */
#define FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE        (1048U)

/* Sets a specific address for the device. */
#define FWUPCOMM_CFG_DEVICE_ADDRESS                 (0xA0)

/* Sets the timeout time for sending in communications. */
#define FWUPCOMM_CFG_CMD_SEND_TIMEOUT               (500U)

/* Sets the timeout time for receiving in communications. */
#define FWUPCOMM_CFG_CMD_RECV_TIMEOUT               (500U)

/* Select whether or not to enable Common command. */
#define FWUPCOMM_CFG_CMD_COMMON_ENABLE              (1)

/* Sets the name of the handler function to be called when a Common command is received. */
#define FWUPCOMM_CFG_CMD_HANDLER_COMMON             (R_FWUPCOMM_CmdHandler_Common)

/* Select whether or not to enable FWUP command. */
#define FWUPCOMM_CFG_CMD_FWUP_ENABLE                (1)

/* Sets the name of the handler function to be called when an FWUP command is received. */
#define FWUPCOMM_CFG_CMD_HANDLER_FWUP               (R_FWUPCOMM_CmdHandler_FWUP)

/* Sets the version number of commands. */
#define FWUPCOMM_CFG_CMD_VER                        (0x01)

/* Sets the size of command data to be included with the FWUP START command. Set to multiples of 4. */
#define FWUPCOMM_CFG_CMD_FWUP_START_DATA_SIZE       (0U)

/* Sets the size of the block of firmware to be included with the FWUP WRITE command. Set to multiples of 4. */
#define FWUPCOMM_CFG_CMD_FWUP_WRITE_FW_BLOCK_SIZE   (1024U)

/* Sets the maximum size of data to be included with a Common command. Set to multiples of 4. */
#define FWUPCOMM_CFG_CMD_COMMON_MAX_DATA_SIZE       (12U)


#endif /* FWUPCOMM_CONFIG_H_ */
