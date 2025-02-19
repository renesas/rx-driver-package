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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdhi_rx_target.h
* Device       : RX72M
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX72M
* Description  : Interface file for SDHI API for RX
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 30.07.2019 2.05    First Release
**********************************************************************************************************************/
#ifndef SDHI_RX_TARGET_H
#define SDHI_RX_TARGET_H

#if defined(BSP_MCU_RX72M)

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* #define for SDHI grp int status flag bit & level */
#define SDHI_CH0_INT_CDETI         (0x00000008u)          /* SDHI channel 0 interrupt SDHICD   */
#define SDHI_CH0_INT_CACI          (0x00000010u)          /* SDHI channel 0 interrupt MEM_INT  */
#define SDHI_CH0_INT_SDACI         (0x00000020u)          /* SDHI channel 0 interrupt SDIO_INT */

#define SDHI_CH1_INT_CDETI         (0x00000008u)          /* SDHI channel 1 interrupt SDHICD   */
#define SDHI_CH1_INT_CACI          (0x00000010u)          /* SDHI channel 1 interrupt MEM_INT  */
#define SDHI_CH1_INT_SDACI         (0x00000020u)          /* SDHI channel 1 interrupt SDIO_INT */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* SDHI_RX_TARGET_H */

#endif /* defined(BSP_MCU_RX72M) */

