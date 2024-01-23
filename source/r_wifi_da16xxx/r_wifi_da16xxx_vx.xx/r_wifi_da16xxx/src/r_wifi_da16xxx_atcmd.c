/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_wifi_da16xxx_atcmd.c
 * Description  : AT command functions definition.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"
#include "r_wifi_da16xxx_os_wrap.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define CMD_BUF_MAX     (DA16XXX_AT_CMD_BUF_MAX)
#define RESP_BUF_MAX    (DA16XXX_AT_RESP_BUF_MAX)
#define PREFIX_LEN_MAX  (128)

#define AT_RETURN_TEXT_OK   "OK\r\n"
#define AT_RETURN_TEXT_ERR  "ERROR:%d\r\n"

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* SCI portconfiguration */
typedef struct
{
    sci_ch_t ch;           /* SCI channel */
    void (*func)(void);    /* Function name of SCI Port configuration */
    uint16_t tx_size;      /* TX buffer size */
    uint16_t rx_size;      /* RX buffer size */
} st_sci_conf_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* User callback */
#if(1 == WIFI_CFG_USE_CALLBACK_FUNCTION)
void WIFI_CFG_CALLBACK_FUNCTION_NAME(void * p_args);
void (*const p_wifi_callback)(void *p_args) = WIFI_CFG_CALLBACK_FUNCTION_NAME;
#else
void (* const p_wifi_callback)(void *p_args) = NULL;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* Port configurations */
static st_sci_conf_t * get_port_config (void);

/* SCI configurations */
#if WIFI_CFG_CTS_SW_CTRL == 0
static st_sci_conf_t *s_port_cfg = NULL;
#endif
static const st_sci_conf_t s_sci_cfg[] =
{
#if SCI_CFG_CH0_INCLUDED == 1
    {SCI_CH0 , R_SCI_PinSet_SCI0  ,SCI_CFG_CH0_TX_BUFSIZ  ,SCI_CFG_CH0_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH1_INCLUDED == 1
    {SCI_CH1 , R_SCI_PinSet_SCI1  ,SCI_CFG_CH1_TX_BUFSIZ  ,SCI_CFG_CH1_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH2_INCLUDED == 1
    {SCI_CH2 , R_SCI_PinSet_SCI2  ,SCI_CFG_CH2_TX_BUFSIZ  ,SCI_CFG_CH2_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH3_INCLUDED == 1
    {SCI_CH3 , R_SCI_PinSet_SCI3  ,SCI_CFG_CH3_TX_BUFSIZ  ,SCI_CFG_CH3_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH4_INCLUDED == 1
    {SCI_CH4 , R_SCI_PinSet_SCI4  ,SCI_CFG_CH4_TX_BUFSIZ  ,SCI_CFG_CH4_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH5_INCLUDED == 1
    {SCI_CH5 , R_SCI_PinSet_SCI5  ,SCI_CFG_CH5_TX_BUFSIZ  ,SCI_CFG_CH5_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH6_INCLUDED == 1
    {SCI_CH6 , R_SCI_PinSet_SCI6  ,SCI_CFG_CH6_TX_BUFSIZ  ,SCI_CFG_CH6_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH7_INCLUDED == 1
    {SCI_CH7 , R_SCI_PinSet_SCI7  ,SCI_CFG_CH7_TX_BUFSIZ  ,SCI_CFG_CH7_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH8_INCLUDED == 1
    {SCI_CH8 , R_SCI_PinSet_SCI8  ,SCI_CFG_CH8_TX_BUFSIZ  ,SCI_CFG_CH8_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH9_INCLUDED == 1
    {SCI_CH9 , R_SCI_PinSet_SCI9  ,SCI_CFG_CH9_TX_BUFSIZ  ,SCI_CFG_CH9_RX_BUFSIZ  },
#endif
#if SCI_CFG_CH10_INCLUDED == 1
    {SCI_CH10, R_SCI_PinSet_SCI10 ,SCI_CFG_CH10_TX_BUFSIZ ,SCI_CFG_CH10_RX_BUFSIZ },
#endif
#if SCI_CFG_CH11_INCLUDED == 1
    {SCI_CH11, R_SCI_PinSet_SCI11 ,SCI_CFG_CH11_TX_BUFSIZ ,SCI_CFG_CH11_RX_BUFSIZ },
#endif
#if SCI_CFG_CH12_INCLUDED == 1
    {SCI_CH12, R_SCI_PinSet_SCI12 ,SCI_CFG_CH12_TX_BUFSIZ ,SCI_CFG_CH12_RX_BUFSIZ },
#endif
    {SCI_NUM_CH, NULL ,0 ,0 }
};

/* Buffer control */
static uint8_t  s_cmd_buf[CMD_BUF_MAX];
static uint8_t  s_resp_buf[RESP_BUF_MAX];
static char     s_resp_prefix[PREFIX_LEN_MAX];
static uint32_t s_rcv_cnt = 0;
static uint32_t s_read_cnt = 0;
static uint32_t s_start_pos = 0;
static uint32_t s_atcmd_resp_timeout = ATCMD_RESP_TIMEOUT;

/**********************************************************************************************************************
 * Function Name: flow_ctrl_init
 * Description  : Initialise hardware flow.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void flow_ctrl_init (void)
{
#if WIFI_CFG_CTS_SW_CTRL == 0 /* SCI H/W handles CTS, output RTS as GPIO */
    R_SCI_Control(g_uart_tbl.sci_hdl, SCI_CMD_EN_CTS_IN, NULL);
    WIFI_RTS_PDR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 1;  /* Output */
    WIFI_RTS_PODR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN)  = 0;  /* low    */
#else /* SCI H/W handles RTS, input CTS as GPIO */
    /* Do nothing */
    R_BSP_NOP();
#endif
}
/**********************************************************************************************************************
 * End of function flow_ctrl_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: flow_ctrl_set
 * Description  : Control RTS pin.
 * Arguments    : flow - Control direction
 * Return Value : None
 *********************************************************************************************************************/
void flow_ctrl_set (e_flow_ctrl_t flow)
{
#if WIFI_CFG_CTS_SW_CTRL == 0 /* SCI H/W handles CTS, output RTS as GPIO */
    WIFI_RTS_PODR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = flow;
#else /* SCI H/W handles RTS, input CTS as GPIO */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    if (flow == RTS_OFF)
    {
        WIFI_RTS_PODR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN)  = 0;
        WIFI_RTS_PDR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 0;
        WIFI_SET_PFS(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = WIFI_CFG_PFS_SET_VALUE;
        WIFI_SET_PMR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 1;
    }
    else
    {
        WIFI_SET_PFS(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 0x00U;
        WIFI_SET_PMR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 0;
        WIFI_RTS_PDR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 1;
        WIFI_RTS_PODR(WIFI_CFG_RTS_PORT, WIFI_CFG_RTS_PIN) = 1;
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
#endif
}
/**********************************************************************************************************************
 * End of function flow_ctrl_set
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: get_port_config
 * Description  : get port configuration table pointer.
 * Arguments    : none
 * Return Value : SUCCESS : SCI configuration table(st_sci_conf_t) pointer by port.
 *                FAIL    : NULL
 *********************************************************************************************************************/
static st_sci_conf_t * get_port_config(void)
{
    uint16_t i;
    st_sci_conf_t * p_tbl = NULL;

    /* Set table pointer */
    for (i = 0;; i++ )
    {
        if (SCI_NUM_CH == s_sci_cfg[i].ch)
        {
            break;
        }
        if (WIFI_CFG_SCI_CHANNEL == s_sci_cfg[i].ch)
        {
            p_tbl = (st_sci_conf_t *)&s_sci_cfg[i];
            break;
        }
    }
    return p_tbl;
}
/**********************************************************************************************************************
 * End of function get_port_config
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: uart_port_open
 * Description  : Open serial port.
 * Arguments    : p_cb  - Callback function of SCI interrupts
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *********************************************************************************************************************/
e_atcmd_err_t uart_port_open(void (* const p_cb)(void *p_args))
{
    st_uart_tbl_t * p_uart = &g_uart_tbl;
    st_sci_conf_t * p_cfg = get_port_config();

    if (NULL == p_cfg)
    {
        WIFI_LOG_ERROR(("uart_port_open: Cannot get port config!"));
        return ATCMD_ERR_MODULE_COM;
    }

    /* Port settings */
    p_cfg->func();
#if WIFI_CFG_CTS_SW_CTRL == 0
    s_port_cfg = p_cfg;
#endif

    memset(&p_uart->sci_hdl , 0, sizeof(sci_hdl_t));
    p_uart->sci_config.async.baud_rate    = WIFI_CFG_SCI_BAUDRATE;
    p_uart->sci_config.async.clk_src      = SCI_CLK_INT;
    p_uart->sci_config.async.data_size    = SCI_DATA_8BIT;
    p_uart->sci_config.async.parity_en    = SCI_PARITY_OFF;
    p_uart->sci_config.async.parity_type  = SCI_EVEN_PARITY;
    p_uart->sci_config.async.stop_bits    = SCI_STOPBITS_1;
    p_uart->sci_config.async.int_priority = WIFI_CFG_SCI_INTERRUPT_LEVEL;
    if (SCI_SUCCESS != R_SCI_Open(p_cfg->ch, SCI_MODE_ASYNC, &p_uart->sci_config, p_cb, &p_uart->sci_hdl))
    {
        WIFI_LOG_ERROR(("uart_port_open: Cannot open SCI channel %d!", p_cfg->ch));
        return ATCMD_ERR_MODULE_COM;
    }
    return ATCMD_OK;
}
/**********************************************************************************************************************
 * End of function uart_port_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: uart_port_close
 * Description  : Close serial port.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void uart_port_close(void)
{
    st_uart_tbl_t * p_uart = &g_uart_tbl;

    if (0 != p_uart->sci_hdl)
    {
        R_SCI_Control(p_uart->sci_hdl, SCI_CMD_RX_Q_FLUSH, NULL);
        R_SCI_Control(p_uart->sci_hdl, SCI_CMD_TX_Q_FLUSH, NULL);
        R_SCI_Close(p_uart->sci_hdl);
    }
}
/**********************************************************************************************************************
 * End of function uart_port_close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_resp_byteq_open
 * Description  : open BYTEQ for AT command response.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_BYTEQ_OPEN
 *********************************************************************************************************************/
wifi_err_t at_resp_byteq_open(void)
{
        wifi_err_t ret = WIFI_SUCCESS;

        if (BYTEQ_SUCCESS != R_BYTEQ_Open(g_uart_tbl.recv_buf, RESP_BUF_MAX, &g_uart_tbl.byteq_hdl))
        {
            WIFI_LOG_ERROR(("at_resp_byteq_open: Fail to open BYTEQ for AT command response"));
            ret = WIFI_ERR_BYTEQ_OPEN;
        }

        return ret;
}
/**********************************************************************************************************************
 * End of function at_resp_byteq_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_send_raw
 * Description  : Send raw AT commands on UART.
 * Arguments    : data
 *                length
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *                ATCMD_ERR_TIMEOUT
 *********************************************************************************************************************/
e_atcmd_err_t at_send_raw(uint8_t *data, uint16_t const length)
{
    e_atcmd_err_t ret = ATCMD_OK;
    uint32_t index = 0;
#if WIFI_CFG_CTS_SW_CTRL == 0
    uint32_t send_length = 0;
#endif

    // HW flow enable
    flow_ctrl_set(RTS_OFF);

#if WIFI_CFG_CTS_SW_CTRL == 0
    while ((index < length) && (ret == ATCMD_OK))
    {
        if ((length - index) > s_port_cfg->tx_size)
        {
            send_length = s_port_cfg->tx_size;
        }
        else
        {
            send_length = length - index;
        }

        /* Send AT command */
        g_uart_tbl.tx_end_flag = 0;
        if (SCI_SUCCESS != R_SCI_Send(g_uart_tbl.sci_hdl, &data[index], send_length))
        {
            ret = ATCMD_ERR_MODULE_COM;
            break;
        }

        /* wait for transmission end */
        tick_count_start(s_atcmd_resp_timeout, NULL);
        while (0 == g_uart_tbl.tx_end_flag)
        {
            if (TICK_EXPIRED == tick_count_check())
            {
                ret = ATCMD_ERR_TIMEOUT;
                break;
            }
            else
            {
                os_wrap_sleep(1, UNIT_TICK);
            }
        }
        index += send_length;
    }
#else
    tick_count_start(s_atcmd_resp_timeout, NULL);
    while (index < length)
    {
        if (TICK_EXPIRED == tick_count_check())
        {
            ret = ATCMD_ERR_TIMEOUT;
            break;
        }

        if (1 == WIFI_CTS_PIDR(WIFI_CFG_CTS_PORT, WIFI_CFG_CTS_PIN))
        {
            continue;
        }

        /* Send AT command */
        g_uart_tbl.tx_end_flag = 0;
        if (SCI_SUCCESS != R_SCI_Send(g_uart_tbl.sci_hdl, &data[index++], 1))
        {
            ret = ATCMD_ERR_MODULE_COM;
            break;
        }

        /* wait for transmission end */
        tick_count_start(s_atcmd_resp_timeout, NULL);
        while (0 == g_uart_tbl.tx_end_flag)
        {
            if (TICK_EXPIRED == tick_count_check())
            {
                ret = ATCMD_ERR_TIMEOUT;
                break;
            }
            else
            {
                continue;
            }
        }
    }
#endif

    tick_count_stop();

    // HW flow disable
    flow_ctrl_set(RTS_ON);
    
    return ret;
}
/**********************************************************************************************************************
 * End of function at_send_raw
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_send
 * Description  : Send AT commands on UART.
 * Arguments    : cmd
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *                ATCMD_ERR_TIMEOUT
 *********************************************************************************************************************/
e_atcmd_err_t at_send(const char *cmd, ...)
{
    va_list args;
    uint16_t str_length;

    /* Make AT command */
    va_start(args, cmd);
    str_length = vsnprintf((char *)s_cmd_buf, CMD_BUF_MAX, cmd, args);
    va_end(args);

    if (str_length > (CMD_BUF_MAX - 1))
    {
        str_length = CMD_BUF_MAX - 1;
        WIFI_LOG_WARN(("at_send: command string is truncated (%d > %d).", str_length, CMD_BUF_MAX - 1));
    }

    WIFI_LOG_DEBUG(("at_send: %s", s_cmd_buf));
    return at_send_raw(s_cmd_buf, str_length);
}
/**********************************************************************************************************************
 * End of function at_send
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_recv
 * Description  : Receive response and return RESULT CODE.
 * Arguments    : none
 * Return Value : AT_OK
 *                AT_UNKNOWN_CMD
 *                AT_INSUF_PARAMS
 *                AT_TOO_MANY_PARAMS
 *                AT_INVALID_PARAM
 *                AT_UNSUPPORTED_FUN
 *                AT_NOT_CONNECTED_AP
 *                AT_NO_RESULT
 *                AT_RESP_BUF_OVERFLOW
 *                AT_FUNC_NOT_CONFIG
 *                AT_CMD_TIMEOUT
 *                AT_NVRAM_WR_FAIL
 *                AT_RETEN_MEM_WR_FAIL
 *                AT_INTERNAL_TIMEOUT
 *                AT_INTERNAL_ERROR
 *********************************************************************************************************************/
e_rslt_code_t at_recv(void)
{
    byteq_err_t byteq_ret;
    uint8_t data;
    int8_t err_code;
    uint32_t start_pos = 0;
    e_rslt_code_t  ret = AT_INTERNAL_ERROR;

    /* Initialize */
    s_rcv_cnt = 0;
    s_read_cnt = 0;
    s_start_pos = 0;

    /* Receive and store AT command response */
    tick_count_start(s_atcmd_resp_timeout, NULL);
    while (1)
    {
        /* timeout? */
        if (TICK_EXPIRED == tick_count_check())
        {
            ret = AT_INTERNAL_TIMEOUT;
            WIFI_LOG_WARN(("at_recv: response timeout!"));
            break;
        }

        /* read data from SCI's BYTEQ */
#if defined(__CCRX__) || defined(__ICCRX__) || defined (__RX__)
        R_BSP_InterruptsDisable();
        byteq_ret = R_BYTEQ_Get(g_uart_tbl.byteq_hdl, &data);
        R_BSP_InterruptsEnable();
#endif
        if (BYTEQ_ERR_QUEUE_EMPTY == byteq_ret)
        {
            os_wrap_sleep(1, UNIT_TICK);
            continue;
        }

        if (BYTEQ_SUCCESS == byteq_ret)
        {
            /* Buffer full? */
            if (RESP_BUF_MAX <= s_rcv_cnt)
            {
                WIFI_LOG_ERROR(("at_recv: Buffer over flow!!"));
                post_err_event(WIFI_EVENT_RCV_TASK_RXB_OVF_ERR, 0);
                break;
            }

            s_resp_buf[s_rcv_cnt++] = data;
            s_resp_buf[s_rcv_cnt]   = 0;

            /* End of line? */
            if ('\n' == s_resp_buf[s_rcv_cnt - 1])
            {
                /* restart tick count */
                tick_count_start(s_atcmd_resp_timeout, NULL);

                WIFI_LOG_DEBUG(("%s", &s_resp_buf[start_pos]));
                /* Check for match */
                if (NULL != strstr((const char *) &s_resp_buf[start_pos], AT_RETURN_TEXT_OK))
                {
                    tick_count_stop();
                    return AT_OK;
                }
                else if (1 == sscanf((const char *) &s_resp_buf[start_pos], AT_RETURN_TEXT_ERR, (int *) &err_code))
                {
                    tick_count_stop();
                    return err_code;
                }
                /* set pointer to next line */
                start_pos = s_rcv_cnt;
            }
        }
    }
    return ret;
}
/**********************************************************************************************************************
 * End of function at_recv
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_exec
 * Description  : Send AT commands and Receive response on UART.
 *                Recommend using for AT basic command.
 * Arguments    : cmd
 * Return Value : Same as at_recv() function.
 *********************************************************************************************************************/
e_rslt_code_t at_exec(const char *cmd, ...)
{
    va_list args;
    uint16_t str_length;
    e_rslt_code_t ret = AT_INTERNAL_ERROR;

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return AT_INTERNAL_ERROR;
    }

    /* Make AT command */
    va_start(args, cmd);
    str_length = vsnprintf((char *)s_cmd_buf, CMD_BUF_MAX, cmd, args);
    va_end(args);

    if (str_length > (CMD_BUF_MAX - 1))
    {
        str_length = CMD_BUF_MAX - 1;
        WIFI_LOG_WARN(("at_exec: command string is truncated (%d > %d).", str_length, CMD_BUF_MAX - 1));
    }

    WIFI_LOG_DEBUG(("at_exec: %s", s_cmd_buf));
    if (ATCMD_OK == at_send_raw(s_cmd_buf, str_length))
    {
        /* Receive response */
        ret = at_recv();
    }

    memset(s_cmd_buf, 0, str_length);

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return ret;
}
/**********************************************************************************************************************
 * End of function at_exec
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_exec_wo_mutex
 * Description  : Send AT commands and Receive response on UART.
 *                Recommend using for AT extended command.
 * Arguments    : cmd
 * Return Value : Same as at_recv() function.
 *********************************************************************************************************************/
e_rslt_code_t at_exec_wo_mutex (const char *cmd, ...)
{
    va_list args;
    uint16_t str_length;
    e_rslt_code_t ret = AT_INTERNAL_ERROR;

    /* Make AT command */
    va_start(args, cmd);
    str_length = vsnprintf((char *)s_cmd_buf, CMD_BUF_MAX, cmd, args);
    va_end(args);

    if (str_length > (CMD_BUF_MAX - 1))
    {
        str_length = CMD_BUF_MAX - 1;
        WIFI_LOG_WARN(("at_exec: command string is truncated (%d > %d).", str_length, CMD_BUF_MAX - 1));
    }

    WIFI_LOG_DEBUG(("at_exec: %s", s_cmd_buf));
    if (ATCMD_OK == at_send_raw(s_cmd_buf, str_length))
    {
        /* Receive response */
        ret = at_recv();
    }

    memset(s_cmd_buf, 0, str_length);

    return ret;
}
/**********************************************************************************************************************
 * End of function at_exec_wo_mutex
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_read
 * Description  : Read buffer with prefix. (example at_read("abcd = %s\r\n", p_char) )
 * Arguments    : response_fmt
 * Return Value : 0     : data not found
 *                other : data found
 *********************************************************************************************************************/
uint32_t at_read(const char *response_fmt, ...)
{
    va_list   args;
    uint32_t  rtn = DATA_NOT_FOUND;
    char    * sp;

    /* No received data? */
    if (0 == s_rcv_cnt)
    {
        return 0;
    }

    va_start(args, response_fmt);

    /* Make prefix */
    memset(s_resp_prefix, 0, sizeof(s_resp_prefix));
    strcpy(s_resp_prefix, response_fmt);
    sp = strstr((const char *)response_fmt, "%");
    if (0 != sp)
    {
        s_resp_prefix[(uint32_t)sp - (uint32_t)response_fmt] = 0;
    }

    /* Read from received response */
    while (1)
    {
        if (s_read_cnt >= s_rcv_cnt)
        {
            break;
        }

        /* End of line? */
        if ('\n' == s_resp_buf[s_read_cnt++])
        {
            /* Check for match */
            if (0 == strncmp((const char *)&s_resp_buf[s_start_pos], s_resp_prefix, strlen(s_resp_prefix)))
            {
                if (EOF == vsscanf((const char *)&s_resp_buf[s_start_pos], response_fmt, args))
                {
                    WIFI_LOG_ERROR(("at_read: Input data failure (%s)", response_fmt));
                    rtn = DATA_NOT_FOUND;
                }
                else
                {
                    rtn = DATA_FOUND;
                }
                s_start_pos = s_read_cnt;
                break;
            }

            /* set pointer to next line */
            s_start_pos = s_read_cnt;
        }
    }

    va_end(args);
    return rtn;
}
/**********************************************************************************************************************
 * End of function at_read
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_read_wo_prefix
 * Description  : Read buffer without prefix. (example at_read_wo_prefix("%s\r\n", p_char) )
 * Arguments    : response_fmt
 * Return Value : 0     : data not found
 *                other : data found
 *********************************************************************************************************************/
uint32_t at_read_wo_prefix(const char *response_fmt, ...)
{
    uint32_t ret;
    va_list args;

    va_start(args, response_fmt);
    if (EOF == vsscanf((const char *)&s_resp_buf[s_start_pos], response_fmt, args))
    {
        WIFI_LOG_ERROR(("at_read_wo_prefix: Input data failure (%s)", response_fmt));
        ret = DATA_NOT_FOUND;
    }
    else
    {
        ret = DATA_FOUND;
    }
    va_end(args);
    return ret;
}
/**********************************************************************************************************************
 * End of function at_read_wo_prefix
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_move_to_next_line
 * Description  : Move pointer to next line of response buffer.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void at_move_to_next_line(void)
{
    while (1)
    {
        if (s_read_cnt >= s_rcv_cnt)
        {
            break;
        }

        /* Set pointer to next line */
        if ('\n' == s_resp_buf[s_read_cnt++])
        {
            s_start_pos = s_read_cnt;
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function at_move_to_next_line
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_move_to_first_line
 * Description  : Set pointer to first line of response buffer.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void at_move_to_first_line(void)
{
    s_start_pos = 0;
}
/**********************************************************************************************************************
 * End of function at_move_to_first_line
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_get_current_line
 * Description  : Get pointer on current line of response buffer.
 * Arguments    : None
 * Return Value : Pointer of current line
 *********************************************************************************************************************/
uint8_t * at_get_current_line(void)
{
    return &s_resp_buf[s_start_pos];
}
/**********************************************************************************************************************
 * End of function at_get_current_line
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_set_timeout
 * Description  : Set timeout threshold for AT command response (msec).
 * Arguments    : timeout_ms
 * Return Value : None
 *********************************************************************************************************************/
void at_set_timeout (uint32_t timeout_ms)
{
    s_atcmd_resp_timeout = timeout_ms;
}

/**********************************************************************************************************************
 * Function Name: post_err_event
 * Description  : Post error event to user callback function.
 * Arguments    : err_event - error event
 *                sock_idx  - socket index
 * Return Value : None
 *********************************************************************************************************************/
void post_err_event(wifi_err_event_enum_t err_event, uint32_t sock_idx)
{
    wifi_err_event_t evt;

    evt.event = err_event;
    evt.socket_number = sock_idx;
    if (NULL != p_wifi_callback)
    {
        p_wifi_callback(&evt);
    }
}
/**********************************************************************************************************************
 * End of function post_err_event
 *********************************************************************************************************************/
