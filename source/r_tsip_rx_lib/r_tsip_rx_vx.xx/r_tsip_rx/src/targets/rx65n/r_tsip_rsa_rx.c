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
 * File Name    : r_tsip_rsa_rx.c
 * Version      : 1.20
 * Description  : Interface definition for the r_tsip_rsa_rx TSIP module.
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
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "r_tsip_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if TSIP_PRV_USE_RSASSA
    #if TSIP_SHA_1 == 1
static uint8_t s_rsa_sig_sha1_prefix[] =
{
    0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14
};
    #endif
    #if TSIP_SHA_256 == 1
static uint8_t s_rsa_sig_sha256_prefix[] =
{
    0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20
};
    #endif
    #if TSIP_MD5 == 1
static uint8_t s_rsa_sig_md5_prefix[] =
{
    0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05, 0x05, 0x00, 0x04, 0x10
};
    #endif
#endif /* TSIP_PRV_USE_RSASSA */

static e_tsip_err_t calc_hash_data(uint8_t *p_mes, uint8_t *p_hash, uint32_t mes_len, uint8_t hash_type);
static e_tsip_err_t set_rsassapkcs_hash_data(tsip_rsa_byte_data_t *p_message_hash, uint8_t hash_type,
        uint32_t rsa_key_byte_size, uint8_t *data_buff);
static e_tsip_err_t rsassa_emsa_pss_verify (tsip_rsa_byte_data_t * message, uint8_t hash_type, uint32_t em_bits,
        uint8_t * em);
static uint32_t get_keyn_size(uint32_t *prsa_key_index, uint32_t key_max_size);
static e_tsip_err_t get_rand_rsaes_pkcs(uint32_t rand_size, uint8_t *prand_data);

#if TSIP_PRV_USE_RSASSA
/***********************************************************************************************************************
* Function Name: calc_hash_data
*******************************************************************************************************************/ /**
* @details       Hash message text according to the hash type.
* @param[in]     p_mes message text
* @param[out]    p_hash pointer of hash data
* @param[in]     mes_len message size(byte)
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_Sha1Init()
* @see           R_TSIP_Sha1Update()
* @see           R_TSIP_Sha1Final()
* @see           R_TSIP_Sha256Init()
* @see           R_TSIP_Sha256Update()
* @see           R_TSIP_Sha256Final()
* @see           R_TSIP_Md5Init()
* @see           R_TSIP_Md5Update()
* @see           R_TSIP_Md5Final()
* @note          None
*/
static e_tsip_err_t calc_hash_data(uint8_t *p_mes, uint8_t *p_hash, uint32_t mes_len, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    tsip_sha_md5_handle_t ex_sha_handle =
    {
        0
    };
    uint32_t hash_length = 0;

    if (R_TSIP_RSA_HASH_SHA1 == hash_type)
    {
        R_TSIP_Sha1Init(&ex_sha_handle);
        error_code = R_TSIP_Sha1Update(&ex_sha_handle, p_mes, mes_len);
        if (TSIP_SUCCESS != error_code)
        {
            return error_code;
        }
        error_code = R_TSIP_Sha1Final(&ex_sha_handle, p_hash, &hash_length);
    }
    else if (R_TSIP_RSA_HASH_SHA256 == hash_type)
    {
        R_TSIP_Sha256Init(&ex_sha_handle);
        error_code = R_TSIP_Sha256Update(&ex_sha_handle, p_mes, mes_len);
        if (TSIP_SUCCESS != error_code)
        {
            return error_code;
        }
        error_code = R_TSIP_Sha256Final(&ex_sha_handle, p_hash, &hash_length);
    }
    else    /* if (R_TSIP_RSA_HASH_MD5 == hash_type) */
    {
        R_TSIP_Md5Init(&ex_sha_handle);
        error_code = R_TSIP_Md5Update(&ex_sha_handle, p_mes, mes_len);
        if (TSIP_SUCCESS != error_code)
        {
            return error_code;
        }
        error_code = R_TSIP_Md5Final(&ex_sha_handle, p_hash, &hash_length);
    }

    return error_code;
}
/*******************************
 End of function calc_hash_data
 *******************************/

/***********************************************************************************************************************
* Function Name: set_rsassapkcs_hash_data
*******************************************************************************************************************/ /**
* @details       Prefix the hash of the message according to RSASSA-PKCS1-V1_5.
* @param[in]     p_mes message text or hash value
* @param[in]     hash_type hash type
* @param[in]     rsa_key_byte_size RSA data size
* @param[out]    data_buff pointer of Sig data
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           calc_hash_data()
* @note          None
*/
static e_tsip_err_t set_rsassapkcs_hash_data(tsip_rsa_byte_data_t *p_message_hash, uint8_t hash_type,
        uint32_t rsa_key_byte_size, uint8_t *data_buff)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    tsip_rsa_byte_data_t prefix =
    {
        0
    };
    uint32_t hash_data_buff[R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE / sizeof(uint32_t)] =
    {
        0
    };
    uint32_t hash_length = 0;
    uint32_t data_ptr = 0;

    if ((R_TSIP_RSA_HASH_SHA256 < hash_type) || (1 < p_message_hash->data_type))
    {
        return TSIP_ERR_PARAMETER;
    }
    if (0 == p_message_hash->data_type)
    {
        /* Casting uint8_t pointer is used for address. */
        error_code = calc_hash_data(p_message_hash->pdata, (uint8_t*)hash_data_buff, p_message_hash->data_length,
                hash_type);
        if (TSIP_SUCCESS != error_code)
        {
            return error_code;
        }
    }

    if (R_TSIP_RSA_HASH_SHA1 == hash_type)
    {
        prefix.pdata = s_rsa_sig_sha1_prefix;
        prefix.data_length = sizeof(s_rsa_sig_sha1_prefix);
        hash_length = R_TSIP_SHA1_HASH_LENGTH_BYTE_SIZE;
    }
    else if (R_TSIP_RSA_HASH_SHA256 == hash_type)
    {
        prefix.pdata = s_rsa_sig_sha256_prefix;
        prefix.data_length = sizeof(s_rsa_sig_sha256_prefix);
        hash_length = R_TSIP_SHA256_HASH_LENGTH_BYTE_SIZE;
    }
    else    /* if (R_TSIP_RSA_HASH_MD5 == hash_type) */
    {
        prefix.pdata = s_rsa_sig_md5_prefix;
        prefix.data_length = sizeof(s_rsa_sig_md5_prefix);
        hash_length = R_TSIP_MD5_HASH_LENGTH_BYTE_SIZE;
    }

    data_buff[0] = 0x00;
    data_buff[1] = 0x01;
    data_ptr = ((rsa_key_byte_size - prefix.data_length) - hash_length) - 1;
    memset(data_buff + 2, 0xFF, data_ptr);
    data_buff[data_ptr] = 0x0;
    data_ptr++;
    memcpy(data_buff + data_ptr, prefix.pdata, prefix.data_length);
    data_ptr += prefix.data_length;
    if (0 == p_message_hash->data_type)
    {
        memcpy(data_buff + data_ptr, hash_data_buff, hash_length);
    }
    else    /* if (1 == p_message_hash->data_type) */
    {
        memcpy(data_buff + data_ptr, p_message_hash->pdata, hash_length);
    }

    return TSIP_SUCCESS;
}
/*******************************
 End of function set_rsassapkcs_hash_data
 *******************************/

/***********************************************************************************************************************
* Function Name: rsassa_emsa_pss_encode
*******************************************************************************************************************/ /**
* @details       EMSA-PSS-ENCODE generate encoded message
* @param[in]     message message or hash value information
* @param[in]     hash_type hash type
* @param[in]     em_bits maximal bit length of em
* @param[out]    em encoded message
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           calc_hash_data()
* @see           R_TSIP_GenerateRandomNumberPrivate()
* @note          None
*/
e_tsip_err_t rsassa_emsa_pss_encode(tsip_rsa_byte_data_t *message, uint8_t hash_type, uint32_t em_bits, uint8_t *em)
{
    e_tsip_err_t error_code;
    uint32_t     em_len = (em_bits +7) / 8;
    uint32_t     hash_length;
    uint8_t      m_dash[72];
    uint8_t      em_h[32];
    uint32_t     salt_length = 32;
    uint8_t      mgf_input[36];
    uint8_t      mgf_mask[240];
    uint32_t     i;

    if (R_TSIP_RSA_HASH_SHA1 == hash_type)
    {
        hash_length = 20;
    }
    else if (R_TSIP_RSA_HASH_SHA256 == hash_type)
    {
        hash_length = 32;
    }
    else
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0 == message->data_type)
    {
        error_code = calc_hash_data(message->pdata, &m_dash[8], message->data_length, hash_type);
    }
    else if (1 == message->data_type)
    {
        memcpy(&m_dash[8], message->pdata, hash_length);
        error_code = TSIP_SUCCESS;
    }
    else
    {
        return TSIP_ERR_PARAMETER;
    }

    if (TSIP_SUCCESS == error_code)
    {
        memset(m_dash, 0, 8);

        /* salt_length is 32 byte */
        /* Casting uint32_t pointer is used for address. */
        R_TSIP_GenerateRandomNumberPrivate((uint32_t *)&m_dash[hash_length + 8]);

        /* Casting uint32_t pointer is used for address. */
        R_TSIP_GenerateRandomNumberPrivate((uint32_t *)&m_dash[hash_length + 8 + 16]);

        error_code = calc_hash_data(m_dash, em_h, hash_length + salt_length + 8, hash_type);
    }

    if (TSIP_SUCCESS == error_code)
    {
        memset(em, 0, em_len - (salt_length + hash_length + 2));
        em[em_len - (salt_length + hash_length + 2)] = 0x01;
        memcpy(&em[em_len - (salt_length + hash_length + 1)], &m_dash[hash_length + 8], salt_length);
        memcpy(&em[em_len - (hash_length + 1)], em_h, hash_length);

        memcpy(mgf_input, em_h, hash_length);
        memset(&mgf_input[hash_length], 0, 4);
        for (i = 0; i <= (((em_len - hash_length) - 1) / hash_length); i++)
        {
            if (TSIP_SUCCESS == error_code)
            {
                error_code = calc_hash_data(mgf_input, &mgf_mask[i *hash_length], hash_length + 4, hash_type);
                mgf_input[hash_length + 3]++;
            }
        }
    }

    if (TSIP_SUCCESS == error_code)
    {
        for (i = 0; i < (em_len - (hash_length + 1)); i++)
        {
            em[i] ^= mgf_mask[i];
        }
        em[0]          &= (0xff >> ((em_len *8) - em_bits));
        em[em_len - 1] = 0xbc;
    }

    return error_code;
}
/*******************************
 End of function rsassa_emsa_pss_encode
 *******************************/

/***********************************************************************************************************************
* Function Name: rsassa_emsa_pss_verify
*******************************************************************************************************************/ /**
* @details       EMSA-PSS-VERIFY signature verification
* @param[in]     message message or hash value information
* @param[in]     hash_type hash type
* @param[in]     em_bits maximal bit length of em
* @param[in]     em encoded message
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           calc_hash_data()
* @note          None
*/
static e_tsip_err_t rsassa_emsa_pss_verify(tsip_rsa_byte_data_t *message, uint8_t hash_type, uint32_t em_bits,
        uint8_t *em)
{
    e_tsip_err_t error_code;
    uint32_t     em_len = (em_bits +7) / 8;
    uint32_t     hash_length;
    uint8_t      m_dash[72];
    uint8_t      db[256];
    uint32_t     salt_length = 32;
    uint8_t      mgf_input[36];
    uint8_t      mgf_mask[240];
    uint8_t      h_dash[32];
    uint32_t     i;
    int32_t      result;

    if (R_TSIP_RSA_HASH_SHA1 == hash_type)
    {
        hash_length = 20;
    }
    else if (R_TSIP_RSA_HASH_SHA256 == hash_type)
    {
        hash_length = 32;
    }
    else
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0 == message->data_type)
    {
        error_code = calc_hash_data(message->pdata, &m_dash[8], message->data_length, hash_type);
    }
    else if (1 == message->data_type)
    {
        memcpy(&m_dash[8], message->pdata, hash_length);
        error_code = TSIP_SUCCESS;
    }
    else
    {
        return TSIP_ERR_PARAMETER;
    }

    if (0xbc != em[em_len - 1])
    {
        error_code = TSIP_ERR_AUTHENTICATION;
    }
    if (0x00 != (em[0] >> ((8 + em_bits) - (em_len *8))))
    {
        error_code = TSIP_ERR_AUTHENTICATION;
    }

    if (TSIP_SUCCESS == error_code)
    {
        memcpy(db, em, em_len);

        memcpy(mgf_input, &em[em_len - (hash_length + 1)], hash_length);
        memset(&mgf_input[hash_length], 0, 4);
        for (i = 0; i <= ((em_len - (hash_length + 1)) / hash_length); i++)
        {
            if (TSIP_SUCCESS == error_code)
            {
                error_code = calc_hash_data(mgf_input, &mgf_mask[i *hash_length], hash_length + 4, hash_type);
                mgf_input[hash_length + 3]++;
            }
        }
    }

    if (TSIP_SUCCESS == error_code)
    {
        for (i = 0; i < (em_len - (hash_length + 1)); i++)
        {
            db[i] ^= mgf_mask[i];
        }
        db[0] &= (0xff >> (8 - ((em_len *8) - em_bits)));

        for (i = 0; i < (em_len - (hash_length + salt_length + 2)); i++)
        {
            if (0x00 != db[i])
            {
                error_code = TSIP_ERR_AUTHENTICATION;
            }
        }
        if (0x01 != db[em_len - (hash_length + salt_length + 2)])
        {
            error_code = TSIP_ERR_AUTHENTICATION;
        }
    }

    if (TSIP_SUCCESS == error_code)
    {
        memset(m_dash, 0, 8);
        memcpy(&m_dash[8 + hash_length], &db[em_len - (hash_length + salt_length + 1)], salt_length);

        error_code = calc_hash_data(m_dash, h_dash, 8 + hash_length + salt_length, hash_type);
    }

    if (TSIP_SUCCESS == error_code)
    {
        result = memcmp(&em[em_len - (hash_length + 1)], h_dash, hash_length);
        if (0 != result)
        {
            error_code = TSIP_ERR_AUTHENTICATION;
        }
    }

    return error_code;
}
/*******************************
 End of function rsassa_emsa_pss_verify
 *******************************/
#endif /* TSIP_PRV_USE_RSASSA */

#if TSIP_PRV_USE_RSAES
/***********************************************************************************************************************
* Function Name: get_keyn_size
*******************************************************************************************************************/ /**
* @details       Get key n size from RSA key index
* @param[in]     prsa_key_index RSA key index
* @param[in]     key_max_size key data max byte size
* @return        key_size
* @note          None
*/
static uint32_t get_keyn_size(uint32_t *prsa_key_index, uint32_t key_max_size)
{
    bool get_output_data_length = false;
    /* Casting uint8_t pointer is used for address. */
    uint8_t *p_rsa_key_n_data = (uint8_t*)prsa_key_index;
    uint32_t key_size_tmp = key_max_size;
    uint32_t i = 0;

    for (i = 0; i < key_max_size; i++)
    {
        if (!get_output_data_length)
        {
            if (0 == p_rsa_key_n_data[i])
            {
                key_size_tmp--;
            }
            else
            {
                get_output_data_length = true;
            }
        }
    }

    return key_size_tmp;
}
/*******************************
 End of function get_keyn_size
 *******************************/

/***********************************************************************************************************************
* Function Name: get_rand_rsaes_pkcs
*******************************************************************************************************************/ /**
* @details       In accordance with RSAES-PKCS1-V1_5 to obtain random number data of rand_size that does not contain 0.
* @param[in]     rand_size random data size
* @param[out]    prand_data random data area pointer
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @see           R_TSIP_GenerateRandomNumberPrivate()
* @note          None
*/
static e_tsip_err_t get_rand_rsaes_pkcs(uint32_t rand_size, uint8_t *prand_data)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t rand_tmp[16] =
    {
        0
    };
    uint32_t rest_size = rand_size;
    uint32_t rand_ptr = 0;
    uint32_t i = 0;

    /* WAIT_LOOP */
    while (rest_size)
    {
        /* Casting uint32_t pointer is used for address. */
        ercd = R_TSIP_GenerateRandomNumberPrivate((uint32_t*)rand_tmp);
        if (TSIP_SUCCESS != ercd)
        {
            break;
        }
        for (i = 0; i < 16; i++)
        {
            if (0 != rand_tmp[i])
            {
                prand_data[rand_ptr] = rand_tmp[i];
                rand_ptr++;
                rest_size--;
            }
            if (0 == rest_size)
            {
                break;
            }
        }
    }

    return ercd;
}
/*******************************
 End of function get_rand_rsaes_pkcs
 *******************************/
#endif /* TSIP_PRV_USE_RSAES */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa1024PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA1024 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa1024PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa1024_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa1024PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA1024_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa1024PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa2048PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa2048PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa2048PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa2048_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa2048PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA2048_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa2048PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_2048 */

#if TSIP_PRV_USE_RSA_3072
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa3072PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA3072 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa3072PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa3072PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa3072_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa3072PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA3072_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa3072PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_3072 */

#if TSIP_PRV_USE_RSA_4096
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa4096PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Public key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA4096 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa4096PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa4096PublicKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa4096_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa4096PublicKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA4096_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa4096PublicKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_4096 */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa1024PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Private key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA1024 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa1024PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa1024_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa1024PrivateKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA1024_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa1024PrivateKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa2048PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       Generate RSA Private key index data
* @param[in]     encrypted_provisioning_key Input the provisioning key includes encrypted CBC/CBC-MAC key for user key
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa2048PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa2048PrivateKeyIndex(uint8_t *encrypted_provisioning_key, uint8_t *iv,
        uint8_t *encrypted_key, tsip_rsa2048_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint32_t install_key_ring_index = TSIP_INSTALL_KEY_RING_INDEX;
    error_code = R_TSIP_GenerateRsa2048PrivateKeyIndexSub(&install_key_ring_index,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)encrypted_provisioning_key, (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*******************************
 End of function R_TSIP_GenerateRsa2048PrivateKeyIndex
 *******************************/
#endif /* TSIP_PRV_USE_RSA_2048 || TSIP_TLS */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateRsa1024PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating RSA1024 public user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA1024 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateRsa1024PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateRsa1024PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa1024_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_UpdateRsa1024PublicKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA1024_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateRsa1024PublicKeyIndex
 *************************************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateRsa2048PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating RSA2048 public user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateRsa2048PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateRsa2048PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa2048_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_UpdateRsa2048PublicKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA2048_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateRsa2048PublicKeyIndex
 *************************************************/
#endif /* TSIP_PRV_USE_RSA_2048 */

#if TSIP_PRV_USE_RSA_3072
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateRsa3072PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating RSA3072 public user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA3072 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateRsa3072PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateRsa3072PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa3072_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_UpdateRsa3072PublicKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA3072_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateRsa3072PublicKeyIndex
 *************************************************/
#endif /* TSIP_PRV_USE_RSA_3072 */

#if TSIP_PRV_USE_RSA_4096
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateRsa4096PublicKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating RSA4096 public user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA4096 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateRsa4096PublicKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateRsa4096PublicKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa4096_public_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_UpdateRsa4096PublicKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA4096_PUBLIC;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/*************************************************
 End of function R_TSIP_UpdateRsa4096PublicKeyIndex
 *************************************************/
#endif /* TSIP_PRV_USE_RSA_4096 */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateRsa1024PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating RSA1024 private user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA1024 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateRsa1024PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateRsa1024PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa1024_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_UpdateRsa1024PrivateKeyIndexSub(
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA1024_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/**************************************************
 End of function R_TSIP_UpdateRsa1024PrivateKeyIndex
 **************************************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_UpdateRsa2048PrivateKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for updating RSA2048 private user key index
* @param[in]     iv Input the IV for user key CBC encrypt
* @param[in]     encrypted_key Input the user key encrypted with AES128-ECB mode
* @param[out]    key_index Output the user Key Generation Information of RSA2048 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_UpdateRsa2048PrivateKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateRsa2048PrivateKeyIndex(uint8_t *iv, uint8_t *encrypted_key,
        tsip_rsa2048_private_key_index_t *key_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code = R_TSIP_UpdateRsa2048PrivateKeyIndexSub(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)iv, (uint32_t*)encrypted_key, (uint32_t*)&key_index->value);
    if (TSIP_SUCCESS == error_code)
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE;
    }
    else
    {
        key_index->type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/**************************************************
 End of function R_TSIP_UpdateRsa2048PrivateKeyIndex
 **************************************************/
#endif /* TSIP_PRV_USE_RSA_2048 || TSIP_TLS */

#if TSIP_PRV_USE_RSA_1024
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa1024RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting random User Key Generation Information of RSA1024 bit.
* @param[out]    key_pair_index Output the pair of user private key and user public key generation information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa1024RandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024RandomKeyIndex(tsip_rsa1024_key_pair_index_t *key_pair_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code =  R_TSIP_GenerateRsa1024RandomKeyIndexSub(TSIP_RSA_RETRY_COUNT_FOR_RSA_KEY_GENERATION,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)&key_pair_index->public.value, (uint32_t*)&key_pair_index->private.value);
    if (TSIP_SUCCESS == error_code)
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_RSA1024_PUBLIC;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_RSA1024_PRIVATE;
    }
    else
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_INVALID;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateRsa1024RandomKeyIndex
 ***************************************/
#endif /* TSIP_PRV_USE_RSA_1024 */

#if TSIP_PRV_USE_RSA_2048 || TSIP_TLS
/***********************************************************************************************************************
* Function Name: R_TSIP_GenerateRsa2048RandomKeyIndex
*******************************************************************************************************************/ /**
* @details       The API for outputting random User Key Generation Information of RSA2048 bit.
* @param[out]    key_pair_index Output the pair of user private key and user public key generation information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           R_TSIP_GenerateRsa2048RandomKeyIndexSub()
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa2048RandomKeyIndex(tsip_rsa2048_key_pair_index_t *key_pair_index)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    error_code =  R_TSIP_GenerateRsa2048RandomKeyIndexSub(TSIP_RSA_RETRY_COUNT_FOR_RSA_KEY_GENERATION,
        /* Casting uint32_t pointer is used for address. */
        (uint32_t*)&key_pair_index->public.value, (uint32_t*)&key_pair_index->private.value);
    if (TSIP_SUCCESS == error_code)
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_RSA2048_PUBLIC;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE;
    }
    else
    {
        key_pair_index->public.type = TSIP_KEY_INDEX_TYPE_INVALID;
        key_pair_index->private.type = TSIP_KEY_INDEX_TYPE_INVALID;
    }
    return error_code;
}
/***************************************
 End of function R_TSIP_GenerateRsa2048RandomKeyIndex
 ***************************************/
#endif /* TSIP_PRV_USE_RSA_2048 || TSIP_TLS */

#if TSIP_RSASSA_1024 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPkcs1024SignatureGenerate
*******************************************************************************************************************/ /**
* @details       RSASSA-PKCS_v1.5 generate signature data
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index RSA private key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           set_rsassapkcs_hash_data()
* @see           R_TSIP_Rsa1024ModularExponentDecryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPkcs1024SignatureGenerate(tsip_rsa_byte_data_t *message_hash,
        tsip_rsa_byte_data_t *signature, tsip_rsa1024_private_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint8_t data_buff[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };

    error_code = set_rsassapkcs_hash_data(message_hash, hash_type, sizeof(data_buff), data_buff);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = R_TSIP_Rsa1024ModularExponentDecryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = sizeof(data_buff);
    }

    return error_code;

}
/*******************************
 End of function R_TSIP_RsassaPkcs1024SignatureGenerate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPkcs1024SignatureVerification
*******************************************************************************************************************/ /**
* @details       RSASSA-PKCS_v1.5 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index RSA public key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Rsa1024ModularExponentEncryptPrivate()
* @see           set_rsassapkcs_hash_data()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPkcs1024SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa1024_public_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint8_t data_buff[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t decrypt_data[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };

    memcpy(data_buff, signature->pdata, sizeof(data_buff));
    error_code = R_TSIP_Rsa1024ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)decrypt_data);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = set_rsassapkcs_hash_data(message_hash, hash_type, sizeof(data_buff), data_buff);
    if (TSIP_SUCCESS == error_code)
    {
        if (memcmp(data_buff, decrypt_data, sizeof(data_buff)))
        {
            error_code = TSIP_ERR_AUTHENTICATION;
        }
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPkcs1024SignatureVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPss1024SignatureGenerate
*******************************************************************************************************************/ /**
* @details       RSASSA-PSS generate signature data
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index RSA private key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           rsassa_emsa_pss_encode()
* @see           R_TSIP_Rsa1024ModularExponentDecryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPss1024SignatureGenerate(tsip_rsa_byte_data_t *message, tsip_rsa_byte_data_t *signature,
        tsip_rsa1024_private_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code;
    uint8_t      em[128];

    if (TSIP_KEY_INDEX_TYPE_RSA1024_PRIVATE != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }

    /* EMSA-PSS-ENCODE */
    error_code = rsassa_emsa_pss_encode(message, hash_type, (R_TSIP_RSA_1024_KEY_N_LENGTH_BYTE_SIZE *8) - 1, em);

    /* RSA signature */
    if (TSIP_SUCCESS == error_code)
    {
        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Rsa1024ModularExponentDecryptPrivate((uint32_t *)&key_index->value, (uint32_t *)em,
            (uint32_t *)signature->pdata);
    }

    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = 0x0080;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPss1024SignatureGenerate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPss1024SignatureVerification
*******************************************************************************************************************/ /**
* @details       RSASSA-PSS signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index RSA public key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Rsa1024ModularExponentEncryptPrivate()
* @see           rsassa_emsa_pss_verify()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPss1024SignatureVerification(tsip_rsa_byte_data_t *signature, tsip_rsa_byte_data_t *message,
        tsip_rsa1024_public_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code;
    uint8_t      em[128];

    if (TSIP_KEY_INDEX_TYPE_RSA1024_PUBLIC != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }

    /* RSA verification */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Rsa1024ModularExponentEncryptPrivate((uint32_t *)&key_index->value,
        (uint32_t *)signature->pdata, (uint32_t *)em);

    /* EMSA-PSS-VERIFY */
    if (TSIP_SUCCESS == error_code)
    {
        error_code = rsassa_emsa_pss_verify(message, hash_type, (R_TSIP_RSA_1024_KEY_N_LENGTH_BYTE_SIZE *8) - 1, em);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPss1024SignatureVerification
 *******************************/
#endif /* TSIP_RSASSA_1024 == 1 */

#if TSIP_RSAES_1024 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsaesPkcs1024Encrypt
*******************************************************************************************************************/ /**
* @details       RSAES-PKCS1-v1_5 Encrypt with RSA 1024bit Public key
* @param[in]     plain plain data and plain data length
* @param[out]    cipher cipher data and cipher data length
* @param[in]     key_index key index information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           get_keyn_size()
* @see           get_rand_rsaes_pkcs()
* @see           R_TSIP_Rsa1024ModularExponentEncryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsaesPkcs1024Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa1024_public_key_index_t *key_index)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t input_data_arry[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t output_data_arry[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };
    uint32_t key_size = 0;
    uint32_t ptr = 0;

    /* Casting uint32_t pointer is used for address. */
    key_size = get_keyn_size((uint32_t*)&key_index->value.key_n, R_TSIP_RSA_1024_DATA_BYTE_SIZE);
    if (((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE > key_size) || (cipher->data_length < key_size))
    || (plain->data_length > (key_size - R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE)))
    {
        return TSIP_ERR_PARAMETER;
    }

    /***   RSAES-PKCS#1_V1.5 format   ***/
    /*       (1)   |  (2)   | (3)  |  (4)   | (5) */
    /* EM = [0x00] | [0x02] | [PS] | [0x00] | [M] */
    ptr = R_TSIP_RSA_1024_DATA_BYTE_SIZE - key_size;
    input_data_arry[ptr] = 0x00;
    ptr++;
    input_data_arry[ptr] = 0x02;
    ptr++;

    get_rand_rsaes_pkcs(((key_size - plain->data_length) - 3), &input_data_arry[ptr]);
    ptr += ((key_size - plain->data_length) - 3);
    input_data_arry[ptr] = 0x00;
    ptr++;

    memcpy(&input_data_arry[ptr], plain->pdata, plain->data_length);

    ercd = R_TSIP_Rsa1024ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)input_data_arry, (uint32_t*)output_data_arry);
    if (TSIP_SUCCESS == ercd)
    {
        memcpy(cipher->pdata, &output_data_arry[R_TSIP_RSA_1024_DATA_BYTE_SIZE - key_size], key_size);
        cipher->data_length = key_size;
    }

    return ercd;
}
/*******************************
 End of function R_TSIP_RsaesPkcs1024Encrypt
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsaesPkcs1024Decrypt
*******************************************************************************************************************/ /**
* @details       RSAES-PKCS1-v1_5 Decrypt with RSA 1024bit Private key
* @param[in]     cipher cipher data and cipher data length
* @param[out]    plain plain data and plain data length
* @param[in]     key_index key index information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_Rsa1024ModularExponentDecryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsaesPkcs1024Decrypt(tsip_rsa_byte_data_t *cipher, tsip_rsa_byte_data_t *plain,
        tsip_rsa1024_private_key_index_t *key_index)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t input_data_arry[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t output_data_arry[R_TSIP_RSA_1024_DATA_BYTE_SIZE] =
    {
        0
    };
    uint32_t key_size = 0;
    uint32_t ptr = 0;
    uint32_t ptr_start = 0;
    uint8_t* p_start = 0;
    uint8_t* p_zero_ptr = 0;

    key_size = cipher->data_length;
    if (((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE > key_size) || ((sizeof(input_data_arry)) < key_size))
    || (plain->data_length < (key_size - R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE)))
    {
        return TSIP_ERR_PARAMETER;
    }

    memcpy(&input_data_arry[R_TSIP_RSA_1024_DATA_BYTE_SIZE - key_size], cipher->pdata, cipher->data_length);
    ercd = R_TSIP_Rsa1024ModularExponentDecryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)input_data_arry, (uint32_t*)output_data_arry);
    if (TSIP_SUCCESS != ercd)
    {
        return ercd;
    }

    /***   RSAES-PKCS#1_V1.5 format   ***/
    /*       (1)   |  (2)   | (3)  |  (4)   | (5) */
    /* EM = [0x00] | [0x02] | [PS] | [0x00] | [M] */
    ptr = R_TSIP_RSA_1024_DATA_BYTE_SIZE - key_size;
    ptr_start = ptr;
    /* (1) */
    if (0x00 != output_data_arry[ptr])
    {
        return TSIP_ERR_FAIL;
    }
    ptr++;
    /* (2) */
    if (0x02 != output_data_arry[ptr])
    {
        return TSIP_ERR_FAIL;
    }
    ptr++;
    /* (3) and (4) */
    p_start = &output_data_arry[ptr];
    /* Casting uint8_t pointer is used for address. */
    p_zero_ptr = (uint8_t*)memchr(p_start, 0x00, key_size - 2); /* "2" is (1)+(2) */
    if ((NULL == p_zero_ptr) || ((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE - 3) > (p_zero_ptr - p_start)))
    {
        return TSIP_ERR_FAIL;
    }
    ptr += ((p_zero_ptr - p_start) + 1);

    plain->data_length = key_size - (ptr - ptr_start);
    memcpy(plain->pdata, &output_data_arry[ptr], plain->data_length);

    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_RsaesPkcs1024Decrypt
 *******************************/
#endif /* TSIP_RSAES_1024 == 1 */

#if TSIP_RSASSA_2048 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPkcs2048SignatureGenerate
*******************************************************************************************************************/ /**
* @details       RSASSA-PKCS_v1.5 generate signature data
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index RSA private key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @see           set_rsassapkcs_hash_data()
* @see           R_TSIP_Rsa2048ModularExponentDecryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPkcs2048SignatureGenerate(tsip_rsa_byte_data_t *message_hash,
        tsip_rsa_byte_data_t *signature, tsip_rsa2048_private_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint8_t data_buff[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };

    error_code = set_rsassapkcs_hash_data(message_hash, hash_type, sizeof(data_buff), data_buff);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = R_TSIP_Rsa2048ModularExponentDecryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)signature->pdata);
    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = sizeof(data_buff);
    }

    return error_code;

}
/*******************************
 End of function R_TSIP_RsassaPkcs2048SignatureGenerate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPkcs2048SignatureVerification
*******************************************************************************************************************/ /**
* @details       RSASSA-PKCS_v1.5 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index RSA public key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Rsa2048ModularExponentEncryptPrivate()
* @see           set_rsassapkcs_hash_data()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPkcs2048SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa2048_public_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint8_t data_buff[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t decrypt_data[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };

    memcpy(data_buff, signature->pdata, sizeof(data_buff));
    error_code = R_TSIP_Rsa2048ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)decrypt_data);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = set_rsassapkcs_hash_data(message_hash, hash_type, sizeof(data_buff), data_buff);
    if (TSIP_SUCCESS == error_code)
    {
        if (memcmp(data_buff, decrypt_data, sizeof(data_buff)))
        {
            error_code = TSIP_ERR_AUTHENTICATION;
        }
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPkcs2048SignatureVerification
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPss2048SignatureGenerate
*******************************************************************************************************************/ /**
* @details       RSASSA-PSS generate signature data
* @param[in]     message_hash message or hash value information
* @param[out]    signature signature information
* @param[in]     key_index RSA private key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           rsassa_emsa_pss_encode()
* @see           R_TSIP_Rsa2048ModularExponentDecryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPss2048SignatureGenerate(tsip_rsa_byte_data_t *message, tsip_rsa_byte_data_t *signature,
        tsip_rsa2048_private_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code;
    uint8_t      em[256];

    if (TSIP_KEY_INDEX_TYPE_RSA2048_PRIVATE != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }

    /* EMSA-PSS-ENCODE */
    error_code = rsassa_emsa_pss_encode(message, hash_type, (R_TSIP_RSA_2048_KEY_N_LENGTH_BYTE_SIZE *8) - 1, em);

    /* RSA signature */
    if (TSIP_SUCCESS == error_code)
    {
        /* Casting uint32_t pointer is used for address. */
        error_code = R_TSIP_Rsa2048ModularExponentDecryptPrivate((uint32_t *)&key_index->value, (uint32_t *)em,
            (uint32_t *)signature->pdata);
    }

    if (TSIP_SUCCESS == error_code)
    {
        signature->data_length = 0x0100;
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPss2048SignatureGenerate
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPss2048SignatureVerification
*******************************************************************************************************************/ /**
* @details       RSASSA-PSS signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index RSA public key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Rsa2048ModularExponentEncryptPrivate()
* @see           rsassa_emsa_pss_verify()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPss2048SignatureVerification(tsip_rsa_byte_data_t *signature, tsip_rsa_byte_data_t *message,
        tsip_rsa2048_public_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code;
    uint8_t      em[256];

    if (TSIP_KEY_INDEX_TYPE_RSA2048_PUBLIC != key_index->type)
    {
        return TSIP_ERR_KEY_SET;
    }

    /* RSA verification */
    /* Casting uint32_t pointer is used for address. */
    error_code = R_TSIP_Rsa2048ModularExponentEncryptPrivate((uint32_t *)&key_index->value,
        (uint32_t *)signature->pdata, (uint32_t *)em);

    /* EMSA-PSS-VERIFY */
    if (TSIP_SUCCESS == error_code)
    {
        error_code = rsassa_emsa_pss_verify(message, hash_type, (R_TSIP_RSA_2048_KEY_N_LENGTH_BYTE_SIZE *8) - 1, em);
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPss2048SignatureVerification
 *******************************/
#endif /* TSIP_RSASSA_2048 == 1 */

#if TSIP_RSAES_2048 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsaesPkcs2048Encrypt
*******************************************************************************************************************/ /**
* @details       RSAES-PKCS1-v1_5 Encrypt with RSA 2048bit Public key
* @param[in]     plain plain data and plain data length
* @param[out]    cipher cipher data and cipher data length
* @param[in]     key_index key index information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           get_keyn_size()
* @see           get_rand_rsaes_pkcs()
* @see           R_TSIP_Rsa2048ModularExponentEncryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsaesPkcs2048Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa2048_public_key_index_t *key_index)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t input_data_arry[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t output_data_arry[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };
    uint32_t key_size = 0;
    uint32_t ptr = 0;

    /* Casting uint32_t pointer is used for address. */
    key_size = get_keyn_size((uint32_t*)&key_index->value.key_n, R_TSIP_RSA_2048_DATA_BYTE_SIZE);
    if (((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE > key_size) || (cipher->data_length < key_size))
    || (plain->data_length > (key_size - R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE)))
    {
        return TSIP_ERR_PARAMETER;
    }

    /***   RSAES-PKCS#1_V1.5 format   ***/
    /*       (1)   |  (2)   | (3)  |  (4)   | (5) */
    /* EM = [0x00] | [0x02] | [PS] | [0x00] | [M] */
    ptr = R_TSIP_RSA_2048_DATA_BYTE_SIZE - key_size;
    input_data_arry[ptr] = 0x00;
    ptr++;
    input_data_arry[ptr] = 0x02;
    ptr++;

    get_rand_rsaes_pkcs(((key_size - plain->data_length) - 3), &input_data_arry[ptr]);
    ptr += ((key_size - plain->data_length) - 3);
    input_data_arry[ptr] = 0x00;
    ptr++;

    memcpy(&input_data_arry[ptr], plain->pdata, plain->data_length);

    ercd = R_TSIP_Rsa2048ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)input_data_arry, (uint32_t*)output_data_arry);
    if (TSIP_SUCCESS == ercd)
    {
        memcpy(cipher->pdata, &output_data_arry[R_TSIP_RSA_2048_DATA_BYTE_SIZE - key_size], key_size);
        cipher->data_length = key_size;
    }

    return ercd;
}
/*******************************
 End of function R_TSIP_RsaesPkcs2048Encrypt
 *******************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_RsaesPkcs2048Decrypt
*******************************************************************************************************************/ /**
* @details       RSAES-PKCS1-v1_5 Decrypt with RSA 2048bit Private key
* @param[in]     cipher cipher data and cipher data length
* @param[out]    plain plain data and plain data length
* @param[in]     key_index key index information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           R_TSIP_Rsa2048ModularExponentDecryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsaesPkcs2048Decrypt(tsip_rsa_byte_data_t *cipher, tsip_rsa_byte_data_t *plain,
        tsip_rsa2048_private_key_index_t *key_index)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t input_data_arry[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t output_data_arry[R_TSIP_RSA_2048_DATA_BYTE_SIZE] =
    {
        0
    };
    uint32_t key_size = 0;
    uint32_t ptr = 0;
    uint32_t ptr_start = 0;
    uint8_t* p_start = 0;
    uint8_t* p_zero_ptr = 0;

    key_size = cipher->data_length;
    if (((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE > key_size) || ((sizeof(input_data_arry)) < key_size))
    || (plain->data_length < (key_size - R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE)))
    {
        return TSIP_ERR_PARAMETER;
    }

    memcpy(&input_data_arry[R_TSIP_RSA_2048_DATA_BYTE_SIZE - key_size], cipher->pdata, cipher->data_length);
    ercd = R_TSIP_Rsa2048ModularExponentDecryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)input_data_arry, (uint32_t*)output_data_arry);
    if (TSIP_SUCCESS != ercd)
    {
        return ercd;
    }

    /***   RSAES-PKCS#1_V1.5 format   ***/
    /*       (1)   |  (2)   | (3)  |  (4)   | (5) */
    /* EM = [0x00] | [0x02] | [PS] | [0x00] | [M] */
    ptr = R_TSIP_RSA_2048_DATA_BYTE_SIZE - key_size;
    ptr_start = ptr;
    /* (1) */
    if (0x00 != output_data_arry[ptr])
    {
        return TSIP_ERR_FAIL;
    }
    ptr++;
    /* (2) */
    if (0x02 != output_data_arry[ptr])
    {
        return TSIP_ERR_FAIL;
    }
    ptr++;
    /* (3) and (4) */
    p_start = &output_data_arry[ptr];
    /* Casting uint8_t pointer is used for address. */
    p_zero_ptr = (uint8_t*)memchr(p_start, 0x00, key_size - 2); /* "2" is (1)+(2) */
    if ((NULL == p_zero_ptr) || ((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE - 3) > (p_zero_ptr - p_start)))
    {
        return TSIP_ERR_FAIL;
    }
    ptr += ((p_zero_ptr - p_start) + 1);

    plain->data_length = key_size - (ptr - ptr_start);
    memcpy(plain->pdata, &output_data_arry[ptr], plain->data_length);

    return TSIP_SUCCESS;
}
/*******************************
 End of function R_TSIP_RsaesPkcs2048Decrypt
 *******************************/
#endif /* TSIP_RSAES_2048 == 1 */

#if TSIP_RSASSA_3072 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPkcs3072SignatureVerification
*******************************************************************************************************************/ /**
* @details       RSASSA-PKCS_v1.5 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index RSA public key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Rsa3072ModularExponentEncryptPrivate()
* @see           set_rsassapkcs_hash_data()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPkcs3072SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa3072_public_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint8_t data_buff[R_TSIP_RSA_3072_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t decrypt_data[R_TSIP_RSA_3072_DATA_BYTE_SIZE] =
    {
        0
    };

    memcpy(data_buff, signature->pdata, sizeof(data_buff));
    error_code = R_TSIP_Rsa3072ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)decrypt_data);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = set_rsassapkcs_hash_data(message_hash, hash_type, sizeof(data_buff), data_buff);
    if (TSIP_SUCCESS == error_code)
    {
        if (memcmp(data_buff, decrypt_data, sizeof(data_buff)))
        {
            error_code = TSIP_ERR_AUTHENTICATION;
        }
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPkcs3072SignatureVerification
 *******************************/
#endif /* TSIP_RSASSA_3072 == 1 */

#if TSIP_RSASSA_4096 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsassaPkcs4096SignatureVerification
*******************************************************************************************************************/ /**
* @details       RSASSA-PKCS_v1.5 signature verification
* @param[in]     signature signature information
* @param[in]     message_hash message or hash value information
* @param[in]     key_index RSA public key index
* @param[in]     hash_type hash type
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @retval        TSIP_ERR_AUTHENTICATION: Authentication NG
* @see           R_TSIP_Rsa4096ModularExponentEncryptPrivate()
* @see           set_rsassapkcs_hash_data()
* @note          None
*/
e_tsip_err_t R_TSIP_RsassaPkcs4096SignatureVerification(tsip_rsa_byte_data_t *signature,
        tsip_rsa_byte_data_t *message_hash, tsip_rsa4096_public_key_index_t *key_index, uint8_t hash_type)
{
    e_tsip_err_t error_code = TSIP_SUCCESS;
    uint8_t data_buff[R_TSIP_RSA_4096_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t decrypt_data[R_TSIP_RSA_4096_DATA_BYTE_SIZE] =
    {
        0
    };

    memcpy(data_buff, signature->pdata, sizeof(data_buff));
    error_code = R_TSIP_Rsa4096ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)data_buff, (uint32_t*)decrypt_data);
    if (TSIP_SUCCESS != error_code)
    {
        return error_code;
    }

    error_code = set_rsassapkcs_hash_data(message_hash, hash_type, sizeof(data_buff), data_buff);
    if (TSIP_SUCCESS == error_code)
    {
        if (memcmp(data_buff, decrypt_data, sizeof(data_buff)))
        {
            error_code = TSIP_ERR_AUTHENTICATION;
        }
    }

    return error_code;
}
/*******************************
 End of function R_TSIP_RsassaPkcs4096SignatureVerification
 *******************************/
#endif /* TSIP_RSASSA_4096 == 1 */

#if TSIP_RSAES_3072 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsaesPkcs3072Encrypt
*******************************************************************************************************************/ /**
* @details       RSAES-PKCS1-v1_5 Encrypt with RSA 3072bit Public key
* @param[in]     plain plain data and plain data length
* @param[out]    cipher cipher data and cipher data length
* @param[in]     key_index key index information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           get_keyn_size()
* @see           get_rand_rsaes_pkcs()
* @see           R_TSIP_Rsa3072ModularExponentEncryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsaesPkcs3072Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa3072_public_key_index_t *key_index)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t input_data_arry[R_TSIP_RSA_3072_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t output_data_arry[R_TSIP_RSA_3072_DATA_BYTE_SIZE] =
    {
        0
    };
    uint32_t key_size = 0;
    uint32_t ptr = 0;

    /* Casting uint32_t pointer is used for address. */
    key_size = get_keyn_size((uint32_t*)&key_index->value.key_n, R_TSIP_RSA_3072_DATA_BYTE_SIZE);
    if (((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE > key_size) || (cipher->data_length < key_size))
    || (plain->data_length > (key_size - R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE)))
    {
        return TSIP_ERR_PARAMETER;
    }

    /***   RSAES-PKCS#1_V1.5 format   ***/
    /*       (1)   |  (2)   | (3)  |  (4)   | (5) */
    /* EM = [0x00] | [0x02] | [PS] | [0x00] | [M] */
    ptr = R_TSIP_RSA_3072_DATA_BYTE_SIZE - key_size;
    input_data_arry[ptr] = 0x00;
    ptr++;
    input_data_arry[ptr] = 0x02;
    ptr++;

    get_rand_rsaes_pkcs(((key_size - plain->data_length) - 3), &input_data_arry[ptr]);
    ptr += ((key_size - plain->data_length) - 3);
    input_data_arry[ptr] = 0x00;
    ptr++;

    memcpy(&input_data_arry[ptr], plain->pdata, plain->data_length);

    ercd = R_TSIP_Rsa3072ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)input_data_arry, (uint32_t*)output_data_arry);
    if (TSIP_SUCCESS == ercd)
    {
        memcpy(cipher->pdata, &output_data_arry[R_TSIP_RSA_3072_DATA_BYTE_SIZE - key_size], key_size);
        cipher->data_length = key_size;
    }

    return ercd;
}
/*******************************
 End of function R_TSIP_RsaesPkcs3072Encrypt
 *******************************/
#endif /* TSIP_RSAES_3072 == 1 */

#if TSIP_RSAES_4096 == 1
/***********************************************************************************************************************
* Function Name: R_TSIP_RsaesPkcs4096Encrypt
*******************************************************************************************************************/ /**
* @details       RSAES-PKCS1-v1_5 Encrypt with RSA 4096bit Public key
* @param[in]     plain plain data and plain data length
* @param[out]    cipher cipher data and cipher data length
* @param[in]     key_index key index information
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @retval        TSIP_ERR_PARAMETER: Input parameter illegal
* @see           get_keyn_size()
* @see           get_rand_rsaes_pkcs()
* @see           R_TSIP_Rsa4096ModularExponentEncryptPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_RsaesPkcs4096Encrypt(tsip_rsa_byte_data_t *plain, tsip_rsa_byte_data_t *cipher,
        tsip_rsa4096_public_key_index_t *key_index)
{
    e_tsip_err_t ercd = TSIP_SUCCESS;
    uint8_t input_data_arry[R_TSIP_RSA_4096_DATA_BYTE_SIZE] =
    {
        0
    };
    uint8_t output_data_arry[R_TSIP_RSA_4096_DATA_BYTE_SIZE] =
    {
        0
    };
    uint32_t key_size = 0;
    uint32_t ptr = 0;

    /* Casting uint32_t pointer is used for address. */
    key_size = get_keyn_size((uint32_t*)&key_index->value.key_n, R_TSIP_RSA_4096_DATA_BYTE_SIZE);
    if (((R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE > key_size) || (cipher->data_length < key_size))
    || (plain->data_length > (key_size - R_TSIP_RSA_RSAES_PKCS_MIN_KEY_N_BYTE_SIZE)))
    {
        return TSIP_ERR_PARAMETER;
    }

    /***   RSAES-PKCS#1_V1.5 format   ***/
    /*       (1)   |  (2)   | (3)  |  (4)   | (5) */
    /* EM = [0x00] | [0x02] | [PS] | [0x00] | [M] */
    ptr = R_TSIP_RSA_4096_DATA_BYTE_SIZE - key_size;
    input_data_arry[ptr] = 0x00;
    ptr++;
    input_data_arry[ptr] = 0x02;
    ptr++;

    get_rand_rsaes_pkcs(((key_size - plain->data_length) - 3), &input_data_arry[ptr]);
    ptr += ((key_size - plain->data_length) - 3);
    input_data_arry[ptr] = 0x00;
    ptr++;

    memcpy(&input_data_arry[ptr], plain->pdata, plain->data_length);

    ercd = R_TSIP_Rsa4096ModularExponentEncryptPrivate(
            /* Casting uint32_t pointer is used for address. */
            (uint32_t*)&key_index->value, (uint32_t*)input_data_arry, (uint32_t*)output_data_arry);
    if (TSIP_SUCCESS == ercd)
    {
        memcpy(cipher->pdata, &output_data_arry[R_TSIP_RSA_4096_DATA_BYTE_SIZE - key_size], key_size);
        cipher->data_length = key_size;
    }

    return ercd;
}
/*******************************
 End of function R_TSIP_RsaesPkcs4096Encrypt
 *******************************/
#endif /* TSIP_RSAES_4096 == 1 */

/***********************************************************************************************************************
* Function Name: R_TSIP_Rsa2048DhKeyAgreement
*******************************************************************************************************************/ /**
* @details       Calculate DH with RSA-2048.
* @param[in]     key_index key index information
* @param[in]     sender_private_key_index private key index information to use DH calculation
* @param[in]     message message value
* @param[in]     receiver_modulus modulus exponent from receiver + MAC 2048 bit modulus exponent || 128 bit
* @param[out]    sender_modulus modulus exponent from sender + MAC 2048 bit modulus exponent || 128 bit
* @retval        TSIP_SUCCESS: Normal termination.
* @retval        TSIP_ERR_FAIL: Internal error occurred.
* @retval        TSIP_ERR_RESOURCE_CONFLICT: resource conflict
* @retval        TSIP_ERR_KEY_SET: Input illegal user Key Generation Information
* @see           R_TSIP_Rsa2048DhKeyAgreementPrivate()
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa2048DhKeyAgreement(tsip_aes_key_index_t *key_index,
        tsip_rsa2048_private_key_index_t *sender_private_key_index, uint8_t *message, uint8_t *receiver_modulus,
        uint8_t *sender_modulus)
{
    e_tsip_err_t error_code;

    /* Casting uint8_t pointer to uint32_t pointer to adjust to TSIP key format. */
    error_code = R_TSIP_Rsa2048DhKeyAgreementPrivate(
    /* uint32_t *InData_KeyIndex,       */key_index->value,
    /* uint32_t *InData_PrimNumIndex,   */(uint32_t*)&sender_private_key_index->value,
    /* uint32_t *InData_Message,        */(uint32_t*)message,
    /* uint32_t *InData_ModExp,         */(uint32_t*)receiver_modulus,
    /* uint32_t *OutData_ModExp         */(uint32_t*)sender_modulus
    );

    return error_code;
}
/*******************************
 End of function R_TSIP_Rsa2048DhKeyAgreement
 *******************************/

