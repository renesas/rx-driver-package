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
* Copyright (C) 2016(2019) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdsi_rx_if.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Interface file for SDSI API for RX
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.09.2016 1.00    First Release
*         : 31.07.2017 2.00    Supported RX65N-2MB.
*         :                    Fixed to correspond to Renesas coding rule.
*         : 01.02.2019 2.01    Changed Minor version to 2.01.
*         : 04.04.2019 2.02    Added support for GNUC and ICCRX.
*                              Fixed coding style.
**********************************************************************************************************************/

#ifndef SDSI_RX_IF_H_
#define SDSI_RX_IF_H_

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Used to get which MCU is currently being used. */
#include "platform.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Version Number of API. */
#define RX_SDSI_VERSION_MAJOR       (2)
#define RX_SDSI_VERSION_MINOR       (2)

#define SDSI_CH0                    (0)
#define SDSI_CH1                    (1)

/* SDSI function1 register 1-5  */
#define SDSI_FUNC1_REG1             (0x00000001)
#define SDSI_FUNC1_REG2             (0x00000002)
#define SDSI_FUNC1_REG3             (0x00000003)
#define SDSI_FUNC1_REG4             (0x00000004)
#define SDSI_FUNC1_REG5             (0x00000005)

/* ---- Direct Transfer mode ---- */
#define SDSI_MODE_DIRECT_ADDR_FIXED (0x00000001)    /* Direct transfer address is fixed. */
#define SDSI_MODE_DIRECT_ADDR_INC   (0x00000002)    /* Direct transfer address is incremented after each transfer. */
#define SDSI_MODE_DIRECT_BUS_LOCK   (0x00000010)    /* Internal main bus is locked. */
#define SDSI_MODE_DIRECT_BUS_UNLOCK (0x00000020)    /* Internal main bus is not locked. */

/* ---- IO commands ---- */
#define SDSI_CMD52_W                (0x01)          /* IO_WRITE_DIRECT */
#define SDSI_CMD53_W                (0x02)          /* IO_WRITE_EXTENDED */
#define SDSI_CMD53_R                (0x04)          /* IO_READ_EXTENDED */

/* Card detection disable (rise) interrupt and card detection disable (fall) interrupt. */
#define SDSI_CD_RISE                (0)
#define SDSI_CD_FALL                (1)

#define SDSI_ADDR_BOUNDARY          (0x00000003)
#define SDSI_OFFSET_BOUNDARY        (0x00000003)

/* ---- Error code ---- */
typedef enum e_sdsi_status
{
    SDSI_SUCCESS              =   0,
    SDSI_ERR                  =  -1,
    SDSI_ERR_BUSY             =  -2,
    SDSI_ERR_ADDRESS_BOUNDARY =  -3      /* Not specified buffer address in 4-byte unit */
} sdsi_status_t;

typedef struct
{
    uint32_t    reg_no;
    uint32_t    offset;
    uint32_t    * p_buff;
} sdsi_reg_t;

typedef struct
{
    uint32_t    reg_no;
    uint32_t    offset;
    uint32_t    * p_buff;
} sdsi_cis_reg_t;

typedef struct
{
    uint32_t    reg_no;
    uint32_t    offset;
    uint8_t    * p_buff;
} sdsi_func_reg_t;

typedef struct
{
    uint32_t    adr;
    uint32_t    mode;
} sdsi_direct_trans_t;

typedef struct
{
    uint32_t    adr;
    uint16_t    blkcnt;
    uint16_t    bytcnt;
    uint8_t     sdcmdcr;
    uint8_t     cmd;
    uint8_t     rsv[2];
} sdsi_cmd_t;

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
sdsi_status_t R_SDSI_Open(uint32_t channel, void * p_sdsi_workarea);
sdsi_status_t R_SDSI_Close(uint32_t channel);
sdsi_status_t R_SDSI_Initialize(uint32_t channel);
sdsi_status_t R_SDSI_End(uint32_t channel);
sdsi_status_t R_SDSI_CflagPolling(uint32_t channel);
sdsi_status_t R_SDSI_WriteCisReg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg);
sdsi_status_t R_SDSI_ReadCisReg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg);
sdsi_status_t R_SDSI_WriteFuncReg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg);
sdsi_status_t R_SDSI_ReadFuncReg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg);
sdsi_status_t R_SDSI_WriteIntVectorReg(uint32_t channel, uint8_t vector);
sdsi_status_t R_SDSI_ReadIntVectorReg(uint32_t channel, uint8_t * p_vector);
sdsi_status_t R_SDSI_ReadIntClearReg(uint32_t channel, uint8_t * p_vector);
sdsi_status_t R_SDSI_EnableDirectTrans(uint32_t channel, sdsi_direct_trans_t * p_sdsi_direct_trans);
sdsi_status_t R_SDSI_DisableDirectTrans(uint32_t channel);
sdsi_status_t R_SDSI_SetDirectTransAdr(uint32_t channel, uint32_t adr);
sdsi_status_t R_SDSI_GetDirectTransAdr(uint32_t channel, uint32_t * p_adr);
sdsi_status_t R_SDSI_RegistIntCallback(uint32_t channel, sdsi_status_t (* callback)(sdsi_cmd_t *));
sdsi_status_t R_SDSI_RegistCdIntCallback(uint32_t channel, sdsi_status_t (* callback)(uint32_t));
sdsi_status_t R_SDSI_RegistDtIntCallback(uint32_t channel, sdsi_status_t (* callback)(sdsi_cmd_t *));
uint32_t      R_SDSI_GetVersion(void);
void          R_SDSI_IntHandler0(void * vect);
sdsi_status_t R_SDSI_SetLogHdlAddress(uint32_t user_long_que);
uint32_t      R_SDSI_Log(uint32_t flg, uint32_t fid, uint32_t line);


#endif /* _SDSI_RX_IF_H_ */

