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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_wifi_da16xxx_tcp.c
 * Description  : TCP protocol API functions definition for DA16XXX.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if WIFI_CFG_TCP_SUPPORT == 1
#define TCP_TBL_MAX     (WIFI_CFG_TCP_CREATABLE_SOCKETS)            // Socket table
#define TCP_BUF_MAX     (WIFI_CFG_TCP_SOCKET_RECEIVE_BUFFER_SIZE)   // Socket buffer

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* Socket table information */
typedef struct st_sock_tbl
{
    uint8_t                 ipaddr[4];
    uint32_t                port;
    wifi_socket_status_t    status;
    uint8_t                 ipver;
    wifi_socket_type_t      type;
    byteq_hdl_t             byteq_hdl;
    uint32_t                put_err_cnt;
    uint8_t                 recv_buf[TCP_BUF_MAX];
    int32_t                 recv_len;
    st_wifi_timer           timer_tx;
    st_wifi_timer           timer_rx;
} st_sock_tbl_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
static st_sock_tbl_t g_sock_tbl[TCP_TBL_MAX];
static uint8_t g_rx_buff[TEMP_BUF_MAX];
static uint8_t g_sock_list[TCP_TBL_MAX + 2]; //number of sockets + 2 others socket type
static volatile uint8_t g_cur_sock_idx = UINT8_MAX;
static volatile uint8_t g_rx_idx = 0;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetAvailableSocket
 * Description  : Get the next available socket ID.
 * Arguments    : socket_id
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetAvailableSocket (uint32_t * socket_id)
{
    uint32_t i = 0;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetAvailableTlsSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (NULL == socket_id)
    {
        return WIFI_ERR_PARAMETER;
    }

    for (i = 0; i < TCP_TBL_MAX; i++)
    {
        if (g_sock_tbl[i].status == WIFI_SOCKET_STATUS_CLOSED)
        {
            *socket_id = i;
            return WIFI_SUCCESS;
        }
    }

    *socket_id = UINT8_MAX;
    return WIFI_ERR_SOCKET_NUM;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetAvailableSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_GetSocketStatus
 * Description  : Get the socket status.
 * Arguments    : socket_number
 *                socket_status
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_GetSocketStatus (uint32_t socket_number, wifi_socket_status_t *socket_status)
{
    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_GetSocketStatus: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    /* Check parameters */
    if (NULL == socket_status)
    {
        return WIFI_ERR_PARAMETER;
    }

    if (socket_number >= TCP_TBL_MAX)
    {
        return WIFI_ERR_SOCKET_NUM;
    }

    *socket_status = g_sock_tbl[socket_number].status;

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_GetSocketStatus
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CreateSocket
 * Description  : Create a new socket instance.
 * Arguments    : socket_number
 *                type
 *                ip_version
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_CREATE
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CreateSocket (uint32_t socket_number, wifi_socket_type_t type, uint8_t ip_version)
{
    static bool socket_init = false;
    wifi_err_t ret = WIFI_ERR_SOCKET_CREATE;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_CreateSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (socket_number >= TCP_TBL_MAX || WIFI_SOCKET_TYPE_TCP_CLIENT != type)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* init socket table */
    if (socket_init == false)
    {
        memset(g_sock_list, UINT8_MAX, sizeof(g_sock_list));
        memset(g_sock_tbl, 0, sizeof(g_sock_tbl));
        socket_init = true;
    }

    if (WIFI_SOCKET_STATUS_CLOSED == g_sock_tbl[socket_number].status)
    {
        g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_SOCKET;    /* socket status   */
        g_sock_tbl[socket_number].ipver = ip_version;                       /* ip_version      */
        g_sock_tbl[socket_number].type = type;                              /* type            */
        g_sock_tbl[socket_number].put_err_cnt = 0;
        if (BYTEQ_SUCCESS == R_BYTEQ_Open(g_sock_tbl[socket_number].recv_buf,
                                          TCP_BUF_MAX,
                                          &g_sock_tbl[socket_number].byteq_hdl))
        {
            WIFI_LOG_INFO(("R_WIFI_DA16XXX_CreateSocket: Creating socket %lu!", socket_number));
            ret = WIFI_SUCCESS;
        }
        else
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_CreateSocket: Cannot open BYTEQ for socket %lu!", socket_number));
            ret = WIFI_ERR_BYTEQ_OPEN;
        }
    }
    else
    {
        WIFI_LOG_WARN(("R_WIFI_DA16XXX_CreateSocket: socket %lu has already created!", socket_number));
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_CreateSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TcpConnect
 * Description  : Connect to a specific IP and Port using socket.
 * Arguments    : socket_number
 *                ip_address
 *                port
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TcpConnect (uint32_t socket_number, uint8_t * ip_address, uint16_t port)
{
    wifi_err_t  api_ret = WIFI_ERR_MODULE_COM;
    uint8_t     cid = 0;

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_TcpConnect: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if (socket_number >= TCP_TBL_MAX || NULL == ip_address || 0 == port)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* socket created? */
    if (WIFI_SOCKET_STATUS_SOCKET != g_sock_tbl[socket_number].status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_TcpConnect: socket %lu is not created!", socket_number));
        return WIFI_ERR_SOCKET_NUM;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    at_set_timeout(10000);
    if (AT_OK == at_exec_wo_mutex("AT+TRTC=%d.%d.%d.%d,%lu,0\r",
                                  ip_address[0], ip_address[1], ip_address[2], ip_address[3], port))
    {
        if (DATA_FOUND == at_read("+TRTC:%d", &cid))
        {
            g_sock_tbl[socket_number].ipaddr[0] = ip_address[0];
            g_sock_tbl[socket_number].ipaddr[1] = ip_address[1];
            g_sock_tbl[socket_number].ipaddr[2] = ip_address[2];
            g_sock_tbl[socket_number].ipaddr[3] = ip_address[3];
            g_sock_tbl[socket_number].port = port;
            g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_CONNECTED;
            g_sock_list[cid] = socket_number;
            api_ret = WIFI_SUCCESS;
            WIFI_LOG_INFO(("R_WIFI_DA16XXX_TcpConnect: connected socket %lu to TCP server.", socket_number));
        }
    }

    at_set_timeout(ATCMD_RESP_TIMEOUT);

    /* Give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_TcpConnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_SendSocket
 * Description  : Send data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of sent data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_SendSocket (uint32_t socket_number, uint8_t * data,
                                   uint32_t length, uint32_t timeout_ms)
{
    uint32_t    send_idx = 0;
    uint8_t     send_data[DA16XXX_AT_CMD_BUF_MAX] = {0};
    int32_t     send_length;
    uint8_t     cid = 0;
    int32_t     api_ret = 0;
    e_atcmd_err_t at_ret = ATCMD_OK;
    e_rslt_code_t rslt_ret = AT_OK;
    OS_TICK     tick_tmp;
    uint32_t    tx_length;

    /* Connect access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SendSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((socket_number >= TCP_TBL_MAX) || (NULL == data))
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Not connect? */
    if (WIFI_SOCKET_STATUS_CONNECTED != g_sock_tbl[socket_number].status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_SendSocket: socket #%lu is not connected!", socket_number));
        return WIFI_ERR_SOCKET_NUM;
    }

    /* get CID number */
    for (cid = g_sock_tbl[socket_number].type; cid < TCP_TBL_MAX + 2; cid++)
    {
        if (g_sock_list[cid] == socket_number)
        {
            break;
        }
    }
    if (cid == TCP_TBL_MAX + 2)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Set timeout */
    at_set_timeout(timeout_ms);
    tick_tmp = os_wrap_tickcount_get();
    while (send_idx < length)
    {
        /* get length of data sent */
        if (length - send_idx > (uint32_t)(DA16XXX_AT_SOCK_TX_MAX))
        {
            tx_length = (uint32_t)(DA16XXX_AT_SOCK_TX_MAX);
        }
        else
        {
            tx_length = length - send_idx;
        }
        /* get prefix length */
#if WIFI_CFG_SCI_BAUDRATE == 115200
        send_length = snprintf((char *) send_data, DA16XXX_AT_CMD_BUF_MAX, "\x1BS%d%ld,%d.%d.%d.%d,%lu,%s,",
                               cid, (int) tx_length,
                               g_sock_tbl[socket_number].ipaddr[0],
                               g_sock_tbl[socket_number].ipaddr[1],
                               g_sock_tbl[socket_number].ipaddr[2],
                               g_sock_tbl[socket_number].ipaddr[3],
                               g_sock_tbl[socket_number].port, "r");

        WIFI_LOG_DEBUG(("SendSocket: %s", send_data));
        at_ret = at_send_raw((uint8_t *) send_data, (uint32_t) send_length);
        if (ATCMD_ERR_TIMEOUT == at_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_send_raw() timed out (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            break;
        }
        else if (ATCMD_OK != at_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_send_raw() failed (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            break;
        }
#else
        rslt_ret = at_exec_wo_mutex("\x1BH%d,%ld,%d.%d.%d.%d,%lu\r",
                                    cid, (int) tx_length,
                                    g_sock_tbl[socket_number].ipaddr[0],
                                    g_sock_tbl[socket_number].ipaddr[1],
                                    g_sock_tbl[socket_number].ipaddr[2],
                                    g_sock_tbl[socket_number].ipaddr[3],
                                    g_sock_tbl[socket_number].port);
        if (AT_INTERNAL_TIMEOUT == rslt_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_exec() timed out (ret=%ld)!", rslt_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            break;
        }
        else if (AT_OK != rslt_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_exec() failed (ret=%ld)!", rslt_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            break;
        }
#endif

        at_ret = at_send_raw((uint8_t *) data + send_idx, tx_length);
        if (ATCMD_ERR_TIMEOUT == at_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_send_raw() timed out (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            break;
        }
        else if (ATCMD_OK != at_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_send_raw() failed (ret=%ld)!", at_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            break;
        }

        rslt_ret = at_recv();
        if (AT_INTERNAL_TIMEOUT == rslt_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_recv() timed out (ret=%ld)!", rslt_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_TIMEOUT;
            break;
        }
        else if (AT_OK != rslt_ret)
        {
            WIFI_LOG_ERROR(("SendSocket: at_recv() failed (ret=%ld)!", rslt_ret));
            api_ret = (int32_t) WIFI_ERR_MODULE_COM;
            break;
        }

        send_idx += tx_length;
    }
    if (api_ret >= WIFI_SUCCESS)
    {
        api_ret = send_idx;
    }

    at_set_timeout(ATCMD_RESP_TIMEOUT);
    os_wrap_mutex_give(MUTEX_TX);
    tick_tmp = os_wrap_tickcount_get() - tick_tmp;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_SendSocket: socket %lu ret=%ld (%lu).", socket_number, api_ret, tick_tmp));
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_SendSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_ReceiveSocket
 * Description  : Receive data on connecting socket.
 * Arguments    : socket_number
 *                data
 *                length
 *                timeout_ms
 * Return Value : Positive number - number of received data
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
int32_t R_WIFI_DA16XXX_ReceiveSocket (uint32_t socket_number, uint8_t * data,
                                      uint32_t length, uint32_t timeout_ms)
{
    int32_t     api_ret = WIFI_SUCCESS;
    uint32_t    recv_cnt = 0;
    byteq_err_t byteq_ret;
    OS_TICK     tick_tmp = 0;

    /* Connect access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_ReceiveSocket: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Check parameters */
    if ((socket_number >= TCP_TBL_MAX) || (NULL == data))
    {
        return WIFI_ERR_PARAMETER;
    }

    g_sock_tbl[socket_number].timer_rx.threshold = OS_WRAP_MS_TO_TICKS(timeout_ms);
    if (0 < timeout_ms)
    {
        g_sock_tbl[socket_number].timer_rx.tick_sta = os_wrap_tickcount_get();
    }
    while (recv_cnt < length)
    {
        R_BSP_InterruptsDisable();
        byteq_ret = R_BYTEQ_Get(g_sock_tbl[socket_number].byteq_hdl, (uint8_t *) (data + recv_cnt));
        R_BSP_InterruptsEnable();
        if (BYTEQ_SUCCESS == byteq_ret)
        {
            recv_cnt++;
            continue;
        }

        /* timeout? */
        if (0 < timeout_ms)
        {
            tick_tmp = os_wrap_tickcount_get() - g_sock_tbl[socket_number].timer_rx.tick_sta;
            if (g_sock_tbl[socket_number].timer_rx.threshold <= tick_tmp)
            {
                WIFI_LOG_WARN(("R_WIFI_DA16XXX_ReceiveSocket: timeout!"));
                break;
            }
        }
        else if (0 == timeout_ms)
        {
            /* return immediately when no data in buffer */
            break;
        }
        os_wrap_sleep(10, UNIT_MSEC);
    }

    /* Not connect? */
    if ((recv_cnt == 0) && (WIFI_SOCKET_STATUS_CONNECTED != g_sock_tbl[socket_number].status))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_ReceiveSocket: socket %lu is not connected!", socket_number));
        return WIFI_ERR_SOCKET_NUM;
    }

    WIFI_LOG_INFO(("R_WIFI_DA16XXX_ReceiveSocket: socket %lu recv_cnt=%ld (%lu).", socket_number, recv_cnt, tick_tmp));
    api_ret = recv_cnt;
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_ReceiveSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_CloseSocket
 * Description  : Disconnect a specific socket connection.
 * Arguments    : socket_number
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MODULE_TIMEOUT
 *                WIFI_ERR_SOCKET_NUM
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_CloseSocket (uint32_t socket_number)
{
    wifi_err_t api_ret = WIFI_SUCCESS;
    e_rslt_code_t at_rep = AT_OK;
    uint8_t    cid = 0;

    /* Check parameters */
    if (socket_number >= TCP_TBL_MAX)
    {
        return WIFI_ERR_SOCKET_NUM;
    }

    /* Disconnected WiFi module? */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        return WIFI_ERR_NOT_OPEN;
    }

    /* Connected access point? */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_WARN(("R_WIFI_DA16XXX_CloseSocket: Not connected to access point!"));
        goto CLOSE_SOCKET;
    }

    /* get CID number */
    for (cid = g_sock_tbl[socket_number].type; cid < TCP_TBL_MAX + 2; cid++)
    {
        if (g_sock_list[cid] == socket_number)
        {
            break;
        }
    }
    if ((TCP_TBL_MAX + 2 == cid) || (WIFI_SOCKET_STATUS_CONNECTED != g_sock_tbl[socket_number].status))
    {
        goto CLOSE_SOCKET;
    }

    at_set_timeout(5000);
    at_rep = at_exec("AT+TRTRM=%d\r", cid);
    if (AT_INTERNAL_TIMEOUT == at_rep)
    {
        api_ret = WIFI_ERR_MODULE_TIMEOUT;
        goto RETURN_ERROR;
    }
    else if (AT_OK != at_rep)
    {
        api_ret = WIFI_ERR_MODULE_COM;
        goto RETURN_ERROR;
    }

CLOSE_SOCKET:
    R_BYTEQ_Close(g_sock_tbl[socket_number].byteq_hdl);
    g_sock_tbl[socket_number].put_err_cnt = 0;
    g_sock_tbl[socket_number].status = WIFI_SOCKET_STATUS_CLOSED;
    g_sock_list[cid] = UINT8_MAX;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_CloseSocket: socket %lu is closed!", socket_number));

RETURN_ERROR:
    at_set_timeout(ATCMD_RESP_TIMEOUT);
    return api_ret;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_CloseSocket
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_TcpReconnect
 * Description  : Reconnect TCP socket.
 * Arguments    : socket_number - UINT8_MAX for reconnect all disconnected sockets.
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_SOCKET_NUM
 *                WIFI_ERR_TAKE_MUTEX
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_TcpReconnect (uint32_t socket_number)
{
    uint8_t cid;
    uint8_t sock_num;

    if (UINT8_MAX == socket_number)
    {
        for (cid = 0; cid < TCP_TBL_MAX + 2; cid++)
        {
            if (UINT8_MAX != g_sock_list[cid])
            {
                sock_num = g_sock_list[cid];
                R_WIFI_DA16XXX_TcpConnect(sock_num, g_sock_tbl[sock_num].ipaddr, g_sock_tbl[sock_num].port);
            }
        }
        return WIFI_SUCCESS;
    }
    else
    {
        return R_WIFI_DA16XXX_TcpConnect(socket_number,
                                         g_sock_tbl[socket_number].ipaddr,
                                         g_sock_tbl[socket_number].port);
    }
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_TcpReconnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_socket_data
 * Description  : Handles incoming socket data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_socket_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data)
{
    uint8_t cid = UINT8_MAX;

    switch (*state)
    {
        case WIFI_RECV_PARAM_CID:
        {
            if (',' == data)
            {
                g_rx_buff[g_rx_idx] = 0;
                cid = strtol((char *) g_rx_buff, NULL, 10);
                if (*type == WIFI_RESP_TRXTC)
                {
                    /* Socket disconnect event received */
                    g_rx_idx = 0;
                    g_cur_sock_idx = g_sock_list[cid];
                    g_sock_tbl[g_cur_sock_idx].put_err_cnt = 0;
                    g_sock_tbl[g_cur_sock_idx].status = WIFI_SOCKET_STATUS_SOCKET;
                    *type = WIFI_RESP_NONE;
                    *state = WIFI_RECV_PREFIX;
                }
                else if (*type == WIFI_RESP_TRDTC)
                {
                    g_cur_sock_idx = g_sock_list[cid];
                    *state = WIFI_RECV_PARAM_IP;
                }
            }
            else
            {
                g_rx_buff[g_rx_idx++] = data;
                g_rx_idx = g_rx_idx % TEMP_BUF_MAX;
            }
            break;
        }
        case WIFI_RECV_PARAM_IP:
        {
            if (',' == data)
            {
                *state = WIFI_RECV_PARAM_PORT;
            }
            break;
        }
        case WIFI_RECV_PARAM_PORT:
        {
            if (',' == data)
            {
                *state = WIFI_RECV_PARAM_LEN;
                g_rx_idx = 0;
            }
            break;
        }
        case WIFI_RECV_PARAM_LEN:
        {
            if (',' == data)
            {
                g_rx_buff[g_rx_idx] = 0;
                *state = WIFI_RECV_DATA;
                g_sock_tbl[g_cur_sock_idx].recv_len = strtol((char *) g_rx_buff, NULL, 10);
            }
            else
            {
                g_rx_buff[g_rx_idx++] = data;
                g_rx_idx = g_rx_idx % TEMP_BUF_MAX;
            }
            break;
        }

        case WIFI_RECV_DATA:
        {
            if (0 < g_sock_tbl[g_cur_sock_idx].recv_len--)
            {
                if (BYTEQ_SUCCESS != R_BYTEQ_Put(g_sock_tbl[g_cur_sock_idx].byteq_hdl, data))
                {
                    g_sock_tbl[g_cur_sock_idx].put_err_cnt++;
                    post_err_event(WIFI_EVENT_SOCKET_RXQ_OVF_ERR, g_cur_sock_idx);
                }
            }

            if (0 >= g_sock_tbl[g_cur_sock_idx].recv_len)
            {
                g_rx_idx = 0;
                g_cur_sock_idx = UINT8_MAX;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }

        default:
        {
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function da16xxx_handle_incoming_socket_data
 *********************************************************************************************************************/
#endif /* WIFI_CFG_TCP_SUPPORT */
