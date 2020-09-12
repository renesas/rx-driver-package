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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
 * File Name    : r_uid_rx.h
 * Description  : This is a private header file used internally by the Unique ID Read API module.
 *              : It should not be modified or included by the user in their application.
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.12.2014 1.00    First Release
 *         : 20.05.2019 1.11    Added macro
 ******************************************************************************/

#ifndef RX_UID_API_PRIVATE_HEADER_FILE
#define RX_UID_API_PRIVATE_HEADER_FILE


/*******************************************************************************
 Macro definitions
 ******************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

typedef enum {
    UID_UNINITIALIZED = 0,  /* Uninitialized */
    UID_READY               /* Unique ID Read Executable State */
} uid_status_t;


#endif /* RX_UID_API_PRIVATE_HEADER_FILE */

/* end of file */
