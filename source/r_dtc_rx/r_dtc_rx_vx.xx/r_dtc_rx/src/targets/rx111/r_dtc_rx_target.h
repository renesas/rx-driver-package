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
* Copyright (C) 2014(2015-2016) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dtc_rx_target.h
* Description  : Functions for using DTC on RX111 devices. 
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2014 1.00    Initial revision
*         : 17.07.2014 2.00    Second  revision
*         : 12.11.2014 2.01    Added RX113.
*         : 30.01.2015 2.02    Added RX71M.
*         : 30.09.2016 2.05    Changed "DTC_RX_TSRGET" to "DTC_RX_TARGET".
*         :                    Added DTC IP Version.
*******************************************************************************/
#ifndef DTC_RX_TARGET_H
#define DTC_RX_TARGET_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* The number of activation sources */
#define DTC_NUM_INTERRUPT_SRC       (46)
/* Size of DTC Vector table (in byte units) */
#define DTC_VECTOR_TABLE_SIZE_BYTES (0x3E8 + 0x400)
/* DTC IP Version */
#define DTC_IP                      (DTC_IP_VER_DTCa)

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/


#endif /* DTC_RX_TARGET_H */

/* End of File */

