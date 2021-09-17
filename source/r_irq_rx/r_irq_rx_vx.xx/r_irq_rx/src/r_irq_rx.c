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
/***********************************************************************************************************************
* File Name    : r_irq_rx.c
* Device(s)    : Renesas RX Family
* Tool-Chain   : Renesas RX Standard Toolchain  
* H/W Platform : 
* Description  : IRQ support API for RX Family MCUs
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 15.07.2013  1.00    First Release
*         : 10.02.2014          Additional list of supported MCUs in comments. No change to code.
*         : 04.04.2014  1.20    Using the definition of VECT_ICU_IRQ15 to check the number of IRQs.
*         : 30.09.2015  1.70    Using the definition of VECT_ICU_IRQ7  to check the number of IRQs.
*         : 15.03.2016  2.00    Applied the coding rules of Renesas.
*         : 07.09.2016  2.10    Applied the coding rules of Renesas.
*         : 28.09.2018  2.30    Updated GSCE coding rules.
*         : 20.05.2019  3.00    Added support for GNUC and ICCRX.
*         : 15.08.2019  3.20    Fixed warnings in IAR.
*         : 25.11.2019  3.30    Modified comment of API function to Doxygen style.
*         : 15.04.2021  3.80    Added R_IRQ_IRClear() function to clear IR flag.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_irq_rx_if.h"
#include "r_irq_rx_config.h"
#include "r_irq_rx_private.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define IRQ_VECT_BASE   (VECT_ICU_IRQ0)   /* IRQ vector numbers are all offsets from here. */
#define IRQ_IPR_BASE    (IPR_ICU_IRQ0)    /* IRQ priority register locations are all offsets from here. */

#define IRQ_TRIGGER_MASK (0xF3)            /* Reserved bits in IRQ detection sense register. */

/***********************************************************************************************************************
Exported global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

#if IRQ_CFG_USE_IRQ0 == 1
irq_callback p_callback_irq0;
const irq_init_block_t g_irq0_handle =
{
    IRQ_NUM_0,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ0,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ0,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ0,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ0,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq0,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ0.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ1 == 1
irq_callback p_callback_irq1;
const irq_init_block_t g_irq1_handle =
{
    IRQ_NUM_1,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ1,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ1,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ1,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ1,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq1,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ1.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ1,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ2 == 1
irq_callback p_callback_irq2;
const irq_init_block_t g_irq2_handle =
{
    IRQ_NUM_2,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ2,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ2,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ2,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ2,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq2,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ2.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ2,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ3 == 1
irq_callback p_callback_irq3;
const irq_init_block_t g_irq3_handle =
{
    IRQ_NUM_3,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ3,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ3,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ3,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ3,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq3,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ3.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ3,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ4 == 1
irq_callback p_callback_irq4;
const irq_init_block_t g_irq4_handle =
{
    IRQ_NUM_4,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ4,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ4,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ4,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ4,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq4,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ4.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ4,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ5 == 1
irq_callback p_callback_irq5;
const irq_init_block_t g_irq5_handle =
{
    IRQ_NUM_5,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ5,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ5,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ5,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ5,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq5,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ5.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ5,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if defined(VECT_ICU_IRQ7)
#if IRQ_CFG_USE_IRQ6 == 1
irq_callback p_callback_irq6;
const irq_init_block_t g_irq6_handle =
{
    IRQ_NUM_6,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ6,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ6,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ6,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ6,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq6,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ6.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ6,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ7 == 1
irq_callback p_callback_irq7;
const irq_init_block_t g_irq7_handle =
{
    IRQ_NUM_7,                            /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ7,         /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ7,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ7,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ7,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq7,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ7.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ7,                   /* I/O port input data bit mask for this IRQ. */
};
#endif
#endif

#if defined(VECT_ICU_IRQ15)
#if IRQ_CFG_USE_IRQ8 == 1
irq_callback p_callback_irq8;
const irq_init_block_t g_irq8_handle =
{
    IRQ_NUM_8,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ8,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ8,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ8,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ8,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq8,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ8.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ8,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ9 == 1
irq_callback p_callback_irq9;
const irq_init_block_t g_irq9_handle =
{
    IRQ_NUM_9,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ9,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ9,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ9,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ9,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq9,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ9.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ9,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ10 == 1
irq_callback p_callback_irq10;
const irq_init_block_t g_irq10_handle =
{
    IRQ_NUM_10,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ10,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ10,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ10,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ10,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq10,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ10.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ10,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ11 == 1
irq_callback p_callback_irq11;
const irq_init_block_t g_irq11_handle =
{
    IRQ_NUM_11,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ11,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ11,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ11,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ11,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq11,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ11.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ11,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ12 == 1
irq_callback p_callback_irq12;
const irq_init_block_t g_irq12_handle =
{
    IRQ_NUM_12,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ12,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ12,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ12,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ12,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq12,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ12.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ12,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ13 == 1
irq_callback p_callback_irq13;
const irq_init_block_t g_irq13_handle =
{
    IRQ_NUM_13,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ13,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ13,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ13,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ13,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq13,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ13.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ13,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ14 == 1
irq_callback p_callback_irq14;
const irq_init_block_t g_irq14_handle =
{
    IRQ_NUM_14,                           /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ14,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ14,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ14,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ14,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq14,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ14.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ14,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ15 == 1
irq_callback p_callback_irq15;
const irq_init_block_t g_irq15_handle =
{
    IRQ_NUM_15,                            /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ15,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IER_ICU_IRQ15,               /* Index to the Interrupt enable register location for this interrupt. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ15,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ15,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq15,                    /* pointer to callback function pointer. */
    (uint8_t *)&IRQ_PORT_IRQ15.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ15,                   /* I/O port input data bit mask for this IRQ. */
};
#endif
#endif /* end defined(VECT_ICU_IRQ15) */

static const irq_handle_t girq_handles[] =
{
    #if IRQ_CFG_USE_IRQ0 == 1
    /* irq handle */
    (irq_handle_t)&g_irq0_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ1 == 1
    /* irq handle */
    (irq_handle_t)&g_irq1_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ2 == 1
    /* irq handle */
    (irq_handle_t)&g_irq2_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ3 == 1
    /* irq handle */
    (irq_handle_t)&g_irq3_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ4 == 1
    /* irq handle */
    (irq_handle_t)&g_irq4_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ5 == 1
    /* irq handle */
    (irq_handle_t)&g_irq5_handle,
    #else
    NULL,
    #endif
#if defined(VECT_ICU_IRQ7)
    #if IRQ_CFG_USE_IRQ6 == 1
    /* irq handle */
    (irq_handle_t)&g_irq6_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ7 == 1
    /* irq handle */
    (irq_handle_t)&g_irq7_handle,
    #else
    NULL,
    #endif
#endif
#if defined(VECT_ICU_IRQ15)
    #if IRQ_CFG_USE_IRQ8 == 1
    /* irq handle */
    (irq_handle_t)&g_irq8_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ9 == 1
    /* irq handle */
    (irq_handle_t)&g_irq9_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ10 == 1
    /* irq handle */
    (irq_handle_t)&g_irq10_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ11 == 1
    /* irq handle */
    (irq_handle_t)&g_irq11_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ12 == 1
    /* irq handle */
    (irq_handle_t)&g_irq12_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ13 == 1
    /* irq handle */
    (irq_handle_t)&g_irq13_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ14 == 1
    /* irq handle */
    (irq_handle_t)&g_irq14_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ15 == 1
    /* irq handle */
    (irq_handle_t)&g_irq15_handle,
    #else
    NULL,
    #endif
#endif /* end defined(VECT_ICU_IRQ15) */
};

#if IRQ_CFG_PARAM_CHECKING == 1
static uint8_t girq_opened[IRQ_NUM_MAX] = {0};
#endif

/***********************************************************************************************************************
Function definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IRQ_Open
********************************************************************************************************************//**
 * @brief This function initializes the associated IRQ registers, enables interrupts, and provides the handle for use
 * with other API functions. This function must be called before calling any other API functions.
 * @param[in] irq_number Number of the IRQ to be initialized.
 * @param[in] trigger Enumerated type for trigger type: low level, rising edge, falling edge, both edges.
 * @param[in] priority Enumerated priority level setting for the IRQ.
 * @param[in] phandle Pointer to a location for handle for IRQ. Handle value will be set by this function.
 * @param[in] pcallback Pointer to function called from interrupt.
 * @retval IRQ_SUCCESS: Successful; IRQ initialized
 * @retval IRQ_ERR_BAD_NUM: IRQ number is invalid or unavailable
 * @retval IRQ_ERR_NOT_CLOSED: IRQ currently in operation; Perform R_IRQ_Close() first
 * @retval IRQ_ERR_INVALID_PTR: phandle pointer is NULL
 * @retval IRQ_ERR_INVALID_ARG: An invalid argument value was passed.
 * @retval IRQ_ERR_LOCK: The lock could not be acquired.
 * @details The Open function is responsible for preparing an IRQ for operation. After completion of the Open function
 * the IRQ shall be enabled and ready to service interrupts. This function must be called once prior to calling any other
 * IRQ API functions. Once successfully completed, the status of the selected IRQ will be set to "open". After that this
 * function should not be called again for the same IRQ without first performing a "close" by calling R_IRQ_Close().
 * @note
 * None.
 */
irq_err_t   R_IRQ_Open (irq_number_t     irq_number,
                        irq_trigger_t    trigger,
                        irq_prio_t       priority,
                        irq_handle_t    *phandle,
                        void        (*const pcallback)(void *pargs))
{
    irq_err_t ret = IRQ_SUCCESS;
    irq_handle_t  my_handle;
    uint8_t ier_index;
    uint8_t ien_mask;

#if IRQ_CFG_REQUIRE_LOCK == 1
    bool        lock_result = false;
#endif

#if IRQ_CFG_PARAM_CHECKING == 1
    if ((IRQ_NUM_MAX <= irq_number) || (NULL == girq_handles[irq_number]))
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (girq_opened[irq_number]) /* Check for IRQ already opened. */
    {
        return  IRQ_ERR_NOT_CLOSED;
    }
    if ((trigger & IRQ_TRIGGER_MASK) || (priority > BSP_MCU_IPL_MAX))
    {
        return IRQ_ERR_INVALID_ARG;
    }

    /* check argument phandle, pcallback  */
    if (((NULL == phandle) || (NULL == pcallback)) || (FIT_NO_FUNC == pcallback))
    {
        return IRQ_ERR_INVALID_PTR;
    }
#endif

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Attempt to acquire lock for this IRQ. Prevents reentrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_IRQ0 + (uint8_t)irq_number));

    if(false == lock_result)
    {
        return IRQ_ERR_LOCK; /* The open function is currently locked. */
    }
#endif

    my_handle = girq_handles[irq_number];
    ier_index = my_handle->ier_reg_index;
    ien_mask = my_handle->ien_bit_mask;

    /* Disable interrupt request. */
    ICU.IER[ier_index].BYTE &= (~ien_mask);

#if defined(VECT_ICU_IRQ15)
    if (7 >= irq_number) /* IRQs 0-7 share these registers. */
    {
#endif
        /* Disable digital filter.  */
        ICU.IRQFLTE0.BYTE &= (~ien_mask);

        /* Set up the digital filter sampling clock divisor IRQFLTC0.FCLKSELi[1:0] bits for the IRQ. */
        ICU.IRQFLTC0.WORD &= (uint16_t)~(0x0003 << (irq_number * 2));                   /* First clear them. */
        ICU.IRQFLTC0.WORD |= ((uint16_t)(my_handle->filt_clk_div) << (irq_number * 2)); /* Now set them. */
#if defined(VECT_ICU_IRQ15)
    }

    else  /* IRQs 8-15 share these registers. */
    {
         /* Disable digital filter.  */
        ICU.IRQFLTE1.BYTE &= (~ien_mask);

        /* Set up the digital filter sampling clock divisor IRQFLTC0.FCLKSELi[1:0] bits for the IRQ. */
        ICU.IRQFLTC1.WORD &= (uint16_t)~(0x0003 << ((irq_number - 8) * 2));             /* First clear them. */
        ICU.IRQFLTC1.WORD |= ((uint16_t)(my_handle->filt_clk_div) << ((irq_number - 8) * 2)); /* Now set them. */
    }
#endif

    /* Set the method of detection for the interrupt in the IRQCRi.IRQMD[1:0] bits. */
    ICU.IRQCR[irq_number].BYTE = (uint8_t)trigger;


    if(IRQ_TRIG_LOWLEV != trigger)
    {
        /* Clear the corresponding IRn.IR flag to 0 (if edge detection is in use). */
        ICU.IR[IRQ_VECT_BASE + irq_number].BYTE = 0;
    }

#if defined(VECT_ICU_IRQ15)
    if (7 >= irq_number) /* IRQs 0-7. */
    {
#endif
        /* Set the IRQFLTEn.FLTENi bit (digital filter enabled) according to user configuration. */
        ICU.IRQFLTE0.BYTE |= (uint8_t)(((my_handle->filt_enable) << irq_number) & ien_mask);
#if defined(VECT_ICU_IRQ15)
    }
    else
    {
        /* Set the IRQFLTEn.FLTENi bit (digital filter enabled) according to user configuration. */
        ICU.IRQFLTE1.BYTE |= (uint8_t)(((my_handle->filt_enable) << (irq_number - 8)) & ien_mask);
    }
#endif

    /* Set the interrupt priority level. */
    ICU.IPR[IRQ_IPR_BASE + (uint8_t)irq_number].BYTE = (uint8_t)(priority & 0x0F);

    *(my_handle->pirq_callback) = pcallback; /* Assign the callback function pointer. */

    *phandle = my_handle;   /* Return a pointer to a handle for the IRQ. */

#if IRQ_CFG_PARAM_CHECKING == 1
    girq_opened[irq_number] = 1;      /* Flag that IRQ has now been opened. */
#endif

    /* Set the IERm.IENj bit to 1 (interrupt request enabled). */
    ICU.IER[ier_index].BYTE |= ien_mask;   /* Enable interrupt request. */

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Release lock for this IRQ. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_IRQ0 + (uint8_t)irq_number));
#endif
    return ret;
}
/***********************************************************************************************************************
End of function R_IRQ_Open
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IRQ_Control
********************************************************************************************************************//**
 * @brief The Control function is responsible for handling special hardware or software operations for the IRQ.
 * @param[in] handle Handle for the IRQ.
 * @param[in] cmd Enumerated command codes:\n
 * IRQ_CMD_SET_PRIO	- Changes the interrupt priority level.\n
 * IRQ_CMD_SET_TRIG - Changes the interrupt triggering mode.
 * @param[in] pcmd_data Pointer to the command-data structure parameter of type void that is used to reference the location
 * of any data specific to the command that is needed for its completion.
 * @retval IRQ_SUCCESS: Command successfully completed.
 * @retval IRQ_ERR_NOT_OPENED:	The IRQ has not been opened.  Perform R_IRQ_Open() first
 * @retval IRQ_ERR_BAD_NUM: 	IRQ number is invalid or unavailable
 * @retval IRQ_ERR_UNKNOWN_CMD: Control command is not recognized.
 * @retval IRQ_ERR_INVALID_PTR: pcmd_data pointer or handle is NULL
 * @retval IRQ_ERR_INVALID_ARG: An element of the pcmd_data structure contains an invalid value.
 * @retval IRQ_ERR_LOCK The lock could not be acquired
 * @details This function is responsible for handling special hardware or software operations for the IRQ . It takes
 * an IRQ handle to identify the selected IRQ, an enumerated command value to select the operation to be performed,
 * and a void pointer to a location that contains information or data required to complete the operation. This pointer
 * must point to storage that has been type-cast by the caller for the particular command using the appropriate type
 * provided in "r_irq_rx_if.h".
 * @note
 * None.
 */
irq_err_t   R_IRQ_Control(irq_handle_t  const handle,
                          irq_cmd_t     const cmd,
                          void               *pcmd_data)
{
    irq_err_t ret = IRQ_SUCCESS;
    irq_prio_t    *prio_cmd_dat;
    irq_trigger_t *ptrig_cmd_dat;
    uint8_t irq_number = 0;         /* Place for dereferenced IRQ number. */
    uint8_t ien_temp;               /* For saving interrupt enable state. */
    uint8_t ier_index;              /* For holding decoded IER register number index.*/

#if IRQ_CFG_REQUIRE_LOCK == 1
    bool        lock_result = false;
#endif

    /* Validate input arguments. */
#if IRQ_CFG_PARAM_CHECKING == 1
    if ((NULL == handle) || (NULL == pcmd_data))
    {
        return IRQ_ERR_INVALID_PTR;
    }

    if (IRQ_NUM_MAX <= handle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }

    if (!(girq_opened[handle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
    if (IRQ_CMD_LIST_END <= cmd)
    {
        return IRQ_ERR_UNKNOWN_CMD;
    }
#endif
    irq_number = handle->irq_num;
    ier_index  = handle->ier_reg_index;

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Attempt to acquire lock for this IRQ. Prevents reentrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));

    if(false == lock_result)
    {
        return IRQ_ERR_LOCK; /* The open function is currently locked. */
    }
#endif

    /* Save a copy so it can be restored later. */
    ien_temp = ICU.IER[ier_index].BYTE;

    /* Disable interrupt request. */
    ICU.IER[ier_index].BYTE &= (~(handle->ien_bit_mask));

    if(IRQ_CMD_SET_PRIO == cmd)
    {
        /* casting void * type to irq_prio_t* type is valid */
        prio_cmd_dat = (irq_prio_t*)pcmd_data;

        #if IRQ_CFG_PARAM_CHECKING == 1
        if ((*prio_cmd_dat) > BSP_MCU_IPL_MAX)
        {
            #if IRQ_CFG_REQUIRE_LOCK == 1
            /* Release lock for this IRQ. */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));
            #endif
            return IRQ_ERR_INVALID_ARG;
        }
        #endif

        /* Set the interrupt priority level. */
        ICU.IPR[IRQ_IPR_BASE + irq_number].BYTE = (uint8_t)((*prio_cmd_dat) & 0x0F);
    }
    else if(IRQ_CMD_SET_TRIG == cmd)
    {
        /* casting void * type to irq_trigger_t* type is valid */
        ptrig_cmd_dat = (irq_trigger_t*)pcmd_data;

        #if IRQ_CFG_PARAM_CHECKING == 1
        if ((*ptrig_cmd_dat) & IRQ_TRIGGER_MASK)
        {
            #if IRQ_CFG_REQUIRE_LOCK == 1
            /* Release lock for this IRQ. */
            R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));
            #endif
            return IRQ_ERR_INVALID_ARG;
        }
        #endif

        /* Set the method of detection for the interrupt in the IRQCRi.IRQMD[1:0] bits. */
        ICU.IRQCR[irq_number].BYTE = (uint8_t)(*ptrig_cmd_dat);


        if(IRQ_TRIG_LOWLEV != (*ptrig_cmd_dat))
        {
            /* Clear the corresponding IRn.IR flag to 0 (if edge detection is in use). */
            ICU.IR[IRQ_VECT_BASE + irq_number].BYTE = 0;
        }
    }
    else
    {
     /* Nothing else. New commands would go here. */
    }

    /* Restore original interrupt enable state. */
    ICU.IER[ier_index].BYTE = ien_temp;

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Release lock for this IRQ. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));
#endif
    return ret;
}
/***********************************************************************************************************************
End of function R_IRQ_Control
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_IRQ_Close
********************************************************************************************************************//**
 * @brief Fully disables the IRQ designated by the handle.
 * @param[in] handle Handle for the IRQ.
 * @retval IRQ_SUCCESS: Successful; IRQ closed
 * @retval IRQ_ERR_NOT_OPENED: The IRQ has not been opened so closing has no effect.
 * @retval IRQ_ERR_BAD_NUM: IRQ number is invalid or unavailable.
 * @retval IRQ_ERR_INVALID_PTR: A required pointer argument is NULL.
 * @details This function frees the IRQ by clearing its assignment to a port, and disables the associated interrupts.
 * The IRQ handle is modified to indicate that it is no longer in the 'open' state. The IRQ cannot be used again
 * until it has been reopened with the R_IRQ_Open function. If this function is called for an IRQ that is not in the
 * open state then an error code is returned.
 * @note
 * None.
 */
irq_err_t   R_IRQ_Close(irq_handle_t handle)
{
    irq_err_t ret = IRQ_SUCCESS;

    /* Validate input arguments. */
#if IRQ_CFG_PARAM_CHECKING == 1
    if (NULL == handle)
    {
        return IRQ_ERR_INVALID_PTR;
    }
    if (IRQ_NUM_MAX <= handle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(girq_opened[handle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif

    /* Disable interrupt request. */
    ICU.IER[handle->ier_reg_index].BYTE &= (~(handle->ien_bit_mask));

#if defined(VECT_ICU_IRQ15)
    if (7 >= handle->irq_num) /* IRQs 0-7 share these registers. */
    {
#endif
        /* Disable digital filter.  */
        ICU.IRQFLTE0.BYTE &= (~(handle->ien_bit_mask));
#if defined(VECT_ICU_IRQ15)
    }

    else  /* IRQs 8-15 share these registers. */
    {
        /* Disable digital filter.  */
        ICU.IRQFLTE1.BYTE &= (~(handle->ien_bit_mask));
    }
#endif

#if IRQ_CFG_PARAM_CHECKING == 1
    girq_opened[handle->irq_num] = 0;      /* Flag that IRQ has now been closed. */
#endif

    return ret;
}
/***********************************************************************************************************************
End of function R_IRQ_Close
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IRQ_ReadInput
********************************************************************************************************************//**
 * @brief This function reads the current level of the pin assigned to the specified IRQ.
 * @param[in] handle Handle for the IRQ.
 * @param[in] plevel Pointer to location where the input pin state can be returned.
 * @retval IRQ_SUCCESS: Operation successfully completed.
 * @retval IRQ_ERR_NOT_OPENED: The IRQ has not been opened.  Perform R_IRQ_Open() first.
 * @retval IRQ_ERR_BAD_NUM: IRQ number is invalid or unavailable.
 * @retval IRQ_ERR_INVALID_PTR: plevel data pointer or handle is NULL.
 * @details This function reads the current level of the pin assigned to the specified IRQ. This is a realtime read which
 * may indicate a different value than the level that initially triggered an interrupt. One example use is for cases in
 * which a switch has triggered an interrupt and then needs to be polled for debounce.
 * @note
 * None.
 */
irq_err_t   R_IRQ_ReadInput(irq_handle_t const handle, uint8_t *plevel)
{
    irq_err_t ret = IRQ_SUCCESS;

    /* Validate input arguments. */
#if IRQ_CFG_PARAM_CHECKING == 1
    if ((NULL == handle) || (NULL == plevel))
    {
        return IRQ_ERR_INVALID_PTR;
    }
    if (IRQ_NUM_MAX <= handle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(girq_opened[handle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif

    /* casting handle * type to uint8_t* type is valid */
    *plevel = (uint8_t)((*handle->pirq_in_port) & handle->irq_port_bit);

    return ret;
}
/***********************************************************************************************************************
End of function R_IRQ_ReadInput
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IRQ_InterruptEnable
********************************************************************************************************************//**
 * @brief This function enables or disables the ICU interrupt for the specified IRQ.
 * @param[in] handle Handle for the IRQ.
 * @param[in] enable true  = enable the interrupt.\n
 * false = disable interrupt.
 * @retval IRQ_SUCCESS: Operation successfully completed.
 * @retval IRQ_ERR_NOT_OPENED: The IRQ has not been opened.  Perform R_IRQ_Open() first.
 * @retval IRQ_ERR_BAD_NUM: IRQ number is invalid or unavailable.
 * @retval IRQ_ERR_INVALID_PTR: handle is NULL.
 * @details The function enables or disables the ICU interrupt for the IRQ specified by the handle argument. This function
 * is potentially called frequently and is expected to execute quickly.
 * @note
 * None.
 */
irq_err_t   R_IRQ_InterruptEnable (irq_handle_t const handle, bool enable)
{
    irq_err_t ret = IRQ_SUCCESS;

    /* Validate input arguments. */
#if IRQ_CFG_PARAM_CHECKING == 1
    if (NULL == handle)
    {
        return IRQ_ERR_INVALID_PTR;
    }
    if (IRQ_NUM_MAX <= handle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(girq_opened[handle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif

    if(enable)
    {
        /* Enable interrupt request. */
        ICU.IER[handle->ier_reg_index].BYTE |= handle->ien_bit_mask;
    }
    else
    {
        /* Disable interrupt request. */
        ICU.IER[handle->ier_reg_index].BYTE &= (~(handle->ien_bit_mask));
    }

    /* dummy read to complete pipelining. */
    if(ICU.IER[handle->ier_reg_index].BYTE)
    {
        R_BSP_NOP();
    }

    return ret;
}
/***********************************************************************************************************************
End of function R_IRQ_InterruptEnable
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_IRQ_GetVersion
********************************************************************************************************************//**
 * @brief This function returns the driver version number at runtime.
 * @return Version number with major and minor version digits packed into a single 32-bit value.
 * @details The function returns the version of this module. The version number is encoded such that the top 2 bytes are
 * the major version number and the bottom 2 bytes are the minor version number.
 * @note
 * None.
 */
uint32_t R_IRQ_GetVersion(void)
{
    uint32_t version_number = 0;

    /* Bring in major version number. */
    version_number = ((uint16_t)IRQ_RX_VERSION_MAJOR) << 16;

    /* Bring in minor version number. */
    version_number |= (uint16_t)IRQ_RX_VERSION_MINOR;
    return version_number;
}
/***********************************************************************************************************************
End of function R_IRQ_GetVersion
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IRQ_IRClear
********************************************************************************************************************//**
 * @brief This function clears the IR flag for the specified IRQ.
 * @param[in] handle Handle for the IRQ.
 * @retval IRQ_SUCCESS: Operation successfully completed.
 * @retval IRQ_ERR_NOT_OPENED: The IRQ has not been opened.  Perform R_IRQ_Open() first.
 * @retval IRQ_ERR_BAD_NUM: IRQ number is invalid or unavailable.
 * @retval IRQ_ERR_INVALID_PTR: handle is NULL.
 * @details The function clears the IR flag for the IRQ specified by the handle argument.
 * @note
 * The IR flag is cleared only when edge detection is used. \n
 * When the interrupt request destination is the DTC or DMAC, do not write 0 to the IR flag.
 */
irq_err_t   R_IRQ_IRClear (irq_handle_t  const handle)
{
    irq_err_t ret = IRQ_SUCCESS;

    /* Validate input arguments. */
#if IRQ_CFG_PARAM_CHECKING == 1
    if (NULL == handle)
    {
        return IRQ_ERR_INVALID_PTR;
    }
    if (IRQ_NUM_MAX <= handle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(girq_opened[handle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif

    /* Edge detection */
    if (IRQ_TRIG_LOWLEV != ICU.IRQCR[handle->irq_num].BIT.IRQMD)
    {
        /* Clear the corresponding IRn.IR flag to 0 */
        ICU.IR[IRQ_VECT_BASE + handle->irq_num].BYTE = 0;

        /* Check IR flag */
        if (0 != ICU.IR[IRQ_VECT_BASE + handle->irq_num].BYTE)
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
        
    return ret;
}
/***********************************************************************************************************************
End of function R_IRQ_IRClear
***********************************************************************************************************************/

/***********************************************************************************************************************
* Description  : IRQ interrupt handler routines.
***********************************************************************************************************************/
#if IRQ_CFG_USE_IRQ0 == 1
R_BSP_PRAGMA_INTERRUPT (irq0_isr,VECT(ICU, IRQ0))
/***********************************************************************************************************************
* Function Name: irq0_isr
* Description  : IRQ0 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq0_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq0_handle.pirq_callback))) && (NULL != (*(g_irq0_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq0_handle.pirq_callback))((void*)&(g_irq0_handle));
    }
} /* End of function irq0_isr */
#endif

#if IRQ_CFG_USE_IRQ1 == 1
R_BSP_PRAGMA_INTERRUPT (irq1_isr, VECT(ICU, IRQ1))
/***********************************************************************************************************************
* Function Name: irq1_isr
* Description  : IRQ1 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq1_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq1_handle.pirq_callback))) && (NULL != (*(g_irq1_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq1_handle.pirq_callback))((void*)&(g_irq1_handle.irq_num));
    }
} /* End of function irq1_isr */
#endif

#if IRQ_CFG_USE_IRQ2 == 1
R_BSP_PRAGMA_INTERRUPT (irq2_isr,VECT(ICU, IRQ2))
/***********************************************************************************************************************
* Function Name: irq2_isr
* Description  : IRQ2 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq2_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq2_handle.pirq_callback))) && (NULL != (*(g_irq2_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq2_handle.pirq_callback))((void*)&(g_irq2_handle.irq_num));
    }
} /* End of function irq2_isr */
#endif

#if IRQ_CFG_USE_IRQ3 == 1
R_BSP_PRAGMA_INTERRUPT (irq3_isr,VECT(ICU, IRQ3))
/***********************************************************************************************************************
* Function Name: irq3_isr
* Description  : IRQ3 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq3_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq3_handle.pirq_callback))) && (NULL != (*(g_irq3_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq3_handle.pirq_callback))((void*)&(g_irq3_handle.irq_num));
    }
} /* End of function irq3_isr */
#endif

#if IRQ_CFG_USE_IRQ4 == 1
R_BSP_PRAGMA_INTERRUPT (irq4_isr, VECT(ICU, IRQ4))
/***********************************************************************************************************************
* Function Name: irq4_isr
* Description  : IRQ4 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq4_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq4_handle.pirq_callback))) && (NULL != (*(g_irq4_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq4_handle.pirq_callback))((void*)&(g_irq4_handle.irq_num));
    }
} /* End of function irq4_isr */
#endif

#if IRQ_CFG_USE_IRQ5 == 1
R_BSP_PRAGMA_INTERRUPT (irq5_isr, VECT(ICU, IRQ5))
/***********************************************************************************************************************
* Function Name: irq5_isr
* Description  : IRQ5 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq5_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq5_handle.pirq_callback))) && (NULL != (*(g_irq5_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq5_handle.pirq_callback))((void*)&(g_irq5_handle.irq_num));
    }
} /* End of function irq5_isr */
#endif

#if defined(VECT_ICU_IRQ7)
#if IRQ_CFG_USE_IRQ6 == 1
R_BSP_PRAGMA_INTERRUPT (irq6_isr,VECT(ICU, IRQ6))
/***********************************************************************************************************************
* Function Name: irq6_isr
* Description  : IRQ6 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq6_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq6_handle.pirq_callback))) && (NULL != (*(g_irq6_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq6_handle.pirq_callback))((void*)&(g_irq6_handle.irq_num));
    }
} /* End of function irq6_isr */
#endif

#if IRQ_CFG_USE_IRQ7 == 1
R_BSP_PRAGMA_INTERRUPT (irq7_isr,VECT(ICU, IRQ7))
/***********************************************************************************************************************
* Function Name: irq7_isr
* Description  : IRQ7 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq7_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq7_handle.pirq_callback))) && (NULL != (*(g_irq7_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq7_handle.pirq_callback))((void*)&(g_irq7_handle.irq_num));
    }
} /* End of function irq7_isr */
#endif
#endif /* End of #if defined(VECT_ICU_IRQ7) */

#if defined(VECT_ICU_IRQ15)
#if IRQ_CFG_USE_IRQ8 == 1
R_BSP_PRAGMA_INTERRUPT (irq8_isr,VECT(ICU, IRQ8))
/***********************************************************************************************************************
* Function Name: irq8_isr
* Description  : IRQ8 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq8_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq8_handle.pirq_callback))) && (NULL != (*(g_irq8_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq8_handle.pirq_callback))((void*)&(g_irq8_handle.irq_num));
    }
} /* End of function irq8_isr */
#endif

#if IRQ_CFG_USE_IRQ9 == 1
R_BSP_PRAGMA_INTERRUPT (irq9_isr,VECT(ICU, IRQ9))
/***********************************************************************************************************************
* Function Name: irq9_isr
* Description  : IRQ9 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq9_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq9_handle.pirq_callback))) && (NULL != (*(g_irq9_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq9_handle.pirq_callback))((void*)&(g_irq9_handle.irq_num));
    }
} /* End of function irq9_isr */
#endif

#if IRQ_CFG_USE_IRQ10 == 1
R_BSP_PRAGMA_INTERRUPT (irq10_isr,VECT(ICU, IRQ10))
/***********************************************************************************************************************
* Function Name: irq10_isr
* Description  : IRQ10 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq10_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq10_handle.pirq_callback))) && (NULL != (*(g_irq10_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq10_handle.pirq_callback))((void*)&(g_irq10_handle.irq_num));
    }
} /* End of function irq10_isr */
#endif

#if IRQ_CFG_USE_IRQ11 == 1
R_BSP_PRAGMA_INTERRUPT (irq11_isr,VECT(ICU, IRQ11))
/***********************************************************************************************************************
* Function Name: irq11_isr
* Description  : IRQ11 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq11_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq11_handle.pirq_callback))) && (NULL != (*(g_irq11_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq11_handle.pirq_callback))((void*)&(g_irq11_handle.irq_num));
    }
} /* End of function irq11_isr */
#endif

#if IRQ_CFG_USE_IRQ12 == 1
R_BSP_PRAGMA_INTERRUPT (irq12_isr, VECT(ICU, IRQ12))
/***********************************************************************************************************************
* Function Name: irq12_isr
* Description  : IRQ12 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq12_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq12_handle.pirq_callback))) && (NULL != (*(g_irq12_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq12_handle.pirq_callback))((void*)&(g_irq12_handle.irq_num));
    }
} /* End of function irq12_isr */
#endif

#if IRQ_CFG_USE_IRQ13 == 1
R_BSP_PRAGMA_INTERRUPT (irq13_isr, VECT(ICU, IRQ13))
/***********************************************************************************************************************
* Function Name: irq13_isr
* Description  : IRQ13 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq13_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq13_handle.pirq_callback))) && (NULL != (*(g_irq13_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq13_handle.pirq_callback))((void*)&(g_irq13_handle.irq_num));
    }
} /* End of function irq13_isr */
#endif

#if IRQ_CFG_USE_IRQ14 == 1
R_BSP_PRAGMA_INTERRUPT (irq14_isr, VECT(ICU, IRQ14))
/***********************************************************************************************************************
* Function Name: irq14_isr
* Description  : IRQ14 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq14_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq14_handle.pirq_callback))) && (NULL != (*(g_irq14_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq14_handle.pirq_callback))((void*)&(g_irq14_handle.irq_num));
    }
} /* End of function irq14_isr */
#endif

#if IRQ_CFG_USE_IRQ15 == 1
R_BSP_PRAGMA_INTERRUPT (irq15_isr,VECT(ICU, IRQ15))
/***********************************************************************************************************************
* Function Name: irq15_isr
* Description  : IRQ15 callback function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
R_BSP_ATTRIB_INTERRUPT void irq15_isr(void)
{
    /* check callback address */
    if((FIT_NO_FUNC != (*(g_irq15_handle.pirq_callback))) && (NULL != (*(g_irq15_handle.pirq_callback))))
    {
        /* casting void * type to callback* type is valid */
        (*(g_irq15_handle.pirq_callback))((void*)&(g_irq15_handle.irq_num));
    }
}/* End of function irq15_isr */
#endif
#endif /*end defined(VECT_ICU_IRQ15) */

/***********************************************************************************************************************
end  r_irq_rx.c
***********************************************************************************************************************/
