/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : r_flash_spi_lib.c
 *
 ***************************************************************************/

#include "cri_aero_config_ais.h"

#if CRI_AERO_CONFIG_AIS_USE_FLASH

#include "r_flash_spi_lib.h"
#include "r_sys_time_rx_if.h"
#include "r_flash_spi_if.h"


#define SERIAL_FLASH_64KB_SIZE      (0x00010000)



static flash_spi_info_t Flash_Info_W;
static flash_spi_info_t Flash_Info_R;
static flash_spi_erase_info_t Flash_Info_E;
static uint8_t  gDevNo = FLASH_SPI_DEV0;
static uint8_t  gStat;


bool r_flash_spi_lib_Initialize()
{
	if (SYS_TIME_SUCCESS != R_SYS_TIME_RegisterPeriodicCallback(R_FLASH_SPI_1ms_Interval, 1)) { return false; };
	if (FLASH_SPI_SUCCESS != R_FLASH_SPI_Open(gDevNo)) { return false; };
	if (FLASH_SPI_SUCCESS != R_FLASH_SPI_Set_4byte_Address_Mode(gDevNo)) { return false; };
	if (FLASH_SPI_SUCCESS != R_FLASH_SPI_Quad_Enable(gDevNo)) { return false; };
	if (FLASH_SPI_SUCCESS != R_FLASH_SPI_Read_Status(gDevNo, &gStat)) { return false; }; 
	if (FLASH_SPI_SUCCESS != R_FLASH_SPI_Set_Write_Protect(gDevNo, 0)) { return false; };
	if (FLASH_SPI_SUCCESS != R_FLASH_SPI_Read_Status(gDevNo, &gStat)) { return false; };  
	return true;
}

void r_flash_spi_lib_Finalize()
{
}

bool r_flash_spi_lib_write_256(const uint8_t *memory_addr, uint32_t flash_addr)
{
	if(flash_addr%SERIAL_FLASH_64KB_SIZE==0)
	{
		Flash_Info_E.addr = flash_addr;
		Flash_Info_E.mode = FLASH_SPI_MODE_B64K_ERASE;
		if (R_FLASH_SPI_Erase(gDevNo, &Flash_Info_E) == FLASH_SPI_SUCCESS)
		{
		}
		else
		{
			return(false);
		}
		for(;;)
		{
			if (R_FLASH_SPI_Polling(gDevNo, FLASH_SPI_MODE_ERASE_POLL) == FLASH_SPI_SUCCESS)
			{
				break;
			}
		}
	}

	{
		Flash_Info_W.addr = flash_addr;
		Flash_Info_W.cnt = 256;
		Flash_Info_W.p_data = (uint8_t *)memory_addr;
		Flash_Info_W.op_mode = FLASH_SPI_QUAD;
		flash_spi_status_t ret=R_FLASH_SPI_Write_Data_Page(gDevNo, &Flash_Info_W);

		if (ret == FLASH_SPI_SUCCESS)
		{
		}
		else
		{
			return false;
		}
		for(;;)
		{
			flash_spi_status_t ret;
			ret = R_FLASH_SPI_Polling(gDevNo, FLASH_SPI_MODE_PROG_POLL);
			if (ret == FLASH_SPI_SUCCESS_BUSY)
			{
			}
			else if (ret == FLASH_SPI_SUCCESS)
			{
				return true;
			}
			else {
				return false;
			}
		}
	}
}

bool r_flash_spi_lib_read(uint8_t *memory_addr, uint32_t flash_addr, uint32_t cnt_byte)
{
	Flash_Info_R.addr = flash_addr;
	Flash_Info_R.cnt = cnt_byte;
	Flash_Info_R.p_data = memory_addr;
	Flash_Info_R.op_mode = FLASH_SPI_QUAD;

	if (R_FLASH_SPI_Read_Data(gDevNo, &Flash_Info_R) == FLASH_SPI_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif
