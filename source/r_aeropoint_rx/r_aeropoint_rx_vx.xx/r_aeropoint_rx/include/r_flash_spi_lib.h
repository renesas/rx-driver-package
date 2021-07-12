/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : r_flash_spi_lib.h
 *
 ***************************************************************************/
#ifndef R_FLASH_SPI_LIB_H
#define R_FLASH_SPI_LIB_H


#include "platform.h"


void r_flash_spi_lib_test();

bool r_flash_spi_lib_Initialize();

void r_flash_spi_lib_Finalize();

bool r_flash_spi_lib_write_256(const uint8_t *memory_addr, uint32_t flash_addr);


bool r_flash_spi_lib_read(uint8_t *memory_addr, uint32_t flash_addr, uint32_t cnt_byte);

#endif 