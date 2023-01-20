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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.06.2015 1.00     First Release
*              : 31.08.2015 1.01     Macro definitions change from "LVDBT" to "VBTLVDI".
*                                    Because it was changed by latest version of "iodefine.h".
*              : 01.02.2019 1.02     Changed Minor version to 1.02.
*              : 20.05.2019 1.03     Supported the following compilers:
*                                    - GCC for Renesas RX
*                                    - IAR C/C++ Compiler for Renesas RX
*                                    Updated version to 1.03
*              : 30.06.2019 1.04     Added support for RX23W
*                                    Updated version to 1.04
*              : 10.06.2020 1.05     Updated version to 1.05
*              : 31.03.2021 2.00     Added support for RX671
*              : 13.09.2021 2.10     Added the demo for RX671
*              : 29.07.2022 2.20     Updated demo projects
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx_if.h
* Description  : Functions for using Battery Backup Function on RX devices. 
*******************************************************************************/
#ifndef R_VBATT_RX_IF_H
#define R_VBATT_RX_IF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"

R_BSP_PRAGMA_UNPACK


/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Version Number of API. */
#define VBATT_VERSION_MAJOR           (2)
#define VBATT_VERSION_MINOR           (20)


/* VBATT function grouping */
#if defined(BSP_MCU_RX230) || defined(BSP_MCU_RX231) || defined(BSP_MCU_RX23W)
#define GROUP_VBATT
#endif
#if defined(BSP_MCU_RX671)
#define GROUP_VBATTB
#endif


#ifdef GROUP_VBATT
/* VBATT Pin Voltage Drop Detection Level */
#define VBATT_DTCT_LEVEL_2_20_V       (0U)  /* VBATT pin voltage drop detection level is 2.20V      */
#define VBATT_DTCT_LEVEL_2_00_V       (1U)  /* VBATT pin voltage drop detection level is 2.00V      */

/* VBATT pin voltage drop detection function */
#define VBATT_DTCT_DISABLE            (0U)  /* VBATT pin voltage drop detection disabled
                                               VBATT pin voltage drop detection interrupt disabled  */
#define VBATT_DTCT_ENABLE_INT_DISABLE (1U)  /* VBATT pin voltage drop detection enabled
                                               VBATT pin voltage drop detection interrupt disabled  */
#define VBATT_DTCT_ENABLE_NMI_ENABLE  (2U)  /* VBATT pin voltage drop detection enabled
                                               VBATT pin voltage drop detection interrupt enabled
                                               VBATT pin voltage drop detection interrupt enable type
                                               is non-maskable interrupt                            */
#define VBATT_DTCT_ENABLE_INT_ENABLE  (3U)  /* VBATT pin voltage drop detection enabled
                                               VBATT pin voltage drop detection interrupt enabled
                                               VBATT pin voltage drop detection interrupt enable type
                                               is maskable interrupt                                */
#endif
#ifdef GROUP_VBATTB
#define VBATT_TAMPER_CH0  ((uint8_t)(0x00))  /* Mask for tamper channel 0 */
#define VBATT_TAMPER_CH1  ((uint8_t)(0x01))  /* Mask for tamper channel 1 */
#define VBATT_TAMPER_CH2  ((uint8_t)(0x02))  /* Mask for tamper channel 2 */

#define VBATT_TAMPER_DISABLE              ((uint8_t)(0x00)) /* The TAMPIn/RTCICn signal input is disabled. */
#define VBATT_TAMPER_ENABLE               ((uint8_t)(0x01)) /* The TAMPIn/RTCICn signal input is enabled. */

#define VBATT_TAMPER_DETECT_INT_DISABLE    ((uint8_t)(0x00)) /* Tamper detection interrupt is disabled. */
#define VBATT_TAMPER_DETECT_INT_ENABLE     ((uint8_t)(0x01)) /* Tamper detection interrupt is disabled. */

#define VBATT_TAMPER_ERASE_DISABLE         ((uint8_t)(0x00)) /* Backup registers are not erased in response to a
                                                         tamper  event. */
#define VBATT_TAMPER_ERASE_ENABLE          ((uint8_t)(0x01)) /* Backup registers are erased in response to a tamper
                                                         event. */

#define VBATT_TAMPER_TCE_RTCIC_PIN            ((uint8_t)(0x00)) /* Input signal from the RTCIC pin. */
#define VBATT_TAMPER_TCE_TAMPER_EVENT         ((uint8_t)(0x01)) /* Tamper event. */

#define VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE   ((uint8_t)(0x00)) /* Noise filter for the TAMPIn/RTCICn pin is disabled. */
#define VBATT_TAMPER_CHEN_NOISE_FILTER_ENABLE    ((uint8_t)(0x01)) /* Noise filter for the TAMPIn/RTCICn pin is enabled. */

#define VBATT_TAMPER_CHEN_FALLING_EDGE           ((uint8_t)(0x00)) /* A falling edge of the input on the TAMPIn pin. */
#define VBATT_TAMPER_CHEN_RISING_EDGE            ((uint8_t)(0x01)) /* A rising edge of the input on the TAMPIn pin. */

#define VBATT_TAMPER_CHEN_INPUT_DISABLE          ((uint8_t)(0x00)) /* The RTCICn signal input is disabled. */
#define VBATT_TAMPER_CHEN_INPUT_ENABLE           ((uint8_t)(0x01)) /* The RTCICn signal input is enabled. */
#endif

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/* Battery Backup Function API error codes */
typedef enum
{
    VBATT_SUCCESS = 0U,                     /* Successful operation                                 */
    VBATT_ERR_LOCK_FUNC,                    /* Another task is handling API function                */
    VBATT_ERR_LVD0_SETTING,                 /* Setting error LVD0 of OFS1                           */
    VBATT_ERR_INVALID_ARG,                  /* Parameter error                                      */
    VBATT_ERR_FUNC_INVALID,                 /* Invalid calling function                             */
    VBATT_ERR_OTHER                         /* Other error                                          */
} vbatt_return_t;

/* Macro definitions to be set in the argument of the callback function */
typedef enum
{
    VBATT_NONE = 0U,
    VBATT_NOT_DROP_VOLTAGE,                 /* Battery backup power voltage drop not detected       */
    VBATT_DROP_VOLTAGE,                     /* Battery backup power voltage drop detected           */
    VBATT_MASKABLE_INTERRUPT,               /* Under a Maskable interrupt                           */
    VBATT_NON_MASKABLE_INTERRUPT,           /* Under a Non-maskable interrupt                       */
    VBATT_TAMPER_CH0_INTERRUPT,             /* Under a Tamper Channel 0 Maskable interrupt      */
    VBATT_TAMPER_CH1_INTERRUPT,             /* Under a Tamper Channel 1 Maskable interrupt      */
    VBATT_TAMPER_CH2_INTERRUPT              /* Under a Tamper Channel 2 Maskable interrupt      */
} vbatt_cb_evt_t;

/*----------------------------------------------------------------------------*/
/*   Define Battery Backup Function information structure type.               */
/*----------------------------------------------------------------------------*/
/*----- Callback function type. -----*/
typedef void (*vbatt_callback_t)(vbatt_cb_evt_t * vbatt_cb_event);  /* Callback function type   */

/*----- Structure type. -----*/
/* Battery Backup Function information structure */
typedef volatile struct
{
    vbatt_callback_t    callbackfunc;       /* Callback function                                    */
} vbatt_info_t;

/* VBATT Pin Voltage Drop Detection Function information structure for set again */
typedef volatile struct
{
#ifdef GROUP_VBATT
    uint8_t         rsv2;                   /* reserve                                              */
    uint8_t         rsv1;                   /* reserve                                              */
    uint8_t         vbatt_int_priority;     /* Interrupt priority level of 
                                               VBATT Pin Voltage Drop Detection maskable interrupt  */
    union
    {
        uint8_t     byte;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_4
        (
            uint8_t rsv:3,                  /* reserve                                              */
            uint8_t lvd_level:2,            /* VBATT Pin Voltage Drop Detection Level               */
            uint8_t lvd_detect:2,           /* VBATT Pin Voltage Drop Detection Function            */
            uint8_t func:1                  /* Battery Backup Function                              */
        ) bit;
    } vbatt_ctrl;
#endif
#ifdef GROUP_VBATTB

    uint8_t tamper_channel;                   /* Tamper channel is enabled. */

    /* VBATT_TAMPER_INT_ENABLE
    VBATT_TAMPER_INT_DISABLE */
    uint8_t tamper_detection_interrupt;     /* TAMPCR
                                            0: Tamper n detection interrupt is disabled.
                                            1: Tamper n detection interrupt is enabled. */
    /* VBATT_TAMPER_ERASE_ENABLE
       VBATT_TAMPER_ERASE_DISABLE */
    uint8_t tamper_erase;                   /* TAMPCR
                                            0: Backup registers are not erased in response to a tamper n event.
                                            1: Backup registers are erased in response to a tamper n event. */
    /* VBATT_TAMPER_TCE_RTCIC_PIN
       VBATT_TAMPER_TCE_TEMPER_EVENT */
    uint8_t time_capture_source;            /* TCECR
                                            0: Input signal from the RTCICn pin
                                            1: Tamper n event */
    /* VBATT_TAMPER_CHEN_INPUT_DISABLE
       VBATT_TAMPER_CHEN_INPUT_ENABLE */
    uint8_t channel_input;                  /* TAMPICR1
                                            0: The RTCICn signal input is disabled.
                                            1: The RTCICn signal input is enabled. */
    /* VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE
       VBATT_TAMPER_CHEN_NOISE_FILTER_ENABLE */
    uint8_t channel_noise_filter;           /* TAMPICR2
                                            0: Noise filter for the RTCICn pin is disabled.
                                            1: Noise filter for the RTCICn pin is enabled. */
    /* VBATT_TAMPER_CHEN_FALLING_EDGE
       VBATT_TAMPER_CHEN_RISING_EDGE */
    uint8_t channel_trigger_select;         /* TAMPICR2
                                            0: A falling edge of the input on the RTCICn pin.
                                            1: A rising edge of the input on the RTCICn pin. */

    uint8_t tamper_int_priority;               /* interrupt priority; 1=low, 15=high */
#endif /* #ifdef GROUP_VBATTB */
} vbatt_ctrl_info_t;

/* Battery Backup Function status information structure */
typedef volatile struct
{
#ifdef GROUP_VBATT
    union
    {
        uint8_t     byte;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_3
        (
            uint8_t rsv:6,                  /* reserve                                              */
            uint8_t vbatt_mon:1,            /* VBATT Pin Voltage Monitor Flag                       */
            uint8_t rsv1:1                   /* reserve     */
        ) bit;
    } vbatt_status;
#endif
#ifdef GROUP_VBATTB
    uint8_t tamper_channel;  /* Tamper channel need get status. */
    uint8_t tamper_detection_flag;   /* Tamper Detection Flag */
    uint8_t tamper_level_monitoring_flag;   /* Channel Level Monitoring Flag */
    bool  action_clear; /* Action clear status register */
#endif
} vbatt_status_t;



/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
/* Battery Backup Function Driver API functions */
vbatt_return_t      R_VBATT_Open(vbatt_info_t * p_vbatt_info);
vbatt_return_t      R_VBATT_Control(vbatt_ctrl_info_t * p_vbatt_ctrl_info);
vbatt_return_t      R_VBATT_GetStatus(vbatt_status_t * p_vbatt_status);
uint32_t            R_VBATT_GetVersion(void);
#ifdef GROUP_VBATTB
vbatt_return_t R_VBATT_ReadBackupData(uint8_t index, uint8_t * p_data);
vbatt_return_t R_VBATT_WriteBackupData(uint8_t index, uint8_t * p_data);
#endif

R_BSP_PRAGMA_PACKOPTION

#endif /* R_VBATT_RX_IF_H */

/* End of File */

