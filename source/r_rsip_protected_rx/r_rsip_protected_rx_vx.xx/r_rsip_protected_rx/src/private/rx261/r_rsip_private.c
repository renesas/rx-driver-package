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
* File Name    : r_rsip_private.c
* Description  : Interface definition for the RSIP private layer.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "r_rsip_protected_rx_if.h"
#include "r_rsip_public.h"
#include "r_rsip_private.h"
#include "r_rsip_wrapper.h"
#include "../../primitive/rx261/r_rsip_primitive.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* For HMAC-SHA */
#define RSIP_PRV_WORD_SIZE_HMAC_MAC_BUFFER                  (8U)

/* For AES-ECB/CBC/CTR */
#define RSIP_PRV_CMD_AES_CIPHER_ECB_ENCRYPT    (0U)
#define RSIP_PRV_CMD_AES_CIPHER_ECB_DECRYPT    (1U)
#define RSIP_PRV_CMD_AES_CIPHER_CBC_ENCRYPT    (2U)
#define RSIP_PRV_CMD_AES_CIPHER_CBC_DECRYPT    (3U)
#define RSIP_PRV_CMD_AES_CIPHER_CTR_CRYPT      (4U)

/* Primitive function names */
#if RSIP_CFG_AES_128_ENABLE
    #define RSIP_PRV_FUNC_KEY_GENERATE_AES_128                 r_rsip_p07
    #define RSIP_PRV_FUNC_KEY_WRAP_AES_128                     r_rsip_wrapper_p6f_aes128
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_AES_128                r_rsip_p6e_aes128
    #define RSIP_PRV_FUNC_RFC3394_AES_128_KEY_WRAP             r_rsip_wrapper_p8f_aes128
    #define RSIP_PRV_FUNC_RFC3394_AES_128_KEY_UNWRAP           r_rsip_wrapper_p90_aes128
    #if RSIP_CFG_AES_ECB_CBC_CTR_ENABLE
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_128         r_rsip_wrapper_p47i_aes128ecb_encrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_128         r_rsip_wrapper_p47i_aes128ecb_decrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_128         r_rsip_wrapper_p47i_aes128cbc_encrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_128         r_rsip_wrapper_p47i_aes128cbc_decrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_128             r_rsip_wrapper_p47i_aes128ctr_crypt
        #define RSIP_PRV_FUNC_AES_CIPHER_UPDATE_128               r_rsip_p47u
        #define RSIP_PRV_FUNC_AES_CIPHER_FINAL_128                r_rsip_p47f
    #else
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_128         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_128         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_128         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_128         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_128             NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_UPDATE_128               NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_FINAL_128                NULL
    #endif
#else
    #define RSIP_PRV_FUNC_KEY_GENERATE_AES_128                 NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_AES_128                     NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_AES_128                NULL
    #define RSIP_PRV_FUNC_RFC3394_AES_128_KEY_WRAP             NULL
    #define RSIP_PRV_FUNC_RFC3394_AES_128_KEY_UNWRAP           NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_128          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_128          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_128          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_128          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_128              NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_UPDATE_128                NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_FINAL_128                 NULL
#endif
#if RSIP_CFG_AES_GCM_ENABLE
    #define RSIP_PRV_FUNC_AES_GCM_ENC_INIT_128                 r_rsip_wrapper_p29i_aes128gcm_encrypt
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_AAD_128           r_rsip_p29a
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_TRANSITION_128    r_rsip_p29t
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_128               r_rsip_p29u
    #define RSIP_PRV_FUNC_AES_GCM_ENC_FINAL_128                r_rsip_p29f
    #define RSIP_PRV_FUNC_AES_GCM_DEC_INIT_128                 r_rsip_wrapper_p32i_aes128gcm_decrypt
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_AAD_128           r_rsip_p32a
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_TRANSITION_128    r_rsip_p32t
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_128               r_rsip_p32u
    #define RSIP_PRV_FUNC_AES_GCM_DEC_FINAL_128                r_rsip_p32f
#else
    #define RSIP_PRV_FUNC_AES_GCM_ENC_INIT_128                 NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_AAD_128           NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_TRANSITION_128    NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_128               NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_FINAL_128                NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_INIT_128                 NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_AAD_128           NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_TRANSITION_128    NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_128               NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_FINAL_128                NULL
#endif
#if RSIP_CFG_AES_CCM_ENABLE
    #define RSIP_PRV_FUNC_AES_CCM_ENC_INIT_128                 r_rsip_wrapper_p95i_aes128ccm_encrypt
    #define RSIP_PRV_FUNC_AES_CCM_ENC_UPDATE_128               r_rsip_p95u
    #define RSIP_PRV_FUNC_AES_CCM_ENC_FINAL_128                r_rsip_wrapper_p95f_aes128ccm_encrypt
    #define RSIP_PRV_FUNC_AES_CCM_DEC_INIT_128                 r_rsip_wrapper_p98i_aes128ccm_decrypt
    #define RSIP_PRV_FUNC_AES_CCM_DEC_UPDATE_128               r_rsip_p98u
    #define RSIP_PRV_FUNC_AES_CCM_DEC_FINAL_128                r_rsip_wrapper_p98f_aes128ccm_decrypt
#else
    #define RSIP_PRV_FUNC_AES_CCM_ENC_INIT_128                 NULL
    #define RSIP_PRV_FUNC_AES_CCM_ENC_UPDATE_128               NULL
    #define RSIP_PRV_FUNC_AES_CCM_ENC_FINAL_128                NULL
    #define RSIP_PRV_FUNC_AES_CCM_DEC_INIT_128                 NULL
    #define RSIP_PRV_FUNC_AES_CCM_DEC_UPDATE_128               NULL
    #define RSIP_PRV_FUNC_AES_CCM_DEC_FINAL_128                NULL
#endif
#if RSIP_CFG_AES_CMAC_ENABLE
    #define RSIP_PRV_FUNC_AES_MAC_INIT_128                     r_rsip_wrapper_p41i_aes128mac
    #define RSIP_PRV_FUNC_AES_MAC_UPDATE_128                   r_rsip_p41u
    #define RSIP_PRV_FUNC_AES_MAC_GENERATE_FINAL_128           r_rsip_wrapper_p41f_aes128mac_generate
    #define RSIP_PRV_FUNC_AES_MAC_VERIFY_FINAL_128             r_rsip_wrapper_p41f_aes128mac_verify
#else
    #define RSIP_PRV_FUNC_AES_MAC_INIT_128                     NULL
    #define RSIP_PRV_FUNC_AES_MAC_UPDATE_128                   NULL
    #define RSIP_PRV_FUNC_AES_MAC_GENERATE_FINAL_128           NULL
    #define RSIP_PRV_FUNC_AES_MAC_VERIFY_FINAL_128             NULL
#endif

#if RSIP_CFG_AES_256_ENABLE
    #define RSIP_PRV_FUNC_KEY_GENERATE_AES_256                 r_rsip_p08
    #define RSIP_PRV_FUNC_KEY_WRAP_AES_256                     r_rsip_wrapper_p6f_aes256
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_AES_256                r_rsip_p6e_aes256
    #define RSIP_PRV_FUNC_KEY_WRAP_AES_256_KUK                 r_rsip_p6e_aes256_KUK
    #define RSIP_PRV_FUNC_RFC3394_AES_256_KEY_WRAP             r_rsip_wrapper_p8f_aes256
    #define RSIP_PRV_FUNC_RFC3394_AES_256_KEY_UNWRAP           r_rsip_wrapper_p90_aes256
    #if RSIP_CFG_AES_ECB_CBC_CTR_ENABLE
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_256         r_rsip_wrapper_p50i_aes256ecb_encrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_256         r_rsip_wrapper_p50i_aes256ecb_decrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_256         r_rsip_wrapper_p50i_aes256cbc_encrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_256         r_rsip_wrapper_p50i_aes256cbc_decrypt
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_256             r_rsip_wrapper_p50i_aes256ctr_crypt
        #define RSIP_PRV_FUNC_AES_CIPHER_UPDATE_256               r_rsip_p50u
        #define RSIP_PRV_FUNC_AES_CIPHER_FINAL_256                r_rsip_p50f
    #else
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_256         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_256         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_256         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_256         NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_256             NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_UPDATE_256               NULL
        #define RSIP_PRV_FUNC_AES_CIPHER_FINAL_256                NULL
    #endif
#else
    #define RSIP_PRV_FUNC_KEY_GENERATE_AES_256                 NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_AES_256                     NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_AES_256                NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_AES_256_KUK                 NULL
    #define RSIP_PRV_FUNC_RFC3394_AES_256_KEY_WRAP             NULL
    #define RSIP_PRV_FUNC_RFC3394_AES_256_KEY_UNWRAP           NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_256          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_256          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_256          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_256          NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_256              NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_UPDATE_256                NULL
    #define RSIP_PRV_FUNC_AES_CIPHER_FINAL_256                 NULL
#endif
#if RSIP_CFG_AES_GCM_ENABLE
    #define RSIP_PRV_FUNC_AES_GCM_ENC_INIT_256                 r_rsip_wrapper_p34i_aes256gcm_encrypt
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_AAD_256           r_rsip_p34a
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_TRANSITION_256    r_rsip_p34t
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_256               r_rsip_p34u
    #define RSIP_PRV_FUNC_AES_GCM_ENC_FINAL_256                r_rsip_p34f
    #define RSIP_PRV_FUNC_AES_GCM_DEC_INIT_256                 r_rsip_wrapper_p36i_aes256gcm_decrypt
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_AAD_256           r_rsip_p36a
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_TRANSITION_256    r_rsip_p36t
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_256               r_rsip_p36u
    #define RSIP_PRV_FUNC_AES_GCM_DEC_FINAL_256                r_rsip_p36f
#else
    #define RSIP_PRV_FUNC_AES_GCM_ENC_INIT_256                 NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_AAD_256           NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_TRANSITION_256    NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_256               NULL
    #define RSIP_PRV_FUNC_AES_GCM_ENC_FINAL_256                NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_INIT_256                 NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_AAD_256           NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_TRANSITION_256    NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_256               NULL
    #define RSIP_PRV_FUNC_AES_GCM_DEC_FINAL_256                NULL
#endif
#if RSIP_CFG_AES_CCM_ENABLE
    #define RSIP_PRV_FUNC_AES_CCM_ENC_INIT_256                 r_rsip_wrapper_pa1i_aes256ccm_encrypt
    #define RSIP_PRV_FUNC_AES_CCM_ENC_UPDATE_256               r_rsip_pa1u
    #define RSIP_PRV_FUNC_AES_CCM_ENC_FINAL_256                r_rsip_pa1f
    #define RSIP_PRV_FUNC_AES_CCM_DEC_INIT_256                 r_rsip_wrapper_pa4i_aes256ccm_decrypt
    #define RSIP_PRV_FUNC_AES_CCM_DEC_UPDATE_256               r_rsip_pa4u
    #define RSIP_PRV_FUNC_AES_CCM_DEC_FINAL_256                r_rsip_pa4f
#else
    #define RSIP_PRV_FUNC_AES_CCM_ENC_INIT_256                 NULL
    #define RSIP_PRV_FUNC_AES_CCM_ENC_UPDATE_256               NULL
    #define RSIP_PRV_FUNC_AES_CCM_ENC_FINAL_256                NULL
    #define RSIP_PRV_FUNC_AES_CCM_DEC_INIT_256                 NULL
    #define RSIP_PRV_FUNC_AES_CCM_DEC_UPDATE_256               NULL
    #define RSIP_PRV_FUNC_AES_CCM_DEC_FINAL_256                NULL
#endif
#if RSIP_CFG_AES_CMAC_ENABLE
    #define RSIP_PRV_FUNC_AES_MAC_INIT_256                     r_rsip_wrapper_p44i_aes256mac
    #define RSIP_PRV_FUNC_AES_MAC_UPDATE_256                   r_rsip_p44u
    #define RSIP_PRV_FUNC_AES_MAC_GENERATE_FINAL_256           r_rsip_wrapper_p44f_aes256mac_generate
    #define RSIP_PRV_FUNC_AES_MAC_VERIFY_FINAL_256             r_rsip_wrapper_p44f_aes256mac_verify
#else
    #define RSIP_PRV_FUNC_AES_MAC_INIT_256                     NULL
    #define RSIP_PRV_FUNC_AES_MAC_UPDATE_256                   NULL
    #define RSIP_PRV_FUNC_AES_MAC_GENERATE_FINAL_256           NULL
    #define RSIP_PRV_FUNC_AES_MAC_VERIFY_FINAL_256             NULL
#endif

#if RSIP_CFG_ECC_SECP256R1_ENABLE
    #define RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_SECP256R1       r_rsip_wrapper_pf4_secp256r1
    #define RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_BRAINPOOLP256R1 r_rsip_wrapper_pf4_brainpoolp256r1
    #define RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_SECP256K1       r_rsip_wrapper_pf4_secp256k1
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256R1_PUBLIC         r_rsip_wrapper_p6f_secp256r1_pub
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256R1_PRIVATE        r_rsip_wrapper_p6f_secp256r1_priv
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_BRAINPOOLP256R1_PUBLIC   r_rsip_wrapper_p6f_brainpoolp256r1_pub
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_BRAINPOOLP256R1_PRIVATE  r_rsip_wrapper_p6f_brainpoolp256r1_priv
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256K1_PUBLIC         r_rsip_wrapper_p6f_secp256k1_pub
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256K1_PRIVATE        r_rsip_wrapper_p6f_secp256k1_priv

    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256_PUBLIC         r_rsip_p6e_eccp256_public
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256_PRIVATE        r_rsip_p6e_eccp256_private
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256R1_PUBLIC       r_rsip_p6e_eccp256r1_public
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256R1_PRIVATE      r_rsip_p6e_eccp256r1_private
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_SECP256K1_PUBLIC    r_rsip_p6e_eccsecp256k1_public
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_SECP256K1_PRIVATE   r_rsip_p6e_eccsecp256k1_private
    #define RSIP_PRV_FUNC_ECDSA_SIGN_SECP256R1                  r_rsip_wrapper_pf0_secp256r1
    #define RSIP_PRV_FUNC_ECDSA_SIGN_BRAINPOOLP256R1            r_rsip_wrapper_pf0_brainpoolp256r1
    #define RSIP_PRV_FUNC_ECDSA_SIGN_SECP256K1                  r_rsip_wrapper_pf0_secp256k1
    #define RSIP_PRV_FUNC_ECDSA_VERIFY_SECP256R1                r_rsip_wrapper_pf1_secp256r1
    #define RSIP_PRV_FUNC_ECDSA_VERIFY_BRAINPOOLP256R1          r_rsip_wrapper_pf1_brainpoolp256r1
    #define RSIP_PRV_FUNC_ECDSA_VERIFY_SECP256K1                r_rsip_wrapper_pf1_secp256k1
#else
    #define RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_SECP256R1       NULL
    #define RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_BRAINPOOLP256R1 NULL
    #define RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_SECP256K1       NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256R1_PUBLIC         NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256R1_PRIVATE        NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_BRAINPOOLP256R1_PUBLIC   NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_BRAINPOOLP256R1_PRIVATE  NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256K1_PUBLIC         NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256K1_PRIVATE        NULL

    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256_PUBLIC         NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256_PRIVATE        NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256R1_PUBLIC       NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256R1_PRIVATE      NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_SECP256K1_PUBLIC    NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_SECP256K1_PRIVATE   NULL
    #define RSIP_PRV_FUNC_ECDSA_SIGN_SECP256R1                  NULL
    #define RSIP_PRV_FUNC_ECDSA_SIGN_BRAINPOOLP256R1            NULL
    #define RSIP_PRV_FUNC_ECDSA_SIGN_SECP256K1                  NULL
    #define RSIP_PRV_FUNC_ECDSA_VERIFY_SECP256R1                NULL
    #define RSIP_PRV_FUNC_ECDSA_VERIFY_BRAINPOOLP256R1          NULL
    #define RSIP_PRV_FUNC_ECDSA_VERIFY_SECP256K1                NULL
#endif

#if RSIP_CFG_HMAC_SHA224_ENABLE
    #define RSIP_PRV_FUNC_KEY_GENERATE_HMAC_SHA224             r_rsip_p0a
    #define RSIP_PRV_FUNC_KEY_WRAP_HMAC_SHA224                 r_rsip_wrapper_p6f_hmacsha224
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_HMAC_SHA224            r_rsip_p6e_hmacsha224
#else
    #define RSIP_PRV_FUNC_KEY_GENERATE_HMAC_SHA224             NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_HMAC_SHA224                 NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_HMAC_SHA224            NULL
#endif

#if RSIP_CFG_HMAC_SHA256_ENABLE
    #define RSIP_PRV_FUNC_KEY_GENERATE_HMAC_SHA256             r_rsip_p0b
    #define RSIP_PRV_FUNC_KEY_WRAP_HMAC_SHA256                 r_rsip_wrapper_p6f_hmacsha256
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_HMAC_SHA256            r_rsip_p6e_hmacsha256
#else
    #define RSIP_PRV_FUNC_KEY_GENERATE_HMAC_SHA256             NULL
    #define RSIP_PRV_FUNC_KEY_WRAP_HMAC_SHA256                 NULL
    #define RSIP_PRV_FUNC_INIT_KEY_WRAP_HMAC_SHA256            NULL
#endif

#if RSIP_CFG_FIRMWARE_UPDATE_ENABLE
    #define RSIP_PRV_FUNC_FIRMWARE_UPDATE_INIT                  r_rsip_p23i
    #define RSIP_PRV_FUNC_FIRMWARE_UPDATE_UPDATE                r_rsip_p23u
    #define RSIP_PRV_FUNC_FIRMWARE_UPDATE_FINAL                 r_rsip_p23f
#else
    #define RSIP_PRV_FUNC_FIRMWARE_UPDATE_INIT                  NULL
    #define RSIP_PRV_FUNC_FIRMWARE_UPDATE_UPDATE                NULL
    #define RSIP_PRV_FUNC_FIRMWARE_UPDATE_FINAL                 NULL
#endif

#if RSIP_CFG_SECURE_BOOT_ENABLE
    #define RSIP_PRV_FUNC_SECURE_BOOT_INIT                  r_rsip_p26i
    #define RSIP_PRV_FUNC_SECURE_BOOT_UPDATE                r_rsip_p26u
    #define RSIP_PRV_FUNC_SECURE_BOOT_FINAL                 r_rsip_p26f
#else
    #define RSIP_PRV_FUNC_SECURE_BOOT_INIT                  NULL
    #define RSIP_PRV_FUNC_SECURE_BOOT_UPDATE                NULL
    #define RSIP_PRV_FUNC_SECURE_BOOT_FINAL                 NULL
#endif

#define RSIP_PRV_FUNC_RANDOM_NUMBER_GENERATE                r_rsip_p20
#define RSIP_PRV_FUNC_GHASH_COMPUTE                         r_rsip_p21

/* Block length (in bytes) of SHA */
#define SHA_BLOCK8_LEN          (64u)
#define HMAC_LAST_BLOCK_ADD_LEN (64u)
#define SHA_STOP_BIT            (0x80)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum e_rsip_hmac_cmd
{
    RSIP_HMAC_CMD_SIGN   = 0U,
    RSIP_HMAC_CMD_VERIFY = 1U,
} rsip_hmac_cmd_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void set_sha2_final_block (uint8_t * p_message, uint32_t message_length, uint32_t total_message_length,
        uint8_t * p_last_block, uint32_t * p_last_block_length);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
uint32_t const * S_INST2;
uint32_t INST_DATA_SIZE;
uint32_t KEY_INDEX_SIZE;
uint32_t WRAPPED_KEY_SIZE;

const bool g_sha_enabled[RSIP_HASH_TYPE_NUM] =
{
    [RSIP_HASH_TYPE_SHA224] = RSIP_CFG_SHA224_ENABLE,
    [RSIP_HASH_TYPE_SHA256] = RSIP_CFG_SHA256_ENABLE,
};

RSIP_SEC_C_SECURE_BOOT
static const rsip_func_initial_key_wrap_t gsp_func_initial_key_wrap_aes[RSIP_KEY_AES_NUM] =
{
    [RSIP_KEY_AES_128] = RSIP_PRV_FUNC_INIT_KEY_WRAP_AES_128,
    [RSIP_KEY_AES_256] = RSIP_PRV_FUNC_INIT_KEY_WRAP_AES_256
};
RSIP_SEC_DEFAULT

static const rsip_func_initial_key_wrap_t gsp_func_initial_key_wrap_ecc_pub[RSIP_KEY_ECC_NUM] =
{
    [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256_PUBLIC,
    [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_SECP256K1_PUBLIC,
    [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256R1_PUBLIC
};

static const rsip_func_initial_key_wrap_t gsp_func_initial_key_wrap_ecc_priv[RSIP_KEY_ECC_NUM] =
{
    [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256_PRIVATE,
    [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_SECP256K1_PRIVATE,
    [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_INIT_KEY_WRAP_ECC_P256R1_PRIVATE
};

static const rsip_func_initial_key_wrap_t gsp_func_initial_key_wrap_hmac[RSIP_KEY_HMAC_NUM] =
{
    [RSIP_KEY_HMAC_SHA224] = RSIP_PRV_FUNC_INIT_KEY_WRAP_HMAC_SHA224,
    [RSIP_KEY_HMAC_SHA256] = RSIP_PRV_FUNC_INIT_KEY_WRAP_HMAC_SHA256
};

static const uint32_t gs_hmac_cmd[] =
{
    [RSIP_HMAC_CMD_SIGN]   = BSWAP_32BIG_C(0U),
    [RSIP_HMAC_CMD_VERIFY] = BSWAP_32BIG_C(1U),
};

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
const bool g_hmac_enabled[RSIP_KEY_HMAC_NUM] =
{
    [RSIP_KEY_HMAC_SHA224] = RSIP_CFG_HMAC_SHA224_ENABLE,
    [RSIP_KEY_HMAC_SHA256] = RSIP_CFG_HMAC_SHA256_ENABLE
};

RSIP_SEC_C_SECURE_BOOT
const rsip_func_t g_func =
{
    .p_key_generate_aes            =
    {
        [RSIP_KEY_AES_128] = RSIP_PRV_FUNC_KEY_GENERATE_AES_128,
        [RSIP_KEY_AES_256] = RSIP_PRV_FUNC_KEY_GENERATE_AES_256
    },
    .p_key_generate_hmac           =
    {
        [RSIP_KEY_HMAC_SHA224] = RSIP_PRV_FUNC_KEY_GENERATE_HMAC_SHA224,
        [RSIP_KEY_HMAC_SHA256] = RSIP_PRV_FUNC_KEY_GENERATE_HMAC_SHA256
    },
    .p_key_pair_generate_ecc       =
    {
        [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_SECP256R1,
        [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_BRAINPOOLP256R1,
        [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_KEY_PAIR_GENERATE_ECC_SECP256K1,
    },
    .p_encrypted_key_wrap_aes     =
    {
        [RSIP_KEY_AES_128] = RSIP_PRV_FUNC_KEY_WRAP_AES_128,
        [RSIP_KEY_AES_256] = RSIP_PRV_FUNC_KEY_WRAP_AES_256
    },
    .p_initial_key_update_key_wrap_aes      =
    {
        RSIP_PRV_FUNC_KEY_WRAP_AES_256_KUK
    },
    .p_encrypted_key_wrap_ecc_pub  =
    {
        [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256R1_PUBLIC,
        [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_KEY_WRAP_ECC_BRAINPOOLP256R1_PUBLIC,
        [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256K1_PUBLIC,
    },
    .p_encrypted_key_wrap_ecc_priv =
    {
        [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256R1_PRIVATE,
        [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_KEY_WRAP_ECC_BRAINPOOLP256R1_PRIVATE,
        [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_KEY_WRAP_ECC_SECP256K1_PRIVATE,
    },
    .p_rfc3394_key_wrap            =
    {
        [RSIP_KEY_AES_128] = RSIP_PRV_FUNC_RFC3394_AES_128_KEY_WRAP,
        [RSIP_KEY_AES_256] = RSIP_PRV_FUNC_RFC3394_AES_256_KEY_WRAP
    },
    .p_rfc3394_key_unwrap          =
    {
        [RSIP_KEY_AES_128] = RSIP_PRV_FUNC_RFC3394_AES_128_KEY_UNWRAP,
        [RSIP_KEY_AES_256] = RSIP_PRV_FUNC_RFC3394_AES_256_KEY_UNWRAP
    },
    .p_aes_cipher                  =
    {
        [RSIP_KEY_AES_128] =
        {
            .p_init_ecb_enc = RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_128,
            .p_init_ecb_dec = RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_128,
            .p_init_cbc_enc = RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_128,
            .p_init_cbc_dec = RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_128,
            .p_init_ctr = RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_128,
            .p_update = RSIP_PRV_FUNC_AES_CIPHER_UPDATE_128,
            .p_final = RSIP_PRV_FUNC_AES_CIPHER_FINAL_128,
        },
        [RSIP_KEY_AES_256] =
        {
            .p_init_ecb_enc = RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_ENC_256,
            .p_init_ecb_dec = RSIP_PRV_FUNC_AES_CIPHER_INIT_ECB_DEC_256,
            .p_init_cbc_enc = RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_ENC_256,
            .p_init_cbc_dec = RSIP_PRV_FUNC_AES_CIPHER_INIT_CBC_DEC_256,
            .p_init_ctr = RSIP_PRV_FUNC_AES_CIPHER_INIT_CTR_256,
            .p_update = RSIP_PRV_FUNC_AES_CIPHER_UPDATE_256,
            .p_final = RSIP_PRV_FUNC_AES_CIPHER_FINAL_256,
        }
    },
    .p_aes_gcm_enc =
        {
        [RSIP_KEY_AES_128] =
        {
            .p_init = RSIP_PRV_FUNC_AES_GCM_ENC_INIT_128,
            .p_updateAad = RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_AAD_128,
            .p_updateTransition = RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_TRANSITION_128,
            .p_update = RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_128,
            .p_encryptFinal = RSIP_PRV_FUNC_AES_GCM_ENC_FINAL_128
        },
        [RSIP_KEY_AES_256] =
        {
            .p_init = RSIP_PRV_FUNC_AES_GCM_ENC_INIT_256,
            .p_updateAad = RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_AAD_256,
            .p_updateTransition = RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_TRANSITION_256,
            .p_update = RSIP_PRV_FUNC_AES_GCM_ENC_UPDATE_256,
            .p_encryptFinal = RSIP_PRV_FUNC_AES_GCM_ENC_FINAL_256
        }
    },
    .p_aes_gcm_dec =
    {
        [RSIP_KEY_AES_128] =
        {
            .p_init = RSIP_PRV_FUNC_AES_GCM_DEC_INIT_128,
            .p_updateAad = RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_AAD_128,
            .p_updateTransition = RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_TRANSITION_128,
            .p_update = RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_128,
            .p_decryptFinal = RSIP_PRV_FUNC_AES_GCM_DEC_FINAL_128
        },
        [RSIP_KEY_AES_256] =
        {
            .p_init = RSIP_PRV_FUNC_AES_GCM_DEC_INIT_256,
            .p_updateAad = RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_AAD_256,
            .p_updateTransition = RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_TRANSITION_256,
            .p_update = RSIP_PRV_FUNC_AES_GCM_DEC_UPDATE_256,
            .p_decryptFinal = RSIP_PRV_FUNC_AES_GCM_DEC_FINAL_256
        }
    },
    .p_aes_ccm_enc =
    {
        [RSIP_KEY_AES_128] =
        {
            .p_encryptInit = RSIP_PRV_FUNC_AES_CCM_ENC_INIT_128,
            .p_update = RSIP_PRV_FUNC_AES_CCM_ENC_UPDATE_128,
            .p_encryptFinal = RSIP_PRV_FUNC_AES_CCM_ENC_FINAL_128
        },
        [RSIP_KEY_AES_256] =
        {
            .p_encryptInit = RSIP_PRV_FUNC_AES_CCM_ENC_INIT_256,
            .p_update = RSIP_PRV_FUNC_AES_CCM_ENC_UPDATE_256,
            .p_encryptFinal = RSIP_PRV_FUNC_AES_CCM_ENC_FINAL_256
        }
    },
    .p_aes_ccm_dec =
    {
        [RSIP_KEY_AES_128] =
        {
            .p_decryptInit = RSIP_PRV_FUNC_AES_CCM_DEC_INIT_128,
            .p_update = RSIP_PRV_FUNC_AES_CCM_DEC_UPDATE_128,
            .p_decryptFinal = RSIP_PRV_FUNC_AES_CCM_DEC_FINAL_128
        },
        [RSIP_KEY_AES_256] =
        {
            .p_decryptInit = RSIP_PRV_FUNC_AES_CCM_DEC_INIT_256,
            .p_update = RSIP_PRV_FUNC_AES_CCM_DEC_UPDATE_256,
            .p_decryptFinal = RSIP_PRV_FUNC_AES_CCM_DEC_FINAL_256
        }
    },
    .p_aes_mac =
    {
        [RSIP_KEY_AES_128] =
        {
            .p_init = RSIP_PRV_FUNC_AES_MAC_INIT_128,
            .p_update = RSIP_PRV_FUNC_AES_MAC_UPDATE_128,
            .p_generateFinal = RSIP_PRV_FUNC_AES_MAC_GENERATE_FINAL_128,
            .p_verifyFinal = RSIP_PRV_FUNC_AES_MAC_VERIFY_FINAL_128
        },
        [RSIP_KEY_AES_256] =
        {
            .p_init = RSIP_PRV_FUNC_AES_MAC_INIT_256,
            .p_update = RSIP_PRV_FUNC_AES_MAC_UPDATE_256,
            .p_generateFinal = RSIP_PRV_FUNC_AES_MAC_GENERATE_FINAL_256,
            .p_verifyFinal = RSIP_PRV_FUNC_AES_MAC_VERIFY_FINAL_256
        }
    },
    .p_ecdsa_sign =
    {
        [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_ECDSA_SIGN_SECP256R1,
        [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_ECDSA_SIGN_BRAINPOOLP256R1,
        [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_ECDSA_SIGN_SECP256K1
    },
    .p_ecdsa_verify =
    {
        [RSIP_KEY_ECC_SECP256R1] = RSIP_PRV_FUNC_ECDSA_VERIFY_SECP256R1,
        [RSIP_KEY_ECC_BRAINPOOLP256R1] = RSIP_PRV_FUNC_ECDSA_VERIFY_BRAINPOOLP256R1,
        [RSIP_KEY_ECC_SECP256K1] = RSIP_PRV_FUNC_ECDSA_VERIFY_SECP256K1
    },
    .p_encrypted_key_wrap_hmac =
    {
        [RSIP_KEY_HMAC_SHA224] = RSIP_PRV_FUNC_KEY_WRAP_HMAC_SHA224,
        [RSIP_KEY_HMAC_SHA256] = RSIP_PRV_FUNC_KEY_WRAP_HMAC_SHA256
    },
    .p_rng           = RSIP_PRV_FUNC_RANDOM_NUMBER_GENERATE,
    .p_ghash_compute = RSIP_PRV_FUNC_GHASH_COMPUTE,
    .p_fwup =
    {
        .p_init          = RSIP_PRV_FUNC_FIRMWARE_UPDATE_INIT,
        .p_update        = RSIP_PRV_FUNC_FIRMWARE_UPDATE_UPDATE,
        .p_final         = RSIP_PRV_FUNC_FIRMWARE_UPDATE_FINAL,
    },
    .p_sb =
    {
        .p_init          = RSIP_PRV_FUNC_SECURE_BOOT_INIT,
        .p_update        = RSIP_PRV_FUNC_SECURE_BOOT_UPDATE,
        .p_final         = RSIP_PRV_FUNC_SECURE_BOOT_FINAL,
    },
};
RSIP_SEC_DEFAULT
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
RSIP_SEC_P_SECURE_BOOT
rsip_err_t r_rsip_open(void)
{
    uint32_t indata_lc = RSIP_DUMMY_LC;
    rsip_err_t rsip_ret = RSIP_ERR_FAIL;

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Casting structure pointer is used for address. */
    MSTP_SECURITY = 0U;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    r_rsip_p00();

    rsip_ret = r_rsip_p81();

    if (RSIP_SUCCESS == rsip_ret)
    {
#if defined __LIT
        RSIP.REG_0018H.WORD = 0x006b006bU;
        RSIP.REG_001CH.WORD = 0x00eb00ebU;
#else
        RSIP.REG_0018H.WORD = R_BSP_REVL(0x006b006bU);
        RSIP.REG_001CH.WORD = R_BSP_REVL(0x00eb00ebU);
#endif  /* defined __LIT */
        rsip_ret = r_rsip_p82();
        if (RSIP_ERR_RETRY == rsip_ret)
        {
            rsip_ret = r_rsip_p82();
            if (RSIP_ERR_RETRY == rsip_ret)
            {
                rsip_ret = r_rsip_p82();
            }
        }
    }

    if (RSIP_SUCCESS == rsip_ret)
    {
        rsip_ret = r_rsip_p40(&indata_lc);
    }

    return rsip_ret;
}

RSIP_SEC_P_SECURE_BOOT
rsip_err_t r_rsip_close(void)
{
    r_rsip_p00();

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Casting structure pointer is used for address. */
    MSTP_SECURITY = 1U;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return RSIP_SUCCESS;
}

RSIP_SEC_P_SECURE_BOOT
rsip_err_t r_rsip_fwup_start(void)
{
    rsip_err_t rsip_ret = r_rsip_p22();

    return rsip_ret;
}
RSIP_SEC_DEFAULT
/***********************************************************************************************************************
 * Set KUK to S_INT2.
 *
 * @param[in] p_key_update_key_value KUK value.
 *
 * @return void.
 **********************************************************************************************************************/
void r_rsip_kuk_set(const uint8_t * p_key_update_key_value)
{
    S_INST2 = (uint32_t *) p_key_update_key_value;
}

/*******************************************************************************************************************//**
 * prepares to execute the SHA256 hash operation
 *
 * @param handle SHA256 handler (work area)
 *
 * @retval FSP_SUCCESS Normal termination.
 **********************************************************************************************************************/
rsip_err_t r_rsip_sha256initprivate(rsip_sha_handle_t * handle)
{
    uint32_t indata_sha256initval[8] =
    {
        0
    };

    FSP_PARAMETER_NOT_USED(handle);

    indata_sha256initval[0] = change_endian_long(0x6a09e667);
    indata_sha256initval[1] = change_endian_long(0xbb67ae85);
    indata_sha256initval[2] = change_endian_long(0x3c6ef372);
    indata_sha256initval[3] = change_endian_long(0xa54ff53a);
    indata_sha256initval[4] = change_endian_long(0x510e527f);
    indata_sha256initval[5] = change_endian_long(0x9b05688c);
    indata_sha256initval[6] = change_endian_long(0x1f83d9ab);
    indata_sha256initval[7] = change_endian_long(0x5be0cd19);

    memcpy(handle->current_hash, indata_sha256initval, RSIP_PRV_SHA224256_HASH_LENGTH_BYTE_SIZE);

    return RSIP_SUCCESS;
}

/*******************************************************************************************************************//**
 * prepares to execute the SHA256 hash operation
 *
 * @param handle SHA256 handler (work area)
 *
 * @retval FSP_SUCCESS Normal termination.
 **********************************************************************************************************************/
rsip_err_t r_rsip_sha224initprivate(rsip_sha_handle_t * handle)
{
    uint32_t indata_sha224initval[8] =
    {
        0
    };

    FSP_PARAMETER_NOT_USED(handle);

    indata_sha224initval[0] = change_endian_long(0xc1059ed8);
    indata_sha224initval[1] = change_endian_long(0x367cd507);
    indata_sha224initval[2] = change_endian_long(0x3070dd17);
    indata_sha224initval[3] = change_endian_long(0xf70e5939);
    indata_sha224initval[4] = change_endian_long(0xffc00b31);
    indata_sha224initval[5] = change_endian_long(0x68581511);
    indata_sha224initval[6] = change_endian_long(0x64f98fa7);
    indata_sha224initval[7] = change_endian_long(0xbefa4fa4);

    memcpy(handle->current_hash, indata_sha224initval, RSIP_PRV_SHA224256_HASH_LENGTH_BYTE_SIZE);

    return RSIP_SUCCESS;
}

rsip_err_t r_rsip_sha1sha2_init_update(uint8_t         * current_hash,
                                        uint8_t        * p_message,
                                        uint64_t         message_length,
                                        uint32_t       * internal_state)
{
    FSP_PARAMETER_NOT_USED(internal_state);

    return r_rsip_p72((uint32_t *) current_hash,
                        (const uint32_t *) p_message,
                        r_rsip_byte_to_word_convert((uint32_t) message_length),
                        (uint32_t*) current_hash);
}

rsip_err_t r_rsip_sha1sha2_update(uint8_t          * current_hash,
                                    uint8_t        * p_message,
                                    uint64_t         message_length,
                                    uint32_t       * internal_state)
{
    FSP_PARAMETER_NOT_USED(internal_state);

    return r_rsip_p72((uint32_t*) current_hash,
                    (const uint32_t*) p_message,
                    r_rsip_byte_to_word_convert((uint32_t) message_length),
                    (uint32_t*) current_hash);
}

rsip_err_t r_rsip_sha1sha2_init_final(uint8_t * current_hash,
                                        uint8_t * p_message,
                                        uint64_t message_length,
                                        uint8_t * p_digest)
{
    uint32_t set_prc_byte_size;
    set_sha2_final_block ((uint8_t *)p_message, message_length, message_length,
            (uint8_t *)p_message, &set_prc_byte_size);

    return r_rsip_p72((uint32_t*) current_hash,
                    (const uint32_t*) p_message,
                    r_rsip_byte_to_word_convert((uint32_t) set_prc_byte_size),
                    (uint32_t*) p_digest);
}

rsip_err_t r_rsip_sha1sha2_final(uint8_t       * current_hash,
                                uint8_t        * p_message,
                                uint64_t         message_length,
                                uint64_t         total_message_length,
                                uint8_t        * p_digest,
                                uint32_t       * internal_state)
{
    FSP_PARAMETER_NOT_USED(internal_state);
    FSP_PARAMETER_NOT_USED(total_message_length);

    uint32_t set_prc_byte_size;

    set_sha2_final_block ((uint8_t *)p_message, message_length, total_message_length,
            (uint8_t *)p_message, &set_prc_byte_size);

    return r_rsip_p72((uint32_t*) current_hash,
                        (const uint32_t*) p_message,
                        r_rsip_byte_to_word_convert((uint32_t) set_prc_byte_size),
                        (uint32_t*) p_digest);
}

rsip_err_t r_rsip_hmac_init_update(const rsip_wrapped_key_t * p_wrapped_key,
                                    uint8_t                 * p_message,
                                    uint64_t                   message_length,
                                    uint32_t                 * internal_state)
{
    rsip_err_t rsip_ret = RSIP_ERR_FAIL;
    FSP_PARAMETER_NOT_USED(internal_state);

    if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_pdci((uint32_t*)p_wrapped_key->value);
    }
    else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_p76i((uint32_t*)p_wrapped_key->value);
    }
    else
    {
        rsip_ret = RSIP_ERR_FAIL;
    }

    if (RSIP_SUCCESS == rsip_ret)
    {
        if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
        {
            /* Call primitive (cast to match the argument type with the primitive function) */
            r_rsip_pdcu((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)message_length));
            rsip_ret = RSIP_SUCCESS;
        }
        else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
        {
            /* Call primitive (cast to match the argument type with the primitive function) */
            r_rsip_p76u((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)message_length));
            rsip_ret = RSIP_SUCCESS;
        }
        else
        {
            rsip_ret = RSIP_ERR_FAIL;
        }
    }

    return rsip_ret;
}

rsip_err_t r_rsip_hmac_update(const rsip_wrapped_key_t   * p_wrapped_key,
                                uint8_t                  * p_message,
                                uint64_t                   message_length,
                                uint32_t                 * internal_state)
{
    FSP_PARAMETER_NOT_USED(p_wrapped_key);
    FSP_PARAMETER_NOT_USED(internal_state);

    if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        r_rsip_pdcu((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)message_length));
        return RSIP_SUCCESS;
    }
    else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        r_rsip_p76u((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)message_length));
        return RSIP_SUCCESS;
    }
    else
    {
        return RSIP_ERR_FAIL;
    }
}

rsip_err_t r_rsip_hmac_init_final(const rsip_wrapped_key_t   * p_wrapped_key,
                                    uint8_t                  * p_message,
                                    uint64_t                   message_length,
                                    uint8_t                  * p_mac)
{
    rsip_err_t rsip_ret = RSIP_ERR_FAIL;
    FSP_PARAMETER_NOT_USED(p_message);
    FSP_PARAMETER_NOT_USED(message_length);

    uint32_t set_prc_byte_size;

    if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_pdci((uint32_t*)p_wrapped_key->value);

    }
    else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_p76i((uint32_t*)p_wrapped_key->value);

    }
    else
    {
        rsip_ret = RSIP_ERR_FAIL;
    }


    if (RSIP_SUCCESS == rsip_ret)
    {
        set_sha2_final_block ((uint8_t *)p_message, message_length, message_length + HMAC_LAST_BLOCK_ADD_LEN,
                (uint8_t *)p_message, &set_prc_byte_size);
        if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
        {
            r_rsip_pdcu((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
            /* Call primitive (cast to match the argument type with the primitive function) */
            rsip_ret = r_rsip_pdcf(&gs_hmac_cmd[RSIP_HMAC_CMD_SIGN], NULL, NULL, (uint32_t*)p_mac);

        }
        else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
        {
            r_rsip_p76u((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
            /* Call primitive (cast to match the argument type with the primitive function) */
            rsip_ret = r_rsip_p76f(&gs_hmac_cmd[RSIP_HMAC_CMD_SIGN], NULL, NULL, (uint32_t*)p_mac);

        }
        else
        {
            rsip_ret = RSIP_ERR_FAIL;
        }
    }

    return rsip_ret;
}


rsip_err_t r_rsip_hmac_final(const rsip_wrapped_key_t    * p_wrapped_key,
                                uint8_t                  * p_message,
                                uint64_t                   message_length,
                                uint64_t                   total_message_length,
                                uint8_t                  * p_mac,
                                uint32_t                 * internal_state)
{
    rsip_err_t rsip_ret = RSIP_ERR_FAIL;
    FSP_PARAMETER_NOT_USED(p_message);
    FSP_PARAMETER_NOT_USED(message_length);
    FSP_PARAMETER_NOT_USED(total_message_length);
    FSP_PARAMETER_NOT_USED(internal_state);

    uint32_t set_prc_byte_size;

    set_sha2_final_block ((uint8_t *)p_message, message_length, total_message_length+HMAC_LAST_BLOCK_ADD_LEN,
            (uint8_t *)p_message, &set_prc_byte_size);

    if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
    {
        r_rsip_pdcu((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_pdcf(&gs_hmac_cmd[RSIP_HMAC_CMD_SIGN], NULL, NULL, (uint32_t*)p_mac);

    }
    else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
    {
        r_rsip_p76u((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_p76f(&gs_hmac_cmd[RSIP_HMAC_CMD_SIGN], NULL, NULL, (uint32_t*)p_mac);

    }
    else
    {
        rsip_ret = RSIP_ERR_FAIL;
    }

    return rsip_ret;
}

rsip_err_t r_rsip_hmac_init_verify(const rsip_wrapped_key_t  * p_wrapped_key,
                                    uint8_t                  * p_message,
                                    uint64_t                   message_length,
                                    const uint8_t            * p_mac,
                                    uint32_t                   mac_length)
{
    rsip_err_t rsip_ret = RSIP_ERR_FAIL;

    uint32_t InData_MAC[RSIP_PRV_WORD_SIZE_HMAC_MAC_BUFFER] =
    {
        0
    };
    uint32_t set_prc_byte_size;
    memcpy(InData_MAC, p_mac, mac_length);
    uint32_t mac_len[1] =
    {
        change_endian_long(mac_length)
    };

    if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_pdci((uint32_t*)p_wrapped_key->value);

    }
    else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
    {
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_p76i((uint32_t*)p_wrapped_key->value);

    }
    else
    {
        rsip_ret = RSIP_ERR_FAIL;
    }

    if (RSIP_SUCCESS == rsip_ret)
    {
        set_sha2_final_block ((uint8_t *)p_message, message_length, message_length+HMAC_LAST_BLOCK_ADD_LEN,
                        (uint8_t *)p_message, &set_prc_byte_size);
        if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
        {
            r_rsip_pdcu((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
            /* Call primitive (cast to match the argument type with the primitive function) */
            rsip_ret = r_rsip_pdcf(&gs_hmac_cmd[RSIP_HMAC_CMD_VERIFY], InData_MAC, mac_len, (uint32_t*)NULL);
        }
        else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
        {
            r_rsip_p76u((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
            /* Call primitive (cast to match the argument type with the primitive function) */
            rsip_ret = r_rsip_p76f(&gs_hmac_cmd[RSIP_HMAC_CMD_VERIFY], InData_MAC, mac_len, (uint32_t*)NULL);
        }
        else
        {
            rsip_ret = RSIP_ERR_FAIL;
        }
    }

    return rsip_ret;
}

rsip_err_t r_rsip_hmac_verify(const rsip_wrapped_key_t * p_wrapped_key,
                                uint8_t                  * p_message,
                                uint64_t                   message_length,
                                uint64_t                   total_message_length,
                                const uint8_t            * p_mac,
                                uint32_t                   mac_length,
                                uint32_t                 * internal_state)
{
    rsip_err_t rsip_ret = RSIP_ERR_FAIL;
    FSP_PARAMETER_NOT_USED(p_message);
    FSP_PARAMETER_NOT_USED(message_length);
    FSP_PARAMETER_NOT_USED(total_message_length);
    FSP_PARAMETER_NOT_USED(internal_state);

    uint32_t InData_MAC[RSIP_PRV_WORD_SIZE_HMAC_MAC_BUFFER] =
    {
        0
    };

    uint32_t mac_len[1] =
    {
        change_endian_long(mac_length)
    };
    uint32_t set_prc_byte_size;

    set_sha2_final_block ((uint8_t *)p_message, message_length, total_message_length+HMAC_LAST_BLOCK_ADD_LEN,
            (uint8_t *)p_message, &set_prc_byte_size);

    memcpy(InData_MAC, p_mac, mac_length);

    if (RSIP_KEY_HMAC_SHA224 == p_wrapped_key->subtype)
    {
        r_rsip_pdcu((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_pdcf(&gs_hmac_cmd[RSIP_HMAC_CMD_VERIFY], InData_MAC, mac_len, (uint32_t*)NULL);
    }
    else if (RSIP_KEY_HMAC_SHA256 == p_wrapped_key->subtype)
    {
        r_rsip_p76u((const uint32_t*)p_message, r_rsip_byte_to_word_convert((uint32_t)set_prc_byte_size));
        /* Call primitive (cast to match the argument type with the primitive function) */
        rsip_ret = r_rsip_p76f(&gs_hmac_cmd[RSIP_HMAC_CMD_VERIFY], InData_MAC, mac_len, (uint32_t*)NULL);
    }
    else
    {
        rsip_ret = RSIP_ERR_FAIL;
    }

    return rsip_ret;
}

RSIP_SEC_P_SECURE_BOOT
/*******************************************************************************************************************//**
 * Select primitive function of initial key wrapping from key type.
 *
 * @param[in] key_type Key type.
 *
 * @return Pointer to function.
 **********************************************************************************************************************/
rsip_func_initial_key_wrap_t select_func_initial_key_wrap(rsip_key_type_t key_type)
{
    rsip_func_initial_key_wrap_t ret = NULL;
    uint8_t alg     = r_rsip_key_type_to_alg(key_type);
    uint8_t subtype = r_rsip_key_type_to_subtype(key_type);

    switch (alg)
    {
        case RSIP_ALG_AES:
        {
            ret = gsp_func_initial_key_wrap_aes[subtype];
            break;
        }
        case RSIP_ALG_ECC_PUBLIC:
        {
            ret = gsp_func_initial_key_wrap_ecc_pub[subtype];
            break;
        }

        case RSIP_ALG_ECC_PRIVATE:
        {
            ret = gsp_func_initial_key_wrap_ecc_priv[subtype];
            break;
        }

        case RSIP_ALG_HMAC:
        {
            ret = gsp_func_initial_key_wrap_hmac[subtype];
            break;
        }

        default:
        {
            /* Do nothing */
        }
    }

    return ret;
}
RSIP_SEC_DEFAULT
/*******************************************************************************************************************//**
 * Set sha final block.
 *
 * @param[in] p_message      last block_message.
 * @param[in] message_length message length.
 * @param[in] total_message_length all sha message length.
 * @param[in] p_last_block        last block data(this area is need 128byte) this area is .
 * @param[in] p_last_block_length last block length.
 *
 * @return void.
 **********************************************************************************************************************/
static void set_sha2_final_block(uint8_t * p_message,
                                uint32_t message_length,
                                uint32_t total_message_length,
                                uint8_t * p_last_block,
                                uint32_t * p_last_block_length)
{
    memset(&p_last_block[message_length], 0 , SHA_BLOCK8_LEN * 2 - message_length);    
    memcpy(p_last_block, p_message, message_length);
    p_last_block[message_length] = SHA_STOP_BIT;
    if ((SHA_BLOCK8_LEN - message_length) >= 9) /* another block unnecessary */
    {
        /* According c., but support only l < 2**32 case */
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[SHA_BLOCK8_LEN - 4] = (uint8_t) ((total_message_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[SHA_BLOCK8_LEN - 3] = (uint8_t) (((total_message_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[SHA_BLOCK8_LEN - 2] = (uint8_t) (((total_message_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[SHA_BLOCK8_LEN - 1] = (uint8_t) ((total_message_length * 8) & 0xff);
        p_last_block_length[0] = SHA_BLOCK8_LEN;
    }
    else
    {
        /* another block necessary */
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[(2 * SHA_BLOCK8_LEN) - 4] = (uint8_t) ((total_message_length * 8) >> 24);
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[(2 * SHA_BLOCK8_LEN) - 3] = (uint8_t) (((total_message_length * 8) >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[(2 * SHA_BLOCK8_LEN) - 2] = (uint8_t) (((total_message_length * 8) >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        p_last_block[(2 * SHA_BLOCK8_LEN) - 1] = (uint8_t) ((total_message_length * 8) & 0xff);
        p_last_block_length[0] = 2 * SHA_BLOCK8_LEN;
    }

    return;
}
