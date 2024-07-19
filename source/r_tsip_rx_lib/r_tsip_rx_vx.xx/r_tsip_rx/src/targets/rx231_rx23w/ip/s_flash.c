/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
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
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : s_flash.c
 * Version      : 1.11
 * Description  : Key information file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.03.2020 1.09     First Release
 *         : 30.06.2020 1.10     Added support for ARC4, ECC(P-384) API
 *         : 30.09.2020 1.11     Added support for DH, ECDHE P-512r1 API, and generalization of KDF.
 *         :                     Added support for Key wrap API with TSIP-Lite.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_tsip_rx_if.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
TSIP_SEC_C_SECURE_BOOT
uint32_t const S_FLASH[] =
{
#if defined __LIT
    0xc498dd11, 0xef7195c3, 0xf9e4fb02, 0x9920d9d0,
    0x17cde452, 0xacae495c, 0x09a2363a, 0x56129c22,
    0x652d8ad0, 0xeff4e454, 0x28a6f521, 0xa651b752,
    0x00df5a33, 0x55488be8, 0x8678b47d, 0x48128761,
    0xd4dc8d88, 0x6da0bb88, 0xaaac7416, 0x0e8e0b27,
    0x3fe1dc74, 0x8956c906, 0x15ba2a59, 0xf5d5a738,
    0x08236f99, 0x9b7604f5, 0x78a180b2, 0x340da2e2,
    0x8c04be88, 0xf06d34a9, 0x9cee2b78, 0xd9c941d4,
    0x053e167b, 0x76285ade, 0x9ae51bc9, 0xac38acc8,
    0x9f6f6b35, 0x241971e0, 0x5bd11035, 0x0c1e3428,
    0x61058cd8, 0x39bd81e4, 0x8386fa3f, 0x30c68784,
    0x2f0fbaeb, 0xb1104258, 0x434707e6, 0x8148f960,
    0x103babbc, 0x4d2fcfd8, 0xa74cdf19, 0xa2c2cd03,
    0x3851a74c, 0x95565f75, 0x9f5ab616, 0x39a97b4f,
    0xcad8b7af, 0x5c0f7b5c, 0x215404f8, 0x071c6764,
    0x45156660, 0x15a1bfee, 0xd9aaf1de, 0xa50f4724,
    0xd0cc2946, 0xded75dbf, 0x579e6fde, 0x0d758e04,
    0xfa601cf8, 0xd2153e34, 0xffc70942, 0x3599e95b,
    0x9e76aecf, 0xf9999471, 0x98fb5b26, 0xe9968cd0,
    0xfb950eb2, 0xfb7f6d33, 0x259a1084, 0x48bb1ed6,
    0x4e38e16e, 0xfb955a9e, 0x394692c3, 0x3436d100,
    0x02ce85b2, 0x9d90c18c, 0x41775c8f, 0x104c98ab,
    0x962fb4ce, 0x5cb6c1fe, 0x55fd0945, 0xfad5f613,
    0x6e905c83, 0x783dd6d8, 0x43cd33d0, 0x76a29fa4,
    0x5337932e, 0xa9597554, 0x58773e8f, 0xfce6ad45,
    0xbf6483cb, 0x6de129ce, 0x8cd5e208, 0x07d775a3,
    0x21c921bf, 0xf7883fb1, 0x49120e6a, 0xc4fd9923,
    0x908c742f, 0x4ae3e475, 0x8bf2a6be, 0x2d9703eb,
    0x8f086277, 0x0fa2f297, 0x81f6d1b4, 0x5e1d36dd,
    0xdcf867fe, 0x1813c4f9, 0x839d2a15, 0x3725108d,
    0xc3ea3a6f, 0xb8736c66, 0x397e72b0, 0x9f66a562,
    0xa090b4dc, 0xa834e233, 0x4f8bad2a, 0xe7051852,
#else   /* defined __LIT */
    0x11dd98c4, 0xc39571ef, 0x02fbe4f9, 0xd0d92099,
    0x52e4cd17, 0x5c49aeac, 0x3a36a209, 0x229c1256,
    0xd08a2d65, 0x54e4f4ef, 0x21f5a628, 0x52b751a6,
    0x335adf00, 0xe88b4855, 0x7db47886, 0x61871248,
    0x888ddcd4, 0x88bba06d, 0x1674acaa, 0x270b8e0e,
    0x74dce13f, 0x06c95689, 0x592aba15, 0x38a7d5f5,
    0x996f2308, 0xf504769b, 0xb280a178, 0xe2a20d34,
    0x88be048c, 0xa9346df0, 0x782bee9c, 0xd441c9d9,
    0x7b163e05, 0xde5a2876, 0xc91be59a, 0xc8ac38ac,
    0x356b6f9f, 0xe0711924, 0x3510d15b, 0x28341e0c,
    0xd88c0561, 0xe481bd39, 0x3ffa8683, 0x8487c630,
    0xebba0f2f, 0x584210b1, 0xe6074743, 0x60f94881,
    0xbcab3b10, 0xd8cf2f4d, 0x19df4ca7, 0x03cdc2a2,
    0x4ca75138, 0x755f5695, 0x16b65a9f, 0x4f7ba939,
    0xafb7d8ca, 0x5c7b0f5c, 0xf8045421, 0x64671c07,
    0x60661545, 0xeebfa115, 0xdef1aad9, 0x24470fa5,
    0x4629ccd0, 0xbf5dd7de, 0xde6f9e57, 0x048e750d,
    0xf81c60fa, 0x343e15d2, 0x4209c7ff, 0x5be99935,
    0xcfae769e, 0x719499f9, 0x265bfb98, 0xd08c96e9,
    0xb20e95fb, 0x336d7ffb, 0x84109a25, 0xd61ebb48,
    0x6ee1384e, 0x9e5a95fb, 0xc3924639, 0x00d13634,
    0xb285ce02, 0x8cc1909d, 0x8f5c7741, 0xab984c10,
    0xceb42f96, 0xfec1b65c, 0x4509fd55, 0x13f6d5fa,
    0x835c906e, 0xd8d63d78, 0xd033cd43, 0xa49fa276,
    0x2e933753, 0x547559a9, 0x8f3e7758, 0x45ade6fc,
    0xcb8364bf, 0xce29e16d, 0x08e2d58c, 0xa375d707,
    0xbf21c921, 0xb13f88f7, 0x6a0e1249, 0x2399fdc4,
    0x2f748c90, 0x75e4e34a, 0xbea6f28b, 0xeb03972d,
    0x7762088f, 0x97f2a20f, 0xb4d1f681, 0xdd361d5e,
    0xfe67f8dc, 0xf9c41318, 0x152a9d83, 0x8d102537,
    0x6f3aeac3, 0x666c73b8, 0xb0727e39, 0x62a5669f,
    0xdcb490a0, 0x33e234a8, 0x2aad8b4f, 0x521805e7,
#endif  /* defined __LIT */
};
TSIP_SEC_DEFAULT
/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

