/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_flash_ais.c
 *
 ***************************************************************************/


#include "cri_aero_config_ais.h"
#include "cri_aero_flash_internal.h"

#if CRI_AERO_CONFIG_AIS_USE_FLASH
#include "r_flash_spi_lib.h"

CriBool CriAeroFlash_Initialize_AIS(void)
{
	if (r_flash_spi_lib_Initialize() == true)
	{
		return CRI_TRUE;
	}
	else {
		return CRI_FALSE;
	}
}

void CriAeroFlash_Finalize_AIS(void)
{
	r_flash_spi_lib_Finalize();
}

CriBool CriAeroFlash_Write256_AIS(const CriUint8 *pMemoryAddr, CriUint32 FlashAddr)
{
	return r_flash_spi_lib_write_256(pMemoryAddr, FlashAddr);
}

CriBool CriAeroFlash_ReadByte_AIS(CriUint8 *pMemoryAddr, CriUint32 FlashAddr, CriUint32 CntByte)
{
	return r_flash_spi_lib_read(pMemoryAddr, FlashAddr, CntByte);
}

#else

CriBool CriAeroFlash_Initialize_AIS(void)
{
	return CRI_FALSE;
}

void CriAeroFlash_Finalize_AIS(void)
{
}

CriBool CriAeroFlash_Write256_AIS(const CriUint8 *pMemoryAddr, CriUint32 FlashAddr)
{
	return CRI_FALSE;
}

CriBool CriAeroFlash_ReadByte_AIS(CriUint8 *pMemoryAddr, CriUint32 FlashAddr, CriUint32 CntByte)
{
	return CRI_FALSE;
}

#endif
