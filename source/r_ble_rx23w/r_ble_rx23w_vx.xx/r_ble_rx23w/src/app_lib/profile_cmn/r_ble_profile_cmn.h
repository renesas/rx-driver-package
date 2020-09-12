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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#ifndef R_BLE_PROFILE_CMN_H
#define R_BLE_PROFILE_CMN_H

#ifdef ENABLE_PROFILE_UT_TEST
#define UT_MOCK(name) __mock_ ## name
#else
#define UT_MOCK(name) name
#endif

/*******************************************************************************************************************//**
 * @file
 * @defgroup profile_cmn Profile Common Library
 * @{
 * @ingroup app_lib
 * @brief Profile Common Library
 * @details This library provides APIs to encode/decode default type and data types.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
*         : 31.10.2019 1.01    Add doxygen comments.
***********************************************************************************************************************/

/** @defgroup profile_cmn_struct Structures
 *  @{
 *  @brief Structure definition
 */
/*******************************************************************************************************************//**
 * @brief IEEE 11073 FLOAT type.
***********************************************************************************************************************/
typedef struct {
    int8_t  exponent; /**< 8-bit exponent to base 10 */
    int32_t mantissa; /**< 24-bit mantissa */
} st_ble_ieee11073_float_t;

/*******************************************************************************************************************//**
 * @brief IEEE 11073 short FLOAT type.
***********************************************************************************************************************/
typedef struct {
    int8_t  exponent; /**< 4-bit exponent to base 10 */
    int16_t mantissa; /**< 12-bit mantissa */
} st_ble_ieee11073_sfloat_t;

/*******************************************************************************************************************//**
 * @brief Date Time characteristic parameters.
***********************************************************************************************************************/
typedef struct {
    uint16_t year;    /**< Year */
    uint8_t  month;   /**< Month */
    uint8_t  day;     /**< Day */
    uint8_t  hours;   /**< Hours */
    uint8_t  minutes; /**< Minutes */
    uint8_t  seconds; /**< Seconds */
} st_ble_date_time_t;
/*@}*/

#endif /* R_BLE_PROFILE_CMN_H */
/*@}*/