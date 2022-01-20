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
 * @file    oaq_2nd_gen.h
 * @author  Ronald Schreiber
 * @version 3.0.0
 * @brief   This file contains the data structure definitions and
 *          the function definitions for the 2nd generation OAQ algorithm.
 * @details The library contains an algorithm to calculate an ozone
 *          concentration and various air quality index values
 *          from the ZMOD4510 measurements.
 */

#ifndef OAQ_2ND_GEN_H_
 #define OAQ_2ND_GEN_H_

 #ifdef __cplusplus
extern "C" {
 #endif

 #include <stdint.h>
 #include <math.h>
 #include "../zmod4xxx_types.h"

/**
 * @brief Return codes of the algorithm functions.
 */
 #define OAQ_2ND_GEN_OK               (0) /**< everything okay */
 #define OAQ_2ND_GEN_STABILIZATION    (1) /**< sensor in stabilization */

/**
 * @brief Variables that describe the sensor or the algorithm state.
 */
typedef struct
{
    uint16_t
          stabilization_sample;        /**< Number of samples still needed for stabilization. */
    float gcda[8];                     /**< baseline conductances. */
    float log_ra;
    float log_b;
    float beta2;
    float O3_conc_ppb;
    float o3_1h_ppb;
    float o3_8h_ppb;
} oaq_2nd_gen_handle_t;

/**
 * @brief Variables that receive the algorithm outputs.
 */
typedef struct
{
    float rmox[8];                     /**< MOx resistance. */
    float O3_conc_ppb;                 /**< O3_conc_ppb stands for the ozone concentration in part-per-billion */
    uint16_t
        FAST_AQI;                      /**< FAST_AQI stands for a 1-minute average of the Air Quality Index according to the EPA standard based on ozone */
    uint16_t
        EPA_AQI;                       /**< EPA_AQI stands for the Air Quality Index according to the EPA standard based on ozone. */
} oaq_2nd_gen_results_t;

/**
 * @brief   Initializes the OAQ algorithm.
 * @param   [out] handle Pointer to algorithm state variable.
 * @param   [in] dev pointer to the device
 * @return  error code.
 */
int8_t init_oaq_2nd_gen(oaq_2nd_gen_handle_t * handle, zmod4xxx_dev_t * dev);

/**
 * @brief   calculates OAQ results from present sample.
 * @param   [in] handle Pointer to algorithm state variable.
 * @param   [in] dev pointer to the device
 * @param   [in] sensor_results_table array of 32 bytes with the values from the sensor results table.
 * @param   [in] humidity_pct relative ambient humidity (%)
 * @param   [in] temperature_degc ambient temperature (degC)
 * @param   [out] results Pointer for storing the algorithm results.
 * @return  error code.
 */
int8_t calc_oaq_2nd_gen(oaq_2nd_gen_handle_t  * handle,
                        zmod4xxx_dev_t        * dev,
                        const uint8_t         * sensor_results_table,
                        const float             humidity_pct,
                        const float             temperature_degc,
                        oaq_2nd_gen_results_t * results);

 #ifdef __cplusplus
}
 #endif

#endif                                 /* OAQ_2ND_GEN_H_ */
