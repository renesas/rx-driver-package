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
* Copyright (C) 2016(2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdsi_rx_config_reference.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Configures the SDSI for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 30.09.2016 1.00    First Release
*              : 31.07.2017 2.00    Supported RX65N-2MB.
*              :                    Fixed to correspond to Renesas coding rule.
*              : 04.04.2019 2.01    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
**********************************************************************************************************************/
#ifndef SDSI_RX_CONFIG_H_
#define SDSI_RX_CONFIG_H_

/**********************************************************************************************************************
SET SDSI FIT
**********************************************************************************************************************/
/* If do not use FIT environment, comment out. */
#define SDSI_CFG_USE_FIT

/**********************************************************************************************************************
SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
**********************************************************************************************************************/
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
   Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define SDSI_CFG_PARAM_CHECKING_ENABLE     (BSP_CFG_PARAM_CHECKING_ENABLE)

/**********************************************************************************************************************
SPECIFY DEVICES TO INCLUDE SOFTWARE SUPPORT
**********************************************************************************************************************/
/* If use the SDSI channel, define it. */
#define SDSI_CFG_CH0_INCLUDED

/**********************************************************************************************************************
ENABLE DEBUGGING INFORMATION
**********************************************************************************************************************/
/* Stores error log information using LONGQ driver. */
/* #define SDSI_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
SET SDSI INTERRUPT PRIORITY LEVEL
**********************************************************************************************************************/
/* Set SDSI interrupt priority level.
   If the SDSI interrupt assigned to the group interrupt, set it in consideration of the priority of
   other interrupts assigned to the same group. */
#define SDSI_CFG_CH0_INT_LEVEL  (5)

/**********************************************************************************************************************
ENABLE SETTING TO DISABLE SYSTEM INTERRUPT REQUESTS WHILE R_SDSI_Open()
**********************************************************************************************************************/
/* Disable interrupt requests while R_SDSI_Open(). 
   Detect the published SDIO command that the driver is in an uninitialized state and
   reduce possibility to return the response.
   This is performed by setting the "I" bit in the CPU's Processor Status Word (PSW) register.
   The "I" bit of the PSW can only be modified when in Supervisor Mode. If the CPU is in User Mode
   and this function is called then a Privileged Instruction Exception will occur. */

/* #define SDSI_CFG_DISABLE_SYSTEM_INTERRUPT */

/**********************************************************************************************************************
SET FUNCTION BASIC REGISTER (FBR) VALUE
**********************************************************************************************************************/
/* When R_SDSI_Initialize() called, set FBR value. */
#define SDSI_CFG_FBR_ADR_100H   (0x00)  /* b7-b4 : Ignore setting value. */
                                        /* b3-b0 : Function 1 Standard SDIO Function Interface Code */
#define SDSI_CFG_FBR_ADR_101H   (0x01)  /* b7-b0 : Function 1 Extended standard SDIO Function interface code */
/* #define SDSI_CFG_FBR_ADR_102H   (0x00) */ /* b7-b0 : Ignore setting value. */
#define SDSI_CFG_FBR_ADR_103H   (0x00)  /* b7-b0 : Function 1 Standard iSDIO Function Interface Code */
#define SDSI_CFG_FBR_ADR_104H   (0x00)  /* b7-b0 : Function 1 MID_MANF SDIO Card Manufacturer Code */
#define SDSI_CFG_FBR_ADR_105H   (0x00)  /* b7-b0 : Function 1 MID_MANF SDIO Card Manufacturer Code */
#define SDSI_CFG_FBR_ADR_106H   (0x00)  /* b7-b0 : Function 1 MID_CARD Manufacturer Information */
#define SDSI_CFG_FBR_ADR_107H   (0x00)  /* b7-b0 : Function 1 MID_CARD Manufacturer Information */
#define SDSI_CFG_FBR_ADR_108H   (0x00)  /* b7-b0 : Function 1 iSDIO Type Support Code */

/**********************************************************************************************************************
SET SPS BIT OF FUNCTION BASIC REGISTER (FBR)
**********************************************************************************************************************/
/* When R_SDSI_Initialize() called, set SDSICR3.SPS bit (0 or 1). */
#define SDSI_CFG_FBR_SPS_BIT    (0)

/**********************************************************************************************************************
SET SMPC BIT OF CCCR REGISTER
**********************************************************************************************************************/
/* When R_SDSI_Initialize() called, set SDSICR3.SMPC bit (0 or 1). */
#define SDSI_CFG_CCCR_SMPC_BIT  (0)


#endif /* _SDSI_RX_CONFIG_H_ */

