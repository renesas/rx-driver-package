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
* File Name    : r_mmcif_rx_target.h
* Device       : RX71M
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX71M
* Description  : Interface file for MMC API for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
**********************************************************************************************************************/

#ifndef _MMC_RX_TARGET_H
#define _MMC_RX_TARGET_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* ---- Definition of group interrupt source register. ---- */
#define MMC_CH0_INT_CDETIO          (0x00000040u)   /* MMC channel 0 card detection interrupt                       */
#define MMC_CH0_INT_ERRIO           (0x00000080u)   /* MMC channel 0 error interrupt                                */
#define MMC_CH0_INT_ACCIO           (0x00000100u)   /* MMC channel 0 access interrupt                               */

#define MMC_CH1_INT_CDETIO          (0x00000040u)   /* MMC channel 1 card detection interrupt                       */
#define MMC_CH1_INT_ERRIO           (0x00000080u)   /* MMC channel 1 error interrupt                                */
#define MMC_CH1_INT_ACCIO           (0x00000100u)   /* MMC channel 1 access interrupt                               */


/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/


#endif /* _MMC_RX_TARGET_H */

