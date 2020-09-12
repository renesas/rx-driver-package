/**********************************************************************************************************************
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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_ptp_light_rx_config.h
* Description  : Configure the PTP light driver
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 30.10.2015 1.00    Modified from the PTP driver full version
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.11    Corrected the internal operation of getting version function
*		  : 31.07.2019 1.12    Added support for GNUC and ICCRX.
*                              Changed MINT interrupt handler operation.
*		  : 31.08.2019 1.13    Supported RX72M device.
*                              Added Bypass setting.
**********************************************************************************************************************/

#ifndef _PTP_LIGHT_CONFIG_H
#define _PTP_LIGHT_CONFIG_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* ==== PTP light driver mode ==== */
#define PTPL_CFG_MODE_CH0    (0x01ul)    /* enable EPTPC CH0 */
#define PTPL_CFG_MODE_CH1    (0x02ul)    /* enable EPTPC CH1 */
#define PTPL_CFG_MODE_POLL   (0x10ul)    /* EPTPC status check is software polling */
#define PTPL_CFG_MODE_HWINT  (0x20ul)    /* EPTPC status check is hardware interrupt */

//#define PTPL_CFG_MODE (PTPL_CFG_MODE_CH0 | PTPL_CFG_MODE_CH1 | PTPL_CFG_MODE_POLL)
#define PTPL_CFG_MODE (PTPL_CFG_MODE_CH0 | PTPL_CFG_MODE_CH1 | PTPL_CFG_MODE_HWINT)

/* ==== PTP interrupt level definition ==== */
#define PTPL_CFG_INTERRUPT_LEVEL   (2)

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* _PTP_LIGHT_CONFIG_H */

