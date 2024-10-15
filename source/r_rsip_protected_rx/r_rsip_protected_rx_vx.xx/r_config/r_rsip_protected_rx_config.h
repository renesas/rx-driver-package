/* Generated configuration header file - do not edit */
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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsip_protected_rx_config.h
* Description  : Configuration options for the r_rsip_protected_rx_config RSIP module.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release
***********************************************************************************************************************/
#ifndef R_RSIP_RX_CONFIG_HEADER_FILE
#define R_RSIP_RX_CONFIG_HEADER_FILE

/***********************************************************************************************************************
 * Configuration Options
 **********************************************************************************************************************/
#define RSIP_CFG_PARAM_CHECKING_ENABLE                                      (1)

#define RSIP_CFG_AES_128_ENABLE                                             (1)
#define RSIP_CFG_AES_256_ENABLE                                             (1)
#define RSIP_CFG_XTS_AES_128_ENABLE                                         (0)
#define RSIP_CFG_XTS_AES_256_ENABLE                                         (0)
#define RSIP_CFG_AES_ECB_CBC_CTR_ENABLE                                     (1)
#define RSIP_CFG_AES_XTS_ENABLE                                             (0)
#define RSIP_CFG_AES_GCM_ENABLE                                             (1)
#define RSIP_CFG_AES_CCM_ENABLE                                             (1)
#define RSIP_CFG_AES_CMAC_ENABLE                                            (1)

#define RSIP_CFG_ECC_SECP256R1_ENABLE                                       (1)
#define RSIP_CFG_ECC_BRAINPOOLP256R1_ENABLE                                 (1)
#define RSIP_CFG_ECC_SECP256K1_ENABLE                                       (1)
#define RSIP_CFG_ECC_SECP384R1_ENABLE                                       (0)
#define RSIP_CFG_ECC_SECP521R1_ENABLE                                       (0)

#define RSIP_CFG_RSA_2048_ENABLE                                            (0)
#define RSIP_CFG_RSA_3072_ENABLE                                            (0)
#define RSIP_CFG_RSA_4096_ENABLE                                            (0)

#define RSIP_CFG_SHA1_ENABLE                                                (0)
#define RSIP_CFG_SHA224_ENABLE                                              (1)
#define RSIP_CFG_SHA256_ENABLE                                              (1)
#define RSIP_CFG_SHA384_ENABLE                                              (0)
#define RSIP_CFG_SHA512_ENABLE                                              (0)
#define RSIP_CFG_SHA512_224_ENABLE                                          (0)
#define RSIP_CFG_SHA512_256_ENABLE                                          (0)

#define RSIP_CFG_HMAC_SHA1_ENABLE                                           (0)
#define RSIP_CFG_HMAC_SHA224_ENABLE                                         (1)
#define RSIP_CFG_HMAC_SHA256_ENABLE                                         (1)
#define RSIP_CFG_HMAC_SHA384_ENABLE                                         (0)
#define RSIP_CFG_HMAC_SHA512_ENABLE                                         (0)

#define RSIP_CFG_SECURE_BOOT_ENABLE                                         (1)
#define RSIP_CFG_FIRMWARE_UPDATE_ENABLE                                     (1)

#define RSIP_CFG_BYTE_SIZE_SHA_BLOCK_MAX                                    (256U)
#define RSIP_CFG_WORD_SIZE_SHA_INTERNAL_STATE_MAX                           (20U)
#define RSIP_CFG_BYTE_SIZE_HMAC_BLOCK_MAX                                   (128U)
#define RSIP_CFG_WORD_SIZE_HMAC_INTERNAL_STATE_MAX                          (20U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_HMAC_MAX                             RSIP_BYTE_SIZE_WRAPPED_KEY_HMAC_SHA256
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_KEY_UPDATE_KEY                 (64U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_UFPK                           (32U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_128                        (36U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_192                        (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_AES_256                        (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_XTS_AES_128                    (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_XTS_AES_256                    (84U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256R1_PUBLIC           (84U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP384R1_PUBLIC           (116U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP521R1_PUBLIC           (180U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256K1_PUBLIC           (84U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP256R1_PUBLIC     (84U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP384R1_PUBLIC     (116U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP512R1_PUBLIC     (148U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_EDWARDS25519_PUBLIC        (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256R1_PRIVATE          (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP384R1_PRIVATE          (68U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP521R1_PRIVATE          (100U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_SECP256K1_PRIVATE          (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP256R1_PRIVATE    (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP384R1_PRIVATE    (68U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_BRAINPOOLP512R1_PRIVATE    (84U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_ECC_EDWARDS25519_PRIVATE       (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_1024_PUBLIC                (164U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_2048_PUBLIC                (292U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_3072_PUBLIC                (420U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_4096_PUBLIC                (548U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_1024_PRIVATE               (276U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_2048_PRIVATE               (532U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_3072_PRIVATE               (788U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_RSA_4096_PRIVATE               (1044U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA1                      (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA224                    (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA256                    (52U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA384                    (68U)
#define RSIP_CFG_BYTE_SIZE_WRAPPED_KEY_VALUE_HMAC_SHA512                    (84U)

#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_192_QX                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_192_QY                     (28U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_224_QX                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_224_QY                     (32U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_256_QX                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_256_QY                     (36U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_384_QX                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_384_QY                     (52U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_512_QX                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_512_QY                     (68U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_521_QX                     (18U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_521_QY                     (98U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_ECC_EDWARDS25519_Q             (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_1024_N                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_1024_E                     (132U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_2048_N                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_2048_E                     (260U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_3072_N                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_3072_E                     (388U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_4096_N                     (4U)
#define RSIP_CFG_BYTE_POS_WRAPPED_PUBLIC_KEY_RSA_4096_E                     (516U)
/***********************************************************************************************************************
 * Device-dependent
 **********************************************************************************************************************/

#define AES_CCM_SUPPORTED                      (1)

#define SECP384R1_SUPPORTED                    (0)
#define SECP521R1_SUPPORTED                    (0)
#define BRAINPOOLP256R1_SUPPORTED              (1)
#define SECP256K1_SUPPORTED                    (1)

#define RSA_1024_SUPPORTED                     (0)
#define RSA_2048_SUPPORTED                     (0)
#define RSA_3072_SUPPORTED                     (0)
#define RSA_4096_SUPPORTED                     (0)

#define SHA1_SUPPORTED                         (0)
#define SHA224_SUPPORTED                       (1)
#define SHA256_SUPPORTED                       (1)
#define SHA384_SUPPORTED                       (0)
#define SHA512_SUPPORTED                       (0)
#define SHA512_224_SUPPORTED                   (0)
#define SHA512_256_SUPPORTED                   (0)

#define HMAC_SHA1_SUPPORTED                    (0)
#define HMAC_SHA224_SUPPORTED                  (1)
#define HMAC_SHA256_SUPPORTED                  (1)
#define KDF_HMAC_SHA256_SUPPORTED              (0)

#define FWUP_SUPPORTED                         (1)

/* Key update. */
#define RSIP_INSTALL_KEY_RING_INDEX            (0)

#endif /* R_RSIP_RX_CONFIG_HEADER_FILE */
