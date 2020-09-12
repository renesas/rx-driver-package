#ifndef R_WIFI_ESP8266_IF_H
#define R_WIFI_ESP8266_IF_H

//#include "FreeRTOS.h"
#include "platform.h"
#include "r_sci_rx_if.h"
#include "r_byteq_if.h"
#include "r_wifi_esp8266_config.h"

/* Driver Version Number. */
#define WIFI_ESP8266_VERSION_MAJOR           (1)
#define WIFI_ESP8266_VERSION_MINOR           (00)


#define WIFI_SOCKET_IP_PROTOCOL_TCP (6)
#define WIFI_SOCKET_IP_VERSION_4    (4)

#define WIFI_IPV4BYTE_TO_ULONG(in_addr1, in_addr2, in_addr3, in_addr4) \
	(((uint32_t)in_addr1 & 0x000000FF) << 24) | (((uint32_t)in_addr2 & 0x000000FF) << 16) |\
    (((uint32_t)in_addr3 & 0x000000FF) << 8)  | (((uint32_t)in_addr4 & 0x000000FF))

#define WIFI_ULONG_TO_IPV4BYTE_1(in_ul) (uint8_t)(((uint32_t)in_ul) >> 24)
#define WIFI_ULONG_TO_IPV4BYTE_2(in_ul) (uint8_t)(((uint32_t)in_ul) >> 16)
#define WIFI_ULONG_TO_IPV4BYTE_3(in_ul) (uint8_t)(((uint32_t)in_ul) >> 8)
#define WIFI_ULONG_TO_IPV4BYTE_4(in_ul) (uint8_t)((uint32_t)in_ul)

typedef enum
{
	WIFI_SECURITY_OPEN            = 0,
	WIFI_SECURITY_WEP,
	WIFI_SECURITY_WPA,
	WIFI_SECURITY_WPA2,
	WIFI_SECURITY_UNDEFINED,
}wifi_security_t;

typedef enum 			// Wi-Fi APIエラーコード
{
	WIFI_SUCCESS            = 0,	// 成功
	WIFI_ERR_PARAMETER	    = -1,	// 引数が無効です。
	WIFI_ERR_ALREADY_OPEN   = -2,	// すでに初期化済みです
	WIFI_ERR_NOT_OPEN       = -3,	// 初期化していません
	WIFI_ERR_SERIAL_OPEN    = -4,	// シリアルの初期化ができません。
	WIFI_ERR_MODULE_COM     = -5,	// WiFiモジュールとの通信に失敗しました。
	WIFI_ERR_NOT_CONNECT    = -6,	// アクセスポイントに接続していません。
	WIFI_ERR_SOCKET_NUM     = -7,	// 利用可能なソケットがありません。
	WIFI_ERR_SOCKET_CREATE  = -8,	// ソケットを作成できません。
	WIFI_ERR_CHANGE_SOCKET  = -9,	// ソケットを切り替えられません。
	WIFI_ERR_SOCKET_CONNECT = -10,	// ソケットに接続できません。
	WIFI_ERR_BYTEQ_OPEN     = -11,	// BYTEQの割り当てに失敗しました。
	WIFI_ERR_SOCKET_TIMEOUT = -12,	// ソケットの送信でタイムアウトしました。
	WIFI_ERR_TAKE_MUTEX     = -13,	// Mutexの取得に失敗しました。
} wifi_err_t;

typedef enum
{
	WIFI_EVENT_WIFI_REBOOT = 0,
	WIFI_EVENT_WIFI_DISCONNECT,
	WIFI_EVENT_SERIAL_OVF_ERR,
	WIFI_EVENT_SERIAL_FLM_ERR,
	WIFI_EVENT_SERIAL_RXQ_OVF_ERR,
	WIFI_EVENT_RCV_TASK_RXB_OVF_ERR,
	WIFI_EVENT_SOCKET_CLOSED,
	WIFI_EVENT_SOCKET_RXQ_OVF_ERR,
} wifi_err_event_enum_t;

typedef struct
{
		uint8_t ssid[ 33 ];			//SSIDの格納領域
		uint8_t bssid[ 6 ];			//BSSIDの格納領域
		wifi_security_t security;	//セキュリティタイプの格納領域
		int8_t rssi;		     	//信号強度の格納領域
		int8_t channel;				//チャンネル番号の格納領域
		uint8_t hidden;				//Hiddenチャンネルの格納領域
} wifi_scan_result_t;

typedef struct
{
		uint32_t ipaddress;	 //IPアドレス
		uint32_t subnetmask; //サブネットマスク
		uint32_t gateway;	 //ゲートウェイ
} wifi_ip_configuration_t;

typedef struct
{
	wifi_err_event_enum_t event;
	uint32_t socket_no;
}wifi_err_event_t;

wifi_err_t R_WIFI_ESP8266_Open(void);
wifi_err_t R_WIFI_ESP8266_Close(void);
wifi_err_t R_WIFI_ESP8266_Scan(wifi_scan_result_t *ap_results,uint32_t maxnetworks, uint32_t *exist_ap_count);
wifi_err_t R_WIFI_ESP8266_SetDnsServerAddress(uint32_t dnsaddress1, uint32_t dnsaddress2);
wifi_err_t R_WIFI_ESP8266_Connect (uint8_t *pssid, uint8_t *ppass, uint32_t security, uint8_t dhcp_enable, wifi_ip_configuration_t *pipconfig);
wifi_err_t R_WIFI_ESP8266_Disconnect (void);
int32_t    R_WIFI_ESP8266_IsConnected (void);
wifi_err_t R_WIFI_ESP8266_GetMacAddress (uint8_t *pmacaddress);
wifi_err_t R_WIFI_ESP8266_GetIpAddress (wifi_ip_configuration_t *pipconfig);
int32_t    R_WIFI_ESP8266_SocketCreate(uint32_t type, uint32_t ipversion);
wifi_err_t R_WIFI_ESP8266_SocketConnect(int32_t socket_no, uint32_t ipaddr, uint16_t port);
wifi_err_t R_WIFI_ESP8266_SocketClose(int32_t socket_no);
int32_t    R_WIFI_ESP8266_SocketSend (int32_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout_ms);
int32_t    R_WIFI_ESP8266_SocketRecv (int32_t socket_no, uint8_t *pdata, int32_t length, uint32_t timeout_ms);
wifi_err_t R_WIFI_ESP8266_SocketShutdown (int32_t socket_no);

wifi_err_t R_WIFI_ESP8266_DnsQuery (uint8_t *pdomain_name, uint32_t *pipaddress);
wifi_err_t R_WIFI_ESP8266_Ping (uint32_t ipaddr, uint16_t count, uint32_t intervalms);
uint32_t   R_WIFI_ESP8266_GetVersion(void);


#endif /* #define R_WIFI_ESP8266_IF_H */
