/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_dtc.h
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file contains equates and declarations specific to the Data Transfer Controller (DTCa) as used
*                by the QECTSU driver.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY  Version  Description
*              : 08.06.2018  1.00     First Release
***********************************************************************************************************************/
#ifndef __R_DTC_H__
#define __R_DTC_H__

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/*****    (inaccessible directly from the CPU) DTC Mode Register A    *************************************************/
#define _00_SM_FIXED                     (0)        /* Address in the SAR register is fixed                           */
#define _01_SM_FIXEDb                    (1)        /* Address in the SAR register is fixed                           */
#define _10_SM_INC                       (2)        /* SAR value is incremented after data transfer                   */
#define _11_SM_DEC                       (3)        /* SAR value is decremented after data transfer                   */

#define _00_SZ_BYTE                      (0)        /* Byte (8-bit) transfer                                          */
#define _01_SZ_WORD                      (1)        /* Word (16-bit) transfer                                         */
#define _10_SZ_LONG                      (2)        /* Longword (32-bit) transfer                                     */
#define _11_SZ_PROHIBITION               (3)        /* Setting prohibited                                             */

#define _00_MD_NORMAL                    (0)        /* Normal transfer mode                                           */
#define _01_MD_REPEAT                    (1)        /* Repeat transfer mode                                           */
#define _10_MD_BLOCK                     (2)        /* Block transfer mode                                            */
#define _11_MD_PROHIBITION               (3)        /* Setting prohibited                                             */

/*****    (inaccessible directly from the CPU) DTC Mode Register B    *************************************************/
#define _00_DM_FIXED                     (0)        /* Address in the DAR register is fixed                           */
#define _01_DM_FIXEDb                    (1)        /* Address in the DAR register is fixed                           */
#define _10_DM_INC                       (2)        /* DAR value is incremented after data transfer                   */
#define _11_DM_DEC                       (3)        /* DAR value is decremented after data transfer                   */

#define _0_DTS_DESTINATION               (0)        /* Transfer destination side is repeat area or block area         */
#define _1_DTS_SOURCE                    (1)        /* Transfer source side is repeat area or block area              */

#define _0_DISEL_COMPLETED               (0)        /* An interrupt request to the CPU is generated when specified data transfer is completed    */
#define _1_DISEL_EACH                    (1)        /* An interrupt request to the CPU is generated each time DTC data transfer is performed     */

#define _0_CHNS_CONTINUE                 (0)        /* Chain transfer is performed continuously                       */
#define _1_CHNS_CONTER                   (1)        /* Chain transfer is performed only when the transfer counter is changed from 1 to 0 or 1 to CRAH    */

#define _0_CHNE_DISABLE                  (0)        /* Chain transfer is disabled                                     */
#define _1_CHNE_ENABLE                   (1)        /* Chain transfer is enabled                                      */

/*****    0008 2400h DTC Control Register        **********************************************************************/
#define _0_RRS_NOTSKIP                   (0)        /* Transfer information read is not skipped                       */
#define _1_RRS_SKIP                      (1)        /* Transfer information read is skipped when vector numbers match */

/*****    0008 2408h DTC Address Mode Register    *********************************************************************/
#define _0_SHORT_FULL                    (0)        /* Full-address mode                                              */
#define _1_SHORT_SHORT                   (1)        /* Short-address mode                                             */

/*****    0008 240Ch DTC Module Start Register    *********************************************************************/
#define _0_DTCST_STOP                    (0)        /* DTC module stop                                                */
#define _1_DTCST_START                   (1)        /* DTC module start                                               */

/* DTC for CTSU */
#define CTSU_DTC_WR_CNT                  (0x0303)   /* DTC Write count / 3word                                        */
#define CTSU_DTC_RD_CNT                  (0x0202)   /* DTC Read count / 2word                                         */


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#pragma bit_order left
#pragma unpack

/* DTC Vector table */ 
typedef struct
{
    uint16_t word;
    union
    {
        uint8_t byte;
        struct
        {
            uint8_t    CHNE     :1;
            uint8_t    CHNS     :1;
            uint8_t    DISEL    :1;
            uint8_t    DTS      :1;
            uint8_t    DM       :2;
            uint8_t             :2;
        }
        bit;
    }
    MRB;                        /* Mode Register B */
    union
    {
        uint8_t byte;
        struct
        {
            uint8_t    MD    :2;
            uint8_t    SZ    :2;
            uint8_t    SM    :2;
            uint8_t          :2;
        }
        bit;
    }
    MRA;                        /* Mode Register A */
    uint32_t    SAR;            /* Source Address */
    uint32_t    DAR;            /* Destination Address */
    uint16_t    CRB;            /* Count Register A */
    uint16_t    CRA;            /* Count Register B */
} dtc_register_data_t;

#pragma bit_order
#pragma packoption


/***********************************************************************************************************************
* Global variables
***********************************************************************************************************************/


/***********************************************************************************************************************
* Global functions
***********************************************************************************************************************/
void dtc_ctsu_init(void);
void dtc_ctsu_xfer_addrs(uint32_t wr_int_src_addr, uint32_t rd_int_dst_addr);

#endif    // __R_DTC_H__
