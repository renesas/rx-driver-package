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
    0xA63B9A50, 0xEFE9F71B, 0xDD47172F, 0x6D59E97F,
    0xE47040C3, 0x139D7537, 0xAA9D6E6E, 0x1903EC06,
    0x49FEA826, 0xB1A2AE0F, 0x1DCD9E36, 0x36E360BB,
    0x0F5FAD66, 0x894B21D5, 0x12F7D4A8, 0x2558B490,
    0x9F6045C5, 0xF9729669, 0x01F16AE6, 0x993902BF,
    0x4326E825, 0x918A2A15, 0x9D481066, 0xE60323ED,
    0x18432DF4, 0x608EAD4D, 0xA2255895, 0x57335178,
    0x05108C8C, 0x4128AA2E, 0x81C0CE02, 0xEEBB3212,
    0x3515DC9D, 0x7109A5EE, 0xC430811B, 0xAC6AD5AF,
    0x0004DF84, 0x81000608, 0x23DFB7E8, 0x9AF42724,
    0x30FB289C, 0x8945DBE9, 0x3A0424D5, 0xE9829B81,
    0x3BF74F38, 0x0FF66C86, 0x773656E1, 0xBA8B3A9E,
    0x5DE7338B, 0x36069705, 0xB58E481E, 0x5C600A8C,
    0xACCD59B7, 0x85E89E74, 0xA3BB2E47, 0x7BC98DC9,
    0xED383891, 0xDC366873, 0x45FC9716, 0xE3272828,
    0x47B7E281, 0xA2696D62, 0xF73AA0DC, 0xC919AE07,
    0xCEC860D8, 0x06EAEAF1, 0xBBE043D3, 0xE5C799E4,
    0xD72A33DA, 0x455053C0, 0xDBEDF4A9, 0xCC810A06,
    0x1601BD7B, 0x3145B2CC, 0x5E33C9CF, 0x03092CBC,
    0x604B9B3C, 0xEF122FC0, 0x38FF6E3F, 0xA19BBAC5,
    0xBF15C9ED, 0x697E9F97, 0x4FCEB230, 0x557145F2,
    0x632F87CF, 0xDF772926, 0x3AAF5BF0, 0xE72D80BB,
    0x73B3CD38, 0x294C35A3, 0x9DA35300, 0x17B720AD,
    0x2E28EEF1, 0x66F13ABF, 0x0CDE88B8, 0x954DAAD2,
    0x4EA228CB, 0x3D59EFF8, 0x66AA4532, 0x6BA95CF7,
    0x783633F0, 0xDB326FEB, 0xC1A5193F, 0x6957272F,
    0x26D7E582, 0x3C0094E3, 0x54B1F11B, 0x81C8032D,
    0xF3A98953, 0x0EDABBB8, 0x7CDFF389, 0x1075320F,
    0x2E10834B, 0x7E730CD4, 0x440C1726, 0xE7C644E0,
    0x0455F37C, 0x2E4F4ED0, 0xC9C10C78, 0x029BAA67,
    0x6D352E05, 0x10E95C16, 0xB271E886, 0xE74A6DD6,
    0xFD77EAF3, 0xD1D77C3A, 0xB9A4D92B, 0x544C8F40,
#else   /* defined __LIT */
    0x509a3ba6, 0x1bf7e9ef, 0x2f1747dd, 0x7fe9596d,
    0xc34070e4, 0x37759d13, 0x6e6e9daa, 0x06ec0319,
    0x26a8fe49, 0x0faea2b1, 0x369ecd1d, 0xbb60e336,
    0x66ad5f0f, 0xd5214b89, 0xa8d4f712, 0x90b45825,
    0xc545609f, 0x699672f9, 0xe66af101, 0xbf023999,
    0x25e82643, 0x152a8a91, 0x6610489d, 0xed2303e6,
    0xf42d4318, 0x4dad8e60, 0x955825a2, 0x78513357,
    0x8c8c1005, 0x2eaa2841, 0x02cec081, 0x1232bbee,
    0x9ddc1535, 0xeea50971, 0x1b8130c4, 0xafd56aac,
    0x84df0400, 0x08060081, 0xe8b7df23, 0x2427f49a,
    0x9c28fb30, 0xe9db4589, 0xd524043a, 0x819b82e9,
    0x384ff73b, 0x866cf60f, 0xe1563677, 0x9e3a8bba,
    0x8b33e75d, 0x05970636, 0x1e488eb5, 0x8c0a605c,
    0xb759cdac, 0x749ee885, 0x472ebba3, 0xc98dc97b,
    0x913838ed, 0x736836dc, 0x1697fc45, 0x282827e3,
    0x81e2b747, 0x626d69a2, 0xdca03af7, 0x07ae19c9,
    0xd860c8ce, 0xf1eaea06, 0xd343e0bb, 0xe499c7e5,
    0xda332ad7, 0xc0535045, 0xa9f4eddb, 0x060a81cc,
    0x7bbd0116, 0xccb24531, 0xcfc9335e, 0xbc2c0903,
    0x3c9b4b60, 0xc02f12ef, 0x3f6eff38, 0xc5ba9ba1,
    0xedc915bf, 0x979f7e69, 0x30b2ce4f, 0xf2457155,
    0xcf872f63, 0x262977df, 0xf05baf3a, 0xbb802de7,
    0x38cdb373, 0xa3354c29, 0x0053a39d, 0xad20b717,
    0xf1ee282e, 0xbf3af166, 0xb888de0c, 0xd2aa4d95,
    0xcb28a24e, 0xf8ef593d, 0x3245aa66, 0xf75ca96b,
    0xf0333678, 0xeb6f32db, 0x3f19a5c1, 0x2f275769,
    0x82e5d726, 0xe394003c, 0x1bf1b154, 0x2d03c881,
    0x5389a9f3, 0xb8bbda0e, 0x89f3df7c, 0x0f327510,
    0x4b83102e, 0xd40c737e, 0x26170c44, 0xe044c6e7,
    0x7cf35504, 0xd04e4f2e, 0x780cc1c9, 0x67aa9b02,
    0x052e356d, 0x165ce910, 0x86e871b2, 0xd66d4ae7,
    0xf3ea77fd, 0x3a7cd7d1, 0x2bd9a4b9, 0x408f4c54,
#endif  /* defined __LIT */
};
TSIP_SEC_DEFAULT
/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

