/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : CRI SFR Config
 * Module   : Library User's Header for RXV3
 * File     : cri_sfr_config_rxv3.h
 *
 ****************************************************************************/
#ifndef CRI_SFR_CONFIG_RXV3_H_INCLUDED
#define CRI_SFR_CONFIG_RXV3_H_INCLUDED


#define CRISFRCONFIG_DEFAULT_MAX_CHANNELS	(1)


typedef enum {
	CRISFR_PWM_MODE_ONE = 0,

#if 0

	CRISFR_PWM_MODE_ONE_WITH_DTC = 2,

#endif

	CRISFR_PWM_MODE_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF
} CriSfrPwmMode_RXV3;

typedef struct CriSfrConfigTag_RXV3 {
	CriUint32 timer_clk;
	
	CriSfrPwmMode_RXV3 pwm_mode;

	CriUint32 gptw_channel_number;

	volatile __evenaccess CriUint32* gptw_gtadsmr;
	volatile __evenaccess CriUint32* gptw_gtber;
	volatile __evenaccess CriUint32* gptw_gtccra;
	volatile __evenaccess CriUint32* gptw_gtccrb;
	volatile __evenaccess CriUint32* gptw_gtccrc;
	volatile __evenaccess CriUint32* gptw_gtccrd;
	volatile __evenaccess CriUint32* gptw_gtccre;
	volatile __evenaccess CriUint32* gptw_gtccrf;
	volatile __evenaccess CriUint32* gptw_gtclr;
	volatile __evenaccess CriUint32* gptw_gtcnt;
	volatile __evenaccess CriUint32* gptw_gtcr;
	volatile __evenaccess CriUint32* gptw_gtcsr;
	volatile __evenaccess CriUint32* gptw_gtdnsr;
	volatile __evenaccess CriUint32* gptw_gteitc;
	volatile __evenaccess CriUint32* gptw_gtintad;
	volatile __evenaccess CriUint32* gptw_gtior;
	volatile __evenaccess CriUint32* gptw_gtitc;
	volatile __evenaccess CriUint32* gptw_gtpr;
	volatile __evenaccess CriUint32* gptw_gtpsr;
	volatile __evenaccess CriUint32* gptw_gtssr;
	volatile __evenaccess CriUint32* gptw_gtstp;
	volatile __evenaccess CriUint32* gptw_gtstr;
	volatile __evenaccess CriUint32* gptw_gtuddtyc;
	volatile __evenaccess CriUint32* gptw_gtupsr;
	volatile __evenaccess CriUint32* gptw_gtwp;

} CriSfrConfig_RXV3;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif 

