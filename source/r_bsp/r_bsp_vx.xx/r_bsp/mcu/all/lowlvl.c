/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : lowlvl.c
* Description  : Functions to support stream I/O to the E1 virtual Console
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2019 3.00     Merged processing of all devices.
*                               Fixed coding style.
*         : 25.11.2022 3.01     Modiffied comment.
*         : 31.05.2024 3.02     Fixed coding style.
*         : 26.02.2025 3.03     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/* When using the user startup program, disable the following code. */
#if BSP_CFG_STARTUP_DISABLE == 0

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define BSP_PRV_E1_DBG_PORT (*(volatile st_dbg_t     R_BSP_EVENACCESS_SFR *)0x84080)
#define BSP_PRV_TXFL0EN     (0x00000100)          /* debug TX flow control bit */
#define BSP_PRV_RXFL0EN     (0x00001000)          /* debug RX flow control bit */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct
{
    uint32_t   tx_data;     /* Debug Virtual Console TX data */
    char       wk1[12];     /* spacer */
    uint32_t   rx_data;     /* Debug Virtual Console RX data */
    char       wk2[44];     /* spacer */
    uint32_t   dbgstat;     /* Debug Virtual Console Status */
} st_dbg_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
#if BSP_CFG_USER_CHARPUT_ENABLED != 0
/* If user has indicated they want to provide their own charput function then this is the prototype. */
void BSP_CFG_USER_CHARPUT_FUNCTION(char output_char);
#endif

#if BSP_CFG_USER_CHARGET_ENABLED != 0
/* If user has indicated they want to provide their own charget function then this is the prototype. */
char BSP_CFG_USER_CHARGET_FUNCTION(void);
#endif

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: charput
* Description  : Outputs a character on a serial port
* Arguments    : character to output
* Return Value : none
***********************************************************************************************************************/
void charput(char output_char)
{
    /* If user has provided their own charput() function, then call it. */
#if BSP_CFG_USER_CHARPUT_ENABLED == 1
    BSP_CFG_USER_CHARPUT_FUNCTION(output_char);
#else
    /* Wait for transmit buffer to be empty */
    /* WAIT_LOOP */
    while(0 != (BSP_PRV_E1_DBG_PORT.dbgstat & BSP_PRV_TXFL0EN))
    {
        /* do nothing */
        R_BSP_NOP();
    }

    /* Write the character out */
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_E1_DBG_PORT.tx_data = (int32_t)output_char;
#endif
} /* End of function charput() */

/***********************************************************************************************************************
* Function Name: charget
* Description  : Gets a character on a serial port
* Arguments    : none
* Return Value : received character
***********************************************************************************************************************/
char charget(void)
{
    /* If user has provided their own charget() function, then call it. */
#if BSP_CFG_USER_CHARGET_ENABLED == 1
    return BSP_CFG_USER_CHARGET_FUNCTION();
#else
    /* Wait for recieve buffer to be ready */
    /* WAIT_LOOP */
    while(0 == (BSP_PRV_E1_DBG_PORT.dbgstat & BSP_PRV_RXFL0EN))
    {
        /* do nothing */
        R_BSP_NOP();
    }

    /* Read data, send back up */
    /* Casting is valid because it matches the type to the retern value. */
    return (char)BSP_PRV_E1_DBG_PORT.rx_data;
#endif
} /* End of function charget() */

#endif /* BSP_CFG_STARTUP_DISABLE == 0 */

