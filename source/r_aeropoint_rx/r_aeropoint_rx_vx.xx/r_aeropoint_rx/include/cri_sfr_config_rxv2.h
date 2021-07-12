/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : CRI SFR Config
 * Module   : Library User's Header for RXV2
 * File     : cri_sfr_config_rxv2.h
 *
 ****************************************************************************/
#ifndef CRI_SFR_CONFIG_RXV2_H_INCLUDED
#define CRI_SFR_CONFIG_RXV2_H_INCLUDED


#define CRISFRCONFIG_DEFAULT_MAX_CHANNELS	(1)


typedef enum {
	CRISFR_PWM_MODE_ONE = 0,

	CRISFR_PWM_MODE_RESET_SYNC = 1,

	CRISFR_PWM_MODE_ONE_WITH_DTC = 2,

	CRISFR_PWM_MODE_ENUM_SIZE_IS_4BYTES = 0x7FFFFFFF
} CriSfrPwmMode_RXV2;

typedef struct CriSfrConfigTag_RXV2 {
	CriUint32 timer_clk;

	CriSfrPwmMode_RXV2 pwm_mode;

	volatile __evenaccess CriUint8*  pwm_mtu_tstr;
	volatile __evenaccess CriUint8*  pwm_mtu_toer;
	volatile __evenaccess CriUint8*  pwm_mtu_tocr1;

	volatile __evenaccess CriUint8*  pwm_mtu_trwer;

	volatile __evenaccess CriUint8*  pwm_mtu_tgcr;

	volatile __evenaccess CriUint8*  pwm_mtu_titcr;

	CriUint8 pwm_mtua_id[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];

	CriUint8 pwm_mtub_id[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];

	volatile __evenaccess CriUint8*  pwm_mtua_tcr[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtua_tmdr[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtua_tiorh[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtua_tiorl[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtua_tier[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtua_tsr[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtua_tcnt[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtua_tgra[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtua_tgrb[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtua_tgrc[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtua_tgrd[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtua_tbtm[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];

	volatile __evenaccess CriUint8*  pwm_mtub_tcr[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtub_tmdr[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtub_tiorh[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtub_tiorl[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtub_tier[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtub_tsr[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtub_tcnt[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtub_tgra[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtub_tgrb[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtub_tgrc[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint16* pwm_mtub_tgrd[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];
	volatile __evenaccess CriUint8*  pwm_mtub_tbtm[CRISFRCONFIG_DEFAULT_MAX_CHANNELS];

	CriUint8 pwm_mtua_intr_vect_num;
	CriUint8 pwm_mtub_intr_vect_num;
	volatile __evenaccess CriUint8*  dtc_dtccr;
	volatile __evenaccess CriUint8*  dtc_dtcadmod;
	volatile __evenaccess CriUint32* dtc_dtcvbr;
	volatile __evenaccess CriUint8*  dtc_dtcst;
	volatile void***	 dtc_vect_tbl;
} CriSfrConfig_RXV2;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif 
