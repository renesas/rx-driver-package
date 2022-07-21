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
* File Name    : r_bsp_interrupt_config.h
* Description  : This module maps Interrupt A & B interrupts. More information on how this is done is given below.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version   Description
*         : 22.04.2022 1.00      First Release
***********************************************************************************************************************/
#ifndef R_BSP_INTERRUPT_CONFIG_REF_HEADER_FILE
#define R_BSP_INTERRUPT_CONFIG_REF_HEADER_FILE

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* If you wish to use one of the interrupt below then set the appropriate configuration macro to the vector number you
 * wish to use for that interrupt. For example, if you want to use the RTC carry interrupt (CUP) at vector 176 then you
 * would do the following:
 *
 * #define BSP_MAPPED_INT_CFG_B_VECT_RTC_CUP                                 176
 */

/* Interrupt B Sources.
 * -Valid vector numbers are 128-207.
 * -There are more vector slots for B sources than actual B sources.
 * -If a peripheral interrupt is going to be used to wake up the MCU from All-Module Clock Stop Mode then it must be
 *  in a vector between 146 to 157. Peripheral interrupts that can do this are TMR interrupts interrupt.
 *  By default the TMR interrupts are chosen since there are 12 of them and there are 12 slots.
 */
#define BSP_MAPPED_INT_CFG_B_VECT_CMT2_CMI2                               128
#define BSP_MAPPED_INT_CFG_B_VECT_CMT3_CMI3                               129
#define BSP_MAPPED_INT_CFG_B_VECT_TMR0_CMIA0                              146
#define BSP_MAPPED_INT_CFG_B_VECT_TMR0_CMIB0                              147
#define BSP_MAPPED_INT_CFG_B_VECT_TMR0_OVI0                               148
#define BSP_MAPPED_INT_CFG_B_VECT_TMR1_CMIA1                              149
#define BSP_MAPPED_INT_CFG_B_VECT_TMR1_CMIB1                              150
#define BSP_MAPPED_INT_CFG_B_VECT_TMR1_OVI1                               151
#define BSP_MAPPED_INT_CFG_B_VECT_TMR2_CMIA2                              152
#define BSP_MAPPED_INT_CFG_B_VECT_TMR2_CMIB2                              153
#define BSP_MAPPED_INT_CFG_B_VECT_TMR2_OVI2                               154
#define BSP_MAPPED_INT_CFG_B_VECT_TMR3_CMIA3                              155
#define BSP_MAPPED_INT_CFG_B_VECT_TMR3_CMIB3                              156
#define BSP_MAPPED_INT_CFG_B_VECT_TMR3_OVI3                               157
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW0_IC0I0                             130
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW0_IC1I0                             131
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW0_OC0I0                             132
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW0_OC1I0                             133
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW1_IC0I1                             134
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW1_IC1I1                             135
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW1_OC0I1                             136
#define BSP_MAPPED_INT_CFG_B_VECT_CMTW1_OC1I1                             137
#define BSP_MAPPED_INT_CFG_B_VECT_RTC_CUP                                 138
#define BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12ADI0                         139
#define BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12GBADI0                       140
#define BSP_MAPPED_INT_CFG_B_VECT_S12ADC0_S12GCADI0                       141
#define BSP_MAPPED_INT_CFG_B_VECT_ELC_ELSR18I                             142
#define BSP_MAPPED_INT_CFG_B_VECT_ELC_ELSR19I                             143
#define BSP_MAPPED_INT_CFG_B_VECT_CMPC0_CMPC0                             158
#define BSP_MAPPED_INT_CFG_B_VECT_CMPC1_CMPC1                             159
#define BSP_MAPPED_INT_CFG_B_VECT_CMPC2_CMPC2                             160
#define BSP_MAPPED_INT_CFG_B_VECT_CMPC3_CMPC3                             161
#define BSP_MAPPED_INT_CFG_B_VECT_CANFD_RFDREQ0                           162
#define BSP_MAPPED_INT_CFG_B_VECT_CANFD_RFDREQ1                           163
#define BSP_MAPPED_INT_CFG_B_VECT_CANFD0_CFDREQ0                          164

/* Interrupt A Sources.
 * -Valid vector numbers are 208-255.
 * -There are more A sources than A vector slots. By default none of the MTU8 interrupts are 
 *  mapped.
 */
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIA0                              208
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIB0                              209
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIC0                              210
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGID0                              211
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TCIV0                              212
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIE0                              213
#define BSP_MAPPED_INT_CFG_A_VECT_MTU0_TGIF0                              214
#define BSP_MAPPED_INT_CFG_A_VECT_MTU1_TGIA1                              215
#define BSP_MAPPED_INT_CFG_A_VECT_MTU1_TGIB1                              216
#define BSP_MAPPED_INT_CFG_A_VECT_MTU1_TCIV1                              217
#define BSP_MAPPED_INT_CFG_A_VECT_MTU1_TCIU1                              218
#define BSP_MAPPED_INT_CFG_A_VECT_MTU2_TGIA2                              219
#define BSP_MAPPED_INT_CFG_A_VECT_MTU2_TGIB2                              220
#define BSP_MAPPED_INT_CFG_A_VECT_MTU2_TCIV2                              221
#define BSP_MAPPED_INT_CFG_A_VECT_MTU2_TCIU2                              222
#define BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3                              223
#define BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIB3                              224
#define BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIC3                              225
#define BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGID3                              226
#define BSP_MAPPED_INT_CFG_A_VECT_MTU3_TCIV3                              227
#define BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIA4                              228
#define BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIB4                              229
#define BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGIC4                              230
#define BSP_MAPPED_INT_CFG_A_VECT_MTU4_TGID4                              231
#define BSP_MAPPED_INT_CFG_A_VECT_MTU4_TCIV4                              232
#define BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIU5                              233
#define BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIV5                              234
#define BSP_MAPPED_INT_CFG_A_VECT_MTU5_TGIW5                              235
#define BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIA6                              236
#define BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIB6                              237
#define BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGIC6                              238
#define BSP_MAPPED_INT_CFG_A_VECT_MTU6_TGID6                              239
#define BSP_MAPPED_INT_CFG_A_VECT_MTU6_TCIV6                              240
#define BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIA7                              241
#define BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIB7                              242
#define BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGIC7                              243
#define BSP_MAPPED_INT_CFG_A_VECT_MTU7_TGID7                              244
#define BSP_MAPPED_INT_CFG_A_VECT_MTU7_TCIV7                              245
#define BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIA8
#define BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIB8
#define BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGIC8
#define BSP_MAPPED_INT_CFG_A_VECT_MTU8_TGID8
#define BSP_MAPPED_INT_CFG_A_VECT_MTU8_TCIV8
#define BSP_MAPPED_INT_CFG_A_VECT_RSPI0_SPCI0                             246
#define BSP_MAPPED_INT_CFG_A_VECT_RSCI10_AED                              247
#define BSP_MAPPED_INT_CFG_A_VECT_RSCI11_AED                              248
#define BSP_MAPPED_INT_CFG_A_VECT_CANFD_EC1EI                             249
#define BSP_MAPPED_INT_CFG_A_VECT_CANFD_EC2EI                             250
#define BSP_MAPPED_INT_CFG_A_VECT_CANFD_ECOVI                             251

#endif /* R_BSP_INTERRUPT_CONFIG_REF_HEADER_FILE */

