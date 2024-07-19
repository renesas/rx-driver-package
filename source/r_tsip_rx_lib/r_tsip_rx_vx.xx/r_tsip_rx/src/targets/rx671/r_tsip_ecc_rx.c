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
 * Copyright (C) 2019-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_tsip_ecc_rx.c
 * Version      : 1.20
 * Description  : Interface definition for the r_tsip_ecc_rx TSIP module.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.09.2019 1.08     First Release
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
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "r_tsip_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Hash data output position */
#define TSIP_PRV_HASH_WORD_POS_ECDSA_P192   (2)             /* For ECDSA P-192 : (256 - 192) / 32 */
#define TSIP_PRV_HASH_WORD_POS_ECDSA_P224   (1)             /* For ECDSA P-224 : (256 - 224) / 32 */
#define TSIP_PRV_HASH_WORD_POS_ECDSA_P256   (0)             /* For ECDSA P-256 : (256 - 256) / 32 */
#define TSIP_PRV_HASH_WORD_POS_ECDSA_P384   (0xFFFFFFFCul)  /* For ECDSA P-384 : (256 - 384) / 32 */

/* Block length (in bytes) of ECDH */
#define TSIP_PRV_INDATA_BYTEDATA_LEN        (32 * 4)            /* 32 words */
#define TSIP_PRV_OTHER_INFO_BYTE_LEN_2B     ((7 + 16 * 1) * 4)  /* 2 blocks */
#define TSIP_PRV_OTHER_INFO_BYTE_LEN_3B     ((7 + 16 * 2) * 4)  /* 3 blocks */

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if TSIP_ECDH_P256
/* For R_TSIP_EcdhP256ReadPublicKey */
typedef struct st_read_public_key
{
    uint32_t    cmd;
    uint8_t     bytedata[TSIP_PRV_INDATA_BYTEDATA_LEN];
} st_read_public_key_t;

/* For R_TSIP_EcdhP256KeyDerivation */
typedef struct st_key_derivation
{
    uint32_t    keyindextype;
    uint32_t    kdftype;
    uint32_t    max_cnt_byte;
    uint8_t     paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_3B];
} st_key_derivation_t;
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
extern uint32_t g_private_id_counter;
#if TSIP_ECDH_P256
uint32_t g_ecdh256_private_id;
#endif

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if TSIP_ECDH_P256
static const TSIP_KEY_INDEX_TYPE s_key_index_type[3] =
{
    TSIP_KEY_INDEX_TYPE_AES128_FOR_ECDH, TSIP_KEY_INDEX_TYPE_AES256_FOR_ECDH, TSIP_KEY_INDEX_TYPE_HMAC_SHA256_FOR_ECDH
};
#endif

static e_tsip_err_t set_ecdsa_hash_data(tsip_ecdsa_byte_data_t *p_message_hash, uint32_t *data_buff,
        uint32_t hash_word_pos);

#if TSIP_PRV_USE_ECDSA
/***********************************************************************************************************************
* Function Name: set_ecdsa_hash_data
*******************************************************************************************************************/ /**
* @details       For messages, SHA hash operation is performed and the hash value is set according to ECDSA.
* @param[in]     p_message_hash message or hash value information
* @param[out]    data_buff pointer to the hash value area, including 0 padding
* @param[in]     hash_word_pos hash value output word position from the beginning of the data_buff
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: resource conflict
* @retval        TSIP_ERR_FAIL: Input parameter illegal
* @see           R_TSIP_Sha256Init()
* @see           R_TSIP_Sha256Update()
* @see           R_TSIP_Sha256Final()
* @note          None
*/
static e_tsip_err_t set_ecdsa_hash_data(tsip_ecdsa_byte_data_t *p_message_hash, uint32_t *data_buff,
        uint32_t hash_word_pos)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    tsip_sha_md5_handle_t ex_sha_handle =
    {
        0
    };
    uint32_t hash_length = 0;

    if (0 == p_message_hash->data_type)
    {
        if (TSIP_PRV_HASH_WORD_POS_ECDSA_P384 != hash_word_pos)
        {
#if TSIP_SHA_256
            R_TSIP_Sha256Init(&ex_sha_handle);
            error_code = R_TSIP_Sha256Update(&ex_sha_handle, p_message_hash->pdata, p_message_hash->data_length);
            if (TSIP_SUCCESS == error_code)
            {
                /* Casting uint8_t pointer is used for address. */
                error_code = R_TSIP_Sha256Final(&ex_sha_handle, (uint8_t*)&data_buff[hash_word_pos], &hash_length);
            }
#else
    #error Please specify TSIP_SHA_256 macro to 1 if you need ECDSA in r_tsip_rx_config.h
#endif  /* TSIP_SHA_256 */
        }
        else
        {
#if TSIP_USER_SHA_384_ENABLED
            /* Casting uint8_t pointer is used for address. */
            if (0 != TSIP_USER_SHA_384_FUNCTION(p_message_hash->pdata, (uint8_t*)data_buff,
                p_message_hash->data_length))
#else
#endif  /* TSIP_USER_SHA_384_ENABLED */
            {
                error_code = TSIP_ERR_FAIL;
            }
        }
    }
    else if (1 == p_message_hash->data_type)
    {
        if (TSIP_PRV_HASH_WORD_POS_ECDSA_P384 != hash_word_pos)
        {
            memcpy(&data_buff[hash_word_pos], p_message_hash->pdata,
                    R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE - hash_word_pos * (sizeof(uint32_t)));
        }
        else
        {
            memcpy(data_buff, p_message_hash->pdata, R_TSIP_SHA384_HASH_LENGTH_BYTE_SIZE);
        }
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }

    return error_code;
}
/*******************************
 End of function set_ecdsa_hash_data
 *******************************/
#endif /* TSIP_PRV_USE_ECDSA */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP192PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-192 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-192 public key encrypted and MAC.
* @param[out]    key_index ECC P-192 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP192PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(2); /* P-192 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPublicKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P192_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP192PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP224PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-224 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-224 public key encrypted and MAC.
* @param[out]    key_index ECC P-224 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP224PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(1); /* P-224 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPublicKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P224_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP224PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP256PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-256 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-256 public key encrypted and MAC.
* @param[out]    key_index ECC P-256 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP256PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(0); /* P-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPublicKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP256PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP384PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-384 public user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-384 public key encrypted and MAC.
* @param[out]    key_index ECC P-384 public key user key generation information (33 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccP384PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP384PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    error_code = R_TSIP_GenerateEccP384PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P384_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP384PublicKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP192PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-192 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-192 private key encrypted and MAC.
* @param[out]    key_index ECC P-192 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP192PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(2); /* P-192 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPrivateKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P192_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP192PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP224PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-224 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-224 private key encrypted and MAC.
* @param[out]    key_index ECC P-224 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP224PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(1); /* P-224 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPrivateKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P224_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP224PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP256PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-256 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-256 private key encrypted and MAC.
* @param[out]    key_index ECC P-256 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP256PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    indata_cmd = change_endian_long(0); /* P-256 */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_GenerateEccPrivateKeyIndexSub(&install_key_ring_index, (uint32_t*)encrypted_provisioning_key,
        /* Casting uint32_t pointer is used for address. */
        &indata_cmd, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP256PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP384PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-384 private user key index.
* @param[in]     encrypted_provisioning_key Provisioning key wrapped in DLM.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-384 private key encrypted and MAC.
* @param[out]    key_index ECC P-384 private key user key generation information (21 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccP384PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP384PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_ecc_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;

    error_code = R_TSIP_GenerateEccP384PrivateKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P384_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateEccP384PrivateKeyIndex
 *******************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP192PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-192 public user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-192 public key encrypted and MAC.
* @param[out]    key_index ECC P-192 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP192PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(2); /* P-192 */
    error_code = R_TSIP_UpdateEccPublicKeyIndexSub(&indata_cmd,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P192_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateEccP192PublicKeyIndex
 *************************************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP224PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-224 public user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-224 public key encrypted and MAC.
* @param[out]    key_index ECC P-224 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP224PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(1); /* P-224 */
    error_code = R_TSIP_UpdateEccPublicKeyIndexSub(&indata_cmd,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P224_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateEccP224PublicKeyIndex
 *************************************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP256PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-256 public user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-256 public key encrypted and MAC.
* @param[out]    key_index ECC P-256 public key user key generation information (25 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccPublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP256PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(0); /* P-256 */
    error_code = R_TSIP_UpdateEccPublicKeyIndexSub(&indata_cmd,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateEccP256PublicKeyIndex
 *************************************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP384PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-384 public user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-384 public key encrypted and MAC.
* @param[out]    key_index ECC P-384 public key user key generation information (33 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccP384PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP384PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    error_code = R_TSIP_UpdateEccP384PublicKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P384_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateEccP384PublicKeyIndex
 *************************************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP192PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-192 private user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-192 private key encrypted and MAC.
* @param[out]    key_index ECC P-192 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP192PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(2); /* P-192 */
    error_code = R_TSIP_UpdateEccPrivateKeyIndexSub(&indata_cmd,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P192_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/**************************************************
 End of function R_TSIP_UpdateEccP192PrivateKeyIndex
 **************************************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP224PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-224 private user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-224 private key encrypted and MAC.
* @param[out]    key_index ECC P-224 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP224PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(1); /* P-224 */
    error_code = R_TSIP_UpdateEccPrivateKeyIndexSub(&indata_cmd,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P224_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/**************************************************
 End of function R_TSIP_UpdateEccP224PrivateKeyIndex
 **************************************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP256PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-256 private user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-256 private key encrypted and MAC.
* @param[out]    key_index ECC P-256 private key user key generation information (17 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccPrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP256PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(0); /* P-256 */
    error_code = R_TSIP_UpdateEccPrivateKeyIndexSub(&indata_cmd,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/**************************************************
 End of function R_TSIP_UpdateEccP256PrivateKeyIndex
 **************************************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateEccP384PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Update ECC P-384 private user key index.
* @param[in]     iv The initial vector used when encrypted_key was generated.
* @param[in]     encrypted_key ECC P-384 private key encrypted and MAC.
* @param[out]    key_index ECC P-384 private key user key generation information (21 words).
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateEccP384PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateEccP384PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_ecc_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    error_code = R_TSIP_UpdateEccP384PrivateKeyIndexSub(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P384_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/**************************************************
 End of function R_TSIP_UpdateEccP384PrivateKeyIndex
 **************************************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP192RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-192 random user key pair index.
* @param[out]    key_pair_index Output the pair of user private key and user public key generation information.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccRandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP192RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(2); /* P-192 */
    error_code =  R_TSIP_GenerateEccRandomKeyIndexSub(&indata_cmd,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)&key_pair_index->public.value, (uint32_t*)&key_pair_index->private.value);
    if (TSIP_SUCCESS == error_code)
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_ECC_P192_PUBLIC;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_ECC_P192_PRIVATE;
    }
    else
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_INVALID;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateEccP192RandomKeyIndex
 ***************************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP224RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-224 random user key pair index.
* @param[out]    key_pair_index Output the pair of user private key and user public key generation information.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccRandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP224RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(1); /* P-224 */
    error_code =  R_TSIP_GenerateEccRandomKeyIndexSub(&indata_cmd,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)&key_pair_index->public.value, (uint32_t*)&key_pair_index->private.value);
    if (TSIP_SUCCESS == error_code)
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_ECC_P224_PUBLIC;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_ECC_P224_PRIVATE;
    }
    else
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_INVALID;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateEccP224RandomKeyIndex
 ***************************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP256RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-256 random user key pair index.
* @param[out]    key_pair_index Output the pair of user private key and user public key generation information.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccRandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP256RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    indata_cmd = change_endian_long(0); /* P-256 */
    error_code =  R_TSIP_GenerateEccRandomKeyIndexSub(&indata_cmd,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)&key_pair_index->public.value, (uint32_t*)&key_pair_index->private.value);
    if (TSIP_SUCCESS == error_code)
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_ECC_P256_PUBLIC;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_ECC_P256_PRIVATE;
    }
    else
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_INVALID;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateEccP256RandomKeyIndex
 ***************************************/
#endif /* TSIP_ECDSA_P256 || TSIP_ECDH_P256 || TSIP_TLS */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateEccP384RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate ECC P-384 random user key pair index.
* @param[out]    key_pair_index Output the pair of user private key and user public key generation information.
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateEccP384RandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP384RandomKeyIndex(tsip_ecc_key_pair_index_t *key_pair_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;

    error_code =  R_TSIP_GenerateEccP384RandomKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)&key_pair_index->public.value, (uint32_t*)&key_pair_index->private.value);
    if (TSIP_SUCCESS == error_code)
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_ECC_P384_PUBLIC;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_ECC_P384_PRIVATE;
    }
    else
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_INVALID;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_INVALID;
    }

    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateEccP384RandomKeyIndex
 ***************************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP192SignatureGenerate
*******************************************************************************************************************/ /**
* @details       ECDSA P-192 signature generation
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index ECC P-192 private key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaSigunatureGenerateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP192SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[TSIP_PRV_HASH_WORD_POS_ECDSA_P192 + (R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P192);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    indata_cmd = change_endian_long(2); /* P-192 */
    error_code = R_TSIP_EcdsaSigunatureGenerateSub(
            /* Casting uint32_t pointer is used for address. */
            &indata_cmd, (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = R_TSIP_ECDSA_DATA_BYTE_SIZE;
    }

    return error_code;

}
/*******************************
 End of function R_TSIP_EcdsaP192SignatureGenerate
 *******************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP224SignatureGenerate
*******************************************************************************************************************/ /**
* @details       ECDSA P-224 signature generation
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index ECC P-192 private key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaSigunatureGenerateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP224SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[TSIP_PRV_HASH_WORD_POS_ECDSA_P224 + (R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P224);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    indata_cmd = change_endian_long(1); /* P-224 */
    error_code = R_TSIP_EcdsaSigunatureGenerateSub(
            /* Casting uint32_t pointer is used for address. */
            &indata_cmd, (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = R_TSIP_ECDSA_DATA_BYTE_SIZE;
    }

    return error_code;

}
/*******************************
 End of function R_TSIP_EcdsaP224SignatureGenerate
 *******************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP256SignatureGenerate
*******************************************************************************************************************/ /**
* @details       ECDSA P-256 signature generation
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index ECC P-192 private key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaSigunatureGenerateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP256SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[TSIP_PRV_HASH_WORD_POS_ECDSA_P256 + (R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P256);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    indata_cmd = change_endian_long(0); /* P-256 */
    error_code = R_TSIP_EcdsaSigunatureGenerateSub(
            /* Casting uint32_t pointer is used for address. */
            &indata_cmd, (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = R_TSIP_ECDSA_DATA_BYTE_SIZE;
    }

    return error_code;

}
/*******************************
 End of function R_TSIP_EcdsaP256SignatureGenerate
 *******************************/
#endif /* TSIP_ECDSA_P256 */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP384SignatureGenerate
*******************************************************************************************************************/ /**
* @details       ECDSA P-384 signature generation
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index ECC P-384 private key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaP384SigunatureGenerateSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP384SignatureGenerate(tsip_ecdsa_byte_data_t *message_hash, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[(R_TSIP_SHA384_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P384);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = R_TSIP_EcdsaP384SigunatureGenerateSub(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = R_TSIP_ECDSA_P384_DATA_BYTE_SIZE;
    }

    return error_code;

}
/*******************************
 End of function R_TSIP_EcdsaP384SignatureGenerate
 *******************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDSA_P192
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP192SignatureVerification
*******************************************************************************************************************/ /**
* @details       ECDSA P-192 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index ECC P-192 public key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred or signature verification failed.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaSigunatureVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP192SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[TSIP_PRV_HASH_WORD_POS_ECDSA_P192 + (R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P192);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    indata_cmd = change_endian_long(2); /* P-192 */
    error_code = R_TSIP_EcdsaSigunatureVerificationSub(
            /* Casting uint32_t pointer is used for address. */
            &indata_cmd, (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);

    return error_code;
}
/*******************************
 End of function R_TSIP_EcdsaP192SignatureVerification
 *******************************/
#endif /* TSIP_ECDSA_P192 */

#if TSIP_ECDSA_P224
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP224SignatureVerification
*******************************************************************************************************************/ /**
* @details       ECDSA P-224 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index ECC P-224 public key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred or signature verification failed.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaSigunatureVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP224SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[TSIP_PRV_HASH_WORD_POS_ECDSA_P224 + (R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P224);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    indata_cmd = change_endian_long(1); /* P-224 */
    error_code = R_TSIP_EcdsaSigunatureVerificationSub(
            /* Casting uint32_t pointer is used for address. */
            &indata_cmd, (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);

    return error_code;
}
/*******************************
 End of function R_TSIP_EcdsaP224SignatureVerification
 *******************************/
#endif /* TSIP_ECDSA_P224 */

#if TSIP_ECDSA_P256
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP256SignatureVerification
*******************************************************************************************************************/ /**
* @details       ECDSA P-256 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index ECC P-256 public key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred or signature verification failed.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaSigunatureVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP256SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[TSIP_PRV_HASH_WORD_POS_ECDSA_P256 + (R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P256);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    indata_cmd = change_endian_long(0); /* P-256 */
    error_code = R_TSIP_EcdsaSigunatureVerificationSub(
            /* Casting uint32_t pointer is used for address. */
            &indata_cmd, (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);

    return error_code;
}
/*******************************
 End of function R_TSIP_EcdsaP256SignatureVerification
 *******************************/
#endif /* TSIP_ECDSA_P256 */

#if TSIP_ECDSA_P384
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdsaP384SignatureVerification
*******************************************************************************************************************/ /**
* @details       ECDSA P-384 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index ECC P-384 public key index
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred or signature verification failed.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           set_ecdsa_hash_data()
* @see           R_TSIP_EcdsaP384SigunatureVerificationSub()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaP384SignatureVerification(tsip_ecdsa_byte_data_t *signature,
        tsip_ecdsa_byte_data_t *message_hash, tsip_ecc_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t data_buff[(R_TSIP_SHA384_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t))] =
    {
        0
    };

    error_code = set_ecdsa_hash_data(message_hash, data_buff, TSIP_PRV_HASH_WORD_POS_ECDSA_P384);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = R_TSIP_EcdsaP384SigunatureVerificationSub(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);

    return error_code;
}
/*******************************
 End of function R_TSIP_EcdsaP384SignatureVerification
 *******************************/
#endif /* TSIP_ECDSA_P384 */

#if TSIP_ECDH_P256
/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhP256Init
*******************************************************************************************************************/ /**
* @details       Prepare for ECDH key exchange.
* @param[out]    handle Handler for ECDH(work area).
* @param[in]     key_type type of key to be exchanged(0:ECDHE, 1:ECDH)
* @param[in]     use_key_id key_id use or not(0:do not use the key_id, 1:use the key_id)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhP256Init(tsip_ecdh_handle_t *handle, uint32_t key_type, uint32_t use_key_id)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    if ((1 >= key_type) && (1 >= use_key_id))
    {
        memset(handle, 0, sizeof(tsip_ecdh_handle_t));
        handle->flag_call_init = 1;
        g_private_id_counter++;
        g_ecdh256_private_id = g_private_id_counter;
        handle->id = g_ecdh256_private_id;
        handle->key_type = key_type;
        handle->flag_use_key_id = use_key_id;
    }
    else
    {
        error_code = TSIP_ERR_PARAMETER;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_EcdhP256Init
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhP256ReadPublicKey
*******************************************************************************************************************/ /**
* @details       Verify signature of ECC P-256 public key and output public key generation information.
* @param[in,out] handle Handler for ECDH(work area).
* @param[in]     public_key_index public key index for signature verification
* @param[in]     public_key_data public key data("QeU" or "key_id || QeU")
* @param[in]     signature signature information
* @param[out]    key_index public key index for public_key_data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_EcdhReadPublicKeyPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhP256ReadPublicKey(tsip_ecdh_handle_t *handle, tsip_ecc_public_key_index_t *public_key_index,
        uint8_t *public_key_data, tsip_ecdsa_byte_data_t *signature, tsip_ecc_public_key_index_t *key_index)
{
    st_read_public_key_t indata =
    {
        0
    };
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if ((0 == handle->flag_call_init) || (1 == handle->flag_call_read_public))
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_ecdh256_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->flag_call_read_public = 1;

    if (1 == handle->flag_use_key_id)
    {
        indata.cmd = change_endian_long(1);
        memcpy(indata.bytedata, public_key_data, 1 + R_TSIP_ECC_PUBLIC_KEY_BYTE_SIZE);  /* key_id || QeU copy */
        indata.bytedata[1 + R_TSIP_ECC_PUBLIC_KEY_BYTE_SIZE] = 0x80;    /* stop bit */
        indata.bytedata[TSIP_PRV_INDATA_BYTEDATA_LEN - 2] = 0x02;
        indata.bytedata[TSIP_PRV_INDATA_BYTEDATA_LEN - 1] = 0x08;   /* message length is 520bit */
    }
    else
    {
        indata.cmd = change_endian_long(0);
        memcpy(indata.bytedata, public_key_data, R_TSIP_ECC_PUBLIC_KEY_BYTE_SIZE);  /* QeU copy */
        indata.bytedata[R_TSIP_ECC_PUBLIC_KEY_BYTE_SIZE] = 0x80;    /* stop bit */
        indata.bytedata[TSIP_PRV_INDATA_BYTEDATA_LEN - 2] = 0x02;
        indata.bytedata[TSIP_PRV_INDATA_BYTEDATA_LEN - 1] = 0x00;   /* message length is 512bit */
    }

    error_code = R_TSIP_EcdhReadPublicKeyPrivate(&indata.cmd,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&public_key_index->value, (uint32_t*)indata.bytedata, (uint32_t*)signature->pdata, (uint32_t*)
            &key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_EcdhP256ReadPublicKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhP256MakePublicKey
*******************************************************************************************************************/ /**
* @details       Generate signature for public key generation information for ECDH key exchange.
* @param[in,out] handle Handler for ECDH(work area).
* @param[in]     public_key_index public key index(for ECDH)
* @param[in]     private_key_index private key index for signature generation
* @param[out]    public_key public key for key exchange
* @param[out]    signature signature information
* @param[out]    key_index private key index generated from random numbers(for ECDHE)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_EcdhMakePublicKeyPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhP256MakePublicKey(tsip_ecdh_handle_t *handle, tsip_ecc_public_key_index_t *public_key_index,
        tsip_ecc_private_key_index_t *private_key_index, uint8_t *public_key, tsip_ecdsa_byte_data_t *signature,
        tsip_ecc_private_key_index_t *key_index)
{
    uint32_t indata_cmd = 0;
    uint32_t ecdh_key_type = 0;
    uint32_t key_id = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if ((0 == handle->flag_call_init) || (1 == handle->flag_call_make_public))
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_ecdh256_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->flag_call_make_public = 1;

    if (1 == handle->key_type)
    {
        ecdh_key_type = change_endian_long(1);
    }
    if (1 == handle->flag_use_key_id)
    {
        indata_cmd = change_endian_long(1);
        key_id = change_endian_long(handle->key_id & 0x000000FF);
    }
    else
    {
        indata_cmd = change_endian_long(0);
    }

    error_code = R_TSIP_EcdhMakePublicKeyPrivate(&indata_cmd, &ecdh_key_type,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&public_key_index->value, (uint32_t*)&private_key_index->value, &key_id,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)public_key, (uint32_t*)signature->pdata, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = R_TSIP_ECDSA_DATA_BYTE_SIZE;
    }
    if (1 != handle->key_type)
    {
        if (TSIP_SUCCESS == error_code)
        {
            key_index->type = TSIP_KEY_INDEX_TYPE_ECC_P256_PRIVATE;
        }
        else
        {
            key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
        }
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_EcdhP256MakePublicKey
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhP256CalculateSharedSecretIndex
*******************************************************************************************************************/ /**
* @details       Output key generation information of shared secret "Z" from partner public key and your private key.
* @param[in,out] handle Handler for ECDH(work area).
* @param[in]     public_key_index public key index
* @param[in]     private_key_index private key index
* @param[out]    shared_secret_index key index of shared secret "Z"
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_EcdhCalculateSharedSecretIndexPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhP256CalculateSharedSecretIndex(tsip_ecdh_handle_t *handle,
        tsip_ecc_public_key_index_t *public_key_index, tsip_ecc_private_key_index_t *private_key_index,
        tsip_ecdh_key_index_t *shared_secret_index)
{
    uint32_t ecdh_key_type = 0;
    e_tsip_err_t error_code = TSIP_SUCCESS;

    if (0 == handle->flag_call_init)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    if (handle->id != g_ecdh256_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }

    handle->flag_call_shared_secret = 1;

    ecdh_key_type = change_endian_long(handle->key_type);

    error_code = R_TSIP_EcdhCalculateSharedSecretIndexPrivate(&ecdh_key_type,
    /* Casting uint32_t pointer is used for address. */
    (uint32_t*)&public_key_index->value, (uint32_t*)&private_key_index->value, (uint32_t*)&shared_secret_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        shared_secret_index->type = TSIP_KEY_INDEX_TYPE_ECDH_SHARED_SECRET;
    }
    else
    {
        shared_secret_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_EcdhP256CalculateSharedSecretIndex
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdhP256KeyDerivation
*******************************************************************************************************************/ /**
* @details       Key is derived using the shared secret "Z" as the key material.
* @param[in,out] handle Handler for ECDH(work area).
* @param[in]     shared_secret_index key index of shared secret "Z"
* @param[in]     key_type Derived key type(0:AES-128, 1:AES-256, 2:SHA256-HMAC)
* @param[in]     kdf_type Algorithm used for key derivation calculation(0:SHA256, 1:SHA256-HMAC)
* @param[in]     other_info additional data used in key derivation
* @param[in]     other_info_length additional data length(byte)
* @param[in]     salt_key_index Salt key index
* @param[out]    key_index Key index corresponding to key_type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_PROHIBIT_FUNCTION: Illegal function called
* @see           R_TSIP_EcdhKeyDerivationPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdhP256KeyDerivation(tsip_ecdh_handle_t *handle, tsip_ecdh_key_index_t *shared_secret_index,
        uint32_t key_type, uint32_t kdf_type, uint8_t *other_info, uint32_t other_info_length,
        tsip_hmac_sha_key_index_t *salt_key_index, tsip_aes_key_index_t *key_index)
{
    /* InData_PaddedMsg = AlgorithmID || PartyUInfo || PartyVInfo || 1 || 0padding || Message length(64bit)
     * Message length value = AlgorithmID bit length + PartyUInfo bit length + PartyVInfo bit length + 288bit
     * MAX_CNT(word) = 7 + 16n */

    e_tsip_err_t error_code = TSIP_SUCCESS;
    st_key_derivation_t indata =
    {
        0
    };
    uint32_t message_bit_length = 0;

    if (0 == handle->flag_call_shared_secret)
    {
        return TSIP_ERR_PROHIBIT_FUNCTION;
    }
    handle->flag_call_init = 0;
    handle->flag_call_make_public = 0;
    handle->flag_call_read_public = 0;
    handle->flag_call_shared_secret = 0;
    if (handle->id != g_ecdh256_private_id)
    {
        return TSIP_ERR_PARAMETER;
    }
    /* 147 = TSIP_PRV_OTHER_INFO_BYTE_LEN_3B - 9 */
    if (((2 < key_type) || (1 < kdf_type)) || (147 < other_info_length))
    {
        return TSIP_ERR_PARAMETER;
    }

    message_bit_length = (other_info_length * 8) + 288;
    if (0 != kdf_type)
    {
        message_bit_length += (64 * 8);
    }

    memcpy(indata.paddedmsg, other_info, other_info_length);
    indata.paddedmsg[other_info_length] = 0x80; /* stop bit */

    if ((TSIP_PRV_OTHER_INFO_BYTE_LEN_2B - 9) >= other_info_length)
    {
        /* another block unnecessary */
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_2B - 4] = (uint8_t)((message_bit_length >> 24) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_2B - 3] = (uint8_t)((message_bit_length >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_2B - 2] = (uint8_t)((message_bit_length >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_2B - 1] = (uint8_t)((message_bit_length) & 0xff);
        indata.max_cnt_byte = TSIP_PRV_OTHER_INFO_BYTE_LEN_2B;
    }
    else
    {
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_3B - 4] = (uint8_t)((message_bit_length >> 24) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_3B - 3] = (uint8_t)((message_bit_length >> 16) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_3B - 2] = (uint8_t)((message_bit_length >> 8) & 0xff);
        /* Casting uint32_t data to uint8_t data array. */
        indata.paddedmsg[TSIP_PRV_OTHER_INFO_BYTE_LEN_3B - 1] = (uint8_t)((message_bit_length) & 0xff);
        indata.max_cnt_byte = TSIP_PRV_OTHER_INFO_BYTE_LEN_3B;
    }

    indata.keyindextype = change_endian_long(key_type);
    indata.kdftype      = change_endian_long(kdf_type);

    error_code = R_TSIP_EcdhKeyDerivationPrivate(&indata.keyindextype, shared_secret_index->value, &indata.kdftype,
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)indata.paddedmsg, indata.max_cnt_byte >> 2, salt_key_index->value, key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = s_key_index_type[key_type];
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    memset(handle, 0, sizeof(tsip_ecdh_handle_t));
    return error_code;
}
/*******************************
 End of function R_TSIP_EcdhP256KeyDerivation
 *******************************/
#endif  /* TSIP_ECDH_P256 */

/***********************************************************************************************************************
* Function Name: R_TSIP_EcdheP512KeyAgreement
*******************************************************************************************************************/ /**
* @details       Generate key pair with Brainpool P512r1 and calculate ECDHE
* @param[in]     key_index key index information
* @param[in]     receiver_public_key Brainpool P512r1 public key Q of receiver (1024 bit) || MAC (128 bit)
* @param[out]    sender_public_key Brainpool P512r1 public key Q of sender (1024 bit) || MAC (128 bit)
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_EcdheP512KeyAgreementPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_EcdheP512KeyAgreement(tsip_aes_key_index_t *key_index, uint8_t *receiver_public_key,
        uint8_t *sender_public_key)
{
    e_tsip_err_t error_code;

    /* Casting uint8_t pointer to uint32_t pointer to adjust to TSIP key format. */
    error_code = R_TSIP_EcdheP512KeyAgreementPrivate(
    /* uint32_t *InData_KeyIndex,   */key_index->value,
    /* uint32_t *InData_PubKey,     */(uint32_t *)receiver_public_key,
    /* uint32_t *OutData_PubKey     */(uint32_t *)sender_public_key
    );

    return error_code;
}
/*******************************
 End of function R_TSIP_EcdheP512KeyAgreement
 *******************************/

