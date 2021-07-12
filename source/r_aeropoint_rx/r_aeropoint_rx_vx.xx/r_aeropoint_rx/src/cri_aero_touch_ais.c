/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_touch_ais.c
 *
 ****************************************************************************/

#include "cri_aero_config_ais.h"
#include "cri_aero_touch_internal.h"

#if CRI_AERO_CONFIG_AIS_USE_COMMAND_TOUCH

#include "platform.h"
#include "cri_aero_movie.h"
#include "cri_aero_command_queue_internal.h"

#include "r_smc_entry.h"
#include "r_sci_iic_rx_if.h"
#include "r_gpio_rx_if.h" 



typedef struct {
	uint8_t DeviceMode;
	uint8_t GestureID;
	uint8_t NumPoints;
} REPORT_DATA;

typedef struct {
	uint8_t xHigh;
	uint8_t xLow;
	uint8_t yHigh;
	uint8_t yLow;
	uint8_t ID;
} TOUCH_DATA;

extern CriTouchEvent _cri_gTouchEvent;
extern CriUint32     _cri_gTouchPosX;
extern CriUint32     _cri_gTouchPosY;

static sci_iic_info_t _gIICInfo;

static uint8_t _aBuffer[63] = { 0 };
static uint8_t _Busy = 0;
static uint8_t _Busy_Movie = 0;

static CriSint32 g_slide_no = -1;

static int16_t g_offx = 0;
static int16_t g_offy = 0;

static void cri_aero_touch_delaywait(unsigned long loop_cnt);


void CRIAPI CriAeroTouch_Initialize_AIS(void)
{
	{
#if (CRI_AERO_PIN_USE_TOUCH_MODULE == 1)
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_TOUCH_MODULE, GPIO_LEVEL_LOW);
		R_GPIO_PinDirectionSet(CRI_AERO_PIN_CONFIG_TOUCH_MODULE, GPIO_DIRECTION_OUTPUT);
		CriAeroTouch_SoftwareDelayMs_AIS(10);
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_TOUCH_MODULE, GPIO_LEVEL_HIGH);
		CriAeroTouch_SoftwareDelayMs_AIS(300);
#endif

	}

	_gIICInfo.ch_no = CRI_AERO_USE_CH_TOUCH_MODULE;        
	if (R_SCI_IIC_Open(&_gIICInfo) != SCI_IIC_SUCCESS) {
		return;
	}

	_Busy = 0;

	return;
}

static void criAeroTouch_SensingCallback(void)
{
	CriAeroTouch_Sensing_AIS();

	return;
}

void CRIAPI CriAeroTouch_Timer_AIS(CriSint32 SlideNo)
{
	static uint8_t slave_addr_eeprom[] = { CRI_AERO_SLV_ADDRESS_TOUCH_MODULE }; 
	static uint8_t access_addr1[] = { 0x00 };
	volatile sci_iic_return_t ret;

	static CriSint32 _wait = 0;

	if (_Busy) {
		return;
	}

	if (_Busy_Movie == 1) {
		_Busy_Movie = 0;
		_wait = 0;
	}

	g_slide_no = SlideNo;

	if (_wait < 30) {
		_wait++;
		return;
	}

	_gIICInfo.p_slv_adr = slave_addr_eeprom;
	_gIICInfo.p_data1st = access_addr1;
	_gIICInfo.p_data2nd = _aBuffer;
	_gIICInfo.dev_sts = SCI_IIC_NO_INIT;
	_gIICInfo.cnt1st = sizeof(access_addr1);
	_gIICInfo.cnt2nd = sizeof(_aBuffer);
	_gIICInfo.callbackfunc = &criAeroTouch_SensingCallback;

	ret = R_SCI_IIC_MasterReceive(&_gIICInfo);
	if (ret == SCI_IIC_SUCCESS) {
		_Busy = 1;
	}
	else if (ret == SCI_IIC_ERR_BUS_BUSY)
	{
	}
	else {
	}

	return;
}

void CRIAPI CriAeroTouch_Sensing_AIS(void)
{
	sci_iic_mcu_status_t      iic_status;
	volatile sci_iic_return_t ret;

	ret = R_SCI_IIC_GetStatus(&_gIICInfo, &iic_status);

	if ((ret == SCI_IIC_SUCCESS) && (iic_status.BIT.NACK == 1)) {
		static int           IsTouched;
		REPORT_DATA          Report;
		TOUCH_DATA           TouchPoint;
		uint8_t            * pBuffer;

		pBuffer = _aBuffer;
		Report.DeviceMode = *pBuffer++;
		Report.GestureID = *pBuffer++;
		Report.NumPoints = *pBuffer++;
		TouchPoint.xHigh = (*pBuffer++) & 0x0F;
		TouchPoint.xLow = *pBuffer++;
		TouchPoint.yHigh = (*pBuffer++) & 0x0F;
		TouchPoint.yLow = *pBuffer++;


		if ((Report.GestureID == 0) && (Report.NumPoints == 1)) {
			if (CriAeroMovie_IsPlay()) {
				CriAeroCommandQueue_QueueGotoSlide(g_slide_no);
				CriAeroMovie_Stop();
				_Busy_Movie = 1;
				goto exit;
			}

			if (IsTouched == 0) {
				IsTouched = 1;
				_cri_gTouchEvent = CRI_TEVT_DOWN;
			}

			g_offx = ((TouchPoint.xHigh & 0x0F) << 8 | TouchPoint.xLow);
			g_offy = ((TouchPoint.yHigh & 0x0F) << 8 | TouchPoint.yLow);

			_cri_gTouchPosX = (CriSint32)g_offx;
			_cri_gTouchPosY = (CriSint32)g_offy;

		}
		else {
			if (IsTouched) {
				IsTouched = 0;

				_cri_gTouchEvent = CRI_TEVT_UP;

			}
		}
	exit:
		_Busy = 0;
	}


	return;
}


#pragma inline_asm cri_aero_touch_delaywait

static void cri_aero_touch_delaywait(unsigned long loop_cnt)
{
	R_BSP_ASM_INTERNAL_USED(loop_cnt)
		R_BSP_ASM_BEGIN
		R_BSP_ASM(BRA.B   R_BSP_ASM_LAB_NEXT(0))
		R_BSP_ASM(NOP)
		R_BSP_ASM_LAB(0:)
		R_BSP_ASM(NOP)
		R_BSP_ASM(SUB     #01H, R1)
		R_BSP_ASM(BNE.B   R_BSP_ASM_LAB_PREV(0))
		R_BSP_ASM_END
}


#define SDC_SD_OVERHEAD_LOOP_COUNT     (4)
#define SDC_SD_CPU_CYCLES_PER_LOOP     (4)

typedef enum
{
	DELAY_MICROSECS = 1000000,
	DELAY_MILLISECS = 1000,
	DELAY_SECS = 1
} cri_aero_touch_delay_units_t;

static bool cri_aero_touch_softwaredelay(uint32_t delay, cri_aero_touch_delay_units_t units)
{
	uint64_t loop_cnt;

	loop_cnt = (((uint64_t)delay * (uint64_t)BSP_ICLK_HZ) / (SDC_SD_CPU_CYCLES_PER_LOOP * units))
		- SDC_SD_OVERHEAD_LOOP_COUNT;

	if ((loop_cnt > 0xFFFFFFFF) || (loop_cnt == 0) ||
		((units != DELAY_MICROSECS) && (units != DELAY_MILLISECS) && (units != DELAY_SECS)))
	{
		return (false);
	}
	cri_aero_touch_delaywait((uint32_t)loop_cnt);
	return (true);
}

void CriAeroTouch_SoftwareDelayMs_AIS(Uint32 Ms)
{
	cri_aero_touch_softwaredelay(Ms, DELAY_MILLISECS);
}

#else

void CRIAPI CriAeroTouch_Initialize_AIS(void)
{
}

void CriAeroTouch_Sensing_AIS(void)
{
}

void CRIAPI CriAeroTouch_Timer_AIS(CriSint32 SlideNo)
{
}

#endif


