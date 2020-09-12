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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
 * File Name    : r_uid_rx_if.h
 * Description  : This module provides the interface file to be included by the user
 *              : when using the Unique ID Read API.
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.12.2014 1.00    First Release
 *         : 01.12.2017 1.10    Added support for RX130
 *         : 20.05.2019 1.11    Updated version to 1.11
 *         : 31.07.2019 1.13    Added support for RX13T
 ******************************************************************************/

#ifndef UID_INTERFACE_HEADER_FILE
#define UID_INTERFACE_HEADER_FILE

/*******************************************************************************
 Macro definitions
 ******************************************************************************/
/* Version Number of API. */
#define UID_RX_VERSION_MAJOR           (1)
#define UID_RX_VERSION_MINOR           (13)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/* Flash API error codes */
typedef enum _uid_err {
    UID_SUCCESS = 0,           /* Operation success.  */
    UID_ERR_UNINITIALIZED,     /* Uninitialized.      */
    UID_ERR_LOCK_FUNC,         /* Function Locking.   */
    UID_ERR_FAILURE,           /* Operation failure.  */
} uid_err_t;

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
uid_err_t R_UID_Open(void);
uid_err_t R_UID_Read(uint8_t *);
uint32_t R_UID_GetVersion(void);

#endif /* UID_INTERFACE_HEADER_FILE */

/* end of file */
