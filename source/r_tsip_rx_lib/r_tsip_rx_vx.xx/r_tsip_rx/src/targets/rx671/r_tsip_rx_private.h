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
 * Copyright (C) 2017-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_rx_private.h
 * Version      : 1.21
 * Description  : TSIP function private header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.09.2017 1.03     First Release
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
 *         : 24.05.2023 1.18     Added support for RX26T
 *         : 30.11.2023 1.19     Update example of Secure Bootloader / Firmware Update
 *         : 28.02.2024 1.20     Applied software workaround of AES-CCM decryption
 *         : 28.06.2024 1.21     Added support for TLS1.2 server
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_tsip_rx_if.h"
#include "iodefine/r_tsip_rx671_iodefine.h"

#ifndef R_TSIP_RX_PRIVATE_HEADER_FILE
#define R_TSIP_RX_PRIVATE_HEADER_FILE

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Depends on configuration */
#define TSIP_PRV_USE_AES_128_ECB_CBC_CTR (TSIP_AES_128_ECB_ENCRYPT || TSIP_AES_128_ECB_DECRYPT \
        || TSIP_AES_128_CBC_ENCRYPT || TSIP_AES_128_CBC_DECRYPT || TSIP_AES_128_CTR)
#define TSIP_PRV_USE_AES_256_ECB_CBC_CTR (TSIP_AES_256_ECB_ENCRYPT || TSIP_AES_256_ECB_DECRYPT \
        || TSIP_AES_256_CBC_ENCRYPT || TSIP_AES_256_CBC_DECRYPT || TSIP_AES_256_CTR)

#define TSIP_PRV_USE_AES_128_CCM (TSIP_AES_128_CCM_ENCRYPT || TSIP_AES_128_CCM_DECRYPT)
#define TSIP_PRV_USE_AES_256_CCM (TSIP_AES_256_CCM_ENCRYPT || TSIP_AES_256_CCM_DECRYPT)
#define TSIP_PRV_USE_AES_CCM     (TSIP_PRV_USE_AES_128_CCM || TSIP_PRV_USE_AES_256_CCM)

#define TSIP_PRV_USE_AES_128_GCM (TSIP_AES_128_GCM_ENCRYPT || TSIP_AES_128_GCM_DECRYPT)
#define TSIP_PRV_USE_AES_256_GCM (TSIP_AES_256_GCM_ENCRYPT || TSIP_AES_256_GCM_DECRYPT)
#define TSIP_PRV_USE_AES_GCM     (TSIP_PRV_USE_AES_128_GCM || TSIP_PRV_USE_AES_256_GCM)

#define TSIP_PRV_USE_AES_128 (TSIP_PRV_USE_AES_128_ECB_CBC_CTR || TSIP_AES_128_CMAC || TSIP_PRV_USE_AES_128_CCM \
        || TSIP_PRV_USE_AES_128_GCM || TSIP_AES_128_KEY_WRAP)
#define TSIP_PRV_USE_AES_256 (TSIP_PRV_USE_AES_256_ECB_CBC_CTR || TSIP_AES_256_CMAC || TSIP_PRV_USE_AES_256_CCM \
        || TSIP_PRV_USE_AES_256_GCM || TSIP_AES_256_KEY_WRAP)

#define TSIP_PRV_USE_TDES \
        (TSIP_TDES_ECB_ENCRYPT || TSIP_TDES_ECB_DECRYPT || TSIP_TDES_CBC_ENCRYPT || TSIP_TDES_CBC_DECRYPT)

#define TSIP_PRV_USE_ARC4 (TSIP_TSIP_ARC4_ENCRYPT || TSIP_ARC4_DECRYPT)

#define TSIP_PRV_USE_HMAC (TSIP_SHA_1_HMAC || TSIP_SHA_256_HMAC)

#define TSIP_PRV_USE_RSA_1024 (TSIP_RSAES_1024 || TSIP_RSASSA_1024)
#define TSIP_PRV_USE_RSA_2048 (TSIP_RSAES_2048 || TSIP_RSASSA_2048)
#define TSIP_PRV_USE_RSA_3072 (TSIP_RSAES_3072 || TSIP_RSASSA_3072)
#define TSIP_PRV_USE_RSA_4096 (TSIP_RSAES_4096 || TSIP_RSASSA_4096)

#define TSIP_PRV_USE_RSAES (TSIP_RSAES_1024 || TSIP_RSAES_2048 || TSIP_RSAES_3072 || TSIP_RSAES_4096)

#define TSIP_PRV_USE_RSASSA (TSIP_RSASSA_1024 || TSIP_RSASSA_2048 || TSIP_RSASSA_3072 || TSIP_RSASSA_4096)

#define TSIP_PRV_USE_ECDSA (TSIP_ECDSA_P192 | TSIP_ECDSA_P224 | TSIP_ECDSA_P256 | TSIP_ECDSA_P384)

#define TSIP_PRV_USE_ECC (TSIP_PRV_USE_ECDSA | TSIP_ECDH_P256)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
extern uint32_t const S_FLASH[];

extern uint32_t S_RAM[R_TSIP_SRAM_WORD_SIZE];
extern uint32_t S_HEAP[R_TSIP_SHEAP_WORD_SIZE];
extern uint32_t S_INST[R_TSIP_SINST_WORD_SIZE];
extern uint32_t S_INST2[R_TSIP_SINST2_WORD_SIZE];

extern TSIP_GEN_MAC_CB_FUNC_T TSIP_GEN_MAC_CB_FUNC;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* --------------------- TSIP driver wrapper layer ---------------------- */

e_tsip_err_t R_TSIP_GenerateRandomNumberPrivate(uint32_t *random);

e_tsip_err_t R_TSIP_Aes128EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes128EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes128CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes128CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);

e_tsip_err_t R_TSIP_Aes256EcbEncryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes256EcbDecryptInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes256CbcEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
e_tsip_err_t R_TSIP_Aes256CbcDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);

e_tsip_err_t R_TSIP_Aes128CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256CtrInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128CtrUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CtrUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CtrFinalPrivate();
e_tsip_err_t R_TSIP_Aes256CtrFinalPrivate();

e_tsip_err_t R_TSIP_Aes128GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256GcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         R_TSIP_Aes128GcmEncryptUpdateTransitionPrivate(void);
void         R_TSIP_Aes128GcmDecryptUpdateTransitionPrivate(void);
void         R_TSIP_Aes256GcmEncryptUpdateTransitionPrivate(void);
void         R_TSIP_Aes256GcmDecryptUpdateTransitionPrivate(void);

e_tsip_err_t R_TSIP_Aes128CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t InData_TextLen,
        uint32_t *InData_IV, uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes128CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes128CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, e_tsip_tls13_phase_t tls13_phase,
        e_tsip_tls13_mode_t tls13_mode, tsip_tls13_handle_t *tls13_handle, uint32_t InData_TextLen,
        uint32_t InData_MACLength, uint32_t *InData_IV, uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes128CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_MAC, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256CcmEncryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes256CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes256CcmDecryptInitPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
e_tsip_err_t R_TSIP_Aes256CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);

e_tsip_err_t R_TSIP_Aes128CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes128CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes128CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen);
e_tsip_err_t R_TSIP_Aes256CmacGenerateInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256CmacVerifyInitPrivate(tsip_aes_key_index_t *key_index);
e_tsip_err_t R_TSIP_Aes256CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen);

e_tsip_err_t R_TSIP_Aes128KeyWrapPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_WrappedKeyIndex, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256KeyWrapPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_WrappedKeyIndex, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes128KeyUnWrapPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_Text, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_Aes256KeyUnWrapPrivate(tsip_aes_key_index_t *key_index, uint32_t *InData_WrappedKeyType,
        uint32_t *InData_Text, uint32_t *OutData_KeyIndex);

e_tsip_err_t R_TSIP_Sha1InitPrivate(tsip_sha_md5_handle_t *handle);
e_tsip_err_t R_TSIP_Sha1UpdatePrivate(tsip_sha_md5_handle_t *handle, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha1FinalPrivate(tsip_sha_md5_handle_t *handle, uint32_t MAX_CNT,
        uint32_t *OutData_MsgDigest, uint32_t *OutData_Length);
e_tsip_err_t R_TSIP_Sha256InitPrivate(tsip_sha_md5_handle_t *handle);
e_tsip_err_t R_TSIP_Sha256UpdatePrivate(tsip_sha_md5_handle_t *handle, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha256FinalPrivate(tsip_sha_md5_handle_t *handle, uint32_t MAX_CNT,
        uint32_t *OutData_MsgDigest, uint32_t *OutData_Length);
e_tsip_err_t R_TSIP_Md5InitPrivate(tsip_sha_md5_handle_t *handle);
e_tsip_err_t R_TSIP_Md5UpdatePrivate(tsip_sha_md5_handle_t *handle, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Md5FinalPrivate(tsip_sha_md5_handle_t *handle, uint32_t MAX_CNT,
        uint32_t *OutData_MsgDigest, uint32_t *OutData_Length);

e_tsip_err_t R_TSIP_Sha1HmacGenerateInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha1HmacGenerateUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha1HmacGenerateFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *OutData_Mac);
e_tsip_err_t R_TSIP_Sha1HmacVerifyInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha1HmacVerifyUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha1HmacVerifyFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_Mac,
        uint32_t *InData_length);
e_tsip_err_t R_TSIP_Sha256HmacGenerateInitPrivate(tsip_hmac_sha_handle_t *handle,
        tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha256HmacGenerateUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha256HmacGenerateFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *OutData_Mac);
e_tsip_err_t R_TSIP_Sha256HmacVerifyInitPrivate(tsip_hmac_sha_handle_t *handle, tsip_hmac_sha_key_index_t *key_index);
e_tsip_err_t R_TSIP_Sha256HmacVerifyUpdatePrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha256HmacVerifyFinalPrivate(tsip_hmac_sha_handle_t *handle, uint32_t *InData_Mac,
        uint32_t *InData_length);

e_tsip_err_t R_TSIP_Rsa1024ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa1024ModularExponentDecryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa2048ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa2048ModularExponentDecryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa3072ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa4096ModularExponentEncryptPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa2048DhKeyAgreementPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_PrimNumIndex,
        uint32_t *InData_Message, uint32_t *InData_ModExp, uint32_t *OutData_ModExp);

e_tsip_err_t R_TSIP_TdesEcbEncryptInitPrivate(tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_TdesEcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_TdesEcbEncryptFinalPrivate(void);
e_tsip_err_t R_TSIP_TdesEcbDecryptInitPrivate(tsip_tdes_key_index_t *key_index);
e_tsip_err_t R_TSIP_TdesEcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_TdesEcbDecryptFinalPrivate(void);
e_tsip_err_t R_TSIP_TdesCbcEncryptInitPrivate(tsip_tdes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_TdesCbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_TdesCbcEncryptFinalPrivate(void);
e_tsip_err_t R_TSIP_TdesCbcDecryptInitPrivate(tsip_tdes_key_index_t *key_index, uint32_t *InData_IV);
e_tsip_err_t R_TSIP_TdesCbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_TdesCbcDecryptFinalPrivate(void);

e_tsip_err_t R_TSIP_Arc4EncryptInitPrivate(tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_Arc4EncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Arc4EncryptFinalPrivate(void);
e_tsip_err_t R_TSIP_Arc4DecryptInitPrivate(tsip_arc4_key_index_t *key_index);
e_tsip_err_t R_TSIP_Arc4DecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Arc4DecryptFinalPrivate(void);

e_tsip_err_t R_TSIP_EcdhReadPublicKeyPrivate(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_data,
        uint32_t *InData_Signature, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_EcdhMakePublicKeyPrivate(uint32_t *InData_Cmd, uint32_t *InData_KeyType,
        uint32_t *InData_PubKeyIndex, uint32_t *InData_PrivKeyIndex, uint32_t *InData_key_id, uint32_t *OutData_data,
        uint32_t *OutData_Signature, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_EcdhCalculateSharedSecretIndexPrivate(uint32_t *InData_KeyType, uint32_t *InData_PubKeyIndex,
        uint32_t *InData_PrivKeyIndex, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_EcdhKeyDerivationPrivate(uint32_t *InData_KeyIndexType, uint32_t *InData_KeyIndex,
        uint32_t *InData_KDFType, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT, uint32_t *InData_SaltKeyIndex,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_EcdheP512KeyAgreementPrivate(uint32_t *InData_KeyIndex, uint32_t *InData_PubKey,
        uint32_t *OutData_PubKey);


/* --------------------- TSIP control procedure related ---------------------- */

void         R_TSIP_SoftwareResetSub(void);
e_tsip_err_t R_TSIP_SelfCheck1Sub(void);
e_tsip_err_t R_TSIP_SelfCheck2Sub(void);

e_tsip_err_t R_TSIP_GenerateAes128KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateAes256KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateAes128RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateAes256RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateTdesKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateArc4KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateTdesRandomKeyIndexSub(uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateArc4RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa1024PublicKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa1024PrivateKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa2048PublicKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa2048PrivateKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa3072PublicKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa4096PublicKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa1024RandomKeyIndexSub(uint32_t MAX_CNT, uint32_t *OutData_PubKeyIndex,
        uint32_t *OutData_PrivKeyIndex);
e_tsip_err_t R_TSIP_GenerateRsa2048RandomKeyIndexSub(uint32_t MAX_CNT, uint32_t *OutData_PubKeyIndex,
        uint32_t *OutData_PrivKeyIndex);
e_tsip_err_t R_TSIP_GenerateTlsRsaInstallDataSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData);
e_tsip_err_t R_TSIP_GenerateTlsP256EccKeyIndexSub(uint32_t *OutData_KeyIndex, uint32_t *OutData_PubKey);
e_tsip_err_t R_TSIP_GenerateEccPrivateKeyIndexSub(uint32_t *InData_SharedKeyIndex,
        uint32_t *InData_SessionKey, uint32_t *InData_Cmd, uint32_t *InData_IV,
        uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateEccP384PrivateKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateEccPublicKeyIndexSub(uint32_t *InData_SharedKeyIndex,
        uint32_t *InData_SessionKey, uint32_t *InData_Cmd, uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateEccP384PublicKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateEccRandomKeyIndexSub(uint32_t *InData_Cmd, uint32_t *OutData_PubKeyIndex,
        uint32_t *OutData_PrivKeyIndex);
e_tsip_err_t R_TSIP_GenerateEccP384RandomKeyIndexSub(uint32_t *OutData_PubKeyIndex, uint32_t *OutData_PrivKeyIndex);
e_tsip_err_t R_TSIP_GenerateShaHmacKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_Cmd, uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_GenerateRandomNumberSub(uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_GenerateUpdateKeyRingKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData);

e_tsip_err_t R_TSIP_UpdateAes128KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateAes256KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateTdesKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateArc4KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateRsa1024PublicKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateRsa1024PrivateKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateRsa2048PublicKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateRsa2048PrivateKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateRsa3072PublicKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateRsa4096PublicKeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateTlsRsaDataSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData);
e_tsip_err_t R_TSIP_UpdateEccPrivateKeyIndexSub(uint32_t *InData_Cmd, uint32_t *InData_IV,
        uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateEccP384PrivateKeyIndexSub(uint32_t *InData_IV,
        uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateEccPublicKeyIndexSub(uint32_t *InData_Cmd, uint32_t *InData_IV,
        uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateEccP384PublicKeyIndexSub(uint32_t *InData_IV,
        uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_UpdateShaHmacKeyIndexSub(uint32_t *InData_Cmd, uint32_t *InData_IV, uint32_t *InData_InstData,
        uint32_t *OutData_KeyIndex);

e_tsip_err_t R_TSIP_StartUpdateFirmwareSub(void);
e_tsip_err_t R_TSIP_GenerateFirmwareMacSub(uint32_t *InData_KeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_UpProgram, uint32_t *InData_IV, uint32_t *OutData_Program, uint32_t MAX_CNT,
        tsip_firmware_generate_mac_resume_handle_t *tsip_firmware_generate_mac_resume_handle);
e_tsip_err_t R_TSIP_VerifyFirmwareMacSub(uint32_t *InData_Program, uint32_t MAX_CNT);

e_tsip_err_t R_TSIP_Aes128EncryptDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_Cmd,
        uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes128EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128EncryptDecryptFinalSub(void);
e_tsip_err_t R_TSIP_Aes256EncryptDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_Cmd,
        uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes256EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256EncryptDecryptFinalSub(void);

e_tsip_err_t R_TSIP_Ghash(uint32_t *InData_HV, uint32_t *InData_IV, uint32_t *InData_Text, uint32_t *OutData_DataT,
        uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmEncryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_DataType, uint32_t *InData_Cmd,
        uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_SeqNum);
void         R_TSIP_Aes128GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_DataType, uint32_t *InData_Cmd,
        uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_SeqNum);
void         R_TSIP_Aes128GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         R_TSIP_Aes128GcmDecryptUpdateTransitionSub(void);
void         R_TSIP_Aes128GcmEncryptUpdateTransitionSub(void);
void         R_TSIP_Aes256GcmDecryptUpdateTransitionSub(void);
void         R_TSIP_Aes256GcmEncryptUpdateTransitionSub(void);
void         R_TSIP_Aes128GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         R_TSIP_Aes128GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmEncryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes256GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         R_TSIP_Aes256GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         R_TSIP_Aes256GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         R_TSIP_Aes256GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);

e_tsip_err_t R_TSIP_Aes128CcmEncryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_DataType, uint32_t *InData_Cmd,
        uint32_t *InData_TextLen, uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len, uint32_t *InData_SeqNum);
void R_TSIP_Aes128CcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes128CcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_DataType, uint32_t *InData_Cmd,
        uint32_t *InData_TextLen, uint32_t *InData_MACLength, uint32_t *InData_KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len, uint32_t *InData_SeqNum);
void R_TSIP_Aes128CcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_MAC, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Aes256CcmEncryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
void R_TSIP_Aes256CcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmEncryptFinalSub(uint32_t *InData_TextLen, uint32_t *InData_Text, uint32_t *OutData_Text,
        uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Aes256CcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
void R_TSIP_Aes256CcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);

e_tsip_err_t R_TSIP_Aes128CmacInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex);
void         R_TSIP_Aes128CmacUpdateSub(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes128CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen, uint32_t *OutData_DataT);
e_tsip_err_t R_TSIP_Aes256CmacInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex);
void         R_TSIP_Aes256CmacUpdateSub(uint32_t *InData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Aes256CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen, uint32_t *OutData_DataT);

e_tsip_err_t R_TSIP_AESKeyWrapSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t KEY_INDEX_SIZE, uint32_t
        *InData_WrappedKeyType, uint32_t *InData_WrappedKeyIndex, uint32_t WRAPPED_KEY_SIZE, uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_AESKeyUnwrapSub(uint32_t *InData_KeyType, uint32_t *InData_Cmd, uint32_t *InData_KeyIndex,
        uint32_t KEY_INDEX_SIZE, uint32_t *InData_WrappedKeyType, uint32_t *InData_Text, uint32_t WRAPPED_KEY_SIZE,
        uint32_t *OutData_KeyIndex);

e_tsip_err_t R_TSIP_Sha1GenerateMessageDigestSub(uint32_t *InData_SHA1InitVal, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT, uint32_t *OutData_MsgDigest);
e_tsip_err_t R_TSIP_Sha224256GenerateMessageDigestSub(uint32_t *InData_SHAInitVal, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT, uint32_t *OutData_MsgDigest);
e_tsip_err_t R_TSIP_Md5GenerateMessageDigestSub(uint32_t *InData_MD5InitVal, uint32_t *InData_PaddedMsg,
        uint32_t MAX_CNT, uint32_t *OutData_MsgDigest);

e_tsip_err_t R_TSIP_Sha1HmacInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex);
void         R_TSIP_Sha1HmacUpdateSub(uint32_t *InData_PaddedMsg, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha1HmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_MAC, uint32_t *InData_length,
        uint32_t *OutData_MAC);
e_tsip_err_t R_TSIP_Sha256HmacInitSub(uint32_t *InData_KeyType, uint32_t *InData_Cmd, uint32_t *InData_KeyIndex);
void         R_TSIP_Sha256HmacUpdateSub(uint32_t *InData_PaddedMsg, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Sha256HmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_MAC, uint32_t *InData_length,
        uint32_t *OutData_MAC);

e_tsip_err_t R_TSIP_TdesEncryptDecryptInitSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void R_TSIP_TdesEncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_TdesEncryptDecryptFinalSub(void);
e_tsip_err_t R_TSIP_Arc4EncryptDecryptInitSub(uint32_t *InData_KeyIndex);
void R_TSIP_Arc4EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
e_tsip_err_t R_TSIP_Arc4EncryptDecryptFinalSub(void);

e_tsip_err_t R_TSIP_TlsRootCertificateVerificationSub(uint32_t *InData_Sel_PubKeyType, uint32_t *InData_Certificates,
        uint32_t *InData_CertificatesLength, uint32_t *InData_Signature, uint32_t *InData_CertificatesInfo,
        uint32_t *OutData_PubKey);
e_tsip_err_t R_TSIP_TlsCertificateVerificationSub(uint32_t *InData_Sel_InDataPubKeyType,
        uint32_t *InData_Sel_OutDataPubKeyType, uint32_t *InData_PubKey,
        uint32_t *InData_TBSCertificate, uint32_t *InData_TBSCertificateLength, uint32_t *InData_Signature,
        uint32_t *InData_TBSCertificateInfo, uint32_t *OutData_PubKey);
e_tsip_err_t R_TSIP_TlsEncryptPreMasterSecretSub(uint32_t *InData_PubKey, uint32_t *InData_PreMasterSecret,
        uint32_t *OutData_PreMasterSecret);
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecretSub(uint32_t *OutData_PreMasterSecret);
e_tsip_err_t R_TSIP_TlsGenerateMasterSecretSub(uint32_t *InData_Sel_CipherSuite, uint32_t *InData_PreMasterSecret,
        uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *OutData_MasterSecret);
e_tsip_err_t R_TSIP_TlsGenerateSessionKeySub(uint32_t *InData_Sel_CipherSuite, uint32_t *InData_MasterSecret,
        uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *InData_NonceExplicit,
        uint32_t *OutData_ClientMACKeyOperationCode, uint32_t *OutData_ServerMACKeyOperationCode,
        uint32_t *OutData_ClientEncKeyOperationCode, uint32_t *OutData_ServerEncKeyOperationCode,
        uint32_t* OutData_ClientIV, uint32_t* OutData_ServerIV);
e_tsip_err_t R_TSIP_TlsGenerateVerifyDataSub(uint32_t *InData_Sel_VerifyData, uint32_t *InData_MasterSecret,
        uint32_t *InData_HandShakeHash, uint32_t *OutData_VerifyData);
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecretWithEccP256KeySub(uint32_t *InData_PubKey, uint32_t *InData_KeyIndex,
        uint32_t *OutData_PreMasterSecretIndex);
e_tsip_err_t R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrievesSub(uint32_t *InData_Sel_PubKeyType,
        uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *InData_Sel_CompressType,
        uint32_t *InData_SKE_Message, uint32_t *InData_SKE_Signature, uint32_t *InData_PubKey,
        uint32_t *OutData_EphemeralPubKey);
e_tsip_err_t R_TSIP_TlsGenerateExtendedMasterSecretSub(uint32_t *InData_Sel_CipherSuite,
        uint32_t *InData_PreMasterSecret, uint32_t *InData_MsgHash, uint32_t *OutData_ExMasterSecret);

e_tsip_err_t R_TSIP_GenerateTls13P256EccKeyIndexSub(uint32_t * InData_Cmd, uint32_t * InData_Handle,
        uint32_t * OutData_KeyIndex, uint32_t * OutData_PubKey);
e_tsip_err_t R_TSIP_Tls13GenerateEcdheSharedSecretSub(uint32_t * InData_Cmd, uint32_t * InData_PubKey,
        uint32_t * InData_KeyIndex, uint32_t * OutData_SharedSecretKeyIndex);
e_tsip_err_t R_TSIP_Tls13GenerateHandshakeSecretSub(uint32_t * InData_SharedSecretKeyIndex,
        uint32_t * OutData_Handshake_SecretKeyIndex);
e_tsip_err_t R_TSIP_Tls13GenerateServerHandshakeTrafficKeySub(uint32_t * InData_Cmd,
        uint32_t * InData_Handshake_SecretKeyIndex, uint32_t * InData_MsgHash,
        uint32_t * OutData_ServerWriteKeyIndex, uint32_t * OutData_ServerFinishedKeyIndex);
e_tsip_err_t R_TSIP_Tls13ServerHandshakeVerificationSub(uint32_t * InData_Cmd,
        uint32_t * InData_ServerFinishedKeyIndex, uint32_t * InData_MsgHash, uint32_t * InData_ServerFinished,
        uint32_t * OutData_VerifyDataIndex);
e_tsip_err_t R_TSIP_Tls13GenerateClientHandshakeTrafficKeySub(uint32_t * InData_Cmd,
        uint32_t * InData_Handshake_SecretKeyIndex, uint32_t * InData_MsgHash, uint32_t * OutData_ClientWriteKeyIndex,
        uint32_t * OutData_ClientFinishedKeyIndex);
e_tsip_err_t R_TSIP_Tls13GenerateMasterSecretSub(uint32_t * InData_Cmd, uint32_t * InData_Handshake_SecretKeyIndex,
        uint32_t * InData_VerifyDataIndex, uint32_t * OutData_Handle, uint32_t * OutData_Master_SecretKeyIndex);
e_tsip_err_t R_TSIP_Tls13GenerateApplicationTrafficKeySub(uint32_t * InData_Cmd, uint32_t * InData_Handle,
        uint32_t * InData_Master_SecretKeyIndex, uint32_t * InData_MsgHash,
        uint32_t * OutData_ServerApplicationSecretKeyIndex, uint32_t * OutData_ClientApplicationSecretKeyIndex,
        uint32_t * OutData_ServerWriteKeyIndex, uint32_t * OutData_ClientWriteKeyIndex);
e_tsip_err_t R_TSIP_Tls13UpdateApplicationTrafficKeySub(uint32_t * InData_Cmd, uint32_t * InData_UpdateKeyType,
        uint32_t * InData_Handle, uint32_t * InData_ApplicationSecretKeyIndex,
        uint32_t * OutData_ApplicationSecretKeyIndex, uint32_t * OutData_ApplicationWriteKeyIndex);

e_tsip_err_t R_TSIP_Rsa1024ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa1024ModularExponentDecryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa2048ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa2048ModularExponentDecryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text,
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa3072ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text, 
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa4096ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text, 
        uint32_t *OutData_Text);
e_tsip_err_t R_TSIP_Rsa2048DhKeyAgreementSub(uint32_t *InData_KeyIndex, uint32_t *InData_PrimNumIndex,
        uint32_t *InData_Message, uint32_t *InData_ModExp, uint32_t *OutData_ModExp);

e_tsip_err_t R_TSIP_EcdsaSigunatureGenerateSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex,
        uint32_t *InData_MsgDgst, uint32_t *OutData_Signature);
e_tsip_err_t R_TSIP_EcdsaP384SigunatureGenerateSub(uint32_t *InData_KeyIndex,
        uint32_t *InData_MsgDgst, uint32_t *OutData_Signature);
e_tsip_err_t R_TSIP_EcdsaSigunatureVerificationSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex,
        uint32_t *InData_MsgDgst, uint32_t *InData_Signature);
e_tsip_err_t R_TSIP_EcdsaP384SigunatureVerificationSub(uint32_t *InData_KeyIndex,
        uint32_t *InData_MsgDgst, uint32_t *InData_Signature);

e_tsip_err_t R_TSIP_DlmsCosemQeuSignatureVerificationSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex,
        uint32_t *InData_data, uint32_t *InData_Signature, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_DlmsCosemQevSignatureGenerationSub(uint32_t *InData_Cmd, uint32_t *InData_KeyType,
        uint32_t *InData_PubKeyIndex, uint32_t *InData_PrivKeyIndex, uint32_t *InData_key_id, uint32_t *OutData_data,
        uint32_t *OutData_Signature, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_DlmsCosemCalculateZSub(uint32_t *InData_KeyType, uint32_t *InData_PubKeyIndex,
        uint32_t *InData_PrivKeyIndex, uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_DlmsCosemCalculateKekSub(uint32_t *InData_KeyIndexType, uint32_t *InData_KeyIndex,
        uint32_t *InData_KDFType, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT, uint32_t *InData_SaltKeyIndex,
        uint32_t *OutData_KeyIndex);
e_tsip_err_t R_TSIP_EcdheP512KeyAgreementSub(uint32_t *InData_KeyIndex, uint32_t *InData_PubKey,
        uint32_t *OutData_PubKey);

void R_TSIP_TlsRootCertificateVerificationSubSub(void);
void R_TSIP_TlsGenerateSubSub(void);

void R_TSIP_SelfCheck1SubSub(void);
void R_TSIP_SelfCheck2SubSub(void);

void RX671_func000(uint32_t *InData_PaddedMsg, int32_t MAX_CNT);
void RX671_func001(void);
void RX671_func002(void);
void RX671_func003(uint32_t* ARG1);
void RX671_func004(uint32_t ARG1);
void RX671_func005(uint32_t ARG1);
void RX671_func006(void);
void RX671_func007(void);
void RX671_func008(void);
void RX671_func009(void);
void RX671_func010(uint32_t ARG1);
void RX671_func011(uint32_t *ARG1);
void RX671_func015(void);
void RX671_func022(void);
void RX671_func023(void);
void RX671_func025(uint32_t ARG1);
void RX671_func027(uint32_t ARG1);
void RX671_func028(uint32_t ARG1);
void RX671_func030(void);
void RX671_func031(uint32_t *ARG1);
void RX671_func040(uint32_t ARG1);
void RX671_func041(uint32_t ARG1);
void RX671_func050(uint32_t ARG1);
void RX671_func051(void);
void RX671_func052(uint32_t ARG1);
void RX671_func053(uint32_t ARG1);
void RX671_func054(uint32_t ARG1, uint32_t ARG2);
void RX671_func060(void);
void RX671_func061(void);
void RX671_func070(void);
void RX671_func071(void);
void RX671_func072(void);
void RX671_func075(void);
void RX671_func080(void);
void RX671_func100(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void RX671_func101(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void RX671_func102(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void RX671_func103(void);
void RX671_func200(void);
void RX671_func202(void);
void RX671_func205(void);
void RX671_func206(void);
void RX671_func207(void);
void RX671_func304(void);
void RX671_func307(void);
void RX671_func308(void);
void RX671_func309(void);
void RX671_func310(void);
void RX671_func311(void);
void RX671_func312(uint32_t ARG1);
void RX671_func313(uint32_t ARG1);
void RX671_func314(uint32_t ARG1);
void RX671_func315(uint32_t ARG1);
void RX671_func316(void);
void RX671_func317(void);
void RX671_func318(void);
void RX671_func319(uint32_t ARG1);
void RX671_func320(uint32_t ARG1);
void RX671_func321(uint32_t ARG1);
void RX671_func322(uint32_t ARG1);
void RX671_func323(void);
void RX671_func324(void);
void RX671_func325(void);

void firm_mac_read(uint32_t *InData_Program);

e_tsip_err_t rsassa_emsa_pss_encode(tsip_rsa_byte_data_t *message, uint8_t hash_type, uint32_t em_bits, uint8_t *em);

/**********************************************************************************************************************
 * Function Name: change_endian_long
 * Description  : change endian
 * Arguments    : data -
 *                    input data
 * Return Value : the data changing endian
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(change_endian_long)
uint32_t change_endian_long(uint32_t data)
{
#if defined __LIT
    /* Casting is valid because it matches the type to the right side or argument. */
    return R_BSP_REVL(data);
#else
    return data;
#endif  /* defined __LIT */
}
/*********************************
 End of function change_endian_long
 **********************************/

#endif /* R_TSIP_RX_PRIVATE_HEADER_FILE */
