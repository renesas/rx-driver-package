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
 * File Name    : r_wifi_da16xxx_http.c
 * Description  : HTTP protocol API functions definition for DA16XXX.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if WIFI_CFG_HTTP_SUPPORT == 1
#if WIFI_CFG_HTTP_TLS_AUTH != 0
#include WIFI_CFG_HTTP_CERTS_HEADER

#define HTTP_CERT_INCLUDE
#endif /* WIFI_CFG_HTTP_TLS_AUTH */

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* HTTP table information */
typedef struct st_da16xxx_http
{
    wifi_http_buffer_t    * buffer;                         ///< HTTP response buffer.
    uint8_t                 alpn_count;                     ///< ALPN Protocols count. Max value is 3.
    wifi_http_tls_auth_t    tls_level;                      ///< HTTP TLS Authentication level.
    const char            * p_root_ca;                      ///< String representing a trusted server root certificate.
    uint32_t                root_ca_size;                   ///< Size associated with root CA Certificate.
    const char            * p_client_cert;                  ///< String representing a Client certificate.
    uint32_t                client_cert_size;               ///< Size associated with Client certificate.
    const char            * p_client_private_key;           ///< String representing Client Private Key.
    uint32_t                private_key_size;               ///< Size associated with Client Private Key.
    uint32_t                open;                           ///< Flag to track HTTP status.
    volatile uint8_t        request_status;                 ///< Flag to track HTTP request status.
} st_da16xxx_http_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
st_da16xxx_http_t g_http_tbl;
static volatile uint8_t g_rx_idx = 0;
static volatile bool g_http_stt_code = false;
static char* g_http_init_rsp_buf = NULL;

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* sub functions */
static uint8_t get_alpn_number(void);

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HttpOpen
 * Description  : Initialize the DA16XXX on-chip HTTP Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *                WIFI_ERR_HTTP_ALREADY_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HttpOpen(void)
{
    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: R_WIFI_DA16XXX_Connect is not called!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    if (DA16XXX_HTTP_CLOSE != g_http_tbl.open)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen is called before!"));
        return WIFI_ERR_HTTP_ALREADY_OPEN;
    }

    memset(&g_http_tbl, 0, sizeof(g_http_tbl));

    /* Clear ALPN and SNI settings */
    if (AT_OK != at_exec("AT+NWHTCALPNDEL\r"))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot clear ALPN setting!"));
        return WIFI_ERR_MODULE_COM;
    }

    if (AT_OK != at_exec("AT+NWHTCSNIDEL\r"))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot clear SNI setting!"));
        return WIFI_ERR_MODULE_COM;
    }

    /* Get ALPN number */
    g_http_tbl.alpn_count = get_alpn_number();

    /* Set ALPN settings */
    if (g_http_tbl.alpn_count != 0)
    {
        /* take mutex */
        if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
        {
            return WIFI_ERR_TAKE_MUTEX;
        }

        at_send("AT+NWHTCALPN=%d", g_http_tbl.alpn_count);

        if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN1), ""))
        {
            at_send_raw((uint8_t *) ",", 1);
            at_send_raw((uint8_t *) WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN1),
                        strlen(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN1)));
        }
        if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN2), ""))
        {
            at_send_raw((uint8_t *) ",", 1);
            at_send_raw((uint8_t *) WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN2),
                        strlen(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN2)));
        }
        if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN3), ""))
        {
            at_send_raw((uint8_t *) ",", 1);
            at_send_raw((uint8_t *) WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN3),
                        strlen(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN3)));
        }

        if (AT_OK != at_exec_wo_mutex("\r"))
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot set ALPN!"));
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
            return WIFI_ERR_MODULE_COM;
        }

        /* give mutex */
        os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
    }

    /* Set Server Name Indication (SNI) */
    if ((0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_SNI_NAME), "")))
    {
        if (AT_OK != at_exec("AT+NWHTCSNI=%s\r", WIFI_STRING_CONVERT(WIFI_CFG_HTTP_SNI_NAME)))
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot set SNI!"));
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* Set TLS Authentication Level */
    g_http_tbl.tls_level = (wifi_http_tls_auth_t) WIFI_CFG_HTTP_TLS_AUTH;
    if (AT_OK != at_exec("AT+NWHTCTLSAUTH=%d\r", g_http_tbl.tls_level))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot set TLS Authentication Level!"));
        return WIFI_ERR_MODULE_COM;
    }
#ifdef HTTP_CERT_INCLUDE
    g_http_tbl.p_root_ca = WIFI_CFG_HTTP_ROOT_CA;
    g_http_tbl.root_ca_size = strlen(g_http_tbl.p_root_ca);
    g_http_tbl.p_client_cert = WIFI_CFG_HTTP_CLIENT_CERT;
    g_http_tbl.client_cert_size = strlen(g_http_tbl.p_client_cert);
    g_http_tbl.p_client_private_key = WIFI_CFG_HTTP_PRIVATE_KEY;
    g_http_tbl.private_key_size = strlen(g_http_tbl.p_client_private_key);

    if ((g_http_tbl.root_ca_size > DA16XXX_MAX_CERT_SIZE) ||
        (g_http_tbl.client_cert_size > DA16XXX_MAX_CERT_SIZE) ||
        (g_http_tbl.private_key_size > DA16XXX_MAX_CERT_SIZE))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: The key provided is larger than the maximum size (%d)!",
                        DA16XXX_MAX_CERT_SIZE));
        return WIFI_ERR_PARAMETER;
    }
#else
    g_http_tbl.p_root_ca = NULL;
    g_http_tbl.root_ca_size = 0;
    g_http_tbl.p_client_cert = NULL;
    g_http_tbl.client_cert_size = 0;
    g_http_tbl.p_client_private_key = NULL;
    g_http_tbl.private_key_size = 0;
#endif

    /* Send TLS certificate verification */
    if ((NULL != g_http_tbl.p_root_ca) && (NULL != g_http_tbl.p_client_cert) &&
        (NULL != g_http_tbl.p_client_private_key))
    {
        /* take mutex */
        if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
        {
            return WIFI_ERR_TAKE_MUTEX;
        }
        /* Program the root CA Certificate */

        /* Put the DA16XXX module into certificate/key input mode */
        at_send("%sC3,", DA16XXX_CERT_START);

        /* Send certificate/key ascii text */
        at_send_raw((uint8_t WIFI_FAR *) g_http_tbl.p_root_ca, g_http_tbl.root_ca_size);

        /* Send Indication of the end of content  */
        at_send("%s", DA16XXX_CERT_END);
        if (AT_OK != at_recv())
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot write root CA certificate!"));
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
            return WIFI_ERR_MODULE_COM;
        }

        /* Program the client certificate */

        /* Put the DA16XXX module into certificate/key input mode */
        at_send("%sC4,", DA16XXX_CERT_START);

        /* Send certificate/key ascii text */
        at_send_raw((uint8_t WIFI_FAR *) g_http_tbl.p_client_cert, g_http_tbl.client_cert_size);

        /* Send Indication of the end of content  */
        at_send("%s", DA16XXX_CERT_END);
        if (AT_OK != at_recv())
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot write client certificate!"));
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
            return WIFI_ERR_MODULE_COM;
        }

        /* Program the private key */

        /* Put the DA16XXX module into certificate/key input mode */
        at_send("%sC5,", DA16XXX_CERT_START);

        /* Send certificate/key ascii text */
        at_send_raw((uint8_t WIFI_FAR *) g_http_tbl.p_client_private_key, g_http_tbl.private_key_size);

        /* Send Indication of the end of content  */
        at_send("%s", DA16XXX_CERT_END);
        if (AT_OK != at_recv())
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpOpen: Cannot write private key!"));
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
            return WIFI_ERR_MODULE_COM;
        }

        /* give mutex */
        os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
    }

    g_http_tbl.open = DA16XXX_HTTP_OPEN;
    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_HttpOpen
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HttpClose
 * Description  : Close the DA16XXX HTTP Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_HTTP_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HttpClose(void)
{
    /* Do parameter checking */
    if (g_http_tbl.open != DA16XXX_HTTP_OPEN)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpClose: HTTP Client is not opened!"));
        return WIFI_ERR_HTTP_NOT_OPEN;
    }

    /* Clear ALPN and SNI settings */
    if (AT_OK != at_exec("AT+NWHTCALPNDEL\r"))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpClose: Cannot clear ALPN setting!"));
        return WIFI_ERR_MODULE_COM;
    }

    if (AT_OK != at_exec("AT+NWHTCSNIDEL\r"))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpClose: Cannot clear SNI setting!"));
        return WIFI_ERR_MODULE_COM;
    }

    g_http_tbl.open = DA16XXX_HTTP_CLOSE;
    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_HttpClose
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_HttpSend
 * Description  : Send the HTTP request with the configured buffers.
 * Arguments    : request
 *                buffer
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_OPEN
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_TAKE_MUTEX
 *                WIFI_ERR_HTTP_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_HttpSend (wifi_http_request_t request, wifi_http_buffer_t *buffer)
{
    e_rslt_code_t ret = AT_OK;
    uint8_t retry_count;
    uint8_t retry_max = 10;
    const char * http_request[3] = {"get", "post", "put"};

    /* Do parameter checking */
    if (0 != R_WIFI_DA16XXX_IsOpened())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: R_WIFI_DA16XXX_Open is not called!"));
        return WIFI_ERR_NOT_OPEN;
    }

    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: R_WIFI_DA16XXX_Connect is not called!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    if (DA16XXX_HTTP_OPEN != g_http_tbl.open)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: R_WIFI_DA16XXX_HttpOpen is not called!"));
        return WIFI_ERR_HTTP_NOT_OPEN;
    }

    if ((NULL == request.http_endpoint) && (NULL == buffer))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: Argument is NULL!"));
        return WIFI_ERR_PARAMETER;
    }

    if (NULL == buffer->response_buffer)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: Response buffer is NULL!"));
        return WIFI_ERR_PARAMETER;
    }

    /* Point to use's response buffer */
    g_http_tbl.buffer = buffer;

    /* Save the initial response_buffer's address */
    g_http_init_rsp_buf = g_http_tbl.buffer->response_buffer;

    /* Reset HTTP buffer length*/
    g_http_tbl.buffer->resp_length = 0;

    /* Reset HTTP request status */
    g_http_tbl.request_status = 0x00;

    /* take mutex */
    if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
    {
        return WIFI_ERR_TAKE_MUTEX;
    }

    /* Send out the HTTP request */
    if (AT_OK != at_send("AT+NWHTCH=%s,%s", request.http_endpoint, http_request[request.method]))
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: Cannot send out request header!"));
        os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
        return WIFI_ERR_MODULE_COM;
    }

    if (NULL != request.request_body)
    {
        at_send_raw((uint8_t WIFI_FAR *) ",'", 2);
        if (AT_OK != at_send_raw((uint8_t WIFI_FAR *) request.request_body, request.length))
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: Cannot send request body!"));
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
            return WIFI_ERR_MODULE_COM;
        }
        at_send_raw((uint8_t WIFI_FAR *) "'", 1);
    }

    /* Send and get AT CMD response */
    ret = at_exec_wo_mutex("\r");
    if (AT_OK != ret)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: Cannot send out the HTTP request (%d)!", ret));
        os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
        return WIFI_ERR_MODULE_COM;
    }

    /* give mutex */
    os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);

    /* Waiting HTTP request status */
    for (retry_count = 0; retry_count < retry_max; retry_count++)
    {
        if (g_http_stt_code == true)
        {
            g_http_stt_code = false;
            break;
        }
        else
        {
            os_wrap_sleep(1000, UNIT_MSEC);
        }
    }

    if (retry_count == retry_max)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: HTTP request timed out!"));
        return WIFI_ERR_MODULE_COM;
    }
    else if (g_http_tbl.request_status != 0x00)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_HttpSend: HTTP request status error (%#04X))!", g_http_tbl.request_status));
        return WIFI_ERR_MODULE_COM;
    }
    else
    {
        return WIFI_SUCCESS;
    }
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_HttpSend
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: get_alpn_number
 * Description  : Get the number of ALPN.
 * Arguments    : none
 * Return Value : number of ALPN
 *********************************************************************************************************************/
static uint8_t get_alpn_number(void)
{
    /* Count alpn */
    uint8_t count = 0;

    /* If alpn populated then increment */
    if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN1), ""))
    {
        count++;
    }
    if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN2), ""))
    {
        count++;
    }
    if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_HTTP_ALPN3), ""))
    {
        count++;
    }

    return count;
}
/**********************************************************************************************************************
 * End of function get_alpn_number
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_http_data
 * Description  : Handles incoming HTTP data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_http_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data)
{
    switch (*state)
    {
        case WIFI_RECV_PARAM_CID:
        {
            if (g_rx_idx > 3)
            {
                g_rx_idx = 0;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            else if (data == '\r')
            {
                g_rx_idx = 0;
                g_http_stt_code = true;
                g_http_tbl.buffer->response_buffer = g_http_init_rsp_buf;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            else if (data >= '0' && data <= '9')
            {
                g_http_tbl.request_status = g_http_tbl.request_status * 10 + (data - '0');
                g_rx_idx++;
            }
            else
            {
                g_rx_idx = 0;
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }

        case WIFI_RECV_PARAM_LEN:
        {
            if (data == ',')
            {
                *state = WIFI_RECV_DATA;
            }
            else if (data >= '0' && data <= '9')
            {
                g_http_tbl.buffer->resp_length = g_http_tbl.buffer->resp_length * 10 + (data - '0');
            }
            else
            {
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }

        case WIFI_RECV_DATA:
        {
            if (g_http_tbl.buffer->resp_length > 0)
            {
                *(g_http_tbl.buffer->response_buffer++) = data;
                *(g_http_tbl.buffer->response_buffer) = '\0';
                g_http_tbl.buffer->resp_length--;
            }

            if (g_http_tbl.buffer->resp_length <= 0)
            {
                *type = WIFI_RESP_NONE;
                *state = WIFI_RECV_PREFIX;
            }
            break;
        }

        default:
        {
            g_rx_idx = 0;
            *type = WIFI_RESP_NONE;
            *state = WIFI_RECV_PREFIX;
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function da16xxx_handle_incoming_http_data
 *********************************************************************************************************************/
#endif /* WIFI_CFG_HTTP_SUPPORT */
