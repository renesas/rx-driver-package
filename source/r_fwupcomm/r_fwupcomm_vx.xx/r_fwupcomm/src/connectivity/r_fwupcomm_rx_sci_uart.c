/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_rx_sci_uart.c
 * Description  : Functions for the RX MCU to communicate using SCI UART.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.05.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_fwupcomm_private.h"

#if (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_SCI_UART)

/* Structure variable defining the interface for packet communications. */
const r_fwupcomm_ch_api_t r_fwupcomm_ch_api =
{
    .open = r_fwupcomm_rx_sci_uart_open,
    .close = r_fwupcomm_rx_sci_uart_close,
    .send = r_fwupcomm_rx_sci_uart_send,
    .recv = r_fwupcomm_rx_sci_uart_recv,
    .rx_reset = r_fwupcomm_rx_sci_uart_rx_reset
};

static sci_hdl_t     s_fwupcomm_rx_sci_uart_hdl;
static bool          s_fwupcomm_sci_ch_open_flag = false;
volatile static bool s_fwupcomm_sci_tx_end_flag  = true;

static void r_fwupcomm_rx_sci_uart_cb (void * p_args);

/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_sci_uart_open
 * Description  : Opens a communications channel.
 * Return Value : FWUPCOMM_SUCCESS            : The channel was successfully initialized.
 *              : FWUPCOMM_ERR_CH_ALREADY_OPEN: The communications channel has already been opened.
 *              : FWUPCOMM_ERR_NOT_OPEN       : Initializing the communications channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t r_fwupcomm_rx_sci_uart_open(r_fwupcomm_hdl_t * hdl)
{
    sci_err_t sci_err = SCI_SUCCESS;
    sci_cfg_t sci_cfg =
    {
        .async =
        {
            .baud_rate = FWUPCOMM_CFG_SCI_UART_BITRATE,
            .clk_src = SCI_CLK_INT,
            .data_size = SCI_DATA_8BIT,
            .parity_en = SCI_PARITY_OFF,
            .stop_bits = SCI_STOPBITS_1,
            .int_priority = FWUPCOMM_CFG_SCI_UART_INT_PRIORITY
        }
    };

    if (true == s_fwupcomm_sci_ch_open_flag)
    {
        return FWUPCOMM_ERR_CH_ALREADY_OPEN;
    }

    sci_err = R_SCI_Open(FWUPCOMM_SCI_CH,
                            SCI_MODE_ASYNC,
                            &sci_cfg,
                            &r_fwupcomm_rx_sci_uart_cb,
                            &s_fwupcomm_rx_sci_uart_hdl);

    if (SCI_SUCCESS != sci_err)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

    FWUPCOMM_SCI_PINSET();

    s_fwupcomm_sci_tx_end_flag  = true;
    s_fwupcomm_sci_ch_open_flag = true;

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_sci_uart_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_sci_uart_close
 * Description  : Closes a communications channel.
 * Return Value : None
 *********************************************************************************************************************/
void r_fwupcomm_rx_sci_uart_close(r_fwupcomm_hdl_t * hdl)
{
    R_SCI_Close(s_fwupcomm_rx_sci_uart_hdl);

    s_fwupcomm_sci_tx_end_flag  = true;
    s_fwupcomm_sci_ch_open_flag = false;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_sci_uart_close
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_sci_uart_send
 * Description  : Sends data by using a communications channel.
 * Arguments    : src : Pointer to the destination for storing data to be sent
 *              : size: Size of data to be sent
 * Return Value : FWUPCOMM_SUCCESS:          The channel was successfully initialized.
 *              : FWUPCOMM_ERR_INVALID_PTR:  The src pointer is NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG:  size is 0.
 *              : FWUPCOMM_ERR_NOT_OPEN:     The communications channel has not been opened.
 *              : FWUPCOMM_ERR_CH_SEND_BUSY: The communications channel was busy so sending of data failed.
 *              : FWUPCOMM_ERR_CH_SEND:      Sending of data in the communications channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t r_fwupcomm_rx_sci_uart_send(r_fwupcomm_hdl_t * hdl,
                                           uint8_t * const src, uint16_t size)
{
    sci_err_t sci_err = SCI_SUCCESS;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == src)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
    if (0 == size)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }
#endif

    if (false == s_fwupcomm_sci_ch_open_flag)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

    if (true != s_fwupcomm_sci_tx_end_flag)
    {
        return FWUPCOMM_ERR_CH_SEND_BUSY;
    }

    s_fwupcomm_sci_tx_end_flag = false;

    sci_err = R_SCI_Send(s_fwupcomm_rx_sci_uart_hdl, src, size);

    if (SCI_SUCCESS != sci_err)
    {
        s_fwupcomm_sci_tx_end_flag = true;
        return FWUPCOMM_ERR_CH_SEND;
    }

    /* WAIT_LOOP */
    while (false == s_fwupcomm_sci_tx_end_flag)
    {
        ;
    }

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_sci_uart_send
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_sci_uart_recv
 * Description  : Receives data by using a communications channel.
 * Arguments    : dest: Pointer to the buffer for storing received data
 *              : size: Required size of received data
 * Return Value : FWUPCOMM_SUCCESS:             The channel was successfully initialized.
 *              : FWUPCOMM_ERR_INVALID_PTR:     The dest pointer is NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG:     size is 0.
 *              : FWUPCOMM_ERR_NOT_OPEN:        The communications channel has not been opened.
 *              : FWUPCOMM_ERR_CH_RECV_NO_DATA: The communications channel does not have enough received data.
 *********************************************************************************************************************/
fwupcomm_err_t r_fwupcomm_rx_sci_uart_recv(r_fwupcomm_hdl_t * hdl,
                                           uint8_t * const dest, uint16_t size)
{
    sci_err_t sci_err = SCI_SUCCESS;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == dest)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
    if (0 == size)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }
#endif

    if (false == s_fwupcomm_sci_ch_open_flag)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

    sci_err = R_SCI_Receive(s_fwupcomm_rx_sci_uart_hdl,
                            dest,
                            size);

    if (SCI_SUCCESS != sci_err)
    {
        return FWUPCOMM_ERR_CH_RECV_NO_DATA;
    }

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_sci_uart_recv
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_sci_uart_rx_reset
 * Description  : Empties the reception buffer of the communications channel.
 * Return Value : None
 *********************************************************************************************************************/
void r_fwupcomm_rx_sci_uart_rx_reset(r_fwupcomm_hdl_t * hdl)
{
    R_SCI_Control(s_fwupcomm_rx_sci_uart_hdl,
                    SCI_CMD_RX_Q_FLUSH,
                    NULL);
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_sci_uart_rx_reset
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_rx_sci_uart_cb
 * Description  : Callback functions for SCI FIT module.
 * Argument     : p_args: Pointer to the argument passed from the SCI FIT module
 * Return Value : None
 *********************************************************************************************************************/
static void r_fwupcomm_rx_sci_uart_cb(void * p_args)
{
    sci_cb_args_t * p_sci_args = (sci_cb_args_t *)p_args;  /* Casting to sci_cb_args_t * type is valid */

    if (SCI_EVT_TEI == p_sci_args->event)
    {
        s_fwupcomm_sci_tx_end_flag = true;
    }
    else
    {
        ;
    }
}
/**********************************************************************************************************************
 End of function r_fwupcomm_rx_sci_uart_cb
 *********************************************************************************************************************/

#endif /* (FWUPCOMM_USE_CH == FWUPCOMM_CH_RX_SCI_UART) */
