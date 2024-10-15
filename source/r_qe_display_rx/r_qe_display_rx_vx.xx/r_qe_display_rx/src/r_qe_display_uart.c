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
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_qe_display_uart.c
 * Version      : 1.10
 * Description  : Communication control related processing with QE.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version   Description
 *         : 19.07.2024 1.00      First Release
 *         : 20.09.2024 1.10      Fixed the calculation process for the number of divided receptions
 *                                in the r_uart_callback function.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "r_qe_display_private.h"

#if (USE_DMAC == QE_DISPLAY_UART_DMA_ENABLE)
#include "r_dmaca_rx_if.h"
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/* == Static variables ============================================================================================= */

/* Receive data buffer */
static uint8_t s_uart_id_cmd_buffer;
static uint8_t s_uart_data_buffer[QE_DISPLAY_SIZE_REV_COMMAND_ALL];

/* Transmission */
static uint8_t s_uart_response;
static uint8_t s_uart_max_data_size[2];

static uint8_t s_uart_verify_data_buf[QE_DISPLAY_SIZE_RES_VERIFY_DATA];
static uint8_t s_uart_verify_data[] = "QE_DISPLAY";

/* Command */
static uint8_t s_uart_command;

/* Sequence status */
static uint8_t s_uart_status = QE_DISPLAY_STS_CLOSE;

/* SCI configuration variable */
static sci_cfg_t s_uart_config;
static sci_hdl_t s_uart_control;


#if (USE_INTERRUPT == QE_DISPLAY_UART_DMA_ENABLE)
/* Receive interrupt count number */
static volatile uint8_t s_uart_intterupt_count;

static uint16_t s_uart_remain_data_size;
static uint16_t s_uart_split_num;
static uint16_t s_uart_buf_index;
#else
static uint16_t s_uart_rev_size;
#endif

/* CMT interrupt counter */
static volatile uint8_t s_uart_timeout_count;


/* == Static functions ============================================================================================== */

static void r_uart_reinit (void);

static bool r_uart_data_verify (void);
static void r_uart_send_verify_data (void);
static void r_uart_send_max_data_size (void);

static void r_uart_timeout_operation (bool operation);
static void r_uart_timeout_callback (void * p_data);
static void r_uart_callback (void * p_args);



/* Function Name: r_display_status_check */
/*******************************************************************************************************************//**
 * @brief Process according to status.
 * @param  None
 * @retval QE_DISPLAY_SUCCESS            Processing has been completed successfully.
 * @retval QE_DISPLAY_ERR_NOT_OPEN       R_QE_DISPLAY_Open function has not been executed.
 * @retval QE_DISPLAY_ERR_LCD_OTHER      Other LCD errors.
 * @retval QE_DISPLAY_ERR_UART_OTHER     Other serial communication errors.
 **********************************************************************************************************************/
qe_display_err_t r_display_status_check(void)
{
    qe_display_err_t display_ret = QE_DISPLAY_SUCCESS;

    switch (s_uart_status)
    {
        case QE_DISPLAY_STS_UART_CONNECTION:

            if (r_uart_data_verify())
            {
                r_uart_send_verify_data();
                display_ret = QE_DISPLAY_SUCCESS;

                r_set_uart_status(QE_DISPLAY_STS_IDLE);
                r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
            }
            else
            {
                r_uart_send_response(QE_DISPLAY_RES_NACK);
                display_ret = QE_DISPLAY_ERR_UART_OTHER;

                r_set_uart_status(QE_DISPLAY_STS_IDLE);
                r_set_uart_command(QE_DISPLAY_COMMAND_NULL);

                r_debug_print("UART Error: Connection failure!!\n");
            }

            break;

        case QE_DISPLAY_STS_LCD_REINIT:

            display_ret = r_display_glcdc_reinit();

            r_set_uart_status(QE_DISPLAY_STS_IDLE);
            r_set_uart_command(QE_DISPLAY_COMMAND_NULL);

            break;

        case QE_DISPLAY_STS_ERR_ID:

            r_uart_reinit();

            display_ret = QE_DISPLAY_ERR_UART_OTHER;

            r_debug_print("UART Error: Identifier Receive!!\n");

            break;

        case QE_DISPLAY_STS_ERR_COMMAND:

            r_uart_reinit();

            display_ret = QE_DISPLAY_ERR_UART_OTHER;

            r_debug_print("UART Error: Command Receive!!\n");

            break;

        case QE_DISPLAY_STS_ERR_RECEIVE_DATA:

            r_uart_reinit();

            display_ret = QE_DISPLAY_ERR_UART_OTHER;

            r_debug_print("UART Error: Data Receive!!\n");

            break;

        case QE_DISPLAY_STS_ERR_TIMEOUT:

            r_uart_reinit();

            display_ret = QE_DISPLAY_ERR_UART_OTHER;

            r_debug_print("UART Error: Receive timeout!!\n");

            break;

        case QE_DISPLAY_STS_CLOSE:

            display_ret = QE_DISPLAY_ERR_NOT_OPEN;

            r_debug_print("This module not opened!!\n");

            break;

        default:

            R_BSP_NOP(); /* do nothing */

    }

    return display_ret;
}
/**********************************************************************************************************************
 * End of function r_display_status_check
 *********************************************************************************************************************/


/* Function Name: r_debug_print */
/*******************************************************************************************************************//**
 * @brief Print processing
 * @param[in] * p_str    Print string
 * @retval None
 **********************************************************************************************************************/
void r_debug_print(char * p_str)
{
#if (1 == QE_DISPLAY_CFG_DEBUG_PRINT_ENABLE)
    printf("%s", p_str);
#else
    /* do nothing */
#endif
}
/**********************************************************************************************************************
 * End of function r_debug_print
 *********************************************************************************************************************/


/* Function Name: r_get_uart_buffer_pointer */
/*******************************************************************************************************************//**
 * @brief Get the start address of the receive data buffer.
 * @param  None
 * @retval Start address of the receive data buffer.
 **********************************************************************************************************************/
uint8_t* r_get_uart_buffer_pointer(void)
{
    return (uint8_t*) &s_uart_data_buffer[0];
}
/**********************************************************************************************************************
 * End of function r_get_uart_buffer_pointer
 *********************************************************************************************************************/


/* Function Name: r_get_uart_command */
/*******************************************************************************************************************//**
 * @brief Get current command.
 * @param  None
 * @retval Current command.
 **********************************************************************************************************************/
uint8_t r_get_uart_command(void)
{
    return s_uart_command;
}
/**********************************************************************************************************************
 * End of function r_get_uart_command
 *********************************************************************************************************************/


/* Function Name: r_set_uart_command */
/*******************************************************************************************************************//**
 * @brief Command update.
 * @param[in] command  Command to set
 * @retval None
 **********************************************************************************************************************/
void r_set_uart_command(uint8_t command)
{
    s_uart_command = command;
}
/**********************************************************************************************************************
 * End of function r_set_uart_command
 *********************************************************************************************************************/


/* Function Name: r_get_uart_status */
/*******************************************************************************************************************//**
 * @brief Get current status.
 * @param  None
 * @retval Current status
 **********************************************************************************************************************/
uint8_t r_get_uart_status(void)
{
    return s_uart_status;
}
/**********************************************************************************************************************
 * End of function r_get_uart_status
 *********************************************************************************************************************/


/* Function Name: r_set_uart_status */
/*******************************************************************************************************************//**
 * @brief Status update.
 * @param[in]  status   Status to set
 * @retval None
 **********************************************************************************************************************/
void r_set_uart_status(uint8_t status)
{
    s_uart_status = status;
}
/**********************************************************************************************************************
 * End of function r_set_uart_status
 *********************************************************************************************************************/


/* Function Name: r_uart_init */
/*******************************************************************************************************************//**
 * @brief Initial SCI settings.
 * @param  None
 * @retval QE_DISPLAY_SUCCESS            Processing has been completed successfully.
 * @retval QE_DISPLAY_ERR_UART_CONFIG    Serial communication setting error.
 **********************************************************************************************************************/
qe_display_err_t r_uart_init(void)
{
    qe_display_err_t display_ret = QE_DISPLAY_SUCCESS;
    sci_err_t        sci_ret;

    s_uart_config.async.baud_rate    = QE_DISPLAY_CFG_UART_BAUDRATE;
    s_uart_config.async.clk_src      = SCI_CLK_INT;
    s_uart_config.async.data_size    = SCI_DATA_8BIT;
    s_uart_config.async.parity_en    = SCI_PARITY_OFF;
    s_uart_config.async.parity_type  = SCI_EVEN_PARITY;
    s_uart_config.async.stop_bits    = SCI_STOPBITS_1;
    s_uart_config.async.int_priority = QE_DISPLAY_CFG_UART_INTERRUPT_PRIORITY_LEVEL;

    sci_ret = R_SCI_Open(QE_DISPLAY_CFG_UART_CH, SCI_MODE_ASYNC, &s_uart_config, r_uart_callback,
                            &s_uart_control);

    if (SCI_SUCCESS == sci_ret)
    {
        SCI_PIN_SET();
        r_set_uart_status(QE_DISPLAY_STS_IDLE);
        r_set_uart_command(QE_DISPLAY_COMMAND_NULL);

#ifdef QE_DISPLAY_UART_DMA_ENABLE
        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_id_cmd_buffer, QE_DISPLAY_SIZE_ID);

        if (SCI_SUCCESS != sci_ret)
        {
            r_debug_print("UART Error: Could not initialize UART!!\n");
            display_ret = QE_DISPLAY_ERR_UART_CONFIG;
        }

        s_uart_rev_size         = 1;

#else
        s_uart_intterupt_count  = 0;
        s_uart_split_num        = 1;
        s_uart_buf_index        = 0;
        s_uart_remain_data_size = 0;
#endif

    }
    else
    {
        r_debug_print("UART Error: Could not initialize UART!!\n");
        display_ret = QE_DISPLAY_ERR_UART_CONFIG;
    }

    return display_ret;
}
/**********************************************************************************************************************
 * End of function r_uart_init
 *********************************************************************************************************************/


/* Function Name: r_uart_reinit */
/*******************************************************************************************************************//**
 * @brief Re-initial SCI settings.
 * @param  None
 * @retval None
 **********************************************************************************************************************/
static void r_uart_reinit(void)
{
    uint32_t tx_wait;

    /* Waiting time for sending 2 bytes. */
    tx_wait = (20000000 / QE_DISPLAY_CFG_UART_BAUDRATE); /* round down */

    R_BSP_SoftwareDelay(tx_wait, BSP_DELAY_MICROSECS);

    R_SCI_Close(s_uart_control);

#if (USE_DMAC == QE_DISPLAY_UART_DMA_ENABLE)
    R_DMACA_Close(QE_DISPLAY_UART_DMA_TX_CH);
    R_DMACA_Close(QE_DISPLAY_UART_DMA_RX_CH);
#endif

    r_uart_init();
}
/**********************************************************************************************************************
 * End of function r_uart_reinit
 *********************************************************************************************************************/


/* Function Name: r_uart_send_response */
/*******************************************************************************************************************//**
 * @brief Sending response data.
 * @param[in]  response  Response value to QE for Display.
 * @retval None
 **********************************************************************************************************************/
void r_uart_send_response(uint8_t response)
{
    s_uart_response = response;

    R_SCI_Send(s_uart_control, &s_uart_response, QE_DISPLAY_SIZE_RES_COMMAND);
}
/**********************************************************************************************************************
 * End of function r_uart_send_response
 *********************************************************************************************************************/


/* Function Name: r_uart_data_verify */
/*******************************************************************************************************************//**
 * @brief Verifying connection confirmation data.
 * @param  None
 * @retval true     Verification successful
 * @retval false    Verification failed
 **********************************************************************************************************************/
static bool r_uart_data_verify(void)
{
    uint8_t i;

    for (i = 0; i < QE_DISPLAY_SIZE_VERIFY_DATA; i++)
    {
        if (s_uart_data_buffer[i] != s_uart_verify_data[i])
        {
            return false;
        }
    }

    return true;
}
/**********************************************************************************************************************
 * End of function r_uart_data_verify
 *********************************************************************************************************************/


/* Function Name: r_uart_send_verify_data */
/*******************************************************************************************************************//**
 * @brief Sending connection response data
 * @param  None
 * @retval None
 **********************************************************************************************************************/
static void r_uart_send_verify_data(void)
{
    uint8_t i;

    s_uart_verify_data_buf[0] = QE_DISPLAY_RES_ACK;

    for (i = 0; i < QE_DISPLAY_SIZE_VERIFY_DATA; i++)
    {
        s_uart_verify_data_buf[i+1] = s_uart_verify_data[i];
    }

    R_SCI_Send(s_uart_control, &s_uart_verify_data_buf[0], QE_DISPLAY_SIZE_RES_VERIFY_DATA);
}
/**********************************************************************************************************************
 * End of function r_uart_send_verify_data
 *********************************************************************************************************************/


/* Function Name: r_uart_send_max_data_size */
/*******************************************************************************************************************//**
 * @brief Sending the maximum size that can be communication
 * @param  None
 * @retval None
 **********************************************************************************************************************/
static void r_uart_send_max_data_size(void)
{
    uint16_t receivable_size;

    receivable_size = (uint16_t) QE_DISPLAY_SIZE_MAX_RECEIVABLE;

    s_uart_max_data_size[0] = (uint8_t) (receivable_size & 0x00FF);
    s_uart_max_data_size[1] = (uint8_t) (receivable_size >> 8);

    R_SCI_Send(s_uart_control, &s_uart_max_data_size[0], QE_DISPLAY_SIZE_RES_MAX_DATA);
}
/**********************************************************************************************************************
 * End of function r_uart_send_max_data_size
 *********************************************************************************************************************/


/* Function Name: r_uart_timeout_operation */
/*******************************************************************************************************************//**
 * @brief Start or stop timeout count processing.
 * @param[in] operation     true: timer start, false: timer stop
 * @retval None
 **********************************************************************************************************************/
static void r_uart_timeout_operation(bool operation)
{
    static bool s_secure_f;
    static uint32_t s_cmt_channel;
    bool cmt_ret;

    if (true == operation)
    {
        s_uart_timeout_count = 0;

        if (false == s_secure_f)
        {
            cmt_ret = R_CMT_CreatePeriodic(2, r_uart_timeout_callback, &s_cmt_channel);

            if (true != cmt_ret)
            {
                s_secure_f = false;
                return;
            }

            s_secure_f = true;
        }
    }
    else
    {
        if (true == s_secure_f)
        {
            R_CMT_Stop(s_cmt_channel);
            s_secure_f = false;
        }
    }
}
/**********************************************************************************************************************
 * End of function r_uart_timeout_operation
 *********************************************************************************************************************/


/* Function Name: r_uart_timeout_callback */
/*******************************************************************************************************************//**
 * @brief Timeout processing callback (cmt interrupt processing)
 * @param[in] * p_data     CMT channel used
 * @retval None
 **********************************************************************************************************************/
static void r_uart_timeout_callback(void * p_data)
{
    uint32_t cmt_channel = *((uint32_t*) p_data);

    s_uart_timeout_count++;

    if (QE_DISPLAY_UART_TIMEOUT <= s_uart_timeout_count)
    {
        R_CMT_Stop(cmt_channel);

        r_uart_send_response(QE_DISPLAY_RES_NACK);
        r_set_uart_status(QE_DISPLAY_STS_ERR_TIMEOUT);
    }
}
/**********************************************************************************************************************
 * End of function r_uart_timeout_callback
 *********************************************************************************************************************/


#ifndef QE_DISPLAY_UART_DMA_ENABLE

/* Function Name: r_uart_callback */
/*******************************************************************************************************************//**
 * @brief QE UART callback function (Use byteq in uart)
 * @param[in]  * p_args      callback event parameters
 * @retval     none
 **********************************************************************************************************************/
static void r_uart_callback(void * p_args)
{
    sci_cb_args_t * p_sci_cb_args;
    volatile sci_err_t sci_ret;

    p_sci_cb_args = (sci_cb_args_t*) p_args;

    if (SCI_EVT_TEI == p_sci_cb_args->event)
    {
        return;
    }

    if (SCI_EVT_RX_CHAR == p_sci_cb_args->event)
    {
        s_uart_intterupt_count++;
    }

    /* Exec Phase */
    if ((SCI_EVT_RX_CHAR == p_sci_cb_args->event) && (QE_DISPLAY_STS_EXEC == s_uart_status))
    {
        R_SCI_Control(s_uart_control, SCI_CMD_RX_Q_FLUSH , FIT_NO_PTR);

        r_debug_print("Adjustment processing in progress!!\n");
        r_uart_send_response(QE_DISPLAY_RES_BUSY);

        s_uart_intterupt_count = 0;

        return;
    }

    /* Data Receive Phase */
    if ((SCI_EVT_RX_CHAR == p_sci_cb_args->event) && (QE_DISPLAY_STS_DATA == s_uart_status))
    {
        if (QE_DISPLAY_COMMAND_NULL != s_uart_command)
        {
            if (1 == s_uart_split_num)
            {
                if (s_uart_remain_data_size == s_uart_intterupt_count)
                {
                    r_uart_timeout_operation(false);
                    s_uart_intterupt_count = 0;

                    sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_data_buffer[s_uart_buf_index],
                            s_uart_remain_data_size);

                    if (SCI_SUCCESS != sci_ret)
                    {
                        r_set_uart_status(QE_DISPLAY_STS_ERR_RECEIVE_DATA);
                        r_uart_send_response(QE_DISPLAY_RES_NACK);
                        return;
                    }

                    switch (s_uart_command)
                    {
                        case QE_DISPLAY_COMMAND_CONNECTION:
                            r_set_uart_status(QE_DISPLAY_STS_UART_CONNECTION);

                            break;
                        default:
                            r_set_uart_status(QE_DISPLAY_STS_EXEC);
                            r_uart_send_response(QE_DISPLAY_RES_ACK);
                    }

                }
            }
            else
            {
                if (QE_DISPLAY_SIZE_DATA_RECEIVABLE == s_uart_intterupt_count)
                {
                    sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_data_buffer[s_uart_buf_index],
                            QE_DISPLAY_SIZE_DATA_RECEIVABLE);

                    if (SCI_SUCCESS != sci_ret)
                    {
                        r_set_uart_status(QE_DISPLAY_STS_ERR_RECEIVE_DATA);
                        r_uart_send_response(QE_DISPLAY_RES_NACK);
                        r_uart_timeout_operation(false);
                        return;
                    }

                    s_uart_remain_data_size -= QE_DISPLAY_SIZE_DATA_RECEIVABLE;
                    s_uart_buf_index        += QE_DISPLAY_SIZE_DATA_RECEIVABLE;
                    s_uart_split_num--;
                    s_uart_intterupt_count = 0;

                    r_uart_timeout_operation(true);
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_uart_send_response(QE_DISPLAY_RES_NEXT);
                }

            }
        }
        else
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_RECEIVE_DATA);
            r_uart_send_response(QE_DISPLAY_RES_NACK);
            r_uart_timeout_operation(false);
        }

        return;
    }

    /* Command Receive Phase */
    if ((SCI_EVT_RX_CHAR == p_sci_cb_args->event) && (QE_DISPLAY_STS_COMMAND == s_uart_status))
    {
        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_id_cmd_buffer, QE_DISPLAY_SIZE_COMMAND);

        if (SCI_SUCCESS != sci_ret)
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
            r_uart_send_response(QE_DISPLAY_RES_NACK);
            r_uart_timeout_operation(false);
            return;
        }

        if ((QE_DISPLAY_COMMAND_NULL == s_uart_command) || (QE_DISPLAY_COMMAND_CONNECTION == s_uart_id_cmd_buffer))
        {
            s_uart_split_num = 1;
            s_uart_buf_index = 0;

            switch (s_uart_id_cmd_buffer)
            {
                case QE_DISPLAY_COMMAND_CONNECTION:

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_VERIFY_DATA;
                    r_set_uart_status(QE_DISPLAY_STS_DATA);
                    r_set_uart_command(QE_DISPLAY_COMMAND_CONNECTION);
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_MAX_COM_SIZE:

                    r_uart_send_max_data_size();
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                    r_uart_timeout_operation(false);

                    break;

                case QE_DISPLAY_COMMAND_ALL:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_ALL)
                    {
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_ALL / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_ALL % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_ALL;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_ALL);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_ALL));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_TIMING:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_TIMING)
                    {
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_TIMING / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_TIMING % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_TIMING;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_TIMING);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_TIMING));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_OUTCTL:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL)
                    {
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_OUTCTL);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_OUTCTL));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_LAYER1:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_LAYER1)
                    {
                        /* Note: Normally this condition is never true */
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_LAYER1 / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_LAYER1 % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_LAYER1;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_LAYER1);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_LAYER1));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_LAYER2:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_LAYER2)
                    {
                        /* Note: Normally this condition is never true */
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_LAYER2 / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_LAYER2 % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_LAYER2;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_LAYER2);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_LAYER2));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_PROCIDURE:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER)
                    {
                        /* Note: Normally this condition is never true */
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_PROCIDURE);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_PROCIDURE));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_DITHER:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_DITHER)
                    {
                        /* Note: Normally this condition is never true */
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_DITHER / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_DITHER % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_DITHER;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_DITHER);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_DITHER));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_BRIGHTNESS:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS)
                    {
                        /* Note: Normally this condition is never true */
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_BRIGHTNESS);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_BRIGHTNESS));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_CONTRAST:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST)
                    {
                        /* Note: Normally this condition is never true */
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_CONTRAST);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_CONTRAST));
                    r_uart_timeout_operation(true);

                    break;

                case QE_DISPLAY_COMMAND_GAMMA:

                    /* Prepare receive data */
                    if (QE_DISPLAY_SIZE_DATA_RECEIVABLE < QE_DISPLAY_SIZE_REV_COMMAND_GAMMA)
                    {
                        s_uart_split_num = QE_DISPLAY_SIZE_REV_COMMAND_GAMMA / QE_DISPLAY_SIZE_DATA_RECEIVABLE;

                        if (0 < (QE_DISPLAY_SIZE_REV_COMMAND_GAMMA % QE_DISPLAY_SIZE_DATA_RECEIVABLE))
                        {
                            s_uart_split_num++;
                        }
                    }

                    s_uart_remain_data_size = QE_DISPLAY_SIZE_REV_COMMAND_GAMMA;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_GAMMA);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_GAMMA));
                    r_uart_timeout_operation(true);

                    break;

                default:

                    r_uart_send_response(QE_DISPLAY_RES_NACK);
                    r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
                    r_uart_timeout_operation(false);

            }
        }
        else
        {
            r_uart_send_response(QE_DISPLAY_RES_NACK);
            r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
        }

        s_uart_intterupt_count = 0;

        return;
    }

    /* ID Receive Phase */
    if ((SCI_EVT_RX_CHAR == p_sci_cb_args->event) && (QE_DISPLAY_STS_IDLE == s_uart_status))
    {
        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_id_cmd_buffer, QE_DISPLAY_SIZE_ID);

        if (SCI_SUCCESS != sci_ret)
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_ID);
            r_uart_send_response(QE_DISPLAY_RES_NACK);
            r_uart_timeout_operation(false);
            return;
        }


        switch (s_uart_id_cmd_buffer)
        {
            case QE_DISPLAY_ID_COMMAND:

                r_set_uart_status(QE_DISPLAY_STS_COMMAND);
                r_uart_timeout_operation(true);

                break;

            case QE_DISPLAY_ID_DATA:

                r_set_uart_status(QE_DISPLAY_STS_DATA);
                r_uart_timeout_operation(true);

                break;

            default:

                r_uart_send_response(QE_DISPLAY_RES_NACK);
                r_set_uart_status(QE_DISPLAY_STS_ERR_ID);
        }

        s_uart_intterupt_count = 0;

        return;
    }

    /* Skipping data in other statuses */
    if ((SCI_EVT_RX_CHAR == p_sci_cb_args->event) && (QE_DISPLAY_STS_LCD_REINIT <= s_uart_status))
    {
        R_SCI_Control(s_uart_control, SCI_CMD_RX_Q_FLUSH , FIT_NO_PTR);

        s_uart_intterupt_count = 0;

        return;
    }

}
#else

/* Function Name: r_uart_callback */
/*******************************************************************************************************************//**
 * @brief QE UART callback function (Use dmac/dtc in uart)
 * @param[in]  * p_args      callback event parameters
 * @retval     none
 **********************************************************************************************************************/
static void r_uart_callback(void * p_args)
{
    sci_cb_args_t * p_sci_cb_args;
    volatile static sci_err_t sci_ret;
    uint8_t * p_buffer;
    uint16_t rev_size;

    p_sci_cb_args = (sci_cb_args_t*) p_args;

    /* Exec Phase */
    if ((SCI_EVT_RX_DONE == p_sci_cb_args->event) && (QE_DISPLAY_STS_EXEC == s_uart_status))
    {
        r_debug_print("Adjustment processing in progress!!\n");
        r_uart_send_response(QE_DISPLAY_RES_BUSY);

        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_id_cmd_buffer, QE_DISPLAY_SIZE_ID);

        if (SCI_SUCCESS != sci_ret)
        {
            r_debug_print("UART Error: Fatal error. Please system reset!!\n");
            r_uart_send_response(QE_DISPLAY_RES_NACK);
        }

        return;
    }

    /* Data Receive Phase */
    if ((SCI_EVT_RX_DONE == p_sci_cb_args->event) && (QE_DISPLAY_STS_DATA == s_uart_status))
    {
        r_uart_timeout_operation(false);
        s_uart_rev_size = 1;

        switch (s_uart_command)
        {
            case QE_DISPLAY_COMMAND_NULL:

                r_set_uart_status(QE_DISPLAY_STS_ERR_RECEIVE_DATA);
                r_uart_send_response(QE_DISPLAY_RES_NACK);

                break;

            case QE_DISPLAY_COMMAND_CONNECTION:

                r_set_uart_status(QE_DISPLAY_STS_UART_CONNECTION);

                break;

            default:

                r_set_uart_status(QE_DISPLAY_STS_EXEC);
                r_uart_send_response(QE_DISPLAY_RES_ACK);
        }

        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_id_cmd_buffer, QE_DISPLAY_SIZE_ID);

        if (SCI_SUCCESS != sci_ret)
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
            r_uart_send_response(QE_DISPLAY_RES_NACK);
        }

        return;
    }

    /* Command Receive Phase */
    if ((SCI_EVT_RX_DONE == p_sci_cb_args->event) && (QE_DISPLAY_STS_COMMAND == s_uart_status))
    {
        if ((QE_DISPLAY_COMMAND_NULL == s_uart_command) || (QE_DISPLAY_COMMAND_CONNECTION == s_uart_id_cmd_buffer))
        {

            switch (s_uart_id_cmd_buffer)
            {
                case QE_DISPLAY_COMMAND_CONNECTION:

                    r_set_uart_status(QE_DISPLAY_STS_DATA);
                    r_set_uart_command(QE_DISPLAY_COMMAND_CONNECTION);
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_data_buffer[0];
                    rev_size = QE_DISPLAY_SIZE_VERIFY_DATA;

                    break;

                case QE_DISPLAY_COMMAND_MAX_COM_SIZE:

                    r_uart_send_max_data_size();

                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                    r_uart_timeout_operation(false);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_ALL:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_ALL;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_ALL);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_ALL));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_TIMING:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_TIMING;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_TIMING);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_TIMING));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_OUTCTL:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_OUTCTL);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_OUTCTL));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_LAYER1:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_LAYER1;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_LAYER1);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_LAYER1));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_LAYER2:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_LAYER2;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_LAYER2);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_LAYER2));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_PROCIDURE:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_PROCIDURE);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_PROCIDURE));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_DITHER:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_DITHER;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_DITHER);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_DITHER));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_BRIGHTNESS:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_BRIGHTNESS);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_BRIGHTNESS));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_CONTRAST:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_CONTRAST);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_CONTRAST));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                case QE_DISPLAY_COMMAND_GAMMA:

                    s_uart_rev_size = QE_DISPLAY_SIZE_REV_COMMAND_GAMMA;
                    r_set_uart_status(QE_DISPLAY_STS_IDLE);
                    r_set_uart_command(QE_DISPLAY_COMMAND_GAMMA);
                    r_uart_send_response(RES_ACCEPT(QE_DISPLAY_COMMAND_GAMMA));
                    r_uart_timeout_operation(true);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;

                    break;

                default:

                    s_uart_rev_size = QE_DISPLAY_SIZE_COMMAND;
                    r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
                    r_uart_send_response(QE_DISPLAY_RES_NACK);
                    r_uart_timeout_operation(false);

                    p_buffer = &s_uart_id_cmd_buffer;
                    rev_size = QE_DISPLAY_SIZE_ID;
            }
        }
        else
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
            r_uart_send_response(QE_DISPLAY_RES_NACK);

            p_buffer = &s_uart_id_cmd_buffer;
            rev_size = QE_DISPLAY_SIZE_ID;
        }

        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) p_buffer, rev_size);

        if (SCI_SUCCESS != sci_ret)
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_COMMAND);
            r_uart_send_response(QE_DISPLAY_RES_NACK);
            r_uart_timeout_operation(false);
        }

        return;
    }

    /* ID Receive Phase */
    if ((SCI_EVT_RX_DONE == p_sci_cb_args->event) && (QE_DISPLAY_STS_IDLE == s_uart_status))
    {

        switch (s_uart_id_cmd_buffer)
        {
            case QE_DISPLAY_ID_COMMAND:

                p_buffer = &s_uart_id_cmd_buffer;
                rev_size = QE_DISPLAY_SIZE_COMMAND;
                r_set_uart_status(QE_DISPLAY_STS_COMMAND);
                r_uart_timeout_operation(true);

                break;

            case QE_DISPLAY_ID_DATA:

                p_buffer = &s_uart_data_buffer[0];
                rev_size = s_uart_rev_size;
                r_set_uart_status(QE_DISPLAY_STS_DATA);
                r_uart_timeout_operation(true);

                break;

            default:

                p_buffer = &s_uart_id_cmd_buffer;
                rev_size = QE_DISPLAY_SIZE_ID;
                r_set_uart_status(QE_DISPLAY_STS_ERR_ID);
                r_uart_send_response(QE_DISPLAY_RES_NACK);
        }

        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) p_buffer, rev_size);

        if (SCI_SUCCESS != sci_ret)
        {
            r_set_uart_status(QE_DISPLAY_STS_ERR_ID);
            r_uart_send_response(QE_DISPLAY_RES_NACK);
            r_uart_timeout_operation(false);
        }

        return;
    }

    /* Skipping data in other statuses */
    if ((SCI_EVT_RX_DONE == p_sci_cb_args->event) && (QE_DISPLAY_STS_LCD_REINIT <= s_uart_status))
    {
        sci_ret = R_SCI_Receive(s_uart_control, (uint8_t*) &s_uart_id_cmd_buffer, QE_DISPLAY_SIZE_ID);

        if (SCI_SUCCESS != sci_ret)
        {
            r_debug_print("UART Error: Fatal error. Please system reset!!\n");
            r_uart_send_response(QE_DISPLAY_RES_NACK);
        }

        return;
    }

}
#endif
/**********************************************************************************************************************
 * End of function r_uart_callback
 *********************************************************************************************************************/

/* End of File */
