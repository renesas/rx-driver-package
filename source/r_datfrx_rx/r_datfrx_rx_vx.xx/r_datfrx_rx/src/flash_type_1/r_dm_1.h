/************************************************************************************************
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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_dm_1.h
* Version      : 2.00
* Description  : DATFRX interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_datfrx_rx_config.h"               /* DATFRX Configuration definitions    */
#include "r_flash_rx_if.h"                    /* FLASH module                        */
#include "r_flash_dm_rx_if.h"                 /* DATFRX I/F definitions              */
#include "r_flash_dm_private.h"               /* DATFRX Private module definitions   */

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_DM_1_H
#define R_DM_1_H

#if(FLASH_TYPE == FLASH_TYPE_1)

#include "r_dispatch_1.h"
#define FLASH_DM_BUSY_WAIT      (400)           /* Unit : ms */

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global variables
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

#endif /* R_DM_1_H */

/* End of File */
