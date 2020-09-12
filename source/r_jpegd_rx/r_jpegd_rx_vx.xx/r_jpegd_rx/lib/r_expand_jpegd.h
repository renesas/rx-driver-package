/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2006-2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name     : r_expand_jpegd.h
* Version       :
* Device(s)     :
* Tool-Chain    :
* H/W Platform  :
* Description   : The header file of JPEG File Expand Library
*******************************************************************************/

#ifndef __r_expand_jpegd_h__
#define __r_expand_jpegd_h__

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_stdint.h"
#include "r_mw_version.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Return Value for library */
#define EXPAND_JPEGD_OK					(0)
#define EXPAND_JPEGD_ERROR_HEADER		(-1)
#define EXPAND_JPEGD_ERROR_DECODE		(-2)
#define EXPAND_JPEGD_NOT_SUPPORT		(-3)
#define EXPAND_JPEGD_ERROR_RST			(-4)
#define EXPAND_JPEGD_ERROR_SOI			(-5)
#define EXPAND_JPEGD_ERROR_EOI			(-6)


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern const mw_version_t R_expand_jpegd_version;

void R_init_jpeg(void);
int16_t R_expand_jpeg(uint8_t *input, int32_t fsize, uint16_t *outptr, int32_t offset);
int16_t R_get_info_jpeg(uint8_t *input, int32_t fsize, uint16_t *w, uint16_t *h);

#endif /* __r_expand_jpegd_h__ */
