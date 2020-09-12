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
* File Name    : r_ptp_rx_if.h
* Description  : Interface file for PTP (EPTPC part) API for RX
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

#ifndef _PTP_RX_H
#define _PTP_RX_H

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
#define RX_PTP_VERSION_MAJOR           (1)
#define RX_PTP_VERSION_MINOR           (16)

/* Number of ports */
#define NUM_PORT (2) /* Set 2 in the RX64M/71M/72M */

/* PTP driver (PTP synchronization part) return value */
typedef enum
{
	PTP_ERR_TOUT = -3,  	/* Timeout error */
    PTP_ERR_PARAM = -2, 	/* Parameter error */
	PTP_ERR = -1,       	/* General error */
	PTP_OK = 0
} ptp_return_t;

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

/* Multicast (MC) frames filter setting */
typedef enum 
{
    MC_REC_ALL = 0, /* Receive all MC frames (legacy compatible) */
	MC_REC_NO,      /* Do not receive MC frame */
    MC_REC_REG0,    /* Receive only the MC frame registered FMAC0R(U/L) */
    MC_REC_REG1,    /* Receive only the MC frame registered FMAC1R(U/L) */
} MCRecFil;

/* Clock type and port number */
typedef enum 
{
	PD_ORDINARY_CLOCK_PORT0 = 0, /* Ordinary Clock port0 */
	PD_ORDINARY_CLOCK_PORT1,     /* Ordinary Clock port1 */
	PD_BOUNDARY_CLOCK,           /* Boundary Clock */
	PD_TRANSPARENT_CLOCK,        /* Transparent Clock */
} PTPDevice;

/* Delay correction protocol */
typedef enum 
{
	NP_P2P = 0, /* Peer to peer */
	NP_E2E,     /* End to end */
} DelayMechanism;

/* Master, Slave or Listening */
typedef enum 
{ /* Those states are different from PTP state enumeration value */
	ST_MASTER = 0, /* Master state */
	ST_SLAVE,      /* Slave state */
    ST_LIST,       /* Listening state */
} PTPState;

/* MINT interrupt register */
typedef enum
{
    MINT_FUNC_STCA = 0, /* Interrupt from STCA   */
    MINT_FUNC_PRC,      /* Interrupt from PRC-TC */
    MINT_FUNC_SYN0,     /* Interrupt from SYNFP0 */
    MINT_FUNC_SYN1,     /* Interrupt from SYNFP1 */
} MINT_Reg;

/* Timer channel (bit map form) */
typedef enum 
{
	INT_CYC0 = 0x01,
	INT_CYC1 = 0x02,
	INT_CYC2 = 0x04,
	INT_CYC3 = 0x08,
	INT_CYC4 = 0x10,
	INT_CYC5 = 0x20
} IntCycCh;

/* STCA mode and gradient setting */
typedef enum 
{
	STCA_MODE1    = 0x00, /* Mode1 (not use STCA) */
	STCA_MODE2_HW = 0x02, /* Mode2 (use STCA) and HW gradient setting */
	STCA_MODE2_SW = 0x03, /* Mode2 (use STCA) and SW gradient setting */
} STCA_GRAD;

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* ==== PTP data type structure ==== */
/* 48 bit unsigned integer */
typedef struct
{
    uint16_t hi;
    uint32_t lo;
} UInt48;

/* 64 bit signed integer */
typedef struct
{
    int32_t hi;
    uint32_t lo;
} Int64;

/* 64 bit unsigned integer */
typedef struct
{
	uint32_t hi;
	uint32_t lo;
} UInt64;

/* 64 bit scaled nano second unit expression */
typedef struct
{
	Int64 scaledNanoseconds;
} TimeInterval;

/* PTP message timestamp expression */
typedef struct
{
	UInt48 secondsField;
	uint32_t nanosecondsField;
} Timestamp;

/* ==== Driver control structure ==== */
/* PTP port related structure (MAC address, IP address, PTP state and delay mechanism) */
typedef struct 
{
	uint8_t	macAddr[6];
	uint8_t	ipAddr[4];
	PTPState state;
	DelayMechanism delay;
} PTPPort;

/* PTP configuration structure (device and port information) */
typedef struct 
{
	PTPDevice device;
	PTPPort port[NUM_PORT];
} PTPConfig;

/* Synchronous state change detection structure */
typedef struct 
{
	UInt64 th_val;	/* Threshold value of synchronous state or deviation */
	uint8_t times;	/* Times of successive detection */
} SyncDet;

/* PTP sub configuration structure */
typedef struct 
{
	UInt48 delay_asymmetry;
	uint8_t w10_times;	/* Times of sync reception getting worst10 completed */
	SyncDet dev;		/* Condition of deviation state detection */
	SyncDet syn;		/* Condition of synchronous state detection */
} PTPSubConfig;

/* MINT interrupt handler type definition */ 
typedef void (*MINT_HNDLR)(uint32_t);

/* Timer interrupt handler type definition */ 
typedef void (*TMR_HNDLR)(uint32_t);

/* Announce flagField type structure */
typedef union
{
	uint32_t LONG;
	R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_14 (
		uint32_t Rsv1:17,
		uint32_t profileSpec2:1,
		uint32_t profileSpec1:1,
		uint32_t Rsv2:2,
		uint32_t unicastFlag:1,
		uint32_t Rsv3:1,
		uint32_t alternateMasterFlag:1,
		uint32_t Rsv4:2,
		uint32_t frequencyTraceable:1,
		uint32_t timeTraceable:1,
		uint32_t ptpTimescale:1,
		uint32_t currentUtcOffsetValid:1,
		uint32_t leap59:1,
		uint32_t leap61:1
	) BIT;
} AnceFlag;

/* PTP clock quality structure */
typedef struct 
{
	uint8_t clockClass;
	uint8_t clockAccuracy;
	uint16_t offsetScaledLogVariance;
} ClkQuality;

/* Announce message field type structure */
typedef struct
{
	uint8_t grandmasterPriority1;
	uint8_t grandmasterPriority2;
	ClkQuality grandmasterClockQuality;
	int8_t *grandmasterIdentity;
} AnceMsg;

/**********************************************************************************************************************
Exported global variables
**********************************************************************************************************************/

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
uint32_t R_PTP_GetVersion(void);
void R_PTP_Reset(void);
ptp_return_t R_PTP_SetTran(TranMode *mode, RelEnabDir *dir);
ptp_return_t R_PTP_SetMCFilter(uint8_t ch, MCRecFil fil, uint32_t *fmac);
ptp_return_t R_PTP_SetExtPromiscuous(uint8_t ch, bool is_set);
ptp_return_t R_PTP_Init(PTPConfig *tbl);
ptp_return_t R_PTP_SubConfig(uint8_t ch, PTPSubConfig *tbl);
void R_PTP_RegMINTHndr(MINT_Reg reg, uint32_t event, MINT_HNDLR func);
ptp_return_t R_PTP_RegTmrHndr(IntCycCh ch, TMR_HNDLR func);
ptp_return_t R_PTP_ELC_Ind(IntCycCh ch, uint8_t edge, uint8_t set);
ptp_return_t R_PTP_ELC_SetClr(IntCycCh ch, uint8_t edge, uint8_t set);
ptp_return_t R_PTP_Tmr_Set(IntCycCh ch, UInt64 event, uint32_t cycle, uint32_t pulse);
ptp_return_t R_PTP_GetLcClk(Timestamp *clk);
ptp_return_t R_PTP_SetLcClk(Timestamp *clk);
ptp_return_t R_PTP_ChkW10(void);
ptp_return_t R_PTP_GetW10(uint32_t *p_w10, uint32_t *m_w10);
ptp_return_t R_PTP_SetGradLimit(uint32_t *p_lim, uint32_t *m_lim);
ptp_return_t R_PTP_GetMPortID(uint8_t ch, uint32_t *clk, uint16_t *port);
ptp_return_t R_PTP_SetMPortID(uint8_t ch, uint32_t *clk, uint16_t *port);
ptp_return_t R_PTP_GetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf);
ptp_return_t R_PTP_SetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf);
ptp_return_t R_PTP_GetSyncInfo(uint8_t ch, TimeInterval *ofm, TimeInterval *mpd);
ptp_return_t R_PTP_UpdClkID(uint8_t ch, int8_t *id);
ptp_return_t R_PTP_UpdDomainNum(uint8_t ch, uint8_t dnum);
ptp_return_t R_PTP_UpdAnceFlags(uint8_t ch, AnceFlag *flags);
ptp_return_t R_PTP_UpdAnceMsgs(uint8_t ch, AnceMsg *msgs);
ptp_return_t R_PTP_UpdSyncAnceInterval(uint8_t ch, int8_t *sync, int8_t *ance);
ptp_return_t R_PTP_UpdDelayMsgInterval(uint8_t ch, int8_t *interval, uint32_t *tout);
ptp_return_t R_PTP_Start(void);
ptp_return_t R_PTP_Stop(void);
ptp_return_t R_PTP_SetPortState(uint8_t ch, PTPState state);
uint32_t R_PTP_GetSyncCH(void);
ptp_return_t R_PTP_SetInterrupt(uint8_t ch);
ptp_return_t R_PTP_ChkInterrupt(uint8_t ch, bool *is_det);
ptp_return_t R_PTP_ClrInterrupt(uint8_t ch);
void R_PTP_DisableTmr(void);
ptp_return_t R_PTP_SetSyncDet(SyncDet *dev, SyncDet *syn, bool is_enab);
ptp_return_t R_PTP_SetSynctout(uint32_t tout, bool is_enab);
void Eptpc_isr(void);

#endif /* _PTP_RX_H */
