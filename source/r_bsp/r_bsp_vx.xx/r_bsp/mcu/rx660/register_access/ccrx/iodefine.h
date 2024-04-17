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
/*******************************************************************************
*
* Device     : RX/RX600/RX660
*
* File Name  : iodefine.h
*
* Abstract   : Definition of I/O Register.
*
* History    : V0.40  (2021-06-24)  [Hardware Manual Revision : 0.40]
*            : V0.50  (2021-07-27)  [Hardware Manual Revision : 0.50]
*            : V0.50A (2021-09-21)  [Hardware Manual Revision : 0.50]
*            : V0.50B (2021-09-28)  [Hardware Manual Revision : 0.50]
*            : V0.50C (2021-09-29)  [Hardware Manual Revision : 0.50]
*            : V0.50D (2021-10-13)  [Hardware Manual Revision : 0.50]
*            : V0.50E (2021-10-28)  [Hardware Manual Revision : 0.50]
*            : V0.50F (2022-01-14)  [Hardware Manual Revision : 0.50]
*            : V0.50G (2022-02-10)  [Hardware Manual Revision : 0.50]
*            : V1.00  (2022-02-21)  [Hardware Manual Revision : 1.00]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
*  Copyright(c) 2021 Renesas Electronics Corporation.
*
*********************************************************************************/
/********************************************************************************/
/*                                                                              */
/*  DESCRIPTION : Definition of ICU Register                                    */
/*  CPU TYPE    : RX660                                                         */
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
/*       MSTP(SCI0) = 0;    // SCI0,SMCI0                         expands to :  */
/*         SYSTEM.MSTPCRB.BIT.MSTPB31 = 0;                                      */
/*       MSTP(MTU4) = 0;    // MTU,MTU0,MTU1,MTU2,MTU3,MTU4,...   expands to :  */
/*         SYSTEM.MSTPCRA.BIT.MSTPA9  = 0;                                      */
/*       MSTP(CMT3) = 0;    // CMT2,CMT3                          expands to :  */
/*         SYSTEM.MSTPCRA.BIT.MSTPA14 = 0;                                      */
/*                                                                              */
/*                                                                              */
/********************************************************************************/
#ifndef __RX660IODEFINE_HEADER__
#define __RX660IODEFINE_HEADER__

#define	IEN_BSC_BUSERR		IEN0
#define	IEN_RAM_RAMERR		IEN2
#define	IEN_FCU_FIFERR		IEN5
#define	IEN_FCU_FRDYI		IEN7
#define	IEN_ICU_SWINT2		IEN2
#define	IEN_ICU_SWINT		IEN3
#define	IEN_CMT0_CMI0		IEN4
#define	IEN_CMT1_CMI1		IEN5
#define	IEN_CMTW0_CMWI0		IEN6
#define	IEN_CMTW1_CMWI1		IEN7
#define	IEN_RSCI10_RXI		IEN0
#define	IEN_RSCI10_TXI		IEN1
#define	IEN_RSPI0_SPRI0		IEN6
#define	IEN_RSPI0_SPTI0		IEN7
#define	IEN_RSCI11_RXI		IEN2
#define	IEN_RSCI11_TXI		IEN3
#define	IEN_RIIC0_RXI0		IEN4
#define	IEN_RIIC0_TXI0		IEN5
#define	IEN_RIIC2_RXI2		IEN6
#define	IEN_RIIC2_TXI2		IEN7
#define	IEN_SCI0_RXI0		IEN2
#define	IEN_SCI0_TXI0		IEN3
#define	IEN_SCI1_RXI1		IEN4
#define	IEN_SCI1_TXI1		IEN5
#define	IEN_SCI2_RXI2		IEN6
#define	IEN_SCI2_TXI2		IEN7
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
#define	IEN_SCI3_RXI3		IEN0
#define	IEN_SCI3_TXI3		IEN1
#define	IEN_SCI4_RXI4		IEN2
#define	IEN_SCI4_TXI4		IEN3
#define	IEN_SCI5_RXI5		IEN4
#define	IEN_SCI5_TXI5		IEN5
#define	IEN_SCI6_RXI6		IEN6
#define	IEN_SCI6_TXI6		IEN7
#define	IEN_LVD1_LVD1		IEN0
#define	IEN_LVD2_LVD2		IEN1
#define	IEN_RTC_ALM			IEN4
#define	IEN_RTC_PRD			IEN5
#define	IEN_REMC0_REMCI0	IEN6
#define	IEN_IWDT_IWUNI		IEN7
#define	IEN_WDT_WUNI		IEN0
#define	IEN_SCI7_RXI7		IEN2
#define	IEN_SCI7_TXI7		IEN3
#define	IEN_SCI8_RXI8		IEN4
#define	IEN_SCI8_TXI8		IEN5
#define	IEN_SCI9_RXI9		IEN6
#define	IEN_SCI9_TXI9		IEN7
#define	IEN_SCI10_RXI10		IEN0
#define	IEN_SCI10_TXI10		IEN1
#define	IEN_ICU_GROUPBL2	IEN3
#define	IEN_ICU_GROUPBL0	IEN6
#define	IEN_ICU_GROUPBL1	IEN7
#define	IEN_ICU_GROUPAL0	IEN0
#define	IEN_SCI11_RXI11		IEN2
#define	IEN_SCI11_TXI11		IEN3
#define	IEN_SCI12_RXI12		IEN4
#define	IEN_SCI12_TXI12		IEN5
#define	IEN_DMAC_DMAC0I		IEN0
#define	IEN_DMAC_DMAC1I		IEN1
#define	IEN_DMAC_DMAC2I		IEN2
#define	IEN_DMAC_DMAC3I		IEN3
#define	IEN_DMAC_DMAC74I	IEN4
#define	IEN_OST_OSTDI		IEN5
#define	IEN_PERIB_INTB128	IEN0
#define	IEN_PERIB_INTB129	IEN1
#define	IEN_PERIB_INTB130	IEN2
#define	IEN_PERIB_INTB131	IEN3
#define	IEN_PERIB_INTB132	IEN4
#define	IEN_PERIB_INTB133	IEN5
#define	IEN_PERIB_INTB134	IEN6
#define	IEN_PERIB_INTB135	IEN7
#define	IEN_PERIB_INTB136	IEN0
#define	IEN_PERIB_INTB137	IEN1
#define	IEN_PERIB_INTB138	IEN2
#define	IEN_PERIB_INTB139	IEN3
#define	IEN_PERIB_INTB140	IEN4
#define	IEN_PERIB_INTB141	IEN5
#define	IEN_PERIB_INTB142	IEN6
#define	IEN_PERIB_INTB143	IEN7
#define	IEN_PERIB_INTB144	IEN0
#define	IEN_PERIB_INTB145	IEN1
#define	IEN_PERIB_INTB146	IEN2
#define	IEN_PERIB_INTB147	IEN3
#define	IEN_PERIB_INTB148	IEN4
#define	IEN_PERIB_INTB149	IEN5
#define	IEN_PERIB_INTB150	IEN6
#define	IEN_PERIB_INTB151	IEN7
#define	IEN_PERIB_INTB152	IEN0
#define	IEN_PERIB_INTB153	IEN1
#define	IEN_PERIB_INTB154	IEN2
#define	IEN_PERIB_INTB155	IEN3
#define	IEN_PERIB_INTB156	IEN4
#define	IEN_PERIB_INTB157	IEN5
#define	IEN_PERIB_INTB158	IEN6
#define	IEN_PERIB_INTB159	IEN7
#define	IEN_PERIB_INTB160	IEN0
#define	IEN_PERIB_INTB161	IEN1
#define	IEN_PERIB_INTB162	IEN2
#define	IEN_PERIB_INTB163	IEN3
#define	IEN_PERIB_INTB164	IEN4
#define	IEN_PERIB_INTB165	IEN5
#define	IEN_PERIB_INTB166	IEN6
#define	IEN_PERIB_INTB167	IEN7
#define	IEN_PERIB_INTB168	IEN0
#define	IEN_PERIB_INTB169	IEN1
#define	IEN_PERIB_INTB170	IEN2
#define	IEN_PERIB_INTB171	IEN3
#define	IEN_PERIB_INTB172	IEN4
#define	IEN_PERIB_INTB173	IEN5
#define	IEN_PERIB_INTB174	IEN6
#define	IEN_PERIB_INTB175	IEN7
#define	IEN_PERIB_INTB176	IEN0
#define	IEN_PERIB_INTB177	IEN1
#define	IEN_PERIB_INTB178	IEN2
#define	IEN_PERIB_INTB179	IEN3
#define	IEN_PERIB_INTB180	IEN4
#define	IEN_PERIB_INTB181	IEN5
#define	IEN_PERIB_INTB182	IEN6
#define	IEN_PERIB_INTB183	IEN7
#define	IEN_PERIB_INTB184	IEN0
#define	IEN_PERIB_INTB185	IEN1
#define	IEN_PERIB_INTB186	IEN2
#define	IEN_PERIB_INTB187	IEN3
#define	IEN_PERIB_INTB188	IEN4
#define	IEN_PERIB_INTB189	IEN5
#define	IEN_PERIB_INTB190	IEN6
#define	IEN_PERIB_INTB191	IEN7
#define	IEN_PERIB_INTB192	IEN0
#define	IEN_PERIB_INTB193	IEN1
#define	IEN_PERIB_INTB194	IEN2
#define	IEN_PERIB_INTB195	IEN3
#define	IEN_PERIB_INTB196	IEN4
#define	IEN_PERIB_INTB197	IEN5
#define	IEN_PERIB_INTB198	IEN6
#define	IEN_PERIB_INTB199	IEN7
#define	IEN_PERIB_INTB200	IEN0
#define	IEN_PERIB_INTB201	IEN1
#define	IEN_PERIB_INTB202	IEN2
#define	IEN_PERIB_INTB203	IEN3
#define	IEN_PERIB_INTB204	IEN4
#define	IEN_PERIB_INTB205	IEN5
#define	IEN_PERIB_INTB206	IEN6
#define	IEN_PERIB_INTB207	IEN7
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
#define	VECT_CMTW0_CMWI0	30
#define	VECT_CMTW1_CMWI1	31
#define	VECT_RSCI10_RXI		32
#define	VECT_RSCI10_TXI		33
#define	VECT_RSPI0_SPRI0	38
#define	VECT_RSPI0_SPTI0	39
#define	VECT_RSCI11_RXI		42
#define	VECT_RSCI11_TXI		43
#define	VECT_RIIC0_RXI0		52
#define	VECT_RIIC0_TXI0		53
#define	VECT_RIIC2_RXI2		54
#define	VECT_RIIC2_TXI2		55
#define	VECT_SCI0_RXI0		58
#define	VECT_SCI0_TXI0		59
#define	VECT_SCI1_RXI1		60
#define	VECT_SCI1_TXI1		61
#define	VECT_SCI2_RXI2		62
#define	VECT_SCI2_TXI2		63
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
#define	VECT_SCI3_RXI3		80
#define	VECT_SCI3_TXI3		81
#define	VECT_SCI4_RXI4		82
#define	VECT_SCI4_TXI4		83
#define	VECT_SCI5_RXI5		84
#define	VECT_SCI5_TXI5		85
#define	VECT_SCI6_RXI6		86
#define	VECT_SCI6_TXI6		87
#define	VECT_LVD1_LVD1		88
#define	VECT_LVD2_LVD2		89
#define	VECT_RTC_ALM		92
#define	VECT_RTC_PRD		93
#define	VECT_REMC0_REMCI0	94
#define	VECT_IWDT_IWUNI		95
#define	VECT_WDT_WUNI		96
#define	VECT_SCI7_RXI7		98
#define	VECT_SCI7_TXI7		99
#define	VECT_SCI8_RXI8		100
#define	VECT_SCI8_TXI8		101
#define	VECT_SCI9_RXI9		102
#define	VECT_SCI9_TXI9		103
#define	VECT_SCI10_RXI10	104
#define	VECT_SCI10_TXI10	105
#define	VECT_ICU_GROUPBL2	107
#define	VECT_ICU_GROUPBL0	110
#define	VECT_ICU_GROUPBL1	111
#define	VECT_ICU_GROUPAL0	112
#define	VECT_SCI11_RXI11	114
#define	VECT_SCI11_TXI11	115
#define	VECT_SCI12_RXI12	116
#define	VECT_SCI12_TXI12	117
#define	VECT_DMAC_DMAC0I	120
#define	VECT_DMAC_DMAC1I	121
#define	VECT_DMAC_DMAC2I	122
#define	VECT_DMAC_DMAC3I	123
#define	VECT_DMAC_DMAC74I	124
#define	VECT_OST_OSTDI		125
#define	VECT_PERIB_INTB128	128
#define	VECT_PERIB_INTB129	129
#define	VECT_PERIB_INTB130	130
#define	VECT_PERIB_INTB131	131
#define	VECT_PERIB_INTB132	132
#define	VECT_PERIB_INTB133	133
#define	VECT_PERIB_INTB134	134
#define	VECT_PERIB_INTB135	135
#define	VECT_PERIB_INTB136	136
#define	VECT_PERIB_INTB137	137
#define	VECT_PERIB_INTB138	138
#define	VECT_PERIB_INTB139	139
#define	VECT_PERIB_INTB140	140
#define	VECT_PERIB_INTB141	141
#define	VECT_PERIB_INTB142	142
#define	VECT_PERIB_INTB143	143
#define	VECT_PERIB_INTB144	144
#define	VECT_PERIB_INTB145	145
#define	VECT_PERIB_INTB146	146
#define	VECT_PERIB_INTB147	147
#define	VECT_PERIB_INTB148	148
#define	VECT_PERIB_INTB149	149
#define	VECT_PERIB_INTB150	150
#define	VECT_PERIB_INTB151	151
#define	VECT_PERIB_INTB152	152
#define	VECT_PERIB_INTB153	153
#define	VECT_PERIB_INTB154	154
#define	VECT_PERIB_INTB155	155
#define	VECT_PERIB_INTB156	156
#define	VECT_PERIB_INTB157	157
#define	VECT_PERIB_INTB158	158
#define	VECT_PERIB_INTB159	159
#define	VECT_PERIB_INTB160	160
#define	VECT_PERIB_INTB161	161
#define	VECT_PERIB_INTB162	162
#define	VECT_PERIB_INTB163	163
#define	VECT_PERIB_INTB164	164
#define	VECT_PERIB_INTB165	165
#define	VECT_PERIB_INTB166	166
#define	VECT_PERIB_INTB167	167
#define	VECT_PERIB_INTB168	168
#define	VECT_PERIB_INTB169	169
#define	VECT_PERIB_INTB170	170
#define	VECT_PERIB_INTB171	171
#define	VECT_PERIB_INTB172	172
#define	VECT_PERIB_INTB173	173
#define	VECT_PERIB_INTB174	174
#define	VECT_PERIB_INTB175	175
#define	VECT_PERIB_INTB176	176
#define	VECT_PERIB_INTB177	177
#define	VECT_PERIB_INTB178	178
#define	VECT_PERIB_INTB179	179
#define	VECT_PERIB_INTB180	180
#define	VECT_PERIB_INTB181	181
#define	VECT_PERIB_INTB182	182
#define	VECT_PERIB_INTB183	183
#define	VECT_PERIB_INTB184	184
#define	VECT_PERIB_INTB185	185
#define	VECT_PERIB_INTB186	186
#define	VECT_PERIB_INTB187	187
#define	VECT_PERIB_INTB188	188
#define	VECT_PERIB_INTB189	189
#define	VECT_PERIB_INTB190	190
#define	VECT_PERIB_INTB191	191
#define	VECT_PERIB_INTB192	192
#define	VECT_PERIB_INTB193	193
#define	VECT_PERIB_INTB194	194
#define	VECT_PERIB_INTB195	195
#define	VECT_PERIB_INTB196	196
#define	VECT_PERIB_INTB197	197
#define	VECT_PERIB_INTB198	198
#define	VECT_PERIB_INTB199	199
#define	VECT_PERIB_INTB200	200
#define	VECT_PERIB_INTB201	201
#define	VECT_PERIB_INTB202	202
#define	VECT_PERIB_INTB203	203
#define	VECT_PERIB_INTB204	204
#define	VECT_PERIB_INTB205	205
#define	VECT_PERIB_INTB206	206
#define	VECT_PERIB_INTB207	207
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
#define	MSTP_DA		SYSTEM.MSTPCRA.BIT.MSTPA19
#define	MSTP_S12AD	SYSTEM.MSTPCRA.BIT.MSTPA17
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
#define	MSTP_MTU8	SYSTEM.MSTPCRA.BIT.MSTPA9
#define	MSTP_TMR0	SYSTEM.MSTPCRA.BIT.MSTPA5
#define	MSTP_TMR1	SYSTEM.MSTPCRA.BIT.MSTPA5
#define	MSTP_TMR01	SYSTEM.MSTPCRA.BIT.MSTPA5
#define	MSTP_TMR2	SYSTEM.MSTPCRA.BIT.MSTPA4
#define	MSTP_TMR3	SYSTEM.MSTPCRA.BIT.MSTPA4
#define	MSTP_TMR23	SYSTEM.MSTPCRA.BIT.MSTPA4
#define	MSTP_CMTW0	SYSTEM.MSTPCRA.BIT.MSTPA1
#define	MSTP_CMTW1	SYSTEM.MSTPCRA.BIT.MSTPA0
#define	MSTP_SCI0	SYSTEM.MSTPCRB.BIT.MSTPB31
#define	MSTP_SMCI0	SYSTEM.MSTPCRB.BIT.MSTPB31
#define	MSTP_SCI1	SYSTEM.MSTPCRB.BIT.MSTPB30
#define	MSTP_SMCI1	SYSTEM.MSTPCRB.BIT.MSTPB30
#define	MSTP_SCI2	SYSTEM.MSTPCRB.BIT.MSTPB29
#define	MSTP_SMCI2	SYSTEM.MSTPCRB.BIT.MSTPB29
#define	MSTP_SCI3	SYSTEM.MSTPCRB.BIT.MSTPB28
#define	MSTP_SMCI3	SYSTEM.MSTPCRB.BIT.MSTPB28
#define	MSTP_SCI4	SYSTEM.MSTPCRB.BIT.MSTPB27
#define	MSTP_SMCI4	SYSTEM.MSTPCRB.BIT.MSTPB27
#define	MSTP_SCI5	SYSTEM.MSTPCRB.BIT.MSTPB26
#define	MSTP_SMCI5	SYSTEM.MSTPCRB.BIT.MSTPB26
#define	MSTP_SCI6	SYSTEM.MSTPCRB.BIT.MSTPB25
#define	MSTP_SMCI6	SYSTEM.MSTPCRB.BIT.MSTPB25
#define	MSTP_SCI7	SYSTEM.MSTPCRB.BIT.MSTPB24
#define	MSTP_SMCI7	SYSTEM.MSTPCRB.BIT.MSTPB24
#define	MSTP_CRC	SYSTEM.MSTPCRB.BIT.MSTPB23
#define	MSTP_RIIC0	SYSTEM.MSTPCRB.BIT.MSTPB21
#define	MSTP_RSPI0	SYSTEM.MSTPCRB.BIT.MSTPB17
#define	MSTP_CMPC0	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC1	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC2	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_CMPC3	SYSTEM.MSTPCRB.BIT.MSTPB10
#define	MSTP_ELC	SYSTEM.MSTPCRB.BIT.MSTPB9
#define	MSTP_DOC	SYSTEM.MSTPCRB.BIT.MSTPB6
#define	MSTP_SCI12	SYSTEM.MSTPCRB.BIT.MSTPB4
#define	MSTP_SMCI12	SYSTEM.MSTPCRB.BIT.MSTPB4
#define	MSTP_SCI8	SYSTEM.MSTPCRC.BIT.MSTPC27
#define	MSTP_SMCI8	SYSTEM.MSTPCRC.BIT.MSTPC27
#define	MSTP_SCI9	SYSTEM.MSTPCRC.BIT.MSTPC26
#define	MSTP_SMCI9	SYSTEM.MSTPCRC.BIT.MSTPC26
#define	MSTP_SCI10	SYSTEM.MSTPCRC.BIT.MSTPC25
#define	MSTP_SMCI10	SYSTEM.MSTPCRC.BIT.MSTPC25
#define	MSTP_SCI11	SYSTEM.MSTPCRC.BIT.MSTPC24
#define	MSTP_SMCI11	SYSTEM.MSTPCRC.BIT.MSTPC24
#define	MSTP_CAC	SYSTEM.MSTPCRC.BIT.MSTPC19
#define	MSTP_RIIC2	SYSTEM.MSTPCRC.BIT.MSTPC17
#define	MSTP_RAM0	SYSTEM.MSTPCRC.BIT.MSTPC0
#define	MSTP_CANFD	SYSTEM.MSTPCRD.BIT.MSTPD10
#define	MSTP_CANFD0	SYSTEM.MSTPCRD.BIT.MSTPD10
#define	MSTP_REMC0	SYSTEM.MSTPCRD.BIT.MSTPD7
#define	MSTP_RSCI10	SYSTEM.MSTPCRD.BIT.MSTPD3
#define	MSTP_RSCI11	SYSTEM.MSTPCRD.BIT.MSTPD2

#define	IS_SCI0_TEI0		IS0
#define	IS_SCI0_ERI0		IS1
#define	IS_SCI1_TEI1		IS2
#define	IS_SCI1_ERI1		IS3
#define	IS_SCI2_TEI2		IS4
#define	IS_SCI2_ERI2		IS5
#define	IS_SCI3_TEI3		IS6
#define	IS_SCI3_ERI3		IS7
#define	IS_SCI4_TEI4		IS8
#define	IS_SCI4_ERI4		IS9
#define	IS_SCI5_TEI5		IS10
#define	IS_SCI5_ERI5		IS11
#define	IS_SCI6_TEI6		IS12
#define	IS_SCI6_ERI6		IS13
#define	IS_SCI7_TEI7		IS14
#define	IS_SCI7_ERI7		IS15
#define	IS_SCI12_TEI12		IS16
#define	IS_SCI12_ERI12		IS17
#define	IS_SCI12_SCIX0		IS18
#define	IS_SCI12_SCIX1		IS19
#define	IS_SCI12_SCIX2		IS20
#define	IS_SCI12_SCIX3		IS21
#define	IS_CAC_FERRI		IS26
#define	IS_CAC_MENDI		IS27
#define	IS_CAC_OVFI			IS28
#define	IS_DOC_DOPCI		IS29
#define	IS_POE3_OEI1		IS9
#define	IS_POE3_OEI2		IS10
#define	IS_POE3_OEI3		IS11
#define	IS_POE3_OEI4		IS12
#define	IS_RIIC0_TEI0		IS13
#define	IS_RIIC0_EEI0		IS14
#define	IS_RIIC2_TEI2		IS15
#define	IS_RIIC2_EEI2		IS16
#define	IS_S12AD_S12CMPAI	IS20
#define	IS_S12AD_S12CMPBI	IS21
#define	IS_SCI8_TEI8		IS24
#define	IS_SCI8_ERI8		IS25
#define	IS_SCI9_TEI9		IS26
#define	IS_SCI9_ERI9		IS27
#define	IS_CANFD0_CHEI		IS1
#define	IS_CANFD0_CFRI		IS2
#define	IS_CANFD_GLEI		IS3
#define	IS_CANFD_RFRI		IS4
#define	IS_CANFD0_CHTI		IS5
#define	IS_CANFD_RMRI		IS6
#define	IS_SCI10_TEI10		IS8
#define	IS_SCI10_ERI10		IS9
#define	IS_SCI11_TEI11		IS12
#define	IS_SCI11_ERI11		IS13
#define	IS_RSPI0_SPII0		IS16
#define	IS_RSPI0_SPEI0		IS17
#define	IS_RSCI10_TEI		IS24
#define	IS_RSCI10_ERI		IS25
#define	IS_RSCI10_BFD		IS26
#define	IS_RSCI11_TEI		IS27
#define	IS_RSCI11_ERI		IS28
#define	IS_RSCI11_BFD		IS29

#define	EN_SCI0_TEI0		EN0
#define	EN_SCI0_ERI0		EN1
#define	EN_SCI1_TEI1		EN2
#define	EN_SCI1_ERI1		EN3
#define	EN_SCI2_TEI2		EN4
#define	EN_SCI2_ERI2		EN5
#define	EN_SCI3_TEI3		EN6
#define	EN_SCI3_ERI3		EN7
#define	EN_SCI4_TEI4		EN8
#define	EN_SCI4_ERI4		EN9
#define	EN_SCI5_TEI5		EN10
#define	EN_SCI5_ERI5		EN11
#define	EN_SCI6_TEI6		EN12
#define	EN_SCI6_ERI6		EN13
#define	EN_SCI7_TEI7		EN14
#define	EN_SCI7_ERI7		EN15
#define	EN_SCI12_TEI12		EN16
#define	EN_SCI12_ERI12		EN17
#define	EN_SCI12_SCIX0		EN18
#define	EN_SCI12_SCIX1		EN19
#define	EN_SCI12_SCIX2		EN20
#define	EN_SCI12_SCIX3		EN21
#define	EN_CAC_FERRI		EN26
#define	EN_CAC_MENDI		EN27
#define	EN_CAC_OVFI			EN28
#define	EN_DOC_DOPCI		EN29
#define	EN_POE3_OEI1		EN9
#define	EN_POE3_OEI2		EN10
#define	EN_POE3_OEI3		EN11
#define	EN_POE3_OEI4		EN12
#define	EN_RIIC0_TEI0		EN13
#define	EN_RIIC0_EEI0		EN14
#define	EN_RIIC2_TEI2		EN15
#define	EN_RIIC2_EEI2		EN16
#define	EN_S12AD_S12CMPAI	EN20
#define	EN_S12AD_S12CMPBI	EN21
#define	EN_SCI8_TEI8		EN24
#define	EN_SCI8_ERI8		EN25
#define	EN_SCI9_TEI9		EN26
#define	EN_SCI9_ERI9		EN27
#define	EN_CANFD0_CHEI		EN1
#define	EN_CANFD0_CFRI		EN2
#define	EN_CANFD_GLEI		EN3
#define	EN_CANFD_RFRI		EN4
#define	EN_CANFD0_CHTI		EN5
#define	EN_CANFD_RMRI		EN6
#define	EN_SCI10_TEI10		EN8
#define	EN_SCI10_ERI10		EN9
#define	EN_SCI11_TEI11		EN12
#define	EN_SCI11_ERI11		EN13
#define	EN_RSPI0_SPII0		EN16
#define	EN_RSPI0_SPEI0		EN17
#define	EN_RSCI10_TEI		EN24
#define	EN_RSCI10_ERI		EN25
#define	EN_RSCI10_BFD		EN26
#define	EN_RSCI11_TEI		EN27
#define	EN_RSCI11_ERI		EN28
#define	EN_RSCI11_BFD		EN29

#define	GEN_SCI0_TEI0		GENBL0
#define	GEN_SCI0_ERI0		GENBL0
#define	GEN_SCI1_TEI1		GENBL0
#define	GEN_SCI1_ERI1		GENBL0
#define	GEN_SCI2_TEI2		GENBL0
#define	GEN_SCI2_ERI2		GENBL0
#define	GEN_SCI3_TEI3		GENBL0
#define	GEN_SCI3_ERI3		GENBL0
#define	GEN_SCI4_TEI4		GENBL0
#define	GEN_SCI4_ERI4		GENBL0
#define	GEN_SCI5_TEI5		GENBL0
#define	GEN_SCI5_ERI5		GENBL0
#define	GEN_SCI6_TEI6		GENBL0
#define	GEN_SCI6_ERI6		GENBL0
#define	GEN_SCI7_TEI7		GENBL0
#define	GEN_SCI7_ERI7		GENBL0
#define	GEN_SCI12_TEI12		GENBL0
#define	GEN_SCI12_ERI12		GENBL0
#define	GEN_SCI12_SCIX0		GENBL0
#define	GEN_SCI12_SCIX1		GENBL0
#define	GEN_SCI12_SCIX2		GENBL0
#define	GEN_SCI12_SCIX3		GENBL0
#define	GEN_CAC_FERRI		GENBL0
#define	GEN_CAC_MENDI		GENBL0
#define	GEN_CAC_OVFI		GENBL0
#define	GEN_DOC_DOPCI		GENBL0
#define	GEN_POE3_OEI1		GENBL1
#define	GEN_POE3_OEI2		GENBL1
#define	GEN_POE3_OEI3		GENBL1
#define	GEN_POE3_OEI4		GENBL1
#define	GEN_RIIC0_TEI0		GENBL1
#define	GEN_RIIC0_EEI0		GENBL1
#define	GEN_RIIC2_TEI2		GENBL1
#define	GEN_RIIC2_EEI2		GENBL1
#define	GEN_S12AD_S12CMPAI	GENBL1
#define	GEN_S12AD_S12CMPBI	GENBL1
#define	GEN_SCI8_TEI8		GENBL1
#define	GEN_SCI8_ERI8		GENBL1
#define	GEN_SCI9_TEI9		GENBL1
#define	GEN_SCI9_ERI9		GENBL1
#define	GEN_CANFD0_CHEI		GENBL2
#define	GEN_CANFD0_CFRI		GENBL2
#define	GEN_CANFD_GLEI		GENBL2
#define	GEN_CANFD_RFRI		GENBL2
#define	GEN_CANFD0_CHTI		GENBL2
#define	GEN_CANFD_RMRI		GENBL2
#define	GEN_SCI10_TEI10		GENAL0
#define	GEN_SCI10_ERI10		GENAL0
#define	GEN_SCI11_TEI11		GENAL0
#define	GEN_SCI11_ERI11		GENAL0
#define	GEN_RSPI0_SPII0		GENAL0
#define	GEN_RSPI0_SPEI0		GENAL0
#define	GEN_RSCI10_TEI		GENAL0
#define	GEN_RSCI10_ERI		GENAL0
#define	GEN_RSCI10_BFD		GENAL0
#define	GEN_RSCI11_TEI		GENAL0
#define	GEN_RSCI11_ERI		GENAL0
#define	GEN_RSCI11_BFD		GENAL0

#define	GRP_SCI0_TEI0		GRPBL0
#define	GRP_SCI0_ERI0		GRPBL0
#define	GRP_SCI1_TEI1		GRPBL0
#define	GRP_SCI1_ERI1		GRPBL0
#define	GRP_SCI2_TEI2		GRPBL0
#define	GRP_SCI2_ERI2		GRPBL0
#define	GRP_SCI3_TEI3		GRPBL0
#define	GRP_SCI3_ERI3		GRPBL0
#define	GRP_SCI4_TEI4		GRPBL0
#define	GRP_SCI4_ERI4		GRPBL0
#define	GRP_SCI5_TEI5		GRPBL0
#define	GRP_SCI5_ERI5		GRPBL0
#define	GRP_SCI6_TEI6		GRPBL0
#define	GRP_SCI6_ERI6		GRPBL0
#define	GRP_SCI7_TEI7		GRPBL0
#define	GRP_SCI7_ERI7		GRPBL0
#define	GRP_SCI12_TEI12		GRPBL0
#define	GRP_SCI12_ERI12		GRPBL0
#define	GRP_SCI12_SCIX0		GRPBL0
#define	GRP_SCI12_SCIX1		GRPBL0
#define	GRP_SCI12_SCIX2		GRPBL0
#define	GRP_SCI12_SCIX3		GRPBL0
#define	GRP_CAC_FERRI		GRPBL0
#define	GRP_CAC_MENDI		GRPBL0
#define	GRP_CAC_OVFI		GRPBL0
#define	GRP_DOC_DOPCI		GRPBL0
#define	GRP_POE3_OEI1		GRPBL1
#define	GRP_POE3_OEI2		GRPBL1
#define	GRP_POE3_OEI3		GRPBL1
#define	GRP_POE3_OEI4		GRPBL1
#define	GRP_RIIC0_TEI0		GRPBL1
#define	GRP_RIIC0_EEI0		GRPBL1
#define	GRP_RIIC2_TEI2		GRPBL1
#define	GRP_RIIC2_EEI2		GRPBL1
#define	GRP_S12AD_S12CMPAI	GRPBL1
#define	GRP_S12AD_S12CMPBI	GRPBL1
#define	GRP_SCI8_TEI8		GRPBL1
#define	GRP_SCI8_ERI8		GRPBL1
#define	GRP_SCI9_TEI9		GRPBL1
#define	GRP_SCI9_ERI9		GRPBL1
#define	GRP_CANFD0_CHEI		GRPBL2
#define	GRP_CANFD0_CFRI		GRPBL2
#define	GRP_CANFD_GLEI		GRPBL2
#define	GRP_CANFD_RFRI		GRPBL2
#define	GRP_CANFD0_CHTI		GRPBL2
#define	GRP_CANFD_RMRI		GRPBL2
#define	GRP_SCI10_TEI10		GRPAL0
#define	GRP_SCI10_ERI10		GRPAL0
#define	GRP_SCI11_TEI11		GRPAL0
#define	GRP_SCI11_ERI11		GRPAL0
#define	GRP_RSPI0_SPII0		GRPAL0
#define	GRP_RSPI0_SPEI0		GRPAL0
#define	GRP_RSCI10_TEI		GRPAL0
#define	GRP_RSCI10_ERI		GRPAL0
#define	GRP_RSCI10_BFD		GRPAL0
#define	GRP_RSCI11_TEI		GRPAL0
#define	GRP_RSCI11_ERI		GRPAL0
#define	GRP_RSCI11_BFD		GRPAL0

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

#define	BSC		(*(volatile struct st_bsc    __evenaccess *)0x81300)
#define	CAC		(*(volatile struct st_cac    __evenaccess *)0x8B000)
#define	CANFD	(*(volatile struct st_canfd  __evenaccess *)0xA8014)
#define	CANFD0	(*(volatile struct st_canfd0 __evenaccess *)0xA8000)
#define	CMPC0	(*(volatile struct st_cmpc   __evenaccess *)0xA0C80)
#define	CMPC1	(*(volatile struct st_cmpc   __evenaccess *)0xA0CA0)
#define	CMPC2	(*(volatile struct st_cmpc   __evenaccess *)0xA0CC0)
#define	CMPC3	(*(volatile struct st_cmpc   __evenaccess *)0xA0CE0)
#define	CMT		(*(volatile struct st_cmt    __evenaccess *)0x88000)
#define	CMT0	(*(volatile struct st_cmt0   __evenaccess *)0x88002)
#define	CMT1	(*(volatile struct st_cmt0   __evenaccess *)0x88008)
#define	CMT2	(*(volatile struct st_cmt0   __evenaccess *)0x88012)
#define	CMT3	(*(volatile struct st_cmt0   __evenaccess *)0x88018)
#define	CMTW0	(*(volatile struct st_cmtw   __evenaccess *)0x94200)
#define	CMTW1	(*(volatile struct st_cmtw   __evenaccess *)0x94280)
#define	CRC		(*(volatile struct st_crc    __evenaccess *)0x88280)
#define	DA		(*(volatile struct st_da     __evenaccess *)0x88040)
#define	DMAC	(*(volatile struct st_dmac   __evenaccess *)0x82200)
#define	DMAC0	(*(volatile struct st_dmac0  __evenaccess *)0x82000)
#define	DMAC1	(*(volatile struct st_dmac1  __evenaccess *)0x82040)
#define	DMAC2	(*(volatile struct st_dmac1  __evenaccess *)0x82080)
#define	DMAC3	(*(volatile struct st_dmac1  __evenaccess *)0x820C0)
#define	DMAC4	(*(volatile struct st_dmac1  __evenaccess *)0x82100)
#define	DMAC5	(*(volatile struct st_dmac1  __evenaccess *)0x82140)
#define	DMAC6	(*(volatile struct st_dmac1  __evenaccess *)0x82180)
#define	DMAC7	(*(volatile struct st_dmac1  __evenaccess *)0x821C0)
#define	DOC		(*(volatile struct st_doc    __evenaccess *)0xA0580)
#define	DTC		(*(volatile struct st_dtc    __evenaccess *)0x82400)
#define	ELC		(*(volatile struct st_elc    __evenaccess *)0x8B100)
#define	FLASH	(*(volatile struct st_flash  __evenaccess *)0x8C294)
#define	ICU		(*(volatile struct st_icu    __evenaccess *)0x87000)
#define	IWDT	(*(volatile struct st_iwdt   __evenaccess *)0x88030)
#define	MPC		(*(volatile struct st_mpc    __evenaccess *)0x8C100)
#define	MPU		(*(volatile struct st_mpu    __evenaccess *)0x86400)
#define	MTU		(*(volatile struct st_mtu    __evenaccess *)0xC120A)
#define	MTU0	(*(volatile struct st_mtu0   __evenaccess *)0xC1290)
#define	MTU1	(*(volatile struct st_mtu1   __evenaccess *)0xC1290)
#define	MTU2	(*(volatile struct st_mtu2   __evenaccess *)0xC1292)
#define	MTU3	(*(volatile struct st_mtu3   __evenaccess *)0xC1200)
#define	MTU4	(*(volatile struct st_mtu4   __evenaccess *)0xC1200)
#define	MTU5	(*(volatile struct st_mtu5   __evenaccess *)0xC1A94)
#define	MTU6	(*(volatile struct st_mtu6   __evenaccess *)0xC1A00)
#define	MTU7	(*(volatile struct st_mtu7   __evenaccess *)0xC1A00)
#define	MTU8	(*(volatile struct st_mtu8   __evenaccess *)0xC1298)
#define	OFSM	(*(volatile struct st_ofsm   __evenaccess *)0x120040)
#define	POE3	(*(volatile struct st_poe    __evenaccess *)0x8C4C0)
#define	PORT0	(*(volatile struct st_port0  __evenaccess *)0x8C000)
#define	PORT1	(*(volatile struct st_port1  __evenaccess *)0x8C001)
#define	PORT2	(*(volatile struct st_port2  __evenaccess *)0x8C002)
#define	PORT3	(*(volatile struct st_port3  __evenaccess *)0x8C003)
#define	PORT4	(*(volatile struct st_port4  __evenaccess *)0x8C004)
#define	PORT5	(*(volatile struct st_port5  __evenaccess *)0x8C005)
#define	PORT6	(*(volatile struct st_port6  __evenaccess *)0x8C006)
#define	PORT7	(*(volatile struct st_port7  __evenaccess *)0x8C007)
#define	PORT8	(*(volatile struct st_port8  __evenaccess *)0x8C008)
#define	PORT9	(*(volatile struct st_port9  __evenaccess *)0x8C009)
#define	PORTA	(*(volatile struct st_porta  __evenaccess *)0x8C00A)
#define	PORTB	(*(volatile struct st_portb  __evenaccess *)0x8C00B)
#define	PORTC	(*(volatile struct st_portc  __evenaccess *)0x8C00C)
#define	PORTD	(*(volatile struct st_portd  __evenaccess *)0x8C00D)
#define	PORTE	(*(volatile struct st_porte  __evenaccess *)0x8C00E)
#define	PORTF	(*(volatile struct st_portf  __evenaccess *)0x8C00F)
#define	PORTH	(*(volatile struct st_porth  __evenaccess *)0x8C011)
#define	PORTJ	(*(volatile struct st_portj  __evenaccess *)0x8C012)
#define	PORTK	(*(volatile struct st_portk  __evenaccess *)0x8C013)
#define	PORTL	(*(volatile struct st_portl  __evenaccess *)0x8C014)
#define	PORTN	(*(volatile struct st_portn  __evenaccess *)0x8C016)
#define	RAM		(*(volatile struct st_ram    __evenaccess *)0x81200)
#define	REMC0	(*(volatile struct st_remc   __evenaccess *)0xA0B00)
#define	RIIC0	(*(volatile struct st_riic   __evenaccess *)0x88300)
#define	RIIC2	(*(volatile struct st_riic   __evenaccess *)0x88340)
#define	RSCI10	(*(volatile struct st_rsci   __evenaccess *)0xE2000)
#define	RSCI11	(*(volatile struct st_rsci   __evenaccess *)0xE2080)
#define	RSPI0	(*(volatile struct st_rspi   __evenaccess *)0xD0100)
#define	RTC		(*(volatile struct st_rtc    __evenaccess *)0x8C400)
#define	S12AD	(*(volatile struct st_s12ad  __evenaccess *)0x88084)
#define	SCI0	(*(volatile struct st_sci0   __evenaccess *)0x8A000)
#define	SCI1	(*(volatile struct st_sci0   __evenaccess *)0x8A020)
#define	SCI2	(*(volatile struct st_sci0   __evenaccess *)0x8A040)
#define	SCI3	(*(volatile struct st_sci0   __evenaccess *)0x8A060)
#define	SCI4	(*(volatile struct st_sci0   __evenaccess *)0x8A080)
#define	SCI5	(*(volatile struct st_sci0   __evenaccess *)0x8A0A0)
#define	SCI6	(*(volatile struct st_sci0   __evenaccess *)0x8A0C0)
#define	SCI7	(*(volatile struct st_sci0   __evenaccess *)0x8A0E0)
#define	SCI8	(*(volatile struct st_sci0   __evenaccess *)0x8A100)
#define	SCI9	(*(volatile struct st_sci0   __evenaccess *)0x8A120)
#define	SCI10	(*(volatile struct st_sci10  __evenaccess *)0xD0000)
#define	SCI11	(*(volatile struct st_sci10  __evenaccess *)0xD0020)
#define	SCI12	(*(volatile struct st_sci12  __evenaccess *)0x8B300)
#define	SMCI0	(*(volatile struct st_smci   __evenaccess *)0x8A000)
#define	SMCI1	(*(volatile struct st_smci   __evenaccess *)0x8A020)
#define	SMCI2	(*(volatile struct st_smci   __evenaccess *)0x8A040)
#define	SMCI3	(*(volatile struct st_smci   __evenaccess *)0x8A060)
#define	SMCI4	(*(volatile struct st_smci   __evenaccess *)0x8A080)
#define	SMCI5	(*(volatile struct st_smci   __evenaccess *)0x8A0A0)
#define	SMCI6	(*(volatile struct st_smci   __evenaccess *)0x8A0C0)
#define	SMCI7	(*(volatile struct st_smci   __evenaccess *)0x8A0E0)
#define	SMCI8	(*(volatile struct st_smci   __evenaccess *)0x8A100)
#define	SMCI9	(*(volatile struct st_smci   __evenaccess *)0x8A120)
#define	SMCI10	(*(volatile struct st_smci   __evenaccess *)0xD0000)
#define	SMCI11	(*(volatile struct st_smci   __evenaccess *)0xD0020)
#define	SMCI12	(*(volatile struct st_smci   __evenaccess *)0x8B300)
#define	SYSTEM	(*(volatile struct st_system __evenaccess *)0x80000)
#define	TEMPS	(*(volatile struct st_temps  __evenaccess *)0x7FB17C)
#define	TMR0	(*(volatile struct st_tmr0   __evenaccess *)0x88200)
#define	TMR1	(*(volatile struct st_tmr1   __evenaccess *)0x88201)
#define	TMR2	(*(volatile struct st_tmr0   __evenaccess *)0x88210)
#define	TMR3	(*(volatile struct st_tmr1   __evenaccess *)0x88211)
#define	TMR01	(*(volatile struct st_tmr01  __evenaccess *)0x88204)
#define	TMR23	(*(volatile struct st_tmr01  __evenaccess *)0x88214)
#define	WDT		(*(volatile struct st_wdt    __evenaccess *)0x88020)

typedef enum enum_ir {
IR_BSC_BUSERR=16,IR_RAM_RAMERR=18,
IR_FCU_FIFERR=21,IR_FCU_FRDYI=23,
IR_ICU_SWINT2=26,IR_ICU_SWINT,
IR_CMT0_CMI0,
IR_CMT1_CMI1,
IR_CMTW0_CMWI0,
IR_CMTW1_CMWI1,
IR_RSCI10_RXI,IR_RSCI10_TXI,
IR_RSPI0_SPRI0=38,IR_RSPI0_SPTI0,
IR_RSCI11_RXI=42,IR_RSCI11_TXI,
IR_RIIC0_RXI0=52,IR_RIIC0_TXI0,
IR_RIIC2_RXI2,IR_RIIC2_TXI2,
IR_SCI0_RXI0=58,IR_SCI0_TXI0,
IR_SCI1_RXI1,IR_SCI1_TXI1,
IR_SCI2_RXI2,IR_SCI2_TXI2,
IR_ICU_IRQ0,IR_ICU_IRQ1,IR_ICU_IRQ2,IR_ICU_IRQ3,IR_ICU_IRQ4,IR_ICU_IRQ5,IR_ICU_IRQ6,IR_ICU_IRQ7,IR_ICU_IRQ8,IR_ICU_IRQ9,IR_ICU_IRQ10,IR_ICU_IRQ11,IR_ICU_IRQ12,IR_ICU_IRQ13,IR_ICU_IRQ14,IR_ICU_IRQ15,
IR_SCI3_RXI3,IR_SCI3_TXI3,
IR_SCI4_RXI4,IR_SCI4_TXI4,
IR_SCI5_RXI5,IR_SCI5_TXI5,
IR_SCI6_RXI6,IR_SCI6_TXI6,
IR_LVD1_LVD1,
IR_LVD2_LVD2,
IR_RTC_ALM=92,IR_RTC_PRD,
IR_REMC0_REMCI0,
IR_IWDT_IWUNI,
IR_WDT_WUNI,
IR_SCI7_RXI7=98,IR_SCI7_TXI7,
IR_SCI8_RXI8,IR_SCI8_TXI8,
IR_SCI9_RXI9,IR_SCI9_TXI9,
IR_SCI10_RXI10,IR_SCI10_TXI10,
IR_ICU_GROUPBL2=107,IR_ICU_GROUPBL0=110,IR_ICU_GROUPBL1,IR_ICU_GROUPAL0,
IR_SCI11_RXI11=114,IR_SCI11_TXI11,
IR_SCI12_RXI12,IR_SCI12_TXI12,
IR_DMAC_DMAC0I=120,IR_DMAC_DMAC1I,IR_DMAC_DMAC2I,IR_DMAC_DMAC3I,IR_DMAC_DMAC74I,
IR_OST_OSTDI,
IR_PERIB_INTB128=128,IR_PERIB_INTB129,IR_PERIB_INTB130,IR_PERIB_INTB131,IR_PERIB_INTB132,
IR_PERIB_INTB133,IR_PERIB_INTB134,IR_PERIB_INTB135,IR_PERIB_INTB136,IR_PERIB_INTB137,
IR_PERIB_INTB138,IR_PERIB_INTB139,IR_PERIB_INTB140,IR_PERIB_INTB141,IR_PERIB_INTB142,
IR_PERIB_INTB143,IR_PERIB_INTB144,IR_PERIB_INTB145,IR_PERIB_INTB146,IR_PERIB_INTB147,
IR_PERIB_INTB148,IR_PERIB_INTB149,IR_PERIB_INTB150,IR_PERIB_INTB151,IR_PERIB_INTB152,
IR_PERIB_INTB153,IR_PERIB_INTB154,IR_PERIB_INTB155,IR_PERIB_INTB156,IR_PERIB_INTB157,
IR_PERIB_INTB158,IR_PERIB_INTB159,IR_PERIB_INTB160,IR_PERIB_INTB161,IR_PERIB_INTB162,
IR_PERIB_INTB163,IR_PERIB_INTB164,IR_PERIB_INTB165,IR_PERIB_INTB166,IR_PERIB_INTB167,
IR_PERIB_INTB168,IR_PERIB_INTB169,IR_PERIB_INTB170,IR_PERIB_INTB171,IR_PERIB_INTB172,
IR_PERIB_INTB173,IR_PERIB_INTB174,IR_PERIB_INTB175,IR_PERIB_INTB176,IR_PERIB_INTB177,
IR_PERIB_INTB178,IR_PERIB_INTB179,IR_PERIB_INTB180,IR_PERIB_INTB181,IR_PERIB_INTB182,
IR_PERIB_INTB183,IR_PERIB_INTB184,IR_PERIB_INTB185,IR_PERIB_INTB186,IR_PERIB_INTB187,
IR_PERIB_INTB188,IR_PERIB_INTB189,IR_PERIB_INTB190,IR_PERIB_INTB191,IR_PERIB_INTB192,
IR_PERIB_INTB193,IR_PERIB_INTB194,IR_PERIB_INTB195,IR_PERIB_INTB196,IR_PERIB_INTB197,
IR_PERIB_INTB198,IR_PERIB_INTB199,IR_PERIB_INTB200,IR_PERIB_INTB201,IR_PERIB_INTB202,
IR_PERIB_INTB203,IR_PERIB_INTB204,IR_PERIB_INTB205,IR_PERIB_INTB206,IR_PERIB_INTB207,
IR_PERIA_INTA208,IR_PERIA_INTA209,IR_PERIA_INTA210,IR_PERIA_INTA211,IR_PERIA_INTA212,
IR_PERIA_INTA213,IR_PERIA_INTA214,IR_PERIA_INTA215,IR_PERIA_INTA216,IR_PERIA_INTA217,
IR_PERIA_INTA218,IR_PERIA_INTA219,IR_PERIA_INTA220,IR_PERIA_INTA221,IR_PERIA_INTA222,
IR_PERIA_INTA223,IR_PERIA_INTA224,IR_PERIA_INTA225,IR_PERIA_INTA226,IR_PERIA_INTA227,
IR_PERIA_INTA228,IR_PERIA_INTA229,IR_PERIA_INTA230,IR_PERIA_INTA231,IR_PERIA_INTA232,
IR_PERIA_INTA233,IR_PERIA_INTA234,IR_PERIA_INTA235,IR_PERIA_INTA236,IR_PERIA_INTA237,
IR_PERIA_INTA238,IR_PERIA_INTA239,IR_PERIA_INTA240,IR_PERIA_INTA241,IR_PERIA_INTA242,
IR_PERIA_INTA243,IR_PERIA_INTA244,IR_PERIA_INTA245,IR_PERIA_INTA246,IR_PERIA_INTA247,
IR_PERIA_INTA248,IR_PERIA_INTA249,IR_PERIA_INTA250,IR_PERIA_INTA251,IR_PERIA_INTA252,
IR_PERIA_INTA253,IR_PERIA_INTA254,IR_PERIA_INTA255
} enum_ir_t;

typedef enum enum_dtce {
DTCE_ICU_SWINT2=26,DTCE_ICU_SWINT,
DTCE_CMT0_CMI0,
DTCE_CMT1_CMI1,
DTCE_CMTW0_CMWI0,
DTCE_CMTW1_CMWI1,
DTCE_RSCI10_RXI,DTCE_RSCI10_TXI,
DTCE_RSPI0_SPRI0=38,DTCE_RSPI0_SPTI0,
DTCE_RSCI11_RXI=42,DTCE_RSCI11_TXI,
DTCE_RIIC0_RXI0=52,DTCE_RIIC0_TXI0,
DTCE_RIIC2_RXI2,DTCE_RIIC2_TXI2,
DTCE_SCI0_RXI0=58,DTCE_SCI0_TXI0,
DTCE_SCI1_RXI1,DTCE_SCI1_TXI1,
DTCE_SCI2_RXI2,DTCE_SCI2_TXI2,
DTCE_ICU_IRQ0,DTCE_ICU_IRQ1,DTCE_ICU_IRQ2,DTCE_ICU_IRQ3,DTCE_ICU_IRQ4,DTCE_ICU_IRQ5,DTCE_ICU_IRQ6,DTCE_ICU_IRQ7,
DTCE_ICU_IRQ8,DTCE_ICU_IRQ9,DTCE_ICU_IRQ10,DTCE_ICU_IRQ11,DTCE_ICU_IRQ12,DTCE_ICU_IRQ13,DTCE_ICU_IRQ14,DTCE_ICU_IRQ15,
DTCE_SCI3_RXI3,DTCE_SCI3_TXI3,
DTCE_SCI4_RXI4,DTCE_SCI4_TXI4,
DTCE_SCI5_RXI5,DTCE_SCI5_TXI5,
DTCE_SCI6_RXI6,DTCE_SCI6_TXI6,
DTCE_SCI7_RXI7=98,DTCE_SCI7_TXI7,
DTCE_SCI8_RXI8,DTCE_SCI8_TXI8,
DTCE_SCI9_RXI9,DTCE_SCI9_TXI9,
DTCE_SCI10_RXI10,DTCE_SCI10_TXI10,
DTCE_SCI11_RXI11=114,DTCE_SCI11_TXI11,
DTCE_SCI12_RXI12,DTCE_SCI12_TXI12,
DTCE_DMAC_DMAC0I=120,DTCE_DMAC_DMAC1I,DTCE_DMAC_DMAC2I,DTCE_DMAC_DMAC3I,
DTCE_PERIB_INTB128=128,DTCE_PERIB_INTB129,DTCE_PERIB_INTB130,DTCE_PERIB_INTB131,DTCE_PERIB_INTB132,
DTCE_PERIB_INTB133,DTCE_PERIB_INTB134,DTCE_PERIB_INTB135,DTCE_PERIB_INTB136,DTCE_PERIB_INTB137,
DTCE_PERIB_INTB138,DTCE_PERIB_INTB139,DTCE_PERIB_INTB140,DTCE_PERIB_INTB141,DTCE_PERIB_INTB142,
DTCE_PERIB_INTB143,DTCE_PERIB_INTB144,DTCE_PERIB_INTB145,DTCE_PERIB_INTB146,DTCE_PERIB_INTB147,
DTCE_PERIB_INTB148,DTCE_PERIB_INTB149,DTCE_PERIB_INTB150,DTCE_PERIB_INTB151,DTCE_PERIB_INTB152,
DTCE_PERIB_INTB153,DTCE_PERIB_INTB154,DTCE_PERIB_INTB155,DTCE_PERIB_INTB156,DTCE_PERIB_INTB157,
DTCE_PERIB_INTB158,DTCE_PERIB_INTB159,DTCE_PERIB_INTB160,DTCE_PERIB_INTB161,DTCE_PERIB_INTB162,
DTCE_PERIB_INTB163,DTCE_PERIB_INTB164,DTCE_PERIB_INTB165,DTCE_PERIB_INTB166,DTCE_PERIB_INTB167,
DTCE_PERIB_INTB168,DTCE_PERIB_INTB169,DTCE_PERIB_INTB170,DTCE_PERIB_INTB171,DTCE_PERIB_INTB172,
DTCE_PERIB_INTB173,DTCE_PERIB_INTB174,DTCE_PERIB_INTB175,DTCE_PERIB_INTB176,DTCE_PERIB_INTB177,
DTCE_PERIB_INTB178,DTCE_PERIB_INTB179,DTCE_PERIB_INTB180,DTCE_PERIB_INTB181,DTCE_PERIB_INTB182,
DTCE_PERIB_INTB183,DTCE_PERIB_INTB184,DTCE_PERIB_INTB185,DTCE_PERIB_INTB186,DTCE_PERIB_INTB187,
DTCE_PERIB_INTB188,DTCE_PERIB_INTB189,DTCE_PERIB_INTB190,DTCE_PERIB_INTB191,DTCE_PERIB_INTB192,
DTCE_PERIB_INTB193,DTCE_PERIB_INTB194,DTCE_PERIB_INTB195,DTCE_PERIB_INTB196,DTCE_PERIB_INTB197,
DTCE_PERIB_INTB198,DTCE_PERIB_INTB199,DTCE_PERIB_INTB200,DTCE_PERIB_INTB201,DTCE_PERIB_INTB202,
DTCE_PERIB_INTB203,DTCE_PERIB_INTB204,DTCE_PERIB_INTB205,DTCE_PERIB_INTB206,DTCE_PERIB_INTB207,
DTCE_PERIA_INTA208,DTCE_PERIA_INTA209,DTCE_PERIA_INTA210,DTCE_PERIA_INTA211,DTCE_PERIA_INTA212,
DTCE_PERIA_INTA213,DTCE_PERIA_INTA214,DTCE_PERIA_INTA215,DTCE_PERIA_INTA216,DTCE_PERIA_INTA217,
DTCE_PERIA_INTA218,DTCE_PERIA_INTA219,DTCE_PERIA_INTA220,DTCE_PERIA_INTA221,DTCE_PERIA_INTA222,
DTCE_PERIA_INTA223,DTCE_PERIA_INTA224,DTCE_PERIA_INTA225,DTCE_PERIA_INTA226,DTCE_PERIA_INTA227,
DTCE_PERIA_INTA228,DTCE_PERIA_INTA229,DTCE_PERIA_INTA230,DTCE_PERIA_INTA231,DTCE_PERIA_INTA232,
DTCE_PERIA_INTA233,DTCE_PERIA_INTA234,DTCE_PERIA_INTA235,DTCE_PERIA_INTA236,DTCE_PERIA_INTA237,
DTCE_PERIA_INTA238,DTCE_PERIA_INTA239,DTCE_PERIA_INTA240,DTCE_PERIA_INTA241,DTCE_PERIA_INTA242,
DTCE_PERIA_INTA243,DTCE_PERIA_INTA244,DTCE_PERIA_INTA245,DTCE_PERIA_INTA246,DTCE_PERIA_INTA247,
DTCE_PERIA_INTA248,DTCE_PERIA_INTA249,DTCE_PERIA_INTA250,DTCE_PERIA_INTA251,DTCE_PERIA_INTA252,
DTCE_PERIA_INTA253,DTCE_PERIA_INTA254,DTCE_PERIA_INTA255
} enum_dtce_t;

typedef enum enum_ier {
IER_BSC_BUSERR=0x02,
IER_RAM_RAMERR=0x02,
IER_FCU_FIFERR=0x02,IER_FCU_FRDYI=0x02,
IER_ICU_SWINT2=0x03,IER_ICU_SWINT=0x03,
IER_CMT0_CMI0=0x03,
IER_CMT1_CMI1=0x03,
IER_CMTW0_CMWI0=0x03,
IER_CMTW1_CMWI1=0x03,
IER_RSCI10_RXI=0x04,IER_RSCI10_TXI=0x04,
IER_RSPI0_SPRI0=0x04,IER_RSPI0_SPTI0=0x04,
IER_RSCI11_RXI=0x05,IER_RSCI11_TXI=0x05,
IER_RIIC0_RXI0=0x06,IER_RIIC0_TXI0=0x06,
IER_RIIC2_RXI2=0x06,IER_RIIC2_TXI2=0x06,
IER_SCI0_RXI0=0x07,IER_SCI0_TXI0=0x07,
IER_SCI1_RXI1=0x07,IER_SCI1_TXI1=0x07,
IER_SCI2_RXI2=0x07,IER_SCI2_TXI2=0x07,
IER_ICU_IRQ0=0x08,IER_ICU_IRQ1=0x08,IER_ICU_IRQ2=0x08,IER_ICU_IRQ3=0x08,IER_ICU_IRQ4=0x08,IER_ICU_IRQ5=0x08,IER_ICU_IRQ6=0x08,IER_ICU_IRQ7=0x08,IER_ICU_IRQ8=0x09,IER_ICU_IRQ9=0x09,IER_ICU_IRQ10=0x09,IER_ICU_IRQ11=0x09,IER_ICU_IRQ12=0x09,IER_ICU_IRQ13=0x09,IER_ICU_IRQ14=0x09,IER_ICU_IRQ15=0x09,
IER_SCI3_RXI3=0x0A,IER_SCI3_TXI3=0x0A,
IER_SCI4_RXI4=0x0A,IER_SCI4_TXI4=0x0A,
IER_SCI5_RXI5=0x0A,IER_SCI5_TXI5=0x0A,
IER_SCI6_RXI6=0x0A,IER_SCI6_TXI6=0x0A,
IER_LVD1_LVD1=0x0B,
IER_LVD2_LVD2=0x0B,
IER_RTC_ALM=0x0B,IER_RTC_PRD=0x0B,
IER_REMC0_REMCI0=0x0B,
IER_IWDT_IWUNI=0x0B,
IER_WDT_WUNI=0x0C,
IER_SCI7_RXI7=0x0C,IER_SCI7_TXI7=0x0C,
IER_SCI8_RXI8=0x0C,IER_SCI8_TXI8=0x0C,
IER_SCI9_RXI9=0x0C,IER_SCI9_TXI9=0x0C,
IER_SCI10_RXI10=0x0D,IER_SCI10_TXI10=0x0D,
IER_ICU_GROUPBL2=0x0D,IER_ICU_GROUPBL0=0x0D,IER_ICU_GROUPBL1=0x0D,IER_ICU_GROUPAL0=0x0E,
IER_SCI11_RXI11=0x0E,IER_SCI11_TXI11=0x0E,
IER_SCI12_RXI12=0x0E,IER_SCI12_TXI12=0x0E,
IER_DMAC_DMAC0I=0x0F,IER_DMAC_DMAC1I=0x0F,IER_DMAC_DMAC2I=0x0F,IER_DMAC_DMAC3I=0x0F,IER_DMAC_DMAC74I=0x0F,
IER_OST_OSTDI=0x0F,
IER_PERIB_INTB128=0x10,IER_PERIB_INTB129=0x10,IER_PERIB_INTB130=0x10,IER_PERIB_INTB131=0x10,IER_PERIB_INTB132=0x10,
IER_PERIB_INTB133=0x10,IER_PERIB_INTB134=0x10,IER_PERIB_INTB135=0x10,IER_PERIB_INTB136=0x11,IER_PERIB_INTB137=0x11,
IER_PERIB_INTB138=0x11,IER_PERIB_INTB139=0x11,IER_PERIB_INTB140=0x11,IER_PERIB_INTB141=0x11,IER_PERIB_INTB142=0x11,
IER_PERIB_INTB143=0x11,IER_PERIB_INTB144=0x12,IER_PERIB_INTB145=0x12,IER_PERIB_INTB146=0x12,IER_PERIB_INTB147=0x12,
IER_PERIB_INTB148=0x12,IER_PERIB_INTB149=0x12,IER_PERIB_INTB150=0x12,IER_PERIB_INTB151=0x12,IER_PERIB_INTB152=0x13,
IER_PERIB_INTB153=0x13,IER_PERIB_INTB154=0x13,IER_PERIB_INTB155=0x13,IER_PERIB_INTB156=0x13,IER_PERIB_INTB157=0x13,
IER_PERIB_INTB158=0x13,IER_PERIB_INTB159=0x13,IER_PERIB_INTB160=0x14,IER_PERIB_INTB161=0x14,IER_PERIB_INTB162=0x14,
IER_PERIB_INTB163=0x14,IER_PERIB_INTB164=0x14,IER_PERIB_INTB165=0x14,IER_PERIB_INTB166=0x14,IER_PERIB_INTB167=0x14,
IER_PERIB_INTB168=0x15,IER_PERIB_INTB169=0x15,IER_PERIB_INTB170=0x15,IER_PERIB_INTB171=0x15,IER_PERIB_INTB172=0x15,
IER_PERIB_INTB173=0x15,IER_PERIB_INTB174=0x15,IER_PERIB_INTB175=0x15,IER_PERIB_INTB176=0x16,IER_PERIB_INTB177=0x16,
IER_PERIB_INTB178=0x16,IER_PERIB_INTB179=0x16,IER_PERIB_INTB180=0x16,IER_PERIB_INTB181=0x16,IER_PERIB_INTB182=0x16,
IER_PERIB_INTB183=0x16,IER_PERIB_INTB184=0x17,IER_PERIB_INTB185=0x17,IER_PERIB_INTB186=0x17,IER_PERIB_INTB187=0x17,
IER_PERIB_INTB188=0x17,IER_PERIB_INTB189=0x17,IER_PERIB_INTB190=0x17,IER_PERIB_INTB191=0x17,IER_PERIB_INTB192=0x18,
IER_PERIB_INTB193=0x18,IER_PERIB_INTB194=0x18,IER_PERIB_INTB195=0x18,IER_PERIB_INTB196=0x18,IER_PERIB_INTB197=0x18,
IER_PERIB_INTB198=0x18,IER_PERIB_INTB199=0x18,IER_PERIB_INTB200=0x19,IER_PERIB_INTB201=0x19,IER_PERIB_INTB202=0x19,
IER_PERIB_INTB203=0x19,IER_PERIB_INTB204=0x19,IER_PERIB_INTB205=0x19,IER_PERIB_INTB206=0x19,IER_PERIB_INTB207=0x19,
IER_PERIA_INTA208=0x1A,IER_PERIA_INTA209=0x1A,IER_PERIA_INTA210=0x1A,IER_PERIA_INTA211=0x1A,IER_PERIA_INTA212=0x1A,
IER_PERIA_INTA213=0x1A,IER_PERIA_INTA214=0x1A,IER_PERIA_INTA215=0x1A,IER_PERIA_INTA216=0x1B,IER_PERIA_INTA217=0x1B,
IER_PERIA_INTA218=0x1B,IER_PERIA_INTA219=0x1B,IER_PERIA_INTA220=0x1B,IER_PERIA_INTA221=0x1B,IER_PERIA_INTA222=0x1B,
IER_PERIA_INTA223=0x1B,IER_PERIA_INTA224=0x1C,IER_PERIA_INTA225=0x1C,IER_PERIA_INTA226=0x1C,IER_PERIA_INTA227=0x1C,
IER_PERIA_INTA228=0x1C,IER_PERIA_INTA229=0x1C,IER_PERIA_INTA230=0x1C,IER_PERIA_INTA231=0x1C,IER_PERIA_INTA232=0x1D,
IER_PERIA_INTA233=0x1D,IER_PERIA_INTA234=0x1D,IER_PERIA_INTA235=0x1D,IER_PERIA_INTA236=0x1D,IER_PERIA_INTA237=0x1D,
IER_PERIA_INTA238=0x1D,IER_PERIA_INTA239=0x1D,IER_PERIA_INTA240=0x1E,IER_PERIA_INTA241=0x1E,IER_PERIA_INTA242=0x1E,
IER_PERIA_INTA243=0x1E,IER_PERIA_INTA244=0x1E,IER_PERIA_INTA245=0x1E,IER_PERIA_INTA246=0x1E,IER_PERIA_INTA247=0x1E,
IER_PERIA_INTA248=0x1F,IER_PERIA_INTA249=0x1F,IER_PERIA_INTA250=0x1F,IER_PERIA_INTA251=0x1F,IER_PERIA_INTA252=0x1F,
IER_PERIA_INTA253=0x1F,IER_PERIA_INTA254=0x1F,IER_PERIA_INTA255=0x1F
} enum_ier_t;

typedef enum enum_ipr {
IPR_BSC_BUSERR=0,
IPR_RAM_RAMERR=0,
IPR_FCU_FIFERR=1,IPR_FCU_FRDYI=2,
IPR_ICU_SWINT2=3,IPR_ICU_SWINT=3,
IPR_CMT0_CMI0=4,
IPR_CMT1_CMI1=5,
IPR_CMTW0_CMWI0=6,
IPR_CMTW1_CMWI1=7,
IPR_RSCI10_RXI=32,IPR_RSCI10_TXI=33,
IPR_RSPI0_SPRI0=38,IPR_RSPI0_SPTI0=39,
IPR_RSCI11_RXI=42,IPR_RSCI11_TXI=43,
IPR_RIIC0_RXI0=52,IPR_RIIC0_TXI0=53,
IPR_RIIC2_RXI2=54,IPR_RIIC2_TXI2=55,
IPR_SCI0_RXI0=58,IPR_SCI0_TXI0=59,
IPR_SCI1_RXI1=60,IPR_SCI1_TXI1=61,
IPR_SCI2_RXI2=62,IPR_SCI2_TXI2=63,
IPR_ICU_IRQ0=64,IPR_ICU_IRQ1=65,IPR_ICU_IRQ2=66,IPR_ICU_IRQ3=67,IPR_ICU_IRQ4=68,IPR_ICU_IRQ5=69,IPR_ICU_IRQ6=70,IPR_ICU_IRQ7=71,IPR_ICU_IRQ8=72,IPR_ICU_IRQ9=73,IPR_ICU_IRQ10=74,IPR_ICU_IRQ11=75,IPR_ICU_IRQ12=76,IPR_ICU_IRQ13=77,IPR_ICU_IRQ14=78,IPR_ICU_IRQ15=79,
IPR_SCI3_RXI3=80,IPR_SCI3_TXI3=81,
IPR_SCI4_RXI4=82,IPR_SCI4_TXI4=83,
IPR_SCI5_RXI5=84,IPR_SCI5_TXI5=85,
IPR_SCI6_RXI6=86,IPR_SCI6_TXI6=87,
IPR_LVD1_LVD1=88,
IPR_LVD2_LVD2=89,
IPR_RTC_ALM=92,IPR_RTC_PRD=93,
IPR_REMC0_REMCI0=94,
IPR_IWDT_IWUNI=95,
IPR_WDT_WUNI=96,
IPR_SCI7_RXI7=98,IPR_SCI7_TXI7=99,
IPR_SCI8_RXI8=100,IPR_SCI8_TXI8=101,
IPR_SCI9_RXI9=102,IPR_SCI9_TXI9=103,
IPR_SCI10_RXI10=104,IPR_SCI10_TXI10=105,
IPR_ICU_GROUPBL2=107,IPR_ICU_GROUPBL0=110,IPR_ICU_GROUPBL1=111,IPR_ICU_GROUPAL0=112,
IPR_SCI11_RXI11=114,IPR_SCI11_TXI11=115,
IPR_SCI12_RXI12=116,IPR_SCI12_TXI12=117,
IPR_DMAC_DMAC0I=120,IPR_DMAC_DMAC1I=121,IPR_DMAC_DMAC2I=122,IPR_DMAC_DMAC3I=123,IPR_DMAC_DMAC74I=124,
IPR_OST_OSTDI=125,
IPR_PERIB_INTB128=128,IPR_PERIB_INTB129=129,IPR_PERIB_INTB130=130,IPR_PERIB_INTB131=131,IPR_PERIB_INTB132=132,
IPR_PERIB_INTB133=133,IPR_PERIB_INTB134=134,IPR_PERIB_INTB135=135,IPR_PERIB_INTB136=136,IPR_PERIB_INTB137=137,
IPR_PERIB_INTB138=138,IPR_PERIB_INTB139=139,IPR_PERIB_INTB140=140,IPR_PERIB_INTB141=141,IPR_PERIB_INTB142=142,
IPR_PERIB_INTB143=143,IPR_PERIB_INTB144=144,IPR_PERIB_INTB145=145,IPR_PERIB_INTB146=146,IPR_PERIB_INTB147=147,
IPR_PERIB_INTB148=148,IPR_PERIB_INTB149=149,IPR_PERIB_INTB150=150,IPR_PERIB_INTB151=151,IPR_PERIB_INTB152=152,
IPR_PERIB_INTB153=153,IPR_PERIB_INTB154=154,IPR_PERIB_INTB155=155,IPR_PERIB_INTB156=156,IPR_PERIB_INTB157=157,
IPR_PERIB_INTB158=158,IPR_PERIB_INTB159=159,IPR_PERIB_INTB160=160,IPR_PERIB_INTB161=161,IPR_PERIB_INTB162=162,
IPR_PERIB_INTB163=163,IPR_PERIB_INTB164=164,IPR_PERIB_INTB165=165,IPR_PERIB_INTB166=166,IPR_PERIB_INTB167=167,
IPR_PERIB_INTB168=168,IPR_PERIB_INTB169=169,IPR_PERIB_INTB170=170,IPR_PERIB_INTB171=171,IPR_PERIB_INTB172=172,
IPR_PERIB_INTB173=173,IPR_PERIB_INTB174=174,IPR_PERIB_INTB175=175,IPR_PERIB_INTB176=176,IPR_PERIB_INTB177=177,
IPR_PERIB_INTB178=178,IPR_PERIB_INTB179=179,IPR_PERIB_INTB180=180,IPR_PERIB_INTB181=181,IPR_PERIB_INTB182=182,
IPR_PERIB_INTB183=183,IPR_PERIB_INTB184=184,IPR_PERIB_INTB185=185,IPR_PERIB_INTB186=186,IPR_PERIB_INTB187=187,
IPR_PERIB_INTB188=188,IPR_PERIB_INTB189=189,IPR_PERIB_INTB190=190,IPR_PERIB_INTB191=191,IPR_PERIB_INTB192=192,
IPR_PERIB_INTB193=193,IPR_PERIB_INTB194=194,IPR_PERIB_INTB195=195,IPR_PERIB_INTB196=196,IPR_PERIB_INTB197=197,
IPR_PERIB_INTB198=198,IPR_PERIB_INTB199=199,IPR_PERIB_INTB200=200,IPR_PERIB_INTB201=201,IPR_PERIB_INTB202=202,
IPR_PERIB_INTB203=203,IPR_PERIB_INTB204=204,IPR_PERIB_INTB205=205,IPR_PERIB_INTB206=206,IPR_PERIB_INTB207=207,
IPR_PERIA_INTA208=208,IPR_PERIA_INTA209=209,IPR_PERIA_INTA210=210,IPR_PERIA_INTA211=211,IPR_PERIA_INTA212=212,
IPR_PERIA_INTA213=213,IPR_PERIA_INTA214=214,IPR_PERIA_INTA215=215,IPR_PERIA_INTA216=216,IPR_PERIA_INTA217=217,
IPR_PERIA_INTA218=218,IPR_PERIA_INTA219=219,IPR_PERIA_INTA220=220,IPR_PERIA_INTA221=221,IPR_PERIA_INTA222=222,
IPR_PERIA_INTA223=223,IPR_PERIA_INTA224=224,IPR_PERIA_INTA225=225,IPR_PERIA_INTA226=226,IPR_PERIA_INTA227=227,
IPR_PERIA_INTA228=228,IPR_PERIA_INTA229=229,IPR_PERIA_INTA230=230,IPR_PERIA_INTA231=231,IPR_PERIA_INTA232=232,
IPR_PERIA_INTA233=233,IPR_PERIA_INTA234=234,IPR_PERIA_INTA235=235,IPR_PERIA_INTA236=236,IPR_PERIA_INTA237=237,
IPR_PERIA_INTA238=238,IPR_PERIA_INTA239=239,IPR_PERIA_INTA240=240,IPR_PERIA_INTA241=241,IPR_PERIA_INTA242=242,
IPR_PERIA_INTA243=243,IPR_PERIA_INTA244=244,IPR_PERIA_INTA245=245,IPR_PERIA_INTA246=246,IPR_PERIA_INTA247=247,
IPR_PERIA_INTA248=248,IPR_PERIA_INTA249=249,IPR_PERIA_INTA250=250,IPR_PERIA_INTA251=251,IPR_PERIA_INTA252=252,
IPR_PERIA_INTA253=253,IPR_PERIA_INTA254=254,IPR_PERIA_INTA255=255,
IPR_ICU_SWI=3,
IPR_CMT0_=4,
IPR_CMT1_=5,
IPR_CMTW0_=6,
IPR_CMTW1_=7,
IPR_LVD1_=88,
IPR_LVD2_=89,
IPR_REMC0_=94,
IPR_IWDT_=95,
IPR_WDT_=96,
IPR_OST_=125
} enum_ipr_t;

#pragma bit_order left
#pragma unpack

typedef struct st_bsc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char STSCLR:1;
		} BIT;
	} BERCLR;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TOEN:1;
			unsigned char IGAEN:1;
		} BIT;
	} BEREN;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char MST:3;
			unsigned char :2;
			unsigned char TO:1;
			unsigned char IA:1;
		} BIT;
	} BERSR1;
	char           wk2[1];
	union {
		unsigned short WORD;
		struct {
			unsigned short ADDR:13;
			unsigned short :3;
		} BIT;
	} BERSR2;
	char           wk3[4];
	union {
		unsigned short WORD;
		struct {
			unsigned short :2;
			unsigned short BPEB:2;
			unsigned short BPFB:2;
			unsigned short BPHB:2;
			unsigned short BPGB:2;
			unsigned short BPIB:2;
			unsigned short BPRO:2;
			unsigned short BPRA:2;
		} BIT;
	} BUSPRI;
	char           wk4[7408];
	union {
		unsigned short WORD;
		struct {
			unsigned short PRMOD:1;
			unsigned short :5;
			unsigned short PWENB:1;
			unsigned short PRENB:1;
			unsigned short :4;
			unsigned short EWENB:1;
			unsigned short :2;
			unsigned short WRMOD:1;
		} BIT;
	} CS0MOD;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long CSRWAIT:5;
			unsigned long :3;
			unsigned long CSWWAIT:5;
			unsigned long :5;
			unsigned long CSPRWAIT:3;
			unsigned long :5;
			unsigned long CSPWWAIT:3;
		} BIT;
	} CS0WCR1;
	union {
		unsigned long LONG;
		struct {
			unsigned long :1;
			unsigned long CSON:3;
			unsigned long :1;
			unsigned long WDON:3;
			unsigned long :1;
			unsigned long WRON:3;
			unsigned long :1;
			unsigned long RDON:3;
			unsigned long :2;
			unsigned long AWAIT:2;
			unsigned long :1;
			unsigned long WDOFF:3;
			unsigned long :1;
			unsigned long CSWOFF:3;
			unsigned long :1;
			unsigned long CSROFF:3;
		} BIT;
	} CS0WCR2;
	char           wk5[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short PRMOD:1;
			unsigned short :5;
			unsigned short PWENB:1;
			unsigned short PRENB:1;
			unsigned short :4;
			unsigned short EWENB:1;
			unsigned short :2;
			unsigned short WRMOD:1;
		} BIT;
	} CS1MOD;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long CSRWAIT:5;
			unsigned long :3;
			unsigned long CSWWAIT:5;
			unsigned long :5;
			unsigned long CSPRWAIT:3;
			unsigned long :5;
			unsigned long CSPWWAIT:3;
		} BIT;
	} CS1WCR1;
	union {
		unsigned long LONG;
		struct {
			unsigned long :1;
			unsigned long CSON:3;
			unsigned long :1;
			unsigned long WDON:3;
			unsigned long :1;
			unsigned long WRON:3;
			unsigned long :1;
			unsigned long RDON:3;
			unsigned long :2;
			unsigned long AWAIT:2;
			unsigned long :1;
			unsigned long WDOFF:3;
			unsigned long :1;
			unsigned long CSWOFF:3;
			unsigned long :1;
			unsigned long CSROFF:3;
		} BIT;
	} CS1WCR2;
	char           wk6[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short PRMOD:1;
			unsigned short :5;
			unsigned short PWENB:1;
			unsigned short PRENB:1;
			unsigned short :4;
			unsigned short EWENB:1;
			unsigned short :2;
			unsigned short WRMOD:1;
		} BIT;
	} CS2MOD;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long CSRWAIT:5;
			unsigned long :3;
			unsigned long CSWWAIT:5;
			unsigned long :5;
			unsigned long CSPRWAIT:3;
			unsigned long :5;
			unsigned long CSPWWAIT:3;
		} BIT;
	} CS2WCR1;
	union {
		unsigned long LONG;
		struct {
			unsigned long :1;
			unsigned long CSON:3;
			unsigned long :1;
			unsigned long WDON:3;
			unsigned long :1;
			unsigned long WRON:3;
			unsigned long :1;
			unsigned long RDON:3;
			unsigned long :2;
			unsigned long AWAIT:2;
			unsigned long :1;
			unsigned long WDOFF:3;
			unsigned long :1;
			unsigned long CSWOFF:3;
			unsigned long :1;
			unsigned long CSROFF:3;
		} BIT;
	} CS2WCR2;
	char           wk7[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short PRMOD:1;
			unsigned short :5;
			unsigned short PWENB:1;
			unsigned short PRENB:1;
			unsigned short :4;
			unsigned short EWENB:1;
			unsigned short :2;
			unsigned short WRMOD:1;
		} BIT;
	} CS3MOD;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long CSRWAIT:5;
			unsigned long :3;
			unsigned long CSWWAIT:5;
			unsigned long :5;
			unsigned long CSPRWAIT:3;
			unsigned long :5;
			unsigned long CSPWWAIT:3;
		} BIT;
	} CS3WCR1;
	union {
		unsigned long LONG;
		struct {
			unsigned long :1;
			unsigned long CSON:3;
			unsigned long :1;
			unsigned long WDON:3;
			unsigned long :1;
			unsigned long WRON:3;
			unsigned long :1;
			unsigned long RDON:3;
			unsigned long :2;
			unsigned long AWAIT:2;
			unsigned long :1;
			unsigned long WDOFF:3;
			unsigned long :1;
			unsigned long CSWOFF:3;
			unsigned long :1;
			unsigned long CSROFF:3;
		} BIT;
	} CS3WCR2;
	char           wk8[1990];
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short MPXEN:1;
			unsigned short :3;
			unsigned short EMODE:1;
			unsigned short :2;
			unsigned short BSIZE:2;
			unsigned short :3;
			unsigned short EXENB:1;
		} BIT;
	} CS0CR;
	char           wk9[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :4;
			unsigned short WRCV:4;
			unsigned short :4;
			unsigned short RRCV:4;
		} BIT;
	} CS0REC;
	char           wk10[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short MPXEN:1;
			unsigned short :3;
			unsigned short EMODE:1;
			unsigned short :2;
			unsigned short BSIZE:2;
			unsigned short :3;
			unsigned short EXENB:1;
		} BIT;
	} CS1CR;
	char           wk11[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :4;
			unsigned short WRCV:4;
			unsigned short :4;
			unsigned short RRCV:4;
		} BIT;
	} CS1REC;
	char           wk12[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short MPXEN:1;
			unsigned short :3;
			unsigned short EMODE:1;
			unsigned short :2;
			unsigned short BSIZE:2;
			unsigned short :3;
			unsigned short EXENB:1;
		} BIT;
	} CS2CR;
	char           wk13[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :4;
			unsigned short WRCV:4;
			unsigned short :4;
			unsigned short RRCV:4;
		} BIT;
	} CS2REC;
	char           wk14[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short MPXEN:1;
			unsigned short :3;
			unsigned short EMODE:1;
			unsigned short :2;
			unsigned short BSIZE:2;
			unsigned short :3;
			unsigned short EXENB:1;
		} BIT;
	} CS3CR;
	char           wk15[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short :4;
			unsigned short WRCV:4;
			unsigned short :4;
			unsigned short RRCV:4;
		} BIT;
	} CS3REC;
	char           wk16[68];
	union {
		unsigned short WORD;
		struct {
			unsigned short RCVENM7:1;
			unsigned short RCVENM6:1;
			unsigned short RCVENM5:1;
			unsigned short RCVENM4:1;
			unsigned short RCVENM3:1;
			unsigned short RCVENM2:1;
			unsigned short RCVENM1:1;
			unsigned short RCVENM0:1;
			unsigned short RCVEN7:1;
			unsigned short RCVEN6:1;
			unsigned short RCVEN5:1;
			unsigned short RCVEN4:1;
			unsigned short RCVEN3:1;
			unsigned short RCVEN2:1;
			unsigned short RCVEN1:1;
			unsigned short RCVEN0:1;
		} BIT;
	} CSRECEN;
} st_bsc_t;

typedef struct st_cac {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char CFME:1;
		} BIT;
	} CACR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char EDGES:2;
			unsigned char TCSS:2;
			unsigned char FMCS:3;
			unsigned char CACREFE:1;
		} BIT;
	} CACR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DFS:2;
			unsigned char RCDS:2;
			unsigned char RSCS:3;
			unsigned char RPS:1;
		} BIT;
	} CACR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char OVFFCL:1;
			unsigned char MENDFCL:1;
			unsigned char FERRFCL:1;
			unsigned char :1;
			unsigned char OVFIE:1;
			unsigned char MENDIE:1;
			unsigned char FERRIE:1;
		} BIT;
	} CAICR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char OVFF:1;
			unsigned char MENDF:1;
			unsigned char FERRF:1;
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
			unsigned long ITP:16;
			unsigned long :3;
			unsigned long TSCS:1;
			unsigned long TSP:4;
			unsigned long :2;
			unsigned long OMRC:1;
			unsigned long DLLCS:1;
			unsigned long MME:1;
			unsigned long DRE:1;
			unsigned long DCE:1;
			unsigned long TPRI:1;
		} BIT;
	} GCFG;
	union {
		unsigned long LONG;
		struct {
			unsigned long :15;
			unsigned long TSCR:1;
			unsigned long :4;
			unsigned long POIE:1;
			unsigned long THLIE:1;
			unsigned long MLIE:1;
			unsigned long DEIE:1;
			unsigned long :5;
			unsigned long SLPRQ:1;
			unsigned long MDC:2;
		} BIT;
	} GCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long RAMST:1;
			unsigned long SLPST:1;
			unsigned long HLTST:1;
			unsigned long RSTST:1;
		} BIT;
	} GSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :15;
			unsigned long EEDF0:1;
			unsigned long :12;
			unsigned long PODF:1;
			unsigned long THLDF:1;
			unsigned long MLDF:1;
			unsigned long DEDF:1;
		} BIT;
	} GESR;
	unsigned long  TSCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long AFLWE:1;
			unsigned long :7;
			unsigned long PAGE:1;
		} BIT;
	} AFCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :10;
			unsigned long RN0:6;
			unsigned long :16;
		} BIT;
	} AFCFG;
	union {
		unsigned long LONG;
		struct {
			unsigned long :21;
			unsigned long PLS:3;
			unsigned long :2;
			unsigned long NMB:6;
		} BIT;
	} RMCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long NDF:32;
		} BIT;
	} RMNDR;
	union {
		unsigned long LONG;
		struct {
			unsigned long RMIE31:1;
			unsigned long RMIE30:1;
			unsigned long RMIE29:1;
			unsigned long RMIE28:1;
			unsigned long RMIE27:1;
			unsigned long RMIE26:1;
			unsigned long RMIE25:1;
			unsigned long RMIE24:1;
			unsigned long RMIE23:1;
			unsigned long RMIE22:1;
			unsigned long RMIE21:1;
			unsigned long RMIE20:1;
			unsigned long RMIE19:1;
			unsigned long RMIE18:1;
			unsigned long RMIE17:1;
			unsigned long RMIE16:1;
			unsigned long RMIE15:1;
			unsigned long RMIE14:1;
			unsigned long RMIE13:1;
			unsigned long RMIE12:1;
			unsigned long RMIE11:1;
			unsigned long RMIE10:1;
			unsigned long RMIE9:1;
			unsigned long RMIE8:1;
			unsigned long RMIE7:1;
			unsigned long RMIE6:1;
			unsigned long RMIE5:1;
			unsigned long RMIE4:1;
			unsigned long RMIE3:1;
			unsigned long RMIE2:1;
			unsigned long RMIE1:1;
			unsigned long RMIE0:1;
		} BIT;
	} RMIER;
	union {
		unsigned long LONG;
		struct {
			unsigned long :16;
			unsigned long RFITH:3;
			unsigned long RFIM:1;
			unsigned long :1;
			unsigned long FDS:3;
			unsigned long :1;
			unsigned long PLS:3;
			unsigned long :2;
			unsigned long RFIE:1;
			unsigned long RFE:1;
		} BIT;
	} RFCR[2];
	union {
		unsigned long LONG;
		struct {
			unsigned long :18;
			unsigned long FLVL:6;
			unsigned long :4;
			unsigned long RFIF:1;
			unsigned long LOST:1;
			unsigned long FULL:1;
			unsigned long EMPTY:1;
		} BIT;
	} RFSR[2];
	unsigned long  RFPCR[2];
	union {
		unsigned long LONG;
		struct {
			unsigned long TINT:8;
			unsigned long FDS:3;
			unsigned long :3;
			unsigned long LTM:2;
			unsigned long CFITH:3;
			unsigned long CFIM:1;
			unsigned long ITR:1;
			unsigned long ITCS:1;
			unsigned long :1;
			unsigned long MODE:1;
			unsigned long :1;
			unsigned long PLS:3;
			unsigned long :1;
			unsigned long CFTIE:1;
			unsigned long CFRIE:1;
			unsigned long CFE:1;
		} BIT;
	} CFCR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :18;
			unsigned long FLVL:6;
			unsigned long :3;
			unsigned long CFTIF:1;
			unsigned long CFRIF:1;
			unsigned long LOST:1;
			unsigned long FULL:1;
			unsigned long EMPTY:1;
		} BIT;
	} CFSR0;
	unsigned long  CFPCR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long CFEMP0:1;
			unsigned long :6;
			unsigned long RFEMP1:1;
			unsigned long RFEMP0:1;
		} BIT;
	} FESR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long CFFUL0:1;
			unsigned long :6;
			unsigned long RFFUL1:1;
			unsigned long RFFUL0:1;
		} BIT;
	} FFSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long CFML0:1;
			unsigned long :6;
			unsigned long RFML1:1;
			unsigned long RFML0:1;
		} BIT;
	} FMLSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :30;
			unsigned long RFIF1:1;
			unsigned long RFIF0:1;
		} BIT;
	} RFISR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char ONESHOT:1;
			unsigned char TARQ:1;
			unsigned char TXRQ:1;
		} BIT;
	} TMCR[4];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char TARQS:1;
			unsigned char TXRQS:1;
			unsigned char TXRF:2;
			unsigned char TXSF:1;
		} BIT;
	} TMSR[4];
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long TXRQS3:1;
			unsigned long TXRQS2:1;
			unsigned long TXRQS1:1;
			unsigned long TXRQS0:1;
		} BIT;
	} TMTRSR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long TARQS3:1;
			unsigned long TARQS2:1;
			unsigned long TARQS1:1;
			unsigned long TARQS0:1;
		} BIT;
	} TMARSR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long TXCF3:1;
			unsigned long TXCF2:1;
			unsigned long TXCF1:1;
			unsigned long TXCF0:1;
		} BIT;
	} TMTCSR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long TAF3:1;
			unsigned long TAF2:1;
			unsigned long TAF1:1;
			unsigned long TAF0:1;
		} BIT;
	} TMTASR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long TMIE3:1;
			unsigned long TMIE2:1;
			unsigned long TMIE1:1;
			unsigned long TMIE0:1;
		} BIT;
	} TMIER0;
	char           wk0[24];
	union {
		unsigned long LONG;
		struct {
			unsigned long :27;
			unsigned long THIF0:1;
			unsigned long CFTIF0:1;
			unsigned long TQIF0:1;
			unsigned long TAIF0:1;
			unsigned long TSIF0:1;
		} BIT;
	} TISR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :12;
			unsigned long RTPS:4;
			unsigned long :16;
		} BIT;
	} GTMCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :29;
			unsigned long RTME:1;
			unsigned long :2;
		} BIT;
	} GTMER;
	union {
		unsigned long LONG;
		struct {
			unsigned long :22;
			unsigned long TSCPS:2;
			unsigned long :7;
			unsigned long PXEDIS:1;
		} BIT;
	} GFDCFG;
	char           wk1[4];
	unsigned long  GTMLKR;
	char           wk2[4];
	union {
		unsigned long LONG;
		struct {
			unsigned long :27;
			unsigned long IGES:5;
		} BIT;
	} AFIGSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :16;
			unsigned long KEY:8;
			unsigned long :7;
			unsigned long IGEE:1;
		} BIT;
	} AFIGER;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long CFDTE0:1;
			unsigned long :6;
			unsigned long RFDTE1:1;
			unsigned long RFDTE0:1;
		} BIT;
	} DTCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long CFDTS0:1;
			unsigned long :6;
			unsigned long RFDTS1:1;
			unsigned long RFDTS0:1;
		} BIT;
	} DTSR;
	char           wk3[8];
	union {
		unsigned long LONG;
		struct {
			unsigned long :16;
			unsigned long KEY:8;
			unsigned long :7;
			unsigned long SRST:1;
		} BIT;
	} GRCR;
	char           wk4[68];
	struct {
		union {
			unsigned long LONG;
			struct {
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long LPC:1;
				unsigned long ID:29;
			} BIT;
		} IDR;
		union {
			unsigned long LONG;
			struct {
				unsigned long IDEM:1;
				unsigned long RTRM:1;
				unsigned long IFL1:1;
				unsigned long IDM:29;
			} BIT;
		} MASK;
		union {
			unsigned long LONG;
			struct {
				unsigned long PTR:16;
				unsigned long DMBE:1;
				unsigned long :2;
				unsigned long DMB:5;
				unsigned long IFL0:1;
				unsigned long :3;
				unsigned long DLC:4;
			} BIT;
		} PTR0;
		union {
			unsigned long LONG;
			struct {
				unsigned long :23;
				unsigned long CF0E:1;
				unsigned long :6;
				unsigned long RF1E:1;
				unsigned long RF0E:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long THENT:1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long THENT:1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :28;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
				unsigned long IDE:1;
				unsigned long RTR:1;
				unsigned long :1;
				unsigned long ID:29;
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
				unsigned long DLC:4;
				unsigned long :12;
				unsigned long TS:16;
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
				unsigned long PTR:16;
				unsigned long :6;
				unsigned long IFL:2;
				unsigned long :5;
				unsigned long FDF:1;
				unsigned long BRS:1;
				unsigned long ESI:1;
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
			unsigned long :14;
			unsigned long EC2EAS:1;
			unsigned long EC1EAS:1;
			unsigned long ECEDWC:2;
			unsigned long :2;
			unsigned long ECOVF:1;
			unsigned long EC2EC:1;
			unsigned long EC1EC:1;
			unsigned long :2;
			unsigned long ECEDE:1;
			unsigned long EC1ECD:1;
			unsigned long EC2EIE:1;
			unsigned long EC1EIE:1;
			unsigned long EC2EF:1;
			unsigned long EC1EF:1;
			unsigned long ECEF:1;
		} BIT;
	} ECCSR;
	union {
		unsigned short WORD;
		struct {
			unsigned short ECTMWC:2;
			unsigned short :6;
			unsigned short ECTME:1;
			unsigned short :5;
			unsigned short ECDIS:1;
			unsigned short :1;
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
			unsigned long TSEG2:7;
			unsigned long TSEG1:8;
			unsigned long SJW:7;
			unsigned long BRP:10;
		} BIT;
	} NBCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long ROME:1;
			unsigned long BFT:1;
			unsigned long :3;
			unsigned long CTMS:2;
			unsigned long CTME:1;
			unsigned long EDM:1;
			unsigned long BOM:2;
			unsigned long :1;
			unsigned long TDCVIE:1;
			unsigned long SCOVIE:1;
			unsigned long ECOVIE:1;
			unsigned long TAIE:1;
			unsigned long ALIE:1;
			unsigned long BLIE:1;
			unsigned long OLIE:1;
			unsigned long BORIE:1;
			unsigned long BOEIE:1;
			unsigned long EPIE:1;
			unsigned long EWIE:1;
			unsigned long BEIE:1;
			unsigned long :4;
			unsigned long RTBO:1;
			unsigned long SLPRQ:1;
			unsigned long MDC:2;
		} BIT;
	} CHCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long TEC:8;
			unsigned long REC:8;
			unsigned long :7;
			unsigned long RESI:1;
			unsigned long CRDY:1;
			unsigned long RECST:1;
			unsigned long TRMST:1;
			unsigned long BOST:1;
			unsigned long EPST:1;
			unsigned long SLPST:1;
			unsigned long HLTST:1;
			unsigned long RSTST:1;
		} BIT;
	} CHSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :1;
			unsigned long CRC15:15;
			unsigned long :1;
			unsigned long ADEDF:1;
			unsigned long B0EDF:1;
			unsigned long B1EDF:1;
			unsigned long CEDF:1;
			unsigned long AEDF:1;
			unsigned long FEDF:1;
			unsigned long SEDF:1;
			unsigned long ALDF:1;
			unsigned long BLDF:1;
			unsigned long OLDF:1;
			unsigned long BORDF:1;
			unsigned long BOEDF:1;
			unsigned long EPDF:1;
			unsigned long EWDF:1;
			unsigned long BEDF:1;
		} BIT;
	} CHESR;
	char           wk0[124];
	union {
		unsigned long LONG;
		struct {
			unsigned long :22;
			unsigned long QDS:2;
			unsigned long TQIM:1;
			unsigned long :1;
			unsigned long TQIE:1;
			unsigned long :4;
			unsigned long TQE:1;
		} BIT;
	} TQCR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :21;
			unsigned long FLVL:3;
			unsigned long :5;
			unsigned long TQIF:1;
			unsigned long FULL:1;
			unsigned long EMPTY:1;
		} BIT;
	} TQSR0;
	unsigned long  TQPCR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :21;
			unsigned long THRC:1;
			unsigned long THIM:1;
			unsigned long THIE:1;
			unsigned long :7;
			unsigned long THE:1;
		} BIT;
	} THCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :20;
			unsigned long FLVL:4;
			unsigned long :4;
			unsigned long THIF:1;
			unsigned long LOST:1;
			unsigned long FULL:1;
			unsigned long EMPTY:1;
		} BIT;
	} THSR;
	unsigned long  THPCR;
	char           wk1[92];
	union {
		unsigned long LONG;
		struct {
			unsigned long :4;
			unsigned long SJW:4;
			unsigned long :4;
			unsigned long TSEG2:4;
			unsigned long :3;
			unsigned long TSEG1:5;
			unsigned long BRP:8;
		} BIT;
	} DBCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :1;
			unsigned long CLOE:1;
			unsigned long REFE:1;
			unsigned long FDOE:1;
			unsigned long :4;
			unsigned long TDCO:8;
			unsigned long :5;
			unsigned long TESI:1;
			unsigned long TDCE:1;
			unsigned long SSPC:1;
			unsigned long :5;
			unsigned long ECC:3;
		} BIT;
	} FDCFG;
	union {
		unsigned long LONG;
		struct {
			unsigned long :30;
			unsigned long SCCL:1;
			unsigned long ECCL:1;
		} BIT;
	} FDCTR;
	union {
		unsigned long LONG;
		struct {
			unsigned long SC:8;
			unsigned long EC:8;
			unsigned long TDCV:1;
			unsigned long :5;
			unsigned long SCOV:1;
			unsigned long ECOV:1;
			unsigned long TDCR:8;
		} BIT;
	} FDSTS;
	union {
		unsigned long LONG;
		struct {
			unsigned long :4;
			unsigned long SBC:4;
			unsigned long :3;
			unsigned long CRC21:21;
		} BIT;
	} FDCRC;
	char           wk2[1580];
	union {
		unsigned long LONG;
		struct {
			unsigned long TS:16;
			unsigned long :11;
			unsigned long BN:2;
			unsigned long BT:3;
		} BIT;
	} THACR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :14;
			unsigned long IFL:2;
			unsigned long PTR:16;
		} BIT;
	} THACR1;
} st_canfd0_t;

typedef struct st_cmpc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char HCMPON:1;
			unsigned char CDFS:2;
			unsigned char CEG:2;
			unsigned char :1;
			unsigned char COE:1;
			unsigned char CINV:1;
		} BIT;
	} CMPCTL;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char CMPSEL:4;
		} BIT;
	} CMPSEL0;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char CVRS:4;
		} BIT;
	} CMPSEL1;
	char           wk2[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char CMPMON0:1;
		} BIT;
	} CMPMON;
	char           wk3[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char CPOE:1;
		} BIT;
	} CMPIOC;
} st_cmpc_t;

typedef struct st_cmt {
	union {
		unsigned short WORD;
		struct {
			unsigned short :14;
			unsigned short STR1:1;
			unsigned short STR0:1;
		} BIT;
	} CMSTR0;
	char           wk0[14];
	union {
		unsigned short WORD;
		struct {
			unsigned short :14;
			unsigned short STR3:1;
			unsigned short STR2:1;
		} BIT;
	} CMSTR1;
} st_cmt_t;

typedef struct st_cmt0 {
	union {
		unsigned short WORD;
		struct {
			unsigned short :9;
			unsigned short CMIE:1;
			unsigned short :4;
			unsigned short CKS:2;
		} BIT;
	} CMCR;
	unsigned short CMCNT;
	unsigned short CMCOR;
} st_cmt0_t;

typedef struct st_cmtw {
	union {
		unsigned short WORD;
		struct {
			unsigned short :15;
			unsigned short STR:1;
		} BIT;
	} CMWSTR;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short CCLR:3;
			unsigned short :3;
			unsigned short CMS:1;
			unsigned short :1;
			unsigned short OC1IE:1;
			unsigned short OC0IE:1;
			unsigned short IC1IE:1;
			unsigned short IC0IE:1;
			unsigned short CMWIE:1;
			unsigned short :1;
			unsigned short CKS:2;
		} BIT;
	} CMWCR;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short CMWE:1;
			unsigned short :1;
			unsigned short OC1E:1;
			unsigned short OC0E:1;
			unsigned short OC1:2;
			unsigned short OC0:2;
			unsigned short :2;
			unsigned short IC1E:1;
			unsigned short IC0E:1;
			unsigned short IC1:2;
			unsigned short IC0:2;
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
			unsigned char DORCLR:1;
			unsigned char LMS:1;
			unsigned char :3;
			unsigned char GPS:3;
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
			unsigned char DAOE1:1;
			unsigned char DAOE0:1;
			unsigned char DAE:1;
			unsigned char :5;
		} BIT;
	} DACR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DPSEL:1;
			unsigned char :7;
		} BIT;
	} DADPR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAADST:1;
			unsigned char :7;
		} BIT;
	} DAADSCR;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char OUTREF1:1;
			unsigned char OUTREF0:1;
			unsigned char OUTDA1:1;
			unsigned char OUTDA0:1;
		} BIT;
	} DADSELR;
} st_da_t;

typedef struct st_dmac {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DMST:1;
		} BIT;
	} DMAST;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char DMIS7:1;
			unsigned char DMIS6:1;
			unsigned char DMIS5:1;
			unsigned char DMIS4:1;
			unsigned char :4;
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
			unsigned short MD:2;
			unsigned short DTS:2;
			unsigned short :2;
			unsigned short SZ:2;
			unsigned short :6;
			unsigned short DCTG:2;
		} BIT;
	} DMTMD;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char DTIE:1;
			unsigned char ESIE:1;
			unsigned char RPTIE:1;
			unsigned char SARIE:1;
			unsigned char DARIE:1;
		} BIT;
	} DMINT;
	union {
		unsigned short WORD;
		struct {
			unsigned short SM:2;
			unsigned short :1;
			unsigned short SARA:5;
			unsigned short DM:2;
			unsigned short :1;
			unsigned short DARA:5;
		} BIT;
	} DMAMD;
	char           wk2[2];
	unsigned long  DMOFR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DTE:1;
		} BIT;
	} DMCNT;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char CLRS:1;
			unsigned char :3;
			unsigned char SWREQ:1;
		} BIT;
	} DMREQ;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ACT:1;
			unsigned char :2;
			unsigned char DTIF:1;
			unsigned char :3;
			unsigned char ESIF:1;
		} BIT;
	} DMSTS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DISEL:1;
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
			unsigned short MD:2;
			unsigned short DTS:2;
			unsigned short :2;
			unsigned short SZ:2;
			unsigned short :6;
			unsigned short DCTG:2;
		} BIT;
	} DMTMD;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char DTIE:1;
			unsigned char ESIE:1;
			unsigned char RPTIE:1;
			unsigned char SARIE:1;
			unsigned char DARIE:1;
		} BIT;
	} DMINT;
	union {
		unsigned short WORD;
		struct {
			unsigned short SM:2;
			unsigned short :1;
			unsigned short SARA:5;
			unsigned short DM:2;
			unsigned short :1;
			unsigned short DARA:5;
		} BIT;
	} DMAMD;
	char           wk2[6];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DTE:1;
		} BIT;
	} DMCNT;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char CLRS:1;
			unsigned char :3;
			unsigned char SWREQ:1;
		} BIT;
	} DMREQ;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ACT:1;
			unsigned char :2;
			unsigned char DTIF:1;
			unsigned char :3;
			unsigned char ESIF:1;
		} BIT;
	} DMSTS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DISEL:1;
		} BIT;
	} DMCSL;
} st_dmac1_t;

typedef struct st_doc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char DOPCIE:1;
			unsigned char DCSEL:3;
			unsigned char DOPSZ:1;
			unsigned char :1;
			unsigned char OMS:2;
		} BIT;
	} DOCR;
	char           wk0[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DOPCF:1;
		} BIT;
	} DOSR;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DOPCFCL:1;
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
			unsigned char :3;
			unsigned char RRS:1;
			unsigned char :4;
		} BIT;
	} DTCCR;
	char           wk0[3];
	void          *DTCVBR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SHORT:1;
		} BIT;
	} DTCADMOD;
	char           wk1[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DTCST:1;
		} BIT;
	} DTCST;
	char           wk2[1];
	union {
		unsigned short WORD;
		struct {
			unsigned short ACT:1;
			unsigned short :7;
			unsigned short VECN:8;
		} BIT;
	} DTCSTS;
	void          *DTCIBR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SQTFRL:1;
		} BIT;
	} DTCOR;
	char           wk3[1];
	union {
		unsigned short WORD;
		struct {
			unsigned short ESPSEL:1;
			unsigned short :7;
			unsigned short VECN:8;
		} BIT;
	} DTCSQE;
	unsigned long  DTCDISP;
} st_dtc_t;

typedef struct st_elc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELCON:1;
			unsigned char :7;
		} BIT;
	} ELCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR0;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR4;
	char           wk1[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR7;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR10;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR11;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR12;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR13;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR15;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR16;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR18;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR19;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR20;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR21;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR22;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR23;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR24;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR25;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR26;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR27;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR28;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char MTU3MD:2;
			unsigned char :4;
			unsigned char MTU0MD:2;
		} BIT;
	} ELOPA;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char MTU4MD:2;
		} BIT;
	} ELOPB;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char CMT1MD:2;
			unsigned char :2;
		} BIT;
	} ELOPC;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TMR3MD:2;
			unsigned char TMR2MD:2;
			unsigned char TMR1MD:2;
			unsigned char TMR0MD:2;
		} BIT;
	} ELOPD;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PGR7:1;
			unsigned char PGR6:1;
			unsigned char PGR5:1;
			unsigned char PGR4:1;
			unsigned char PGR3:1;
			unsigned char PGR2:1;
			unsigned char PGR1:1;
			unsigned char PGR0:1;
		} BIT;
	} PGR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PGR7:1;
			unsigned char PGR6:1;
			unsigned char PGR5:1;
			unsigned char PGR4:1;
			unsigned char PGR3:1;
			unsigned char PGR2:1;
			unsigned char PGR1:1;
			unsigned char PGR0:1;
		} BIT;
	} PGR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PGCO:3;
			unsigned char :1;
			unsigned char PGCOVE:1;
			unsigned char PGCI:2;
		} BIT;
	} PGC1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PGCO:3;
			unsigned char :1;
			unsigned char PGCOVE:1;
			unsigned char PGCI:2;
		} BIT;
	} PGC2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PDBF7:1;
			unsigned char PDBF6:1;
			unsigned char PDBF5:1;
			unsigned char PDBF4:1;
			unsigned char PDBF3:1;
			unsigned char PDBF2:1;
			unsigned char PDBF1:1;
			unsigned char PDBF0:1;
		} BIT;
	} PDBF1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PDBF7:1;
			unsigned char PDBF6:1;
			unsigned char PDBF5:1;
			unsigned char PDBF4:1;
			unsigned char PDBF3:1;
			unsigned char PDBF2:1;
			unsigned char PDBF1:1;
			unsigned char PDBF0:1;
		} BIT;
	} PDBF2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PSM:2;
			unsigned char PSP:2;
			unsigned char PSB:3;
		} BIT;
	} PEL0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PSM:2;
			unsigned char PSP:2;
			unsigned char PSB:3;
		} BIT;
	} PEL1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PSM:2;
			unsigned char PSP:2;
			unsigned char PSB:3;
		} BIT;
	} PEL2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PSM:2;
			unsigned char PSP:2;
			unsigned char PSB:3;
		} BIT;
	} PEL3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char WI:1;
			unsigned char WE:1;
			unsigned char :5;
			unsigned char SEG:1;
		} BIT;
	} ELSEGR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR30;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR31;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR32;
	char           wk6[13];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char MTU8MD:2;
			unsigned char MTU7MD:2;
			unsigned char MTU6MD:2;
		} BIT;
	} ELOPE;
	char           wk7[15];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ELS:8;
		} BIT;
	} ELSR56;
} st_elc_t;

typedef struct st_flash {
	char           wk100[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char FLWE:2;
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
			unsigned char CFAE:1;
			unsigned char :2;
			unsigned char CMDLK:1;
			unsigned char DFAE:1;
			unsigned char :3;
		} BIT;
	} FASTAT;
	char           wk3[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CFAEIE:1;
			unsigned char :2;
			unsigned char CMDLKIE:1;
			unsigned char DFAEIE:1;
			unsigned char :3;
		} BIT;
	} FAEINT;
	char           wk4[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char FRDYIE:1;
		} BIT;
	} FRDYIE;
	char           wk5[23];
	union {
		unsigned long LONG;
		struct {
			unsigned long FSADDR:32;
		} BIT;
	} FSADDR;
	union {
		unsigned long LONG;
		struct {
			unsigned long FEADDR:32;
		} BIT;
	} FEADDR;
	char           wk6[72];
	union {
		unsigned long LONG;
		struct {
			unsigned long :16;
			unsigned long FRDY:1;
			unsigned long ILGLERR:1;
			unsigned long ERSERR:1;
			unsigned long PRGERR:1;
			unsigned long SUSRDY:1;
			unsigned long DBFULL:1;
			unsigned long ERSSPD:1;
			unsigned long PRGSPD:1;
			unsigned long :1;
			unsigned long FLWEERR:1;
			unsigned long :6;
		} BIT;
	} FSTATR;
	union {
		unsigned short WORD;
		struct {
			unsigned short KEY:8;
			unsigned short FENTRYD:1;
			unsigned short :6;
			unsigned short FENTRYC:1;
		} BIT;
	} FENTRYR;
	char           wk7[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short KEY:8;
			unsigned short :7;
			unsigned short FPROTCN:1;
		} BIT;
	} FPROTR;
	char           wk8[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short KEY:8;
			unsigned short :7;
			unsigned short SUINIT:1;
		} BIT;
	} FSUINITR;
	char           wk9[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char FLOCKST:1;
		} BIT;
	} FLKSTAT;
	char           wk10[15];
	union {
		unsigned short WORD;
		struct {
			unsigned short CMDR:8;
			unsigned short PCMDR:8;
		} BIT;
	} FCMDR;
	char           wk11[30];
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short PEERRST:8;
		} BIT;
	} FPESTAT;
	char           wk12[14];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char BCDIR:1;
		} BIT;
	} FBCCNT;
	char           wk13[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char BCST:1;
		} BIT;
	} FBCSTAT;
	char           wk14[3];
	union {
		unsigned long LONG;
		struct {
			unsigned long :13;
			unsigned long PSADR:19;
		} BIT;
	} FPSADDR;
	char           wk15[4];
	union {
		unsigned short WORD;
		struct {
			unsigned short :15;
			unsigned short ESUSPMD:1;
		} BIT;
	} FCPSR;
	char           wk16[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short KEY:8;
			unsigned short PCKA:8;
		} BIT;
	} FPCKAR;
} st_flash_t;

typedef struct st_icu {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char IR:1;
		} BIT;
	} IR[256];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DTCE:1;
		} BIT;
	} DTCER[256];
	union {
		unsigned char BYTE;
		struct {
			unsigned char IEN7:1;
			unsigned char IEN6:1;
			unsigned char IEN5:1;
			unsigned char IEN4:1;
			unsigned char IEN3:1;
			unsigned char IEN2:1;
			unsigned char IEN1:1;
			unsigned char IEN0:1;
		} BIT;
	} IER[32];
	char           wk0[192];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SWINT:1;
		} BIT;
	} SWINTR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SWINT2:1;
		} BIT;
	} SWINT2R;
	char           wk1[14];
	union {
		unsigned short WORD;
		struct {
			unsigned short FIEN:1;
			unsigned short :7;
			unsigned short FVCT:8;
		} BIT;
	} FIR;
	char           wk2[14];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char IPR:4;
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
			unsigned char :4;
			unsigned char IRQMD:2;
			unsigned char :2;
		} BIT;
	} IRQCR[16];
	char           wk11[16];
	union {
		unsigned char BYTE;
		struct {
			unsigned char FLTEN7:1;
			unsigned char FLTEN6:1;
			unsigned char FLTEN5:1;
			unsigned char FLTEN4:1;
			unsigned char FLTEN3:1;
			unsigned char FLTEN2:1;
			unsigned char FLTEN1:1;
			unsigned char FLTEN0:1;
		} BIT;
	} IRQFLTE0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char FLTEN15:1;
			unsigned char FLTEN14:1;
			unsigned char FLTEN13:1;
			unsigned char FLTEN12:1;
			unsigned char FLTEN11:1;
			unsigned char FLTEN10:1;
			unsigned char FLTEN9:1;
			unsigned char FLTEN8:1;
		} BIT;
	} IRQFLTE1;
	char           wk12[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short FCLKSEL7:2;
			unsigned short FCLKSEL6:2;
			unsigned short FCLKSEL5:2;
			unsigned short FCLKSEL4:2;
			unsigned short FCLKSEL3:2;
			unsigned short FCLKSEL2:2;
			unsigned short FCLKSEL1:2;
			unsigned short FCLKSEL0:2;
		} BIT;
	} IRQFLTC0;
	union {
		unsigned short WORD;
		struct {
			unsigned short FCLKSEL15:2;
			unsigned short FCLKSEL14:2;
			unsigned short FCLKSEL13:2;
			unsigned short FCLKSEL12:2;
			unsigned short FCLKSEL11:2;
			unsigned short FCLKSEL10:2;
			unsigned short FCLKSEL9:2;
			unsigned short FCLKSEL8:2;
		} BIT;
	} IRQFLTC1;
	char           wk13[84];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char RAMST:1;
			unsigned char LVD2ST:1;
			unsigned char LVD1ST:1;
			unsigned char IWDTST:1;
			unsigned char WDTST:1;
			unsigned char OSTST:1;
			unsigned char NMIST:1;
		} BIT;
	} NMISR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char RAMEN:1;
			unsigned char LVD2EN:1;
			unsigned char LVD1EN:1;
			unsigned char IWDTEN:1;
			unsigned char WDTEN:1;
			unsigned char OSTEN:1;
			unsigned char NMIEN:1;
		} BIT;
	} NMIER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char LVD2CLR:1;
			unsigned char LVD1CLR:1;
			unsigned char IWDTCLR:1;
			unsigned char WDTCLR:1;
			unsigned char OSTCLR:1;
			unsigned char NMICLR:1;
		} BIT;
	} NMICLR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char NMIMD:1;
			unsigned char :3;
		} BIT;
	} NMICR;
	char           wk14[12];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char NFLTEN:1;
		} BIT;
	} NMIFLTE;
	char           wk15[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char NFCLKSEL:2;
		} BIT;
	} NMIFLTC;
	char           wk16[155];
	union {
		unsigned long LONG;
		struct {
			unsigned long IS31:1;
			unsigned long IS30:1;
			unsigned long IS29:1;
			unsigned long IS28:1;
			unsigned long IS27:1;
			unsigned long IS26:1;
			unsigned long IS25:1;
			unsigned long IS24:1;
			unsigned long IS23:1;
			unsigned long IS22:1;
			unsigned long IS21:1;
			unsigned long IS20:1;
			unsigned long IS19:1;
			unsigned long IS18:1;
			unsigned long IS17:1;
			unsigned long IS16:1;
			unsigned long IS15:1;
			unsigned long IS14:1;
			unsigned long IS13:1;
			unsigned long IS12:1;
			unsigned long IS11:1;
			unsigned long IS10:1;
			unsigned long IS9:1;
			unsigned long IS8:1;
			unsigned long IS7:1;
			unsigned long IS6:1;
			unsigned long IS5:1;
			unsigned long IS4:1;
			unsigned long IS3:1;
			unsigned long IS2:1;
			unsigned long IS1:1;
			unsigned long IS0:1;
		} BIT;
	} GRPBL0;
	union {
		unsigned long LONG;
		struct {
			unsigned long IS31:1;
			unsigned long IS30:1;
			unsigned long IS29:1;
			unsigned long IS28:1;
			unsigned long IS27:1;
			unsigned long IS26:1;
			unsigned long IS25:1;
			unsigned long IS24:1;
			unsigned long IS23:1;
			unsigned long IS22:1;
			unsigned long IS21:1;
			unsigned long IS20:1;
			unsigned long IS19:1;
			unsigned long IS18:1;
			unsigned long IS17:1;
			unsigned long IS16:1;
			unsigned long IS15:1;
			unsigned long IS14:1;
			unsigned long IS13:1;
			unsigned long IS12:1;
			unsigned long IS11:1;
			unsigned long IS10:1;
			unsigned long IS9:1;
			unsigned long IS8:1;
			unsigned long IS7:1;
			unsigned long IS6:1;
			unsigned long IS5:1;
			unsigned long IS4:1;
			unsigned long IS3:1;
			unsigned long IS2:1;
			unsigned long IS1:1;
			unsigned long IS0:1;
		} BIT;
	} GRPBL1;
	union {
		unsigned long LONG;
		struct {
			unsigned long IS31:1;
			unsigned long IS30:1;
			unsigned long IS29:1;
			unsigned long IS28:1;
			unsigned long IS27:1;
			unsigned long IS26:1;
			unsigned long IS25:1;
			unsigned long IS24:1;
			unsigned long IS23:1;
			unsigned long IS22:1;
			unsigned long IS21:1;
			unsigned long IS20:1;
			unsigned long IS19:1;
			unsigned long IS18:1;
			unsigned long IS17:1;
			unsigned long IS16:1;
			unsigned long IS15:1;
			unsigned long IS14:1;
			unsigned long IS13:1;
			unsigned long IS12:1;
			unsigned long IS11:1;
			unsigned long IS10:1;
			unsigned long IS9:1;
			unsigned long IS8:1;
			unsigned long IS7:1;
			unsigned long IS6:1;
			unsigned long IS5:1;
			unsigned long IS4:1;
			unsigned long IS3:1;
			unsigned long IS2:1;
			unsigned long IS1:1;
			unsigned long IS0:1;
		} BIT;
	} GRPBL2;
	char           wk17[52];
	union {
		unsigned long LONG;
		struct {
			unsigned long EN31:1;
			unsigned long EN30:1;
			unsigned long EN29:1;
			unsigned long EN28:1;
			unsigned long EN27:1;
			unsigned long EN26:1;
			unsigned long EN25:1;
			unsigned long EN24:1;
			unsigned long EN23:1;
			unsigned long EN22:1;
			unsigned long EN21:1;
			unsigned long EN20:1;
			unsigned long EN19:1;
			unsigned long EN18:1;
			unsigned long EN17:1;
			unsigned long EN16:1;
			unsigned long EN15:1;
			unsigned long EN14:1;
			unsigned long EN13:1;
			unsigned long EN12:1;
			unsigned long EN11:1;
			unsigned long EN10:1;
			unsigned long EN9:1;
			unsigned long EN8:1;
			unsigned long EN7:1;
			unsigned long EN6:1;
			unsigned long EN5:1;
			unsigned long EN4:1;
			unsigned long EN3:1;
			unsigned long EN2:1;
			unsigned long EN1:1;
			unsigned long EN0:1;
		} BIT;
	} GENBL0;
	union {
		unsigned long LONG;
		struct {
			unsigned long EN31:1;
			unsigned long EN30:1;
			unsigned long EN29:1;
			unsigned long EN28:1;
			unsigned long EN27:1;
			unsigned long EN26:1;
			unsigned long EN25:1;
			unsigned long EN24:1;
			unsigned long EN23:1;
			unsigned long EN22:1;
			unsigned long EN21:1;
			unsigned long EN20:1;
			unsigned long EN19:1;
			unsigned long EN18:1;
			unsigned long EN17:1;
			unsigned long EN16:1;
			unsigned long EN15:1;
			unsigned long EN14:1;
			unsigned long EN13:1;
			unsigned long EN12:1;
			unsigned long EN11:1;
			unsigned long EN10:1;
			unsigned long EN9:1;
			unsigned long EN8:1;
			unsigned long EN7:1;
			unsigned long EN6:1;
			unsigned long EN5:1;
			unsigned long EN4:1;
			unsigned long EN3:1;
			unsigned long EN2:1;
			unsigned long EN1:1;
			unsigned long EN0:1;
		} BIT;
	} GENBL1;
	union {
		unsigned long LONG;
		struct {
			unsigned long EN31:1;
			unsigned long EN30:1;
			unsigned long EN29:1;
			unsigned long EN28:1;
			unsigned long EN27:1;
			unsigned long EN26:1;
			unsigned long EN25:1;
			unsigned long EN24:1;
			unsigned long EN23:1;
			unsigned long EN22:1;
			unsigned long EN21:1;
			unsigned long EN20:1;
			unsigned long EN19:1;
			unsigned long EN18:1;
			unsigned long EN17:1;
			unsigned long EN16:1;
			unsigned long EN15:1;
			unsigned long EN14:1;
			unsigned long EN13:1;
			unsigned long EN12:1;
			unsigned long EN11:1;
			unsigned long EN10:1;
			unsigned long EN9:1;
			unsigned long EN8:1;
			unsigned long EN7:1;
			unsigned long EN6:1;
			unsigned long EN5:1;
			unsigned long EN4:1;
			unsigned long EN3:1;
			unsigned long EN2:1;
			unsigned long EN1:1;
			unsigned long EN0:1;
		} BIT;
	} GENBL2;
	char           wk18[132];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBR1;
	char           wk19[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBR5;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBR6;
	char           wk20[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBR8;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBR9;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBRA;
	char           wk21[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBRC;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIBRD;
	char           wk22[114];
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR128;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR129;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR130;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR131;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR132;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR133;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR134;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR135;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR136;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR137;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR138;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR139;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR140;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR141;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR142;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBXR143;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR144;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR145;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR146;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR147;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR148;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR149;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR150;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR151;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR152;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR153;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR154;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR155;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR156;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR157;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR158;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR159;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR160;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR161;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR162;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR163;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR164;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR165;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR166;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR167;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR168;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR169;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR170;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR171;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR172;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR173;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR174;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR175;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR176;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR177;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR178;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR179;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR180;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR181;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR182;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR183;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR184;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR185;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR186;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR187;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR188;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR189;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR190;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR191;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR192;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR193;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR194;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR195;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR196;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR197;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR198;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR199;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR200;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR201;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR202;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR203;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR204;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR205;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR206;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIBR207;
	char           wk23[96];
	union {
		unsigned long LONG;
		struct {
			unsigned long IS31:1;
			unsigned long IS30:1;
			unsigned long IS29:1;
			unsigned long IS28:1;
			unsigned long IS27:1;
			unsigned long IS26:1;
			unsigned long IS25:1;
			unsigned long IS24:1;
			unsigned long IS23:1;
			unsigned long IS22:1;
			unsigned long IS21:1;
			unsigned long IS20:1;
			unsigned long IS19:1;
			unsigned long IS18:1;
			unsigned long IS17:1;
			unsigned long IS16:1;
			unsigned long IS15:1;
			unsigned long IS14:1;
			unsigned long IS13:1;
			unsigned long IS12:1;
			unsigned long IS11:1;
			unsigned long IS10:1;
			unsigned long IS9:1;
			unsigned long IS8:1;
			unsigned long IS7:1;
			unsigned long IS6:1;
			unsigned long IS5:1;
			unsigned long IS4:1;
			unsigned long IS3:1;
			unsigned long IS2:1;
			unsigned long IS1:1;
			unsigned long IS0:1;
		} BIT;
	} GRPAL0;
	char           wk24[60];
	union {
		unsigned long LONG;
		struct {
			unsigned long EN31:1;
			unsigned long EN30:1;
			unsigned long EN29:1;
			unsigned long EN28:1;
			unsigned long EN27:1;
			unsigned long EN26:1;
			unsigned long EN25:1;
			unsigned long EN24:1;
			unsigned long EN23:1;
			unsigned long EN22:1;
			unsigned long EN21:1;
			unsigned long EN20:1;
			unsigned long EN19:1;
			unsigned long EN18:1;
			unsigned long EN17:1;
			unsigned long EN16:1;
			unsigned long EN15:1;
			unsigned long EN14:1;
			unsigned long EN13:1;
			unsigned long EN12:1;
			unsigned long EN11:1;
			unsigned long EN10:1;
			unsigned long EN9:1;
			unsigned long EN8:1;
			unsigned long EN7:1;
			unsigned long EN6:1;
			unsigned long EN5:1;
			unsigned long EN4:1;
			unsigned long EN3:1;
			unsigned long EN2:1;
			unsigned long EN1:1;
			unsigned long EN0:1;
		} BIT;
	} GENAL0;
	char           wk25[140];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIAR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIAR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIAR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIAR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIAR4;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIAR5;
	char           wk26[5];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIARB;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIR7:1;
			unsigned char PIR6:1;
			unsigned char PIR5:1;
			unsigned char PIR4:1;
			unsigned char PIR3:1;
			unsigned char PIR2:1;
			unsigned char PIR1:1;
			unsigned char PIR0:1;
		} BIT;
	} PIARC;
	char           wk27[195];
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR208;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR209;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR210;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR211;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR212;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR213;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR214;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR215;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR216;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR217;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR218;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR219;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR220;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR221;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR222;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR223;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR224;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR225;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR226;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR227;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR228;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR229;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR230;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR231;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR232;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR233;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR234;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR235;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR236;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR237;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR238;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR239;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR240;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR241;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR242;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR243;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR244;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR245;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR246;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR247;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR248;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR249;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR250;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR251;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR252;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR253;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR254;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLI:8;
		} BIT;
	} SLIAR255;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char WPRC:1;
		} BIT;
	} SLIPRCR;
} st_icu_t;

typedef struct st_iwdt {
	unsigned char  IWDTRR;
	char           wk0[1];
	union {
		unsigned short WORD;
		struct {
			unsigned short :2;
			unsigned short RPSS:2;
			unsigned short :2;
			unsigned short RPES:2;
			unsigned short CKS:4;
			unsigned short :2;
			unsigned short TOPS:2;
		} BIT;
	} IWDTCR;
	union {
		unsigned short WORD;
		struct {
			unsigned short REFEF:1;
			unsigned short UNDFF:1;
			unsigned short CNTVAL:14;
		} BIT;
	} IWDTSR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RSTIRQS:1;
			unsigned char :7;
		} BIT;
	} IWDTRCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char SLCSTP:1;
			unsigned char :7;
		} BIT;
	} IWDTCSTPR;
} st_iwdt_t;

typedef struct st_mpc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char CS3E:1;
			unsigned char CS2E:1;
			unsigned char CS1E:1;
			unsigned char CS0E:1;
		} BIT;
	} PFCSE;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CS3S:2;
			unsigned char CS2S:2;
			unsigned char CS1S:2;
			unsigned char CS0S:2;
		} BIT;
	} PFCSS0;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char A15E:1;
			unsigned char A14E:1;
			unsigned char A13E:1;
			unsigned char A12E:1;
			unsigned char A11E:1;
			unsigned char A10E:1;
			unsigned char A9E:1;
			unsigned char A8E:1;
		} BIT;
	} PFAOE0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char A20E:1;
			unsigned char A19E:1;
			unsigned char A18E:1;
			unsigned char A17E:1;
			unsigned char A16E:1;
		} BIT;
	} PFAOE1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char WR1BC1E:1;
			unsigned char :1;
			unsigned char DHE:1;
			unsigned char BCLKO:1;
			unsigned char ADRHMS2:1;
			unsigned char ADRHMS:1;
			unsigned char ADRLE:1;
		} BIT;
	} PFBCR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char ALEOE:1;
			unsigned char WAITS:2;
		} BIT;
	} PFBCR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char D3S:2;
			unsigned char D2S:2;
			unsigned char D1S:2;
			unsigned char D0S:2;
		} BIT;
	} PFBCR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DLHS:1;
		} BIT;
	} PFBCR3;
	char           wk2[21];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B0WI:1;
			unsigned char PFSWE:1;
			unsigned char :6;
		} BIT;
	} PWPR;
	char           wk3[32];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P00PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P01PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P02PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P03PFS;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P05PFS;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P07PFS;
	char           wk6[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P12PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P13PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P14PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P15PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P16PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P17PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P20PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P21PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P22PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P23PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P24PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P25PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P26PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P27PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P30PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P31PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P32PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P33PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P34PFS;
	char           wk7[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P36PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P37PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P40PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P41PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P42PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P43PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P44PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P45PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P46PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P47PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P50PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P51PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P52PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P53PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P54PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P55PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P56PFS;
	char           wk8[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P60PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P61PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P62PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P63PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P64PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P65PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P66PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P67PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P70PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P71PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P72PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} P73PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P74PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P75PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P76PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P77PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P80PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P81PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P82PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P83PFS;
	char           wk9[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P86PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P87PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P90PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P91PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P92PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} P93PFS;
	char           wk10[4];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA0PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA1PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA3PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA4PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA5PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA6PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PA7PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB0PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB1PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB3PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB4PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB5PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB6PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PB7PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC0PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC1PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC3PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC4PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC5PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC6PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PC7PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD0PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD1PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD3PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD4PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD5PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD6PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PD7PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE0PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE1PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE3PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE4PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE5PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE6PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char ASEL:1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PE7PFS;
	char           wk11[5];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char :6;
		} BIT;
	} PF5PFS;
	char           wk12[10];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PH0PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PH1PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PH2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PH3PFS;
	char           wk13[5];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PJ1PFS;
	char           wk14[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PJ3PFS;
	char           wk15[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char ISEL:1;
			unsigned char PSEL:6;
		} BIT;
	} PJ5PFS;
	char           wk16[4];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PK2PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PK3PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PK4PFS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char PSEL:6;
		} BIT;
	} PK5PFS;
} st_mpc_t;

typedef struct st_mpu {
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE0;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE0;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE1;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE1;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE2;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE2;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE3;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE3;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE4;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE4;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE5;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE5;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE6;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE6;
	union {
		unsigned long LONG;
		struct {
			unsigned long RSPN:28;
			unsigned long :4;
		} BIT;
	} RSPAGE7;
	union {
		unsigned long LONG;
		struct {
			unsigned long REPN:28;
			unsigned long UAC:3;
			unsigned long V:1;
		} BIT;
	} REPAGE7;
	char           wk0[192];
	union {
		unsigned long LONG;
		struct {
			unsigned long :31;
			unsigned long MPEN:1;
		} BIT;
	} MPEN;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long UBAC:3;
			unsigned long :1;
		} BIT;
	} MPBAC;
	union {
		unsigned long LONG;
		struct {
			unsigned long :31;
			unsigned long CLR:1;
		} BIT;
	} MPECLR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :29;
			unsigned long DRW:1;
			unsigned long DMPER:1;
			unsigned long IMPER:1;
		} BIT;
	} MPESTS;
	char           wk1[4];
	union {
		unsigned long LONG;
		struct {
			unsigned long DEA:32;
		} BIT;
	} MPDEA;
	char           wk2[8];
	union {
		unsigned long LONG;
		struct {
			unsigned long SA:32;
		} BIT;
	} MPSA;
	union {
		unsigned short WORD;
		struct {
			unsigned short :15;
			unsigned short S:1;
		} BIT;
	} MPOPS;
	union {
		unsigned short WORD;
		struct {
			unsigned short :15;
			unsigned short INV:1;
		} BIT;
	} MPOPI;
	union {
		unsigned long LONG;
		struct {
			unsigned long :8;
			unsigned long HITI:8;
			unsigned long :12;
			unsigned long UHACI:3;
			unsigned long :1;
		} BIT;
	} MHITI;
	union {
		unsigned long LONG;
		struct {
			unsigned long :8;
			unsigned long HITD:8;
			unsigned long :12;
			unsigned long UHACD:3;
			unsigned long :1;
		} BIT;
	} MHITD;
} st_mpu_t;

typedef struct st_mtu {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char OE4D:1;
			unsigned char OE4C:1;
			unsigned char OE3D:1;
			unsigned char OE4B:1;
			unsigned char OE4A:1;
			unsigned char OE3B:1;
		} BIT;
	} TOERA;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char BDC:1;
			unsigned char N:1;
			unsigned char P:1;
			unsigned char FB:1;
			unsigned char WF:1;
			unsigned char VF:1;
			unsigned char UF:1;
		} BIT;
	} TGCRA;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PSYE:1;
			unsigned char :2;
			unsigned char TOCL:1;
			unsigned char TOCS:1;
			unsigned char OLSN:1;
			unsigned char OLSP:1;
		} BIT;
	} TOCR1A;
	union {
		unsigned char BYTE;
		struct {
			unsigned char BF:2;
			unsigned char OLS3N:1;
			unsigned char OLS3P:1;
			unsigned char OLS2N:1;
			unsigned char OLS2P:1;
			unsigned char OLS1N:1;
			unsigned char OLS1P:1;
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
			unsigned char T3AEN:1;
			unsigned char T3ACOR:3;
			unsigned char T4VEN:1;
			unsigned char T4VCOR:3;
		} BIT;
	} TITCR1A;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char T3ACNT:3;
			unsigned char :1;
			unsigned char T4VCNT:3;
		} BIT;
	} TITCNT1A;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char BTE:2;
		} BIT;
	} TBTERA;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TDER:1;
		} BIT;
	} TDERA;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char OLS3N:1;
			unsigned char OLS3P:1;
			unsigned char OLS2N:1;
			unsigned char OLS2P:1;
			unsigned char OLS1N:1;
			unsigned char OLS1P:1;
		} BIT;
	} TOLBRA;
	char           wk6[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TITM:1;
		} BIT;
	} TITMRA;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TRG4COR:3;
		} BIT;
	} TITCR2A;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TRG4CNT:3;
		} BIT;
	} TITCNT2A;
	char           wk7[35];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCE:1;
			unsigned char :5;
			unsigned char SCC:1;
			unsigned char WRE:1;
		} BIT;
	} TWCRA;
	char           wk8[15];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DRS:1;
		} BIT;
	} TMDR2A;
	char           wk9[15];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CST4:1;
			unsigned char CST3:1;
			unsigned char :2;
			unsigned char CST8:1;
			unsigned char CST2:1;
			unsigned char CST1:1;
			unsigned char CST0:1;
		} BIT;
	} TSTRA;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SYNC4:1;
			unsigned char SYNC3:1;
			unsigned char :3;
			unsigned char SYNC2:1;
			unsigned char SYNC1:1;
			unsigned char SYNC0:1;
		} BIT;
	} TSYRA;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SCH0:1;
			unsigned char SCH1:1;
			unsigned char SCH2:1;
			unsigned char SCH3:1;
			unsigned char SCH4:1;
			unsigned char :1;
			unsigned char SCH6:1;
			unsigned char SCH7:1;
		} BIT;
	} TCSYSTR;
	char           wk10[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char RWE:1;
		} BIT;
	} TRWERA;
	char           wk11[1925];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char OE7D:1;
			unsigned char OE7C:1;
			unsigned char OE6D:1;
			unsigned char OE7B:1;
			unsigned char OE7A:1;
			unsigned char OE6B:1;
		} BIT;
	} TOERB;
	char           wk12[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char PSYE:1;
			unsigned char :2;
			unsigned char TOCL:1;
			unsigned char TOCS:1;
			unsigned char OLSN:1;
			unsigned char OLSP:1;
		} BIT;
	} TOCR1B;
	union {
		unsigned char BYTE;
		struct {
			unsigned char BF:2;
			unsigned char OLS3N:1;
			unsigned char OLS3P:1;
			unsigned char OLS2N:1;
			unsigned char OLS2P:1;
			unsigned char OLS1N:1;
			unsigned char OLS1P:1;
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
			unsigned char T6AEN:1;
			unsigned char T6ACOR:3;
			unsigned char T7VEN:1;
			unsigned char T7VCOR:3;
		} BIT;
	} TITCR1B;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char T6ACNT:3;
			unsigned char :1;
			unsigned char T7VCNT:3;
		} BIT;
	} TITCNT1B;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char BTE:2;
		} BIT;
	} TBTERB;
	char           wk16[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TDER:1;
		} BIT;
	} TDERB;
	char           wk17[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char OLS3N:1;
			unsigned char OLS3P:1;
			unsigned char OLS2N:1;
			unsigned char OLS2P:1;
			unsigned char OLS1N:1;
			unsigned char OLS1P:1;
		} BIT;
	} TOLBRB;
	char           wk18[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TITM:1;
		} BIT;
	} TITMRB;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TRG7COR:3;
		} BIT;
	} TITCR2B;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TRG7CNT:3;
		} BIT;
	} TITCNT2B;
	char           wk19[35];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCE:1;
			unsigned char :5;
			unsigned char SCC:1;
			unsigned char WRE:1;
		} BIT;
	} TWCRB;
	char           wk20[15];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char DRS:1;
		} BIT;
	} TMDR2B;
	char           wk21[15];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CST7:1;
			unsigned char CST6:1;
			unsigned char :6;
		} BIT;
	} TSTRB;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SYNC7:1;
			unsigned char SYNC6:1;
			unsigned char :6;
		} BIT;
	} TSYRB;
	char           wk22[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char RWE:1;
		} BIT;
	} TRWERB;
} st_mtu_t;

typedef struct st_mtu0 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR0;
	char           wk0[8];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCRC;
	char           wk1[102];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCLR:3;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char BFE:1;
			unsigned char BFB:1;
			unsigned char BFA:1;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOD:4;
			unsigned char IOC:4;
		} BIT;
	} TIORL;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char :2;
			unsigned char TCIEV:1;
			unsigned char TGIED:1;
			unsigned char TGIEC:1;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
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
			unsigned char TTGE2:1;
			unsigned char :5;
			unsigned char TGIEF:1;
			unsigned char TGIEE:1;
		} BIT;
	} TIER2;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TTSE:1;
			unsigned char TTSB:1;
			unsigned char TTSA:1;
		} BIT;
	} TBTM;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
} st_mtu0_t;

typedef struct st_mtu1 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR1;
	char           wk1[238];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char CCLR:2;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIOR;
	char           wk2[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char :1;
			unsigned char TCIEU:1;
			unsigned char TCIEV:1;
			unsigned char :2;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
		} BIT;
	} TIER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TCFD:1;
			unsigned char :7;
		} BIT;
	} TSR;
	unsigned short TCNT;
	unsigned short TGRA;
	unsigned short TGRB;
	char           wk3[4];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char I2BE:1;
			unsigned char I2AE:1;
			unsigned char I1BE:1;
			unsigned char I1AE:1;
		} BIT;
	} TICCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char PHCKSEL:1;
			unsigned char LWA:1;
		} BIT;
	} TMDR3;
	char           wk4[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char PCB:2;
			unsigned char TPSC2:3;
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
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR2;
	char           wk0[365];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char CCLR:2;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIOR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char :1;
			unsigned char TCIEU:1;
			unsigned char TCIEV:1;
			unsigned char :2;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
		} BIT;
	} TIER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TCFD:1;
			unsigned char :7;
		} BIT;
	} TSR;
	unsigned short TCNT;
	unsigned short TGRA;
	unsigned short TGRB;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char PCB:2;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
} st_mtu2_t;

typedef struct st_mtu3 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCLR:3;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char BFB:1;
			unsigned char BFA:1;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOD:4;
			unsigned char IOC:4;
		} BIT;
	} TIORL;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char :2;
			unsigned char TCIEV:1;
			unsigned char TGIED:1;
			unsigned char TGIEC:1;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
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
			unsigned char TCFD:1;
			unsigned char :7;
		} BIT;
	} TSR;
	char           wk7[11];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TTSB:1;
			unsigned char TTSA:1;
		} BIT;
	} TBTM;
	char           wk8[19];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
	char           wk9[37];
	unsigned short TGRE;
	char           wk10[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR3;
} st_mtu3_t;

typedef struct st_mtu4 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCLR:3;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char BFB:1;
			unsigned char BFA:1;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOD:4;
			unsigned char IOC:4;
		} BIT;
	} TIORL;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char TTGE2:1;
			unsigned char :1;
			unsigned char TCIEV:1;
			unsigned char TGIED:1;
			unsigned char TGIEC:1;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
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
			unsigned char TCFD:1;
			unsigned char :7;
		} BIT;
	} TSR;
	char           wk8[11];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TTSB:1;
			unsigned char TTSA:1;
		} BIT;
	} TBTM;
	char           wk9[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short BF:2;
			unsigned short :6;
			unsigned short UT4AE:1;
			unsigned short DT4AE:1;
			unsigned short UT4BE:1;
			unsigned short DT4BE:1;
			unsigned short ITA3AE:1;
			unsigned short ITA4VE:1;
			unsigned short ITB3AE:1;
			unsigned short ITB4VE:1;
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
			unsigned char :5;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
	char           wk12[38];
	unsigned short TGRE;
	unsigned short TGRF;
	char           wk13[28];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR4;
} st_mtu4_t;

typedef struct st_mtu5 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char :1;
			unsigned char NFWEN:1;
			unsigned char NFVEN:1;
			unsigned char NFUEN:1;
		} BIT;
	} NFCR5;
	char           wk1[490];
	unsigned short TCNTU;
	unsigned short TGRU;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TPSC:2;
		} BIT;
	} TCRU;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char CKEG:2;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2U;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char IOC:5;
		} BIT;
	} TIORU;
	char           wk2[9];
	unsigned short TCNTV;
	unsigned short TGRV;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TPSC:2;
		} BIT;
	} TCRV;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char CKEG:2;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2V;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char IOC:5;
		} BIT;
	} TIORV;
	char           wk3[9];
	unsigned short TCNTW;
	unsigned short TGRW;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TPSC:2;
		} BIT;
	} TCRW;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char CKEG:2;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2W;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char IOC:5;
		} BIT;
	} TIORW;
	char           wk4[11];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TGIE5U:1;
			unsigned char TGIE5V:1;
			unsigned char TGIE5W:1;
		} BIT;
	} TIER;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char CSTU5:1;
			unsigned char CSTV5:1;
			unsigned char CSTW5:1;
		} BIT;
	} TSTR;
	char           wk6[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char CMPCLR5U:1;
			unsigned char CMPCLR5V:1;
			unsigned char CMPCLR5W:1;
		} BIT;
	} TCNTCMPCLR;
} st_mtu5_t;

typedef struct st_mtu6 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCLR:3;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char BFB:1;
			unsigned char BFA:1;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOD:4;
			unsigned char IOC:4;
		} BIT;
	} TIORL;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char :2;
			unsigned char TCIEV:1;
			unsigned char TGIED:1;
			unsigned char TGIEC:1;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
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
			unsigned char TCFD:1;
			unsigned char :7;
		} BIT;
	} TSR;
	char           wk7[11];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TTSB:1;
			unsigned char TTSA:1;
		} BIT;
	} TBTM;
	char           wk8[19];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
	char           wk9[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CE0A:1;
			unsigned char CE0B:1;
			unsigned char CE0C:1;
			unsigned char CE0D:1;
			unsigned char CE1A:1;
			unsigned char CE1B:1;
			unsigned char CE2A:1;
			unsigned char CE2B:1;
		} BIT;
	} TSYCR;
	char           wk10[33];
	unsigned short TGRE;
	char           wk11[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR6;
} st_mtu6_t;

typedef struct st_mtu7 {
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCLR:3;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char BFB:1;
			unsigned char BFA:1;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	char           wk2[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOD:4;
			unsigned char IOC:4;
		} BIT;
	} TIORL;
	char           wk3[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTGE:1;
			unsigned char TTGE2:1;
			unsigned char :1;
			unsigned char TCIEV:1;
			unsigned char TGIED:1;
			unsigned char TGIEC:1;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
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
			unsigned char TCFD:1;
			unsigned char :7;
		} BIT;
	} TSR;
	char           wk8[11];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char TTSB:1;
			unsigned char TTSA:1;
		} BIT;
	} TBTM;
	char           wk9[6];
	union {
		unsigned short WORD;
		struct {
			unsigned short BF:2;
			unsigned short :6;
			unsigned short UT7AE:1;
			unsigned short DT7AE:1;
			unsigned short UT7BE:1;
			unsigned short DT7BE:1;
			unsigned short ITA6AE:1;
			unsigned short ITA7VE:1;
			unsigned short ITB6AE:1;
			unsigned short ITB7VE:1;
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
			unsigned char :5;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
	char           wk12[38];
	unsigned short TGRE;
	unsigned short TGRF;
	char           wk13[28];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR7;
} st_mtu7_t;

typedef struct st_mtu8 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char NFCS:2;
			unsigned char NFDEN:1;
			unsigned char NFCEN:1;
			unsigned char NFBEN:1;
			unsigned char NFAEN:1;
		} BIT;
	} NFCR8;
	char           wk0[871];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CCLR:3;
			unsigned char CKEG:2;
			unsigned char TPSC:3;
		} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char BFB:1;
			unsigned char BFA:1;
			unsigned char MD:4;
		} BIT;
	} TMDR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOB:4;
			unsigned char IOA:4;
		} BIT;
	} TIORH;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IOD:4;
			unsigned char IOC:4;
		} BIT;
	} TIORL;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char TCIEV:1;
			unsigned char TGIED:1;
			unsigned char TGIEC:1;
			unsigned char TGIEB:1;
			unsigned char TGIEA:1;
		} BIT;
	} TIER;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char TPSC2:3;
		} BIT;
	} TCR2;
	char           wk2[1];
	unsigned long  TCNT;
	unsigned long  TGRA;
	unsigned long  TGRB;
	unsigned long  TGRC;
	unsigned long  TGRD;
} st_mtu8_t;

typedef struct st_ofsm {
	union {
		unsigned long LONG;
		struct {
			unsigned long RDPR:1;
			unsigned long WRPR:1;
			unsigned long SEPR:1;
			unsigned long :1;
			unsigned long SPE:1;
			unsigned long :2;
			unsigned long IDE:1;
			unsigned long :6;
			unsigned long OCDE:1;
			unsigned long :17;
		} BIT;
	} SPCC;
	char           wk0[4];
	union {
		unsigned long LONG;
		struct {
			unsigned long :5;
			unsigned long TMEF:3;
			unsigned long :24;
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
			unsigned long :29;
			unsigned long MDE:3;
		} BIT;
	} MDE;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long WDTRSTIRQS:1;
			unsigned long WDTRPSS:2;
			unsigned long WDTRPES:2;
			unsigned long WDTCKS:4;
			unsigned long WDTTOPS:2;
			unsigned long WDTSTRT:1;
			unsigned long :2;
			unsigned long IWDTSLCSTP:1;
			unsigned long :1;
			unsigned long IWDTRSTIRQS:1;
			unsigned long IWDTRPSS:2;
			unsigned long IWDTRPES:2;
			unsigned long IWDTCKS:4;
			unsigned long IWDTTOPS:2;
			unsigned long IWDTSTRT:1;
			unsigned long :1;
		} BIT;
	} OFS0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :23;
			unsigned long HOCOEN:1;
			unsigned long :5;
			unsigned long LVDAS:1;
			unsigned long VDSEL:2;
		} BIT;
	} OFS1;
	char           wk2[12];
	union {
		unsigned long LONG;
		struct {
			unsigned long CODE:32;
		} BIT;
	} ROMCODE;
} st_ofsm_t;

typedef struct st_poe {
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short POE0F:1;
			unsigned short :3;
			unsigned short PIE1:1;
			unsigned short :6;
			unsigned short POE0M:2;
		} BIT;
	} ICSR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short OSF1:1;
			unsigned short :5;
			unsigned short OCE1:1;
			unsigned short OIE1:1;
			unsigned short :8;
		} BIT;
	} OCSR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short POE4F:1;
			unsigned short :3;
			unsigned short PIE2:1;
			unsigned short :6;
			unsigned short POE4M:2;
		} BIT;
	} ICSR2;
	union {
		unsigned short WORD;
		struct {
			unsigned short OSF2:1;
			unsigned short :5;
			unsigned short OCE2:1;
			unsigned short OIE2:1;
			unsigned short :8;
		} BIT;
	} OCSR2;
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short POE8F:1;
			unsigned short :2;
			unsigned short POE8E:1;
			unsigned short PIE3:1;
			unsigned short :6;
			unsigned short POE8M:2;
		} BIT;
	} ICSR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char MTUCH0HIZ:1;
			unsigned char MTUCH67HIZ:1;
			unsigned char MTUCH34HIZ:1;
		} BIT;
	} SPOER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char MTU0DZE:1;
			unsigned char MTU0CZE:1;
			unsigned char MTU0BZE:1;
			unsigned char MTU0AZE:1;
		} BIT;
	} POECR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short MTU3BDZE:1;
			unsigned short MTU4ACZE:1;
			unsigned short MTU4BDZE:1;
			unsigned short :5;
			unsigned short MTU6BDZE:1;
			unsigned short MTU7ACZE:1;
			unsigned short MTU7BDZE:1;
		} BIT;
	} POECR2;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short :2;
			unsigned short IC5ADDMT67ZE:1;
			unsigned short IC4ADDMT67ZE:1;
			unsigned short IC3ADDMT67ZE:1;
			unsigned short :1;
			unsigned short IC1ADDMT67ZE:1;
			unsigned short :3;
			unsigned short IC5ADDMT34ZE:1;
			unsigned short IC4ADDMT34ZE:1;
			unsigned short IC3ADDMT34ZE:1;
			unsigned short IC2ADDMT34ZE:1;
			unsigned short :2;
		} BIT;
	} POECR4;
	union {
		unsigned short WORD;
		struct {
			unsigned short :10;
			unsigned short IC5ADDMT0ZE:1;
			unsigned short IC4ADDMT0ZE:1;
			unsigned short :1;
			unsigned short IC2ADDMT0ZE:1;
			unsigned short IC1ADDMT0ZE:1;
			unsigned short :1;
		} BIT;
	} POECR5;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short POE10F:1;
			unsigned short :2;
			unsigned short POE10E:1;
			unsigned short PIE4:1;
			unsigned short :6;
			unsigned short POE10M:2;
		} BIT;
	} ICSR4;
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short POE11F:1;
			unsigned short :2;
			unsigned short POE11E:1;
			unsigned short PIE5:1;
			unsigned short :6;
			unsigned short POE11M:2;
		} BIT;
	} ICSR5;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short OLSEN:1;
			unsigned short :1;
			unsigned short OLSG2B:1;
			unsigned short OLSG2A:1;
			unsigned short OLSG1B:1;
			unsigned short OLSG1A:1;
			unsigned short OLSG0B:1;
			unsigned short OLSG0A:1;
		} BIT;
	} ALR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short :3;
			unsigned short OSTSTF:1;
			unsigned short :2;
			unsigned short OSTSTE:1;
			unsigned short :9;
		} BIT;
	} ICSR6;
	char           wk2[6];
	union {
		unsigned char BYTE;
		struct {
			unsigned char M0BSEL:4;
			unsigned char M0ASEL:4;
		} BIT;
	} M0SELR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char M0DSEL:4;
			unsigned char M0CSEL:4;
		} BIT;
	} M0SELR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char M3DSEL:4;
			unsigned char M3BSEL:4;
		} BIT;
	} M3SELR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char M4CSEL:4;
			unsigned char M4ASEL:4;
		} BIT;
	} M4SELR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char M4DSEL:4;
			unsigned char M4BSEL:4;
		} BIT;
	} M4SELR2;
} st_poe_t;

typedef struct st_port0 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[62];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port0_t;

typedef struct st_port1 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PMR;
	char           wk3[32];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :4;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[61];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} DSCR;
} st_port1_t;

typedef struct st_port2 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[33];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[60];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port2_t;

typedef struct st_port3 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[34];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :3;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[59];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port3_t;

typedef struct st_port4 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[35];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[58];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
} st_port4_t;

typedef struct st_port5 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[36];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[57];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port5_t;

typedef struct st_port6 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[37];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[56];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port6_t;

typedef struct st_port7 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[38];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[55];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port7_t;

typedef struct st_port8 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[39];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :4;
		} BIT;
	} ODR1;
	char           wk4[54];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port8_t;

typedef struct st_port9 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[40];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	char           wk4[54];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_port9_t;

typedef struct st_porta {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[41];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[52];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_porta_t;

typedef struct st_portb {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[42];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[51];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_portb_t;

typedef struct st_portc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[43];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[50];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_portc_t;

typedef struct st_portd {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[44];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[49];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_portd_t;

typedef struct st_porte {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[45];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[48];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_porte_t;

typedef struct st_portf {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char :5;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char :5;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char :5;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char :5;
		} BIT;
	} PMR;
	char           wk3[47];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} ODR1;
	char           wk4[47];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char :5;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char :5;
		} BIT;
	} DSCR;
} st_portf_t;

typedef struct st_porth {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[48];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :4;
		} BIT;
	} ODR1;
	char           wk4[45];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :2;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_porth_t;

typedef struct st_portj {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char :1;
			unsigned char B1:1;
			unsigned char :1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char :1;
			unsigned char B1:1;
			unsigned char :1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char :1;
			unsigned char B1:1;
			unsigned char :1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char :1;
			unsigned char B1:1;
			unsigned char :1;
		} BIT;
	} PMR;
	char           wk3[49];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :3;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :1;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[44];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char :1;
			unsigned char B1:1;
			unsigned char :1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char :1;
			unsigned char B1:1;
			unsigned char :1;
		} BIT;
	} DSCR;
} st_portj_t;

typedef struct st_portk {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PMR;
	char           wk3[50];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :4;
		} BIT;
	} ODR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR1;
	char           wk4[43];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char B5:1;
			unsigned char B4:1;
			unsigned char B3:1;
			unsigned char B2:1;
			unsigned char :2;
		} BIT;
	} DSCR;
} st_portk_t;

typedef struct st_portl {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PMR;
	char           wk3[51];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char B2:1;
			unsigned char :1;
			unsigned char B0:1;
		} BIT;
	} ODR0;
	char           wk4[43];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char B1:1;
			unsigned char B0:1;
		} BIT;
	} DSCR;
} st_portl_t;

typedef struct st_portn {
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :6;
		} BIT;
	} PDR;
	char           wk0[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :6;
		} BIT;
	} PODR;
	char           wk1[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :6;
		} BIT;
	} PIDR;
	char           wk2[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :6;
		} BIT;
	} PMR;
	char           wk3[54];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char B6:1;
			unsigned char :1;
			unsigned char B4:1;
			unsigned char :4;
		} BIT;
	} ODR1;
	char           wk4[40];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :6;
		} BIT;
	} PCR;
	char           wk5[31];
	union {
		unsigned char BYTE;
		struct {
			unsigned char B7:1;
			unsigned char B6:1;
			unsigned char :6;
		} BIT;
	} DSCR;
} st_portn_t;

typedef struct st_ram {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char RAMMODE:2;
		} BIT;
	} RAMMODE;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char RAMERR:1;
		} BIT;
	} RAMSTS;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char KW:7;
			unsigned char RAMPRCR:1;
		} BIT;
	} RAMPRCR;
	char           wk1[3];
	union {
		unsigned long LONG;
		struct {
			unsigned long :13;
			unsigned long READ:16;
			unsigned long :3;
		} BIT;
	} RAMECAD;
} st_ram_t;

typedef struct st_remc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char FILSEL:1;
			unsigned char :1;
			unsigned char EC:1;
			unsigned char INFLG:1;
			unsigned char FIL:1;
			unsigned char INV:1;
			unsigned char ENFLG:1;
		} BIT;
	} REMCON0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char INTMD:1;
			unsigned char CSRC:4;
			unsigned char EN:1;
			unsigned char TYP:2;
		} BIT;
	} REMCON1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SDFLG:1;
			unsigned char D1FLG:1;
			unsigned char D0FLG:1;
			unsigned char HDFLG:1;
			unsigned char BFULFLG:1;
			unsigned char DRFLG:1;
			unsigned char REFLG:1;
			unsigned char CPFLG:1;
		} BIT;
	} REMSTS;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SDINT:1;
			unsigned char :1;
			unsigned char DINT:1;
			unsigned char HDINT:1;
			unsigned char BFULINT:1;
			unsigned char DRINT:1;
			unsigned char REINT:1;
			unsigned char CPINT:1;
		} BIT;
	} REMINT;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char CPN:4;
		} BIT;
	} REMCPC;
	union {
		unsigned short WORD;
		struct {
			unsigned short CPD:16;
		} BIT;
	} REMCPD;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short HDPMIN:11;
		} BIT;
	} HDPMIN;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short HDPMAX:11;
		} BIT;
	} HDPMAX;
	union {
		unsigned char BYTE;
		struct {
			unsigned char D0PMIN:8;
		} BIT;
	} D0PMIN;
	union {
		unsigned char BYTE;
		struct {
			unsigned char D0PMAX:8;
		} BIT;
	} D0PMAX;
	union {
		unsigned char BYTE;
		struct {
			unsigned char D1PMIN:8;
		} BIT;
	} D1PMIN;
	union {
		unsigned char BYTE;
		struct {
			unsigned char D1PMAX:8;
		} BIT;
	} D1PMAX;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short SDPMIN:11;
		} BIT;
	} SDPMIN;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short SDPMAX:11;
		} BIT;
	} SDPMAX;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short PE:11;
		} BIT;
	} REMPE;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char RBIT:7;
		} BIT;
	} REMRBIT;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT0:8;
		} BIT;
	} REMDAT0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT1:8;
		} BIT;
	} REMDAT1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT2:8;
		} BIT;
	} REMDAT2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT3:8;
		} BIT;
	} REMDAT3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT4:8;
		} BIT;
	} REMDAT4;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT5:8;
		} BIT;
	} REMDAT5;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT6:8;
		} BIT;
	} REMDAT6;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DAT7:8;
		} BIT;
	} REMDAT7;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short TIM:11;
		} BIT;
	} REMTIM;
} st_remc_t;

typedef struct st_riic {
	union {
		unsigned char BYTE;
		struct {
			unsigned char ICE:1;
			unsigned char IICRST:1;
			unsigned char CLO:1;
			unsigned char SOWP:1;
			unsigned char SCLO:1;
			unsigned char SDAO:1;
			unsigned char SCLI:1;
			unsigned char SDAI:1;
		} BIT;
	} ICCR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char BBSY:1;
			unsigned char MST:1;
			unsigned char TRS:1;
			unsigned char :1;
			unsigned char SP:1;
			unsigned char RS:1;
			unsigned char ST:1;
			unsigned char :1;
		} BIT;
	} ICCR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char MTWP:1;
			unsigned char CKS:3;
			unsigned char BCWP:1;
			unsigned char BC:3;
		} BIT;
	} ICMR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DLCS:1;
			unsigned char SDDL:3;
			unsigned char :1;
			unsigned char TMOH:1;
			unsigned char TMOL:1;
			unsigned char TMOS:1;
		} BIT;
	} ICMR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SMBS:1;
			unsigned char WAIT:1;
			unsigned char RDRFS:1;
			unsigned char ACKWP:1;
			unsigned char ACKBT:1;
			unsigned char ACKBR:1;
			unsigned char NF:2;
		} BIT;
	} ICMR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char SCLE:1;
			unsigned char NFE:1;
			unsigned char NACKE:1;
			unsigned char SALE:1;
			unsigned char NALE:1;
			unsigned char MALE:1;
			unsigned char TMOE:1;
		} BIT;
	} ICFER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char HOAE:1;
			unsigned char :1;
			unsigned char DIDE:1;
			unsigned char :1;
			unsigned char GCAE:1;
			unsigned char SAR2E:1;
			unsigned char SAR1E:1;
			unsigned char SAR0E:1;
		} BIT;
	} ICSER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TIE:1;
			unsigned char TEIE:1;
			unsigned char RIE:1;
			unsigned char NAKIE:1;
			unsigned char SPIE:1;
			unsigned char STIE:1;
			unsigned char ALIE:1;
			unsigned char TMOIE:1;
		} BIT;
	} ICIER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char HOA:1;
			unsigned char :1;
			unsigned char DID:1;
			unsigned char :1;
			unsigned char GCA:1;
			unsigned char AAS2:1;
			unsigned char AAS1:1;
			unsigned char AAS0:1;
		} BIT;
	} ICSR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TDRE:1;
			unsigned char TEND:1;
			unsigned char RDRF:1;
			unsigned char NACKF:1;
			unsigned char STOP:1;
			unsigned char START:1;
			unsigned char AL:1;
			unsigned char TMOF:1;
		} BIT;
	} ICSR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SVA:7;
			unsigned char SVA0:1;
		} BIT;
	} SARL0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SVA:2;
			unsigned char FS:1;
		} BIT;
	} SARU0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SVA:7;
			unsigned char SVA0:1;
		} BIT;
	} SARL1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SVA:2;
			unsigned char FS:1;
		} BIT;
	} SARU1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SVA:7;
			unsigned char SVA0:1;
		} BIT;
	} SARL2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SVA:2;
			unsigned char FS:1;
		} BIT;
	} SARU2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char BRL:5;
		} BIT;
	} ICBRL;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char BRH:5;
		} BIT;
	} ICBRH;
	unsigned char  ICDRT;
	unsigned char  ICDRR;
} st_riic_t;

typedef struct st_rsci {
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
			unsigned long :3;
			unsigned long AFER:1;
			unsigned long APER:1;
			unsigned long :2;
			unsigned long ORER:1;
			unsigned long :11;
			unsigned long FER:1;
			unsigned long PER:1;
			unsigned long DR:1;
			unsigned long MPB:1;
			unsigned long RDAT:9;
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
			unsigned long :19;
			unsigned long SYNC:1;
			unsigned long :2;
			unsigned long MPBT:1;
			unsigned long TDAT:9;
		} BIT;
	} TDR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :7;
			unsigned long SSE:1;
			unsigned long :2;
			unsigned long TEIE:1;
			unsigned long TIE:1;
			unsigned long :3;
			unsigned long RIE:1;
			unsigned long :5;
			unsigned long IDSEL:1;
			unsigned long DCME:1;
			unsigned long MPIE:1;
			unsigned long :3;
			unsigned long TE:1;
			unsigned long :3;
			unsigned long RE:1;
		} BIT;
	} SCR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long NFEN:1;
			unsigned long :1;
			unsigned long NFCS:3;
			unsigned long :3;
			unsigned long HDSEL:1;
			unsigned long :3;
			unsigned long LOOP:1;
			unsigned long :2;
			unsigned long RINV:1;
			unsigned long TINV:1;
			unsigned long :2;
			unsigned long PM:1;
			unsigned long PE:1;
			unsigned long :2;
			unsigned long SPB2IO:1;
			unsigned long SPB2DT:1;
			unsigned long :2;
			unsigned long CRSEP:1;
			unsigned long CTSE:1;
		} BIT;
	} SCR1;
	union {
		unsigned long LONG;
		struct {
			unsigned long MDDR:8;
			unsigned long :2;
			unsigned long CKS:2;
			unsigned long :3;
			unsigned long BRME:1;
			unsigned long BRR:8;
			unsigned long :1;
			unsigned long ABCSE:1;
			unsigned long ABCS:1;
			unsigned long BGDM:1;
			unsigned long :1;
			unsigned long BCP:3;
		} BIT;
	} SCR2;
	union {
		unsigned long LONG;
		struct {
			unsigned long :2;
			unsigned long BLK:1;
			unsigned long GM:1;
			unsigned long :1;
			unsigned long ACS0:1;
			unsigned long CKE:2;
			unsigned long :2;
			unsigned long DEEN:1;
			unsigned long FM:1;
			unsigned long MP:1;
			unsigned long MOD:3;
			unsigned long RXDESEL:1;
			unsigned long STOP:1;
			unsigned long DINV:1;
			unsigned long DDIR:1;
			unsigned long :2;
			unsigned long CHR:2;
			unsigned long BPEN:1;
			unsigned long :5;
			unsigned long CPOL:1;
			unsigned long CPHA:1;
		} BIT;
	} SCR3;
	union {
		unsigned long LONG;
		struct {
			unsigned long TTMG:4;
			unsigned long RTMG:4;
			unsigned long :6;
			unsigned long TTADJ:1;
			unsigned long RTADJ:1;
			unsigned long :7;
			unsigned long CMPD:9;
		} BIT;
	} SCR4;
	char           wk0[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char LPS:1;
			unsigned char AOE:1;
			unsigned char :1;
			unsigned char HBSE:1;
		} BIT;
	} HBSCR;
	char           wk1[1];
	union {
		unsigned long LONG;
		struct {
			unsigned long :8;
			unsigned long IICSCLS:2;
			unsigned long IICSDAS:2;
			unsigned long :1;
			unsigned long IICSTPREQ:1;
			unsigned long IICRSTAREQ:1;
			unsigned long IICSTAREQ:1;
			unsigned long :2;
			unsigned long IICACKT:1;
			unsigned long :3;
			unsigned long IICCSC:1;
			unsigned long IICINTM:1;
			unsigned long :3;
			unsigned long IICDL:5;
		} BIT;
	} SIMR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :3;
			unsigned long RSTRG:5;
			unsigned long RFRST:1;
			unsigned long :2;
			unsigned long RTRG:5;
			unsigned long TFRST:1;
			unsigned long :2;
			unsigned long TTRG:5;
			unsigned long :7;
			unsigned long DRES:1;
		} BIT;
	} FCR;
	char           wk2[4];
	union {
		unsigned long LONG;
		struct {
			unsigned long :5;
			unsigned long SBERIE:1;
			unsigned long SYERIE:1;
			unsigned long PFERIE:1;
			unsigned long :2;
			unsigned long RPPAT:2;
			unsigned long RPLEN:4;
			unsigned long :2;
			unsigned long TPPAT:2;
			unsigned long TPLEN:4;
			unsigned long :1;
			unsigned long SBLEN:1;
			unsigned long SYNCE:1;
			unsigned long SBPTN:1;
			unsigned long :1;
			unsigned long SADJE:1;
			unsigned long ENCS:1;
			unsigned long DECS:1;
		} BIT;
	} MMCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :11;
			unsigned long DEHLD:5;
			unsigned long :3;
			unsigned long DESU:5;
			unsigned long :7;
			unsigned long DELVL:1;
		} BIT;
	} DECR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :6;
			unsigned long BCCS:2;
			unsigned long :1;
			unsigned long AEDIE:1;
			unsigned long COFIE:1;
			unsigned long BFDIE:1;
			unsigned long :2;
			unsigned long BCDIE:1;
			unsigned long BFOIE:1;
			unsigned long PIBS:3;
			unsigned long PIBE:1;
			unsigned long CF1DS:2;
			unsigned long CF0RE:1;
			unsigned long BFE:1;
			unsigned long :6;
			unsigned long TCSS:2;
		} BIT;
	} XCR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long CF1CE:8;
			unsigned long SCF1D:8;
			unsigned long PCF1D:8;
			unsigned long :2;
			unsigned long BRME:1;
			unsigned long SDST:1;
			unsigned long :3;
			unsigned long TCST:1;
		} BIT;
	} XCR1;
	union {
		unsigned long LONG;
		struct {
			unsigned long BFLW:16;
			unsigned long CF0CE:8;
			unsigned long CF0D:8;
		} BIT;
	} XCR2;
	char           wk3[8];
	union {
		unsigned long LONG;
		struct {
			unsigned long RDRF:1;
			unsigned long TEND:1;
			unsigned long TDRE:1;
			unsigned long AFER:1;
			unsigned long APER:1;
			unsigned long MFF:1;
			unsigned long :1;
			unsigned long ORER:1;
			unsigned long :5;
			unsigned long DFER:1;
			unsigned long DPER:1;
			unsigned long DCMF:1;
			unsigned long RXDMON:1;
			unsigned long :10;
			unsigned long ERS:1;
			unsigned long :4;
		} BIT;
	} SSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long IICSTIF:1;
			unsigned long :2;
			unsigned long IICACKR:1;
		} BIT;
	} SISR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :2;
			unsigned long FEC:6;
			unsigned long :2;
			unsigned long PEC:6;
			unsigned long :2;
			unsigned long R:6;
			unsigned long :7;
			unsigned long DR:1;
		} BIT;
	} RFSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :26;
			unsigned long T:6;
		} BIT;
	} TFSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :25;
			unsigned long RSYNC:1;
			unsigned long :1;
			unsigned long MCER:1;
			unsigned long :1;
			unsigned long SBER:1;
			unsigned long SYER:1;
			unsigned long PFER:1;
		} BIT;
	} MMSR;
	union {
		unsigned long LONG;
		struct {
			unsigned long CF1RD:8;
			unsigned long CF0RD:8;
			unsigned long AEDF:1;
			unsigned long COF:1;
			unsigned long PIBDF:1;
			unsigned long CF1MF:1;
			unsigned long CF0MF:1;
			unsigned long BFDF:1;
			unsigned long BCDF:1;
			unsigned long BFOF:1;
			unsigned long :6;
			unsigned long RXDSF:1;
			unsigned long SFSF:1;
		} BIT;
	} XSR0;
	union {
		unsigned long LONG;
		struct {
			unsigned long :16;
			unsigned long CCV:16;
		} BIT;
	} XSR1;
	char           wk4[4];
	union {
		unsigned long LONG;
		struct {
			unsigned long RDRFC:1;
			unsigned long :1;
			unsigned long TDREC:1;
			unsigned long AFERC:1;
			unsigned long APERC:1;
			unsigned long MFFC:1;
			unsigned long :1;
			unsigned long ORERC:1;
			unsigned long :5;
			unsigned long DFERC:1;
			unsigned long DPERC:1;
			unsigned long DCMFC:1;
			unsigned long :11;
			unsigned long ERSC:1;
			unsigned long :4;
		} BIT;
	} SSCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :28;
			unsigned long IICSTIFC:1;
			unsigned long :3;
		} BIT;
	} SISCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :31;
			unsigned long DRC:1;
		} BIT;
	} RFSCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :27;
			unsigned long MCERC:1;
			unsigned long :1;
			unsigned long SBERC:1;
			unsigned long SYERC:1;
			unsigned long PFERC:1;
		} BIT;
	} MMSCR;
	union {
		unsigned long LONG;
		struct {
			unsigned long :16;
			unsigned long AEDCL:1;
			unsigned long COFC:1;
			unsigned long PIBDCL:1;
			unsigned long CF1MCL:1;
			unsigned long CF0MCL:1;
			unsigned long BFDCL:1;
			unsigned long BCDCL:1;
			unsigned long BFOC:1;
			unsigned long :8;
		} BIT;
	} XSCR;
} st_rsci_t;

typedef struct st_rspi {
	union {
		unsigned char BYTE;
		struct {
			unsigned char SPRIE:1;
			unsigned char SPE:1;
			unsigned char SPTIE:1;
			unsigned char SPEIE:1;
			unsigned char MSTR:1;
			unsigned char MODFEN:1;
			unsigned char TXMD:1;
			unsigned char SPMS:1;
		} BIT;
	} SPCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char SSL3P:1;
			unsigned char SSL2P:1;
			unsigned char SSL1P:1;
			unsigned char SSL0P:1;
		} BIT;
	} SSLP;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char MOIFE:1;
			unsigned char MOIFV:1;
			unsigned char :2;
			unsigned char SPLP2:1;
			unsigned char SPLP:1;
		} BIT;
	} SPPCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SPRF:1;
			unsigned char SPCF:1;
			unsigned char SPTEF:1;
			unsigned char UDRF:1;
			unsigned char PERF:1;
			unsigned char MODF:1;
			unsigned char IDLNF:1;
			unsigned char OVRF:1;
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
			unsigned char :5;
			unsigned char SPSLN:3;
		} BIT;
	} SPSCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char SPECM:3;
			unsigned char :1;
			unsigned char SPCP:3;
		} BIT;
	} SPSSR;
	unsigned char  SPBR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char SPBYT:1;
			unsigned char SPLW:1;
			unsigned char SPRDTD:1;
			unsigned char :2;
			unsigned char SPFC:2;
		} BIT;
	} SPDCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SCKDL:3;
		} BIT;
	} SPCKD;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SLNDL:3;
		} BIT;
	} SSLND;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SPNDL:3;
		} BIT;
	} SPND;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char SCKASE:1;
			unsigned char PTE:1;
			unsigned char SPIIE:1;
			unsigned char SPOE:1;
			unsigned char SPPE:1;
		} BIT;
	} SPCR2;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD0;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD1;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD2;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD3;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD4;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD5;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD6;
	union {
		unsigned short WORD;
		struct {
			unsigned short SCKDEN:1;
			unsigned short SLNDEN:1;
			unsigned short SPNDEN:1;
			unsigned short LSBF:1;
			unsigned short SPB:4;
			unsigned short SSLKP:1;
			unsigned short SSLA:3;
			unsigned short BRDV:2;
			unsigned short CPOL:1;
			unsigned short CPHA:1;
		} BIT;
	} SPCMD7;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char DINV:1;
			unsigned char BYSW:1;
		} BIT;
	} SPDCR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char SPCIE:1;
			unsigned char :2;
			unsigned char SCKDDIS:1;
			unsigned char RXMD:1;
		} BIT;
	} SPCR3;
} st_rspi_t;

typedef struct st_rtc {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char F1HZ:1;
			unsigned char F2HZ:1;
			unsigned char F4HZ:1;
			unsigned char F8HZ:1;
			unsigned char F16HZ:1;
			unsigned char F32HZ:1;
			unsigned char F64HZ:1;
		} BIT;
	} R64CNT;
	char           wk0[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char SEC10:3;
				unsigned char SEC1:4;
			} BIT;
		} RSECCNT;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNT:8;
			} BIT;
		} BCNT0;
	};
	char           wk1[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char MIN10:3;
				unsigned char MIN1:4;
			} BIT;
		} RMINCNT;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNT:8;
			} BIT;
		} BCNT1;
	};
	char           wk2[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char PM:1;
				unsigned char HR10:2;
				unsigned char HR1:4;
			} BIT;
		} RHRCNT;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNT:8;
			} BIT;
		} BCNT2;
	};
	char           wk3[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :5;
				unsigned char DAYW:3;
			} BIT;
		} RWKCNT;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNT:8;
			} BIT;
		} BCNT3;
	};
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char DATE10:2;
			unsigned char DATE1:4;
		} BIT;
	} RDAYCNT;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char MON10:1;
			unsigned char MON1:4;
		} BIT;
	} RMONCNT;
	char           wk6[1];
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short YR10:4;
			unsigned short YR1:4;
		} BIT;
	} RYRCNT;
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char SEC10:3;
				unsigned char SEC1:4;
			} BIT;
		} RSECAR;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTAR:8;
			} BIT;
		} BCNT0AR;
	};
	char           wk7[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char MIN10:3;
				unsigned char MIN1:4;
			} BIT;
		} RMINAR;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTAR:8;
			} BIT;
		} BCNT1AR;
	};
	char           wk8[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char PM:1;
				unsigned char HR10:2;
				unsigned char HR1:4;
			} BIT;
		} RHRAR;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTAR:8;
			} BIT;
		} BCNT2AR;
	};
	char           wk9[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char :4;
				unsigned char DAYW:3;
			} BIT;
		} RWKAR;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTAR:8;
			} BIT;
		} BCNT3AR;
	};
	char           wk10[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char :1;
				unsigned char DATE10:2;
				unsigned char DATE1:4;
			} BIT;
		} RDAYAR;
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:8;
			} BIT;
		} BCNT0AER;
	};
	char           wk11[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char :2;
				unsigned char MON10:1;
				unsigned char MON1:4;
			} BIT;
		} RMONAR;
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:8;
			} BIT;
		} BCNT1AER;
	};
	char           wk12[1];
	union {
		union {
			unsigned short WORD;
			struct {
				unsigned short :8;
				unsigned short YR10:4;
				unsigned short YR1:4;
			} BIT;
		} RYRAR;
		union {
			unsigned short WORD;
			struct {
				unsigned short :8;
				unsigned short ENB:8;
			} BIT;
		} BCNT2AER;
	};
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:1;
				unsigned char :7;
			} BIT;
		} RYRAREN;
		union {
			unsigned char BYTE;
			struct {
				unsigned char ENB:8;
			} BIT;
		} BCNT3AER;
	};
	char           wk13[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PES:4;
			unsigned char RTCOS:1;
			unsigned char PIE:1;
			unsigned char CIE:1;
			unsigned char AIE:1;
		} BIT;
	} RCR1;
	char           wk14[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CNTMD:1;
			unsigned char HR24:1;
			unsigned char AADJP:1;
			unsigned char AADJE:1;
			unsigned char RTCOE:1;
			unsigned char ADJ30:1;
			unsigned char RESET:1;
			unsigned char START:1;
		} BIT;
	} RCR2;
	char           wk15[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char RTCEN:1;
		} BIT;
	} RCR3;
	char           wk16[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char RCKSEL:1;
		} BIT;
	} RCR4;
	char           wk17[5];
	union {
		unsigned char BYTE;
		struct {
			unsigned char PMADJ:2;
			unsigned char ADJ:6;
		} BIT;
	} RADJ;
	char           wk18[17];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TCEN:1;
			unsigned char :1;
			unsigned char TCNF:2;
			unsigned char :1;
			unsigned char TCST:1;
			unsigned char TCCT:2;
		} BIT;
	} RTCCR0;
	char           wk19[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TCEN:1;
			unsigned char :1;
			unsigned char TCNF:2;
			unsigned char :1;
			unsigned char TCST:1;
			unsigned char TCCT:2;
		} BIT;
	} RTCCR1;
	char           wk20[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TCEN:1;
			unsigned char :1;
			unsigned char TCNF:2;
			unsigned char :1;
			unsigned char TCST:1;
			unsigned char TCCT:2;
		} BIT;
	} RTCCR2;
	char           wk21[13];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char SEC10:3;
				unsigned char SEC1:4;
			} BIT;
		} RSECCP0;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP0:8;
			} BIT;
		} BCNT0CP0;
	};
	char           wk22[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char MIN10:3;
				unsigned char MIN1:4;
			} BIT;
		} RMINCP0;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP0:8;
			} BIT;
		} BCNT1CP0;
	};
	char           wk23[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char PM:1;
				unsigned char HR10:2;
				unsigned char HR1:4;
			} BIT;
		} RHRCP0;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP0:8;
			} BIT;
		} BCNT2CP0;
	};
	char           wk24[3];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP0:8;
			} BIT;
		} BCNT3CP0;
		union {
			unsigned char BYTE;
			struct {
				unsigned char :2;
				unsigned char DATE10:2;
				unsigned char DATE1:4;
			} BIT;
		} RDAYCP0;
	};
	char           wk25[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char MON10:1;
			unsigned char MON1:4;
		} BIT;
	} RMONCP0;
	char           wk26[5];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char SEC10:3;
				unsigned char SEC1:4;
			} BIT;
		} RSECCP1;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP1:8;
			} BIT;
		} BCNT0CP1;
	};
	char           wk27[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char MIN10:3;
				unsigned char MIN1:4;
			} BIT;
		} RMINCP1;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP1:8;
			} BIT;
		} BCNT1CP1;
	};
	char           wk28[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char PM:1;
				unsigned char HR10:2;
				unsigned char HR1:4;
			} BIT;
		} RHRCP1;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP1:8;
			} BIT;
		} BCNT2CP1;
	};
	char           wk29[3];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :2;
				unsigned char DATE10:2;
				unsigned char DATE1:4;
			} BIT;
		} RDAYCP1;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP1:8;
			} BIT;
		} BCNT3CP1;
	};
	char           wk30[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char MON10:1;
			unsigned char MON1:4;
		} BIT;
	} RMONCP1;
	char           wk31[5];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char SEC10:3;
				unsigned char SEC1:4;
			} BIT;
		} RSECCP2;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP2:8;
			} BIT;
		} BCNT0CP2;
	};
	char           wk32[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char MIN10:3;
				unsigned char MIN1:4;
			} BIT;
		} RMINCP2;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP2:8;
			} BIT;
		} BCNT1CP2;
	};
	char           wk33[1];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :1;
				unsigned char PM:1;
				unsigned char HR10:2;
				unsigned char HR1:4;
			} BIT;
		} RHRCP2;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP2:8;
			} BIT;
		} BCNT2CP2;
	};
	char           wk34[3];
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char :2;
				unsigned char DATE10:2;
				unsigned char DATE1:4;
			} BIT;
		} RDAYCP2;
		union {
			unsigned char BYTE;
			struct {
				unsigned char BCNTCP2:8;
			} BIT;
		} BCNT3CP2;
	};
	char           wk35[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char MON10:1;
			unsigned char MON1:4;
		} BIT;
	} RMONCP2;
} st_rtc_t;

typedef struct st_s12ad {
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char VREFSEL:1;
		} BIT;
	} ADVREFCR;
	char           wk0[3963];
	union {
		unsigned short WORD;
		struct {
			unsigned short ADST:1;
			unsigned short ADCS:2;
			unsigned short ADIE:1;
			unsigned short :2;
			unsigned short TRGE:1;
			unsigned short EXTRG:1;
			unsigned short DBLE:1;
			unsigned short GBADIE:1;
			unsigned short :1;
			unsigned short DBLANS:5;
		} BIT;
	} ADCSR;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short ANSA015:1;
			unsigned short ANSA014:1;
			unsigned short ANSA013:1;
			unsigned short ANSA012:1;
			unsigned short ANSA011:1;
			unsigned short ANSA010:1;
			unsigned short ANSA009:1;
			unsigned short ANSA008:1;
			unsigned short ANSA007:1;
			unsigned short ANSA006:1;
			unsigned short ANSA005:1;
			unsigned short ANSA004:1;
			unsigned short ANSA003:1;
			unsigned short ANSA002:1;
			unsigned short ANSA001:1;
			unsigned short ANSA000:1;
		} BIT;
	} ADANSA0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short ANSA107:1;
			unsigned short ANSA106:1;
			unsigned short ANSA105:1;
			unsigned short ANSA104:1;
			unsigned short ANSA103:1;
			unsigned short ANSA102:1;
			unsigned short ANSA101:1;
			unsigned short ANSA100:1;
		} BIT;
	} ADANSA1;
	union {
		unsigned short WORD;
		struct {
			unsigned short ADS015:1;
			unsigned short ADS014:1;
			unsigned short ADS013:1;
			unsigned short ADS012:1;
			unsigned short ADS011:1;
			unsigned short ADS010:1;
			unsigned short ADS009:1;
			unsigned short ADS008:1;
			unsigned short ADS007:1;
			unsigned short ADS006:1;
			unsigned short ADS005:1;
			unsigned short ADS004:1;
			unsigned short ADS003:1;
			unsigned short ADS002:1;
			unsigned short ADS001:1;
			unsigned short ADS000:1;
		} BIT;
	} ADADS0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short ADS107:1;
			unsigned short ADS106:1;
			unsigned short ADS105:1;
			unsigned short ADS104:1;
			unsigned short ADS103:1;
			unsigned short ADS102:1;
			unsigned short ADS101:1;
			unsigned short ADS100:1;
		} BIT;
	} ADADS1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char AVEE:1;
			unsigned char :4;
			unsigned char ADC:3;
		} BIT;
	} ADADC;
	char           wk2[1];
	union {
		unsigned short WORD;
		struct {
			unsigned short ADRFMT:1;
			unsigned short :3;
			unsigned short DIAGM:1;
			unsigned short DIAGLD:1;
			unsigned short DIAGVAL:2;
			unsigned short :2;
			unsigned short ACE:1;
			unsigned short :5;
		} BIT;
	} ADCER;
	union {
		unsigned short WORD;
		struct {
			unsigned short :2;
			unsigned short TRSA:6;
			unsigned short :2;
			unsigned short TRSB:6;
		} BIT;
	} ADSTRGR;
	union {
		unsigned short WORD;
		struct {
			unsigned short :4;
			unsigned short OCSB:1;
			unsigned short TSSB:1;
			unsigned short OCSA:1;
			unsigned short TSSA:1;
			unsigned short :6;
			unsigned short OCSAD:1;
			unsigned short TSSAD:1;
		} BIT;
	} ADEXICR;
	union {
		unsigned short WORD;
		struct {
			unsigned short ANSB015:1;
			unsigned short ANSB014:1;
			unsigned short ANSB013:1;
			unsigned short ANSB012:1;
			unsigned short ANSB011:1;
			unsigned short ANSB010:1;
			unsigned short ANSB009:1;
			unsigned short ANSB008:1;
			unsigned short ANSB007:1;
			unsigned short ANSB006:1;
			unsigned short ANSB005:1;
			unsigned short ANSB004:1;
			unsigned short ANSB003:1;
			unsigned short ANSB002:1;
			unsigned short ANSB001:1;
			unsigned short ANSB000:1;
		} BIT;
	} ADANSB0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short ANSB107:1;
			unsigned short ANSB106:1;
			unsigned short ANSB105:1;
			unsigned short ANSB104:1;
			unsigned short ANSB103:1;
			unsigned short ANSB102:1;
			unsigned short ANSB101:1;
			unsigned short ANSB100:1;
		} BIT;
	} ADANSB1;
	unsigned short ADDBLDR;
	unsigned short ADTSDR;
	unsigned short ADOCDR;
	union {
		unsigned short WORD;
		union {
			struct {
				unsigned short DIAGST:2;
				unsigned short :2;
				unsigned short AD:12;
			} RIGHT;
			struct {
				unsigned short AD:12;
				unsigned short :2;
				unsigned short DIAGST:2;
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
	unsigned short ADDR12;
	unsigned short ADDR13;
	unsigned short ADDR14;
	unsigned short ADDR15;
	unsigned short ADDR16;
	unsigned short ADDR17;
	unsigned short ADDR18;
	unsigned short ADDR19;
	unsigned short ADDR20;
	unsigned short ADDR21;
	unsigned short ADDR22;
	unsigned short ADDR23;
	char           wk3[42];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char ADNDIS:5;
		} BIT;
	} ADDISCR;
	char           wk4[2];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char ELCC:3;
		} BIT;
	} ADELCCR;
	char           wk5[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short GBRP:1;
			unsigned short LGRRS:1;
			unsigned short :12;
			unsigned short GBRSCN:1;
			unsigned short PGS:1;
		} BIT;
	} ADGSPCR;
	char           wk6[2];
	unsigned short ADDBLDRA;
	unsigned short ADDBLDRB;
	char           wk7[4];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char MONCMPB:1;
			unsigned char MONCMPA:1;
			unsigned char :3;
			unsigned char MONCOMB:1;
		} BIT;
	} ADWINMON;
	char           wk8[3];
	union {
		unsigned short WORD;
		struct {
			unsigned short CMPAIE:1;
			unsigned short WCMPE:1;
			unsigned short CMPBIE:1;
			unsigned short :1;
			unsigned short CMPAE:1;
			unsigned short :1;
			unsigned short CMPBE:1;
			unsigned short :7;
			unsigned short CMPAB:2;
		} BIT;
	} ADCMPCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char CMPSOC:1;
			unsigned char CMPSTS:1;
		} BIT;
	} ADCMPANSER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char CMPLOC:1;
			unsigned char CMPLTS:1;
		} BIT;
	} ADCMPLER;
	union {
		unsigned short WORD;
		struct {
			unsigned short CMPCHA015:1;
			unsigned short CMPCHA014:1;
			unsigned short CMPCHA013:1;
			unsigned short CMPCHA012:1;
			unsigned short CMPCHA011:1;
			unsigned short CMPCHA010:1;
			unsigned short CMPCHA009:1;
			unsigned short CMPCHA008:1;
			unsigned short CMPCHA007:1;
			unsigned short CMPCHA006:1;
			unsigned short CMPCHA005:1;
			unsigned short CMPCHA004:1;
			unsigned short CMPCHA003:1;
			unsigned short CMPCHA002:1;
			unsigned short CMPCHA001:1;
			unsigned short CMPCHA000:1;
		} BIT;
	} ADCMPANSR0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short CMPCHA107:1;
			unsigned short CMPCHA106:1;
			unsigned short CMPCHA105:1;
			unsigned short CMPCHA104:1;
			unsigned short CMPCHA103:1;
			unsigned short CMPCHA102:1;
			unsigned short CMPCHA101:1;
			unsigned short CMPCHA100:1;
		} BIT;
	} ADCMPANSR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short CMPLCHA015:1;
			unsigned short CMPLCHA014:1;
			unsigned short CMPLCHA013:1;
			unsigned short CMPLCHA012:1;
			unsigned short CMPLCHA011:1;
			unsigned short CMPLCHA010:1;
			unsigned short CMPLCHA009:1;
			unsigned short CMPLCHA008:1;
			unsigned short CMPLCHA007:1;
			unsigned short CMPLCHA006:1;
			unsigned short CMPLCHA005:1;
			unsigned short CMPLCHA004:1;
			unsigned short CMPLCHA003:1;
			unsigned short CMPLCHA002:1;
			unsigned short CMPLCHA001:1;
			unsigned short CMPLCHA000:1;
		} BIT;
	} ADCMPLR0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short CMPLCHA107:1;
			unsigned short CMPLCHA106:1;
			unsigned short CMPLCHA105:1;
			unsigned short CMPLCHA104:1;
			unsigned short CMPLCHA103:1;
			unsigned short CMPLCHA102:1;
			unsigned short CMPLCHA101:1;
			unsigned short CMPLCHA100:1;
		} BIT;
	} ADCMPLR1;
	unsigned short ADCMPDR0;
	unsigned short ADCMPDR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short CMPSTCHA015:1;
			unsigned short CMPSTCHA014:1;
			unsigned short CMPSTCHA013:1;
			unsigned short CMPSTCHA012:1;
			unsigned short CMPSTCHA011:1;
			unsigned short CMPSTCHA010:1;
			unsigned short CMPSTCHA009:1;
			unsigned short CMPSTCHA008:1;
			unsigned short CMPSTCHA007:1;
			unsigned short CMPSTCHA006:1;
			unsigned short CMPSTCHA005:1;
			unsigned short CMPSTCHA004:1;
			unsigned short CMPSTCHA003:1;
			unsigned short CMPSTCHA002:1;
			unsigned short CMPSTCHA001:1;
			unsigned short CMPSTCHA000:1;
		} BIT;
	} ADCMPSR0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short CMPSTCHA107:1;
			unsigned short CMPSTCHA106:1;
			unsigned short CMPSTCHA105:1;
			unsigned short CMPSTCHA104:1;
			unsigned short CMPSTCHA103:1;
			unsigned short CMPSTCHA102:1;
			unsigned short CMPSTCHA101:1;
			unsigned short CMPSTCHA100:1;
		} BIT;
	} ADCMPSR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char CMPFOC:1;
			unsigned char CMPFTS:1;
		} BIT;
	} ADCMPSER;
	char           wk9[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char CMPLB:1;
			unsigned char :1;
			unsigned char CMPCHB:6;
		} BIT;
	} ADCMPBNSR;
	char           wk10[1];
	unsigned short ADWINLLB;
	unsigned short ADWINULB;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char CMPSTB:1;
		} BIT;
	} ADCMPBSR;
	char           wk11[39];
	union {
		unsigned short WORD;
		struct {
			unsigned short ANSC015:1;
			unsigned short ANSC014:1;
			unsigned short ANSC013:1;
			unsigned short ANSC012:1;
			unsigned short ANSC011:1;
			unsigned short ANSC010:1;
			unsigned short ANSC009:1;
			unsigned short ANSC008:1;
			unsigned short ANSC007:1;
			unsigned short ANSC006:1;
			unsigned short ANSC005:1;
			unsigned short ANSC004:1;
			unsigned short ANSC003:1;
			unsigned short ANSC002:1;
			unsigned short ANSC001:1;
			unsigned short ANSC000:1;
		} BIT;
	} ADANSC0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :8;
			unsigned short ANSC107:1;
			unsigned short ANSC106:1;
			unsigned short ANSC105:1;
			unsigned short ANSC104:1;
			unsigned short ANSC103:1;
			unsigned short ANSC102:1;
			unsigned short ANSC101:1;
			unsigned short ANSC100:1;
		} BIT;
	} ADANSC1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char OCSC:1;
			unsigned char TSSC:1;
		} BIT;
	} ADGCEXCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char GRCE:1;
			unsigned char GCADIE:1;
			unsigned char TRSC:6;
		} BIT;
	} ADGCTRGR;
	char           wk12[3];
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
	unsigned char  ADSSTR12;
	unsigned char  ADSSTR13;
	unsigned char  ADSSTR14;
	unsigned char  ADSSTR15;
	char           wk13[208];
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
	unsigned char  ADSCS12;
	unsigned char  ADSCS13;
	unsigned char  ADSCS14;
	unsigned char  ADSCS15;
	unsigned char  ADSCS16;
	unsigned char  ADSCS17;
	unsigned char  ADSCS18;
	unsigned char  ADSCS19;
	unsigned char  ADSCS20;
	unsigned char  ADSCS21;
	unsigned char  ADSCS22;
	unsigned char  ADSCS23;
	char           wk14[10];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char VDE:1;
		} BIT;
	} ADVMONCR;
	char           wk15[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char VDO:1;
		} BIT;
	} ADVMONO;
} st_s12ad_t;

typedef struct st_sci0 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CM:1;
			unsigned char CHR:1;
			unsigned char PE:1;
			unsigned char PM:1;
			unsigned char STOP:1;
			unsigned char MP:1;
			unsigned char CKS:2;
		} BIT;
	} SMR;
	unsigned char  BRR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TIE:1;
			unsigned char RIE:1;
			unsigned char TE:1;
			unsigned char RE:1;
			unsigned char MPIE:1;
			unsigned char TEIE:1;
			unsigned char CKE:2;
		} BIT;
	} SCR;
	unsigned char  TDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TDRE:1;
			unsigned char RDRF:1;
			unsigned char ORER:1;
			unsigned char FER:1;
			unsigned char PER:1;
			unsigned char TEND:1;
			unsigned char MPB:1;
			unsigned char MPBT:1;
		} BIT;
	} SSR;
	unsigned char  RDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char BCP2:1;
			unsigned char :2;
			unsigned char CHR1:1;
			unsigned char SDIR:1;
			unsigned char SINV:1;
			unsigned char :1;
			unsigned char SMIF:1;
		} BIT;
	} SCMR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RXDESEL:1;
			unsigned char BGDM:1;
			unsigned char NFEN:1;
			unsigned char ABCS:1;
			unsigned char ABCSE:1;
			unsigned char BRME:1;
			unsigned char ITE:1;
			unsigned char ACS0:1;
		} BIT;
	} SEMR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char NFCS:3;
		} BIT;
	} SNFR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IICDL:5;
			unsigned char :2;
			unsigned char IICM:1;
		} BIT;
	} SIMR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char IICACKT:1;
			unsigned char :3;
			unsigned char IICCSC:1;
			unsigned char IICINTM:1;
		} BIT;
	} SIMR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IICSCLS:2;
			unsigned char IICSDAS:2;
			unsigned char IICSTIF:1;
			unsigned char IICSTPREQ:1;
			unsigned char IICRSTAREQ:1;
			unsigned char IICSTAREQ:1;
		} BIT;
	} SIMR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char IICACKR:1;
		} BIT;
	} SISR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char CKPH:1;
			unsigned char CKPOL:1;
			unsigned char :1;
			unsigned char MFF:1;
			unsigned char :1;
			unsigned char MSS:1;
			unsigned char CTSE:1;
			unsigned char SSE:1;
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
			unsigned char DCME:1;
			unsigned char IDSEL:1;
			unsigned char :1;
			unsigned char DFER:1;
			unsigned char DPER:1;
			unsigned char :2;
			unsigned char DCMF:1;
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
			unsigned short :7;
			unsigned short CMPD:9;
		} BIT;
	} CDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTADJ:1;
			unsigned char RTADJ:1;
			unsigned char TINV:1;
			unsigned char RINV:1;
			unsigned char :1;
			unsigned char SPB2IO:1;
			unsigned char SPB2DT:1;
			unsigned char RXDMON:1;
		} BIT;
	} SPTR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTMG:4;
			unsigned char RTMG:4;
		} BIT;
	} TMGR;
} st_sci0_t;

typedef struct st_sci10 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CM:1;
			unsigned char CHR:1;
			unsigned char PE:1;
			unsigned char PM:1;
			unsigned char STOP:1;
			unsigned char MP:1;
			unsigned char CKS:2;
		} BIT;
	} SMR;
	unsigned char  BRR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TIE:1;
			unsigned char RIE:1;
			unsigned char TE:1;
			unsigned char RE:1;
			unsigned char MPIE:1;
			unsigned char TEIE:1;
			unsigned char CKE:2;
		} BIT;
	} SCR;
	unsigned char  TDR;
	union {
		union {
			unsigned char BYTE;
			struct {
				unsigned char TDFE:1;
				unsigned char RDF:1;
				unsigned char ORER:1;
				unsigned char FER:1;
				unsigned char PER:1;
				unsigned char TEND:1;
				unsigned char :1;
				unsigned char DR:1;
			} BIT;
		} SSRFIFO;
		union {
			unsigned char BYTE;
			struct {
				unsigned char TDRE:1;
				unsigned char RDRF:1;
				unsigned char ORER:1;
				unsigned char FER:1;
				unsigned char PER:1;
				unsigned char TEND:1;
				unsigned char MPB:1;
				unsigned char MPBT:1;
			} BIT;
		} SSR;
	};
	unsigned char  RDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char BCP2:1;
			unsigned char :2;
			unsigned char CHR1:1;
			unsigned char SDIR:1;
			unsigned char SINV:1;
			unsigned char :1;
			unsigned char SMIF:1;
		} BIT;
	} SCMR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RXDESEL:1;
			unsigned char BGDM:1;
			unsigned char NFEN:1;
			unsigned char ABCS:1;
			unsigned char ABCSE:1;
			unsigned char BRME:1;
			unsigned char ITE:1;
			unsigned char ACS0:1;
		} BIT;
	} SEMR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char NFCS:3;
		} BIT;
	} SNFR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IICDL:5;
			unsigned char :2;
			unsigned char IICM:1;
		} BIT;
	} SIMR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char IICACKT:1;
			unsigned char :3;
			unsigned char IICCSC:1;
			unsigned char IICINTM:1;
		} BIT;
	} SIMR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IICSCLS:2;
			unsigned char IICSDAS:2;
			unsigned char IICSTIF:1;
			unsigned char IICSTPREQ:1;
			unsigned char IICRSTAREQ:1;
			unsigned char IICSTAREQ:1;
		} BIT;
	} SIMR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char IICACKR:1;
		} BIT;
	} SISR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char CKPH:1;
			unsigned char CKPOL:1;
			unsigned char :1;
			unsigned char MFF:1;
			unsigned char :1;
			unsigned char MSS:1;
			unsigned char CTSE:1;
			unsigned char SSE:1;
		} BIT;
	} SPMR;
	union {
		union {
			unsigned short WORD;
			struct {
				unsigned char H;
				unsigned char L;
			} BYTE;
			struct {
				unsigned short :6;
				unsigned short MPBT:1;
				unsigned short TDAT:9;
			} BIT;
		} FTDR;
		union {
			unsigned short WORD;
			struct {
				unsigned char TDRH;
				unsigned char TDRL;
			} BYTE;
		} TDRHL;
	};
	union {
		union {
			unsigned short WORD;
			struct {
				unsigned char H;
				unsigned char L;
			} BYTE;
			struct {
				unsigned short :1;
				unsigned short RDF:1;
				unsigned short ORER:1;
				unsigned short FER:1;
				unsigned short PER:1;
				unsigned short DR:1;
				unsigned short MPB:1;
				unsigned short RDAT:9;
			} BIT;
		} FRDR;
		union {
			unsigned short WORD;
			struct {
				unsigned char RDRH;
				unsigned char RDRL;
			} BYTE;
		} RDRHL;
	};
	unsigned char  MDDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DCME:1;
			unsigned char IDSEL:1;
			unsigned char :1;
			unsigned char DFER:1;
			unsigned char DPER:1;
			unsigned char :2;
			unsigned char DCMF:1;
		} BIT;
	} DCCR;
	union {
		unsigned short WORD;
		struct {
			unsigned char H;
			unsigned char L;
		} BYTE;
		struct {
			unsigned char RSTRG:4;
			unsigned char RTRG:4;
			unsigned char TTRG:4;
			unsigned char DRES:1;
			unsigned char TFRST:1;
			unsigned char RFRST:1;
			unsigned char FM:1;
		} BIT;
	} FCR;
	union {
		unsigned short WORD;
		struct {
			unsigned char H;
			unsigned char L;
		} BYTE;
		struct {
			unsigned char :3;
			unsigned char T:5;
			unsigned char :3;
			unsigned char R:5;
		} BIT;
	} FDR;
	union {
		unsigned short WORD;
		struct {
			unsigned char H;
			unsigned char L;
		} BYTE;
		struct {
			unsigned char :3;
			unsigned char PNUM:5;
			unsigned char :1;
			unsigned char FNUM:5;
			unsigned char :1;
			unsigned char ORER:1;
		} BIT;
	} LSR;
	union {
		unsigned short WORD;
		struct {
			unsigned char H;
			unsigned char L;
		} BYTE;
		struct {
			unsigned short :7;
			unsigned short CMPD:9;
		} BIT;
	} CDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTADJ:1;
			unsigned char RTADJ:1;
			unsigned char TINV:1;
			unsigned char RINV:1;
			unsigned char :1;
			unsigned char SPB2IO:1;
			unsigned char SPB2DT:1;
			unsigned char RXDMON:1;
		} BIT;
	} SPTR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TTMG:4;
			unsigned char RTMG:4;
		} BIT;
	} TMGR;
} st_sci10_t;

typedef struct st_sci12 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CM:1;
			unsigned char CHR:1;
			unsigned char PE:1;
			unsigned char PM:1;
			unsigned char STOP:1;
			unsigned char MP:1;
			unsigned char CKS:2;
		} BIT;
	} SMR;
	unsigned char  BRR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TIE:1;
			unsigned char RIE:1;
			unsigned char TE:1;
			unsigned char RE:1;
			unsigned char MPIE:1;
			unsigned char TEIE:1;
			unsigned char CKE:2;
		} BIT;
	} SCR;
	unsigned char  TDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char TDRE:1;
			unsigned char RDRF:1;
			unsigned char ORER:1;
			unsigned char FER:1;
			unsigned char PER:1;
			unsigned char TEND:1;
			unsigned char MPB:1;
			unsigned char MPBT:1;
		} BIT;
	} SSR;
	unsigned char  RDR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char BCP2:1;
			unsigned char :2;
			unsigned char CHR1:1;
			unsigned char SDIR:1;
			unsigned char SINV:1;
			unsigned char :1;
			unsigned char SMIF:1;
		} BIT;
	} SCMR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RXDESEL:1;
			unsigned char BGDM:1;
			unsigned char NFEN:1;
			unsigned char ABCS:1;
			unsigned char :1;
			unsigned char BRME:1;
			unsigned char ITE:1;
			unsigned char ACS0:1;
		} BIT;
	} SEMR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char NFCS:3;
		} BIT;
	} SNFR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IICDL:5;
			unsigned char :2;
			unsigned char IICM:1;
		} BIT;
	} SIMR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char IICACKT:1;
			unsigned char :3;
			unsigned char IICCSC:1;
			unsigned char IICINTM:1;
		} BIT;
	} SIMR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char IICSCLS:2;
			unsigned char IICSDAS:2;
			unsigned char IICSTIF:1;
			unsigned char IICSTPREQ:1;
			unsigned char IICRSTAREQ:1;
			unsigned char IICSTAREQ:1;
		} BIT;
	} SIMR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char IICACKR:1;
		} BIT;
	} SISR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char CKPH:1;
			unsigned char CKPOL:1;
			unsigned char :1;
			unsigned char MFF:1;
			unsigned char :1;
			unsigned char MSS:1;
			unsigned char CTSE:1;
			unsigned char SSE:1;
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
			unsigned char :7;
			unsigned char ESME:1;
		} BIT;
	} ESMER;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char BRME:1;
			unsigned char RXDSF:1;
			unsigned char SFSF:1;
			unsigned char :1;
		} BIT;
	} CR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char PIBS:3;
			unsigned char PIBE:1;
			unsigned char CF1DS:2;
			unsigned char CF0RE:1;
			unsigned char BFE:1;
		} BIT;
	} CR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RTS:2;
			unsigned char BCCS:2;
			unsigned char :1;
			unsigned char DFCS:3;
		} BIT;
	} CR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SDST:1;
		} BIT;
	} CR3;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char SHARPS:1;
			unsigned char :2;
			unsigned char RXDXPS:1;
			unsigned char TXDXPS:1;
		} BIT;
	} PCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char AEDIE:1;
			unsigned char BCDIE:1;
			unsigned char PIBDIE:1;
			unsigned char CF1MIE:1;
			unsigned char CF0MIE:1;
			unsigned char BFDIE:1;
		} BIT;
	} ICR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char AEDF:1;
			unsigned char BCDF:1;
			unsigned char PIBDF:1;
			unsigned char CF1MF:1;
			unsigned char CF0MF:1;
			unsigned char BFDF:1;
		} BIT;
	} STR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :2;
			unsigned char AEDCL:1;
			unsigned char BCDCL:1;
			unsigned char PIBDCL:1;
			unsigned char CF1MCL:1;
			unsigned char CF0MCL:1;
			unsigned char BFDCL:1;
		} BIT;
	} STCR;
	unsigned char  CF0DR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char CF0CE7:1;
			unsigned char CF0CE6:1;
			unsigned char CF0CE5:1;
			unsigned char CF0CE4:1;
			unsigned char CF0CE3:1;
			unsigned char CF0CE2:1;
			unsigned char CF0CE1:1;
			unsigned char CF0CE0:1;
		} BIT;
	} CF0CR;
	unsigned char  CF0RR;
	unsigned char  PCF1DR;
	unsigned char  SCF1DR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char CF1CE7:1;
			unsigned char CF1CE6:1;
			unsigned char CF1CE5:1;
			unsigned char CF1CE4:1;
			unsigned char CF1CE3:1;
			unsigned char CF1CE2:1;
			unsigned char CF1CE1:1;
			unsigned char CF1CE0:1;
		} BIT;
	} CF1CR;
	unsigned char  CF1RR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TCST:1;
		} BIT;
	} TCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char TCSS:3;
			unsigned char TWRC:1;
			unsigned char :1;
			unsigned char TOMS:2;
		} BIT;
	} TMR;
	unsigned char  TPRE;
	unsigned char  TCNT;
} st_sci12_t;

typedef struct st_smci {
	union {
		unsigned char BYTE;
		struct {
			unsigned char GM:1;
			unsigned char BLK:1;
			unsigned char PE:1;
			unsigned char PM:1;
			unsigned char BCP:2;
			unsigned char CKS:2;
		} BIT;
	} SMR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TIE:1;
			unsigned char RIE:1;
			unsigned char TE:1;
			unsigned char RE:1;
			unsigned char MPIE:1;
			unsigned char TEIE:1;
			unsigned char CKE:2;
		} BIT;
	} SCR;
	char           wk1[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char TDRE:1;
			unsigned char RDRF:1;
			unsigned char ORER:1;
			unsigned char ERS:1;
			unsigned char PER:1;
			unsigned char TEND:1;
			unsigned char MPB:1;
			unsigned char MPBT:1;
		} BIT;
	} SSR;
	char           wk2[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char BCP2:1;
			unsigned char :2;
			unsigned char CHR1:1;
			unsigned char SDIR:1;
			unsigned char SINV:1;
			unsigned char :1;
			unsigned char SMIF:1;
		} BIT;
	} SCMR;
} st_smci_t;

typedef struct st_system {
	union {
		unsigned short WORD;
		struct {
			unsigned short :15;
			unsigned short MD:1;
		} BIT;
	} MDMONR;
	union {
		unsigned short WORD;
		struct {
			unsigned short :10;
			unsigned short UBTS:1;
			unsigned short :5;
		} BIT;
	} MDSR;
	char           wk0[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short KEY:8;
			unsigned short :6;
			unsigned short EXBE:1;
			unsigned short ROME:1;
		} BIT;
	} SYSCR0;
	union {
		unsigned short WORD;
		struct {
			unsigned short :15;
			unsigned short RAME:1;
		} BIT;
	} SYSCR1;
	char           wk1[2];
	union {
		unsigned short WORD;
		struct {
			unsigned short SSBY:1;
			unsigned short OPE:1;
			unsigned short :14;
		} BIT;
	} SBYCR;
	char           wk2[2];
	union {
		unsigned long LONG;
		struct {
			unsigned long ACSE:1;
			unsigned long :1;
			unsigned long MSTPA29:1;
			unsigned long MSTPA28:1;
			unsigned long MSTPA27:1;
			unsigned long :2;
			unsigned long MSTPA24:1;
			unsigned long :4;
			unsigned long MSTPA19:1;
			unsigned long :1;
			unsigned long MSTPA17:1;
			unsigned long :1;
			unsigned long MSTPA15:1;
			unsigned long MSTPA14:1;
			unsigned long :4;
			unsigned long MSTPA9:1;
			unsigned long :3;
			unsigned long MSTPA5:1;
			unsigned long MSTPA4:1;
			unsigned long :2;
			unsigned long MSTPA1:1;
			unsigned long MSTPA0:1;
		} BIT;
	} MSTPCRA;
	union {
		unsigned long LONG;
		struct {
			unsigned long MSTPB31:1;
			unsigned long MSTPB30:1;
			unsigned long MSTPB29:1;
			unsigned long MSTPB28:1;
			unsigned long MSTPB27:1;
			unsigned long MSTPB26:1;
			unsigned long MSTPB25:1;
			unsigned long MSTPB24:1;
			unsigned long MSTPB23:1;
			unsigned long :1;
			unsigned long MSTPB21:1;
			unsigned long :3;
			unsigned long MSTPB17:1;
			unsigned long :6;
			unsigned long MSTPB10:1;
			unsigned long MSTPB9:1;
			unsigned long :2;
			unsigned long MSTPB6:1;
			unsigned long :1;
			unsigned long MSTPB4:1;
			unsigned long :4;
		} BIT;
	} MSTPCRB;
	union {
		unsigned long LONG;
		struct {
			unsigned long :4;
			unsigned long MSTPC27:1;
			unsigned long MSTPC26:1;
			unsigned long MSTPC25:1;
			unsigned long MSTPC24:1;
			unsigned long :4;
			unsigned long MSTPC19:1;
			unsigned long :1;
			unsigned long MSTPC17:1;
			unsigned long :16;
			unsigned long MSTPC0:1;
		} BIT;
	} MSTPCRC;
	union {
		unsigned long LONG;
		struct {
			unsigned long :21;
			unsigned long MSTPD10:1;
			unsigned long :2;
			unsigned long MSTPD7:1;
			unsigned long :3;
			unsigned long MSTPD3:1;
			unsigned long MSTPD2:1;
			unsigned long :2;
		} BIT;
	} MSTPCRD;
	union {
		unsigned long LONG;
		struct {
			unsigned long FCK:4;
			unsigned long ICK:4;
			unsigned long PSTOP1:1;
			unsigned long :3;
			unsigned long BCK:4;
			unsigned long PCKA:4;
			unsigned long PCKB:4;
			unsigned long PCKC:4;
			unsigned long PCKD:4;
		} BIT;
	} SCKCR;
	union {
		unsigned short WORD;
		struct {
			unsigned short CFDCK:4;
			unsigned short :12;
		} BIT;
	} SCKCR2;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short CKSEL:3;
			unsigned short :8;
		} BIT;
	} SCKCR3;
	union {
		unsigned short WORD;
		struct {
			unsigned short :2;
			unsigned short STC:6;
			unsigned short :3;
			unsigned short PLLSRCSEL:1;
			unsigned short :2;
			unsigned short PLIDIV:2;
		} BIT;
	} PLLCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char PLLEN:1;
		} BIT;
	} PLLCR2;
	char           wk3[5];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char BCLKDIV:1;
		} BIT;
	} BCKCR;
	char           wk4[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char MOSTP:1;
		} BIT;
	} MOSCCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SOSTP:1;
		} BIT;
	} SOSCCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char LCSTP:1;
		} BIT;
	} LOCOCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char ILCSTP:1;
		} BIT;
	} ILOCOCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char HCSTP:1;
		} BIT;
	} HOCOCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char HCFRQ:2;
		} BIT;
	} HOCOCR2;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char FLLEN:1;
		} BIT;
	} FLLCR1;
	union {
		unsigned short WORD;
		struct {
			unsigned short :5;
			unsigned short MPY:11;
		} BIT;
	} FLLCR2;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char ILCOVF:1;
			unsigned char HCOVF:1;
			unsigned char PLOVF:1;
			unsigned char SOOVF:1;
			unsigned char MOOVF:1;
		} BIT;
	} OSCOVFSR;
	char           wk6[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char OSTDE:1;
			unsigned char :6;
			unsigned char OSTDIE:1;
		} BIT;
	} OSTDCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char OSTDF:1;
		} BIT;
	} OSTDSR;
	char           wk7[95];
	union {
		unsigned char BYTE;
		struct {
			unsigned char RSTCKEN:1;
			unsigned char :4;
			unsigned char RSTCKSEL:3;
		} BIT;
	} RSTCKCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char MSTS:8;
		} BIT;
	} MOSCWTCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char SSTS:8;
		} BIT;
	} SOSCWTCR;
	char           wk8[28];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char SWRF:1;
			unsigned char WDTRF:1;
			unsigned char IWDTRF:1;
		} BIT;
	} RSTSR2;
	char           wk9[1];
	unsigned short SWRR;
	char           wk10[28];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char LVD1IRQSEL:1;
			unsigned char LVD1IDTSEL:2;
		} BIT;
	} LVD1CR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char LVD1MON:1;
			unsigned char LVD1DET:1;
		} BIT;
	} LVD1SR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :5;
			unsigned char LVD2IRQSEL:1;
			unsigned char LVD2IDTSEL:2;
		} BIT;
	} LVD2CR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :6;
			unsigned char LVD2MON:1;
			unsigned char LVD2DET:1;
		} BIT;
	} LVD2SR;
	char           wk11[794];
	union {
		unsigned short WORD;
		struct {
			unsigned short PRKEY:8;
			unsigned short :4;
			unsigned short PRC3:1;
			unsigned short :1;
			unsigned short PRC1:1;
			unsigned short PRC0:1;
		} BIT;
	} PRCR;
	char           wk12[48768];
	union {
		unsigned char BYTE;
		struct {
			unsigned char DPSBY:1;
			unsigned char IOKEEP:1;
			unsigned char :4;
			unsigned char DEEPCUT:2;
		} BIT;
	} DPSBYCR;
	char           wk13[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char DIRQ7E:1;
			unsigned char DIRQ6E:1;
			unsigned char DIRQ5E:1;
			unsigned char DIRQ4E:1;
			unsigned char DIRQ3E:1;
			unsigned char DIRQ2E:1;
			unsigned char DIRQ1E:1;
			unsigned char DIRQ0E:1;
		} BIT;
	} DPSIER0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DIRQ15E:1;
			unsigned char DIRQ14E:1;
			unsigned char DIRQ13E:1;
			unsigned char DIRQ12E:1;
			unsigned char DIRQ11E:1;
			unsigned char DIRQ10E:1;
			unsigned char DIRQ9E:1;
			unsigned char DIRQ8E:1;
		} BIT;
	} DPSIER1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char DNMIE:1;
			unsigned char DRTCAIE:1;
			unsigned char DRTCIIE:1;
			unsigned char DLVD2IE:1;
			unsigned char DLVD1IE:1;
		} BIT;
	} DPSIER2;
	char           wk14[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char DIRQ7F:1;
			unsigned char DIRQ6F:1;
			unsigned char DIRQ5F:1;
			unsigned char DIRQ4F:1;
			unsigned char DIRQ3F:1;
			unsigned char DIRQ2F:1;
			unsigned char DIRQ1F:1;
			unsigned char DIRQ0F:1;
		} BIT;
	} DPSIFR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DIRQ15F:1;
			unsigned char DIRQ14F:1;
			unsigned char DIRQ13F:1;
			unsigned char DIRQ12F:1;
			unsigned char DIRQ11F:1;
			unsigned char DIRQ10F:1;
			unsigned char DIRQ9F:1;
			unsigned char DIRQ8F:1;
		} BIT;
	} DPSIFR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char DNMIF:1;
			unsigned char DRTCAIF:1;
			unsigned char DRTCIIF:1;
			unsigned char DLVD2IF:1;
			unsigned char DLVD1IF:1;
		} BIT;
	} DPSIFR2;
	char           wk15[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char DIRQ7EG:1;
			unsigned char DIRQ6EG:1;
			unsigned char DIRQ5EG:1;
			unsigned char DIRQ4EG:1;
			unsigned char DIRQ3EG:1;
			unsigned char DIRQ2EG:1;
			unsigned char DIRQ1EG:1;
			unsigned char DIRQ0EG:1;
		} BIT;
	} DPSIEGR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char DIRQ15EG:1;
			unsigned char DIRQ14EG:1;
			unsigned char DIRQ13EG:1;
			unsigned char DIRQ12EG:1;
			unsigned char DIRQ11EG:1;
			unsigned char DIRQ10EG:1;
			unsigned char DIRQ9EG:1;
			unsigned char DIRQ8EG:1;
		} BIT;
	} DPSIEGR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char DNMIEG:1;
			unsigned char :2;
			unsigned char DLVD2EG:1;
			unsigned char DLVD1EG:1;
		} BIT;
	} DPSIEGR2;
	char           wk16[3];
	union {
		unsigned char BYTE;
		struct {
			unsigned char DPSRSTF:1;
			unsigned char :3;
			unsigned char LVD2RF:1;
			unsigned char LVD1RF:1;
			unsigned char LVD0RF:1;
			unsigned char PORF:1;
		} BIT;
	} RSTSR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char CWSF:1;
		} BIT;
	} RSTSR1;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char SOFE:1;
		} BIT;
	} SOFCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char MOSEL:1;
			unsigned char MODRV2:2;
			unsigned char :4;
		} BIT;
	} MOFCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char HOCOPCNT:1;
		} BIT;
	} HOCOPCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RICVLS:1;
			unsigned char :7;
		} BIT;
	} VOLSR;
	char           wk17[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :1;
			unsigned char LVD2E:1;
			unsigned char LVD1E:1;
			unsigned char :5;
		} BIT;
	} LVCMPCR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char LVD2LVL:4;
			unsigned char LVD1LVL:4;
		} BIT;
	} LVDLVLR;
	char           wk18[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char LVD1RN:1;
			unsigned char LVD1RI:1;
			unsigned char LVD1FSAMP:2;
			unsigned char :1;
			unsigned char LVD1CMPE:1;
			unsigned char LVD1DFDIS:1;
			unsigned char LVD1RIE:1;
		} BIT;
	} LVD1CR0;
	union {
		unsigned char BYTE;
		struct {
			unsigned char LVD2RN:1;
			unsigned char LVD2RI:1;
			unsigned char LVD2FSAMP:2;
			unsigned char :1;
			unsigned char LVD2CMPE:1;
			unsigned char LVD2DFDIS:1;
			unsigned char LVD2RIE:1;
		} BIT;
	} LVD2CR0;
	char           wk19[4];
	unsigned char  DPSBKR[32];
} st_system_t;

typedef struct st_temps {
	unsigned long  TSCDR;
} st_temps_t;

typedef struct st_tmr0 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CMIEB:1;
			unsigned char CMIEA:1;
			unsigned char OVIE:1;
			unsigned char CCLR:2;
			unsigned char :3;
		} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :3;
			unsigned char ADTE:1;
			unsigned char OSB:2;
			unsigned char OSA:2;
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
			unsigned char TMRIS:1;
			unsigned char :2;
			unsigned char CSS:2;
			unsigned char CKS:3;
		} BIT;
	} TCCR;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TCS:1;
		} BIT;
	} TCSTR;
} st_tmr0_t;

typedef struct st_tmr1 {
	union {
		unsigned char BYTE;
		struct {
			unsigned char CMIEB:1;
			unsigned char CMIEA:1;
			unsigned char OVIE:1;
			unsigned char CCLR:2;
			unsigned char :3;
		} BIT;
	} TCR;
	char           wk0[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :4;
			unsigned char OSB:2;
			unsigned char OSA:2;
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
			unsigned char TMRIS:1;
			unsigned char :2;
			unsigned char CSS:2;
			unsigned char CKS:3;
		} BIT;
	} TCCR;
	char           wk5[1];
	union {
		unsigned char BYTE;
		struct {
			unsigned char :7;
			unsigned char TCS:1;
		} BIT;
	} TCSTR;
} st_tmr1_t;

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
			unsigned short :2;
			unsigned short RPSS:2;
			unsigned short :2;
			unsigned short RPES:2;
			unsigned short CKS:4;
			unsigned short :2;
			unsigned short TOPS:2;
		} BIT;
	} WDTCR;
	union {
		unsigned short WORD;
		struct {
			unsigned short REFEF:1;
			unsigned short UNDFF:1;
			unsigned short CNTVAL:14;
		} BIT;
	} WDTSR;
	union {
		unsigned char BYTE;
		struct {
			unsigned char RSTIRQS:1;
			unsigned char :7;
		} BIT;
	} WDTRCR;
} st_wdt_t;

#pragma bit_order
#pragma packoption

#endif
