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
* Copyright (C) 2016-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx_private_targets.h
* Description  : Header file for include to do a target product header.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Added support for RX65N.
*              :                     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Added support for RX24U, RX24T(512KB).
*              :                     Deleted unnecessary header information.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 31.10.2017 2.31     Added the demo for RX65N, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*              : 16.11.2018 2.41     Added XML document number.
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin.
*              : 20.05.2019 3.00     Added support for GNUC and ICCRX.
*              : 28.06.2019 3.10     Added support for RX23W.
*              : 15.08.2019 3.20     Added support for RX72M.
*              : 25.11.2019 3.30     Added support RX13T
*                                    Removed support for Generation 1 devices.
*              : 30.12.2019 3.40     Added support RX66N, RX72N.
***********************************************************************************************************************/
#ifndef LVD_PRIVATE_TARGETS_HEADER_FILE
#define LVD_PRIVATE_TARGETS_HEADER_FILE

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX110)
    #include "./targets/rx110/r_lvd_rx110.h"
#elif defined(BSP_MCU_RX111)
    #include "./targets/rx111/r_lvd_rx111.h"
#elif defined(BSP_MCU_RX113)
    #include "./targets/rx113/r_lvd_rx113.h"
#elif defined(BSP_MCU_RX130)
    #include "./targets/rx130/r_lvd_rx130.h"
#elif defined(BSP_MCU_RX13T)
    #include "./targets/rx13t/r_lvd_rx13t.h"
#elif defined(BSP_MCU_RX23T)
    #include "./targets/rx23t/r_lvd_rx23t.h"
#elif defined(BSP_MCU_RX230)
    #include "./targets/rx230/r_lvd_rx230.h"
#elif defined(BSP_MCU_RX231)
    #include "./targets/rx231/r_lvd_rx231.h"
#elif defined(BSP_MCU_RX23W)
    #include "./targets/rx23w/r_lvd_rx23w.h"
#elif defined(BSP_MCU_RX24T)
    #include "./targets/rx24t/r_lvd_rx24t.h"
#elif defined(BSP_MCU_RX24U)
    #include "./targets/rx24u/r_lvd_rx24u.h"
#elif defined(BSP_MCU_RX64M)
    #include "./targets/rx64m/r_lvd_rx64m.h"
#elif defined(BSP_MCU_RX65N)
    #include "./targets/rx65n/r_lvd_rx65n.h"
#elif defined(BSP_MCU_RX66N)
    #include "./targets/rx66n/r_lvd_rx66n.h"
#elif defined(BSP_MCU_RX66T)
    #include "./targets/rx66t/r_lvd_rx66t.h"
#elif defined(BSP_MCU_RX71M)
    #include "./targets/rx71m/r_lvd_rx71m.h"
#elif defined(BSP_MCU_RX72T)
    #include "./targets/rx72t/r_lvd_rx72t.h"
#elif defined(BSP_MCU_RX72M)
    #include "./targets/rx72m/r_lvd_rx72m.h"
#elif defined(BSP_MCU_RX72N)
    #include "./targets/rx72n/r_lvd_rx72n.h"
#else
    #error "This MCU is not supported by the current r_lvd_rx module."
#endif

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define LVD_RX_VERSION_MAJOR                    (3)
#define LVD_RX_VERSION_MINOR                    (40)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

#endif/* LVD_PRIVATE_TARGETS_HEADER_FILE */

/* End of File */

