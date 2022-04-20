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
 * @file    iaq_2nd_gen_ulp.h
 * @author  Ronald Schreiber
 * @version x.y.z
 * @brief   This file contains the data structure definitions and
 *          the function definitions for the 2nd Gen IAQ ULP algorithm.
 * @details The library contains an algorithm to calculate an eCO2, EtOH, TVOC and
 *          IAQ index from ZMOD4410 measurements.
 *          The implementation is made to allow more than one sensor.
 *
 */

#ifndef IAQ_2ND_GEN_ULP_H_
 #define IAQ_2ND_GEN_ULP_H_

 #ifdef __cplusplus
extern "C" {
 #endif

 #include <stdint.h>
 #include <math.h>
 #include "../zmod4xxx_types.h"

/** \addtogroup RetCodes Return codes of the algorithm functions.
 *  @{
 */
 #define IAQ_2ND_GEN_ULP_OK               (0) /**< everything okay */
 #define IAQ_2ND_GEN_ULP_STABILIZATION    (1) /**< sensor in stabilization */
/** @}*/

/**
 * @brief   The algorithm handle contains variables that describe the sensor
 *          or the algorithm state. It needs to be passed with between
 *          initialization and after each algorithm calculation.
 */
typedef struct
{
    float log_nonlog_rcda[3];          /**< various baselines. */
    uint8_t
            stabilization_sample;      /**< Number of remaining stabilization samples. */
    uint8_t need_filter_init;
    float   tvoc_smooth;
    float   tvoc_deltafilter;
    float   acchw;
    float   accow;
    float   etoh;
    float   eco2;
} iaq_2nd_gen_ulp_handle_t;

/**
 * @brief Variables that receive the algorithm outputs.
 */
typedef struct
{
    float rmox[13];                    /**< MOx resistance. */
    float log_nonlog_rcda[3];          /**< various baselines. */
    float iaq;                         /**< IAQ index. */
    float tvoc;                        /**< TVOC concentration (mg/m^3). */
    float etoh;                        /**< EtOH concentration (ppm). */
    float eco2;                        /**< eCO2 concentration (ppm). */
} iaq_2nd_gen_ulp_results_t;

/**
 * @brief   calculates IAQ results from present sample.
 * @param   [in] handle Pointer to algorithm state variable.
 * @param   [in] dev Pointer to the device.
 * @param   [in] sensor_results_table Array of 32 bytes with the values from the sensor results table.
 * @param   [in] humidity_pct relative ambient humidity (%)
 * @param   [in] temperature_degc ambient temperature (degC)
 * @param   [out] results Pointer for storing the algorithm results.
 * @return  error code.
 */
int8_t calc_iaq_2nd_gen_ulp(iaq_2nd_gen_ulp_handle_t  * handle,
                            zmod4xxx_dev_t            * dev,
                            const uint8_t             * sensor_results_table,
                            const float                 humidity_pct,
                            const float                 temperature_degc,
                            iaq_2nd_gen_ulp_results_t * results);

/**
 * @brief   Initializes the IAQ algorithm.
 * @param   [out] handle Pointer to algorithm state variable.
 * @return  error code.
 */
int8_t init_iaq_2nd_gen_ulp(iaq_2nd_gen_ulp_handle_t * handle);

 #ifdef __cplusplus
}
 #endif

#endif                                 /* IAQ_2ND_GEN_ULP_H_ */
