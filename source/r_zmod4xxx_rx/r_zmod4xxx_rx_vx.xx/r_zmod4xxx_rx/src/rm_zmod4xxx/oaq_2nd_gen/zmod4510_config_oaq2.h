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
 * @file    zmod4510_config_oaq2.h
 * @brief   This is the configuration for ZMOD4510 module - oaq_2nd_gen library
 * @version 3.0.0
 * @author Renesas Electronics Corporation
 */

#ifndef _ZMOD4510_CONFIG_OAQ_2ND_GEN_H_
#define _ZMOD4510_CONFIG_OAQ_2ND_GEN_H_

#include <stdio.h>
#include "../zmod4xxx_types.h"

#define ZMOD4510_PROD_DATA_LEN    10

#define ZMOD4XXX_H_ADDR           0x40
#define ZMOD4XXX_D_ADDR           0x50
#define ZMOD4XXX_M_ADDR           0x60
#define ZMOD4XXX_S_ADDR           0x68

static uint8_t data_set_4510_init[] =
{
    0x00, 0x50,
    0x00, 0x28,0xC3,  0xE3,
    0x00, 0x00,0x80,  0x40
};

static uint8_t data_set_4510_oaq_2nd_gen[] =
{
    0x00, 0x50, 0xFE, 0x70,
    0x00, 0x10,
    0x23, 0x03,
    0x00, 0x00, 0x06, 0x41,
    0x06, 0x41, 0x06, 0x41,
    0x06, 0x41, 0x06, 0x41,
    0x06, 0x41, 0x06, 0x41,
    0x86, 0x41
};

zmod4xxx_conf g_zmod4510_oaq_2nd_gen_sensor_type[] =
{
    {
        .start = 0x80,
        .h     = {.addr = ZMOD4XXX_H_ADDR, .len = 2,  .data_buf = &data_set_4510_init[0]       },
        .d     = {.addr = ZMOD4XXX_D_ADDR, .len = 2,  .data_buf = &data_set_4510_init[2]       },
        .m     = {.addr = ZMOD4XXX_M_ADDR, .len = 2,  .data_buf = &data_set_4510_init[4]       },
        .s     = {.addr = ZMOD4XXX_S_ADDR, .len = 4,  .data_buf = &data_set_4510_init[6]       },
        .r     = {.addr = 0x97,            .len = 4},
    },

    {
        .start         = 0x80,
        .h             = {.addr = ZMOD4XXX_H_ADDR, .len = 4, .data_buf = &data_set_4510_oaq_2nd_gen[0]},
        .d             = {.addr = ZMOD4XXX_D_ADDR, .len = 2, .data_buf = &data_set_4510_oaq_2nd_gen[4]},
        .m             = {.addr = ZMOD4XXX_M_ADDR, .len = 2, .data_buf = &data_set_4510_oaq_2nd_gen[6]},
        .s             = {.addr = ZMOD4XXX_S_ADDR, .len = 18, .data_buf = &data_set_4510_oaq_2nd_gen[8]},
        .r             = {.addr = 0x97,    .len = 18},
        .prod_data_len = ZMOD4510_PROD_DATA_LEN,
    },
};

#endif                                 // _ZMOD4510_CONFIG_OAQ_2ND_GEN_H_
