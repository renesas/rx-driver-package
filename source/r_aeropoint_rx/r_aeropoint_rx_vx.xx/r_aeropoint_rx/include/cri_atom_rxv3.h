/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : CRI Atom Microcontroller
 * Module   : Library User's Header for RXV3
 * File     : cri_atom_rxv3.h
 *
 ****************************************************************************/

#ifndef	CRI_INCL_CRI_ATOM_RXV3_H
#define	CRI_INCL_CRI_ATOM_RXV3_H

#include "cri_atom.h"
#include "cri_sfr_config_rxv3.h"


#define CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE		(56)


#define CRIATOM_CALCULATE_INPUT_BUFFER_ADX_MIN_SIZE(in_sfreq, svr_freq)	(																					\
	((in_sfreq)/(svr_freq)%32)																																\
		? ( (((in_sfreq)/(svr_freq)/32+1)*18) > CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE ) ? (((in_sfreq)/(svr_freq)/32+1)*18) : CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE	\
		: ( (((in_sfreq)/(svr_freq)/32)  *18) > CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE ) ? (((in_sfreq)/(svr_freq)/32)  *18) : CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE	\
)


typedef struct CriAtomDeviceConfigTag_RXV3 {
	const CriSfrConfig_RXV3 *sfr_config;
} CriAtomDeviceConfig_RXV3;

typedef struct CriAtomConfigTag_RXV3 {
	CriAtomConfig config;
	
	CriUint16 output_sampling_rate;
	
	CriUint16 output_channels;
	
	CriAtomDeviceConfig_RXV3 device_config;
} CriAtomConfig_RXV3;


#ifdef __cplusplus
extern "C" {
#endif

CriSint32 CRIAPI criAtom_CalculateWorkSize_RXV3(const CriAtomConfig_RXV3* config);

void CRIAPI criAtom_Initialize_RXV3(const CriAtomConfig_RXV3* config, void* work, CriSint32 work_size);

void CRIAPI criAtom_Finalize_RXV3(void);

void CRIAPI criAtom_ExecuteIntr_RXV3(void);

#ifdef __cplusplus
}
#endif

#endif	


