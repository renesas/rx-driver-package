/**********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
#include "cri_aero_config_ais.h"

#if CRI_AERO_CONFIG_AIS_USE_SD

#include "platform.h"
#include "r_sdc_sd_rx_if.h"
#include "r_sdc_sd_rx_config.h"
#include "r_sdc_sdmem_rx_pin_config.h"
#include "r_sdc_sdmem_rx_pin.h"
#include "r_gpio_rx_if.h" 

#define SDC_SD_OVERHEAD_LOOP_COUNT     (4)
#define SDC_SD_CPU_CYCLES_PER_LOOP     (4)



sdc_sd_status_t r_sdc_sdmem_power_Initialize(uint32_t card_no)
{
    if (SDC_SD_CARD_NO0 == card_no)
    {



#if (CRI_AERO_PIN_USE_SDHI_POWER == 1)
    	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_CMD_ASSIGN_TO_GPIO);
    	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_CMD_IN_PULL_UP_DISABLE);

   	#if (CRI_AERO_PIN_ACTIVE_SDHI_POWER == 1)
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_LEVEL_LOW);
	#else
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_LEVEL_HIGH);
	#endif
		R_GPIO_PinDirectionSet(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_DIRECTION_OUTPUT);
#endif




    }
    else
    {
    }

    return SDC_SD_SUCCESS;
} 

sdc_sd_status_t r_sdc_sdmem_power_on(uint32_t card_no)
{


#if (CRI_AERO_PIN_USE_SDHI_POWER == 1)
	if (SDC_SD_CARD_NO0 == card_no)
	{

#if (CRI_AERO_PIN_ACTIVE_SDHI_POWER == 1)
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_LEVEL_HIGH);
#else
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_LEVEL_LOW);
#endif

	}
	else
	{
		nop();
	}

	if (true != r_sdc_sdmem_softwaredelay(SDC_SD_CFG_POWER_ON_WAIT,	SDC_SD_DELAY_MILLISECS)) {
		return SDC_SD_ERR;
	}
#endif

    return SDC_SD_SUCCESS;
} 

sdc_sd_status_t r_sdc_sdmem_power_off(uint32_t card_no)
{


#if (CRI_AERO_PIN_USE_SDHI_POWER == 1)
	if (SDC_SD_CARD_NO0 == card_no)
	{

#if (CRI_AERO_PIN_ACTIVE_SDHI_POWER == 1)
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_LEVEL_LOW);
#else
		R_GPIO_PinWrite(CRI_AERO_PIN_CONFIG_SDHI_POWER, GPIO_LEVEL_HIGH);
#endif

	}
	else
	{
		nop();
	}

	if (true != r_sdc_sdmem_softwaredelay(SDC_SD_CFG_POWER_ON_WAIT,	SDC_SD_DELAY_MILLISECS)) {
		return SDC_SD_ERR;
	}
#endif






    return SDC_SD_SUCCESS;
} 

#pragma inline_asm r_sdc_sdmem_delaywait

#if !defined(__VS__)
static void r_sdc_sdmem_delaywait (unsigned long loop_cnt)
{
  R_BSP_ASM_INTERNAL_USED(loop_cnt)
  R_BSP_ASM_BEGIN
  R_BSP_ASM(    BRA.B   R_BSP_ASM_LAB_NEXT(0)     )
  R_BSP_ASM(    NOP                               )
  R_BSP_ASM_LAB(0:                                )
  R_BSP_ASM(    NOP                               )
  R_BSP_ASM(    SUB     #01H, R1                  )
  R_BSP_ASM(    BNE.B   R_BSP_ASM_LAB_PREV(0)     )
  R_BSP_ASM_END
} 
#endif

bool r_sdc_sdmem_softwaredelay(uint32_t delay, sdc_sd_delay_units_t units)
{
    uint64_t loop_cnt;

    loop_cnt = (((uint64_t)delay * (uint64_t)BSP_ICLK_HZ) / (SDC_SD_CPU_CYCLES_PER_LOOP * units))
                - SDC_SD_OVERHEAD_LOOP_COUNT;

    if ((loop_cnt > 0xFFFFFFFF) || (loop_cnt == 0) ||
        ((units != SDC_SD_DELAY_MICROSECS) && (units != SDC_SD_DELAY_MILLISECS) && (units != SDC_SD_DELAY_SECS)))
    {
        return (false);
    }
    r_sdc_sdmem_delaywait((uint32_t)loop_cnt);
    return (true);
} 

#endif

