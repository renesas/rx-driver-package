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
* File Name    : r_ptp_light_rx_if.h
* Description  : Interface file for PTP light driver API for RX
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

#ifndef _PTP_LIGHT_RX_H
#define _PTP_LIGHT_RX_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Used to get which MCU is currently being used. */
#include "platform.h"
#include "r_ptp_light_rx_config.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Version Number of API. */
#define RX_PTP_LIGHT_VERSION_MAJOR           (1)
#define RX_PTP_LIGHT_VERSION_MINOR           (13)

/* Number of ports */
#define NUM_PORT (2) /* Set 2 in the RX64M/71M/72M */

/* PTP light driver return value */
typedef enum
{
	PTPL_ERR_TOUT = -3,  /* Timeout error */
    PTPL_ERR_PARAM = -2, /* Parameter error */
	PTPL_ERR = -1,       /* General error */
	PTPL_OK = 0
} ptpl_return_t;

/* Inter ports transfer mode */
typedef enum 
{
	ST_FOR = 0, /* Store and forward mode (legacy compatible) */
	CT_THR = 1  /* Cut through mode */
} TranMode;

/* Relay enable directions (bit map form) */
typedef enum 
{
    ENAB_NO = 0x00, /* Prohibit relay */
	ENAB_01 = 0x01, /* Enable CH0 to CH1 */
	ENAB_10 = 0x02, /* Enable CH1 to CH0 */
    ENAB_BT = 0x03  /* Enable CH0 to CH1 and CH1 to CH0 */
} RelEnabDir;

/* Multicast(MC) frames filter setting */
typedef enum 
{
    MC_REC_ALL = 0, /* Receive all MC frames (legacy compatible) */
	MC_REC_NO,      /* Do not receive MC frame */
    MC_REC_REG0,    /* Receive only the MC frame registered FMAC0R(U/L) */
    MC_REC_REG1,    /* Receive only the MC frame registered FMAC1R(U/L) */
} MCRecFil;

/* MINT interrupt register */
typedef enum
{
    MINT_FUNC_PRC = 0,	/* Interrupt from PRC-TC */
    MINT_FUNC_SYN0,     /* Interrupt from SYNFP0 */
    MINT_FUNC_SYN1,     /* Interrupt from SYNFP1 */
} MINT_Reg;

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* ==== Driver control structure ==== */
/* PTP part port related structure */
typedef struct 
{
	uint8_t	macAddr[6];
	uint8_t	ipAddr[4];
} PTPLPort;

/* PTP part configuration structure (port information) */
typedef struct 
{
	PTPLPort port[NUM_PORT];
} PTPLConfig;

/* MINT interrupt handler type definition */ 
typedef void (*MINT_HNDLR)(uint32_t);

/**********************************************************************************************************************
Exported global variables
**********************************************************************************************************************/

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
uint32_t R_PTPL_GetVersion(void);
void R_PTPL_Reset(void);
ptpl_return_t R_PTPL_SetTran(TranMode *mode, RelEnabDir *dir);
ptpl_return_t R_PTPL_SetMCFilter(uint8_t ch, MCRecFil fil, uint32_t *fmac);
ptpl_return_t R_PTPL_SetExtPromiscuous(uint8_t ch, bool is_set);
ptpl_return_t R_PTPL_Init(PTPLConfig *tbl);
void R_PTPL_RegMINTHndr(MINT_Reg reg, uint32_t event, MINT_HNDLR func);
ptpl_return_t R_PTPL_GetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf);
ptpl_return_t R_PTPL_SetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf);
ptpl_return_t R_PTPL_SetInterrupt(uint8_t ch);
ptpl_return_t R_PTPL_ChkInterrupt(uint8_t ch, bool *is_det);
ptpl_return_t R_PTPL_ClrInterrupt(uint8_t ch);
void Eptpc_isr(void);

#endif /* _PTP_LIGHT_RX_H */
