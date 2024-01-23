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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/********************************************************************************/
/*                                                                              */
/* Device     : RX/RX200/RX26T                                                  */
/* File Name  : iodefine.h                                                      */
/* Abstract   : Definition of I/O Register.                                     */
/* History    : V0.40  (2022-04-28)  [Hardware Manual Revision : 0.40]          */
/*            : V0.50  (2022-06-01)  [Hardware Manual Revision : 0.50]          */
/*            : V0.50A (2022-07-04)  [Hardware Manual Revision : 0.50]          */
/*            : V0.50B (2022-08-25)  [Hardware Manual Revision : 0.50]          */
/*            : V1.00  (2022-12-13)  [Hardware Manual Revision : 0.60]          */
/*            : V1.00A (2023-01-27)  [Hardware Manual Revision : 1.00]          */
/*            : V1.10  (2023-07-07)  [Hardware Manual Revision : 1.10]          */
/*            : V1.10A (2023-08-22)  [Hardware Manual Revision : 1.10]          */
/* Note       : This is a typical example.                                      */
/*                                                                              */
/*  Copyright(c) 2023 Renesas Electronics Corporation.                          */
/*                                                                              */
/********************************************************************************/
/*                                                                              */
/*  DESCRIPTION : Definition of ICU Register                                    */
/*  CPU TYPE    : RX26T                                                         */
/*                                                                              */
/*  Usage : IR,DTCER,IER,IPR of ICU Register                                    */
/*     The following IR, DTCE, IEN, IPR macro functions simplify usage.         */
/*     The bit access operation is "Bit_Name(interrupt source,name)".           */
/*     A part of the name can be omitted.                                       */
/*     for example :                                                            */
/*       IR(BSC,BUSERR) = 0;     expands to :                                   */
/*         ICU.IR[16].BIT.IR = 0;                                               */
/*                                                                              */
/*       DTCE(ICU,IRQ0) = 1;     expands to :                                   */
/*         ICU.DTCER[64].BIT.DTCE = 1;                                          */
/*                                                                              */
/*       IEN(CMT0,CMI0) = 1;     expands to :                                   */
/*         ICU.IER[0x03].BIT.IEN4 = 1;                                          */
/*                                                                              */
/*       IPR(ICU,SWINT2) = 2;    expands to :                                   */
/*       IPR(ICU,SWI   ) = 2;    // SWINT2,SWINT share IPR level.               */
/*         ICU.IPR[3].BIT.IPR = 2;                                              */
/*                                                                              */
/*  Usage : #pragma interrupt Function_Identifier(vect=**)                      */
/*     The number of vector is "(interrupt source, name)".                      */
/*     for example :                                                            */
/*       #pragma interrupt INT_IRQ0(vect=VECT(ICU,IRQ0))          expands to :  */
/*         #pragma interrupt INT_IRQ0(vect=64)                                  */
/*       #pragma interrupt INT_CMT0_CMI0(vect=VECT(CMT0,CMI0))    expands to :  */
/*         #pragma interrupt INT_CMT0_CMI0(vect=28)                             */
/*                                                                              */
/*  Usage : MSTPCRA,MSTPCRB,MSTPCRC of SYSTEM Register                          */
/*     The bit access operation is "MSTP(name)".                                */
/*     The name that can be used is a macro name defined with "iodefine.h".     */
/*     for example :                                                            */
/*       MSTP(TMR2) = 0;    // TMR2,TMR3,TMR23                    expands to :  */
/*         SYSTEM.MSTPCRA.BIT.MSTPA4  = 0;                                      */
/*       MSTP(MTU4) = 0;    // MTU,MTU0,MTU1,MTU2,MTU3,MTU4,...   expands to :  */
/*         SYSTEM.MSTPCRA.BIT.MSTPA9  = 0;                                      */
/*       MSTP(CMT3) = 0;    // CMT2,CMT3                          expands to :  */
/*         SYSTEM.MSTPCRA.BIT.MSTPA14 = 0;                                      */
/*                                                                              */
/*                                                                              */
/********************************************************************************/
#ifndef __RX26TIODEFINE_HEADER__
#define __RX26TIODEFINE_HEADER__

#define	IEN_BSC_BUSERR		IEN0
#define	IEN_RAM_RAMERR		IEN2
#define	IEN_FCU_FIFERR		IEN5
#define	IEN_FCU_FRDYI		IEN7
#define	IEN_ICU_SWINT2		IEN2
#define	IEN_ICU_SWINT		IEN3
#define	IEN_CMT0_CMI0		IEN4
#define	IEN_CMT1_CMI1		IEN5
#define	IEN_CMT2_CMI2		IEN6
#define	IEN_CMT3_CMI3		IEN7
#define	IEN_RSPI0_SPRI0		IEN6
#define	IEN_RSPI0_SPTI0		IEN7
#define	IEN_RI3C0_RESPI		IEN0
#define	IEN_RI3C0_CMDI		IEN1
#define	IEN_RI3C0_IBII		IEN2
#define	IEN_RI3C0_RCVI		IEN3
#define	IEN_RSPIA0_SPRI		IEN0
#define	IEN_RSPIA0_SPTI		IEN1
#define	IEN_RIIC0_RXI0		IEN4
#define	IEN_RIIC0_TXI0		IEN5
#define	IEN_SCI1_RXI1		IEN4
#define	IEN_SCI1_TXI1		IEN5
#define	IEN_ICU_IRQ0		IEN0
#define	IEN_ICU_IRQ1		IEN1
#define	IEN_ICU_IRQ2		IEN2
#define	IEN_ICU_IRQ3		IEN3
#define	IEN_ICU_IRQ4		IEN4
#define	IEN_ICU_IRQ5		IEN5
#define	IEN_ICU_IRQ6		IEN6
#define	IEN_ICU_IRQ7		IEN7
#define	IEN_ICU_IRQ8		IEN0
#define	IEN_ICU_IRQ9		IEN1
#define	IEN_ICU_IRQ10		IEN2
#define	IEN_ICU_IRQ11		IEN3
#define	IEN_ICU_IRQ12		IEN4
#define	IEN_ICU_IRQ13		IEN5
#define	IEN_ICU_IRQ14		IEN6
#define	IEN_ICU_IRQ15		IEN7
#define	IEN_SCI5_RXI5		IEN4
#define	IEN_SCI5_TXI5		IEN5
#define	IEN_SCI6_RXI6		IEN6
#define	IEN_SCI6_TXI6		IEN7
#define	IEN_LVD1_LVD1		IEN0
#define	IEN_LVD2_LVD2		IEN1
#define	IEN_IWDT_IWUNI		IEN7
#define	IEN_WDT_WUNI		IEN0
#define	IEN_RSCI8_RXI		IEN4
#define	IEN_RSCI8_TXI		IEN5
#define	IEN_RSCI9_RXI		IEN6
#define	IEN_RSCI9_TXI		IEN7
#define	IEN_ICU_GROUPBL2	IEN3
#define	IEN_ICU_GROUPBL0	IEN6
#define	IEN_ICU_GROUPBL1	IEN7
#define	IEN_ICU_GROUPAL0	IEN0
#define	IEN_ICU_GROUPAL1	IEN1
#define	IEN_RSCI11_RXI		IEN2
#define	IEN_RSCI11_TXI		IEN3
#define	IEN_SCI12_RXI12		IEN4
#define	IEN_SCI12_TXI12		IEN5
#define	IEN_RI3C0_RXI		IEN6
#define	IEN_RI3C0_TXI		IEN7
#define	IEN_DMAC_DMAC0I		IEN0
#define	IEN_DMAC_DMAC1I		IEN1
#define	IEN_DMAC_DMAC2I		IEN2
#define	IEN_DMAC_DMAC3I		IEN3
#define	IEN_DMAC_DMAC74I	IEN4
#define	IEN_OST_OSTDI		IEN5
#define	IEN_S12AD_S12ADI	IEN0
#define	IEN_S12AD_S12GBADI	IEN1
#define	IEN_S12AD_S12GCADI	IEN2
#define	IEN_S12AD1_S12ADI1	IEN4
#define	IEN_S12AD1_S12GBADI1	IEN5
#define	IEN_S12AD1_S12GCADI1	IEN6
#define	IEN_S12AD2_S12ADI2	IEN0
#define	IEN_S12AD2_S12GBADI2	IEN1
#define	IEN_S12AD2_S12GCADI2	IEN2
#define	IEN_CANFD_RFDREQ0	IEN4
#define	IEN_CANFD_RFDREQ1	IEN5
#define	IEN_CANFD0_CFDREQ0	IEN6
#define	IEN_TMR0_CMIA0		IEN2
#define	IEN_TMR0_CMIB0		IEN3
#define	IEN_TMR0_OVI0		IEN4
#define	IEN_TMR1_CMIA1		IEN5
#define	IEN_TMR1_CMIB1		IEN6
#define	IEN_TMR1_OVI1		IEN7
#define	IEN_TMR2_CMIA2		IEN0
#define	IEN_TMR2_CMIB2		IEN1
#define	IEN_TMR2_OVI2		IEN2
#define	IEN_TMR3_CMIA3		IEN3
#define	IEN_TMR3_CMIB3		IEN4
#define	IEN_TMR3_OVI3		IEN5
#define	IEN_TMR4_CMIA4		IEN6
#define	IEN_TMR4_CMIB4		IEN7
#define	IEN_TMR4_OVI4		IEN0
#define	IEN_TMR5_CMIA5		IEN1
#define	IEN_TMR5_CMIB5		IEN2
#define	IEN_TMR5_OVI5		IEN3
#define	IEN_TMR6_CMIA6		IEN4
#define	IEN_TMR6_CMIB6		IEN5
#define	IEN_TMR6_OVI6		IEN6
#define	IEN_TMR7_CMIA7		IEN7
#define	IEN_TMR7_CMIB7		IEN0
#define	IEN_TMR7_OVI7		IEN1
#define	IEN_ELC_ELSR18I		IEN7
#define	IEN_ELC_ELSR19I		IEN0
#define	IEN_TSIP_RD			IEN1
#define	IEN_TSIP_WR			IEN2
#define	IEN_TSIP_ERR		IEN3
#define	IEN_CMPC0_CMPC0		IEN4
#define	IEN_CMPC1_CMPC1		IEN5
#define	IEN_CMPC2_CMPC2		IEN6
#define	IEN_CMPC3_CMPC3		IEN7
#define	IEN_CMPC4_CMPC4		IEN0
#define	IEN_CMPC5_CMPC5		IEN1
#define	IEN_CMTW0_CMWI0		IEN2
#define	IEN_CMTW0_IC0I0		IEN3
#define	IEN_CMTW0_IC1I0		IEN4
#define	IEN_CMTW0_OC0I0		IEN5
#define	IEN_CMTW0_OC1I0		IEN6
#define	IEN_CMTW1_CMWI1		IEN7
#define	IEN_CMTW1_IC0I1		IEN0
#define	IEN_CMTW1_IC1I1		IEN1
#define	IEN_CMTW1_OC0I1		IEN2
#define	IEN_CMTW1_OC1I1		IEN3
#define	IEN_RSCI9_AED		IEN5
#define	IEN_PERIA_INTA208	IEN0
#define	IEN_PERIA_INTA209	IEN1
#define	IEN_PERIA_INTA210	IEN2
#define	IEN_PERIA_INTA211	IEN3
#define	IEN_PERIA_INTA212	IEN4
#define	IEN_PERIA_INTA213	IEN5
#define	IEN_PERIA_INTA214	IEN6
#define	IEN_PERIA_INTA215	IEN7
#define	IEN_PERIA_INTA216	IEN0
#define	IEN_PERIA_INTA217	IEN1
#define	IEN_PERIA_INTA218	IEN2
#define	IEN_PERIA_INTA219	IEN3
#define	IEN_PERIA_INTA220	IEN4
#define	IEN_PERIA_INTA221	IEN5
#define	IEN_PERIA_INTA222	IEN6
#define	IEN_PERIA_INTA223	IEN7
#define	IEN_PERIA_INTA224	IEN0
#define	IEN_PERIA_INTA225	IEN1
#define	IEN_PERIA_INTA226	IEN2
#define	IEN_PERIA_INTA227	IEN3
#define	IEN_PERIA_INTA228	IEN4
#define	IEN_PERIA_INTA229	IEN5
#define	IEN_PERIA_INTA230	IEN6
#define	IEN_PERIA_INTA231	IEN7
#define	IEN_PERIA_INTA232	IEN0
#define	IEN_PERIA_INTA233	IEN1
#define	IEN_PERIA_INTA234	IEN2
#define	IEN_PERIA_INTA235	IEN3
#define	IEN_PERIA_INTA236	IEN4
#define	IEN_PERIA_INTA237	IEN5
#define	IEN_PERIA_INTA238	IEN6
#define	IEN_PERIA_INTA239	IEN7
#define	IEN_PERIA_INTA240	IEN0
#define	IEN_PERIA_INTA241	IEN1
#define	IEN_PERIA_INTA242	IEN2
#define	IEN_PERIA_INTA243	IEN3
#define	IEN_PERIA_INTA244	IEN4
#define	IEN_PERIA_INTA245	IEN5
#define	IEN_PERIA_INTA246	IEN6
#define	IEN_PERIA_INTA247	IEN7
#define	IEN_PERIA_INTA248	IEN0
#define	IEN_PERIA_INTA249	IEN1
#define	IEN_PERIA_INTA250	IEN2
#define	IEN_PERIA_INTA251	IEN3
#define	IEN_PERIA_INTA252	IEN4
#define	IEN_PERIA_INTA253	IEN5
#define	IEN_PERIA_INTA254	IEN6
#define	IEN_PERIA_INTA255	IEN7

#define	VECT_BSC_BUSERR		16
#define	VECT_RAM_RAMERR		18
#define	VECT_FCU_FIFERR		21
#define	VECT_FCU_FRDYI		23
#define	VECT_ICU_SWINT2		26
#define	VECT_ICU_SWINT		27
#define	VECT_CMT0_CMI0		28
#define	VECT_CMT1_CMI1		29
#define	VECT_CMT2_CMI2		30
#define	VECT_CMT3_CMI3		31
#define	VECT_RSPI0_SPRI0	38
#define	VECT_RSPI0_SPTI0	39
#define	VECT_RI3C0_RESPI	40
#define	VECT_RI3C0_CMDI		41
#define	VECT_RI3C0_IBII		42
#define	VECT_RI3C0_RCVI		43
#define	VECT_RSPIA0_SPRI	48
#define	VECT_RSPIA0_SPTI	49
#define	VECT_RIIC0_RXI0		52
#define	VECT_RIIC0_TXI0		53
#define	VECT_SCI1_RXI1		60
#define	VECT_SCI1_TXI1		61
#define	VECT_ICU_IRQ0		64
#define	VECT_ICU_IRQ1		65
#define	VECT_ICU_IRQ2		66
#define	VECT_ICU_IRQ3		67
#define	VECT_ICU_IRQ4		68
#define	VECT_ICU_IRQ5		69
#define	VECT_ICU_IRQ6		70
#define	VECT_ICU_IRQ7		71
#define	VECT_ICU_IRQ8		72
#define	VECT_ICU_IRQ9		73
#define	VECT_ICU_IRQ10		74
#define	VECT_ICU_IRQ11		75
#define	VECT_ICU_IRQ12		76
#define	VECT_ICU_IRQ13		77
#define	VECT_ICU_IRQ14		78
#define	VECT_ICU_IRQ15		79
#define	VECT_SCI5_RXI5		84
#define	VECT_SCI5_TXI5		85
#define	VECT_SCI6_RXI6		86
#define	VECT_SCI6_TXI6		87
#define	VECT_LVD1_LVD1		88
#define	VECT_LVD2_LVD2		89
#define	VECT_IWDT_IWUNI		95
#define	VECT_WDT_WUNI		96
#define	VECT_RSCI8_RXI		100
#define	VECT_RSCI8_TXI		101
#define	VECT_RSCI9_RXI		102
#define	VECT_RSCI9_TXI		103
#define	VECT_ICU_GROUPBL2	107
#define	VECT_ICU_GROUPBL0	110
#define	VECT_ICU_GROUPBL1	111
#define	VECT_ICU_GROUPAL0	112
#define	VECT_ICU_GROUPAL1	113
#define	VECT_RSCI11_RXI		114
#define	VECT_RSCI11_TXI		115
#define	VECT_SCI12_RXI12	116
#define	VECT_SCI12_TXI12	117
#define	VECT_RI3C0_RXI		118
#define	VECT_RI3C0_TXI		119
#define	VECT_DMAC_DMAC0I	120
#define	VECT_DMAC_DMAC1I	121
#define	VECT_DMAC_DMAC2I	122
#define	VECT_DMAC_DMAC3I	123
#define	VECT_DMAC_DMAC74I	124
#define	VECT_OST_OSTDI		125
#define	VECT_S12AD_S12ADI	128
#define	VECT_S12AD_S12GBADI	129
#define	VECT_S12AD_S12GCADI	130
#define	VECT_S12AD1_S12ADI1	132
#define	VECT_S12AD1_S12GBADI1	133
#define	VECT_S12AD1_S12GCADI1	134
#define	VECT_S12AD2_S12ADI2	136
#define	VECT_S12AD2_S12GBADI2	137
#define	VECT_S12AD2_S12GCADI2	138
#define	VECT_CANFD_RFDREQ0	140
#define	VECT_CANFD_RFDREQ1	141
#define	VECT_CANFD0_CFDREQ0	142
#define	VECT_TMR0_CMIA0		146
#define	VECT_TMR0_CMIB0		147
#define	VECT_TMR0_OVI0		148
#define	VECT_TMR1_CMIA1		149
#define	VECT_TMR1_CMIB1		150
#define	VECT_TMR1_OVI1		151
#define	VECT_TMR2_CMIA2		152
#define	VECT_TMR2_CMIB2		153
#define	VECT_TMR2_OVI2		154
#define	VECT_TMR3_CMIA3		155
#define	VECT_TMR3_CMIB3		156
#define	VECT_TMR3_OVI3		157
#define	VECT_TMR4_CMIA4		158
#define	VECT_TMR4_CMIB4		159
#define	VECT_TMR4_OVI4		160
#define	VECT_TMR5_CMIA5		161
#define	VECT_TMR5_CMIB5		162
#define	VECT_TMR5_OVI5		163
#define	VECT_TMR6_CMIA6		164
#define	VECT_TMR6_CMIB6		165
#define	VECT_TMR6_OVI6		166
#define	VECT_TMR7_CMIA7		167
#define	VECT_TMR7_CMIB7		168
#define	VECT_TMR7_OVI7		169
#define	VECT_ELC_ELSR18I	175
#define	VECT_ELC_ELSR19I	176
#define	VECT_TSIP_RD		177
#define	VECT_TSIP_WR		178
#define	VECT_TSIP_ERR		179
#define	VECT_CMPC0_CMPC0	180
#define	VECT_CMPC1_CMPC1	181
#define	VECT_CMPC2_CMPC2	182
#define	VECT_CMPC3_CMPC3	183
#define	VECT_CMPC4_CMPC4	184
#define	VECT_CMPC5_CMPC5	185
#define	VECT_CMTW0_CMWI0	186
#define	VECT_CMTW0_IC0I0	187
#define	VECT_CMTW0_IC1I0	188
#define	VECT_CMTW0_OC0I0	189
#define	VECT_CMTW0_OC1I0	190
#define	VECT_CMTW1_CMWI1	191
#define	VECT_CMTW1_IC0I1	192
#define	VECT_CMTW1_IC1I1	193
#define	VECT_CMTW1_OC0I1	194
#define	VECT_CMTW1_OC1I1	195
#define	VECT_RSCI9_AED		197
#define	VECT_PERIA_INTA208	208
#define	VECT_PERIA_INTA209	209
#define	VECT_PERIA_INTA210	210
#define	VECT_PERIA_INTA211	211
#define	VECT_PERIA_INTA212	212
#define	VECT_PERIA_INTA213	213
#define	VECT_PERIA_INTA214	214
#define	VECT_PERIA_INTA215	215
#define	VECT_PERIA_INTA216	216
#define	VECT_PERIA_INTA217	217
#define	VECT_PERIA_INTA218	218
#define	VECT_PERIA_INTA219	219
#define	VECT_PERIA_INTA220	220
#define	VECT_PERIA_INTA221	221
#define	VECT_PERIA_INTA222	222
#define	VECT_PERIA_INTA223	223
#define	VECT_PERIA_INTA224	224
#define	VECT_PERIA_INTA225	225
#define	VECT_PERIA_INTA226	226
#define	VECT_PERIA_INTA227	227
#define	VECT_PERIA_INTA228	228
#define	VECT_PERIA_INTA229	229
#define	VECT_PERIA_INTA230	230
#define	VECT_PERIA_INTA231	231
#define	VECT_PERIA_INTA232	232
#define	VECT_PERIA_INTA233	233
#define	VECT_PERIA_INTA234	234
#define	VECT_PERIA_INTA235	235
#define	VECT_PERIA_INTA236	236
#define	VECT_PERIA_INTA237	237
#define	VECT_PERIA_INTA238	238
#define	VECT_PERIA_INTA239	239
#define	VECT_PERIA_INTA240	240
#define	VECT_PERIA_INTA241	241
#define	VECT_PERIA_INTA242	242
#define	VECT_PERIA_INTA243	243
#define	VECT_PERIA_INTA244	244
#define	VECT_PERIA_INTA245	245
#define	VECT_PERIA_INTA246	246
#define	VECT_PERIA_INTA247	247
#define	VECT_PERIA_INTA248	248
#define	VECT_PERIA_INTA249	249
#define	VECT_PERIA_INTA250	250
#define	VECT_PERIA_INTA251	251
#define	VECT_PERIA_INTA252	252
#define	VECT_PERIA_INTA253	253
#define	VECT_PERIA_INTA254	254
#define	VECT_PERIA_INTA255	255

#define	MSTP_DMAC	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC0	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC1	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC2	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC3	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC4	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC5	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC6	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DMAC7	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_DTC	SYSTEM.MSTPCRA.BIT.MSTPA28
#define	MSTP_S12AD2	SYSTEM.MSTPCRA.BIT.MSTPA23
#define	MSTP_DA		SYSTEM.MSTPCRA.BIT.MSTPA19
#define	MSTP_S12AD	SYSTEM.MSTPCRA.BIT.MSTPA17
#define	MSTP_S12AD1	SYSTEM.MSTPCRA.BIT.MSTPA16
#define	MSTP_CMT0	SYSTEM.MSTPCRA.BIT.MSTPA15
#define	MSTP_CMT1	SYSTEM.MSTPCRA.BIT.MSTPA15
#define	MSTP_CMT2	SYSTEM.MSTPCRA.BIT.MSTPA14
#define	MSTP_CMT3	SYSTEM.MSTPCRA.BIT.MSTPA14
#define	MSTP_MTU	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU0	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU1	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU2	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU3	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU4	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU5	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU6	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU7	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_MTU9	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_GPTW	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW0	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW1	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW2	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW3	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW4	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW5	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW6	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_GPTW7	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_HRPWM	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_POEG	SYSTEM.MSTPCRA.BIT.MSTPA7
#define	MSTP_TMR0	SYSTEM.MSTPCRA.BIT.MSTPA5
#define	MSTP_TMR1	SYSTEM.MSTPCRA.BIT.MSTPA5
#define	MSTP_TMR01	SYSTEM.MSTPCRA.BIT.MSTPA5
#define	MSTP_TMR2	SYSTEM.MSTPCRA.BIT.MSTPA4
#define	MSTP_TMR3	SYSTEM.MSTPCRA.BIT.MSTPA4
#define	MSTP_TMR23	SYSTEM.MSTPCRA.BIT.MSTPA4
#define	MSTP_TMR4	SYSTEM.MSTPCRA.BIT.MSTPA3
#define	MSTP_TMR5	SYSTEM.MSTPCRA.BIT.MSTPA3
#define	MSTP_TMR45	SYSTEM.MSTPCRA.BIT.MSTPA3
#define	MSTP_TMR6	SYSTEM.MSTPCRA.BIT.MSTPA2
#define	MSTP_TMR7	SYSTEM.MSTPCRA.BIT.MSTPA2
#define	MSTP_TMR67	SYSTEM.MSTPCRA.BIT.MSTPA2
#define	MSTP_CMTW0	SYSTEM.MSTPCRA.BIT.MSTPA1
#define	MSTP_CMTW1	SYSTEM.MSTPCRA.BIT.MSTPA0
#define	MSTP_SCI1	SYSTEM.MSTPCRB.BIT.MSTPB30
#define	MSTP_SMCI1	SYSTEM.MSTPCRB.BIT.MSTPB30
#define	MSTP_SCI5	SYSTEM.MSTPCRB.BIT.MSTPB26
#define	MSTP_SMCI5	SYSTEM.MSTPCRB.BIT.MSTPB26
#define	MSTP_SCI6	SYSTEM.MSTPCRB.BIT.MSTPB25
#define	MSTP_SMCI6	SYSTEM.MSTPCRB.BIT.MSTPB25
#define	MSTP_CRC	SYSTEM.MSTPCRB.BIT.MSTPB23
#define	MSTP_RIIC0	SYSTEM.MSTPCRB.BIT.MSTPB21
#define	MSTP_RSPI0	SYSTEM.MSTPCRB.BIT.MSTPB17
#define	MSTP_CMPC0	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC1	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC2	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC3	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC4	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC5	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_ELC	SYSTEM.MSTPCRB.BIT.MSTPB9
#define	MSTP_DOC	SYSTEM.MSTPCRB.BIT.MSTPB6
#define	MSTP_SCI12	SYSTEM.MSTPCRB.BIT.MSTPB4
#define	MSTP_SMCI12	SYSTEM.MSTPCRB.BIT.MSTPB4
#define	MSTP_RSCI8	SYSTEM.MSTPCRC.BIT.MSTPC27
#define	MSTP_RSCI9	SYSTEM.MSTPCRC.BIT.MSTPC26
#define	MSTP_RSCI11	SYSTEM.MSTPCRC.BIT.MSTPC24
#define	MSTP_CAC	SYSTEM.MSTPCRC.BIT.MSTPC19
#define	MSTP_RAM	SYSTEM.MSTPCRC.BIT.MSTPC0
#define	MSTP_TSIP	SYSTEM.MSTPCRD.BIT.MSTPD27
#define	MSTP_RSPIA0	SYSTEM.MSTPCRD.BIT.MSTPD26
#define	MSTP_CANFD	SYSTEM.MSTPCRD.BIT.MSTPD10
#define	MSTP_CANFD0	SYSTEM.MSTPCRD.BIT.MSTPD10
#define	MSTP_RI3C0	SYSTEM.MSTPCRD.BIT.MSTPD5

#define IS_SCI1_TEI1		IS2
#define IS_SCI1_ERI1		IS3
#define IS_SCI5_TEI5		IS10
#define IS_SCI5_ERI5		IS11
#define IS_SCI6_TEI6		IS12
#define IS_SCI6_ERI6		IS13
#define IS_SCI12_TEI12		IS16
#define IS_SCI12_ERI12		IS17
#define IS_SCI12_SCIX0		IS18
#define IS_SCI12_SCIX1		IS19
#define IS_SCI12_SCIX2		IS20
#define IS_SCI12_SCIX3		IS21
#define IS_CAC_FERRI		IS26
#define IS_CAC_MENDI		IS27
#define IS_CAC_OVFI		IS28
#define IS_DOC_DOPCI		IS29
#define IS_POEG_POEGGAI		IS0
#define IS_POEG_POEGGBI		IS1
#define IS_POEG_POEGGCI		IS2
#define IS_POEG_POEGGDI		IS3
#define IS_POE_OEI5		IS8
#define IS_POE_OEI1		IS9
#define IS_POE_OEI2		IS10
#define IS_POE_OEI3		IS11
#define IS_POE_OEI4		IS12
#define IS_RIIC0_TEI0		IS13
#define IS_RIIC0_EEI0		IS14
#define IS_S12AD2_S12CMPAI2		IS18
#define IS_S12AD2_S12CMPBI2		IS19
#define IS_S12AD_S12CMPAI		IS20
#define IS_S12AD_S12CMPBI		IS21
#define IS_S12AD1_S12CMPAI1		IS22
#define IS_S12AD1_S12CMPBI1		IS23
#define IS_RSCI8_TEI		IS24
#define IS_RSCI8_ERI		IS25
#define IS_RSCI9_TEI		IS26
#define IS_RSCI9_ERI		IS27
#define IS_RSCI9_BFD		IS31
#define IS_CANFD0_CHEI		IS1
#define IS_CANFD0_CFRI		IS2
#define IS_CANFD_GLEI		IS3
#define IS_CANFD_RFRI		IS4
#define IS_CANFD0_CHTI		IS5
#define IS_CANFD_RMRI		IS6
#define IS_RSCI11_TEI		IS12
#define IS_RSCI11_ERI		IS13
#define IS_RSCI11_BFD		IS14
#define IS_RSPI0_SPII0		IS16
#define IS_RSPI0_SPEI0		IS17
#define IS_RSPIA0_SPII		IS22
#define IS_RSPIA0_SPEI		IS23
#define IS_RI3C0_EEI		IS13

#define EN_SCI1_TEI1		EN2
#define EN_SCI1_ERI1		EN3
#define EN_SCI5_TEI5		EN10
#define EN_SCI5_ERI5		EN11
#define EN_SCI6_TEI6		EN12
#define EN_SCI6_ERI6		EN13
#define EN_SCI12_TEI12		EN16
#define EN_SCI12_ERI12		EN17
#define EN_SCI12_SCIX0		EN18
#define EN_SCI12_SCIX1		EN19
#define EN_SCI12_SCIX2		EN20
#define EN_SCI12_SCIX3		EN21
#define EN_CAC_FERRI		EN26
#define EN_CAC_MENDI		EN27
#define EN_CAC_OVFI		EN28
#define EN_DOC_DOPCI		EN29
#define EN_POEG_POEGGAI		EN0
#define EN_POEG_POEGGBI		EN1
#define EN_POEG_POEGGCI		EN2
#define EN_POEG_POEGGDI		EN3
#define EN_POE_OEI5		EN8
#define EN_POE_OEI1		EN9
#define EN_POE_OEI2		EN10
#define EN_POE_OEI3		EN11
#define EN_POE_OEI4		EN12
#define EN_RIIC0_TEI0		EN13
#define EN_RIIC0_EEI0		EN14
#define EN_S12AD2_S12CMPAI2		EN18
#define EN_S12AD2_S12CMPBI2		EN19
#define EN_S12AD_S12CMPAI		EN20
#define EN_S12AD_S12CMPBI		EN21
#define EN_S12AD1_S12CMPAI1		EN22
#define EN_S12AD1_S12CMPBI1		EN23
#define EN_RSCI8_TEI		EN24
#define EN_RSCI8_ERI		EN25
#define EN_RSCI9_TEI		EN26
#define EN_RSCI9_ERI		EN27
#define EN_RSCI9_BFD		EN31
#define EN_CANFD0_CHEI		EN1
#define EN_CANFD0_CFRI		EN2
#define EN_CANFD_GLEI		EN3
#define EN_CANFD_RFRI		EN4
#define EN_CANFD0_CHTI		EN5
#define EN_CANFD_RMRI		EN6
#define EN_RSCI11_TEI		EN12
#define EN_RSCI11_ERI		EN13
#define EN_RSCI11_BFD		EN14
#define EN_RSPI0_SPII0		EN16
#define EN_RSPI0_SPEI0		EN17
#define EN_RSPIA0_SPII		EN22
#define EN_RSPIA0_SPEI		EN23
#define EN_RI3C0_EEI		EN13

#define GEN_SCI1_TEI1		GENBL0
#define GEN_SCI1_ERI1		GENBL0
#define GEN_SCI5_TEI5		GENBL0
#define GEN_SCI5_ERI5		GENBL0
#define GEN_SCI6_TEI6		GENBL0
#define GEN_SCI6_ERI6		GENBL0
#define GEN_SCI12_TEI12		GENBL0
#define GEN_SCI12_ERI12		GENBL0
#define GEN_SCI12_SCIX0		GENBL0
#define GEN_SCI12_SCIX1		GENBL0
#define GEN_SCI12_SCIX2		GENBL0
#define GEN_SCI12_SCIX3		GENBL0
#define GEN_CAC_FERRI		GENBL0
#define GEN_CAC_MENDI		GENBL0
#define GEN_CAC_OVFI		GENBL0
#define GEN_DOC_DOPCI		GENBL0
#define GEN_POEG_POEGGAI		GENBL1
#define GEN_POEG_POEGGBI		GENBL1
#define GEN_POEG_POEGGCI		GENBL1
#define GEN_POEG_POEGGDI		GENBL1
#define GEN_POE_OEI5		GENBL1
#define GEN_POE_OEI1		GENBL1
#define GEN_POE_OEI2		GENBL1
#define GEN_POE_OEI3		GENBL1
#define GEN_POE_OEI4		GENBL1
#define GEN_RIIC0_TEI0		GENBL1
#define GEN_RIIC0_EEI0		GENBL1
#define GEN_S12AD2_S12CMPAI2		GENBL1
#define GEN_S12AD2_S12CMPBI2		GENBL1
#define GEN_S12AD_S12CMPAI		GENBL1
#define GEN_S12AD_S12CMPBI		GENBL1
#define GEN_S12AD1_S12CMPAI1		GENBL1
#define GEN_S12AD1_S12CMPBI1		GENBL1
#define GEN_RSCI8_TEI		GENBL1
#define GEN_RSCI8_ERI		GENBL1
#define GEN_RSCI9_TEI		GENBL1
#define GEN_RSCI9_ERI		GENBL1
#define GEN_RSCI9_BFD		GENBL1
#define GEN_CANFD0_CHEI		GENBL2
#define GEN_CANFD0_CFRI		GENBL2
#define GEN_CANFD_GLEI		GENBL2
#define GEN_CANFD_RFRI		GENBL2
#define GEN_CANFD0_CHTI		GENBL2
#define GEN_CANFD_RMRI		GENBL2
#define GEN_RSCI11_TEI		GENAL0
#define GEN_RSCI11_ERI		GENAL0
#define GEN_RSCI11_BFD		GENAL0
#define GEN_RSPI0_SPII0		GENAL0
#define GEN_RSPI0_SPEI0		GENAL0
#define GEN_RSPIA0_SPII		GENAL0
#define GEN_RSPIA0_SPEI		GENAL0
#define GEN_RI3C0_EEI		GENAL1

#define GRP_SCI1_TEI1		GRPBL0
#define GRP_SCI1_ERI1		GRPBL0
#define GRP_SCI5_TEI5		GRPBL0
#define GRP_SCI5_ERI5		GRPBL0
#define GRP_SCI6_TEI6		GRPBL0
#define GRP_SCI6_ERI6		GRPBL0
#define GRP_SCI12_TEI12		GRPBL0
#define GRP_SCI12_ERI12		GRPBL0
#define GRP_SCI12_SCIX0		GRPBL0
#define GRP_SCI12_SCIX1		GRPBL0
#define GRP_SCI12_SCIX2		GRPBL0
#define GRP_SCI12_SCIX3		GRPBL0
#define GRP_CAC_FERRI		GRPBL0
#define GRP_CAC_MENDI		GRPBL0
#define GRP_CAC_OVFI		GRPBL0
#define GRP_DOC_DOPCI		GRPBL0
#define GRP_POEG_POEGGAI		GRPBL1
#define GRP_POEG_POEGGBI		GRPBL1
#define GRP_POEG_POEGGCI		GRPBL1
#define GRP_POEG_POEGGDI		GRPBL1
#define GRP_POE_OEI5		GRPBL1
#define GRP_POE_OEI1		GRPBL1
#define GRP_POE_OEI2		GRPBL1
#define GRP_POE_OEI3		GRPBL1
#define GRP_POE_OEI4		GRPBL1
#define GRP_RIIC0_TEI0		GRPBL1
#define GRP_RIIC0_EEI0		GRPBL1
#define GRP_S12AD2_S12CMPAI2		GRPBL1
#define GRP_S12AD2_S12CMPBI2		GRPBL1
#define GRP_S12AD_S12CMPAI		GRPBL1
#define GRP_S12AD_S12CMPBI		GRPBL1
#define GRP_S12AD1_S12CMPAI1		GRPBL1
#define GRP_S12AD1_S12CMPBI1		GRPBL1
#define GRP_RSCI8_TEI		GRPBL1
#define GRP_RSCI8_ERI		GRPBL1
#define GRP_RSCI9_TEI		GRPBL1
#define GRP_RSCI9_ERI		GRPBL1
#define GRP_RSCI9_BFD		GRPBL1
#define GRP_CANFD0_CHEI		GRPBL2
#define GRP_CANFD0_CFRI		GRPBL2
#define GRP_CANFD_GLEI		GRPBL2
#define GRP_CANFD_RFRI		GRPBL2
#define GRP_CANFD0_CHTI		GRPBL2
#define GRP_CANFD_RMRI		GRPBL2
#define GRP_RSCI11_TEI		GRPAL0
#define GRP_RSCI11_ERI		GRPAL0
#define GRP_RSCI11_BFD		GRPAL0
#define GRP_RSPI0_SPII0		GRPAL0
#define GRP_RSPI0_SPEI0		GRPAL0
#define GRP_RSPIA0_SPII		GRPAL0
#define GRP_RSPIA0_SPEI		GRPAL0
#define GRP_RI3C0_EEI		GRPAL1

#define	__IR( x )		ICU.IR[ IR ## x ].BIT.IR
#define	 _IR( x )		__IR( x )
#define	  IR( x , y )	_IR( _ ## x ## _ ## y )
#define	__DTCE( x )		ICU.DTCER[ DTCE ## x ].BIT.DTCE
#define	 _DTCE( x )		__DTCE( x )
#define	  DTCE( x , y )	_DTCE( _ ## x ## _ ## y )
#define	__IEN( x )		ICU.IER[ IER ## x ].BIT.IEN ## x
#define	 _IEN( x )		__IEN( x )
#define	  IEN( x , y )	_IEN( _ ## x ## _ ## y )
#define	__IPR( x )		ICU.IPR[ IPR ## x ].BIT.IPR
#define	 _IPR( x )		__IPR( x )
#define	  IPR( x , y )	_IPR( _ ## x ## _ ## y )
#define	__VECT( x )		VECT ## x
#define	 _VECT( x )		__VECT( x )
#define	  VECT( x , y )	_VECT( _ ## x ## _ ## y )
#define	__MSTP( x )		MSTP ## x
#define	 _MSTP( x )		__MSTP( x )
#define	  MSTP( x )		_MSTP( _ ## x )

#define	__IS( x )		ICU.GRP ## x.BIT.IS ## x
#define	 _IS( x )		__IS( x )
#define	  IS( x , y )	_IS( _ ## x ## _ ## y )
#define	__EN( x )		ICU.GEN ## x.BIT.EN ## x
#define	 _EN( x )		__EN( x )
#define	  EN( x , y )	_EN( _ ## x ## _ ## y )
#define	__CLR( x )		ICU.GCR ## x.BIT.CLR ## x
#define	 _CLR( x )		__CLR( x )
#define	  CLR( x , y )	_CLR( _ ## x ## _ ## y )

#define	BSC		(*(volatile struct st_bsc     *)0x81300)
#define	CAC		(*(volatile struct st_cac     *)0x8B000)
#define	CANFD	(*(volatile struct st_canfd   *)0xA8014)
#define	CANFD0	(*(volatile struct st_canfd0  *)0xA8000)
#define	CMPC0	(*(volatile struct st_cmpc    *)0xA0C80)
#define	CMPC1	(*(volatile struct st_cmpc    *)0xA0CA0)
#define	CMPC2	(*(volatile struct st_cmpc    *)0xA0CC0)
#define	CMPC3	(*(volatile struct st_cmpc    *)0xA0CE0)
#define	CMPC4	(*(volatile struct st_cmpc    *)0xA0D00)
#define	CMPC5	(*(volatile struct st_cmpc    *)0xA0D20)
#define	CMT		(*(volatile struct st_cmt     *)0x88000)
#define	CMT0	(*(volatile struct st_cmt0    *)0x88002)
#define	CMT1	(*(volatile struct st_cmt0    *)0x88008)
#define	CMT2	(*(volatile struct st_cmt0    *)0x88012)
#define	CMT3	(*(volatile struct st_cmt0    *)0x88018)
#define	CMTW0	(*(volatile struct st_cmtw    *)0x94200)
#define	CMTW1	(*(volatile struct st_cmtw    *)0x94280)
#define	CRC		(*(volatile struct st_crc     *)0x88280)
#define	DA		(*(volatile struct st_da      *)0x88040)
#define	DMAC	(*(volatile struct st_dmac    *)0x82200)
#define	DMAC0	(*(volatile struct st_dmac0   *)0x82000)
#define	DMAC1	(*(volatile struct st_dmac1   *)0x82040)
#define	DMAC2	(*(volatile struct st_dmac1   *)0x82080)
#define	DMAC3	(*(volatile struct st_dmac1   *)0x820C0)
#define	DMAC4	(*(volatile struct st_dmac1   *)0x82100)
#define	DMAC5	(*(volatile struct st_dmac1   *)0x82140)
#define	DMAC6	(*(volatile struct st_dmac1   *)0x82180)
#define	DMAC7	(*(volatile struct st_dmac1   *)0x821C0)
#define	DOC		(*(volatile struct st_doc     *)0xA0580)
#define	DTC		(*(volatile struct st_dtc     *)0x82400)
#define	ELC		(*(volatile struct st_elc     *)0x8B100)
#define	FLASH	(*(volatile struct st_flash   *)0x8C294)
#define	GPTW	(*(volatile struct st_gptw    *)0xC2B00)
#define	GPTW0	(*(volatile struct st_gptw0   *)0xC2000)
#define	GPTW1	(*(volatile struct st_gptw0   *)0xC2100)
#define	GPTW2	(*(volatile struct st_gptw0   *)0xC2200)
#define	GPTW3	(*(volatile struct st_gptw0   *)0xC2300)
#define	GPTW4	(*(volatile struct st_gptw0   *)0xC2400)
#define	GPTW5	(*(volatile struct st_gptw0   *)0xC2500)
#define	GPTW6	(*(volatile struct st_gptw0   *)0xC2600)
#define	GPTW7	(*(volatile struct st_gptw0   *)0xC2700)
#define	HRPWM	(*(volatile struct st_hrpwm   *)0xC2A00)
#define	ICU		(*(volatile struct st_icu     *)0x87000)
#define	IWDT	(*(volatile struct st_iwdt    *)0x88030)
#define	MPC		(*(volatile struct st_mpc     *)0x8C11F)
#define	MPU		(*(volatile struct st_mpu     *)0x86400)
#define	MTU		(*(volatile struct st_mtu     *)0xC120A)
#define	MTU0	(*(volatile struct st_mtu0    *)0xC1290)
#define	MTU1	(*(volatile struct st_mtu1    *)0xC1290)
#define	MTU2	(*(volatile struct st_mtu2    *)0xC1292)
#define	MTU3	(*(volatile struct st_mtu3    *)0xC1200)
#define	MTU4	(*(volatile struct st_mtu4    *)0xC1200)
#define	MTU5	(*(volatile struct st_mtu5    *)0xC1A94)
#define	MTU6	(*(volatile struct st_mtu6    *)0xC1A00)
#define	MTU7	(*(volatile struct st_mtu7    *)0xC1A00)
#define	MTU9	(*(volatile struct st_mtu9    *)0xC1296)
#define	OFSM	(*(volatile struct st_ofsm    *)0x120040)
#define	POE		(*(volatile struct st_poe     *)0x9E400)
#define	POEG	(*(volatile struct st_poeg    *)0x9E000)
#define	PORT	(*(volatile struct st_port    *)0x8C110)
#define	PORT0	(*(volatile struct st_port0   *)0x8C000)
#define	PORT1	(*(volatile struct st_port1   *)0x8C001)
#define	PORT2	(*(volatile struct st_port2   *)0x8C002)
#define	PORT3	(*(volatile struct st_port3   *)0x8C003)
#define	PORT4	(*(volatile struct st_port4   *)0x8C004)
#define	PORT5	(*(volatile struct st_port5   *)0x8C005)
#define	PORT6	(*(volatile struct st_port6   *)0x8C006)
#define	PORT7	(*(volatile struct st_port7   *)0x8C007)
#define	PORT8	(*(volatile struct st_port8   *)0x8C008)
#define	PORT9	(*(volatile struct st_port9   *)0x8C009)
#define	PORTA	(*(volatile struct st_porta   *)0x8C00A)
#define	PORTB	(*(volatile struct st_portb   *)0x8C00B)
#define	PORTD	(*(volatile struct st_portd   *)0x8C00D)
#define	PORTE	(*(volatile struct st_porte   *)0x8C00E)
#define	PORTN	(*(volatile struct st_portn   *)0x8C016)
#define	RAM		(*(volatile struct st_ram     *)0x81200)
#define	RI3C0	(*(volatile struct st_ri3c    *)0xEC000)
#define	RIIC0	(*(volatile struct st_riic    *)0x88300)
#define	RSCI8	(*(volatile struct st_rsci8   *)0xA1400)
#define	RSCI9	(*(volatile struct st_rsci9   *)0xA1480)
#define	RSCI11	(*(volatile struct st_rsci11  *)0xE2080)
#define	RSPI0	(*(volatile struct st_rspi    *)0xD0100)
#define	RSPIA0	(*(volatile struct st_rspia   *)0xE2800)
#define	S12AD	(*(volatile struct st_s12ad   *)0x89000)
#define	S12AD1	(*(volatile struct st_s12ad1  *)0x89200)
#define	S12AD2	(*(volatile struct st_s12ad2  *)0x89400)
#define	SCI1	(*(volatile struct st_sci1    *)0x8A020)
#define	SCI5	(*(volatile struct st_sci1    *)0x8A0A0)
#define	SCI6	(*(volatile struct st_sci1    *)0x8A0C0)
#define	SCI12	(*(volatile struct st_sci12   *)0x8B300)
#define	SMCI1	(*(volatile struct st_smci    *)0x8A020)
#define	SMCI5	(*(volatile struct st_smci    *)0x8A0A0)
#define	SMCI6	(*(volatile struct st_smci    *)0x8A0C0)
#define	SMCI12	(*(volatile struct st_smci    *)0x8B300)
#define	SYSTEM	(*(volatile struct st_system  *)0x80000)
#define	TEMPS	(*(volatile struct st_temps   *)0x7FB17C)
#define	TFU		(*(volatile struct st_tfu     *)0x81404)
#define	TMR0	(*(volatile struct st_tmr0    *)0x88200)
#define	TMR1	(*(volatile struct st_tmr1    *)0x88201)
#define	TMR2	(*(volatile struct st_tmr0    *)0x88210)
#define	TMR3	(*(volatile struct st_tmr1    *)0x88211)
#define	TMR4	(*(volatile struct st_tmr4    *)0x88220)
#define	TMR5	(*(volatile struct st_tmr5    *)0x88221)
#define	TMR6	(*(volatile struct st_tmr4    *)0x88230)
#define	TMR7	(*(volatile struct st_tmr5    *)0x88231)
#define	TMR01	(*(volatile struct st_tmr01   *)0x88204)
#define	TMR23	(*(volatile struct st_tmr01   *)0x88214)
#define	TMR45	(*(volatile struct st_tmr01   *)0x88224)
#define	TMR67	(*(volatile struct st_tmr01   *)0x88234)
#define	WDT		(*(volatile struct st_wdt     *)0x88020)

typedef enum enum_ir {
IR_BSC_BUSERR=16,IR_RAM_RAMERR=18,
IR_FCU_FIFERR=21,IR_FCU_FRDYI=23,
IR_ICU_SWINT2=26,IR_ICU_SWINT,
IR_CMT0_CMI0,
IR_CMT1_CMI1,
IR_CMT2_CMI2,
IR_CMT3_CMI3,
IR_RSPI0_SPRI0=38,IR_RSPI0_SPTI0,
IR_RI3C0_RESPI,IR_RI3C0_CMDI,IR_RI3C0_IBII,IR_RI3C0_RCVI,
IR_RSPIA0_SPRI=48,IR_RSPIA0_SPTI,
IR_RIIC0_RXI0=52,IR_RIIC0_TXI0,
IR_SCI1_RXI1=60,IR_SCI1_TXI1,
IR_ICU_IRQ0=64,IR_ICU_IRQ1,IR_ICU_IRQ2,IR_ICU_IRQ3,IR_ICU_IRQ4,IR_ICU_IRQ5,IR_ICU_IRQ6,IR_ICU_IRQ7,
IR_ICU_IRQ8,IR_ICU_IRQ9,IR_ICU_IRQ10,IR_ICU_IRQ11,IR_ICU_IRQ12,IR_ICU_IRQ13,IR_ICU_IRQ14,IR_ICU_IRQ15,
IR_SCI5_RXI5=84,IR_SCI5_TXI5,
IR_SCI6_RXI6,IR_SCI6_TXI6,
IR_LVD1_LVD1,
IR_LVD2_LVD2,
IR_IWDT_IWUNI=95,
IR_WDT_WUNI,
IR_RSCI8_RXI=100,IR_RSCI8_TXI,
IR_RSCI9_RXI,IR_RSCI9_TXI,
IR_ICU_GROUPBL2=107,IR_ICU_GROUPBL0=110,IR_ICU_GROUPBL1,IR_ICU_GROUPAL0,IR_ICU_GROUPAL1,
IR_RSCI11_RXI,IR_RSCI11_TXI,
IR_SCI12_RXI12,IR_SCI12_TXI12,
IR_RI3C0_RXI,IR_RI3C0_TXI,
IR_DMAC_DMAC0I,IR_DMAC_DMAC1I,IR_DMAC_DMAC2I,IR_DMAC_DMAC3I,IR_DMAC_DMAC74I,
IR_OST_OSTDI,
IR_S12AD_S12ADI=128,IR_S12AD_S12GBADI,IR_S12AD_S12GCADI,
IR_S12AD1_S12ADI1=132,IR_S12AD1_S12GBADI1,IR_S12AD1_S12GCADI1,
IR_S12AD2_S12ADI2=136,IR_S12AD2_S12GBADI2,IR_S12AD2_S12GCADI2,
IR_CANFD_RFDREQ0=140,IR_CANFD_RFDREQ1,
IR_CANFD0_CFDREQ0,
IR_TMR0_CMIA0=146,IR_TMR0_CMIB0,IR_TMR0_OVI0,
IR_TMR1_CMIA1,IR_TMR1_CMIB1,IR_TMR1_OVI1,
IR_TMR2_CMIA2,IR_TMR2_CMIB2,IR_TMR2_OVI2,
IR_TMR3_CMIA3,IR_TMR3_CMIB3,IR_TMR3_OVI3,
IR_TMR4_CMIA4,IR_TMR4_CMIB4,IR_TMR4_OVI4,
IR_TMR5_CMIA5,IR_TMR5_CMIB5,IR_TMR5_OVI5,
IR_TMR6_CMIA6,IR_TMR6_CMIB6,IR_TMR6_OVI6,
IR_TMR7_CMIA7,IR_TMR7_CMIB7,IR_TMR7_OVI7,
IR_ELC_ELSR18I=175,IR_ELC_ELSR19I,
IR_TSIP_RD,IR_TSIP_WR,IR_TSIP_ERR,
IR_CMPC0_CMPC0,
IR_CMPC1_CMPC1,
IR_CMPC2_CMPC2,
IR_CMPC3_CMPC3,
IR_CMPC4_CMPC4,
IR_CMPC5_CMPC5,
IR_CMTW0_CMWI0,IR_CMTW0_IC0I0,IR_CMTW0_IC1I0,IR_CMTW0_OC0I0,IR_CMTW0_OC1I0,
IR_CMTW1_CMWI1,IR_CMTW1_IC0I1,IR_CMTW1_IC1I1,IR_CMTW1_OC0I1,IR_CMTW1_OC1I1,
IR_RSCI9_AED=197,
IR_PERIA_INTA208=208,IR_PERIA_INTA209,IR_PERIA_INTA210,IR_PERIA_INTA211,IR_PERIA_INTA212,IR_PERIA_INTA213,
IR_PERIA_INTA214,IR_PERIA_INTA215,IR_PERIA_INTA216,IR_PERIA_INTA217,IR_PERIA_INTA218,IR_PERIA_INTA219,
IR_PERIA_INTA220,IR_PERIA_INTA221,IR_PERIA_INTA222,IR_PERIA_INTA223,IR_PERIA_INTA224,IR_PERIA_INTA225,
IR_PERIA_INTA226,IR_PERIA_INTA227,IR_PERIA_INTA228,IR_PERIA_INTA229,IR_PERIA_INTA230,IR_PERIA_INTA231,
IR_PERIA_INTA232,IR_PERIA_INTA233,IR_PERIA_INTA234,IR_PERIA_INTA235,IR_PERIA_INTA236,IR_PERIA_INTA237,
IR_PERIA_INTA238,IR_PERIA_INTA239,IR_PERIA_INTA240,IR_PERIA_INTA241,IR_PERIA_INTA242,IR_PERIA_INTA243,
IR_PERIA_INTA244,IR_PERIA_INTA245,IR_PERIA_INTA246,IR_PERIA_INTA247,IR_PERIA_INTA248,IR_PERIA_INTA249,
IR_PERIA_INTA250,IR_PERIA_INTA251,IR_PERIA_INTA252,IR_PERIA_INTA253,IR_PERIA_INTA254,IR_PERIA_INTA255
} enum_ir_t;

typedef enum enum_dtce {
DTCE_ICU_SWINT2=26,DTCE_ICU_SWINT,
DTCE_CMT0_CMI0,
DTCE_CMT1_CMI1,
DTCE_CMT2_CMI2,
DTCE_CMT3_CMI3,
DTCE_RSPI0_SPRI0=38,DTCE_RSPI0_SPTI0,
DTCE_RI3C0_RESPI,DTCE_RI3C0_CMDI,DTCE_RI3C0_IBII,DTCE_RI3C0_RCVI,
DTCE_RSPIA0_SPRI=48,DTCE_RSPIA0_SPTI,
DTCE_RIIC0_RXI0=52,DTCE_RIIC0_TXI0,
DTCE_SCI1_RXI1=60,DTCE_SCI1_TXI1,
DTCE_ICU_IRQ0=64,DTCE_ICU_IRQ1,DTCE_ICU_IRQ2,DTCE_ICU_IRQ3,DTCE_ICU_IRQ4,DTCE_ICU_IRQ5,DTCE_ICU_IRQ6,DTCE_ICU_IRQ7,
DTCE_ICU_IRQ8,DTCE_ICU_IRQ9,DTCE_ICU_IRQ10,DTCE_ICU_IRQ11,DTCE_ICU_IRQ12,DTCE_ICU_IRQ13,DTCE_ICU_IRQ14,DTCE_ICU_IRQ15,
DTCE_SCI5_RXI5=84,DTCE_SCI5_TXI5,
DTCE_SCI6_RXI6,DTCE_SCI6_TXI6,
DTCE_RSCI8_RXI=100,DTCE_RSCI8_TXI,
DTCE_RSCI9_RXI,DTCE_RSCI9_TXI,
DTCE_RSCI11_RXI=114,DTCE_RSCI11_TXI,
DTCE_SCI12_RXI12,DTCE_SCI12_TXI12,
DTCE_RI3C0_RXI,DTCE_RI3C0_TXI,
DTCE_DMAC_DMAC0I,DTCE_DMAC_DMAC1I,DTCE_DMAC_DMAC2I,DTCE_DMAC_DMAC3I,
DTCE_S12AD_S12ADI=128,DTCE_S12AD_S12GBADI,DTCE_S12AD_S12GCADI,
DTCE_S12AD1_S12ADI1=132,DTCE_S12AD1_S12GBADI1,DTCE_S12AD1_S12GCADI1,
DTCE_S12AD2_S12ADI2=136,DTCE_S12AD2_S12GBADI2,DTCE_S12AD2_S12GCADI2,
DTCE_CANFD_RFDREQ0=140,DTCE_CANFD_RFDREQ1,
DTCE_CANFD0_CFDREQ0,
DTCE_TMR0_CMIA0=146,DTCE_TMR0_CMIB0,
DTCE_TMR1_CMIA1=149,DTCE_TMR1_CMIB1,
DTCE_TMR2_CMIA2=152,DTCE_TMR2_CMIB2,
DTCE_TMR3_CMIA3=155,DTCE_TMR3_CMIB3,
DTCE_TMR4_CMIA4=158,DTCE_TMR4_CMIB4,
DTCE_TMR5_CMIA5=161,DTCE_TMR5_CMIB5,
DTCE_TMR6_CMIA6=164,DTCE_TMR6_CMIB6,
DTCE_TMR7_CMIA7=167,DTCE_TMR7_CMIB7,
DTCE_ELC_ELSR18I=175,DTCE_ELC_ELSR19I,
DTCE_TSIP_RD,DTCE_TSIP_WR,
DTCE_CMPC0_CMPC0=180,
DTCE_CMPC1_CMPC1,
DTCE_CMPC2_CMPC2,
DTCE_CMPC3_CMPC3,
DTCE_CMPC4_CMPC4,
DTCE_CMPC5_CMPC5,
DTCE_CMTW0_CMWI0,DTCE_CMTW0_IC0I0,DTCE_CMTW0_IC1I0,DTCE_CMTW0_OC0I0,DTCE_CMTW0_OC1I0,
DTCE_CMTW1_CMWI1,DTCE_CMTW1_IC0I1,DTCE_CMTW1_IC1I1,DTCE_CMTW1_OC0I1,DTCE_CMTW1_OC1I1,
DTCE_RSCI9_AED=197,
DTCE_PERIA_INTA208=208,DTCE_PERIA_INTA209,DTCE_PERIA_INTA210,DTCE_PERIA_INTA211,DTCE_PERIA_INTA212,DTCE_PERIA_INTA213,
DTCE_PERIA_INTA214,DTCE_PERIA_INTA215,DTCE_PERIA_INTA216,DTCE_PERIA_INTA217,DTCE_PERIA_INTA218,DTCE_PERIA_INTA219,
DTCE_PERIA_INTA220,DTCE_PERIA_INTA221,DTCE_PERIA_INTA222,DTCE_PERIA_INTA223,DTCE_PERIA_INTA224,DTCE_PERIA_INTA225,
DTCE_PERIA_INTA226,DTCE_PERIA_INTA227,DTCE_PERIA_INTA228,DTCE_PERIA_INTA229,DTCE_PERIA_INTA230,DTCE_PERIA_INTA231,
DTCE_PERIA_INTA232,DTCE_PERIA_INTA233,DTCE_PERIA_INTA234,DTCE_PERIA_INTA235,DTCE_PERIA_INTA236,DTCE_PERIA_INTA237,
DTCE_PERIA_INTA238,DTCE_PERIA_INTA239,DTCE_PERIA_INTA240,DTCE_PERIA_INTA241,DTCE_PERIA_INTA242,DTCE_PERIA_INTA243,
DTCE_PERIA_INTA244,DTCE_PERIA_INTA245,DTCE_PERIA_INTA246,DTCE_PERIA_INTA247,DTCE_PERIA_INTA248,DTCE_PERIA_INTA249,
DTCE_PERIA_INTA250,DTCE_PERIA_INTA251,DTCE_PERIA_INTA252,DTCE_PERIA_INTA253,DTCE_PERIA_INTA254,DTCE_PERIA_INTA255
} enum_dtce_t;

typedef enum enum_ier {
IER_BSC_BUSERR=0x02,
IER_RAM_RAMERR=0x02,
IER_FCU_FIFERR=0x02,IER_FCU_FRDYI=0x02,
IER_ICU_SWINT2=0x03,IER_ICU_SWINT=0x03,
IER_CMT0_CMI0=0x03,
IER_CMT1_CMI1=0x03,
IER_CMT2_CMI2=0x03,
IER_CMT3_CMI3=0x03,
IER_RSPI0_SPRI0=0x04,IER_RSPI0_SPTI0=0x04,
IER_RI3C0_RESPI=0x05,IER_RI3C0_CMDI=0x05,IER_RI3C0_IBII=0x05,IER_RI3C0_RCVI=0x05,
IER_RSPIA0_SPRI=0x06,IER_RSPIA0_SPTI=0x06,
IER_RIIC0_RXI0=0x06,IER_RIIC0_TXI0=0x06,
IER_SCI1_RXI1=0x07,IER_SCI1_TXI1=0x07,
IER_ICU_IRQ0=0x08,IER_ICU_IRQ1=0x08,IER_ICU_IRQ2=0x08,IER_ICU_IRQ3=0x08,IER_ICU_IRQ4=0x08,IER_ICU_IRQ5=0x08,IER_ICU_IRQ6=0x08,IER_ICU_IRQ7=0x08,
IER_ICU_IRQ8=0x09,IER_ICU_IRQ9=0x09,IER_ICU_IRQ10=0x09,IER_ICU_IRQ11=0x09,IER_ICU_IRQ12=0x09,IER_ICU_IRQ13=0x09,IER_ICU_IRQ14=0x09,IER_ICU_IRQ15=0x09,
IER_SCI5_RXI5=0x0A,IER_SCI5_TXI5=0x0A,
IER_SCI6_RXI6=0x0A,IER_SCI6_TXI6=0x0A,
IER_LVD1_LVD1=0x0B,
IER_LVD2_LVD2=0x0B,
IER_IWDT_IWUNI=0x0B,
IER_WDT_WUNI=0x0C,
IER_RSCI8_RXI=0x0C,IER_RSCI8_TXI=0x0C,
IER_RSCI9_RXI=0x0C,IER_RSCI9_TXI=0x0C,
IER_ICU_GROUPBL2=0x0D,IER_ICU_GROUPBL0=0x0D,IER_ICU_GROUPBL1=0x0D,IER_ICU_GROUPAL0=0x0E,IER_ICU_GROUPAL1=0x0E,
IER_RSCI11_RXI=0x0E,IER_RSCI11_TXI=0x0E,
IER_SCI12_RXI12=0x0E,IER_SCI12_TXI12=0x0E,
IER_RI3C0_RXI=0x0E,IER_RI3C0_TXI=0x0E,
IER_DMAC_DMAC0I=0x0F,IER_DMAC_DMAC1I=0x0F,IER_DMAC_DMAC2I=0x0F,IER_DMAC_DMAC3I=0x0F,IER_DMAC_DMAC74I=0x0F,
IER_OST_OSTDI=0x0F,
IER_S12AD_S12ADI=0x10,IER_S12AD_S12GBADI=0x10,IER_S12AD_S12GCADI=0x10,
IER_S12AD1_S12ADI1=0x10,IER_S12AD1_S12GBADI1=0x10,IER_S12AD1_S12GCADI1=0x10,
IER_S12AD2_S12ADI2=0x11,IER_S12AD2_S12GBADI2=0x11,IER_S12AD2_S12GCADI2=0x11,
IER_CANFD_RFDREQ0=0x11,IER_CANFD_RFDREQ1=0x11,
IER_CANFD0_CFDREQ0=0x11,
IER_TMR0_CMIA0=0x12,IER_TMR0_CMIB0=0x12,IER_TMR0_OVI0=0x12,
IER_TMR1_CMIA1=0x12,IER_TMR1_CMIB1=0x12,IER_TMR1_OVI1=0x12,
IER_TMR2_CMIA2=0x13,IER_TMR2_CMIB2=0x13,IER_TMR2_OVI2=0x13,
IER_TMR3_CMIA3=0x13,IER_TMR3_CMIB3=0x13,IER_TMR3_OVI3=0x13,
IER_TMR4_CMIA4=0x13,IER_TMR4_CMIB4=0x13,IER_TMR4_OVI4=0x14,
IER_TMR5_CMIA5=0x14,IER_TMR5_CMIB5=0x14,IER_TMR5_OVI5=0x14,
IER_TMR6_CMIA6=0x14,IER_TMR6_CMIB6=0x14,IER_TMR6_OVI6=0x14,
IER_TMR7_CMIA7=0x14,IER_TMR7_CMIB7=0x15,IER_TMR7_OVI7=0x15,
IER_ELC_ELSR18I=0x15,IER_ELC_ELSR19I=0x16,
IER_TSIP_RD=0x16,IER_TSIP_WR=0x16,IER_TSIP_ERR=0x16,
IER_CMPC0_CMPC0=0x16,
IER_CMPC1_CMPC1=0x16,
IER_CMPC2_CMPC2=0x16,
IER_CMPC3_CMPC3=0x16,
IER_CMPC4_CMPC4=0x17,
IER_CMPC5_CMPC5=0x17,
IER_CMTW0_CMWI0=0x17,IER_CMTW0_IC0I0=0x17,IER_CMTW0_IC1I0=0x17,IER_CMTW0_OC0I0=0x17,IER_CMTW0_OC1I0=0x17,
IER_CMTW1_CMWI1=0x17,IER_CMTW1_IC0I1=0x18,IER_CMTW1_IC1I1=0x18,IER_CMTW1_OC0I1=0x18,IER_CMTW1_OC1I1=0x18,
IER_RSCI9_AED=0x18,
IER_PERIA_INTA208=0x1A,IER_PERIA_INTA209=0x1A,IER_PERIA_INTA210=0x1A,IER_PERIA_INTA211=0x1A,IER_PERIA_INTA212=0x1A,IER_PERIA_INTA213=0x1A,IER_PERIA_INTA214=0x1A,
IER_PERIA_INTA215=0x1A,IER_PERIA_INTA216=0x1B,IER_PERIA_INTA217=0x1B,IER_PERIA_INTA218=0x1B,IER_PERIA_INTA219=0x1B,IER_PERIA_INTA220=0x1B,IER_PERIA_INTA221=0x1B,
IER_PERIA_INTA222=0x1B,IER_PERIA_INTA223=0x1B,IER_PERIA_INTA224=0x1C,IER_PERIA_INTA225=0x1C,IER_PERIA_INTA226=0x1C,IER_PERIA_INTA227=0x1C,IER_PERIA_INTA228=0x1C,
IER_PERIA_INTA229=0x1C,IER_PERIA_INTA230=0x1C,IER_PERIA_INTA231=0x1C,IER_PERIA_INTA232=0x1D,IER_PERIA_INTA233=0x1D,IER_PERIA_INTA234=0x1D,IER_PERIA_INTA235=0x1D,
IER_PERIA_INTA236=0x1D,IER_PERIA_INTA237=0x1D,IER_PERIA_INTA238=0x1D,IER_PERIA_INTA239=0x1D,IER_PERIA_INTA240=0x1E,IER_PERIA_INTA241=0x1E,IER_PERIA_INTA242=0x1E,
IER_PERIA_INTA243=0x1E,IER_PERIA_INTA244=0x1E,IER_PERIA_INTA245=0x1E,IER_PERIA_INTA246=0x1E,IER_PERIA_INTA247=0x1E,IER_PERIA_INTA248=0x1F,IER_PERIA_INTA249=0x1F,
IER_PERIA_INTA250=0x1F,IER_PERIA_INTA251=0x1F,IER_PERIA_INTA252=0x1F,IER_PERIA_INTA253=0x1F,IER_PERIA_INTA254=0x1F,IER_PERIA_INTA255=0x1F
} enum_ier_t;

typedef enum enum_ipr {
IPR_BSC_BUSERR=0,
IPR_RAM_RAMERR=0,
IPR_FCU_FIFERR=1,IPR_FCU_FRDYI=2,
IPR_ICU_SWINT2=3,IPR_ICU_SWINT=3,
IPR_CMT0_CMI0=4,
IPR_CMT1_CMI1=5,
IPR_CMT2_CMI2=6,
IPR_CMT3_CMI3=7,
IPR_RSPI0_SPRI0=38,IPR_RSPI0_SPTI0=39,
IPR_RI3C0_RESPI=40,IPR_RI3C0_CMDI=41,IPR_RI3C0_IBII=42,IPR_RI3C0_RCVI=43,
IPR_RSPIA0_SPRI=48,IPR_RSPIA0_SPTI=49,
IPR_RIIC0_RXI0=52,IPR_RIIC0_TXI0=53,
IPR_SCI1_RXI1=60,IPR_SCI1_TXI1=61,
IPR_ICU_IRQ0=64,IPR_ICU_IRQ1=65,IPR_ICU_IRQ2=66,IPR_ICU_IRQ3=67,IPR_ICU_IRQ4=68,IPR_ICU_IRQ5=69,IPR_ICU_IRQ6=70,IPR_ICU_IRQ7=71,
IPR_ICU_IRQ8=72,IPR_ICU_IRQ9=73,IPR_ICU_IRQ10=74,IPR_ICU_IRQ11=75,IPR_ICU_IRQ12=76,IPR_ICU_IRQ13=77,IPR_ICU_IRQ14=78,IPR_ICU_IRQ15=79,
IPR_SCI5_RXI5=84,IPR_SCI5_TXI5=85,
IPR_SCI6_RXI6=86,IPR_SCI6_TXI6=87,
IPR_LVD1_LVD1=88,
IPR_LVD2_LVD2=89,
IPR_IWDT_IWUNI=95,
IPR_WDT_WUNI=96,
IPR_RSCI8_RXI=100,IPR_RSCI8_TXI=101,
IPR_RSCI9_RXI=102,IPR_RSCI9_TXI=103,
IPR_ICU_GROUPBL2=107,IPR_ICU_GROUPBL0=110,IPR_ICU_GROUPBL1=111,IPR_ICU_GROUPAL0=112,IPR_ICU_GROUPAL1=113,
IPR_RSCI11_RXI=114,IPR_RSCI11_TXI=115,
IPR_SCI12_RXI12=116,IPR_SCI12_TXI12=117,
IPR_RI3C0_RXI=118,IPR_RI3C0_TXI=119,
IPR_DMAC_DMAC0I=120,IPR_DMAC_DMAC1I=121,IPR_DMAC_DMAC2I=122,IPR_DMAC_DMAC3I=123,IPR_DMAC_DMAC74I=124,
IPR_OST_OSTDI=125,
IPR_S12AD_S12ADI=128,IPR_S12AD_S12GBADI=129,IPR_S12AD_S12GCADI=130,
IPR_S12AD1_S12ADI1=132,IPR_S12AD1_S12GBADI1=133,IPR_S12AD1_S12GCADI1=134,
IPR_S12AD2_S12ADI2=136,IPR_S12AD2_S12GBADI2=137,IPR_S12AD2_S12GCADI2=138,
IPR_CANFD_RFDREQ0=140,IPR_CANFD_RFDREQ1=141,
IPR_CANFD0_CFDREQ0=142,
IPR_TMR0_CMIA0=146,IPR_TMR0_CMIB0=146,IPR_TMR0_OVI0=146,
IPR_TMR1_CMIA1=149,IPR_TMR1_CMIB1=149,IPR_TMR1_OVI1=149,
IPR_TMR2_CMIA2=152,IPR_TMR2_CMIB2=152,IPR_TMR2_OVI2=152,
IPR_TMR3_CMIA3=155,IPR_TMR3_CMIB3=155,IPR_TMR3_OVI3=155,
IPR_TMR4_CMIA4=158,IPR_TMR4_CMIB4=158,IPR_TMR4_OVI4=158,
IPR_TMR5_CMIA5=161,IPR_TMR5_CMIB5=161,IPR_TMR5_OVI5=161,
IPR_TMR6_CMIA6=164,IPR_TMR6_CMIB6=164,IPR_TMR6_OVI6=164,
IPR_TMR7_CMIA7=167,IPR_TMR7_CMIB7=167,IPR_TMR7_OVI7=167,
IPR_ELC_ELSR18I=175,IPR_ELC_ELSR19I=176,
IPR_TSIP_RD=177,IPR_TSIP_WR=177,IPR_TSIP_ERR=179,
IPR_CMPC0_CMPC0=180,
IPR_CMPC1_CMPC1=181,
IPR_CMPC2_CMPC2=182,
IPR_CMPC3_CMPC3=183,
IPR_CMPC4_CMPC4=184,
IPR_CMPC5_CMPC5=185,
IPR_CMTW0_CMWI0=186,IPR_CMTW0_IC0I0=187,IPR_CMTW0_IC1I0=188,IPR_CMTW0_OC0I0=189,IPR_CMTW0_OC1I0=190,
IPR_CMTW1_CMWI1=191,IPR_CMTW1_IC0I1=192,IPR_CMTW1_IC1I1=193,IPR_CMTW1_OC0I1=194,IPR_CMTW1_OC1I1=195,
IPR_RSCI9_AED=197,
IPR_PERIA_INTA208=208,IPR_PERIA_INTA209=209,IPR_PERIA_INTA210=210,IPR_PERIA_INTA211=211,IPR_PERIA_INTA212=212,IPR_PERIA_INTA213=213,
IPR_PERIA_INTA214=214,IPR_PERIA_INTA215=215,IPR_PERIA_INTA216=216,IPR_PERIA_INTA217=217,IPR_PERIA_INTA218=218,IPR_PERIA_INTA219=219,
IPR_PERIA_INTA220=220,IPR_PERIA_INTA221=221,IPR_PERIA_INTA222=222,IPR_PERIA_INTA223=223,IPR_PERIA_INTA224=224,IPR_PERIA_INTA225=225,
IPR_PERIA_INTA226=226,IPR_PERIA_INTA227=227,IPR_PERIA_INTA228=228,IPR_PERIA_INTA229=229,IPR_PERIA_INTA230=230,IPR_PERIA_INTA231=231,
IPR_PERIA_INTA232=232,IPR_PERIA_INTA233=233,IPR_PERIA_INTA234=234,IPR_PERIA_INTA235=235,IPR_PERIA_INTA236=236,IPR_PERIA_INTA237=237,
IPR_PERIA_INTA238=238,IPR_PERIA_INTA239=239,IPR_PERIA_INTA240=240,IPR_PERIA_INTA241=241,IPR_PERIA_INTA242=242,IPR_PERIA_INTA243=243,
IPR_PERIA_INTA244=244,IPR_PERIA_INTA245=245,IPR_PERIA_INTA246=246,IPR_PERIA_INTA247=247,IPR_PERIA_INTA248=248,IPR_PERIA_INTA249=249,
IPR_PERIA_INTA250=250,IPR_PERIA_INTA251=251,IPR_PERIA_INTA252=252,IPR_PERIA_INTA253=253,IPR_PERIA_INTA254=254,IPR_PERIA_INTA255=255,
IPR_ICU_SWI=3,
IPR_CMT0_=4,
IPR_CMT1_=5,
IPR_CMT2_=6,
IPR_CMT3_=7,
IPR_LVD1_=88,
IPR_LVD2_=89,
IPR_IWDT_=95,
IPR_WDT_=96,
IPR_OST_=125,
IPR_CANFD0_=142,
IPR_TMR0_=146,
IPR_TMR1_=149,
IPR_TMR2_=152,
IPR_TMR3_=155,
IPR_TMR4_=158,
IPR_TMR5_=161,
IPR_TMR6_=164,
IPR_TMR7_=167,
IPR_CMPC0_=180,
IPR_CMPC1_=181,
IPR_CMPC2_=182,
IPR_CMPC3_=183,
IPR_CMPC4_=184,
IPR_CMPC5_=185
} enum_ipr_t;


#pragma pack(4)


typedef struct st_bsc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char STSCLR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char STSCLR : 1;
#endif
	} BIT;
	} BERCLR;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IGAEN : 1;
			unsigned char TOEN : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TOEN : 1;
			unsigned char IGAEN : 1;
#endif
	} BIT;
	} BEREN;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IA : 1;
			unsigned char TO : 1;
			unsigned char  : 2;
			unsigned char MST : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char MST : 3;
			unsigned char  : 2;
			unsigned char TO : 1;
			unsigned char IA : 1;
#endif
	} BIT;
	} BERSR1;
	char           wk2[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 3;
			unsigned short ADDR : 13;
#else
			unsigned short ADDR : 13;
			unsigned short  : 3;
#endif
	} BIT;
	} BERSR2;
	char           wk3[4];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short BPRA : 2;
			unsigned short BPRO : 2;
			unsigned short BPIB : 2;
			unsigned short BPGB : 2;
			unsigned short BPHB : 2;
			unsigned short BPFB : 2;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short BPFB : 2;
			unsigned short BPHB : 2;
			unsigned short BPGB : 2;
			unsigned short BPIB : 2;
			unsigned short BPRO : 2;
			unsigned short BPRA : 2;
#endif
	} BIT;
	} BUSPRI;
} st_bsc_t;

typedef struct st_cac {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CFME : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CFME : 1;
#endif
	} BIT;
	} CACR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CACREFE : 1;
			unsigned char FMCS : 3;
			unsigned char TCSS : 2;
			unsigned char EDGES : 2;
#else
			unsigned char EDGES : 2;
			unsigned char TCSS : 2;
			unsigned char FMCS : 3;
			unsigned char CACREFE : 1;
#endif
	} BIT;
	} CACR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RPS : 1;
			unsigned char RSCS : 3;
			unsigned char RCDS : 2;
			unsigned char DFS : 2;
#else
			unsigned char DFS : 2;
			unsigned char RCDS : 2;
			unsigned char RSCS : 3;
			unsigned char RPS : 1;
#endif
	} BIT;
	} CACR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FERRIE : 1;
			unsigned char MENDIE : 1;
			unsigned char OVFIE : 1;
			unsigned char  : 1;
			unsigned char FERRFCL : 1;
			unsigned char MENDFCL : 1;
			unsigned char OVFFCL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char OVFFCL : 1;
			unsigned char MENDFCL : 1;
			unsigned char FERRFCL : 1;
			unsigned char  : 1;
			unsigned char OVFIE : 1;
			unsigned char MENDIE : 1;
			unsigned char FERRIE : 1;
#endif
	} BIT;
	} CAICR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FERRF : 1;
			unsigned char MENDF : 1;
			unsigned char OVFF : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char OVFF : 1;
			unsigned char MENDF : 1;
			unsigned char FERRF : 1;
#endif
	} BIT;
	} CASTR;
	char           wk0[1];
	unsigned short CAULVR;
	unsigned short CALLVR;
	unsigned short CACNTBR;
} st_cac_t;

typedef struct st_canfd {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TPRI : 1;
			unsigned long DCE : 1;
			unsigned long DRE : 1;
			unsigned long MME : 1;
			unsigned long DLLCS : 1;
			unsigned long OMRC : 1;
			unsigned long  : 2;
			unsigned long TSP : 4;
			unsigned long TSCS : 1;
			unsigned long  : 3;
			unsigned long ITP : 16;
#else
			unsigned long ITP : 16;
			unsigned long  : 3;
			unsigned long TSCS : 1;
			unsigned long TSP : 4;
			unsigned long  : 2;
			unsigned long OMRC : 1;
			unsigned long DLLCS : 1;
			unsigned long MME : 1;
			unsigned long DRE : 1;
			unsigned long DCE : 1;
			unsigned long TPRI : 1;
#endif
	} BIT;
	} GCFG;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MDC : 2;
			unsigned long SLPRQ : 1;
			unsigned long  : 5;
			unsigned long DEIE : 1;
			unsigned long MLIE : 1;
			unsigned long THLIE : 1;
			unsigned long POIE : 1;
			unsigned long  : 4;
			unsigned long TSCR : 1;
			unsigned long  : 15;
#else
			unsigned long  : 15;
			unsigned long TSCR : 1;
			unsigned long  : 4;
			unsigned long POIE : 1;
			unsigned long THLIE : 1;
			unsigned long MLIE : 1;
			unsigned long DEIE : 1;
			unsigned long  : 5;
			unsigned long SLPRQ : 1;
			unsigned long MDC : 2;
#endif
	} BIT;
	} GCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RSTST : 1;
			unsigned long HLTST : 1;
			unsigned long SLPST : 1;
			unsigned long RAMST : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long RAMST : 1;
			unsigned long SLPST : 1;
			unsigned long HLTST : 1;
			unsigned long RSTST : 1;
#endif
	} BIT;
	} GSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DEDF : 1;
			unsigned long MLDF : 1;
			unsigned long THLDF : 1;
			unsigned long PODF : 1;
			unsigned long  : 12;
			unsigned long EEDF0 : 1;
			unsigned long  : 15;
#else
			unsigned long  : 15;
			unsigned long EEDF0 : 1;
			unsigned long  : 12;
			unsigned long PODF : 1;
			unsigned long THLDF : 1;
			unsigned long MLDF : 1;
			unsigned long DEDF : 1;
#endif
	} BIT;
	} GESR;
	unsigned long  TSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PAGE : 1;
			unsigned long  : 7;
			unsigned long AFLWE : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long AFLWE : 1;
			unsigned long  : 7;
			unsigned long PAGE : 1;
#endif
	} BIT;
	} AFCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 16;
			unsigned long RN0 : 6;
			unsigned long  : 10;
#else
			unsigned long  : 10;
			unsigned long RN0 : 6;
			unsigned long  : 16;
#endif
	} BIT;
	} AFCFG;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long NMB : 6;
			unsigned long  : 2;
			unsigned long PLS : 3;
			unsigned long  : 21;
#else
			unsigned long  : 21;
			unsigned long PLS : 3;
			unsigned long  : 2;
			unsigned long NMB : 6;
#endif
	} BIT;
	} RMCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long NDF : 32;
#else
			unsigned long NDF : 32;
#endif
	} BIT;
	} RMNDR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RMIE0 : 1;
			unsigned long RMIE1 : 1;
			unsigned long RMIE2 : 1;
			unsigned long RMIE3 : 1;
			unsigned long RMIE4 : 1;
			unsigned long RMIE5 : 1;
			unsigned long RMIE6 : 1;
			unsigned long RMIE7 : 1;
			unsigned long RMIE8 : 1;
			unsigned long RMIE9 : 1;
			unsigned long RMIE10 : 1;
			unsigned long RMIE11 : 1;
			unsigned long RMIE12 : 1;
			unsigned long RMIE13 : 1;
			unsigned long RMIE14 : 1;
			unsigned long RMIE15 : 1;
			unsigned long RMIE16 : 1;
			unsigned long RMIE17 : 1;
			unsigned long RMIE18 : 1;
			unsigned long RMIE19 : 1;
			unsigned long RMIE20 : 1;
			unsigned long RMIE21 : 1;
			unsigned long RMIE22 : 1;
			unsigned long RMIE23 : 1;
			unsigned long RMIE24 : 1;
			unsigned long RMIE25 : 1;
			unsigned long RMIE26 : 1;
			unsigned long RMIE27 : 1;
			unsigned long RMIE28 : 1;
			unsigned long RMIE29 : 1;
			unsigned long RMIE30 : 1;
			unsigned long RMIE31 : 1;
#else
			unsigned long RMIE31 : 1;
			unsigned long RMIE30 : 1;
			unsigned long RMIE29 : 1;
			unsigned long RMIE28 : 1;
			unsigned long RMIE27 : 1;
			unsigned long RMIE26 : 1;
			unsigned long RMIE25 : 1;
			unsigned long RMIE24 : 1;
			unsigned long RMIE23 : 1;
			unsigned long RMIE22 : 1;
			unsigned long RMIE21 : 1;
			unsigned long RMIE20 : 1;
			unsigned long RMIE19 : 1;
			unsigned long RMIE18 : 1;
			unsigned long RMIE17 : 1;
			unsigned long RMIE16 : 1;
			unsigned long RMIE15 : 1;
			unsigned long RMIE14 : 1;
			unsigned long RMIE13 : 1;
			unsigned long RMIE12 : 1;
			unsigned long RMIE11 : 1;
			unsigned long RMIE10 : 1;
			unsigned long RMIE9 : 1;
			unsigned long RMIE8 : 1;
			unsigned long RMIE7 : 1;
			unsigned long RMIE6 : 1;
			unsigned long RMIE5 : 1;
			unsigned long RMIE4 : 1;
			unsigned long RMIE3 : 1;
			unsigned long RMIE2 : 1;
			unsigned long RMIE1 : 1;
			unsigned long RMIE0 : 1;
#endif
	} BIT;
	} RMIER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFE : 1;
			unsigned long RFIE : 1;
			unsigned long  : 2;
			unsigned long PLS : 3;
			unsigned long  : 1;
			unsigned long FDS : 3;
			unsigned long  : 1;
			unsigned long RFIM : 1;
			unsigned long RFITH : 3;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long RFITH : 3;
			unsigned long RFIM : 1;
			unsigned long  : 1;
			unsigned long FDS : 3;
			unsigned long  : 1;
			unsigned long PLS : 3;
			unsigned long  : 2;
			unsigned long RFIE : 1;
			unsigned long RFE : 1;
#endif
	} BIT;
	} RFCR[2];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EMPTY : 1;
			unsigned long FULL : 1;
			unsigned long LOST : 1;
			unsigned long RFIF : 1;
			unsigned long  : 4;
			unsigned long FLVL : 6;
			unsigned long  : 18;
#else
			unsigned long  : 18;
			unsigned long FLVL : 6;
			unsigned long  : 4;
			unsigned long RFIF : 1;
			unsigned long LOST : 1;
			unsigned long FULL : 1;
			unsigned long EMPTY : 1;
#endif
	} BIT;
	} RFSR[2];
	unsigned long  RFPCR[2];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CFE : 1;
			unsigned long CFRIE : 1;
			unsigned long CFTIE : 1;
			unsigned long  : 1;
			unsigned long PLS : 3;
			unsigned long  : 1;
			unsigned long MODE : 1;
			unsigned long  : 1;
			unsigned long ITCS : 1;
			unsigned long ITR : 1;
			unsigned long CFIM : 1;
			unsigned long CFITH : 3;
			unsigned long LTM : 2;
			unsigned long  : 3;
			unsigned long FDS : 3;
			unsigned long TINT : 8;
#else
			unsigned long TINT : 8;
			unsigned long FDS : 3;
			unsigned long  : 3;
			unsigned long LTM : 2;
			unsigned long CFITH : 3;
			unsigned long CFIM : 1;
			unsigned long ITR : 1;
			unsigned long ITCS : 1;
			unsigned long  : 1;
			unsigned long MODE : 1;
			unsigned long  : 1;
			unsigned long PLS : 3;
			unsigned long  : 1;
			unsigned long CFTIE : 1;
			unsigned long CFRIE : 1;
			unsigned long CFE : 1;
#endif
	} BIT;
	} CFCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EMPTY : 1;
			unsigned long FULL : 1;
			unsigned long LOST : 1;
			unsigned long CFRIF : 1;
			unsigned long CFTIF : 1;
			unsigned long  : 3;
			unsigned long FLVL : 6;
			unsigned long  : 18;
#else
			unsigned long  : 18;
			unsigned long FLVL : 6;
			unsigned long  : 3;
			unsigned long CFTIF : 1;
			unsigned long CFRIF : 1;
			unsigned long LOST : 1;
			unsigned long FULL : 1;
			unsigned long EMPTY : 1;
#endif
	} BIT;
	} CFSR0;
	unsigned long  CFPCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFEMP0 : 1;
			unsigned long RFEMP1 : 1;
			unsigned long  : 6;
			unsigned long CFEMP0 : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long CFEMP0 : 1;
			unsigned long  : 6;
			unsigned long RFEMP1 : 1;
			unsigned long RFEMP0 : 1;
#endif
	} BIT;
	} FESR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFFUL0 : 1;
			unsigned long RFFUL1 : 1;
			unsigned long  : 6;
			unsigned long CFFUL0 : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long CFFUL0 : 1;
			unsigned long  : 6;
			unsigned long RFFUL1 : 1;
			unsigned long RFFUL0 : 1;
#endif
	} BIT;
	} FFSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFML0 : 1;
			unsigned long RFML1 : 1;
			unsigned long  : 6;
			unsigned long CFML0 : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long CFML0 : 1;
			unsigned long  : 6;
			unsigned long RFML1 : 1;
			unsigned long RFML0 : 1;
#endif
	} BIT;
	} FMLSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFIF0 : 1;
			unsigned long RFIF1 : 1;
			unsigned long  : 30;
#else
			unsigned long  : 30;
			unsigned long RFIF1 : 1;
			unsigned long RFIF0 : 1;
#endif
	} BIT;
	} RFISR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TXRQ : 1;
			unsigned char TARQ : 1;
			unsigned char ONESHOT : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char ONESHOT : 1;
			unsigned char TARQ : 1;
			unsigned char TXRQ : 1;
#endif
	} BIT;
	} TMCR[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TXSF : 1;
			unsigned char TXRF : 2;
			unsigned char TXRQS : 1;
			unsigned char TARQS : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char TARQS : 1;
			unsigned char TXRQS : 1;
			unsigned char TXRF : 2;
			unsigned char TXSF : 1;
#endif
	} BIT;
	} TMSR[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TXRQS0 : 1;
			unsigned long TXRQS1 : 1;
			unsigned long TXRQS2 : 1;
			unsigned long TXRQS3 : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long TXRQS3 : 1;
			unsigned long TXRQS2 : 1;
			unsigned long TXRQS1 : 1;
			unsigned long TXRQS0 : 1;
#endif
	} BIT;
	} TMTRSR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TARQS0 : 1;
			unsigned long TARQS1 : 1;
			unsigned long TARQS2 : 1;
			unsigned long TARQS3 : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long TARQS3 : 1;
			unsigned long TARQS2 : 1;
			unsigned long TARQS1 : 1;
			unsigned long TARQS0 : 1;
#endif
	} BIT;
	} TMARSR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TXCF0 : 1;
			unsigned long TXCF1 : 1;
			unsigned long TXCF2 : 1;
			unsigned long TXCF3 : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long TXCF3 : 1;
			unsigned long TXCF2 : 1;
			unsigned long TXCF1 : 1;
			unsigned long TXCF0 : 1;
#endif
	} BIT;
	} TMTCSR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TAF0 : 1;
			unsigned long TAF1 : 1;
			unsigned long TAF2 : 1;
			unsigned long TAF3 : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long TAF3 : 1;
			unsigned long TAF2 : 1;
			unsigned long TAF1 : 1;
			unsigned long TAF0 : 1;
#endif
	} BIT;
	} TMTASR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TMIE0 : 1;
			unsigned long TMIE1 : 1;
			unsigned long TMIE2 : 1;
			unsigned long TMIE3 : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long TMIE3 : 1;
			unsigned long TMIE2 : 1;
			unsigned long TMIE1 : 1;
			unsigned long TMIE0 : 1;
#endif
	} BIT;
	} TMIER0;
	char           wk0[24];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TSIF0 : 1;
			unsigned long TAIF0 : 1;
			unsigned long TQIF0 : 1;
			unsigned long CFTIF0 : 1;
			unsigned long THIF0 : 1;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long THIF0 : 1;
			unsigned long CFTIF0 : 1;
			unsigned long TQIF0 : 1;
			unsigned long TAIF0 : 1;
			unsigned long TSIF0 : 1;
#endif
	} BIT;
	} TISR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 16;
			unsigned long RTPS : 4;
			unsigned long  : 12;
#else
			unsigned long  : 12;
			unsigned long RTPS : 4;
			unsigned long  : 16;
#endif
	} BIT;
	} GTMCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 2;
			unsigned long RTME : 1;
			unsigned long  : 29;
#else
			unsigned long  : 29;
			unsigned long RTME : 1;
			unsigned long  : 2;
#endif
	} BIT;
	} GTMER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PXEDIS : 1;
			unsigned long  : 7;
			unsigned long TSCPS : 2;
			unsigned long  : 22;
#else
			unsigned long  : 22;
			unsigned long TSCPS : 2;
			unsigned long  : 7;
			unsigned long PXEDIS : 1;
#endif
	} BIT;
	} GFDCFG;
	char           wk1[4];
	unsigned long  GTMLKR;
	char           wk2[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IGES : 5;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long IGES : 5;
#endif
	} BIT;
	} AFIGSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IGEE : 1;
			unsigned long  : 7;
			unsigned long KEY : 8;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long KEY : 8;
			unsigned long  : 7;
			unsigned long IGEE : 1;
#endif
	} BIT;
	} AFIGER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFDTE0 : 1;
			unsigned long RFDTE1 : 1;
			unsigned long  : 6;
			unsigned long CFDTE0 : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long CFDTE0 : 1;
			unsigned long  : 6;
			unsigned long RFDTE1 : 1;
			unsigned long RFDTE0 : 1;
#endif
	} BIT;
	} DTCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RFDTS0 : 1;
			unsigned long RFDTS1 : 1;
			unsigned long  : 6;
			unsigned long CFDTS0 : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long CFDTS0 : 1;
			unsigned long  : 6;
			unsigned long RFDTS1 : 1;
			unsigned long RFDTS0 : 1;
#endif
	} BIT;
	} DTSR;
	char           wk3[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SRST : 1;
			unsigned long  : 7;
			unsigned long KEY : 8;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long KEY : 8;
			unsigned long  : 7;
			unsigned long SRST : 1;
#endif
	} BIT;
	} GRCR;
	char           wk4[68];
	struct {
		union {
			unsigned long LONG;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long LPC : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long LPC : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} IDR;
		union {
			unsigned long LONG;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IDM : 29;
			unsigned long IFL1 : 1;
			unsigned long RTRM : 1;
			unsigned long IDEM : 1;
#else
			unsigned long IDEM : 1;
			unsigned long RTRM : 1;
			unsigned long IFL1 : 1;
			unsigned long IDM : 29;
#endif
	} BIT;
		} MASK;
		union {
			unsigned long LONG;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DLC : 4;
			unsigned long  : 3;
			unsigned long IFL0 : 1;
			unsigned long DMB : 5;
			unsigned long  : 2;
			unsigned long DMBE : 1;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long DMBE : 1;
			unsigned long  : 2;
			unsigned long DMB : 5;
			unsigned long IFL0 : 1;
			unsigned long  : 3;
			unsigned long DLC : 4;
#endif
	} BIT;
		} PTR0;
		union {
			unsigned long LONG;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RF0E : 1;
			unsigned long RF1E : 1;
			unsigned long  : 6;
			unsigned long CF0E : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long CF0E : 1;
			unsigned long  : 6;
			unsigned long RF1E : 1;
			unsigned long RF0E : 1;
#endif
	} BIT;
		} PTR1;
	} AFL[16];
	char           wk5[96];
	unsigned long  RTPAR[64];
	char           wk6[416];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RFB[2];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long THENT : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long THENT : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} CFB[1];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long THENT : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long THENT : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 28;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 28;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} TMB[4];
	char           wk7[492];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB0;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB1;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB2;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB3;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB4;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB5;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB6;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB7;
	char           wk8[416];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB8;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB9;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB10;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB11;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB12;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB13;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB14;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB15;
	char           wk9[416];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB16;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB17;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB18;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB19;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB20;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB21;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB22;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB23;
	char           wk10[416];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB24;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB25;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB26;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB27;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB28;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB29;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB30;
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ID : 29;
			unsigned long  : 1;
			unsigned long RTR : 1;
			unsigned long IDE : 1;
#else
			unsigned long IDE : 1;
			unsigned long RTR : 1;
			unsigned long  : 1;
			unsigned long ID : 29;
#endif
	} BIT;
		} HF0;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TS : 16;
			unsigned long  : 12;
			unsigned long DLC : 4;
#else
			unsigned long DLC : 4;
			unsigned long  : 12;
			unsigned long TS : 16;
#endif
	} BIT;
		} HF1;
		union {
			unsigned long LONG;
			struct {
				unsigned short L;
				unsigned short H;
			} WORD;
			struct {
				unsigned char LL;
				unsigned char LH;
				unsigned char HL;
				unsigned char HH;
			} BYTE;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ESI : 1;
			unsigned long BRS : 1;
			unsigned long FDF : 1;
			unsigned long  : 5;
			unsigned long IFL : 2;
			unsigned long  : 6;
			unsigned long PTR : 16;
#else
			unsigned long PTR : 16;
			unsigned long  : 6;
			unsigned long IFL : 2;
			unsigned long  : 5;
			unsigned long FDF : 1;
			unsigned long BRS : 1;
			unsigned long ESI : 1;
#endif
	} BIT;
		} HF2;
		union {
			unsigned long DF[16];
			unsigned char DATA[64];
		};
	} RMB31;
	char           wk11[276608];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ECEF : 1;
			unsigned long EC1EF : 1;
			unsigned long EC2EF : 1;
			unsigned long EC1EIE : 1;
			unsigned long EC2EIE : 1;
			unsigned long EC1ECD : 1;
			unsigned long ECEDE : 1;
			unsigned long  : 2;
			unsigned long EC1EC : 1;
			unsigned long EC2EC : 1;
			unsigned long ECOVF : 1;
			unsigned long  : 2;
			unsigned long ECEDWC : 2;
			unsigned long EC1EAS : 1;
			unsigned long EC2EAS : 1;
			unsigned long  : 14;
#else
			unsigned long  : 14;
			unsigned long EC2EAS : 1;
			unsigned long EC1EAS : 1;
			unsigned long ECEDWC : 2;
			unsigned long  : 2;
			unsigned long ECOVF : 1;
			unsigned long EC2EC : 1;
			unsigned long EC1EC : 1;
			unsigned long  : 2;
			unsigned long ECEDE : 1;
			unsigned long EC1ECD : 1;
			unsigned long EC2EIE : 1;
			unsigned long EC1EIE : 1;
			unsigned long EC2EF : 1;
			unsigned long EC1EF : 1;
			unsigned long ECEF : 1;
#endif
	} BIT;
	} ECCSR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 1;
			unsigned short ECDIS : 1;
			unsigned short  : 5;
			unsigned short ECTME : 1;
			unsigned short  : 6;
			unsigned short ECTMWC : 2;
#else
			unsigned short ECTMWC : 2;
			unsigned short  : 6;
			unsigned short ECTME : 1;
			unsigned short  : 5;
			unsigned short ECDIS : 1;
			unsigned short  : 1;
#endif
	} BIT;
	} ECTMR;
	char           wk12[6];
	unsigned long  ECTDR;
	unsigned long  ECEAR;
} st_canfd_t;

typedef struct st_canfd0 {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BRP : 10;
			unsigned long SJW : 7;
			unsigned long TSEG1 : 8;
			unsigned long TSEG2 : 7;
#else
			unsigned long TSEG2 : 7;
			unsigned long TSEG1 : 8;
			unsigned long SJW : 7;
			unsigned long BRP : 10;
#endif
	} BIT;
	} NBCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MDC : 2;
			unsigned long SLPRQ : 1;
			unsigned long RTBO : 1;
			unsigned long  : 4;
			unsigned long BEIE : 1;
			unsigned long EWIE : 1;
			unsigned long EPIE : 1;
			unsigned long BOEIE : 1;
			unsigned long BORIE : 1;
			unsigned long OLIE : 1;
			unsigned long BLIE : 1;
			unsigned long ALIE : 1;
			unsigned long TAIE : 1;
			unsigned long ECOVIE : 1;
			unsigned long SCOVIE : 1;
			unsigned long TDCVIE : 1;
			unsigned long  : 1;
			unsigned long BOM : 2;
			unsigned long EDM : 1;
			unsigned long CTME : 1;
			unsigned long CTMS : 2;
			unsigned long  : 3;
			unsigned long BFT : 1;
			unsigned long ROME : 1;
#else
			unsigned long ROME : 1;
			unsigned long BFT : 1;
			unsigned long  : 3;
			unsigned long CTMS : 2;
			unsigned long CTME : 1;
			unsigned long EDM : 1;
			unsigned long BOM : 2;
			unsigned long  : 1;
			unsigned long TDCVIE : 1;
			unsigned long SCOVIE : 1;
			unsigned long ECOVIE : 1;
			unsigned long TAIE : 1;
			unsigned long ALIE : 1;
			unsigned long BLIE : 1;
			unsigned long OLIE : 1;
			unsigned long BORIE : 1;
			unsigned long BOEIE : 1;
			unsigned long EPIE : 1;
			unsigned long EWIE : 1;
			unsigned long BEIE : 1;
			unsigned long  : 4;
			unsigned long RTBO : 1;
			unsigned long SLPRQ : 1;
			unsigned long MDC : 2;
#endif
	} BIT;
	} CHCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RSTST : 1;
			unsigned long HLTST : 1;
			unsigned long SLPST : 1;
			unsigned long EPST : 1;
			unsigned long BOST : 1;
			unsigned long TRMST : 1;
			unsigned long RECST : 1;
			unsigned long CRDY : 1;
			unsigned long RESI : 1;
			unsigned long  : 7;
			unsigned long REC : 8;
			unsigned long TEC : 8;
#else
			unsigned long TEC : 8;
			unsigned long REC : 8;
			unsigned long  : 7;
			unsigned long RESI : 1;
			unsigned long CRDY : 1;
			unsigned long RECST : 1;
			unsigned long TRMST : 1;
			unsigned long BOST : 1;
			unsigned long EPST : 1;
			unsigned long SLPST : 1;
			unsigned long HLTST : 1;
			unsigned long RSTST : 1;
#endif
	} BIT;
	} CHSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BEDF : 1;
			unsigned long EWDF : 1;
			unsigned long EPDF : 1;
			unsigned long BOEDF : 1;
			unsigned long BORDF : 1;
			unsigned long OLDF : 1;
			unsigned long BLDF : 1;
			unsigned long ALDF : 1;
			unsigned long SEDF : 1;
			unsigned long FEDF : 1;
			unsigned long AEDF : 1;
			unsigned long CEDF : 1;
			unsigned long B1EDF : 1;
			unsigned long B0EDF : 1;
			unsigned long ADEDF : 1;
			unsigned long  : 1;
			unsigned long CRC15 : 15;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long CRC15 : 15;
			unsigned long  : 1;
			unsigned long ADEDF : 1;
			unsigned long B0EDF : 1;
			unsigned long B1EDF : 1;
			unsigned long CEDF : 1;
			unsigned long AEDF : 1;
			unsigned long FEDF : 1;
			unsigned long SEDF : 1;
			unsigned long ALDF : 1;
			unsigned long BLDF : 1;
			unsigned long OLDF : 1;
			unsigned long BORDF : 1;
			unsigned long BOEDF : 1;
			unsigned long EPDF : 1;
			unsigned long EWDF : 1;
			unsigned long BEDF : 1;
#endif
	} BIT;
	} CHESR;
	char           wk0[124];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TQE : 1;
			unsigned long  : 4;
			unsigned long TQIE : 1;
			unsigned long  : 1;
			unsigned long TQIM : 1;
			unsigned long QDS : 2;
			unsigned long  : 22;
#else
			unsigned long  : 22;
			unsigned long QDS : 2;
			unsigned long TQIM : 1;
			unsigned long  : 1;
			unsigned long TQIE : 1;
			unsigned long  : 4;
			unsigned long TQE : 1;
#endif
	} BIT;
	} TQCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EMPTY : 1;
			unsigned long FULL : 1;
			unsigned long TQIF : 1;
			unsigned long  : 5;
			unsigned long FLVL : 3;
			unsigned long  : 21;
#else
			unsigned long  : 21;
			unsigned long FLVL : 3;
			unsigned long  : 5;
			unsigned long TQIF : 1;
			unsigned long FULL : 1;
			unsigned long EMPTY : 1;
#endif
	} BIT;
	} TQSR0;
	unsigned long  TQPCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long THE : 1;
			unsigned long  : 7;
			unsigned long THIE : 1;
			unsigned long THIM : 1;
			unsigned long THRC : 1;
			unsigned long  : 21;
#else
			unsigned long  : 21;
			unsigned long THRC : 1;
			unsigned long THIM : 1;
			unsigned long THIE : 1;
			unsigned long  : 7;
			unsigned long THE : 1;
#endif
	} BIT;
	} THCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EMPTY : 1;
			unsigned long FULL : 1;
			unsigned long LOST : 1;
			unsigned long THIF : 1;
			unsigned long  : 4;
			unsigned long FLVL : 4;
			unsigned long  : 20;
#else
			unsigned long  : 20;
			unsigned long FLVL : 4;
			unsigned long  : 4;
			unsigned long THIF : 1;
			unsigned long LOST : 1;
			unsigned long FULL : 1;
			unsigned long EMPTY : 1;
#endif
	} BIT;
	} THSR;
	unsigned long  THPCR;
	char           wk1[92];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BRP : 8;
			unsigned long TSEG1 : 5;
			unsigned long  : 3;
			unsigned long TSEG2 : 4;
			unsigned long  : 4;
			unsigned long SJW : 4;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long SJW : 4;
			unsigned long  : 4;
			unsigned long TSEG2 : 4;
			unsigned long  : 3;
			unsigned long TSEG1 : 5;
			unsigned long BRP : 8;
#endif
	} BIT;
	} DBCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ECC : 3;
			unsigned long  : 5;
			unsigned long SSPC : 1;
			unsigned long TDCE : 1;
			unsigned long TESI : 1;
			unsigned long  : 5;
			unsigned long TDCO : 8;
			unsigned long  : 4;
			unsigned long FDOE : 1;
			unsigned long REFE : 1;
			unsigned long CLOE : 1;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long CLOE : 1;
			unsigned long REFE : 1;
			unsigned long FDOE : 1;
			unsigned long  : 4;
			unsigned long TDCO : 8;
			unsigned long  : 5;
			unsigned long TESI : 1;
			unsigned long TDCE : 1;
			unsigned long SSPC : 1;
			unsigned long  : 5;
			unsigned long ECC : 3;
#endif
	} BIT;
	} FDCFG;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ECCL : 1;
			unsigned long SCCL : 1;
			unsigned long  : 30;
#else
			unsigned long  : 30;
			unsigned long SCCL : 1;
			unsigned long ECCL : 1;
#endif
	} BIT;
	} FDCTR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDCR : 8;
			unsigned long ECOV : 1;
			unsigned long SCOV : 1;
			unsigned long  : 5;
			unsigned long TDCV : 1;
			unsigned long EC : 8;
			unsigned long SC : 8;
#else
			unsigned long SC : 8;
			unsigned long EC : 8;
			unsigned long TDCV : 1;
			unsigned long  : 5;
			unsigned long SCOV : 1;
			unsigned long ECOV : 1;
			unsigned long TDCR : 8;
#endif
	} BIT;
	} FDSTS;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CRC21 : 21;
			unsigned long  : 3;
			unsigned long SBC : 4;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long SBC : 4;
			unsigned long  : 3;
			unsigned long CRC21 : 21;
#endif
	} BIT;
	} FDCRC;
	char           wk2[1580];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BT : 3;
			unsigned long BN : 2;
			unsigned long  : 11;
			unsigned long TS : 16;
#else
			unsigned long TS : 16;
			unsigned long  : 11;
			unsigned long BN : 2;
			unsigned long BT : 3;
#endif
	} BIT;
	} THACR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PTR : 16;
			unsigned long IFL : 2;
			unsigned long  : 14;
#else
			unsigned long  : 14;
			unsigned long IFL : 2;
			unsigned long PTR : 16;
#endif
	} BIT;
	} THACR1;
} st_canfd0_t;

typedef struct st_cmpc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CINV : 1;
			unsigned char COE : 1;
			unsigned char  : 1;
			unsigned char CEG : 2;
			unsigned char CDFS : 2;
			unsigned char HCMPON : 1;
#else
			unsigned char HCMPON : 1;
			unsigned char CDFS : 2;
			unsigned char CEG : 2;
			unsigned char  : 1;
			unsigned char COE : 1;
			unsigned char CINV : 1;
#endif
	} BIT;
	} CMPCTL;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPSEL : 4;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char CMPSEL : 4;
#endif
	} BIT;
	} CMPSEL0;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CVRS : 4;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char CVRS : 4;
#endif
	} BIT;
	} CMPSEL1;
	char           wk2[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPMON0 : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CMPMON0 : 1;
#endif
	} BIT;
	} CMPMON;
	char           wk3[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CPOE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CPOE : 1;
#endif
	} BIT;
	} CMPIOC;
	char           wk4[7];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CNFS : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CNFS : 1;
#endif
	} BIT;
	} CMPCTL2;
} st_cmpc_t;

typedef struct st_cmt {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short STR0 : 1;
			unsigned short STR1 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short STR1 : 1;
			unsigned short STR0 : 1;
#endif
	} BIT;
	} CMSTR0;
	char           wk0[14];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short STR2 : 1;
			unsigned short STR3 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short STR3 : 1;
			unsigned short STR2 : 1;
#endif
	} BIT;
	} CMSTR1;
} st_cmt_t;

typedef struct st_cmt0 {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CKS : 2;
			unsigned short  : 4;
			unsigned short CMIE : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short CMIE : 1;
			unsigned short  : 4;
			unsigned short CKS : 2;
#endif
	} BIT;
	} CMCR;
	unsigned short CMCNT;
	unsigned short CMCOR;
} st_cmt0_t;

typedef struct st_cmtw {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short STR : 1;
			unsigned short  : 15;
#else
			unsigned short  : 15;
			unsigned short STR : 1;
#endif
	} BIT;
	} CMWSTR;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CKS : 2;
			unsigned short  : 1;
			unsigned short CMWIE : 1;
			unsigned short IC0IE : 1;
			unsigned short IC1IE : 1;
			unsigned short OC0IE : 1;
			unsigned short OC1IE : 1;
			unsigned short  : 1;
			unsigned short CMS : 1;
			unsigned short  : 3;
			unsigned short CCLR : 3;
#else
			unsigned short CCLR : 3;
			unsigned short  : 3;
			unsigned short CMS : 1;
			unsigned short  : 1;
			unsigned short OC1IE : 1;
			unsigned short OC0IE : 1;
			unsigned short IC1IE : 1;
			unsigned short IC0IE : 1;
			unsigned short CMWIE : 1;
			unsigned short  : 1;
			unsigned short CKS : 2;
#endif
	} BIT;
	} CMWCR;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short IC0 : 2;
			unsigned short IC1 : 2;
			unsigned short IC0E : 1;
			unsigned short IC1E : 1;
			unsigned short  : 2;
			unsigned short OC0 : 2;
			unsigned short OC1 : 2;
			unsigned short OC0E : 1;
			unsigned short OC1E : 1;
			unsigned short  : 1;
			unsigned short CMWE : 1;
#else
			unsigned short CMWE : 1;
			unsigned short  : 1;
			unsigned short OC1E : 1;
			unsigned short OC0E : 1;
			unsigned short OC1 : 2;
			unsigned short OC0 : 2;
			unsigned short  : 2;
			unsigned short IC1E : 1;
			unsigned short IC0E : 1;
			unsigned short IC1 : 2;
			unsigned short IC0 : 2;
#endif
	} BIT;
	} CMWIOR;
	char           wk2[6];
	unsigned long  CMWCNT;
	unsigned long  CMWCOR;
	unsigned long  CMWICR0;
	unsigned long  CMWICR1;
	unsigned long  CMWOCR0;
	unsigned long  CMWOCR1;
} st_cmtw_t;

typedef struct st_crc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char GPS : 3;
			unsigned char  : 3;
			unsigned char LMS : 1;
			unsigned char DORCLR : 1;
#else
			unsigned char DORCLR : 1;
			unsigned char LMS : 1;
			unsigned char  : 3;
			unsigned char GPS : 3;
#endif
	} BIT;
	} CRCCR;
	char           wk0[3];
	union {
		unsigned long LONG;
		unsigned char BYTE;
	} CRCDIR;
	union {
		unsigned long LONG;
		unsigned short WORD;
		unsigned char BYTE;
	} CRCDOR;
} st_crc_t;

typedef struct st_da {
	unsigned short DADR0;
	unsigned short DADR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 5;
			unsigned char DAE : 1;
			unsigned char DAOE0 : 1;
			unsigned char DAOE1 : 1;
#else
			unsigned char DAOE1 : 1;
			unsigned char DAOE0 : 1;
			unsigned char DAE : 1;
			unsigned char  : 5;
#endif
	} BIT;
	} DACR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char DPSEL : 1;
#else
			unsigned char DPSEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} DADPR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char DAADST : 1;
#else
			unsigned char DAADST : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} DAADSCR;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OUTDA0 : 1;
			unsigned char OUTDA1 : 1;
			unsigned char OUTREF0 : 1;
			unsigned char OUTREF1 : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char OUTREF1 : 1;
			unsigned char OUTREF0 : 1;
			unsigned char OUTDA1 : 1;
			unsigned char OUTDA0 : 1;
#endif
	} BIT;
	} DADSELR;
} st_da_t;

typedef struct st_dmac {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DMST : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DMST : 1;
#endif
	} BIT;
	} DMAST;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 4;
			unsigned char DMIS4 : 1;
			unsigned char DMIS5 : 1;
			unsigned char DMIS6 : 1;
			unsigned char DMIS7 : 1;
#else
			unsigned char DMIS7 : 1;
			unsigned char DMIS6 : 1;
			unsigned char DMIS5 : 1;
			unsigned char DMIS4 : 1;
			unsigned char  : 4;
#endif
	} BIT;
	} DMIST;
} st_dmac_t;

typedef struct st_dmac0 {
	void          *DMSAR;
	void          *DMDAR;
	unsigned long  DMCRA;
	unsigned short DMCRB;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DCTG : 2;
			unsigned short  : 6;
			unsigned short SZ : 2;
			unsigned short  : 2;
			unsigned short DTS : 2;
			unsigned short MD : 2;
#else
			unsigned short MD : 2;
			unsigned short DTS : 2;
			unsigned short  : 2;
			unsigned short SZ : 2;
			unsigned short  : 6;
			unsigned short DCTG : 2;
#endif
	} BIT;
	} DMTMD;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DARIE : 1;
			unsigned char SARIE : 1;
			unsigned char RPTIE : 1;
			unsigned char ESIE : 1;
			unsigned char DTIE : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char DTIE : 1;
			unsigned char ESIE : 1;
			unsigned char RPTIE : 1;
			unsigned char SARIE : 1;
			unsigned char DARIE : 1;
#endif
	} BIT;
	} DMINT;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DARA : 5;
			unsigned short  : 1;
			unsigned short DM : 2;
			unsigned short SARA : 5;
			unsigned short  : 1;
			unsigned short SM : 2;
#else
			unsigned short SM : 2;
			unsigned short  : 1;
			unsigned short SARA : 5;
			unsigned short DM : 2;
			unsigned short  : 1;
			unsigned short DARA : 5;
#endif
	} BIT;
	} DMAMD;
	char           wk2[2];
	unsigned long  DMOFR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DTE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DTE : 1;
#endif
	} BIT;
	} DMCNT;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SWREQ : 1;
			unsigned char  : 3;
			unsigned char CLRS : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char CLRS : 1;
			unsigned char  : 3;
			unsigned char SWREQ : 1;
#endif
	} BIT;
	} DMREQ;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ESIF : 1;
			unsigned char  : 3;
			unsigned char DTIF : 1;
			unsigned char  : 2;
			unsigned char ACT : 1;
#else
			unsigned char ACT : 1;
			unsigned char  : 2;
			unsigned char DTIF : 1;
			unsigned char  : 3;
			unsigned char ESIF : 1;
#endif
	} BIT;
	} DMSTS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DISEL : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DISEL : 1;
#endif
	} BIT;
	} DMCSL;
} st_dmac0_t;

typedef struct st_dmac1 {
	void          *DMSAR;
	void          *DMDAR;
	unsigned long  DMCRA;
	unsigned short DMCRB;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DCTG : 2;
			unsigned short  : 6;
			unsigned short SZ : 2;
			unsigned short  : 2;
			unsigned short DTS : 2;
			unsigned short MD : 2;
#else
			unsigned short MD : 2;
			unsigned short DTS : 2;
			unsigned short  : 2;
			unsigned short SZ : 2;
			unsigned short  : 6;
			unsigned short DCTG : 2;
#endif
	} BIT;
	} DMTMD;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DARIE : 1;
			unsigned char SARIE : 1;
			unsigned char RPTIE : 1;
			unsigned char ESIE : 1;
			unsigned char DTIE : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char DTIE : 1;
			unsigned char ESIE : 1;
			unsigned char RPTIE : 1;
			unsigned char SARIE : 1;
			unsigned char DARIE : 1;
#endif
	} BIT;
	} DMINT;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DARA : 5;
			unsigned short  : 1;
			unsigned short DM : 2;
			unsigned short SARA : 5;
			unsigned short  : 1;
			unsigned short SM : 2;
#else
			unsigned short SM : 2;
			unsigned short  : 1;
			unsigned short SARA : 5;
			unsigned short DM : 2;
			unsigned short  : 1;
			unsigned short DARA : 5;
#endif
	} BIT;
	} DMAMD;
	char           wk2[6];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DTE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DTE : 1;
#endif
	} BIT;
	} DMCNT;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SWREQ : 1;
			unsigned char  : 3;
			unsigned char CLRS : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char CLRS : 1;
			unsigned char  : 3;
			unsigned char SWREQ : 1;
#endif
	} BIT;
	} DMREQ;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ESIF : 1;
			unsigned char  : 3;
			unsigned char DTIF : 1;
			unsigned char  : 2;
			unsigned char ACT : 1;
#else
			unsigned char ACT : 1;
			unsigned char  : 2;
			unsigned char DTIF : 1;
			unsigned char  : 3;
			unsigned char ESIF : 1;
#endif
	} BIT;
	} DMSTS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DISEL : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DISEL : 1;
#endif
	} BIT;
	} DMCSL;
} st_dmac1_t;

typedef struct st_doc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OMS : 2;
			unsigned char  : 1;
			unsigned char DOPSZ : 1;
			unsigned char DCSEL : 3;
			unsigned char DOPCIE : 1;
#else
			unsigned char DOPCIE : 1;
			unsigned char DCSEL : 3;
			unsigned char DOPSZ : 1;
			unsigned char  : 1;
			unsigned char OMS : 2;
#endif
	} BIT;
	} DOCR;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DOPCF : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DOPCF : 1;
#endif
	} BIT;
	} DOSR;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DOPCFCL : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DOPCFCL : 1;
#endif
	} BIT;
	} DOSCR;
	char           wk2[3];
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
	} DODIR;
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
	} DODSR0;
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
	} DODSR1;
} st_doc_t;

typedef struct st_dtc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 4;
			unsigned char RRS : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char RRS : 1;
			unsigned char  : 4;
#endif
	} BIT;
	} DTCCR;
	char           wk0[3];
	void          *DTCVBR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SHORT : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SHORT : 1;
#endif
	} BIT;
	} DTCADMOD;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DTCST : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DTCST : 1;
#endif
	} BIT;
	} DTCST;
	char           wk2[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short VECN : 8;
			unsigned short  : 7;
			unsigned short ACT : 1;
#else
			unsigned short ACT : 1;
			unsigned short  : 7;
			unsigned short VECN : 8;
#endif
	} BIT;
	} DTCSTS;
	void          *DTCIBR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SQTFRL : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SQTFRL : 1;
#endif
	} BIT;
	} DTCOR;
	char           wk3[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short VECN : 8;
			unsigned short  : 7;
			unsigned short ESPSEL : 1;
#else
			unsigned short ESPSEL : 1;
			unsigned short  : 7;
			unsigned short VECN : 8;
#endif
	} BIT;
	} DTCSQE;
	unsigned long  DTCDISP;
} st_dtc_t;

typedef struct st_elc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ELCON : 1;
#else
			unsigned char ELCON : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} ELCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR0;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR4;
	char           wk1[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR7;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR10;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR11;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR12;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR13;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR15;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR16;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR18;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR19;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR20;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR21;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR22;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR23;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR24;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR25;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR26;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR27;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR28;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MTU0MD : 2;
			unsigned char  : 4;
			unsigned char MTU3MD : 2;
#else
			unsigned char MTU3MD : 2;
			unsigned char  : 4;
			unsigned char MTU0MD : 2;
#endif
	} BIT;
	} ELOPA;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MTU4MD : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char MTU4MD : 2;
#endif
	} BIT;
	} ELOPB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 2;
			unsigned char CMT1MD : 2;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char CMT1MD : 2;
			unsigned char  : 2;
#endif
	} BIT;
	} ELOPC;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TMR0MD : 2;
			unsigned char TMR1MD : 2;
			unsigned char TMR2MD : 2;
			unsigned char TMR3MD : 2;
#else
			unsigned char TMR3MD : 2;
			unsigned char TMR2MD : 2;
			unsigned char TMR1MD : 2;
			unsigned char TMR0MD : 2;
#endif
	} BIT;
	} ELOPD;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PGR0 : 1;
			unsigned char PGR1 : 1;
			unsigned char PGR2 : 1;
			unsigned char PGR3 : 1;
			unsigned char PGR4 : 1;
			unsigned char PGR5 : 1;
			unsigned char PGR6 : 1;
			unsigned char PGR7 : 1;
#else
			unsigned char PGR7 : 1;
			unsigned char PGR6 : 1;
			unsigned char PGR5 : 1;
			unsigned char PGR4 : 1;
			unsigned char PGR3 : 1;
			unsigned char PGR2 : 1;
			unsigned char PGR1 : 1;
			unsigned char PGR0 : 1;
#endif
	} BIT;
	} PGR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PGR0 : 1;
			unsigned char PGR1 : 1;
			unsigned char PGR2 : 1;
			unsigned char PGR3 : 1;
			unsigned char PGR4 : 1;
			unsigned char PGR5 : 1;
			unsigned char PGR6 : 1;
			unsigned char PGR7 : 1;
#else
			unsigned char PGR7 : 1;
			unsigned char PGR6 : 1;
			unsigned char PGR5 : 1;
			unsigned char PGR4 : 1;
			unsigned char PGR3 : 1;
			unsigned char PGR2 : 1;
			unsigned char PGR1 : 1;
			unsigned char PGR0 : 1;
#endif
	} BIT;
	} PGR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PGCI : 2;
			unsigned char PGCOVE : 1;
			unsigned char  : 1;
			unsigned char PGCO : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PGCO : 3;
			unsigned char  : 1;
			unsigned char PGCOVE : 1;
			unsigned char PGCI : 2;
#endif
	} BIT;
	} PGC1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PGCI : 2;
			unsigned char PGCOVE : 1;
			unsigned char  : 1;
			unsigned char PGCO : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PGCO : 3;
			unsigned char  : 1;
			unsigned char PGCOVE : 1;
			unsigned char PGCI : 2;
#endif
	} BIT;
	} PGC2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PDBF0 : 1;
			unsigned char PDBF1 : 1;
			unsigned char PDBF2 : 1;
			unsigned char PDBF3 : 1;
			unsigned char PDBF4 : 1;
			unsigned char PDBF5 : 1;
			unsigned char PDBF6 : 1;
			unsigned char PDBF7 : 1;
#else
			unsigned char PDBF7 : 1;
			unsigned char PDBF6 : 1;
			unsigned char PDBF5 : 1;
			unsigned char PDBF4 : 1;
			unsigned char PDBF3 : 1;
			unsigned char PDBF2 : 1;
			unsigned char PDBF1 : 1;
			unsigned char PDBF0 : 1;
#endif
	} BIT;
	} PDBF1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PDBF0 : 1;
			unsigned char PDBF1 : 1;
			unsigned char PDBF2 : 1;
			unsigned char PDBF3 : 1;
			unsigned char PDBF4 : 1;
			unsigned char PDBF5 : 1;
			unsigned char PDBF6 : 1;
			unsigned char PDBF7 : 1;
#else
			unsigned char PDBF7 : 1;
			unsigned char PDBF6 : 1;
			unsigned char PDBF5 : 1;
			unsigned char PDBF4 : 1;
			unsigned char PDBF3 : 1;
			unsigned char PDBF2 : 1;
			unsigned char PDBF1 : 1;
			unsigned char PDBF0 : 1;
#endif
	} BIT;
	} PDBF2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSB : 3;
			unsigned char PSP : 2;
			unsigned char PSM : 2;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PSM : 2;
			unsigned char PSP : 2;
			unsigned char PSB : 3;
#endif
	} BIT;
	} PEL0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSB : 3;
			unsigned char PSP : 2;
			unsigned char PSM : 2;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PSM : 2;
			unsigned char PSP : 2;
			unsigned char PSB : 3;
#endif
	} BIT;
	} PEL1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSB : 3;
			unsigned char PSP : 2;
			unsigned char PSM : 2;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PSM : 2;
			unsigned char PSP : 2;
			unsigned char PSB : 3;
#endif
	} BIT;
	} PEL2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSB : 3;
			unsigned char PSP : 2;
			unsigned char PSM : 2;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PSM : 2;
			unsigned char PSP : 2;
			unsigned char PSB : 3;
#endif
	} BIT;
	} PEL3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SEG : 1;
			unsigned char  : 5;
			unsigned char WE : 1;
			unsigned char WI : 1;
#else
			unsigned char WI : 1;
			unsigned char WE : 1;
			unsigned char  : 5;
			unsigned char SEG : 1;
#endif
	} BIT;
	} ELSEGR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR30;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR31;
	char           wk6[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR33;
	char           wk7[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR45;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MTU6MD : 2;
			unsigned char MTU7MD : 2;
			unsigned char  : 2;
			unsigned char MTU9MD : 2;
#else
			unsigned char MTU9MD : 2;
			unsigned char  : 2;
			unsigned char MTU7MD : 2;
			unsigned char MTU6MD : 2;
#endif
	} BIT;
	} ELOPE;
	char           wk8[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMTW0MD : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char CMTW0MD : 2;
#endif
	} BIT;
	} ELOPH;
	char           wk9[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR46;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR47;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR48;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR49;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR50;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR51;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR52;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR53;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR54;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR55;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR56;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR57;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELS : 8;
#else
			unsigned char ELS : 8;
#endif
	} BIT;
	} ELSR58;
} st_elc_t;

typedef struct st_flash {
	char           wk00[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FLWE : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char FLWE : 2;
#endif
	} BIT;
	} FWEPROR;
	char           wk0[7794397];
	unsigned long  UIDR0;
	char           wk1[108];
	unsigned long  UIDR1;
	unsigned long  UIDR2;
	char           wk2[11812];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char DFAE : 1;
			unsigned char CMDLK : 1;
			unsigned char  : 2;
			unsigned char CFAE : 1;
#else
			unsigned char CFAE : 1;
			unsigned char  : 2;
			unsigned char CMDLK : 1;
			unsigned char DFAE : 1;
			unsigned char  : 3;
#endif
	} BIT;
	} FASTAT;
	char           wk3[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char DFAEIE : 1;
			unsigned char CMDLKIE : 1;
			unsigned char  : 2;
			unsigned char CFAEIE : 1;
#else
			unsigned char CFAEIE : 1;
			unsigned char  : 2;
			unsigned char CMDLKIE : 1;
			unsigned char DFAEIE : 1;
			unsigned char  : 3;
#endif
	} BIT;
	} FAEINT;
	char           wk4[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FRDYIE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char FRDYIE : 1;
#endif
	} BIT;
	} FRDYIE;
	char           wk5[23];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long FSADDR : 32;
#else
			unsigned long FSADDR : 32;
#endif
	} BIT;
	} FSADDR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long FEADDR : 32;
#else
			unsigned long FEADDR : 32;
#endif
	} BIT;
	} FEADDR;
	char           wk6[72];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 6;
			unsigned long FLWEERR : 1;
			unsigned long  : 1;
			unsigned long PRGSPD : 1;
			unsigned long ERSSPD : 1;
			unsigned long DBFULL : 1;
			unsigned long SUSRDY : 1;
			unsigned long PRGERR : 1;
			unsigned long ERSERR : 1;
			unsigned long ILGLERR : 1;
			unsigned long FRDY : 1;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long FRDY : 1;
			unsigned long ILGLERR : 1;
			unsigned long ERSERR : 1;
			unsigned long PRGERR : 1;
			unsigned long SUSRDY : 1;
			unsigned long DBFULL : 1;
			unsigned long ERSSPD : 1;
			unsigned long PRGSPD : 1;
			unsigned long  : 1;
			unsigned long FLWEERR : 1;
			unsigned long  : 6;
#endif
	} BIT;
	} FSTATR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short FENTRYC : 1;
			unsigned short  : 6;
			unsigned short FENTRYD : 1;
			unsigned short KEY : 8;
#else
			unsigned short KEY : 8;
			unsigned short FENTRYD : 1;
			unsigned short  : 6;
			unsigned short FENTRYC : 1;
#endif
	} BIT;
	} FENTRYR;
	char           wk7[6];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short SUINIT : 1;
			unsigned short  : 7;
			unsigned short KEY : 8;
#else
			unsigned short KEY : 8;
			unsigned short  : 7;
			unsigned short SUINIT : 1;
#endif
	} BIT;
	} FSUINITR;
	char           wk8[18];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PCMDR : 8;
			unsigned short CMDR : 8;
#else
			unsigned short CMDR : 8;
			unsigned short PCMDR : 8;
#endif
	} BIT;
	} FCMDR;
	char           wk9[30];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PEERRST : 8;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short PEERRST : 8;
#endif
	} BIT;
	} FPESTAT;
	char           wk10[14];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BCDIR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char BCDIR : 1;
#endif
	} BIT;
	} FBCCNT;
	char           wk11[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BCST : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char BCST : 1;
#endif
	} BIT;
	} FBCSTAT;
	char           wk12[3];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PSADR : 19;
			unsigned long  : 13;
#else
			unsigned long  : 13;
			unsigned long PSADR : 19;
#endif
	} BIT;
	} FPSADDR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long FAWS : 13;
			unsigned long  : 2;
			unsigned long FSPR : 1;
			unsigned long FAWE : 13;
			unsigned long  : 2;
			unsigned long BTFLG : 1;
#else
			unsigned long BTFLG : 1;
			unsigned long  : 2;
			unsigned long FAWE : 13;
			unsigned long FSPR : 1;
			unsigned long  : 2;
			unsigned long FAWS : 13;
#endif
	} BIT;
	} FAWMON;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ESUSPMD : 1;
			unsigned short  : 15;
#else
			unsigned short  : 15;
			unsigned short ESUSPMD : 1;
#endif
	} BIT;
	} FCPSR;
	char           wk13[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PCKA : 8;
			unsigned short KEY : 8;
#else
			unsigned short KEY : 8;
			unsigned short PCKA : 8;
#endif
	} BIT;
	} FPCKAR;
	char           wk14[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short SAS : 2;
			unsigned short  : 6;
			unsigned short KEY : 8;
#else
			unsigned short KEY : 8;
			unsigned short  : 6;
			unsigned short SAS : 2;
#endif
	} BIT;
	} FSUACR;
} st_flash_t;

typedef struct st_gptw {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long UF : 1;
			unsigned long VF : 1;
			unsigned long WF : 1;
			unsigned long  : 1;
			unsigned long U : 1;
			unsigned long V : 1;
			unsigned long W : 1;
			unsigned long  : 1;
			unsigned long EN : 1;
			unsigned long  : 7;
			unsigned long FB : 1;
			unsigned long P : 1;
			unsigned long N : 1;
			unsigned long INV : 1;
			unsigned long RV : 1;
			unsigned long ALIGN : 1;
			unsigned long  : 2;
			unsigned long GRP : 2;
			unsigned long GODF : 1;
			unsigned long  : 2;
			unsigned long NFEN : 1;
			unsigned long NFCS : 2;
#else
			unsigned long NFCS : 2;
			unsigned long NFEN : 1;
			unsigned long  : 2;
			unsigned long GODF : 1;
			unsigned long GRP : 2;
			unsigned long  : 2;
			unsigned long ALIGN : 1;
			unsigned long RV : 1;
			unsigned long INV : 1;
			unsigned long N : 1;
			unsigned long P : 1;
			unsigned long FB : 1;
			unsigned long  : 7;
			unsigned long EN : 1;
			unsigned long  : 1;
			unsigned long W : 1;
			unsigned long V : 1;
			unsigned long U : 1;
			unsigned long  : 1;
			unsigned long WF : 1;
			unsigned long VF : 1;
			unsigned long UF : 1;
#endif
	} BIT;
	} OPSCR;
} st_gptw_t;

typedef struct st_gptw0 {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long WP : 1;
			unsigned long STRWP : 1;
			unsigned long STPWP : 1;
			unsigned long CLRWP : 1;
			unsigned long CMNWP : 1;
			unsigned long  : 3;
			unsigned long PRKEY : 8;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long PRKEY : 8;
			unsigned long  : 3;
			unsigned long CMNWP : 1;
			unsigned long CLRWP : 1;
			unsigned long STPWP : 1;
			unsigned long STRWP : 1;
			unsigned long WP : 1;
#endif
	} BIT;
	} GTWP;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CSTRT0 : 1;
			unsigned long CSTRT1 : 1;
			unsigned long CSTRT2 : 1;
			unsigned long CSTRT3 : 1;
			unsigned long CSTRT4 : 1;
			unsigned long CSTRT5 : 1;
			unsigned long CSTRT6 : 1;
			unsigned long CSTRT7 : 1;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long CSTRT7 : 1;
			unsigned long CSTRT6 : 1;
			unsigned long CSTRT5 : 1;
			unsigned long CSTRT4 : 1;
			unsigned long CSTRT3 : 1;
			unsigned long CSTRT2 : 1;
			unsigned long CSTRT1 : 1;
			unsigned long CSTRT0 : 1;
#endif
	} BIT;
	} GTSTR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CSTOP0 : 1;
			unsigned long CSTOP1 : 1;
			unsigned long CSTOP2 : 1;
			unsigned long CSTOP3 : 1;
			unsigned long CSTOP4 : 1;
			unsigned long CSTOP5 : 1;
			unsigned long CSTOP6 : 1;
			unsigned long CSTOP7 : 1;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long CSTOP7 : 1;
			unsigned long CSTOP6 : 1;
			unsigned long CSTOP5 : 1;
			unsigned long CSTOP4 : 1;
			unsigned long CSTOP3 : 1;
			unsigned long CSTOP2 : 1;
			unsigned long CSTOP1 : 1;
			unsigned long CSTOP0 : 1;
#endif
	} BIT;
	} GTSTP;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CCLR0 : 1;
			unsigned long CCLR1 : 1;
			unsigned long CCLR2 : 1;
			unsigned long CCLR3 : 1;
			unsigned long CCLR4 : 1;
			unsigned long CCLR5 : 1;
			unsigned long CCLR6 : 1;
			unsigned long CCLR7 : 1;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long CCLR7 : 1;
			unsigned long CCLR6 : 1;
			unsigned long CCLR5 : 1;
			unsigned long CCLR4 : 1;
			unsigned long CCLR3 : 1;
			unsigned long CCLR2 : 1;
			unsigned long CCLR1 : 1;
			unsigned long CCLR0 : 1;
#endif
	} BIT;
	} GTCLR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SSGTRGAR : 1;
			unsigned long SSGTRGAF : 1;
			unsigned long SSGTRGBR : 1;
			unsigned long SSGTRGBF : 1;
			unsigned long SSGTRGCR : 1;
			unsigned long SSGTRGCF : 1;
			unsigned long SSGTRGDR : 1;
			unsigned long SSGTRGDF : 1;
			unsigned long SSCARBL : 1;
			unsigned long SSCARBH : 1;
			unsigned long SSCAFBL : 1;
			unsigned long SSCAFBH : 1;
			unsigned long SSCBRAL : 1;
			unsigned long SSCBRAH : 1;
			unsigned long SSCBFAL : 1;
			unsigned long SSCBFAH : 1;
			unsigned long SSELCA : 1;
			unsigned long SSELCB : 1;
			unsigned long SSELCC : 1;
			unsigned long SSELCD : 1;
			unsigned long SSELCE : 1;
			unsigned long SSELCF : 1;
			unsigned long SSELCG : 1;
			unsigned long SSELCH : 1;
			unsigned long  : 7;
			unsigned long CSTRT : 1;
#else
			unsigned long CSTRT : 1;
			unsigned long  : 7;
			unsigned long SSELCH : 1;
			unsigned long SSELCG : 1;
			unsigned long SSELCF : 1;
			unsigned long SSELCE : 1;
			unsigned long SSELCD : 1;
			unsigned long SSELCC : 1;
			unsigned long SSELCB : 1;
			unsigned long SSELCA : 1;
			unsigned long SSCBFAH : 1;
			unsigned long SSCBFAL : 1;
			unsigned long SSCBRAH : 1;
			unsigned long SSCBRAL : 1;
			unsigned long SSCAFBH : 1;
			unsigned long SSCAFBL : 1;
			unsigned long SSCARBH : 1;
			unsigned long SSCARBL : 1;
			unsigned long SSGTRGDF : 1;
			unsigned long SSGTRGDR : 1;
			unsigned long SSGTRGCF : 1;
			unsigned long SSGTRGCR : 1;
			unsigned long SSGTRGBF : 1;
			unsigned long SSGTRGBR : 1;
			unsigned long SSGTRGAF : 1;
			unsigned long SSGTRGAR : 1;
#endif
	} BIT;
	} GTSSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PSGTRGAR : 1;
			unsigned long PSGTRGAF : 1;
			unsigned long PSGTRGBR : 1;
			unsigned long PSGTRGBF : 1;
			unsigned long PSGTRGCR : 1;
			unsigned long PSGTRGCF : 1;
			unsigned long PSGTRGDR : 1;
			unsigned long PSGTRGDF : 1;
			unsigned long PSCARBL : 1;
			unsigned long PSCARBH : 1;
			unsigned long PSCAFBL : 1;
			unsigned long PSCAFBH : 1;
			unsigned long PSCBRAL : 1;
			unsigned long PSCBRAH : 1;
			unsigned long PSCBFAL : 1;
			unsigned long PSCBFAH : 1;
			unsigned long PSELCA : 1;
			unsigned long PSELCB : 1;
			unsigned long PSELCC : 1;
			unsigned long PSELCD : 1;
			unsigned long PSELCE : 1;
			unsigned long PSELCF : 1;
			unsigned long PSELCG : 1;
			unsigned long PSELCH : 1;
			unsigned long  : 7;
			unsigned long CSTOP : 1;
#else
			unsigned long CSTOP : 1;
			unsigned long  : 7;
			unsigned long PSELCH : 1;
			unsigned long PSELCG : 1;
			unsigned long PSELCF : 1;
			unsigned long PSELCE : 1;
			unsigned long PSELCD : 1;
			unsigned long PSELCC : 1;
			unsigned long PSELCB : 1;
			unsigned long PSELCA : 1;
			unsigned long PSCBFAH : 1;
			unsigned long PSCBFAL : 1;
			unsigned long PSCBRAH : 1;
			unsigned long PSCBRAL : 1;
			unsigned long PSCAFBH : 1;
			unsigned long PSCAFBL : 1;
			unsigned long PSCARBH : 1;
			unsigned long PSCARBL : 1;
			unsigned long PSGTRGDF : 1;
			unsigned long PSGTRGDR : 1;
			unsigned long PSGTRGCF : 1;
			unsigned long PSGTRGCR : 1;
			unsigned long PSGTRGBF : 1;
			unsigned long PSGTRGBR : 1;
			unsigned long PSGTRGAF : 1;
			unsigned long PSGTRGAR : 1;
#endif
	} BIT;
	} GTPSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CSGTRGAR : 1;
			unsigned long CSGTRGAF : 1;
			unsigned long CSGTRGBR : 1;
			unsigned long CSGTRGBF : 1;
			unsigned long CSGTRGCR : 1;
			unsigned long CSGTRGCF : 1;
			unsigned long CSGTRGDR : 1;
			unsigned long CSGTRGDF : 1;
			unsigned long CSCARBL : 1;
			unsigned long CSCARBH : 1;
			unsigned long CSCAFBL : 1;
			unsigned long CSCAFBH : 1;
			unsigned long CSCBRAL : 1;
			unsigned long CSCBRAH : 1;
			unsigned long CSCBFAL : 1;
			unsigned long CSCBFAH : 1;
			unsigned long CSELCA : 1;
			unsigned long CSELCB : 1;
			unsigned long CSELCC : 1;
			unsigned long CSELCD : 1;
			unsigned long CSELCE : 1;
			unsigned long CSELCF : 1;
			unsigned long CSELCG : 1;
			unsigned long CSELCH : 1;
			unsigned long CSCMSC : 3;
			unsigned long CP1CCE : 1;
			unsigned long  : 3;
			unsigned long CCLR : 1;
#else
			unsigned long CCLR : 1;
			unsigned long  : 3;
			unsigned long CP1CCE : 1;
			unsigned long CSCMSC : 3;
			unsigned long CSELCH : 1;
			unsigned long CSELCG : 1;
			unsigned long CSELCF : 1;
			unsigned long CSELCE : 1;
			unsigned long CSELCD : 1;
			unsigned long CSELCC : 1;
			unsigned long CSELCB : 1;
			unsigned long CSELCA : 1;
			unsigned long CSCBFAH : 1;
			unsigned long CSCBFAL : 1;
			unsigned long CSCBRAH : 1;
			unsigned long CSCBRAL : 1;
			unsigned long CSCAFBH : 1;
			unsigned long CSCAFBL : 1;
			unsigned long CSCARBH : 1;
			unsigned long CSCARBL : 1;
			unsigned long CSGTRGDF : 1;
			unsigned long CSGTRGDR : 1;
			unsigned long CSGTRGCF : 1;
			unsigned long CSGTRGCR : 1;
			unsigned long CSGTRGBF : 1;
			unsigned long CSGTRGBR : 1;
			unsigned long CSGTRGAF : 1;
			unsigned long CSGTRGAR : 1;
#endif
	} BIT;
	} GTCSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long USGTRGAR : 1;
			unsigned long USGTRGAF : 1;
			unsigned long USGTRGBR : 1;
			unsigned long USGTRGBF : 1;
			unsigned long USGTRGCR : 1;
			unsigned long USGTRGCF : 1;
			unsigned long USGTRGDR : 1;
			unsigned long USGTRGDF : 1;
			unsigned long USCARBL : 1;
			unsigned long USCARBH : 1;
			unsigned long USCAFBL : 1;
			unsigned long USCAFBH : 1;
			unsigned long USCBRAL : 1;
			unsigned long USCBRAH : 1;
			unsigned long USCBFAL : 1;
			unsigned long USCBFAH : 1;
			unsigned long USELCA : 1;
			unsigned long USELCB : 1;
			unsigned long USELCC : 1;
			unsigned long USELCD : 1;
			unsigned long USELCE : 1;
			unsigned long USELCF : 1;
			unsigned long USELCG : 1;
			unsigned long USELCH : 1;
			unsigned long USILVL : 4;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long USILVL : 4;
			unsigned long USELCH : 1;
			unsigned long USELCG : 1;
			unsigned long USELCF : 1;
			unsigned long USELCE : 1;
			unsigned long USELCD : 1;
			unsigned long USELCC : 1;
			unsigned long USELCB : 1;
			unsigned long USELCA : 1;
			unsigned long USCBFAH : 1;
			unsigned long USCBFAL : 1;
			unsigned long USCBRAH : 1;
			unsigned long USCBRAL : 1;
			unsigned long USCAFBH : 1;
			unsigned long USCAFBL : 1;
			unsigned long USCARBH : 1;
			unsigned long USCARBL : 1;
			unsigned long USGTRGDF : 1;
			unsigned long USGTRGDR : 1;
			unsigned long USGTRGCF : 1;
			unsigned long USGTRGCR : 1;
			unsigned long USGTRGBF : 1;
			unsigned long USGTRGBR : 1;
			unsigned long USGTRGAF : 1;
			unsigned long USGTRGAR : 1;
#endif
	} BIT;
	} GTUPSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DSGTRGAR : 1;
			unsigned long DSGTRGAF : 1;
			unsigned long DSGTRGBR : 1;
			unsigned long DSGTRGBF : 1;
			unsigned long DSGTRGCR : 1;
			unsigned long DSGTRGCF : 1;
			unsigned long DSGTRGDR : 1;
			unsigned long DSGTRGDF : 1;
			unsigned long DSCARBL : 1;
			unsigned long DSCARBH : 1;
			unsigned long DSCAFBL : 1;
			unsigned long DSCAFBH : 1;
			unsigned long DSCBRAL : 1;
			unsigned long DSCBRAH : 1;
			unsigned long DSCBFAL : 1;
			unsigned long DSCBFAH : 1;
			unsigned long DSELCA : 1;
			unsigned long DSELCB : 1;
			unsigned long DSELCC : 1;
			unsigned long DSELCD : 1;
			unsigned long DSELCE : 1;
			unsigned long DSELCF : 1;
			unsigned long DSELCG : 1;
			unsigned long DSELCH : 1;
			unsigned long DSILVL : 4;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long DSILVL : 4;
			unsigned long DSELCH : 1;
			unsigned long DSELCG : 1;
			unsigned long DSELCF : 1;
			unsigned long DSELCE : 1;
			unsigned long DSELCD : 1;
			unsigned long DSELCC : 1;
			unsigned long DSELCB : 1;
			unsigned long DSELCA : 1;
			unsigned long DSCBFAH : 1;
			unsigned long DSCBFAL : 1;
			unsigned long DSCBRAH : 1;
			unsigned long DSCBRAL : 1;
			unsigned long DSCAFBH : 1;
			unsigned long DSCAFBL : 1;
			unsigned long DSCARBH : 1;
			unsigned long DSCARBL : 1;
			unsigned long DSGTRGDF : 1;
			unsigned long DSGTRGDR : 1;
			unsigned long DSGTRGCF : 1;
			unsigned long DSGTRGCR : 1;
			unsigned long DSGTRGBF : 1;
			unsigned long DSGTRGBR : 1;
			unsigned long DSGTRGAF : 1;
			unsigned long DSGTRGAR : 1;
#endif
	} BIT;
	} GTDNSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ASGTRGAR : 1;
			unsigned long ASGTRGAF : 1;
			unsigned long ASGTRGBR : 1;
			unsigned long ASGTRGBF : 1;
			unsigned long ASGTRGCR : 1;
			unsigned long ASGTRGCF : 1;
			unsigned long ASGTRGDR : 1;
			unsigned long ASGTRGDF : 1;
			unsigned long ASCARBL : 1;
			unsigned long ASCARBH : 1;
			unsigned long ASCAFBL : 1;
			unsigned long ASCAFBH : 1;
			unsigned long ASCBRAL : 1;
			unsigned long ASCBRAH : 1;
			unsigned long ASCBFAL : 1;
			unsigned long ASCBFAH : 1;
			unsigned long ASELCA : 1;
			unsigned long ASELCB : 1;
			unsigned long ASELCC : 1;
			unsigned long ASELCD : 1;
			unsigned long ASELCE : 1;
			unsigned long ASELCF : 1;
			unsigned long ASELCG : 1;
			unsigned long ASELCH : 1;
			unsigned long ASOC : 1;
			unsigned long  : 7;
#else
			unsigned long  : 7;
			unsigned long ASOC : 1;
			unsigned long ASELCH : 1;
			unsigned long ASELCG : 1;
			unsigned long ASELCF : 1;
			unsigned long ASELCE : 1;
			unsigned long ASELCD : 1;
			unsigned long ASELCC : 1;
			unsigned long ASELCB : 1;
			unsigned long ASELCA : 1;
			unsigned long ASCBFAH : 1;
			unsigned long ASCBFAL : 1;
			unsigned long ASCBRAH : 1;
			unsigned long ASCBRAL : 1;
			unsigned long ASCAFBH : 1;
			unsigned long ASCAFBL : 1;
			unsigned long ASCARBH : 1;
			unsigned long ASCARBL : 1;
			unsigned long ASGTRGDF : 1;
			unsigned long ASGTRGDR : 1;
			unsigned long ASGTRGCF : 1;
			unsigned long ASGTRGCR : 1;
			unsigned long ASGTRGBF : 1;
			unsigned long ASGTRGBR : 1;
			unsigned long ASGTRGAF : 1;
			unsigned long ASGTRGAR : 1;
#endif
	} BIT;
	} GTICASR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BSGTRGAR : 1;
			unsigned long BSGTRGAF : 1;
			unsigned long BSGTRGBR : 1;
			unsigned long BSGTRGBF : 1;
			unsigned long BSGTRGCR : 1;
			unsigned long BSGTRGCF : 1;
			unsigned long BSGTRGDR : 1;
			unsigned long BSGTRGDF : 1;
			unsigned long BSCARBL : 1;
			unsigned long BSCARBH : 1;
			unsigned long BSCAFBL : 1;
			unsigned long BSCAFBH : 1;
			unsigned long BSCBRAL : 1;
			unsigned long BSCBRAH : 1;
			unsigned long BSCBFAL : 1;
			unsigned long BSCBFAH : 1;
			unsigned long BSELCA : 1;
			unsigned long BSELCB : 1;
			unsigned long BSELCC : 1;
			unsigned long BSELCD : 1;
			unsigned long BSELCE : 1;
			unsigned long BSELCF : 1;
			unsigned long BSELCG : 1;
			unsigned long BSELCH : 1;
			unsigned long BSOC : 1;
			unsigned long  : 7;
#else
			unsigned long  : 7;
			unsigned long BSOC : 1;
			unsigned long BSELCH : 1;
			unsigned long BSELCG : 1;
			unsigned long BSELCF : 1;
			unsigned long BSELCE : 1;
			unsigned long BSELCD : 1;
			unsigned long BSELCC : 1;
			unsigned long BSELCB : 1;
			unsigned long BSELCA : 1;
			unsigned long BSCBFAH : 1;
			unsigned long BSCBFAL : 1;
			unsigned long BSCBRAH : 1;
			unsigned long BSCBRAL : 1;
			unsigned long BSCAFBH : 1;
			unsigned long BSCAFBL : 1;
			unsigned long BSCARBH : 1;
			unsigned long BSCARBL : 1;
			unsigned long BSGTRGDF : 1;
			unsigned long BSGTRGDR : 1;
			unsigned long BSGTRGCF : 1;
			unsigned long BSGTRGCR : 1;
			unsigned long BSGTRGBF : 1;
			unsigned long BSGTRGBR : 1;
			unsigned long BSGTRGAF : 1;
			unsigned long BSGTRGAR : 1;
#endif
	} BIT;
	} GTICBSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CST : 1;
			unsigned long  : 7;
			unsigned long ICDS : 1;
			unsigned long SCGTIOC : 1;
			unsigned long SSCGRP : 2;
			unsigned long  : 3;
			unsigned long SSCEN : 1;
			unsigned long MD : 4;
			unsigned long  : 3;
			unsigned long TPCS : 4;
			unsigned long CKEG : 2;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long CKEG : 2;
			unsigned long TPCS : 4;
			unsigned long  : 3;
			unsigned long MD : 4;
			unsigned long SSCEN : 1;
			unsigned long  : 3;
			unsigned long SSCGRP : 2;
			unsigned long SCGTIOC : 1;
			unsigned long ICDS : 1;
			unsigned long  : 7;
			unsigned long CST : 1;
#endif
	} BIT;
	} GTCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long UD : 1;
			unsigned long UDF : 1;
			unsigned long  : 14;
			unsigned long OADTY : 2;
			unsigned long OADTYF : 1;
			unsigned long OADTYR : 1;
			unsigned long  : 4;
			unsigned long OBDTY : 2;
			unsigned long OBDTYF : 1;
			unsigned long OBDTYR : 1;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long OBDTYR : 1;
			unsigned long OBDTYF : 1;
			unsigned long OBDTY : 2;
			unsigned long  : 4;
			unsigned long OADTYR : 1;
			unsigned long OADTYF : 1;
			unsigned long OADTY : 2;
			unsigned long  : 14;
			unsigned long UDF : 1;
			unsigned long UD : 1;
#endif
	} BIT;
	} GTUDDTYC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long GTIOA : 5;
			unsigned long CPSCIR : 1;
			unsigned long OADFLT : 1;
			unsigned long OAHLD : 1;
			unsigned long OAE : 1;
			unsigned long OADF : 2;
			unsigned long OAEOCD : 1;
			unsigned long PSYE : 1;
			unsigned long NFAEN : 1;
			unsigned long NFCSA : 2;
			unsigned long GTIOB : 5;
			unsigned long  : 1;
			unsigned long OBDFLT : 1;
			unsigned long OBHLD : 1;
			unsigned long OBE : 1;
			unsigned long OBDF : 2;
			unsigned long OBEOCD : 1;
			unsigned long  : 1;
			unsigned long NFBEN : 1;
			unsigned long NFCSB : 2;
#else
			unsigned long NFCSB : 2;
			unsigned long NFBEN : 1;
			unsigned long  : 1;
			unsigned long OBEOCD : 1;
			unsigned long OBDF : 2;
			unsigned long OBE : 1;
			unsigned long OBHLD : 1;
			unsigned long OBDFLT : 1;
			unsigned long  : 1;
			unsigned long GTIOB : 5;
			unsigned long NFCSA : 2;
			unsigned long NFAEN : 1;
			unsigned long PSYE : 1;
			unsigned long OAEOCD : 1;
			unsigned long OADF : 2;
			unsigned long OAE : 1;
			unsigned long OAHLD : 1;
			unsigned long OADFLT : 1;
			unsigned long CPSCIR : 1;
			unsigned long GTIOA : 5;
#endif
	} BIT;
	} GTIOR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long GTINTA : 1;
			unsigned long GTINTB : 1;
			unsigned long GTINTC : 1;
			unsigned long GTINTD : 1;
			unsigned long GTINTE : 1;
			unsigned long GTINTF : 1;
			unsigned long GTINTPR : 2;
			unsigned long SCFA : 1;
			unsigned long SCFB : 1;
			unsigned long SCFC : 1;
			unsigned long SCFD : 1;
			unsigned long SCFE : 1;
			unsigned long SCFF : 1;
			unsigned long SCFPO : 1;
			unsigned long SCFPU : 1;
			unsigned long ADTRAUEN : 1;
			unsigned long ADTRADEN : 1;
			unsigned long ADTRBUEN : 1;
			unsigned long ADTRBDEN : 1;
			unsigned long  : 4;
			unsigned long GRP : 2;
			unsigned long  : 2;
			unsigned long GRPDTE : 1;
			unsigned long GRPABH : 1;
			unsigned long GRPABL : 1;
			unsigned long GTINTPC : 1;
#else
			unsigned long GTINTPC : 1;
			unsigned long GRPABL : 1;
			unsigned long GRPABH : 1;
			unsigned long GRPDTE : 1;
			unsigned long  : 2;
			unsigned long GRP : 2;
			unsigned long  : 4;
			unsigned long ADTRBDEN : 1;
			unsigned long ADTRBUEN : 1;
			unsigned long ADTRADEN : 1;
			unsigned long ADTRAUEN : 1;
			unsigned long SCFPU : 1;
			unsigned long SCFPO : 1;
			unsigned long SCFF : 1;
			unsigned long SCFE : 1;
			unsigned long SCFD : 1;
			unsigned long SCFC : 1;
			unsigned long SCFB : 1;
			unsigned long SCFA : 1;
			unsigned long GTINTPR : 2;
			unsigned long GTINTF : 1;
			unsigned long GTINTE : 1;
			unsigned long GTINTD : 1;
			unsigned long GTINTC : 1;
			unsigned long GTINTB : 1;
			unsigned long GTINTA : 1;
#endif
	} BIT;
	} GTINTAD;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TCFA : 1;
			unsigned long TCFB : 1;
			unsigned long TCFC : 1;
			unsigned long TCFD : 1;
			unsigned long TCFE : 1;
			unsigned long TCFF : 1;
			unsigned long TCFPO : 1;
			unsigned long TCFPU : 1;
			unsigned long ITCNT : 3;
			unsigned long  : 4;
			unsigned long TUCF : 1;
			unsigned long ADTRAUF : 1;
			unsigned long ADTRADF : 1;
			unsigned long ADTRBUF : 1;
			unsigned long ADTRBDF : 1;
			unsigned long  : 4;
			unsigned long ODF : 1;
			unsigned long  : 3;
			unsigned long DTEF : 1;
			unsigned long OABHF : 1;
			unsigned long OABLF : 1;
			unsigned long PCF : 1;
#else
			unsigned long PCF : 1;
			unsigned long OABLF : 1;
			unsigned long OABHF : 1;
			unsigned long DTEF : 1;
			unsigned long  : 3;
			unsigned long ODF : 1;
			unsigned long  : 4;
			unsigned long ADTRBDF : 1;
			unsigned long ADTRBUF : 1;
			unsigned long ADTRADF : 1;
			unsigned long ADTRAUF : 1;
			unsigned long TUCF : 1;
			unsigned long  : 4;
			unsigned long ITCNT : 3;
			unsigned long TCFPU : 1;
			unsigned long TCFPO : 1;
			unsigned long TCFF : 1;
			unsigned long TCFE : 1;
			unsigned long TCFD : 1;
			unsigned long TCFC : 1;
			unsigned long TCFB : 1;
			unsigned long TCFA : 1;
#endif
	} BIT;
	} GTST;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BD : 4;
			unsigned long  : 4;
			unsigned long DBRTECA : 1;
			unsigned long DBRTSCA : 1;
			unsigned long DBRTECB : 1;
			unsigned long DBRTSCB : 1;
			unsigned long DBRTEADA : 1;
			unsigned long DBRTSADA : 1;
			unsigned long DBRTEADB : 1;
			unsigned long DBRTSADB : 1;
			unsigned long CCRA : 2;
			unsigned long CCRB : 2;
			unsigned long PR : 2;
			unsigned long CCRSWT : 1;
			unsigned long  : 1;
			unsigned long ADTTA : 2;
			unsigned long ADTDA : 1;
			unsigned long  : 1;
			unsigned long ADTTB : 2;
			unsigned long ADTDB : 1;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long ADTDB : 1;
			unsigned long ADTTB : 2;
			unsigned long  : 1;
			unsigned long ADTDA : 1;
			unsigned long ADTTA : 2;
			unsigned long  : 1;
			unsigned long CCRSWT : 1;
			unsigned long PR : 2;
			unsigned long CCRB : 2;
			unsigned long CCRA : 2;
			unsigned long DBRTSADB : 1;
			unsigned long DBRTEADB : 1;
			unsigned long DBRTSADA : 1;
			unsigned long DBRTEADA : 1;
			unsigned long DBRTSCB : 1;
			unsigned long DBRTECB : 1;
			unsigned long DBRTSCA : 1;
			unsigned long DBRTECA : 1;
			unsigned long  : 4;
			unsigned long BD : 4;
#endif
	} BIT;
	} GTBER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ITLA : 1;
			unsigned long ITLB : 1;
			unsigned long ITLC : 1;
			unsigned long ITLD : 1;
			unsigned long ITLE : 1;
			unsigned long ITLF : 1;
			unsigned long IVTC : 2;
			unsigned long IVTT : 3;
			unsigned long  : 1;
			unsigned long ADTAL : 1;
			unsigned long  : 1;
			unsigned long ADTBL : 1;
			unsigned long  : 17;
#else
			unsigned long  : 17;
			unsigned long ADTBL : 1;
			unsigned long  : 1;
			unsigned long ADTAL : 1;
			unsigned long  : 1;
			unsigned long IVTT : 3;
			unsigned long IVTC : 2;
			unsigned long ITLF : 1;
			unsigned long ITLE : 1;
			unsigned long ITLD : 1;
			unsigned long ITLC : 1;
			unsigned long ITLB : 1;
			unsigned long ITLA : 1;
#endif
	} BIT;
	} GTITC;
	unsigned long  GTCNT;
	unsigned long  GTCCRA;
	unsigned long  GTCCRB;
	unsigned long  GTCCRC;
	unsigned long  GTCCRE;
	unsigned long  GTCCRD;
	unsigned long  GTCCRF;
	unsigned long  GTPR;
	unsigned long  GTPBR;
	unsigned long  GTPDBR;
	unsigned long  GTADTRA;
	unsigned long  GTADTBRA;
	unsigned long  GTADTDBRA;
	unsigned long  GTADTRB;
	unsigned long  GTADTBRB;
	unsigned long  GTADTDBRB;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDE : 1;
			unsigned long  : 3;
			unsigned long TDBUE : 1;
			unsigned long TDBDE : 1;
			unsigned long  : 2;
			unsigned long TDFER : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long TDFER : 1;
			unsigned long  : 2;
			unsigned long TDBDE : 1;
			unsigned long TDBUE : 1;
			unsigned long  : 3;
			unsigned long TDE : 1;
#endif
	} BIT;
	} GTDTCR;
	unsigned long  GTDVU;
	unsigned long  GTDVD;
	unsigned long  GTDBU;
	unsigned long  GTDBD;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SOS : 2;
			unsigned long  : 30;
#else
			unsigned long  : 30;
			unsigned long SOS : 2;
#endif
	} BIT;
	} GTSOS;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SOTR : 1;
			unsigned long  : 31;
#else
			unsigned long  : 31;
			unsigned long SOTR : 1;
#endif
	} BIT;
	} GTSOTR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ADSMS0 : 2;
			unsigned long  : 6;
			unsigned long ADSMEN0 : 1;
			unsigned long  : 7;
			unsigned long ADSMS1 : 2;
			unsigned long  : 6;
			unsigned long ADSMEN1 : 1;
			unsigned long  : 7;
#else
			unsigned long  : 7;
			unsigned long ADSMEN1 : 1;
			unsigned long  : 6;
			unsigned long ADSMS1 : 2;
			unsigned long  : 7;
			unsigned long ADSMEN0 : 1;
			unsigned long  : 6;
			unsigned long ADSMS0 : 2;
#endif
	} BIT;
	} GTADSMR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EIVTC1 : 2;
			unsigned long  : 2;
			unsigned long EIVTT1 : 4;
			unsigned long  : 4;
			unsigned long EITCNT1 : 4;
			unsigned long EIVTC2 : 2;
			unsigned long  : 2;
			unsigned long EIVTT2 : 4;
			unsigned long EITCNT2IV : 4;
			unsigned long EITCNT2 : 4;
#else
			unsigned long EITCNT2 : 4;
			unsigned long EITCNT2IV : 4;
			unsigned long EIVTT2 : 4;
			unsigned long  : 2;
			unsigned long EIVTC2 : 2;
			unsigned long EITCNT1 : 4;
			unsigned long  : 4;
			unsigned long EIVTT1 : 4;
			unsigned long  : 2;
			unsigned long EIVTC1 : 2;
#endif
	} BIT;
	} GTEITC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EITLA : 3;
			unsigned long  : 1;
			unsigned long EITLB : 3;
			unsigned long  : 1;
			unsigned long EITLC : 3;
			unsigned long  : 1;
			unsigned long EITLD : 3;
			unsigned long  : 1;
			unsigned long EITLE : 3;
			unsigned long  : 1;
			unsigned long EITLF : 3;
			unsigned long  : 1;
			unsigned long EITLV : 3;
			unsigned long  : 1;
			unsigned long EITLU : 3;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long EITLU : 3;
			unsigned long  : 1;
			unsigned long EITLV : 3;
			unsigned long  : 1;
			unsigned long EITLF : 3;
			unsigned long  : 1;
			unsigned long EITLE : 3;
			unsigned long  : 1;
			unsigned long EITLD : 3;
			unsigned long  : 1;
			unsigned long EITLC : 3;
			unsigned long  : 1;
			unsigned long EITLB : 3;
			unsigned long  : 1;
			unsigned long EITLA : 3;
#endif
	} BIT;
	} GTEITLI1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EADTAL : 3;
			unsigned long  : 1;
			unsigned long EADTBL : 3;
			unsigned long  : 25;
#else
			unsigned long  : 25;
			unsigned long EADTBL : 3;
			unsigned long  : 1;
			unsigned long EADTAL : 3;
#endif
	} BIT;
	} GTEITLI2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EBTLCA : 3;
			unsigned long  : 1;
			unsigned long EBTLCB : 3;
			unsigned long  : 1;
			unsigned long EBTLPR : 3;
			unsigned long  : 5;
			unsigned long EBTLADA : 3;
			unsigned long  : 1;
			unsigned long EBTLADB : 3;
			unsigned long  : 1;
			unsigned long EBTLDVU : 3;
			unsigned long  : 1;
			unsigned long EBTLDVD : 3;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long EBTLDVD : 3;
			unsigned long  : 1;
			unsigned long EBTLDVU : 3;
			unsigned long  : 1;
			unsigned long EBTLADB : 3;
			unsigned long  : 1;
			unsigned long EBTLADA : 3;
			unsigned long  : 5;
			unsigned long EBTLPR : 3;
			unsigned long  : 1;
			unsigned long EBTLCB : 3;
			unsigned long  : 1;
			unsigned long EBTLCA : 3;
#endif
	} BIT;
	} GTEITLB;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ICLFA : 3;
			unsigned long  : 1;
			unsigned long ICLFSELC : 6;
			unsigned long  : 6;
			unsigned long ICLFB : 3;
			unsigned long  : 1;
			unsigned long ICLFSELD : 6;
			unsigned long  : 6;
#else
			unsigned long  : 6;
			unsigned long ICLFSELD : 6;
			unsigned long  : 1;
			unsigned long ICLFB : 3;
			unsigned long  : 6;
			unsigned long ICLFSELC : 6;
			unsigned long  : 1;
			unsigned long ICLFA : 3;
#endif
	} BIT;
	} GTICLF;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PCEN : 1;
			unsigned long  : 7;
			unsigned long ASTP : 1;
			unsigned long  : 7;
			unsigned long PCNT : 12;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long PCNT : 12;
			unsigned long  : 7;
			unsigned long ASTP : 1;
			unsigned long  : 7;
			unsigned long PCEN : 1;
#endif
	} BIT;
	} GTPC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ADCMSC1 : 2;
			unsigned long  : 2;
			unsigned long ADCMST1 : 4;
			unsigned long ADCMSCNT1IV : 4;
			unsigned long ADCMSCNT1 : 4;
			unsigned long ADCMSC2 : 2;
			unsigned long  : 2;
			unsigned long ADCMST2 : 4;
			unsigned long ADCMSCNT2IV : 4;
			unsigned long ADCMSCNT2 : 4;
#else
			unsigned long ADCMSCNT2 : 4;
			unsigned long ADCMSCNT2IV : 4;
			unsigned long ADCMST2 : 4;
			unsigned long  : 2;
			unsigned long ADCMSC2 : 2;
			unsigned long ADCMSCNT1 : 4;
			unsigned long ADCMSCNT1IV : 4;
			unsigned long ADCMST1 : 4;
			unsigned long  : 2;
			unsigned long ADCMSC1 : 2;
#endif
	} BIT;
	} GTADCMSC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ADCMSAL : 3;
			unsigned long  : 1;
			unsigned long ADCMSBL : 3;
			unsigned long  : 9;
			unsigned long ADCMBSA : 3;
			unsigned long  : 1;
			unsigned long ADCMBSB : 3;
			unsigned long  : 9;
#else
			unsigned long  : 9;
			unsigned long ADCMBSB : 3;
			unsigned long  : 1;
			unsigned long ADCMBSA : 3;
			unsigned long  : 9;
			unsigned long ADCMSBL : 3;
			unsigned long  : 1;
			unsigned long ADCMSAL : 3;
#endif
	} BIT;
	} GTADCMSS;
	char           wk0[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SECSEL0 : 1;
			unsigned long SECSEL1 : 1;
			unsigned long SECSEL2 : 1;
			unsigned long SECSEL3 : 1;
			unsigned long SECSEL4 : 1;
			unsigned long SECSEL5 : 1;
			unsigned long SECSEL6 : 1;
			unsigned long SECSEL7 : 1;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long SECSEL7 : 1;
			unsigned long SECSEL6 : 1;
			unsigned long SECSEL5 : 1;
			unsigned long SECSEL4 : 1;
			unsigned long SECSEL3 : 1;
			unsigned long SECSEL2 : 1;
			unsigned long SECSEL1 : 1;
			unsigned long SECSEL0 : 1;
#endif
	} BIT;
	} GTSECSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SBDCE : 1;
			unsigned long SBDPE : 1;
			unsigned long SBDAE : 1;
			unsigned long SBDDE : 1;
			unsigned long  : 4;
			unsigned long SBDCD : 1;
			unsigned long SBDPD : 1;
			unsigned long SBDAD : 1;
			unsigned long SBDDD : 1;
			unsigned long  : 4;
			unsigned long SPCE : 1;
			unsigned long SSCE : 1;
			unsigned long  : 6;
			unsigned long SPCD : 1;
			unsigned long SSCD : 1;
			unsigned long  : 6;
#else
			unsigned long  : 6;
			unsigned long SSCD : 1;
			unsigned long SPCD : 1;
			unsigned long  : 6;
			unsigned long SSCE : 1;
			unsigned long SPCE : 1;
			unsigned long  : 4;
			unsigned long SBDDD : 1;
			unsigned long SBDAD : 1;
			unsigned long SBDPD : 1;
			unsigned long SBDCD : 1;
			unsigned long  : 4;
			unsigned long SBDDE : 1;
			unsigned long SBDAE : 1;
			unsigned long SBDPE : 1;
			unsigned long SBDCE : 1;
#endif
	} BIT;
	} GTSECR;
	char           wk1[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CCTCA : 1;
			unsigned long CCTCB : 1;
			unsigned long CCTPR : 1;
			unsigned long CCTADA : 1;
			unsigned long CCTADB : 1;
			unsigned long CCTDV : 1;
			unsigned long  : 2;
			unsigned long CMTCA : 2;
			unsigned long CMTCB : 2;
			unsigned long  : 1;
			unsigned long CMTADA : 1;
			unsigned long CMTADB : 1;
			unsigned long  : 1;
			unsigned long CPTCA : 1;
			unsigned long CPTCB : 1;
			unsigned long CPTPR : 1;
			unsigned long CPTADA : 1;
			unsigned long CPTADB : 1;
			unsigned long CPTDV : 1;
			unsigned long  : 2;
			unsigned long CP3DB : 1;
			unsigned long CPBTD : 1;
			unsigned long OLTTA : 2;
			unsigned long OLTTB : 2;
			unsigned long  : 2;
#else
			unsigned long  : 2;
			unsigned long OLTTB : 2;
			unsigned long OLTTA : 2;
			unsigned long CPBTD : 1;
			unsigned long CP3DB : 1;
			unsigned long  : 2;
			unsigned long CPTDV : 1;
			unsigned long CPTADB : 1;
			unsigned long CPTADA : 1;
			unsigned long CPTPR : 1;
			unsigned long CPTCB : 1;
			unsigned long CPTCA : 1;
			unsigned long  : 1;
			unsigned long CMTADB : 1;
			unsigned long CMTADA : 1;
			unsigned long  : 1;
			unsigned long CMTCB : 2;
			unsigned long CMTCA : 2;
			unsigned long  : 2;
			unsigned long CCTDV : 1;
			unsigned long CCTADB : 1;
			unsigned long CCTADA : 1;
			unsigned long CCTPR : 1;
			unsigned long CCTCB : 1;
			unsigned long CCTCA : 1;
#endif
	} BIT;
	} GTBER2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long GTIOAB : 5;
			unsigned long  : 11;
			unsigned long GTIOBB : 5;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long GTIOBB : 5;
			unsigned long  : 11;
			unsigned long GTIOAB : 5;
#endif
	} BIT;
	} GTOLBR;
	char           wk2[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ICAFA : 1;
			unsigned long ICAFB : 1;
			unsigned long ICAFC : 1;
			unsigned long ICAFD : 1;
			unsigned long ICAFE : 1;
			unsigned long ICAFF : 1;
			unsigned long ICAFPO : 1;
			unsigned long ICAFPU : 1;
			unsigned long ICACLK : 1;
			unsigned long  : 5;
			unsigned long ICAGRP : 2;
			unsigned long ICBFA : 1;
			unsigned long ICBFB : 1;
			unsigned long ICBFC : 1;
			unsigned long ICBFD : 1;
			unsigned long ICBFE : 1;
			unsigned long ICBFF : 1;
			unsigned long ICBFPO : 1;
			unsigned long ICBFPU : 1;
			unsigned long ICBCLK : 1;
			unsigned long  : 5;
			unsigned long ICBGRP : 2;
#else
			unsigned long ICBGRP : 2;
			unsigned long  : 5;
			unsigned long ICBCLK : 1;
			unsigned long ICBFPU : 1;
			unsigned long ICBFPO : 1;
			unsigned long ICBFF : 1;
			unsigned long ICBFE : 1;
			unsigned long ICBFD : 1;
			unsigned long ICBFC : 1;
			unsigned long ICBFB : 1;
			unsigned long ICBFA : 1;
			unsigned long ICAGRP : 2;
			unsigned long  : 5;
			unsigned long ICACLK : 1;
			unsigned long ICAFPU : 1;
			unsigned long ICAFPO : 1;
			unsigned long ICAFF : 1;
			unsigned long ICAFE : 1;
			unsigned long ICAFD : 1;
			unsigned long ICAFC : 1;
			unsigned long ICAFB : 1;
			unsigned long ICAFA : 1;
#endif
	} BIT;
	} GTICCR;
} st_gptw0_t;

typedef struct st_hrpwm {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLLEN : 1;
			unsigned short HRRST : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short HRRST : 1;
			unsigned short DLLEN : 1;
#endif
	} BIT;
	} HROCR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short HRSEL0 : 1;
			unsigned short HRSEL1 : 1;
			unsigned short HRSEL2 : 1;
			unsigned short HRSEL3 : 1;
			unsigned short  : 4;
			unsigned short HRDIS0 : 1;
			unsigned short HRDIS1 : 1;
			unsigned short HRDIS2 : 1;
			unsigned short HRDIS3 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short HRDIS3 : 1;
			unsigned short HRDIS2 : 1;
			unsigned short HRDIS1 : 1;
			unsigned short HRDIS0 : 1;
			unsigned short  : 4;
			unsigned short HRSEL3 : 1;
			unsigned short HRSEL2 : 1;
			unsigned short HRSEL1 : 1;
			unsigned short HRSEL0 : 1;
#endif
	} BIT;
	} HROCR2;
	char           wk0[20];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR0A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR0B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR1A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR1B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR2A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR2B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR3A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRREAR3B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR0A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR0B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR1A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR1B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR2A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR2B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR3A;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DLY : 5;
			unsigned short  : 11;
#else
			unsigned short  : 11;
			unsigned short DLY : 5;
#endif
	} BIT;
	} HRFEAR3B;
} st_hrpwm_t;

typedef struct st_icu {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char IR : 1;
#endif
	} BIT;
	} IR[256];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DTCE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DTCE : 1;
#endif
	} BIT;
	} DTCER[256];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IEN0 : 1;
			unsigned char IEN1 : 1;
			unsigned char IEN2 : 1;
			unsigned char IEN3 : 1;
			unsigned char IEN4 : 1;
			unsigned char IEN5 : 1;
			unsigned char IEN6 : 1;
			unsigned char IEN7 : 1;
#else
			unsigned char IEN7 : 1;
			unsigned char IEN6 : 1;
			unsigned char IEN5 : 1;
			unsigned char IEN4 : 1;
			unsigned char IEN3 : 1;
			unsigned char IEN2 : 1;
			unsigned char IEN1 : 1;
			unsigned char IEN0 : 1;
#endif
	} BIT;
	} IER[32];
	char           wk0[192];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SWINT : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SWINT : 1;
#endif
	} BIT;
	} SWINTR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SWINT2 : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SWINT2 : 1;
#endif
	} BIT;
	} SWINT2R;
	char           wk1[14];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short FVCT : 8;
			unsigned short  : 7;
			unsigned short FIEN : 1;
#else
			unsigned short FIEN : 1;
			unsigned short  : 7;
			unsigned short FVCT : 8;
#endif
	} BIT;
	} FIR;
	char           wk2[14];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IPR : 4;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char IPR : 4;
#endif
	} BIT;
	} IPR[256];
	unsigned char  DMRSR0;
	char           wk3[3];
	unsigned char  DMRSR1;
	char           wk4[3];
	unsigned char  DMRSR2;
	char           wk5[3];
	unsigned char  DMRSR3;
	char           wk6[3];
	unsigned char  DMRSR4;
	char           wk7[3];
	unsigned char  DMRSR5;
	char           wk8[3];
	unsigned char  DMRSR6;
	char           wk9[3];
	unsigned char  DMRSR7;
	char           wk10[227];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 2;
			unsigned char IRQMD : 2;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char IRQMD : 2;
			unsigned char  : 2;
#endif
	} BIT;
	} IRQCR[16];
	char           wk11[16];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FLTEN0 : 1;
			unsigned char FLTEN1 : 1;
			unsigned char FLTEN2 : 1;
			unsigned char FLTEN3 : 1;
			unsigned char FLTEN4 : 1;
			unsigned char FLTEN5 : 1;
			unsigned char FLTEN6 : 1;
			unsigned char FLTEN7 : 1;
#else
			unsigned char FLTEN7 : 1;
			unsigned char FLTEN6 : 1;
			unsigned char FLTEN5 : 1;
			unsigned char FLTEN4 : 1;
			unsigned char FLTEN3 : 1;
			unsigned char FLTEN2 : 1;
			unsigned char FLTEN1 : 1;
			unsigned char FLTEN0 : 1;
#endif
	} BIT;
	} IRQFLTE0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FLTEN8 : 1;
			unsigned char FLTEN9 : 1;
			unsigned char FLTEN10 : 1;
			unsigned char FLTEN11 : 1;
			unsigned char FLTEN12 : 1;
			unsigned char FLTEN13 : 1;
			unsigned char FLTEN14 : 1;
			unsigned char FLTEN15 : 1;
#else
			unsigned char FLTEN15 : 1;
			unsigned char FLTEN14 : 1;
			unsigned char FLTEN13 : 1;
			unsigned char FLTEN12 : 1;
			unsigned char FLTEN11 : 1;
			unsigned char FLTEN10 : 1;
			unsigned char FLTEN9 : 1;
			unsigned char FLTEN8 : 1;
#endif
	} BIT;
	} IRQFLTE1;
	char           wk12[6];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short FCLKSEL0 : 2;
			unsigned short FCLKSEL1 : 2;
			unsigned short FCLKSEL2 : 2;
			unsigned short FCLKSEL3 : 2;
			unsigned short FCLKSEL4 : 2;
			unsigned short FCLKSEL5 : 2;
			unsigned short FCLKSEL6 : 2;
			unsigned short FCLKSEL7 : 2;
#else
			unsigned short FCLKSEL7 : 2;
			unsigned short FCLKSEL6 : 2;
			unsigned short FCLKSEL5 : 2;
			unsigned short FCLKSEL4 : 2;
			unsigned short FCLKSEL3 : 2;
			unsigned short FCLKSEL2 : 2;
			unsigned short FCLKSEL1 : 2;
			unsigned short FCLKSEL0 : 2;
#endif
	} BIT;
	} IRQFLTC0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short FCLKSEL8 : 2;
			unsigned short FCLKSEL9 : 2;
			unsigned short FCLKSEL10 : 2;
			unsigned short FCLKSEL11 : 2;
			unsigned short FCLKSEL12 : 2;
			unsigned short FCLKSEL13 : 2;
			unsigned short FCLKSEL14 : 2;
			unsigned short FCLKSEL15 : 2;
#else
			unsigned short FCLKSEL15 : 2;
			unsigned short FCLKSEL14 : 2;
			unsigned short FCLKSEL13 : 2;
			unsigned short FCLKSEL12 : 2;
			unsigned short FCLKSEL11 : 2;
			unsigned short FCLKSEL10 : 2;
			unsigned short FCLKSEL9 : 2;
			unsigned short FCLKSEL8 : 2;
#endif
	} BIT;
	} IRQFLTC1;
	char           wk13[84];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NMIST : 1;
			unsigned char OSTST : 1;
			unsigned char WDTST : 1;
			unsigned char IWDTST : 1;
			unsigned char LVD1ST : 1;
			unsigned char LVD2ST : 1;
			unsigned char RAMST : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char RAMST : 1;
			unsigned char LVD2ST : 1;
			unsigned char LVD1ST : 1;
			unsigned char IWDTST : 1;
			unsigned char WDTST : 1;
			unsigned char OSTST : 1;
			unsigned char NMIST : 1;
#endif
	} BIT;
	} NMISR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NMIEN : 1;
			unsigned char OSTEN : 1;
			unsigned char WDTEN : 1;
			unsigned char IWDTEN : 1;
			unsigned char LVD1EN : 1;
			unsigned char LVD2EN : 1;
			unsigned char RAMEN : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char RAMEN : 1;
			unsigned char LVD2EN : 1;
			unsigned char LVD1EN : 1;
			unsigned char IWDTEN : 1;
			unsigned char WDTEN : 1;
			unsigned char OSTEN : 1;
			unsigned char NMIEN : 1;
#endif
	} BIT;
	} NMIER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NMICLR : 1;
			unsigned char OSTCLR : 1;
			unsigned char WDTCLR : 1;
			unsigned char IWDTCLR : 1;
			unsigned char LVD1CLR : 1;
			unsigned char LVD2CLR : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char LVD2CLR : 1;
			unsigned char LVD1CLR : 1;
			unsigned char IWDTCLR : 1;
			unsigned char WDTCLR : 1;
			unsigned char OSTCLR : 1;
			unsigned char NMICLR : 1;
#endif
	} BIT;
	} NMICLR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char NMIMD : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char NMIMD : 1;
			unsigned char  : 3;
#endif
	} BIT;
	} NMICR;
	char           wk14[12];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFLTEN : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char NFLTEN : 1;
#endif
	} BIT;
	} NMIFLTE;
	char           wk15[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFCLKSEL : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char NFCLKSEL : 2;
#endif
	} BIT;
	} NMIFLTC;
	char           wk16[155];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IS0 : 1;
			unsigned long IS1 : 1;
			unsigned long IS2 : 1;
			unsigned long IS3 : 1;
			unsigned long IS4 : 1;
			unsigned long IS5 : 1;
			unsigned long IS6 : 1;
			unsigned long IS7 : 1;
			unsigned long IS8 : 1;
			unsigned long IS9 : 1;
			unsigned long IS10 : 1;
			unsigned long IS11 : 1;
			unsigned long IS12 : 1;
			unsigned long IS13 : 1;
			unsigned long IS14 : 1;
			unsigned long IS15 : 1;
			unsigned long IS16 : 1;
			unsigned long IS17 : 1;
			unsigned long IS18 : 1;
			unsigned long IS19 : 1;
			unsigned long IS20 : 1;
			unsigned long IS21 : 1;
			unsigned long IS22 : 1;
			unsigned long IS23 : 1;
			unsigned long IS24 : 1;
			unsigned long IS25 : 1;
			unsigned long IS26 : 1;
			unsigned long IS27 : 1;
			unsigned long IS28 : 1;
			unsigned long IS29 : 1;
			unsigned long IS30 : 1;
			unsigned long IS31 : 1;
#else
			unsigned long IS31 : 1;
			unsigned long IS30 : 1;
			unsigned long IS29 : 1;
			unsigned long IS28 : 1;
			unsigned long IS27 : 1;
			unsigned long IS26 : 1;
			unsigned long IS25 : 1;
			unsigned long IS24 : 1;
			unsigned long IS23 : 1;
			unsigned long IS22 : 1;
			unsigned long IS21 : 1;
			unsigned long IS20 : 1;
			unsigned long IS19 : 1;
			unsigned long IS18 : 1;
			unsigned long IS17 : 1;
			unsigned long IS16 : 1;
			unsigned long IS15 : 1;
			unsigned long IS14 : 1;
			unsigned long IS13 : 1;
			unsigned long IS12 : 1;
			unsigned long IS11 : 1;
			unsigned long IS10 : 1;
			unsigned long IS9 : 1;
			unsigned long IS8 : 1;
			unsigned long IS7 : 1;
			unsigned long IS6 : 1;
			unsigned long IS5 : 1;
			unsigned long IS4 : 1;
			unsigned long IS3 : 1;
			unsigned long IS2 : 1;
			unsigned long IS1 : 1;
			unsigned long IS0 : 1;
#endif
	} BIT;
	} GRPBL0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IS0 : 1;
			unsigned long IS1 : 1;
			unsigned long IS2 : 1;
			unsigned long IS3 : 1;
			unsigned long IS4 : 1;
			unsigned long IS5 : 1;
			unsigned long IS6 : 1;
			unsigned long IS7 : 1;
			unsigned long IS8 : 1;
			unsigned long IS9 : 1;
			unsigned long IS10 : 1;
			unsigned long IS11 : 1;
			unsigned long IS12 : 1;
			unsigned long IS13 : 1;
			unsigned long IS14 : 1;
			unsigned long IS15 : 1;
			unsigned long IS16 : 1;
			unsigned long IS17 : 1;
			unsigned long IS18 : 1;
			unsigned long IS19 : 1;
			unsigned long IS20 : 1;
			unsigned long IS21 : 1;
			unsigned long IS22 : 1;
			unsigned long IS23 : 1;
			unsigned long IS24 : 1;
			unsigned long IS25 : 1;
			unsigned long IS26 : 1;
			unsigned long IS27 : 1;
			unsigned long IS28 : 1;
			unsigned long IS29 : 1;
			unsigned long IS30 : 1;
			unsigned long IS31 : 1;
#else
			unsigned long IS31 : 1;
			unsigned long IS30 : 1;
			unsigned long IS29 : 1;
			unsigned long IS28 : 1;
			unsigned long IS27 : 1;
			unsigned long IS26 : 1;
			unsigned long IS25 : 1;
			unsigned long IS24 : 1;
			unsigned long IS23 : 1;
			unsigned long IS22 : 1;
			unsigned long IS21 : 1;
			unsigned long IS20 : 1;
			unsigned long IS19 : 1;
			unsigned long IS18 : 1;
			unsigned long IS17 : 1;
			unsigned long IS16 : 1;
			unsigned long IS15 : 1;
			unsigned long IS14 : 1;
			unsigned long IS13 : 1;
			unsigned long IS12 : 1;
			unsigned long IS11 : 1;
			unsigned long IS10 : 1;
			unsigned long IS9 : 1;
			unsigned long IS8 : 1;
			unsigned long IS7 : 1;
			unsigned long IS6 : 1;
			unsigned long IS5 : 1;
			unsigned long IS4 : 1;
			unsigned long IS3 : 1;
			unsigned long IS2 : 1;
			unsigned long IS1 : 1;
			unsigned long IS0 : 1;
#endif
	} BIT;
	} GRPBL1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IS0 : 1;
			unsigned long IS1 : 1;
			unsigned long IS2 : 1;
			unsigned long IS3 : 1;
			unsigned long IS4 : 1;
			unsigned long IS5 : 1;
			unsigned long IS6 : 1;
			unsigned long IS7 : 1;
			unsigned long IS8 : 1;
			unsigned long IS9 : 1;
			unsigned long IS10 : 1;
			unsigned long IS11 : 1;
			unsigned long IS12 : 1;
			unsigned long IS13 : 1;
			unsigned long IS14 : 1;
			unsigned long IS15 : 1;
			unsigned long IS16 : 1;
			unsigned long IS17 : 1;
			unsigned long IS18 : 1;
			unsigned long IS19 : 1;
			unsigned long IS20 : 1;
			unsigned long IS21 : 1;
			unsigned long IS22 : 1;
			unsigned long IS23 : 1;
			unsigned long IS24 : 1;
			unsigned long IS25 : 1;
			unsigned long IS26 : 1;
			unsigned long IS27 : 1;
			unsigned long IS28 : 1;
			unsigned long IS29 : 1;
			unsigned long IS30 : 1;
			unsigned long IS31 : 1;
#else
			unsigned long IS31 : 1;
			unsigned long IS30 : 1;
			unsigned long IS29 : 1;
			unsigned long IS28 : 1;
			unsigned long IS27 : 1;
			unsigned long IS26 : 1;
			unsigned long IS25 : 1;
			unsigned long IS24 : 1;
			unsigned long IS23 : 1;
			unsigned long IS22 : 1;
			unsigned long IS21 : 1;
			unsigned long IS20 : 1;
			unsigned long IS19 : 1;
			unsigned long IS18 : 1;
			unsigned long IS17 : 1;
			unsigned long IS16 : 1;
			unsigned long IS15 : 1;
			unsigned long IS14 : 1;
			unsigned long IS13 : 1;
			unsigned long IS12 : 1;
			unsigned long IS11 : 1;
			unsigned long IS10 : 1;
			unsigned long IS9 : 1;
			unsigned long IS8 : 1;
			unsigned long IS7 : 1;
			unsigned long IS6 : 1;
			unsigned long IS5 : 1;
			unsigned long IS4 : 1;
			unsigned long IS3 : 1;
			unsigned long IS2 : 1;
			unsigned long IS1 : 1;
			unsigned long IS0 : 1;
#endif
	} BIT;
	} GRPBL2;
	char           wk17[52];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EN0 : 1;
			unsigned long EN1 : 1;
			unsigned long EN2 : 1;
			unsigned long EN3 : 1;
			unsigned long EN4 : 1;
			unsigned long EN5 : 1;
			unsigned long EN6 : 1;
			unsigned long EN7 : 1;
			unsigned long EN8 : 1;
			unsigned long EN9 : 1;
			unsigned long EN10 : 1;
			unsigned long EN11 : 1;
			unsigned long EN12 : 1;
			unsigned long EN13 : 1;
			unsigned long EN14 : 1;
			unsigned long EN15 : 1;
			unsigned long EN16 : 1;
			unsigned long EN17 : 1;
			unsigned long EN18 : 1;
			unsigned long EN19 : 1;
			unsigned long EN20 : 1;
			unsigned long EN21 : 1;
			unsigned long EN22 : 1;
			unsigned long EN23 : 1;
			unsigned long EN24 : 1;
			unsigned long EN25 : 1;
			unsigned long EN26 : 1;
			unsigned long EN27 : 1;
			unsigned long EN28 : 1;
			unsigned long EN29 : 1;
			unsigned long EN30 : 1;
			unsigned long EN31 : 1;
#else
			unsigned long EN31 : 1;
			unsigned long EN30 : 1;
			unsigned long EN29 : 1;
			unsigned long EN28 : 1;
			unsigned long EN27 : 1;
			unsigned long EN26 : 1;
			unsigned long EN25 : 1;
			unsigned long EN24 : 1;
			unsigned long EN23 : 1;
			unsigned long EN22 : 1;
			unsigned long EN21 : 1;
			unsigned long EN20 : 1;
			unsigned long EN19 : 1;
			unsigned long EN18 : 1;
			unsigned long EN17 : 1;
			unsigned long EN16 : 1;
			unsigned long EN15 : 1;
			unsigned long EN14 : 1;
			unsigned long EN13 : 1;
			unsigned long EN12 : 1;
			unsigned long EN11 : 1;
			unsigned long EN10 : 1;
			unsigned long EN9 : 1;
			unsigned long EN8 : 1;
			unsigned long EN7 : 1;
			unsigned long EN6 : 1;
			unsigned long EN5 : 1;
			unsigned long EN4 : 1;
			unsigned long EN3 : 1;
			unsigned long EN2 : 1;
			unsigned long EN1 : 1;
			unsigned long EN0 : 1;
#endif
	} BIT;
	} GENBL0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EN0 : 1;
			unsigned long EN1 : 1;
			unsigned long EN2 : 1;
			unsigned long EN3 : 1;
			unsigned long EN4 : 1;
			unsigned long EN5 : 1;
			unsigned long EN6 : 1;
			unsigned long EN7 : 1;
			unsigned long EN8 : 1;
			unsigned long EN9 : 1;
			unsigned long EN10 : 1;
			unsigned long EN11 : 1;
			unsigned long EN12 : 1;
			unsigned long EN13 : 1;
			unsigned long EN14 : 1;
			unsigned long EN15 : 1;
			unsigned long EN16 : 1;
			unsigned long EN17 : 1;
			unsigned long EN18 : 1;
			unsigned long EN19 : 1;
			unsigned long EN20 : 1;
			unsigned long EN21 : 1;
			unsigned long EN22 : 1;
			unsigned long EN23 : 1;
			unsigned long EN24 : 1;
			unsigned long EN25 : 1;
			unsigned long EN26 : 1;
			unsigned long EN27 : 1;
			unsigned long EN28 : 1;
			unsigned long EN29 : 1;
			unsigned long EN30 : 1;
			unsigned long EN31 : 1;
#else
			unsigned long EN31 : 1;
			unsigned long EN30 : 1;
			unsigned long EN29 : 1;
			unsigned long EN28 : 1;
			unsigned long EN27 : 1;
			unsigned long EN26 : 1;
			unsigned long EN25 : 1;
			unsigned long EN24 : 1;
			unsigned long EN23 : 1;
			unsigned long EN22 : 1;
			unsigned long EN21 : 1;
			unsigned long EN20 : 1;
			unsigned long EN19 : 1;
			unsigned long EN18 : 1;
			unsigned long EN17 : 1;
			unsigned long EN16 : 1;
			unsigned long EN15 : 1;
			unsigned long EN14 : 1;
			unsigned long EN13 : 1;
			unsigned long EN12 : 1;
			unsigned long EN11 : 1;
			unsigned long EN10 : 1;
			unsigned long EN9 : 1;
			unsigned long EN8 : 1;
			unsigned long EN7 : 1;
			unsigned long EN6 : 1;
			unsigned long EN5 : 1;
			unsigned long EN4 : 1;
			unsigned long EN3 : 1;
			unsigned long EN2 : 1;
			unsigned long EN1 : 1;
			unsigned long EN0 : 1;
#endif
	} BIT;
	} GENBL1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EN0 : 1;
			unsigned long EN1 : 1;
			unsigned long EN2 : 1;
			unsigned long EN3 : 1;
			unsigned long EN4 : 1;
			unsigned long EN5 : 1;
			unsigned long EN6 : 1;
			unsigned long EN7 : 1;
			unsigned long EN8 : 1;
			unsigned long EN9 : 1;
			unsigned long EN10 : 1;
			unsigned long EN11 : 1;
			unsigned long EN12 : 1;
			unsigned long EN13 : 1;
			unsigned long EN14 : 1;
			unsigned long EN15 : 1;
			unsigned long EN16 : 1;
			unsigned long EN17 : 1;
			unsigned long EN18 : 1;
			unsigned long EN19 : 1;
			unsigned long EN20 : 1;
			unsigned long EN21 : 1;
			unsigned long EN22 : 1;
			unsigned long EN23 : 1;
			unsigned long EN24 : 1;
			unsigned long EN25 : 1;
			unsigned long EN26 : 1;
			unsigned long EN27 : 1;
			unsigned long EN28 : 1;
			unsigned long EN29 : 1;
			unsigned long EN30 : 1;
			unsigned long EN31 : 1;
#else
			unsigned long EN31 : 1;
			unsigned long EN30 : 1;
			unsigned long EN29 : 1;
			unsigned long EN28 : 1;
			unsigned long EN27 : 1;
			unsigned long EN26 : 1;
			unsigned long EN25 : 1;
			unsigned long EN24 : 1;
			unsigned long EN23 : 1;
			unsigned long EN22 : 1;
			unsigned long EN21 : 1;
			unsigned long EN20 : 1;
			unsigned long EN19 : 1;
			unsigned long EN18 : 1;
			unsigned long EN17 : 1;
			unsigned long EN16 : 1;
			unsigned long EN15 : 1;
			unsigned long EN14 : 1;
			unsigned long EN13 : 1;
			unsigned long EN12 : 1;
			unsigned long EN11 : 1;
			unsigned long EN10 : 1;
			unsigned long EN9 : 1;
			unsigned long EN8 : 1;
			unsigned long EN7 : 1;
			unsigned long EN6 : 1;
			unsigned long EN5 : 1;
			unsigned long EN4 : 1;
			unsigned long EN3 : 1;
			unsigned long EN2 : 1;
			unsigned long EN1 : 1;
			unsigned long EN0 : 1;
#endif
	} BIT;
	} GENBL2;
	char           wk18[436];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IS0 : 1;
			unsigned long IS1 : 1;
			unsigned long IS2 : 1;
			unsigned long IS3 : 1;
			unsigned long IS4 : 1;
			unsigned long IS5 : 1;
			unsigned long IS6 : 1;
			unsigned long IS7 : 1;
			unsigned long IS8 : 1;
			unsigned long IS9 : 1;
			unsigned long IS10 : 1;
			unsigned long IS11 : 1;
			unsigned long IS12 : 1;
			unsigned long IS13 : 1;
			unsigned long IS14 : 1;
			unsigned long IS15 : 1;
			unsigned long IS16 : 1;
			unsigned long IS17 : 1;
			unsigned long IS18 : 1;
			unsigned long IS19 : 1;
			unsigned long IS20 : 1;
			unsigned long IS21 : 1;
			unsigned long IS22 : 1;
			unsigned long IS23 : 1;
			unsigned long IS24 : 1;
			unsigned long IS25 : 1;
			unsigned long IS26 : 1;
			unsigned long IS27 : 1;
			unsigned long IS28 : 1;
			unsigned long IS29 : 1;
			unsigned long IS30 : 1;
			unsigned long IS31 : 1;
#else
			unsigned long IS31 : 1;
			unsigned long IS30 : 1;
			unsigned long IS29 : 1;
			unsigned long IS28 : 1;
			unsigned long IS27 : 1;
			unsigned long IS26 : 1;
			unsigned long IS25 : 1;
			unsigned long IS24 : 1;
			unsigned long IS23 : 1;
			unsigned long IS22 : 1;
			unsigned long IS21 : 1;
			unsigned long IS20 : 1;
			unsigned long IS19 : 1;
			unsigned long IS18 : 1;
			unsigned long IS17 : 1;
			unsigned long IS16 : 1;
			unsigned long IS15 : 1;
			unsigned long IS14 : 1;
			unsigned long IS13 : 1;
			unsigned long IS12 : 1;
			unsigned long IS11 : 1;
			unsigned long IS10 : 1;
			unsigned long IS9 : 1;
			unsigned long IS8 : 1;
			unsigned long IS7 : 1;
			unsigned long IS6 : 1;
			unsigned long IS5 : 1;
			unsigned long IS4 : 1;
			unsigned long IS3 : 1;
			unsigned long IS2 : 1;
			unsigned long IS1 : 1;
			unsigned long IS0 : 1;
#endif
	} BIT;
	} GRPAL0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IS0 : 1;
			unsigned long IS1 : 1;
			unsigned long IS2 : 1;
			unsigned long IS3 : 1;
			unsigned long IS4 : 1;
			unsigned long IS5 : 1;
			unsigned long IS6 : 1;
			unsigned long IS7 : 1;
			unsigned long IS8 : 1;
			unsigned long IS9 : 1;
			unsigned long IS10 : 1;
			unsigned long IS11 : 1;
			unsigned long IS12 : 1;
			unsigned long IS13 : 1;
			unsigned long IS14 : 1;
			unsigned long IS15 : 1;
			unsigned long IS16 : 1;
			unsigned long IS17 : 1;
			unsigned long IS18 : 1;
			unsigned long IS19 : 1;
			unsigned long IS20 : 1;
			unsigned long IS21 : 1;
			unsigned long IS22 : 1;
			unsigned long IS23 : 1;
			unsigned long IS24 : 1;
			unsigned long IS25 : 1;
			unsigned long IS26 : 1;
			unsigned long IS27 : 1;
			unsigned long IS28 : 1;
			unsigned long IS29 : 1;
			unsigned long IS30 : 1;
			unsigned long IS31 : 1;
#else
			unsigned long IS31 : 1;
			unsigned long IS30 : 1;
			unsigned long IS29 : 1;
			unsigned long IS28 : 1;
			unsigned long IS27 : 1;
			unsigned long IS26 : 1;
			unsigned long IS25 : 1;
			unsigned long IS24 : 1;
			unsigned long IS23 : 1;
			unsigned long IS22 : 1;
			unsigned long IS21 : 1;
			unsigned long IS20 : 1;
			unsigned long IS19 : 1;
			unsigned long IS18 : 1;
			unsigned long IS17 : 1;
			unsigned long IS16 : 1;
			unsigned long IS15 : 1;
			unsigned long IS14 : 1;
			unsigned long IS13 : 1;
			unsigned long IS12 : 1;
			unsigned long IS11 : 1;
			unsigned long IS10 : 1;
			unsigned long IS9 : 1;
			unsigned long IS8 : 1;
			unsigned long IS7 : 1;
			unsigned long IS6 : 1;
			unsigned long IS5 : 1;
			unsigned long IS4 : 1;
			unsigned long IS3 : 1;
			unsigned long IS2 : 1;
			unsigned long IS1 : 1;
			unsigned long IS0 : 1;
#endif
	} BIT;
	} GRPAL1;
	char           wk19[56];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EN0 : 1;
			unsigned long EN1 : 1;
			unsigned long EN2 : 1;
			unsigned long EN3 : 1;
			unsigned long EN4 : 1;
			unsigned long EN5 : 1;
			unsigned long EN6 : 1;
			unsigned long EN7 : 1;
			unsigned long EN8 : 1;
			unsigned long EN9 : 1;
			unsigned long EN10 : 1;
			unsigned long EN11 : 1;
			unsigned long EN12 : 1;
			unsigned long EN13 : 1;
			unsigned long EN14 : 1;
			unsigned long EN15 : 1;
			unsigned long EN16 : 1;
			unsigned long EN17 : 1;
			unsigned long EN18 : 1;
			unsigned long EN19 : 1;
			unsigned long EN20 : 1;
			unsigned long EN21 : 1;
			unsigned long EN22 : 1;
			unsigned long EN23 : 1;
			unsigned long EN24 : 1;
			unsigned long EN25 : 1;
			unsigned long EN26 : 1;
			unsigned long EN27 : 1;
			unsigned long EN28 : 1;
			unsigned long EN29 : 1;
			unsigned long EN30 : 1;
			unsigned long EN31 : 1;
#else
			unsigned long EN31 : 1;
			unsigned long EN30 : 1;
			unsigned long EN29 : 1;
			unsigned long EN28 : 1;
			unsigned long EN27 : 1;
			unsigned long EN26 : 1;
			unsigned long EN25 : 1;
			unsigned long EN24 : 1;
			unsigned long EN23 : 1;
			unsigned long EN22 : 1;
			unsigned long EN21 : 1;
			unsigned long EN20 : 1;
			unsigned long EN19 : 1;
			unsigned long EN18 : 1;
			unsigned long EN17 : 1;
			unsigned long EN16 : 1;
			unsigned long EN15 : 1;
			unsigned long EN14 : 1;
			unsigned long EN13 : 1;
			unsigned long EN12 : 1;
			unsigned long EN11 : 1;
			unsigned long EN10 : 1;
			unsigned long EN9 : 1;
			unsigned long EN8 : 1;
			unsigned long EN7 : 1;
			unsigned long EN6 : 1;
			unsigned long EN5 : 1;
			unsigned long EN4 : 1;
			unsigned long EN3 : 1;
			unsigned long EN2 : 1;
			unsigned long EN1 : 1;
			unsigned long EN0 : 1;
#endif
	} BIT;
	} GENAL0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long EN0 : 1;
			unsigned long EN1 : 1;
			unsigned long EN2 : 1;
			unsigned long EN3 : 1;
			unsigned long EN4 : 1;
			unsigned long EN5 : 1;
			unsigned long EN6 : 1;
			unsigned long EN7 : 1;
			unsigned long EN8 : 1;
			unsigned long EN9 : 1;
			unsigned long EN10 : 1;
			unsigned long EN11 : 1;
			unsigned long EN12 : 1;
			unsigned long EN13 : 1;
			unsigned long EN14 : 1;
			unsigned long EN15 : 1;
			unsigned long EN16 : 1;
			unsigned long EN17 : 1;
			unsigned long EN18 : 1;
			unsigned long EN19 : 1;
			unsigned long EN20 : 1;
			unsigned long EN21 : 1;
			unsigned long EN22 : 1;
			unsigned long EN23 : 1;
			unsigned long EN24 : 1;
			unsigned long EN25 : 1;
			unsigned long EN26 : 1;
			unsigned long EN27 : 1;
			unsigned long EN28 : 1;
			unsigned long EN29 : 1;
			unsigned long EN30 : 1;
			unsigned long EN31 : 1;
#else
			unsigned long EN31 : 1;
			unsigned long EN30 : 1;
			unsigned long EN29 : 1;
			unsigned long EN28 : 1;
			unsigned long EN27 : 1;
			unsigned long EN26 : 1;
			unsigned long EN25 : 1;
			unsigned long EN24 : 1;
			unsigned long EN23 : 1;
			unsigned long EN22 : 1;
			unsigned long EN21 : 1;
			unsigned long EN20 : 1;
			unsigned long EN19 : 1;
			unsigned long EN18 : 1;
			unsigned long EN17 : 1;
			unsigned long EN16 : 1;
			unsigned long EN15 : 1;
			unsigned long EN14 : 1;
			unsigned long EN13 : 1;
			unsigned long EN12 : 1;
			unsigned long EN11 : 1;
			unsigned long EN10 : 1;
			unsigned long EN9 : 1;
			unsigned long EN8 : 1;
			unsigned long EN7 : 1;
			unsigned long EN6 : 1;
			unsigned long EN5 : 1;
			unsigned long EN4 : 1;
			unsigned long EN3 : 1;
			unsigned long EN2 : 1;
			unsigned long EN1 : 1;
			unsigned long EN0 : 1;
#endif
	} BIT;
	} GENAL1;
	char           wk20[136];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR4;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR5;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR6;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR7;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR8;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR9;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIARA;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIARB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIARC;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIARD;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIARE;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIARF;
	char           wk21[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR12;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR13;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PIR0 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR7 : 1;
#else
			unsigned char PIR7 : 1;
			unsigned char PIR6 : 1;
			unsigned char PIR5 : 1;
			unsigned char PIR4 : 1;
			unsigned char PIR3 : 1;
			unsigned char PIR2 : 1;
			unsigned char PIR1 : 1;
			unsigned char PIR0 : 1;
#endif
	} BIT;
	} PIAR14;
	char           wk22[187];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR208;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR209;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR210;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR211;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR212;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR213;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR214;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR215;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR216;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR217;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR218;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR219;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR220;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR221;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR222;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR223;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR224;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR225;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR226;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR227;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR228;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR229;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR230;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR231;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR232;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR233;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR234;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR235;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR236;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR237;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR238;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR239;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR240;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR241;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR242;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR243;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR244;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR245;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR246;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR247;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR248;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR249;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR250;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR251;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR252;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR253;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR254;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLI : 8;
#else
			unsigned char SLI : 8;
#endif
	} BIT;
	} SLIAR255;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char WPRC : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char WPRC : 1;
#endif
	} BIT;
	} SLIPRCR;
} st_icu_t;

typedef struct st_iwdt {
	unsigned char  IWDTRR;
	char           wk0[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short TOPS : 2;
			unsigned short  : 2;
			unsigned short CKS : 4;
			unsigned short RPES : 2;
			unsigned short  : 2;
			unsigned short RPSS : 2;
			unsigned short  : 2;
#else
			unsigned short  : 2;
			unsigned short RPSS : 2;
			unsigned short  : 2;
			unsigned short RPES : 2;
			unsigned short CKS : 4;
			unsigned short  : 2;
			unsigned short TOPS : 2;
#endif
	} BIT;
	} IWDTCR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CNTVAL : 14;
			unsigned short UNDFF : 1;
			unsigned short REFEF : 1;
#else
			unsigned short REFEF : 1;
			unsigned short UNDFF : 1;
			unsigned short CNTVAL : 14;
#endif
	} BIT;
	} IWDTSR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char RSTIRQS : 1;
#else
			unsigned char RSTIRQS : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} IWDTRCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char SLCSTP : 1;
#else
			unsigned char SLCSTP : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} IWDTCSTPR;
} st_iwdt_t;

typedef struct st_mpc {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char PFSWE : 1;
			unsigned char B0WI : 1;
#else
			unsigned char B0WI : 1;
			unsigned char PFSWE : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} PWPR;
	char           wk0[32];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P00PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P01PFS;
	char           wk1[6];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P10PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P11PFS;
	char           wk2[6];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P20PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P21PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P22PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P23PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P24PFS;
	char           wk3[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P27PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P30PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P31PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P32PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P33PFS;
	char           wk4[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P36PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P37PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P40PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P41PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P42PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P43PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P44PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P45PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P46PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P47PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P50PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} P51PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P52PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P53PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P54PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P55PFS;
	char           wk5[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P60PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P61PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P62PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P63PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P64PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char ISEL : 1;
			unsigned char ASEL : 1;
#else
			unsigned char ASEL : 1;
			unsigned char ISEL : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} P65PFS;
	char           wk6[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P70PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P71PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P72PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P73PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P74PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P75PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P76PFS;
	char           wk7[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P80PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P81PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P82PFS;
	char           wk8[5];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P90PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P91PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P92PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P93PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P94PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P95PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} P96PFS;
	char           wk9[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PA0PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PA1PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PA2PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PA3PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PA4PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PA5PFS;
	char           wk10[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB0PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB1PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB2PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB3PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB4PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB5PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB6PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PB7PFS;
	char           wk11[8];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD0PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD1PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD2PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD3PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD4PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD5PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD6PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PD7PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PE0PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PE1PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PE2PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PE3PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PE4PFS;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PE5PFS;
	char           wk12[65];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PSEL : 6;
			unsigned char ISEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char ISEL : 1;
			unsigned char PSEL : 6;
#endif
	} BIT;
	} PN7PFS;
} st_mpc_t;

typedef struct st_mpu {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE3;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE3;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE4;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE4;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE5;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE5;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE6;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE6;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long RSPN : 28;
#else
			unsigned long RSPN : 28;
			unsigned long  : 4;
#endif
	} BIT;
	} RSPAGE7;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long V : 1;
			unsigned long UAC : 3;
			unsigned long REPN : 28;
#else
			unsigned long REPN : 28;
			unsigned long UAC : 3;
			unsigned long V : 1;
#endif
	} BIT;
	} REPAGE7;
	char           wk0[192];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MPEN : 1;
			unsigned long  : 31;
#else
			unsigned long  : 31;
			unsigned long MPEN : 1;
#endif
	} BIT;
	} MPEN;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 1;
			unsigned long UBAC : 3;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long UBAC : 3;
			unsigned long  : 1;
#endif
	} BIT;
	} MPBAC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CLR : 1;
			unsigned long  : 31;
#else
			unsigned long  : 31;
			unsigned long CLR : 1;
#endif
	} BIT;
	} MPECLR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IMPER : 1;
			unsigned long DMPER : 1;
			unsigned long DRW : 1;
			unsigned long  : 29;
#else
			unsigned long  : 29;
			unsigned long DRW : 1;
			unsigned long DMPER : 1;
			unsigned long IMPER : 1;
#endif
	} BIT;
	} MPESTS;
	char           wk1[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DEA : 32;
#else
			unsigned long DEA : 32;
#endif
	} BIT;
	} MPDEA;
	char           wk2[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SA : 32;
#else
			unsigned long SA : 32;
#endif
	} BIT;
	} MPSA;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short S : 1;
			unsigned short  : 15;
#else
			unsigned short  : 15;
			unsigned short S : 1;
#endif
	} BIT;
	} MPOPS;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short INV : 1;
			unsigned short  : 15;
#else
			unsigned short  : 15;
			unsigned short INV : 1;
#endif
	} BIT;
	} MPOPI;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 1;
			unsigned long UHACI : 3;
			unsigned long  : 12;
			unsigned long HITI : 8;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long HITI : 8;
			unsigned long  : 12;
			unsigned long UHACI : 3;
			unsigned long  : 1;
#endif
	} BIT;
	} MHITI;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 1;
			unsigned long UHACD : 3;
			unsigned long  : 12;
			unsigned long HITD : 8;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long HITD : 8;
			unsigned long  : 12;
			unsigned long UHACD : 3;
			unsigned long  : 1;
#endif
	} BIT;
	} MHITD;
} st_mpu_t;

typedef struct st_mtu {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OE3B : 1;
			unsigned char OE4A : 1;
			unsigned char OE4B : 1;
			unsigned char OE3D : 1;
			unsigned char OE4C : 1;
			unsigned char OE4D : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char OE4D : 1;
			unsigned char OE4C : 1;
			unsigned char OE3D : 1;
			unsigned char OE4B : 1;
			unsigned char OE4A : 1;
			unsigned char OE3B : 1;
#endif
	} BIT;
	} TOERA;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char UF : 1;
			unsigned char VF : 1;
			unsigned char WF : 1;
			unsigned char FB : 1;
			unsigned char P : 1;
			unsigned char N : 1;
			unsigned char BDC : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char BDC : 1;
			unsigned char N : 1;
			unsigned char P : 1;
			unsigned char FB : 1;
			unsigned char WF : 1;
			unsigned char VF : 1;
			unsigned char UF : 1;
#endif
	} BIT;
	} TGCRA;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OLSP : 1;
			unsigned char OLSN : 1;
			unsigned char TOCS : 1;
			unsigned char TOCL : 1;
			unsigned char  : 2;
			unsigned char PSYE : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PSYE : 1;
			unsigned char  : 2;
			unsigned char TOCL : 1;
			unsigned char TOCS : 1;
			unsigned char OLSN : 1;
			unsigned char OLSP : 1;
#endif
	} BIT;
	} TOCR1A;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OLS1P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS3N : 1;
			unsigned char BF : 2;
#else
			unsigned char BF : 2;
			unsigned char OLS3N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS1P : 1;
#endif
	} BIT;
	} TOCR2A;
	char           wk1[4];
	unsigned short TCDRA;
	unsigned short TDDRA;
	char           wk2[8];
	unsigned short TCNTSA;
	unsigned short TCBRA;
	char           wk3[12];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char T4VCOR : 3;
			unsigned char T4VEN : 1;
			unsigned char T3ACOR : 3;
			unsigned char T3AEN : 1;
#else
			unsigned char T3AEN : 1;
			unsigned char T3ACOR : 3;
			unsigned char T4VEN : 1;
			unsigned char T4VCOR : 3;
#endif
	} BIT;
	} TITCR1A;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char T4VCNT : 3;
			unsigned char  : 1;
			unsigned char T3ACNT : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char T3ACNT : 3;
			unsigned char  : 1;
			unsigned char T4VCNT : 3;
#endif
	} BIT;
	} TITCNT1A;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BTE : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char BTE : 2;
#endif
	} BIT;
	} TBTERA;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TDER : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TDER : 1;
#endif
	} BIT;
	} TDERA;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OLS1P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS3N : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char OLS3N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS1P : 1;
#endif
	} BIT;
	} TOLBRA;
	char           wk6[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TITM : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TITM : 1;
#endif
	} BIT;
	} TITMRA;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRG4COR : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TRG4COR : 3;
#endif
	} BIT;
	} TITCR2A;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRG4CNT : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TRG4CNT : 3;
#endif
	} BIT;
	} TITCNT2A;
	char           wk7[35];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char WRE : 1;
			unsigned char SCC : 1;
			unsigned char  : 5;
			unsigned char CCE : 1;
#else
			unsigned char CCE : 1;
			unsigned char  : 5;
			unsigned char SCC : 1;
			unsigned char WRE : 1;
#endif
	} BIT;
	} TWCRA;
	char           wk8[15];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DRS : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DRS : 1;
#endif
	} BIT;
	} TMDR2A;
	char           wk9[15];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CST0 : 1;
			unsigned char CST1 : 1;
			unsigned char CST2 : 1;
			unsigned char  : 1;
			unsigned char CST9 : 1;
			unsigned char  : 1;
			unsigned char CST3 : 1;
			unsigned char CST4 : 1;
#else
			unsigned char CST4 : 1;
			unsigned char CST3 : 1;
			unsigned char  : 1;
			unsigned char CST9 : 1;
			unsigned char  : 1;
			unsigned char CST2 : 1;
			unsigned char CST1 : 1;
			unsigned char CST0 : 1;
#endif
	} BIT;
	} TSTRA;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SYNC0 : 1;
			unsigned char SYNC1 : 1;
			unsigned char SYNC2 : 1;
			unsigned char SYNC9 : 1;
			unsigned char  : 2;
			unsigned char SYNC3 : 1;
			unsigned char SYNC4 : 1;
#else
			unsigned char SYNC4 : 1;
			unsigned char SYNC3 : 1;
			unsigned char  : 2;
			unsigned char SYNC9 : 1;
			unsigned char SYNC2 : 1;
			unsigned char SYNC1 : 1;
			unsigned char SYNC0 : 1;
#endif
	} BIT;
	} TSYRA;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SCH7 : 1;
			unsigned char SCH6 : 1;
			unsigned char SCH9 : 1;
			unsigned char SCH4 : 1;
			unsigned char SCH3 : 1;
			unsigned char SCH2 : 1;
			unsigned char SCH1 : 1;
			unsigned char SCH0 : 1;
#else
			unsigned char SCH0 : 1;
			unsigned char SCH1 : 1;
			unsigned char SCH2 : 1;
			unsigned char SCH3 : 1;
			unsigned char SCH4 : 1;
			unsigned char SCH9 : 1;
			unsigned char SCH6 : 1;
			unsigned char SCH7 : 1;
#endif
	} BIT;
	} TCSYSTR;
	char           wk10[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RWE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char RWE : 1;
#endif
	} BIT;
	} TRWERA;
	char           wk11[1925];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OE6B : 1;
			unsigned char OE7A : 1;
			unsigned char OE7B : 1;
			unsigned char OE6D : 1;
			unsigned char OE7C : 1;
			unsigned char OE7D : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char OE7D : 1;
			unsigned char OE7C : 1;
			unsigned char OE6D : 1;
			unsigned char OE7B : 1;
			unsigned char OE7A : 1;
			unsigned char OE6B : 1;
#endif
	} BIT;
	} TOERB;
	char           wk12[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char UF : 1;
			unsigned char VF : 1;
			unsigned char WF : 1;
			unsigned char FB : 1;
			unsigned char P : 1;
			unsigned char N : 1;
			unsigned char BDC : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char BDC : 1;
			unsigned char N : 1;
			unsigned char P : 1;
			unsigned char FB : 1;
			unsigned char WF : 1;
			unsigned char VF : 1;
			unsigned char UF : 1;
#endif
	} BIT;
	} TGCRB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OLSP : 1;
			unsigned char OLSN : 1;
			unsigned char TOCS : 1;
			unsigned char TOCL : 1;
			unsigned char  : 2;
			unsigned char PSYE : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char PSYE : 1;
			unsigned char  : 2;
			unsigned char TOCL : 1;
			unsigned char TOCS : 1;
			unsigned char OLSN : 1;
			unsigned char OLSP : 1;
#endif
	} BIT;
	} TOCR1B;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OLS1P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS3N : 1;
			unsigned char BF : 2;
#else
			unsigned char BF : 2;
			unsigned char OLS3N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS1P : 1;
#endif
	} BIT;
	} TOCR2B;
	char           wk13[4];
	unsigned short TCDRB;
	unsigned short TDDRB;
	char           wk14[8];
	unsigned short TCNTSB;
	unsigned short TCBRB;
	char           wk15[12];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char T7VCOR : 3;
			unsigned char T7VEN : 1;
			unsigned char T6ACOR : 3;
			unsigned char T6AEN : 1;
#else
			unsigned char T6AEN : 1;
			unsigned char T6ACOR : 3;
			unsigned char T7VEN : 1;
			unsigned char T7VCOR : 3;
#endif
	} BIT;
	} TITCR1B;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char T7VCNT : 3;
			unsigned char  : 1;
			unsigned char T6ACNT : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char T6ACNT : 3;
			unsigned char  : 1;
			unsigned char T7VCNT : 3;
#endif
	} BIT;
	} TITCNT1B;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BTE : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char BTE : 2;
#endif
	} BIT;
	} TBTERB;
	char           wk16[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TDER : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TDER : 1;
#endif
	} BIT;
	} TDERB;
	char           wk17[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OLS1P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS3N : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char OLS3N : 1;
			unsigned char OLS3P : 1;
			unsigned char OLS2N : 1;
			unsigned char OLS2P : 1;
			unsigned char OLS1N : 1;
			unsigned char OLS1P : 1;
#endif
	} BIT;
	} TOLBRB;
	char           wk18[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TITM : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TITM : 1;
#endif
	} BIT;
	} TITMRB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRG7COR : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TRG7COR : 3;
#endif
	} BIT;
	} TITCR2B;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRG7CNT : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TRG7CNT : 3;
#endif
	} BIT;
	} TITCNT2B;
	char           wk19[35];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char WRE : 1;
			unsigned char SCC : 1;
			unsigned char  : 5;
			unsigned char CCE : 1;
#else
			unsigned char CCE : 1;
			unsigned char  : 5;
			unsigned char SCC : 1;
			unsigned char WRE : 1;
#endif
	} BIT;
	} TWCRB;
	char           wk20[15];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DRS : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char DRS : 1;
#endif
	} BIT;
	} TMDR2B;
	char           wk21[15];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char CST6 : 1;
			unsigned char CST7 : 1;
#else
			unsigned char CST7 : 1;
			unsigned char CST6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} TSTRB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char SYNC6 : 1;
			unsigned char SYNC7 : 1;
#else
			unsigned char SYNC7 : 1;
			unsigned char SYNC6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} TSYRB;
	char           wk22[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RWE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char RWE : 1;
#endif
	} BIT;
	} TRWERB;
	char           wk23[683];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TADSTRS0 : 5;
			unsigned char  : 2;
			unsigned char TADSMEN0 : 1;
#else
			unsigned char TADSMEN0 : 1;
			unsigned char  : 2;
			unsigned char TADSTRS0 : 5;
#endif
	} BIT;
	} TADSTRGR0;
	char           wk24[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TADSTRS1 : 5;
			unsigned char  : 2;
			unsigned char TADSMEN1 : 1;
#else
			unsigned char TADSMEN1 : 1;
			unsigned char  : 2;
			unsigned char TADSTRS1 : 5;
#endif
	} BIT;
	} TADSTRGR1;
} st_mtu_t;

typedef struct st_mtu0 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR0;
	char           wk0[8];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCRC;
	char           wk1[102];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 3;
#else
			unsigned char CCLR : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char BFA : 1;
			unsigned char BFB : 1;
			unsigned char BFE : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char BFE : 1;
			unsigned char BFB : 1;
			unsigned char BFA : 1;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 4;
			unsigned char IOD : 4;
#else
			unsigned char IOD : 4;
			unsigned char IOC : 4;
#endif
	} BIT;
	} TIORL;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIED : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 2;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char  : 2;
			unsigned char TCIEV : 1;
			unsigned char TGIED : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	char           wk2[1];
	unsigned short TCNT;
	unsigned short TGRA;
	unsigned short TGRB;
	unsigned short TGRC;
	unsigned short TGRD;
	char           wk3[16];
	unsigned short TGRE;
	unsigned short TGRF;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEE : 1;
			unsigned char TGIEF : 1;
			unsigned char  : 5;
			unsigned char TTGE2 : 1;
#else
			unsigned char TTGE2 : 1;
			unsigned char  : 5;
			unsigned char TGIEF : 1;
			unsigned char TGIEE : 1;
#endif
	} BIT;
	} TIER2;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TTSA : 1;
			unsigned char TTSB : 1;
			unsigned char TTSE : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TTSE : 1;
			unsigned char TTSB : 1;
			unsigned char TTSA : 1;
#endif
	} BIT;
	} TBTM;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
} st_mtu0_t;

typedef struct st_mtu1 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char  : 2;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR1;
	char           wk1[238];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 2;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char CCLR : 2;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIOR;
	char           wk2[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char  : 2;
			unsigned char TCIEV : 1;
			unsigned char TCIEU : 1;
			unsigned char  : 1;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char  : 1;
			unsigned char TCIEU : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 2;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char TCFD : 1;
#else
			unsigned char TCFD : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} TSR;
	unsigned short TCNT;
	unsigned short TGRA;
	unsigned short TGRB;
	char           wk3[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char I1AE : 1;
			unsigned char I1BE : 1;
			unsigned char I2AE : 1;
			unsigned char I2BE : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char I2BE : 1;
			unsigned char I2AE : 1;
			unsigned char I1BE : 1;
			unsigned char I1AE : 1;
#endif
	} BIT;
	} TICCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LWA : 1;
			unsigned char PHCKSEL : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char PHCKSEL : 1;
			unsigned char LWA : 1;
#endif
	} BIT;
	} TMDR3;
	char           wk4[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char PCB : 2;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char PCB : 2;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
	char           wk5[11];
	unsigned long  TCNTLW;
	unsigned long  TGRALW;
	unsigned long  TGRBLW;
} st_mtu1_t;

typedef struct st_mtu2 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char  : 2;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR2;
	char           wk0[365];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 2;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char CCLR : 2;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIOR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char  : 2;
			unsigned char TCIEV : 1;
			unsigned char TCIEU : 1;
			unsigned char  : 1;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char  : 1;
			unsigned char TCIEU : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 2;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char TCFD : 1;
#else
			unsigned char TCFD : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} TSR;
	unsigned short TCNT;
	unsigned short TGRA;
	unsigned short TGRB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char PCB : 2;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char PCB : 2;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
} st_mtu2_t;

typedef struct st_mtu3 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 3;
#else
			unsigned char CCLR : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char BFA : 1;
			unsigned char BFB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char BFB : 1;
			unsigned char BFA : 1;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 4;
			unsigned char IOD : 4;
#else
			unsigned char IOD : 4;
			unsigned char IOC : 4;
#endif
	} BIT;
	} TIORL;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIED : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 2;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char  : 2;
			unsigned char TCIEV : 1;
			unsigned char TGIED : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	char           wk3[7];
	unsigned short TCNT;
	char           wk4[6];
	unsigned short TGRA;
	unsigned short TGRB;
	char           wk5[8];
	unsigned short TGRC;
	unsigned short TGRD;
	char           wk6[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char TCFD : 1;
#else
			unsigned char TCFD : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} TSR;
	char           wk7[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TTSA : 1;
			unsigned char TTSB : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TTSB : 1;
			unsigned char TTSA : 1;
#endif
	} BIT;
	} TBTM;
	char           wk8[19];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
	char           wk9[37];
	unsigned short TGRE;
	char           wk10[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR3;
} st_mtu3_t;

typedef struct st_mtu4 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 3;
#else
			unsigned char CCLR : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char BFA : 1;
			unsigned char BFB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char BFB : 1;
			unsigned char BFA : 1;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 4;
			unsigned char IOD : 4;
#else
			unsigned char IOD : 4;
			unsigned char IOC : 4;
#endif
	} BIT;
	} TIORL;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIED : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 1;
			unsigned char TTGE2 : 1;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char TTGE2 : 1;
			unsigned char  : 1;
			unsigned char TCIEV : 1;
			unsigned char TGIED : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	char           wk4[8];
	unsigned short TCNT;
	char           wk5[8];
	unsigned short TGRA;
	unsigned short TGRB;
	char           wk6[8];
	unsigned short TGRC;
	unsigned short TGRD;
	char           wk7[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char TCFD : 1;
#else
			unsigned char TCFD : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} TSR;
	char           wk8[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TTSA : 1;
			unsigned char TTSB : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TTSB : 1;
			unsigned char TTSA : 1;
#endif
	} BIT;
	} TBTM;
	char           wk9[6];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ITB4VE : 1;
			unsigned short ITB3AE : 1;
			unsigned short ITA4VE : 1;
			unsigned short ITA3AE : 1;
			unsigned short DT4BE : 1;
			unsigned short UT4BE : 1;
			unsigned short DT4AE : 1;
			unsigned short UT4AE : 1;
			unsigned short  : 6;
			unsigned short BF : 2;
#else
			unsigned short BF : 2;
			unsigned short  : 6;
			unsigned short UT4AE : 1;
			unsigned short DT4AE : 1;
			unsigned short UT4BE : 1;
			unsigned short DT4BE : 1;
			unsigned short ITA3AE : 1;
			unsigned short ITA4VE : 1;
			unsigned short ITB3AE : 1;
			unsigned short ITB4VE : 1;
#endif
	} BIT;
	} TADCR;
	char           wk10[2];
	unsigned short TADCORA;
	unsigned short TADCORB;
	unsigned short TADCOBRA;
	unsigned short TADCOBRB;
	char           wk11[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
	char           wk12[38];
	unsigned short TGRE;
	unsigned short TGRF;
	char           wk13[28];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR4;
} st_mtu4_t;

typedef struct st_mtu5 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFUEN : 1;
			unsigned char NFVEN : 1;
			unsigned char NFWEN : 1;
			unsigned char  : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char  : 1;
			unsigned char NFWEN : 1;
			unsigned char NFVEN : 1;
			unsigned char NFUEN : 1;
#endif
	} BIT;
	} NFCR5;
	char           wk1[490];
	unsigned short TCNTU;
	unsigned short TGRU;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TPSC : 2;
#endif
	} BIT;
	} TCRU;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char CKEG : 2;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2U;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char IOC : 5;
#endif
	} BIT;
	} TIORU;
	char           wk2[9];
	unsigned short TCNTV;
	unsigned short TGRV;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TPSC : 2;
#endif
	} BIT;
	} TCRV;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char CKEG : 2;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2V;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char IOC : 5;
#endif
	} BIT;
	} TIORV;
	char           wk3[9];
	unsigned short TCNTW;
	unsigned short TGRW;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TPSC : 2;
#endif
	} BIT;
	} TCRW;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char CKEG : 2;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2W;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char IOC : 5;
#endif
	} BIT;
	} TIORW;
	char           wk4[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIE5W : 1;
			unsigned char TGIE5V : 1;
			unsigned char TGIE5U : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TGIE5U : 1;
			unsigned char TGIE5V : 1;
			unsigned char TGIE5W : 1;
#endif
	} BIT;
	} TIER;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CSTW5 : 1;
			unsigned char CSTV5 : 1;
			unsigned char CSTU5 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char CSTU5 : 1;
			unsigned char CSTV5 : 1;
			unsigned char CSTW5 : 1;
#endif
	} BIT;
	} TSTR;
	char           wk6[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPCLR5W : 1;
			unsigned char CMPCLR5V : 1;
			unsigned char CMPCLR5U : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char CMPCLR5U : 1;
			unsigned char CMPCLR5V : 1;
			unsigned char CMPCLR5W : 1;
#endif
	} BIT;
	} TCNTCMPCLR;
} st_mtu5_t;

typedef struct st_mtu6 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 3;
#else
			unsigned char CCLR : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char BFA : 1;
			unsigned char BFB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char BFB : 1;
			unsigned char BFA : 1;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 4;
			unsigned char IOD : 4;
#else
			unsigned char IOD : 4;
			unsigned char IOC : 4;
#endif
	} BIT;
	} TIORL;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIED : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 2;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char  : 2;
			unsigned char TCIEV : 1;
			unsigned char TGIED : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	char           wk3[7];
	unsigned short TCNT;
	char           wk4[6];
	unsigned short TGRA;
	unsigned short TGRB;
	char           wk5[8];
	unsigned short TGRC;
	unsigned short TGRD;
	char           wk6[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char TCFD : 1;
#else
			unsigned char TCFD : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} TSR;
	char           wk7[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TTSA : 1;
			unsigned char TTSB : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TTSB : 1;
			unsigned char TTSA : 1;
#endif
	} BIT;
	} TBTM;
	char           wk8[19];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
	char           wk9[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CE2B : 1;
			unsigned char CE2A : 1;
			unsigned char CE1B : 1;
			unsigned char CE1A : 1;
			unsigned char CE0D : 1;
			unsigned char CE0C : 1;
			unsigned char CE0B : 1;
			unsigned char CE0A : 1;
#else
			unsigned char CE0A : 1;
			unsigned char CE0B : 1;
			unsigned char CE0C : 1;
			unsigned char CE0D : 1;
			unsigned char CE1A : 1;
			unsigned char CE1B : 1;
			unsigned char CE2A : 1;
			unsigned char CE2B : 1;
#endif
	} BIT;
	} TSYCR;
	char           wk10[33];
	unsigned short TGRE;
	char           wk11[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR6;
} st_mtu6_t;

typedef struct st_mtu7 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 3;
#else
			unsigned char CCLR : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char BFA : 1;
			unsigned char BFB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char BFB : 1;
			unsigned char BFA : 1;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 4;
			unsigned char IOD : 4;
#else
			unsigned char IOD : 4;
			unsigned char IOC : 4;
#endif
	} BIT;
	} TIORL;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIED : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 1;
			unsigned char TTGE2 : 1;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char TTGE2 : 1;
			unsigned char  : 1;
			unsigned char TCIEV : 1;
			unsigned char TGIED : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	char           wk4[8];
	unsigned short TCNT;
	char           wk5[8];
	unsigned short TGRA;
	unsigned short TGRB;
	char           wk6[8];
	unsigned short TGRC;
	unsigned short TGRD;
	char           wk7[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char TCFD : 1;
#else
			unsigned char TCFD : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} TSR;
	char           wk8[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TTSA : 1;
			unsigned char TTSB : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char TTSB : 1;
			unsigned char TTSA : 1;
#endif
	} BIT;
	} TBTM;
	char           wk9[6];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ITB7VE : 1;
			unsigned short ITB6AE : 1;
			unsigned short ITA7VE : 1;
			unsigned short ITA6AE : 1;
			unsigned short DT7BE : 1;
			unsigned short UT7BE : 1;
			unsigned short DT7AE : 1;
			unsigned short UT7AE : 1;
			unsigned short  : 6;
			unsigned short BF : 2;
#else
			unsigned short BF : 2;
			unsigned short  : 6;
			unsigned short UT7AE : 1;
			unsigned short DT7AE : 1;
			unsigned short UT7BE : 1;
			unsigned short DT7BE : 1;
			unsigned short ITA6AE : 1;
			unsigned short ITA7VE : 1;
			unsigned short ITB6AE : 1;
			unsigned short ITB7VE : 1;
#endif
	} BIT;
	} TADCR;
	char           wk10[2];
	unsigned short TADCORA;
	unsigned short TADCORB;
	unsigned short TADCOBRA;
	unsigned short TADCOBRB;
	char           wk11[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
	char           wk12[38];
	unsigned short TGRE;
	unsigned short TGRF;
	char           wk13[28];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR7;
} st_mtu7_t;

typedef struct st_mtu9 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFAEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFDEN : 1;
			unsigned char NFCS : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char NFCS : 2;
			unsigned char NFDEN : 1;
			unsigned char NFCEN : 1;
			unsigned char NFBEN : 1;
			unsigned char NFAEN : 1;
#endif
	} BIT;
	} NFCR9;
	char           wk0[745];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC : 3;
			unsigned char CKEG : 2;
			unsigned char CCLR : 3;
#else
			unsigned char CCLR : 3;
			unsigned char CKEG : 2;
			unsigned char TPSC : 3;
#endif
	} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MD : 4;
			unsigned char BFA : 1;
			unsigned char BFB : 1;
			unsigned char BFE : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char BFE : 1;
			unsigned char BFB : 1;
			unsigned char BFA : 1;
			unsigned char MD : 4;
#endif
	} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOA : 4;
			unsigned char IOB : 4;
#else
			unsigned char IOB : 4;
			unsigned char IOA : 4;
#endif
	} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IOC : 4;
			unsigned char IOD : 4;
#else
			unsigned char IOD : 4;
			unsigned char IOC : 4;
#endif
	} BIT;
	} TIORL;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEA : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIED : 1;
			unsigned char TCIEV : 1;
			unsigned char  : 2;
			unsigned char TTGE : 1;
#else
			unsigned char TTGE : 1;
			unsigned char  : 2;
			unsigned char TCIEV : 1;
			unsigned char TGIED : 1;
			unsigned char TGIEC : 1;
			unsigned char TGIEB : 1;
			unsigned char TGIEA : 1;
#endif
	} BIT;
	} TIER;
	char           wk1[1];
	unsigned short TCNT;
	unsigned short TGRA;
	unsigned short TGRB;
	unsigned short TGRC;
	unsigned short TGRD;
	char           wk2[16];
	unsigned short TGRE;
	unsigned short TGRF;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TGIEE : 1;
			unsigned char TGIEF : 1;
			unsigned char  : 5;
			unsigned char TTGE2 : 1;
#else
			unsigned char TTGE2 : 1;
			unsigned char  : 5;
			unsigned char TGIEF : 1;
			unsigned char TGIEE : 1;
#endif
	} BIT;
	} TIER2;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TTSA : 1;
			unsigned char TTSB : 1;
			unsigned char TTSE : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TTSE : 1;
			unsigned char TTSB : 1;
			unsigned char TTSA : 1;
#endif
	} BIT;
	} TBTM;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TPSC2 : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char TPSC2 : 3;
#endif
	} BIT;
	} TCR2;
} st_mtu9_t;

typedef struct st_ofsm {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 17;
			unsigned long OCDE : 1;
			unsigned long  : 6;
			unsigned long IDE : 1;
			unsigned long  : 2;
			unsigned long SPE : 1;
			unsigned long  : 1;
			unsigned long SEPR : 1;
			unsigned long WRPR : 1;
			unsigned long RDPR : 1;
#else
			unsigned long RDPR : 1;
			unsigned long WRPR : 1;
			unsigned long SEPR : 1;
			unsigned long  : 1;
			unsigned long SPE : 1;
			unsigned long  : 2;
			unsigned long IDE : 1;
			unsigned long  : 6;
			unsigned long OCDE : 1;
			unsigned long  : 17;
#endif
	} BIT;
	} SPCC;
	char           wk0[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 24;
			unsigned long TMEF : 3;
			unsigned long  : 1;
			unsigned long TMEFDB : 3;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long TMEFDB : 3;
			unsigned long  : 1;
			unsigned long TMEF : 3;
			unsigned long  : 24;
#endif
	} BIT;
	} TMEF;
	char           wk1[4];
	union {
		struct {
			unsigned long ID4:8;
			unsigned long ID3:8;
			unsigned long ID2:8;
			unsigned long ID1:8;
			unsigned long ID8:8;
			unsigned long ID7:8;
			unsigned long ID6:8;
			unsigned long ID5:8;
			unsigned long ID12:8;
			unsigned long ID11:8;
			unsigned long ID10:8;
			unsigned long ID9:8;
			unsigned long ID16:8;
			unsigned long ID15:8;
			unsigned long ID14:8;
			unsigned long ID13:8;
		} BIT;
	} OSIS;
	unsigned long  TMINF;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MDE : 3;
			unsigned long  : 1;
			unsigned long BANKMD : 3;
			unsigned long  : 25;
#else
			unsigned long  : 25;
			unsigned long BANKMD : 3;
			unsigned long  : 1;
			unsigned long MDE : 3;
#endif
	} BIT;
	} MDE;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 1;
			unsigned long IWDTSTRT : 1;
			unsigned long IWDTTOPS : 2;
			unsigned long IWDTCKS : 4;
			unsigned long IWDTRPES : 2;
			unsigned long IWDTRPSS : 2;
			unsigned long IWDTRSTIRQS : 1;
			unsigned long  : 1;
			unsigned long IWDTSLCSTP : 1;
			unsigned long  : 2;
			unsigned long WDTSTRT : 1;
			unsigned long WDTTOPS : 2;
			unsigned long WDTCKS : 4;
			unsigned long WDTRPES : 2;
			unsigned long WDTRPSS : 2;
			unsigned long WDTRSTIRQS : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long WDTRSTIRQS : 1;
			unsigned long WDTRPSS : 2;
			unsigned long WDTRPES : 2;
			unsigned long WDTCKS : 4;
			unsigned long WDTTOPS : 2;
			unsigned long WDTSTRT : 1;
			unsigned long  : 2;
			unsigned long IWDTSLCSTP : 1;
			unsigned long  : 1;
			unsigned long IWDTRSTIRQS : 1;
			unsigned long IWDTRPSS : 2;
			unsigned long IWDTRPES : 2;
			unsigned long IWDTCKS : 4;
			unsigned long IWDTTOPS : 2;
			unsigned long IWDTSTRT : 1;
			unsigned long  : 1;
#endif
	} BIT;
	} OFS0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long VDSEL : 2;
			unsigned long LVDAS : 1;
			unsigned long  : 5;
			unsigned long HOCOEN : 1;
			unsigned long  : 23;
#else
			unsigned long  : 23;
			unsigned long HOCOEN : 1;
			unsigned long  : 5;
			unsigned long LVDAS : 1;
			unsigned long VDSEL : 2;
#endif
	} BIT;
	} OFS1;
	char           wk2[32];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BANKSWP : 3;
			unsigned long  : 29;
#else
			unsigned long  : 29;
			unsigned long BANKSWP : 3;
#endif
	} BIT;
	} BANKSEL;
	char           wk3[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long FAWS : 13;
			unsigned long  : 2;
			unsigned long FSPR : 1;
			unsigned long FAWE : 13;
			unsigned long  : 2;
			unsigned long BTFLG : 1;
#else
			unsigned long BTFLG : 1;
			unsigned long  : 2;
			unsigned long FAWE : 13;
			unsigned long FSPR : 1;
			unsigned long  : 2;
			unsigned long FAWS : 13;
#endif
	} BIT;
	} FAW;
} st_ofsm_t;

typedef struct st_poe {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE0M : 4;
			unsigned short POE0M2 : 4;
			unsigned short PIE1 : 1;
			unsigned short  : 3;
			unsigned short POE0F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE0F : 1;
			unsigned short  : 3;
			unsigned short PIE1 : 1;
			unsigned short POE0M2 : 4;
			unsigned short POE0M : 4;
#endif
	} BIT;
	} ICSR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 8;
			unsigned short OIE1 : 1;
			unsigned short OCE1 : 1;
			unsigned short  : 5;
			unsigned short OSF1 : 1;
#else
			unsigned short OSF1 : 1;
			unsigned short  : 5;
			unsigned short OCE1 : 1;
			unsigned short OIE1 : 1;
			unsigned short  : 8;
#endif
	} BIT;
	} OCSR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE4M : 4;
			unsigned short POE4M2 : 4;
			unsigned short PIE2 : 1;
			unsigned short  : 3;
			unsigned short POE4F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE4F : 1;
			unsigned short  : 3;
			unsigned short PIE2 : 1;
			unsigned short POE4M2 : 4;
			unsigned short POE4M : 4;
#endif
	} BIT;
	} ICSR2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 8;
			unsigned short OIE2 : 1;
			unsigned short OCE2 : 1;
			unsigned short  : 5;
			unsigned short OSF2 : 1;
#else
			unsigned short OSF2 : 1;
			unsigned short  : 5;
			unsigned short OCE2 : 1;
			unsigned short OIE2 : 1;
			unsigned short  : 8;
#endif
	} BIT;
	} OCSR2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE8M : 4;
			unsigned short POE8M2 : 4;
			unsigned short PIE3 : 1;
			unsigned short POE8E : 1;
			unsigned short  : 2;
			unsigned short POE8F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE8F : 1;
			unsigned short  : 2;
			unsigned short POE8E : 1;
			unsigned short PIE3 : 1;
			unsigned short POE8M2 : 4;
			unsigned short POE8M : 4;
#endif
	} BIT;
	} ICSR3;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MTU0AZE : 1;
			unsigned char MTU0BZE : 1;
			unsigned char MTU0CZE : 1;
			unsigned char MTU0DZE : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char MTU0DZE : 1;
			unsigned char MTU0CZE : 1;
			unsigned char MTU0BZE : 1;
			unsigned char MTU0AZE : 1;
#endif
	} BIT;
	} POECR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short MTU7BDZE : 1;
			unsigned short MTU7ACZE : 1;
			unsigned short MTU6BDZE : 1;
			unsigned short  : 5;
			unsigned short MTU4BDZE : 1;
			unsigned short MTU4ACZE : 1;
			unsigned short MTU3BDZE : 1;
			unsigned short  : 5;
#else
			unsigned short  : 5;
			unsigned short MTU3BDZE : 1;
			unsigned short MTU4ACZE : 1;
			unsigned short MTU4BDZE : 1;
			unsigned short  : 5;
			unsigned short MTU6BDZE : 1;
			unsigned short MTU7ACZE : 1;
			unsigned short MTU7BDZE : 1;
#endif
	} BIT;
	} POECR2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short GPT0ABZE : 1;
			unsigned short GPT1ABZE : 1;
			unsigned short GPT2ABZE : 1;
			unsigned short GPT3ABZE : 1;
			unsigned short GPT4ABZE : 1;
			unsigned short GPT5ABZE : 1;
			unsigned short GPT6ABZE : 1;
			unsigned short GPT7ABZE : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short GPT7ABZE : 1;
			unsigned short GPT6ABZE : 1;
			unsigned short GPT5ABZE : 1;
			unsigned short GPT4ABZE : 1;
			unsigned short GPT3ABZE : 1;
			unsigned short GPT2ABZE : 1;
			unsigned short GPT1ABZE : 1;
			unsigned short GPT0ABZE : 1;
#endif
	} BIT;
	} POECR3;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDMT34ZE : 1;
			unsigned short IC1ADDMT34ZE : 1;
			unsigned short IC2ADDMT34ZE : 1;
			unsigned short IC3ADDMT34ZE : 1;
			unsigned short IC4ADDMT34ZE : 1;
			unsigned short IC5ADDMT34ZE : 1;
			unsigned short IC6ADDMT34ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDMT34ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDMT34ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDMT34ZE : 1;
			unsigned short IC5ADDMT34ZE : 1;
			unsigned short IC4ADDMT34ZE : 1;
			unsigned short IC3ADDMT34ZE : 1;
			unsigned short IC2ADDMT34ZE : 1;
			unsigned short IC1ADDMT34ZE : 1;
			unsigned short CMADDMT34ZE : 1;
#endif
	} BIT;
	} POECR4;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDMT0ZE : 1;
			unsigned short IC1ADDMT0ZE : 1;
			unsigned short IC2ADDMT0ZE : 1;
			unsigned short IC3ADDMT0ZE : 1;
			unsigned short IC4ADDMT0ZE : 1;
			unsigned short IC5ADDMT0ZE : 1;
			unsigned short IC6ADDMT0ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDMT0ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDMT0ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDMT0ZE : 1;
			unsigned short IC5ADDMT0ZE : 1;
			unsigned short IC4ADDMT0ZE : 1;
			unsigned short IC3ADDMT0ZE : 1;
			unsigned short IC2ADDMT0ZE : 1;
			unsigned short IC1ADDMT0ZE : 1;
			unsigned short CMADDMT0ZE : 1;
#endif
	} BIT;
	} POECR5;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDGPT01ZE : 1;
			unsigned short IC1ADDGPT01ZE : 1;
			unsigned short IC2ADDGPT01ZE : 1;
			unsigned short IC3ADDGPT01ZE : 1;
			unsigned short IC4ADDGPT01ZE : 1;
			unsigned short IC5ADDGPT01ZE : 1;
			unsigned short IC6ADDGPT01ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDGPT01ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDGPT01ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDGPT01ZE : 1;
			unsigned short IC5ADDGPT01ZE : 1;
			unsigned short IC4ADDGPT01ZE : 1;
			unsigned short IC3ADDGPT01ZE : 1;
			unsigned short IC2ADDGPT01ZE : 1;
			unsigned short IC1ADDGPT01ZE : 1;
			unsigned short CMADDGPT01ZE : 1;
#endif
	} BIT;
	} POECR6;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE10M : 4;
			unsigned short POE10M2 : 4;
			unsigned short PIE4 : 1;
			unsigned short POE10E : 1;
			unsigned short  : 2;
			unsigned short POE10F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE10F : 1;
			unsigned short  : 2;
			unsigned short POE10E : 1;
			unsigned short PIE4 : 1;
			unsigned short POE10M2 : 4;
			unsigned short POE10M : 4;
#endif
	} BIT;
	} ICSR4;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE11M : 4;
			unsigned short POE11M2 : 4;
			unsigned short PIE5 : 1;
			unsigned short POE11E : 1;
			unsigned short  : 2;
			unsigned short POE11F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE11F : 1;
			unsigned short  : 2;
			unsigned short POE11E : 1;
			unsigned short PIE5 : 1;
			unsigned short POE11M2 : 4;
			unsigned short POE11M : 4;
#endif
	} BIT;
	} ICSR5;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short OLSG0A : 1;
			unsigned short OLSG0B : 1;
			unsigned short OLSG1A : 1;
			unsigned short OLSG1B : 1;
			unsigned short OLSG2A : 1;
			unsigned short OLSG2B : 1;
			unsigned short  : 1;
			unsigned short OLSEN : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short OLSEN : 1;
			unsigned short  : 1;
			unsigned short OLSG2B : 1;
			unsigned short OLSG2A : 1;
			unsigned short OLSG1B : 1;
			unsigned short OLSG1A : 1;
			unsigned short OLSG0B : 1;
			unsigned short OLSG0A : 1;
#endif
	} BIT;
	} ALR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 9;
			unsigned short OSTSTE : 1;
			unsigned short  : 2;
			unsigned short OSTSTF : 1;
			unsigned short  : 3;
#else
			unsigned short  : 3;
			unsigned short OSTSTF : 1;
			unsigned short  : 2;
			unsigned short OSTSTE : 1;
			unsigned short  : 9;
#endif
	} BIT;
	} ICSR6;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short OLSG4A : 1;
			unsigned short OLSG4B : 1;
			unsigned short OLSG5A : 1;
			unsigned short OLSG5B : 1;
			unsigned short OLSG6A : 1;
			unsigned short OLSG6B : 1;
			unsigned short  : 1;
			unsigned short OLSEN : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short OLSEN : 1;
			unsigned short  : 1;
			unsigned short OLSG6B : 1;
			unsigned short OLSG6A : 1;
			unsigned short OLSG5B : 1;
			unsigned short OLSG5A : 1;
			unsigned short OLSG4B : 1;
			unsigned short OLSG4A : 1;
#endif
	} BIT;
	} ALR2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE12M : 4;
			unsigned short POE12M2 : 4;
			unsigned short PIE7 : 1;
			unsigned short POE12E : 1;
			unsigned short  : 2;
			unsigned short POE12F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE12F : 1;
			unsigned short  : 2;
			unsigned short POE12E : 1;
			unsigned short PIE7 : 1;
			unsigned short POE12M2 : 4;
			unsigned short POE12M : 4;
#endif
	} BIT;
	} ICSR7;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short MTU9AZE : 1;
			unsigned short MTU9BZE : 1;
			unsigned short MTU9CZE : 1;
			unsigned short MTU9DZE : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short MTU9DZE : 1;
			unsigned short MTU9CZE : 1;
			unsigned short MTU9BZE : 1;
			unsigned short MTU9AZE : 1;
#endif
	} BIT;
	} POECR7;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDMT9ZE : 1;
			unsigned short IC1ADDMT9ZE : 1;
			unsigned short IC2ADDMT9ZE : 1;
			unsigned short IC3ADDMT9ZE : 1;
			unsigned short IC4ADDMT9ZE : 1;
			unsigned short IC5ADDMT9ZE : 1;
			unsigned short IC6ADDMT9ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDMT9ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDMT9ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDMT9ZE : 1;
			unsigned short IC5ADDMT9ZE : 1;
			unsigned short IC4ADDMT9ZE : 1;
			unsigned short IC3ADDMT9ZE : 1;
			unsigned short IC2ADDMT9ZE : 1;
			unsigned short IC1ADDMT9ZE : 1;
			unsigned short CMADDMT9ZE : 1;
#endif
	} BIT;
	} POECR8;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short C0FLAG : 1;
			unsigned short C1FLAG : 1;
			unsigned short C2FLAG : 1;
			unsigned short C3FLAG : 1;
			unsigned short C4FLAG : 1;
			unsigned short C5FLAG : 1;
			unsigned short  : 10;
#else
			unsigned short  : 10;
			unsigned short C5FLAG : 1;
			unsigned short C4FLAG : 1;
			unsigned short C3FLAG : 1;
			unsigned short C2FLAG : 1;
			unsigned short C1FLAG : 1;
			unsigned short C0FLAG : 1;
#endif
	} BIT;
	} POECMPFR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POEREQ0 : 1;
			unsigned short POEREQ1 : 1;
			unsigned short POEREQ2 : 1;
			unsigned short POEREQ3 : 1;
			unsigned short POEREQ4 : 1;
			unsigned short POEREQ5 : 1;
			unsigned short  : 10;
#else
			unsigned short  : 10;
			unsigned short POEREQ5 : 1;
			unsigned short POEREQ4 : 1;
			unsigned short POEREQ3 : 1;
			unsigned short POEREQ2 : 1;
			unsigned short POEREQ1 : 1;
			unsigned short POEREQ0 : 1;
#endif
	} BIT;
	} POECMPSEL;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 8;
			unsigned short OIE3 : 1;
			unsigned short OCE3 : 1;
			unsigned short  : 5;
			unsigned short OSF3 : 1;
#else
			unsigned short OSF3 : 1;
			unsigned short  : 5;
			unsigned short OCE3 : 1;
			unsigned short OIE3 : 1;
			unsigned short  : 8;
#endif
	} BIT;
	} OCSR3;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short OLSG0A : 1;
			unsigned short OLSG0B : 1;
			unsigned short OLSG1A : 1;
			unsigned short OLSG1B : 1;
			unsigned short OLSG2A : 1;
			unsigned short OLSG2B : 1;
			unsigned short  : 1;
			unsigned short OLSEN : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short OLSEN : 1;
			unsigned short  : 1;
			unsigned short OLSG2B : 1;
			unsigned short OLSG2A : 1;
			unsigned short OLSG1B : 1;
			unsigned short OLSG1A : 1;
			unsigned short OLSG0B : 1;
			unsigned short OLSG0A : 1;
#endif
	} BIT;
	} ALR3;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short MTUCH34HIZ : 1;
			unsigned short MTUCH67HIZ : 1;
			unsigned short MTUCH0HIZ : 1;
			unsigned short GPT01HIZ : 1;
			unsigned short GPT23HIZ : 1;
			unsigned short  : 1;
			unsigned short MTUCH9HIZ : 1;
			unsigned short  : 1;
			unsigned short GPT02HIZ : 1;
			unsigned short GPT46HIZ : 1;
			unsigned short GPT79HIZ : 1;
			unsigned short  : 5;
#else
			unsigned short  : 5;
			unsigned short GPT79HIZ : 1;
			unsigned short GPT46HIZ : 1;
			unsigned short GPT02HIZ : 1;
			unsigned short  : 1;
			unsigned short MTUCH9HIZ : 1;
			unsigned short  : 1;
			unsigned short GPT23HIZ : 1;
			unsigned short GPT01HIZ : 1;
			unsigned short MTUCH0HIZ : 1;
			unsigned short MTUCH67HIZ : 1;
			unsigned short MTUCH34HIZ : 1;
#endif
	} BIT;
	} SPOER;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short MTU0AME : 1;
			unsigned short MTU0BME : 1;
			unsigned short MTU0CME : 1;
			unsigned short MTU0DME : 1;
			unsigned short  : 4;
			unsigned short MTU9AME : 1;
			unsigned short MTU9BME : 1;
			unsigned short MTU9CME : 1;
			unsigned short MTU9DME : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short MTU9DME : 1;
			unsigned short MTU9CME : 1;
			unsigned short MTU9BME : 1;
			unsigned short MTU9AME : 1;
			unsigned short  : 4;
			unsigned short MTU0DME : 1;
			unsigned short MTU0CME : 1;
			unsigned short MTU0BME : 1;
			unsigned short MTU0AME : 1;
#endif
	} BIT;
	} PMMCR0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short MTU7BME : 1;
			unsigned short MTU7AME : 1;
			unsigned short MTU6BME : 1;
			unsigned short MTU7DME : 1;
			unsigned short MTU7CME : 1;
			unsigned short MTU6DME : 1;
			unsigned short  : 2;
			unsigned short MTU4BME : 1;
			unsigned short MTU4AME : 1;
			unsigned short MTU3BME : 1;
			unsigned short MTU4DME : 1;
			unsigned short MTU4CME : 1;
			unsigned short MTU3DME : 1;
			unsigned short  : 2;
#else
			unsigned short  : 2;
			unsigned short MTU3DME : 1;
			unsigned short MTU4CME : 1;
			unsigned short MTU4DME : 1;
			unsigned short MTU3BME : 1;
			unsigned short MTU4AME : 1;
			unsigned short MTU4BME : 1;
			unsigned short  : 2;
			unsigned short MTU6DME : 1;
			unsigned short MTU7CME : 1;
			unsigned short MTU7DME : 1;
			unsigned short MTU6BME : 1;
			unsigned short MTU7AME : 1;
			unsigned short MTU7BME : 1;
#endif
	} BIT;
	} PMMCR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short GPT0AME : 1;
			unsigned short GPT0BME : 1;
			unsigned short GPT1AME : 1;
			unsigned short GPT1BME : 1;
			unsigned short GPT2AME : 1;
			unsigned short GPT2BME : 1;
			unsigned short GPT3AME : 1;
			unsigned short GPT3BME : 1;
			unsigned short GPT4AME : 1;
			unsigned short GPT4BME : 1;
			unsigned short GPT5AME : 1;
			unsigned short GPT5BME : 1;
			unsigned short GPT6AME : 1;
			unsigned short GPT6BME : 1;
			unsigned short GPT7AME : 1;
			unsigned short GPT7BME : 1;
#else
			unsigned short GPT7BME : 1;
			unsigned short GPT7AME : 1;
			unsigned short GPT6BME : 1;
			unsigned short GPT6AME : 1;
			unsigned short GPT5BME : 1;
			unsigned short GPT5AME : 1;
			unsigned short GPT4BME : 1;
			unsigned short GPT4AME : 1;
			unsigned short GPT3BME : 1;
			unsigned short GPT3AME : 1;
			unsigned short GPT2BME : 1;
			unsigned short GPT2AME : 1;
			unsigned short GPT1BME : 1;
			unsigned short GPT1AME : 1;
			unsigned short GPT0BME : 1;
			unsigned short GPT0AME : 1;
#endif
	} BIT;
	} PMMCR2;
	char           wk1[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX4;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX5;
	char           wk2[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short POE9M : 4;
			unsigned short POE9M2 : 4;
			unsigned short PIE8 : 1;
			unsigned short POE9E : 1;
			unsigned short  : 2;
			unsigned short POE9F : 1;
			unsigned short  : 2;
			unsigned short INV : 1;
#else
			unsigned short INV : 1;
			unsigned short  : 2;
			unsigned short POE9F : 1;
			unsigned short  : 2;
			unsigned short POE9E : 1;
			unsigned short PIE8 : 1;
			unsigned short POE9M2 : 4;
			unsigned short POE9M : 4;
#endif
	} BIT;
	} ICSR8;
	char           wk3[4];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 8;
			unsigned short OIE4 : 1;
			unsigned short OCE4 : 1;
			unsigned short  : 5;
			unsigned short OSF4 : 1;
#else
			unsigned short OSF4 : 1;
			unsigned short  : 5;
			unsigned short OCE4 : 1;
			unsigned short OIE4 : 1;
			unsigned short  : 8;
#endif
	} BIT;
	} OCSR4;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 8;
			unsigned short OIE5 : 1;
			unsigned short OCE5 : 1;
			unsigned short  : 5;
			unsigned short OSF5 : 1;
#else
			unsigned short OSF5 : 1;
			unsigned short  : 5;
			unsigned short OCE5 : 1;
			unsigned short OIE5 : 1;
			unsigned short  : 8;
#endif
	} BIT;
	} OCSR5;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short OLSG0A : 1;
			unsigned short OLSG0B : 1;
			unsigned short OLSG1A : 1;
			unsigned short OLSG1B : 1;
			unsigned short OLSG2A : 1;
			unsigned short OLSG2B : 1;
			unsigned short  : 1;
			unsigned short OLSEN : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short OLSEN : 1;
			unsigned short  : 1;
			unsigned short OLSG2B : 1;
			unsigned short OLSG2A : 1;
			unsigned short OLSG1B : 1;
			unsigned short OLSG1A : 1;
			unsigned short OLSG0B : 1;
			unsigned short OLSG0A : 1;
#endif
	} BIT;
	} ALR4;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short OLSG0A : 1;
			unsigned short OLSG0B : 1;
			unsigned short  : 5;
			unsigned short OLSEN : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short OLSEN : 1;
			unsigned short  : 5;
			unsigned short OLSG0B : 1;
			unsigned short OLSG0A : 1;
#endif
	} BIT;
	} ALR5;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDMT67ZE : 1;
			unsigned short IC1ADDMT67ZE : 1;
			unsigned short IC2ADDMT67ZE : 1;
			unsigned short IC3ADDMT67ZE : 1;
			unsigned short IC4ADDMT67ZE : 1;
			unsigned short IC5ADDMT67ZE : 1;
			unsigned short IC6ADDMT67ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDMT67ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDMT67ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDMT67ZE : 1;
			unsigned short IC5ADDMT67ZE : 1;
			unsigned short IC4ADDMT67ZE : 1;
			unsigned short IC3ADDMT67ZE : 1;
			unsigned short IC2ADDMT67ZE : 1;
			unsigned short IC1ADDMT67ZE : 1;
			unsigned short CMADDMT67ZE : 1;
#endif
	} BIT;
	} POECR4B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDGPT23ZE : 1;
			unsigned short IC1ADDGPT23ZE : 1;
			unsigned short IC2ADDGPT23ZE : 1;
			unsigned short IC3ADDGPT23ZE : 1;
			unsigned short IC4ADDGPT23ZE : 1;
			unsigned short IC5ADDGPT23ZE : 1;
			unsigned short IC6ADDGPT23ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDGPT23ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDGPT23ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDGPT23ZE : 1;
			unsigned short IC5ADDGPT23ZE : 1;
			unsigned short IC4ADDGPT23ZE : 1;
			unsigned short IC3ADDGPT23ZE : 1;
			unsigned short IC2ADDGPT23ZE : 1;
			unsigned short IC1ADDGPT23ZE : 1;
			unsigned short CMADDGPT23ZE : 1;
#endif
	} BIT;
	} POECR6B;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDGPT02ZE : 1;
			unsigned short IC1ADDGPT02ZE : 1;
			unsigned short IC2ADDGPT02ZE : 1;
			unsigned short IC3ADDGPT02ZE : 1;
			unsigned short IC4ADDGPT02ZE : 1;
			unsigned short IC5ADDGPT02ZE : 1;
			unsigned short IC6ADDGPT02ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDGPT02ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDGPT02ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDGPT02ZE : 1;
			unsigned short IC5ADDGPT02ZE : 1;
			unsigned short IC4ADDGPT02ZE : 1;
			unsigned short IC3ADDGPT02ZE : 1;
			unsigned short IC2ADDGPT02ZE : 1;
			unsigned short IC1ADDGPT02ZE : 1;
			unsigned short CMADDGPT02ZE : 1;
#endif
	} BIT;
	} POECR9;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDGPT46ZE : 1;
			unsigned short IC1ADDGPT46ZE : 1;
			unsigned short IC2ADDGPT46ZE : 1;
			unsigned short IC3ADDGPT46ZE : 1;
			unsigned short IC4ADDGPT46ZE : 1;
			unsigned short IC5ADDGPT46ZE : 1;
			unsigned short IC6ADDGPT46ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDGPT46ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDGPT46ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDGPT46ZE : 1;
			unsigned short IC5ADDGPT46ZE : 1;
			unsigned short IC4ADDGPT46ZE : 1;
			unsigned short IC3ADDGPT46ZE : 1;
			unsigned short IC2ADDGPT46ZE : 1;
			unsigned short IC1ADDGPT46ZE : 1;
			unsigned short CMADDGPT46ZE : 1;
#endif
	} BIT;
	} POECR10;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMADDGPT79ZE : 1;
			unsigned short IC1ADDGPT79ZE : 1;
			unsigned short IC2ADDGPT79ZE : 1;
			unsigned short IC3ADDGPT79ZE : 1;
			unsigned short IC4ADDGPT79ZE : 1;
			unsigned short IC5ADDGPT79ZE : 1;
			unsigned short IC6ADDGPT79ZE : 1;
			unsigned short  : 1;
			unsigned short IC8ADDGPT79ZE : 1;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short IC8ADDGPT79ZE : 1;
			unsigned short  : 1;
			unsigned short IC6ADDGPT79ZE : 1;
			unsigned short IC5ADDGPT79ZE : 1;
			unsigned short IC4ADDGPT79ZE : 1;
			unsigned short IC3ADDGPT79ZE : 1;
			unsigned short IC2ADDGPT79ZE : 1;
			unsigned short IC1ADDGPT79ZE : 1;
			unsigned short CMADDGPT79ZE : 1;
#endif
	} BIT;
	} POECR11;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX6;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX7;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POEREQ0 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POEREQ5 : 1;
			unsigned char POEREQ4 : 1;
			unsigned char POEREQ3 : 1;
			unsigned char POEREQ2 : 1;
			unsigned char POEREQ1 : 1;
			unsigned char POEREQ0 : 1;
#endif
	} BIT;
	} POECMPEX8;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE0MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE0MS : 6;
#endif
	} BIT;
	} IMCR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE4MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE4MS : 6;
#endif
	} BIT;
	} IMCR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE8MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE8MS : 6;
#endif
	} BIT;
	} IMCR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE10MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE10MS : 6;
#endif
	} BIT;
	} IMCR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M0ASEL : 4;
			unsigned char M0BSEL : 4;
#else
			unsigned char M0BSEL : 4;
			unsigned char M0ASEL : 4;
#endif
	} BIT;
	} M0SELR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M0CSEL : 4;
			unsigned char M0DSEL : 4;
#else
			unsigned char M0DSEL : 4;
			unsigned char M0CSEL : 4;
#endif
	} BIT;
	} M0SELR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M3BSEL : 4;
			unsigned char M3DSEL : 4;
#else
			unsigned char M3DSEL : 4;
			unsigned char M3BSEL : 4;
#endif
	} BIT;
	} M3SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M4ASEL : 4;
			unsigned char M4CSEL : 4;
#else
			unsigned char M4CSEL : 4;
			unsigned char M4ASEL : 4;
#endif
	} BIT;
	} M4SELR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M4BSEL : 4;
			unsigned char M4DSEL : 4;
#else
			unsigned char M4DSEL : 4;
			unsigned char M4BSEL : 4;
#endif
	} BIT;
	} M4SELR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M6BSEL : 4;
			unsigned char M6DSEL : 4;
#else
			unsigned char M6DSEL : 4;
			unsigned char M6BSEL : 4;
#endif
	} BIT;
	} M6SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M7ASEL : 4;
			unsigned char M7CSEL : 4;
#else
			unsigned char M7CSEL : 4;
			unsigned char M7ASEL : 4;
#endif
	} BIT;
	} M7SELR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M7BSEL : 4;
			unsigned char M7DSEL : 4;
#else
			unsigned char M7DSEL : 4;
			unsigned char M7BSEL : 4;
#endif
	} BIT;
	} M7SELR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M9ASEL : 4;
			unsigned char M9BSEL : 4;
#else
			unsigned char M9BSEL : 4;
			unsigned char M9ASEL : 4;
#endif
	} BIT;
	} M9SELR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char M9CSEL : 4;
			unsigned char M9DSEL : 4;
#else
			unsigned char M9DSEL : 4;
			unsigned char M9CSEL : 4;
#endif
	} BIT;
	} M9SELR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G0ASEL : 4;
			unsigned char G0BSEL : 4;
#else
			unsigned char G0BSEL : 4;
			unsigned char G0ASEL : 4;
#endif
	} BIT;
	} G0SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G1ASEL : 4;
			unsigned char G1BSEL : 4;
#else
			unsigned char G1BSEL : 4;
			unsigned char G1ASEL : 4;
#endif
	} BIT;
	} G1SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G2ASEL : 4;
			unsigned char G2BSEL : 4;
#else
			unsigned char G2BSEL : 4;
			unsigned char G2ASEL : 4;
#endif
	} BIT;
	} G2SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G3ASEL : 4;
			unsigned char G3BSEL : 4;
#else
			unsigned char G3BSEL : 4;
			unsigned char G3ASEL : 4;
#endif
	} BIT;
	} G3SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G4ASEL : 4;
			unsigned char G4BSEL : 4;
#else
			unsigned char G4BSEL : 4;
			unsigned char G4ASEL : 4;
#endif
	} BIT;
	} G4SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G5ASEL : 4;
			unsigned char G5BSEL : 4;
#else
			unsigned char G5BSEL : 4;
			unsigned char G5ASEL : 4;
#endif
	} BIT;
	} G5SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G6ASEL : 4;
			unsigned char G6BSEL : 4;
#else
			unsigned char G6BSEL : 4;
			unsigned char G6ASEL : 4;
#endif
	} BIT;
	} G6SELR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char G7ASEL : 4;
			unsigned char G7BSEL : 4;
#else
			unsigned char G7BSEL : 4;
			unsigned char G7ASEL : 4;
#endif
	} BIT;
	} G7SELR;
	char           wk5[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE11MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE11MS : 6;
#endif
	} BIT;
	} IMCR4;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE12MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE12MS : 6;
#endif
	} BIT;
	} IMCR5;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POE9MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char POE9MS : 6;
#endif
	} BIT;
	} IMCR6;
	char           wk6[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMP0MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char CMP0MS : 6;
#endif
	} BIT;
	} IMCR9;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMP1MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char CMP1MS : 6;
#endif
	} BIT;
	} IMCR10;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMP2MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char CMP2MS : 6;
#endif
	} BIT;
	} IMCR11;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMP3MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char CMP3MS : 6;
#endif
	} BIT;
	} IMCR12;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMP4MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char CMP4MS : 6;
#endif
	} BIT;
	} IMCR13;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMP5MS : 6;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char CMP5MS : 6;
#endif
	} BIT;
	} IMCR14;
} st_poe_t;

typedef struct st_poeg {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PIDF : 1;
			unsigned long IOCF : 1;
			unsigned long OSTPF : 1;
			unsigned long SSF : 1;
			unsigned long PIDE : 1;
			unsigned long IOCE : 1;
			unsigned long OSTPE : 1;
			unsigned long  : 1;
			unsigned long CDRE0 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE5 : 1;
			unsigned long  : 2;
			unsigned long ST : 1;
			unsigned long  : 7;
			unsigned long NFPSC : 1;
			unsigned long ELSEL : 1;
			unsigned long NFSN : 2;
			unsigned long INV : 1;
			unsigned long NFEN : 1;
			unsigned long NFCS : 2;
#else
			unsigned long NFCS : 2;
			unsigned long NFEN : 1;
			unsigned long INV : 1;
			unsigned long NFSN : 2;
			unsigned long ELSEL : 1;
			unsigned long NFPSC : 1;
			unsigned long  : 7;
			unsigned long ST : 1;
			unsigned long  : 2;
			unsigned long CDRE5 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE0 : 1;
			unsigned long  : 1;
			unsigned long OSTPE : 1;
			unsigned long IOCE : 1;
			unsigned long PIDE : 1;
			unsigned long SSF : 1;
			unsigned long OSTPF : 1;
			unsigned long IOCF : 1;
			unsigned long PIDF : 1;
#endif
	} BIT;
	} POEGGA;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSEL : 5;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long MSEL : 5;
#endif
	} BIT;
	} POEGICRA;
	char           wk0[56];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short WP : 1;
			unsigned short  : 7;
			unsigned short PRKEY : 8;
#else
			unsigned short PRKEY : 8;
			unsigned short  : 7;
			unsigned short WP : 1;
#endif
	} BIT;
	} GTONCWPA;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short NE : 1;
			unsigned short  : 3;
			unsigned short NFS : 4;
			unsigned short NFV : 1;
			unsigned short  : 2;
			unsigned short MSEL : 5;
#else
			unsigned short MSEL : 5;
			unsigned short  : 2;
			unsigned short NFV : 1;
			unsigned short NFS : 4;
			unsigned short  : 3;
			unsigned short NE : 1;
#endif
	} BIT;
	} GTONCCRA;
	char           wk2[186];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PIDF : 1;
			unsigned long IOCF : 1;
			unsigned long OSTPF : 1;
			unsigned long SSF : 1;
			unsigned long PIDE : 1;
			unsigned long IOCE : 1;
			unsigned long OSTPE : 1;
			unsigned long  : 1;
			unsigned long CDRE0 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE5 : 1;
			unsigned long  : 2;
			unsigned long ST : 1;
			unsigned long  : 7;
			unsigned long NFPSC : 1;
			unsigned long ELSEL : 1;
			unsigned long NFSN : 2;
			unsigned long INV : 1;
			unsigned long NFEN : 1;
			unsigned long NFCS : 2;
#else
			unsigned long NFCS : 2;
			unsigned long NFEN : 1;
			unsigned long INV : 1;
			unsigned long NFSN : 2;
			unsigned long ELSEL : 1;
			unsigned long NFPSC : 1;
			unsigned long  : 7;
			unsigned long ST : 1;
			unsigned long  : 2;
			unsigned long CDRE5 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE0 : 1;
			unsigned long  : 1;
			unsigned long OSTPE : 1;
			unsigned long IOCE : 1;
			unsigned long PIDE : 1;
			unsigned long SSF : 1;
			unsigned long OSTPF : 1;
			unsigned long IOCF : 1;
			unsigned long PIDF : 1;
#endif
	} BIT;
	} POEGGB;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSEL : 5;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long MSEL : 5;
#endif
	} BIT;
	} POEGICRB;
	char           wk3[56];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short WP : 1;
			unsigned short  : 7;
			unsigned short PRKEY : 8;
#else
			unsigned short PRKEY : 8;
			unsigned short  : 7;
			unsigned short WP : 1;
#endif
	} BIT;
	} GTONCWPB;
	char           wk4[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short NE : 1;
			unsigned short  : 3;
			unsigned short NFS : 4;
			unsigned short NFV : 1;
			unsigned short  : 2;
			unsigned short MSEL : 5;
#else
			unsigned short MSEL : 5;
			unsigned short  : 2;
			unsigned short NFV : 1;
			unsigned short NFS : 4;
			unsigned short  : 3;
			unsigned short NE : 1;
#endif
	} BIT;
	} GTONCCRB;
	char           wk5[186];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PIDF : 1;
			unsigned long IOCF : 1;
			unsigned long OSTPF : 1;
			unsigned long SSF : 1;
			unsigned long PIDE : 1;
			unsigned long IOCE : 1;
			unsigned long OSTPE : 1;
			unsigned long  : 1;
			unsigned long CDRE0 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE5 : 1;
			unsigned long  : 2;
			unsigned long ST : 1;
			unsigned long  : 7;
			unsigned long NFPSC : 1;
			unsigned long ELSEL : 1;
			unsigned long NFSN : 2;
			unsigned long INV : 1;
			unsigned long NFEN : 1;
			unsigned long NFCS : 2;
#else
			unsigned long NFCS : 2;
			unsigned long NFEN : 1;
			unsigned long INV : 1;
			unsigned long NFSN : 2;
			unsigned long ELSEL : 1;
			unsigned long NFPSC : 1;
			unsigned long  : 7;
			unsigned long ST : 1;
			unsigned long  : 2;
			unsigned long CDRE5 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE0 : 1;
			unsigned long  : 1;
			unsigned long OSTPE : 1;
			unsigned long IOCE : 1;
			unsigned long PIDE : 1;
			unsigned long SSF : 1;
			unsigned long OSTPF : 1;
			unsigned long IOCF : 1;
			unsigned long PIDF : 1;
#endif
	} BIT;
	} POEGGC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSEL : 5;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long MSEL : 5;
#endif
	} BIT;
	} POEGICRC;
	char           wk6[56];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short WP : 1;
			unsigned short  : 7;
			unsigned short PRKEY : 8;
#else
			unsigned short PRKEY : 8;
			unsigned short  : 7;
			unsigned short WP : 1;
#endif
	} BIT;
	} GTONCWPC;
	char           wk7[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short NE : 1;
			unsigned short  : 3;
			unsigned short NFS : 4;
			unsigned short NFV : 1;
			unsigned short  : 2;
			unsigned short MSEL : 5;
#else
			unsigned short MSEL : 5;
			unsigned short  : 2;
			unsigned short NFV : 1;
			unsigned short NFS : 4;
			unsigned short  : 3;
			unsigned short NE : 1;
#endif
	} BIT;
	} GTONCCRC;
	char           wk8[186];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PIDF : 1;
			unsigned long IOCF : 1;
			unsigned long OSTPF : 1;
			unsigned long SSF : 1;
			unsigned long PIDE : 1;
			unsigned long IOCE : 1;
			unsigned long OSTPE : 1;
			unsigned long  : 1;
			unsigned long CDRE0 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE5 : 1;
			unsigned long  : 2;
			unsigned long ST : 1;
			unsigned long  : 7;
			unsigned long NFPSC : 1;
			unsigned long ELSEL : 1;
			unsigned long NFSN : 2;
			unsigned long INV : 1;
			unsigned long NFEN : 1;
			unsigned long NFCS : 2;
#else
			unsigned long NFCS : 2;
			unsigned long NFEN : 1;
			unsigned long INV : 1;
			unsigned long NFSN : 2;
			unsigned long ELSEL : 1;
			unsigned long NFPSC : 1;
			unsigned long  : 7;
			unsigned long ST : 1;
			unsigned long  : 2;
			unsigned long CDRE5 : 1;
			unsigned long CDRE4 : 1;
			unsigned long CDRE3 : 1;
			unsigned long CDRE2 : 1;
			unsigned long CDRE1 : 1;
			unsigned long CDRE0 : 1;
			unsigned long  : 1;
			unsigned long OSTPE : 1;
			unsigned long IOCE : 1;
			unsigned long PIDE : 1;
			unsigned long SSF : 1;
			unsigned long OSTPF : 1;
			unsigned long IOCF : 1;
			unsigned long PIDF : 1;
#endif
	} BIT;
	} POEGGD;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSEL : 5;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long MSEL : 5;
#endif
	} BIT;
	} POEGICRD;
	char           wk9[56];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short WP : 1;
			unsigned short  : 7;
			unsigned short PRKEY : 8;
#else
			unsigned short PRKEY : 8;
			unsigned short  : 7;
			unsigned short WP : 1;
#endif
	} BIT;
	} GTONCWPD;
	char           wk10[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short NE : 1;
			unsigned short  : 3;
			unsigned short NFS : 4;
			unsigned short NFV : 1;
			unsigned short  : 2;
			unsigned short MSEL : 5;
#else
			unsigned short MSEL : 5;
			unsigned short  : 2;
			unsigned short NFV : 1;
			unsigned short NFS : 4;
			unsigned short  : 3;
			unsigned short NE : 1;
#endif
	} BIT;
	} GTONCCRD;
} st_poeg_t;

typedef struct st_port {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short P0HLD : 1;
			unsigned short P1HLD : 1;
			unsigned short P2HLD : 1;
			unsigned short P3HLD : 1;
			unsigned short P4HLD : 1;
			unsigned short P5HLD : 1;
			unsigned short P6HLD : 1;
			unsigned short P7HLD : 1;
			unsigned short P8HLD : 1;
			unsigned short P9HLD : 1;
			unsigned short PAHLD : 1;
			unsigned short PBHLD : 1;
			unsigned short  : 1;
			unsigned short PDHLD : 1;
			unsigned short PEHLD : 1;
			unsigned short  : 1;
#else
			unsigned short  : 1;
			unsigned short PEHLD : 1;
			unsigned short PDHLD : 1;
			unsigned short  : 1;
			unsigned short PBHLD : 1;
			unsigned short PAHLD : 1;
			unsigned short P9HLD : 1;
			unsigned short P8HLD : 1;
			unsigned short P7HLD : 1;
			unsigned short P6HLD : 1;
			unsigned short P5HLD : 1;
			unsigned short P4HLD : 1;
			unsigned short P3HLD : 1;
			unsigned short P2HLD : 1;
			unsigned short P1HLD : 1;
			unsigned short P0HLD : 1;
#endif
	} BIT;
	} POHSR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 7;
			unsigned short PNHLD : 1;
			unsigned short  : 8;
#else
			unsigned short  : 8;
			unsigned short PNHLD : 1;
			unsigned short  : 7;
#endif
	} BIT;
	} POHSR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char POHE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char POHE : 1;
#endif
	} BIT;
	} POHCR;
	char           wk0[13];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char GPSEMLE : 1;
			unsigned char GPSMD : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char GPSMD : 1;
			unsigned char GPSEMLE : 1;
#endif
	} BIT;
	} GPSEXT;
} st_port_t;

typedef struct st_port0 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	char           wk4[63];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
} st_port0_t;

typedef struct st_port1 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[32];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	char           wk4[62];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
} st_port1_t;

typedef struct st_port2 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char  : 2;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char  : 2;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char  : 2;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char  : 2;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char  : 2;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char  : 2;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char  : 2;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char  : 2;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[33];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 5;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 5;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[60];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char  : 2;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char  : 2;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char  : 2;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char  : 2;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
} st_port2_t;

typedef struct st_port3 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char  : 2;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 2;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char  : 2;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 2;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char  : 2;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 2;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char  : 2;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 2;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[34];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 4;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 4;
#endif
	} BIT;
	} ODR1;
	char           wk4[59];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char  : 2;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 2;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
} st_port3_t;

typedef struct st_port4 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[35];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[58];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
} st_port4_t;

typedef struct st_port5 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[36];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[57];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
} st_port5_t;

typedef struct st_port6 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[37];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[56];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
} st_port6_t;

typedef struct st_port7 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[38];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[55];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
	char           wk6[71];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char  : 1;
#endif
	} BIT;
	} DSCR2;
} st_port7_t;

typedef struct st_port8 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[39];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	char           wk4[55];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
	char           wk6[71];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 1;
			unsigned char B1 : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char B1 : 1;
			unsigned char  : 1;
#endif
	} BIT;
	} DSCR2;
} st_port8_t;

typedef struct st_port9 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[40];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[53];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
	char           wk6[71];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR2;
} st_port9_t;

typedef struct st_porta {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[41];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[52];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
} st_porta_t;

typedef struct st_portb {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[42];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[51];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 2;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char  : 2;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
	char           wk6[71];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 5;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char  : 5;
#endif
	} BIT;
	} DSCR2;
} st_portb_t;

typedef struct st_portd {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[44];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[49];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
	char           wk6[71];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char B3 : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char B3 : 1;
			unsigned char  : 3;
#endif
	} BIT;
	} DSCR2;
} st_portd_t;

typedef struct st_porte {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char  : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char  : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char B2 : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char B2 : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PMR;
	char           wk3[45];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 3;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 3;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char  : 1;
			unsigned char B2 : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char B2 : 1;
			unsigned char  : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} ODR1;
	char           wk4[48];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char  : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char B0 : 1;
			unsigned char B1 : 1;
			unsigned char  : 1;
			unsigned char B3 : 1;
			unsigned char B4 : 1;
			unsigned char B5 : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char B5 : 1;
			unsigned char B4 : 1;
			unsigned char B3 : 1;
			unsigned char  : 1;
			unsigned char B1 : 1;
			unsigned char B0 : 1;
#endif
	} BIT;
	} DSCR;
} st_porte_t;

typedef struct st_portn {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} PMR;
	char           wk3[54];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 4;
			unsigned char B4 : 1;
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char B6 : 1;
			unsigned char  : 1;
			unsigned char B4 : 1;
			unsigned char  : 4;
#endif
	} BIT;
	} ODR1;
	char           wk4[40];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 6;
			unsigned char B6 : 1;
			unsigned char B7 : 1;
#else
			unsigned char B7 : 1;
			unsigned char B6 : 1;
			unsigned char  : 6;
#endif
	} BIT;
	} DSCR;
} st_portn_t;

typedef struct st_ram {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RAMMODE : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char RAMMODE : 2;
#endif
	} BIT;
	} RAMMODE;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RAMERR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char RAMERR : 1;
#endif
	} BIT;
	} RAMSTS;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RAMPRCR : 1;
			unsigned char KW : 7;
#else
			unsigned char KW : 7;
			unsigned char RAMPRCR : 1;
#endif
	} BIT;
	} RAMPRCR;
	char           wk1[3];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 3;
			unsigned long READ : 16;
			unsigned long  : 13;
#else
			unsigned long  : 13;
			unsigned long READ : 16;
			unsigned long  : 3;
#endif
	} BIT;
	} RAMECAD;
} st_ram_t;

typedef struct st_ri3c {
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long OMS : 1;
			unsigned long  : 31;
#else
			unsigned long  : 31;
			unsigned long OMS : 1;
#endif
	} BIT;
	} ICMR;
	char           wk0[16];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IBAINC : 1;
			unsigned long  : 7;
			unsigned long HJC : 1;
			unsigned long  : 20;
			unsigned long ABORT : 1;
			unsigned long RESUME : 1;
			unsigned long ICE : 1;
#else
			unsigned long ICE : 1;
			unsigned long RESUME : 1;
			unsigned long ABORT : 1;
			unsigned long  : 20;
			unsigned long HJC : 1;
			unsigned long  : 7;
			unsigned long IBAINC : 1;
#endif
	} BIT;
	} ICCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 16;
			unsigned long DADR : 7;
			unsigned long  : 8;
			unsigned long DAV : 1;
#else
			unsigned long DAV : 1;
			unsigned long  : 8;
			unsigned long DADR : 7;
			unsigned long  : 16;
#endif
	} BIT;
	} ICCAR;
	char           wk1[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MRST : 1;
			unsigned long CQRST : 1;
			unsigned long RQRST : 1;
			unsigned long TBRST : 1;
			unsigned long RBRST : 1;
			unsigned long IQRST : 1;
			unsigned long SQRST : 1;
			unsigned long  : 9;
			unsigned long ISRST : 1;
			unsigned long  : 15;
#else
			unsigned long  : 15;
			unsigned long ISRST : 1;
			unsigned long  : 9;
			unsigned long SQRST : 1;
			unsigned long IQRST : 1;
			unsigned long RBRST : 1;
			unsigned long TBRST : 1;
			unsigned long RQRST : 1;
			unsigned long CQRST : 1;
			unsigned long MRST : 1;
#endif
	} BIT;
	} ICRCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 2;
			unsigned long ACF : 1;
			unsigned long  : 4;
			unsigned long WP : 1;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long WP : 1;
			unsigned long  : 4;
			unsigned long ACF : 1;
			unsigned long  : 2;
#endif
	} BIT;
	} ICMMR;
	char           wk2[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 10;
			unsigned long BERF : 1;
			unsigned long  : 21;
#else
			unsigned long  : 21;
			unsigned long BERF : 1;
			unsigned long  : 10;
#endif
	} BIT;
	} ICISR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 10;
			unsigned long BERDE : 1;
			unsigned long  : 21;
#else
			unsigned long  : 21;
			unsigned long BERDE : 1;
			unsigned long  : 10;
#endif
	} BIT;
	} ICISER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 10;
			unsigned long BERIE : 1;
			unsigned long  : 21;
#else
			unsigned long  : 21;
			unsigned long BERIE : 1;
			unsigned long  : 10;
#endif
	} BIT;
	} ICISIER;
	char           wk3[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 19;
			unsigned long INDEX : 5;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long INDEX : 5;
			unsigned long  : 19;
#endif
	} BIT;
	} ICDCTIR;
	char           wk4[16];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RHJRN : 1;
			unsigned long RCRRN : 1;
			unsigned long  : 1;
			unsigned long RTIRN : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long RTIRN : 1;
			unsigned long  : 1;
			unsigned long RCRRN : 1;
			unsigned long RHJRN : 1;
#endif
	} BIT;
	} ICINCR;
	char           wk5[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 16;
			unsigned long TA0DE : 1;
			unsigned long  : 15;
#else
			unsigned long  : 15;
			unsigned long TA0DE : 1;
			unsigned long  : 16;
#endif
	} BIT;
	} ICTCR;
	char           wk6[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ODLW : 8;
			unsigned long ODHW : 8;
			unsigned long PPLW : 6;
			unsigned long  : 2;
			unsigned long PPHW : 6;
			unsigned long  : 1;
			unsigned long ODDBL : 1;
#else
			unsigned long ODDBL : 1;
			unsigned long  : 1;
			unsigned long PPHW : 6;
			unsigned long  : 2;
			unsigned long PPLW : 6;
			unsigned long ODHW : 8;
			unsigned long ODLW : 8;
#endif
	} BIT;
	} ICSBR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ODLW : 8;
			unsigned long ODHW : 8;
			unsigned long PPLW : 6;
			unsigned long  : 2;
			unsigned long PPHW : 6;
			unsigned long  : 2;
#else
			unsigned long  : 2;
			unsigned long PPHW : 6;
			unsigned long  : 2;
			unsigned long PPLW : 6;
			unsigned long ODHW : 8;
			unsigned long ODLW : 8;
#endif
	} BIT;
	} ICEBR;
	unsigned long  ICBFTR;
	unsigned long  ICBATR;
	unsigned long  ICBITR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SDAO : 1;
			unsigned long SCLO : 1;
			unsigned long SOWP : 1;
			unsigned long  : 29;
#else
			unsigned long  : 29;
			unsigned long SOWP : 1;
			unsigned long SCLO : 1;
			unsigned long SDAO : 1;
#endif
	} BIT;
	} ICOCR;
	char           wk7[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TMOS : 2;
			unsigned long  : 2;
			unsigned long TMOL : 1;
			unsigned long TMOH : 1;
			unsigned long TMOM : 2;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long TMOM : 2;
			unsigned long TMOH : 1;
			unsigned long TMOL : 1;
			unsigned long  : 2;
			unsigned long TMOS : 2;
#endif
	} BIT;
	} ICTOR;
	char           wk8[28];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long STT : 16;
			unsigned long  : 12;
			unsigned long AASE : 1;
			unsigned long  : 1;
			unsigned long PBSE : 1;
			unsigned long APSE : 1;
#else
			unsigned long APSE : 1;
			unsigned long PBSE : 1;
			unsigned long  : 1;
			unsigned long AASE : 1;
			unsigned long  : 12;
			unsigned long STT : 16;
#endif
	} BIT;
	} ICSTCR;
	char           wk9[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 16;
			unsigned long DLGTH : 16;
#else
			unsigned long DLGTH : 16;
			unsigned long  : 16;
#endif
	} BIT;
	} ICTDLR;
	char           wk10[140];
	unsigned long  ICCQR;
	unsigned long  ICRQR;
	union {
		unsigned long LONG;
		unsigned char BYTE;
	} ICDR;
	char           wk11[32];
	unsigned long  ICIQR;
	unsigned long  ICSQR;
	char           wk12[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CQTH : 8;
			unsigned long RQTH : 8;
			unsigned long IDSS : 8;
			unsigned long IQTH : 8;
#else
			unsigned long IQTH : 8;
			unsigned long IDSS : 8;
			unsigned long RQTH : 8;
			unsigned long CQTH : 8;
#endif
	} BIT;
	} ICQBTCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TETH : 3;
			unsigned long  : 5;
			unsigned long RFTH : 3;
			unsigned long  : 5;
			unsigned long TSTH : 3;
			unsigned long  : 5;
			unsigned long RSTH : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long RSTH : 3;
			unsigned long  : 5;
			unsigned long TSTH : 3;
			unsigned long  : 5;
			unsigned long RFTH : 3;
			unsigned long  : 5;
			unsigned long TETH : 3;
#endif
	} BIT;
	} ICDBTCR;
	char           wk13[40];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SQTH : 8;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long SQTH : 8;
#endif
	} BIT;
	} ICSQTCR;
	char           wk14[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long START : 1;
			unsigned long STOP : 1;
			unsigned long HDRXDF : 1;
			unsigned long  : 17;
			unsigned long TMOF : 1;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long TMOF : 1;
			unsigned long  : 17;
			unsigned long HDRXDF : 1;
			unsigned long STOP : 1;
			unsigned long START : 1;
#endif
	} BIT;
	} ICSR2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long STDE : 1;
			unsigned long SPDE : 1;
			unsigned long HDRXDE : 1;
			unsigned long  : 17;
			unsigned long TMOE : 1;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long TMOE : 1;
			unsigned long  : 17;
			unsigned long HDRXDE : 1;
			unsigned long SPDE : 1;
			unsigned long STDE : 1;
#endif
	} BIT;
	} ICSER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long STIE : 1;
			unsigned long SPIE : 1;
			unsigned long HDRXIE : 1;
			unsigned long  : 17;
			unsigned long TMOIE : 1;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long TMOIE : 1;
			unsigned long  : 17;
			unsigned long HDRXIE : 1;
			unsigned long SPIE : 1;
			unsigned long STIE : 1;
#endif
	} BIT;
	} ICSIER;
	char           wk15[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDRE : 1;
			unsigned long RDRF : 1;
			unsigned long IQEFF : 1;
			unsigned long CQEF : 1;
			unsigned long RQFF : 1;
			unsigned long DTAF : 1;
			unsigned long  : 3;
			unsigned long DTEF : 1;
			unsigned long  : 10;
			unsigned long SQFF : 1;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long SQFF : 1;
			unsigned long  : 10;
			unsigned long DTEF : 1;
			unsigned long  : 3;
			unsigned long DTAF : 1;
			unsigned long RQFF : 1;
			unsigned long CQEF : 1;
			unsigned long IQEFF : 1;
			unsigned long RDRF : 1;
			unsigned long TDRE : 1;
#endif
	} BIT;
	} ICCSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDE : 1;
			unsigned long RDE : 1;
			unsigned long IQEFDE : 1;
			unsigned long CQEDE : 1;
			unsigned long RQFDE : 1;
			unsigned long DTADE : 1;
			unsigned long  : 3;
			unsigned long DTEDE : 1;
			unsigned long  : 10;
			unsigned long SQFDE : 1;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long SQFDE : 1;
			unsigned long  : 10;
			unsigned long DTEDE : 1;
			unsigned long  : 3;
			unsigned long DTADE : 1;
			unsigned long RQFDE : 1;
			unsigned long CQEDE : 1;
			unsigned long IQEFDE : 1;
			unsigned long RDE : 1;
			unsigned long TDE : 1;
#endif
	} BIT;
	} ICCSER;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TIE : 1;
			unsigned long RIE : 1;
			unsigned long IQEFIE : 1;
			unsigned long CQEIE : 1;
			unsigned long RQFIE : 1;
			unsigned long DTAIE : 1;
			unsigned long  : 3;
			unsigned long DTEIE : 1;
			unsigned long  : 10;
			unsigned long SQFIE : 1;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long SQFIE : 1;
			unsigned long  : 10;
			unsigned long DTEIE : 1;
			unsigned long  : 3;
			unsigned long DTAIE : 1;
			unsigned long RQFIE : 1;
			unsigned long CQEIE : 1;
			unsigned long IQEFIE : 1;
			unsigned long RIE : 1;
			unsigned long TIE : 1;
#endif
	} BIT;
	} ICCSIER;
	char           wk16[36];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BFREE : 1;
			unsigned long BAVL : 1;
			unsigned long BIDL : 1;
			unsigned long  : 29;
#else
			unsigned long  : 29;
			unsigned long BIDL : 1;
			unsigned long BAVL : 1;
			unsigned long BFREE : 1;
#endif
	} BIT;
	} ICBSR;
	char           wk17[16];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SADR : 7;
			unsigned long  : 5;
			unsigned long IBIPL : 1;
			unsigned long TIRRJ : 1;
			unsigned long CRRRJ : 1;
			unsigned long IBITSE : 1;
			unsigned long DADR : 8;
			unsigned long  : 5;
			unsigned long NACKRC : 2;
			unsigned long TYPE : 1;
#else
			unsigned long TYPE : 1;
			unsigned long NACKRC : 2;
			unsigned long  : 5;
			unsigned long DADR : 8;
			unsigned long IBITSE : 1;
			unsigned long CRRRJ : 1;
			unsigned long TIRRJ : 1;
			unsigned long IBIPL : 1;
			unsigned long  : 5;
			unsigned long SADR : 7;
#endif
	} BIT;
	} ICTDATR0;
	char           wk18[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SADR : 7;
			unsigned long  : 5;
			unsigned long IBIPL : 1;
			unsigned long TIRRJ : 1;
			unsigned long CRRRJ : 1;
			unsigned long IBITSE : 1;
			unsigned long DADR : 8;
			unsigned long  : 5;
			unsigned long NACKRC : 2;
			unsigned long TYPE : 1;
#else
			unsigned long TYPE : 1;
			unsigned long NACKRC : 2;
			unsigned long  : 5;
			unsigned long DADR : 8;
			unsigned long IBITSE : 1;
			unsigned long CRRRJ : 1;
			unsigned long TIRRJ : 1;
			unsigned long IBIPL : 1;
			unsigned long  : 5;
			unsigned long SADR : 7;
#endif
	} BIT;
	} ICTDATR1;
	char           wk19[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SADR : 7;
			unsigned long  : 5;
			unsigned long IBIPL : 1;
			unsigned long TIRRJ : 1;
			unsigned long CRRRJ : 1;
			unsigned long IBITSE : 1;
			unsigned long DADR : 8;
			unsigned long  : 5;
			unsigned long NACKRC : 2;
			unsigned long TYPE : 1;
#else
			unsigned long TYPE : 1;
			unsigned long NACKRC : 2;
			unsigned long  : 5;
			unsigned long DADR : 8;
			unsigned long IBITSE : 1;
			unsigned long CRRRJ : 1;
			unsigned long TIRRJ : 1;
			unsigned long IBIPL : 1;
			unsigned long  : 5;
			unsigned long SADR : 7;
#endif
	} BIT;
	} ICTDATR2;
	char           wk20[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SADR : 7;
			unsigned long  : 5;
			unsigned long IBIPL : 1;
			unsigned long TIRRJ : 1;
			unsigned long CRRRJ : 1;
			unsigned long IBITSE : 1;
			unsigned long DADR : 8;
			unsigned long  : 5;
			unsigned long NACKRC : 2;
			unsigned long TYPE : 1;
#else
			unsigned long TYPE : 1;
			unsigned long NACKRC : 2;
			unsigned long  : 5;
			unsigned long DADR : 8;
			unsigned long IBITSE : 1;
			unsigned long CRRRJ : 1;
			unsigned long TIRRJ : 1;
			unsigned long IBIPL : 1;
			unsigned long  : 5;
			unsigned long SADR : 7;
#endif
	} BIT;
	} ICTDATR3;
	char           wk21[96];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SADR : 7;
			unsigned long  : 9;
			unsigned long DADR : 8;
			unsigned long  : 5;
			unsigned long NACKRC : 2;
			unsigned long TYPE : 1;
#else
			unsigned long TYPE : 1;
			unsigned long NACKRC : 2;
			unsigned long  : 5;
			unsigned long DADR : 8;
			unsigned long  : 9;
			unsigned long SADR : 7;
#endif
	} BIT;
	} ICEDATR;
	char           wk22[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SADR : 10;
			unsigned long  : 2;
			unsigned long IBIPL : 1;
			unsigned long  : 3;
			unsigned long DADR : 7;
			unsigned long  : 9;
#else
			unsigned long  : 9;
			unsigned long DADR : 7;
			unsigned long  : 3;
			unsigned long IBIPL : 1;
			unsigned long  : 2;
			unsigned long SADR : 10;
#endif
	} BIT;
	} ICDAR0;
	char           wk23[28];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long LIMIT : 1;
			unsigned long IBIRQC : 1;
			unsigned long IBIPL : 1;
			unsigned long OFLC : 1;
			unsigned long  : 2;
			unsigned long ROLE : 2;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long ROLE : 2;
			unsigned long  : 2;
			unsigned long OFLC : 1;
			unsigned long IBIPL : 1;
			unsigned long IBIRQC : 1;
			unsigned long LIMIT : 1;
			unsigned long  : 8;
#endif
	} BIT;
	} ICTDCTR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long LIMIT : 1;
			unsigned long IBIRQC : 1;
			unsigned long IBIPL : 1;
			unsigned long OFLC : 1;
			unsigned long  : 2;
			unsigned long ROLE : 2;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long ROLE : 2;
			unsigned long  : 2;
			unsigned long OFLC : 1;
			unsigned long IBIPL : 1;
			unsigned long IBIRQC : 1;
			unsigned long LIMIT : 1;
			unsigned long  : 8;
#endif
	} BIT;
	} ICTDCTR1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long LIMIT : 1;
			unsigned long IBIRQC : 1;
			unsigned long IBIPL : 1;
			unsigned long OFLC : 1;
			unsigned long  : 2;
			unsigned long ROLE : 2;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long ROLE : 2;
			unsigned long  : 2;
			unsigned long OFLC : 1;
			unsigned long IBIPL : 1;
			unsigned long IBIRQC : 1;
			unsigned long LIMIT : 1;
			unsigned long  : 8;
#endif
	} BIT;
	} ICTDCTR2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long LIMIT : 1;
			unsigned long IBIRQC : 1;
			unsigned long IBIPL : 1;
			unsigned long OFLC : 1;
			unsigned long  : 2;
			unsigned long ROLE : 2;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long ROLE : 2;
			unsigned long  : 2;
			unsigned long OFLC : 1;
			unsigned long IBIPL : 1;
			unsigned long IBIRQC : 1;
			unsigned long LIMIT : 1;
			unsigned long  : 8;
#endif
	} BIT;
	} ICTDCTR3;
	char           wk24[64];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DCR : 8;
			unsigned long LIMIT : 1;
			unsigned long IBIRQC : 1;
			unsigned long IBIPL : 1;
			unsigned long OFLC : 1;
			unsigned long  : 2;
			unsigned long ROLE : 2;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long ROLE : 2;
			unsigned long  : 2;
			unsigned long OFLC : 1;
			unsigned long IBIPL : 1;
			unsigned long IBIRQC : 1;
			unsigned long LIMIT : 1;
			unsigned long DCR : 8;
#endif
	} BIT;
	} ICDCTR;
	unsigned long  ICPIDLR;
	unsigned long  ICPIDHR;
	char           wk25[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 16;
			unsigned long TADR : 10;
			unsigned long  : 4;
			unsigned long SAV : 1;
			unsigned long DAV : 1;
#else
			unsigned long DAV : 1;
			unsigned long SAV : 1;
			unsigned long  : 4;
			unsigned long TADR : 10;
			unsigned long  : 16;
#endif
	} BIT;
	} ICDAMR0;
	char           wk26[28];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ENINT : 1;
			unsigned long ENCR : 1;
			unsigned long  : 1;
			unsigned long ENHJ : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long ENHJ : 1;
			unsigned long  : 1;
			unsigned long ENCR : 1;
			unsigned long ENINT : 1;
#endif
	} BIT;
	} ICTEVR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long ENTAS0 : 1;
			unsigned long ENTAS1 : 1;
			unsigned long ENTAS2 : 1;
			unsigned long ENTAS3 : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long ENTAS3 : 1;
			unsigned long ENTAS2 : 1;
			unsigned long ENTAS1 : 1;
			unsigned long ENTAS0 : 1;
#endif
	} BIT;
	} ICASR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MWL : 16;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long MWL : 16;
#endif
	} BIT;
	} ICMWLR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MRL : 16;
			unsigned long IBIPL : 8;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long IBIPL : 8;
			unsigned long MRL : 16;
#endif
	} BIT;
	} ICMRLR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TM : 8;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long TM : 8;
#endif
	} BIT;
	} ICTMR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PNDINT : 4;
			unsigned long  : 1;
			unsigned long PERR : 1;
			unsigned long CAS : 2;
			unsigned long VRSV : 8;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long VRSV : 8;
			unsigned long CAS : 2;
			unsigned long PERR : 1;
			unsigned long  : 1;
			unsigned long PNDINT : 4;
#endif
	} BIT;
	} ICDSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSWDR : 3;
			unsigned long  : 29;
#else
			unsigned long  : 29;
			unsigned long MSWDR : 3;
#endif
	} BIT;
	} ICMWSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSRDR : 3;
			unsigned long TSCO : 3;
			unsigned long  : 26;
#else
			unsigned long  : 26;
			unsigned long TSCO : 3;
			unsigned long MSRDR : 3;
#endif
	} BIT;
	} ICMRSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MRTT : 24;
			unsigned long  : 7;
			unsigned long MRTTE : 1;
#else
			unsigned long MRTTE : 1;
			unsigned long  : 7;
			unsigned long MRTT : 24;
#endif
	} BIT;
	} ICMTTR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long FREQ : 8;
			unsigned long INAC : 8;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long INAC : 8;
			unsigned long FREQ : 8;
			unsigned long  : 8;
#endif
	} BIT;
	} ICTSIR;
	char           wk27[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BC : 5;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long BC : 5;
#endif
	} BIT;
	} ICBCR;
	char           wk28[16];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CQFL : 8;
			unsigned long RQFL : 8;
			unsigned long IQFL : 8;
			unsigned long ISC : 5;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long ISC : 5;
			unsigned long IQFL : 8;
			unsigned long RQFL : 8;
			unsigned long CQFL : 8;
#endif
	} BIT;
	} ICQBSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TBFL : 8;
			unsigned long RBFL : 8;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long RBFL : 8;
			unsigned long TBFL : 8;
#endif
	} BIT;
	} ICDBSR;
	char           wk29[36];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SQFL : 8;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long SQFL : 8;
#endif
	} BIT;
	} ICSQSR;
	char           wk30[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SCLI : 1;
			unsigned long SDAI : 1;
			unsigned long SCLO : 1;
			unsigned long SDAO : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long SDAO : 1;
			unsigned long SCLO : 1;
			unsigned long SDAI : 1;
			unsigned long SCLI : 1;
#endif
	} BIT;
	} ICIMR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CE2C : 8;
			unsigned long  : 24;
#else
			unsigned long  : 24;
			unsigned long CE2C : 8;
#endif
	} BIT;
	} ICCECR;
} st_ri3c_t;

typedef struct st_riic {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SDAI : 1;
			unsigned char SCLI : 1;
			unsigned char SDAO : 1;
			unsigned char SCLO : 1;
			unsigned char SOWP : 1;
			unsigned char CLO : 1;
			unsigned char IICRST : 1;
			unsigned char ICE : 1;
#else
			unsigned char ICE : 1;
			unsigned char IICRST : 1;
			unsigned char CLO : 1;
			unsigned char SOWP : 1;
			unsigned char SCLO : 1;
			unsigned char SDAO : 1;
			unsigned char SCLI : 1;
			unsigned char SDAI : 1;
#endif
	} BIT;
	} ICCR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 1;
			unsigned char ST : 1;
			unsigned char RS : 1;
			unsigned char SP : 1;
			unsigned char  : 1;
			unsigned char TRS : 1;
			unsigned char MST : 1;
			unsigned char BBSY : 1;
#else
			unsigned char BBSY : 1;
			unsigned char MST : 1;
			unsigned char TRS : 1;
			unsigned char  : 1;
			unsigned char SP : 1;
			unsigned char RS : 1;
			unsigned char ST : 1;
			unsigned char  : 1;
#endif
	} BIT;
	} ICCR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BC : 3;
			unsigned char BCWP : 1;
			unsigned char CKS : 3;
			unsigned char MTWP : 1;
#else
			unsigned char MTWP : 1;
			unsigned char CKS : 3;
			unsigned char BCWP : 1;
			unsigned char BC : 3;
#endif
	} BIT;
	} ICMR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TMOS : 1;
			unsigned char TMOL : 1;
			unsigned char TMOH : 1;
			unsigned char  : 1;
			unsigned char SDDL : 3;
			unsigned char DLCS : 1;
#else
			unsigned char DLCS : 1;
			unsigned char SDDL : 3;
			unsigned char  : 1;
			unsigned char TMOH : 1;
			unsigned char TMOL : 1;
			unsigned char TMOS : 1;
#endif
	} BIT;
	} ICMR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NF : 2;
			unsigned char ACKBR : 1;
			unsigned char ACKBT : 1;
			unsigned char ACKWP : 1;
			unsigned char RDRFS : 1;
			unsigned char WAIT : 1;
			unsigned char SMBS : 1;
#else
			unsigned char SMBS : 1;
			unsigned char WAIT : 1;
			unsigned char RDRFS : 1;
			unsigned char ACKWP : 1;
			unsigned char ACKBT : 1;
			unsigned char ACKBR : 1;
			unsigned char NF : 2;
#endif
	} BIT;
	} ICMR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TMOE : 1;
			unsigned char MALE : 1;
			unsigned char NALE : 1;
			unsigned char SALE : 1;
			unsigned char NACKE : 1;
			unsigned char NFE : 1;
			unsigned char SCLE : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char SCLE : 1;
			unsigned char NFE : 1;
			unsigned char NACKE : 1;
			unsigned char SALE : 1;
			unsigned char NALE : 1;
			unsigned char MALE : 1;
			unsigned char TMOE : 1;
#endif
	} BIT;
	} ICFER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SAR0E : 1;
			unsigned char SAR1E : 1;
			unsigned char SAR2E : 1;
			unsigned char GCAE : 1;
			unsigned char  : 1;
			unsigned char DIDE : 1;
			unsigned char  : 1;
			unsigned char HOAE : 1;
#else
			unsigned char HOAE : 1;
			unsigned char  : 1;
			unsigned char DIDE : 1;
			unsigned char  : 1;
			unsigned char GCAE : 1;
			unsigned char SAR2E : 1;
			unsigned char SAR1E : 1;
			unsigned char SAR0E : 1;
#endif
	} BIT;
	} ICSER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TMOIE : 1;
			unsigned char ALIE : 1;
			unsigned char STIE : 1;
			unsigned char SPIE : 1;
			unsigned char NAKIE : 1;
			unsigned char RIE : 1;
			unsigned char TEIE : 1;
			unsigned char TIE : 1;
#else
			unsigned char TIE : 1;
			unsigned char TEIE : 1;
			unsigned char RIE : 1;
			unsigned char NAKIE : 1;
			unsigned char SPIE : 1;
			unsigned char STIE : 1;
			unsigned char ALIE : 1;
			unsigned char TMOIE : 1;
#endif
	} BIT;
	} ICIER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char AAS0 : 1;
			unsigned char AAS1 : 1;
			unsigned char AAS2 : 1;
			unsigned char GCA : 1;
			unsigned char  : 1;
			unsigned char DID : 1;
			unsigned char  : 1;
			unsigned char HOA : 1;
#else
			unsigned char HOA : 1;
			unsigned char  : 1;
			unsigned char DID : 1;
			unsigned char  : 1;
			unsigned char GCA : 1;
			unsigned char AAS2 : 1;
			unsigned char AAS1 : 1;
			unsigned char AAS0 : 1;
#endif
	} BIT;
	} ICSR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TMOF : 1;
			unsigned char AL : 1;
			unsigned char START : 1;
			unsigned char STOP : 1;
			unsigned char NACKF : 1;
			unsigned char RDRF : 1;
			unsigned char TEND : 1;
			unsigned char TDRE : 1;
#else
			unsigned char TDRE : 1;
			unsigned char TEND : 1;
			unsigned char RDRF : 1;
			unsigned char NACKF : 1;
			unsigned char STOP : 1;
			unsigned char START : 1;
			unsigned char AL : 1;
			unsigned char TMOF : 1;
#endif
	} BIT;
	} ICSR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SVA0 : 1;
			unsigned char SVA : 7;
#else
			unsigned char SVA : 7;
			unsigned char SVA0 : 1;
#endif
	} BIT;
	} SARL0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FS : 1;
			unsigned char SVA : 2;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SVA : 2;
			unsigned char FS : 1;
#endif
	} BIT;
	} SARU0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SVA0 : 1;
			unsigned char SVA : 7;
#else
			unsigned char SVA : 7;
			unsigned char SVA0 : 1;
#endif
	} BIT;
	} SARL1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FS : 1;
			unsigned char SVA : 2;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SVA : 2;
			unsigned char FS : 1;
#endif
	} BIT;
	} SARU1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SVA0 : 1;
			unsigned char SVA : 7;
#else
			unsigned char SVA : 7;
			unsigned char SVA0 : 1;
#endif
	} BIT;
	} SARL2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FS : 1;
			unsigned char SVA : 2;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SVA : 2;
			unsigned char FS : 1;
#endif
	} BIT;
	} SARU2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BRL : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char BRL : 5;
#endif
	} BIT;
	} ICBRL;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BRH : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char BRH : 5;
#endif
	} BIT;
	} ICBRH;
	unsigned char  ICDRT;
	unsigned char  ICDRR;
} st_riic_t;

typedef struct st_rsci8 {
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
		struct {
			unsigned char LL;
			unsigned char LH;
			unsigned char HL;
			unsigned char HH;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RDAT : 9;
			unsigned long MPB : 1;
			unsigned long DR : 1;
			unsigned long PER : 1;
			unsigned long FER : 1;
			unsigned long  : 11;
			unsigned long ORER : 1;
			unsigned long  : 2;
			unsigned long APER : 1;
			unsigned long AFER : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long AFER : 1;
			unsigned long APER : 1;
			unsigned long  : 2;
			unsigned long ORER : 1;
			unsigned long  : 11;
			unsigned long FER : 1;
			unsigned long PER : 1;
			unsigned long DR : 1;
			unsigned long MPB : 1;
			unsigned long RDAT : 9;
#endif
	} BIT;
	} RDR;
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
		struct {
			unsigned char LL;
			unsigned char LH;
			unsigned char HL;
			unsigned char HH;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDAT : 9;
			unsigned long MPBT : 1;
			unsigned long  : 2;
			unsigned long SYNC : 1;
			unsigned long  : 19;
#else
			unsigned long  : 19;
			unsigned long SYNC : 1;
			unsigned long  : 2;
			unsigned long MPBT : 1;
			unsigned long TDAT : 9;
#endif
	} BIT;
	} TDR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RE : 1;
			unsigned long  : 3;
			unsigned long TE : 1;
			unsigned long  : 3;
			unsigned long MPIE : 1;
			unsigned long DCME : 1;
			unsigned long IDSEL : 1;
			unsigned long  : 5;
			unsigned long RIE : 1;
			unsigned long  : 3;
			unsigned long TIE : 1;
			unsigned long TEIE : 1;
			unsigned long  : 2;
			unsigned long SSE : 1;
			unsigned long  : 7;
#else
			unsigned long  : 7;
			unsigned long SSE : 1;
			unsigned long  : 2;
			unsigned long TEIE : 1;
			unsigned long TIE : 1;
			unsigned long  : 3;
			unsigned long RIE : 1;
			unsigned long  : 5;
			unsigned long IDSEL : 1;
			unsigned long DCME : 1;
			unsigned long MPIE : 1;
			unsigned long  : 3;
			unsigned long TE : 1;
			unsigned long  : 3;
			unsigned long RE : 1;
#endif
	} BIT;
	} SCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CTSE : 1;
			unsigned long CRSEP : 1;
			unsigned long  : 2;
			unsigned long SPB2DT : 1;
			unsigned long SPB2IO : 1;
			unsigned long  : 2;
			unsigned long PE : 1;
			unsigned long PM : 1;
			unsigned long  : 2;
			unsigned long TINV : 1;
			unsigned long RINV : 1;
			unsigned long  : 2;
			unsigned long LOOP : 1;
			unsigned long  : 3;
			unsigned long HDSEL : 1;
			unsigned long  : 3;
			unsigned long NFCS : 3;
			unsigned long  : 1;
			unsigned long NFEN : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long NFEN : 1;
			unsigned long  : 1;
			unsigned long NFCS : 3;
			unsigned long  : 3;
			unsigned long HDSEL : 1;
			unsigned long  : 3;
			unsigned long LOOP : 1;
			unsigned long  : 2;
			unsigned long RINV : 1;
			unsigned long TINV : 1;
			unsigned long  : 2;
			unsigned long PM : 1;
			unsigned long PE : 1;
			unsigned long  : 2;
			unsigned long SPB2IO : 1;
			unsigned long SPB2DT : 1;
			unsigned long  : 2;
			unsigned long CRSEP : 1;
			unsigned long CTSE : 1;
#endif
	} BIT;
	} SCR1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BCP : 3;
			unsigned long  : 1;
			unsigned long BGDM : 1;
			unsigned long ABCS : 1;
			unsigned long ABCSE : 1;
			unsigned long  : 1;
			unsigned long BRR : 8;
			unsigned long BRME : 1;
			unsigned long  : 3;
			unsigned long CKS : 2;
			unsigned long  : 2;
			unsigned long MDDR : 8;
#else
			unsigned long MDDR : 8;
			unsigned long  : 2;
			unsigned long CKS : 2;
			unsigned long  : 3;
			unsigned long BRME : 1;
			unsigned long BRR : 8;
			unsigned long  : 1;
			unsigned long ABCSE : 1;
			unsigned long ABCS : 1;
			unsigned long BGDM : 1;
			unsigned long  : 1;
			unsigned long BCP : 3;
#endif
	} BIT;
	} SCR2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long  : 5;
			unsigned long SYNDIS : 1;
			unsigned long CHR : 2;
			unsigned long  : 2;
			unsigned long DDIR : 1;
			unsigned long DINV : 1;
			unsigned long STOP : 1;
			unsigned long RXDESEL : 1;
			unsigned long MOD : 3;
			unsigned long MP : 1;
			unsigned long FM : 1;
			unsigned long DEEN : 1;
			unsigned long  : 2;
			unsigned long CKE : 2;
			unsigned long  : 2;
			unsigned long GM : 1;
			unsigned long BLK : 1;
			unsigned long  : 2;
#else
			unsigned long  : 2;
			unsigned long BLK : 1;
			unsigned long GM : 1;
			unsigned long  : 2;
			unsigned long CKE : 2;
			unsigned long  : 2;
			unsigned long DEEN : 1;
			unsigned long FM : 1;
			unsigned long MP : 1;
			unsigned long MOD : 3;
			unsigned long RXDESEL : 1;
			unsigned long STOP : 1;
			unsigned long DINV : 1;
			unsigned long DDIR : 1;
			unsigned long  : 2;
			unsigned long CHR : 2;
			unsigned long SYNDIS : 1;
			unsigned long  : 5;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SCR3;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CMPD : 9;
			unsigned long  : 7;
			unsigned long RTADJ : 1;
			unsigned long TTADJ : 1;
			unsigned long  : 6;
			unsigned long RTMG : 4;
			unsigned long TTMG : 4;
#else
			unsigned long TTMG : 4;
			unsigned long RTMG : 4;
			unsigned long  : 6;
			unsigned long TTADJ : 1;
			unsigned long RTADJ : 1;
			unsigned long  : 7;
			unsigned long CMPD : 9;
#endif
	} BIT;
	} SCR4;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char HBSE : 1;
			unsigned char  : 1;
			unsigned char AOE : 1;
			unsigned char LPS : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char LPS : 1;
			unsigned char AOE : 1;
			unsigned char  : 1;
			unsigned char HBSE : 1;
#endif
	} BIT;
	} HBSCR;
	char           wk1[1];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IICDL : 5;
			unsigned long  : 3;
			unsigned long IICINTM : 1;
			unsigned long IICCSC : 1;
			unsigned long  : 3;
			unsigned long IICACKT : 1;
			unsigned long  : 2;
			unsigned long IICSTAREQ : 1;
			unsigned long IICRSTAREQ : 1;
			unsigned long IICSTPREQ : 1;
			unsigned long  : 1;
			unsigned long IICSDAS : 2;
			unsigned long IICSCLS : 2;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long IICSCLS : 2;
			unsigned long IICSDAS : 2;
			unsigned long  : 1;
			unsigned long IICSTPREQ : 1;
			unsigned long IICRSTAREQ : 1;
			unsigned long IICSTAREQ : 1;
			unsigned long  : 2;
			unsigned long IICACKT : 1;
			unsigned long  : 3;
			unsigned long IICCSC : 1;
			unsigned long IICINTM : 1;
			unsigned long  : 3;
			unsigned long IICDL : 5;
#endif
	} BIT;
	} SIMR;
	char           wk2[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DELVL : 1;
			unsigned long  : 7;
			unsigned long DESU : 5;
			unsigned long  : 3;
			unsigned long DEHLD : 5;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long DEHLD : 5;
			unsigned long  : 3;
			unsigned long DESU : 5;
			unsigned long  : 7;
			unsigned long DELVL : 1;
#endif
	} BIT;
	} DECR;
	char           wk3[20];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long ERS : 1;
			unsigned long  : 10;
			unsigned long RXDMON : 1;
			unsigned long DCMF : 1;
			unsigned long DPER : 1;
			unsigned long DFER : 1;
			unsigned long  : 5;
			unsigned long ORER : 1;
			unsigned long  : 1;
			unsigned long MFF : 1;
			unsigned long APER : 1;
			unsigned long AFER : 1;
			unsigned long TDRE : 1;
			unsigned long TEND : 1;
			unsigned long RDRF : 1;
#else
			unsigned long RDRF : 1;
			unsigned long TEND : 1;
			unsigned long TDRE : 1;
			unsigned long AFER : 1;
			unsigned long APER : 1;
			unsigned long MFF : 1;
			unsigned long  : 1;
			unsigned long ORER : 1;
			unsigned long  : 5;
			unsigned long DFER : 1;
			unsigned long DPER : 1;
			unsigned long DCMF : 1;
			unsigned long RXDMON : 1;
			unsigned long  : 10;
			unsigned long ERS : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} SSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IICACKR : 1;
			unsigned long  : 2;
			unsigned long IICSTIF : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long IICSTIF : 1;
			unsigned long  : 2;
			unsigned long IICACKR : 1;
#endif
	} BIT;
	} SISR;
	char           wk4[24];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long ERSC : 1;
			unsigned long  : 11;
			unsigned long DCMFC : 1;
			unsigned long DPERC : 1;
			unsigned long DFERC : 1;
			unsigned long  : 5;
			unsigned long ORERC : 1;
			unsigned long  : 1;
			unsigned long MFFC : 1;
			unsigned long APERC : 1;
			unsigned long AFERC : 1;
			unsigned long TDREC : 1;
			unsigned long  : 1;
			unsigned long RDRFC : 1;
#else
			unsigned long RDRFC : 1;
			unsigned long  : 1;
			unsigned long TDREC : 1;
			unsigned long AFERC : 1;
			unsigned long APERC : 1;
			unsigned long MFFC : 1;
			unsigned long  : 1;
			unsigned long ORERC : 1;
			unsigned long  : 5;
			unsigned long DFERC : 1;
			unsigned long DPERC : 1;
			unsigned long DCMFC : 1;
			unsigned long  : 11;
			unsigned long ERSC : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} SSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 3;
			unsigned long IICSTIFC : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long IICSTIFC : 1;
			unsigned long  : 3;
#endif
	} BIT;
	} SISCR;
} st_rsci8_t;

typedef struct st_rsci9 {
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
		struct {
			unsigned char LL;
			unsigned char LH;
			unsigned char HL;
			unsigned char HH;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RDAT : 9;
			unsigned long MPB : 1;
			unsigned long DR : 1;
			unsigned long PER : 1;
			unsigned long FER : 1;
			unsigned long  : 11;
			unsigned long ORER : 1;
			unsigned long  : 2;
			unsigned long APER : 1;
			unsigned long AFER : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long AFER : 1;
			unsigned long APER : 1;
			unsigned long  : 2;
			unsigned long ORER : 1;
			unsigned long  : 11;
			unsigned long FER : 1;
			unsigned long PER : 1;
			unsigned long DR : 1;
			unsigned long MPB : 1;
			unsigned long RDAT : 9;
#endif
	} BIT;
	} RDR;
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
		struct {
			unsigned char LL;
			unsigned char LH;
			unsigned char HL;
			unsigned char HH;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDAT : 9;
			unsigned long MPBT : 1;
			unsigned long  : 2;
			unsigned long SYNC : 1;
			unsigned long  : 19;
#else
			unsigned long  : 19;
			unsigned long SYNC : 1;
			unsigned long  : 2;
			unsigned long MPBT : 1;
			unsigned long TDAT : 9;
#endif
	} BIT;
	} TDR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RE : 1;
			unsigned long  : 3;
			unsigned long TE : 1;
			unsigned long  : 3;
			unsigned long MPIE : 1;
			unsigned long DCME : 1;
			unsigned long IDSEL : 1;
			unsigned long  : 5;
			unsigned long RIE : 1;
			unsigned long  : 3;
			unsigned long TIE : 1;
			unsigned long TEIE : 1;
			unsigned long  : 2;
			unsigned long SSE : 1;
			unsigned long  : 7;
#else
			unsigned long  : 7;
			unsigned long SSE : 1;
			unsigned long  : 2;
			unsigned long TEIE : 1;
			unsigned long TIE : 1;
			unsigned long  : 3;
			unsigned long RIE : 1;
			unsigned long  : 5;
			unsigned long IDSEL : 1;
			unsigned long DCME : 1;
			unsigned long MPIE : 1;
			unsigned long  : 3;
			unsigned long TE : 1;
			unsigned long  : 3;
			unsigned long RE : 1;
#endif
	} BIT;
	} SCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CTSE : 1;
			unsigned long CRSEP : 1;
			unsigned long  : 2;
			unsigned long SPB2DT : 1;
			unsigned long SPB2IO : 1;
			unsigned long  : 2;
			unsigned long PE : 1;
			unsigned long PM : 1;
			unsigned long  : 2;
			unsigned long TINV : 1;
			unsigned long RINV : 1;
			unsigned long  : 2;
			unsigned long LOOP : 1;
			unsigned long  : 3;
			unsigned long HDSEL : 1;
			unsigned long  : 3;
			unsigned long NFCS : 3;
			unsigned long  : 1;
			unsigned long NFEN : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long NFEN : 1;
			unsigned long  : 1;
			unsigned long NFCS : 3;
			unsigned long  : 3;
			unsigned long HDSEL : 1;
			unsigned long  : 3;
			unsigned long LOOP : 1;
			unsigned long  : 2;
			unsigned long RINV : 1;
			unsigned long TINV : 1;
			unsigned long  : 2;
			unsigned long PM : 1;
			unsigned long PE : 1;
			unsigned long  : 2;
			unsigned long SPB2IO : 1;
			unsigned long SPB2DT : 1;
			unsigned long  : 2;
			unsigned long CRSEP : 1;
			unsigned long CTSE : 1;
#endif
	} BIT;
	} SCR1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BCP : 3;
			unsigned long  : 1;
			unsigned long BGDM : 1;
			unsigned long ABCS : 1;
			unsigned long ABCSE : 1;
			unsigned long  : 1;
			unsigned long BRR : 8;
			unsigned long BRME : 1;
			unsigned long  : 3;
			unsigned long CKS : 2;
			unsigned long  : 2;
			unsigned long MDDR : 8;
#else
			unsigned long MDDR : 8;
			unsigned long  : 2;
			unsigned long CKS : 2;
			unsigned long  : 3;
			unsigned long BRME : 1;
			unsigned long BRR : 8;
			unsigned long  : 1;
			unsigned long ABCSE : 1;
			unsigned long ABCS : 1;
			unsigned long BGDM : 1;
			unsigned long  : 1;
			unsigned long BCP : 3;
#endif
	} BIT;
	} SCR2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long  : 5;
			unsigned long SYNDIS : 1;
			unsigned long CHR : 2;
			unsigned long  : 2;
			unsigned long DDIR : 1;
			unsigned long DINV : 1;
			unsigned long STOP : 1;
			unsigned long RXDESEL : 1;
			unsigned long MOD : 3;
			unsigned long MP : 1;
			unsigned long FM : 1;
			unsigned long DEEN : 1;
			unsigned long  : 2;
			unsigned long CKE : 2;
			unsigned long  : 2;
			unsigned long GM : 1;
			unsigned long BLK : 1;
			unsigned long  : 2;
#else
			unsigned long  : 2;
			unsigned long BLK : 1;
			unsigned long GM : 1;
			unsigned long  : 2;
			unsigned long CKE : 2;
			unsigned long  : 2;
			unsigned long DEEN : 1;
			unsigned long FM : 1;
			unsigned long MP : 1;
			unsigned long MOD : 3;
			unsigned long RXDESEL : 1;
			unsigned long STOP : 1;
			unsigned long DINV : 1;
			unsigned long DDIR : 1;
			unsigned long  : 2;
			unsigned long CHR : 2;
			unsigned long SYNDIS : 1;
			unsigned long  : 5;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SCR3;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CMPD : 9;
			unsigned long  : 7;
			unsigned long RTADJ : 1;
			unsigned long TTADJ : 1;
			unsigned long  : 6;
			unsigned long RTMG : 4;
			unsigned long TTMG : 4;
#else
			unsigned long TTMG : 4;
			unsigned long RTMG : 4;
			unsigned long  : 6;
			unsigned long TTADJ : 1;
			unsigned long RTADJ : 1;
			unsigned long  : 7;
			unsigned long CMPD : 9;
#endif
	} BIT;
	} SCR4;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char HBSE : 1;
			unsigned char  : 1;
			unsigned char AOE : 1;
			unsigned char LPS : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char LPS : 1;
			unsigned char AOE : 1;
			unsigned char  : 1;
			unsigned char HBSE : 1;
#endif
	} BIT;
	} HBSCR;
	char           wk1[1];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IICDL : 5;
			unsigned long  : 3;
			unsigned long IICINTM : 1;
			unsigned long IICCSC : 1;
			unsigned long  : 3;
			unsigned long IICACKT : 1;
			unsigned long  : 2;
			unsigned long IICSTAREQ : 1;
			unsigned long IICRSTAREQ : 1;
			unsigned long IICSTPREQ : 1;
			unsigned long  : 1;
			unsigned long IICSDAS : 2;
			unsigned long IICSCLS : 2;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long IICSCLS : 2;
			unsigned long IICSDAS : 2;
			unsigned long  : 1;
			unsigned long IICSTPREQ : 1;
			unsigned long IICRSTAREQ : 1;
			unsigned long IICSTAREQ : 1;
			unsigned long  : 2;
			unsigned long IICACKT : 1;
			unsigned long  : 3;
			unsigned long IICCSC : 1;
			unsigned long IICINTM : 1;
			unsigned long  : 3;
			unsigned long IICDL : 5;
#endif
	} BIT;
	} SIMR;
	char           wk2[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DECS : 1;
			unsigned long ENCS : 1;
			unsigned long SADJE : 1;
			unsigned long  : 1;
			unsigned long SBPTN : 1;
			unsigned long SYNCE : 1;
			unsigned long SBLEN : 1;
			unsigned long  : 1;
			unsigned long TPLEN : 4;
			unsigned long TPPAT : 2;
			unsigned long  : 2;
			unsigned long RPLEN : 4;
			unsigned long RPPAT : 2;
			unsigned long  : 2;
			unsigned long PFERIE : 1;
			unsigned long SYERIE : 1;
			unsigned long SBERIE : 1;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SBERIE : 1;
			unsigned long SYERIE : 1;
			unsigned long PFERIE : 1;
			unsigned long  : 2;
			unsigned long RPPAT : 2;
			unsigned long RPLEN : 4;
			unsigned long  : 2;
			unsigned long TPPAT : 2;
			unsigned long TPLEN : 4;
			unsigned long  : 1;
			unsigned long SBLEN : 1;
			unsigned long SYNCE : 1;
			unsigned long SBPTN : 1;
			unsigned long  : 1;
			unsigned long SADJE : 1;
			unsigned long ENCS : 1;
			unsigned long DECS : 1;
#endif
	} BIT;
	} MMCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DELVL : 1;
			unsigned long  : 7;
			unsigned long DESU : 5;
			unsigned long  : 3;
			unsigned long DEHLD : 5;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long DEHLD : 5;
			unsigned long  : 3;
			unsigned long DESU : 5;
			unsigned long  : 7;
			unsigned long DELVL : 1;
#endif
	} BIT;
	} DECR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TCSS : 2;
			unsigned long  : 6;
			unsigned long BFE : 1;
			unsigned long CF0RE : 1;
			unsigned long CF1DS : 2;
			unsigned long PIBE : 1;
			unsigned long PIBS : 3;
			unsigned long BFOIE : 1;
			unsigned long BCDIE : 1;
			unsigned long  : 2;
			unsigned long BFDIE : 1;
			unsigned long COFIE : 1;
			unsigned long AEDIE : 1;
			unsigned long  : 1;
			unsigned long BCCS : 2;
			unsigned long  : 6;
#else
			unsigned long  : 6;
			unsigned long BCCS : 2;
			unsigned long  : 1;
			unsigned long AEDIE : 1;
			unsigned long COFIE : 1;
			unsigned long BFDIE : 1;
			unsigned long  : 2;
			unsigned long BCDIE : 1;
			unsigned long BFOIE : 1;
			unsigned long PIBS : 3;
			unsigned long PIBE : 1;
			unsigned long CF1DS : 2;
			unsigned long CF0RE : 1;
			unsigned long BFE : 1;
			unsigned long  : 6;
			unsigned long TCSS : 2;
#endif
	} BIT;
	} XCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TCST : 1;
			unsigned long  : 3;
			unsigned long SDST : 1;
			unsigned long BRME : 1;
			unsigned long  : 2;
			unsigned long PCF1D : 8;
			unsigned long SCF1D : 8;
			unsigned long CF1CE : 8;
#else
			unsigned long CF1CE : 8;
			unsigned long SCF1D : 8;
			unsigned long PCF1D : 8;
			unsigned long  : 2;
			unsigned long BRME : 1;
			unsigned long SDST : 1;
			unsigned long  : 3;
			unsigned long TCST : 1;
#endif
	} BIT;
	} XCR1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CF0D : 8;
			unsigned long CF0CE : 8;
			unsigned long BFLW : 16;
#else
			unsigned long BFLW : 16;
			unsigned long CF0CE : 8;
			unsigned long CF0D : 8;
#endif
	} BIT;
	} XCR2;
	char           wk3[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long ERS : 1;
			unsigned long  : 10;
			unsigned long RXDMON : 1;
			unsigned long DCMF : 1;
			unsigned long DPER : 1;
			unsigned long DFER : 1;
			unsigned long  : 5;
			unsigned long ORER : 1;
			unsigned long  : 1;
			unsigned long MFF : 1;
			unsigned long APER : 1;
			unsigned long AFER : 1;
			unsigned long TDRE : 1;
			unsigned long TEND : 1;
			unsigned long RDRF : 1;
#else
			unsigned long RDRF : 1;
			unsigned long TEND : 1;
			unsigned long TDRE : 1;
			unsigned long AFER : 1;
			unsigned long APER : 1;
			unsigned long MFF : 1;
			unsigned long  : 1;
			unsigned long ORER : 1;
			unsigned long  : 5;
			unsigned long DFER : 1;
			unsigned long DPER : 1;
			unsigned long DCMF : 1;
			unsigned long RXDMON : 1;
			unsigned long  : 10;
			unsigned long ERS : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} SSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IICACKR : 1;
			unsigned long  : 2;
			unsigned long IICSTIF : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long IICSTIF : 1;
			unsigned long  : 2;
			unsigned long IICACKR : 1;
#endif
	} BIT;
	} SISR;
	char           wk4[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PFER : 1;
			unsigned long SYER : 1;
			unsigned long SBER : 1;
			unsigned long  : 1;
			unsigned long MCER : 1;
			unsigned long  : 1;
			unsigned long RSYNC : 1;
			unsigned long  : 25;
#else
			unsigned long  : 25;
			unsigned long RSYNC : 1;
			unsigned long  : 1;
			unsigned long MCER : 1;
			unsigned long  : 1;
			unsigned long SBER : 1;
			unsigned long SYER : 1;
			unsigned long PFER : 1;
#endif
	} BIT;
	} MMSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SFSF : 1;
			unsigned long RXDSF : 1;
			unsigned long  : 6;
			unsigned long BFOF : 1;
			unsigned long BCDF : 1;
			unsigned long BFDF : 1;
			unsigned long CF0MF : 1;
			unsigned long CF1MF : 1;
			unsigned long PIBDF : 1;
			unsigned long COF : 1;
			unsigned long AEDF : 1;
			unsigned long CF0RD : 8;
			unsigned long CF1RD : 8;
#else
			unsigned long CF1RD : 8;
			unsigned long CF0RD : 8;
			unsigned long AEDF : 1;
			unsigned long COF : 1;
			unsigned long PIBDF : 1;
			unsigned long CF1MF : 1;
			unsigned long CF0MF : 1;
			unsigned long BFDF : 1;
			unsigned long BCDF : 1;
			unsigned long BFOF : 1;
			unsigned long  : 6;
			unsigned long RXDSF : 1;
			unsigned long SFSF : 1;
#endif
	} BIT;
	} XSR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CCV : 16;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long CCV : 16;
#endif
	} BIT;
	} XSR1;
	char           wk5[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long ERSC : 1;
			unsigned long  : 11;
			unsigned long DCMFC : 1;
			unsigned long DPERC : 1;
			unsigned long DFERC : 1;
			unsigned long  : 5;
			unsigned long ORERC : 1;
			unsigned long  : 1;
			unsigned long MFFC : 1;
			unsigned long APERC : 1;
			unsigned long AFERC : 1;
			unsigned long TDREC : 1;
			unsigned long  : 1;
			unsigned long RDRFC : 1;
#else
			unsigned long RDRFC : 1;
			unsigned long  : 1;
			unsigned long TDREC : 1;
			unsigned long AFERC : 1;
			unsigned long APERC : 1;
			unsigned long MFFC : 1;
			unsigned long  : 1;
			unsigned long ORERC : 1;
			unsigned long  : 5;
			unsigned long DFERC : 1;
			unsigned long DPERC : 1;
			unsigned long DCMFC : 1;
			unsigned long  : 11;
			unsigned long ERSC : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} SSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 3;
			unsigned long IICSTIFC : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long IICSTIFC : 1;
			unsigned long  : 3;
#endif
	} BIT;
	} SISCR;
	char           wk6[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PFERC : 1;
			unsigned long SYERC : 1;
			unsigned long SBERC : 1;
			unsigned long  : 1;
			unsigned long MCERC : 1;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long MCERC : 1;
			unsigned long  : 1;
			unsigned long SBERC : 1;
			unsigned long SYERC : 1;
			unsigned long PFERC : 1;
#endif
	} BIT;
	} MMSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long BFOC : 1;
			unsigned long BCDCL : 1;
			unsigned long BFDCL : 1;
			unsigned long CF0MCL : 1;
			unsigned long CF1MCL : 1;
			unsigned long PIBDCL : 1;
			unsigned long COFC : 1;
			unsigned long AEDCL : 1;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long AEDCL : 1;
			unsigned long COFC : 1;
			unsigned long PIBDCL : 1;
			unsigned long CF1MCL : 1;
			unsigned long CF0MCL : 1;
			unsigned long BFDCL : 1;
			unsigned long BCDCL : 1;
			unsigned long BFOC : 1;
			unsigned long  : 8;
#endif
	} BIT;
	} XSCR;
} st_rsci9_t;

typedef struct st_rsci11 {
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
		struct {
			unsigned char LL;
			unsigned char LH;
			unsigned char HL;
			unsigned char HH;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RDAT : 9;
			unsigned long MPB : 1;
			unsigned long DR : 1;
			unsigned long PER : 1;
			unsigned long FER : 1;
			unsigned long  : 11;
			unsigned long ORER : 1;
			unsigned long  : 2;
			unsigned long APER : 1;
			unsigned long AFER : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long AFER : 1;
			unsigned long APER : 1;
			unsigned long  : 2;
			unsigned long ORER : 1;
			unsigned long  : 11;
			unsigned long FER : 1;
			unsigned long PER : 1;
			unsigned long DR : 1;
			unsigned long MPB : 1;
			unsigned long RDAT : 9;
#endif
	} BIT;
	} RDR;
	union {
		unsigned long LONG;
		struct {
			unsigned short L;
			unsigned short H;
		} WORD;
		struct {
			unsigned char LL;
			unsigned char LH;
			unsigned char HL;
			unsigned char HH;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TDAT : 9;
			unsigned long MPBT : 1;
			unsigned long  : 2;
			unsigned long SYNC : 1;
			unsigned long  : 19;
#else
			unsigned long  : 19;
			unsigned long SYNC : 1;
			unsigned long  : 2;
			unsigned long MPBT : 1;
			unsigned long TDAT : 9;
#endif
	} BIT;
	} TDR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long RE : 1;
			unsigned long  : 3;
			unsigned long TE : 1;
			unsigned long  : 3;
			unsigned long MPIE : 1;
			unsigned long DCME : 1;
			unsigned long IDSEL : 1;
			unsigned long  : 5;
			unsigned long RIE : 1;
			unsigned long  : 3;
			unsigned long TIE : 1;
			unsigned long TEIE : 1;
			unsigned long  : 2;
			unsigned long SSE : 1;
			unsigned long  : 7;
#else
			unsigned long  : 7;
			unsigned long SSE : 1;
			unsigned long  : 2;
			unsigned long TEIE : 1;
			unsigned long TIE : 1;
			unsigned long  : 3;
			unsigned long RIE : 1;
			unsigned long  : 5;
			unsigned long IDSEL : 1;
			unsigned long DCME : 1;
			unsigned long MPIE : 1;
			unsigned long  : 3;
			unsigned long TE : 1;
			unsigned long  : 3;
			unsigned long RE : 1;
#endif
	} BIT;
	} SCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CTSE : 1;
			unsigned long CRSEP : 1;
			unsigned long  : 2;
			unsigned long SPB2DT : 1;
			unsigned long SPB2IO : 1;
			unsigned long  : 2;
			unsigned long PE : 1;
			unsigned long PM : 1;
			unsigned long  : 2;
			unsigned long TINV : 1;
			unsigned long RINV : 1;
			unsigned long  : 2;
			unsigned long LOOP : 1;
			unsigned long  : 3;
			unsigned long HDSEL : 1;
			unsigned long  : 3;
			unsigned long NFCS : 3;
			unsigned long  : 1;
			unsigned long NFEN : 1;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long NFEN : 1;
			unsigned long  : 1;
			unsigned long NFCS : 3;
			unsigned long  : 3;
			unsigned long HDSEL : 1;
			unsigned long  : 3;
			unsigned long LOOP : 1;
			unsigned long  : 2;
			unsigned long RINV : 1;
			unsigned long TINV : 1;
			unsigned long  : 2;
			unsigned long PM : 1;
			unsigned long PE : 1;
			unsigned long  : 2;
			unsigned long SPB2IO : 1;
			unsigned long SPB2DT : 1;
			unsigned long  : 2;
			unsigned long CRSEP : 1;
			unsigned long CTSE : 1;
#endif
	} BIT;
	} SCR1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long BCP : 3;
			unsigned long  : 1;
			unsigned long BGDM : 1;
			unsigned long ABCS : 1;
			unsigned long ABCSE : 1;
			unsigned long  : 1;
			unsigned long BRR : 8;
			unsigned long BRME : 1;
			unsigned long  : 3;
			unsigned long CKS : 2;
			unsigned long  : 2;
			unsigned long MDDR : 8;
#else
			unsigned long MDDR : 8;
			unsigned long  : 2;
			unsigned long CKS : 2;
			unsigned long  : 3;
			unsigned long BRME : 1;
			unsigned long BRR : 8;
			unsigned long  : 1;
			unsigned long ABCSE : 1;
			unsigned long ABCS : 1;
			unsigned long BGDM : 1;
			unsigned long  : 1;
			unsigned long BCP : 3;
#endif
	} BIT;
	} SCR2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long  : 5;
			unsigned long SYNDIS : 1;
			unsigned long CHR : 2;
			unsigned long  : 2;
			unsigned long DDIR : 1;
			unsigned long DINV : 1;
			unsigned long STOP : 1;
			unsigned long RXDESEL : 1;
			unsigned long MOD : 3;
			unsigned long MP : 1;
			unsigned long FM : 1;
			unsigned long DEEN : 1;
			unsigned long  : 2;
			unsigned long CKE : 2;
			unsigned long  : 2;
			unsigned long GM : 1;
			unsigned long BLK : 1;
			unsigned long  : 2;
#else
			unsigned long  : 2;
			unsigned long BLK : 1;
			unsigned long GM : 1;
			unsigned long  : 2;
			unsigned long CKE : 2;
			unsigned long  : 2;
			unsigned long DEEN : 1;
			unsigned long FM : 1;
			unsigned long MP : 1;
			unsigned long MOD : 3;
			unsigned long RXDESEL : 1;
			unsigned long STOP : 1;
			unsigned long DINV : 1;
			unsigned long DDIR : 1;
			unsigned long  : 2;
			unsigned long CHR : 2;
			unsigned long SYNDIS : 1;
			unsigned long  : 5;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SCR3;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CMPD : 9;
			unsigned long  : 7;
			unsigned long RTADJ : 1;
			unsigned long TTADJ : 1;
			unsigned long  : 6;
			unsigned long RTMG : 4;
			unsigned long TTMG : 4;
#else
			unsigned long TTMG : 4;
			unsigned long RTMG : 4;
			unsigned long  : 6;
			unsigned long TTADJ : 1;
			unsigned long RTADJ : 1;
			unsigned long  : 7;
			unsigned long CMPD : 9;
#endif
	} BIT;
	} SCR4;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char HBSE : 1;
			unsigned char  : 1;
			unsigned char AOE : 1;
			unsigned char LPS : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char LPS : 1;
			unsigned char AOE : 1;
			unsigned char  : 1;
			unsigned char HBSE : 1;
#endif
	} BIT;
	} HBSCR;
	char           wk1[1];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IICDL : 5;
			unsigned long  : 3;
			unsigned long IICINTM : 1;
			unsigned long IICCSC : 1;
			unsigned long  : 3;
			unsigned long IICACKT : 1;
			unsigned long  : 2;
			unsigned long IICSTAREQ : 1;
			unsigned long IICRSTAREQ : 1;
			unsigned long IICSTPREQ : 1;
			unsigned long  : 1;
			unsigned long IICSDAS : 2;
			unsigned long IICSCLS : 2;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long IICSCLS : 2;
			unsigned long IICSDAS : 2;
			unsigned long  : 1;
			unsigned long IICSTPREQ : 1;
			unsigned long IICRSTAREQ : 1;
			unsigned long IICSTAREQ : 1;
			unsigned long  : 2;
			unsigned long IICACKT : 1;
			unsigned long  : 3;
			unsigned long IICCSC : 1;
			unsigned long IICINTM : 1;
			unsigned long  : 3;
			unsigned long IICDL : 5;
#endif
	} BIT;
	} SIMR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DRES : 1;
			unsigned long  : 7;
			unsigned long TTRG : 5;
			unsigned long  : 2;
			unsigned long TFRST : 1;
			unsigned long RTRG : 5;
			unsigned long  : 2;
			unsigned long RFRST : 1;
			unsigned long RSTRG : 5;
			unsigned long  : 3;
#else
			unsigned long  : 3;
			unsigned long RSTRG : 5;
			unsigned long RFRST : 1;
			unsigned long  : 2;
			unsigned long RTRG : 5;
			unsigned long TFRST : 1;
			unsigned long  : 2;
			unsigned long TTRG : 5;
			unsigned long  : 7;
			unsigned long DRES : 1;
#endif
	} BIT;
	} FCR;
	char           wk2[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DECS : 1;
			unsigned long ENCS : 1;
			unsigned long SADJE : 1;
			unsigned long  : 1;
			unsigned long SBPTN : 1;
			unsigned long SYNCE : 1;
			unsigned long SBLEN : 1;
			unsigned long  : 1;
			unsigned long TPLEN : 4;
			unsigned long TPPAT : 2;
			unsigned long  : 2;
			unsigned long RPLEN : 4;
			unsigned long RPPAT : 2;
			unsigned long  : 2;
			unsigned long PFERIE : 1;
			unsigned long SYERIE : 1;
			unsigned long SBERIE : 1;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SBERIE : 1;
			unsigned long SYERIE : 1;
			unsigned long PFERIE : 1;
			unsigned long  : 2;
			unsigned long RPPAT : 2;
			unsigned long RPLEN : 4;
			unsigned long  : 2;
			unsigned long TPPAT : 2;
			unsigned long TPLEN : 4;
			unsigned long  : 1;
			unsigned long SBLEN : 1;
			unsigned long SYNCE : 1;
			unsigned long SBPTN : 1;
			unsigned long  : 1;
			unsigned long SADJE : 1;
			unsigned long ENCS : 1;
			unsigned long DECS : 1;
#endif
	} BIT;
	} MMCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DELVL : 1;
			unsigned long  : 7;
			unsigned long DESU : 5;
			unsigned long  : 3;
			unsigned long DEHLD : 5;
			unsigned long  : 11;
#else
			unsigned long  : 11;
			unsigned long DEHLD : 5;
			unsigned long  : 3;
			unsigned long DESU : 5;
			unsigned long  : 7;
			unsigned long DELVL : 1;
#endif
	} BIT;
	} DECR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TCSS : 2;
			unsigned long  : 6;
			unsigned long BFE : 1;
			unsigned long CF0RE : 1;
			unsigned long CF1DS : 2;
			unsigned long PIBE : 1;
			unsigned long PIBS : 3;
			unsigned long BFOIE : 1;
			unsigned long BCDIE : 1;
			unsigned long  : 2;
			unsigned long BFDIE : 1;
			unsigned long COFIE : 1;
			unsigned long AEDIE : 1;
			unsigned long  : 1;
			unsigned long BCCS : 2;
			unsigned long  : 6;
#else
			unsigned long  : 6;
			unsigned long BCCS : 2;
			unsigned long  : 1;
			unsigned long AEDIE : 1;
			unsigned long COFIE : 1;
			unsigned long BFDIE : 1;
			unsigned long  : 2;
			unsigned long BCDIE : 1;
			unsigned long BFOIE : 1;
			unsigned long PIBS : 3;
			unsigned long PIBE : 1;
			unsigned long CF1DS : 2;
			unsigned long CF0RE : 1;
			unsigned long BFE : 1;
			unsigned long  : 6;
			unsigned long TCSS : 2;
#endif
	} BIT;
	} XCR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long TCST : 1;
			unsigned long  : 3;
			unsigned long SDST : 1;
			unsigned long BRME : 1;
			unsigned long  : 2;
			unsigned long PCF1D : 8;
			unsigned long SCF1D : 8;
			unsigned long CF1CE : 8;
#else
			unsigned long CF1CE : 8;
			unsigned long SCF1D : 8;
			unsigned long PCF1D : 8;
			unsigned long  : 2;
			unsigned long BRME : 1;
			unsigned long SDST : 1;
			unsigned long  : 3;
			unsigned long TCST : 1;
#endif
	} BIT;
	} XCR1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CF0D : 8;
			unsigned long CF0CE : 8;
			unsigned long BFLW : 16;
#else
			unsigned long BFLW : 16;
			unsigned long CF0CE : 8;
			unsigned long CF0D : 8;
#endif
	} BIT;
	} XCR2;
	char           wk3[8];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long ERS : 1;
			unsigned long  : 10;
			unsigned long RXDMON : 1;
			unsigned long DCMF : 1;
			unsigned long DPER : 1;
			unsigned long DFER : 1;
			unsigned long  : 5;
			unsigned long ORER : 1;
			unsigned long  : 1;
			unsigned long MFF : 1;
			unsigned long APER : 1;
			unsigned long AFER : 1;
			unsigned long TDRE : 1;
			unsigned long TEND : 1;
			unsigned long RDRF : 1;
#else
			unsigned long RDRF : 1;
			unsigned long TEND : 1;
			unsigned long TDRE : 1;
			unsigned long AFER : 1;
			unsigned long APER : 1;
			unsigned long MFF : 1;
			unsigned long  : 1;
			unsigned long ORER : 1;
			unsigned long  : 5;
			unsigned long DFER : 1;
			unsigned long DPER : 1;
			unsigned long DCMF : 1;
			unsigned long RXDMON : 1;
			unsigned long  : 10;
			unsigned long ERS : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} SSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long IICACKR : 1;
			unsigned long  : 2;
			unsigned long IICSTIF : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long IICSTIF : 1;
			unsigned long  : 2;
			unsigned long IICACKR : 1;
#endif
	} BIT;
	} SISR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DR : 1;
			unsigned long  : 7;
			unsigned long R : 6;
			unsigned long  : 2;
			unsigned long PEC : 6;
			unsigned long  : 2;
			unsigned long FEC : 6;
			unsigned long  : 2;
#else
			unsigned long  : 2;
			unsigned long FEC : 6;
			unsigned long  : 2;
			unsigned long PEC : 6;
			unsigned long  : 2;
			unsigned long R : 6;
			unsigned long  : 7;
			unsigned long DR : 1;
#endif
	} BIT;
	} RFSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long T : 6;
			unsigned long  : 26;
#else
			unsigned long  : 26;
			unsigned long T : 6;
#endif
	} BIT;
	} TFSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PFER : 1;
			unsigned long SYER : 1;
			unsigned long SBER : 1;
			unsigned long  : 1;
			unsigned long MCER : 1;
			unsigned long  : 1;
			unsigned long RSYNC : 1;
			unsigned long  : 25;
#else
			unsigned long  : 25;
			unsigned long RSYNC : 1;
			unsigned long  : 1;
			unsigned long MCER : 1;
			unsigned long  : 1;
			unsigned long SBER : 1;
			unsigned long SYER : 1;
			unsigned long PFER : 1;
#endif
	} BIT;
	} MMSR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SFSF : 1;
			unsigned long RXDSF : 1;
			unsigned long  : 6;
			unsigned long BFOF : 1;
			unsigned long BCDF : 1;
			unsigned long BFDF : 1;
			unsigned long CF0MF : 1;
			unsigned long CF1MF : 1;
			unsigned long PIBDF : 1;
			unsigned long COF : 1;
			unsigned long AEDF : 1;
			unsigned long CF0RD : 8;
			unsigned long CF1RD : 8;
#else
			unsigned long CF1RD : 8;
			unsigned long CF0RD : 8;
			unsigned long AEDF : 1;
			unsigned long COF : 1;
			unsigned long PIBDF : 1;
			unsigned long CF1MF : 1;
			unsigned long CF0MF : 1;
			unsigned long BFDF : 1;
			unsigned long BCDF : 1;
			unsigned long BFOF : 1;
			unsigned long  : 6;
			unsigned long RXDSF : 1;
			unsigned long SFSF : 1;
#endif
	} BIT;
	} XSR0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CCV : 16;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long CCV : 16;
#endif
	} BIT;
	} XSR1;
	char           wk4[4];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long ERSC : 1;
			unsigned long  : 11;
			unsigned long DCMFC : 1;
			unsigned long DPERC : 1;
			unsigned long DFERC : 1;
			unsigned long  : 5;
			unsigned long ORERC : 1;
			unsigned long  : 1;
			unsigned long MFFC : 1;
			unsigned long APERC : 1;
			unsigned long AFERC : 1;
			unsigned long TDREC : 1;
			unsigned long  : 1;
			unsigned long RDRFC : 1;
#else
			unsigned long RDRFC : 1;
			unsigned long  : 1;
			unsigned long TDREC : 1;
			unsigned long AFERC : 1;
			unsigned long APERC : 1;
			unsigned long MFFC : 1;
			unsigned long  : 1;
			unsigned long ORERC : 1;
			unsigned long  : 5;
			unsigned long DFERC : 1;
			unsigned long DPERC : 1;
			unsigned long DCMFC : 1;
			unsigned long  : 11;
			unsigned long ERSC : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} SSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 3;
			unsigned long IICSTIFC : 1;
			unsigned long  : 28;
#else
			unsigned long  : 28;
			unsigned long IICSTIFC : 1;
			unsigned long  : 3;
#endif
	} BIT;
	} SISCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long DRC : 1;
			unsigned long  : 31;
#else
			unsigned long  : 31;
			unsigned long DRC : 1;
#endif
	} BIT;
	} RFSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PFERC : 1;
			unsigned long SYERC : 1;
			unsigned long SBERC : 1;
			unsigned long  : 1;
			unsigned long MCERC : 1;
			unsigned long  : 27;
#else
			unsigned long  : 27;
			unsigned long MCERC : 1;
			unsigned long  : 1;
			unsigned long SBERC : 1;
			unsigned long SYERC : 1;
			unsigned long PFERC : 1;
#endif
	} BIT;
	} MMSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 8;
			unsigned long BFOC : 1;
			unsigned long BCDCL : 1;
			unsigned long BFDCL : 1;
			unsigned long CF0MCL : 1;
			unsigned long CF1MCL : 1;
			unsigned long PIBDCL : 1;
			unsigned long COFC : 1;
			unsigned long AEDCL : 1;
			unsigned long  : 16;
#else
			unsigned long  : 16;
			unsigned long AEDCL : 1;
			unsigned long COFC : 1;
			unsigned long PIBDCL : 1;
			unsigned long CF1MCL : 1;
			unsigned long CF0MCL : 1;
			unsigned long BFDCL : 1;
			unsigned long BCDCL : 1;
			unsigned long BFOC : 1;
			unsigned long  : 8;
#endif
	} BIT;
	} XSCR;
} st_rsci11_t;

typedef struct st_rspi {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPMS : 1;
			unsigned char TXMD : 1;
			unsigned char MODFEN : 1;
			unsigned char MSTR : 1;
			unsigned char SPEIE : 1;
			unsigned char SPTIE : 1;
			unsigned char SPE : 1;
			unsigned char SPRIE : 1;
#else
			unsigned char SPRIE : 1;
			unsigned char SPE : 1;
			unsigned char SPTIE : 1;
			unsigned char SPEIE : 1;
			unsigned char MSTR : 1;
			unsigned char MODFEN : 1;
			unsigned char TXMD : 1;
			unsigned char SPMS : 1;
#endif
	} BIT;
	} SPCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SSL0P : 1;
			unsigned char SSL1P : 1;
			unsigned char SSL2P : 1;
			unsigned char SSL3P : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char SSL3P : 1;
			unsigned char SSL2P : 1;
			unsigned char SSL1P : 1;
			unsigned char SSL0P : 1;
#endif
	} BIT;
	} SSLP;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPLP : 1;
			unsigned char SPLP2 : 1;
			unsigned char  : 2;
			unsigned char MOIFV : 1;
			unsigned char MOIFE : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char MOIFE : 1;
			unsigned char MOIFV : 1;
			unsigned char  : 2;
			unsigned char SPLP2 : 1;
			unsigned char SPLP : 1;
#endif
	} BIT;
	} SPPCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OVRF : 1;
			unsigned char IDLNF : 1;
			unsigned char MODF : 1;
			unsigned char PERF : 1;
			unsigned char UDRF : 1;
			unsigned char SPTEF : 1;
			unsigned char SPCF : 1;
			unsigned char SPRF : 1;
#else
			unsigned char SPRF : 1;
			unsigned char SPCF : 1;
			unsigned char SPTEF : 1;
			unsigned char UDRF : 1;
			unsigned char PERF : 1;
			unsigned char MODF : 1;
			unsigned char IDLNF : 1;
			unsigned char OVRF : 1;
#endif
	} BIT;
	} SPSR;
	union {
		unsigned long LONG;
		struct {
			unsigned short H;
		} WORD;
		struct {
			unsigned char HH;
		} BYTE;
	} SPDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPSLN : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SPSLN : 3;
#endif
	} BIT;
	} SPSCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPCP : 3;
			unsigned char  : 1;
			unsigned char SPECM : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char SPECM : 3;
			unsigned char  : 1;
			unsigned char SPCP : 3;
#endif
	} BIT;
	} SPSSR;
	unsigned char  SPBR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPFC : 2;
			unsigned char  : 2;
			unsigned char SPRDTD : 1;
			unsigned char SPLW : 1;
			unsigned char SPBYT : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char SPBYT : 1;
			unsigned char SPLW : 1;
			unsigned char SPRDTD : 1;
			unsigned char  : 2;
			unsigned char SPFC : 2;
#endif
	} BIT;
	} SPDCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SCKDL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SCKDL : 3;
#endif
	} BIT;
	} SPCKD;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLNDL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SLNDL : 3;
#endif
	} BIT;
	} SSLND;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPNDL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SPNDL : 3;
#endif
	} BIT;
	} SPND;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPPE : 1;
			unsigned char SPOE : 1;
			unsigned char SPIIE : 1;
			unsigned char PTE : 1;
			unsigned char SCKASE : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char SCKASE : 1;
			unsigned char PTE : 1;
			unsigned char SPIIE : 1;
			unsigned char SPOE : 1;
			unsigned char SPPE : 1;
#endif
	} BIT;
	} SPCR2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD3;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD4;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD5;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD6;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CPHA : 1;
			unsigned short CPOL : 1;
			unsigned short BRDV : 2;
			unsigned short SSLA : 3;
			unsigned short SSLKP : 1;
			unsigned short SPB : 4;
			unsigned short LSBF : 1;
			unsigned short SPNDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SCKDEN : 1;
#else
			unsigned short SCKDEN : 1;
			unsigned short SLNDEN : 1;
			unsigned short SPNDEN : 1;
			unsigned short LSBF : 1;
			unsigned short SPB : 4;
			unsigned short SSLKP : 1;
			unsigned short SSLA : 3;
			unsigned short BRDV : 2;
			unsigned short CPOL : 1;
			unsigned short CPHA : 1;
#endif
	} BIT;
	} SPCMD7;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BYSW : 1;
			unsigned char DINV : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char DINV : 1;
			unsigned char BYSW : 1;
#endif
	} BIT;
	} SPDCR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RXMD : 1;
			unsigned char SCKDDIS : 1;
			unsigned char  : 2;
			unsigned char SPCIE : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char SPCIE : 1;
			unsigned char  : 2;
			unsigned char SCKDDIS : 1;
			unsigned char RXMD : 1;
#endif
	} BIT;
	} SPCR3;
} st_rspi_t;

typedef struct st_rspia {
	union {
		unsigned long LONG;
		struct {
			unsigned short H;
		} WORD;
		struct {
			unsigned char HH;
		} BYTE;
	} SPDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SCKDL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SCKDL : 3;
#endif
	} BIT;
	} SPCKD;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SLNDL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SLNDL : 3;
#endif
	} BIT;
	} SSLND;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPNDL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SPNDL : 3;
#endif
	} BIT;
	} SPND;
	char           wk0[1];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long SPE : 1;
			unsigned long  : 6;
			unsigned long MRCKS : 1;
			unsigned long SPPE : 1;
			unsigned long SPOE : 1;
			unsigned long  : 1;
			unsigned long PTE : 1;
			unsigned long SCKASE : 1;
			unsigned long SCKDDIS : 1;
			unsigned long MODFEN : 1;
			unsigned long  : 1;
			unsigned long SPEIE : 1;
			unsigned long SPRIE : 1;
			unsigned long SPIIE : 1;
			unsigned long RDRIS : 1;
			unsigned long SPTIE : 1;
			unsigned long SPCIE : 1;
			unsigned long  : 2;
			unsigned long SPMS : 1;
			unsigned long FRFS : 1;
			unsigned long  : 2;
			unsigned long CMMD : 2;
			unsigned long MSTR : 1;
			unsigned long SYNDIS : 1;
#else
			unsigned long SYNDIS : 1;
			unsigned long MSTR : 1;
			unsigned long CMMD : 2;
			unsigned long  : 2;
			unsigned long FRFS : 1;
			unsigned long SPMS : 1;
			unsigned long  : 2;
			unsigned long SPCIE : 1;
			unsigned long SPTIE : 1;
			unsigned long RDRIS : 1;
			unsigned long SPIIE : 1;
			unsigned long SPRIE : 1;
			unsigned long SPEIE : 1;
			unsigned long  : 1;
			unsigned long MODFEN : 1;
			unsigned long SCKDDIS : 1;
			unsigned long SCKASE : 1;
			unsigned long PTE : 1;
			unsigned long  : 1;
			unsigned long SPOE : 1;
			unsigned long SPPE : 1;
			unsigned long MRCKS : 1;
			unsigned long  : 6;
			unsigned long SPE : 1;
#endif
	} BIT;
	} SPCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RFC : 5;
			unsigned char  : 1;
			unsigned char TERM : 1;
			unsigned char START : 1;
#else
			unsigned char START : 1;
			unsigned char TERM : 1;
			unsigned char  : 1;
			unsigned char RFC : 5;
#endif
	} BIT;
	} SPRMCR;
	unsigned char  SPDRCSR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPLP : 1;
			unsigned char SPLP2 : 1;
			unsigned char  : 2;
			unsigned char MOIFV : 1;
			unsigned char MOIFE : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char MOIFE : 1;
			unsigned char MOIFV : 1;
			unsigned char  : 2;
			unsigned char SPLP2 : 1;
			unsigned char SPLP : 1;
#endif
	} BIT;
	} SPPCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SSL0P : 1;
			unsigned char SSL1P : 1;
			unsigned char SSL2P : 1;
			unsigned char SSL3P : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char SSL3P : 1;
			unsigned char SSL2P : 1;
			unsigned char SSL1P : 1;
			unsigned char SSL0P : 1;
#endif
	} BIT;
	} SSLP;
	unsigned char  SPBR;
	char           wk2[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPSLN : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SPSLN : 3;
#endif
	} BIT;
	} SPSCR;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD0;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD1;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD2;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD3;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD4;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD5;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD6;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long CPHA : 1;
			unsigned long CPOL : 1;
			unsigned long BRDV : 2;
			unsigned long  : 3;
			unsigned long SSLKP : 1;
			unsigned long  : 4;
			unsigned long LSBF : 1;
			unsigned long SPNDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SCKDEN : 1;
			unsigned long SPB : 5;
			unsigned long  : 3;
			unsigned long SSLA : 3;
			unsigned long  : 5;
#else
			unsigned long  : 5;
			unsigned long SSLA : 3;
			unsigned long  : 3;
			unsigned long SPB : 5;
			unsigned long SCKDEN : 1;
			unsigned long SLNDEN : 1;
			unsigned long SPNDEN : 1;
			unsigned long LSBF : 1;
			unsigned long  : 4;
			unsigned long SSLKP : 1;
			unsigned long  : 3;
			unsigned long BRDV : 2;
			unsigned long CPOL : 1;
			unsigned long CPHA : 1;
#endif
	} BIT;
	} SPCMD7;
	char           wk3[12];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short BYSW : 1;
			unsigned short  : 2;
			unsigned short SPRDTD : 1;
			unsigned short DINV : 1;
			unsigned short  : 3;
			unsigned short SPFC : 2;
			unsigned short  : 6;
#else
			unsigned short  : 6;
			unsigned short SPFC : 2;
			unsigned short  : 3;
			unsigned short DINV : 1;
			unsigned short SPRDTD : 1;
			unsigned short  : 2;
			unsigned short BYSW : 1;
#endif
	} BIT;
	} SPDCR;
	char           wk4[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short RTRG : 2;
			unsigned short  : 6;
			unsigned short TTRG : 2;
			unsigned short  : 6;
#else
			unsigned short  : 6;
			unsigned short TTRG : 2;
			unsigned short  : 6;
			unsigned short RTRG : 2;
#endif
	} BIT;
	} SPFCR;
	char           wk5[11];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SPCP : 3;
			unsigned char  : 1;
			unsigned char SPECM : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char SPECM : 3;
			unsigned char  : 1;
			unsigned char SPCP : 3;
#endif
	} BIT;
	} SPSSR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 7;
			unsigned short RRDYF : 1;
			unsigned short OVRF : 1;
			unsigned short IDLNF : 1;
			unsigned short MODF : 1;
			unsigned short PERF : 1;
			unsigned short UDRF : 1;
			unsigned short SPTEF : 1;
			unsigned short SPCF : 1;
			unsigned short SPRF : 1;
#else
			unsigned short SPRF : 1;
			unsigned short SPCF : 1;
			unsigned short SPTEF : 1;
			unsigned short UDRF : 1;
			unsigned short PERF : 1;
			unsigned short MODF : 1;
			unsigned short IDLNF : 1;
			unsigned short OVRF : 1;
			unsigned short RRDYF : 1;
			unsigned short  : 7;
#endif
	} BIT;
	} SPSR;
	char           wk6[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FREE : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char FREE : 3;
#endif
	} BIT;
	} SPTFSR;
	char           wk7[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FILL : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char FILL : 3;
#endif
	} BIT;
	} SPRFSR;
	char           wk8[13];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 7;
			unsigned short RRDYFC : 1;
			unsigned short OVRFC : 1;
			unsigned short  : 1;
			unsigned short MODFC : 1;
			unsigned short PERFC : 1;
			unsigned short UDRFC : 1;
			unsigned short SPTEFC : 1;
			unsigned short SPCFC : 1;
			unsigned short SPRFC : 1;
#else
			unsigned short SPRFC : 1;
			unsigned short SPCFC : 1;
			unsigned short SPTEFC : 1;
			unsigned short UDRFC : 1;
			unsigned short PERFC : 1;
			unsigned short MODFC : 1;
			unsigned short  : 1;
			unsigned short OVRFC : 1;
			unsigned short RRDYFC : 1;
			unsigned short  : 7;
#endif
	} BIT;
	} SPSCLR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char FCLR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char FCLR : 1;
#endif
	} BIT;
	} SPFCLR;
} st_rspia_t;

typedef struct st_s12ad {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DBLANS : 5;
			unsigned short  : 1;
			unsigned short GBADIE : 1;
			unsigned short DBLE : 1;
			unsigned short EXTRG : 1;
			unsigned short TRGE : 1;
			unsigned short  : 2;
			unsigned short ADIE : 1;
			unsigned short ADCS : 2;
			unsigned short ADST : 1;
#else
			unsigned short ADST : 1;
			unsigned short ADCS : 2;
			unsigned short ADIE : 1;
			unsigned short  : 2;
			unsigned short TRGE : 1;
			unsigned short EXTRG : 1;
			unsigned short DBLE : 1;
			unsigned short GBADIE : 1;
			unsigned short  : 1;
			unsigned short DBLANS : 5;
#endif
	} BIT;
	} ADCSR;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSA000 : 1;
			unsigned short ANSA001 : 1;
			unsigned short ANSA002 : 1;
			unsigned short ANSA003 : 1;
			unsigned short ANSA004 : 1;
			unsigned short ANSA005 : 1;
			unsigned short ANSA006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short ANSA006 : 1;
			unsigned short ANSA005 : 1;
			unsigned short ANSA004 : 1;
			unsigned short ANSA003 : 1;
			unsigned short ANSA002 : 1;
			unsigned short ANSA001 : 1;
			unsigned short ANSA000 : 1;
#endif
	} BIT;
	} ADANSA0;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ADS000 : 1;
			unsigned short ADS001 : 1;
			unsigned short ADS002 : 1;
			unsigned short ADS003 : 1;
			unsigned short ADS004 : 1;
			unsigned short ADS005 : 1;
			unsigned short ADS006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short ADS006 : 1;
			unsigned short ADS005 : 1;
			unsigned short ADS004 : 1;
			unsigned short ADS003 : 1;
			unsigned short ADS002 : 1;
			unsigned short ADS001 : 1;
			unsigned short ADS000 : 1;
#endif
	} BIT;
	} ADADS0;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ADC : 3;
			unsigned char  : 4;
			unsigned char AVEE : 1;
#else
			unsigned char AVEE : 1;
			unsigned char  : 4;
			unsigned char ADC : 3;
#endif
	} BIT;
	} ADADC;
	char           wk3[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 5;
			unsigned short ACE : 1;
			unsigned short ASE : 1;
			unsigned short  : 1;
			unsigned short DIAGVAL : 2;
			unsigned short DIAGLD : 1;
			unsigned short DIAGM : 1;
			unsigned short  : 3;
			unsigned short ADRFMT : 1;
#else
			unsigned short ADRFMT : 1;
			unsigned short  : 3;
			unsigned short DIAGM : 1;
			unsigned short DIAGLD : 1;
			unsigned short DIAGVAL : 2;
			unsigned short  : 1;
			unsigned short ASE : 1;
			unsigned short ACE : 1;
			unsigned short  : 5;
#endif
	} BIT;
	} ADCER;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short TRSB : 7;
			unsigned short  : 1;
			unsigned short TRSA : 7;
			unsigned short  : 1;
#else
			unsigned short  : 1;
			unsigned short TRSA : 7;
			unsigned short  : 1;
			unsigned short TRSB : 7;
#endif
	} BIT;
	} ADSTRGR;
	char           wk4[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSB000 : 1;
			unsigned short ANSB001 : 1;
			unsigned short ANSB002 : 1;
			unsigned short ANSB003 : 1;
			unsigned short ANSB004 : 1;
			unsigned short ANSB005 : 1;
			unsigned short ANSB006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short ANSB006 : 1;
			unsigned short ANSB005 : 1;
			unsigned short ANSB004 : 1;
			unsigned short ANSB003 : 1;
			unsigned short ANSB002 : 1;
			unsigned short ANSB001 : 1;
			unsigned short ANSB000 : 1;
#endif
	} BIT;
	} ADANSB0;
	char           wk5[2];
	unsigned short ADDBLDR;
	char           wk6[4];
	union {
		unsigned short WORD;
		union {
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short AD : 12;
			unsigned short  : 2;
			unsigned short DIAGST : 2;
#else
			unsigned short DIAGST : 2;
			unsigned short  : 2;
			unsigned short AD : 12;
#endif
	} RIGHT;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DIAGST : 2;
			unsigned short  : 2;
			unsigned short AD : 12;
#else
			unsigned short AD : 12;
			unsigned short  : 2;
			unsigned short DIAGST : 2;
#endif
	} LEFT;
		} BIT;
	} ADRD;
	unsigned short ADDR0;
	unsigned short ADDR1;
	unsigned short ADDR2;
	unsigned short ADDR3;
	unsigned short ADDR4;
	unsigned short ADDR5;
	unsigned short ADDR6;
	char           wk7[56];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short SSTSH : 8;
			unsigned short SHANS : 3;
			unsigned short  : 5;
#else
			unsigned short  : 5;
			unsigned short SHANS : 3;
			unsigned short SSTSH : 8;
#endif
	} BIT;
	} ADSHCR;
	char           wk8[18];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ADNDIS : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char ADNDIS : 5;
#endif
	} BIT;
	} ADDISCR;
	char           wk9[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SHMD : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SHMD : 1;
#endif
	} BIT;
	} ADSHMSR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELCC : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char ELCC : 3;
#endif
	} BIT;
	} ADELCCR;
	char           wk10[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PGS : 1;
			unsigned short GBRSCN : 1;
			unsigned short  : 12;
			unsigned short LGRRS : 1;
			unsigned short GBRP : 1;
#else
			unsigned short GBRP : 1;
			unsigned short LGRRS : 1;
			unsigned short  : 12;
			unsigned short GBRSCN : 1;
			unsigned short PGS : 1;
#endif
	} BIT;
	} ADGSPCR;
	char           wk11[2];
	unsigned short ADDBLDRA;
	unsigned short ADDBLDRB;
	char           wk12[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MONCOMB : 1;
			unsigned char  : 3;
			unsigned char MONCMPA : 1;
			unsigned char MONCMPB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char MONCMPB : 1;
			unsigned char MONCMPA : 1;
			unsigned char  : 3;
			unsigned char MONCOMB : 1;
#endif
	} BIT;
	} ADWINMON;
	char           wk13[3];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPAB : 2;
			unsigned short  : 7;
			unsigned short CMPBE : 1;
			unsigned short  : 1;
			unsigned short CMPAE : 1;
			unsigned short  : 1;
			unsigned short CMPBIE : 1;
			unsigned short WCMPE : 1;
			unsigned short CMPAIE : 1;
#else
			unsigned short CMPAIE : 1;
			unsigned short WCMPE : 1;
			unsigned short CMPBIE : 1;
			unsigned short  : 1;
			unsigned short CMPAE : 1;
			unsigned short  : 1;
			unsigned short CMPBE : 1;
			unsigned short  : 7;
			unsigned short CMPAB : 2;
#endif
	} BIT;
	} ADCMPCR;
	char           wk14[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPCHA000 : 1;
			unsigned short CMPCHA001 : 1;
			unsigned short CMPCHA002 : 1;
			unsigned short CMPCHA003 : 1;
			unsigned short CMPCHA004 : 1;
			unsigned short CMPCHA005 : 1;
			unsigned short CMPCHA006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short CMPCHA006 : 1;
			unsigned short CMPCHA005 : 1;
			unsigned short CMPCHA004 : 1;
			unsigned short CMPCHA003 : 1;
			unsigned short CMPCHA002 : 1;
			unsigned short CMPCHA001 : 1;
			unsigned short CMPCHA000 : 1;
#endif
	} BIT;
	} ADCMPANSR0;
	char           wk15[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPLCHA000 : 1;
			unsigned short CMPLCHA001 : 1;
			unsigned short CMPLCHA002 : 1;
			unsigned short CMPLCHA003 : 1;
			unsigned short CMPLCHA004 : 1;
			unsigned short CMPLCHA005 : 1;
			unsigned short CMPLCHA006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short CMPLCHA006 : 1;
			unsigned short CMPLCHA005 : 1;
			unsigned short CMPLCHA004 : 1;
			unsigned short CMPLCHA003 : 1;
			unsigned short CMPLCHA002 : 1;
			unsigned short CMPLCHA001 : 1;
			unsigned short CMPLCHA000 : 1;
#endif
	} BIT;
	} ADCMPLR0;
	char           wk16[2];
	unsigned short ADCMPDR0;
	unsigned short ADCMPDR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPSTCHA000 : 1;
			unsigned short CMPSTCHA001 : 1;
			unsigned short CMPSTCHA002 : 1;
			unsigned short CMPSTCHA003 : 1;
			unsigned short CMPSTCHA004 : 1;
			unsigned short CMPSTCHA005 : 1;
			unsigned short CMPSTCHA006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short CMPSTCHA006 : 1;
			unsigned short CMPSTCHA005 : 1;
			unsigned short CMPSTCHA004 : 1;
			unsigned short CMPSTCHA003 : 1;
			unsigned short CMPSTCHA002 : 1;
			unsigned short CMPSTCHA001 : 1;
			unsigned short CMPSTCHA000 : 1;
#endif
	} BIT;
	} ADCMPSR0;
	char           wk17[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPCHB : 6;
			unsigned char  : 1;
			unsigned char CMPLB : 1;
#else
			unsigned char CMPLB : 1;
			unsigned char  : 1;
			unsigned char CMPCHB : 6;
#endif
	} BIT;
	} ADCMPBNSR;
	char           wk18[1];
	unsigned short ADWINLLB;
	unsigned short ADWINULB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPSTB : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CMPSTB : 1;
#endif
	} BIT;
	} ADCMPBSR;
	char           wk19[39];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSC000 : 1;
			unsigned short ANSC001 : 1;
			unsigned short ANSC002 : 1;
			unsigned short ANSC003 : 1;
			unsigned short ANSC004 : 1;
			unsigned short ANSC005 : 1;
			unsigned short ANSC006 : 1;
			unsigned short  : 9;
#else
			unsigned short  : 9;
			unsigned short ANSC006 : 1;
			unsigned short ANSC005 : 1;
			unsigned short ANSC004 : 1;
			unsigned short ANSC003 : 1;
			unsigned short ANSC002 : 1;
			unsigned short ANSC001 : 1;
			unsigned short ANSC000 : 1;
#endif
	} BIT;
	} ADANSC0;
	char           wk20[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRSC : 6;
			unsigned char GCADIE : 1;
			unsigned char GRCE : 1;
#else
			unsigned char GRCE : 1;
			unsigned char GCADIE : 1;
			unsigned char TRSC : 6;
#endif
	} BIT;
	} ADGCTRGR;
	char           wk21[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRSC6 : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TRSC6 : 1;
#endif
	} BIT;
	} ADGCTRGR2;
	char           wk22[3];
	unsigned char  ADSSTR0;
	unsigned char  ADSSTR1;
	unsigned char  ADSSTR2;
	unsigned char  ADSSTR3;
	unsigned char  ADSSTR4;
	unsigned char  ADSSTR5;
	unsigned char  ADSSTR6;
	char           wk23[185];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short P000CR : 4;
			unsigned short P001CR : 4;
			unsigned short P002CR : 4;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short P002CR : 4;
			unsigned short P001CR : 4;
			unsigned short P000CR : 4;
#endif
	} BIT;
	} ADPGACR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short P000GAIN : 4;
			unsigned short P001GAIN : 4;
			unsigned short P002GAIN : 4;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short P002GAIN : 4;
			unsigned short P001GAIN : 4;
			unsigned short P000GAIN : 4;
#endif
	} BIT;
	} ADPGAGS0;
	char           wk24[28];
	unsigned char  ADSCS0;
	unsigned char  ADSCS1;
	unsigned char  ADSCS2;
	unsigned char  ADSCS3;
	unsigned char  ADSCS4;
	unsigned char  ADSCS5;
	unsigned char  ADSCS6;
} st_s12ad_t;

typedef struct st_s12ad1 {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DBLANS : 5;
			unsigned short  : 1;
			unsigned short GBADIE : 1;
			unsigned short DBLE : 1;
			unsigned short EXTRG : 1;
			unsigned short TRGE : 1;
			unsigned short  : 2;
			unsigned short ADIE : 1;
			unsigned short ADCS : 2;
			unsigned short ADST : 1;
#else
			unsigned short ADST : 1;
			unsigned short ADCS : 2;
			unsigned short ADIE : 1;
			unsigned short  : 2;
			unsigned short TRGE : 1;
			unsigned short EXTRG : 1;
			unsigned short DBLE : 1;
			unsigned short GBADIE : 1;
			unsigned short  : 1;
			unsigned short DBLANS : 5;
#endif
	} BIT;
	} ADCSR;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSA000 : 1;
			unsigned short ANSA001 : 1;
			unsigned short ANSA002 : 1;
			unsigned short ANSA003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short ANSA003 : 1;
			unsigned short ANSA002 : 1;
			unsigned short ANSA001 : 1;
			unsigned short ANSA000 : 1;
#endif
	} BIT;
	} ADANSA0;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ADS000 : 1;
			unsigned short ADS001 : 1;
			unsigned short ADS002 : 1;
			unsigned short ADS003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short ADS003 : 1;
			unsigned short ADS002 : 1;
			unsigned short ADS001 : 1;
			unsigned short ADS000 : 1;
#endif
	} BIT;
	} ADADS0;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ADC : 3;
			unsigned char  : 4;
			unsigned char AVEE : 1;
#else
			unsigned char AVEE : 1;
			unsigned char  : 4;
			unsigned char ADC : 3;
#endif
	} BIT;
	} ADADC;
	char           wk3[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 5;
			unsigned short ACE : 1;
			unsigned short ASE : 1;
			unsigned short  : 1;
			unsigned short DIAGVAL : 2;
			unsigned short DIAGLD : 1;
			unsigned short DIAGM : 1;
			unsigned short  : 3;
			unsigned short ADRFMT : 1;
#else
			unsigned short ADRFMT : 1;
			unsigned short  : 3;
			unsigned short DIAGM : 1;
			unsigned short DIAGLD : 1;
			unsigned short DIAGVAL : 2;
			unsigned short  : 1;
			unsigned short ASE : 1;
			unsigned short ACE : 1;
			unsigned short  : 5;
#endif
	} BIT;
	} ADCER;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short TRSB : 7;
			unsigned short  : 1;
			unsigned short TRSA : 7;
			unsigned short  : 1;
#else
			unsigned short  : 1;
			unsigned short TRSA : 7;
			unsigned short  : 1;
			unsigned short TRSB : 7;
#endif
	} BIT;
	} ADSTRGR;
	char           wk4[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSB000 : 1;
			unsigned short ANSB001 : 1;
			unsigned short ANSB002 : 1;
			unsigned short ANSB003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short ANSB003 : 1;
			unsigned short ANSB002 : 1;
			unsigned short ANSB001 : 1;
			unsigned short ANSB000 : 1;
#endif
	} BIT;
	} ADANSB0;
	char           wk5[2];
	unsigned short ADDBLDR;
	char           wk6[4];
	union {
		unsigned short WORD;
		union {
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short AD : 12;
			unsigned short  : 2;
			unsigned short DIAGST : 2;
#else
			unsigned short DIAGST : 2;
			unsigned short  : 2;
			unsigned short AD : 12;
#endif
	} RIGHT;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DIAGST : 2;
			unsigned short  : 2;
			unsigned short AD : 12;
#else
			unsigned short AD : 12;
			unsigned short  : 2;
			unsigned short DIAGST : 2;
#endif
	} LEFT;
		} BIT;
	} ADRD;
	unsigned short ADDR0;
	unsigned short ADDR1;
	unsigned short ADDR2;
	unsigned short ADDR3;
	char           wk7[62];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short SSTSH : 8;
			unsigned short SHANS : 3;
			unsigned short  : 5;
#else
			unsigned short  : 5;
			unsigned short SHANS : 3;
			unsigned short SSTSH : 8;
#endif
	} BIT;
	} ADSHCR;
	char           wk8[18];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ADNDIS : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char ADNDIS : 5;
#endif
	} BIT;
	} ADDISCR;
	char           wk9[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SHMD : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SHMD : 1;
#endif
	} BIT;
	} ADSHMSR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELCC : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char ELCC : 3;
#endif
	} BIT;
	} ADELCCR;
	char           wk10[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PGS : 1;
			unsigned short GBRSCN : 1;
			unsigned short  : 12;
			unsigned short LGRRS : 1;
			unsigned short GBRP : 1;
#else
			unsigned short GBRP : 1;
			unsigned short LGRRS : 1;
			unsigned short  : 12;
			unsigned short GBRSCN : 1;
			unsigned short PGS : 1;
#endif
	} BIT;
	} ADGSPCR;
	char           wk11[2];
	unsigned short ADDBLDRA;
	unsigned short ADDBLDRB;
	char           wk12[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MONCOMB : 1;
			unsigned char  : 3;
			unsigned char MONCMPA : 1;
			unsigned char MONCMPB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char MONCMPB : 1;
			unsigned char MONCMPA : 1;
			unsigned char  : 3;
			unsigned char MONCOMB : 1;
#endif
	} BIT;
	} ADWINMON;
	char           wk13[3];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPAB : 2;
			unsigned short  : 7;
			unsigned short CMPBE : 1;
			unsigned short  : 1;
			unsigned short CMPAE : 1;
			unsigned short  : 1;
			unsigned short CMPBIE : 1;
			unsigned short WCMPE : 1;
			unsigned short CMPAIE : 1;
#else
			unsigned short CMPAIE : 1;
			unsigned short WCMPE : 1;
			unsigned short CMPBIE : 1;
			unsigned short  : 1;
			unsigned short CMPAE : 1;
			unsigned short  : 1;
			unsigned short CMPBE : 1;
			unsigned short  : 7;
			unsigned short CMPAB : 2;
#endif
	} BIT;
	} ADCMPCR;
	char           wk14[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPCHA000 : 1;
			unsigned short CMPCHA001 : 1;
			unsigned short CMPCHA002 : 1;
			unsigned short CMPCHA003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short CMPCHA003 : 1;
			unsigned short CMPCHA002 : 1;
			unsigned short CMPCHA001 : 1;
			unsigned short CMPCHA000 : 1;
#endif
	} BIT;
	} ADCMPANSR0;
	char           wk15[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPLCHA000 : 1;
			unsigned short CMPLCHA001 : 1;
			unsigned short CMPLCHA002 : 1;
			unsigned short CMPLCHA003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short CMPLCHA003 : 1;
			unsigned short CMPLCHA002 : 1;
			unsigned short CMPLCHA001 : 1;
			unsigned short CMPLCHA000 : 1;
#endif
	} BIT;
	} ADCMPLR0;
	char           wk16[2];
	unsigned short ADCMPDR0;
	unsigned short ADCMPDR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPSTCHA000 : 1;
			unsigned short CMPSTCHA001 : 1;
			unsigned short CMPSTCHA002 : 1;
			unsigned short CMPSTCHA003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short CMPSTCHA003 : 1;
			unsigned short CMPSTCHA002 : 1;
			unsigned short CMPSTCHA001 : 1;
			unsigned short CMPSTCHA000 : 1;
#endif
	} BIT;
	} ADCMPSR0;
	char           wk17[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPCHB : 6;
			unsigned char  : 1;
			unsigned char CMPLB : 1;
#else
			unsigned char CMPLB : 1;
			unsigned char  : 1;
			unsigned char CMPCHB : 6;
#endif
	} BIT;
	} ADCMPBNSR;
	char           wk18[1];
	unsigned short ADWINLLB;
	unsigned short ADWINULB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPSTB : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CMPSTB : 1;
#endif
	} BIT;
	} ADCMPBSR;
	char           wk19[39];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSC000 : 1;
			unsigned short ANSC001 : 1;
			unsigned short ANSC002 : 1;
			unsigned short ANSC003 : 1;
			unsigned short  : 12;
#else
			unsigned short  : 12;
			unsigned short ANSC003 : 1;
			unsigned short ANSC002 : 1;
			unsigned short ANSC001 : 1;
			unsigned short ANSC000 : 1;
#endif
	} BIT;
	} ADANSC0;
	char           wk20[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRSC : 6;
			unsigned char GCADIE : 1;
			unsigned char GRCE : 1;
#else
			unsigned char GRCE : 1;
			unsigned char GCADIE : 1;
			unsigned char TRSC : 6;
#endif
	} BIT;
	} ADGCTRGR;
	char           wk21[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRSC6 : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TRSC6 : 1;
#endif
	} BIT;
	} ADGCTRGR2;
	char           wk22[3];
	unsigned char  ADSSTR0;
	unsigned char  ADSSTR1;
	unsigned char  ADSSTR2;
	unsigned char  ADSSTR3;
	char           wk23[188];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short P100CR : 4;
			unsigned short P101CR : 4;
			unsigned short P102CR : 4;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short P102CR : 4;
			unsigned short P101CR : 4;
			unsigned short P100CR : 4;
#endif
	} BIT;
	} ADPGACR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short P100GAIN : 4;
			unsigned short P101GAIN : 4;
			unsigned short P102GAIN : 4;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short P102GAIN : 4;
			unsigned short P101GAIN : 4;
			unsigned short P100GAIN : 4;
#endif
	} BIT;
	} ADPGAGS0;
	char           wk24[28];
	unsigned char  ADSCS0;
	unsigned char  ADSCS1;
	unsigned char  ADSCS2;
	unsigned char  ADSCS3;
} st_s12ad1_t;

typedef struct st_s12ad2 {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DBLANS : 5;
			unsigned short  : 1;
			unsigned short GBADIE : 1;
			unsigned short DBLE : 1;
			unsigned short EXTRG : 1;
			unsigned short TRGE : 1;
			unsigned short  : 2;
			unsigned short ADIE : 1;
			unsigned short ADCS : 2;
			unsigned short ADST : 1;
#else
			unsigned short ADST : 1;
			unsigned short ADCS : 2;
			unsigned short ADIE : 1;
			unsigned short  : 2;
			unsigned short TRGE : 1;
			unsigned short EXTRG : 1;
			unsigned short DBLE : 1;
			unsigned short GBADIE : 1;
			unsigned short  : 1;
			unsigned short DBLANS : 5;
#endif
	} BIT;
	} ADCSR;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSA000 : 1;
			unsigned short ANSA001 : 1;
			unsigned short ANSA002 : 1;
			unsigned short ANSA003 : 1;
			unsigned short ANSA004 : 1;
			unsigned short ANSA005 : 1;
			unsigned short ANSA006 : 1;
			unsigned short ANSA007 : 1;
			unsigned short ANSA008 : 1;
			unsigned short ANSA009 : 1;
			unsigned short ANSA010 : 1;
			unsigned short ANSA011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short ANSA011 : 1;
			unsigned short ANSA010 : 1;
			unsigned short ANSA009 : 1;
			unsigned short ANSA008 : 1;
			unsigned short ANSA007 : 1;
			unsigned short ANSA006 : 1;
			unsigned short ANSA005 : 1;
			unsigned short ANSA004 : 1;
			unsigned short ANSA003 : 1;
			unsigned short ANSA002 : 1;
			unsigned short ANSA001 : 1;
			unsigned short ANSA000 : 1;
#endif
	} BIT;
	} ADANSA0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSA100 : 1;
			unsigned short ANSA101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short ANSA101 : 1;
			unsigned short ANSA100 : 1;
#endif
	} BIT;
	} ADANSA1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ADS000 : 1;
			unsigned short ADS001 : 1;
			unsigned short ADS002 : 1;
			unsigned short ADS003 : 1;
			unsigned short ADS004 : 1;
			unsigned short ADS005 : 1;
			unsigned short ADS006 : 1;
			unsigned short ADS007 : 1;
			unsigned short ADS008 : 1;
			unsigned short ADS009 : 1;
			unsigned short ADS010 : 1;
			unsigned short ADS011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short ADS011 : 1;
			unsigned short ADS010 : 1;
			unsigned short ADS009 : 1;
			unsigned short ADS008 : 1;
			unsigned short ADS007 : 1;
			unsigned short ADS006 : 1;
			unsigned short ADS005 : 1;
			unsigned short ADS004 : 1;
			unsigned short ADS003 : 1;
			unsigned short ADS002 : 1;
			unsigned short ADS001 : 1;
			unsigned short ADS000 : 1;
#endif
	} BIT;
	} ADADS0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ADS100 : 1;
			unsigned short ADS101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short ADS101 : 1;
			unsigned short ADS100 : 1;
#endif
	} BIT;
	} ADADS1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ADC : 3;
			unsigned char  : 4;
			unsigned char AVEE : 1;
#else
			unsigned char AVEE : 1;
			unsigned char  : 4;
			unsigned char ADC : 3;
#endif
	} BIT;
	} ADADC;
	char           wk1[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 5;
			unsigned short ACE : 1;
			unsigned short ASE : 1;
			unsigned short  : 1;
			unsigned short DIAGVAL : 2;
			unsigned short DIAGLD : 1;
			unsigned short DIAGM : 1;
			unsigned short  : 3;
			unsigned short ADRFMT : 1;
#else
			unsigned short ADRFMT : 1;
			unsigned short  : 3;
			unsigned short DIAGM : 1;
			unsigned short DIAGLD : 1;
			unsigned short DIAGVAL : 2;
			unsigned short  : 1;
			unsigned short ASE : 1;
			unsigned short ACE : 1;
			unsigned short  : 5;
#endif
	} BIT;
	} ADCER;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short TRSB : 7;
			unsigned short  : 1;
			unsigned short TRSA : 7;
			unsigned short  : 1;
#else
			unsigned short  : 1;
			unsigned short TRSA : 7;
			unsigned short  : 1;
			unsigned short TRSB : 7;
#endif
	} BIT;
	} ADSTRGR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short TSSAD : 1;
			unsigned short OCSAD : 1;
			unsigned short  : 6;
			unsigned short TSSA : 1;
			unsigned short OCSA : 1;
			unsigned short TSSB : 1;
			unsigned short OCSB : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short OCSB : 1;
			unsigned short TSSB : 1;
			unsigned short OCSA : 1;
			unsigned short TSSA : 1;
			unsigned short  : 6;
			unsigned short OCSAD : 1;
			unsigned short TSSAD : 1;
#endif
	} BIT;
	} ADEXICR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSB000 : 1;
			unsigned short ANSB001 : 1;
			unsigned short ANSB002 : 1;
			unsigned short ANSB003 : 1;
			unsigned short ANSB004 : 1;
			unsigned short ANSB005 : 1;
			unsigned short ANSB006 : 1;
			unsigned short ANSB007 : 1;
			unsigned short ANSB008 : 1;
			unsigned short ANSB009 : 1;
			unsigned short ANSB010 : 1;
			unsigned short ANSB011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short ANSB011 : 1;
			unsigned short ANSB010 : 1;
			unsigned short ANSB009 : 1;
			unsigned short ANSB008 : 1;
			unsigned short ANSB007 : 1;
			unsigned short ANSB006 : 1;
			unsigned short ANSB005 : 1;
			unsigned short ANSB004 : 1;
			unsigned short ANSB003 : 1;
			unsigned short ANSB002 : 1;
			unsigned short ANSB001 : 1;
			unsigned short ANSB000 : 1;
#endif
	} BIT;
	} ADANSB0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSB100 : 1;
			unsigned short ANSB101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short ANSB101 : 1;
			unsigned short ANSB100 : 1;
#endif
	} BIT;
	} ADANSB1;
	unsigned short ADDBLDR;
	unsigned short ADTSDR;
	unsigned short ADOCDR;
	union {
		unsigned short WORD;
		union {
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short AD : 12;
			unsigned short  : 2;
			unsigned short DIAGST : 2;
#else
			unsigned short DIAGST : 2;
			unsigned short  : 2;
			unsigned short AD : 12;
#endif
	} RIGHT;
			struct {
				
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short DIAGST : 2;
			unsigned short  : 2;
			unsigned short AD : 12;
#else
			unsigned short AD : 12;
			unsigned short  : 2;
			unsigned short DIAGST : 2;
#endif
	} LEFT;
		} BIT;
	} ADRD;
	unsigned short ADDR0;
	unsigned short ADDR1;
	unsigned short ADDR2;
	unsigned short ADDR3;
	unsigned short ADDR4;
	unsigned short ADDR5;
	unsigned short ADDR6;
	unsigned short ADDR7;
	unsigned short ADDR8;
	unsigned short ADDR9;
	unsigned short ADDR10;
	unsigned short ADDR11;
	char           wk2[8];
	unsigned short ADDR16;
	unsigned short ADDR17;
	char           wk3[54];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ADNDIS : 5;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char ADNDIS : 5;
#endif
	} BIT;
	} ADDISCR;
	char           wk4[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ELCC : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char ELCC : 3;
#endif
	} BIT;
	} ADELCCR;
	char           wk5[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PGS : 1;
			unsigned short GBRSCN : 1;
			unsigned short  : 12;
			unsigned short LGRRS : 1;
			unsigned short GBRP : 1;
#else
			unsigned short GBRP : 1;
			unsigned short LGRRS : 1;
			unsigned short  : 12;
			unsigned short GBRSCN : 1;
			unsigned short PGS : 1;
#endif
	} BIT;
	} ADGSPCR;
	char           wk6[2];
	unsigned short ADDBLDRA;
	unsigned short ADDBLDRB;
	char           wk7[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MONCOMB : 1;
			unsigned char  : 3;
			unsigned char MONCMPA : 1;
			unsigned char MONCMPB : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char MONCMPB : 1;
			unsigned char MONCMPA : 1;
			unsigned char  : 3;
			unsigned char MONCOMB : 1;
#endif
	} BIT;
	} ADWINMON;
	char           wk8[3];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPAB : 2;
			unsigned short  : 7;
			unsigned short CMPBE : 1;
			unsigned short  : 1;
			unsigned short CMPAE : 1;
			unsigned short  : 1;
			unsigned short CMPBIE : 1;
			unsigned short WCMPE : 1;
			unsigned short CMPAIE : 1;
#else
			unsigned short CMPAIE : 1;
			unsigned short WCMPE : 1;
			unsigned short CMPBIE : 1;
			unsigned short  : 1;
			unsigned short CMPAE : 1;
			unsigned short  : 1;
			unsigned short CMPBE : 1;
			unsigned short  : 7;
			unsigned short CMPAB : 2;
#endif
	} BIT;
	} ADCMPCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPSTS : 1;
			unsigned char CMPSOC : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char CMPSOC : 1;
			unsigned char CMPSTS : 1;
#endif
	} BIT;
	} ADCMPANSER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPLTS : 1;
			unsigned char CMPLOC : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char CMPLOC : 1;
			unsigned char CMPLTS : 1;
#endif
	} BIT;
	} ADCMPLER;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPCHA000 : 1;
			unsigned short CMPCHA001 : 1;
			unsigned short CMPCHA002 : 1;
			unsigned short CMPCHA003 : 1;
			unsigned short CMPCHA004 : 1;
			unsigned short CMPCHA005 : 1;
			unsigned short CMPCHA006 : 1;
			unsigned short CMPCHA007 : 1;
			unsigned short CMPCHA008 : 1;
			unsigned short CMPCHA009 : 1;
			unsigned short CMPCHA010 : 1;
			unsigned short CMPCHA011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short CMPCHA011 : 1;
			unsigned short CMPCHA010 : 1;
			unsigned short CMPCHA009 : 1;
			unsigned short CMPCHA008 : 1;
			unsigned short CMPCHA007 : 1;
			unsigned short CMPCHA006 : 1;
			unsigned short CMPCHA005 : 1;
			unsigned short CMPCHA004 : 1;
			unsigned short CMPCHA003 : 1;
			unsigned short CMPCHA002 : 1;
			unsigned short CMPCHA001 : 1;
			unsigned short CMPCHA000 : 1;
#endif
	} BIT;
	} ADCMPANSR0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPCHA100 : 1;
			unsigned short CMPCHA101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short CMPCHA101 : 1;
			unsigned short CMPCHA100 : 1;
#endif
	} BIT;
	} ADCMPANSR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPLCHA000 : 1;
			unsigned short CMPLCHA001 : 1;
			unsigned short CMPLCHA002 : 1;
			unsigned short CMPLCHA003 : 1;
			unsigned short CMPLCHA004 : 1;
			unsigned short CMPLCHA005 : 1;
			unsigned short CMPLCHA006 : 1;
			unsigned short CMPLCHA007 : 1;
			unsigned short CMPLCHA008 : 1;
			unsigned short CMPLCHA009 : 1;
			unsigned short CMPLCHA010 : 1;
			unsigned short CMPLCHA011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short CMPLCHA011 : 1;
			unsigned short CMPLCHA010 : 1;
			unsigned short CMPLCHA009 : 1;
			unsigned short CMPLCHA008 : 1;
			unsigned short CMPLCHA007 : 1;
			unsigned short CMPLCHA006 : 1;
			unsigned short CMPLCHA005 : 1;
			unsigned short CMPLCHA004 : 1;
			unsigned short CMPLCHA003 : 1;
			unsigned short CMPLCHA002 : 1;
			unsigned short CMPLCHA001 : 1;
			unsigned short CMPLCHA000 : 1;
#endif
	} BIT;
	} ADCMPLR0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPLCHA100 : 1;
			unsigned short CMPLCHA101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short CMPLCHA101 : 1;
			unsigned short CMPLCHA100 : 1;
#endif
	} BIT;
	} ADCMPLR1;
	unsigned short ADCMPDR0;
	unsigned short ADCMPDR1;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPSTCHA000 : 1;
			unsigned short CMPSTCHA001 : 1;
			unsigned short CMPSTCHA002 : 1;
			unsigned short CMPSTCHA003 : 1;
			unsigned short CMPSTCHA004 : 1;
			unsigned short CMPSTCHA005 : 1;
			unsigned short CMPSTCHA006 : 1;
			unsigned short CMPSTCHA007 : 1;
			unsigned short CMPSTCHA008 : 1;
			unsigned short CMPSTCHA009 : 1;
			unsigned short CMPSTCHA010 : 1;
			unsigned short CMPSTCHA011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short CMPSTCHA011 : 1;
			unsigned short CMPSTCHA010 : 1;
			unsigned short CMPSTCHA009 : 1;
			unsigned short CMPSTCHA008 : 1;
			unsigned short CMPSTCHA007 : 1;
			unsigned short CMPSTCHA006 : 1;
			unsigned short CMPSTCHA005 : 1;
			unsigned short CMPSTCHA004 : 1;
			unsigned short CMPSTCHA003 : 1;
			unsigned short CMPSTCHA002 : 1;
			unsigned short CMPSTCHA001 : 1;
			unsigned short CMPSTCHA000 : 1;
#endif
	} BIT;
	} ADCMPSR0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPSTCHA100 : 1;
			unsigned short CMPSTCHA101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short CMPSTCHA101 : 1;
			unsigned short CMPSTCHA100 : 1;
#endif
	} BIT;
	} ADCMPSR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPFTS : 1;
			unsigned char CMPFOC : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char CMPFOC : 1;
			unsigned char CMPFTS : 1;
#endif
	} BIT;
	} ADCMPSER;
	char           wk9[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPCHB : 6;
			unsigned char  : 1;
			unsigned char CMPLB : 1;
#else
			unsigned char CMPLB : 1;
			unsigned char  : 1;
			unsigned char CMPCHB : 6;
#endif
	} BIT;
	} ADCMPBNSR;
	char           wk10[1];
	unsigned short ADWINLLB;
	unsigned short ADWINULB;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CMPSTB : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CMPSTB : 1;
#endif
	} BIT;
	} ADCMPBSR;
	char           wk11[39];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSC000 : 1;
			unsigned short ANSC001 : 1;
			unsigned short ANSC002 : 1;
			unsigned short ANSC003 : 1;
			unsigned short ANSC004 : 1;
			unsigned short ANSC005 : 1;
			unsigned short ANSC006 : 1;
			unsigned short ANSC007 : 1;
			unsigned short ANSC008 : 1;
			unsigned short ANSC009 : 1;
			unsigned short ANSC010 : 1;
			unsigned short ANSC011 : 1;
			unsigned short  : 4;
#else
			unsigned short  : 4;
			unsigned short ANSC011 : 1;
			unsigned short ANSC010 : 1;
			unsigned short ANSC009 : 1;
			unsigned short ANSC008 : 1;
			unsigned short ANSC007 : 1;
			unsigned short ANSC006 : 1;
			unsigned short ANSC005 : 1;
			unsigned short ANSC004 : 1;
			unsigned short ANSC003 : 1;
			unsigned short ANSC002 : 1;
			unsigned short ANSC001 : 1;
			unsigned short ANSC000 : 1;
#endif
	} BIT;
	} ADANSC0;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short ANSC100 : 1;
			unsigned short ANSC101 : 1;
			unsigned short  : 14;
#else
			unsigned short  : 14;
			unsigned short ANSC101 : 1;
			unsigned short ANSC100 : 1;
#endif
	} BIT;
	} ADANSC1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TSSC : 1;
			unsigned char OCSC : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char OCSC : 1;
			unsigned char TSSC : 1;
#endif
	} BIT;
	} ADGCEXCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRSC : 6;
			unsigned char GCADIE : 1;
			unsigned char GRCE : 1;
#else
			unsigned char GRCE : 1;
			unsigned char GCADIE : 1;
			unsigned char TRSC : 6;
#endif
	} BIT;
	} ADGCTRGR;
	char           wk12[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TRSC6 : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TRSC6 : 1;
#endif
	} BIT;
	} ADGCTRGR2;
	unsigned char  ADSSTRL;
	unsigned char  ADSSTRT;
	unsigned char  ADSSTRO;
	unsigned char  ADSSTR0;
	unsigned char  ADSSTR1;
	unsigned char  ADSSTR2;
	unsigned char  ADSSTR3;
	unsigned char  ADSSTR4;
	unsigned char  ADSSTR5;
	unsigned char  ADSSTR6;
	unsigned char  ADSSTR7;
	unsigned char  ADSSTR8;
	unsigned char  ADSSTR9;
	unsigned char  ADSSTR10;
	unsigned char  ADSSTR11;
	char           wk13[212];
	unsigned char  ADSCS0;
	unsigned char  ADSCS1;
	unsigned char  ADSCS2;
	unsigned char  ADSCS3;
	unsigned char  ADSCS4;
	unsigned char  ADSCS5;
	unsigned char  ADSCS6;
	unsigned char  ADSCS7;
	unsigned char  ADSCS8;
	unsigned char  ADSCS9;
	unsigned char  ADSCS10;
	unsigned char  ADSCS11;
	char           wk14[4];
	unsigned char  ADSCS12;
	unsigned char  ADSCS13;
	char           wk15[16];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char VDE : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char VDE : 1;
#endif
	} BIT;
	} ADVMONCR;
	char           wk16[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char VDO : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char VDO : 1;
#endif
	} BIT;
	} ADVMONO;
} st_s12ad2_t;

typedef struct st_sci1 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 2;
			unsigned char MP : 1;
			unsigned char STOP : 1;
			unsigned char PM : 1;
			unsigned char PE : 1;
			unsigned char CHR : 1;
			unsigned char CM : 1;
#else
			unsigned char CM : 1;
			unsigned char CHR : 1;
			unsigned char PE : 1;
			unsigned char PM : 1;
			unsigned char STOP : 1;
			unsigned char MP : 1;
			unsigned char CKS : 2;
#endif
	} BIT;
	} SMR;
	unsigned char  BRR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKE : 2;
			unsigned char TEIE : 1;
			unsigned char MPIE : 1;
			unsigned char RE : 1;
			unsigned char TE : 1;
			unsigned char RIE : 1;
			unsigned char TIE : 1;
#else
			unsigned char TIE : 1;
			unsigned char RIE : 1;
			unsigned char TE : 1;
			unsigned char RE : 1;
			unsigned char MPIE : 1;
			unsigned char TEIE : 1;
			unsigned char CKE : 2;
#endif
	} BIT;
	} SCR;
	unsigned char  TDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MPBT : 1;
			unsigned char MPB : 1;
			unsigned char TEND : 1;
			unsigned char PER : 1;
			unsigned char FER : 1;
			unsigned char ORER : 1;
			unsigned char RDRF : 1;
			unsigned char TDRE : 1;
#else
			unsigned char TDRE : 1;
			unsigned char RDRF : 1;
			unsigned char ORER : 1;
			unsigned char FER : 1;
			unsigned char PER : 1;
			unsigned char TEND : 1;
			unsigned char MPB : 1;
			unsigned char MPBT : 1;
#endif
	} BIT;
	} SSR;
	unsigned char  RDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SMIF : 1;
			unsigned char  : 1;
			unsigned char SINV : 1;
			unsigned char SDIR : 1;
			unsigned char CHR1 : 1;
			unsigned char  : 2;
			unsigned char BCP2 : 1;
#else
			unsigned char BCP2 : 1;
			unsigned char  : 2;
			unsigned char CHR1 : 1;
			unsigned char SDIR : 1;
			unsigned char SINV : 1;
			unsigned char  : 1;
			unsigned char SMIF : 1;
#endif
	} BIT;
	} SCMR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ACS0 : 1;
			unsigned char ITE : 1;
			unsigned char BRME : 1;
			unsigned char ABCSE : 1;
			unsigned char ABCS : 1;
			unsigned char NFEN : 1;
			unsigned char BGDM : 1;
			unsigned char RXDESEL : 1;
#else
			unsigned char RXDESEL : 1;
			unsigned char BGDM : 1;
			unsigned char NFEN : 1;
			unsigned char ABCS : 1;
			unsigned char ABCSE : 1;
			unsigned char BRME : 1;
			unsigned char ITE : 1;
			unsigned char ACS0 : 1;
#endif
	} BIT;
	} SEMR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFCS : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char NFCS : 3;
#endif
	} BIT;
	} SNFR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICM : 1;
			unsigned char  : 2;
			unsigned char IICDL : 5;
#else
			unsigned char IICDL : 5;
			unsigned char  : 2;
			unsigned char IICM : 1;
#endif
	} BIT;
	} SIMR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICINTM : 1;
			unsigned char IICCSC : 1;
			unsigned char  : 3;
			unsigned char IICACKT : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char IICACKT : 1;
			unsigned char  : 3;
			unsigned char IICCSC : 1;
			unsigned char IICINTM : 1;
#endif
	} BIT;
	} SIMR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICSTAREQ : 1;
			unsigned char IICRSTAREQ : 1;
			unsigned char IICSTPREQ : 1;
			unsigned char IICSTIF : 1;
			unsigned char IICSDAS : 2;
			unsigned char IICSCLS : 2;
#else
			unsigned char IICSCLS : 2;
			unsigned char IICSDAS : 2;
			unsigned char IICSTIF : 1;
			unsigned char IICSTPREQ : 1;
			unsigned char IICRSTAREQ : 1;
			unsigned char IICSTAREQ : 1;
#endif
	} BIT;
	} SIMR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICACKR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char IICACKR : 1;
#endif
	} BIT;
	} SISR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SSE : 1;
			unsigned char CTSE : 1;
			unsigned char MSS : 1;
			unsigned char  : 1;
			unsigned char MFF : 1;
			unsigned char  : 1;
			unsigned char CKPOL : 1;
			unsigned char CKPH : 1;
#else
			unsigned char CKPH : 1;
			unsigned char CKPOL : 1;
			unsigned char  : 1;
			unsigned char MFF : 1;
			unsigned char  : 1;
			unsigned char MSS : 1;
			unsigned char CTSE : 1;
			unsigned char SSE : 1;
#endif
	} BIT;
	} SPMR;
	union {
		unsigned short WORD;
		struct {
			unsigned char TDRH;
			unsigned char TDRL;
		} BYTE;
	} TDRHL;
	union {
		unsigned short WORD;
		struct {
			unsigned char RDRH;
			unsigned char RDRL;
		} BYTE;
	} RDRHL;
	unsigned char  MDDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DCMF : 1;
			unsigned char  : 2;
			unsigned char DPER : 1;
			unsigned char DFER : 1;
			unsigned char  : 1;
			unsigned char IDSEL : 1;
			unsigned char DCME : 1;
#else
			unsigned char DCME : 1;
			unsigned char IDSEL : 1;
			unsigned char  : 1;
			unsigned char DFER : 1;
			unsigned char DPER : 1;
			unsigned char  : 2;
			unsigned char DCMF : 1;
#endif
	} BIT;
	} DCCR;
	char           wk0[6];
	union {
		unsigned short WORD;
		struct {
			unsigned char H;
			unsigned char L;
		} BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CMPD : 9;
			unsigned short  : 7;
#else
			unsigned short  : 7;
			unsigned short CMPD : 9;
#endif
	} BIT;
	} CDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RXDMON : 1;
			unsigned char SPB2DT : 1;
			unsigned char SPB2IO : 1;
			unsigned char  : 1;
			unsigned char RINV : 1;
			unsigned char TINV : 1;
			unsigned char RTADJ : 1;
			unsigned char TTADJ : 1;
#else
			unsigned char TTADJ : 1;
			unsigned char RTADJ : 1;
			unsigned char TINV : 1;
			unsigned char RINV : 1;
			unsigned char  : 1;
			unsigned char SPB2IO : 1;
			unsigned char SPB2DT : 1;
			unsigned char RXDMON : 1;
#endif
	} BIT;
	} SPTR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RTMG : 4;
			unsigned char TTMG : 4;
#else
			unsigned char TTMG : 4;
			unsigned char RTMG : 4;
#endif
	} BIT;
	} TMGR;
} st_sci1_t;

typedef struct st_sci12 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 2;
			unsigned char MP : 1;
			unsigned char STOP : 1;
			unsigned char PM : 1;
			unsigned char PE : 1;
			unsigned char CHR : 1;
			unsigned char CM : 1;
#else
			unsigned char CM : 1;
			unsigned char CHR : 1;
			unsigned char PE : 1;
			unsigned char PM : 1;
			unsigned char STOP : 1;
			unsigned char MP : 1;
			unsigned char CKS : 2;
#endif
	} BIT;
	} SMR;
	unsigned char  BRR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKE : 2;
			unsigned char TEIE : 1;
			unsigned char MPIE : 1;
			unsigned char RE : 1;
			unsigned char TE : 1;
			unsigned char RIE : 1;
			unsigned char TIE : 1;
#else
			unsigned char TIE : 1;
			unsigned char RIE : 1;
			unsigned char TE : 1;
			unsigned char RE : 1;
			unsigned char MPIE : 1;
			unsigned char TEIE : 1;
			unsigned char CKE : 2;
#endif
	} BIT;
	} SCR;
	unsigned char  TDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MPBT : 1;
			unsigned char MPB : 1;
			unsigned char TEND : 1;
			unsigned char PER : 1;
			unsigned char FER : 1;
			unsigned char ORER : 1;
			unsigned char RDRF : 1;
			unsigned char TDRE : 1;
#else
			unsigned char TDRE : 1;
			unsigned char RDRF : 1;
			unsigned char ORER : 1;
			unsigned char FER : 1;
			unsigned char PER : 1;
			unsigned char TEND : 1;
			unsigned char MPB : 1;
			unsigned char MPBT : 1;
#endif
	} BIT;
	} SSR;
	unsigned char  RDR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SMIF : 1;
			unsigned char  : 1;
			unsigned char SINV : 1;
			unsigned char SDIR : 1;
			unsigned char CHR1 : 1;
			unsigned char  : 2;
			unsigned char BCP2 : 1;
#else
			unsigned char BCP2 : 1;
			unsigned char  : 2;
			unsigned char CHR1 : 1;
			unsigned char SDIR : 1;
			unsigned char SINV : 1;
			unsigned char  : 1;
			unsigned char SMIF : 1;
#endif
	} BIT;
	} SCMR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ACS0 : 1;
			unsigned char ITE : 1;
			unsigned char BRME : 1;
			unsigned char  : 1;
			unsigned char ABCS : 1;
			unsigned char NFEN : 1;
			unsigned char BGDM : 1;
			unsigned char RXDESEL : 1;
#else
			unsigned char RXDESEL : 1;
			unsigned char BGDM : 1;
			unsigned char NFEN : 1;
			unsigned char ABCS : 1;
			unsigned char  : 1;
			unsigned char BRME : 1;
			unsigned char ITE : 1;
			unsigned char ACS0 : 1;
#endif
	} BIT;
	} SEMR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char NFCS : 3;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char NFCS : 3;
#endif
	} BIT;
	} SNFR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICM : 1;
			unsigned char  : 2;
			unsigned char IICDL : 5;
#else
			unsigned char IICDL : 5;
			unsigned char  : 2;
			unsigned char IICM : 1;
#endif
	} BIT;
	} SIMR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICINTM : 1;
			unsigned char IICCSC : 1;
			unsigned char  : 3;
			unsigned char IICACKT : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char IICACKT : 1;
			unsigned char  : 3;
			unsigned char IICCSC : 1;
			unsigned char IICINTM : 1;
#endif
	} BIT;
	} SIMR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICSTAREQ : 1;
			unsigned char IICRSTAREQ : 1;
			unsigned char IICSTPREQ : 1;
			unsigned char IICSTIF : 1;
			unsigned char IICSDAS : 2;
			unsigned char IICSCLS : 2;
#else
			unsigned char IICSCLS : 2;
			unsigned char IICSDAS : 2;
			unsigned char IICSTIF : 1;
			unsigned char IICSTPREQ : 1;
			unsigned char IICRSTAREQ : 1;
			unsigned char IICSTAREQ : 1;
#endif
	} BIT;
	} SIMR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IICACKR : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char IICACKR : 1;
#endif
	} BIT;
	} SISR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SSE : 1;
			unsigned char CTSE : 1;
			unsigned char MSS : 1;
			unsigned char  : 1;
			unsigned char MFF : 1;
			unsigned char  : 1;
			unsigned char CKPOL : 1;
			unsigned char CKPH : 1;
#else
			unsigned char CKPH : 1;
			unsigned char CKPOL : 1;
			unsigned char  : 1;
			unsigned char MFF : 1;
			unsigned char  : 1;
			unsigned char MSS : 1;
			unsigned char CTSE : 1;
			unsigned char SSE : 1;
#endif
	} BIT;
	} SPMR;
	union {
		unsigned short WORD;
		struct {
			unsigned char TDRH;
			unsigned char TDRL;
		} BYTE;
	} TDRHL;
	union {
		unsigned short WORD;
		struct {
			unsigned char RDRH;
			unsigned char RDRL;
		} BYTE;
	} RDRHL;
	unsigned char  MDDR;
	char           wk0[13];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ESME : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char ESME : 1;
#endif
	} BIT;
	} ESMER;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 1;
			unsigned char SFSF : 1;
			unsigned char RXDSF : 1;
			unsigned char BRME : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char BRME : 1;
			unsigned char RXDSF : 1;
			unsigned char SFSF : 1;
			unsigned char  : 1;
#endif
	} BIT;
	} CR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BFE : 1;
			unsigned char CF0RE : 1;
			unsigned char CF1DS : 2;
			unsigned char PIBE : 1;
			unsigned char PIBS : 3;
#else
			unsigned char PIBS : 3;
			unsigned char PIBE : 1;
			unsigned char CF1DS : 2;
			unsigned char CF0RE : 1;
			unsigned char BFE : 1;
#endif
	} BIT;
	} CR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char DFCS : 3;
			unsigned char  : 1;
			unsigned char BCCS : 2;
			unsigned char RTS : 2;
#else
			unsigned char RTS : 2;
			unsigned char BCCS : 2;
			unsigned char  : 1;
			unsigned char DFCS : 3;
#endif
	} BIT;
	} CR2;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SDST : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char SDST : 1;
#endif
	} BIT;
	} CR3;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TXDXPS : 1;
			unsigned char RXDXPS : 1;
			unsigned char  : 2;
			unsigned char SHARPS : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char SHARPS : 1;
			unsigned char  : 2;
			unsigned char RXDXPS : 1;
			unsigned char TXDXPS : 1;
#endif
	} BIT;
	} PCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BFDIE : 1;
			unsigned char CF0MIE : 1;
			unsigned char CF1MIE : 1;
			unsigned char PIBDIE : 1;
			unsigned char BCDIE : 1;
			unsigned char AEDIE : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char AEDIE : 1;
			unsigned char BCDIE : 1;
			unsigned char PIBDIE : 1;
			unsigned char CF1MIE : 1;
			unsigned char CF0MIE : 1;
			unsigned char BFDIE : 1;
#endif
	} BIT;
	} ICR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BFDF : 1;
			unsigned char CF0MF : 1;
			unsigned char CF1MF : 1;
			unsigned char PIBDF : 1;
			unsigned char BCDF : 1;
			unsigned char AEDF : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char AEDF : 1;
			unsigned char BCDF : 1;
			unsigned char PIBDF : 1;
			unsigned char CF1MF : 1;
			unsigned char CF0MF : 1;
			unsigned char BFDF : 1;
#endif
	} BIT;
	} STR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BFDCL : 1;
			unsigned char CF0MCL : 1;
			unsigned char CF1MCL : 1;
			unsigned char PIBDCL : 1;
			unsigned char BCDCL : 1;
			unsigned char AEDCL : 1;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char AEDCL : 1;
			unsigned char BCDCL : 1;
			unsigned char PIBDCL : 1;
			unsigned char CF1MCL : 1;
			unsigned char CF0MCL : 1;
			unsigned char BFDCL : 1;
#endif
	} BIT;
	} STCR;
	unsigned char  CF0DR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CF0CE0 : 1;
			unsigned char CF0CE1 : 1;
			unsigned char CF0CE2 : 1;
			unsigned char CF0CE3 : 1;
			unsigned char CF0CE4 : 1;
			unsigned char CF0CE5 : 1;
			unsigned char CF0CE6 : 1;
			unsigned char CF0CE7 : 1;
#else
			unsigned char CF0CE7 : 1;
			unsigned char CF0CE6 : 1;
			unsigned char CF0CE5 : 1;
			unsigned char CF0CE4 : 1;
			unsigned char CF0CE3 : 1;
			unsigned char CF0CE2 : 1;
			unsigned char CF0CE1 : 1;
			unsigned char CF0CE0 : 1;
#endif
	} BIT;
	} CF0CR;
	unsigned char  CF0RR;
	unsigned char  PCF1DR;
	unsigned char  SCF1DR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CF1CE0 : 1;
			unsigned char CF1CE1 : 1;
			unsigned char CF1CE2 : 1;
			unsigned char CF1CE3 : 1;
			unsigned char CF1CE4 : 1;
			unsigned char CF1CE5 : 1;
			unsigned char CF1CE6 : 1;
			unsigned char CF1CE7 : 1;
#else
			unsigned char CF1CE7 : 1;
			unsigned char CF1CE6 : 1;
			unsigned char CF1CE5 : 1;
			unsigned char CF1CE4 : 1;
			unsigned char CF1CE3 : 1;
			unsigned char CF1CE2 : 1;
			unsigned char CF1CE1 : 1;
			unsigned char CF1CE0 : 1;
#endif
	} BIT;
	} CF1CR;
	unsigned char  CF1RR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TCST : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TCST : 1;
#endif
	} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TOMS : 2;
			unsigned char  : 1;
			unsigned char TWRC : 1;
			unsigned char TCSS : 3;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char TCSS : 3;
			unsigned char TWRC : 1;
			unsigned char  : 1;
			unsigned char TOMS : 2;
#endif
	} BIT;
	} TMR;
	unsigned char  TPRE;
	unsigned char  TCNT;
} st_sci12_t;

typedef struct st_smci {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 2;
			unsigned char BCP : 2;
			unsigned char PM : 1;
			unsigned char PE : 1;
			unsigned char BLK : 1;
			unsigned char GM : 1;
#else
			unsigned char GM : 1;
			unsigned char BLK : 1;
			unsigned char PE : 1;
			unsigned char PM : 1;
			unsigned char BCP : 2;
			unsigned char CKS : 2;
#endif
	} BIT;
	} SMR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKE : 2;
			unsigned char TEIE : 1;
			unsigned char MPIE : 1;
			unsigned char RE : 1;
			unsigned char TE : 1;
			unsigned char RIE : 1;
			unsigned char TIE : 1;
#else
			unsigned char TIE : 1;
			unsigned char RIE : 1;
			unsigned char TE : 1;
			unsigned char RE : 1;
			unsigned char MPIE : 1;
			unsigned char TEIE : 1;
			unsigned char CKE : 2;
#endif
	} BIT;
	} SCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MPBT : 1;
			unsigned char MPB : 1;
			unsigned char TEND : 1;
			unsigned char PER : 1;
			unsigned char ERS : 1;
			unsigned char ORER : 1;
			unsigned char RDRF : 1;
			unsigned char TDRE : 1;
#else
			unsigned char TDRE : 1;
			unsigned char RDRF : 1;
			unsigned char ORER : 1;
			unsigned char ERS : 1;
			unsigned char PER : 1;
			unsigned char TEND : 1;
			unsigned char MPB : 1;
			unsigned char MPBT : 1;
#endif
	} BIT;
	} SSR;
	char           wk2[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char SMIF : 1;
			unsigned char  : 1;
			unsigned char SINV : 1;
			unsigned char SDIR : 1;
			unsigned char CHR1 : 1;
			unsigned char  : 2;
			unsigned char BCP2 : 1;
#else
			unsigned char BCP2 : 1;
			unsigned char  : 2;
			unsigned char CHR1 : 1;
			unsigned char SDIR : 1;
			unsigned char SINV : 1;
			unsigned char  : 1;
			unsigned char SMIF : 1;
#endif
	} BIT;
	} SCMR;
} st_smci_t;

typedef struct st_system {
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short MD : 1;
			unsigned short  : 15;
#else
			unsigned short  : 15;
			unsigned short MD : 1;
#endif
	} BIT;
	} MDMONR;
	char           wk0[6];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short RAME : 1;
			unsigned short  : 15;
#else
			unsigned short  : 15;
			unsigned short RAME : 1;
#endif
	} BIT;
	} SYSCR1;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 15;
			unsigned short SSBY : 1;
#else
			unsigned short SSBY : 1;
			unsigned short  : 15;
#endif
	} BIT;
	} SBYCR;
	char           wk2[2];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSTPA0 : 1;
			unsigned long MSTPA1 : 1;
			unsigned long MSTPA2 : 1;
			unsigned long MSTPA3 : 1;
			unsigned long MSTPA4 : 1;
			unsigned long MSTPA5 : 1;
			unsigned long  : 1;
			unsigned long MSTPA7 : 1;
			unsigned long  : 1;
			unsigned long MSTPA9 : 1;
			unsigned long  : 4;
			unsigned long MSTPA14 : 1;
			unsigned long MSTPA15 : 1;
			unsigned long MSTPA16 : 1;
			unsigned long MSTPA17 : 1;
			unsigned long  : 1;
			unsigned long MSTPA19 : 1;
			unsigned long  : 3;
			unsigned long MSTPA23 : 1;
			unsigned long MSTPA24 : 1;
			unsigned long  : 2;
			unsigned long MSTPA27 : 1;
			unsigned long MSTPA28 : 1;
			unsigned long MSTPA29 : 1;
			unsigned long  : 1;
			unsigned long ACSE : 1;
#else
			unsigned long ACSE : 1;
			unsigned long  : 1;
			unsigned long MSTPA29 : 1;
			unsigned long MSTPA28 : 1;
			unsigned long MSTPA27 : 1;
			unsigned long  : 2;
			unsigned long MSTPA24 : 1;
			unsigned long MSTPA23 : 1;
			unsigned long  : 3;
			unsigned long MSTPA19 : 1;
			unsigned long  : 1;
			unsigned long MSTPA17 : 1;
			unsigned long MSTPA16 : 1;
			unsigned long MSTPA15 : 1;
			unsigned long MSTPA14 : 1;
			unsigned long  : 4;
			unsigned long MSTPA9 : 1;
			unsigned long  : 1;
			unsigned long MSTPA7 : 1;
			unsigned long  : 1;
			unsigned long MSTPA5 : 1;
			unsigned long MSTPA4 : 1;
			unsigned long MSTPA3 : 1;
			unsigned long MSTPA2 : 1;
			unsigned long MSTPA1 : 1;
			unsigned long MSTPA0 : 1;
#endif
	} BIT;
	} MSTPCRA;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 4;
			unsigned long MSTPB4 : 1;
			unsigned long  : 1;
			unsigned long MSTPB6 : 1;
			unsigned long  : 2;
			unsigned long MSTPB9 : 1;
			unsigned long MSTPB10 : 1;
			unsigned long  : 6;
			unsigned long MSTPB17 : 1;
			unsigned long  : 3;
			unsigned long MSTPB21 : 1;
			unsigned long  : 1;
			unsigned long MSTPB23 : 1;
			unsigned long  : 1;
			unsigned long MSTPB25 : 1;
			unsigned long MSTPB26 : 1;
			unsigned long  : 3;
			unsigned long MSTPB30 : 1;
			unsigned long  : 1;
#else
			unsigned long  : 1;
			unsigned long MSTPB30 : 1;
			unsigned long  : 3;
			unsigned long MSTPB26 : 1;
			unsigned long MSTPB25 : 1;
			unsigned long  : 1;
			unsigned long MSTPB23 : 1;
			unsigned long  : 1;
			unsigned long MSTPB21 : 1;
			unsigned long  : 3;
			unsigned long MSTPB17 : 1;
			unsigned long  : 6;
			unsigned long MSTPB10 : 1;
			unsigned long MSTPB9 : 1;
			unsigned long  : 2;
			unsigned long MSTPB6 : 1;
			unsigned long  : 1;
			unsigned long MSTPB4 : 1;
			unsigned long  : 4;
#endif
	} BIT;
	} MSTPCRB;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long MSTPC0 : 1;
			unsigned long  : 18;
			unsigned long MSTPC19 : 1;
			unsigned long  : 4;
			unsigned long MSTPC24 : 1;
			unsigned long  : 1;
			unsigned long MSTPC26 : 1;
			unsigned long MSTPC27 : 1;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long MSTPC27 : 1;
			unsigned long MSTPC26 : 1;
			unsigned long  : 1;
			unsigned long MSTPC24 : 1;
			unsigned long  : 4;
			unsigned long MSTPC19 : 1;
			unsigned long  : 18;
			unsigned long MSTPC0 : 1;
#endif
	} BIT;
	} MSTPCRC;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 5;
			unsigned long MSTPD5 : 1;
			unsigned long  : 4;
			unsigned long MSTPD10 : 1;
			unsigned long  : 15;
			unsigned long MSTPD26 : 1;
			unsigned long MSTPD27 : 1;
			unsigned long  : 4;
#else
			unsigned long  : 4;
			unsigned long MSTPD27 : 1;
			unsigned long MSTPD26 : 1;
			unsigned long  : 15;
			unsigned long MSTPD10 : 1;
			unsigned long  : 4;
			unsigned long MSTPD5 : 1;
			unsigned long  : 5;
#endif
	} BIT;
	} MSTPCRD;
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long PCKD : 4;
			unsigned long PCKC : 4;
			unsigned long PCKB : 4;
			unsigned long PCKA : 4;
			unsigned long  : 8;
			unsigned long ICK : 4;
			unsigned long FCK : 4;
#else
			unsigned long FCK : 4;
			unsigned long ICK : 4;
			unsigned long  : 8;
			unsigned long PCKA : 4;
			unsigned long PCKB : 4;
			unsigned long PCKC : 4;
			unsigned long PCKD : 4;
#endif
	} BIT;
	} SCKCR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 12;
			unsigned short CFDCK : 4;
#else
			unsigned short CFDCK : 4;
			unsigned short  : 12;
#endif
	} BIT;
	} SCKCR2;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short  : 8;
			unsigned short CKSEL : 3;
			unsigned short  : 5;
#else
			unsigned short  : 5;
			unsigned short CKSEL : 3;
			unsigned short  : 8;
#endif
	} BIT;
	} SCKCR3;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PLIDIV : 2;
			unsigned short  : 2;
			unsigned short PLLSRCSEL : 1;
			unsigned short  : 3;
			unsigned short STC : 6;
			unsigned short  : 2;
#else
			unsigned short  : 2;
			unsigned short STC : 6;
			unsigned short  : 3;
			unsigned short PLLSRCSEL : 1;
			unsigned short  : 2;
			unsigned short PLIDIV : 2;
#endif
	} BIT;
	} PLLCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PLLEN : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char PLLEN : 1;
#endif
	} BIT;
	} PLLCR2;
	char           wk3[7];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MOSTP : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char MOSTP : 1;
#endif
	} BIT;
	} MOSCCR;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LCSTP : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char LCSTP : 1;
#endif
	} BIT;
	} LOCOCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char ILCSTP : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char ILCSTP : 1;
#endif
	} BIT;
	} ILOCOCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char HCSTP : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char HCSTP : 1;
#endif
	} BIT;
	} HOCOCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char HCFRQ : 2;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char HCFRQ : 2;
#endif
	} BIT;
	} HOCOCR2;
	char           wk5[4];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MOOVF : 1;
			unsigned char  : 1;
			unsigned char PLOVF : 1;
			unsigned char HCOVF : 1;
			unsigned char ILCOVF : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char ILCOVF : 1;
			unsigned char HCOVF : 1;
			unsigned char PLOVF : 1;
			unsigned char  : 1;
			unsigned char MOOVF : 1;
#endif
	} BIT;
	} OSCOVFSR;
	char           wk6[3];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OSTDIE : 1;
			unsigned char  : 6;
			unsigned char OSTDE : 1;
#else
			unsigned char OSTDE : 1;
			unsigned char  : 6;
			unsigned char OSTDIE : 1;
#endif
	} BIT;
	} OSTDCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OSTDF : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char OSTDF : 1;
#endif
	} BIT;
	} OSTDSR;
	char           wk7[95];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char RSTCKSEL : 3;
			unsigned char  : 4;
			unsigned char RSTCKEN : 1;
#else
			unsigned char RSTCKEN : 1;
			unsigned char  : 4;
			unsigned char RSTCKSEL : 3;
#endif
	} BIT;
	} RSTCKCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char MSTS : 8;
#else
			unsigned char MSTS : 8;
#endif
	} BIT;
	} MOSCWTCR;
	char           wk8[29];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IWDTRF : 1;
			unsigned char WDTRF : 1;
			unsigned char SWRF : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char SWRF : 1;
			unsigned char WDTRF : 1;
			unsigned char IWDTRF : 1;
#endif
	} BIT;
	} RSTSR2;
	char           wk9[1];
	unsigned short SWRR;
	char           wk10[12];
	union {
		unsigned long LONG;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned long  : 10;
			unsigned long SCI5RXD : 2;
			unsigned long  : 4;
			unsigned long SCI8RXD : 2;
			unsigned long SCI9RXD : 2;
			unsigned long  : 2;
			unsigned long SCI11RXD : 2;
			unsigned long  : 8;
#else
			unsigned long  : 8;
			unsigned long SCI11RXD : 2;
			unsigned long  : 2;
			unsigned long SCI9RXD : 2;
			unsigned long SCI8RXD : 2;
			unsigned long  : 4;
			unsigned long SCI5RXD : 2;
			unsigned long  : 10;
#endif
	} BIT;
	} PRDFR0;
	char           wk11[12];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD1IDTSEL : 2;
			unsigned char LVD1IRQSEL : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char LVD1IRQSEL : 1;
			unsigned char LVD1IDTSEL : 2;
#endif
	} BIT;
	} LVD1CR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD1DET : 1;
			unsigned char LVD1MON : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char LVD1MON : 1;
			unsigned char LVD1DET : 1;
#endif
	} BIT;
	} LVD1SR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD2IDTSEL : 2;
			unsigned char LVD2IRQSEL : 1;
			unsigned char  : 5;
#else
			unsigned char  : 5;
			unsigned char LVD2IRQSEL : 1;
			unsigned char LVD2IDTSEL : 2;
#endif
	} BIT;
	} LVD2CR1;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD2DET : 1;
			unsigned char LVD2MON : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char LVD2MON : 1;
			unsigned char LVD2DET : 1;
#endif
	} BIT;
	} LVD2SR;
	char           wk12[794];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short PRC0 : 1;
			unsigned short PRC1 : 1;
			unsigned short  : 1;
			unsigned short PRC3 : 1;
			unsigned short  : 4;
			unsigned short PRKEY : 8;
#else
			unsigned short PRKEY : 8;
			unsigned short  : 4;
			unsigned short PRC3 : 1;
			unsigned short  : 1;
			unsigned short PRC1 : 1;
			unsigned short PRC0 : 1;
#endif
	} BIT;
	} PRCR;
	char           wk13[48784];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char PORF : 1;
			unsigned char LVD0RF : 1;
			unsigned char LVD1RF : 1;
			unsigned char LVD2RF : 1;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char LVD2RF : 1;
			unsigned char LVD1RF : 1;
			unsigned char LVD0RF : 1;
			unsigned char PORF : 1;
#endif
	} BIT;
	} RSTSR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CWSF : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char CWSF : 1;
#endif
	} BIT;
	} RSTSR1;
	char           wk14[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 4;
			unsigned char MODRV2 : 2;
			unsigned char MOSEL : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char MOSEL : 1;
			unsigned char MODRV2 : 2;
			unsigned char  : 4;
#endif
	} BIT;
	} MOFCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char HOCOPCNT : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char HOCOPCNT : 1;
#endif
	} BIT;
	} HOCOPCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char RICVLS : 1;
#else
			unsigned char RICVLS : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} VOLSR;
	char           wk15[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 5;
			unsigned char LVD1E : 1;
			unsigned char LVD2E : 1;
			unsigned char  : 1;
#else
			unsigned char  : 1;
			unsigned char LVD2E : 1;
			unsigned char LVD1E : 1;
			unsigned char  : 5;
#endif
	} BIT;
	} LVCMPCR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD1LVL : 4;
			unsigned char LVD2LVL : 4;
#else
			unsigned char LVD2LVL : 4;
			unsigned char LVD1LVL : 4;
#endif
	} BIT;
	} LVDLVLR;
	char           wk16[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD1RIE : 1;
			unsigned char LVD1DFDIS : 1;
			unsigned char LVD1CMPE : 1;
			unsigned char  : 1;
			unsigned char LVD1FSAMP : 2;
			unsigned char LVD1RI : 1;
			unsigned char LVD1RN : 1;
#else
			unsigned char LVD1RN : 1;
			unsigned char LVD1RI : 1;
			unsigned char LVD1FSAMP : 2;
			unsigned char  : 1;
			unsigned char LVD1CMPE : 1;
			unsigned char LVD1DFDIS : 1;
			unsigned char LVD1RIE : 1;
#endif
	} BIT;
	} LVD1CR0;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char LVD2RIE : 1;
			unsigned char LVD2DFDIS : 1;
			unsigned char LVD2CMPE : 1;
			unsigned char  : 1;
			unsigned char LVD2FSAMP : 2;
			unsigned char LVD2RI : 1;
			unsigned char LVD2RN : 1;
#else
			unsigned char LVD2RN : 1;
			unsigned char LVD2RI : 1;
			unsigned char LVD2FSAMP : 2;
			unsigned char  : 1;
			unsigned char LVD2CMPE : 1;
			unsigned char LVD2DFDIS : 1;
			unsigned char LVD2RIE : 1;
#endif
	} BIT;
	} LVD2CR0;
} st_system_t;

typedef struct st_temps {
	unsigned long  TSCDR;
} st_temps_t;

typedef struct st_tfu {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char IUF : 1;
			unsigned char  : 3;
			unsigned char OF : 2;
			unsigned char  : 2;
#else
			unsigned char  : 2;
			unsigned char OF : 2;
			unsigned char  : 3;
			unsigned char IUF : 1;
#endif
	} BIT;
	} FXSCIOC;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 4;
			unsigned char OUF : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char OUF : 1;
			unsigned char  : 4;
#endif
	} BIT;
	} FXATIOC;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char BSYF : 1;
			unsigned char ERRF : 1;
			unsigned char  : 6;
#else
			unsigned char  : 6;
			unsigned char ERRF : 1;
			unsigned char BSYF : 1;
#endif
	} BIT;
	} TRGSTS;
	char           wk1[7];
	float  FPSCDT0;
	float  FPSCDT1;
	float  FPATDT0;
	float  FPATDT1;
	signed long  FXSCDT0;
	signed long  FXSCDT1;
	signed long  FXATDT0;
	signed long  FXATDT1;
	unsigned long  DTSR0;
	unsigned long  DTSR1;
} st_tfu_t;

typedef struct st_tmr0 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char CCLR : 2;
			unsigned char OVIE : 1;
			unsigned char CMIEA : 1;
			unsigned char CMIEB : 1;
#else
			unsigned char CMIEB : 1;
			unsigned char CMIEA : 1;
			unsigned char OVIE : 1;
			unsigned char CCLR : 2;
			unsigned char  : 3;
#endif
	} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OSA : 2;
			unsigned char OSB : 2;
			unsigned char ADTE : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char ADTE : 1;
			unsigned char OSB : 2;
			unsigned char OSA : 2;
#endif
	} BIT;
	} TCSR;
	char           wk1[1];
	unsigned char  TCORA;
	char           wk2[1];
	unsigned char  TCORB;
	char           wk3[1];
	unsigned char  TCNT;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 3;
			unsigned char CSS : 2;
			unsigned char  : 2;
			unsigned char TMRIS : 1;
#else
			unsigned char TMRIS : 1;
			unsigned char  : 2;
			unsigned char CSS : 2;
			unsigned char CKS : 3;
#endif
	} BIT;
	} TCCR;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TCS : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TCS : 1;
#endif
	} BIT;
	} TCSTR;
} st_tmr0_t;

typedef struct st_tmr1 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char CCLR : 2;
			unsigned char OVIE : 1;
			unsigned char CMIEA : 1;
			unsigned char CMIEB : 1;
#else
			unsigned char CMIEB : 1;
			unsigned char CMIEA : 1;
			unsigned char OVIE : 1;
			unsigned char CCLR : 2;
			unsigned char  : 3;
#endif
	} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OSA : 2;
			unsigned char OSB : 2;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char OSB : 2;
			unsigned char OSA : 2;
#endif
	} BIT;
	} TCSR;
	char           wk1[1];
	unsigned char  TCORA;
	char           wk2[1];
	unsigned char  TCORB;
	char           wk3[1];
	unsigned char  TCNT;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 3;
			unsigned char CSS : 2;
			unsigned char  : 2;
			unsigned char TMRIS : 1;
#else
			unsigned char TMRIS : 1;
			unsigned char  : 2;
			unsigned char CSS : 2;
			unsigned char CKS : 3;
#endif
	} BIT;
	} TCCR;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char TCS : 1;
			unsigned char  : 7;
#else
			unsigned char  : 7;
			unsigned char TCS : 1;
#endif
	} BIT;
	} TCSTR;
} st_tmr1_t;

typedef struct st_tmr4 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char CCLR : 2;
			unsigned char OVIE : 1;
			unsigned char CMIEA : 1;
			unsigned char CMIEB : 1;
#else
			unsigned char CMIEB : 1;
			unsigned char CMIEA : 1;
			unsigned char OVIE : 1;
			unsigned char CCLR : 2;
			unsigned char  : 3;
#endif
	} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OSA : 2;
			unsigned char OSB : 2;
			unsigned char ADTE : 1;
			unsigned char  : 3;
#else
			unsigned char  : 3;
			unsigned char ADTE : 1;
			unsigned char OSB : 2;
			unsigned char OSA : 2;
#endif
	} BIT;
	} TCSR;
	char           wk1[1];
	unsigned char  TCORA;
	char           wk2[1];
	unsigned char  TCORB;
	char           wk3[1];
	unsigned char  TCNT;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 3;
			unsigned char CSS : 2;
			unsigned char  : 2;
			unsigned char TMRIS : 1;
#else
			unsigned char TMRIS : 1;
			unsigned char  : 2;
			unsigned char CSS : 2;
			unsigned char CKS : 3;
#endif
	} BIT;
	} TCCR;
} st_tmr4_t;

typedef struct st_tmr5 {
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 3;
			unsigned char CCLR : 2;
			unsigned char OVIE : 1;
			unsigned char CMIEA : 1;
			unsigned char CMIEB : 1;
#else
			unsigned char CMIEB : 1;
			unsigned char CMIEA : 1;
			unsigned char OVIE : 1;
			unsigned char CCLR : 2;
			unsigned char  : 3;
#endif
	} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char OSA : 2;
			unsigned char OSB : 2;
			unsigned char  : 4;
#else
			unsigned char  : 4;
			unsigned char OSB : 2;
			unsigned char OSA : 2;
#endif
	} BIT;
	} TCSR;
	char           wk1[1];
	unsigned char  TCORA;
	char           wk2[1];
	unsigned char  TCORB;
	char           wk3[1];
	unsigned char  TCNT;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char CKS : 3;
			unsigned char CSS : 2;
			unsigned char  : 2;
			unsigned char TMRIS : 1;
#else
			unsigned char TMRIS : 1;
			unsigned char  : 2;
			unsigned char CSS : 2;
			unsigned char CKS : 3;
#endif
	} BIT;
	} TCCR;
} st_tmr5_t;

typedef struct st_tmr01 {
	unsigned short TCORA;
	unsigned short TCORB;
	unsigned short TCNT;
	unsigned short TCCR;
} st_tmr01_t;

typedef struct st_wdt {
	unsigned char  WDTRR;
	char           wk0[1];
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short TOPS : 2;
			unsigned short  : 2;
			unsigned short CKS : 4;
			unsigned short RPES : 2;
			unsigned short  : 2;
			unsigned short RPSS : 2;
			unsigned short  : 2;
#else
			unsigned short  : 2;
			unsigned short RPSS : 2;
			unsigned short  : 2;
			unsigned short RPES : 2;
			unsigned short CKS : 4;
			unsigned short  : 2;
			unsigned short TOPS : 2;
#endif
	} BIT;
	} WDTCR;
	union {
		unsigned short WORD;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned short CNTVAL : 14;
			unsigned short UNDFF : 1;
			unsigned short REFEF : 1;
#else
			unsigned short REFEF : 1;
			unsigned short UNDFF : 1;
			unsigned short CNTVAL : 14;
#endif
	} BIT;
	} WDTSR;
	union {
		unsigned char BYTE;
		struct {
			
#ifdef __RX_LITTLE_ENDIAN__
			unsigned char  : 7;
			unsigned char RSTIRQS : 1;
#else
			unsigned char RSTIRQS : 1;
			unsigned char  : 7;
#endif
	} BIT;
	} WDTRCR;
} st_wdt_t;


#pragma pack()

#endif

