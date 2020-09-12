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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
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
#define VBATT_VERSION_MAJOR           (1)
#define VBATT_VERSION_MINOR           (04)

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
    VBATT_NON_MASKABLE_INTERRUPT            /* Under a Non-maskable interrupt                       */
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
} vbatt_ctrl_info_t;

/* Battery Backup Function status information structure */
typedef volatile struct
{
    union
    {
        uint8_t     byte;
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_3
        (
            uint8_t rsv:6,                  /* reserve                                              */
            uint8_t vbatt_mon:1,            /* VBATT Pin Voltage Monitor Flag                       */
            uint8_t pwr_drp_dtct:1          /* Battery Backup Power Voltage Drop Detection Flag     */
        ) bit;
    } vbatt_status;
} vbatt_status_t;


/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
/* Battery Backup Function Driver API functions */
vbatt_return_t      R_VBATT_Open(vbatt_info_t * p_vbatt_info);
vbatt_return_t      R_VBATT_Control(vbatt_ctrl_info_t * p_vbatt_ctrl_info);
vbatt_return_t      R_VBATT_GetStatus(vbatt_status_t * p_vbatt_status);
uint32_t            R_VBATT_GetVersion(void);


R_BSP_PRAGMA_PACKOPTION

#endif /* R_VBATT_RX_IF_H */

/* End of File */

