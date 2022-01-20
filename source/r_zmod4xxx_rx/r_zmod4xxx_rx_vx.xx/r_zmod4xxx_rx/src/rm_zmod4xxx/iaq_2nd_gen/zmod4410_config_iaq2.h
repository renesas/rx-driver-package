/***********************************************************************************************************************
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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/**
 * @file zmod4410_config_iaq2.h
 * @brief This is the configuration for zmod4410 module - iaq_2nd_gen library
 * @version 2.2.0
 * @author Renesas Electronics Corporation
 */

#ifndef _ZMOD4410_CONFIG_IAQ_2ND_GEN_H
#define _ZMOD4410_CONFIG_IAQ_2ND_GEN_H

#include <stdio.h>
#include "../zmod4xxx_types.h"

#define ZMOD4410_H_ADDR           0x40
#define ZMOD4410_D_ADDR           0x50
#define ZMOD4410_M_ADDR           0x60
#define ZMOD4410_S_ADDR           0x68

#define ZMOD4410_PROD_DATA_LEN    7

static uint8_t data_set_4410i[] =
{
    0x00, 0x50,
    0x00, 0x28,0xC3,  0xE3,
    0x00, 0x00,0x80,  0x40
};

static uint8_t data_set_4410_iaq_2nd_gen[] =
{
    0x00, 0x50, 0xFF, 0x38,
    0xFE, 0xD4, 0xFE, 0x70,
    0xFE, 0x0C, 0xFD, 0xA8,
    0xFD, 0x44, 0xFC, 0xE0,
    0x00, 0x52, 0x02, 0x67,
    0x00, 0xCD, 0x03, 0x34,
    0x23, 0x03, 0xA3, 0x43,
    0x00, 0x00, 0x06, 0x49,
    0x06, 0x4A, 0x06, 0x4B,
    0x06, 0x4C, 0x06, 0x4D,
    0x06, 0x4E, 0x06, 0x97,
    0x06, 0xD7, 0x06, 0x57,
    0x06, 0x4E, 0x06, 0x4D,
    0x06, 0x4C, 0x06, 0x4B,
    0x06, 0x4A, 0x86, 0x59
};

zmod4xxx_conf g_zmod4410_iaq_2nd_gen_sensor_type[] =
{
    {
        .start = 0x80,
        .h     = {.addr = ZMOD4410_H_ADDR, .len = 2,  .data_buf = &data_set_4410i[0]            },
        .d     = {.addr = ZMOD4410_D_ADDR, .len = 2,  .data_buf = &data_set_4410i[2]            },
        .m     = {.addr = ZMOD4410_M_ADDR, .len = 2,  .data_buf = &data_set_4410i[4]            },
        .s     = {.addr = ZMOD4410_S_ADDR, .len = 4,  .data_buf = &data_set_4410i[6]            },
        .r     = {.addr = 0x97,            .len = 4},
    },

    {
        .start         = 0x80,
        .h             = {.addr = ZMOD4410_H_ADDR, .len = 16, .data_buf = &data_set_4410_iaq_2nd_gen[0]},
        .d             = {.addr = ZMOD4410_D_ADDR, .len = 8, .data_buf = &data_set_4410_iaq_2nd_gen[16]},
        .m             = {.addr = ZMOD4410_M_ADDR, .len = 4, .data_buf = &data_set_4410_iaq_2nd_gen[24]},
        .s             = {.addr = ZMOD4410_S_ADDR, .len = 32, .data_buf = &data_set_4410_iaq_2nd_gen[28]},
        .r             = {.addr = 0x97,    .len = 32},
        .prod_data_len = ZMOD4410_PROD_DATA_LEN,
    },
};

#endif                                 // _ZMOD4410_CONFIG_IAQ_2ND_GEN_H
