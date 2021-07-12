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
#ifndef R_SDC_SDMEM_RX_PIN_H
#define R_SDC_SDMEM_RX_PIN_H

#include "r_sdc_sdmem_rx_pin_config.h"
#include "r_sdc_sd_rx_if.h"

typedef enum
{
    SDC_SD_DELAY_MICROSECS = 1000000,  
    SDC_SD_DELAY_MILLISECS = 1000,     
    SDC_SD_DELAY_SECS = 1              
} sdc_sd_delay_units_t;

sdc_sd_status_t r_sdc_sdmem_power_Initialize(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_power_on(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_power_off(uint32_t card_no);
bool            r_sdc_sdmem_softwaredelay(uint32_t delay, sdc_sd_delay_units_t units);

#endif 

