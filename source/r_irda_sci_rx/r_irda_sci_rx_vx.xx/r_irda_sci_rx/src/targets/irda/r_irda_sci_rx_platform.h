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
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 27.01.2015 1.01     Chage symbol name of Arguments 
*                                    None change proceedures.
*******************************************************************************/
/*******************************************************************************
* System Name  : interface file for IrDA fit module.
* File Name    : r_irda_sci_rx_platform.h
* Version      : 1.01
* Device       : RX
* Abstract     : 
* Tool-Chain   : Renesas RXC Toolchain v2.02.00
* OS           : not use
* H/W Platform : RSKRX113
* Description  : Functions for using IrDA on RX devices. 
* Limitation   : none
*******************************************************************************/
/* Guards against multiple inclusion */
#ifndef IRDA_SCI_PLATFORM_H

/*******************************************************************************
  Macro definitions
*******************************************************************************/
#define IRDA_SCI_PLATFORM_H

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Include specifics for chosen MCU.  */
#if defined(BSP_MCU_RX113)
    #include "./rx113/r_irda_sci_rx113_private.h"
#else
    #error "This MCU is not supported by the current r_irda_sci_rx module."
#endif


#endif /* IRDA_SCI_PLATFORM_H */

/* End of File */
