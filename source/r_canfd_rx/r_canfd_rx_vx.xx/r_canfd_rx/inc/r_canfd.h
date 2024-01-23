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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_canfd.h
* Description  : Internal structures and definitions for the CANFD driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.11.2021 1.00    Initial Release
*           13.12.2023 1.31    Added WAIT_LOOP comments.
***********************************************************************************************************************/
#ifndef R_CANFD_H
#define R_CANFD_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_canfd_rx_config.h"
#include "r_can_api.h"
#include "platform.h"
/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup CANFD
 * @{
 **********************************************************************************************************************/

/* =========================================================================================================================== */
/* ================                                          R_CANFD                                          ================ */
/* =========================================================================================================================== */

/* ========================================================  GCFG  ======================================================== */
 #define R_CANFD_GCFG_TPRI_Pos                (0UL)          /*!< TPRI (Bit 0)                                          */
 #define R_CANFD_GCFG_TPRI_Msk                (0x1UL)        /*!< TPRI (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_GCFG_DCE_Pos                 (1UL)          /*!< DCE (Bit 1)                                           */
 #define R_CANFD_GCFG_DCE_Msk                 (0x2UL)        /*!< DCE (Bitfield-Mask: 0x01)                             */
 #define R_CANFD_GCFG_DRE_Pos                 (2UL)          /*!< DRE (Bit 2)                                           */
 #define R_CANFD_GCFG_DRE_Msk                 (0x4UL)        /*!< DRE (Bitfield-Mask: 0x01)                             */
 #define R_CANFD_GCFG_MME_Pos                 (3UL)          /*!< MME (Bit 3)                                           */
 #define R_CANFD_GCFG_MME_Msk                 (0x8UL)        /*!< MME (Bitfield-Mask: 0x01)                             */
 #define R_CANFD_GCFG_DLLCS_Pos               (4UL)          /*!< DLLCS (Bit 4)                                           */
 #define R_CANFD_GCFG_DLLCS_Msk               (0x10UL)       /*!< DLLCS (Bitfield-Mask: 0x01)                             */
 #define R_CANFD_GCFG_OMRC_Pos                (5UL)          /*!< OMRC (Bit 5)                                         */
 #define R_CANFD_GCFG_OMRC_Msk                (0x20UL)       /*!< OMRC (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_GCFG_TSP_Pos                 (8UL)          /*!< TSP (Bit 8)                                           */
 #define R_CANFD_GCFG_TSP_Msk                 (0xf00UL)      /*!< TSP (Bitfield-Mask: 0x0f)                             */
 #define R_CANFD_GCFG_TSCS_Pos                (12UL)         /*!< TSCS (Bit 12)                                         */
 #define R_CANFD_GCFG_TSCS_Msk                (0x1000UL)     /*!< TSCS (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_GCFG_ITP_Pos                 (16UL)         /*!< ITP (Bit 16)                                        */
 #define R_CANFD_GCFG_ITP_Msk                 (0xffff0000UL) /*!< ITP (Bitfield-Mask: 0xffff)                         */
/* ========================================================  GCR  ======================================================== */
 #define R_CANFD_GCR_GMDC_Pos                (0UL)          /*!< GMDC (Bit 0)                                          */
 #define R_CANFD_GCR_GMDC_Msk                (0x3UL)        /*!< GMDC (Bitfield-Mask: 0x03)                            */
 #define R_CANFD_GCR_SLPRQ_Pos               (2UL)          /*!< SLPRQ (Bit 2)                                         */
 #define R_CANFD_GCR_SLPRQ_Msk               (0x4UL)        /*!< SLPRQ (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_GCR_DEIE_Pos                (8UL)          /*!< DEIE (Bit 8)                                          */
 #define R_CANFD_GCR_DEIE_Msk                (0x100UL)      /*!< DEIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_GCR_MLIE_Pos                (9UL)          /*!< MLIE (Bit 9)                                          */
 #define R_CANFD_GCR_MLIE_Msk                (0x200UL)      /*!< MLIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_GCR_THLIE_Pos               (10UL)         /*!< THLIE (Bit 10)                                       */
 #define R_CANFD_GCR_THLIE_Msk               (0x400UL)      /*!< THLIE (Bitfield-Mask: 0x01)                          */
 #define R_CANFD_GCR_POIE_Pos                (11UL)         /*!< POIE (Bit 11)                                      */
 #define R_CANFD_GCR_POIE_Msk                (0x800UL)      /*!< POIE (Bitfield-Mask: 0x01)                         */
 #define R_CANFD_GCR_TSCR_Pos                (16UL)         /*!< TSCR (Bit 16)                                        */
 #define R_CANFD_GCR_TSCR_Msk                (0x10000UL)    /*!< TSCR (Bitfield-Mask: 0x01)                           */
/* ========================================================  GSR  ======================================================== */
#define R_CANFD_GSR_RSTST_Pos             (0UL)          /*!< RSTST (Bit 0)                                       */
#define R_CANFD_GSR_RSTST_Msk             (0x1UL)        /*!< RSTST (Bitfield-Mask: 0x01)                         */
#define R_CANFD_GSR_HLTST_Pos             (1UL)          /*!< HLTST (Bit 1)                                       */
#define R_CANFD_GSR_HLTST_Msk             (0x2UL)        /*!< HLTST (Bitfield-Mask: 0x01)                         */
#define R_CANFD_GSR_SLPST_Pos             (2UL)          /*!< SLPST (Bit 2)                                       */
#define R_CANFD_GSR_SLPST_Msk             (0x4UL)        /*!< SLPST (Bitfield-Mask: 0x01)                         */
#define R_CANFD_GSR_RAMST_Pos             (3UL)          /*!< RAMST (Bit 3)                                      */
#define R_CANFD_GSR_RAMST_Msk             (0x8UL)        /*!< RAMST (Bitfield-Mask: 0x01)                        */
/* ======================================================  AFCR  ====================================================== */
#define R_CANFD_AFCR_PAGE_Pos           (0UL)          /*!< AFLPN (Bit 0)                                         */
#define R_CANFD_AFCR_PAGE_Msk           (0xfUL)        /*!< AFLPN (Bitfield-Mask: 0x0f)                           */
#define R_CANFD_AFCR_AFLWE_Pos          (8UL)          /*!< AFLWE (Bit 8)                                        */
#define R_CANFD_AFCR_AFLWE_Msk          (0x100UL)      /*!< AFLWE (Bitfield-Mask: 0x01)                          */
/* ======================================================  AFCFG  ====================================================== */
#define R_CANFD_AFCFG_RNC0_Pos            (16UL)         /*!< RNC0 (Bit 16)                                         */
#define R_CANFD_AFCFG_RNC0_Msk            (0x1ff0000UL)  /*!< RNC0 (Bitfield-Mask: 0x1ff)                           */
/* ========================================================  RMCR  ======================================================== */
 #define R_CANFD_RMCR_NMB_Pos               (0UL)          /*!< NMB (Bit 0)                                         */
 #define R_CANFD_RMCR_NMB_Msk               (0xffUL)       /*!< NMB (Bitfield-Mask: 0xff)                           */
 #define R_CANFD_RMCR_PLS_Pos               (8UL)          /*!< PLS (Bit 8)                                         */
 #define R_CANFD_RMCR_PLS_Msk               (0x700UL)      /*!< PLS (Bitfield-Mask: 0x07)                           */
/* ========================================================  RFCR  ======================================================== */
 #define R_CANFD_RFCR_RFE_Pos                 (0UL)          /*!< RFE (Bit 0)                                           */
 #define R_CANFD_RFCR_RFE_Msk                 (0x1UL)        /*!< RFE (Bitfield-Mask: 0x01)                             */
 #define R_CANFD_RFCR_RFIE_Pos                (1UL)          /*!< RFIE (Bit 1)                                          */
 #define R_CANFD_RFCR_RFIE_Msk                (0x2UL)        /*!< RFIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_RFCR_PLS_Pos               (4UL)          /*!< PLS (Bit 4)                                         */
 #define R_CANFD_RFCR_PLS_Msk               (0x70UL)       /*!< PLS (Bitfield-Mask: 0x07)                           */
 #define R_CANFD_RFCR_FDS_Pos                (8UL)          /*!< FDS (Bit 8)                                          */
 #define R_CANFD_RFCR_FDS_Msk                (0x700UL)      /*!< FDS (Bitfield-Mask: 0x07)                            */
 #define R_CANFD_RFCR_RFIM_Pos                (12UL)         /*!< RFIM (Bit 12)                                         */
 #define R_CANFD_RFCR_RFIM_Msk                (0x1000UL)     /*!< RFIM (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_RFCR_RFITH_Pos              (13UL)         /*!< RFITH (Bit 13)                                       */
 #define R_CANFD_RFCR_RFITH_Msk              (0xe000UL)     /*!< RFITH (Bitfield-Mask: 0x07)                          */
/* =======================================================  RFSR  ======================================================== */
#define R_CANFD_RFSR_EMPTY_Pos              (0UL)          /*!< EMPTY (Bit 0)                                         */
#define R_CANFD_RFSR_EMPTY_Msk              (0x1UL)        /*!< EMPTY (Bitfield-Mask: 0x01)                           */
#define R_CANFD_RFSR_FULL_Pos              (1UL)          /*!< FULL (Bit 1)                                         */
#define R_CANFD_RFSR_FULL_Msk              (0x2UL)        /*!< FULL (Bitfield-Mask: 0x01)                           */
#define R_CANFD_RFSR_LOST_Pos              (2UL)          /*!< LOST (Bit 2)                                         */
#define R_CANFD_RFSR_LOST_Msk              (0x4UL)        /*!< LOST (Bitfield-Mask: 0x01)                           */
#define R_CANFD_RFSR_RFIF_Pos               (3UL)          /*!< RFIF (Bit 3)                                          */
#define R_CANFD_RFSR_RFIF_Msk               (0x8UL)        /*!< RFIF (Bitfield-Mask: 0x01)                            */
#define R_CANFD_RFSR_FLVL_Pos               (8UL)          /*!< FLVL (Bit 8)                                          */
#define R_CANFD_RFSR_FLVL_Msk               (0xff00UL)     /*!< FLVL (Bitfield-Mask: 0xff)                            */
/* =====================================================  TISR  ====================================================== */
#define R_CANFD_TISR_TSIF0_Pos          (0UL)          /*!< TSIF0 (Bit 0)                                         */
#define R_CANFD_TISR_TSIF0_Msk          (0x1UL)        /*!< TSIF0 (Bitfield-Mask: 0x01)                           */
#define R_CANFD_TISR_TAIF0_Pos          (1UL)          /*!< TAIF0 (Bit 1)                                         */
#define R_CANFD_TISR_TAIF0_Msk          (0x2UL)        /*!< TAIF0 (Bitfield-Mask: 0x01)                           */
#define R_CANFD_TISR_TQIF0_Pos          (2UL)          /*!< TQIF0 (Bit 2)                                         */
#define R_CANFD_TISR_TQIF0_Msk          (0x4UL)        /*!< TQIF0 (Bitfield-Mask: 0x01)                           */
#define R_CANFD_TISR_CFTIF0_Pos         (3UL)          /*!< CFTIF0 (Bit 3)                                        */
#define R_CANFD_TISR_CFTIF0_Msk         (0x8UL)        /*!< CFTIF0 (Bitfield-Mask: 0x01)                          */
#define R_CANFD_TISR_THIF0_Pos          (4UL)          /*!< THIF0 (Bit 4)                                         */
#define R_CANFD_TISR_THIF0_Msk          (0x10UL)       /*!< THIF0 (Bitfield-Mask: 0x01)                           */
/* =======================================================  FESR  ======================================================== */
#define R_CANFD_FESR_RFEMP0_Pos             (0UL)          /*!< RFEMP0 (Bit 0)                                        */
#define R_CANFD_FESR_RFEMP0_Msk             (0x1UL)        /*!< RFEMP0 (Bitfield-Mask: 0x01)                          */
#define R_CANFD_FESR_RFEMP1_Pos             (1UL)          /*!< RFEMP1 (Bit 1)                                        */
#define R_CANFD_FESR_RFEMP1_Msk             (0x2UL)        /*!< RFEMP1 (Bitfield-Mask: 0x01)                          */
#define R_CANFD_FESR_RFEMP_Msk             (0x3UL)        /*!< RFXEMP (Bitfield-Mask: 0x03)                          */
#define R_CANFD_FESR_CFEMP0_Pos             (8UL)          /*!< CFEMP0 (Bit 8)                                        */
#define R_CANFD_FESR_CFEMP0_Msk             (0x100UL)      /*!< CFEMP0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  NBCR  ========================================================== */
#define R_CANFD_NBCR_BRP_Pos      (0UL)          /*!< BRP (Bit 0)                                          */
#define R_CANFD_NBCR_BRP_Msk      (0x3ffUL)      /*!< BRP (Bitfield-Mask: 0x3ff)                           */
#define R_CANFD_NBCR_SJW_Pos      (10UL)         /*!< SJW (Bit 10)                                         */
#define R_CANFD_NBCR_SJW_Msk      (0x1fc00UL)    /*!< SJW (Bitfield-Mask: 0x7f)                            */
#define R_CANFD_NBCR_TSEG1_Pos    (17UL)         /*!< TSEG1 (Bit 17)                                       */
#define R_CANFD_NBCR_TSEG1_Msk    (0x1fe0000UL)  /*!< TSEG1 (Bitfield-Mask: 0xff)                          */
#define R_CANFD_NBCR_TSEG2_Pos    (25UL)         /*!< TSEG2 (Bit 25)                                       */
#define R_CANFD_NBCR_TSEG2_Msk    (0xfe000000UL) /*!< TSEG2 (Bitfield-Mask: 0x7f)                          */
/* =========================================================================================================================== */
/* ================                                           CFDC2                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DBCR  ========================================================== */
#define R_CANFD_DBCR_BRP_Pos       (0UL)          /*!< BRP (Bit 0)                                          */
#define R_CANFD_DBCR_BRP_Msk       (0xffUL)       /*!< BRP (Bitfield-Mask: 0xff)                            */
#define R_CANFD_DBCR_TSEG1_Pos     (8UL)          /*!< TSEG1 (Bit 8)                                        */
#define R_CANFD_DBCR_TSEG1_Msk     (0x1f00UL)     /*!< TSEG1 (Bitfield-Mask: 0x1f)                          */
#define R_CANFD_DBCR_TSEG2_Pos     (16UL)         /*!< TSEG2 (Bit 16)                                       */
#define R_CANFD_DBCR_TSEG2_Msk     (0xf0000UL)    /*!< TSEG2 (Bitfield-Mask: 0x0f)                          */
#define R_CANFD_DBCR_SJW_Pos       (24UL)         /*!< SJW (Bit 24)                                         */
#define R_CANFD_DBCR_SJW_Msk       (0xf000000UL)  /*!< SJW (Bitfield-Mask: 0x0f)                            */

/* =========================================================  FDCFG  ========================================================= */
#define R_CANFD_FDCFG_ECC_Pos    (0UL)          /*!< ECC (Bit 0)                                        */
#define R_CANFD_FDCFG_ECC_Msk    (0x7UL)        /*!< ECC (Bitfield-Mask: 0x07)                          */
#define R_CANFD_FDCFG_SSPC_Pos     (8UL)          /*!< SSPC (Bit 8)                                         */
#define R_CANFD_FDCFG_SSPC_Msk     (0x100UL)      /*!< SSPC (Bitfield-Mask: 0x01)                           */
#define R_CANFD_FDCFG_TDCE_Pos      (9UL)          /*!< TDCE (Bit 9)                                          */
#define R_CANFD_FDCFG_TDCE_Msk      (0x200UL)      /*!< TDCE (Bitfield-Mask: 0x01)                            */
#define R_CANFD_FDCFG_TESI_Pos      (10UL)         /*!< TESI (Bit 10)                                         */
#define R_CANFD_FDCFG_TESI_Msk      (0x400UL)      /*!< TESI (Bitfield-Mask: 0x01)                            */
#define R_CANFD_FDCFG_TDCO_Pos      (16UL)         /*!< TDCO (Bit 16)                                         */
#define R_CANFD_FDCFG_TDCO_Msk      (0xff0000UL)   /*!< TDCO (Bitfield-Mask: 0xff)                            */
#define R_CANFD_FDCFG_FDOE_Pos      (28UL)         /*!< FDOE (Bit 28)                                         */
#define R_CANFD_FDCFG_FDOE_Msk      (0x10000000UL) /*!< FDOE (Bitfield-Mask: 0x01)                            */
#define R_CANFD_FDCFG_REFE_Pos      (29UL)         /*!< REFE (Bit 29)                                         */
#define R_CANFD_FDCFG_REFE_Msk      (0x20000000UL) /*!< REFE (Bitfield-Mask: 0x01)                            */
#define R_CANFD_FDCFG_CLOE_Pos      (30UL)         /*!< CLOE (Bit 30)                                         */
#define R_CANFD_FDCFG_CLOE_Msk      (0x40000000UL) /*!< CLOE (Bitfield-Mask: 0x01)                            */
/* ==========================================================  CHCR  ========================================================== */
 #define R_CANFD_CHCR_MDC_Pos        (0UL)          /*!< MDC (Bit 0)                                         */
 #define R_CANFD_CHCR_MDC_Msk        (0x3UL)        /*!< MDC (Bitfield-Mask: 0x03)                           */
 #define R_CANFD_CHCR_SLPRQ_Pos      (2UL)          /*!< SLPRQ (Bit 2)                                         */
 #define R_CANFD_CHCR_SLPRQ_Msk      (0x4UL)        /*!< SLPRQ (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_CHCR_RTBO_Pos       (3UL)          /*!< RTBO (Bit 3)                                          */
 #define R_CANFD_CHCR_RTBO_Msk       (0x8UL)        /*!< RTBO (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_BEIE_Pos       (8UL)          /*!< BEIE (Bit 8)                                          */
 #define R_CANFD_CHCR_BEIE_Msk       (0x100UL)      /*!< BEIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_EWIE_Pos       (9UL)          /*!< EWIE (Bit 9)                                          */
 #define R_CANFD_CHCR_EWIE_Msk       (0x200UL)      /*!< EWIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_EPIE_Pos       (10UL)         /*!< EPIE (Bit 10)                                         */
 #define R_CANFD_CHCR_EPIE_Msk       (0x400UL)      /*!< EPIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_BOEIE_Pos      (11UL)         /*!< BOEIE (Bit 11)                                        */
 #define R_CANFD_CHCR_BOEIE_Msk      (0x800UL)      /*!< BOEIE (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_CHCR_BORIE_Pos      (12UL)         /*!< BORIE (Bit 12)                                        */
 #define R_CANFD_CHCR_BORIE_Msk      (0x1000UL)     /*!< BORIE (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_CHCR_OLIE_Pos       (13UL)         /*!< OLIE (Bit 13)                                         */
 #define R_CANFD_CHCR_OLIE_Msk       (0x2000UL)     /*!< OLIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_BLIE_Pos       (14UL)         /*!< BLIE (Bit 14)                                         */
 #define R_CANFD_CHCR_BLIE_Msk       (0x4000UL)     /*!< BLIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_ALIE_Pos       (15UL)         /*!< ALIE (Bit 15)                                         */
 #define R_CANFD_CHCR_ALIE_Msk       (0x8000UL)     /*!< ALIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_TAIE_Pos       (16UL)         /*!< TAIE (Bit 16)                                         */
 #define R_CANFD_CHCR_TAIE_Msk       (0x10000UL)    /*!< TAIE (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_ECOVIE_Pos     (17UL)         /*!< ECOVIE (Bit 17)                                       */
 #define R_CANFD_CHCR_ECOVIE_Msk     (0x20000UL)    /*!< ECOVIE (Bitfield-Mask: 0x01)                          */
 #define R_CANFD_CHCR_SCOVIE_Pos     (18UL)         /*!< SCOVIE (Bit 18)                                       */
 #define R_CANFD_CHCR_SCOVIE_Msk     (0x40000UL)    /*!< SCOVIE (Bitfield-Mask: 0x01)                          */
 #define R_CANFD_CHCR_TDCVIE_Pos     (19UL)         /*!< TDCVIE (Bit 19)                                      */
 #define R_CANFD_CHCR_TDCVIE_Msk     (0x80000UL)    /*!< TDCVIE (Bitfield-Mask: 0x01)                         */
 #define R_CANFD_CHCR_BOM_Pos        (21UL)         /*!< BOM (Bit 21)                                          */
 #define R_CANFD_CHCR_BOM_Msk        (0x600000UL)   /*!< BOM (Bitfield-Mask: 0x03)                             */
 #define R_CANFD_CHCR_EDM_Pos        (23UL)         /*!< ERRD (Bit 23)                                         */
 #define R_CANFD_CHCR_EDM_Msk        (0x800000UL)   /*!< ERRD (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_CTME_Pos       (24UL)         /*!< CTME (Bit 24)                                         */
 #define R_CANFD_CHCR_CTME_Msk       (0x1000000UL)  /*!< CTME (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_CTMS_Pos       (25UL)         /*!< CTMS (Bit 25)                                         */
 #define R_CANFD_CHCR_CTMS_Msk       (0x6000000UL)  /*!< CTMS (Bitfield-Mask: 0x03)                            */
 #define R_CANFD_CHCR_BFT_Pos        (30UL)         /*!< BFT (Bit 30)                                         */
 #define R_CANFD_CHCR_BFT_Msk        (0x40000000UL) /*!< BFT (Bitfield-Mask: 0x01)                            */
 #define R_CANFD_CHCR_ROME_Pos       (31UL)         /*!< ROME (Bit 31)                                          */
 #define R_CANFD_CHCR_ROME_Msk       (0x80000000UL) /*!< ROME (Bitfield-Mask: 0x01)                             */

/* =========================================================================================================================== */
/* ================                                           TMB                                           ================ */
/* =========================================================================================================================== */

/* ==========================================================  HF0  =========================================================== */
#define R_CANFD_TMB_HF0_ID_Pos          (0UL)          /*!< ID (Bit 0)                                          */
#define R_CANFD_TMB_HF0_ID_Msk          (0x1fffffffUL) /*!< ID (Bitfield-Mask: 0x1fffffff)                      */
#define R_CANFD_TMB_HF0_RTR_Pos         (30UL)         /*!< RTR (Bit 30)                                        */
#define R_CANFD_TMB_HF0_RTR_Msk         (0x40000000UL) /*!< RTR (Bitfield-Mask: 0x01)                           */
#define R_CANFD_TMB_HF0_IDE_Pos         (31UL)         /*!< IDE (Bit 31)                                        */
#define R_CANFD_TMB_HF0_IDE_Msk         (0x80000000UL) /*!< IDE (Bitfield-Mask: 0x01)                           */

/* ==========================================================  HF1  ========================================================== */
#define R_CANFD_TMB_HF1_DLC_Pos      (28UL)         /*!< DLC (Bit 28)                                        */
#define R_CANFD_TMB_HF1_DLC_Msk      (0xf0000000UL) /*!< DLC (Bitfield-Mask: 0x0f)                           */
/* =========================================================  FDCTR  ========================================================= */
 #define R_CANFD_CFDTM_FDCTR_TMESI_Pos    (0UL)          /*!< TMESI (Bit 0)                                         */
 #define R_CANFD_CFDTM_FDCTR_TMESI_Msk    (0x1UL)        /*!< TMESI (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_CFDTM_FDCTR_TMBRS_Pos    (1UL)          /*!< TMBRS (Bit 1)                                         */
 #define R_CANFD_CFDTM_FDCTR_TMBRS_Msk    (0x2UL)        /*!< TMBRS (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_CFDTM_FDCTR_TMFDF_Pos    (2UL)          /*!< TMFDF (Bit 2)                                         */
 #define R_CANFD_CFDTM_FDCTR_TMFDF_Msk    (0x4UL)        /*!< TMFDF (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_CFDTM_FDCTR_TMIFL_Pos    (8UL)          /*!< TMIFL (Bit 8)                                         */
 #define R_CANFD_CFDTM_FDCTR_TMIFL_Msk    (0x300UL)      /*!< TMIFL (Bitfield-Mask: 0x03)                           */
 #define R_CANFD_CFDTM_FDCTR_TMPTR_Pos    (16UL)         /*!< TMPTR (Bit 16)                                        */
 #define R_CANFD_CFDTM_FDCTR_TMPTR_Msk    (0xffff0000UL) /*!< TMPTR (Bitfield-Mask: 0xffff)                         */
/* ==========================================================  DF  =========================================================== */
 #define R_CANFD_CFDTM_DF_TMDB_Pos        (0UL)          /*!< TMDB (Bit 0)                                          */
 #define R_CANFD_CFDTM_DF_TMDB_Msk        (0xffUL)       /*!< TMDB (Bitfield-Mask: 0xff)                            */
/* ==========================================================  CHSR  ========================================================== */
#define R_CANFD_CHSR_RSTST_Pos    (0UL)          /*!< RSTST (Bit 0)                                       */
#define R_CANFD_CHSR_RSTST_Msk    (0x1UL)        /*!< RSTST (Bitfield-Mask: 0x01)                         */
#define R_CANFD_CHSR_HLTST_Pos    (1UL)          /*!< HLTST (Bit 1)                                       */
#define R_CANFD_CHSR_HLTST_Msk    (0x2UL)        /*!< HLTST (Bitfield-Mask: 0x01)                         */
#define R_CANFD_CHSR_SLPST_Pos    (2UL)          /*!< SLPST (Bit 2)                                       */
#define R_CANFD_CHSR_SLPST_Msk    (0x4UL)        /*!< SLPST (Bitfield-Mask: 0x01)                         */
#define R_CANFD_CHSR_EPST_Pos      (3UL)          /*!< EPST (Bit 3)                                         */
#define R_CANFD_CHSR_EPST_Msk      (0x8UL)        /*!< EPST (Bitfield-Mask: 0x01)                           */
#define R_CANFD_CHSR_BOST_Pos      (4UL)          /*!< BOST (Bit 4)                                         */
#define R_CANFD_CHSR_BOST_Msk      (0x10UL)       /*!< BOST (Bitfield-Mask: 0x01)                           */
#define R_CANFD_CHSR_TRMST_Pos     (5UL)          /*!< TRMST (Bit 5)                                        */
#define R_CANFD_CHSR_TRMST_Msk     (0x20UL)       /*!< TRMST (Bitfield-Mask: 0x01)                          */
#define R_CANFD_CHSR_RECST_Pos     (6UL)          /*!< RECST (Bit 6)                                        */
#define R_CANFD_CHSR_RECST_Msk     (0x40UL)       /*!< RECST (Bitfield-Mask: 0x01)                          */
#define R_CANFD_CHSR_CRDY_Pos     (7UL)          /*!< CRDY (Bit 7)                                        */
#define R_CANFD_CHSR_CRDY_Msk     (0x80UL)       /*!< CRDY (Bitfield-Mask: 0x01)                          */
#define R_CANFD_CHSR_RESI_Pos       (8UL)          /*!< RESI (Bit 8)                                          */
#define R_CANFD_CHSR_RESI_Msk       (0x100UL)      /*!< RESI (Bitfield-Mask: 0x01)                            */
#define R_CANFD_CHSR_REC_Pos        (16UL)         /*!< REC (Bit 16)                                          */
#define R_CANFD_CHSR_REC_Msk        (0xff0000UL)   /*!< REC (Bitfield-Mask: 0xff)                             */
#define R_CANFD_CHSR_TEC_Pos        (24UL)         /*!< TEC (Bit 24)                                          */
#define R_CANFD_CHSR_TEC_Msk        (0xff000000UL) /*!< TEC (Bitfield-Mask: 0xff)                             */

/* =========================================================================================================================== */
/* ================                                            RMBn                                             ================ */
/* =========================================================================================================================== */

/* ==========================================================  HF0  =========================================================== */
 #define R_CANFD_RMBn_HF0_ID_Pos        (0UL)          /*!< RMID (Bit 0)                                          */
 #define R_CANFD_RMBn_HF0_ID_Msk        (0x1fffffffUL) /*!< RMID (Bitfield-Mask: 0x1fffffff)                      */
 #define R_CANFD_RMBn_HF0_RTR_Pos       (30UL)         /*!< RMRTR (Bit 30)                                        */
 #define R_CANFD_RMBn_HF0_RTR_Msk       (0x40000000UL) /*!< RMRTR (Bitfield-Mask: 0x01)                           */
 #define R_CANFD_RMBn_HF0_IDE_Pos       (31UL)         /*!< RMIDE (Bit 31)                                        */
 #define R_CANFD_RMBn_HF0_IDE_Msk       (0x80000000UL) /*!< RMIDE (Bitfield-Mask: 0x01)                           */
/* ==========================================================  HF1  ========================================================== */
 #define R_CANFD_RMBn_HF1_TS_Pos       (0UL)          /*!< RMTS (Bit 0)                                          */
 #define R_CANFD_RMBn_HF1_TS_Msk       (0xffffUL)     /*!< RMTS (Bitfield-Mask: 0xffff)                          */
 #define R_CANFD_RMBn_HF1_DLC_Pos      (28UL)         /*!< RMDLC (Bit 28)                                        */
 #define R_CANFD_RMBn_HF1_DLC_Msk      (0xf0000000UL) /*!< RMDLC (Bitfield-Mask: 0x0f)                           */
/* =========================================================  FDSTS  ========================================================= */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMESI_Pos    (0UL)          /*!< RMESI (Bit 0)                                         */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMESI_Msk    (0x1UL)        /*!< RMESI (Bitfield-Mask: 0x01)                           */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMBRS_Pos    (1UL)          /*!< RMBRS (Bit 1)                                         */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMBRS_Msk    (0x2UL)        /*!< RMBRS (Bitfield-Mask: 0x01)                           */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMFDF_Pos    (2UL)          /*!< RMFDF (Bit 2)                                         */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMFDF_Msk    (0x4UL)        /*!< RMFDF (Bitfield-Mask: 0x01)                           */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMIFL_Pos    (8UL)          /*!< RMIFL (Bit 8)                                         */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMIFL_Msk    (0x300UL)      /*!< RMIFL (Bitfield-Mask: 0x03)                           */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMPTR_Pos    (16UL)         /*!< RMPTR (Bit 16)                                        */
 #define R_CANFDL_CFDRMC_RM_FDSTS_RMPTR_Msk    (0xffff0000UL) /*!< RMPTR (Bitfield-Mask: 0xffff)                         */
/* ==========================================================  DF  =========================================================== */
 #define R_CANFDL_CFDRMC_RM_DF_RMDB_Pos        (0UL)          /*!< RMDB (Bit 0)                                          */
 #define R_CANFDL_CFDRMC_RM_DF_RMDB_Msk        (0xffUL)       /*!< RMDB (Bitfield-Mask: 0xff)                            */
/* ========================================================  GSR  ======================================================== */
#define R_CANFD_GSR_GRSTSTS_Pos             (0UL)          /*!< GRSTSTS (Bit 0)                                       */
#define R_CANFD_GSR_GRSTSTS_Msk             (0x1UL)        /*!< GRSTSTS (Bitfield-Mask: 0x01)                         */
#define R_CANFD_GSR_GHLTSTS_Pos             (1UL)          /*!< GHLTSTS (Bit 1)                                       */
#define R_CANFD_GSR_GHLTSTS_Msk             (0x2UL)        /*!< GHLTSTS (Bitfield-Mask: 0x01)                         */
#define R_CANFD_GSR_GSLPSTS_Pos             (2UL)          /*!< GSLPSTS (Bit 2)                                       */
#define R_CANFD_GSR_GSLPSTS_Msk             (0x4UL)        /*!< GSLPSTS (Bitfield-Mask: 0x01)                         */
#define R_CANFD_GSR_GRAMINIT_Pos            (3UL)          /*!< GRAMINIT (Bit 3)                                      */
#define R_CANFD_GSR_GRAMINIT_Msk            (0x8UL)        /*!< GRAMINIT (Bitfield-Mask: 0x01)                        */


/* Function-like macro used to wait for a condition to be met, most often used to wait for hardware register updates.
 * This macro can be redefined to add a timeout if necessary. */
#ifndef FSP_HARDWARE_REGISTER_WAIT
 #define FSP_HARDWARE_REGISTER_WAIT(reg, required_value)    while ((reg) != (required_value)) { /* Wait. */} /* WAIT_LOOP */
#endif




/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Available modules. */
typedef enum e_fsp_ip
{
    FSP_IP_CANFD  = 76,                ///< CAN-FD
} fsp_ip_t;

/** CANFD Status */
typedef enum e_canfd_status
{
    CANFD_STATUS_RESET_MODE    = 0x001, ///< Channel in Reset mode
    CANFD_STATUS_HALT_MODE     = 0x002, ///< Channel in Halt mode
    CANFD_STATUS_SLEEP_MODE    = 0x004, ///< Channel in Sleep mode
    CANFD_STATUS_ERROR_PASSIVE = 0x008, ///< Channel in error-passive state
    CANFD_STATUS_BUS_OFF       = 0x010, ///< Channel in bus-off state
    CANFD_STATUS_TRANSMITTING  = 0x020, ///< Channel is transmitting
    CANFD_STATUS_RECEIVING     = 0x040, ///< Channel is receiving
    CANFD_STATUS_READY         = 0x080, ///< Channel is ready for communication
    CANFD_STATUS_ESI           = 0x100, ///< At least one CAN-FD message was received with the ESI flag set
} canfd_status_t;

/** CANFD Error Code */
typedef enum e_canfd_error
{
    CANFD_ERROR_CHANNEL_BUS              = 0x00000001, ///< Bus Error
    CANFD_ERROR_CHANNEL_WARNING          = 0x00000002, ///< Error Warning (TX/RX error count over 0x5F)
    CANFD_ERROR_CHANNEL_PASSIVE          = 0x00000004, ///< Error Passive (TX/RX error count over 0x7F)
    CANFD_ERROR_CHANNEL_BUS_OFF_ENTRY    = 0x00000008, ///< Bus-Off State Entry
    CANFD_ERROR_CHANNEL_BUS_OFF_RECOVERY = 0x00000010, ///< Recovery from Bus-Off State
    CANFD_ERROR_CHANNEL_OVERLOAD         = 0x00000020, ///< Overload
    CANFD_ERROR_CHANNEL_BUS_LOCK         = 0x00000040, ///< Bus Locked
    CANFD_ERROR_CHANNEL_ARBITRATION_LOSS = 0x00000080, ///< Arbitration Lost
    CANFD_ERROR_CHANNEL_STUFF            = 0x00000100, ///< Stuff Error
    CANFD_ERROR_CHANNEL_FORM             = 0x00000200, ///< Form Error
    CANFD_ERROR_CHANNEL_ACK              = 0x00000400, ///< ACK Error
    CANFD_ERROR_CHANNEL_CRC              = 0x00000800, ///< CRC Error
    CANFD_ERROR_CHANNEL_BIT_RECESSIVE    = 0x00001000, ///< Bit Error (recessive) Error
    CANFD_ERROR_CHANNEL_BIT_DOMINANT     = 0x00002000, ///< Bit Error (dominant) Error
    CANFD_ERROR_CHANNEL_ACK_DELIMITER    = 0x00004000, ///< ACK Delimiter Error
    CANFD_ERROR_GLOBAL_DLC               = 0x00010000, ///< DLC Error
    CANFD_ERROR_GLOBAL_MESSAGE_LOST      = 0x00020000, ///< Message Lost
    CANFD_ERROR_GLOBAL_PAYLOAD_OVERFLOW  = 0x00080000, ///< FD Payload Overflow
    CANFD_ERROR_GLOBAL_TXQ_OVERWRITE     = 0x00100000, ///< TX Queue Message Overwrite
    CANFD_ERROR_GLOBAL_TXQ_MESSAGE_LOST  = 0x00400000, ///< TX Queue Message Lost
    CANFD_ERROR_GLOBAL_CH0_SCAN_FAIL     = 0x01000000, ///< Channel 0 RX Scan Failure
    CANFD_ERROR_GLOBAL_CH1_SCAN_FAIL     = 0x02000000, ///< Channel 1 RX Scan Failure
    CANFD_ERROR_GLOBAL_CH0_ECC           = 0x10000000, ///< Channel 0 ECC Error
    CANFD_ERROR_GLOBAL_CH1_ECC           = 0x20000000, ///< Channel 1 ECC Error
} canfd_error_t;

/** CANFD Transmit Message Buffer (TX MB) */
typedef enum e_canfd_tx_mb
{
    CANFD_TX_MB_0 = 0,
    CANFD_TX_MB_1 = 1,
    CANFD_TX_MB_2 = 2,
    CANFD_TX_MB_3 = 3,
} canfd_tx_mb_t;

/** CANFD Receive Buffer (MB + FIFO) */
typedef enum e_canfd_rx_buffer
{
    CANFD_RX_BUFFER_MB_0   = 0,
    CANFD_RX_BUFFER_MB_1   = 1,
    CANFD_RX_BUFFER_MB_2   = 2,
    CANFD_RX_BUFFER_MB_3   = 3,
    CANFD_RX_BUFFER_MB_4   = 4,
    CANFD_RX_BUFFER_MB_5   = 5,
    CANFD_RX_BUFFER_MB_6   = 6,
    CANFD_RX_BUFFER_MB_7   = 7,
    CANFD_RX_BUFFER_MB_8   = 8,
    CANFD_RX_BUFFER_MB_9   = 9,
    CANFD_RX_BUFFER_MB_10  = 10,
    CANFD_RX_BUFFER_MB_11  = 11,
    CANFD_RX_BUFFER_MB_12  = 12,
    CANFD_RX_BUFFER_MB_13  = 13,
    CANFD_RX_BUFFER_MB_14  = 14,
    CANFD_RX_BUFFER_MB_15  = 15,
    CANFD_RX_BUFFER_MB_16  = 16,
    CANFD_RX_BUFFER_MB_17  = 17,
    CANFD_RX_BUFFER_MB_18  = 18,
    CANFD_RX_BUFFER_MB_19  = 19,
    CANFD_RX_BUFFER_MB_20  = 20,
    CANFD_RX_BUFFER_MB_21  = 21,
    CANFD_RX_BUFFER_MB_22  = 22,
    CANFD_RX_BUFFER_MB_23  = 23,
    CANFD_RX_BUFFER_MB_24  = 24,
    CANFD_RX_BUFFER_MB_25  = 25,
    CANFD_RX_BUFFER_MB_26  = 26,
    CANFD_RX_BUFFER_MB_27  = 27,
    CANFD_RX_BUFFER_MB_28  = 28,
    CANFD_RX_BUFFER_MB_29  = 29,
    CANFD_RX_BUFFER_MB_30  = 30,
    CANFD_RX_BUFFER_MB_31  = 31,
    CANFD_RX_BUFFER_FIFO_0 = 32,
    CANFD_RX_BUFFER_FIFO_1 = 33,
} canfd_rx_buffer_t;

/** CANFD Receive Message Buffer (RX MB) */
typedef enum e_canfd_rx_mb
{
    CANFD_RX_MB_NONE = 0,
    CANFD_RX_MB_0    = 0x80,
    CANFD_RX_MB_1    = 0x80 + 1,
    CANFD_RX_MB_2    = 0x80 + 2,
    CANFD_RX_MB_3    = 0x80 + 3,
    CANFD_RX_MB_4    = 0x80 + 4,
    CANFD_RX_MB_5    = 0x80 + 5,
    CANFD_RX_MB_6    = 0x80 + 6,
    CANFD_RX_MB_7    = 0x80 + 7,
    CANFD_RX_MB_8    = 0x80 + 8,
    CANFD_RX_MB_9    = 0x80 + 9,
    CANFD_RX_MB_10   = 0x80 + 10,
    CANFD_RX_MB_11   = 0x80 + 11,
    CANFD_RX_MB_12   = 0x80 + 12,
    CANFD_RX_MB_13   = 0x80 + 13,
    CANFD_RX_MB_14   = 0x80 + 14,
    CANFD_RX_MB_15   = 0x80 + 15,
    CANFD_RX_MB_16   = 0x80 + 16,
    CANFD_RX_MB_17   = 0x80 + 17,
    CANFD_RX_MB_18   = 0x80 + 18,
    CANFD_RX_MB_19   = 0x80 + 19,
    CANFD_RX_MB_20   = 0x80 + 20,
    CANFD_RX_MB_21   = 0x80 + 21,
    CANFD_RX_MB_22   = 0x80 + 22,
    CANFD_RX_MB_23   = 0x80 + 23,
    CANFD_RX_MB_24   = 0x80 + 24,
    CANFD_RX_MB_25   = 0x80 + 25,
    CANFD_RX_MB_26   = 0x80 + 26,
    CANFD_RX_MB_27   = 0x80 + 27,
    CANFD_RX_MB_28   = 0x80 + 28,
    CANFD_RX_MB_29   = 0x80 + 29,
    CANFD_RX_MB_30   = 0x80 + 30,
    CANFD_RX_MB_31   = 0x80 + 31,
} canfd_rx_mb_t;

/** CANFD Receive FIFO (RX FIFO) */
typedef enum e_canfd_rx_fifo
{
    CANFD_RX_FIFO_0 = (1U),
    CANFD_RX_FIFO_1 = (1U << 1),
} canfd_rx_fifo_t;

/** CANFD AFL Minimum DLC settings */
typedef enum e_canfd_minimum_dlc
{
    CANFD_MINIMUM_DLC_0 = 0,
    CANFD_MINIMUM_DLC_1,
    CANFD_MINIMUM_DLC_2,
    CANFD_MINIMUM_DLC_3,
    CANFD_MINIMUM_DLC_4,
    CANFD_MINIMUM_DLC_5,
    CANFD_MINIMUM_DLC_6,
    CANFD_MINIMUM_DLC_7,
    CANFD_MINIMUM_DLC_8,
    CANFD_MINIMUM_DLC_12,
    CANFD_MINIMUM_DLC_16,
    CANFD_MINIMUM_DLC_20,
    CANFD_MINIMUM_DLC_24,
    CANFD_MINIMUM_DLC_32,
    CANFD_MINIMUM_DLC_48,
    CANFD_MINIMUM_DLC_64,
} canfd_minimum_dlc_t;

/** CANFD Frame Options */
typedef enum e_canfd_frame_option
{
    CANFD_FRAME_OPTION_ERROR = 0x01,   ///< Error state set (ESI).
    CANFD_FRAME_OPTION_BRS   = 0x02,   ///< Bit Rate Switching (BRS) enabled.
    CANFD_FRAME_OPTION_FD    = 0x04,   ///< Flexible Data frame (FDF).
} canfd_frame_options_t;

/* CAN Instance Control Block   */
typedef struct st_canfd_instance_ctrl
{
    /* Parameters to control CAN peripheral device */
    can_cfg_t const    * p_cfg;                 // Pointer to the configuration structure
    uint32_t             open;                  // Open status of channel.
    can_operation_mode_t operation_mode;        // Can operation mode.
    can_test_mode_t      test_mode;             // Can operation mode.
    void (* p_callback)(can_callback_args_t *); // Pointer to callback
    can_callback_args_t * p_callback_memory;    // Pointer to optional callback argument memory
    void const          * p_context;            // Pointer to context to be passed into callback function
} canfd_instance_ctrl_t;

/** AFL Entry */
typedef struct st_canfd_afl_entry_t
{
	uint32_t id                 : 29; ///< ID to match against
	uint32_t rs                 : 1;
	can_frame_type_t frame_type : 1;  ///< Frame type (Data or Remote)
	can_id_mode_t    id_mode    : 1;  ///< ID mode (Standard or Extended)

	uint32_t mask_id         	: 29; ///< ID Mask
	uint32_t rs1                : 1;
	uint32_t mask_frame_type 	: 1;  ///< Only accept frames with the configured frame type
	uint32_t mask_id_mode    	: 1;  ///< Only accept frames with the configured ID mode

	canfd_minimum_dlc_t minimum_dlc 	: 4; ///< Minimum DLC value to accept (valid when DLC Check is enabled)
	uint32_t rs2                    	: 4;
	canfd_rx_mb_t rx_buffer         	: 8; ///< RX Message Buffer to receive messages accepted by this rule
	uint32_t rs3                  		: 16;
	canfd_rx_fifo_t fifo_select_flags;   ///< RX FIFO(s) to receive messages accepted by this rule
} canfd_afl_entry_t;

/** CANFD Global Configuration */
typedef struct st_canfd_global_cfg
{
uint32_t global_interrupts;        ///< Global control options (CFDGCTR register setting)
    uint32_t global_config;            ///< Global configuration options (CFDGCFG register setting)
    uint32_t rx_fifo_config[2];        ///< RX FIFO configuration (CFDRFCCn register settings)
    uint32_t rx_mb_config;             ///< Number and size of RX Message Buffers (CFDRMNB register setting)
    uint8_t  global_err_ipl;           ///< Global Error interrupt priority
    uint8_t  rx_fifo_ipl;              ///< RX FIFO interrupt priority
} canfd_global_cfg_t;

/** CANFD Extended Configuration */
typedef struct st_canfd_extended_cfg
{
    canfd_afl_entry_t const * p_afl;           ///< AFL rules list
    uint32_t               txmb_txi_enable;    ///< Array of TX Message Buffer enable bits
    uint32_t               error_interrupts;   ///< Error interrupt enable bits
    can_bit_timing_cfg_t * p_data_timing;      ///< FD Data Rate (when bitrate switching is used)
    uint8_t                delay_compensation; ///< FD Transceiver Delay Compensation (enable or disable)
    canfd_global_cfg_t   * p_global_cfg;       ///< Global configuration (global error callback channel only)
} canfd_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const can_api_t g_canfd_on_canfd;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_CANFD_Open(can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg);
fsp_err_t R_CANFD_Close(can_ctrl_t * const p_api_ctrl);
fsp_err_t R_CANFD_Write(can_ctrl_t * const p_api_ctrl, uint32_t const buffer, can_frame_t * const p_frame);
fsp_err_t R_CANFD_Read(can_ctrl_t * const p_api_ctrl, uint32_t const buffer, can_frame_t * const p_frame);
fsp_err_t R_CANFD_ModeTransition(can_ctrl_t * const   p_api_ctrl,
                                 can_operation_mode_t operation_mode,
                                 can_test_mode_t      test_mode);
fsp_err_t R_CANFD_InfoGet(can_ctrl_t * const p_api_ctrl, can_info_t * const p_info);
fsp_err_t R_CANFD_CallbackSet(can_ctrl_t * const          p_api_ctrl,
                              void (                    * p_callback)(can_callback_args_t *),
                              void const * const          p_context,
                              can_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end defgroup CAN)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* R_CANFD_H */
