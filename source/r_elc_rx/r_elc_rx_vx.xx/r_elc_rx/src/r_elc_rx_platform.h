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
 * File Name    : r_elc_rx_platform.h
 * Version      : 1.0
 * Description  : Functions for using Event Link Controller module
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           01.07.2016 1.0     Initial Release
 *           12.09.2016 1.10    Added support for RX65N
 *           16.06.2017 1.20    Added support for RX65N(2MB), RX130(512KB)
 ***********************************************************************************************************************/
#ifndef ELC_RX_PLATFORM_H
    #define ELC_RX_PLATFORM_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
    #include "platform.h"
    #if defined(BSP_MCU_RX113)
        #include "./targets/rx113/r_elc_rx113_private.h"  /* RX113 */
    #endif
    #if defined(BSP_MCU_RX130)
        #include "./targets/rx130/r_elc_rx130_private.h"  /* RX130 */
    #endif
    #if defined(BSP_MCU_RX230)
        #include "./targets/rx230/r_elc_rx230_private.h"  /* RX230 */
    #endif
    #if defined(BSP_MCU_RX231)
        #include "./targets/rx231/r_elc_rx231_private.h"  /* RX231 */
    #endif
    #if defined(BSP_MCU_RX65N)
        #include "./targets/rx65n/r_elc_rx65n_private.h"  /* RX65N */
    #endif

#endif /* ELC_RX_PLATFORM_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
