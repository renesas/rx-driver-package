/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : r_sdc_sdmem_rx_lib.h
 *
 ***************************************************************************/

#ifndef R_SDC_SDMEM_RX_LIB_H
#define R_SDC_SDMEM_RX_LIB_H

#include "platform.h"


bool r_sdc_sdmem_rx_lib_Initialize();

bool r_sdc_sdmem_rx_lib_Finalize();

bool r_sdc_sdmem_rx_lib_write(const uint8_t *p_buff, uint32_t lbn, uint32_t cnt);

bool r_sdc_sdmem_rx_lib_read(uint8_t *p_buff, uint32_t lbn, uint32_t cnt);

bool r_sdc_sdmem_rx_lib_detect();

bool r_sdc_sdmem_rx_lib_get_ditection();


#endif
