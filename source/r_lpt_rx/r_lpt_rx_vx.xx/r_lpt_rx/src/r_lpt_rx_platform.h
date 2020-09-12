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
 * File Name    : r_lpt_rx_platform.h
 * Version      : 1.10
 * Description  : Functions for using Low Power Timer
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.03.2016 1.00    First Release
 *         : 01.07.2016 1.10    Added support for RX130,RX230,RX231
 ***********************************************************************************************************************/
#ifndef LPT_PLATFORM_H
#define LPT_PLATFORM_H

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Include specifics for chosen MCU.  */
#if defined(BSP_MCU_RX113)
    #include "./targets/rx113/r_lpt_rx113_private.h"
#elif defined(BSP_MCU_RX130)
    #include "./targets/rx130/r_lpt_rx130_private.h"
#elif defined(BSP_MCU_RX230)
    #include "./targets/rx230/r_lpt_rx230_private.h"
#elif defined(BSP_MCU_RX231)
    #include "./targets/rx231/r_lpt_rx231_private.h"
#else
    #error "This MCU is not supported by the current r_lpt_rx module."
#endif


#endif /* LPT_PLATFORM_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
