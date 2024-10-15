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
* File Name    : r_rsip_wrapper.h
* Description  : RSIP function wrapper header file.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/
#ifndef R_RSIP_WRAPPER_H
#define R_RSIP_WRAPPER_H
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include "r_rsip_protected_rx_if.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Key update inst data word */
#define RSIP_OEM_KEY_SIZE_DUMMY_INST_DATA_WORD                        (0)
#define RSIP_OEM_KEY_SIZE_AES128_INST_DATA_WORD                       (8)
#define RSIP_OEM_KEY_SIZE_AES256_INST_DATA_WORD                       (12)
#define RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PUBLIC_KEY_INST_DATA_WORD     (20)
#define RSIP_OEM_KEY_SIZE_ECC_SECP256R1_PRIVATE_KEY_INST_DATA_WORD    (12)
#define RSIP_OEM_KEY_SIZE_HMAC_SHA256_KEY_INST_DATA_WORD              (12)

/* Wrap Key type and size word */
#define RSIP_KEY_INDEX_SIZE_AES128_WORD                       (9)
#define RSIP_WRAPPED_KEY_SIZE_AES128_WORD                     (6)
#define RSIP_KEY_INDEX_SIZE_AES256_WORD                       (13)
#define RSIP_WRAPPED_KEY_SIZE_AES256_WORD                     (10)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef enum e_rsip_ecc_curve_type
{
    RSIP_ECC_CURVE_TYPE_NIST,
    RSIP_ECC_CURVE_TYPE_Brainpool,
    RSIP_ECC_CURVE_TYPE_Koblitz
} rsip_ecc_type_t;

typedef enum e_rsip_ecc_cmd_type
{
    RSIP_ECC_CMD_P256
} rsip_ecc_cmd_type_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
rsip_err_t r_rsip_wrapper_pf0_secp256r1 (const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        uint32_t       OutData_Signature[]);
rsip_err_t r_rsip_wrapper_pf0_brainpoolp256r1 (const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        uint32_t       OutData_Signature[]);
rsip_err_t r_rsip_wrapper_pf0_secp256k1 (const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        uint32_t       OutData_Signature[]);
rsip_err_t r_rsip_wrapper_pf1_secp256r1 (const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        const uint32_t InData_Signature[]);
rsip_err_t r_rsip_wrapper_pf1_brainpoolp256r1 (const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        const uint32_t InData_Signature[]);
rsip_err_t r_rsip_wrapper_pf1_secp256k1 (const uint32_t InData_KeyIndex[],
                                        const uint32_t InData_MsgDgst[],
                                        const uint32_t InData_Signature[]);
rsip_err_t r_rsip_wrapper_pf4_secp256r1 (uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[]);
rsip_err_t r_rsip_wrapper_pf4_brainpoolp256r1 (uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[]);
rsip_err_t r_rsip_wrapper_pf4_secp256k1 (uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[]);
rsip_err_t r_rsip_p6e_aes128 (const uint32_t   InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_aes256 (const uint32_t   InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_eccp256_public (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_eccp256_private (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_eccp256r1_public (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_eccp256r1_private (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_eccsecp256k1_public (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_eccsecp256k1_private (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_hmacsha224 (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_hmacsha256 (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_p6e_aes256_KUK (const uint32_t InData_SessionKey[],
                                const uint32_t InData_IV[],
                                const uint32_t InData_InstData[],
                                uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_aes128 (const uint32_t InData_IV[],
                                    const uint32_t InData_InstData[],
                                    uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_aes256 (const uint32_t InData_IV[],
                                    const uint32_t InData_InstData[],
                                    uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_secp256r1_pub (const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_secp256r1_priv (const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_hmacsha224 (const uint32_t InData_IV[],
                                        const uint32_t InData_InstData[],
                                        uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_hmacsha256 (const uint32_t InData_IV[],
                                        const uint32_t InData_InstData[],
                                        uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_brainpoolp256r1_pub (const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_brainpoolp256r1_priv (const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_secp256k1_pub (const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p6f_secp256k1_priv (const uint32_t InData_IV[],
                                            const uint32_t InData_InstData[],
                                            uint32_t       OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p8f_aes128 (const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_WrappedKeyIndex[],
                                    uint32_t              OutData_Text[]);
rsip_err_t r_rsip_wrapper_p8f_aes256 (const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_WrappedKeyIndex[],
                                    uint32_t              OutData_Text[]);
rsip_err_t r_rsip_wrapper_p90_aes128 (const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_Text[],
                                    uint32_t              OutData_KeyIndex[]);
rsip_err_t r_rsip_wrapper_p90_aes256 (const uint32_t        InData_KeyIndex[],
                                    const rsip_key_type_t key_type,
                                    const uint32_t        InData_Text[],
                                    uint32_t              OutData_KeyIndex[]);

/* AES-ECB/CBC/CTR */
rsip_err_t r_rsip_wrapper_p47i_aes128ecb_encrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p47i_aes128ecb_decrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p47i_aes128cbc_encrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p47i_aes128cbc_decrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p47i_aes128ctr_crypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p50i_aes256ecb_encrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p50i_aes256ecb_decrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p50i_aes256cbc_encrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p50i_aes256cbc_decrypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);
rsip_err_t r_rsip_wrapper_p50i_aes256ctr_crypt (const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);

/* AES-GCM */
rsip_err_t r_rsip_wrapper_p29i_aes128gcm_encrypt (const uint32_t * InData_KeyIndex, const uint32_t * InData_IV);
rsip_err_t r_rsip_wrapper_p32i_aes128gcm_decrypt (const uint32_t * InData_KeyIndex, const uint32_t * InData_IV);
rsip_err_t r_rsip_wrapper_p34i_aes256gcm_encrypt (const uint32_t * InData_KeyIndex, const uint32_t * InData_IV);
rsip_err_t r_rsip_wrapper_p36i_aes256gcm_decrypt (const uint32_t * InData_KeyIndex, const uint32_t * InData_IV);

/* AES-CCM */
rsip_err_t r_rsip_wrapper_p95i_aes128ccm_encrypt (const uint32_t * InData_KeyIndex,
                                                const uint32_t * InData_TextLen,
                                                const uint32_t * InData_IV,
                                                const uint32_t * InData_Header,
                                                uint32_t         Header_Len);
rsip_err_t r_rsip_wrapper_p95f_aes128ccm_encrypt (const uint32_t * InData_Text,
                                                const uint32_t * InData_TextLen,
                                                uint32_t       * OutData_Text,
                                                uint32_t       * OutData_MAC);
rsip_err_t r_rsip_wrapper_p98i_aes128ccm_decrypt (const uint32_t * InData_KeyIndex,
                                                const uint32_t * InData_TextLen,
                                                const uint32_t * InData_MACLength,
                                                const uint32_t * InData_IV,
                                                const uint32_t * InData_Header,
                                                uint32_t         Header_Len);
rsip_err_t r_rsip_wrapper_p98f_aes128ccm_decrypt (const uint32_t * InData_Text,
                                                const uint32_t * InData_TextLen,
                                                const uint32_t * InData_MAC,
                                                const uint32_t * InData_MACLength,
                                                uint32_t       * OutData_Text);

rsip_err_t r_rsip_wrapper_pa1i_aes256ccm_encrypt (const uint32_t * InData_KeyIndex,
                                                const uint32_t * InData_TextLen,
                                                const uint32_t * InData_IV,
                                                const uint32_t * InData_Header,
                                                uint32_t         Header_Len);
rsip_err_t r_rsip_wrapper_pa4i_aes256ccm_decrypt (const uint32_t * InData_KeyIndex,
                                                const uint32_t * InData_TextLen,
                                                const uint32_t * InData_MACLength,
                                                const uint32_t * InData_IV,
                                                const uint32_t * InData_Header,
                                                uint32_t         Header_Len);

/* AES-MAC */
rsip_err_t r_rsip_wrapper_p41i_aes128mac (const uint32_t * InData_KeyIndex);
rsip_err_t r_rsip_wrapper_p41f_aes128mac_generate (const uint32_t * InData_Text,
                                                    uint32_t       * OutData_DataT,
                                                    const uint32_t   all_msg_len);
rsip_err_t r_rsip_wrapper_p41f_aes128mac_verify (const uint32_t * InData_Text,
                                                const uint32_t * InData_DataT,
                                                const uint32_t * InData_DataTLen,
                                                const uint32_t   all_msg_len);
rsip_err_t r_rsip_wrapper_p44i_aes256mac (const uint32_t * InData_KeyIndex);
rsip_err_t r_rsip_wrapper_p44f_aes256mac_generate (const uint32_t * InData_Text,
                                                    uint32_t       * OutData_DataT,
                                                    const uint32_t   all_msg_len);
rsip_err_t r_rsip_wrapper_p44f_aes256mac_verify (const uint32_t * InData_Text,
                                                const uint32_t * InData_DataT,
                                                const uint32_t * InData_DataTLen,
                                                const uint32_t   all_msg_len);

#endif                                 /* R_RSIP_WRAPPER_H */
