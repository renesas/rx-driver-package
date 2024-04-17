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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : mcu_init.c
* Description  : Performs initialization common to all MCUs in this Group
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 28.02.2023 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Get specifics on this MCU. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* RX MCUs come in different packages and different pin counts.
 * Each bit of PORTm.PDR corresponds to each pin of port m; I/O direction can be specified in 1-bit units.
 * Each bit of PDR corresponding to port m that does not exist is reserved.
 * Also, each bit of PDR corresponding to P40 to P42, P44 to P46, PH0, PH4, and PE2 pins is reserved, because such pins
 * are input only.
 * Make settings of the reserved bit according to the description in section 20.4, Initialization of the Port Direction
 * Register (PDR). These values are then ORed into the direction registers to set non-existent pins as outputs or 
 * inputs, which can help save power.
 */
#if BSP_PACKAGE_PINS == 100
    /* Refer User's Manual: Hardware Table 20.4. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTN_NE_PIN_MASK     (0x00)
#elif   BSP_PACKAGE_PINS == 80
    /* Refer User's Manual: Hardware Table 20.5. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x18)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x0C)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x0E)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x17)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x23)
    #define BSP_PRV_PORTN_NE_PIN_MASK     (0x00)
#elif   BSP_PACKAGE_PINS == 64
    /* Refer User's Manual: Hardware Table 20.6. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x01)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x98)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x23)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x3B)
    #define BSP_PRV_PORTN_NE_PIN_MASK     (0x00)
#elif   BSP_PACKAGE_PINS == 48
    /* Refer User's Manual: Hardware Table 20.7. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x02)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x9C)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x33)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x3B)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x01)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x41)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x57)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x3B)
    #define BSP_PRV_PORTN_NE_PIN_MASK     (0x00)
#else /* (BSP_PACKAGE_PINS != 100)&&(BSP_PACKAGE_PINS != 80)&&(BSP_PACKAGE_PINS != 64)&&(BSP_PACKAGE_PINS != 48) */
    #error "ERROR - This package is not defined in mcu_init.c"
#endif /* BSP_PACKAGE_PINS == 100 */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: bsp_non_existent_port_init
* Description  : For MCUs that do not have the maximum number of pins for their group (e.g. MCU with 80 pins when
*                maximum is 100 pins) these 'non-existent' pins that are not bonded out need to be initialized to save
*                power.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void bsp_non_existent_port_init (void)
{
    /* OR in missing pin masks from above. */

    /* Set PORT0.PDR */
    PORT0.PDR.BYTE |= BSP_PRV_PORT0_NE_PIN_MASK;

    /* Set PORT1.PDR */
    PORT1.PDR.BYTE |= BSP_PRV_PORT1_NE_PIN_MASK;

    /* Set PORT2.PDR */
    PORT2.PDR.BYTE |= BSP_PRV_PORT2_NE_PIN_MASK;

    /* Set PORT3.PDR */
    PORT3.PDR.BYTE |= BSP_PRV_PORT3_NE_PIN_MASK;

    /* Set PORT4.PDR */
    PORT4.PDR.BYTE |= BSP_PRV_PORT4_NE_PIN_MASK;

    /* Set PORT5.PDR */
    PORT5.PDR.BYTE |= BSP_PRV_PORT5_NE_PIN_MASK;

    /* Set PORT6.PDR */
    PORT6.PDR.BYTE |= BSP_PRV_PORT6_NE_PIN_MASK;

    /* Set PORT7.PDR */
    PORT7.PDR.BYTE |= BSP_PRV_PORT7_NE_PIN_MASK;

    /* Set PORT8.PDR */
    PORT8.PDR.BYTE |= BSP_PRV_PORT8_NE_PIN_MASK;

    /* Set PORT9.PDR */
    PORT9.PDR.BYTE |= BSP_PRV_PORT9_NE_PIN_MASK;

    /* Set PORTA.PDR */
    PORTA.PDR.BYTE |= BSP_PRV_PORTA_NE_PIN_MASK;

    /* Set PORTB.PDR */
    PORTB.PDR.BYTE |= BSP_PRV_PORTB_NE_PIN_MASK;

    /* Set PORTD.PDR */
    PORTD.PDR.BYTE |= BSP_PRV_PORTD_NE_PIN_MASK;

    /* Set PORTE.PDR */
    PORTE.PDR.BYTE |= BSP_PRV_PORTE_NE_PIN_MASK;

    /* Set PORTN.PDR */
    PORTN.PDR.BYTE |= BSP_PRV_PORTN_NE_PIN_MASK;
} /* End of function bsp_non_existent_port_init() */

/***********************************************************************************************************************
* Function Name: bsp_tfu_init
* Description  : This function does the following as TFU initialization.
*                Set the unit and format of the input values in fixed-point sincos operations.
*                Set the format of the output values in fixed-point sincos operations.
*                Set the unit and format of the output values of atan calculations in fixed-point atanhypot_k 
*                operations.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
#if BSP_CFG_TFU_INITIALIZE_ENABLE == 1
void bsp_tfu_init(void)
{
    /* Set the unit and format of the input values in fixed-point sincos operations. */
    TFU.FXSCIOC.BIT.IUF = BSP_CFG_TFU_SINCOS_INPUT_UNIT_FORMAT;

    /* Set the format of the output values in fixed-point sincos operations. */
    TFU.FXSCIOC.BIT.OF = BSP_CFG_TFU_SINCOS_OUTPUT_FORMAT;

    /* Set the unit and format of the output values of atan calculations in fixed-point atanhypot_k operations. */
    TFU.FXATIOC.BIT.OUF = BSP_CFG_TFU_ATAN_OUTPUT_UNIT_FORMAT;
} /* End of function bsp_tfu_init() */
#endif
