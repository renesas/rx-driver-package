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

/*******************************************************************************************************************//**
 * @addtogroup RM_ZMOD4XXX
 * @{
 **********************************************************************************************************************/

#ifndef RM_ZMOD4XXX_H
#define RM_ZMOD4XXX_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "rm_zmod4xxx_api.h"

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#include "r_zmod4xxx_rx_config.h"
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#include "r_zmod4xxx_rl_config.h"
#else
 #include "rm_zmod4xxx_lib_cfg.h"
 #include "rm_zmod4xxx_cfg.h"
#endif

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#else

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER
#endif

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Definitions of Buffer Size */
#define RM_ZMOD4XXX_MAX_I2C_BUF_SIZE        (64) // Maximum I2C buffer size

/* Definitions of delay time */
#if (RM_ZMOD4XXX_CFG_OPERATION_MODE == 1)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (0)
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 2)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (5475) // See Table 3 in the ZMOD4410 Programming Manual.
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 3)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (1990) // See Table 4 in the ZMOD4410 Programming Manual.
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 4)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (0)
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 5)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (1990) // See Table 6 in the ZMOD4410 Programming Manual.
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 6)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (0)
#elif (RM_ZMOD4XXX_CFG_OPERATION_MODE == 7)
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (1990) // See Table 4 in the ZMOD4510 Programming Manual.
#else
 #define RM_ZMOD4XXX_ALG_REQ_DELAY_IN_MS    (0)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ZMOD4XXX initialization process block */
typedef struct st_rm_zmod4xxx_init_process_params
{
    volatile uint32_t            delay_ms;               ///< Delay milliseconds.
    volatile bool                communication_finished; ///< Communication flag for blocking.
    volatile bool                measurement_finished;   ///< IRQ flag
    volatile rm_zmod4xxx_event_t event;                  ///< Callback event
} rm_zmod4xxx_init_process_params_t;

typedef struct st_rm_zmod4xxx_status_params
{
    uint8_t       value;
    volatile bool flag;
} rm_zmod4xxx_status_params_t;

/** ZMOD4XXX control block */
typedef struct st_rm_zmod4xxx_instance_ctrl
{
    uint32_t open;                                                   ///< Open flag
    uint8_t  buf[RM_ZMOD4XXX_MAX_I2C_BUF_SIZE];                      ///< Buffer for I2C communications
    uint8_t  register_address;                                       ///< Register address to access
    rm_zmod4xxx_status_params_t       status;                        ///< Status parameter
    volatile rm_zmod4xxx_event_t      event;                         ///< Callback event
    rm_zmod4xxx_init_process_params_t init_process_params;           ///< For the initialization process.
    rm_zmod4xxx_cfg_t const         * p_cfg;                         ///< Pointer of configuration block
    rm_comms_instance_t const       * p_comms_i2c_instance;          ///< Pointer of I2C Communications Middleware instance structure
    void const * p_timer_instance;                                   ///< Pointer to Timer driver instance.
    void const * p_irq_instance;                                     ///< Pointer to IRQ instance.
    void       * p_zmod4xxx_device;                                  ///< Pointer to ZMOD4XXX device structure.
    void       * p_zmod4xxx_handle;                                  ///< Pointer to ZMOD4XXX library handler.
    void       * p_zmod4xxx_results;                                 ///< Pointer to ZMOD4XXX library results.
    float        temperature;                                        ///< Temperature for OAQ 2nd
    float        humidity;                                           ///< Humidity for OAQ 2nd
    void const * p_context;                                          ///< Pointer to the user-provided context

    /* Pointer to callback and optional working memory */
    void (* p_comms_callback)(rm_zmod4xxx_callback_args_t * p_args); ///< I2C Communications callback
    void (* p_irq_callback)(rm_zmod4xxx_callback_args_t * p_args);   ///< IRQ callback
} rm_zmod4xxx_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern rm_zmod4xxx_api_t const g_zmod4xxx_on_zmod4xxx;

/** @endcond */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t RM_ZMOD4XXX_Open(rm_zmod4xxx_ctrl_t * const p_api_ctrl, rm_zmod4xxx_cfg_t const * const p_cfg);
fsp_err_t RM_ZMOD4XXX_MeasurementStart(rm_zmod4xxx_ctrl_t * const p_api_ctrl);
fsp_err_t RM_ZMOD4XXX_MeasurementStop(rm_zmod4xxx_ctrl_t * const p_api_ctrl);
fsp_err_t RM_ZMOD4XXX_StatusCheck(rm_zmod4xxx_ctrl_t * const p_api_ctrl);
fsp_err_t RM_ZMOD4XXX_Read(rm_zmod4xxx_ctrl_t * const p_api_ctrl, rm_zmod4xxx_raw_data_t * const p_raw_data);
fsp_err_t RM_ZMOD4XXX_TemperatureAndHumiditySet(rm_zmod4xxx_ctrl_t * const p_api_ctrl, float temperature,
                                                float humidity);
fsp_err_t RM_ZMOD4XXX_Iaq1stGenDataCalculate(rm_zmod4xxx_ctrl_t * const         p_api_ctrl,
                                             rm_zmod4xxx_raw_data_t * const     p_raw_data,
                                             rm_zmod4xxx_iaq_1st_data_t * const p_zmod4xxx_data);
fsp_err_t RM_ZMOD4XXX_Iaq2ndGenDataCalculate(rm_zmod4xxx_ctrl_t * const         p_api_ctrl,
                                             rm_zmod4xxx_raw_data_t * const     p_raw_data,
                                             rm_zmod4xxx_iaq_2nd_data_t * const p_zmod4xxx_data);
fsp_err_t RM_ZMOD4XXX_OdorDataCalculate(rm_zmod4xxx_ctrl_t * const      p_api_ctrl,
                                        rm_zmod4xxx_raw_data_t * const  p_raw_data,
                                        rm_zmod4xxx_odor_data_t * const p_zmod4xxx_data);
fsp_err_t RM_ZMOD4XXX_SulfurOdorDataCalculate(rm_zmod4xxx_ctrl_t * const             p_api_ctrl,
                                              rm_zmod4xxx_raw_data_t * const         p_raw_data,
                                              rm_zmod4xxx_sulfur_odor_data_t * const p_zmod4xxx_data);
fsp_err_t RM_ZMOD4XXX_Oaq1stGenDataCalculate(rm_zmod4xxx_ctrl_t * const         p_api_ctrl,
                                             rm_zmod4xxx_raw_data_t * const     p_raw_data,
                                             rm_zmod4xxx_oaq_1st_data_t * const p_zmod4xxx_data);
fsp_err_t RM_ZMOD4XXX_Oaq2ndGenDataCalculate(rm_zmod4xxx_ctrl_t * const         p_api_ctrl,
                                             rm_zmod4xxx_raw_data_t * const     p_raw_data,
                                             rm_zmod4xxx_oaq_2nd_data_t * const p_zmod4xxx_data);
fsp_err_t RM_ZMOD4XXX_Close(rm_zmod4xxx_ctrl_t * const p_api_ctrl);
void      rm_zmod4xxx_comms_i2c_callback(rm_comms_callback_args_t * p_args);

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#else
void rm_zmod4xxx_timer_callback(timer_callback_args_t * p_args);
void rm_zmod4xxx_irq_callback(external_irq_callback_args_t * p_args);

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_FOOTER
#endif

#endif                                 /* RM_ZMOD4XXX_H_*/

/*******************************************************************************************************************//**
 * @} (end addtogroup RM_ZMOD4XXX)
 **********************************************************************************************************************/
