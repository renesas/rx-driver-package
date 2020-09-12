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
* Copyright (C) 2014-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_dac_rx_if.h
* Description  : Functions for using DAC on RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2014 2.00    New API
*           05.09.2014 2.10    Added support for RX64M.
*           20.01.2015 2.20    Added support for RX113.
*           02.03.2015 2.30    Added support for RX71M.
*           10.06.2015 2.40    Added support for RX231.
*           30.09.2015 2.50    Added support for RX23T.
*           01.10.2015 2.60    Added support for RX130.
*           01.12.2015 2.70    Added support for RX230.
*           01.02.2016 2.80    Added support for RX24T.
*           01.03.2016 2.90    Added support for RX65N.
*           31.08.2016 2.91    Added support for RX65N Amp not use.
*           19.12.2016 3.00    Added support for RX24U.
*           21.07.2017 3.10    Added support for RX65N-2M, RX130-512KB.
*           31.10.2017 3.11    Added the demo for RX65N, RX65N-2M.
*           28.09.2018 3.20    Added support for RX66T.
*           16.11.2018 3.21    Added XML document number.
*           01.02.2019 3.30    Added support for RX72T, RX65N-64pin
*           20.05.2019 4.00    Added support for GNUC and ICCRX.
*           28.06.2019 4.10    Added support for RX23W.
*           15.08.2019 4.20    Added support for RX72M.
*           25.11.2019 4.30    Added support for RX13T.
*                              Removed support for Generation 1 devices
*           30.12.2019 4.40    Added support for RX72N, RX66N.
*                              Added support for RX65N, RX72M Amplifier Stabilization Wait Control.
***********************************************************************************************************************/
#ifndef DAC_RX_IF_H
#define DAC_RX_IF_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "platform.h"
#include "r_dac_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define DAC_VERSION_MAJOR  (4)
#define DAC_VERSION_MINOR  (40)


#if defined(BSP_MCU_RX23T) || defined(BSP_MCU_RX24T) || defined(BSP_MCU_RX13T)
#define DAC_CFG_NUM_CH  (1)
#else
#define DAC_CFG_NUM_CH  (2)
#endif

/*****************************************************************************
Typedef definitions
******************************************************************************/

#if defined(BSP_MCU_RX23T) || defined(BSP_MCU_RX24T) || defined(BSP_MCU_RX13T)
typedef enum e_dac_ch           // DAC channel numbers
{
    DAC_CH0    = 0,
    DAC_NUM_CH
} dac_ch_t;
#else
typedef enum e_dac_ch           // DAC channel numbers
{
    DAC_CH0    = 0,
    DAC_CH1    = 1,
    DAC_NUM_CH = 2
} dac_ch_t;
#endif


/* DAC OPEN() PARAMETER */

typedef enum e_dac_refv         // DAC reference voltage
{
    DAC_REFV_AVCC0_AVSS0    = 1,
    DAC_REFV_INTERNAL_AVSS0 = 3,
#ifndef BSP_MCU_RX23W
    DAC_REFV_VREFH_VREFL    = 6
#endif
} dac_refv_t;

typedef enum e_dac_out_sel_da      // DAC output select
{
    DAC_OUT_DA_OFF = 0,      // Turn off output of DA channels
    DAC_OUT_SEL_DA0 = 1,     // Output to DA0 channel pin
    DAC_OUT_SEL_DA1 = 2,     // Output to DA1 channel pin
}dac_out_da;

typedef enum e_dac_out_sel_ref
{
    DAC_OUT_REF_OFF = 0,      // Turn off output of DA channels
    DAC_OUT_SEL_REF0 = 1,     // Output DA0 channel as Vref
    DAC_OUT_SEL_REF1 = 2,     // Output DA1 channel as Vref
}dac_out_ref;

#if defined(BSP_MCU_RX111) || defined(BSP_MCU_RX23T) || defined(BSP_MCU_RX24T) || defined(BSP_MCU_RX13T)
typedef struct st_dac_cfg
{
    bool        fmt_flush_right;
} dac_cfg_t;

#elif defined(BSP_MCU_RX113) || defined(BSP_MCU_RX231) || defined(BSP_MCU_RX230) || defined(BSP_MCU_RX23W)
typedef struct st_dac_cfg
{
    dac_refv_t ref_voltage;
    bool fmt_flush_right;
    bool sync_with_adc;
} dac_cfg_t;

#elif defined(BSP_MCU_RX64M) || defined(BSP_MCU_RX71M) || defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX72M) \
 || defined(BSP_MCU_RX72N) || defined(BSP_MCU_RX66N)
typedef struct st_dac_cfg
{
    bool        fmt_flush_right;
    bool        sync_with_adc;
    uint8_t     sync_unit;                      // 0 or 1
    bool        ch_conv_off_when_output_off;    // applies to both channels
} dac_cfg_t;
#elif defined(BSP_MCU_RX66T) || defined(BSP_MCU_RX72T)
typedef struct st_dac_cfg
{
    bool        fmt_flush_right;
    bool        sync_with_adc;
    bool        ch_conv_off_when_output_off;    // applies to both channels
    dac_out_da  out_sel_da;                     // 1 for output to DA0 pin, 2 for output to DA1 pin
    dac_out_ref out_sel_ref;                    // 1 for output channel 0 as Vref, 2 for output channel 1 as Vref
} dac_cfg_t;

#else //defined(BSP_MCU_RX130) || defined(BSP_MCU_RX24U)
typedef struct st_dac_cfg
{
    bool        fmt_flush_right;
    bool        sync_with_adc;
} dac_cfg_t;
#endif


/* DAC CONTROL() COMMANDS */
typedef enum e_dac_cmd
{
    DAC_CMD_OUTPUT_ON,          // Analog output of channel is enabled.
    DAC_CMD_OUTPUT_OFF,         // Analog output of channel is disabled.
#if defined(BSP_MCU_RX64_ALL) || defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX71M) || defined(BSP_MCU_RX72M) \
 || defined(BSP_MCU_RX72N) || defined(BSP_MCU_RX66N)
    DAC_CMD_AMP_ON,             // Gain of 1 amplifier. See Electrical
    DAC_CMD_AMP_OFF,            // Characteristics in HW User's Manual
#if defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX72M) || defined(BSP_MCU_RX72N) || defined(BSP_MCU_RX66N)
    DAC_CMD_ASW_ON,             // Wait for the channel 0 output buffer amplifier to become stable
                                //(the pin is Hi-Z).
    DAC_CMD_ASW_OFF,             // A wait for stabilization of the channel 0 output buffer amplifier is
                                //released (output is enabled).
#endif
#endif
    DAC_CMD_END_ENUM
} dac_cmd_t;


/* DAC API ERROR CODES */
typedef enum e_dac_err
{
    DAC_SUCCESS=0,
    DAC_ERR_BAD_CHAN,           // non-existent channel number
    DAC_ERR_INVALID_CMD,        // non-existent operation command
    DAC_ERR_INVALID_ARG,        // argument is not valid for parameter
    DAC_ERR_NULL_PTR,           // received null ptr; missing required argument
    DAC_ERR_LOCK_FAILED,        // failed to lock DAC module
    DAC_ERR_UNLOCK_FAILED,      // failed to unlock DAC module
    DAC_ERR_ADC_NOT_POWERED,    // cannot sync because ADC is not powered
    DAC_ERR_ADC_CONVERTING      // cannot sync because ADC is converting
} dac_err_t;


/*****************************************************************************
Public Functions
******************************************************************************/
dac_err_t R_DAC_Open(dac_cfg_t *p_config);
                    
dac_err_t R_DAC_Write(uint8_t const chan, uint16_t data);

dac_err_t R_DAC_Control(uint8_t const chan, dac_cmd_t const cmd);

dac_err_t R_DAC_Close(void);

uint32_t  R_DAC_GetVersion(void);

                                    
#endif /* DAC_RX_IF_H */

