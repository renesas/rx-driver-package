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
* Copyright (C) 2014-2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : base64_decode.h
* Description  : BASE64 decode function
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.09.2014 1.00    First Release
*         : 01.06.2015 1.01    Added RX231.
*         : 15.05.2017 1.02    Clean up source file
***********************************************************************************************************************/

#ifndef BASE64_DECODE_H_
#define BASE64_DECODE_H_

#include "platform.h"

#if defined(__CCRX__)
#define BASE64_FAR
#define STRCHR          ( strchr )
 #if !defined(STRSTR)
 #define STRSTR          ( strstr )
 #endif
#elif defined(__GNUC__)
#define BASE64_FAR
#define STRCHR          ( strchr )
 #if !defined(STRSTR)
 #define STRSTR          ( strstr )
 #endif
#elif defined(__ICCRX__)
#define BASE64_FAR
#define STRCHR          ( strchr )
 #if !defined(STRSTR)
 #define STRSTR          ( strstr )
 #endif
#elif defined(__CCRL__)
#define BASE64_FAR      __far
#define STRCHR          ( _COM_strchr_f )
 #if !defined(STRSTR)
 #define STRSTR          ( _COM_strstr_ff )
 #endif
#elif defined(__ICCRL78__)
#define BASE64_FAR      __far
#define STRCHR          ( strchr )
 #if !defined(STRSTR)
 #define STRSTR          ( strstr )
 #endif
#endif

uint32_t base64_decode(uint8_t BASE64_FAR *source, uint8_t BASE64_FAR *result, uint32_t size);

#endif /* BASE64_DECODE_H_ */
