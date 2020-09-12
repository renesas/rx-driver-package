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
* Copyright (C) 2016-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx_if.h
* Description  : Functions for using LVD on RX devices.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Added support for RX24U, RX24T(512KB).
*              :                     Deleted unnecessary header information.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin.
*              : 20.05.2019 3.00     Added support for GNUC and ICCRX.
***********************************************************************************************************************/
#ifndef LVD_INTERFACE_HEADER_FILE
#define LVD_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* This section describes return values which can be used with the LVD FIT module. */
typedef enum
{
    LVD_SUCCESS = 0,
    LVD_ERR_INVALID_PTR,
    LVD_ERR_INVALID_FUNC, 
    LVD_ERR_INVALID_DATA,
    LVD_ERR_INVALID_CHAN,
    LVD_ERR_INVALID_ARG,
    LVD_ERR_UNSUPPORTED,
    LVD_ERR_ALREADY_OPEN,
    LVD_ERR_NOT_OPENED,
    LVD_ERR_LOCO_STOPPED
} lvd_err_t;

/* This enum defines channels that can be used with the MCU. */
typedef enum
{
    LVD_CHANNEL_1 = 0,
    LVD_CHANNEL_2,
    LVD_CHANNEL_INVALID
} lvd_channel_t;

/* This enum defines voltage detection conditions and influences interrupt conditions and the LVD status. A reset     */
/* occurs when the monitored voltage is below the voltage detection level, thus it is not influenced by this enum.    */
typedef enum
{
    LVD_TRIGGER_RISE = 0,
    LVD_TRIGGER_FALL,
    LVD_TRIGGER_BOTH,
    LVD_TRIGGER_INVALID
}lvd_trigger_t;

/* This enum defines the status whether the monitored voltage is above or below the voltage detection level. */
typedef enum
{
    LVD_STATUS_POSITION_ABOVE = 0,
    LVD_STATUS_POSITION_BELOW,
    LVD_STATUS_POSITION_INVALID
}lvd_status_position_t;

/* This enum defines whether the voltage crossed the voltage detection level. */
typedef enum
{
    LVD_STATUS_CROSS_NONE = 0,
    LVD_STATUS_CROSS_OVER,
    LVD_STATUS_CROSS_INVALID
}lvd_status_cross_t;

/* This data structure defines the structure which is sent to the R_LVD_Open() function. */
typedef struct
{
    lvd_trigger_t   trigger;
}lvd_config_t;

/* This data structure defines the structure which is sent to the callback function. */
typedef struct
{
    bsp_int_src_t   vector;
} lvd_int_cb_args_t;

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/
extern lvd_err_t R_LVD_Open(lvd_channel_t channel, lvd_config_t const *p_cfg, void (*p_callback)(void *));
extern lvd_err_t R_LVD_Close(lvd_channel_t channel);
extern lvd_err_t R_LVD_GetStatus(lvd_channel_t channel, 
                                 lvd_status_position_t *p_status_position, 
                                 lvd_status_cross_t *p_status_cross);
extern lvd_err_t R_LVD_ClearStatus(lvd_channel_t channel);
extern uint32_t  R_LVD_GetVersion(void);

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* LVD_INTERFACE_HEADER_FILE */

/* End of File */

