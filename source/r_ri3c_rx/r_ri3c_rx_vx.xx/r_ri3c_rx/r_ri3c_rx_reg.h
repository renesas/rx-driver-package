/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
 * applicable laws, including copyright laws. 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO 
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the 
 * following link:
 * http://www.renesas.com/disclaimer 
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_ri3c_rx_reg.h
 * Description  : Macros for FIT RI3C registers
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 15.08.2022 1.00    First release
 *                              Support RX26T.
 **********************************************************************************************************************/

#ifndef R_RI3C_REG_H_
 #define R_RI3C_REG_H_

/***********************************************************************************************************************
 * RI3C Masking and Position Register Macro
 **********************************************************************************************************************/

/* =========================================================================================================================== */
/* ================                                          R_RI3C                                           ================ */
/* =========================================================================================================================== */
/* =========================================================  ICMR  ========================================================== */
 #define R_RI3C0_ICMR_OMS_Pos          (0UL)          /*!< OMS (Bit 0)                                                         */
 #define R_RI3C0_ICMR_OMS_Msk          (0x1UL)        /*!< OMS (Bitfield-Mask: 0x01)                                           */
/* =========================================================  ICCR  ========================================================== */
 #define R_RI3C0_ICCR_IBAINC_Pos       (0UL)          /*!< IBAINC (Bit 0)                                                      */
 #define R_RI3C0_ICCR_IBAINC_Msk       (0x1UL)        /*!< IBAINC (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICCR_HJC_Pos          (8UL)          /*!< HJC (Bit 8)                                                         */
 #define R_RI3C0_ICCR_HJC_Msk          (0x100UL)      /*!< HJC (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICCR_ABORT_Pos        (29UL)         /*!< ABORT (Bit 29)                                                      */
 #define R_RI3C0_ICCR_ABORT_Msk        (0x20000000UL) /*!< ABORT (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCR_RESUME_Pos       (30UL)         /*!< RESUME (Bit 30)                                                     */
 #define R_RI3C0_ICCR_RESUME_Msk       (0x40000000UL) /*!< RESUME (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICCR_ICE_Pos          (31UL)         /*!< ICE (Bit 31)                                                        */
 #define R_RI3C0_ICCR_ICE_Msk          (0x80000000UL) /*!< ICE (Bitfield-Mask: 0x01)                                           */
/* ========================================================  ICCAR  ========================================================== */
 #define R_RI3C0_ICCAR_DADR_Pos        (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICCAR_DADR_Msk        (0x7f0000UL)   /*!< DADR (Bitfield-Mask: 0x7f)                                          */
 #define R_RI3C0_ICCAR_DAV_Pos         (31UL)         /*!< DAV (Bit 31)                                                        */
 #define R_RI3C0_ICCAR_DAV_Msk         (0x80000000UL) /*!< DAV (Bitfield-Mask: 0x01)                                           */
/* ========================================================  ICRCR  ========================================================== */
 #define R_RI3C0_ICRCR_MRST_Pos        (0UL)          /*!< MRST (Bit 0)                                                        */
 #define R_RI3C0_ICRCR_MRST_Msk        (0x1UL)        /*!< MRST (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICRCR_CQRST_Pos       (1UL)          /*!< CQRST (Bit 1)                                                       */
 #define R_RI3C0_ICRCR_CQRST_Msk       (0x2UL)        /*!< CQRST (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICRCR_RQRST_Pos       (2UL)          /*!< RQRST (Bit 2)                                                       */
 #define R_RI3C0_ICRCR_RQRST_Msk       (0x4UL)        /*!< RQRST (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICRCR_TBRST_Pos       (3UL)          /*!< TBRST (Bit 3)                                                       */
 #define R_RI3C0_ICRCR_TBRST_Msk       (0x8UL)        /*!< TBRST (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICRCR_RBRST_Pos       (4UL)          /*!< RBRST (Bit 4)                                                       */
 #define R_RI3C0_ICRCR_RBRST_Msk       (0x10UL)       /*!< RBRST (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICRCR_IQRST_Pos       (5UL)          /*!< IQRST (Bit 5)                                                       */
 #define R_RI3C0_ICRCR_IQRST_Msk       (0x20UL)       /*!< IQRST (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICRCR_SQRST_Pos       (6UL)          /*!< SQRST (Bit 6)                                                       */
 #define R_RI3C0_ICRCR_SQRST_Msk       (0x40UL)       /*!< SQRST (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICRCR_ISRST_Pos       (16UL)         /*!< ISRST (Bit 16)                                                      */
 #define R_RI3C0_ICRCR_ISRST_Msk       (0x10000UL)    /*!< ISRST (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICMMR  ========================================================= */
 #define R_RI3C0_ICMMR_ACF_Pos         (2UL)          /*!< ACF (Bit 2)                                                         */
 #define R_RI3C0_ICMMR_ACF_Msk         (0x4UL)        /*!< ACF (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICMMR_WP_Pos          (7UL)          /*!< WP (Bit 7)                                                          */
 #define R_RI3C0_ICMMR_WP_Msk          (0x80UL)       /*!< WP (Bitfield-Mask: 0x01)                                            */
/* =========================================================  ICISR  ========================================================= */
 #define R_RI3C0_ICISR_BERF_Pos        (10UL)         /*!< BERF (Bit 10)                                                       */
 #define R_RI3C0_ICISR_BERF_Msk        (0x400UL)      /*!< BERF (Bitfield-Mask: 0x01)                                          */
/* =========================================================  ICISER  ======================================================== */
 #define R_RI3C0_ICISER_BERDE_Pos      (10UL)         /*!< BERDE (Bit 10)                                                      */
 #define R_RI3C0_ICISER_BERDE_Msk      (0x400UL)      /*!< BERDE (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICISIER  ======================================================= */
 #define R_RI3C0_ICISIER_BERIE_Pos     (10UL)         /*!< BERIE (Bit 10)                                                      */
 #define R_RI3C0_ICISIER_BERIE_Msk     (0x400UL)      /*!< BERIE (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICDCTIR  ======================================================= */
 #define R_RI3C0_ICDCTIR_INDEX_Pos     (19UL)         /*!< INDEX (Bit 19)                                                      */
 #define R_RI3C0_ICDCTIR_INDEX_Msk     (0xf80000UL)   /*!< INDEX (Bitfield-Mask: 0x1f)                                         */
/* ========================================================  ICINCR  ========================================================= */
 #define R_RI3C0_ICINCR_RHJRN_Pos      (0UL)          /*!< RHJRN (Bit 0)                                                       */
 #define R_RI3C0_ICINCR_RHJRN_Msk      (0x1UL)        /*!< RHJRN (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICINCR_RCRRN_Pos      (1UL)          /*!< RCRRN (Bit 1)                                                       */
 #define R_RI3C0_ICINCR_RCRRN_Msk      (0x2UL)        /*!< RCRRN (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICINCR_RTIRN_Pos      (3UL)          /*!< RTIRN (Bit 3)                                                       */
 #define R_RI3C0_ICINCR_RTIRN_Msk      (0x8UL)        /*!< RTIRN (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICTCR  ========================================================= */
 #define R_RI3C0_ICTCR_TA0DE_Pos       (16UL)         /*!< TA0DE (Bit 16)                                                      */
 #define R_RI3C0_ICTCR_TA0DE_Msk       (0x10000UL)    /*!< TA0DE (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICSBR  ========================================================= */
 #define R_RI3C0_ICSBR_ODLW_Pos        (0UL)          /*!< ODLW (Bit 0)                                                        */
 #define R_RI3C0_ICSBR_ODLW_Msk        (0xffUL)       /*!< ODLW (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICSBR_ODHW_Pos        (8UL)          /*!< ODHW (Bit 8)                                                        */
 #define R_RI3C0_ICSBR_ODHW_Msk        (0xff00UL)     /*!< ODHW (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICSBR_PPLW_Pos        (16UL)         /*!< PPLW (Bit 16)                                                       */
 #define R_RI3C0_ICSBR_PPLW_Msk        (0x3f0000UL)   /*!< PPLW (Bitfield-Mask: 0x3f)                                          */
 #define R_RI3C0_ICSBR_PPHW_Pos        (24UL)         /*!< PPHW (Bit 24)                                                       */
 #define R_RI3C0_ICSBR_PPHW_Msk        (0x3f000000UL) /*!< PPHW (Bitfield-Mask: 0x3f)                                          */
 #define R_RI3C0_ICSBR_ODDBL_Pos       (31UL)         /*!< ODDBL (Bit 31)                                                      */
 #define R_RI3C0_ICSBR_ODDBL_Msk       (0x80000000UL) /*!< ODDBL (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICEBR  ========================================================= */
 #define R_RI3C0_ICEBR_ODLW_Pos        (0UL)          /*!< ODLW (Bit 0)                                                        */
 #define R_RI3C0_ICEBR_ODLW_Msk        (0xffUL)       /*!< ODLW (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICEBR_ODHW_Pos        (8UL)          /*!< ODHW (Bit 8)                                                        */
 #define R_RI3C0_ICEBR_ODHW_Msk        (0xff00UL)     /*!< ODHW (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICEBR_PPLW_Pos        (16UL)         /*!< PPLW (Bit 16)                                                       */
 #define R_RI3C0_ICEBR_PPLW_Msk        (0x3f0000UL)   /*!< PPLW (Bitfield-Mask: 0x3f)                                          */
 #define R_RI3C0_ICEBR_PPHW_Pos        (24UL)         /*!< PPHW (Bit 24)                                                       */
 #define R_RI3C0_ICEBR_PPHW_Msk        (0x3f000000UL) /*!< PPHW (Bitfield-Mask: 0x3f)                                          */
/* ========================================================  ICOCR  ========================================================== */
 #define R_RI3C0_ICOCR_SDAO_Pos        (0UL)          /*!< SDAO (Bit 0)                                                        */
 #define R_RI3C0_ICOCR_SDAO_Msk        (0x1UL)        /*!< SDAO (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICOCR_SCLO_Pos        (1UL)          /*!< SCLO (Bit 1)                                                        */
 #define R_RI3C0_ICOCR_SCLO_Msk        (0x2UL)        /*!< SCLO (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICOCR_SOWP_Pos        (2UL)          /*!< SOWP (Bit 2)                                                        */
 #define R_RI3C0_ICOCR_SOWP_Msk        (0x4UL)        /*!< SOWP (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICTOR  ========================================================== */
 #define R_RI3C0_ICTOR_TMOS_Pos        (0UL)          /*!< TMOS (Bit 0)                                                        */
 #define R_RI3C0_ICTOR_TMOS_Msk        (0x3UL)        /*!< TMOS (Bitfield-Mask: 0x03)                                          */
 #define R_RI3C0_ICTOR_TMOL_Pos        (4UL)          /*!< TMOL (Bit 4)                                                        */
 #define R_RI3C0_ICTOR_TMOL_Msk        (0x10UL)       /*!< TMOL (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICTOR_TMOH_Pos        (5UL)          /*!< TMOH (Bit 5)                                                        */
 #define R_RI3C0_ICTOR_TMOH_Msk        (0x20UL)       /*!< TMOH (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICTOR_TMOM_Pos        (6UL)          /*!< TMOM (Bit 6)                                                        */
 #define R_RI3C0_ICTOR_TMOM_Msk        (0xc0UL)       /*!< TMOM (Bitfield-Mask: 0x03)                                          */
/* =======================================================  ICSTCR  ========================================================== */
 #define R_RI3C0_ICSTCR_STT_Pos        (0UL)          /*!< STT (Bit 0)                                                         */
 #define R_RI3C0_ICSTCR_STT_Msk        (0xffffUL)     /*!< STT (Bitfield-Mask: 0xffff)                                         */
 #define R_RI3C0_ICSTCR_AASE_Pos       (28UL)         /*!< AASE (Bit 28)                                                       */
 #define R_RI3C0_ICSTCR_AASE_Msk       (0x10000000UL) /*!< AASE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICSTCR_PBSE_Pos       (30UL)         /*!< PBSE (Bit 30)                                                       */
 #define R_RI3C0_ICSTCR_PBSE_Msk       (0x40000000UL) /*!< PBSE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICSTCR_APSE_Pos       (31UL)         /*!< APSE (Bit 31)                                                       */
 #define R_RI3C0_ICSTCR_APSE_Msk       (0x80000000UL) /*!< APSE (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICTDLR  ========================================================= */
 #define R_RI3C0_ICTDLR_DLGTH_Pos       (16UL)         /*!< DLGTH (Bit 16)                                                     */
 #define R_RI3C0_ICTDLR_DLGTH_Msk       (0xffff0000UL) /*!< DLGTH (Bitfield-Mask: 0xffff)                                      */
/* ========================================================  ICQBTCR  ======================================================== */
 #define R_RI3C0_ICQBTCR_CQTH_Pos      (0UL)          /*!< CQTH (Bit 0)                                                        */
 #define R_RI3C0_ICQBTCR_CQTH_Msk      (0xffUL)       /*!< CQTH (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICQBTCR_RQTH_Pos      (8UL)          /*!< RQTH (Bit 8)                                                        */
 #define R_RI3C0_ICQBTCR_RQTH_Msk      (0xff00UL)     /*!< RQTH (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICQBTCR_IDSS_Pos      (16UL)         /*!< IDSS (Bit 16)                                                       */
 #define R_RI3C0_ICQBTCR_IDSS_Msk      (0xff0000UL)   /*!< IDSS (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICQBTCR_IQTH_Pos      (24UL)         /*!< IQTH (Bit 24)                                                       */
 #define R_RI3C0_ICQBTCR_IQTH_Msk      (0xff000000UL) /*!< IQTH (Bitfield-Mask: 0xff)                                          */
/* =======================================================  ICDBTCR  ========================================================= */
 #define R_RI3C0_ICDBTCR_TETH_Pos      (0UL)            /*!< TETH (Bit 0)                                                      */
 #define R_RI3C0_ICDBTCR_TETH_Msk      (0x7UL)          /*!< TETH (Bitfield-Mask: 0x07)                                        */
 #define R_RI3C0_ICDBTCR_RFTH_Pos      (8UL)            /*!< RFTH (Bit 8)                                                      */
 #define R_RI3C0_ICDBTCR_RFTH_Msk      (0x700UL)        /*!< RFTH (Bitfield-Mask: 0x07)                                        */
 #define R_RI3C0_ICDBTCR_TSTH_Pos      (16UL)           /*!< TSTH (Bit 16)                                                     */
 #define R_RI3C0_ICDBTCR_TSTH_Msk      (0x70000UL)      /*!< TSTH (Bitfield-Mask: 0x07)                                        */
 #define R_RI3C0_ICDBTCR_RSTH_Pos      (24UL)           /*!< RSTH (Bit 24)                                                     */
 #define R_RI3C0_ICDBTCR_RSTH_Msk      (0x7000000UL)    /*!< RSTH (Bitfield-Mask: 0x07)                                        */
/* =======================================================  ICSQTCR  ========================================================= */
 #define R_RI3C0_ICSQTCR_SQTH_Pos      (0UL)            /*!< SQTH (Bit 0)                                                      */
 #define R_RI3C0_ICSQTCR_SQTH_Msk      (0xffUL)         /*!< SQTH (Bitfield-Mask: 0xff)                                        */
/* ==========================================================  ICSR2  ======================================================== */
 #define R_RI3C0_ICSR2_START_Pos       (0UL)            /*!< START (Bit 0)                                                     */
 #define R_RI3C0_ICSR2_START_Msk       (0x1UL)          /*!< START (Bitfield-Mask: 0x01)                                       */
 #define R_RI3C0_ICSR2_STOP_Pos        (1UL)            /*!< STOP (Bit 1)                                                      */
 #define R_RI3C0_ICSR2_STOP_Msk        (0x2UL)          /*!< STOP (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICSR2_HDRXDF_Pos      (2UL)            /*!< HDRXDF (Bit 2)                                                    */
 #define R_RI3C0_ICSR2_HDRXDF_Msk      (0x4UL)          /*!< HDRXDF (Bitfield-Mask: 0x01)                                      */
 #define R_RI3C0_ICSR2_TMOF_Pos        (20UL)           /*!< TMOF (Bit 20)                                                     */
 #define R_RI3C0_ICSR2_TMOF_Msk        (0x100000UL)     /*!< TMOF (Bitfield-Mask: 0x01)                                        */
/* =========================================================  ICSER  ========================================================= */
 #define R_RI3C0_ICSER_STDE_Pos        (0UL)          /*!< STDE (Bit 0)                                                        */
 #define R_RI3C0_ICSER_STDE_Msk        (0x1UL)        /*!< STDE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICSER_SPDE_Pos        (1UL)          /*!< SPDE (Bit 1)                                                        */
 #define R_RI3C0_ICSER_SPDE_Msk        (0x2UL)        /*!< SPDE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICSER_HDRXDE_Pos      (2UL)          /*!< HDRXDE (Bit 2)                                                      */
 #define R_RI3C0_ICSER_HDRXDE_Msk      (0x4UL)        /*!< HDRXDE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICSER_TMOE_Pos        (20UL)         /*!< TMOE (Bit 20)                                                       */
 #define R_RI3C0_ICSER_TMOE_Msk        (0x100000UL)   /*!< TMOE (Bitfield-Mask: 0x01)                                          */
/* ==========================================================  ICSIER  ======================================================= */
 #define R_RI3C0_ICSIER_STIE_Pos       (0UL)          /*!< STIE (Bit 0)                                                        */
 #define R_RI3C0_ICSIER_STIE_Msk       (0x1UL)        /*!< STIE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICSIER_SPIE_Pos       (1UL)          /*!< SPIE (Bit 1)                                                        */
 #define R_RI3C0_ICSIER_SPIE_Msk       (0x2UL)        /*!< SPIE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICSIER_HDRXIE_Pos     (2UL)          /*!< HDRXIE (Bit 2)                                                      */
 #define R_RI3C0_ICSIER_HDRXIE_Msk     (0x4UL)        /*!< HDRXIE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICSIER_TMOIE_Pos      (20UL)         /*!< TMOIE (Bit 20)                                                      */
 #define R_RI3C0_ICSIER_TMOIE_Msk      (0x100000UL)   /*!< TMOIE (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICCSR  ========================================================= */
 #define R_RI3C0_ICCSR_TDRE_Pos        (0UL)          /*!< TDRE (Bit 0)                                                        */
 #define R_RI3C0_ICCSR_TDRE_Msk        (0x1UL)        /*!< TDRE (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICCSR_RDRF_Pos        (1UL)          /*!< RDRF (Bit 1)                                                        */
 #define R_RI3C0_ICCSR_RDRF_Msk        (0x2UL)        /*!< RDRF (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICCSR_IQEFF_Pos       (2UL)          /*!< IQEFF (Bit 2)                                                       */
 #define R_RI3C0_ICCSR_IQEFF_Msk       (0x4UL)        /*!< IQEFF (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSR_CQEF_Pos        (3UL)          /*!< CQEF (Bit 3)                                                        */
 #define R_RI3C0_ICCSR_CQEF_Msk        (0x8UL)        /*!< CQEF (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICCSR_RQFF_Pos        (4UL)          /*!< RQFF (Bit 4)                                                        */
 #define R_RI3C0_ICCSR_RQFF_Msk        (0x10UL)       /*!< RQFF (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICCSR_DTAF_Pos        (5UL)          /*!< DTAF (Bit 5)                                                        */
 #define R_RI3C0_ICCSR_DTAF_Msk        (0x20UL)       /*!< DTAF (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICCSR_DTEF_Pos        (9UL)          /*!< DTEF (Bit 9)                                                        */
 #define R_RI3C0_ICCSR_DTEF_Msk        (0x200UL)      /*!< DTEF (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICCSR_SQFF_Pos        (20UL)         /*!< SQFF (Bit 20)                                                       */
 #define R_RI3C0_ICCSR_SQFF_Msk        (0x100000UL)   /*!< SQFF (Bitfield-Mask: 0x01)                                          */
/* =========================================================  ICCSER  ======================================================== */
 #define R_RI3C0_ICCSER_TDE_Pos        (0UL)          /*!< TDE (Bit 0)                                                         */
 #define R_RI3C0_ICCSER_TDE_Msk        (0x1UL)        /*!< TDE (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICCSER_RDE_Pos        (1UL)          /*!< RDE (Bit 1)                                                         */
 #define R_RI3C0_ICCSER_RDE_Msk        (0x2UL)        /*!< RDE (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICCSER_IQEFDE_Pos     (2UL)          /*!< IQEFDE (Bit 2)                                                      */
 #define R_RI3C0_ICCSER_IQEFDE_Msk     (0x4UL)        /*!< IQEFDE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICCSER_CQEDE_Pos      (3UL)          /*!< CQEDE (Bit 3)                                                       */
 #define R_RI3C0_ICCSER_CQEDE_Msk      (0x8UL)        /*!< CQEDE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSER_RQFDE_Pos      (4UL)          /*!< RQFDE (Bit 4)                                                       */
 #define R_RI3C0_ICCSER_RQFDE_Msk      (0x10UL)       /*!< RQFDE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSER_DTADE_Pos      (5UL)          /*!< DTADE (Bit 5)                                                       */
 #define R_RI3C0_ICCSER_DTADE_Msk      (0x20UL)       /*!< DTADE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSER_DTEDE_Pos      (9UL)          /*!< DTEDE (Bit 9)                                                       */
 #define R_RI3C0_ICCSER_DTEDE_Msk      (0x200UL)      /*!< DTEDE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSER_SQFDE_Pos      (20UL)         /*!< SQFDE (Bit 20)                                                      */
 #define R_RI3C0_ICCSER_SQFDE_Msk      (0x100000UL)   /*!< SQFDE (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICCSIER  ======================================================= */
 #define R_RI3C0_ICCSIER_TIE_Pos       (0UL)          /*!< TIE (Bit 0)                                                         */
 #define R_RI3C0_ICCSIER_TIE_Msk       (0x1UL)        /*!< TIE (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICCSIER_RIE_Pos       (1UL)          /*!< RIE (Bit 1)                                                         */
 #define R_RI3C0_ICCSIER_RIE_Msk       (0x2UL)        /*!< RIE (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICCSIER_IQEFIE_Pos    (2UL)          /*!< IQEFIE (Bit 2)                                                      */
 #define R_RI3C0_ICCSIER_IQEFIE_Msk    (0x4UL)        /*!< IQEFIE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICCSIER_CQEIE_Pos     (3UL)          /*!< CQEIE (Bit 3)                                                       */
 #define R_RI3C0_ICCSIER_CQEIE_Msk     (0x8UL)        /*!< CQEIE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSIER_RQFIE_Pos     (4UL)          /*!< RQFIE (Bit 4)                                                       */
 #define R_RI3C0_ICCSIER_RQFIE_Msk     (0x10UL)       /*!< RQFIE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSIER_DTAIE_Pos     (5UL)          /*!< DTAIE (Bit 5)                                                       */
 #define R_RI3C0_ICCSIER_DTAIE_Msk     (0x20UL)       /*!< DTAIE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSIER_DTEIE_Pos     (9UL)          /*!< DTEIE (Bit 9)                                                       */
 #define R_RI3C0_ICCSIER_DTEIE_Msk     (0x200UL)      /*!< DTEIE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICCSIER_SQFIE_Pos     (20UL)         /*!< SQFIE (Bit 20)                                                      */
 #define R_RI3C0_ICCSIER_SQFIE_Msk     (0x100000UL)   /*!< SQFIE (Bitfield-Mask: 0x01)                                         */
/* =========================================================  ICBSR  ========================================================= */
 #define R_RI3C0_ICBSR_BFREE_Pos       (0UL)          /*!< BFREE (Bit 0)                                                       */
 #define R_RI3C0_ICBSR_BFREE_Msk       (0x1UL)        /*!< BFREE (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICBSR_BAVL_Pos        (1UL)          /*!< BAVL (Bit 1)                                                        */
 #define R_RI3C0_ICBSR_BAVL_Msk        (0x2UL)        /*!< BAVL (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICBSR_BIDL_Pos        (2UL)          /*!< BIDL (Bit 2)                                                        */
 #define R_RI3C0_ICBSR_BIDL_Msk        (0x4UL)        /*!< BIDL (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICTDATR0  ======================================================= */
 #define R_RI3C0_ICTDATR0_SADR_Pos     (0UL)          /*!< SADR (Bit 0)                                                        */
 #define R_RI3C0_ICTDATR0_SADR_Msk     (0x7fUL)       /*!< SADR (Bitfield-Mask: 0x7f)                                          */
 #define R_RI3C0_ICTDATR0_IBIPL_Pos    (12UL)         /*!< IBIPL (Bit 12)                                                      */
 #define R_RI3C0_ICTDATR0_IBIPL_Msk    (0x1000UL)     /*!< IBIPL (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR0_TIRRJ_Pos    (13UL)         /*!< TIRRJ (Bit 13)                                                      */
 #define R_RI3C0_ICTDATR0_TIRRJ_Msk    (0x2000UL)     /*!< TIRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR0_CRRRJ_Pos    (14UL)         /*!< CRRRJ (Bit 14)                                                      */
 #define R_RI3C0_ICTDATR0_CRRRJ_Msk    (0x4000UL)     /*!< CRRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR0_IBITSE_Pos   (15UL)         /*!< IBITSE (Bit 15)                                                     */
 #define R_RI3C0_ICTDATR0_IBITSE_Msk   (0x8000UL)     /*!< IBITSE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICTDATR0_DADR_Pos     (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICTDATR0_DADR_Msk     (0xff0000UL)   /*!< DADR (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICTDATR0_NACKRC_Pos   (29UL)         /*!< NACKRC (Bit 29)                                                     */
 #define R_RI3C0_ICTDATR0_NACKRC_Msk   (0x60000000UL) /*!< NACKRC (Bitfield-Mask: 0x03)                                        */
 #define R_RI3C0_ICTDATR0_TYPE_Pos     (31UL)         /*!< TYPE (Bit 31)                                                       */
 #define R_RI3C0_ICTDATR0_TYPE_Msk     (0x80000000UL) /*!< TYPE (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICTDATR1  ======================================================= */
 #define R_RI3C0_ICTDATR1_SADR_Pos     (0UL)          /*!< SADR (Bit 0)                                                        */
 #define R_RI3C0_ICTDATR1_SADR_Msk     (0x7fUL)       /*!< SADR (Bitfield-Mask: 0x7f)                                          */
 #define R_RI3C0_ICTDATR1_IBIPL_Pos    (12UL)         /*!< IBIPL (Bit 12)                                                      */
 #define R_RI3C0_ICTDATR1_IBIPL_Msk    (0x1000UL)     /*!< IBIPL (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR1_TIRRJ_Pos    (13UL)         /*!< TIRRJ (Bit 13)                                                      */
 #define R_RI3C0_ICTDATR1_TIRRJ_Msk    (0x2000UL)     /*!< TIRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR1_CRRRJ_Pos    (14UL)         /*!< CRRRJ (Bit 14)                                                      */
 #define R_RI3C0_ICTDATR1_CRRRJ_Msk    (0x4000UL)     /*!< CRRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR1_IBITSE_Pos   (15UL)         /*!< IBITSE (Bit 15)                                                     */
 #define R_RI3C0_ICTDATR1_IBITSE_Msk   (0x8000UL)     /*!< IBITSE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICTDATR1_DADR_Pos     (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICTDATR1_DADR_Msk     (0xff0000UL)   /*!< DADR (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICTDATR1_NACKRC_Pos   (29UL)         /*!< NACKRC (Bit 29)                                                     */
 #define R_RI3C0_ICTDATR1_NACKRC_Msk   (0x60000000UL) /*!< NACKRC (Bitfield-Mask: 0x03)                                        */
 #define R_RI3C0_ICTDATR1_TYPE_Pos     (31UL)         /*!< TYPE (Bit 31)                                                       */
 #define R_RI3C0_ICTDATR1_TYPE_Msk     (0x80000000UL) /*!< TYPE (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICTDATR2  ======================================================= */
 #define R_RI3C0_ICTDATR2_SADR_Pos     (0UL)          /*!< SADR (Bit 0)                                                        */
 #define R_RI3C0_ICTDATR2_SADR_Msk     (0x7fUL)       /*!< SADR (Bitfield-Mask: 0x7f)                                          */
 #define R_RI3C0_ICTDATR2_IBIPL_Pos    (12UL)         /*!< IBIPL (Bit 12)                                                      */
 #define R_RI3C0_ICTDATR2_IBIPL_Msk    (0x1000UL)     /*!< IBIPL (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR2_TIRRJ_Pos    (13UL)         /*!< TIRRJ (Bit 13)                                                      */
 #define R_RI3C0_ICTDATR2_TIRRJ_Msk    (0x2000UL)     /*!< TIRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR2_CRRRJ_Pos    (14UL)         /*!< CRRRJ (Bit 14)                                                      */
 #define R_RI3C0_ICTDATR2_CRRRJ_Msk    (0x4000UL)     /*!< CRRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR2_IBITSE_Pos   (15UL)         /*!< IBITSE (Bit 15)                                                     */
 #define R_RI3C0_ICTDATR2_IBITSE_Msk   (0x8000UL)     /*!< IBITSE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICTDATR2_DADR_Pos     (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICTDATR2_DADR_Msk     (0xff0000UL)   /*!< DADR (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICTDATR2_NACKRC_Pos   (29UL)         /*!< NACKRC (Bit 29)                                                     */
 #define R_RI3C0_ICTDATR2_NACKRC_Msk   (0x60000000UL) /*!< NACKRC (Bitfield-Mask: 0x03)                                        */
 #define R_RI3C0_ICTDATR2_TYPE_Pos     (31UL)         /*!< TYPE (Bit 31)                                                       */
 #define R_RI3C0_ICTDATR2_TYPE_Msk     (0x80000000UL) /*!< TYPE (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICTDATR3  ======================================================= */
 #define R_RI3C0_ICTDATR3_SADR_Pos     (0UL)          /*!< SADR (Bit 0)                                                        */
 #define R_RI3C0_ICTDATR3_SADR_Msk     (0x7fUL)       /*!< SADR (Bitfield-Mask: 0x7f)                                          */
 #define R_RI3C0_ICTDATR3_IBIPL_Pos    (12UL)         /*!< IBIPL (Bit 12)                                                      */
 #define R_RI3C0_ICTDATR3_IBIPL_Msk    (0x1000UL)     /*!< IBIPL (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR3_TIRRJ_Pos    (13UL)         /*!< TIRRJ (Bit 13)                                                      */
 #define R_RI3C0_ICTDATR3_TIRRJ_Msk    (0x2000UL)     /*!< TIRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR3_CRRRJ_Pos    (14UL)         /*!< CRRRJ (Bit 14)                                                      */
 #define R_RI3C0_ICTDATR3_CRRRJ_Msk    (0x4000UL)     /*!< CRRRJ (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTDATR3_IBITSE_Pos   (15UL)         /*!< IBITSE (Bit 15)                                                     */
 #define R_RI3C0_ICTDATR3_IBITSE_Msk   (0x8000UL)     /*!< IBITSE (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICTDATR3_DADR_Pos     (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICTDATR3_DADR_Msk     (0xff0000UL)   /*!< DADR (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICTDATR3_NACKRC_Pos   (29UL)         /*!< NACKRC (Bit 29)                                                     */
 #define R_RI3C0_ICTDATR3_NACKRC_Msk   (0x60000000UL) /*!< NACKRC (Bitfield-Mask: 0x03)                                        */
 #define R_RI3C0_ICTDATR3_TYPE_Pos     (31UL)         /*!< TYPE (Bit 31)                                                       */
 #define R_RI3C0_ICTDATR3_TYPE_Msk     (0x80000000UL) /*!< TYPE (Bitfield-Mask: 0x01)                                          */
/* =======================================================  ICEDATR  ========================================================= */
 #define R_RI3C0_ICEDATR_SADR_Pos      (0UL)          /*!< SADR (Bit 0)                                                        */
 #define R_RI3C0_ICEDATR_SADR_Msk      (0x7fUL)       /*!< SADR (Bitfield-Mask: 0x7f)                                          */
 #define R_RI3C0_ICEDATR_DADR_Pos      (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICEDATR_DADR_Msk      (0xff0000UL)   /*!< DADR (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICEDATR_NACKRC_Pos    (29UL)         /*!< NACKRC (Bit 29)                                                     */
 #define R_RI3C0_ICEDATR_NACKRC_Msk    (0x60000000UL) /*!< NACKRC (Bitfield-Mask: 0x03)                                        */
 #define R_RI3C0_ICEDATR_TYPE_Pos      (31UL)         /*!< TYPE (Bit 31)                                                       */
 #define R_RI3C0_ICEDATR_TYPE_Msk      (0x80000000UL) /*!< TYPE (Bitfield-Mask: 0x01)                                          */
/* =======================================================  ICDAR0  ========================================================== */
 #define R_RI3C0_ICDAR0_SADR_Pos       (0UL)          /*!< SADR (Bit 0)                                                        */
 #define R_RI3C0_ICDAR0_SADR_Msk       (0x3ffUL)      /*!< SADR (Bitfield-Mask: 0x3ff)                                         */
 #define R_RI3C0_ICDAR0_IBIPL_Pos      (12UL)         /*!< IBIPL (Bit 12)                                                      */
 #define R_RI3C0_ICDAR0_IBIPL_Msk      (0x1000UL)     /*!< IBIPL (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICDAR0_DADR_Pos       (16UL)         /*!< DADR (Bit 16)                                                       */
 #define R_RI3C0_ICDAR0_DADR_Msk       (0x7f0000UL)   /*!< DADR (Bitfield-Mask: 0x7f)                                          */
/* =========================================================  ICDCTR  ======================================================== */
 #define R_RI3C0_ICDCTR_DCR_Pos        (0UL)          /*!< DCR (Bit 0)                                                         */
 #define R_RI3C0_ICDCTR_DCR_Msk        (0xffUL)       /*!< DCR (Bitfield-Mask: 0xff)                                           */
 #define R_RI3C0_ICDCTR_LIMIT_Pos      (8UL)          /*!< LIMIT (Bit 8)                                                       */
 #define R_RI3C0_ICDCTR_LIMIT_Msk      (0x100UL)      /*!< LIMIT (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICDCTR_IBIRQC_Pos     (9UL)          /*!< IBIRQC (Bit 9)                                                      */
 #define R_RI3C0_ICDCTR_IBIRQC_Msk     (0x200UL)      /*!< IBIRQC (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICDCTR_IBIPL_Pos      (10UL)         /*!< IBIPL (Bit 10)                                                      */
 #define R_RI3C0_ICDCTR_IBIPL_Msk      (0x400UL)      /*!< IBIPL (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICDCTR_OFLC_Pos       (11UL)         /*!< OFLC (Bit 11)                                                       */
 #define R_RI3C0_ICDCTR_OFLC_Msk       (0x800UL)      /*!< OFLC (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICDCTR_ROLE_Pos       (14UL)         /*!< ROLE (Bit 14)                                                       */
 #define R_RI3C0_ICDCTR_ROLE_Msk       (0xc000UL)     /*!< ROLE (Bitfield-Mask: 0x03)                                          */
/* ========================================================  ICDAMR0  ======================================================== */
 #define R_RI3C0_ICDAMR0_TADR_Pos      (16UL)         /*!< TADR (Bit 16)                                                       */
 #define R_RI3C0_ICDAMR0_TADR_Msk      (0x3ff0000UL)  /*!< TADR (Bitfield-Mask: 0x3ff)                                         */
 #define R_RI3C0_ICDAMR0_SAV_Pos       (30UL)         /*!< SAV (Bit 30)                                                        */
 #define R_RI3C0_ICDAMR0_SAV_Msk       (0x40000000UL) /*!< SAV (Bitfield-Mask: 0x01)                                           */
 #define R_RI3C0_ICDAMR0_DAV_Pos       (31UL)         /*!< DAV (Bit 31)                                                        */
 #define R_RI3C0_ICDAMR0_DAV_Msk       (0x80000000UL) /*!< DAV (Bitfield-Mask: 0x01)                                           */
/* ========================================================  ICTEVR  ========================================================= */
 #define R_RI3C0_ICTEVR_ENINT_Pos      (0UL)          /*!< ENINT (Bit 0)                                                       */
 #define R_RI3C0_ICTEVR_ENINT_Msk      (0x1UL)        /*!< ENINT (Bitfield-Mask: 0x01)                                         */
 #define R_RI3C0_ICTEVR_ENCR_Pos       (1UL)          /*!< ENCR (Bit 1)                                                        */
 #define R_RI3C0_ICTEVR_ENCR_Msk       (0x2UL)        /*!< ENCR (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICTEVR_ENHJ_Pos       (3UL)          /*!< ENHJ (Bit 3)                                                        */
 #define R_RI3C0_ICTEVR_ENHJ_Msk       (0x8UL)        /*!< ENHJ (Bitfield-Mask: 0x01)                                          */
/* ========================================================  ICASR  ========================================================== */
 #define R_RI3C0_ICASR_ENTAS0_Pos      (0UL)          /*!< ENTAS0 (Bit 0)                                                      */
 #define R_RI3C0_ICASR_ENTAS0_Msk      (0x1UL)        /*!< ENTAS0 (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICASR_ENTAS1_Pos      (1UL)          /*!< ENTAS1 (Bit 1)                                                      */
 #define R_RI3C0_ICASR_ENTAS1_Msk      (0x2UL)        /*!< ENTAS1 (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICASR_ENTAS2_Pos      (2UL)          /*!< ENTAS2 (Bit 2)                                                      */
 #define R_RI3C0_ICASR_ENTAS2_Msk      (0x4UL)        /*!< ENTAS2 (Bitfield-Mask: 0x01)                                        */
 #define R_RI3C0_ICASR_ENTAS3_Pos      (3UL)          /*!< ENTAS3 (Bit 3)                                                      */
 #define R_RI3C0_ICASR_ENTAS3_Msk      (0x8UL)        /*!< ENTAS3 (Bitfield-Mask: 0x01)                                        */
/* =========================================================  ICMWLR  ======================================================== */
 #define R_RI3C0_ICMWLR_MWL_Pos        (0UL)          /*!< MWL (Bit 0)                                                         */
 #define R_RI3C0_ICMWLR_MWL_Msk        (0xffffUL)     /*!< MWL (Bitfield-Mask: 0xffff)                                         */
/* =========================================================  ICMRLR  ======================================================== */
 #define R_RI3C0_ICMRLR_MRL_Pos        (0UL)          /*!< MRL (Bit 0)                                                         */
 #define R_RI3C0_ICMRLR_MRL_Msk        (0xffffUL)     /*!< MRL (Bitfield-Mask: 0xffff)                                         */
 #define R_RI3C0_ICMRLR_IBIPL_Pos      (16UL)         /*!< IBIPL (Bit 16)                                                      */
 #define R_RI3C0_ICMRLR_IBIPL_Msk      (0xff0000UL)   /*!< IBIPL (Bitfield-Mask: 0xff)                                         */
/* ========================================================  ICTMR  ========================================================== */
 #define R_RI3C0_ICTMR_TM_Pos          (0UL)          /*!< TM (Bit 0)                                                          */
 #define R_RI3C0_ICTMR_TM_Msk          (0xffUL)       /*!< TM (Bitfield-Mask: 0xff)                                            */
/* ========================================================  ICDSR  ========================================================== */
 #define R_RI3C0_ICDSR_PNDINT_Pos      (0UL)          /*!< PNDINT (Bit 0)                                                      */
 #define R_RI3C0_ICDSR_PNDINT_Msk      (0xfUL)        /*!< PNDINT (Bitfield-Mask: 0x0f)                                        */
 #define R_RI3C0_ICDSR_PERR_Pos        (5UL)          /*!< PERR (Bit 5)                                                        */
 #define R_RI3C0_ICDSR_PERR_Msk        (0x20UL)       /*!< PERR (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICDSR_CAS_Pos         (6UL)          /*!< CAS (Bit 6)                                                         */
 #define R_RI3C0_ICDSR_CAS_Msk         (0xc0UL)       /*!< CAS (Bitfield-Mask: 0x03)                                           */
 #define R_RI3C0_ICDSR_VRSV_Pos        (8UL)          /*!< VRSV (Bit 8)                                                        */
 #define R_RI3C0_ICDSR_VRSV_Msk        (0xff00UL)     /*!< VRSV (Bitfield-Mask: 0xff)                                          */
/* ========================================================  ICMWSR  ========================================================= */
 #define R_RI3C0_ICMWSR_MSWDR_Pos      (0UL)          /*!< MSWDR (Bit 0)                                                       */
 #define R_RI3C0_ICMWSR_MSWDR_Msk      (0x7UL)        /*!< MSWDR (Bitfield-Mask: 0x07)                                         */
/* ========================================================  ICMRSR  ========================================================= */
 #define R_RI3C0_ICMRSR_MSRDR_Pos      (0UL)          /*!< MSRDR (Bit 0)                                                       */
 #define R_RI3C0_ICMRSR_MSRDR_Msk      (0x7UL)        /*!< MSRDR (Bitfield-Mask: 0x07)                                         */
 #define R_RI3C0_ICMRSR_TSCO_Pos       (3UL)          /*!< TSCO (Bit 3)                                                        */
 #define R_RI3C0_ICMRSR_TSCO_Msk       (0x38UL)       /*!< TSCO (Bitfield-Mask: 0x07)                                          */
/* ========================================================  ICMTTR  ========================================================= */
 #define R_RI3C0_ICMTTR_MRTT_Pos       (0UL)          /*!< MRTT (Bit 0)                                                        */
 #define R_RI3C0_ICMTTR_MRTT_Msk       (0xffffffUL)   /*!< MRTT (Bitfield-Mask: 0xffffff)                                      */
 #define R_RI3C0_ICMTTR_MRTTE_Pos      (31UL)         /*!< MRTTE (Bit 31)                                                      */
 #define R_RI3C0_ICMTTR_MRTTE_Msk      (0x80000000UL) /*!< MRTTE (Bitfield-Mask: 0x01)                                         */
/* ========================================================  ICBCR  ========================================================== */
 #define R_RI3C0_ICBCR_BC_Pos          (0UL)          /*!< BC (Bit 0)                                                          */
 #define R_RI3C0_ICBCR_BC_Msk          (0x1fUL)       /*!< BC (Bitfield-Mask: 0x1f)                                            */
 #define R_RI3C0_ICBCR_BCWP_Pos        (7UL)          /*!< BCWP (Bit 7)                                                        */
 #define R_RI3C0_ICBCR_BCWP_Msk        (0x80UL)       /*!< BCWP (Bitfield-Mask: 0x01)                                          */
/* =========================================================  ICTSIR  ======================================================== */
 #define R_RI3C0_ICTSIR_FREQ_Pos       (8UL)          /*!< FREQ (Bit 8)                                                        */
 #define R_RI3C0_ICTSIR_FREQ_Msk       (0xff00UL)     /*!< FREQ (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICTSIR_INAC_Pos       (16UL)         /*!< INAC (Bit 16)                                                       */
 #define R_RI3C0_ICTSIR_INAC_Msk       (0xff0000UL)   /*!< INAC (Bitfield-Mask: 0xff)                                          */
/* ========================================================  ICQBSR  ========================================================= */
 #define R_RI3C0_ICQBSR_CQFL_Pos       (0UL)          /*!< CQFL (Bit 0)                                                        */
 #define R_RI3C0_ICQBSR_CQFL_Msk       (0xffUL)       /*!< CQFL (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICQBSR_RQFL_Pos       (8UL)          /*!< RQFL (Bit 8)                                                        */
 #define R_RI3C0_ICQBSR_RQFL_Msk       (0xff00UL)     /*!< RQFL (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICQBSR_IQFL_Pos       (16UL)         /*!< IQFL (Bit 16)                                                       */
 #define R_RI3C0_ICQBSR_IQFL_Msk       (0xff0000UL)   /*!< IQFL (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICQBSR_ISC_Pos        (24UL)         /*!< ISC (Bit 24)                                                        */
 #define R_RI3C0_ICQBSR_ISC_Msk        (0x1f000000UL) /*!< ISC (Bitfield-Mask: 0x1f)                                           */
/* =======================================================  ICDBSR  ========================================================== */
 #define R_RI3C0_ICDBSR_TBFL_Pos       (0UL)          /*!< TBFL (Bit 0)                                                        */
 #define R_RI3C0_ICDBSR_TBFL_Msk       (0xffUL)       /*!< TBFL (Bitfield-Mask: 0xff)                                          */
 #define R_RI3C0_ICDBSR_RBFL_Pos       (8UL)          /*!< RBFL (Bit 8)                                                        */
 #define R_RI3C0_ICDBSR_RBFL_Msk       (0xff00UL)     /*!< RBFL (Bitfield-Mask: 0xff)                                          */
/* =======================================================  ICSQSR  ========================================================== */
 #define R_RI3C0_ICSQSR_SQFL_Pos       (0UL)          /*!< SQFL (Bit 0)                                                        */
 #define R_RI3C0_ICSQSR_SQFL_Msk       (0xffUL)       /*!< SQFL (Bitfield-Mask: 0xff)                                          */
/* ========================================================  ICIMR  ========================================================== */
 #define R_RI3C0_ICIMR_SCLI_Pos        (0UL)          /*!< SCLI (Bit 0)                                                        */
 #define R_RI3C0_ICIMR_SCLI_Msk        (0x1UL)        /*!< SCLI (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICIMR_SDAI_Pos        (1UL)          /*!< SDAI (Bit 1)                                                        */
 #define R_RI3C0_ICIMR_SDAI_Msk        (0x2UL)        /*!< SDAI (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICIMR_SCLO_Pos        (2UL)          /*!< SCLO (Bit 2)                                                        */
 #define R_RI3C0_ICIMR_SCLO_Msk        (0x4UL)        /*!< SCLO (Bitfield-Mask: 0x01)                                          */
 #define R_RI3C0_ICIMR_SDAO_Pos        (3UL)          /*!< SDAO (Bit 3)                                                        */
 #define R_RI3C0_ICIMR_SDAO_Msk        (0x8UL)        /*!< SDAO (Bitfield-Mask: 0x01)                                          */
/* =======================================================  ICCECR  ========================================================== */
 #define R_RI3C0_ICCECR_CE2C_Pos       (0UL)          /*!< CE2C (Bit 0)                                                        */
 #define R_RI3C0_ICCECR_CE2C_Msk       (0xffUL)       /*!< CE2C (Bitfield-Mask: 0xff)                                          */

#endif /* R_RI3C_REG_H_ */