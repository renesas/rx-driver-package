/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) CRI Middleware Co., Ltd.
 *
 * Library  : CRI Damp
 * Module   : Library User's Header for RX130 series
 * File     : cri_atom_sg.h
 *
 ****************************************************************************/

#ifndef CRI_ATOM_SG_H_INCLUDED
#define CRI_ATOM_SG_H_INCLUDED

#include "cri_xpt.h"

#define CRIATOM_SG_TIME_INF		(-1)

#define CRIATOM_SG_REPEAT_INF		(-1)

#define criAtomPlayer_SetDefaultConfigForSgPlayer(p_config)	\
{\
	(p_config)->output_sampling_rate	= CRIATOM_DEFAULT_INPUT_MAX_SAMPLING_RATE;\
}


typedef struct CriAtomSgPlayerConfigTag {
	CriSint32 output_sampling_rate;
} CriAtomSgPlayerConfig;


typedef struct CriAtomSgVariationTag {
	CriSint32 frequency;
	CriSint32 sound_time_ms;
	CriSint32 silent_time_ms;
	CriSint32 repeat_count;
} CriAtomSgVariation;

#ifdef __cplusplus
extern "C" {
#endif


CriSint32 CRIAPI criAtomPlayer_CalculateWorkSizeForSgPlayer(
	const CriAtomSgPlayerConfig *config);


CriAtomPlayerHn CRIAPI criAtomPlayer_CreateSgPlayer(
	const CriAtomSgPlayerConfig *config, void *work, CriSint32 work_size);

void CRIAPI criAtomPlayer_SetSgWaveTable(
	CriAtomPlayerHn player, const CriSint16 *wave_tbl, CriUint32 tbl_num_smpl);

void CRIAPI criAtomPlayer_SetSgFormatForOneShot(
	CriAtomPlayerHn player, CriSint32 frequency, CriSint32 sound_time_ms);


void CRIAPI criAtomPlayer_SetSgFormatForRepeat(
	CriAtomPlayerHn player, CriSint32 frequency,
	CriSint32 sound_time_ms, CriSint32 silent_time_ms, CriSint32 repeat_count);

void CRIAPI criAtomPlayer_SetSgFormatForVariation(
	CriAtomPlayerHn player, const CriAtomSgVariation *variation, CriSint32 size,
	CriSint32 all_repeat_count);

void CRIAPI criAtomPlayer_StopWithRelease(CriAtomPlayerHn player);

void CRIAPI criAtomPlayer_SetSgFadeTime(CriAtomPlayerHn player, CriSint32 fade_time_ms);

#ifdef __cplusplus
}
#endif

#endif 
