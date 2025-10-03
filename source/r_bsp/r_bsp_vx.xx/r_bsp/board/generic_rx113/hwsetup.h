/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : hwsetup.h
* Description  : Hardware setup header file.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
*         : 24.06.2013 1.01     Added comment about hardware_setup() being a r_bsp internal function.
*         : 28.02.2019 1.02     Fixed coding style.
*         : 26.02.2025 1.03     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef HWSETUP_H
#define HWSETUP_H

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
void hardware_setup(void); //r_bsp internal function. DO NOT CALL.

/* End of multiple inclusion prevention macro */
#endif

