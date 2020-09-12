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
*              : 20.05.2019 1.03     Added macro
*******************************************************************************/
/*******************************************************************************
* File Name    : r_vbatt_rx_private.h
* Description  : Functions for using Battery Backup Function on RX devices. 
*******************************************************************************/
#ifndef R_VBATT_RX_PRIVATE_H
#define R_VBATT_RX_PRIVATE_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_vbatt_rx_if.h"
#include "r_vbatt_rx_config.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif
/*----------------------------------------------------------------------------*/
/*  Parameter check of Configuration Options                                  */
/*----------------------------------------------------------------------------*/
#if ((0 != VBATT_CFG_PARAM_CHECKING_ENABLE) && (1 != VBATT_CFG_PARAM_CHECKING_ENABLE))
    #error "ERROR - VBATT_CFG_PARAM_CHECKING_ENABLE - Parameter error in configures file."
#endif

#if ((0 > VBATT_CFG_DETECT_FUNCTION) || (3 < VBATT_CFG_DETECT_FUNCTION))
    #error "ERROR - VBATT_CFG_DETECT_FUNCTION - Parameter error in configures file."
#endif

#if ((1 > VBATT_CFG_INT_PRIORITY) || (15 < VBATT_CFG_INT_PRIORITY))
    #error "ERROR - VBATT_CFG_INT_PRIORITY - Parameter error in configures file."
#endif

/*----------------------------------------------------------------------------*/
/*  Define                                                                    */
/*----------------------------------------------------------------------------*/
/* Define bool type */
#define VBATT_TRUE                      ((bool)(0x01))      /* True                                 */
#define VBATT_FALSE                     ((bool)(0x00))      /* False                                */

/* Define enable/disable */
#define VBATT_ENABLE                    ((uint8_t)(1))      /* Enable                               */
#define VBATT_DISABLE                   ((uint8_t)(0))      /* Disable                              */

/* Define battery backup power voltage drop detection */
#define VBATT_VOLTAGE_DROP_DETECTED     ((uint8_t)(1))  /* Battery backup power voltage drop detected  */
#define VBATT_VOLTAGE_DROP_NOT_DETECTED ((uint8_t)(0))  /* Battery backup power voltage drop not detected */


/*******************************************************************************
Typedef definitions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*  Define Battery Backup Function information structure type.                */
/*----------------------------------------------------------------------------*/
/* API Mode. */
typedef enum
{
    VBATT_MODE_NONE = 0U,                   /* Non-communication mode                               */
    VBATT_MODE_OPEN,                        /* Open mode                                            */
    VBATT_MODE_CONTROL,                     /* Control mode                                         */
    VBATT_MODE_GETSTATUS                    /* Get status mode                                      */
} vbatt_api_mode_t;


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*  Interrupt Function                                                        */
/*----------------------------------------------------------------------------*/
extern void r_vbatt_isr_processing(void);
extern void r_vbatt_nmi_isr_processing(void);


#endif /* R_VBATT_RX_PRIVATE_H */

/* End of File */

