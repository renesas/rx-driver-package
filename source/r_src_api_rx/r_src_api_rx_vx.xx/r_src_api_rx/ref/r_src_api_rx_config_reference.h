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
/******************************************************************************
* File Name    : r_src_api_rx_config_reference.h
* Version      : 1.11
* Device       : RX64M, RX71M
* Description  : SRC programming for the RX family. This file has options to let
*                the user customize parts of the SRC API.
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 11.07.2014 1.00    First Release
*         : 05.09.2014 1.10    Second Release, no code modification on this file.
*         : 11.12.2014 1.11    RX71M support added.
*
******************************************************************************/

#ifndef SRC_API_CONFIG_H_
#define SRC_API_CONFIG_H_

/**************************************/
/*  Configuration to be set by users  */
/**************************************/

/* ---------------------------------------------------------- */
/* 
   Input data empty interrupt configuration (for SRCIDCTRL.IEN)
   'Disable interrupt     (0)
   'Enable interrupt      (1)
*/
#define SRC_IEN         (0)  /* select a value among alternatives above */

/* ---------------------------------------------------------- */
/* 
   Output data empty interrupt configuration (for SRCODCTRL.OEN)
   'Disable interrupt     (0)
   'Enable interrupt      (1)
*/
#define SRC_OEN         (0)   /* select a value among alternatives above */

/* ---------------------------------------------------------- */
/* 
   Channel swap of output data (for SRCODCTRL.OCH)
   'same order of input data     (0) 
   'reverse order of input data  (1) 
*/
#define SRC_OCH         (0)  /* select a value among alternatives above */

/* ---------------------------------------------------------- */
/* 
    Configure condition when IINT flag to be set (for SRCIDCTRL.IFTRG)
    'Set flag when number of data in FIFO is 0 or less    (0)
    'Set flag when number of data in FIFO is 2 or less    (1)
    'Set flag when number of data in FIFO is 4 or less    (2)
    'Set flag when number of data in FIFO is 6 or less    (3)
*/
#define SRC_IFTG        (3)  /* select a value among alternatives above */

/* ---------------------------------------------------------- */
/* 
    Configure condition when OINT frag to be set (for SRCODCTRL.OFTRG)
    'Set flag when number of data in FIFO is 1 or larger    (0)
    'Set flag when number of data in FIFO is 4 or larger    (1)
    'Set flag when number of data in FIFO is 8 or larger    (2)
    'Set flag when number of data in FIFO is 12 or larger   (3)
*/
#define SRC_OFTG        (3)  /* select a value among alternatives above */

/* ============================================================== */
/* ============================================================== */

#endif /* SRC_API_CONFIG_H_ */

