/***********************************************************************************************************************
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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpc_rx_platform.h
 * Description  : The LPC module configures the MCU for the different operating and low power modes.
 *                This file is the LPC module's interface header file and should be included by the application that
 *                intends to use the API.
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.10.2016 1.00    First Release
 *         : 01.07.2019 1.42    Added RX23W.
 *         : 14.11.2019 2.00    Removed RX210.
 ***********************************************************************************************************************/
#ifndef R_LPC_RX_PLATFORM_H
    #define R_LPC_RX_PLATFORM_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
/* Include the target specific interface header files. See these files for the MCU specific interface type definitions.*/
    #if defined(BSP_MCU_RX110)
        #include "./targets/rx110/r_lpc_rx110_if.h"
    #elif defined(BSP_MCU_RX111) 
        #include "./targets/rx111/r_lpc_rx111_if.h"
    #elif defined(BSP_MCU_RX113)
        #include "./targets/rx113/r_lpc_rx113_if.h"
    #elif defined(BSP_MCU_RX130)
        #include "./targets/rx130/r_lpc_rx130_if.h"
    #elif defined(BSP_MCU_RX230)
        #include "./targets/rx230/r_lpc_rx230_if.h"
    #elif defined(BSP_MCU_RX231)
        #include "./targets/rx231/r_lpc_rx231_if.h"
    #elif defined(BSP_MCU_RX23W)
        #include "./targets/rx23w/r_lpc_rx23w_if.h"
    #elif defined(BSP_MCU_RX64M)
        #include "./targets/rx64m/r_lpc_rx64m_if.h"
    #elif defined(BSP_MCU_RX71M)
        #include "./targets/rx71m/r_lpc_rx71m_if.h"
    #else
        #error "This MCU is not supported by the current r_lpc_rx module."
    #endif

#endif /* R_LPC_RX_PLATFORM_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
