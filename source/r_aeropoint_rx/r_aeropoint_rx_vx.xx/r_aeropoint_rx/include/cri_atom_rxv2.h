/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : CRI Atom Microcontroller
 * Module   : Library User's Header for RXV2
 * File     : cri_atom_rxv2.h
 *
 ****************************************************************************/

#ifndef	CRI_INCL_CRI_ATOM_RXV2_H
#define	CRI_INCL_CRI_ATOM_RXV2_H

#include "cri_atom.h"
#include "cri_sfr_config_rxv2.h"


#define CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE		(56)


#define CRIATOM_CALCULATE_INPUT_BUFFER_ADX_MIN_SIZE(in_sfreq, svr_freq)	(																					\
	((in_sfreq)/(svr_freq)%32)																																\
		? ( (((in_sfreq)/(svr_freq)/32+1)*18) > CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE ) ? (((in_sfreq)/(svr_freq)/32+1)*18) : CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE	\
		: ( (((in_sfreq)/(svr_freq)/32)  *18) > CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE ) ? (((in_sfreq)/(svr_freq)/32)  *18) : CRIATOM_INPUT_BUFFER_ADX_MIN_SIZE	\
)


typedef struct CriAtomDeviceConfigTag_RXV2 {
	const CriSfrConfig_RXV2 *sfr_config;
} CriAtomDeviceConfig_RXV2;

typedef struct CriAtomConfigTag_RXV2 {
	CriAtomConfig config;

	CriUint16 output_sampling_rate;

	CriUint16 output_channels;

	CriAtomDeviceConfig_RXV2 device_config;
} CriAtomConfig_RXV2;


#ifdef __cplusplus
extern "C" {
#endif

#define criAtom_CalculateWorkSize_RXV2  criAtom_CalculateWorkSize_RX231
#define criAtom_Initialize_RXV2         criAtom_Initialize_RX231
#define criAtom_Finalize_RXV2           criAtom_Finalize_RX231
#define criAtom_ExecuteIntr_RXV2        criAtom_ExecuteIntr_RX231

CriSint32 CRIAPI criAtom_CalculateWorkSize_RXV2(const CriAtomConfig_RXV2* config);

void CRIAPI criAtom_Initialize_RXV2(const CriAtomConfig_RXV2* config, void* work, CriSint32 work_size);

void CRIAPI criAtom_Finalize_RXV2(void);

void CRIAPI criAtom_ExecuteIntr_RXV2(void);

#ifdef __cplusplus
}
#endif

#endif	

