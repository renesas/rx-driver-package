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
 * Copyright (C) 2015-2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_rx_if.h
 * Version      : 1.17
 * Description  : Interface definition for the r_tsip_rx module.
 *                TSIP means the "Trusted Secure IP" that is Renesas original security IP.
 *                Strong point 1:
 *                 TSIP can hide the "Critical Security Parameter (CSP)" inside the "Cryptographic Boundary".
 *                 These words are defined in NIST FIPS140-2.
 *                Strong point 2:
 *                 TSIP can support AES, SHA, DES, RSA, ECC, TRNG(with DRBG), already certified NIST CAVP test.
 *                 TSIP-Lite can support AES, TRNG(with DRBG), already certified NIST CAVP test.
 *                Strong point 3:
 *                 TSIP can accelerate some crypto operation.
 *                Supported Device:
 *                 TSIP = RX651, RX65N, RX66N, RX671, RX72M, RX72N
 *                 TSIP-Lite = RX231, RX23W, RX66T, RX72T
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.06.2015 1.00     First Release
 *         : 15.05.2017 1.01     Add AES-CMAC
 *         : 30.09.2017 1.03     Add Init/Update/Final API and SHA, RSA
 *         : 28.02.2018 1.04     Change Init/Update/Final API for RX231, add TLS function and 
 *         :                     return values change FIT rules.
 *         : 30.04.2018 1.05     Add TDES, MD5 and RSAES-PKCS1-v1_5 API
 *         : 28.09.2018 1.06     Add RSA Key Generation, AES, TDES, RSA Key update features, RX66T support
 *         : 28.12.2018 1.07     Add RX72T support
 *         : 30.09.2019 1.08     Added support for GCC and IAR compiler, ECC API, RX23W and RX72M
 *         : 31.03.2020 1.09     Added support for AES-CCM, HMAC key generation, ECDH, Key Wrap API, RX66N and RX72N
 *         : 30.06.2020 1.10     Added support for ARC4, ECC(P-384) API
 *         : 30.09.2020 1.11     Added support for DH, ECDHE P-512r1 API, and generalization of KDF.
 *         :                     Added support for Key wrap API with TSIP-Lite.
 *         : 30.06.2021 1.12     Added support for RX23W chip version D
 *         : 31.08.2021 1.13     Added support for RX671
 *         : 22.10.2021 1.14     Added support for TLS1.3
 *         : 31.03.2022 1.15     Added support for TLS1.3(RX72M_RX72N_RX66N)
 *         : 15.09.2022 1.16     Added support for RSA 3k/4k and updated support for TLS1.3
 *         : 20.01.2023 1.17     Added support for TLS1.3 server
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "platform.h"
#include "r_tsip_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#ifndef R_TSIP_RX_HEADER_FILE
#define R_TSIP_RX_HEADER_FILE

#if R_BSP_VERSION_MAJOR < 5
#error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif
#if (defined BSP_MCU_RX231 || defined BSP_MCU_RX23W) && (BSP_CFG_MCU_PART_VERSION == 0xB)  /* B */
#elif defined BSP_MCU_RX23W && BSP_CFG_MCU_PART_VERSION == 0xD  /* D */
#elif (defined BSP_MCU_RX66T || defined BSP_MCU_RX72T) && ((BSP_CFG_MCU_PART_FUNCTION == 0xE /* E */) || \
    (BSP_CFG_MCU_PART_FUNCTION == 0xF /* F */) || (BSP_CFG_MCU_PART_FUNCTION == 0x10 /* G */))
#elif (defined BSP_MCU_RX65N || defined BSP_MCU_RX651) && (BSP_CFG_MCU_PART_ENCRYPTION_INCLUDED == true)
#elif (defined BSP_MCU_RX72M || defined BSP_MCU_RX72N || defined BSP_MCU_RX66N || defined BSP_MCU_RX671) && \
    (BSP_CFG_MCU_PART_FUNCTION == 0x11 /* H */)
#else
#error "Your MCU does not support TSIP functions. Please confirm BSP_MCU_xxx macro in r_bsp_config.h."
#endif  /* defined BSP_MCU_RX231 || defined BSP_MCU_RX23W && BSP_CFG_MCU_PART_VERSION == 0xB */

/* Version Number of API. */
#define TSIP_VERSION_MAJOR    (1u)
#define TSIP_VERSION_MINOR    (17u)

/* Various information. */
#define R_TSIP_SRAM_WORD_SIZE   (20u)
#define R_TSIP_SINST_WORD_SIZE  (140u)
#define R_TSIP_SINST2_WORD_SIZE (16u)
#define R_TSIP_SHEAP_WORD_SIZE  (1632u)

/* For AES operation. */
#define R_TSIP_AES128_KEY_INDEX_WORD_SIZE     (12u)
#define R_TSIP_AES256_KEY_INDEX_WORD_SIZE     (16u)
#define R_TSIP_AES128_KEY_WORD_SIZE           (4u)
#define R_TSIP_AES256_KEY_WORD_SIZE           (8u)
#define R_TSIP_AES128_KEY_BYTE_SIZE           (16u)
#define R_TSIP_AES256_KEY_BYTE_SIZE           (32u)
#define R_TSIP_AES_BLOCK_BYTE_SIZE            (16u)
#define R_TSIP_AES_CBC_IV_BYTE_SIZE           (16u)
#define R_TSIP_AES_GCM_AAD_BLOCK_BYTE_SIZE    (16u)
#define R_TSIP_AES_CCM_B_FORMAT_BYTE_SIZE     (128u)
#define R_TSIP_AES_CCM_COUNTER_BYTE_SIZE      (16u)
#define R_TSIP_KEYWRAP_AES128                 (0u)
#define R_TSIP_KEYWRAP_AES256                 (2u)

/* For TDES operation. */
#define R_TSIP_TDES_KEY_INDEX_WORD_SIZE       (16u)
#define R_TSIP_TDES_KEY_WORD_SIZE             (8u)
#define R_TSIP_TDES_KEY_BYTE_SIZE             (32u)
#define R_TSIP_TDES_BLOCK_BYTE_SIZE           (8u)
#define R_TSIP_TDES_CBC_IV_BYTE_SIZE          (8u)

/* For ARC4 operation. */
#define R_TSIP_ARC4_KEY_INDEX_WORD_SIZE       (72u)
#define R_TSIP_ARC4_KEY_WORD_SIZE             (64u)
#define R_TSIP_ARC4_KEY_BYTE_SIZE             (256u)
#define R_TSIP_ARC4_BLOCK_BYTE_SIZE           (16u)

/* For SHA operation. */
#define R_TSIP_SHA1_HASH_LENGTH_BYTE_SIZE      (20u)
#define R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE    (32u)
#define R_TSIP_SHA384_HASH_LENGTH_BYTE_SIZE    (48u)

/* For MD5 operation. */
#define R_TSIP_MD5_HASH_LENGTH_BYTE_SIZE       (16u)

/* For HMAC operation. */
#define R_TSIP_HMAC_KEY_INDEX_BYTE_SIZE        (64u)
#define R_TSIP_HMAC_KEY_INDEX_WORD_SIZE        (16u)
#define R_TSIP_HMAC_KEY_BYTE_SIZE              (32u)

/* For RSA operation. */
#define R_TSIP_RSA_1024_KEY_N_LENGTH_BYTE_SIZE                  (128u)
#define R_TSIP_RSA_1024_KEY_E_LENGTH_BYTE_SIZE                  (4u)
#define R_TSIP_RSA_1024_KEY_D_LENGTH_BYTE_SIZE                  (128u)
#define R_TSIP_RSA_2048_KEY_N_LENGTH_BYTE_SIZE                  (256u)
#define R_TSIP_RSA_2048_KEY_E_LENGTH_BYTE_SIZE                  (4u)
#define R_TSIP_RSA_2048_KEY_D_LENGTH_BYTE_SIZE                  (256u)
#define R_TSIP_RSA_3072_KEY_N_LENGTH_BYTE_SIZE                  (384u)
#define R_TSIP_RSA_3072_KEY_E_LENGTH_BYTE_SIZE                  (4u)
#define R_TSIP_RSA_4096_KEY_N_LENGTH_BYTE_SIZE                  (512u)
#define R_TSIP_RSA_4096_KEY_E_LENGTH_BYTE_SIZE                  (4u)
#define R_TSIP_RSA_1024_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE   (4u)
#define R_TSIP_RSA_1024_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE   (36u)
#define R_TSIP_RSA_1024_PRIVATE_KEY_MANAGEMENT_INFO_WORD_SIZE   (104u)
#define R_TSIP_RSA_2048_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE   (4u)
#define R_TSIP_RSA_2048_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE   (68u)
#define R_TSIP_RSA_2048_PRIVATE_KEY_MANAGEMENT_INFO_WORD_SIZE   (200u)
#define R_TSIP_RSA_3072_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE   (4u)
#define R_TSIP_RSA_3072_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE   (4u)
#define R_TSIP_RSA_4096_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE   (4u)
#define R_TSIP_RSA_4096_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE   (4u)
#define R_TSIP_RSA_KEY_GENERATION_DUMMY_BYTE_SIZE               (12u)
#define R_TSIP_RSA1024_NE_KEY_BYTE_SIZE                         (144u)
#define R_TSIP_RSA1024_ND_KEY_BYTE_SIZE                         (256u)
#define R_TSIP_RSA2048_NE_KEY_BYTE_SIZE                         (272u)
#define R_TSIP_RSA2048_ND_KEY_BYTE_SIZE                         (512u)
#define R_TSIP_RSA3072_NE_KEY_BYTE_SIZE                         (400u)
#define R_TSIP_RSA4096_NE_KEY_BYTE_SIZE                         (528u)
#define R_TSIP_RSA1024_NE_KEY_INDEX_WORD_SIZE                   (73u)
#define R_TSIP_RSA1024_ND_KEY_INDEX_WORD_SIZE                   (101u)
#define R_TSIP_RSA2048_NE_KEY_INDEX_WORD_SIZE                   (137u)
#define R_TSIP_RSA2048_ND_KEY_INDEX_WORD_SIZE                   (197u)
#define R_TSIP_RSA1024_RANDOM_PUBLIC_KEY_INDEX_WORD_SIZE        (76u)
#define R_TSIP_RSA1024_RANDOM_PRIVATE_KEY_INDEX_WORD_SIZE       (104u)
#define R_TSIP_RSA2048_RANDOM_PUBLIC_KEY_INDEX_WORD_SIZE        (140u)
#define R_TSIP_RSA2048_RANDOM_PRIVATE_KEY_INDEX_WORD_SIZE       (200u)
#define R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE               (11u)
#define R_TSIP_RSA_1024_DATA_BYTE_SIZE                          (128u)
#define R_TSIP_RSA_2048_DATA_BYTE_SIZE                          (256u)
#define R_TSIP_RSA_3072_DATA_BYTE_SIZE                          (384u)
#define R_TSIP_RSA_4096_DATA_BYTE_SIZE                          (512u)

/* RSA HASH type. */
#define R_TSIP_RSA_HASH_MD5                                     (0x00)  /* MD5     */
#define R_TSIP_RSA_HASH_SHA1                                    (0x01)  /* SHA-1   */
#define R_TSIP_RSA_HASH_SHA256                                  (0x02)  /* SHA-256 */

/* For ECC operation. */
#define R_TSIP_ECC_KEY_LENGTH_BYTE_SIZE                     (112u)
#define R_TSIP_ECC_PUBLIC_KEY_MANAGEMENT_INFO_WORD_SIZE     (4u)
#define R_TSIP_ECC_PRIVATE_KEY_MANAGEMENT_INFO_WORD_SIZE    (20u)
#define R_TSIP_ECC_PUBLIC_KEY_BYTE_SIZE                     (64u)
#define R_TSIP_ECC_P384_PUBLIC_KEY_BYTE_SIZE                (96u)
#define R_TSIP_ECC_PRIVATE_KEY_BYTE_SIZE                    (32u)
#define R_TSIP_ECC_P384_PRIVATE_KEY_BYTE_SIZE               (48u)
#define R_TSIP_ECDSA_DATA_BYTE_SIZE                         (64u)
#define R_TSIP_ECDSA_P384_DATA_BYTE_SIZE                    (96u)
#define R_TSIP_SHARED_SECRET_KEY_INDEX_WORD_SIZE            (16u)

/* For TLS. */
#define R_TSIP_TLS_RSA_NE_KEY_BYTE_SIZE                     (272u)
#define R_TSIP_TLS_RSA_NE_KEY_INDEX_WORD_SIZE               (140u)
#define R_TSIP_TLS_ROOT_PUBLIC_KEY_WORD_SIZE                (140u)
#define R_TSIP_TLS_P256_ECC_KEY_WORD_SIZE                   (16u)
#define R_TSIP_TLS_EPHEMERAL_ECDH_PUBLIC_KEY_WORD_SIZE      (16u)
#define R_TSIP_TLS_MASTER_SECRET_WORD_SIZE                  (20u)
#define R_TSIP_TLS_GENERATE_MAC_KEY_WORD_SIZE               (16u)
#define R_TSIP_TLS_GENERATE_AES128_CRYPTO_KEY_WORD_SIZE     (12u)
#define R_TSIP_TLS_GENERATE_AES256_CRYPTO_KEY_WORD_SIZE     (16u)
#define R_TSIP_TLS_GENERATE_VERIFY_DATA_BYTE_SIZE           (12u)
#define R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA                 (0u)
#define R_TSIP_TLS_RSA_WITH_AES_256_CBC_SHA                 (1u)
#define R_TSIP_TLS_RSA_WITH_AES_128_CBC_SHA256              (2u)
#define R_TSIP_TLS_RSA_WITH_AES_256_CBC_SHA256              (3u)
#define R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256      (4u)
#define R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256        (5u)
#define R_TSIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256      (6u)
#define R_TSIP_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256        (7u)
#define R_TSIP_TLS_GENERATE_CLIENT_VERIFY                   (0u)
#define R_TSIP_TLS_GENERATE_SERVER_VERIFY                   (1u)
#define R_TSIP_TLS_PUBLIC_KEY_TYPE_RSA2048                  (0u)
#define R_TSIP_TLS_PUBLIC_KEY_TYPE_RSA4096                  (1u)
#define R_TSIP_TLS_PUBLIC_KEY_TYPE_ECDSA_P256               (2u)

/* TLS-HMAC. */
#define R_TSIP_TLS_HMAC_KEY_INDEX_BYTE_SIZE                 (64u)
#define R_TSIP_TLS_HMAC_KEY_INDEX_WORD_SIZE                 (16u)

/* TLS-AES. */
#define R_TSIP_TLS_AES128_KEY_INDEX_WORD_SIZE               (12u)
#define R_TSIP_TLS_AES256_KEY_INDEX_WORD_SIZE               (16u)

/* TLS1.3 */
#define R_TSIP_TLS13_SHARED_SECRET_KEY_WORD_SIZE            (16u)
#define R_TSIP_TLS13_HANDSHAKE_SECRET_KEY_WORD_SIZE         (16u)
#define R_TSIP_TLS13_FINISHED_KEY_WORD_SIZE                 (16u)
#define R_TSIP_TLS13_MASTER_SECRET_KEY_WORD_SIZE            (12u)
#define R_TSIP_TLS13_APP_SECRET_KEY_WORD_SIZE               (12u)
#define R_TSIP_TLS13_PRE_SHARED_KEY_WORD_SIZE               (12u)
#define R_TSIP_TLS13_BINDER_KEY_WORD_SIZE                   (16u)

/* TLS1.3 sequence number management */
#define R_TSIP_TLS13_INCREMENT_SEQNUM   (0)
#define R_TSIP_TLS13_RESET_SEQNUM       (1)

/* Key update. */
#define R_TSIP_UPDATE_KEY_RING_INDEX_WORD_SIZE  (16u)

/* Firmware update. */
#define R_TSIP_FIRMWARE_MAC_BYTE_SIZE           (16u)
#if defined BSP_MCU_RX231 || defined BSP_MCU_RX23W
#define R_TSIP_SECURE_BOOT_AREA_TOP             (0xFFFF8000)
#else
#define R_TSIP_SECURE_BOOT_AREA_TOP             (0xFFFF0000)
#endif  /* defined BSP_MCU_RX231 || defined BSP_MCU_RX23W */

/* Secure boot section. */
#if TSIP_SECURE_BOOT != 0
/* Required for each variable definition with no initial value to be placed in the SECURE_BOOT section. */
#define TSIP_SEC_B_SECURE_BOOT  R_BSP_ATTRIB_SECTION_CHANGE(B, SECURE_BOOT, 4)
/* Required for each constant definition to be placed in the SECURE_BOOT section. */
#define TSIP_SEC_C_SECURE_BOOT  R_BSP_ATTRIB_SECTION_CHANGE(C, SECURE_BOOT, 4)
/* Required for each variable definition with initial value to be placed in the SECURE_BOOT section. */
#define TSIP_SEC_D_SECURE_BOOT  R_BSP_ATTRIB_SECTION_CHANGE(D, SECURE_BOOT, 4)
/* Required for each function definition to be placed in the SECURE_BOOT section. */
#define TSIP_SEC_P_SECURE_BOOT  R_BSP_ATTRIB_SECTION_CHANGE(P, SECURE_BOOT)
/* Revert to default section. */
#define TSIP_SEC_DEFAULT        R_BSP_ATTRIB_SECTION_CHANGE_END
#else
/* Required for each variable definition with no initial value to be placed in the SECURE_BOOT section.(dummy) */
#define TSIP_SEC_B_SECURE_BOOT
/* Required for each constant definition to be placed in the SECURE_BOOT section.(dummy) */
#define TSIP_SEC_C_SECURE_BOOT
/* Required for each variable definition with initial value to be placed in the SECURE_BOOT section.(dummy) */
#define TSIP_SEC_D_SECURE_BOOT
/* Required for each function definition to be placed in the SECURE_BOOT section.(dummy) */
#define TSIP_SEC_P_SECURE_BOOT
/* Revert to default section.(dummy) */
#define TSIP_SEC_DEFAULT
#endif  /* TSIP_SECURE_BOOT != 0 */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* Return error codes */
typedef enum e_tsip_err
{
    TSIP_SUCCESS = 0u, 
    TSIP_ERR_FAIL,                      // Self-check 1 fail or TSIP function internal err.
    TSIP_ERR_RESOURCE_CONFLICT,         // A resource conflict occurred.
    TSIP_ERR_RETRY,                     // Self-check 2 fail.
    TSIP_ERR_KEY_SET,                   // setting the invalid key.
    TSIP_ERR_AUTHENTICATION,            // Authentication failed.
    TSIP_ERR_CALLBACK_UNREGIST,         // Callback function is not registered.
    TSIP_ERR_PARAMETER,                 // Illegal Input data.
    TSIP_ERR_PROHIBIT_FUNCTION,         // An invalid function call occurred.
    TSIP_RESUME_FIRMWARE_GENERATE_MAC,  // There is a continuation of R_TSIP_GenerateFirmwareMAC.
    TSIP_ERR_VERIFICATION_FAIL,         // Verification of TLS1.3 handshake failed.
} e_tsip_err_t;

/* request type for Callback of firmware update */
typedef enum
{
    TSIP_FW_CB_REQ_PRG_WT = 0u,
    TSIP_FW_CB_REQ_PRG_RD,
    TSIP_FW_CB_REQ_BUFF_CNT,
    TSIP_FW_CB_REQ_PRG_WT_LAST_BLK,
    TSIP_FW_CB_REQ_GET_UPDATE_PRG_CHKSUM,
    TSIP_FW_CB_REQ_STORE_MAC,
} TSIP_FW_CB_REQ_TYPE;

/* TLS1.3 handshake protocol phase */
typedef enum
{
    TSIP_TLS13_PHASE_HANDSHAKE = 0u,
    TSIP_TLS13_PHASE_APPLICATION,
} e_tsip_tls13_phase_t;

/* TLS1.3 handshake protocol mode */
typedef enum
{
    TSIP_TLS13_MODE_FULL_HANDSHAKE = 0u,
    TSIP_TLS13_MODE_RESUMPTION,
    TSIP_TLS13_MODE_0_RTT,
} e_tsip_tls13_mode_t;

/* TLS1.3 cipher suite */
typedef enum
{
    TSIP_TLS13_CIPHER_SUITE_AES_128_GCM_SHA256 = 0u,
    TSIP_TLS13_CIPHER_SUITE_AES_128_CCM_SHA256,
} e_tsip_tls13_cipher_suite_t;

/* TLS1.3 update key type */
typedef enum
{
    TSIP_TLS13_UPDATE_SERVER_KEY = 0u,
    TSIP_TLS13_UPDATE_CLIENT_KEY,
} e_tsip_tls13_update_key_type_t;

/* TLS1.3 signature scheme */
typedef enum
{
    TSIP_TLS13_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 = 0u,
    TSIP_TLS13_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256,
} e_tsip_tls13_signature_scheme_type_t;

/* key index type */
typedef enum
{
    TSIP_KEY_INDEX_TYPE_INVALID = 0u,
    TSIP_KEY_INDEX_TYPE_AES128,
    TSIP_KEY_INDEX_TYPE_AES256,
    TSIP_KEY_INDEX_TYPE_TDES,
    TSIP_KEY_INDEX_TYPE_ARC4,
    TSIP_KEY_INDEX_TYPE_HMAC_SHA1,
    TSIP_KEY_INDEX_TYPE_HMAC_SHA256,
    TSIP_KEY_INDEX_TYPE_RSA1024_PUBLIC,
    TSIP_KEY_INDEX_TYPE_RSA1024_PRIVATE,
    TSIP_KEY_INDEX_TYPE_RSA2048_PUBLIC,
    TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE,
    TSIP_KEY_INDEX_TYPE_RSA3072_PUBLIC,
    TSIP_KEY_INDEX_TYPE_RSA4096_PUBLIC,
    TSIP_KEY_INDEX_TYPE_AES128_FOR_TLS,
    TSIP_KEY_INDEX_TYPE_AES256_FOR_TLS,
    TSIP_KEY_INDEX_TYPE_HMAC_SHA1_FOR_TLS,
    TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_TLS,
    TSIP_KEY_INDEX_TYPE_UPDATE_KEY_RING,
    TSIP_KEY_INDEX_TYPE_TLS_CA_CERTIFICATION_PUBLIC_KEY,
    TSIP_KEY_INDEX_TYPE_TLS_P256_ECC_KEY,
    TSIP_KEY_INDEX_TYPE_ECC_P192_PUBLIC,
    TSIP_KEY_INDEX_TYPE_ECC_P224_PUBLIC,
    TSIP_KEY_INDEX_TYPE_ECC_P256_PUBLIC,
    TSIP_KEY_INDEX_TYPE_ECC_P384_PUBLIC,
    TSIP_KEY_INDEX_TYPE_ECC_P192_PRIVATE,
    TSIP_KEY_INDEX_TYPE_ECC_P224_PRIVATE,
    TSIP_KEY_INDEX_TYPE_ECC_P256_PRIVATE,
    TSIP_KEY_INDEX_TYPE_ECC_P384_PRIVATE,
    TSIP_KEY_INDEX_TYPE_ECDH_SHARED_SECRET,
    TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH,
    TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH,
    TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_ECDH,
    TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_SERVER_FINISHED_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_FINISHED_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_EARLY_SECRET_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_RESUMPTION_MASTER_SECRET_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_PSK_BINDER_KEY_FOR_CLIENT,
    TSIP_KEY_INDEX_TYPE_TLS13_SHARED_SECRET_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_SERVER_WRITE_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_SERVER_FINISHED_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_WRITE_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_CLIENT_FINISHED_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_HANDSHAKE_SECRET_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_MASTER_SECRET_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_EARLY_SECRET_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_APP_TRAFFIC_SECRET_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_RESUMPTION_MASTER_SECRET_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_PRE_SHARED_KEY_FOR_SERVER,
    TSIP_KEY_INDEX_TYPE_TLS13_PSK_BINDER_KEY_FOR_SERVER
} TSIP_KEY_INDEX_TYPE;

/* Byte data structure */
typedef struct tsip_byte_data
{
    uint8_t    *pdata;
    uint32_t    data_length;
    uint32_t    data_type;
} tsip_byte_data_t;

/* RSA byte data structure */
typedef tsip_byte_data_t tsip_rsa_byte_data_t;

/* ECDSA byte data structure */
typedef tsip_byte_data_t tsip_ecdsa_byte_data_t;

/* AES key index data structure */
typedef struct tsip_aes_key_index
{
    uint32_t type;
    /* AES128, AES256, AES128 for TLS, AES256 for TLS are supported */
    uint32_t value[R_TSIP_TLS_AES256_KEY_INDEX_WORD_SIZE];
} tsip_aes_key_index_t;

/* TDES key index data structure */
typedef struct tsip_tdes_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TDES_KEY_INDEX_WORD_SIZE];    /* DES/TDES are supported */
} tsip_tdes_key_index_t;

/* ARC4 key index data structure */
typedef struct tsip_arc4_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_ARC4_KEY_INDEX_WORD_SIZE];    /* ARC4 are supported */
} tsip_arc4_key_index_t;

/* HMAC-SHA key index data structure */
typedef struct tsip_hmac_sha_key_index
{
    uint32_t type;
    /* HMAC-SHA1, HMAC-SHA256, HMAC-SHA1 for TLS, HMAC-SHA256 for TLS are supported */
    uint32_t value[R_TSIP_TLS_HMAC_KEY_INDEX_WORD_SIZE];
    uint32_t tls13_mode;
} tsip_hmac_sha_key_index_t;

/* RSA 1024bit public key index data structure */
typedef struct tsip_rsa1024_public_key_index
{
    uint32_t type;
    struct
    {
        uint32_t    key_management_info1[R_TSIP_RSA_1024_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE];
        uint8_t     key_n[R_TSIP_RSA_1024_KEY_N_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     key_e[R_TSIP_RSA_1024_KEY_E_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     dummy[R_TSIP_RSA_KEY_GENERATION_DUMMY_BYTE_SIZE];    /* dummy data */
        uint32_t    key_management_info2[R_TSIP_RSA_1024_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE];
    }
    value;
} tsip_rsa1024_public_key_index_t;

/* RSA 1024bit private key index data structure */
typedef struct tsip_rsa1024_private_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_RSA_1024_PRIVATE_KEY_MANAGEMENT_INFO_WORD_SIZE];
} tsip_rsa1024_private_key_index_t;

/* RSA 2048bit public key index data structure */
typedef struct tsip_rsa2048_public_key_index
{
    uint32_t type;
    struct
    {
        uint32_t    key_management_info1[R_TSIP_RSA_2048_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE];
        uint8_t     key_n[R_TSIP_RSA_2048_KEY_N_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     key_e[R_TSIP_RSA_2048_KEY_E_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     dummy[R_TSIP_RSA_KEY_GENERATION_DUMMY_BYTE_SIZE];    /* dummy data */
        uint32_t    key_management_info2[R_TSIP_RSA_2048_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE];
    }
    value;
} tsip_rsa2048_public_key_index_t;

/* RSA 2048bit private key index data structure */
typedef struct tsip_rsa2048_private_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_RSA_2048_PRIVATE_KEY_MANAGEMENT_INFO_WORD_SIZE];
} tsip_rsa2048_private_key_index_t;

/* RSA 3072bit public key index data structure */
typedef struct tsip_rsa3072_public_key_index
{
    uint32_t type;
    struct
    {
        uint32_t    key_management_info1[R_TSIP_RSA_3072_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE];
        uint8_t     key_n[R_TSIP_RSA_3072_KEY_N_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     key_e[R_TSIP_RSA_3072_KEY_E_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     dummy[R_TSIP_RSA_KEY_GENERATION_DUMMY_BYTE_SIZE];    /* dummy data */
        uint32_t    key_management_info2[R_TSIP_RSA_3072_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE];
    }
    value;
} tsip_rsa3072_public_key_index_t;

/* RSA 4096bit public key index data structure */
typedef struct tsip_rsa4096_public_key_index
{
    uint32_t type;
    struct
    {
        uint32_t    key_management_info1[R_TSIP_RSA_4096_PUBLIC_KEY_MANAGEMENT_INFO1_WORD_SIZE];
        uint8_t     key_n[R_TSIP_RSA_4096_KEY_N_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     key_e[R_TSIP_RSA_4096_KEY_E_LENGTH_BYTE_SIZE];       /* plaintext */
        uint8_t     dummy[R_TSIP_RSA_KEY_GENERATION_DUMMY_BYTE_SIZE];    /* dummy data */
        uint32_t    key_management_info2[R_TSIP_RSA_4096_PUBLIC_KEY_MANAGEMENT_INFO2_WORD_SIZE];
    }
    value;
} tsip_rsa4096_public_key_index_t;

/* RSA 1024bit key index pair structure */
typedef struct tsip_rsa1024_key_pair_index
{
    tsip_rsa1024_private_key_index_t    private;
    tsip_rsa1024_public_key_index_t     public;
} tsip_rsa1024_key_pair_index_t;

/* RSA 2048bit key index pair structure */
typedef struct tsip_rsa2048_key_pair_index
{
    tsip_rsa2048_private_key_index_t    private;
    tsip_rsa2048_public_key_index_t     public;
} tsip_rsa2048_key_pair_index_t;

/* ECC public key index data structure */
typedef struct tsip_ecc_public_key_index
{
    uint32_t type;
    struct
    {
        uint32_t    key_management_info[R_TSIP_ECC_PUBLIC_KEY_MANAGEMENT_INFO_WORD_SIZE];
        uint8_t     key_q[R_TSIP_ECC_KEY_LENGTH_BYTE_SIZE];
    }
    value;
} tsip_ecc_public_key_index_t;

/* ECC private key index data structure */
typedef struct tsip_ecc_private_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_ECC_PRIVATE_KEY_MANAGEMENT_INFO_WORD_SIZE];
} tsip_ecc_private_key_index_t;

/* ECC key index pair structure */
typedef struct tsip_ecc_key_pair_index
{
    tsip_ecc_private_key_index_t    private;
    tsip_ecc_public_key_index_t     public;
} tsip_ecc_key_pair_index_t;

/* ECDH key index data structure */
typedef struct tsip_ecdh_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_SHARED_SECRET_KEY_INDEX_WORD_SIZE];
} tsip_ecdh_key_index_t;

/* TLS CA certification public key index data structure */
typedef struct tsip_tls_ca_certification_public_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS_ROOT_PUBLIC_KEY_WORD_SIZE];
} tsip_tls_ca_certification_public_key_index_t;

/* TLS P-256 ECC key index data structure */
typedef struct tsip_tls_p256_ecc_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS_P256_ECC_KEY_WORD_SIZE];
} tsip_tls_p256_ecc_key_index_t;

/* Update key ring index data structure */
typedef struct tsip_update_key_ring
{
    uint32_t type;
    uint32_t value[R_TSIP_UPDATE_KEY_RING_INDEX_WORD_SIZE];
} tsip_update_key_ring_t;

/* Ephemeral shared secret key index data structure */
typedef struct tsip_tls13_ephemeral_shared_secret_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_SHARED_SECRET_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_shared_secret_key_index_t;

/* Ephemeral handshake secret key index data structure */
typedef struct tsip_tls13_ephemeral_handshake_secret_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_HANDSHAKE_SECRET_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_handshake_secret_key_index_t;

/* Ephemeral server finished key index data structure */
typedef struct tsip_tls13_ephemeral_server_finished_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_FINISHED_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_server_finished_key_index_t;

/* Ephemeral client finished key index data structure */
typedef struct tsip_tls13_ephemeral_client_finished_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_FINISHED_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_client_finished_key_index_t;

/* Ephemeral master secret key index data structure */
typedef struct tsip_tls13_ephemeral_master_secret_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_MASTER_SECRET_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_master_secret_key_index_t;

/* Ephemeral application traffic secret key index data structure */
typedef struct tsip_tls13_ephemeral_app_secret_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_APP_SECRET_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_app_secret_key_index_t;

/* Ephemeral resumption master secret key index data structure */
typedef struct tsip_tls13_ephemeral_res_master_secret_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_MASTER_SECRET_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_res_master_secret_key_index_t;

/* Ephemeral pre shared key index data structure */
typedef struct tsip_tls13_ephemeral_pre_shared_key_index
{
    uint32_t type;
    uint32_t value[R_TSIP_TLS13_PRE_SHARED_KEY_WORD_SIZE];
} tsip_tls13_ephemeral_pre_shared_key_index_t;

/* The work area for AES */
typedef struct tsip_aes_handle
{
    uint32_t                id;
    uint8_t                 last_1_block_as_fraction[R_TSIP_AES_BLOCK_BYTE_SIZE];
    uint8_t                 flag_call_init;
} tsip_aes_handle_t;

/* The work area for TDES */
typedef struct tsip_tdes_handle
{
    uint32_t                id;
    uint8_t                 last_1_block_as_fraction[R_TSIP_TDES_BLOCK_BYTE_SIZE];
    uint8_t                 flag_call_init;
} tsip_tdes_handle_t;

/* The work area for ARC4 */
typedef struct tsip_arc4_handle
{
    uint32_t                id;
    uint8_t                 last_1_block_as_fraction[R_TSIP_ARC4_BLOCK_BYTE_SIZE];
    uint8_t                 flag_call_init;
} tsip_arc4_handle_t;

/* The work area for MD5 */
typedef struct tsip_sha_md5_handle
{
    uint32_t hash_type;
    uint8_t  sha_buffer[R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE * 4];
    uint32_t all_received_length;
    uint32_t buffering_length;
    /* SHA1(20byte), SHA256(32byte), MD5(16byte) are supported */
    uint8_t  current_hash[R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE];
    uint8_t  flag_call_init;
} tsip_sha_md5_handle_t;

/* The work area for HMAC-SHA */
typedef struct tsip_hmac_sha_handle
{
    uint32_t                    id;
    uint8_t                     hmac_buffer[R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE * 4];
    uint32_t                    all_received_length;
    uint32_t                    buffering_length;
    uint8_t                     flag_call_init;
} tsip_hmac_sha_handle_t;

/* The work area for CMAC */
typedef struct tsip_cmac_handle
{
    uint32_t                id;
    uint8_t                 cmac_buffer[R_TSIP_AES_BLOCK_BYTE_SIZE];
    uint32_t                all_received_length;
    uint32_t                buffering_length;
    uint8_t                 flag_call_init;
} tsip_cmac_handle_t;

/* The work area for GCM */
typedef struct tsip_gcm_handle
{
    uint32_t                id;
    uint8_t                 gcm_buffer[R_TSIP_AES_BLOCK_BYTE_SIZE];
    uint8_t                 gcm_aad_buffer[R_TSIP_AES_GCM_AAD_BLOCK_BYTE_SIZE];
    uint32_t                all_received_length;
    uint32_t                all_received_aad_length;
    uint32_t                buffering_length;
    uint32_t                buffering_aad_length;
    uint8_t                 flag_call_init;
    uint8_t                 flag_update_input_data;
} tsip_gcm_handle_t;

/* The work area for CCM */
typedef struct tsip_ccm_handle
{
    uint32_t                id;
    uint8_t                 formatted_data[R_TSIP_AES_CCM_B_FORMAT_BYTE_SIZE];
    uint8_t                 counter[R_TSIP_AES_CCM_COUNTER_BYTE_SIZE];
    uint8_t                 ccm_buffer[R_TSIP_AES_BLOCK_BYTE_SIZE];
    uint32_t                all_received_length;
    uint32_t                buffering_length;
    uint8_t                 flag_call_init;
} tsip_ccm_handle_t;

/* The work area for ECDH */
typedef struct tsip_ecdh_handle
{
    uint32_t                id;
    uint32_t                flag_use_key_id;
    uint32_t                key_id;
    uint32_t                key_type;
    uint8_t                 flag_call_init;
    uint8_t                 flag_call_make_public;
    uint8_t                 flag_call_read_public;
    uint8_t                 flag_call_shared_secret;
} tsip_ecdh_handle_t;

/* The work area for TLS1.3 */
typedef struct tsip_tls13_handle
{
    uint32_t                    session_handle[4];
    e_tsip_tls13_cipher_suite_t cipher_suite;
    tsip_gcm_handle_t           gcm_handle_tx;
    tsip_gcm_handle_t           gcm_handle_rx;
    tsip_ccm_handle_t           ccm_handle_tx;
    tsip_ccm_handle_t           ccm_handle_rx;
    uint32_t                    plain_length;
    uint32_t                    cipher_length;
    uint64_t                    seqnum_handshake_tx;
    uint64_t                    seqnum_handshake_rx;
    uint64_t                    seqnum_application_tx;
    uint64_t                    seqnum_application_rx;
    uint8_t                     flag_handshake_tx;
    uint8_t                     flag_handshake_rx;
    uint8_t                     flag_application_tx;
    uint8_t                     flag_application_rx;
    uint8_t                     cipher_tag[16];
    uint8_t                     cipher_tag_index;
} tsip_tls13_handle_t;

/* The work area for firmware update */
typedef struct tsip_firmware_generate_mac_resume_handle
{
    uint32_t iLoop;
    uint32_t counter;
    uint32_t previous_counter;
    bool     use_resume_flag;
} tsip_firmware_generate_mac_resume_handle_t;

/* The callback function pointer type for R_TSIP_GenerateFirmwareMAC */
typedef void (*TSIP_GEN_MAC_CB_FUNC_T) (TSIP_FW_CB_REQ_TYPE req_type, uint32_t iLoop, uint32_t *counter,
        uint32_t *InData_UpProgram, uint32_t *OutData_Program, uint32_t MAX_CNT);

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
e_tsip_err_t R_TSIP_Open(tsip_tls_ca_certification_public_key_index_t *key_index_1,
        tsip_update_key_ring_t *key_index_2);
e_tsip_err_t R_TSIP_Close(void);
void R_TSIP_SoftwareReset(void);

e_tsip_err_t R_TSIP_GenerateAes128KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateAes256KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateTdesKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateArc4KeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv, uint8_t *encrypted_key,
        tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa1024PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa1024_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa1024PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa1024_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa2048PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa2048_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa2048PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa2048_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa3072PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa3072_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa4096PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa4096_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateTlsRsaPublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_tls_ca_certification_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP192PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP224PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP256PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP384PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP192PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP224PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP256PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP384PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateSha1HmacKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateSha256HmacKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateTlsP256EccKeyIndex(tsip_tls_p256_ecc_key_index_t *tls_p256_ecc_key_index,
        uint8_t *ephemeral_ecdh_public_key);
e_tsip_err_t R_TSIP_GenerateAes128RandomKeyIndex(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateAes256RandomKeyIndex(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateRsa1024RandomKeyIndex(tsip_rsa1024_key_pair_index_t *key_pair_index);
e_tsip_err_t R_TSIP_GenerateRsa2048RandomKeyIndex(tsip_rsa2048_key_pair_index_t *key_pair_index);
e_tsip_err_t R_TSIP_GenerateTdesRandomKeyIndex(tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateArc4RandomKeyIndex(tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_GenerateEccP192RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index);
e_tsip_err_t R_TSIP_GenerateEccP224RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index);
e_tsip_err_t R_TSIP_GenerateEccP256RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index);
e_tsip_err_t R_TSIP_GenerateEccP384RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index);
e_tsip_err_t R_TSIP_GenerateRandomNumber(uint32_t *random);
e_tsip_err_t R_TSIP_GenerateUpdateKeyRingKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_update_key_ring_t *key_index);
uint32_t R_TSIP_GetVersion(void);

e_tsip_err_t R_TSIP_UpdateAes128KeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateAes256KeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateTdesKeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateArc4KeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateRsa1024PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa1024_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateRsa1024PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa1024_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateRsa2048PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa2048_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateRsa2048PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa2048_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateRsa3072PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa3072_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateRsa4096PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa4096_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateTlsRsaPublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_tls_ca_certification_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP192PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP224PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP256PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP384PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP192PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP224PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP256PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateEccP384PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateSha1HmacKeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_UpdateSha256HmacKeyIndex(uint8_t *iv, uint8_t *encrypted_key, tsip_hmac_sha_key_index_t *key_index);

e_tsip_err_t R_TSIP_StartUpdateFirmware(void);
e_tsip_err_t R_TSIP_GenerateFirmwareMAC(uint32_t *InData_KeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_UpProgram, uint32_t *InData_IV, uint32_t *OutData_Program, uint32_t MAX_CNT,
        TSIP_GEN_MAC_CB_FUNC_T p_callback,
        tsip_firmware_generate_mac_resume_handle_t *tsip_firmware_generate_mac_resume_handle);
e_tsip_err_t R_TSIP_VerifyFirmwareMAC(uint32_t *InData_Program, uint32_t MAX_CNT, uint32_t *InData_MAC);

e_tsip_err_t R_TSIP_Aes128EcbEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128EcbEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Aes128EcbEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_Aes128EcbDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128EcbDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Aes128EcbDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length);
e_tsip_err_t R_TSIP_Aes128CbcEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec);
e_tsip_err_t R_TSIP_Aes128CbcEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Aes128CbcEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_Aes128CbcDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec);
e_tsip_err_t R_TSIP_Aes128CbcDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Aes128CbcDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length);

e_tsip_err_t R_TSIP_Aes256EcbEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256EcbEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Aes256EcbEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_Aes256EcbDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256EcbDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Aes256EcbDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length);
e_tsip_err_t R_TSIP_Aes256CbcEncryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec);
e_tsip_err_t R_TSIP_Aes256CbcEncryptUpdate(tsip_aes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Aes256CbcEncryptFinal(tsip_aes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_Aes256CbcDecryptInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec);
e_tsip_err_t R_TSIP_Aes256CbcDecryptUpdate(tsip_aes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Aes256CbcDecryptFinal(tsip_aes_handle_t *handle, uint8_t *plain, uint32_t *plain_length);

e_tsip_err_t R_TSIP_Aes128CtrInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ictr);
e_tsip_err_t R_TSIP_Aes256CtrInit(tsip_aes_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ictr);
e_tsip_err_t R_TSIP_Aes128CtrUpdate(tsip_aes_handle_t *handle, uint8_t *itext, uint8_t *otext, uint32_t itext_length);
e_tsip_err_t R_TSIP_Aes256CtrUpdate(tsip_aes_handle_t *handle, uint8_t *itext, uint8_t *otext, uint32_t itext_length);
e_tsip_err_t R_TSIP_Aes128CtrFinal(tsip_aes_handle_t *handle);
e_tsip_err_t R_TSIP_Aes256CtrFinal(tsip_aes_handle_t *handle);

e_tsip_err_t R_TSIP_Aes128GcmEncryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len);
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdate(tsip_gcm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_data_len, uint8_t *aad, uint32_t aad_len);
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinal(tsip_gcm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_data_len,
        uint8_t *atag);
e_tsip_err_t R_TSIP_Aes128GcmDecryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len);
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdate(tsip_gcm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_data_len, uint8_t *aad, uint32_t aad_len);
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinal(tsip_gcm_handle_t *handle, uint8_t *plain, uint32_t *plain_data_len,
        uint8_t *atag, uint32_t atag_len);
e_tsip_err_t R_TSIP_Aes256GcmEncryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len);
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdate(tsip_gcm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_data_len, uint8_t *aad, uint32_t aad_len);
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinal(tsip_gcm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_data_len,
        uint8_t *atag);
e_tsip_err_t R_TSIP_Aes256GcmDecryptInit(tsip_gcm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *ivec,
        uint32_t ivec_len);
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdate(tsip_gcm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_data_len, uint8_t *aad, uint32_t aad_len);
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinal(tsip_gcm_handle_t *handle, uint8_t *plain, uint32_t *plain_data_len,
        uint8_t *atag, uint32_t atag_len);

e_tsip_err_t R_TSIP_Aes128CcmEncryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len);
e_tsip_err_t R_TSIP_Aes128CcmEncryptUpdate(tsip_ccm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinal(tsip_ccm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length,
        uint8_t *mac, uint32_t mac_length);
e_tsip_err_t R_TSIP_Aes128CcmDecryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len);
e_tsip_err_t R_TSIP_Aes128CcmDecryptUpdate(tsip_ccm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinal(tsip_ccm_handle_t *handle, uint8_t *plain, uint32_t *plain_length,
        uint8_t *mac, uint32_t mac_length);
e_tsip_err_t R_TSIP_Aes256CcmEncryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len);
e_tsip_err_t R_TSIP_Aes256CcmEncryptUpdate(tsip_ccm_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinal(tsip_ccm_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length,
        uint8_t *mac, uint32_t mac_length);
e_tsip_err_t R_TSIP_Aes256CcmDecryptInit(tsip_ccm_handle_t *handle, tsip_aes_key_index_t *key_index, uint8_t *nonce,
        uint32_t nonce_len, uint8_t *adata, uint8_t a_len, uint32_t payload_len, uint32_t mac_len);
e_tsip_err_t R_TSIP_Aes256CcmDecryptUpdate(tsip_ccm_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinal(tsip_ccm_handle_t *handle, uint8_t *plain, uint32_t *plain_length,
        uint8_t *mac, uint32_t mac_length);

e_tsip_err_t R_TSIP_Aes128CmacGenerateInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128CmacGenerateUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Aes128CmacGenerateFinal(tsip_cmac_handle_t *handle, uint8_t *mac);
e_tsip_err_t R_TSIP_Aes128CmacVerifyInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128CmacVerifyUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Aes128CmacVerifyFinal(tsip_cmac_handle_t *handle, uint8_t *mac, uint32_t mac_length);
e_tsip_err_t R_TSIP_Aes256CmacGenerateInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256CmacGenerateUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Aes256CmacGenerateFinal(tsip_cmac_handle_t *handle, uint8_t *mac);
e_tsip_err_t R_TSIP_Aes256CmacVerifyInit(tsip_cmac_handle_t *handle, tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256CmacVerifyUpdate(tsip_cmac_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Aes256CmacVerifyFinal(tsip_cmac_handle_t *handle, uint8_t *mac, uint32_t mac_length);

e_tsip_err_t R_TSIP_Aes128KeyWrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        tsip_aes_key_index_t *target_key_index, uint32_t *wrapped_key);
e_tsip_err_t R_TSIP_Aes256KeyWrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        tsip_aes_key_index_t *target_key_index, uint32_t *wrapped_key);
e_tsip_err_t R_TSIP_Aes128KeyUnwrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        uint32_t *wrapped_key, tsip_aes_key_index_t *target_key_index);
e_tsip_err_t R_TSIP_Aes256KeyUnwrap(tsip_aes_key_index_t *wrap_key_index, uint32_t target_key_type,
        uint32_t *wrapped_key, tsip_aes_key_index_t *target_key_index);

e_tsip_err_t R_TSIP_TdesEcbEncryptInit(tsip_tdes_handle_t *handle, tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_TdesEcbEncryptUpdate(tsip_tdes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_TdesEcbEncryptFinal(tsip_tdes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_TdesEcbDecryptInit(tsip_tdes_handle_t *handle, tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_TdesEcbDecryptUpdate(tsip_tdes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_TdesEcbDecryptFinal(tsip_tdes_handle_t *handle, uint8_t *plain, uint32_t *plain_length);
e_tsip_err_t R_TSIP_TdesCbcEncryptInit(tsip_tdes_handle_t *handle, tsip_tdes_key_index_t *key_index, uint8_t *ivec);
e_tsip_err_t R_TSIP_TdesCbcEncryptUpdate(tsip_tdes_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_TdesCbcEncryptFinal(tsip_tdes_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_TdesCbcDecryptInit(tsip_tdes_handle_t *handle, tsip_tdes_key_index_t *key_index, uint8_t *ivec);
e_tsip_err_t R_TSIP_TdesCbcDecryptUpdate(tsip_tdes_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_TdesCbcDecryptFinal(tsip_tdes_handle_t *handle, uint8_t *plain, uint32_t *plain_length);

e_tsip_err_t R_TSIP_Arc4EncryptInit(tsip_arc4_handle_t *handle, tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_Arc4EncryptUpdate(tsip_arc4_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Arc4EncryptFinal(tsip_arc4_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_Arc4DecryptInit(tsip_arc4_handle_t *handle, tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_Arc4DecryptUpdate(tsip_arc4_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Arc4DecryptFinal(tsip_arc4_handle_t *handle, uint8_t *plain, uint32_t *plain_length);

e_tsip_err_t R_TSIP_Md5Init(tsip_sha_md5_handle_t *handle);
e_tsip_err_t R_TSIP_Md5Update(tsip_sha_md5_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Md5Final(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length);

e_tsip_err_t R_TSIP_Sha1Init(tsip_sha_md5_handle_t *handle);
e_tsip_err_t R_TSIP_Sha1Update(tsip_sha_md5_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Sha1Final(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length);
e_tsip_err_t R_TSIP_Sha256Init(tsip_sha_md5_handle_t *handle);
e_tsip_err_t R_TSIP_Sha256Update(tsip_sha_md5_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Sha256Final(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length);

e_tsip_err_t R_TSIP_GetCurrentHashDigestValue(tsip_sha_md5_handle_t *handle, uint8_t *digest, uint32_t *digest_length);

e_tsip_err_t R_TSIP_Sha1HmacGenerateInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha1HmacGenerateUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Sha1HmacGenerateFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac);
e_tsip_err_t R_TSIP_Sha256HmacGenerateInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha256HmacGenerateUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message,
        uint32_t message_length);
e_tsip_err_t R_TSIP_Sha256HmacGenerateFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac);
e_tsip_err_t R_TSIP_Sha1HmacVerifyInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha1HmacVerifyUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Sha1HmacVerifyFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac, uint32_t mac_length);
e_tsip_err_t R_TSIP_Sha256HmacVerifyInit(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha256HmacVerifyUpdate(tsip_hmac_sha_handle_t *handle, uint8_t *message, uint32_t message_length);
e_tsip_err_t R_TSIP_Sha256HmacVerifyFinal(tsip_hmac_sha_handle_t *handle, uint8_t *mac, uint32_t mac_length);

e_tsip_err_t R_TSIP_RsassaPkcs1024SignatureGenerate(tsip_rsa_byte_data_t *message_hash,
        tsip_rsa_byte_data_t *signature, tsip_rsa1024_private_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPkcs1024SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa1024_public_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPkcs2048SignatureGenerate(tsip_rsa_byte_data_t *message_hash,
        tsip_rsa_byte_data_t *signature, tsip_rsa2048_private_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPkcs2048SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa2048_public_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPkcs3072SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa3072_public_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPkcs4096SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa4096_public_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPss1024SignatureGenerate(tsip_rsa_byte_data_t *message, tsip_rsa_byte_data_t *signature,
        tsip_rsa1024_private_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPss1024SignatureVerification(tsip_rsa_byte_data_t *signature, tsip_rsa_byte_data_t *message,
        tsip_rsa1024_public_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPss2048SignatureGenerate(tsip_rsa_byte_data_t *message_hash,
        tsip_rsa_byte_data_t *signature, tsip_rsa2048_private_key_index_t *key_index, uint8_t hash_type);
e_tsip_err_t R_TSIP_RsassaPss2048SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa2048_public_key_index_t *key_index, uint8_t hash_type);

e_tsip_err_t R_TSIP_RsaesPkcs1024Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa1024_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_RsaesPkcs1024Decrypt(tsip_rsa_byte_data_t *cipher, tsip_rsa_byte_data_t *plain,
        tsip_rsa1024_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_RsaesPkcs2048Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa2048_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_RsaesPkcs2048Decrypt(tsip_rsa_byte_data_t *cipher, tsip_rsa_byte_data_t *plain,
        tsip_rsa2048_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_RsaesPkcs3072Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa3072_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_RsaesPkcs4096Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa4096_public_key_index_t *key_index);

e_tsip_err_t R_TSIP_Rsa2048DhKeyAgreement(tsip_aes_key_index_t *key_index,
        tsip_rsa2048_private_key_index_t *sender_private_key_index, uint8_t *message, uint8_t *receiver_modulus,
        uint8_t *sender_modulus);

e_tsip_err_t R_TSIP_TlsRootCertificateVerification(uint32_t public_key_type, uint8_t *certificate,
        uint32_t certificate_length, uint32_t public_key_n_start_position, uint32_t public_key_n_end_position,
        uint32_t public_key_e_start_position, uint32_t public_key_e_end_position, uint8_t *signature,
        uint32_t *encrypted_root_public_key);
e_tsip_err_t R_TSIP_TlsCertificateVerification(uint32_t public_key_type, uint32_t *encrypted_input_public_key,
        uint8_t *certificate, uint32_t certificate_length, uint8_t *signature, uint32_t public_key_n_start_position,
        uint32_t public_key_n_end_position, uint32_t public_key_e_start_position, uint32_t public_key_e_end_position,
        uint32_t *encrypted_output_public_key);
e_tsip_err_t R_TSIP_TlsCertificateVerificationExtension(uint32_t public_key_type, uint32_t public_key_output_type,
        uint32_t *encrypted_input_public_key,
        uint8_t *certificate, uint32_t certificate_length, uint8_t *signature, uint32_t public_key_n_start_position,
        uint32_t public_key_n_end_position, uint32_t public_key_e_start_position, uint32_t public_key_e_end_position,
        uint32_t *encrypted_output_public_key);
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecret(uint32_t *tsip_pre_master_secret);
e_tsip_err_t R_TSIP_TlsGenerateMasterSecret(uint32_t select_cipher_suite, uint32_t *tsip_pre_master_secret,
        uint8_t *client_random, uint8_t *server_random, uint32_t *tsip_master_secret);
e_tsip_err_t R_TSIP_TlsEncryptPreMasterSecretWithRsa2048PublicKey(uint32_t *encrypted_public_key,
        uint32_t *tsip_pre_master_secret, uint8_t *encrypted_pre_master_secret);
e_tsip_err_t R_TSIP_TlsGenerateSessionKey(uint32_t select_cipher_suite, uint32_t *tsip_master_secret,
        uint8_t *client_random, uint8_t *server_random, uint8_t* nonce_explicit,
        tsip_hmac_sha_key_index_t *client_mac_key_index, tsip_hmac_sha_key_index_t *server_mac_key_index,
        tsip_aes_key_index_t *client_crypto_key_index, tsip_aes_key_index_t *server_crypto_key_index,
        uint8_t *client_iv, uint8_t *server_iv);
e_tsip_err_t R_TSIP_TlsGenerateVerifyData(uint32_t select_verify_data, uint32_t *tsip_master_secret,
        uint8_t *hand_shake_hash, uint8_t *verify_data);
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecretWithEccP256Key(uint32_t *encrypted_public_key,
        tsip_tls_p256_ecc_key_index_t *tls_p256_ecc_key_index, uint32_t *tsip_pre_master_secret);
e_tsip_err_t R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrieves(uint32_t public_key_type, uint8_t *client_random,
        uint8_t *server_random, uint8_t *server_ephemeral_ecdh_public_key, uint8_t *server_key_exchange_signature,
        uint32_t *encrypted_public_key, uint32_t *encrypted_ephemeral_ecdh_public_key);
e_tsip_err_t R_TSIP_TlsGenerateExtendedMasterSecret(uint32_t select_cipher_suite, uint32_t *tsip_pre_master_secret,
        uint8_t *digest, uint32_t *extended_master_secret);

e_tsip_err_t R_TSIP_EcdsaP192SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP224SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP256SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP384SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP192SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP224SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP256SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdsaP384SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index);

e_tsip_err_t R_TSIP_EcdhP256Init(tsip_ecdh_handle_t *handle, uint32_t key_type, uint32_t use_key_id);
e_tsip_err_t R_TSIP_EcdhP256ReadPublicKey(tsip_ecdh_handle_t *handle, tsip_ecc_public_key_index_t *public_key_index,
        uint8_t *public_key_data, tsip_ecdsa_byte_data_t *signature, tsip_ecc_public_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdhP256MakePublicKey(tsip_ecdh_handle_t *handle, tsip_ecc_public_key_index_t *public_key_index,
        tsip_ecc_private_key_index_t *private_key_index, uint8_t *public_key, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index);
e_tsip_err_t R_TSIP_EcdhP256CalculateSharedSecretIndex(tsip_ecdh_handle_t *handle,
        tsip_ecc_public_key_index_t *public_key_index, tsip_ecc_private_key_index_t *private_key_index,
        tsip_ecdh_key_index_t *shared_secret_index);
e_tsip_err_t R_TSIP_EcdhP256KeyDerivation(tsip_ecdh_handle_t *handle, tsip_ecdh_key_index_t *shared_secret_index,
        uint32_t key_type, uint32_t kdf_type, uint8_t *other_info, uint32_t other_info_length,
        tsip_hmac_sha_key_index_t *salt_key_index, tsip_aes_key_index_t *key_index);

e_tsip_err_t R_TSIP_EcdheP512KeyAgreement(tsip_aes_key_index_t *key_index, uint8_t *receiver_public_key,
        uint8_t *sender_public_key);

e_tsip_err_t R_TSIP_GenerateTls13P256EccKeyIndex(tsip_tls13_handle_t * handle, e_tsip_tls13_mode_t mode,
        tsip_tls_p256_ecc_key_index_t * key_index, uint8_t * ephemeral_ecdh_public_key);
e_tsip_err_t R_TSIP_Tls13GenerateEcdheSharedSecret(e_tsip_tls13_mode_t mode, uint8_t * server_public_key,
        tsip_tls_p256_ecc_key_index_t * key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t * shared_secret_key_index);
e_tsip_err_t R_TSIP_Tls13GenerateHandshakeSecret(
        tsip_tls13_ephemeral_shared_secret_key_index_t * shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t * handshake_secret_key_index);
e_tsip_err_t R_TSIP_Tls13GenerateServerHandshakeTrafficKey(tsip_tls13_handle_t * handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t * handshake_secret_key_index, uint8_t * digest,
        tsip_aes_key_index_t * server_write_key_index,
        tsip_tls13_ephemeral_server_finished_key_index_t * server_finished_key_index);
e_tsip_err_t R_TSIP_Tls13ServerHandshakeVerification(e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_server_finished_key_index_t * server_finished_key_index, uint8_t * digest,
        uint8_t * server_finished, uint32_t * verify_data_index);
e_tsip_err_t R_TSIP_Tls13GenerateClientHandshakeTrafficKey(tsip_tls13_handle_t * handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t * handshake_secret_key_index, uint8_t * digest,
        tsip_aes_key_index_t * client_write_key_index, tsip_hmac_sha_key_index_t * client_finished_key_index);
e_tsip_err_t R_TSIP_Tls13GenerateMasterSecret(tsip_tls13_handle_t * handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t * handshake_secret_key_index, uint32_t * verify_data_index,
        tsip_tls13_ephemeral_master_secret_key_index_t * master_secret_key_index);
e_tsip_err_t R_TSIP_Tls13GenerateApplicationTrafficKey(tsip_tls13_handle_t * handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t * master_secret_key_index, uint8_t * digest,
        tsip_tls13_ephemeral_app_secret_key_index_t * server_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t * client_app_secret_key_index,
        tsip_aes_key_index_t * server_write_key_index, tsip_aes_key_index_t * client_write_key_index);
e_tsip_err_t R_TSIP_Tls13UpdateApplicationTrafficKey(tsip_tls13_handle_t * handle, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_update_key_type_t key_type,
        tsip_tls13_ephemeral_app_secret_key_index_t * input_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t * output_app_secret_key_index,
        tsip_aes_key_index_t * app_write_key_index);
e_tsip_err_t R_TSIP_Tls13EncryptInit(tsip_tls13_handle_t *handle, e_tsip_tls13_phase_t phase, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_cipher_suite_t cipher_suite, tsip_aes_key_index_t *key_index, uint32_t payload_length);
e_tsip_err_t R_TSIP_Tls13EncryptUpdate(tsip_tls13_handle_t *handle, uint8_t *plain, uint8_t *cipher,
        uint32_t plain_length);
e_tsip_err_t R_TSIP_Tls13EncryptFinal(tsip_tls13_handle_t *handle, uint8_t *cipher, uint32_t *cipher_length);
e_tsip_err_t R_TSIP_Tls13DecryptInit(tsip_tls13_handle_t *handle, e_tsip_tls13_phase_t phase, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_cipher_suite_t cipher_suite, tsip_aes_key_index_t *key_index, uint32_t payload_length);
e_tsip_err_t R_TSIP_Tls13DecryptUpdate(tsip_tls13_handle_t *handle, uint8_t *cipher, uint8_t *plain,
        uint32_t cipher_length);
e_tsip_err_t R_TSIP_Tls13DecryptFinal(tsip_tls13_handle_t *handle, uint8_t *plain, uint32_t *plain_length);
#if (defined BSP_MCU_RX65N || defined BSP_MCU_RX651 || \
    defined BSP_MCU_RX72M || defined BSP_MCU_RX72N || defined BSP_MCU_RX66N)
e_tsip_err_t R_TSIP_Tls13GenerateResumptionMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index);
e_tsip_err_t R_TSIP_Tls13GeneratePreSharedKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index, uint8_t *ticket_nonce,
        uint32_t ticket_nonce_len, tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index);
e_tsip_err_t R_TSIP_Tls13GeneratePskBinderKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_hmac_sha_key_index_t * psk_binder_key_index);
e_tsip_err_t R_TSIP_Tls13Generate0RttApplicationWriteKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        uint8_t *digest, tsip_aes_key_index_t *client_write_key_index);
e_tsip_err_t R_TSIP_Tls13GenerateResumptionHandshakeSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index);
#endif /* (defined BSP_MCU_RX65N || defined BSP_MCU_RX651 || \
    defined BSP_MCU_RX72M || defined BSP_MCU_RX72N || defined BSP_MCU_RX66N) */
e_tsip_err_t R_TSIP_Tls13CertificateVerifyGenerate(uint32_t * key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t * digest, uint8_t * certificate_verify,
        uint32_t * certificate_verify_len);
e_tsip_err_t R_TSIP_Tls13CertificateVerifyVerification(uint32_t * key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t * digest, uint8_t * certificate_verify,
        uint32_t certificate_verify_len);

#if (defined BSP_MCU_RX65N || defined BSP_MCU_RX651 || \
    defined BSP_MCU_RX72M || defined BSP_MCU_RX72N || defined BSP_MCU_RX66N)
e_tsip_err_t R_TSIP_GenerateTls13SVP256EccKeyIndex(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls_p256_ecc_key_index_t *key_index, uint8_t *ephemeral_ecdh_public_key);
e_tsip_err_t R_TSIP_Tls13SVGenerateEcdheSharedSecret(e_tsip_tls13_mode_t mode, uint8_t *server_public_key,
        tsip_tls_p256_ecc_key_index_t *key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerateHandshakeSecret(
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerateServerHandshakeTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint8_t *digest,
        tsip_aes_key_index_t *server_write_key_index, tsip_hmac_sha_key_index_t *server_finished_key_index);
e_tsip_err_t R_TSIP_Tls13SVClientHandshakeVerification(e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_client_finished_key_index_t *client_finished_key_index, uint8_t *digest,
        uint8_t *client_finished);
e_tsip_err_t R_TSIP_Tls13SVGenerateClientHandshakeTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index, uint8_t *digest,
        tsip_aes_key_index_t *client_write_key_index,
        tsip_tls13_ephemeral_client_finished_key_index_t *client_finished_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerateMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerateApplicationTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_app_secret_key_index_t *server_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t *client_app_secret_key_index,
        tsip_aes_key_index_t *server_write_key_index, tsip_aes_key_index_t *client_write_key_index);
e_tsip_err_t R_TSIP_Tls13SVUpdateApplicationTrafficKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        e_tsip_tls13_update_key_type_t key_type,
        tsip_tls13_ephemeral_app_secret_key_index_t *input_app_secret_key_index,
        tsip_tls13_ephemeral_app_secret_key_index_t *output_app_secret_key_index,
        tsip_aes_key_index_t *app_write_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerateResumptionMasterSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_master_secret_key_index_t *master_secret_key_index, uint8_t *digest,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index);
e_tsip_err_t R_TSIP_Tls13SVGeneratePreSharedKey(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_res_master_secret_key_index_t *res_master_secret_key_index, uint8_t *ticket_nonce,
        uint32_t ticket_nonce_len, tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index);
e_tsip_err_t R_TSIP_Tls13SVGeneratePskBinderKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_hmac_sha_key_index_t * psk_binder_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerate0RttApplicationWriteKey(tsip_tls13_handle_t *handle,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        uint8_t *digest, tsip_aes_key_index_t *client_write_key_index);
e_tsip_err_t R_TSIP_Tls13SVGenerateResumptionHandshakeSecret(tsip_tls13_handle_t *handle, e_tsip_tls13_mode_t mode,
        tsip_tls13_ephemeral_pre_shared_key_index_t * pre_shared_key_index,
        tsip_tls13_ephemeral_shared_secret_key_index_t *shared_secret_key_index,
        tsip_tls13_ephemeral_handshake_secret_key_index_t *handshake_secret_key_index);
e_tsip_err_t R_TSIP_Tls13SVCertificateVerifyGenerate(uint32_t *key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t *digest, uint8_t *certificate_verify,
        uint32_t *certificate_verify_len);
e_tsip_err_t R_TSIP_Tls13SVCertificateVerifyVerification(uint32_t *key_index,
        e_tsip_tls13_signature_scheme_type_t signature_scheme, uint8_t *digest, uint8_t *certificate_verify,
        uint32_t certificate_verify_len);
#endif /* (defined BSP_MCU_RX65N || defined BSP_MCU_RX651 || \
    defined BSP_MCU_RX72M || defined BSP_MCU_RX72N || defined BSP_MCU_RX66N) */

#if TSIP_USER_SHA_384_ENABLED != 0
uint32_t TSIP_USER_SHA_384_FUNCTION(uint8_t *message, uint8_t *digest, uint32_t message_length);
#endif  /* TSIP_USER_SHA_384_ENABLED != 0 */

#if TSIP_MULTI_THREADING != 0
void TSIP_MULTI_THREADING_LOCK_FUNCTION(void);
void TSIP_MULTI_THREADING_UNLOCK_FUNCTION(void);
#endif  /* TSIP_MULTI_THREADING != 0 */

#endif /* R_TSIP_RX_HEADER_FILE */
