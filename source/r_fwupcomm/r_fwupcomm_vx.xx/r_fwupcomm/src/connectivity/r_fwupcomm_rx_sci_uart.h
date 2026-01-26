/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_rx_sci_uart.h
 * Description  : Functions for the RX MCU to communicate using SCI UART.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.05.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_RX_SCI_UART_H_
#define FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_RX_SCI_UART_H_

#include "r_fwupcomm_ch.h"
#include "r_fwupcomm_config.h"

#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"


#if FWUPCOMM_CFG_SCI_UART_CHANNEL == (0)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI0()
#define FWUPCOMM_SCI_CH    SCI_CH0
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (1)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI1()
#define FWUPCOMM_SCI_CH    SCI_CH1
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (2)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI2()
#define FWUPCOMM_SCI_CH    SCI_CH2
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (3)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI3()
#define FWUPCOMM_SCI_CH    SCI_CH3
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (4)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI4()
#define FWUPCOMM_SCI_CH    SCI_CH4
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (5)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI5()
#define FWUPCOMM_SCI_CH    SCI_CH5
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (6)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI6()
#define FWUPCOMM_SCI_CH    SCI_CH6
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (7)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI7()
#define FWUPCOMM_SCI_CH    SCI_CH7
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (8)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI8()
#define FWUPCOMM_SCI_CH    SCI_CH8
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (9)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI9()
#define FWUPCOMM_SCI_CH    SCI_CH9
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (10)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI10()
#define FWUPCOMM_SCI_CH    SCI_CH10
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (11)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI11()
#define FWUPCOMM_SCI_CH    SCI_CH11
#elif FWUPCOMM_CFG_SCI_UART_CHANNEL == (12)
#define FWUPCOMM_SCI_PINSET()  R_SCI_PinSet_SCI12()
#define FWUPCOMM_SCI_CH    SCI_CH12
#else
#error "Error! Invalid setting for FWUPCOMM_CFG_SCI_UART_CHANNEL in r_fwupcomm_config.h"
#endif

/* Structure variable defining the interface for packet communications */
extern r_fwupcomm_ch_api_t const r_fwupcomm_ch_api;

/* Communications interface functions */
fwupcomm_err_t r_fwupcomm_rx_sci_uart_open (r_fwupcomm_hdl_t * hdl);
void           r_fwupcomm_rx_sci_uart_close (r_fwupcomm_hdl_t * hdl);
fwupcomm_err_t r_fwupcomm_rx_sci_uart_send (r_fwupcomm_hdl_t * hdl, uint8_t * const src, uint16_t size);
fwupcomm_err_t r_fwupcomm_rx_sci_uart_recv (r_fwupcomm_hdl_t * hdl, uint8_t * const dest, uint16_t size);
void           r_fwupcomm_rx_sci_uart_rx_reset (r_fwupcomm_hdl_t * hdl);

#endif /* FWUPCOMM_SRC_CONNECTIVITY_R_FWUPCOMM_RX_SCI_UART_H_ */
