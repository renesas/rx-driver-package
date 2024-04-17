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
* File Name    : mcu_mapped_interrupts.c
* Description  : This module maps Interrupt A & B interrupts. Which interrupts are mapped depends on the macros in
*                r_bsp_interrupt_config.h.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 22.04.2022 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Access to r_bsp. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

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
* Function Name: bsp_mapped_interrupt_open
* Description  : Initializes mapped interrupts. This code does the following for each possible mapped interrupt:
*                1) PREPROCCESOR - Test to see if this interrupt is chosen to be used
*                2) PREPROCESSOR - Figure out which interrupt select register needs to be written to
*                3) RUNTIME C    - Set the appropriate select register with the number of this mapped interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void bsp_mapped_interrupt_open (void)
{
#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMT2_CMI2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMT2_CMI2) = BSP_PRV_INT_B_NUM_CMT2_CMI2;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMT3_CMI3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMT3_CMI3) = BSP_PRV_INT_B_NUM_CMT3_CMI3;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR0_CMIA0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR0_CMIA0) = BSP_PRV_INT_B_NUM_TMR0_CMIA0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR0_CMIB0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR0_CMIB0) = BSP_PRV_INT_B_NUM_TMR0_CMIB0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR0_OVI0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR0_OVI0) = BSP_PRV_INT_B_NUM_TMR0_OVI0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR1_CMIA1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR1_CMIA1) = BSP_PRV_INT_B_NUM_TMR1_CMIA1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR1_CMIB1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR1_CMIB1) = BSP_PRV_INT_B_NUM_TMR1_CMIB1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR1_OVI1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR1_OVI1) = BSP_PRV_INT_B_NUM_TMR1_OVI1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR2_CMIA2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR2_CMIA2) = BSP_PRV_INT_B_NUM_TMR2_CMIA2;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR2_CMIB2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR2_CMIB2) = BSP_PRV_INT_B_NUM_TMR2_CMIB2;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR2_OVI2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR2_OVI2) = BSP_PRV_INT_B_NUM_TMR2_OVI2;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR3_CMIA3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR3_CMIA3) = BSP_PRV_INT_B_NUM_TMR3_CMIA3;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR3_CMIB3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR3_CMIB3) = BSP_PRV_INT_B_NUM_TMR3_CMIB3;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_TMR3_OVI3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_TMR3_OVI3) = BSP_PRV_INT_B_NUM_TMR3_OVI3;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_IC0I0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_IC0I0) = BSP_PRV_INT_B_NUM_CMTW0_IC0I0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_IC1I0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_IC1I0) = BSP_PRV_INT_B_NUM_CMTW0_IC1I0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_OC0I0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_OC0I0) = BSP_PRV_INT_B_NUM_CMTW0_OC0I0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_OC1I0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW0_OC1I0) = BSP_PRV_INT_B_NUM_CMTW0_OC1I0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_IC0I1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_IC0I1) = BSP_PRV_INT_B_NUM_CMTW1_IC0I1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_IC1I1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_IC1I1) = BSP_PRV_INT_B_NUM_CMTW1_IC1I1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_OC0I1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_OC0I1) = BSP_PRV_INT_B_NUM_CMTW1_OC0I1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_OC1I1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMTW1_OC1I1) = BSP_PRV_INT_B_NUM_CMTW1_OC1I1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_RTC_CUP)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_RTC_CUP) = BSP_PRV_INT_B_NUM_RTC_CUP;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12ADI0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12ADI0) = BSP_PRV_INT_B_NUM_S12ADC0_S12ADI0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12GBADI0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12GBADI0) = BSP_PRV_INT_B_NUM_S12ADC0_S12GBADI0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12GCADI0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12GCADI0) = BSP_PRV_INT_B_NUM_S12ADC0_S12GCADI0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_ELC_ELSR18I)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_ELC_ELSR18I) = BSP_PRV_INT_B_NUM_ELC_ELSR18I;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_ELC_ELSR19I)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_ELC_ELSR19I) = BSP_PRV_INT_B_NUM_ELC_ELSR19I;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMPC0_CMPC0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMPC0_CMPC0) = BSP_PRV_INT_B_NUM_CMPC0_CMPC0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMPC1_CMPC1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMPC1_CMPC1) = BSP_PRV_INT_B_NUM_CMPC1_CMPC1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMPC2_CMPC2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMPC2_CMPC2) = BSP_PRV_INT_B_NUM_CMPC2_CMPC2;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CMPC3_CMPC3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CMPC3_CMPC3) = BSP_PRV_INT_B_NUM_CMPC3_CMPC3;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CANFD_RFDREQ0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CANFD_RFDREQ0) = BSP_PRV_INT_B_NUM_CANFD_RFDREQ0;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CANFD_RFDREQ1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CANFD_RFDREQ1) = BSP_PRV_INT_B_NUM_CANFD_RFDREQ1;
#endif

#if BSP_PRV_VALID_MAP_INT(B, BSP_MAPPED_INT_CFG_B_VECT_CANFD0_CFDREQ0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_B, BSP_MAPPED_INT_CFG_B_VECT_CANFD0_CFDREQ0) = BSP_PRV_INT_B_NUM_CANFD0_CFDREQ0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIA0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIA0) = BSP_PRV_INT_A_NUM_MTU0_TGIA0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIB0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIB0) = BSP_PRV_INT_A_NUM_MTU0_TGIB0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIC0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIC0) = BSP_PRV_INT_A_NUM_MTU0_TGIC0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGID0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGID0) = BSP_PRV_INT_A_NUM_MTU0_TGID0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TCIV0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TCIV0) = BSP_PRV_INT_A_NUM_MTU0_TCIV0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIE0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIE0) = BSP_PRV_INT_A_NUM_MTU0_TGIE0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIF0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIF0) = BSP_PRV_INT_A_NUM_MTU0_TGIF0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TGIA1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TGIA1) = BSP_PRV_INT_A_NUM_MTU1_TGIA1;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TGIB1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TGIB1) = BSP_PRV_INT_A_NUM_MTU1_TGIB1;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TCIV1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TCIV1) = BSP_PRV_INT_A_NUM_MTU1_TCIV1;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TCIU1)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU1_TCIU1) = BSP_PRV_INT_A_NUM_MTU1_TCIU1;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TGIA2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TGIA2) = BSP_PRV_INT_A_NUM_MTU2_TGIA2;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TGIB2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TGIB2) = BSP_PRV_INT_A_NUM_MTU2_TGIB2;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TCIV2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TCIV2) = BSP_PRV_INT_A_NUM_MTU2_TCIV2;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TCIU2)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU2_TCIU2) = BSP_PRV_INT_A_NUM_MTU2_TCIU2;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3) = BSP_PRV_INT_A_NUM_MTU3_TGIA3;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIB3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIB3) = BSP_PRV_INT_A_NUM_MTU3_TGIB3;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIC3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIC3) = BSP_PRV_INT_A_NUM_MTU3_TGIC3;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGID3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGID3) = BSP_PRV_INT_A_NUM_MTU3_TGID3;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TCIV3)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU3_TCIV3) = BSP_PRV_INT_A_NUM_MTU3_TCIV3;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIA4)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIA4) = BSP_PRV_INT_A_NUM_MTU4_TGIA4;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIB4)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIB4) = BSP_PRV_INT_A_NUM_MTU4_TGIB4;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIC4)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIC4) = BSP_PRV_INT_A_NUM_MTU4_TGIC4;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGID4)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGID4) = BSP_PRV_INT_A_NUM_MTU4_TGID4;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TCIV4)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU4_TCIV4) = BSP_PRV_INT_A_NUM_MTU4_TCIV4;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIU5)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIU5) = BSP_PRV_INT_A_NUM_MTU5_TGIU5;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIV5)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIV5) = BSP_PRV_INT_A_NUM_MTU5_TGIV5;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIW5)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIW5) = BSP_PRV_INT_A_NUM_MTU5_TGIW5;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIA6)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIA6) = BSP_PRV_INT_A_NUM_MTU6_TGIA6;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIB6)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIB6) = BSP_PRV_INT_A_NUM_MTU6_TGIB6;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIC6)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIC6) = BSP_PRV_INT_A_NUM_MTU6_TGIC6;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGID6)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGID6) = BSP_PRV_INT_A_NUM_MTU6_TGID6;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TCIV6)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU6_TCIV6) = BSP_PRV_INT_A_NUM_MTU6_TCIV6;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIA7)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIA7) = BSP_PRV_INT_A_NUM_MTU7_TGIA7;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIB7)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIB7) = BSP_PRV_INT_A_NUM_MTU7_TGIB7;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIC7)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIC7) = BSP_PRV_INT_A_NUM_MTU7_TGIC7;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGID7)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGID7) = BSP_PRV_INT_A_NUM_MTU7_TGID7;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TCIV7)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU7_TCIV7) = BSP_PRV_INT_A_NUM_MTU7_TCIV7;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIA8)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIA8) = BSP_PRV_INT_A_NUM_MTU8_TGIA8;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIB8)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIB8) = BSP_PRV_INT_A_NUM_MTU8_TGIB8;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIC8)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIC8) = BSP_PRV_INT_A_NUM_MTU8_TGIC8;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGID8)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGID8) = BSP_PRV_INT_A_NUM_MTU8_TGID8;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TCIV8)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_MTU8_TCIV8) = BSP_PRV_INT_A_NUM_MTU8_TCIV8;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_RSPI0_SPCI0)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_RSPI0_SPCI0) = BSP_PRV_INT_A_NUM_RSPI0_SPCI0;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_RSCI10_AED)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_RSCI10_AED) = BSP_PRV_INT_A_NUM_RSCI10_AED;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_RSCI11_AED)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_RSCI11_AED) = BSP_PRV_INT_A_NUM_RSCI11_AED;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_CANFD_EC1EI)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_CANFD_EC1EI) = BSP_PRV_INT_A_NUM_CANFD_EC1EI;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_CANFD_EC2EI)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_CANFD_EC2EI) = BSP_PRV_INT_A_NUM_CANFD_EC2EI;
#endif

#if BSP_PRV_VALID_MAP_INT(A, BSP_MAPPED_INT_CFG_A_VECT_CANFD_ECOVI)
    /* Casting is valid because it matches the type to the right side or argument. */
    BSP_PRV_INT_SELECT(BSP_PRV_A, BSP_MAPPED_INT_CFG_A_VECT_CANFD_ECOVI) = BSP_PRV_INT_A_NUM_CANFD_ECOVI;
#endif

} /* End of function bsp_mapped_interrupt_open() */

