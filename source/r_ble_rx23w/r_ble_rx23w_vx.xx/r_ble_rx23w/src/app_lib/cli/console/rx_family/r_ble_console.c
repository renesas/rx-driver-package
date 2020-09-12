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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name: r_ble_console.c
 * Version : 1.0
 * Description : Command Line Interface Library (For RX23W).
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "r_ble_rx23w_config.h"

#if (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0)

#include "../r_ble_console.h"
#include "r_sci_rx_if.h"

#define BLE_SERIAL_BAUDRATE     (115200)
#define BLE_SERIAL_IRQ_PRIORITY (7)

#if (BLE_CFG_CMD_LINE_CH == 5)
#define BLE_TX_BUFSIZ           (SCI_CFG_CH5_TX_BUFSIZ)
#define BLE_CLI_CH              (SCI_CH5)
#elif (BLE_CFG_CMD_LINE_CH == 8)
#define BLE_TX_BUFSIZ           (SCI_CFG_CH8_TX_BUFSIZ)
#define BLE_CLI_CH              (SCI_CH8)
#elif (BLE_CFG_CMD_LINE_CH == 12)
#define BLE_TX_BUFSIZ           (SCI_CFG_CH12_TX_BUFSIZ)
#define BLE_CLI_CH              (SCI_CH12)
#else /* (BLE_CFG_CMD_LINE_CH == x) */
#define BLE_TX_BUFSIZ           (SCI_CFG_CH1_TX_BUFSIZ)
#define BLE_CLI_CH              (SCI_CH1)
#endif /*  (BLE_CFG_CMD_LINE_CH == x) */

typedef enum {
    BLE_CLI_STATE_NORMAL,
    BLE_CLI_STATE_ESC,
    BLE_CLI_STATE_ESC_SEQ,
} e_ble_cli_state_t;

volatile bool g_cli_tx_flg;
volatile bool g_cli_rx_flg;
static uint8_t state;
static sci_hdl_t serial_hdl;

/*********************************************************************************************************************
 * Function Name: sci_callback
 * Description  : Callback function for SCI event.
 * Arguments    : p_args - Parameters for sci callback function
 * Return Value : None
 ********************************************************************************************************************/
static void sci_callback(void *p_args)
{
    sci_cb_args_t *p_sci_args;

    p_sci_args = (sci_cb_args_t *)p_args;

    switch (p_sci_args->event)
    {
        case SCI_EVT_TEI:
        {
            g_cli_tx_flg = false;
        } break;

        case SCI_EVT_RX_CHAR:
        {
            g_cli_rx_flg = true;
        } break;

        case SCI_EVT_RXBUF_OVFL:
        {
        } break;

        case SCI_EVT_FRAMING_ERR:
        {
        } break;

        case SCI_EVT_OVFL_ERR:
        {
        } break;

        default:
        {
        } break;
    }
}

/*********************************************************************************************************************
 * Function Name: process_normal
 * Description  : Process an input character.
 * Arguments    : c - Input character code
 * Return Value : Character
 ********************************************************************************************************************/
static uint8_t process_normal(uint8_t c)
{
    uint8_t ch = 0x00;

    switch (c)
    {
        case KEY_ESC:
        {
            state = BLE_CLI_STATE_ESC;
        } break;

        default:
        {
            ch = c;
        } break;
    }

    return ch;
}

/*********************************************************************************************************************
 * Function Name: process_escape
 * Description  : Process an escape character.
 * Arguments    : c - Input escape character
 * Return Value : 0x00
 ********************************************************************************************************************/
static uint8_t process_escape(uint8_t c)
{
    state = BLE_CLI_STATE_NORMAL;

    if (c == '[')
    {
        state = BLE_CLI_STATE_ESC_SEQ;
    }

    return 0x00;
}

/*********************************************************************************************************************
 * Function Name: process_escape_sequence
 * Description  : Process an escape sequence.
 * Arguments    : c - Input escape sequence
 * Return Value : KEY_UP or KEY_DOWN or KEY_RIGHT or KEY_LEFT or 0x00
 ********************************************************************************************************************/
static uint8_t process_escape_sequence(uint8_t c)
{
    uint8_t ch = 0x00;

    state = BLE_CLI_STATE_NORMAL;

    switch (c)
    {
        case 'A':
        {
            ch = KEY_UP;
        } break;

        case 'B':
        {
            ch = KEY_DOWN;
        } break;

        case 'C':
        {
            ch = KEY_RIGHT;
        } break;

        case 'D':
        {
            ch = KEY_LEFT;
        } break;

        default:
            break;
    }

    return ch;
}

/*********************************************************************************************************************
 * Function Name: ble_cli_pinset
 * Description  : This function initializes pins for sci module
 * Arguments    : none
 * Return Value : None
 ********************************************************************************************************************/
static void ble_cli_pinset(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

#if (BLE_CFG_CMD_LINE_CH == 1)
    /* Set RXD1/SMISO1 pin */
    MPC.P30PFS.BYTE = 0x0AU;
    PORT3.PMR.BIT.B0 = 1U;

    /* Set TXD1/SMOSI1 pin */
    MPC.P26PFS.BYTE = 0x0AU;
    PORT2.PMR.BIT.B6 = 1U;

#elif (BLE_CFG_CMD_LINE_CH == 5)
    /* Set RXD5/SMISO5 pin */
    MPC.PC2PFS.BYTE = 0x0AU;
    PORTC.PMR.BIT.B2 = 1U;

    /* Set TXD5/SMOSI5 pin */
    MPC.PC3PFS.BYTE = 0x0AU;
    PORTC.PMR.BIT.B3 = 1U;

#elif (BLE_CFG_CMD_LINE_CH == 8)
    /* Set RXD8/SMISO8/SSCL8 pin */
    MPC.PC6PFS.BYTE = 0x0AU;
    PORTC.PMR.BIT.B6 = 1U;

    /* Set TXD8/SMOSI8/SSDA8 pin */
    MPC.PC7PFS.BYTE = 0x0AU;
    PORTC.PMR.BIT.B7 = 1U;

#elif (BLE_CFG_CMD_LINE_CH == 12)
    /* Set RXD12/SMISO12 pin */
    MPC.PE2PFS.BYTE = 0x0CU;
    PORTE.PMR.BIT.B2 = 1U;

    /* Set TXD12/SMOSI12 pin */
    MPC.PE1PFS.BYTE = 0x0CU;
    PORTE.PMR.BIT.B1 = 1U;

#endif /* (BLE_CFG_CMD_LINE_CH == x) */

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

}

/*********************************************************************************************************************
 * Function Name: console_init
 * Description  : Initialize console.
 * Arguments    : none
 * Return Value : none
 ********************************************************************************************************************/
void console_init(void)
{
    sci_cfg_t cfg = {
        .async = {
            .baud_rate    = BLE_SERIAL_BAUDRATE,
            .clk_src      = SCI_CLK_INT,
            .data_size    = SCI_DATA_8BIT,
            .parity_en    = SCI_PARITY_OFF,
            .stop_bits    = SCI_STOPBITS_1,
            .int_priority = BLE_SERIAL_IRQ_PRIORITY,
        },
    };

    R_SCI_Open(BLE_CLI_CH, SCI_MODE_ASYNC, &cfg, &sci_callback, &serial_hdl);

    ble_cli_pinset();

    R_SCI_Control(serial_hdl, SCI_CMD_EN_TEI, NULL);

    g_cli_tx_flg = false;
    g_cli_rx_flg = false;
}

/*********************************************************************************************************************
 * Function Name: console_terminate
 * Description  : Terminate console.
 * Arguments    : none
 * Return Value : none
 ********************************************************************************************************************/
void console_terminate(void)
{
    R_SCI_Close(serial_hdl);
}

/*********************************************************************************************************************
 * Function Name: console_getc
 * Description  : Process a received character.
 * Arguments    : p_c      - Received character code
 *              : p_escape - Escape sequence state 
 * Return Value : Character processing state.
 ********************************************************************************************************************/
bool console_getc(uint8_t *p_c, bool *p_escape)
{
    uint8_t ch;

    *p_c = 0x00;

    while (R_SCI_Receive(serial_hdl, &ch, 1) == SCI_SUCCESS)
    {
        switch (state)
        {
            case BLE_CLI_STATE_NORMAL:
            {
                *p_escape = false;
                *p_c      = process_normal(ch);
            } break;

            case BLE_CLI_STATE_ESC:
            {
                *p_escape = false;
                *p_c      = process_escape(ch);
            } break;

            case BLE_CLI_STATE_ESC_SEQ:
            {
                *p_escape = true;
                *p_c      = process_escape_sequence(ch);
            } break;

            default:
                break;

        }

        if ((*p_c) != 0x00)
        {
            break;
        }
    }

    return (bool)((*p_c) != 0x00);
}

/*********************************************************************************************************************
 * Function Name: console_putsf
 * Description  : Output debug message.
 * Arguments    : p_format      - Debug message
 * Return Value : none
 ********************************************************************************************************************/
void console_putsf(const char *p_format, ...)
{
    uint8_t b[BLE_TX_BUFSIZ] = {0};
    va_list va;
    sci_err_t ret;

    va_start(va, p_format);
    vsnprintf((char *)b, BLE_TX_BUFSIZ, p_format, va);
    va_end(va);

    /* TODO: Remove this after support UART DMA transmission. */    
    do
    {
        ret = R_SCI_Send(serial_hdl, b, (uint16_t)strlen((const char *)b));
    } while ((ret == SCI_ERR_XCVR_BUSY) || (ret == SCI_ERR_INSUFFICIENT_SPACE));

    if( ret == SCI_SUCCESS )
    {
        g_cli_tx_flg = true;
    }
}

#else /* (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

/*********************************************************************************************************************
 * Function Name: console_putsf
 * Description  : Dummy function
 * Arguments    : p_format      - Debug message
 * Return Value : none
 ********************************************************************************************************************/
void console_putsf(const char *p_format, ...)
{
    (void)p_format;
}
#endif /* (BLE_CFG_CMD_LINE_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */
