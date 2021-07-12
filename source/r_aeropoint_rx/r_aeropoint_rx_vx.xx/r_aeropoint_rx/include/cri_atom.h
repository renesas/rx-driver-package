/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2011 CRI Middleware Co., Ltd.
 *
 * Library  : CRI Atom
 * Module   : Library User's Header
 * File     : cri_atom.h
 *
 ****************************************************************************/
#ifndef CRI_INCL_CRI_ATOM_H
#define CRI_INCL_CRI_ATOM_H

#include "cri_xpt.h"
#include "cri_atom_device.h"
#define CRI_ATOM_VERSION				(0x01030700)
#define CRI_ATOM_VER_NUM				"1.03.07"
#define CRI_ATOM_VER_NAME				"CRI Atom"


#define criAtom_SetDefaultConfig(p_config)	\
{\
	(p_config)->thread_model = CRIATOM_THREAD_MODEL_SINGLE;\
	(p_config)->server_frequency = 60;\
	(p_config)->fs_config = NULL;\
	(p_config)->context = NULL;\
}

#define criAtomPlayer_SetDefaultConfigForAdxPlayer(p_config)	\
{\
	(p_config)->max_channels		= CRIATOM_DEFAULT_INPUT_MAX_CHANNELS;\
	(p_config)->max_sampling_rate	= CRIATOM_DEFAULT_INPUT_MAX_SAMPLING_RATE;\
	(p_config)->streaming_flag		= CRI_TRUE;\
	(p_config)->sound_renderer_type	= CRIATOM_SOUND_RENDERER_DEFAULT;\
	(p_config)->decode_latency		= CRIATOM_DEFAULT_DECODE_LATENCY;\
}

#define criAtomPlayer_SetDefaultConfigForHcaPlayer(p_config)	\
{\
	(p_config)->max_channels		= CRIATOM_DEFAULT_INPUT_MAX_CHANNELS;\
	(p_config)->max_sampling_rate	= CRIATOM_DEFAULT_INPUT_MAX_SAMPLING_RATE;\
	(p_config)->streaming_flag		= CRI_TRUE;\
	(p_config)->sound_renderer_type	= CRIATOM_SOUND_RENDERER_DEFAULT;\
	(p_config)->decode_latency		= CRIATOM_DEFAULT_DECODE_LATENCY;\
}


typedef enum {
	CRIATOM_THREAD_MODEL_MULTI = 0,
	
	CRIATOM_THREAD_MODEL_USER_MULTI = 1,
	
	CRIATOM_THREAD_MODEL_SINGLE = 2,
	

	CRIATOM_THREAD_MODEL_ENUM_SIZE_IS_4BYTES = CRIATOM_DEVICE_ENUM_SIZE
} CriAtomThreadModel;

typedef struct CriAtomConfigTag {
	CriAtomThreadModel thread_model;
	
	CriUint16 server_frequency;
	
	const void *fs_config;

	void *context;
} CriAtomConfig;

typedef enum {
	CRIATOM_SOUND_RENDERER_DEFAULT = 0,
	CRIATOM_SOUND_RENDERER_NATIVE = 1,
	CRIATOM_SOUND_RENDERER_ASR = 2,
	
	CRIATOM_SOUND_RENDERER_HW1 = 1,
	CRIATOM_SOUND_RENDERER_HW2 = 9,
	
	CRIATOM_SOUND_RENDERER_ENUM_SIZE_IS_4BYTES = CRIATOM_DEVICE_ENUM_SIZE
} CriAtomSoundRendererType;

typedef CriUint32 CriAtomFormat;
#define CRIATOM_FORMAT_NONE			(0x00000000)		
#define CRIATOM_FORMAT_ADX			(0x00000001)		
#define CRIATOM_FORMAT_HCA			(0x00000003)		
#define CRIATOM_FORMAT_SG			(0x00000004)		

typedef struct CriAtomAdxPlayerConfigTag {
	CriSint32 max_channels;
	
	CriSint32 max_sampling_rate;
	
	CriBool streaming_flag;
	
	CriAtomSoundRendererType sound_renderer_type;
	
	CriSint32 decode_latency;
} CriAtomAdxPlayerConfig;

typedef struct CriAtomHcaPlayerConfigTag {
	CriSint32 max_channels;
	
	CriSint32 max_sampling_rate;
	
	CriBool streaming_flag;
	
	CriAtomSoundRendererType sound_renderer_type;
	
	CriSint32 decode_latency;
} CriAtomHcaPlayerConfig;

typedef enum {
	CRIATOMPLAYER_STATUS_STOP = 0,		
	CRIATOMPLAYER_STATUS_PREP = 1,		
	CRIATOMPLAYER_STATUS_PLAYING = 2,	
	CRIATOMPLAYER_STATUS_PLAYEND = 3,	
	CRIATOMPLAYER_STATUS_ERROR = 4,		
	
	CRIATOMPLAYER_STATUS_ENUM_SIZE_IS_4BYTES = CRIATOM_DEVICE_ENUM_SIZE
} CriAtomPlayerStatus;

struct CriAtomPlayerTag;
typedef struct CriAtomPlayerTag *CriAtomPlayerHn;

typedef void (CRIAPI *CriAtomPlayerDataRequestCbFunc)(void *obj, CriAtomPlayerHn player);



#ifdef __cplusplus
extern "C" {
#endif

CriUint32 CRIAPI criAtom_GetVersionNumber(void);

CriSint32 CRIAPI criAtom_CalculateWorkSize(const CriAtomConfig *config);

void CRIAPI criAtom_Initialize(const CriAtomConfig *config, void *work, CriSint32 work_size);

void CRIAPI criAtom_Finalize(void);

void CRIAPI criAtom_ExecuteMain(void);

CriSint32 CRIAPI criAtomPlayer_CalculateWorkSizeForAdxPlayer(
	const CriAtomAdxPlayerConfig *config);

CriAtomPlayerHn CRIAPI criAtomPlayer_CreateAdxPlayer(
	const CriAtomAdxPlayerConfig *config, void *work, CriSint32 work_size);

CriSint32 CRIAPI criAtomPlayer_GetMinimalInputBufferSizeForHcaPlayer(
	void *data, CriUint32 data_size);

CriSint32 CRIAPI criAtomPlayer_CalculateWorkSizeForHcaPlayer(
	const CriAtomHcaPlayerConfig *config);

CriAtomPlayerHn CRIAPI criAtomPlayer_CreateHcaPlayer(
	const CriAtomHcaPlayerConfig *config, void *work, CriSint32 work_size);

void CRIAPI criAtomPlayer_Destroy(CriAtomPlayerHn player);

void CRIAPI criAtomPlayer_SetData(
	CriAtomPlayerHn player, void *buffer, CriSint32 buffer_size);
	
void CRIAPI criAtomPlayer_DeferCallback(CriAtomPlayerHn player);

void CRIAPI criAtomPlayer_Start(CriAtomPlayerHn player);

void CRIAPI criAtomPlayer_Stop(CriAtomPlayerHn player);

CriAtomPlayerStatus CRIAPI criAtomPlayer_GetStatus(CriAtomPlayerHn player);

void CRIAPI criAtomPlayer_SetVolume(CriAtomPlayerHn player, CriUint8 vol);

void CRIAPI criAtomPlayer_SetDataRequestCallback(
	CriAtomPlayerHn player, CriAtomPlayerDataRequestCbFunc func, void *obj);

CriBool CRIAPI criAtomPlayer_IsDataPlaying(
	CriAtomPlayerHn player, const void *buffer, CriSint32 buffer_size);

void CRIAPI criAtom_SetActivePlayer(CriAtomPlayerHn player);

#ifdef __cplusplus
}
#endif

#endif	

