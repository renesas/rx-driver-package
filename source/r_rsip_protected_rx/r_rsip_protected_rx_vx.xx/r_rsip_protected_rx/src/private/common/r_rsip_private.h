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
* File Name    : r_rsip_private.h
* Description  : RSIP function private header file.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.10.2024 1.00     First Release.
***********************************************************************************************************************/

#ifndef R_RSIP_PRIVATE_H
#define R_RSIP_PRIVATE_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include "r_rsip_protected_rx_if.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RSIP_DUMMY_LC  (0x00000002)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/*
 * Private/Primitive functions
 */

/* Random number generation */
typedef rsip_err_t (* rsip_func_rng_t)(uint32_t OutData_Text[]);

/* Key generation */
typedef rsip_err_t (* rsip_func_key_generate_t)(uint32_t OutData_KeyIndex[]);

/* Key pair generation */
typedef rsip_err_t (* rsip_func_key_pair_generate_t)(uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[]);

/* Encrypted key wrap */
typedef rsip_err_t (* rsip_func_encrypted_key_wrap_t)(const uint32_t InData_IV[], const uint32_t InData_InstData[],
                                                        uint32_t OutData_KeyIndex[]);

/* RFC3394 Key Wrap */
typedef rsip_err_t (* rsip_func_rfc3394_key_wrap_t)(const uint32_t InData_KeyIndex[], const rsip_key_type_t key_type,
                                                    const uint32_t InData_WrappedKeyIndex[], uint32_t OutData_Text[]);

/* RFC3394 Key Unwrap */
typedef rsip_err_t (* rsip_func_rfc3394_key_unwrap_t)(const uint32_t InData_KeyIndex[], const rsip_key_type_t key_type,
                                                        const uint32_t InData_Text[], uint32_t OutData_KeyIndex[]);

/* Initial key wrap */
typedef rsip_err_t (* rsip_func_initial_key_wrap_t)(const uint32_t InData_SessionKey[], const uint32_t InData_IV[],
                                                    const uint32_t InData_InstData[], uint32_t OutData_KeyIndex[]);

/* Initial key update key wrap */
typedef rsip_err_t (* rsip_func_initial_key_update_key_wrap_t)(const uint32_t InData_SessionKey[],
                                                                const uint32_t InData_IV[],
                                                                const uint32_t InData_InstData[],
                                                                uint32_t OutData_KeyIndex[]);

/* AES-ECB/CBC/CTR */
typedef rsip_err_t (* rsip_func_aes_cipher_init_t)(const uint32_t InData_KeyIndex[], const uint32_t InData_IV[]);

/* AES-GCM */
typedef rsip_err_t (* rsip_func_ghash_t)(const uint32_t InData_HV[], const uint32_t InData_IV[],
                                            const uint32_t InData_Text[], uint32_t OutData_DataT[], uint32_t MAX_CNT);

/* ECC */
typedef rsip_err_t (* rsip_func_ecdsa_sign_t)(const uint32_t InData_KeyIndex[], const uint32_t InData_MsgDgst[],
                                                uint32_t OutData_Signature[]);
typedef rsip_err_t (* rsip_func_ecdsa_verify_t)(const uint32_t InData_KeyIndex[], const uint32_t InData_MsgDgst[],
                                                const uint32_t InData_Signature[]);

/*
 * Private/Primitive function subsets
 */

/* AES-ECB/CBC/CTR */
typedef struct st_rsip_func_subset_aes_cipher
{
    rsip_func_aes_cipher_init_t p_init_ecb_enc;
    rsip_func_aes_cipher_init_t p_init_ecb_dec;
    rsip_func_aes_cipher_init_t p_init_cbc_enc;
    rsip_func_aes_cipher_init_t p_init_cbc_dec;
    rsip_func_aes_cipher_init_t p_init_ctr;
    void (* p_update)(const uint32_t * InData_Text, uint32_t * OutData_Text, uint32_t MAX_CNT);
    rsip_err_t (* p_final)();
} rsip_func_subset_aes_cipher_t;

/* AES-GCM */
typedef struct st_rsip_func_subset_aes_gcm
{
    rsip_err_t (* p_init)(const uint32_t * InData_KeyIndex, const uint32_t * InData_IV);
    void (* p_updateAad)(const uint32_t * InData_DataA, uint32_t MAX_CNT);
    void (* p_updateTransition)();
    void (* p_update)(const uint32_t * InData_Text, uint32_t * OutData_Text, uint32_t MAX_CNT);
    rsip_err_t (* p_encryptFinal)(const uint32_t * InData_Text, const uint32_t * InData_DataALen,
                                    const uint32_t * InData_TextLen, uint32_t * OutData_Text, uint32_t * OutData_DataT);
    rsip_err_t (* p_decryptFinal)(const uint32_t * InData_Text, const uint32_t * InData_DataT,
                                    const uint32_t * InData_DataALen, const uint32_t * InData_TextLen,
                                    const uint32_t * InData_DataTLen,
                                    uint32_t * OutData_Text);
} rsip_func_subset_aes_gcm_t;

/* AES-CCM */
typedef struct st_rsip_func_subset_aes_ccm
{
    rsip_err_t (* p_encryptInit)(const uint32_t * InData_KeyIndex, const uint32_t * InData_TextLen,
                                    const uint32_t * InData_IV, const uint32_t * InData_Header, uint32_t Header_Len);
    rsip_err_t (* p_decryptInit)(const uint32_t * InData_KeyIndex, const uint32_t * InData_TextLen,
                                    const uint32_t * InData_MACLength, const uint32_t * InData_IV,
                                    const uint32_t * InData_Header, uint32_t Header_Len);
    void (* p_update)(const uint32_t * InData_Text, uint32_t * OutData_Text, uint32_t MAX_CNT);
    rsip_err_t (* p_encryptFinal)(const uint32_t * InData_Text, const uint32_t * InData_TextLen,
                                    uint32_t * OutData_Text, uint32_t * OutData_MAC);
    rsip_err_t (* p_decryptFinal)(const uint32_t * InData_Text, const uint32_t * InData_TextLen,
                                    const uint32_t * InData_MAC, const uint32_t * InData_MACLength,
                                    uint32_t * OutData_Text);
} rsip_func_subset_aes_ccm_t;

/* AES-CMAC */
typedef struct st_rsip_func_subset_aes_mac
{
    rsip_err_t (* p_init)(const uint32_t * InData_KeyIndex);
    void (* p_update)(const uint32_t * InData_Text, uint32_t MAX_CNT);
    rsip_err_t (* p_generateFinal)(const uint32_t * InData_Text, uint32_t * OutData_DataT, const uint32_t all_msg_len);
    rsip_err_t (* p_verifyFinal)(const uint32_t * InData_Text, const uint32_t * InData_DataT,
                                    const uint32_t * InData_DataTLen, const uint32_t all_msg_len);
} rsip_func_subset_aes_mac_t;

/* FWUP */
typedef struct st_rsip_func_subset_fwup
{
    rsip_err_t (* p_init)(const uint32_t * InData_KeyIndex, const uint32_t * InData_SessionKey,
                            const uint32_t * InData_IV);
    rsip_err_t (* p_update)(const uint32_t * InData_UpProgram, uint32_t MAX_CNT, uint32_t * OutData_Program);
    rsip_err_t (* p_final)(const uint32_t * InData_UpProgram, const uint32_t * InData_UpMAC,
                            uint32_t MAX_CNT, uint32_t * OutData_Program, uint32_t * OutData_MAC);
} rsip_func_subset_fwup_t;

/* SB */
typedef struct st_rsip_func_subset_sb
{
    rsip_err_t (* p_init)();
    rsip_err_t (* p_update)(const uint32_t * InData_UpProgram, uint32_t MAX_CNT);
    rsip_err_t (* p_final)(const uint32_t * InData_UpProgram, const uint32_t * InData_UpMAC, uint32_t MAX_CNT);
} rsip_func_subset_sb_t;

/*
 * Private/Primitive function subsets
 */

typedef struct st_rsip_func
{
    rsip_func_key_generate_t p_key_generate_aes[RSIP_KEY_AES_NUM];
    rsip_func_key_generate_t p_key_generate_hmac[RSIP_KEY_HMAC_NUM];

    rsip_func_key_pair_generate_t  p_key_pair_generate_ecc[RSIP_KEY_ECC_NUM];

    rsip_func_encrypted_key_wrap_t p_encrypted_key_wrap_aes[RSIP_KEY_AES_NUM];
    rsip_func_encrypted_key_wrap_t p_encrypted_key_wrap_ecc_pub[RSIP_KEY_ECC_NUM];
    rsip_func_encrypted_key_wrap_t p_encrypted_key_wrap_ecc_priv[RSIP_KEY_ECC_NUM];
    rsip_func_encrypted_key_wrap_t p_encrypted_key_wrap_hmac[RSIP_KEY_HMAC_NUM];

    rsip_func_initial_key_update_key_wrap_t p_initial_key_update_key_wrap_aes;

    rsip_func_rfc3394_key_wrap_t   p_rfc3394_key_wrap[RSIP_KEY_AES_NUM];
    rsip_func_rfc3394_key_unwrap_t p_rfc3394_key_unwrap[RSIP_KEY_AES_NUM];

    rsip_func_subset_aes_cipher_t p_aes_cipher[RSIP_KEY_AES_NUM];
    rsip_func_subset_aes_gcm_t    p_aes_gcm_enc[RSIP_KEY_AES_NUM];
    rsip_func_subset_aes_gcm_t    p_aes_gcm_dec[RSIP_KEY_AES_NUM];
    rsip_func_subset_aes_ccm_t    p_aes_ccm_enc[RSIP_KEY_AES_NUM];
    rsip_func_subset_aes_ccm_t    p_aes_ccm_dec[RSIP_KEY_AES_NUM];
    rsip_func_subset_aes_mac_t    p_aes_mac[RSIP_KEY_AES_NUM];

    rsip_func_ecdsa_sign_t   p_ecdsa_sign[RSIP_KEY_ECC_NUM];
    rsip_func_ecdsa_verify_t p_ecdsa_verify[RSIP_KEY_ECC_NUM];

    rsip_func_rng_t   p_rng;
    rsip_func_ghash_t p_ghash_compute;

    rsip_func_subset_fwup_t p_fwup;
    rsip_func_subset_sb_t  p_sb;
} rsip_func_t;

/* OEM Command */
typedef enum e_rsip_oem_cmd
{
    RSIP_OEM_CMD_INVALID                     = 0U,
    RSIP_OEM_CMD_AES128                      = 5U,
    RSIP_OEM_CMD_AES256                      = 7U,
    RSIP_OEM_CMD_ECC_SECP256R1_PUBLIC        = 22U,
    RSIP_OEM_CMD_ECC_SECP256R1_PRIVATE       = 23U,
    RSIP_OEM_CMD_HMAC_SHA224                 = 26U,
    RSIP_OEM_CMD_HMAC_SHA256                 = 27U,
    RSIP_OEM_CMD_ECC_BRAINPOOLP256R1_PUBLIC  = 28U,
    RSIP_OEM_CMD_ECC_BRAINPOOLP256R1_PRIVATE = 29U,
    RSIP_OEM_CMD_ECC_SECP256K1_PUBLIC        = 34U,
    RSIP_OEM_CMD_ECC_SECP256K1_PRIVATE       = 35U,
    RSIP_OEM_CMD_KUK                         = 0xff,
} rsip_oem_cmd_t;

typedef enum e_rsip_aes_mode_extended
{
    RSIP_AES_MODE_EXTENDED_ECB_ENCRYPT,
    RSIP_AES_MODE_EXTENDED_ECB_DECRYPT,
    RSIP_AES_MODE_EXTENDED_CBC_ENCRYPT,
    RSIP_AES_MODE_EXTENDED_CBC_DECRYPT,
    RSIP_AES_MODE_EXTENDED_CTR_CRYPT
} rsip_aes_mode_extended_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

extern const bool g_sha_enabled[RSIP_HASH_TYPE_NUM];
extern const bool g_hmac_enabled[RSIP_KEY_HMAC_NUM];

extern const rsip_func_t g_func;

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes the RSIP engine.
 *
 * @return The return value of the internally called primitive functions.
 **********************************************************************************************************************/
rsip_err_t r_rsip_open (void);

/*******************************************************************************************************************//**
 * Finalizes the RSIP engine.
 *
 * @return The return value of the internally called primitive functions.
 **********************************************************************************************************************/
rsip_err_t r_rsip_close (void);

/*******************************************************************************************************************//**
 * Start the FWUP.
 *
 * @return The return value of the internally called primitive functions.
 **********************************************************************************************************************/
rsip_err_t r_rsip_fwup_start (void);

/*******************************************************************************************************************//**
 * Stores input Key Update Key (KUK).
 *
 * @param[in] p_key_update_key_value KUK value.
 **********************************************************************************************************************/
void r_rsip_kuk_store (const uint8_t * p_key_update_key_value);

rsip_err_t r_rsip_sha224initprivate (rsip_sha_handle_t * handle);
rsip_err_t r_rsip_sha256initprivate (rsip_sha_handle_t * handle);
rsip_err_t r_rsip_sha1sha2_init_update (uint8_t        * current_hash,
                                        uint8_t        * p_message,
                                        uint64_t         message_length,
                                        uint32_t       * internal_state);
rsip_err_t r_rsip_sha1sha2_update (uint8_t         * current_hash,
                                    uint8_t        * p_message,
                                    uint64_t         message_length,
                                    uint32_t       * internal_state);
rsip_err_t r_rsip_sha1sha2_init_final (uint8_t         * current_hash,
                                        uint8_t        * p_message,
                                        uint64_t         message_length,
                                        uint8_t        * p_digest);
rsip_err_t r_rsip_sha1sha2_final (uint8_t          * current_hash,
                                    uint8_t        * p_message,
                                    uint64_t         message_length,
                                    uint64_t         total_message_length,
                                    uint8_t        * p_digest,
                                    uint32_t       * internal_state);

rsip_err_t r_rsip_hmac_init_update (const rsip_wrapped_key_t * p_wrapped_key,
                                    uint8_t                  * p_message,
                                    uint64_t                   message_length,
                                    uint32_t                 * internal_state);
rsip_err_t r_rsip_hmac_update (const rsip_wrapped_key_t  * p_wrapped_key,
                                uint8_t                  * p_message,
                                uint64_t                   message_length,
                                uint32_t                 * internal_state);
rsip_err_t r_rsip_hmac_init_final (const rsip_wrapped_key_t  * p_wrapped_key,
                                    uint8_t                  * p_message,
                                    uint64_t                   message_length,
                                    uint8_t                  * p_mac);
rsip_err_t r_rsip_hmac_final (const rsip_wrapped_key_t   * p_wrapped_key,
                                uint8_t                  * p_message,
                                uint64_t                   message_length,
                                uint64_t                   total_message_length,
                                uint8_t                  * p_mac,
                                uint32_t                 * internal_state);
rsip_err_t r_rsip_hmac_init_verify (const rsip_wrapped_key_t * p_wrapped_key,
                                    uint8_t                  * p_message,
                                    uint64_t                   message_length,
                                    const uint8_t            * p_mac,
                                    uint32_t                   mac_length);
rsip_err_t r_rsip_hmac_verify (const rsip_wrapped_key_t  * p_wrapped_key,
                                uint8_t                  * p_message,
                                uint64_t                   message_length,
                                uint64_t                   total_message_length,
                                const uint8_t            * p_mac,
                                uint32_t                   mac_length,
                                uint32_t                 * internal_state);

rsip_oem_cmd_t get_oem_key_cmd (rsip_key_type_t key_type);
uint32_t get_encrypted_key_size (rsip_key_type_t key_type);
void r_rsip_kuk_set (const uint8_t * p_key_update_key_value);
rsip_func_initial_key_wrap_t select_func_initial_key_wrap (rsip_key_type_t key_type);

/**********************************************************************************************************************
 * Function Name: r_rsip_byte_to_word_convert
 * Description  : Converts byte data to word (4-byte) and rounds up it.
 * Arguments    : bytes data
 * Return Value : the bytes data to word
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_byte_to_word_convert)
uint32_t r_rsip_byte_to_word_convert(const uint32_t bytes)
{
    return (bytes + 3) >> 2;
}

/**********************************************************************************************************************
 * Function Name: r_rsip_byte_to_bit_convert_upper
 * Description  : Converts byte data to bit data. This function returns upper 3 digits.
 * Arguments    : bytes data
 * Return Value : the bytes data to bit
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_byte_to_bit_convert_upper)
uint32_t r_rsip_byte_to_bit_convert_upper(const uint64_t bytes)
{
    /* cast to uint32_t */
    return (uint32_t) (bytes >> 29);
}

/**********************************************************************************************************************
 * Function Name: r_rsip_byte_to_bit_convert_lower
 * Description  : Converts byte data to bit data. This function returns lower 32 digits.
 * Arguments    : bytes data
 * Return Value : the bytes data to bit
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(r_rsip_byte_to_bit_convert_lower)
uint32_t r_rsip_byte_to_bit_convert_lower(const uint64_t bytes)
{
    /* cast to uint32_t */
    return (uint32_t) (bytes << 3);
}

#endif                                 /* R_RSIP_PRIVATE_H */
