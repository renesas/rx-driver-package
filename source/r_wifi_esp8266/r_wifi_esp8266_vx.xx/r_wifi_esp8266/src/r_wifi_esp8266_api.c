#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "platform.h"

#include "r_sci_rx_if.h"
#include "r_byteq_if.h"
#include "r_wifi_esp8266_if.h"
#include "r_wifi_esp8266_private.h"
#include "r_sci_rx_if.h"

#if !defined(WIFI_CFG_SCI_CHANNEL)
#error "Error! Need to define WIFI_CFG_SCI_CHANNEL in r_wifi_esp8266_config.h"
#elif WIFI_CFG_SCI_CHANNEL == (0)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI0()
#define SCI_CH_wifi_serial_default           SCI_CH0
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH0_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH0_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (1)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI1()
#define SCI_CH_wifi_serial_default           SCI_CH1
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH1_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH1_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (2)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI2()
#define SCI_CH_wifi_serial_default           SCI_CH2
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH2_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH2_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (3)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI3()
#define SCI_CH_wifi_serial_default           SCI_CH3
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH3_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH3_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (4)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI4()
#define SCI_CH_wifi_serial_default           SCI_CH4
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH4_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH4_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (5)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI5()
#define SCI_CH_wifi_serial_default           SCI_CH5
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH5_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH5_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (6)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI6()
#define SCI_CH_wifi_serial_default           SCI_CH6
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH6_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH6_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (7)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI7()
#define SCI_CH_wifi_serial_default           SCI_CH7
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH7_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH7_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (8)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI8()
#define SCI_CH_wifi_serial_default           SCI_CH8
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH8_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH8_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (9)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI9()
#define SCI_CH_wifi_serial_default           SCI_CH9
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH9_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH9_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (10)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI10()
#define SCI_CH_wifi_serial_default           SCI_CH10
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH10_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH10_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (11)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI11()
#define SCI_CH_wifi_serial_default           SCI_CH11
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH11_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH11_RX_BUFSIZ
#elif WIFI_CFG_SCI_CHANNEL == (12)
#define R_SCI_PinSet_wifi_serial_default()   R_SCI_PinSet_SCI12()
#define SCI_CH_wifi_serial_default           SCI_CH12
#define SCI_TX_BUSIZ_DEFAULT                 SCI_CFG_CH12_TX_BUFSIZ
#define SCI_RX_BUSIZ_DEFAULT                 SCI_CFG_CH12_RX_BUFSIZ
#else
#error "Error! Invalid setting for WIFI_CFG_SCI_CHANNEL in r_wifi_esp8266_config.h"
#endif

#define MUTEX_TX (1 << 0)
#define MUTEX_RX (1 << 1)

const uint8_t wifi_return_text_ok[]          = WIFI_RETURN_TEXT_OK;
const uint8_t wifi_return_text_error[]       = WIFI_RETURN_TEXT_ERROR;
const uint8_t wifi_return_text_ready[]       = WIFI_RETURN_TEXT_READY;
const uint8_t wifi_return_text_ok_go_send[]  = WIFI_RETURN_TEXT_OK_GO_SEND;
const uint8_t wifi_return_text_send_byte[]   = WIFI_RETURN_TEXT_SEND_BYTE;
const uint8_t wifi_return_text_send_ok[]     = WIFI_RETURN_TEXT_SEND_OK;
const uint8_t wifi_return_text_send_fail[]   = WIFI_RETURN_TEXT_SEND_FAIL;
const uint8_t wifi_return_text_busy[]        = WIFI_RETURN_TEXT_BUSY;
const uint8_t wifi_return_dummy[]   = "";

const uint32_t g_wifi_serial_buffsize_table[WIFI_NUMBER_OF_USE_UART][2] =
{
    /*SCI_TX_BUSIZ_DEFAULT*/ /*SCI_RX_BUSIZ_DEFAULT*/
    {SCI_TX_BUSIZ_DEFAULT,    SCI_RX_BUSIZ_DEFAULT,  }
};

const uint8_t * const wifi_result_code[WIFI_RETURN_ENUM_MAX][WIFI_RETURN_STRING_MAX] =
{
	/* text mode*/
	{wifi_return_text_ok},
	{wifi_return_text_error},
	{wifi_return_text_ready},
	{wifi_return_text_ok_go_send},
	{wifi_return_text_send_byte},
	{wifi_return_text_send_ok},
	{wifi_return_text_send_fail},
//	{wifi_return_text_busy},
	{wifi_return_dummy},
	{wifi_return_dummy},
};

wifi_system_status_t g_wifi_system_state = WIFI_SYSTEM_CLOSE;
uint8_t g_wifi_at_command_buff[WIFI_AT_COMMAND_BUFF_SIZE];
uint8_t g_wifi_at_response_buff[WIFI_AT_RESPONSE_BUFF_SIZE];

wifi_at_communication_info_t g_wifi_uart[WIFI_NUMBER_OF_USE_UART];

uint32_t g_wifi_tx_busiz_command;
uint32_t g_wifi_tx_busiz_data;
uint32_t g_wifi_rx_busiz_command;
uint32_t g_wifi_rx_busiz_data;

static void wifi_uart_callback_command_port(void *pArgs);
static void timeout_init(int32_t socket_no, uint16_t timeout_ms);
static void bytetimeout_init(int32_t socket_no, uint16_t timeout_ms);
static int32_t check_timeout(int32_t socket_no, int32_t rcvcount);
static int32_t check_bytetimeout(int32_t socket_no, int32_t rcvcount);
static int32_t wifi_serial_open(uint32_t);
static int32_t wifi_serial_close(void);
static int32_t wifi_execute_at_command(uint8_t serial_ch_id, uint8_t *ptextstring,
		uint16_t timeout_ms, wifi_return_code_t expect_code,  wifi_command_list_t command, int32_t socket_no);

static void socket_timeout_init(uint8_t socket_no, uint32_t timeout_ms, uint8_t flag);
static int32_t socket_check_timeout(uint8_t socket_no, uint8_t flag);
static int32_t wifi_take_mutex(uint8_t mutex_flag);
static void wifi_give_mutex(uint8_t mutex_flag);

static TickType_t g_wifi_tcp_recv_timeout = 3000;		/* ## slowly problem ## unit: 1ms */

static SemaphoreHandle_t g_wifi_tx_semaphore = NULL;
static SemaphoreHandle_t g_wifi_rx_semaphore = NULL;

static const TickType_t xMaxSemaphoreBlockTime = pdMS_TO_TICKS( 60000UL );

uint32_t g_wifi_sci_err_flag;

wifi_err_t R_WIFI_ESP8266_Open(void)
{
	int32_t ret;
	uint8_t *pcbuff;
	wifi_err_t api_ret = WIFI_SUCCESS;
	uint8_t open_phase;
	uint8_t byteq_open_count;
	int32_t k;

	if( WIFI_SYSTEM_CLOSE != g_wifi_system_state)
	{
		return WIFI_ERR_ALREADY_OPEN;
	}

	/* Memory initialize */

    memset(g_wifi_uart, 0, sizeof(g_wifi_uart));
    g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff = g_wifi_at_command_buff;
    g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff_size = sizeof(g_wifi_at_command_buff);
    g_wifi_uart[WIFI_UART_COMMAND_PORT].response_buff = g_wifi_at_response_buff;
    g_wifi_uart[WIFI_UART_COMMAND_PORT].response_buff_size = sizeof(g_wifi_at_response_buff);

    pcbuff = g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff;

	g_wifi_tx_busiz_command = g_wifi_serial_buffsize_table[0][0];
	g_wifi_tx_busiz_data    = g_wifi_serial_buffsize_table[0][0];
	g_wifi_rx_busiz_command = g_wifi_serial_buffsize_table[0][1];
	g_wifi_rx_busiz_data    = g_wifi_serial_buffsize_table[0][1];

	memset(g_wifi_socket, 0, sizeof(g_wifi_socket));

	wifi_init_at_execute_queue();

	open_phase = 0;
	/* Phase 1: ByteQ initialize */
	for(byteq_open_count = 0;byteq_open_count <WIFI_CFG_CREATABLE_SOCKETS; byteq_open_count++)
	{
		if(BYTEQ_SUCCESS != R_BYTEQ_Open(g_wifi_socket[byteq_open_count].socket_recv_buff, sizeof(g_wifi_socket[byteq_open_count].socket_recv_buff), &g_wifi_socket[byteq_open_count].socket_byteq_hdl))
		{
			break;
		}
	}

	if(byteq_open_count >= WIFI_CFG_CREATABLE_SOCKETS)
	{
		/* Success */
		open_phase |= 0x01;
	}
	else
	{
		api_ret = WIFI_ERR_BYTEQ_OPEN;
	}

	if(WIFI_SUCCESS == api_ret)
	{
	    /* Phase 3 Semaphore tx initialize */
	    if (g_wifi_tx_semaphore != NULL)
	    {
	        vSemaphoreDelete(g_wifi_tx_semaphore);
	    }
	    g_wifi_tx_semaphore = xSemaphoreCreateMutex();

	    if( g_wifi_tx_semaphore != NULL )
	    {
			/* Success */
			open_phase |= 0x02;
	    }
	    else
	    {
	    	api_ret = WIFI_ERR_TAKE_MUTEX;
	    }
	}

	if(WIFI_SUCCESS == api_ret)
	{
	    /* Phase 4 Semaphore rx initialize */
	    if (g_wifi_rx_semaphore != NULL)
	    {
	        vSemaphoreDelete(g_wifi_rx_semaphore);
	    }

	    g_wifi_rx_semaphore = xSemaphoreCreateMutex();

	    if( g_wifi_rx_semaphore != NULL )
	    {
			/* Success */
			open_phase |= 0x04;
	    }
	    else
	    {
	    	api_ret = WIFI_ERR_TAKE_MUTEX;
	    }
	}

	if(WIFI_SUCCESS == api_ret)
	{
	    /* Phase 4 Serial initialize */
	    WIFI_RESET_DDR(WIFI_CFG_RESET_PORT, WIFI_CFG_RESET_PIN) = 1;
		WIFI_RESET_DR(WIFI_CFG_RESET_PORT, WIFI_CFG_RESET_PIN) = 0;
		vTaskDelay(2);
		WIFI_RESET_DR(WIFI_CFG_RESET_PORT, WIFI_CFG_RESET_PIN) = 1;
		vTaskDelay(2000);

		ret = wifi_serial_open(WIFI_UART_BAUDRATE_DEFAULT);
		if(0 == ret)
		{
			/* Success */
			open_phase |= 0x08;
		}
		else
		{
	    	api_ret = WIFI_ERR_SERIAL_OPEN;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
	    /* Phase 5 Task initialize */
		if(0 == wifi_start_recv_task())
		{
			vTaskDelay( 2 );
			/* Success */
			open_phase |= 0x10;
		}
		else
		{
			api_ret = WIFI_ERR_SERIAL_OPEN;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		/* no echo */
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "ATE0\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_ECHO_OFF, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		/* Show Version information  */
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+GMR\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_GET_MODULE_VERSION, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
#if (WIFI_CFG_SCI_USE_FLOW_CONTROL == 1)
		sprintf((char *)pcbuff, "AT+UART_CUR=%d,8,1,0,1\r\n",WIFI_CFG_SCI_BAUDRATE);
#else
		sprintf((char *)pcbuff, "AT+UART_CUR=%d,8,1,0,0\r\n",WIFI_CFG_SCI_BAUDRATE);
#endif
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, pcbuff, 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_UART_HISPEED, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		wifi_serial_close();
		wifi_serial_open(WIFI_CFG_SCI_BAUDRATE);
#if (WIFI_CFG_SCI_USE_FLOW_CONTROL == 1)
		R_SCI_Control(g_wifi_uart[WIFI_UART_COMMAND_PORT].wifi_uart_sci_handle, SCI_CMD_EN_CTS_IN, NULL);
		R_BSP_SoftwareDelay(1000, BSP_DELAY_MILLISECS); /* Wait Module baudrate change */
#endif

		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPDNS_CUR=0\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DNS_SRV_ADDRESS, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		/* Station mode is enabled. */
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CWMODE=1\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_WIFI_ACT_MODE, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPSNTPCFG=1,-9,\"ntp1.jst.mfeed.ad.jp\"\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DNS_SRV_ADDRESS, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPMUX=1\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_MULTIPLE_SOCKET, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}

	if(api_ret == WIFI_SUCCESS)
	{
		g_wifi_system_state = WIFI_SYSTEM_OPEN;
	}
	else
	{
		if(0 != (open_phase & 0x10))
		{
			wifi_delete_recv_task();
		}
		if(0 != (open_phase & 0x08))
		{
			wifi_serial_close();
		}
		if(0 != (open_phase & 0x04))
		{
			vSemaphoreDelete(g_wifi_rx_semaphore);
			g_wifi_rx_semaphore = NULL;
		}
		if(0 != (open_phase & 0x02))
		{
			vSemaphoreDelete(g_wifi_tx_semaphore);
			g_wifi_tx_semaphore = NULL;
		}
		if(0 != (open_phase & 0x01))
		{
			for(k = 0; k < byteq_open_count; k++)
			{
				R_BYTEQ_Close(g_wifi_socket[k].socket_byteq_hdl);
			}
		}
	}
	return api_ret;
}

wifi_err_t R_WIFI_ESP8266_Close(void)
{
	int i;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if(0 == R_WIFI_ESP8266_IsConnected())
	{
		R_WIFI_ESP8266_Disconnect();
	}
	wifi_serial_close();
	wifi_delete_recv_task();
	for(i=0; i<WIFI_CFG_CREATABLE_SOCKETS; i++)
	{
		R_BYTEQ_Close(g_wifi_socket[i].socket_byteq_hdl);
	}

	g_wifi_system_state = WIFI_SYSTEM_CLOSE;

	return api_ret;
}

wifi_err_t R_WIFI_ESP8266_SetDnsServerAddress(uint32_t dnsaddress1, uint32_t dnsaddress2)
{
	int32_t ret;
	wifi_err_t api_ret = WIFI_SUCCESS;
    uint8_t mutex_flag;
	uint8_t *pcbuff = g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff;

	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{
		if(dnsaddress1 == 0)
		{
			strcpy((char *)pcbuff,"AT+CIPDNS_CUR=1\r\n");
		}
		else if((dnsaddress2 != 0) && (dnsaddress2 != dnsaddress1))
		{
			sprintf((char *)pcbuff,"AT+CIPDNS_CUR=1,\"%d.%d.%d.%d\",\"%d.%d.%d.%d\"\r\n",
					WIFI_ULONG_TO_IPV4BYTE_1(dnsaddress1), WIFI_ULONG_TO_IPV4BYTE_2(dnsaddress1),
					WIFI_ULONG_TO_IPV4BYTE_3(dnsaddress1), WIFI_ULONG_TO_IPV4BYTE_4(dnsaddress1),
					WIFI_ULONG_TO_IPV4BYTE_1(dnsaddress2), WIFI_ULONG_TO_IPV4BYTE_2(dnsaddress2),
					WIFI_ULONG_TO_IPV4BYTE_4(dnsaddress2), WIFI_ULONG_TO_IPV4BYTE_4(dnsaddress2));
		}
		else
		{
			sprintf((char *)pcbuff,"AT+CIPDNS_CUR=1,\"%d.%d.%d.%d\"\r\n",
					WIFI_ULONG_TO_IPV4BYTE_1(dnsaddress1), WIFI_ULONG_TO_IPV4BYTE_2(dnsaddress1),
					WIFI_ULONG_TO_IPV4BYTE_3(dnsaddress1), WIFI_ULONG_TO_IPV4BYTE_4(dnsaddress1));
		}
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, pcbuff, 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DNS_SRV_ADDRESS, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}
	return api_ret;

}


wifi_err_t R_WIFI_ESP8266_Connect (uint8_t *pssid, uint8_t *ppass, uint32_t security, uint8_t dhcp_enable, wifi_ip_configuration_t *pipconfig)
{
	int32_t i;
	int32_t ret;
	wifi_err_t api_ret = WIFI_SUCCESS;
    uint8_t mutex_flag;
	uint8_t *pcbuff = g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff;

	if((pssid == NULL) || (ppass == NULL) || ((security != WIFI_SECURITY_WPA) && (security != WIFI_SECURITY_WPA2)) || (pipconfig == NULL))
	{
		return WIFI_ERR_PARAMETER;
	}
	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}
	if( 0 == R_WIFI_ESP8266_IsConnected())
	{
		/* Nothing to do. */
		return WIFI_SUCCESS;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{
		if(0 == dhcp_enable)
		{
			/* DHCP Not Use */
			ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CWDHCP_CUR=1,0\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DHCP_MODE, 0xff);
			if(ret != 0)
			{
				api_ret = WIFI_ERR_MODULE_COM;
			}
			if(WIFI_SUCCESS == api_ret)
			{
				sprintf((char *)pcbuff,"AT+CIPSTA=\"%d.%d.%d.%d\",\"%d.%d.%d.%d\",\"%d.%d.%d.%d\"\r\n",
						WIFI_ULONG_TO_IPV4BYTE_1(pipconfig->ipaddress), WIFI_ULONG_TO_IPV4BYTE_2(pipconfig->ipaddress),
						WIFI_ULONG_TO_IPV4BYTE_3(pipconfig->ipaddress), WIFI_ULONG_TO_IPV4BYTE_4(pipconfig->ipaddress),
						WIFI_ULONG_TO_IPV4BYTE_1(pipconfig->gateway), WIFI_ULONG_TO_IPV4BYTE_2(pipconfig->gateway),
						WIFI_ULONG_TO_IPV4BYTE_3(pipconfig->gateway), WIFI_ULONG_TO_IPV4BYTE_4(pipconfig->gateway),
						WIFI_ULONG_TO_IPV4BYTE_1(pipconfig->subnetmask), WIFI_ULONG_TO_IPV4BYTE_2(pipconfig->subnetmask),
						WIFI_ULONG_TO_IPV4BYTE_3(pipconfig->subnetmask), WIFI_ULONG_TO_IPV4BYTE_4(pipconfig->subnetmask));

				/* Set Static IP address */
				ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, pcbuff, 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_STATIC_IP, 0xff);
				if(ret != 0)
				{
					api_ret = WIFI_ERR_MODULE_COM;
				}
			}
		}
		else
		{
			/* DHCP Use */
			ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CWDHCP_CUR=1,1\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DHCP_MODE, 0xff);
			if(ret != 0)
			{
				api_ret = WIFI_ERR_MODULE_COM;
			}
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		sprintf(pcbuff,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",pssid,ppass);
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, pcbuff, 30000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_WIFI_CONNECT, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		if(0 == dhcp_enable)
		{
			g_wifi_ipconfig.ipaddress = pipconfig->ipaddress;
			g_wifi_ipconfig.subnetmask = pipconfig->subnetmask;
			g_wifi_ipconfig.gateway = pipconfig->gateway;
		}
		else
		{
			ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPSTA_CUR?\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_GET_IPADDRESS, 0xff);
			if(ret != 0)
			{
				api_ret = WIFI_ERR_MODULE_COM;
			}
			else
			{
				pipconfig->ipaddress = g_wifi_ipconfig.ipaddress;
				pipconfig->subnetmask = g_wifi_ipconfig.subnetmask;
				pipconfig->gateway = g_wifi_ipconfig.gateway;
			}
		}
		g_wifi_system_state = WIFI_SYSTEM_CONNECT;
	}

	if(WIFI_SUCCESS == api_ret)
	{
		R_BSP_SoftwareDelay(2000,BSP_DELAY_MILLISECS);
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPSNTPTIME?\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DNS_SRV_ADDRESS, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
	}

	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}
	return api_ret;
}


wifi_err_t R_WIFI_ESP8266_Disconnect (void)
{
	int32_t i;
	int32_t ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}
	if( WIFI_SYSTEM_OPEN == g_wifi_system_state)
	{
		/* Nothing to do. */
		return WIFI_SUCCESS;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{
		wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CWQAP\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_WIFI_DISCONNECT, 0xff);
		memset(&g_wifi_ipconfig, 0, sizeof(g_wifi_ipconfig));
		g_wifi_system_state = WIFI_SYSTEM_OPEN;
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}
	return api_ret;
}

int32_t R_WIFI_ESP8266_IsConnected (void)
{
	int32_t ret = -1;

	if(WIFI_SYSTEM_CONNECT == g_wifi_system_state)
	{
		ret = 0;
	}
	return ret;
}

wifi_err_t R_WIFI_ESP8266_GetMacAddress (uint8_t *pmacaddress)
{
	int32_t ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if(NULL == pmacaddress)
	{
		return WIFI_ERR_PARAMETER;
	}
	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}
    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}

    if(WIFI_SUCCESS == api_ret)
	{
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPSTAMAC_CUR?\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_GET_MACADDRESS, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
		memcpy(pmacaddress, g_wifi_macaddress, sizeof(g_wifi_macaddress));
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}
	return api_ret;
}

wifi_err_t R_WIFI_ESP8266_GetIpAddress (wifi_ip_configuration_t *pipconfig)
{
	int32_t ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if(NULL == pipconfig)
	{
		return WIFI_ERR_PARAMETER;
	}
	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}

    if(WIFI_SUCCESS == api_ret)
	{
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CIPSTA_CUR?\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_GET_IPADDRESS, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
		memcpy(pipconfig, &g_wifi_ipconfig, sizeof(wifi_ip_configuration_t));
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}
	return api_ret;
}

wifi_err_t R_WIFI_ESP8266_Scan (wifi_scan_result_t *pap_results, uint32_t maxnetworks, uint32_t *exist_ap_list)
{
    int32_t ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

    if ((NULL == pap_results) || (0 == maxnetworks) || (NULL == exist_ap_list))
    {
    	return WIFI_ERR_PARAMETER;
    }
	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{
	    gp_wifi_ap_results = pap_results;
	    g_wifi_aplistmax = maxnetworks;
		g_wifi_aplist_stored_num = 0;
		g_wifi_aplist_count = 0;
		ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, "AT+CWLAP\r\n", 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_GET_APLIST, 0xff);
		if(ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
		else
		{
			*exist_ap_list = g_wifi_aplist_count;
		}
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}

	return api_ret;
}

int32_t R_WIFI_ESP8266_SocketCreate(uint32_t type, uint32_t ipversion)
{
	int32_t i;
	int32_t ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if(type != WIFI_SOCKET_IP_PROTOCOL_TCP || ipversion != WIFI_SOCKET_IP_VERSION_4)
	{
		return WIFI_ERR_PARAMETER;
	}
	if( 0 != R_WIFI_ESP8266_IsConnected())
	{
		return WIFI_ERR_NOT_CONNECT;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
    	api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{

		for(i=0;i<WIFI_CFG_CREATABLE_SOCKETS;i++)
		{
			if(g_wifi_socket[i].socket_create_flag == 0)
			{
				g_wifi_socket[i].socket_create_flag = 1;
				g_wifi_socket[i].ipversion = ipversion;
				g_wifi_socket[i].protocol = type;
				g_wifi_socket[i].socket_status = WIFI_SOCKET_STATUS_SOCKET;
				g_wifi_socket[i].ssl_flag = 0;
				g_wifi_socket[i].ssl_type = 0;
				R_BYTEQ_Flush(g_wifi_socket[i].socket_byteq_hdl);
				break;
			}
		}

        /* Give back the socketInUse mutex. */
        wifi_give_mutex(mutex_flag);
        if(i < WIFI_CFG_CREATABLE_SOCKETS)
        {
        	ret = i;
        }
        else
        {
        	ret = WIFI_ERR_SOCKET_CREATE;
        }
    }
    else
    {
    	ret = api_ret;
    }

	return ret;
}



wifi_err_t R_WIFI_ESP8266_SocketConnect (int32_t socket_no, uint32_t ipaddr, uint16_t port)
{
	int32_t ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if( (0 == ipaddr) || (0 == port) )
	{
		return  WIFI_ERR_PARAMETER;
	}
	if( (socket_no >= WIFI_CFG_CREATABLE_SOCKETS) || (socket_no < 0) || (g_wifi_socket[socket_no].socket_status != WIFI_SOCKET_STATUS_SOCKET) )
	{
		return  WIFI_ERR_SOCKET_NUM;
	}
	if( 0 != R_WIFI_ESP8266_IsConnected())
	{
		return WIFI_ERR_NOT_CONNECT;
	}

	mutex_flag = (MUTEX_TX | MUTEX_RX);
	if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{

			sprintf((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff,"AT+CIPSTART=%d,\"TCP\",\"%d.%d.%d.%d\",%d\r\n",
					socket_no,
					WIFI_ULONG_TO_IPV4BYTE_1(ipaddr), WIFI_ULONG_TO_IPV4BYTE_2(ipaddr),
					WIFI_ULONG_TO_IPV4BYTE_3(ipaddr), WIFI_ULONG_TO_IPV4BYTE_4(ipaddr),
					port);

			ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff, 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_SOCKET_CONNECT, socket_no);
			if(ret != 0)
			{
				api_ret = WIFI_ERR_MODULE_COM;
			}
	}
	if(WIFI_SUCCESS == api_ret)
	{
		g_wifi_socket[socket_no].socket_status = WIFI_SOCKET_STATUS_CONNECTED;
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}
	return api_ret;
}


int32_t R_WIFI_ESP8266_SocketSend (int32_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout_ms)
{
	volatile int32_t timeout;
	volatile int32_t sended_length;
	int32_t current_send_length;
	int32_t api_ret;
	int32_t ret;
	sci_err_t ercd;
	int8_t get_queue;
	wifi_return_code_t result;
	uint8_t mutex_flag;
	uint32_t ticket_no;

	if( (pdata == NULL) || (length < 0 ))
	{
		return  WIFI_ERR_PARAMETER;
	}

	if( 0 != R_WIFI_ESP8266_IsConnected())
	{
		return WIFI_ERR_NOT_CONNECT;
	}

	if( (socket_no >= WIFI_CFG_CREATABLE_SOCKETS) || (socket_no < 0) || (0 == g_wifi_socket[socket_no].socket_create_flag) || (g_wifi_socket[socket_no].socket_status != WIFI_SOCKET_STATUS_CONNECTED) )
	{
		return  WIFI_ERR_SOCKET_NUM;
	}

	mutex_flag = MUTEX_TX;
    if(0 == wifi_take_mutex(mutex_flag))
	{
		sended_length = 0;

		if(0 != timeout_ms)
		{
			socket_timeout_init(socket_no, timeout_ms, 0);
		}
		while(sended_length < length)
		{

			if(length - sended_length > g_wifi_tx_busiz_data)
			{
				current_send_length = g_wifi_tx_busiz_data;
			}
			else
			{
				current_send_length = length - sended_length;
			}

			if(current_send_length > WIFI_SOCKET_SENDABLE_DATASIZE)
			{
				current_send_length = WIFI_SOCKET_SENDABLE_DATASIZE;
			}

			sprintf((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff, "AT+CIPSEND=%d,%d\r\n" ,socket_no, current_send_length);

			ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff, 10000, WIFI_RETURN_ENUM_OK_GO_SEND, WIFI_COMMAND_SET_SOCKET_SEND_START, socket_no);

			if(ret != 0)
			{
				break;
			}

			timeout = 0;

			timeout_init(WIFI_UART_COMMAND_PORT, 5000);
			ticket_no = wifi_set_request_in_queue( WIFI_COMMAND_SET_SOCKET_SEND_DATA, socket_no );
			g_wifi_uart[WIFI_UART_COMMAND_PORT].tx_end_flag = 0;
			ercd = R_SCI_Send(g_wifi_uart[WIFI_UART_COMMAND_PORT].wifi_uart_sci_handle, pdata+sended_length, current_send_length);
			if(SCI_SUCCESS != ercd)
			{
				break;
			}

			while(1)
			{
				if(0 != g_wifi_uart[WIFI_UART_COMMAND_PORT].tx_end_flag)
				{
					break;
				}
				vTaskDelay(1);
			}

			while(1)
			{
				get_queue = wifi_get_result_from_queue( ticket_no, &result);
				if(0 == get_queue)
				{
					break;
				}
				if(0 != timeout_ms)
				{
					if(-1 == check_timeout(WIFI_UART_COMMAND_PORT, 0))
					{
						wifi_set_result_to_current_running_queue( WIFI_RETURN_ENUM_INTERNAL_TIMEOUT );
						timeout = 1;
						break;
					}
				}
				vTaskDelay(1);
			}

			if(1 == timeout)
			{
				break;
			}
			if(result != WIFI_RETURN_ENUM_SEND_OK)
			{
				break;
			}

			sended_length += current_send_length;
			if(-1 == socket_check_timeout(socket_no, 0))
			{
				break;
			}
		}
		api_ret = sended_length;

		/* Give back the socketInUse mutex. */
		wifi_give_mutex(mutex_flag);
    }
    else
    {
    	api_ret = WIFI_ERR_TAKE_MUTEX;
    }
	return api_ret;
}

int32_t R_WIFI_ESP8266_SocketRecv (int32_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout_ms)
{
	uint32_t recvcnt;
	uint32_t recv_length;
	uint32_t i;
	int32_t api_ret;
	volatile int32_t timeout;
	int32_t ret;
	byteq_err_t byteq_ret;
	uint8_t mutex_flag;

	if( (pdata == NULL) || (length <= 0 ))
	{
		return  WIFI_ERR_PARAMETER;
	}

	if( 0 != R_WIFI_ESP8266_IsConnected())
	{
		return WIFI_ERR_NOT_CONNECT;
	}

	if( (socket_no >= WIFI_CFG_CREATABLE_SOCKETS) || (socket_no < 0) || (0 == g_wifi_socket[socket_no].socket_create_flag) ||  (g_wifi_socket[socket_no].socket_status != WIFI_SOCKET_STATUS_CONNECTED) )
	{
		return  WIFI_ERR_SOCKET_NUM;
	}

	mutex_flag = MUTEX_RX;
    if(0 == wifi_take_mutex(mutex_flag))
    {
		recvcnt = 0;

		if((timeout_ms != 0) && (timeout_ms != portMAX_DELAY))
		{
			socket_timeout_init(socket_no, timeout_ms, 1);
		}

		while(1)
		{
			byteq_ret = R_BYTEQ_Get(g_wifi_socket[socket_no].socket_byteq_hdl, (pdata + recvcnt));
			if(BYTEQ_SUCCESS == byteq_ret)
			{
				recvcnt++;
				if(recvcnt >= length)
				{
					break;
				}
				continue;
			}
			if(g_wifi_socket[socket_no].socket_status != WIFI_SOCKET_STATUS_CONNECTED)
			{
				break;
			}
			if(g_wifi_system_state != WIFI_SYSTEM_CONNECT)
			{
				break;
			}
			if((timeout_ms != 0) && (timeout_ms != portMAX_DELAY))
			{
				if(-1 == socket_check_timeout(socket_no, 1))
				{
					R_BSP_NOP();
					break;
				}
			}
			vTaskDelay(1);
		}
		api_ret = recvcnt;
		/* Give back the socketInUse mutex. */
    	wifi_give_mutex(mutex_flag);
    }
    else
    {
    	api_ret = WIFI_ERR_TAKE_MUTEX;
    }
	return api_ret;
}


wifi_err_t R_WIFI_ESP8266_SocketShutdown (int32_t socket_no)
{
	wifi_err_t api_ret = WIFI_SUCCESS;
	int32_t subroutain_ret;
	uint8_t mutex_flag;

	if((socket_no >= WIFI_CFG_CREATABLE_SOCKETS) || (socket_no < 0) ||
			(0 == g_wifi_socket[socket_no].socket_create_flag) || (g_wifi_socket[socket_no].socket_status <= WIFI_SOCKET_STATUS_SOCKET))
	{
		return WIFI_ERR_SOCKET_NUM;
	}
	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}
	mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 == wifi_take_mutex(mutex_flag))
	{
		sprintf((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff,"AT+CIPCLOSE=%d\r\n",socket_no);
		subroutain_ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff, 10000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_SOCKET_CLOSE, socket_no);
		if(subroutain_ret != 0)
		{
	    	api_ret = WIFI_ERR_MODULE_COM;
		}
		else
		{
			g_wifi_socket[socket_no].socket_status = WIFI_SOCKET_STATUS_SOCKET;
		}
		/* Give back the mutex. */
		wifi_give_mutex(mutex_flag);
    }
    else
    {
    	api_ret = WIFI_ERR_TAKE_MUTEX;
    }
	return api_ret;

}

wifi_err_t R_WIFI_ESP8266_SocketClose (int32_t socket_no)
{
	wifi_err_t api_ret = WIFI_SUCCESS;

	if((socket_no >= WIFI_CFG_CREATABLE_SOCKETS) || (socket_no < 0))
	{
		return WIFI_ERR_SOCKET_NUM;
	}
	if( WIFI_SYSTEM_CLOSE == g_wifi_system_state)
	{
		return WIFI_ERR_NOT_OPEN;
	}

	if(g_wifi_socket[socket_no].socket_create_flag == 1)
	{
		R_WIFI_ESP8266_SocketShutdown (socket_no);
		R_BYTEQ_Flush(g_wifi_socket[socket_no].socket_byteq_hdl);
		g_wifi_socket[socket_no].ipversion = 0;
		g_wifi_socket[socket_no].protocol = 0;
		g_wifi_socket[socket_no].socket_create_flag = 0;
		g_wifi_socket[socket_no].ssl_flag = 0;
		g_wifi_socket[socket_no].ssl_type = 0;
		g_wifi_socket[socket_no].socket_status = WIFI_SOCKET_STATUS_CLOSED;
	}
	return api_ret;

}


wifi_err_t R_WIFI_ESP8266_DnsQuery (uint8_t *pdomain_name, uint32_t *pipaddress)
{
	int32_t func_ret;
	uint8_t mutex_flag;
	wifi_err_t api_ret = WIFI_SUCCESS;

    if ((NULL == pdomain_name) || (NULL == pipaddress))
    {
    	return WIFI_ERR_PARAMETER;
    }
	if( 0 != R_WIFI_ESP8266_IsConnected())
	{
		return WIFI_ERR_NOT_CONNECT;
	}

    mutex_flag = (MUTEX_TX | MUTEX_RX);
    if(0 != wifi_take_mutex(mutex_flag))
	{
		api_ret = WIFI_ERR_TAKE_MUTEX;
	}
	if(WIFI_SUCCESS == api_ret)
	{
		strcpy((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff,"AT+CIPDOMAIN=\"");
		sprintf((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff+strlen((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff),"%s\"\r\n",pdomain_name);

		func_ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff, 20000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_DNSQUERY, 0xff);
		if(func_ret != 0)
		{
			api_ret = WIFI_ERR_MODULE_COM;
		}
		else
		{
			*pipaddress = g_wifi_dnsaddress;
		}
	}
	if(WIFI_ERR_TAKE_MUTEX != api_ret)
	{
		wifi_give_mutex(mutex_flag);
	}

	return api_ret;
}


wifi_err_t R_WIFI_ESP8266_Ping (uint32_t ipaddr, uint16_t count, uint32_t intervalms)
{
	int32_t func_ret;
	uint8_t mutex_flag;
	uint32_t i;
	wifi_err_t api_ret = WIFI_SUCCESS;

	if ((0 == ipaddr) || (0 == count))
	{
		return WIFI_ERR_PARAMETER;
	}
	if( 0 != R_WIFI_ESP8266_IsConnected())
	{
		return WIFI_ERR_NOT_CONNECT;
	}

	for(i=0;i<count;i++)
	{
	    mutex_flag = (MUTEX_TX | MUTEX_RX);
	    if(0 != wifi_take_mutex(mutex_flag))
		{
			api_ret = WIFI_ERR_TAKE_MUTEX;
		}
		if(WIFI_SUCCESS == api_ret)
		{
			strcpy((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff,"AT+PING=\"");
			sprintf((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff+strlen((char *)g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff),"%d.%d.%d.%d\"\r\n",
					(uint8_t)(ipaddr >> 24),(uint8_t)((ipaddr >> 16) & 0xFF), (uint8_t)((ipaddr >> 8) & 0xFF), (uint8_t)((ipaddr) & 0xFF));
			func_ret = wifi_execute_at_command(WIFI_UART_COMMAND_PORT, g_wifi_uart[WIFI_UART_COMMAND_PORT].command_buff, 20000, WIFI_RETURN_ENUM_OK, WIFI_COMMAND_SET_PING, 0xff);
			if(func_ret != 0)
			{
				api_ret = WIFI_ERR_MODULE_COM;
			}
			else
			{
			}
		}
		if(WIFI_ERR_TAKE_MUTEX != api_ret)
		{
			wifi_give_mutex(mutex_flag);
		}
		if(WIFI_SUCCESS == api_ret)
		{
			vTaskDelay(intervalms);
		}
		else
		{
			break;
		}
	}
	return api_ret;
}

uint32_t R_WIFI_ESP8266_GetVersion(void)
{
    /* These version macros are defined in r_wifi_esp8266_if.h. */
    return ((((uint32_t)WIFI_ESP8266_VERSION_MAJOR) << 16) | (uint32_t)WIFI_ESP8266_VERSION_MINOR);
}

static int32_t wifi_execute_at_command(uint8_t serial_ch_id, uint8_t *ptextstring, uint16_t timeout_ms, wifi_return_code_t expect_code,  wifi_command_list_t command, int32_t socket_no)
{
	volatile int32_t timeout;
	sci_err_t ercd;
	int8_t  get_queue;
	uint32_t recvcnt = 0;
	int32_t ret;
	wifi_return_code_t result;
	uint32_t ticket_no;

	timeout_init(serial_ch_id, timeout_ms);

	if(ptextstring != NULL)
	{
		timeout = 0;
		recvcnt = 0;

		ticket_no = wifi_set_request_in_queue( command, socket_no );
		g_wifi_uart[serial_ch_id].tx_end_flag = 0;
		ercd = R_SCI_Send(g_wifi_uart[serial_ch_id].wifi_uart_sci_handle, ptextstring, strlen((const char *)ptextstring));
		if(SCI_SUCCESS != ercd)
		{
			return -1;
		}

		while(1)
		{
			if(0 != g_wifi_uart[serial_ch_id].tx_end_flag)
			{
				break;
			}
			if(-1 == check_timeout(serial_ch_id, recvcnt))
			{
				wifi_set_result_to_current_running_queue( WIFI_RETURN_ENUM_INTERNAL_TIMEOUT );
				timeout = 1;
				break;
			}
			vTaskDelay(1);
		}
		if(timeout == 1)
		{
			return -1;
		}
#if DEBUGLOG == 2
		R_BSP_CpuInterruptLevelWrite (WIFI_CFG_SCI_INTERRUPT_LEVEL-1);
		printf("%s",ptextstring);
		R_BSP_CpuInterruptLevelWrite (0);
#endif
	}
	else
	{
		ticket_no = wifi_set_request_in_queue( command, socket_no );
	}
	while(1)
	{
		get_queue = wifi_get_result_from_queue( ticket_no, &result );
		if(0 == get_queue )
		{
			break;
		}

		if(-1 == check_timeout(serial_ch_id, recvcnt))
		{
			wifi_set_result_to_current_running_queue( WIFI_RETURN_ENUM_INTERNAL_TIMEOUT );
			timeout = 1;
			break;
		}
		vTaskDelay(1);
	}
	if(timeout == 1)
	{
		return -1;
	}

	ret = -1;
	if(result == expect_code)
	{
		ret = 0;
	}
	return ret;
}


static void timeout_init(int32_t serial_ch, uint16_t timeout_ms)
{
	wifi_at_communication_info_t *puart;
	puart = &g_wifi_uart[serial_ch];

	puart->starttime = xTaskGetTickCount();
	puart->endtime = puart->starttime + timeout_ms;
	if(puart->endtime < puart->starttime)
	{
		/* endtime value is overflow */
		puart->timeout_overflow_flag = 1;
	}
	else
	{
		puart->timeout_overflow_flag = 0;
	}
}

static int32_t check_timeout(int32_t serial_ch, int32_t rcvcount)
{
	wifi_at_communication_info_t *puart;
	puart = &g_wifi_uart[serial_ch];

	if(0 == rcvcount)
	{
		puart->thistime = xTaskGetTickCount();
		if(puart->timeout_overflow_flag == 0)
		{
			if(puart->thistime >= puart->endtime || puart->thistime < puart->starttime)
			{
				/* Timeout  */
				return -1;
			}
		}
		else
		{
			if(puart->thistime < puart->starttime && puart->thistime >= puart->endtime)
			{
				/* Timeout  */
				return -1;
			}
		}
	}
	/* Not timeout  */
	return 0;
}

static void bytetimeout_init(int32_t serial_ch, uint16_t timeout_ms)
{
	wifi_at_communication_info_t *puart;
	puart = &g_wifi_uart[serial_ch];

	puart->startbytetime = xTaskGetTickCount();
	puart->endbytetime = puart->startbytetime + timeout_ms;
	if(puart->endbytetime < puart->startbytetime)
	{
		/* overflow */
		puart->byte_timeout_overflow_flag = 1;
	}
	else
	{
		puart->byte_timeout_overflow_flag = 0;
	}
}

static int32_t check_bytetimeout(int32_t serial_ch, int32_t rcvcount)
{
	wifi_at_communication_info_t *puart;
	puart = &g_wifi_uart[serial_ch];

	if(0 != rcvcount)
	{
		puart->thisbytetime = xTaskGetTickCount();
		if(puart->byte_timeout_overflow_flag == 0)
		{
			if(puart->thisbytetime >= puart->endbytetime || puart->thisbytetime < puart->startbytetime)
			{
				return -1;
			}
		}
		else
		{
			if(puart->thisbytetime < puart->startbytetime && puart->thisbytetime >= puart->endbytetime)
			{
				/* Timeout  */
				return -1;
			}
		}
	}
	/* Not timeout  */
	return 0;
}

static void socket_timeout_init(uint8_t socket_no, uint32_t timeout_ms, uint8_t flag)
{
	TickType_t *starttime;
	TickType_t *thistime;
	TickType_t *endtime;
	uint8_t    *timeout_overflow_flag;

	wifi_socket_t *psocket;
	psocket = &g_wifi_socket[socket_no];

	if(0 == flag)
	{
		starttime             = &psocket->send_starttime ;
		thistime              = &psocket->send_thistime ;
		endtime               = &psocket->send_endtime ;
		timeout_overflow_flag = &psocket->send_timeout_overflow_flag;
	}
	else
	{
		starttime             = &psocket->recv_starttime ;
		thistime              = &psocket->recv_thistime ;
		endtime               = &psocket->recv_endtime ;
		timeout_overflow_flag = &psocket->recv_timeout_overflow_flag;
	}
    *starttime = xTaskGetTickCount();
    *endtime = *starttime + timeout_ms;
    if(*endtime < *starttime)
    {
        /* overflow */
        *timeout_overflow_flag = 1;
    }
    else
    {
        *timeout_overflow_flag = 0;
    }

}

static int32_t socket_check_timeout(uint8_t socket_no, uint8_t flag)
{
	TickType_t *starttime;
	TickType_t *thistime;
	TickType_t *endtime;
	uint8_t    *timeout_overflow_flag;
	wifi_socket_t *psocket;

	psocket = &g_wifi_socket[socket_no];
	if(0 == flag)
	{
		starttime             = &psocket->send_starttime ;
		thistime              = &psocket->send_thistime ;
		endtime               = &psocket->send_endtime ;
		timeout_overflow_flag = &psocket->send_timeout_overflow_flag;
	}
	else
	{
		starttime             = &psocket->recv_starttime ;
		thistime              = &psocket->recv_thistime ;
		endtime               = &psocket->recv_endtime ;
		timeout_overflow_flag = &psocket->recv_timeout_overflow_flag;
	}

	*thistime = xTaskGetTickCount();
    if(*timeout_overflow_flag == 0)
    {
        if(*thistime >= *endtime || *thistime < *starttime)
        {
            return -1;
        }
    }
    else
    {
        if(*thistime < *starttime && *thistime <= *endtime)
        {
            /* Not timeout  */
            return -1;
        }
    }
    /* Not timeout  */
    return 0;
}

static int32_t wifi_take_mutex(uint8_t mutex_flag)
{
	if(0 != (mutex_flag & MUTEX_TX))
	{
		if( xSemaphoreTake( g_wifi_tx_semaphore, xMaxSemaphoreBlockTime ) != pdTRUE )
		{
        	return -1;
		}
	}

	if(0 != (mutex_flag & MUTEX_RX))
	{
		if( xSemaphoreTake( g_wifi_rx_semaphore, xMaxSemaphoreBlockTime ) != pdTRUE )
		{
			if(0 != (mutex_flag & MUTEX_TX))
			{
				xSemaphoreGive( g_wifi_tx_semaphore );
			}
			return -1;
		}
	}
	return 0;
}

static void wifi_give_mutex(uint8_t mutex_flag)
{
	if(0 != (mutex_flag & MUTEX_RX))
	{
		xSemaphoreGive( g_wifi_rx_semaphore);
	}
	if(0 != (mutex_flag & MUTEX_TX))
	{
		xSemaphoreGive( g_wifi_tx_semaphore);
	}
	return;
}


static int32_t wifi_serial_open(uint32_t baudrate)
{
	sci_err_t   my_sci_err;


	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.baud_rate    = baudrate;
	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.clk_src      = SCI_CLK_INT;
	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.data_size    = SCI_DATA_8BIT;
	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.parity_en    = SCI_PARITY_OFF;
	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.parity_type  = SCI_EVEN_PARITY;
	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.stop_bits    = SCI_STOPBITS_1;
	g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config.async.int_priority = WIFI_CFG_SCI_INTERRUPT_LEVEL;    // 1=lowest, 15=highest

    my_sci_err = R_SCI_Open(SCI_CH_wifi_serial_default, SCI_MODE_ASYNC, &g_wifi_uart[WIFI_UART_COMMAND_PORT].sci_config, wifi_uart_callback_command_port, &g_wifi_uart[WIFI_UART_COMMAND_PORT].wifi_uart_sci_handle);

    if(SCI_SUCCESS != my_sci_err)
    {
    	return -1;
    }

	R_SCI_PinSet_wifi_serial_default();

    return 0;

}

static int32_t wifi_serial_close(void)
{
    R_SCI_Close(g_wifi_uart[WIFI_UART_COMMAND_PORT].wifi_uart_sci_handle);
    return 0;
}

int32_t wifi_socket_init(void)
{
#if 0
	int i;
	for(i = 0;i<CREATEABLE_SOCKETS; i++)
	{
		if(BYTEQ_SUCCESS != R_BYTEQ_Open(g_wifi_socket[i].socket_recv_buff, sizeof(g_wifi_socket[i].socket_recv_buff), &g_wifi_socket[i].socket_byteq_hdl))
		{
			return -1;
		}
	}

    g_wifi_semaphore = xSemaphoreCreateMutex();

    if( g_wifi_semaphore == NULL )
    {
    	return -1;
    }
#endif
	/* Success. */
	return 0;

}

static void wifi_uart_callback_command_port(void *pArgs)
{
    sci_cb_args_t   *p_args;

    p_args = (sci_cb_args_t *)pArgs;

    if (SCI_EVT_RX_CHAR == p_args->event)
    {
        /* From RXI interrupt; received character data is in p_args->byte */
        R_BSP_NOP();
    }
#if SCI_CFG_TEI_INCLUDED
	else if (SCI_EVT_TEI == p_args->event)
	{
		g_wifi_uart[WIFI_UART_COMMAND_PORT].tx_end_flag = 1;
		R_BSP_NOP();

	}
#endif
    else if (SCI_EVT_RXBUF_OVFL == p_args->event)
    {
        /* From RXI interrupt; rx queue is full; 'lost' data is in p_args->byte
           You will need to increase buffer size or reduce baud rate */
        g_wifi_sci_err_flag = 1;
    }
    else if (SCI_EVT_OVFL_ERR == p_args->event)
    {
        /* From receiver overflow error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        g_wifi_sci_err_flag = 2;
    }
    else if (SCI_EVT_FRAMING_ERR == p_args->event)
    {
        /* From receiver framing error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        g_wifi_sci_err_flag = 3;
    }
    else if (SCI_EVT_PARITY_ERR == p_args->event)
    {
        /* From receiver parity error interrupt; error data is in p_args->byte
           Error condition is cleared in calling interrupt routine */
        R_BSP_NOP();
    }
    else
    {
        /* Do nothing */
    }

} /* End of function my_sci_callback() */
