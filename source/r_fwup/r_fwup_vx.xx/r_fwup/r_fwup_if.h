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
* Copyright (C) 2023-2024 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_fwup_if.h
* Description  : Functions for using Firmware update.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.07.2023 2.00    First Release
*         : 29.09.2023 2.01    Fixed log messages.
*                              Add parameter checking.
*                              Added arguments to R_FWUP_WriteImageProgram API.
*         : 29.03.2024 2.02    Update wrapper functions.
*         : 09.04.2024 2.03    Fixed wrapper function.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "r_fwup_config.h"

#ifndef R_FWUP_IF_H
#define R_FWUP_IF_H

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Version Number of API. */
#define FWUP_VERSION_MAJOR                  (2)
#define FWUP_VERSION_MINOR                  (03)

/* for FWUP_CFG_FUNCTION_MODE */
#define FWUP_FUNC_BOOTLOADER                (0)
#define FWUP_FUNC_USER_PGM                  (1)

/* for FWUP_CFG_UPDATE_MODE */
#define FWUP_DUAL_BANK                      (0)
#define FWUP_SINGLE_BANK_W_BUFFER           (1)
#define FWUP_SINGLE_BANK_WO_BUFFER          (2)
#define FWUP_SINGLE_BANK_W_BUFFER_EXT       (3)

/* __far */
#if defined(__RX)
  #define FWUP_FAR
  #define FWUP_FAR_FUNC
#else
  #if (__ICCRL78__)
    #define FWUP_FAR                __far
    #define FWUP_FAR_FUNC           __far_func
  #else
    #define FWUP_FAR                __far
    #define FWUP_FAR_FUNC           __far
  #endif /* defined(__ICCRL78__) */
#endif /* defined(__RX) */

#define CH_FAR                      char FWUP_FAR
#define C_CH_FAR                    const char FWUP_FAR
#define C_U8_FAR                    const uint8_t FWUP_FAR
#define S_C_CH_FAR                  static C_CH_FAR
#define S_C_U8_FAR                  static C_U8_FAR

/* Stdlib*/
#if defined(__RX)
  #define STRCMP(s1, s2)                  ( strcmp((s1), (s2)) )
  #if !defined(STRSTR)
    #define STRSTR(s1, s2)                ( strstr((s1), (s2)) )
  #endif
  #define STRLEN(s)                       ( strlen((s)) )
  #define MEMCMP(s1, s2, n)               ( memcmp((s1), (s2), (n)) )
  #define MEMCPY(s1, s2, n)               ( memcpy((s1), (s2), (n)) )
#else
  #if defined(__ICCRL78__)
    #define STRCMP(s1, s2)                ( strcmp((s1), (s2)) )
    #if !defined(STRSTR)
      #define STRSTR(s1, s2)              ( strstr((s1), (s2)) )
    #endif
    #define STRLEN(s)                     ( strlen((s)) )
    #define MEMCMP(s1, s2, n)             ( memcmp((s1), (s2), (n)) )
    #define MEMCPY(s1, s2, n)             ( memcpy((s1), (s2), (n)) )
  #elif defined (__CCRL__)
    #define STRCMP(s1, s2)                ( _COM_strcmp_ff((s1), (s2)) )
    #if !defined(STRSTR)
      #define STRSTR(s1, s2)              ( _COM_strstr_ff((s1), (s2)) )
    #endif
    #define STRLEN(s)                     ( _COM_strlen_f((s)) )
    #define MEMCMP(s1, s2, n)             ( _COM_memcmp_ff((s1), (s2), (n)) )
    #define MEMCPY(s1, s2, n)             ( _COM_memcpy_ff((s1), (s2), (n)) )
  #elif defined (__llvm__)
    #define STRCMP(s1, s2)                ( _COM_strcmp_ff((s1), (s2)) )
    #if !defined(STRSTR)
      #define STRSTR(s1, s2)              ( _COM_strstr_ff((s1), (s2)) )
    #endif
    #define STRLEN(s)                     ( _COM_strlen_f((s)) )
    #define MEMCMP(s1, s2, n)             ( _COM_memcmp_ff((s1), (s2), (n)) )
    #define MEMCPY(s1, s2, n)             ( _COM_memcpy_ff((s1), (s2), (n)) )
  #endif /* defined(__ICCRL78__) */
#endif /* defined(__RX) */

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
typedef enum fwup_err
{
    FWUP_SUCCESS = 0,           // Normally terminated.
    FWUP_PROGRESS,              // Illegal terminated.
    FWUP_ERR_FLASH,             // Detect error of r_flash module.
    FWUP_ERR_VERIFY,            // Detect error of r_flash module.
    FWUP_ERR_FAILURE            // Illegal terminated.
} e_fwup_err_t;

typedef enum fwup_area
{
    FWUP_AREA_MAIN = 0,
    FWUP_AREA_BUFFER,
    FWUP_AREA_DATA_FLASH
} e_fwup_area_t;

typedef enum e_fwup_delay_units
{
    FWUP_DELAY_MICROSECS = 0,
    FWUP_DELAY_MILLISECS,
    FWUP_DELAY_SECS
} e_fwup_delay_units_t;

/**********************************************************************************************************************
Public Functions
**********************************************************************************************************************/
e_fwup_err_t R_FWUP_Open (void);
void         R_FWUP_Close (void);
bool         R_FWUP_IsExistImage (e_fwup_area_t area);
e_fwup_err_t R_FWUP_EraseArea (e_fwup_area_t area);
uint32_t     R_FWUP_GetImageSize (void);
e_fwup_err_t R_FWUP_WriteImageHeader (e_fwup_area_t area,
                                      uint8_t FWUP_FAR *p_sig_type,
                                      uint8_t FWUP_FAR *p_sig,
                                      uint32_t sig_size);
e_fwup_err_t R_FWUP_WriteImageProgram (e_fwup_area_t area, uint8_t *p_buf, uint32_t offset, uint32_t buf_size);
e_fwup_err_t R_FWUP_WriteImage (e_fwup_area_t area, uint8_t *p_buf, uint32_t buf_size);
e_fwup_err_t R_FWUP_VerifyImage (e_fwup_area_t area);
e_fwup_err_t R_FWUP_ActivateImage (void);
void         R_FWUP_ExecImage (void);
void         R_FWUP_SoftwareReset (void);
uint32_t     R_FWUP_SoftwareDelay (uint32_t delay, e_fwup_delay_units_t units);
uint32_t     R_FWUP_GetVersion (void);

#endif /* R_FWUP_IF_H */

