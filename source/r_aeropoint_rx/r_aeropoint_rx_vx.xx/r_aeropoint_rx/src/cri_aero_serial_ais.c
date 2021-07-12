/***********************************************************************************************************************
 * File Name    : cri_aero_serial_ais.c
 * Version      : 0.01
 * Creation Date: 2020-06-10
 ***********************************************************************************************************************/

#include "cri_aero_config_ais.h"
#include "cri_aero_serial_internal.h"

#if CRI_AERO_CONFIG_AIS_USE_COMMAND_UART

#include "r_cg_macrodriver.h"
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"
#include <string.h>


#define	BUFFER_NUM	(1024)

#define SCI_CH CRI_AERO_USE_CH_COMMAND_UART

#if (CRI_AERO_USE_CH_COMMAND_UART == 0)
#define SCI_PIN_SET		R_SCI_PinSet_SCI0
#elif (CRI_AERO_USE_CH_COMMAND_UART == 1)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI1
#elif (CRI_AERO_USE_CH_COMMAND_UART == 2)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI2
#elif (CRI_AERO_USE_CH_COMMAND_UART == 3)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI3
#elif (CRI_AERO_USE_CH_COMMAND_UART == 4)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI4
#elif (CRI_AERO_USE_CH_COMMAND_UART == 5)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI5
#elif (CRI_AERO_USE_CH_COMMAND_UART == 6)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI6
#elif (CRI_AERO_USE_CH_COMMAND_UART == 7)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI7
#elif (CRI_AERO_USE_CH_COMMAND_UART == 8)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI8
#elif (CRI_AERO_USE_CH_COMMAND_UART == 9)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI9
#elif (CRI_AERO_USE_CH_COMMAND_UART == 10)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI10
#elif (CRI_AERO_USE_CH_COMMAND_UART == 11)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI11
#elif (CRI_AERO_USE_CH_COMMAND_UART == 12)
#define SCI_PIN_SET 	R_SCI_PinSet_SCI12
#endif



static CriBool s_bStart;
static sci_hdl_t   s_my_sci_handle;
static CriUint8 s_rx_buffer[BUFFER_NUM];
static CriUint32 s_rx_receive;


void criAeroSerial_ReceiveEnd_AIS(void);

static void sci_init_ports(void) 
{

	SCI_PIN_SET();


	return;
}

static  void hook()
{
}


static void my_sci_callback(void *pArgs)
{
	sci_cb_args_t   *args;

	args = (sci_cb_args_t *)pArgs;

	if (args->event == SCI_EVT_RX_CHAR)
	{


	}
	else if (args->event == SCI_EVT_RXBUF_OVFL)
	{
		nop();
	}
	else if (args->event == SCI_EVT_OVFL_ERR)
	{
		nop();
		hook();
	}
	else if (args->event == SCI_EVT_FRAMING_ERR)
	{
		nop();
	}
	else if (args->event == SCI_EVT_PARITY_ERR)
	{
		nop();
		hook();
	}
}

static void sci_open()  
{
	sci_cfg_t   my_sci_config;
	sci_err_t my_sci_err;

	my_sci_config.async.baud_rate = 9600;
	my_sci_config.async.clk_src = SCI_CLK_INT;
	my_sci_config.async.data_size = SCI_DATA_8BIT;
	my_sci_config.async.parity_en = SCI_PARITY_OFF;
	my_sci_config.async.parity_type = SCI_EVEN_PARITY;
	my_sci_config.async.stop_bits = SCI_STOPBITS_1;
	my_sci_config.async.int_priority = 3;

	my_sci_err = R_SCI_Open(SCI_CH, SCI_MODE_ASYNC, &my_sci_config, my_sci_callback, &s_my_sci_handle); 

	if (my_sci_err != SCI_SUCCESS)
	{
		hook();
	}
}


void CriAeroSerial_Initialize_AIS(void)
{
	sci_init_ports();  
}

void CriAeroSerial_Finalize_AIS(void)
{
}

void CriAeroSerial_Start_AIS(void)
{
	sci_open();  
	s_bStart = CRI_TRUE;
}

void CriAeroSerial_Stop_AIS(void)
{
	s_bStart = CRI_FALSE;
}

int criAeroSerial_SendData_AIS(CriUint32 Length, const CriUint8 *pData)
{
	if (!s_bStart)
	{
		return 1;
	}
	R_SCI_Send(s_my_sci_handle, (uint8_t *)pData, Length);
	return 0;
}

void criAeroSerial_UpDate_AIS()
{
	if (!s_bStart)
	{
		return;
	}
	else {
		while (1)
		{
			sci_err_t my_sci_err = R_SCI_Receive(s_my_sci_handle, &s_rx_buffer[s_rx_receive%BUFFER_NUM], 1);
			if (my_sci_err == SCI_ERR_INSUFFICIENT_DATA)
			{
				return;
			}
			else if (my_sci_err == SCI_SUCCESS)
			{
				if (s_rx_buffer[s_rx_receive%BUFFER_NUM] == '\n' || s_rx_receive > 1000)
				{
					criAeroSerial_ParseAndRegist(s_rx_receive, &s_rx_buffer[0]);
					s_rx_receive = 0;
				}
				else {
					s_rx_receive++;
				}
			}
			else {
				hook();
			}
		}
	}
}


#else

void CriAeroSerial_Initialize_AIS(void)
{
}

void CriAeroSerial_Finalize_AIS(void)
{
}

void CriAeroSerial_Start_AIS(void)
{
}

void CriAeroSerial_Stop_AIS(void)
{
}

void criAeroSerial_UpDate_AIS(void)
{

}

int criAeroSerial_SendData_AIS(CriUint32 Length, const CriUint8 *pData)
{
	return 0;
}

#endif

