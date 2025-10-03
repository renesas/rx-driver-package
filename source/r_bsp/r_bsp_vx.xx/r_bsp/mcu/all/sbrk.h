/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : sbrk.h
* Description  : Configures the MCU heap memory.  The size of the heap is defined by the macro HEAPSIZE below.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2019 1.00     First Release
*         : 26.02.2025 1.01     Changed the disclaimer.
*         : 28.05.2025 1.02     Added compile switch of BSP_CFG_LOW_LEVEL_INTERFACE_SBRK_ENABLE.
*                               Deleted the heap definition for OPTLIB.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_bsp_common.h"
#include "r_bsp_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SBRK_H
#define SBRK_H

/* Only use this file if heap is enabled in r_bsp_config. */
#if BSP_CFG_HEAP_BYTES > 0

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0

#if defined(__CCRX__) || defined(__GNUC__)
#if BSP_CFG_LOW_LEVEL_INTERFACE_SBRK_ENABLE == 1
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef union
{
    int32_t  dummy;             /* Dummy for 4-byte boundary */
    int8_t heap[BSP_CFG_HEAP_BYTES];    /* Declaration of the area managed by sbrk*/
} u_heap_type_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/* const size_t _sbrk_size=      // Specifies the minimum unit of */
/* the defined heap area */
int8_t *_s1ptr;

/* Memory allocation function prototype declaration (CC-RX and GNURX+NEWLIB) */
int8_t  *sbrk(size_t size);

#endif /* BSP_CFG_LOW_LEVEL_INTERFACE_SBRK_ENABLE == 1 */
#endif /* defined(__CCRX__), defined(__GNUC__) */

#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

#endif /* BSP_CFG_HEAP_BYTES */

#endif  /* SBRK_H */

