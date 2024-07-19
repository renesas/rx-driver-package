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
 * File Name    : r_wifi_da16xxx_mqtt.c
 * Description  : MQTT protocol API functions definition for DA16XXX.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_wifi_da16xxx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if WIFI_CFG_MQTT_SUPPORT == 1

#if WIFI_CFG_MQTT_CERTS == 1
#include WIFI_CFG_MQTT_CERTS_HEADER

#define CERT_INCLUDE
#endif /* WIFI_CFG_MQTT_CERTS */

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/* MQTT table information */
typedef struct st_da16xxx_mqtt
{
    /* Flag to use MQTT v3.1.1. */
    uint8_t  use_mqtt_v311;
    /* MQTT Rx timer. */
    st_wifi_timer rx_timer;
    /* MQTT Tx timer. */
    st_wifi_timer tx_timer;

    /* Location of user callback. */
    void (* p_mqtt_callback)(wifi_mqtt_callback_args_t * p_args);
    /* Placeholder for user data. Passed to the user callback in mqtt_onchip_da16xxx_callback_args_t. */
    void const   * p_context;
    /* Whether to establish a new, clean session or resume a previous session. */
    uint8_t        clean_session;
    /* ALPN Protocols count. Max value is 3. */
    uint8_t        alpn_count;
    /* ALPN Protocols. */
    const char   * p_alpns[DA16XXX_MAX_ALPN];
    /* TLS Cipher suites count. Max value is 17. */
    uint8_t        tls_cipher_count;
    /* MQTT keep alive period. */
    uint16_t       keep_alive_seconds;
    /* MQTT Client identifier. Must be unique per client. */
    const char   * p_client_identifier;
    /* Length of the client identifier. */
    uint16_t       client_identifier_length;
    /* MQTT endpoint host name. */
    const char   * p_host_name;
    /* MQTT Port number. */
    uint16_t       mqtt_port;
    /* MQTT user name. Set to NULL if not used. */
    const char   * p_mqtt_user_name;
    /* Length of MQTT user name. Set to 0 if not used. */
    uint16_t       user_name_length;
    /* MQTT password. Set to NULL if not used. */
    const char   * p_mqtt_password;
    /* Length of MQTT password. Set to 0 if not used. */
    uint16_t       password_length;
    /* String representing a trusted server root certificate. */
    const char   * p_root_ca;
    /* Size associated with root CA Certificate. */
    uint32_t       root_ca_size;
    /* String representing a Client certificate. */
    const char   * p_client_cert;
    /* Size associated with Client certificate. */
    uint32_t       client_cert_size;
    /* String representing Client Private Key. */
    const char   * p_client_private_key;
    /* Size associated with Client Private Key. */
    uint32_t       private_key_size;
    /* String representing Will Topic. */
    const char   * p_will_topic;
    /* String representing Will Message. */
    const char   * p_will_msg;
    /* Server Name Indication. */
    const char   * p_sni_name;

    /* Will Topic QoS level. */
    wifi_mqtt_qos_t          will_qos_level;
    /* TLS Cipher suites supported. */
    wifi_tls_cipher_suites_t p_tls_cipher_suites[DA16XXX_MQTT_TLS_CIPHER_SUITE_MAX];

    /* Command send buffer. */
    uint8_t  cmd_tx_buff[WIFI_CFG_MQTT_CMD_TX_BUF_SIZE];
    /* Command receive buffer. */
    uint8_t  cmd_rx_buff[WIFI_CFG_MQTT_CMD_RX_BUF_SIZE];
    /* Flag to track MQTT status. */
    volatile e_mqtt_module_status_t     mqtt_status;
    /* Flag to track MQTT publish message status. */
    volatile bool     mqtt_published_status;
    byteq_hdl_t             byteq_hdl;
} st_da16xxx_mqtt_t;

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
st_da16xxx_mqtt_t g_mqtt_tbl;

/* User callback */
#if(1 == WIFI_CFG_MQTT_P_CALLBACK)
void WIFI_CFG_MQTT_P_CALLBACK_FUNCTION_NAME(wifi_mqtt_callback_args_t * p_args);
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/* sub functions */
static uint8_t get_alpn_number(void);
static uint8_t get_cipher_suites_number(void);

/* WIFI MQTT on chip configuration */
static wifi_err_t r_wifi_da16xxx_mqtt_optional_init(void);
static void r_wifi_da16xxx_mqtt_instance_init(void);

/* receive data MQTT buffer */
static uint32_t r_wifi_da16xxx_mqtt_buffer_recv(uint8_t * data, uint32_t length, uint32_t timeout);

/* BYTEQ functions */
static wifi_err_t mqtt_byteq_open(void);
static void mqtt_byteq_close(void);

/* buffer control */
static uint8_t  g_mqtt_buf[WIFI_CFG_MQTT_CMD_RX_BUF_SIZE];
static volatile bool g_mqtt_msg_ended = false;

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttOpen
 * Description  : Initialize the DA16XXX on-chip MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_NOT_CONNECT
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_ALREADY_OPEN
 *                WIFI_ERR_MQTT_INVALID_DATA
 *                WIFI_ERR_MQTT_OUT_OF_MEMORY
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttOpen(void)
{
    /* MQTT connected to access point or not */
    if (0 != R_WIFI_DA16XXX_IsConnected())
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttOpen: Not connected to access point!"));
        return WIFI_ERR_NOT_CONNECT;
    }

    /* Do parameter checking */
    if (DA16XXX_MQTT_CLOSE != g_mqtt_tbl.mqtt_status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttOpen is called before!"));
        return WIFI_ERR_MQTT_ALREADY_OPEN;
    }

    /* MQTT instance initialize */
    r_wifi_da16xxx_mqtt_instance_init();

    /* Initialize BYTEQ on MQTT table */
    if (WIFI_SUCCESS != mqtt_byteq_open())
    {
        return WIFI_ERR_BYTEQ_OPEN;
    }

    /* Clear the stored SNI configurations */
    if (AT_OK != at_exec("AT+NWHTCSNIDEL\r"))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Clear the stored ALPNs configurations */
    if (AT_OK != at_exec("AT+NWHTCALPNDEL\r"))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Set MQTT 3.1.1 settings */
    if (AT_OK != at_exec("AT+NWMQV311=%d\r", g_mqtt_tbl.use_mqtt_v311))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Check for host name and port */
    if (0 == strcmp(g_mqtt_tbl.p_host_name, ""))
    {
        return WIFI_ERR_MQTT_INVALID_DATA;
    }
    if (0 == g_mqtt_tbl.mqtt_port)
    {
        return WIFI_ERR_MQTT_INVALID_DATA;
    }

    /* Set the host name of the MQTT broker with AT+NWMQBR and send */
    if (AT_OK != at_exec("AT+NWMQBR=%s,%d\r", g_mqtt_tbl.p_host_name, g_mqtt_tbl.mqtt_port))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Store the MQTT Username and Password and send */
    if ((0 != strcmp(g_mqtt_tbl.p_mqtt_user_name, "")) && (0 != strcmp(g_mqtt_tbl.p_mqtt_password, "")))
    {
        if (AT_OK != at_exec("AT+NWMQLI='%s','%s'\r", g_mqtt_tbl.p_mqtt_user_name, g_mqtt_tbl.p_mqtt_password))
        {
            return WIFI_ERR_MODULE_COM;
        }

        /* Disable TLS with AT+NWMQTLS */
        if (AT_OK != at_exec("AT+NWMQTLS=0\r"))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }
    else
    {
        /* Store the TLS certificate/private key */
        if ((NULL != g_mqtt_tbl.p_root_ca) && (NULL != g_mqtt_tbl.p_client_cert) &&
            (NULL != g_mqtt_tbl.p_client_private_key))
        {
            /* Check the certificates/keys provided to ensure the TX buffer is large enough (-3 for command string e.g. "C1,") */
            if ((g_mqtt_tbl.root_ca_size > (DA16XXX_MAX_CERT_SIZE - 3)) ||
                (g_mqtt_tbl.client_cert_size > (DA16XXX_MAX_CERT_SIZE - 3)) ||
                (g_mqtt_tbl.private_key_size > (DA16XXX_MAX_CERT_SIZE - 3)))
            {
                return WIFI_ERR_MQTT_OUT_OF_MEMORY;
            }

            /* Enable TLS with AT+NWMQTLS */
            if (AT_OK != at_exec("AT+NWMQTLS=1\r"))
            {
                return WIFI_ERR_MODULE_COM;
            }

            /* take mutex */
            if (OS_WRAP_SUCCESS != os_wrap_mutex_take(MUTEX_TX | MUTEX_RX))
            {
                return WIFI_ERR_TAKE_MUTEX;
            }
            /* Program the root CA Certificate */

            /* Put the DA16XXX module into certificate/key input mode */
            at_send("%sC0,", DA16XXX_CERT_START);

            /* Send certificate/key ascii text */
            at_send_raw((uint8_t WIFI_FAR *) g_mqtt_tbl.p_root_ca, g_mqtt_tbl.root_ca_size);

            /* Send Indication of the end of content  */
            at_send("%s", DA16XXX_CERT_END);
            if (AT_OK != at_recv())
            {
                os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
                return WIFI_ERR_MODULE_COM;
            }

            /* Program the client certificate */

            /* Put the DA16XXX module into certificate/key input mode */
            at_send("%sC1,", DA16XXX_CERT_START);

            /* Send certificate/key ascii text */
            at_send_raw((uint8_t WIFI_FAR *) g_mqtt_tbl.p_client_cert, g_mqtt_tbl.client_cert_size);

            /* Send Indication of the end of content  */
            at_send("%s", DA16XXX_CERT_END);
            if (AT_OK != at_recv())
            {
                os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
                return WIFI_ERR_MODULE_COM;
            }

            /* Program the private key */

            /* Put the DA16XXX module into certificate/key input mode */
            at_send("%sC2,", DA16XXX_CERT_START);

            /* Send certificate/key ascii text */
            at_send_raw((uint8_t WIFI_FAR *) g_mqtt_tbl.p_client_private_key, g_mqtt_tbl.private_key_size);

            /* Send Indication of the end of content  */
            at_send("%s", DA16XXX_CERT_END);
            if (AT_OK != at_recv())
            {
                os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
                return WIFI_ERR_MODULE_COM;
            }

            /* give mutex */
            os_wrap_mutex_give(MUTEX_TX | MUTEX_RX);
        }
    }

    if (0 == g_mqtt_tbl.keep_alive_seconds)
    {
        return WIFI_ERR_PARAMETER;
    }

    /* Set the MQTT ping period */
    if (AT_OK != at_exec("AT+NWMQPING=%d\r", g_mqtt_tbl.keep_alive_seconds))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Perform MQTT optional settings */
    if (WIFI_SUCCESS != r_wifi_da16xxx_mqtt_optional_init())
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Set MQTT state */
    g_mqtt_tbl.mqtt_status = DA16XXX_MQTT_OPEN;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_MqttOpen: MQTT Client is opened!"));

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttOpen
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttDisconnect
 * Description  : Disconnect from DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttDisconnect(void)
{
    /* Do parameter checking */
    if (DA16XXX_MQTT_CLOSE == g_mqtt_tbl.mqtt_status)
    {
        return WIFI_ERR_MQTT_NOT_OPEN;
    }
    if (DA16XXX_MQTT_CONNECTED != g_mqtt_tbl.mqtt_status)
    {
        return WIFI_ERR_MQTT_NOT_CONNECT;
    }

    /* Disable the MQTT Client */
    if (AT_OK != at_exec("AT+NWMQCL=0\r"))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Set MQTT state */
    g_mqtt_tbl.mqtt_status = DA16XXX_MQTT_DISCONNECTED;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_MqttDisconnect: Disconnected the MQTT Broker!"));

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttDisconnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttConnect
 * Description  : Configure and connect the DA16XXX MQTT Client service.
 * Arguments    : timeout_ms
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *                WIFI_ERR_MQTT_CONNECTED
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttConnect(uint32_t timeout_ms)
{
    uint8_t retry_count;
    uint8_t retry_max = 30;

    /* Do parameter checking */
    if (DA16XXX_MQTT_CLOSE == g_mqtt_tbl.mqtt_status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttConnect: MQTT Client is not opened!"));
        return WIFI_ERR_MQTT_NOT_OPEN;
    }
    if (DA16XXX_MQTT_CONNECTED == g_mqtt_tbl.mqtt_status)
    {
        return WIFI_ERR_MQTT_CONNECTED;
    }

    /* Set MQTT status to disconnected */
    g_mqtt_tbl.mqtt_status = DA16XXX_MQTT_DISCONNECTED;

    /* Enable the MQTT Client */
    at_set_timeout(timeout_ms);
    if (AT_OK != at_exec("AT+NWMQCL=1\r"))
    {
        return WIFI_ERR_MODULE_COM;
    }
    at_set_timeout(ATCMD_RESP_TIMEOUT);

    for (retry_count = 0; retry_count < retry_max; retry_count++)
    {
        if (g_mqtt_tbl.mqtt_status == DA16XXX_MQTT_CONNECTED)
        {
            break;
        }
        else
        {
            os_wrap_sleep(1000, UNIT_MSEC);
        }
    }

    if (retry_count == retry_max)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttConnect: Failed to connect to the MQTT Broker!"));
        return WIFI_ERR_MODULE_COM;
    }
    else
    {
        WIFI_LOG_INFO(("R_WIFI_DA16XXX_MqttConnect: Connected to the MQTT Broker!"));
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttConnect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttPublish
 * Description  : Publish a message for a given MQTT topic.
 * Arguments    : p_pub_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *                WIFI_ERR_MQTT_INVALID_DATA
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttPublish(wifi_mqtt_pub_info_t * const p_pub_info)
{
    uint8_t retry_count;
    uint8_t retry_max = 5;

    /* Do parameter checking */
    if (NULL == p_pub_info)
    {
        return WIFI_ERR_PARAMETER;
    }
    if (p_pub_info->payload_length > DA16XXX_MQTT_MAX_PUBMSG_LEN)
    {
        return WIFI_ERR_PARAMETER;
    }
    if (p_pub_info->topic_name_Length > DA16XXX_MQTT_MAX_TOPIC_LEN)
    {
        return WIFI_ERR_PARAMETER;
    }
    if (DA16XXX_MQTT_CONNECTED != g_mqtt_tbl.mqtt_status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttPublish: MQTT Client is not connected to the MQTT Broker!"));
        return WIFI_ERR_MQTT_NOT_CONNECT;
    }

    /* Publish an MQTT message with topic and payload */
    if ((p_pub_info->topic_name_Length + p_pub_info->payload_length) < DA16XXX_MQTT_MAX_PUBTOPICMSG_LEN)
    {
        /* Set MQTT QoS level settings */
        if (AT_OK != at_exec("AT+NWMQQOS=%d\r", p_pub_info->qos))
        {
            return WIFI_ERR_MODULE_COM;
        }

#define TOPIC_AT_PREFIX_LENGTH 11
        if ((TOPIC_AT_PREFIX_LENGTH + strlen(p_pub_info->p_payload) +
                strlen(p_pub_info->p_topic_name)) >= WIFI_CFG_MQTT_CMD_TX_BUF_SIZE)
        {
            return WIFI_ERR_MQTT_INVALID_DATA;
        }

        if (AT_OK != at_exec("AT+NWMQMSG='%s','%s'\r", p_pub_info->p_payload, p_pub_info->p_topic_name))
        {
            return WIFI_ERR_MODULE_COM;
        }

        for (retry_count = 0; retry_count < retry_max; retry_count++)
        {
            if (g_mqtt_tbl.mqtt_published_status == true)
            {
                g_mqtt_tbl.mqtt_published_status = false;
                break;
            }
            else
            {
                os_wrap_sleep(1000, UNIT_MSEC);
            }
        }

        if (retry_count == retry_max)
        {
            WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttPublish: Failed to publish message to the MQTT Broker!\n"));
            return WIFI_ERR_NOT_CONNECT;
        }
    }
    else
    {
        return WIFI_ERR_MQTT_INVALID_DATA;
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttPublish
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttSubscribe
 * Description  : Subscribe to DA16XXX MQTT topics.
 * Arguments    : p_sub_info
 *                subscription_count
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *                WIFI_ERR_MQTT_INVALID_DATA
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttSubscribe(wifi_mqtt_sub_info_t * const p_sub_info,
                                    size_t subscription_count)
{
    uint8_t * at_cmd = g_mqtt_tbl.cmd_tx_buff;

    /* Do parameter checking */
    if (p_sub_info == NULL)
    {
        return WIFI_ERR_PARAMETER;
    }
    if (subscription_count == 0)
    {
        return WIFI_ERR_PARAMETER;
    }
    if (DA16XXX_MQTT_CLOSE == g_mqtt_tbl.mqtt_status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttSubscribe: MQTT Client is not opened!"));
        return WIFI_ERR_MQTT_NOT_OPEN;
    }

    /* Set the MQTT subscriber topic with AT+NWMQTS */
    if (subscription_count < DA16XXX_MQTT_SUBTOPIC_MAX_CNT)
    {
        /* Set MQTT QoS level settings */
        if (AT_OK != at_exec("AT+NWMQQOS=%d\r", p_sub_info[0].qos))
        {
            return WIFI_ERR_MODULE_COM;
        }

        /* Create subscription command by concatenating topics */
        snprintf((char *) at_cmd, WIFI_CFG_MQTT_CMD_TX_BUF_SIZE, "AT+NWMQTS=%d", subscription_count);

        for (int i = 0; i < (int) subscription_count; i++)
        {
            if ((strlen((char *) at_cmd) + strlen(
                    p_sub_info[i].p_topic_filter + 3)) >= WIFI_CFG_MQTT_CMD_TX_BUF_SIZE)
            {
                return WIFI_ERR_MQTT_INVALID_DATA;
            }
            snprintf((char *) at_cmd + strlen((char *) at_cmd),
                    WIFI_CFG_MQTT_CMD_TX_BUF_SIZE,
                     ",'%s'",
                     p_sub_info[i].p_topic_filter);
        }

        snprintf((char *) at_cmd + strlen((char *) at_cmd), WIFI_CFG_MQTT_CMD_TX_BUF_SIZE, "\r");

        /* Subscribe to a Topic */
        if (AT_OK != at_exec((char *) at_cmd))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }
    else
    {
        return WIFI_ERR_MODULE_COM;
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttSubscribe
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttUnSubscribe
 * Description  : Unsubscribe from DA16XXX MQTT topics.
 * Arguments    : p_sub_info
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_PARAMETER
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *                WIFI_ERR_MQTT_INVALID_DATA
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttUnSubscribe (wifi_mqtt_sub_info_t * const p_sub_info)
{
    /* Do parameter checking */
    if (p_sub_info == NULL)
    {
        return WIFI_ERR_PARAMETER;
    }
    if (DA16XXX_MQTT_CONNECTED != g_mqtt_tbl.mqtt_status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttUnSubscribe: MQTT Client is not connected to the MQTT Broker!"));
        return WIFI_ERR_MQTT_NOT_CONNECT;
    }

#define UNSUB_TOPIC_AT_PREFIX_LENGTH 11
    if ((strlen(p_sub_info->p_topic_filter) + UNSUB_TOPIC_AT_PREFIX_LENGTH ) > WIFI_CFG_MQTT_CMD_TX_BUF_SIZE)
    {
        return WIFI_ERR_MQTT_INVALID_DATA;
    }

    /* UnSubscribe a Topic */
    if (AT_OK != at_exec("AT+NWMQUTS='%s'\r", p_sub_info->p_topic_filter))
    {
        return WIFI_ERR_MODULE_COM;
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttUnSubscribe
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttReceive
 * Description  : Receive data subscribed to on DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MQTT_INVALID_DATA
 *                WIFI_ERR_MQTT_NOT_CONNECT
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttReceive (void)
{
    wifi_mqtt_callback_args_t mqtt_data;
    char * ptr = (char *) g_mqtt_buf;
    uint32_t recv_cnt = 0;

    if (DA16XXX_MQTT_CONNECTED == g_mqtt_tbl.mqtt_status)
    {
        /* Check the received data */
        recv_cnt = r_wifi_da16xxx_mqtt_buffer_recv((uint8_t *) ptr,
                                              WIFI_CFG_MQTT_CMD_RX_BUF_SIZE - 1,
                                              g_mqtt_tbl.rx_timer.threshold);

        if (ptr != NULL && recv_cnt != 0)
        {
            char *p_data;
            char *p_topic;
            char p_length[4];
            char WIFI_FAR * p_data_len = strrchr(ptr, ',');
            if (p_data_len != NULL)
            {
                // Get MQTT message length
                strcpy(p_length, p_data_len + 1);
                long int length = strtol(p_length, NULL, 10);
                if (0 == length)
                {
                    WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttReceive: MQTT Message length is wrong!!!\n"));
                    return WIFI_ERR_MQTT_INVALID_DATA;
                }

                // Get MQTT message
                p_data = (char *)malloc(length + 1);
                strncpy(p_data, ptr, length);
                p_data[length] = '\0';

                // Get MQTT Topic
                size_t topic_len = strlen(ptr) - strlen(p_data) - strlen(p_data_len) - 1;
                p_topic = (char *)malloc(topic_len + 1);
                strncpy(p_topic, ptr + length + 1, topic_len);
                p_topic[topic_len] = '\0';

                /* If all checks were successful, assign temporary data to callback args */
                mqtt_data.p_data      = (uint8_t *)p_data;
                mqtt_data.p_topic     = p_topic;
                mqtt_data.data_length = (uint32_t) length;

                /* Call the user callback with successful data */
                g_mqtt_tbl.p_mqtt_callback(&mqtt_data);

                // Free allocated memory
                free(p_data);
                free(p_topic);
            }
            else
            {
                WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttReceive: MQTT message is wrong format!!!\n"));
                return WIFI_ERR_MQTT_INVALID_DATA;
            }
        }
    }
    else
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttReceive: MQTT Client is not connected to the MQTT Broker!!!\n"));
        return WIFI_ERR_MQTT_NOT_CONNECT;
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttReceive
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_WIFI_DA16XXX_MqttClose
 * Description  : Close the DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *                WIFI_ERR_MQTT_NOT_OPEN
 *********************************************************************************************************************/
wifi_err_t R_WIFI_DA16XXX_MqttClose (void)
{
    /* Close Byteq on MQTT */
    mqtt_byteq_close();

    /* Do parameter checking */
    if (DA16XXX_MQTT_CLOSE == g_mqtt_tbl.mqtt_status)
    {
        WIFI_LOG_ERROR(("R_WIFI_DA16XXX_MqttClose: MQTT Client is not opened!"));
        return WIFI_ERR_MQTT_NOT_OPEN;
    }

    /* Reset the MQTT configurations */
    if (AT_OK != at_exec("AT+NWMQDEL\r"))
    {
        return WIFI_ERR_MODULE_COM;
    }

    /* Set MQTT state */
    g_mqtt_tbl.mqtt_status = DA16XXX_MQTT_CLOSE;
    WIFI_LOG_INFO(("R_WIFI_DA16XXX_MqttClose: Closed MQTT Client!"));

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function R_WIFI_DA16XXX_MqttClose
 *********************************************************************************************************************/

/*
 * Sub functions
 */
/**********************************************************************************************************************
 * Function Name: r_wifi_da16xxx_mqtt_optional_init
 * Description  : Optional configurations for DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_MODULE_COM
 *********************************************************************************************************************/
static wifi_err_t r_wifi_da16xxx_mqtt_optional_init(void)
{
    uint8_t * at_cmd = g_mqtt_tbl.cmd_tx_buff;

    /* Set MQTT ALPN settings */
    if (g_mqtt_tbl.alpn_count != 0)
    {
        /* Set up ALPN protocol and count packet */
        snprintf((char *) at_cmd, WIFI_CFG_MQTT_CMD_TX_BUF_SIZE, "AT+NWMQALPN=%d", g_mqtt_tbl.alpn_count);

        for (int i = 0; i < DA16XXX_MAX_ALPN; i++)
        {
            if (0 != strcmp(g_mqtt_tbl.p_alpns[i], ""))
            {
                snprintf((char *) at_cmd + strlen((char *) at_cmd),
                         WIFI_CFG_MQTT_CMD_TX_BUF_SIZE,
                         ",%s",
                         g_mqtt_tbl.p_alpns[i]);
            }
        }

        snprintf((char *) at_cmd + strlen((char *) at_cmd), WIFI_CFG_MQTT_CMD_TX_BUF_SIZE, "\r");

        if (AT_OK != at_exec((char *) at_cmd))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* Set MQTT TLS Cipher Suite settings */
    if (g_mqtt_tbl.tls_cipher_count > 0)
    {
        strncpy((char *) at_cmd, "AT+NWMQCSUIT=", WIFI_CFG_MQTT_CMD_TX_BUF_SIZE);

        for (int i = 0; i < DA16XXX_MQTT_TLS_CIPHER_SUITE_MAX; i++)
        {
            if (0 != g_mqtt_tbl.p_tls_cipher_suites[i])
            {
                snprintf((char *) at_cmd + strlen((char *) at_cmd),
                         WIFI_CFG_MQTT_CMD_TX_BUF_SIZE,
                         "%x,",
                         g_mqtt_tbl.p_tls_cipher_suites[i]);
            }
        }

        snprintf((char *) at_cmd + strlen((char *) at_cmd) - 1,
                WIFI_CFG_MQTT_CMD_TX_BUF_SIZE,
                 "\r");

        if (AT_OK != at_exec((char *) at_cmd))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* Set MQTT SNI settings */
    if ((0 != strcmp(g_mqtt_tbl.p_sni_name, "")) && (strlen(g_mqtt_tbl.p_sni_name) < DA16XXX_MAX_SNI_LEN))
    {
        if (AT_OK != at_exec("AT+NWMQSNI=%s\r", g_mqtt_tbl.p_sni_name))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* Set MQTT Clean Session settings */
    if (0 == g_mqtt_tbl.clean_session)
    {
        if (AT_OK != at_exec("AT+NWMQCS=0\r"))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* Set MQTT Client Identifier settings */
    if (0 != g_mqtt_tbl.client_identifier_length)
    {
        if (AT_OK != at_exec("AT+NWMQCID=%s\r", g_mqtt_tbl.p_client_identifier))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    /* Set MQTT Last Will settings */
    if (0 != strcmp(g_mqtt_tbl.p_will_topic, ""))
    {
        if (AT_OK != at_exec("AT+NWMQWILL='%s','%s',%d\r",
                             g_mqtt_tbl.p_will_topic,
                             g_mqtt_tbl.p_will_msg,
                             g_mqtt_tbl.will_qos_level))
        {
            return WIFI_ERR_MODULE_COM;
        }
    }

    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function r_wifi_da16xxx_mqtt_optional_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_wifi_da16xxx_mqtt_instance_init
 * Description  : MQTT instance configurations for DA16XXX MQTT Client service.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void r_wifi_da16xxx_mqtt_instance_init(void)
{
    /* Memory initialize */
    memset(&g_mqtt_tbl, 0, sizeof(g_mqtt_tbl));

    /* User configurations */
    g_mqtt_tbl.use_mqtt_v311 = WIFI_CFG_MQTT_USE_MQTT_V311;
    g_mqtt_tbl.rx_timer.threshold = OS_WRAP_MS_TO_TICKS(WIFI_CFG_MQTT_RX_TIMEOUT);
    g_mqtt_tbl.tx_timer.threshold = OS_WRAP_MS_TO_TICKS(WIFI_CFG_MQTT_TX_TIMEOUT);

#if WIFI_CFG_MQTT_P_CALLBACK == 1
    g_mqtt_tbl.p_mqtt_callback = WIFI_CFG_MQTT_P_CALLBACK_FUNCTION_NAME;
#else
    g_mqtt_tbl.p_mqtt_callback = NULL;
#endif

    g_mqtt_tbl.clean_session = WIFI_CFG_MQTT_CLEAN_SESSION;

    /* Assign ALPN */
    g_mqtt_tbl.alpn_count = get_alpn_number();
    g_mqtt_tbl.p_alpns[0] = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_ALPN1);
    g_mqtt_tbl.p_alpns[1] = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_ALPN2);
    g_mqtt_tbl.p_alpns[2] = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_ALPN3);

    /* Assign Keep alive duration */
    g_mqtt_tbl.keep_alive_seconds = WIFI_CFG_MQTT_KEEP_ALIVE;

    /* Assign identifier */
    g_mqtt_tbl.p_client_identifier = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_CLIENT_IDENTIFIER);
    if (0 == strcmp(g_mqtt_tbl.p_client_identifier, ""))
    {
        g_mqtt_tbl.client_identifier_length = 0;
    }
    else
    {
        g_mqtt_tbl.client_identifier_length = strlen(g_mqtt_tbl.p_client_identifier);
    }

    /* Assign host name */
    g_mqtt_tbl.p_host_name = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_HOST_NAME);

    /* Assign port */
    g_mqtt_tbl.mqtt_port = WIFI_CFG_MQTT_PORT;

    /* Assign user name */
    g_mqtt_tbl.p_mqtt_user_name = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_USER_NAME);
    if (0 == strcmp(g_mqtt_tbl.p_mqtt_user_name, ""))
    {
        g_mqtt_tbl.user_name_length = 0;
    }
    else
    {
        g_mqtt_tbl.user_name_length = strlen(g_mqtt_tbl.p_mqtt_user_name);
    }

    /* Assign password */
    g_mqtt_tbl.p_mqtt_password = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_PASSWORD);
    if (0 == strcmp(g_mqtt_tbl.p_mqtt_password, ""))
    {
        g_mqtt_tbl.password_length = 0;
    }
    else
    {
        g_mqtt_tbl.password_length = strlen(g_mqtt_tbl.p_mqtt_password);
    }

#ifdef CERT_INCLUDE
    g_mqtt_tbl.p_root_ca = WIFI_CFG_MQTT_ROOT_CA;
    g_mqtt_tbl.root_ca_size = strlen(g_mqtt_tbl.p_root_ca);
    g_mqtt_tbl.p_client_cert = WIFI_CFG_MQTT_CLIENT_CERT;
    g_mqtt_tbl.client_cert_size = strlen(g_mqtt_tbl.p_client_cert);
    g_mqtt_tbl.p_client_private_key = WIFI_CFG_MQTT_PRIVATE_KEY;
    g_mqtt_tbl.private_key_size = strlen(g_mqtt_tbl.p_client_private_key);
#else
    g_mqtt_tbl.p_root_ca = NULL;
    g_mqtt_tbl.root_ca_size = 0;
    g_mqtt_tbl.p_client_cert = NULL;
    g_mqtt_tbl.client_cert_size = 0;
    g_mqtt_tbl.p_client_private_key = NULL;
    g_mqtt_tbl.private_key_size = 0;
#endif

    g_mqtt_tbl.p_will_topic = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_WILL_TOPIC);
    g_mqtt_tbl.p_will_msg = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_WILL_MESSAGE);
    g_mqtt_tbl.p_sni_name = WIFI_STRING_CONVERT(WIFI_CFG_MQTT_SNI_NAME);
    g_mqtt_tbl.will_qos_level = (wifi_mqtt_qos_t) WIFI_CFG_MQTT_WILL_QOS;

    /* Assign cipher suites */
#if WIFI_CFG_MQTT_TLS_CIPHER_SUITES == 1
    g_mqtt_tbl.p_tls_cipher_suites[0] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA;
    g_mqtt_tbl.p_tls_cipher_suites[1] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA;
    g_mqtt_tbl.p_tls_cipher_suites[2] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256;
    g_mqtt_tbl.p_tls_cipher_suites[3] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384;
    g_mqtt_tbl.p_tls_cipher_suites[4] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256;
    g_mqtt_tbl.p_tls_cipher_suites[5] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384;
    g_mqtt_tbl.p_tls_cipher_suites[6] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA;
    g_mqtt_tbl.p_tls_cipher_suites[7] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA;
    g_mqtt_tbl.p_tls_cipher_suites[8] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256;
    g_mqtt_tbl.p_tls_cipher_suites[9] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384;
    g_mqtt_tbl.p_tls_cipher_suites[10] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256;
    g_mqtt_tbl.p_tls_cipher_suites[11] = (wifi_tls_cipher_suites_t) WIFI_CFG_MQTT_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384;
#else
    g_mqtt_tbl.p_tls_cipher_suites[0] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[1] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[2] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[3] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[4] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[5] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[6] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[7] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[8] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[9] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[10] = (wifi_tls_cipher_suites_t) 0;
    g_mqtt_tbl.p_tls_cipher_suites[11] = (wifi_tls_cipher_suites_t) 0;
#endif
    /* Assign number of cipher suites */
    g_mqtt_tbl.tls_cipher_count = get_cipher_suites_number();
}
/**********************************************************************************************************************
 * End of function r_wifi_da16xxx_mqtt_instance_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_wifi_da16xxx_mqtt_buffer_recv
 * Description  : Receive data from stream buffer.
 * Arguments    : data
 *                length
 *                timeout
 * Return Value : Number of received data.
 *********************************************************************************************************************/
static uint32_t r_wifi_da16xxx_mqtt_buffer_recv(uint8_t * data, uint32_t length, uint32_t timeout)
{
    uint32_t recv_cnt = 0;
    byteq_err_t byteq_ret;
    OS_TICK tick_tmp;

    if (0 < timeout)
    {
        g_mqtt_tbl.rx_timer.tick_sta = os_wrap_tickcount_get();
    }
    while (recv_cnt < length)
    {
        R_BSP_InterruptsDisable();
        byteq_ret = R_BYTEQ_Get(g_mqtt_tbl.byteq_hdl, (uint8_t *) (data + recv_cnt));
        R_BSP_InterruptsEnable();

        if (BYTEQ_SUCCESS == byteq_ret)
        {
            if ('\0' == *(data + recv_cnt))
            {
                break;
            }
            else
            {
                recv_cnt++;
                continue;
            }
        }

        /* timeout? */
        if (0 < timeout)
        {
            tick_tmp = os_wrap_tickcount_get() - g_mqtt_tbl.rx_timer.tick_sta;
            if (g_mqtt_tbl.rx_timer.threshold <= tick_tmp)
            {
                WIFI_LOG_WARN(("r_wifi_da16xxx_mqtt_buffer_recv: timeout!"));
                break;
            }
        }
        os_wrap_sleep(1, UNIT_TICK);
    }

    WIFI_LOG_INFO(("r_wifi_da16xxx_mqtt_buffer_recv: recv_cnt=%d.", recv_cnt));

    return recv_cnt;
}
/**********************************************************************************************************************
 * End of function r_wifi_da16xxx_mqtt_buffer_recv
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
    if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_MQTT_ALPN1), ""))
    {
        count++;
    }
    if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_MQTT_ALPN2), ""))
    {
        count++;
    }
    if (0 != strcmp(WIFI_STRING_CONVERT(WIFI_CFG_MQTT_ALPN3), ""))
    {
        count++;
    }

    return count;
}
/**********************************************************************************************************************
 * End of function get_alpn_number
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: get_cipher_suites_number
 * Description  : Get the number of Cipher Suites.
 * Arguments    : none
 * Return Value : number of Cipher Suites
 *********************************************************************************************************************/
static uint8_t get_cipher_suites_number(void)
{
    /* Count Cipher Suites */
    uint8_t count = 0;

    /* If TLS Cipher Suites are selected then increment */
    for (uint8_t i = 0; i < DA16XXX_MQTT_TLS_CIPHER_SUITE_MAX; i++)
    {
        if (0 != g_mqtt_tbl.p_tls_cipher_suites[i])
        {
            count++;
        }
    }

    return count;
}
/**********************************************************************************************************************
 * End of function get_cipher_suites_number
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_mqtt_data
 * Description  : Handles incoming common data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_mqtt_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data)
{
    /* Check if R_WIFI_DA16XXX_MqttConnect() is not called */
    if ( (g_mqtt_tbl.mqtt_status == DA16XXX_MQTT_OPEN) || (g_mqtt_tbl.mqtt_status == DA16XXX_MQTT_CLOSE) )
    {
        *type = WIFI_RESP_NONE;
        *state = WIFI_RECV_PREFIX;
        return;
    }

    switch (*state)
    {
        case WIFI_RECV_PARAM_CID:
        {
            if ('0' == data)
            {
                if (*type == WIFI_RESP_NWMQCL)
                {
                    g_mqtt_tbl.mqtt_status = DA16XXX_MQTT_DISCONNECTED;
                }
                else if (*type == WIFI_RESP_NWMQMSGSND)
                {
                    g_mqtt_tbl.mqtt_published_status = false;
                }
            }
            else if ('1' == data)
            {
                if (*type == WIFI_RESP_NWMQCL)
                {
                    g_mqtt_tbl.mqtt_status = DA16XXX_MQTT_CONNECTED;
                }
                else if (*type == WIFI_RESP_NWMQMSGSND)
                {
                    g_mqtt_tbl.mqtt_published_status = true;
                }
            }
            *type = WIFI_RESP_NONE;
            *state = WIFI_RECV_PREFIX;
            break;
        }

        case WIFI_RECV_DATA:
        {
            if (BYTEQ_SUCCESS != R_BYTEQ_Put(g_mqtt_tbl.byteq_hdl, data))
            {
                WIFI_LOG_ERROR(("MQTT buffer: receiving queue overflow!!!"));
            }
            else
            {
                if (data == '\r')
                {
                    g_mqtt_msg_ended = true;
                }
                else if (data == '\n' && g_mqtt_msg_ended == true)
                {
                    g_mqtt_msg_ended = false;
                    *type = WIFI_RESP_NONE;
                    *state = WIFI_RECV_PREFIX;
                    if (BYTEQ_SUCCESS != R_BYTEQ_Put(g_mqtt_tbl.byteq_hdl, '\0'))
                    {
                        WIFI_LOG_ERROR(("MQTT buffer: receiving queue overflow!!!"));
                    }
                }
                else
                {
                    g_mqtt_msg_ended = false;
                }
            }
            break;
        }

        default:
        {
            *type = WIFI_RESP_NONE;
            *state = WIFI_RECV_PREFIX;
            break;
        }
    }
}
/**********************************************************************************************************************
 * End of function da16xxx_handle_incoming_mqtt_data
 *********************************************************************************************************************/

/*
 * BYTEQ control for MQTT
 */
/**********************************************************************************************************************
 * Function Name: mqtt_byteq_open
 * Description  : open BYTEQ in MQTT table.
 * Arguments    : max_sock - maximum of socket tables
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_BYTEQ_OPEN
 *********************************************************************************************************************/
static wifi_err_t mqtt_byteq_open(void)
{
    if (BYTEQ_SUCCESS != R_BYTEQ_Open(g_mqtt_tbl.cmd_rx_buff, WIFI_CFG_MQTT_CMD_RX_BUF_SIZE, &g_mqtt_tbl.byteq_hdl))
    {
        WIFI_LOG_ERROR(("mqtt_byteq_open: Cannot open BYTEQ for MQTT!!!"));
        return WIFI_ERR_BYTEQ_OPEN;
    }
    return WIFI_SUCCESS;
}
/**********************************************************************************************************************
 * End of function mqtt_byteq_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: mqtt_byteq_close
 * Description  : close BYTEQ on MQTT table.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
static void mqtt_byteq_close(void)
{
    if (0 != g_mqtt_tbl.byteq_hdl)
    {
        R_BYTEQ_Close(g_mqtt_tbl.byteq_hdl);
    }
}
/**********************************************************************************************************************
 * End of function mqtt_byteq_close
 *********************************************************************************************************************/
#endif /* WIFI_CFG_MQTT_SUPPORT */
