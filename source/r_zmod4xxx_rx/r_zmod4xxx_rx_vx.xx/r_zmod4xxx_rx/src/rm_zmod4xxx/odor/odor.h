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
 * @file    odor.h
 * @author  Renesas
 * @version 2.1.2
 * @brief   This file contains the definitions for data structure and the odor
 *          algorithm function.
 * @details This file contains the function definitions for the odor algorithm.
 */

#ifndef ODOR_H_
 #define ODOR_H_

 #include <math.h>
 #include <stdint.h>
 #include "../zmod4xxx_types.h"

 #ifdef __cplusplus
extern "C" {
 #endif

/**
 * @brief Status of the sensor.
 */

 #define ODOR_OK               (0)     /**< Everything ok. */
 #define ODOR_STABILIZATION    (1)     /**< Sensor not stabilized. */

/**
 * @brief Parameters to control the odor algorithm.
 */
typedef struct
{
    float    alpha;                    /**< Slope parameter for odor algorithm. */
    uint32_t stop_delay;               /**< Control signal follow-up time. */
    float    threshold;                /**< Threshold to switch, i.e. 1.3 - corresponds to 30 %
                                        * rise in concentration. */
    uint32_t tau;                      /**< Time constant for averaging. */
    uint32_t stabilization_samples;    /**< Ignore number of samples for sensor
                                        * stabilization. */
} odor_params;

/**
 * @brief Control signal states.
 */
typedef enum
{
    OFF = 0,
    ON  = 1,
} control_signal_state_t;

/**
 * @brief odor results.
 */
typedef struct
{
    control_signal_state_t cs_state;   /**< Control signal input. */
    float conc_ratio;                  /**< Concentration ratio. */
} odor_results_t;

/**
 * @brief   Calculates odor from r_mox and odor parameters.
 * @param   [in] r_mox MOx resistance.
 * @param   [in] params odor algorithm parameters.
 * @param   [in,out] results odor results.
 * @return  Status of the sensor.
 * @retval  0 Sensor stabilized.
 * @retval  1 Sensor not stabilized.
 */
int8_t calc_odor(float r_mox, odor_params * params, odor_results_t * results);

 #ifdef __cplusplus
}
 #endif

#endif                                 /* ODOR_H_ */
