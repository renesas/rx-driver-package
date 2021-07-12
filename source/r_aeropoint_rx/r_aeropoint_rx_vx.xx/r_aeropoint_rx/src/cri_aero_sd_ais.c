/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_sd_ais.c
 *
 ***************************************************************************/
#include "cri_aero_config_ais.h"
#include "cri_aero_sd_internal.h"

#if CRI_AERO_CONFIG_AIS_USE_SD

#include "r_sdc_sdmem_rx_lib.h"

CriBool CriAeroSD_Initialize_AIS(void)
{
	return r_sdc_sdmem_rx_lib_Initialize();
}

CriBool CriAeroSD_Finalize_AIS(void)
{
	return r_sdc_sdmem_rx_lib_Finalize();
}

CriBool CriAeroSd_Write_AIS(const CriUint8 *pBuf, CriUint32 Lbn, CriUint32 Cnt)
{
	return r_sdc_sdmem_rx_lib_write(pBuf, Lbn, Cnt);
}

CriBool CriAeroSd_Read_AIS(CriUint8 *pBuf, CriUint32 Lbn, CriUint32 Cnt)
{
	return r_sdc_sdmem_rx_lib_read(pBuf, Lbn, Cnt);
}

CriBool CriAeroSd_Detect_AIS(void)
{
	return r_sdc_sdmem_rx_lib_detect();
}

CriBool CriAeroSd_GetDitection_AIS(void)
{
	return r_sdc_sdmem_rx_lib_get_ditection();
}

#else

CriBool CriAeroSD_Initialize_AIS(void)
{
	return CRI_FALSE;
}

CriBool CriAeroSD_Finalize_AIS(void)
{
	return CRI_FALSE;
}

CriBool CriAeroSd_Write_AIS(const CriUint8 *pBuf, CriUint32 Lbn, CriUint32 Cnt)
{
	return CRI_FALSE;
}

CriBool CriAeroSd_Read_AIS(CriUint8 *pBuf, CriUint32 Lbn, CriUint32 Cnt)
{
	return CRI_FALSE;
}

CriBool CriAeroSd_Detect_AIS(void)
{
	return CRI_FALSE;
}

CriBool CriAeroSd_GetDitection_AIS(void)
{
	return CRI_FALSE;
}

#endif



