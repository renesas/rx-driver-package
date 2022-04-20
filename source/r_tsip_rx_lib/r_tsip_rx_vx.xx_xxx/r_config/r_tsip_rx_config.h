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
 * Copyright (C) 2015-2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_rx_config.h
 * Version      : 1.11
 * Description  : Configuration options for the r_tsip_rx module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.06.2015 1.00     First Release
 *         : 15.05.2017 1.01     Add AES-CMAC
 *         : 30.09.2017 1.03     Add SHA, RSA
 *         : 28.02.2018 1.04     Add TLS functions, Change: RX231 Init/Update/Final API ,return value
 *         : 30.04.2018 1.05     Add MD5, Triple-DES and RSAES-PKCS1 functions
 *         : 28.09.2018 1.06     Add RSA Key Generation, AES, TDES, RSA Key update features, RX66T support
 *         : 30.09.2019 1.08     Added support for GCC and IAR compiler, ECC API, RX23W and RX72M
 *         : 31.03.2020 1.09     Added support for AES-CCM, HMAC key generation, ECDH, Key wrap API, RX66N and RX72N
 *         : 30.06.2020 1.10     Added support for ARC4, ECC(P-384) API
 *         : 30.09.2020 1.11     Added support for DH, ECDHE P-512r1 API, and generalization of KDF.
 *         :                     Added support for Key wrap API with TSIP-Lite.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#ifndef R_TSIP_RX_CONFIG_HEADER_FILE
#define R_TSIP_RX_CONFIG_HEADER_FILE

/* For AES operation. */
#define TSIP_AES_128_ECB_ENCRYPT    (1)
#define TSIP_AES_256_ECB_ENCRYPT    (1)
#define TSIP_AES_128_ECB_DECRYPT    (1)
#define TSIP_AES_256_ECB_DECRYPT    (1)
#define TSIP_AES_128_CBC_ENCRYPT    (1)
#define TSIP_AES_256_CBC_ENCRYPT    (1)
#define TSIP_AES_128_CBC_DECRYPT    (1)
#define TSIP_AES_256_CBC_DECRYPT    (1)
#define TSIP_AES_128_GCM_ENCRYPT    (1)
#define TSIP_AES_256_GCM_ENCRYPT    (1)
#define TSIP_AES_128_GCM_DECRYPT    (1)
#define TSIP_AES_256_GCM_DECRYPT    (1)
#define TSIP_AES_128_CMAC           (1)
#define TSIP_AES_256_CMAC           (1)
#define TSIP_AES_128_CCM_ENCRYPT    (1)
#define TSIP_AES_256_CCM_ENCRYPT    (1)
#define TSIP_AES_128_CCM_DECRYPT    (1)
#define TSIP_AES_256_CCM_DECRYPT    (1)
#define TSIP_AES_128_KEY_WRAP       (1)
#define TSIP_AES_256_KEY_WRAP       (1)

/* For TDES operation. */
#define TSIP_TDES_ECB_ENCRYPT   (1)
#define TSIP_TDES_ECB_DECRYPT   (1)
#define TSIP_TDES_CBC_ENCRYPT   (1)
#define TSIP_TDES_CBC_DECRYPT   (1)

/* For ARC4 operation. */
#define TSIP_ARC4_ENCRYPT   (1)
#define TSIP_ARC4_DECRYPT   (1)

/* For HASH operation. */
#define TSIP_SHA_1      (1)
#define TSIP_SHA_256    (1)
#define TSIP_MD5        (1)

/* For HMAC operation. */
#define TSIP_SHA_1_HMAC     (1)
#define TSIP_SHA_256_HMAC   (1)

/* For RSA operation. */
#define TSIP_RSAES_1024         (1)
#define TSIP_RSAES_2048         (1)
#define TSIP_RSASSA_1024        (1)
#define TSIP_RSASSA_2048        (1)
    /* 5*(key_length/2) is recommended by NIST FIPS186-4, 5120 is for key_length = 2048. *2 means the margin. */
#define TSIP_RSA_RETRY_COUNT_FOR_RSA_KEY_GENERATION (5120*2)

/* For ECC operation. */
#define TSIP_ECDSA_P192 (1)
#define TSIP_ECDSA_P224 (1)
#define TSIP_ECDSA_P256 (1)
#define TSIP_ECDSA_P384 (1)
#define TSIP_ECDH_P256  (1)
#define TSIP_USER_SHA_384_ENABLED  (0)
#define TSIP_USER_SHA_384_FUNCTION user_sha384_function

/* For TLS. */
#define TSIP_TLS    (1)

/* Firmware update. */
#define TSIP_SECURE_BOOT        (0)
#define TSIP_FIRMWARE_UPDATE    (1)

/* Key update. */
#if defined BSP_MCU_RX231 || defined BSP_MCU_RX23W || defined BSP_MCU_RX66T || defined BSP_MCU_RX72T
#define TSIP_INSTALL_KEY_RING_INDEX (0) /* 0-15 */
#else
#define TSIP_INSTALL_KEY_RING_INDEX (1) /* 0-15 */
#endif  /* defined BSP_MCU_RX231 || defined BSP_MCU_RX23W || defined BSP_MCU_RX66T || defined BSP_MCU_RX72T */

#endif /* R_TSIP_RX_CONFIG_HEADER_FILE */
