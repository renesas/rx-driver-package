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
* Copyright (C) 2013-2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/******************************************************************************
* File Name    : r_irq_rx_if.h
* Device(s)    : Renesas RX Family
* Tool-Chain   : Renesas RX Standard Toolchain  
* H/W Platform : 
* Description  : Definitions and declarations for the IRQ support module API.
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 15.07.2013 1.00    First Release
*         : 11.12.2013         Changed RX63N build option to depend on MCU rather than board.
*         : 04.04.2014 1.20    Using the definition of VECT_ICU_IRQ15 to check the number of IRQs.
*         : 30.09.2015 1.70    Using the definition of VECT_ICU_IRQ7  to check the number of IRQs.
*         : 08.09.2016 2.10    Applied the coding rules of Renesas.
*         : 20.05.2019 3.00    Added support for GNUC and ICCRX.
*         : 15.04.2021 3.80    Added R_IRQ_IRClear() function to clear IR flag.
******************************************************************************/
#ifndef IRQ_RX_IF_H /* Multiple inclusion protection. */
#define IRQ_RX_IF_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* BSP includes */
#include "platform.h"


/******************************************************************************
Macro definitions
******************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* IRQ input pin digital filtering sample clock divisor settings.
 * !!Do not change!! */
#define IRQ_CFG_PCLK_DIV1   (0)
#define IRQ_CFG_PCLK_DIV8   (1)
#define IRQ_CFG_PCLK_DIV32  (2)
#define IRQ_CFG_PCLK_DIV64  (3)


/******************************************************************************
Typedef definitions
******************************************************************************/

/* Enumeration of IRQ numbers. Used for strong type control. */
typedef enum
{
    IRQ_NUM_0 = 0,
    IRQ_NUM_1,
    IRQ_NUM_2,
    IRQ_NUM_3,
    IRQ_NUM_4,
    IRQ_NUM_5,
#if defined(VECT_ICU_IRQ7)
    IRQ_NUM_6,
    IRQ_NUM_7,
#endif
#if defined(VECT_ICU_IRQ15)
    IRQ_NUM_8,
    IRQ_NUM_9,
    IRQ_NUM_10,
    IRQ_NUM_11,
    IRQ_NUM_12,
    IRQ_NUM_13,
    IRQ_NUM_14,
    IRQ_NUM_15,
#endif
    IRQ_NUM_MAX     /* Add more IRQs before here if needed. This must always be the last element. */
} irq_number_t;


/* Enumeration of various IRQ control command codes. */
typedef enum        
{
    IRQ_CMD_SET_PRIO = 0,
    IRQ_CMD_SET_TRIG,

    /* Reserved for future commands. */
    IRQ_CMD_LIST_END   /* This is not a command. Do not use it as a command or change it. */
}irq_cmd_t;

/* Enumeration of IRQ interrupt priority settings. */
typedef enum
{
    IRQ_PRI_0 = 0,
    IRQ_PRI_1,
    IRQ_PRI_2,
    IRQ_PRI_3,
    IRQ_PRI_4,
    IRQ_PRI_5,
    IRQ_PRI_6,
    IRQ_PRI_7,
    IRQ_PRI_8,
    IRQ_PRI_9,
    IRQ_PRI_10,
    IRQ_PRI_11,
    IRQ_PRI_12,
    IRQ_PRI_13,
    IRQ_PRI_14,
    IRQ_PRI_15,
} irq_prio_t;

/* Enumerated type for trigger type: low level, rising edge, falling edge, both edges. */
typedef enum
{
    IRQ_TRIG_LOWLEV     = 0x00,
    IRQ_TRIG_FALLING    = 0x04,
    IRQ_TRIG_RISING     = 0x08,
    IRQ_TRIG_BOTH_EDGE  = 0x0C
} irq_trigger_t;

typedef struct irq_init_block_s *irq_handle_t;

/* IRQ API error codes */
typedef enum _irq_err
{
    IRQ_SUCCESS=0,
    IRQ_ERR_BAD_NUM,        /* Invalid IRQ number. */
    IRQ_ERR_NOT_OPENED,     /* IRQ not yet opened. */
    IRQ_ERR_NOT_CLOSED,     /* IRQ still open from previous open. */
    IRQ_ERR_UNKNOWN_CMD,    /* Control command is not recognized. */
    IRQ_ERR_INVALID_ARG,    /* Argument is not valid for parameter. */
    IRQ_ERR_INVALID_PTR,    /* Received null pointer or pointer out of range; missing required argument. */
    IRQ_ERR_LOCK,           /* The lock procedure failed. */
    IRQ_ERR_UNDEFINED,      /* Undefined error. */
} irq_err_t;


/******************************************************************************
Functions Prototypes
******************************************************************************/
/*  Initializes the IRQ registers, enables interrupts, provides handle for other API functions. */
irq_err_t   R_IRQ_Open (irq_number_t     irq_number,
                        irq_trigger_t    trigger,
                        irq_prio_t       priority,
                        irq_handle_t    *phandle,
                        void     (*const pcallback)(void *pargs));

/* Handles special hardware or software operations for the IRQ. */
irq_err_t   R_IRQ_Control(irq_handle_t  const handle,
                          irq_cmd_t     const cmd,
                          void               *pcmd_data);

/* Fully disables the IRQ designated by the handle. */
irq_err_t   R_IRQ_Close(irq_handle_t handle);

/* Reads the current level of the pin assigned to the specified IRQ. */
irq_err_t   R_IRQ_ReadInput(irq_handle_t  const handle,
                            uint8_t            *plevel);


/* Enables or disables the ICU interrupt for the specified IRQ. */
irq_err_t   R_IRQ_InterruptEnable (irq_handle_t  const handle,
                                   bool                enable);

/* Returns the version number of the module. */
uint32_t  R_IRQ_GetVersion(void);

/* Clears IR Flag */
irq_err_t   R_IRQ_IRClear (irq_handle_t  const handle);

#endif /* IRQ_RX_IF_H    */


