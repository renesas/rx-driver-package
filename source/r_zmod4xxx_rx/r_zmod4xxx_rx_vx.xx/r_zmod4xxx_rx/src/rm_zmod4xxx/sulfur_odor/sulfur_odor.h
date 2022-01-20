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
 * @file    sulfur_odor.h
 * @author  Ronald Schreiber
 * @version 2.1.2
 * @brief   This file contains the data structure definitions and
 *          the function definitions for the sulfor odor algorithm.
 * @details The library contains an algorithm to calculate and rate a
 *          sulfur odor.
 *          The implementation is made to allow more than one sensor.
 *
 */

#ifndef SULFUR_ODOR_H_
 #define SULFUR_ODOR_H_

 #ifdef __cplusplus
extern "C" {
 #endif

 #include <stdint.h>
 #include <math.h>
 #include "../zmod4xxx_types.h"

/**
 * @brief Return codes of the sulfur odor algorithm functions
 */
 #define SULFUR_ODOR_OK              (0)   /**< everything okay */
 #define SULFUR_ODOR_WARMUP          (1)   /**< sensor in stabilization */
 #define SULFUR_ODOR_WRONG_DEVICE    (-32) /**< wrong sensor type */

/**
 * @brief Number of MOX and CDA resistances to store
 */
 #define SULFUR_ODOR_N_RMOX          (9)

/**
 * @brief Odor classifications.
 */
typedef enum
{
    SULFUR_ODOR_ACCEPTABLE = 0,
    SULFUR_ODOR_SULFUR     = 1,
} sulfur_odor_classification_t;

/**
 * @brief Variables that describe the sensor or the algorithm state.
 * This is for internal use only! Do not change values here!
 */
typedef struct
{
    uint8_t stabilization_counter;
    uint8_t class_buffer;
    float   log_rcda[SULFUR_ODOR_N_RMOX];
    sulfur_odor_classification_t odor;
} sulfur_odor_handle_t;

/**
 * @brief Variables that receive the algorithm outputs.
 */
typedef struct
{
    float rmox[SULFUR_ODOR_N_RMOX];    /**< MOx resistance (ohm). */
    float intensity;                   /**< odor intensity rating ranges from 0.0 to 5.0 */
    sulfur_odor_classification_t odor; /**< odor classification */
} sulfur_odor_results_t;

/**
 * @brief   Initializes the algorithm.
 * @param   [out] handle Pointer to algorithm state variable.
 * @param   [in]  dev Pointer to device.
 * @return  error code.
 */
int8_t init_sulfur_odor(sulfur_odor_handle_t * handle, zmod4xxx_dev_t * dev);

/**
 * @brief   calculates results from present sample.
 * @param   [in] handle Pointer to algorithm state variable.
 * @param   [in] dev Pointer to the device.
 * @param   [in] sensor_results_table pointer to array of 32 bytes with the values from the sensor results table.
 * @param   [out] results Pointer for storing the algorithm results.
 * @return  error code.
 */
int8_t calc_sulfur_odor(sulfur_odor_handle_t  * handle,
                        zmod4xxx_dev_t        * dev,
                        const uint8_t         * sensor_results_table,
                        sulfur_odor_results_t * results);

 #ifdef __cplusplus
}
 #endif

#endif                                 /* SULFUR_ODOR_H_ */
