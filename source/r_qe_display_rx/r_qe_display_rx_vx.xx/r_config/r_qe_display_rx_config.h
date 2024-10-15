/**********************************************************************************************************************
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
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_qe_display_rx_config.h
 * Version      : 1.00
 * Description  : Configuration header file of QE for Display module.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 19.07.2024 1.00     First Release
 **********************************************************************************************************************/

#ifndef R_QE_DISPLAY_RX_CONFIG_H
#define R_QE_DISPLAY_RX_CONFIG_H

/***********************************************************************************************************************
 * Configuration Options
 **********************************************************************************************************************/


/* Select SCI channel number.
 * Setting range: 0 to 12
 */
#define QE_DISPLAY_CFG_UART_CH  (0)


/* Specify UART baud rate.
 * Set the baud rate to be the same as the QE for Display settings.
 * The maximum baud rate is device dependent.
 * Refer to the user's manual for device used.
 */
#define QE_DISPLAY_CFG_UART_BAUDRATE (115200)


/* Interrupt priority level of SCI interrupt.
 *  Setting range: 0 to 15
 */
#define QE_DISPLAY_CFG_UART_INTERRUPT_PRIORITY_LEVEL (5)


/* Select whether to send communication and adjustment error messages to the standard output
 * (normally the console of the development environment connected to the debugger).
 *  0 : No error message will be sent.
 *  1 : Sends an error message.
 */
#define QE_DISPLAY_CFG_DEBUG_PRINT_ENABLE (0)


#endif /* R_QE_DISPLAY_RX_CONFIG_H */

/* End of File */
