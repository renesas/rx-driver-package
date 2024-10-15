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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsci_rx_platform.h
* Description  : Functions for using RSCI on the RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2021 1.00    Initial Release
*                              Supported for RX671.
*           31.03.2022 2.10    Supported for RX660.
*           15.08.2022 2.30    Supported for RX26T.
*           28.06.2024 2.60    Supported for RX260, RX261.
************************************************************************************************************************/

#ifndef RSCI_RX_PLATFORM_H
#define RSCI_RX_PLATFORM_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_rsci_rx_if.h"

#if defined(BSP_MCU_RX671)
#include "./targets/rx671/r_rsci_rx671_private.h"  /* RX671 */
#elif defined(BSP_MCU_RX660)
#include "./targets/rx660/r_rsci_rx660_private.h"  /* RX660 */
#elif defined(BSP_MCU_RX26T)
#include "./targets/rx26t/r_rsci_rx26t_private.h"  /* RX26T */
#elif defined(BSP_MCU_RX260)
#include "./targets/rx260/r_rsci_rx260_private.h"  /* RX260 */
#elif defined(BSP_MCU_RX261)
#include "./targets/rx261/r_rsci_rx261_private.h"  /* RX261 */
#else
#error "ERROR - r_rsci_rxXXX_private.h not included."
#endif

#endif /* RSCI_RX_PLATFORM_H */

