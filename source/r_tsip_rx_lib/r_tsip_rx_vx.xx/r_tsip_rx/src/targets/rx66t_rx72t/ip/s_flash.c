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
    0x3e90309a, 0x93167f57, 0x943d016c, 0x3a591269,
    0xeb2b2a13, 0x48faf645, 0x7c766691, 0x37b697fb,
    0x30024f99, 0x0ad9ad6b, 0x890bfe6a, 0x768b881e,
    0x305dce15, 0x7456fced, 0xe4bc73d1, 0xc10ab694,
    0xd654562a, 0x8549b8b0, 0x03425d94, 0x7cd44d67,
    0x3a4a09a5, 0x1d4b3130, 0x888c0ce1, 0x99b69997,
    0xf6d08a44, 0x16de1c11, 0xd078f479, 0x03f90507,
    0xb8fd136e, 0x0fddeca2, 0x91f634bd, 0xccef81ad,
    0x547a199c, 0x00ea10b2, 0x2c04cfdc, 0x6dc62f61,
    0xdccdaf60, 0x94fcf6cb, 0xd85058ad, 0x304de5f9,
    0x1187668e, 0x306a9a66, 0xe1cd2c72, 0x50b45c9c,
    0xff2e9bb8, 0xfbf28e7b, 0x6bf68cc8, 0x0348d55c,
    0xc40ad45b, 0x48b36911, 0x552fb649, 0xef810e91,
    0xc71356ca, 0x0b262a0b, 0x5d129917, 0x9d049b8e,
    0x83cf1f64, 0x2e47c56d, 0x7dfee3a5, 0xb7a5e63f,
    0xa578bd65, 0x82c6bb8a, 0x1429a4a4, 0x345c700e,
    0x05ebdf41, 0x5086e550, 0xeaaa0b09, 0xa1857a42,
    0x04b62c3c, 0x0218f6a4, 0x9d035b99, 0xe68569f6,
    0x87096b3b, 0xe42e3944, 0x4d58949d, 0xd1000bd1,
    0x4f234d9c, 0xd81e7ba6, 0xec1d7339, 0x45d58bc5,
    0x418fe300, 0x36f3777b, 0xec91fa1e, 0x9ccbf072,
    0x472a1d41, 0x552003cb, 0x8c6f449b, 0x6592bd5d,
    0x9111dac5, 0xb3dbb513, 0xb7530840, 0x32141733,
    0x5edc2956, 0x8797a708, 0x7cefc558, 0xc62ed304,
    0x6f57d7a1, 0x4aba3398, 0x0814d738, 0x16c438c0,
    0xca2486e8, 0x8de8c1bd, 0xf970d371, 0x8382d124,
    0xfc5e0c1e, 0x3bd684b8, 0x3793f175, 0x10431d48,
    0xc4ef0b51, 0xe398b298, 0xba1c1785, 0x8a531f37,
    0xdeab14bc, 0xd28b1199, 0xa18f5601, 0x53e223df,
    0xf2021a5a, 0x29745541, 0x0318e5aa, 0xbcf4541d,
    0xb74c6ed7, 0x7fb03e7d, 0x65d3dc70, 0x7e5a0b69,
    0x973918aa, 0xd0508051, 0x29b5e02f, 0x479f48c5,
#else   /* defined __LIT */
    0x9a30903e, 0x577f1693, 0x6c013d94, 0x6912593a,
    0x132a2beb, 0x45f6fa48, 0x9166767c, 0xfb97b637,
    0x994f0230, 0x6badd90a, 0x6afe0b89, 0x1e888b76,
    0x15ce5d30, 0xedfc5674, 0xd173bce4, 0x94b60ac1,
    0x2a5654d6, 0xb0b84985, 0x945d4203, 0x674dd47c,
    0xa5094a3a, 0x30314b1d, 0xe10c8c88, 0x9799b699,
    0x448ad0f6, 0x111cde16, 0x79f478d0, 0x0705f903,
    0x6e13fdb8, 0xa2ecdd0f, 0xbd34f691, 0xad81efcc,
    0x9c197a54, 0xb210ea00, 0xdccf042c, 0x612fc66d,
    0x60afcddc, 0xcbf6fc94, 0xad5850d8, 0xf9e54d30,
    0x8e668711, 0x669a6a30, 0x722ccde1, 0x9c5cb450,
    0xb89b2eff, 0x7b8ef2fb, 0xc88cf66b, 0x5cd54803,
    0x5bd40ac4, 0x1169b348, 0x49b62f55, 0x910e81ef,
    0xca5613c7, 0x0b2a260b, 0x1799125d, 0x8e9b049d,
    0x641fcf83, 0x6dc5472e, 0xa5e3fe7d, 0x3fe6a5b7,
    0x65bd78a5, 0x8abbc682, 0xa4a42914, 0x0e705c34,
    0x41dfeb05, 0x50e58650, 0x090baaea, 0x427a85a1,
    0x3c2cb604, 0xa4f61802, 0x995b039d, 0xf66985e6,
    0x3b6b0987, 0x44392ee4, 0x9d94584d, 0xd10b00d1,
    0x9c4d234f, 0xa67b1ed8, 0x39731dec, 0xc58bd545,
    0x00e38f41, 0x7b77f336, 0x1efa91ec, 0x72f0cb9c,
    0x411d2a47, 0xcb032055, 0x9b446f8c, 0x5dbd9265,
    0xc5da1191, 0x13b5dbb3, 0x400853b7, 0x33171432,
    0x5629dc5e, 0x08a79787, 0x58c5ef7c, 0x04d32ec6,
    0xa1d7576f, 0x9833ba4a, 0x38d71408, 0xc038c416,
    0xe88624ca, 0xbdc1e88d, 0x71d370f9, 0x24d18283,
    0x1e0c5efc, 0xb884d63b, 0x75f19337, 0x481d4310,
    0x510befc4, 0x98b298e3, 0x85171cba, 0x371f538a,
    0xbc14abde, 0x99118bd2, 0x01568fa1, 0xdf23e253,
    0x5a1a02f2, 0x41557429, 0xaae51803, 0x1d54f4bc,
    0xd76e4cb7, 0x7d3eb07f, 0x70dcd365, 0x690b5a7e,
    0xaa183997, 0x518050d0, 0x2fe0b529, 0xc5489f47,
#endif  /* defined __LIT */
};
TSIP_SEC_DEFAULT
/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

