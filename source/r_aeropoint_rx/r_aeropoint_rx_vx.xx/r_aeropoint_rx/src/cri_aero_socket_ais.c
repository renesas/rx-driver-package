/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_socket_ais.c
 *
 ****************************************************************************/
#include "cri_aero_config_ais.h"

#if CRI_AERO_CONFIG_AIS_USE_LAN

#include "r_socket_rx_if.h"
#include "cri_aero_command_queue_internal.h"
#include <string.h>
#include "cri_aero_support_atom_internal.h"
#include "cri_aero_report_internal.h"
#include "cri_aero_command_def.h"
#include "r_ether_rx_pinset.h"
#include "r_ether_rx_config.h"
#include "platform.h"

#define PORT_NUMBER		(7743)

#define _PHASE_ACCEPT	( 0 )
#define _PHASE_RECV		( 1 )
#define _PHASE_WAIT		( 2 )

#define RCV_UNIT_TAG	(64L)

#define NUM_OF_RETRY	(10)

#if   defined(BSP_MCU_RX65N)
#define SERVICE_MESSAGE "RX65N is in service."
#elif defined(BSP_MCU_RX66N)
#define SERVICE_MESSAGE "RX66N is in service."
#elif defined(BSP_MCU_RX72M)
#define SERVICE_MESSAGE "RX72M is in service."
#elif defined(BSP_MCU_RX72N)
#define SERVICE_MESSAGE "RX72N is in service."
#else
#define SERVICE_MESSAGE "RX??? is in service."
#endif


#if (CRI_AERO_USE_CH_LAN == 0) && (ETHER_CFG_MODE_SEL == 0)
#define ETHER_PIN_SET       R_ETHER_PinSet_ETHERC0_MII
#elif (CRI_AERO_USE_CH_LAN == 0) && (ETHER_CFG_MODE_SEL == 1)
#define ETHER_PIN_SET       R_ETHER_PinSet_ETHERC0_RMII
#elif (CRI_AERO_USE_CH_LAN == 1) && (ETHER_CFG_MODE_SEL == 0)
#define ETHER_PIN_SET       R_ETHER_PinSet_ETHERC1_MII
#elif (CRI_AERO_USE_CH_LAN == 1) && (ETHER_CFG_MODE_SEL == 1)
#define ETHER_PIN_SET       R_ETHER_PinSet_ETHERC1_RMII
#endif


static struct sockaddr_in _gSockAcceptAddr;
static fd_set    _gReadSockfds, _gWriteSockfds, _gRDTestSockfds, _gWRTestSockfds;
struct timeval   _gTimeOut;

static CriSint32 _gSock;		
static CriSint32 _gSockClient;	
static CriSint32 _gSockKeep;	
static CriSint32 _gSockAcceptSize;
static CriSint32 _gnfds;



void CRIAPI criAeroSocket_Initialize(void)
{
	struct sockaddr_in _SockBindAddr;

	ETHER_PIN_SET();              

	CriAeroCommandQueue_Initialize();

	if (lan_open() != E_OK) {
		return;
	}

	_gSock = SOCKET_ERROR;
	_gSockClient = SOCKET_ERROR;
	_gSockKeep = SOCKET_ERROR;

	R_SOCKET_Open();

	_gSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_gSock == SOCKET_ERROR) { goto err_exit; }

	_gnfds = _gSock + 1;
	FD_SET(_gSock, &_gReadSockfds);
	FD_SET(_gSock, &_gWriteSockfds);

	if (fcntl(_gSock, F_SETFL, O_NONBLOCK) != E_OK) { goto err_exit; }

	_SockBindAddr.sin_family = AF_INET;
	_SockBindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_SockBindAddr.sin_port = htons(PORT_NUMBER);
	if (bind(_gSock, (struct sockaddr *)&_SockBindAddr, sizeof(_SockBindAddr)) < 0) { goto err_exit; }

	if (listen(_gSock, 1) < 0) { goto err_exit; }

	memset(&_gSockAcceptAddr, 0, sizeof(_gSockAcceptAddr));
	_gSockAcceptSize = sizeof(_gSockAcceptAddr);
	_gSockClient = accept(_gSock, (struct sockaddr *)&_gSockAcceptAddr, (int *)&_gSockAcceptSize);
	if ((SOCKET_ERROR == _gSockClient) && (errno == EAGAIN))
	{
	}
	else
	{
		goto err_exit;
	}

	return;

err_exit:
	if (_gSock != SOCKET_ERROR) {
		closesocket(_gSock);
		_gSock = SOCKET_ERROR;
	}

	return;
}

static void trap(void)
{
	for (;;)
	{
	}
}

void CRIAPI criAeroSocket_Exec(void)
{
	do {
		switch (s_commandStruct.phase) {
		case _PHASE_ACCEPT:
			FD_COPY(&_gReadSockfds, &_gRDTestSockfds);
			FD_COPY(&_gWriteSockfds, &_gWRTestSockfds);
			_gTimeOut.tv_sec = 0;	
			_gTimeOut.tv_usec = 0;
			select(_gnfds, &_gRDTestSockfds, &_gWRTestSockfds, CRI_NULL, &_gTimeOut);

			if (FD_ISSET(_gSock, &_gRDTestSockfds)) {
				memset(&_gSockAcceptAddr, 0, sizeof(_gSockAcceptAddr));
				_gSockAcceptSize = sizeof(_gSockAcceptAddr);
				_gSockClient = accept(_gSock, (struct sockaddr *)&_gSockAcceptAddr, (int *)&_gSockAcceptSize);
				if (_gSockClient != SOCKET_ERROR) {
					s_commandStruct.phase = _PHASE_RECV;
					s_commandStruct.recieved = CRI_FALSE;
					s_commandStruct.tag_mode = _PACKET_MODE_TAG;
					s_commandStruct.targetDataGain = 0;
					s_commandStruct.targetDataSize = 0;
					s_commandStruct.gain = 0;
					s_commandStruct.retry = 0;
				}
			}
			else if (FD_ISSET(_gSock, &_gWRTestSockfds)) {
				s_commandStruct.phase = _PHASE_WAIT;
				s_commandStruct.wait = 10;
				break;
			}
			else {
				break;
			}
		case _PHASE_RECV:
		{
			CriUint32 size = s_commandStruct.tag_mode == _PACKET_MODE_TAG ? RCV_UNIT_TAG : RCV_UNIT_DATA;
			CriUint32 remain = size - s_commandStruct.gain;
			CriSint32 recv_size = recv(_gSockClient, s_commandStruct.recvbuf + s_commandStruct.gain, remain, 0);

			if (recv_size > 0)
			{
				s_commandStruct.recieved = CRI_TRUE;
				s_commandStruct.gain += recv_size;
				if (s_commandStruct.gain == size)
				{
					switch (s_commandStruct.tag_mode)
					{
					case _PACKET_MODE_TAG:	func_packet_mode_tag();	break;
					case _PACKET_MODE_DATA:	func_packet_mode_data(); break;
					case _PACKET_MODE_RLE:	func_packet_mode_rle();	break;
					default:				trap();					break;
					}
				}
				else if (s_commandStruct.gain>size)
				{
					trap();
				}

			}
			else {

				if ((recv_size == SOCKET_ERROR) && ((errno == EAGAIN) || (errno == 0))) {
					s_commandStruct.retry = 0;
				}
				else if ((recv_size == 0) && ((errno == EAGAIN) || (errno == 0))) {
					if (s_commandStruct.targetDataSize == 0) {
						s_commandStruct.retry++;
						if (s_commandStruct.retry > 10) {
							s_commandStruct.phase = _PHASE_ACCEPT;
							s_commandStruct.recieved = CRI_FALSE;

							closesocket(_gSockClient);
							_gSockClient = SOCKET_ERROR;
						}
					}
				}
				else {

					s_commandStruct.phase = _PHASE_ACCEPT;
					s_commandStruct.recieved = CRI_FALSE;

					closesocket(_gSockClient);
					_gSockClient = SOCKET_ERROR;
				}
			}
		}
		break;
		case _PHASE_WAIT:
			s_commandStruct.wait--;
			if (s_commandStruct.wait < 0) {
				s_commandStruct.phase = _PHASE_ACCEPT;
			}
			break;
		}
		CriAeroSupportAtom_Nop_AIS();
	} while (s_commandStruct.tag_mode == _PACKET_MODE_DATA);

	return;
}

void CRIAPI criAeroSocket_Finalize(void)
{
	if (_gSockKeep != SOCKET_ERROR) {
		closesocket(_gSockKeep);
	}

	if (_gSockClient != SOCKET_ERROR) {
		closesocket(_gSockClient);
	}

	if (_gSock != SOCKET_ERROR) {
		closesocket(_gSock);
	}

	R_SOCKET_Close();

	lan_close();

 
	return;
}


CriBool CRIAPI criAeroSocket_SendCommand(const APLCommand *pCommandBuffer)
{
	for (int i = 0; i < NUM_OF_RETRY; i++)
	{
		if (_gSockClient != SOCKET_ERROR)
		{
			int ret;
#if 0
			if ((rand() % 300)<30)
			{
				ret = send(_gSockClient, (const char *)pCommandBuffer, sizeof(struct APLCommand), 0);
			}
			else {
				ret = -1;
			}
#else
			ret = send(_gSockClient, (const char *)pCommandBuffer, sizeof(struct APLCommand), 0);
#endif
			if (ret <= 0)
			{
				CriAeroReport_CountUp(0);
			}
			else if (ret == sizeof(struct APLCommand))
			{
				return CRI_TRUE;
			}
			else
			{
				trap();
			}
		}
		else
		{
			break;
		}
	}

	CriAeroReport_CountUp(1);

	return CRI_FALSE;
}

void CRIAPI criAeroSocket_SendCaptureData(
	void *frameBuffer,
	CriUint32 width,
	CriUint32 height,
	CriUint32 pitch,
	void *paletteData,
	CriUint32 paletteSize
)
{
	APLCommand *pcb = CriAeroCommandQueue_GetBuffer();
	APLCommandForCaptureInfo *pcb2 = (APLCommandForCaptureInfo *)pcb;
	pcb2->Tag = _APL_TAG_COMMAND;
	pcb2->Command = _APL_CMD_CAPTURE_DATA;
	pcb2->Width = width;
	pcb2->Height = height;
	pcb2->PaletteSize = paletteSize;
	criAeroSocket_SendCommand((APLCommand *)pcb2);

	{
		{
			CriUint8 *ptr = (CriUint8 *)paletteData;
			for (int i = 0; i<paletteSize / 64; i++)
			{
				int ret = 0;
			Retry2:
				ret = send(_gSockClient, (const char *)&ptr[i * 64], 64, 0);
				if (ret != 64)
				{
					goto Retry2;
				}
			}
		}

		CriUint8 *ptr = (CriUint8 *)frameBuffer;
		for (CriUint32 h = 0; h < height; h++)
		{
			int gain = 0;
			while (1)
			{
				int rest = width - gain;
				if (rest == 0)
				{
					goto NEXT;
				}
				int ret = send(_gSockClient, (const char *)(&ptr[h*pitch] - gain), rest, 0);
				if (ret <= 0)
				{
				}
				else {
					gain += ret;
				}
			}
		NEXT:;
		}
	}
}


void criAeroSocket_SendInServiceMessage(void)
{
	CriAeroCommandQueue_SendLog(SERVICE_MESSAGE);
}


void r_socket_task_switch(int sock)
{
#if BSP_CFG_RTOS_USED == 0 

#elif BSP_CFG_RTOS_USED == 1 

	vTaskDelay(2 / portTICK_RATE_MS);

#elif BSP_CFG_RTOS_USED == 2 

#elif BSP_CFG_RTOS_USED == 3 

#elif BSP_CFG_RTOS_USED == 4 

	dly_tsk(2);

#endif
}

void r_socket_task_switch_select(void)
{
#if BSP_CFG_RTOS_USED == 0 

#elif BSP_CFG_RTOS_USED == 1 

	vTaskDelay(2 / portTICK_RATE_MS);

#elif BSP_CFG_RTOS_USED == 2 

#elif BSP_CFG_RTOS_USED == 3 

#elif BSP_CFG_RTOS_USED == 4 

	dly_tsk(2);

#endif
}

#if BSP_CFG_RTOS_USED == 1 

extern xSemaphoreHandle r_socket_semaphore;

#elif BSP_CFG_RTOS_USED == 4 

extern ID r_socket_semaphore;

#endif

int r_socket_sem_release(void)
{
	int retcode;
	retcode = 0;

#if BSP_CFG_RTOS_USED == 0 

#elif BSP_CFG_RTOS_USED == 1 

	if (pdTRUE != xSemaphoreGive(r_socket_semaphore))
	{
		retcode = SOXKER_ERROR;
	}

#elif BSP_CFG_RTOS_USED == 2 

#elif BSP_CFG_RTOS_USED == 3 

#elif BSP_CFG_RTOS_USED == 4 

	if (E_OK != sig_sem(r_socket_semaphore))
	{
		retcode = SOXKER_ERROR;
	}

#endif

	return retcode;
}

#else

#include "cri_aero_command_internal.h"

void CRIAPI criAeroSocket_Initialize(void)
{
}

void CRIAPI criAeroSocket_Finalize(void)
{
}

CriBool CRIAPI criAeroSocket_SendCommand(const APLCommand *pCommandBuffer)
{
	return CRI_FALSE;
}

void CRIAPI criAeroSocket_SendCaptureData(
	void *frameBuffer,
	CriUint32 width,
	CriUint32 height,
	CriUint32 pitch,
	void *paletteData,
	CriUint32 paletteSize
)
{
}

void CRIAPI criAeroSocket_Exec(void)
{
}

void r_socket_task_switch_select(void)
{
    return;
}

int r_socket_sem_release(void)
{
    return 0;
}

void r_socket_task_switch(int sock)
{
    return;
}

#endif

