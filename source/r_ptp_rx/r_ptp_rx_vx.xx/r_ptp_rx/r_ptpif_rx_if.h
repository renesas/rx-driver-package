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
* File Name    : r_ptpif_rx_if.h
* Description  : Interface file for PTP (PTPEDMAC part) API for RX
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.12    Corrected the internal operation of getting version functions
*                              Corrected wait operation to information retention of GETINFOR
*                              Corrected the array index deviation of ptp_tmr_isr.
*         : 31.03.2017 1.13    Corrected BC and P2P TC setting.
*                              Modified listening state operations of R_PTP_Init, R_PTP_Start and R_PTP_Stop functions.
*                              Added registering user function to MINT interrupt handler.
*                              Changed MINT interrupt handler operation.
*                              Changed TC&OC combined opeartion to TC only operation.
*                              Added my clockIdentity setting function.
*                              Added domainnumber field updating function.
*                              Added announce message fields updating function.
*                              Added and modified transmission interval setting function.
*                              Added offsetFromMaster updating wait function when device state is P2P slave.
*                              Added PTP port state setting function.
*         : 30.04.2017 1.14    Corrected underflow operation of R_PTP_GetSyncInfo function.
*                              Corrected logMessageInterval out of range judgment.
*		  : 31.07.2019 1.15    Added support for GNUC and ICCRX.
*                              Added "WAIT_LOOP" comments for every loop operation.
*		  : 31.08.2019 1.16    Supported RX72M device.
*                              Added Bypass setting.
**********************************************************************************************************************/

#ifndef _PTPIF_RX_H
#define _PTPIF_RX_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Used to get which MCU is currently being used. */
#include "platform.h"
#include "r_ptp_rx_config.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Version Number of API. */
#define RX_PTPIF_VERSION_MAJOR           (1)
#define RX_PTPIF_VERSION_MINOR           (16)

/* PTP driver (PTP Host interface part) return value */
typedef enum
{
	PTPIF_ERR_PARAM = -6,    /* Parameter error */
	PTPIF_ERR_LEN = -5,      /* Data length error */
    PTPIF_ERR_TACT = -4,     /* No remaining transmit descriptor */
    PTPIF_ERR_NO_DATA = -3,  /* No data received */
    PTPIF_ERR_NOT_TRAN = -2, /* Not transfer enabled */
    PTPIF_ERR = -1,          /* General error */
	PTPIF_OK = 0
} ptpif_return_t;

/* PTPEDMAC interrupt event */
typedef enum
{
	PTPIF_FUNC_READ = 0, /* Frame reception interrupt (FR) */
	PTPIF_FUNC_WRITE,    /* Frame transmission interrupt (TC) */
	PTPIF_FUNC_ERR,      /* Error interrupt (MACE, RFOF, RDE, TFUF, TDE, ADE and RFCOF) */
} PTPIF_INTEVT;

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* PTPEDMAC interrupt handler type definition */ 
typedef void (*PTPIF_HNDLR)(uint8_t, uint32_t); /* Frame transmission and reception, and error handler */

/**********************************************************************************************************************
Exported global variables
**********************************************************************************************************************/

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
uint32_t R_PTPIF_GetVersion(void);
void R_PTPIF_Reset(void);
void R_PTPIF_Init(void);
ptpif_return_t R_PTPIF_Open_ZC2(void);
void R_PTPIF_LinkProcess(void);
ptpif_return_t R_PTPIF_CheckLink_ZC(void);
ptpif_return_t R_PTPIF_Close_ZC2(void);
int32_t R_PTPIF_Read(uint32_t *ch, void *buf);
ptpif_return_t R_PTPIF_Write(uint8_t* buf, uint32_t size);
int32_t R_PTPIF_Read_ZC2(uint32_t *ch, void **buf);
ptpif_return_t R_PTPIF_Read_ZC2_BufRelease(void);
ptpif_return_t R_PTPIF_Write_ZC2_GetBuf(void **buf, uint16_t *size);
ptpif_return_t R_PTPIF_Write_ZC2_SetBuf(uint32_t len);
#if (PTPIF_CFG_MODE & PTPIF_MODE_HWINT)
ptpif_return_t R_PTPIF_RegMsgHndr(PTPIF_INTEVT event, PTPIF_HNDLR func);
#endif

#endif /* _PTPIF_RX_H */
