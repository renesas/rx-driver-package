
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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_rsip_data.c
 * Version      : 1.00
 * Description  : DomainParam data information file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 15.10.2024 1.00     First Release.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "./r_rsip_primitive.h"
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
 
uint32_t const DomainParam_NIST_P256[] =
{
#if defined __LIT
    0x43a40d8f,0x02bf4c91,0xc8973930,0x18af9a96,
    0x2fa5dcb0,0x2245a0c2,0x9acf0494,0x460285de,
    0x3d8627e0,0x519766e1,0x640bb205,0x8fc58f20,
    0x402c659a,0xd8c94a79,0x46f732a8,0x0c366f2e,
    0xf1ad5de7,0x8eec3757,0x4cfea7f9,0xd82b9335,
    0x4943b401,0x23775f07,0x51177157,0xf8f067b9,
    0x05cc6b12,0xfc1cce73,0xc3f1474f,0xa0a42279,
    0xd2b25544,0x2ee49d7f,0x6a751e6b,0xdcea085e,
    0xbf03b1d2,0xc95f336c,0x9518fb1a,0x353973c4,
    0x7fbab04c,0x1a19a8f5,0xed6b119f,0x680f876c,
    0x42cec82a,0x0ce55db6,0x9f4074b8,0x7ce51682,
    0xa4a1c014,0x734ff812,0x87e79543,0xf3af2f3c,
    0x17348448,0x648c1ed1,0x2a69f730,0xc08227d9,
    0x88635681,0xead248b7,0x80eac3c3,0x2a8fba57,
    0x8b82657d,0x18f62f4a,0xfa5f7e7f,0x59458e4f,
    0xd879750b,0x90f7befb,0x4ae8808e,0x18b6c17b,
    0x83f62c9a,0xbbe15e9f,0x177bbe06,0x6ad1a370,
    0x79a9077b,0x211b7ec9,0xc73e0eee,0xfc841fdf,
#else   /* defined __LIT */
    0x8f0da443,0x914cbf02,0x303997c8,0x969aaf18,
    0xb0dca52f,0xc2a04522,0x9404cf9a,0xde850246,
    0xe027863d,0xe1669751,0x05b20b64,0x208fc58f,
    0x9a652c40,0x794ac9d8,0xa832f746,0x2e6f360c,
    0xe75dadf1,0x5737ec8e,0xf9a7fe4c,0x35932bd8,
    0x01b44349,0x075f7723,0x57711751,0xb967f0f8,
    0x126bcc05,0x73ce1cfc,0x4f47f1c3,0x7922a4a0,
    0x4455b2d2,0x7f9de42e,0x6b1e756a,0x5e08eadc,
    0xd2b103bf,0x6c335fc9,0x1afb1895,0xc4733935,
    0x4cb0ba7f,0xf5a8191a,0x9f116bed,0x6c870f68,
    0x2ac8ce42,0xb65de50c,0xb874409f,0x8216e57c,
    0x14c0a1a4,0x12f84f73,0x4395e787,0x3c2faff3,
    0x48843417,0xd11e8c64,0x30f7692a,0xd92782c0,
    0x81566388,0xb748d2ea,0xc3c3ea80,0x57ba8f2a,
    0x7d65828b,0x4a2ff618,0x7f7e5ffa,0x4f8e4559,
    0x0b7579d8,0xfbbef790,0x8e80e84a,0x7bc1b618,
    0x9a2cf683,0x9f5ee1bb,0x06be7b17,0x70a3d16a,
    0x7b07a979,0xc97e1b21,0xee0e3ec7,0xdf1f84fc,
#endif  /* defined __LIT */
};

uint32_t const DomainParam_Brainpool_256r1[] =
{
#if defined __LIT
    0x0849c837,0x49aa1d22,0xb544a674,0x29943e97,
    0xeead3fbd,0xd3822976,0x289d9b6c,0xf6141364,
    0xc614bb21,0x5ee11bf3,0x0669709b,0xc0410021,
    0x27e326d8,0x70d318b1,0xd0f97570,0x02768332,
    0x17a963e9,0x63d87577,0x12ec4f66,0xfe664f2f,
    0x9f7c8327,0xcbb8b8a7,0xe37c0f66,0x37344fb5,
    0xbc8a77ea,0x81630d26,0x8e937e8e,0x1082abe8,
    0x247b578a,0x3986424b,0x4c7c92af,0xf4f0e666,
    0x3ae66e58,0x6218816d,0xf749acc4,0xeabf07a3,
    0x8f60ab1f,0xa479e014,0xf67c1996,0x58eb2be9,
    0x0518ddf8,0x4b80ecb6,0xe0014ae2,0x4a550803,
    0xe0eb407a,0xf15e2d88,0x2601c9f5,0xa2c5765c,
    0x93dfc30e,0x8728df7a,0xd99dfaeb,0xd9cc03cd,
    0xe59ef267,0x6d066f85,0x1cf70dc4,0xc22fed75,
    0xb09339e1,0x4d496ac8,0xb8dbda09,0x7af353c7,
    0x7592c46d,0xff88db50,0xfc58bce2,0x817340f3,
    0x37025916,0xd8b72ccf,0x140f498b,0x1f720542,
    0x6df5d660,0x00de7195,0x2e52bc5d,0xfed27004,
#else   /* defined __LIT */
    0x37c84908,0x221daa49,0x74a644b5,0x973e9429,
    0xbd3fadee,0x762982d3,0x6c9b9d28,0x641314f6,
    0x21bb14c6,0xf31be15e,0x9b706906,0x210041c0,
    0xd826e327,0xb118d370,0x7075f9d0,0x32837602,
    0xe963a917,0x7775d863,0x664fec12,0x2f4f66fe,
    0x27837c9f,0xa7b8b8cb,0x660f7ce3,0xb54f3437,
    0xea778abc,0x260d6381,0x8e7e938e,0xe8ab8210,
    0x8a577b24,0x4b428639,0xaf927c4c,0x66e6f0f4,
    0x586ee63a,0x6d811862,0xc4ac49f7,0xa307bfea,
    0x1fab608f,0x14e079a4,0x96197cf6,0xe92beb58,
    0xf8dd1805,0xb6ec804b,0xe24a01e0,0x0308554a,
    0x7a40ebe0,0x882d5ef1,0xf5c90126,0x5c76c5a2,
    0x0ec3df93,0x7adf2887,0xebfa9dd9,0xcd03ccd9,
    0x67f29ee5,0x856f066d,0xc40df71c,0x75ed2fc2,
    0xe13993b0,0xc86a494d,0x09dadbb8,0xc753f37a,
    0x6dc49275,0x50db88ff,0xe2bc58fc,0xf3407381,
    0x16590237,0xcf2cb7d8,0x8b490f14,0x4205721f,
    0x60d6f56d,0x9571de00,0x5dbc522e,0x0470d2fe,
#endif  /* defined __LIT */
};

uint32_t const DomainParam_Koblitz_secp256k1[] =
{
#if defined __LIT
    0x29331a16,0xaf85d879,0xa18f1a93,0x4d76ba1a,
    0x80888d61,0x1c92c4e3,0xa84a4992,0x84d4c663,
    0x1146e6ad,0x0e98d613,0x03b909f8,0x1243c845,
    0x1a375cf7,0x3d8ce31c,0xe981d77b,0x02ba355f,
    0x0110c12e,0xb6a4d7b0,0x1888e9c5,0x1c1d0b76,
    0x86471b43,0xf662fec9,0xd3a73f6b,0xb1435b97,
    0x05e002f3,0x1cc8c16e,0x0c7866d6,0x2c9b8b8a,
    0xf2cbbd8f,0xe4436ac8,0x2b3e2bf1,0xd96d1a2d,
    0x51fd7be2,0x2c0e3c7f,0xb01f62d4,0xedefd14f,
    0x3f21d2c4,0xaa347768,0x206cfdab,0xca5c0b73,
    0xbb09c45f,0x69a0f597,0xa29bda2f,0x87378437,
    0xe9fe3677,0xed96ae71,0xf0c65d90,0x3cbc3488,
    0x113b1495,0xc1a1abfb,0x870e1fff,0x430ddd4e,
    0x974177d2,0x5b5a8b2d,0xb18c983f,0x6ad17208,
    0x3785fd79,0x39de2455,0x39b306cb,0x8420bd12,
    0x733c39aa,0xbb62f45a,0xa9fd1fa7,0x4dc1c301,
    0x2141eace,0x8ce80e6b,0x56461588,0x1fae639e,
    0xba640567,0x823d0ff9,0xc146a793,0x48b7ee0d,
#else   /* defined __LIT */
    0x161a3329,0x79d885af,0x931a8fa1,0x1aba764d,
    0x618d8880,0xe3c4921c,0x92494aa8,0x63c6d484,
    0xade64611,0x13d6980e,0xf809b903,0x45c84312,
    0xf75c371a,0x1ce38c3d,0x7bd781e9,0x5f35ba02,
    0x2ec11001,0xb0d7a4b6,0xc5e98818,0x760b1d1c,
    0x431b4786,0xc9fe62f6,0x6b3fa7d3,0x975b43b1,
    0xf302e005,0x6ec1c81c,0xd666780c,0x8a8b9b2c,
    0x8fbdcbf2,0xc86a43e4,0xf12b3e2b,0x2d1a6dd9,
    0xe27bfd51,0x7f3c0e2c,0xd4621fb0,0x4fd1efed,
    0xc4d2213f,0x687734aa,0xabfd6c20,0x730b5cca,
    0x5fc409bb,0x97f5a069,0x2fda9ba2,0x37843787,
    0x7736fee9,0x71ae96ed,0x905dc6f0,0x8834bc3c,
    0x95143b11,0xfbaba1c1,0xff1f0e87,0x4edd0d43,
    0xd2774197,0x2d8b5a5b,0x3f988cb1,0x0872d16a,
    0x79fd8537,0x5524de39,0xcb06b339,0x12bd2084,
    0xaa393c73,0x5af462bb,0xa71ffda9,0x01c3c14d,
    0xceea4121,0x6b0ee88c,0x88154656,0x9e63ae1f,
    0x670564ba,0xf90f3d82,0x93a746c1,0x0deeb748,
#endif  /* defined __LIT */
};

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
