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
* Copyright (C) 2013-2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_irq_rx_private.h
* Device(s)    : Renesas RX Family
* Tool-Chain   : Renesas RX Standard Toolchain
* H/W Platform :
* Description  : Private definitions for the RX FIT IRQ support module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 15.07.2013  1.00    First Release
*         : 10.02.2014  1.10    Changed revision to reflect support for more MCUs.
*         : 04.04.2014  1.20    Changed revision.
*         : 01.12.2015  1.30    Changed revision to 1.4
*         : 24.03.2015  1.40    Changed revision to 1.5
*         : 01.05.2015  1.50    Changed revision to 1.6
*         : 30.09.2015  1.70    Using the definition of VECT_ICU_IRQ7  to check the number of IRQs.
*         : 01.10.2015  1.80    Added support for the RX130 Group.
*         : 01.12.2015  1.90    Added support for the RX230 and the RX24T Groups.
*         : 15.03.2016  2.00    Added support for the RX65N Group.
*         : 02.09.2016  2.10    Added support for the RX24U Group.
*         : 21.07.2017  2.20    Added support for the RX130-512KB, RX65N-2MB.
*         : 28.09.2018  2.30    Added support for the RX66T
                                Supporting Smart Configurator
*         : 16.11.2018  2.31    Added XML document number
*         : 01.02.2019  2.40    Added support for RX72T, RX65N-64pin
*         : 20.05.2019  3.00    Added support for GNUC and ICCRX.
*         : 28.06.2019  3.10    Added support for RX23W.
*         : 15.08.2019  3.20    Added support for RX72M.
*         : 25.11.2019  3.30    Added support RX13T.
*         : 30.12.2019  3.40    Added support RX66N, RX72N.
*         : 31.03.2020  3.50    Added support RX23E-A.
*         : 30.06.2020  3.60    Changed revision to reflect demo upgrade.
*         : 31.03.2021  3.70    Added support RX671.
*         : 15.04.2021  3.80    Added support RX140.
*         : 13.09.2021  3.90    Added RX671 Demo.
*         : 14.03.2022  4.00    Added support RX66T-48Pin.
*         : 31.03.2022  4.10    Added support RX660.
***********************************************************************************************************************/
#ifndef R_IRQ_PRIVATE_H_
#define R_IRQ_PRIVATE_H_

#include "platform.h"
#include "r_irq_rx_config.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Version Number of API. */
#define IRQ_RX_VERSION_MAJOR           (4)
#define IRQ_RX_VERSION_MINOR           (10)

/* Bit position of interrupt enable bits in interrupt enable register. */
#define IRQ_IEN_MASK_IRQ0   (0x01)
#define IRQ_IEN_MASK_IRQ1   (0x02)
#define IRQ_IEN_MASK_IRQ2   (0x04)
#define IRQ_IEN_MASK_IRQ3   (0x08)
#define IRQ_IEN_MASK_IRQ4   (0x10)
#define IRQ_IEN_MASK_IRQ5   (0x20)

#if defined(VECT_ICU_IRQ7)
#define IRQ_IEN_MASK_IRQ6   (0x40)
#define IRQ_IEN_MASK_IRQ7   (0x80)
#endif

#if defined(VECT_ICU_IRQ15)
#define IRQ_IEN_MASK_IRQ8   (0x01)
#define IRQ_IEN_MASK_IRQ9   (0x02)
#define IRQ_IEN_MASK_IRQ10   (0x04)
#define IRQ_IEN_MASK_IRQ11   (0x08)
#define IRQ_IEN_MASK_IRQ12   (0x10)
#define IRQ_IEN_MASK_IRQ13   (0x20)
#define IRQ_IEN_MASK_IRQ14   (0x40)
#define IRQ_IEN_MASK_IRQ15   (0x80)
#endif


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef void(*irq_callback)(void *pargs);

typedef enum
{
    IRQ_BIT0 = 0x01,
    IRQ_BIT1 = 0x02,
    IRQ_BIT2 = 0x04,
    IRQ_BIT3 = 0x08,
    IRQ_BIT4 = 0x10,
    IRQ_BIT5 = 0x20,
    IRQ_BIT6 = 0x40,
    IRQ_BIT7 = 0x80,
} irq_8bit_mask_t;

/* Definition of  structure */
typedef struct irq_init_block_s
{
    irq_number_t const irq_num;
    uint8_t const ien_bit_mask;    /* Bit mask for the interrupt enable register bit for this IRQ. */
    uint8_t const ier_reg_index;   /* An index to the Interrupt enable register location for this interrupt. */
    uint8_t const filt_clk_div;    /* PCLK divisor setting for the input pin digital filter. */
    uint8_t const filt_enable;     /* Filter enable setting (on or off) for the input pin digital filter. */
    irq_callback *const pirq_callback; /* pointer to callback function pointer. */
    uint8_t const *pirq_in_port;    /* Pointer to the I/O port input data register for this IRQ. */
    irq_8bit_mask_t irq_port_bit;  /* I/O port input data bit mask for this IRQ. */
} irq_init_block_t;


/******************************************************************************
Port and pin definitions for Smart Configurator
******************************************************************************/
#if ('0' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT0)
#elif ('1' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT1)
#elif ('2' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT2)
#elif ('3' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT3)
#elif ('4' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT4)
#elif ('5' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT5)
#elif ('6' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT6)
#elif ('7' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT7)
#elif ('8' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT8)
#elif ('9' == IRQ_PORT_IRQ0_PORT)
#define IRQ_PORT_IRQ0      (PORT9)
#elif (('A' == IRQ_PORT_IRQ0_PORT) || ('a' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTA)
#elif (('B' == IRQ_PORT_IRQ0_PORT) || ('b' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTB)
#elif (('C' == IRQ_PORT_IRQ0_PORT) || ('c' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTC)
#elif (('D' == IRQ_PORT_IRQ0_PORT) || ('d' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTD)
#elif (('E' == IRQ_PORT_IRQ0_PORT) || ('e' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTE)
#elif (('F' == IRQ_PORT_IRQ0_PORT) || ('f' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTF)
#elif (('G' == IRQ_PORT_IRQ0_PORT) || ('g' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTG)
#elif (('J' == IRQ_PORT_IRQ0_PORT) || ('j' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ0_PORT) || ('h' == IRQ_PORT_IRQ0_PORT))
#define IRQ_PORT_IRQ0      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ0_PORT */

#if ('0' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ0_BIT)
#define IRQ_PORT_BIT_IRQ0  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ0_BIT */

#if (defined(IRQ_PORT_IRQ0) && defined(IRQ_PORT_BIT_IRQ0))
#define IRQ_CFG_USE_IRQ0   (1)
#else
#define IRQ_CFG_USE_IRQ0   (0)
#endif  /* defined(IRQ_PORT_IRQ0) && defined(IRQ_PORT_BIT_IRQ0) */


#if ('0' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT0)
#elif ('1' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT1)
#elif ('2' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT2)
#elif ('3' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT3)
#elif ('4' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT4)
#elif ('5' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT5)
#elif ('6' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT6)
#elif ('7' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT7)
#elif ('8' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT8)
#elif ('9' == IRQ_PORT_IRQ1_PORT)
#define IRQ_PORT_IRQ1      (PORT9)
#elif (('A' == IRQ_PORT_IRQ1_PORT) || ('a' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTA)
#elif (('B' == IRQ_PORT_IRQ1_PORT) || ('b' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTB)
#elif (('C' == IRQ_PORT_IRQ1_PORT) || ('c' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTC)
#elif (('D' == IRQ_PORT_IRQ1_PORT) || ('d' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTD)
#elif (('E' == IRQ_PORT_IRQ1_PORT) || ('e' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTE)
#elif (('F' == IRQ_PORT_IRQ1_PORT) || ('f' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTF)
#elif (('G' == IRQ_PORT_IRQ1_PORT) || ('g' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTG)
#elif (('J' == IRQ_PORT_IRQ1_PORT) || ('j' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ1_PORT) || ('h' == IRQ_PORT_IRQ1_PORT))
#define IRQ_PORT_IRQ1      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ1_PORT */

#if ('0' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ1_BIT)
#define IRQ_PORT_BIT_IRQ1  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ1_BIT */

#if (defined(IRQ_PORT_IRQ1) && defined(IRQ_PORT_BIT_IRQ1))
#define IRQ_CFG_USE_IRQ1   (1)
#else
#define IRQ_CFG_USE_IRQ1   (0)
#endif  /* defined(IRQ_PORT_IRQ1) && defined(IRQ_PORT_BIT_IRQ1) */


#if ('0' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT0)
#elif ('1' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT1)
#elif ('2' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT2)
#elif ('3' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT3)
#elif ('4' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT4)
#elif ('5' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT5)
#elif ('6' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT6)
#elif ('7' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT7)
#elif ('8' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT8)
#elif ('9' == IRQ_PORT_IRQ2_PORT)
#define IRQ_PORT_IRQ2      (PORT9)
#elif (('A' == IRQ_PORT_IRQ2_PORT) || ('a' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTA)
#elif (('B' == IRQ_PORT_IRQ2_PORT) || ('b' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTB)
#elif (('C' == IRQ_PORT_IRQ2_PORT) || ('c' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTC)
#elif (('D' == IRQ_PORT_IRQ2_PORT) || ('d' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTD)
#elif (('E' == IRQ_PORT_IRQ2_PORT) || ('e' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTE)
#elif (('F' == IRQ_PORT_IRQ2_PORT) || ('f' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTF)
#elif (('G' == IRQ_PORT_IRQ2_PORT) || ('g' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTG)
#elif (('J' == IRQ_PORT_IRQ2_PORT) || ('j' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ2_PORT) || ('h' == IRQ_PORT_IRQ2_PORT))
#define IRQ_PORT_IRQ2      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ2_PORT */

#if ('0' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ2_BIT)
#define IRQ_PORT_BIT_IRQ2  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ2_BIT */

#if (defined(IRQ_PORT_IRQ2) && defined(IRQ_PORT_BIT_IRQ2))
#define IRQ_CFG_USE_IRQ2   (1)
#else
#define IRQ_CFG_USE_IRQ2   (0)
#endif  /* defined(IRQ_PORT_IRQ2) && defined(IRQ_PORT_BIT_IRQ2) */


#if ('0' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT0)
#elif ('1' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT1)
#elif ('2' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT2)
#elif ('3' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT3)
#elif ('4' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT4)
#elif ('5' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT5)
#elif ('6' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT6)
#elif ('7' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT7)
#elif ('8' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT8)
#elif ('9' == IRQ_PORT_IRQ3_PORT)
#define IRQ_PORT_IRQ3      (PORT9)
#elif (('A' == IRQ_PORT_IRQ3_PORT) || ('a' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTA)
#elif (('B' == IRQ_PORT_IRQ3_PORT) || ('b' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTB)
#elif (('C' == IRQ_PORT_IRQ3_PORT) || ('c' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTC)
#elif (('D' == IRQ_PORT_IRQ3_PORT) || ('d' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTD)
#elif (('E' == IRQ_PORT_IRQ3_PORT) || ('e' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTE)
#elif (('F' == IRQ_PORT_IRQ3_PORT) || ('f' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTF)
#elif (('G' == IRQ_PORT_IRQ3_PORT) || ('g' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTG)
#elif (('J' == IRQ_PORT_IRQ3_PORT) || ('j' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ3_PORT) || ('h' == IRQ_PORT_IRQ3_PORT))
#define IRQ_PORT_IRQ3      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ3_PORT */

#if ('0' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ3_BIT)
#define IRQ_PORT_BIT_IRQ3  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ3_BIT */

#if (defined(IRQ_PORT_IRQ3) && defined(IRQ_PORT_BIT_IRQ3))
#define IRQ_CFG_USE_IRQ3   (1)
#else
#define IRQ_CFG_USE_IRQ3   (0)
#endif  /* defined(IRQ_PORT_IRQ3) && defined(IRQ_PORT_BIT_IRQ3) */


#if ('0' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT0)
#elif ('1' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT1)
#elif ('2' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT2)
#elif ('3' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT3)
#elif ('4' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT4)
#elif ('5' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT5)
#elif ('6' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT6)
#elif ('7' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT7)
#elif ('8' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT8)
#elif ('9' == IRQ_PORT_IRQ4_PORT)
#define IRQ_PORT_IRQ4      (PORT9)
#elif (('A' == IRQ_PORT_IRQ4_PORT) || ('a' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTA)
#elif (('B' == IRQ_PORT_IRQ4_PORT) || ('b' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTB)
#elif (('C' == IRQ_PORT_IRQ4_PORT) || ('c' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTC)
#elif (('D' == IRQ_PORT_IRQ4_PORT) || ('d' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTD)
#elif (('E' == IRQ_PORT_IRQ4_PORT) || ('e' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTE)
#elif (('F' == IRQ_PORT_IRQ4_PORT) || ('f' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTF)
#elif (('G' == IRQ_PORT_IRQ4_PORT) || ('g' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTG)
#elif (('J' == IRQ_PORT_IRQ4_PORT) || ('j' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ4_PORT) || ('h' == IRQ_PORT_IRQ4_PORT))
#define IRQ_PORT_IRQ4      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ4_PORT */

#if ('0' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ4_BIT)
#define IRQ_PORT_BIT_IRQ4  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ4_BIT */

#if (defined(IRQ_PORT_IRQ4) && defined(IRQ_PORT_BIT_IRQ4))
#define IRQ_CFG_USE_IRQ4   (1)
#else
#define IRQ_CFG_USE_IRQ4   (0)
#endif  /* defined(IRQ_PORT_IRQ4) && defined(IRQ_PORT_BIT_IRQ4) */


#if ('0' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT0)
#elif ('1' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT1)
#elif ('2' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT2)
#elif ('3' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT3)
#elif ('4' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT4)
#elif ('5' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT5)
#elif ('6' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT6)
#elif ('7' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT7)
#elif ('8' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT8)
#elif ('9' == IRQ_PORT_IRQ5_PORT)
#define IRQ_PORT_IRQ5      (PORT9)
#elif (('A' == IRQ_PORT_IRQ5_PORT) || ('a' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTA)
#elif (('B' == IRQ_PORT_IRQ5_PORT) || ('b' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTB)
#elif (('C' == IRQ_PORT_IRQ5_PORT) || ('c' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTC)
#elif (('D' == IRQ_PORT_IRQ5_PORT) || ('d' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTD)
#elif (('E' == IRQ_PORT_IRQ5_PORT) || ('e' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTE)
#elif (('F' == IRQ_PORT_IRQ5_PORT) || ('f' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTF)
#elif (('G' == IRQ_PORT_IRQ5_PORT) || ('g' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTG)
#elif (('J' == IRQ_PORT_IRQ5_PORT) || ('j' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ5_PORT) || ('h' == IRQ_PORT_IRQ5_PORT))
#define IRQ_PORT_IRQ5      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ5_PORT */

#if ('0' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ5_BIT)
#define IRQ_PORT_BIT_IRQ5  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ5_BIT */

#if (defined(IRQ_PORT_IRQ5) && defined(IRQ_PORT_BIT_IRQ5))
#define IRQ_CFG_USE_IRQ5   (1)
#else
#define IRQ_CFG_USE_IRQ5   (0)
#endif  /* defined(IRQ_PORT_IRQ5) && defined(IRQ_PORT_BIT_IRQ5) */


#if ('0' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT0)
#elif ('1' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT1)
#elif ('2' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT2)
#elif ('3' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT3)
#elif ('4' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT4)
#elif ('5' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT5)
#elif ('6' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT6)
#elif ('7' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT7)
#elif ('8' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT8)
#elif ('9' == IRQ_PORT_IRQ6_PORT)
#define IRQ_PORT_IRQ6      (PORT9)
#elif (('A' == IRQ_PORT_IRQ6_PORT) || ('a' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTA)
#elif (('B' == IRQ_PORT_IRQ6_PORT) || ('b' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTB)
#elif (('C' == IRQ_PORT_IRQ6_PORT) || ('c' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTC)
#elif (('D' == IRQ_PORT_IRQ6_PORT) || ('d' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTD)
#elif (('E' == IRQ_PORT_IRQ6_PORT) || ('e' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTE)
#elif (('F' == IRQ_PORT_IRQ6_PORT) || ('f' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTF)
#elif (('G' == IRQ_PORT_IRQ6_PORT) || ('g' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTG)
#elif (('J' == IRQ_PORT_IRQ6_PORT) || ('j' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ6_PORT) || ('h' == IRQ_PORT_IRQ6_PORT))
#define IRQ_PORT_IRQ6      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ6_PORT */

#if ('0' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ6_BIT)
#define IRQ_PORT_BIT_IRQ6  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ6_BIT */

#if (defined(IRQ_PORT_IRQ6) && defined(IRQ_PORT_BIT_IRQ6))
#define IRQ_CFG_USE_IRQ6   (1)
#else
#define IRQ_CFG_USE_IRQ6   (0)
#endif  /* defined(IRQ_PORT_IRQ6) && defined(IRQ_PORT_BIT_IRQ6) */


#if ('0' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT0)
#elif ('1' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT1)
#elif ('2' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT2)
#elif ('3' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT3)
#elif ('4' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT4)
#elif ('5' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT5)
#elif ('6' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT6)
#elif ('7' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT7)
#elif ('8' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT8)
#elif ('9' == IRQ_PORT_IRQ7_PORT)
#define IRQ_PORT_IRQ7      (PORT9)
#elif (('A' == IRQ_PORT_IRQ7_PORT) || ('a' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTA)
#elif (('B' == IRQ_PORT_IRQ7_PORT) || ('b' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTB)
#elif (('C' == IRQ_PORT_IRQ7_PORT) || ('c' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTC)
#elif (('D' == IRQ_PORT_IRQ7_PORT) || ('d' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTD)
#elif (('E' == IRQ_PORT_IRQ7_PORT) || ('e' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTE)
#elif (('F' == IRQ_PORT_IRQ7_PORT) || ('f' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTF)
#elif (('G' == IRQ_PORT_IRQ7_PORT) || ('g' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTG)
#elif (('J' == IRQ_PORT_IRQ7_PORT) || ('j' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ7_PORT) || ('h' == IRQ_PORT_IRQ7_PORT))
#define IRQ_PORT_IRQ7      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ7_PORT */

#if ('0' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ7_BIT)
#define IRQ_PORT_BIT_IRQ7  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ7_BIT */

#if (defined(IRQ_PORT_IRQ7) && defined(IRQ_PORT_BIT_IRQ7))
#define IRQ_CFG_USE_IRQ7   (1)
#else
#define IRQ_CFG_USE_IRQ7   (0)
#endif  /* defined(IRQ_PORT_IRQ7) && defined(IRQ_PORT_BIT_IRQ7) */


#if ('0' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT0)
#elif ('1' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT1)
#elif ('2' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT2)
#elif ('3' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT3)
#elif ('4' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT4)
#elif ('5' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT5)
#elif ('6' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT6)
#elif ('7' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT7)
#elif ('8' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT8)
#elif ('9' == IRQ_PORT_IRQ8_PORT)
#define IRQ_PORT_IRQ8      (PORT9)
#elif (('A' == IRQ_PORT_IRQ8_PORT) || ('a' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTA)
#elif (('B' == IRQ_PORT_IRQ8_PORT) || ('b' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTB)
#elif (('C' == IRQ_PORT_IRQ8_PORT) || ('c' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTC)
#elif (('D' == IRQ_PORT_IRQ8_PORT) || ('d' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTD)
#elif (('E' == IRQ_PORT_IRQ8_PORT) || ('e' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTE)
#elif (('F' == IRQ_PORT_IRQ8_PORT) || ('f' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTF)
#elif (('G' == IRQ_PORT_IRQ8_PORT) || ('g' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTG)
#elif (('J' == IRQ_PORT_IRQ8_PORT) || ('j' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ8_PORT) || ('h' == IRQ_PORT_IRQ8_PORT))
#define IRQ_PORT_IRQ8      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ8_PORT */

#if ('0' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ8_BIT)
#define IRQ_PORT_BIT_IRQ8  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ8_BIT */

#if (defined(IRQ_PORT_IRQ8) && defined(IRQ_PORT_BIT_IRQ8))
#define IRQ_CFG_USE_IRQ8   (1)
#else
#define IRQ_CFG_USE_IRQ8   (0)
#endif  /* defined(IRQ_PORT_IRQ8) && defined(IRQ_PORT_BIT_IRQ8) */


#if ('0' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT0)
#elif ('1' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT1)
#elif ('2' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT2)
#elif ('3' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT3)
#elif ('4' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT4)
#elif ('5' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT5)
#elif ('6' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT6)
#elif ('7' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT7)
#elif ('8' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT8)
#elif ('9' == IRQ_PORT_IRQ9_PORT)
#define IRQ_PORT_IRQ9      (PORT9)
#elif (('A' == IRQ_PORT_IRQ9_PORT) || ('a' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTA)
#elif (('B' == IRQ_PORT_IRQ9_PORT) || ('b' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTB)
#elif (('C' == IRQ_PORT_IRQ9_PORT) || ('c' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTC)
#elif (('D' == IRQ_PORT_IRQ9_PORT) || ('d' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTD)
#elif (('E' == IRQ_PORT_IRQ9_PORT) || ('e' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTE)
#elif (('F' == IRQ_PORT_IRQ9_PORT) || ('f' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTF)
#elif (('G' == IRQ_PORT_IRQ9_PORT) || ('g' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTG)
#elif (('J' == IRQ_PORT_IRQ9_PORT) || ('j' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ9_PORT) || ('h' == IRQ_PORT_IRQ9_PORT))
#define IRQ_PORT_IRQ9      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ9_PORT */

#if ('0' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ9_BIT)
#define IRQ_PORT_BIT_IRQ9  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ9_BIT */

#if (defined(IRQ_PORT_IRQ9) && defined(IRQ_PORT_BIT_IRQ9))
#define IRQ_CFG_USE_IRQ9   (1)
#else
#define IRQ_CFG_USE_IRQ9   (0)
#endif  /* defined(IRQ_PORT_IRQ9) && defined(IRQ_PORT_BIT_IRQ9) */


#if ('0' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT0)
#elif ('1' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT1)
#elif ('2' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT2)
#elif ('3' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT3)
#elif ('4' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT4)
#elif ('5' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT5)
#elif ('6' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT6)
#elif ('7' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT7)
#elif ('8' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT8)
#elif ('9' == IRQ_PORT_IRQ10_PORT)
#define IRQ_PORT_IRQ10      (PORT9)
#elif (('A' == IRQ_PORT_IRQ10_PORT) || ('a' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTA)
#elif (('B' == IRQ_PORT_IRQ10_PORT) || ('b' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTB)
#elif (('C' == IRQ_PORT_IRQ10_PORT) || ('c' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTC)
#elif (('D' == IRQ_PORT_IRQ10_PORT) || ('d' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTD)
#elif (('E' == IRQ_PORT_IRQ10_PORT) || ('e' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTE)
#elif (('F' == IRQ_PORT_IRQ10_PORT) || ('f' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTF)
#elif (('G' == IRQ_PORT_IRQ10_PORT) || ('g' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTG)
#elif (('J' == IRQ_PORT_IRQ10_PORT) || ('j' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ10_PORT) || ('h' == IRQ_PORT_IRQ10_PORT))
#define IRQ_PORT_IRQ10      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ10_PORT */

#if ('0' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ10_BIT)
#define IRQ_PORT_BIT_IRQ10  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ10_BIT */

#if (defined(IRQ_PORT_IRQ10) && defined(IRQ_PORT_BIT_IRQ10))
#define IRQ_CFG_USE_IRQ10   (1)
#else
#define IRQ_CFG_USE_IRQ10   (0)
#endif  /* defined(IRQ_PORT_IRQ10) && defined(IRQ_PORT_BIT_IRQ10) */


#if ('0' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT0)
#elif ('1' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT1)
#elif ('2' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT2)
#elif ('3' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT3)
#elif ('4' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT4)
#elif ('5' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT5)
#elif ('6' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT6)
#elif ('7' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT7)
#elif ('8' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT8)
#elif ('9' == IRQ_PORT_IRQ11_PORT)
#define IRQ_PORT_IRQ11      (PORT9)
#elif (('A' == IRQ_PORT_IRQ11_PORT) || ('a' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTA)
#elif (('B' == IRQ_PORT_IRQ11_PORT) || ('b' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTB)
#elif (('C' == IRQ_PORT_IRQ11_PORT) || ('c' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTC)
#elif (('D' == IRQ_PORT_IRQ11_PORT) || ('d' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTD)
#elif (('E' == IRQ_PORT_IRQ11_PORT) || ('e' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTE)
#elif (('F' == IRQ_PORT_IRQ11_PORT) || ('f' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTF)
#elif (('G' == IRQ_PORT_IRQ11_PORT) || ('g' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTG)
#elif (('J' == IRQ_PORT_IRQ11_PORT) || ('j' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ11_PORT) || ('h' == IRQ_PORT_IRQ11_PORT))
#define IRQ_PORT_IRQ11      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ11_PORT */

#if ('0' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ11_BIT)
#define IRQ_PORT_BIT_IRQ11  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ11_BIT */

#if (defined(IRQ_PORT_IRQ11) && defined(IRQ_PORT_BIT_IRQ11))
#define IRQ_CFG_USE_IRQ11   (1)
#else
#define IRQ_CFG_USE_IRQ11   (0)
#endif  /* defined(IRQ_PORT_IRQ11) && defined(IRQ_PORT_BIT_IRQ11) */


#if ('0' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT0)
#elif ('1' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT1)
#elif ('2' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT2)
#elif ('3' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT3)
#elif ('4' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT4)
#elif ('5' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT5)
#elif ('6' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT6)
#elif ('7' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT7)
#elif ('8' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT8)
#elif ('9' == IRQ_PORT_IRQ12_PORT)
#define IRQ_PORT_IRQ12      (PORT9)
#elif (('A' == IRQ_PORT_IRQ12_PORT) || ('a' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTA)
#elif (('B' == IRQ_PORT_IRQ12_PORT) || ('b' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTB)
#elif (('C' == IRQ_PORT_IRQ12_PORT) || ('c' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTC)
#elif (('D' == IRQ_PORT_IRQ12_PORT) || ('d' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTD)
#elif (('E' == IRQ_PORT_IRQ12_PORT) || ('e' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTE)
#elif (('F' == IRQ_PORT_IRQ12_PORT) || ('f' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTF)
#elif (('G' == IRQ_PORT_IRQ12_PORT) || ('g' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTG)
#elif (('J' == IRQ_PORT_IRQ12_PORT) || ('j' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ12_PORT) || ('h' == IRQ_PORT_IRQ12_PORT))
#define IRQ_PORT_IRQ12      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ12_PORT */

#if ('0' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ12_BIT)
#define IRQ_PORT_BIT_IRQ12  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ12_BIT */

#if (defined(IRQ_PORT_IRQ12) && defined(IRQ_PORT_BIT_IRQ12))
#define IRQ_CFG_USE_IRQ12   (1)
#else
#define IRQ_CFG_USE_IRQ12   (0)
#endif  /* defined(IRQ_PORT_IRQ12) && defined(IRQ_PORT_BIT_IRQ12) */


#if ('0' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT0)
#elif ('1' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT1)
#elif ('2' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT2)
#elif ('3' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT3)
#elif ('4' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT4)
#elif ('5' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT5)
#elif ('6' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT6)
#elif ('7' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT7)
#elif ('8' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT8)
#elif ('9' == IRQ_PORT_IRQ13_PORT)
#define IRQ_PORT_IRQ13      (PORT9)
#elif (('A' == IRQ_PORT_IRQ13_PORT) || ('a' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTA)
#elif (('B' == IRQ_PORT_IRQ13_PORT) || ('b' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTB)
#elif (('C' == IRQ_PORT_IRQ13_PORT) || ('c' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTC)
#elif (('D' == IRQ_PORT_IRQ13_PORT) || ('d' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTD)
#elif (('E' == IRQ_PORT_IRQ13_PORT) || ('e' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTE)
#elif (('F' == IRQ_PORT_IRQ13_PORT) || ('f' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTF)
#elif (('G' == IRQ_PORT_IRQ13_PORT) || ('g' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTG)
#elif (('J' == IRQ_PORT_IRQ13_PORT) || ('j' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ13_PORT) || ('h' == IRQ_PORT_IRQ13_PORT))
#define IRQ_PORT_IRQ13      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ13_PORT */

#if ('0' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ13_BIT)
#define IRQ_PORT_BIT_IRQ13  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ13_BIT */

#if (defined(IRQ_PORT_IRQ13) && defined(IRQ_PORT_BIT_IRQ13))
#define IRQ_CFG_USE_IRQ13   (1)
#else
#define IRQ_CFG_USE_IRQ13   (0)
#endif  /* defined(IRQ_PORT_IRQ13) && defined(IRQ_PORT_BIT_IRQ13) */


#if ('0' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT0)
#elif ('1' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT1)
#elif ('2' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT2)
#elif ('3' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT3)
#elif ('4' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT4)
#elif ('5' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT5)
#elif ('6' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT6)
#elif ('7' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT7)
#elif ('8' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT8)
#elif ('9' == IRQ_PORT_IRQ14_PORT)
#define IRQ_PORT_IRQ14      (PORT9)
#elif (('A' == IRQ_PORT_IRQ14_PORT) || ('a' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTA)
#elif (('B' == IRQ_PORT_IRQ14_PORT) || ('b' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTB)
#elif (('C' == IRQ_PORT_IRQ14_PORT) || ('c' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTC)
#elif (('D' == IRQ_PORT_IRQ14_PORT) || ('d' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTD)
#elif (('E' == IRQ_PORT_IRQ14_PORT) || ('e' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTE)
#elif (('F' == IRQ_PORT_IRQ14_PORT) || ('f' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTF)
#elif (('G' == IRQ_PORT_IRQ14_PORT) || ('g' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTG)
#elif (('J' == IRQ_PORT_IRQ14_PORT) || ('j' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ14_PORT) || ('h' == IRQ_PORT_IRQ14_PORT))
#define IRQ_PORT_IRQ14      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ14_PORT */

#if ('0' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ14_BIT)
#define IRQ_PORT_BIT_IRQ14  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ14_BIT */

#if (defined(IRQ_PORT_IRQ14) && defined(IRQ_PORT_BIT_IRQ14))
#define IRQ_CFG_USE_IRQ14   (1)
#else
#define IRQ_CFG_USE_IRQ14   (0)
#endif  /* defined(IRQ_PORT_IRQ14) && defined(IRQ_PORT_BIT_IRQ14) */


#if ('0' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT0)
#elif ('1' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT1)
#elif ('2' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT2)
#elif ('3' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT3)
#elif ('4' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT4)
#elif ('5' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT5)
#elif ('6' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT6)
#elif ('7' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT7)
#elif ('8' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT8)
#elif ('9' == IRQ_PORT_IRQ15_PORT)
#define IRQ_PORT_IRQ15      (PORT9)
#elif (('A' == IRQ_PORT_IRQ15_PORT) || ('a' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTA)
#elif (('B' == IRQ_PORT_IRQ15_PORT) || ('b' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTB)
#elif (('C' == IRQ_PORT_IRQ15_PORT) || ('c' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTC)
#elif (('D' == IRQ_PORT_IRQ15_PORT) || ('d' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTD)
#elif (('E' == IRQ_PORT_IRQ15_PORT) || ('e' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTE)
#elif (('F' == IRQ_PORT_IRQ15_PORT) || ('f' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTF)
#elif (('G' == IRQ_PORT_IRQ15_PORT) || ('g' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTG)
#elif (('J' == IRQ_PORT_IRQ15_PORT) || ('j' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTJ)
#elif (('H' == IRQ_PORT_IRQ15_PORT) || ('h' == IRQ_PORT_IRQ15_PORT))
#define IRQ_PORT_IRQ15      (PORTH)
#endif  /* '0' == IRQ_PORT_IRQ15_PORT */

#if ('0' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT0)
#elif ('1' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT1)
#elif ('2' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT2)
#elif ('3' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT3)
#elif ('4' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT4)
#elif ('5' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT5)
#elif ('6' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT6)
#elif ('7' == IRQ_PORT_IRQ15_BIT)
#define IRQ_PORT_BIT_IRQ15  (IRQ_BIT7)
#endif  /* '0' == IRQ_PORT_IRQ15_BIT */

#if (defined(IRQ_PORT_IRQ15) && defined(IRQ_PORT_BIT_IRQ15))
#define IRQ_CFG_USE_IRQ15   (1)
#else
#define IRQ_CFG_USE_IRQ15   (0)
#endif  /* defined(IRQ_PORT_IRQ15) && defined(IRQ_PORT_BIT_IRQ15) */


#endif /* R_IRQ_PRIVATE_H_ */
